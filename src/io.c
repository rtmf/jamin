#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/types.h>
#include <unistd.h>
#include <jack/jack.h>
#include <getopt.h>

#include "process.h"
#include "plugin.h"

float sample_rate;

void make_channel(jack_client_t *client, int i, char *port_name);
void cleanup(void);

jack_port_t *input_ports[2];
jack_port_t *output_ports[2];
jack_client_t *client;

int backend_init(int argc, char *argv[])
{
	unsigned int i;
	int opt;
	int show_help = 0;
	char client_name[256];
	char *ioports[4];

	while ((opt = getopt(argc, argv, "h")) != -1) {
		switch (opt) {
			case 'h':
				/* Force help to be shown */
				show_help = 1;
				break;
			default:
				show_help = 1;
				break;
		}
	}

	if ((argc != 5 && argc != 1) || show_help) {
		fprintf(stderr, "Usage %s: [<inport> <inport> <outport> <outport>]\n\n", argv[0]);
		exit(1);
	}

	/* Register with jack */
	snprintf(client_name, 255, "jam");
	if ((client = jack_client_new(client_name)) == 0) {
		fprintf(stderr, "jack server not running?\n");
		exit(1);
	}
	printf("Registering as %s\n", client_name);

	sample_rate = (float)jack_get_sample_rate(client);
	process_init(sample_rate, jack_get_buffer_size(client));

	jack_set_process_callback(client, process, 0);

	if (jack_activate(client)) {
		fprintf (stderr, "cannot activate client");
		exit(1);
	}

	input_ports[0] = jack_port_register(client, "in_L",
			JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput, 0);
	input_ports[1] = jack_port_register(client, "in_R",
			JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput, 0);
	output_ports[0] = jack_port_register(client, "out_L",
			JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0);
	output_ports[1] = jack_port_register(client, "out_R",
			JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0);

	if (argc == 5) {
		for (i=0; i<4; i++) {
			ioports[i] = argv[i+1];
		}
	} else {
		ioports[0] = "alsa_pcm:capture_1";
		ioports[1] = "alsa_pcm:capture_2";
		ioports[2] = "alsa_pcm:playback_1";
		ioports[3] = "alsa_pcm:playback_2";
	}

	if (jack_connect(client, ioports[0], jack_port_name(input_ports[0]))) {
		fprintf(stderr, "Cannot make connection\n");
	}
	if (jack_connect(client, ioports[1], jack_port_name(input_ports[1]))) {
		fprintf(stderr, "Cannot make connection\n");
	}
	if (jack_connect(client, jack_port_name(output_ports[0]), ioports[2])) {
		fprintf(stderr, "Cannot make connection\n");
	}
	if (jack_connect(client, jack_port_name(output_ports[1]), ioports[3])) {
		fprintf(stderr, "Cannot make connection\n");
	}

	return 0;
}

void cleanup()
{
	printf("quiting\n");

	/* Leave the jack graph */
	jack_client_close(client);

	/* And we're done */
	exit(0);
}
