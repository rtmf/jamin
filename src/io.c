/*
 *  io.c -- JAMin I/O driver.
 *
 *  Copyright (C) 2003 Jack O'Quin.
 *  
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

/*  DSP Engine
 *
 *  The DSP engine is managed as if it were firmware running on a
 *  separate signal processing board.  It uses two realtime threads:
 *  the JACK thread and the DSP thread.  The JACK thread runs the JACK
 *  process() callback.  In some cases, signal processing is invoked
 *  directly from the JACK thread.  But, when the JACK period is too
 *  short for efficiently computing the FFT, signal processing should
 *  be done in the DSP thread, instead.
 *
 *  The DSP thread is created if the -t option was not specified and
 *  the process is capable of creating a realtime thread.  Otherwise,
 *  all signal processing will be done in the JACK thread, regardless
 *  of buffer size.
 *
 *  The JACK buffer size could change dynamically due to the
 *  jack_set_buffer_size_callback() function.  So, we do not assume
 *  that this buffer size is fixed.  Since current versions of JACK
 *  (May 2003) do not support that feature, there is no way to test
 *  that it is handled correctly.
 */

/*  Changes to this file should be tested for these conditions...
 *
 *  without -t option
 *	+ JACK running realtime (as root)
 *	   + JACK buffer size < DSP block size
 *	   + JACK buffer size >= DSP block size
 *	+ JACK running realtime (using capabilities)
 *	   + JACK buffer size < DSP block size
 *	   + JACK buffer size >= DSP block size
 *	+ JACK not running realtime
 *
 *  with -t option
 *	+ JACK running realtime
 *	   + JACK buffer size < DSP block size
 *	   + JACK buffer size >= DSP block size
 *	+ JACK not running realtime
 */

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <getopt.h>
#include <errno.h>
#include <assert.h>
#include <jack/jack.h>
#include "config.h"

#include "ringbuffer.h"
#include "process.h"
#include "plugin.h"
#include "io.h"
#include "transport.h"
#include "jackstatus.h"
#include "debug.h"

char *jamin_options = "dFhTtvV";	/* valid JAMin options */
char *pname;				/* `basename $0` */
int dummy_mode = 0;			/* -d option */
int all_errors_fatal = 0;		/* -F option */
int show_help = 0;			/* -h option */
int trace_option = 0;			/* -T option */
int thread_option = 1;			/* -t option */
int debug_level = DBG_OFF;		/* -v option */

/*  Synchronization within the DSP engine is managed as a finite state
 *  machine.  These state transitions are the key to understanding
 *  this component.
 */
#define DSP_INIT	001
#define DSP_ACTIVATING	002
#define DSP_STARTING	004
#define DSP_RUNNING	010
#define DSP_STOPPING	020
#define DSP_STOPPED	040

#define DSP_STATE_IS(x)		((dsp_state)&(x))
#define DSP_STATE_NOT(x)	((dsp_state)&(~(x)))
static volatile int dsp_state = DSP_INIT;

static int have_dsp_thread = 0;		/* DSP thread exists? */
static int use_dsp_thread = 0;		/* DSP thread currently in use? */
static jack_nframes_t dsp_block_size;	/* DSP chunk granularity */
static size_t dsp_block_bytes;		/* DSP chunk size in bytes */

#define DSP_PRIORITY_DIFF 1	/* DSP thread priority difference */
static pthread_t dsp_thread;	/* DSP thread handle */
static pthread_cond_t run_dsp = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t lock_dsp = PTHREAD_MUTEX_INITIALIZER;

#define NCHUNKS 4		/* number of DSP blocks in ringbuffer */
static ringbuffer_t *in_rb[NCHANNELS];	/* input channel ring buffers */
static ringbuffer_t *out_rb[NCHANNELS];	/* output channel ring buffers */

/* JACK connection data */
jack_status_t jst = {0};		/* current JACK status */
jack_client_t *client;			/* JACK client structure */
static int nchannels = NCHANNELS;	/* actual number of channels */
static jack_port_t *input_ports[NCHANNELS];
static jack_port_t *output_ports[NCHANNELS];
static char *in_names[NCHANNELS] = { "in_L", "in_R" };
static char *out_names[NCHANNELS] = { "out_L", "out_R" };
static char *iports[NCHANNELS] = {"alsa_pcm:capture_1", "alsa_pcm:capture_2"};
static char *oports[NCHANNELS] = {"alsa_pcm:playback_1", "alsa_pcm:playback_2"};


