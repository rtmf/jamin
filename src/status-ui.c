/*
 *  status-ui.c -- JACK status user interface.
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

#include <stdio.h>
#include <gtk/gtk.h>
#include <jack/jack.h>
#include <config.h>
#include "jackstatus.h"
#include "transport.h"
#include "status-ui.h"
#include "support.h"


static GtkLabel *l_status_label = NULL;

void status_update(GtkWidget *main_window)
{
    char *state_msg, *rt;
    gchar title[256];
    
    jack_status_t j;

    io_get_status(&j);

    if (!j.active)
	state_msg = "Disconnected";
    else if (j.tinfo.valid & JackTransportState) {
	switch (j.tinfo.transport_state) {
	case JackTransportStopped:
	    state_msg = "Stopped";
	    break;
	case JackTransportRolling:
	    state_msg = "Rolling";
	    break;
	case JackTransportLooping:
	    state_msg = "Looping";
	    break;
	default:
	    state_msg = "[unknown]";
	}
    } else
	state_msg = "";

    if (j.realtime)
      rt = " : Realtime";
    else
      rt = "";

    snprintf(title, sizeof(title), "%s : %4.1f%% CPU : %ld frames : %ld Hz%s",
             state_msg, j.cpu_load, j.buf_size, j.sample_rate, rt);

    if (l_status_label == NULL)
      l_status_label = GTK_LABEL (lookup_widget (main_window, "status_label"));

    gtk_label_set_text (l_status_label, title);
}
