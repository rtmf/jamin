/*
 * Initial main.c file generated by Glade. Edit as required.
 * Glade will not overwrite this file.
 */

#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <gtk/gtk.h>
#include <limits.h>

#include "interface.h"
#include "support.h"
#include "io.h"
#include "geq.h"
#include "limiter-ui.h"
#include "compressor-ui.h"
#include "intrim.h"
#include "process.h"
#include "spectrum.h"
#include "stereo.h"
#include "state.h"
#include "status-ui.h"

GtkWidget *main_window;

gboolean update_meters(gpointer data);

int main(int argc, char *argv[])
{
    char rcfile[PATH_MAX];
    gchar title[40];
    int fd;

#ifdef ENABLE_NLS
    bindtextdomain(GETTEXT_PACKAGE, PACKAGE_LOCALE_DIR);
    bind_textdomain_codeset(GETTEXT_PACKAGE, "UTF-8");
    textdomain(GETTEXT_PACKAGE);
#endif

    printf("JAMin " VERSION "\n");
    printf("(c) 2003 J. Depner, S. Harris, J. O'Quin, R. Parker and P. Shirkey\n");
    printf("This is free software, and you are welcome to redistribute it\n" 
	   "under certain conditions; see the file COPYING for details.\n");


    /* look for the rcfile, if its there parse it */

    snprintf(rcfile, PATH_MAX, "%s/%s", getenv("HOME"), ".jamrc");
    if ((fd = open(rcfile, O_RDONLY)) >= 0) {	close(fd);
	printf("Using jamrc file: %s\n", rcfile);
	gtk_rc_parse(rcfile);
    }


    gtk_set_locale();
    gtk_init(&argc, &argv);
    io_init(argc, argv);
    state_init();
    add_pixmap_directory(PACKAGE_DATA_DIR "/" PACKAGE "/pixmaps");
    main_window = create_window1();
    //status_init();


    /* bind the graphic equaliser sliders to adjustments */

    bind_geq();
    gtk_widget_show(main_window);
    bind_intrim();
    bind_limiter();
    bind_compressors();
    bind_spectrum();
    bind_stereo();
    s_clear_history();


    /* start I/O processing, then run GTK main loop, until "quit" */

    io_activate();
    g_timeout_add(100, update_meters, NULL);
    gtk_main();
    io_cleanup();

    return 0;
}


gboolean update_meters(gpointer data)
{
    static unsigned int    count = 0;


    in_meter_value(in_peak);
    out_meter_value(out_peak);
    limiter_meters_update();
    compressor_meters_update();
    spectrum_update();


    /*  Only update the status once a second.  */

    if (count % 10) status_update (main_window);
    count++;


    return TRUE;
}

/* vi:set ts=8 sts=4 sw=4: */