/****************  Low-level utility functions  ****************/


/* io_trace -- trace I/O activity.
 *
 *  This function can be called from any thread, realtime or normal.
 *  Since it never waits, we avoid the possible priority inversion of
 *  a realtime thread waiting on a non-realtime one.
 */
pthread_mutex_t io_trace_lock = PTHREAD_MUTEX_INITIALIZER;
#define TR_BUFSIZE	256		/* must be power of 2 */
#define TR_MSGSIZE	60
struct io_trace_t {
    jack_nframes_t timestamp;		/* JACK timestamp */
    char message[TR_MSGSIZE];		/* trace message */
};
size_t tr_next = 0;			/* next tr_buf entry */
struct io_trace_t tr_buf[TR_BUFSIZE] = {{0}};

void io_trace(const char *fmt, ...)
{
    va_list ap;

    /* if lock already held, skip this entry */
    if (pthread_mutex_trylock(&io_trace_lock) == 0) {

	/* get frame time from JACK, if it is active. */
	if (client)
	    tr_buf[tr_next].timestamp = jack_frame_time(client);
	else
	    tr_buf[tr_next].timestamp = 0;

	/* format trace message */
	va_start(ap, fmt);
	vsnprintf(tr_buf[tr_next].message, TR_MSGSIZE, fmt, ap);
	va_end(ap);

	tr_next = (tr_next+1) & (TR_BUFSIZE-1);

	pthread_mutex_unlock(&io_trace_lock);
    }
}

/* io_list_trace -- list trace buffer contents
 *
 *  This must be called in a context where waiting is allowed.
 */
void io_list_trace()
{
    size_t t;

    pthread_mutex_lock(&io_trace_lock);

    t = tr_next;
    do {
	if (tr_buf[t].message[0] != '\0')
	    fprintf(stderr, "%s trace [%lu]: %s\n", PACKAGE,
		    tr_buf[t].timestamp, tr_buf[t].message);
	t = (t+1) & (TR_BUFSIZE-1);
    } while (t != tr_next);

    pthread_mutex_unlock(&io_trace_lock);
}


/* io_errlog -- log I/O error.
 *
 *  This is only a stub.  The DSP engine has no business calling
 *  stdio.  Error information needs to be queued for the UI to handle
 *  running in some other thread.
 */
void io_errlog(int err, char *fmt, ...)
{
    va_list ap;
    char buffer[300];

    va_start(ap, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, ap);
    va_end(ap);

    IF_DEBUG(DBG_TERSE,
	     io_trace("error %d: %s", err, buffer));
    fprintf(stderr, "%s internal error %d: %s\n", PACKAGE, err, buffer);
    if (all_errors_fatal) {
	fprintf(stderr, " Terminating due to -F option.\n");
	abort();
    }
}


/* io_new_state -- DSP engine state transition.
 *
 *  May be called from *any* thread context.  Must not wait.
 */
void io_new_state(int next)
{
    /* These transitions don't happen all that often, and they are
     * important.  So, make sure this one is valid */
    switch (next) {
    case DSP_INIT:
	goto invalid;
    case DSP_ACTIVATING:
	if (DSP_STATE_NOT(DSP_INIT))
	    goto invalid;
	break;
    case DSP_STARTING:
	if (DSP_STATE_NOT(DSP_ACTIVATING))
	    goto invalid;
	break;
    case DSP_RUNNING:
	if (DSP_STATE_NOT(DSP_ACTIVATING|DSP_STARTING))
	    goto invalid;
	break;
    case DSP_STOPPING:
	if (DSP_STATE_NOT(DSP_ACTIVATING|DSP_RUNNING|DSP_STARTING))
	    goto invalid;
	break;
    case DSP_STOPPED:
	if (DSP_STATE_NOT(DSP_INIT|DSP_STOPPING))
	    goto invalid;
	break;
    default:
    invalid:
	io_errlog(EDEADLK, "invalid DSP state transition: 0%o -> 0%o.",
		  dsp_state, next);
	return;				/* don't do it */
    }
    dsp_state = next;			/* change to new state */
    IF_DEBUG(DBG_TERSE, io_trace("new DSP state: 0%o.", next));
}


/* io_get_status -- collect current JACK status. */
void io_get_status(jack_status_t *jp)
{
    if (client)
	jst.cpu_load = jack_cpu_load(client);
    *jp = jst;
}


/* io_set_latency -- set DSP engine latencies. */
void io_set_latency(int source, jack_nframes_t delay)
{
    static jack_nframes_t latency_delay[LAT_NSOURCES] = {0};
    static char *latency_sources[LAT_NSOURCES] = {
	"I/O Buffering",
	"Fourier Transform",
	"Limiter"};
    int chan;

    if (source < 0 || source >= LAT_NSOURCES) {
	io_errlog(ENOENT, "unknown latency source: %d.", source);
	return;
    }

    IF_DEBUG(DBG_TERSE,
	     io_trace("latency due to %s is %ld frames.",
		      latency_sources[source], delay));
    jst.latency += delay - latency_delay[source];
    latency_delay[source] = delay;

    /* Set JACK port latencies (after the ports are connected). */
    if (DSP_STATE_NOT(DSP_INIT))
	for (chan = 0; chan < nchannels; chan++)
	    jack_port_set_latency(input_ports[chan], jst.latency);
}


/* io_set_granularity -- set desired I/O block size.
 *
 *  As currently implemented, this function can only be called with
 *  the DSP engine inactive.  The DSP engine also requires that this
 *  block_size be an even multiple or divisor of the JACK buffer size.
 */
void io_set_granularity(jack_nframes_t block_size)
{
    IF_DEBUG(DBG_TERSE,
	     io_trace("%s I/O granularity: %ld", PACKAGE, (long) block_size));

    assert(DSP_STATE_IS(DSP_INIT|DSP_STOPPED));

    if (DSP_STATE_NOT(DSP_STOPPED)) {
	if (jst.buf_size % block_size != 0 &&
	    block_size % jst.buf_size != 0) {
	    io_errlog(EINVAL, "uneven DSP granularity: %ld",
		      (long) block_size);
	    return;
	}
    }
    dsp_block_size = block_size;
    dsp_block_bytes = block_size * sizeof(jack_default_audio_sample_t);
}


/****************  DSP thread functions  ****************/


/* io_get_dsp_buffers -- get buffer addresses for DSP thread.
 *
 *  Returns: 1 if sufficient space available, 0 otherwise.
 */
int io_get_dsp_buffers(int nchannels, 
		       jack_default_audio_sample_t *in[NCHANNELS],
		       jack_default_audio_sample_t *out[NCHANNELS])
{
    int chan;
    ringbuffer_data_t io_vec[2];

    for (chan = 0; chan < nchannels; chan++) {
	if ((ringbuffer_read_space(in_rb[chan]) < dsp_block_bytes) ||
	    (ringbuffer_write_space(out_rb[chan]) < dsp_block_bytes))
	    return 0;			/* not enough space */

	/* Copy buffer pointers to in[] and out[].  If the ringbuffer
	 * space was discontiguous, we either need to rebuffer or
	 * extend the interface to process_signal() to allow this
	 * situation.  But, that's not implemented yet, hence the
	 * asserts. */
	ringbuffer_get_read_vector(in_rb[chan], io_vec);
	in[chan] = (jack_default_audio_sample_t *) io_vec[0].buf;
	assert(io_vec[0].len >= dsp_block_bytes); /* must be contiguous */
	    
	ringbuffer_get_write_vector(out_rb[chan], io_vec);
	out[chan] = (jack_default_audio_sample_t *) io_vec[0].buf;
	assert(io_vec[0].len >= dsp_block_bytes); /* must be contiguous */
    }
    return 1;				/* success */
}


/* io_dsp_thread -- DSP thread main loop.
 *
 *  Main program of a realtime thread separate from and with a lower
 *  priority than the JACK process() thread.  When the JACK period is
 *  small, the process() thread queues multiple blocks, so the DSP can
 *  accumulate enough input to run efficiently.
 *
 *  DSP engine state transitions:
 *	DSP_ACTIVATING -> DSP_STARTING	when ready for input
 *	DSP_STARTING   -> DSP_RUNNING	when output available
 *
 *  Exits when DSP_STOPPING set.
 */
void *io_dsp_thread(void *arg)
{
    jack_default_audio_sample_t *in[NCHANNELS], *out[NCHANNELS];
    int chan;
    int rc;

    IF_DEBUG(DBG_TERSE, io_trace("DSP thread start"));

    /* The DSP lock is held whenever this thread is actually running. */
    pthread_mutex_lock(&lock_dsp);

    /* This check is because we may already have shut down. */
    if (DSP_STATE_IS(DSP_ACTIVATING))
	io_new_state(DSP_STARTING);	/* allow queuing to begin */

    while (DSP_STATE_NOT(DSP_STOPPING)) {

	/* process any buffers queued for DSP */
	while (io_get_dsp_buffers(nchannels, in, out)) {

	    rc = process_signal(dsp_block_size, nchannels, in, out);
	    if (rc != 0)
		io_errlog(EAGAIN, "signal processing error: %d.", rc);

	    IF_DEBUG(DBG_NORMAL, io_trace("DSP process_signal() done"));

	    /* Advance the ring buffers.  This frees up the input
	     * space and queues the output for the JACK process
	     * thread. */
	    for (chan = 0; chan < nchannels; chan++) {
		ringbuffer_write_advance(out_rb[chan], dsp_block_bytes);
		ringbuffer_read_advance(in_rb[chan], dsp_block_bytes);
	    }
	    if (DSP_STATE_IS(DSP_STARTING))
		io_new_state(DSP_RUNNING); /* output available */
	}

	/* Wait for io_schedule() to wake us up.  Make sure data
	 * really are available.  Pthreads can give spurious wakeups,
	 * sometimes. */
	rc = pthread_cond_wait(&run_dsp, &lock_dsp);
	if (rc != 0)
	    io_errlog(EINVAL, "pthread_cond_wait() returns %d.", rc);

	IF_DEBUG(DBG_NORMAL, io_trace("DSP thread wakeup"));

    };

    pthread_mutex_unlock(&lock_dsp);

    IF_DEBUG(DBG_TERSE, io_trace("DSP thread end"));

    return NULL;
}


/****************  JACK thread functions  ****************/


/* io_schedule -- schedule the DSP thread to run.
 *
 *  The DSP thread holds the DSP lock whenever it is running.  In that
 *  case, we need not do anything more here.  This function is called
 *  from the JACK thread, so it must not wait.
 */
void io_schedule()
{
    if (pthread_mutex_trylock(&lock_dsp) == 0) {
	IF_DEBUG(DBG_NORMAL, io_trace(" DSP scheduled"));
	pthread_cond_signal(&run_dsp);
	pthread_mutex_unlock(&lock_dsp);
    }
    else
	IF_DEBUG(DBG_NORMAL, io_trace(" DSP already running"));

}


/* io_queue -- queue JACK buffers to DSP thread.
 *
 *  Runs as a high-priority realtime thread.  Cannot ever wait.
 */
int io_queue(jack_nframes_t nframes, int nchannels,
	     jack_default_audio_sample_t *in[NCHANNELS],
	     jack_default_audio_sample_t *out[NCHANNELS])
{
    int chan;
    int rc = 0;
    size_t nbytes = nframes * sizeof(jack_default_audio_sample_t);
    size_t count;

    if (DSP_STATE_IS(DSP_ACTIVATING))	/* DSP thread not ready? */
	return EBUSY;

    IF_DEBUG(DBG_VERBOSE, io_trace(" DSP input queued"));

    /* queue JACK input buffers for DSP thread */
    for (chan = 0; chan < nchannels; chan++) {
	count = ringbuffer_write(in_rb[chan], (void *) in[chan], nbytes);
	if (count != nbytes) {		/* buffer overflow? */

	    /* This is a realtime bug.  We have input audio with no
	     * place to go.  The DSP thread is not keeping up, and
	     * there's nothing we can do about it here. */
	    IF_DEBUG(DBG_TERSE,
		     ((chan == 0)?
		      io_trace("input overflow, %ld bytes written.", count):
		      NULL));
	    abort();			/* take a dump */
	    rc = ENOSPC;		/* out of space */
	}
    } 

    /* if there is enough input, schedule the DSP thread */
    if (ringbuffer_read_space(in_rb[0]) >= dsp_block_bytes)
	io_schedule();

    /* dequeue the next buffer that has been completed */
    for (chan = 0; chan < nchannels; chan++) {
	count = ringbuffer_read(out_rb[chan], (void *) out[chan], nbytes);
	if (count != nbytes) {		/* not enough output? */

	    /* this is only legit if we're just starting up */
	    if (DSP_STATE_NOT(DSP_STARTING|DSP_STOPPING)) {

		/* This is a realtime bug.  We do not have output
		 * audio when we need it.  The DSP thread is not
		 * keeping up. */
		IF_DEBUG(DBG_TERSE,
			 ((chan == 0)?
			  io_trace("output underflow, %ld bytes read.", count):
			  NULL));
		rc = EPIPE;		/* broken pipe */
	    }

	    /* fill rest of JACK buffer with zeroes */
	    if (count < nbytes) {
		void *addr = ((void *) out[chan]) + count;
		memset(addr, 0, nbytes-count);
	    }
	}
    }

    return rc;
}


/* io_process -- JACK process callback.
 *
 *  Runs as a high-priority realtime thread.  Cannot ever wait.
 */
int io_process(jack_nframes_t nframes, void *arg)
{
    jack_default_audio_sample_t *in[NCHANNELS], *out[NCHANNELS];
    int chan;
    int return_code = 0;
    int rc;

    IF_DEBUG(DBG_VERBOSE, io_trace("JACK process() start"));

#ifndef NEW_JACK_TRANSPORT
    transport_control(nframes);		/* handle JACK transport */
#endif /* NEW_JACK_TRANSPORT */

    /* get input and output buffer addresses from JACK */
    for (chan = 0; chan < nchannels; chan++) {
	in[chan] = jack_port_get_buffer(input_ports[chan], nframes);
	out[chan] = jack_port_get_buffer(output_ports[chan], nframes);
    }

    if (nframes < dsp_block_size) {

	/* This JACK buffer is smaller than desired DSP granularity.
	 * That increase FFT overhead, just when we most want low
	 * latency.  Normally, we schedule a separate thread to handle
	 * this case, queuing buffers to it until dsp_block_size
	 * frames are available.  If there's some reason not to do
	 * that, then process it here in smaller chunks.
	 */
	if (use_dsp_thread) 
	    return_code = io_queue(nframes, nchannels, in, out);
	else
	    return_code = process_signal(nframes, nchannels, in, out);

    } else {

	/* With larger JACK buffers, call DSP directly. */ 
	while (nframes >= dsp_block_size)  {

	    if ((rc = process_signal(dsp_block_size,
				     nchannels, in, out)) != 0)
		return_code = rc;

	    IF_DEBUG(DBG_VERBOSE, io_trace(" DSP block done"));

	    for (chan = 0; chan < nchannels; chan++) {
		in[chan] += dsp_block_size;
		out[chan] += dsp_block_size;
	    }
	    nframes -= dsp_block_size;
	}
    }

    IF_DEBUG(DBG_VERBOSE, io_trace("JACK process() end"));

    return 0;
}


/* io_cleanup -- clean up all DSP I/O resources.
 *
 *  Called in main user interface thread after user requests "quit",
 *  or in JACK thread if shutdown callback invoked.  JACK allows the
 *  shutdown handler to wait, even though it runs in the process()
 *  thread.
 *
 *  DSP engine state transitions:
 *      DSP_INIT	-> DSP_STOPPED
 *	DSP_ACTIVATING	-> DSP_STOPPING -> DSP_STOPPED
 *	DSP_STARTING	-> DSP_STOPPING -> DSP_STOPPED
 *	DSP_RUNNING	-> DSP_STOPPING -> DSP_STOPPED
 *	DSP_STOPPING	-> DSP_STOPPED
 *	DSP_STOPPED	<unchanged>	do nothing, if stopped already
 */
void io_cleanup()
{
    int chan;

    IF_DEBUG(DBG_TERSE, io_trace("shutting down I/O and DSP"));

    switch (dsp_state) {

    case DSP_INIT:			/* should not happen */
	io_new_state(DSP_STOPPED);
	break;

    case DSP_ACTIVATING:
    case DSP_STARTING:
    case DSP_RUNNING:
	if (have_dsp_thread) {
	    pthread_mutex_lock(&lock_dsp);
	    io_new_state(DSP_STOPPING);	/* stop the DSP thread */
	    pthread_cond_signal(&run_dsp);
	    pthread_mutex_unlock(&lock_dsp);
	    pthread_join(dsp_thread, NULL);
	}
	else
	    io_new_state(DSP_STOPPING);
	break;
    };	

    if (DSP_STATE_IS(DSP_STOPPING)) {

	jack_client_close(client);	/* leave the jack graph */
	jst.active = 0;
	client = NULL;
	io_new_state(DSP_STOPPED);

	/* free the ring buffers */
	for (chan = 0; chan < nchannels; chan++) {
	    if (in_rb[chan])
		ringbuffer_free(in_rb[chan]);
	    if (out_rb[chan])
		ringbuffer_free(out_rb[chan]);
	}
    }

    if (trace_option)
	io_list_trace();		/* list trace buffer contents */
}


/****************  Initialization  ****************/

/* io_init -- initialize DSP engine.
 *
 *  DSP engine state transitions:
 *	DSP_INIT -> DSP_STOPPED		when -d command option set
 *	DSP_INIT <unchanged>		otherwise
 */
void io_init(int argc, char *argv[])
{
    int chan;
    int opt;
    char client_name[256];

    /* basename $0 */
    pname = strrchr(argv[0], '/');
    if (pname == 0)
	pname = argv[0];
    else
	pname++;

    while ((opt = getopt(argc, argv, jamin_options)) != -1) {
	switch (opt) {
	case 'd':			/* dummy mode, no JACK */
	    dummy_mode = 1;
	    break;
	case 'F':			/* all errors fatal */
	    all_errors_fatal = 1;
	    break;
	case 'h':			/* show help */
	    show_help = 1;
	    break;
	case 't':			/* no DSP thread */
	    thread_option = 0;
	    break;
	case 'T':			/* list trace output */
	    trace_option = 1;
	    break;
	case 'v':			/* verbose */
	    debug_level += 1;		/* increment output level */
	    break;
	case 'V':			/* version */
	    /* version info already printed */
	    exit(9);
	default:
	    show_help = 1;
	    break;
	}
    }

    /* check input and output port names for each channel */
    if ((argc - optind) >= nchannels)
	for (chan = 0; chan < nchannels; chan++)
	    iports[chan] = argv[optind++];

    if ((argc - optind) >= nchannels)
	for (chan = 0; chan < nchannels; chan++)
	    oports[chan] = argv[optind++];

    if (argc != optind)			/* any extra options? */
	show_help = 1;

    if (show_help) {
	fprintf(stderr,
		"Usage: %s [-%s] [inport1 inport2 [outport1 outport2]]\n\n",
		pname, jamin_options);
	exit(1);
    }

    if (dummy_mode) {
	io_new_state(DSP_STOPPED);
	jst.buf_size = 1024;
	jst.sample_rate = 48000;
	process_init(48000.0f, 1024);
	return;
    }

    /* register as a JACK client */
    snprintf(client_name, 255, PACKAGE);
    printf("Registering as %s\n", client_name);

    client = jack_client_new(client_name);
    if (client == 0) {
	fprintf(stderr,
		"%s: Cannot contact JACK server, is it running?\n", PACKAGE);
	exit(2);
    }

    jst.buf_size = jack_get_buffer_size(client);
    jst.sample_rate = jack_get_sample_rate(client);

    jack_set_process_callback(client, io_process, NULL);
    jack_on_shutdown(client, io_cleanup, NULL);
    // JOQ: jack_set_buffer_size_callback(client, io_bufsize, NULL);
    // JOQ: jack_set_xrun_callback(client, io_xrun, NULL);

    /* initialize process_signal() */
    process_init((float) jst.sample_rate, jst.buf_size);
}


/* io_create_dsp_thread -- create DSP engine thread.
 *
 *  returns:	0 if successful, error code otherwise.
 */
int io_create_dsp_thread()
{
    int rc;
    int policy;
    struct sched_param rt_param, my_param;
    pthread_attr_t attributes;
    pthread_attr_init(&attributes);

    /* Set priority and scheduling parameters based on the attributes
     * of the JACK client thread. */
    rc = pthread_getschedparam(jack_client_thread_id(client),
			       &policy, &rt_param);
    if (rc) {
	io_errlog(EPERM, "cannot get JACK scheduling params, rc = %d.", rc);
	return rc;
    }

    /* Check if JACK is running with --realtime option. */
#ifdef HAVE_JACK_IS_REALTIME		/* requires libjack >= 0.70.5 */
    jst.realtime = jack_is_realtime(client);
#else
    jst.realtime = (policy == SCHED_FIFO);
#endif

    if (jst.realtime) {
	IF_DEBUG(DBG_TERSE,
		 io_trace("JACK realtime priority = %d",
			  rt_param.sched_priority));
	rt_param.sched_priority -= DSP_PRIORITY_DIFF;
	IF_DEBUG(DBG_TERSE,
		 io_trace("DSP realtime priority = %d",
			  rt_param.sched_priority));
    } else
	IF_DEBUG(DBG_TERSE, io_trace("JACK subsystem not realtime"));

    rc = pthread_attr_setschedpolicy(&attributes, policy);
    if (rc) {
	io_errlog(EPERM, "cannot set scheduling policy, rc = %d.", rc);
	return rc;
    }

    rc = pthread_attr_setscope(&attributes, PTHREAD_SCOPE_SYSTEM);
    if (rc) {
	io_errlog(EPERM, "cannot set RT scheduling scope, rc = %d.", rc);
	return rc;
    }

    rc = pthread_attr_setschedparam(&attributes, &rt_param);
    if (rc) {
	io_errlog(EPERM, "cannot set RT priority, rc = %d.", rc);
	return rc;
    }

    /* this should work, but using capabilities it often doesn't */
    rc = pthread_create(&dsp_thread, &attributes, io_dsp_thread, NULL);
    if (rc == 0) {
	IF_DEBUG(DBG_TERSE, io_trace("DSP thread created"));
	return 0;
    }

    IF_DEBUG(DBG_TERSE, io_trace("first pthread_create() returns %d\n", rc));

    /* The following comment was copied from jack/libjack/client.c
     * along with most of this code... */

    /* the version of glibc I've played with has a bug that makes
       that code fail when running under a non-root user but with the
       proper realtime capabilities (in short,  pthread_attr_setschedpolicy 
       does not check for capabilities, only for the uid being
       zero). Newer versions apparently have this fixed. This
       workaround temporarily switches the client thread to the
       proper scheduler and priority, then starts the realtime
       thread so that it can inherit them and finally switches the
       client thread back to what it was before. Sigh. For ardour
       I have to check again and switch the thread explicitly to
       realtime, don't know why or how to debug - nando
    */

    /* get current scheduler and parameters of the client process */
    if ((policy = sched_getscheduler(0)) < 0) {
	io_errlog(EPERM,
		  "Cannot get current client scheduler: %s",
		  strerror(errno));
	return -1;
    }

    memset(&my_param, 0, sizeof(my_param));
    if (sched_getparam(0, &my_param)) {
	io_errlog(EPERM,
		  "Cannot get current client scheduler parameters: %s",
		  strerror(errno));
	return -1;
    }

    /* temporarily change the client process to SCHED_FIFO so that
       the realtime thread can inherit the scheduler and priority
    */
    if (sched_setscheduler(0, SCHED_FIFO, &rt_param)) {
	io_errlog(EPERM, "Cannot temporarily set RT scheduling: %s",
		  strerror(errno));
	return -1;
    }

    /* prepare the attributes for the realtime thread */
    pthread_attr_init(&attributes);
    if ((pthread_attr_setscope(&attributes, PTHREAD_SCOPE_SYSTEM)) ||
	(pthread_attr_setinheritsched(&attributes, PTHREAD_INHERIT_SCHED))) {
	sched_setscheduler(0, policy, &my_param);
	io_errlog(EPERM, "Cannot set RT thread attributes");
	return -1;
    }

    /* create the RT thread */
    rc = pthread_create(&dsp_thread, &attributes, io_dsp_thread, NULL);
    if (rc != 0) {
	sched_setscheduler(0, policy, &my_param);
	fprintf(stderr,
		"%s: not permitted to create realtime DSP thread.\n"
		"\tYou must run as root or use JACK capabilities.\n"
		"\tContinuing operation, but with -t option.\n", PACKAGE);
	IF_DEBUG(DBG_TERSE,
		 io_trace("second pthread_create() returns %d\n", rc));
	return rc;
    }

    /* return this thread to the scheduler it used before */
    sched_setscheduler(0, policy, &my_param);
    IF_DEBUG(DBG_TERSE, io_trace("DSP thread finally created"));
    return 0;
}


/* io_activate -- activate DSP engine.
 *
 *  DSP engine state transitions:
 *	DSP_INIT -> DSP_ACTIVATING
 *	DSP_ACTIVATING -> DSP_RUNNING	if no DSP thread available
 *	DSP_STOPPED <unchanged>		do nothing if engine already stopped
 */
void io_activate()
{
    int chan;
    size_t bufsize;

    if (DSP_STATE_IS(DSP_STOPPED))
	return;

    io_new_state(DSP_ACTIVATING);

    for (chan = 0; chan < nchannels; chan++) {
	input_ports[chan] =
	    jack_port_register(client, in_names[chan],
			       JACK_DEFAULT_AUDIO_TYPE,
			       JackPortIsInput, 0);
	output_ports[chan] =
	    jack_port_register(client, out_names[chan],
			       JACK_DEFAULT_AUDIO_TYPE,
			       JackPortIsOutput, 0);

	if (input_ports[chan] == NULL || output_ports[chan] == NULL) {
	    fprintf(stderr, "%s: Cannot register JACK ports.", PACKAGE);
	    exit(2);
	}
    }

    if (jack_activate(client)) {
	fprintf(stderr, "%s: Cannot activate JACK client.", PACKAGE);
	exit(2);
    }

    jst.active = 1;

    /* connect all the JACK ports */
    for (chan = 0; chan < nchannels; chan++) {
	if (jack_connect(client, iports[chan],
			 jack_port_name(input_ports[chan])))
	    fprintf(stderr, "Cannot connect input port \"%s\"\n",
		    iports[chan]);
	if (jack_connect(client, jack_port_name(output_ports[chan]),
			 oports[chan]))
	    fprintf(stderr, "Cannot connect output port \"%s\"\n",
		    oports[chan]);
    }

    /* Allocate DSP engine ringbuffers.  Be careful to get the sizes
     * right, they are important for correct operation.  If we are
     * running realtime, jack_activate() will already have called
     * mlockall() for this address space.  So, all we need to do is
     * touch all the pages in the buffers. */
    bufsize = dsp_block_bytes * NCHUNKS;
    for (chan = 0; chan < nchannels; chan++) {
	in_rb[chan] = ringbuffer_create(bufsize);
	memset(in_rb[chan]->buf, 0, bufsize);
	out_rb[chan] = ringbuffer_create(bufsize);
	memset(out_rb[chan]->buf, 0, bufsize);
    }

    /* create DSP thread, if desired and able */
    pthread_mutex_lock(&lock_dsp);
    if (thread_option) {
	have_dsp_thread = (io_create_dsp_thread() == 0);
    } else {
	IF_DEBUG(DBG_TERSE, io_trace("no DSP thread created"));
	have_dsp_thread = 0;
    }
    use_dsp_thread = have_dsp_thread && (dsp_block_size > jst.buf_size);
    if (!have_dsp_thread)
	io_new_state(DSP_RUNNING);

    /* If we run the DSP in a separate thread, there will be some
     * additional latency caused by the extra buffering. */
    io_set_latency(LAT_BUFFERS, (use_dsp_thread? dsp_block_size: 0));
    pthread_mutex_unlock(&lock_dsp);
}

/* vi:set ts=8 sts=4 sw=4: */
