/*
 *  Copyright (C) 2003 Jan C. Depner, Jack O'Quin, Steve Harris
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
 *  $Id: callbacks.c,v 1.109 2003/12/10 18:33:17 jdepner Exp $
 */

#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/times.h>
#include <errno.h>
#include <math.h>
#include <string.h>
#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>


#include "main.h"
#include "callbacks.h"
#include "callbacks_help.h"
#include "geq.h"
#include "hdeq.h"
#include "interface.h"
#include "support.h"
#include "process.h"
#include "intrim.h"
#include "compressor-ui.h"
#include "gtkmeter.h"
#include "gtkmeterscale.h"
#include "state.h"
#include "db.h"
#include "status-ui.h"
#include "io-menu.h"
#include "transport.h"
#include "scenes.h"
#include "help.h"


/* vi:set ts=8 sts=4 sw=4: */


static char *help_ptr = general_help;
static gboolean text_focus = FALSE;

void
on_low2mid_value_changed               (GtkRange        *range,
                                        gpointer         user_data)
{
    hdeq_low2mid_set (range);
}


void
on_mid2high_value_changed              (GtkRange        *range,
                                        gpointer         user_data)
{
    hdeq_mid2high_set (range);
}


gboolean
on_low2mid_button_press_event          (GtkWidget       *widget,
                                        GdkEventButton  *event,
                                        gpointer         user_data)
{
    hdeq_low2mid_button (1);

    set_scene_warning_button ();

    return FALSE;
}


gboolean
on_low2mid_button_release_event        (GtkWidget       *widget,
                                        GdkEventButton  *event,
                                        gpointer         user_data)
{
    hdeq_low2mid_button (0);

    return FALSE;
}


gboolean
on_mid2high_button_press_event        (GtkWidget       *widget,
                                        GdkEventButton  *event,
                                        gpointer         user_data)
{
    hdeq_mid2high_button (1);

    set_scene_warning_button ();

    return FALSE;
}


gboolean
on_mid2high_button_release_event      (GtkWidget       *widget,
                                        GdkEventButton  *event,
                                        gpointer         user_data)
{
    hdeq_mid2high_button (0);

    return FALSE;
}


void
on_low2mid_realize                     (GtkWidget       *widget,
                                        gpointer         user_data)
{
    hdeq_low2mid_init ();
}


void
on_mid2high_realize                    (GtkWidget       *widget,
                                        gpointer         user_data)
{
    hdeq_mid2high_init ();
}


void
on_quit_button_clicked                 (GtkButton       *button,
                                        gpointer         user_data)
{
    clean_quit ();
}


gboolean
on_window1_delete_event                (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{
    clean_quit ();

    return FALSE;
}


void
on_window1_show                        (GtkWidget       *widget,
                                        gpointer         user_data)
{
    crossover_init ();

    l_notebook1 = GTK_NOTEBOOK (lookup_widget (main_window, "notebook1"));
}


gboolean
eqb_mod (GtkAdjustment *adj, gpointer user_data)
{
    hdeq_eqb_mod ();

    return FALSE;
}


gboolean
on_EQ_curve_expose_event               (GtkWidget       *widget,
                                        GdkEventExpose  *event,
                                        gpointer         user_data)
{
    hdeq_curve_exposed (widget, event);

    return FALSE;
}


void
on_EQ_curve_realize                    (GtkWidget       *widget,
                                        gpointer         user_data)
{
    hdeq_curve_init (widget);
}


gboolean 
on_EQ_curve_event_box_motion_notify_event
                                        (GtkWidget       *widget,
                                        GdkEventMotion  *event,
                                        gpointer         user_data)
{
    hdeq_curve_motion (event);

    return FALSE;
}


gboolean
on_EQ_curve_event_box_button_press_event
                                        (GtkWidget       *widget,
                                        GdkEventButton  *event,
                                        gpointer         user_data)
{
    hdeq_curve_button_press (event);

    return FALSE;
}


gboolean
on_EQ_curve_event_box_button_release_event
                                        (GtkWidget       *widget,
                                        GdkEventButton  *event,
                                        gpointer         user_data)
{
    hdeq_curve_button_release (event);

    set_scene_warning_button ();

    return FALSE;
}


gboolean
on_EQ_curve_event_box_leave_notify_event
                                        (GtkWidget       *widget,
                                        GdkEventCrossing *event,
                                        gpointer         user_data)
{
    hdeq_curve_set_label ("                ");

    return FALSE;
}


void
on_bypass_button_toggled               (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
    global_bypass = gtk_toggle_button_get_active(togglebutton);
}




void
on_geq_min_gain_spinner_value_changed  (GtkSpinButton   *spinbutton,
                                        gpointer         user_data)
{
    float gain;


    gain = gtk_spin_button_get_value (spinbutton);

    hdeq_set_lower_gain (gain);

    geq_set_range (gain, geq_get_adjustment(0)->upper);

    set_scene_warning_button ();
}


void
on_geq_max_gain_spinner_value_changed  (GtkSpinButton   *spinbutton,
                                        gpointer         user_data)
{
    float gain;


    gain = gtk_spin_button_get_value (spinbutton);

    hdeq_set_upper_gain (gain);

    geq_set_range (geq_get_adjustment(0)->lower, gain);

    set_scene_warning_button ();
}


void
on_lim_out_trim_scale_value_changed        (GtkRange        *range,
                                        gpointer         user_data)
{
    s_set_value_ui(S_LIM_LIMIT,
		    gtk_range_get_adjustment(GTK_RANGE(range))->value);
}


void
on_in_trim_scale_value_changed         (GtkRange        *range,
                                        gpointer         user_data)
{
    s_set_value_ui(S_IN_GAIN, gtk_range_get_adjustment(range)->value);
}


void
on_pan_scale_value_changed             (GtkRange        *range,
                                        gpointer         user_data)
{
    s_set_value_ui(S_IN_PAN, gtk_range_get_adjustment(range)->value);
}


gboolean
on_comp1_curve_expose_event            (GtkWidget       *widget,
                                        GdkEventExpose  *event,
                                        gpointer         user_data)
{
    comp_curve_expose (widget, 0);

    return FALSE;
}


void
on_comp1_curve_realize                 (GtkWidget       *widget,
                                        gpointer         user_data)
{
    comp_curve_realize (widget, 0);
}


gboolean
on_comp2_curve_expose_event            (GtkWidget       *widget,
                                        GdkEventExpose  *event,
                                        gpointer         user_data)
{
    comp_curve_expose (widget, 1);

    return FALSE;
}


void
on_comp2_curve_realize                 (GtkWidget       *widget,
                                        gpointer         user_data)
{
    comp_curve_realize (widget, 1);
}


gboolean
on_comp3_curve_expose_event            (GtkWidget       *widget,
                                        GdkEventExpose  *event,
                                        gpointer         user_data)
{
    comp_curve_expose (widget, 2);

    return FALSE;
}


void
on_comp3_curve_realize                 (GtkWidget       *widget,
                                        gpointer         user_data)
{
    comp_curve_realize (widget, 2);
}


gboolean
on_low_curve_box_motion_notify_event   (GtkWidget       *widget,
                                        GdkEventMotion  *event,
                                        gpointer         user_data)
{
    comp_curve_box_motion (0, event);

    return FALSE;
}


gboolean
on_mid_curve_box_motion_notify_event   (GtkWidget       *widget,
                                        GdkEventMotion  *event,
                                        gpointer         user_data)
{
    comp_curve_box_motion (1, event);

    return FALSE;
}


gboolean
on_high_curve_box_motion_notify_event  (GtkWidget       *widget,
                                        GdkEventMotion  *event,
                                        gpointer         user_data)
{
    comp_curve_box_motion (2, event);

    return FALSE;
}

gboolean
on_low_curve_box_leave_notify_event    (GtkWidget       *widget,
                                        GdkEventCrossing *event,
                                        gpointer         user_data)
{
    draw_comp_curve (0);

    comp_box_leave (0);

    return FALSE;
}


gboolean
on_mid_curve_box_leave_notify_event    (GtkWidget       *widget,
                                        GdkEventCrossing *event,
                                        gpointer         user_data)
{
    draw_comp_curve (1);

    comp_box_leave (1);

    return FALSE;
}


gboolean
on_high_curve_box_leave_notify_event   (GtkWidget       *widget,
                                        GdkEventCrossing *event,
                                        gpointer         user_data)
{
    draw_comp_curve (2);

    comp_box_leave (2);

    return FALSE;
}

gboolean
on_low_curve_box_enter_notify_event    (GtkWidget       *widget,
                                        GdkEventCrossing *event,
                                        gpointer         user_data)
{
    comp_box_enter (0);

    return FALSE;
}


gboolean
on_mid_curve_box_enter_notify_event    (GtkWidget       *widget,
                                        GdkEventCrossing *event,
                                        gpointer         user_data)
{
    comp_box_enter (1);

    return FALSE;
}


gboolean
on_high_curve_box_enter_notify_event   (GtkWidget       *widget,
                                        GdkEventCrossing *event,
                                        gpointer         user_data)
{
    comp_box_enter (2);

    return FALSE;
}


gboolean
on_low_comp_event_box_enter_notify_event
                                        (GtkWidget       *widget,
                                        GdkEventCrossing *event,
                                        gpointer         user_data)
{
    comp_box_enter (0);

    return FALSE;
}


gboolean
on_low_comp_event_box_leave_notify_event
                                        (GtkWidget       *widget,
                                        GdkEventCrossing *event,
                                        gpointer         user_data)
{
    comp_box_leave (0);

    return FALSE;
}


gboolean
on_mid_comp_event_box_enter_notify_event
                                        (GtkWidget       *widget,
                                        GdkEventCrossing *event,
                                        gpointer         user_data)
{
    comp_box_enter (1);

    return FALSE;
}


gboolean
on_mid_comp_event_box_leave_notify_event
                                        (GtkWidget       *widget,
                                        GdkEventCrossing *event,
                                        gpointer         user_data)
{
    comp_box_leave (1);

    return FALSE;
}


gboolean
on_high_comp_event_box_enter_notify_event
                                        (GtkWidget       *widget,
                                        GdkEventCrossing *event,
                                        gpointer         user_data)
{
    comp_box_enter (2);

    return FALSE;
}


gboolean
on_high_comp_event_box_leave_notify_event
                                        (GtkWidget       *widget,
                                        GdkEventCrossing *event,
                                        gpointer         user_data)
{
    comp_box_leave (2);

    return FALSE;
}


GtkWidget*
make_meter (gchar *widget_name, gchar *string1, gchar *string2,
                gint int1, gint int2)
{
    GtkWidget *ret;
    gint dir = GTK_METER_UP;
    GtkAdjustment *adjustment = (GtkAdjustment*) gtk_adjustment_new (0.0,
		    (float)int1, (float)int2, 0.0, 0.0, 0.0);

    if (!string1 || !strcmp(string1, "up")) {
        dir = GTK_METER_UP;
    } else if (!strcmp(string1, "down")) {
	dir = GTK_METER_DOWN;
    } else if (!strcmp(string1, "left")) {
	dir = GTK_METER_LEFT;
    } else if (!strcmp(string1, "right")) {
	dir = GTK_METER_RIGHT;
    }

    ret = gtk_meter_new(adjustment, dir);

    return ret;
}


GtkWidget*
make_mscale (gchar *widget_name, gchar *string1, gchar *string2,
                gint int1, gint int2)
{
    int sides = 0;
    GtkWidget *ret;

    if (string1 && strstr(string1, "left")) {
	sides |= GTK_METERSCALE_LEFT;
    }
    if (string1 && strstr(string1, "right")) {
	sides |= GTK_METERSCALE_RIGHT;
    }
    if (string1 && strstr(string1, "top")) {
	sides |= GTK_METERSCALE_TOP;
    }
    if (string1 && strstr(string1, "bottom")) {
	sides |= GTK_METERSCALE_BOTTOM;
    }

    ret = gtk_meterscale_new(sides, int1, int2);

    return ret;
}


void
on_autobutton_l_toggled                  (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
    comp_set_auto(0, gtk_toggle_button_get_active(togglebutton));
}


void
on_autobutton_m_toggled                  (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
    comp_set_auto(1, gtk_toggle_button_get_active(togglebutton));
}


void
on_autobutton_h_toggled                  (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
    comp_set_auto(2, gtk_toggle_button_get_active(togglebutton));
}


void
on_pre_eq_activate                     (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
    process_set_spec_mode(SPEC_PRE_EQ);
}


void
on_post_eq_activate                    (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
    process_set_spec_mode(SPEC_POST_EQ);
}


void
on_post_compressor_activate            (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
    process_set_spec_mode(SPEC_POST_COMP);
}


void
on_output_activate                     (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
    process_set_spec_mode(SPEC_OUTPUT);
}


void
on_lim_lh_scale_value_changed          (GtkRange        *range,
                                        gpointer         user_data)
{
    s_set_value_ui(S_LIM_TIME,
                 gtk_range_get_adjustment(GTK_RANGE(range))->value);
}

void
on_release_val_label_realize           (GtkWidget       *widget,
                                        gpointer         user_data)
{
    GtkRequisition size;

    gtk_widget_size_request(widget, &size);
    gtk_widget_set_usize(widget, size.width, -1);
}

void
on_hscale_1_l_value_changed               (GtkRange        *range,
                                        gpointer         user_data)
{
    s_set_value_ui(S_STEREO_WIDTH(0),
                   gtk_range_get_adjustment(GTK_RANGE(range))->value);
}


void
on_hscale_1_l_realize                     (GtkWidget       *widget,
                                        gpointer         user_data)
{
    s_set_adjustment(S_STEREO_WIDTH(0),
                     gtk_range_get_adjustment(GTK_RANGE(widget)));
}


void
on_hscale_1_m_value_changed               (GtkRange        *range,
                                        gpointer         user_data)
{
    s_set_value_ui(S_STEREO_WIDTH(1),
                   gtk_range_get_adjustment(GTK_RANGE(range))->value);
}


void
on_hscale_1_m_realize                     (GtkWidget       *widget,
                                        gpointer         user_data)
{
    s_set_adjustment(S_STEREO_WIDTH(1),
                     gtk_range_get_adjustment(GTK_RANGE(widget)));
}


void
on_hscale_1_h_value_changed               (GtkRange        *range,
                                        gpointer         user_data)
{
    s_set_value_ui(S_STEREO_WIDTH(2),
                   gtk_range_get_adjustment(GTK_RANGE(range))->value);
}


void
on_hscale_1_h_realize                     (GtkWidget       *widget,
                                        gpointer         user_data)
{
	s_set_adjustment(S_STEREO_WIDTH(2),
			gtk_range_get_adjustment(GTK_RANGE(widget)));
}


void
on_lim_input_hscale_value_changed      (GtkRange        *range,
                                        gpointer         user_data)
{
    s_set_value_ui(S_LIM_INPUT,
                   gtk_range_get_adjustment(GTK_RANGE(range))->value);
}


void
on_lim_input_hscale_realize            (GtkWidget       *widget,
                                        gpointer         user_data)
{
	s_set_adjustment(S_LIM_INPUT,
			gtk_range_get_adjustment(GTK_RANGE(widget)));
}


void
on_optionmenu1_realize                 (GtkWidget       *widget,
                                        gpointer         user_data)
{
    gtk_option_menu_set_history (GTK_OPTION_MENU(widget), 1);
}


void
on_high_meter_lbl_realize              (GtkWidget       *widget,
                                        gpointer         user_data)
{

}


void
on_low_meter_lbl_realize               (GtkWidget       *widget,
                                        gpointer         user_data)
{

}


void
on_mid_meter_lbl_realize               (GtkWidget       *widget,
                                        gpointer         user_data)
{

}


gboolean
play_toggle                            (GtkWidget       *widget,
                                        GdkEventButton  *event,
                                        gpointer         user_data)
{
    transport_play();
    return FALSE;
}


gboolean
stop_toggle                            (GtkWidget       *widget,
                                        GdkEventButton  *event,
                                        gpointer         user_data)
{
    transport_stop();
    return FALSE;
}

gboolean
rewind_transport                       (GtkWidget       *widget,
                                        GdkEventButton  *event,
                                        gpointer         user_data)
{
    transport_set_position(0);
    return FALSE;
}


gboolean
forward_transport                      (GtkWidget       *widget,
                                        GdkEventButton  *event,
                                        gpointer         user_data)
{

    transport_set_position(1000000000);
    return FALSE;
}

void
on_boost_scale_value_changed           (GtkRange        *range,
                                        gpointer         user_data)
{
    s_set_value_ui(S_BOOST,
                   gtk_range_get_adjustment(GTK_RANGE(range))->value);
}


gboolean
on_scene1_eventbox_button_press_event  (GtkWidget       *widget,
                                        GdkEventButton  *event,
                                        gpointer         user_data)
{
  select_scene (0, event->button);

  return FALSE;
}


gboolean
on_scene2_eventbox_button_press_event  (GtkWidget       *widget,
                                        GdkEventButton  *event,
                                        gpointer         user_data)
{
  select_scene (1, event->button);

  return FALSE;
}


gboolean
on_scene3_eventbox_button_press_event  (GtkWidget       *widget,
                                        GdkEventButton  *event,
                                        gpointer         user_data)
{
  select_scene (2, event->button);

  return FALSE;
}


gboolean
on_scene4_eventbox_button_press_event  (GtkWidget       *widget,
                                        GdkEventButton  *event,
                                        gpointer         user_data)
{
  select_scene (3, event->button);

  return FALSE;
}


gboolean
on_scene5_eventbox_button_press_event  (GtkWidget       *widget,
                                        GdkEventButton  *event,
                                        gpointer         user_data)
{
  select_scene (4, event->button);

  return FALSE;
}


gboolean
on_scene6_eventbox_button_press_event  (GtkWidget       *widget,
                                        GdkEventButton  *event,
                                        gpointer         user_data)
{
  select_scene (5, event->button);

  return FALSE;
}


void
on_setscene_activate                   (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
    set_scene (-1);
}


void
on_clearscene_activate                 (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
    clear_scene (-1);
}


void
on_scene1_name_changed                 (GtkEditable     *editable,
                                        gpointer         user_data)
{
    set_scene_name (0, NULL);
}


void
on_scene2_name_changed                 (GtkEditable     *editable,
                                        gpointer         user_data)
{
    set_scene_name (1, NULL);
}


void
on_scene3_name_changed                 (GtkEditable     *editable,
                                        gpointer         user_data)
{
    set_scene_name (2, NULL);
}


void
on_scene4_name_changed                 (GtkEditable     *editable,
                                        gpointer         user_data)
{
    set_scene_name (3, NULL);
}


void
on_scene5_name_changed                 (GtkEditable     *editable,
                                        gpointer         user_data)
{
    set_scene_name (4, NULL);
}


void
on_scene6_name_changed                 (GtkEditable     *editable,
                                        gpointer         user_data)
{
    set_scene_name (5, NULL);
}


gboolean
on_EQ_curve_event_box_enter_notify_event
                                        (GtkWidget       *widget,
                                        GdkEventCrossing *event,
                                        gpointer         user_data)
{
    help_ptr = hdeq_help;

    return FALSE;
}


gboolean
on_show_help                           (GtkWidget       *widget,
                                        GtkWidgetHelpType  help_type,
                                        gpointer         user_data)
{
    help_message (help_ptr);

    return FALSE;
}


gboolean
on_input_eventbox_enter_notify_event   (GtkWidget       *widget,
                                        GdkEventCrossing *event,
                                        gpointer         user_data)
{
    help_ptr = input_help;

    return FALSE;
}

gboolean
on_geq_eventbox_enter_notify_event     (GtkWidget       *widget,
                                        GdkEventCrossing *event,
                                        gpointer         user_data)
{
    help_ptr = geq_help;

    return FALSE;
}


gboolean
on_spectrum_eventbox_enter_notify_event
                                        (GtkWidget       *widget,
                                        GdkEventCrossing *event,
                                        gpointer         user_data)
{
    help_ptr = spectrum_help;

    return FALSE;
}


gboolean
on_crossover_eventbox_enter_notify_event
                                        (GtkWidget       *widget,
                                        GdkEventCrossing *event,
                                        gpointer         user_data)
{
    help_ptr = crossover_help;

    return FALSE;
}


gboolean
on_comp_curve_eventbox_enter_notify_event
                                        (GtkWidget       *widget,
                                        GdkEventCrossing *event,
                                        gpointer         user_data)
{
    help_ptr = comp_curve_help;

    return FALSE;
}


gboolean
on_comp_eventbox_enter_notify_event    (GtkWidget       *widget,
                                        GdkEventCrossing *event,
                                        gpointer         user_data)
{
    help_ptr = comp_help;

    return FALSE;
}


gboolean
on_stereo_eventbox_enter_notify_event  (GtkWidget       *widget,
                                        GdkEventCrossing *event,
                                        gpointer         user_data)
{
    help_ptr = stereo_help;

    return FALSE;
}


gboolean
on_gain_meter_eventbox_enter_notify_event
                                        (GtkWidget       *widget,
                                        GdkEventCrossing *event,
                                        gpointer         user_data)
{
    help_ptr = gain_meter_help;

    return FALSE;
}


gboolean
on_limiter_eventbox_enter_notify_event (GtkWidget       *widget,
                                        GdkEventCrossing *event,
                                        gpointer         user_data)
{
    help_ptr = limiter_help;

    return FALSE;
}


gboolean
on_boost_eventbox_enter_notify_event   (GtkWidget       *widget,
                                        GdkEventCrossing *event,
                                        gpointer         user_data)
{
    help_ptr = boost_help;

    return FALSE;
}


gboolean
on_output_eventbox_enter_notify_event  (GtkWidget       *widget,
                                        GdkEventCrossing *event,
                                        gpointer         user_data)
{
    help_ptr = output_help;

    return FALSE;
}

gboolean
on_help_button_enter_notify_event      (GtkWidget       *widget,
                                        GdkEventCrossing *event,
                                        gpointer         user_data)
{
    help_ptr = general_help;

    return FALSE;
}


gboolean
on_eq_options_eventbox_enter_notify_event
                                        (GtkWidget       *widget,
                                        GdkEventCrossing *event,
                                        gpointer         user_data)
{
    help_ptr = eq_options_help;

    return FALSE;
}


gboolean
on_spectrum_options_eventbox_enter_notify_event
                                        (GtkWidget       *widget,
                                        GdkEventCrossing *event,
                                        gpointer         user_data)
{
    help_ptr = spectrum_options_help;

    return FALSE;
}


gboolean
on_status_label_eventbox_enter_notify_event
                                        (GtkWidget       *widget,
                                        GdkEventCrossing *event,
                                        gpointer         user_data)
{
    help_ptr = status_label_help;

    return FALSE;
}


gboolean
on_transport_controls_eventbox_enter_notify_event
                                        (GtkWidget       *widget,
                                        GdkEventCrossing *event,
                                        gpointer         user_data)
{
    help_ptr = transport_controls_help;

    return FALSE;
}


gboolean
on_bypass_button_enter_notify_event    (GtkWidget       *widget,
                                        GdkEventCrossing *event,
                                        gpointer         user_data)
{
    help_ptr = bypass_help;

    return FALSE;
}


gboolean
on_scenes_eventbox_enter_notify_event  (GtkWidget       *widget,
                                        GdkEventCrossing *event,
                                        gpointer         user_data)
{
    help_ptr = scenes_help;

    return FALSE;
}


void
on_notebook1_switch_page               (GtkNotebook     *notebook,
                                        GtkNotebookPage *page,
                                        guint            page_num,
                                        gpointer         user_data)
{
    hdeq_notebook1_set_page (page_num);
}


gboolean
on_scene_name_focus_in_event          (GtkWidget       *widget,
                                       GdkEventFocus   *event,
                                       gpointer         user_data)
{
    text_focus = TRUE;
    return FALSE;
}


gboolean
on_scene_name_focus_out_event         (GtkWidget       *widget,
                                       GdkEventFocus   *event,
                                       gpointer         user_data)
{
    text_focus = FALSE;
    return FALSE;
}


void
on_new1_activate                       (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{

}


void
on_open1_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
    GtkFileSelection    *file_selector;
    gchar               string[512];


    file_selector = 
       (GtkFileSelection *) gtk_file_selection_new ("Select a session file");

    if (getenv ("HOME") != NULL)
      {
        snprintf (string, sizeof(string), "%s/.jam/", getenv ("HOME"));
        gtk_file_selection_set_filename (file_selector, string);
      }

    gtk_file_selection_complete (file_selector, "*.jam");

    g_signal_connect (GTK_OBJECT (file_selector->ok_button),
        "clicked", G_CALLBACK (s_load_session_from_ui), file_selector);

    g_signal_connect_swapped (GTK_OBJECT (file_selector->ok_button),
        "clicked", G_CALLBACK (gtk_widget_destroy), (gpointer) file_selector);

    g_signal_connect_swapped (GTK_OBJECT (file_selector->cancel_button),
        "clicked", G_CALLBACK (gtk_widget_destroy), (gpointer) file_selector);

    gtk_widget_show ((GtkWidget *) file_selector);
}


void
on_save_as1_activate                   (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
    GtkFileSelection    *file_selector;
    gchar               *string;


    file_selector = 
       (GtkFileSelection *) gtk_file_selection_new ("Select a session file");

    if (getenv ("HOME") != NULL)
      {
        string = g_strdup_printf("%s/.jam/", getenv ("HOME"));
        gtk_file_selection_set_filename (file_selector, string);
      }

    gtk_file_selection_complete (file_selector, "*.jam");

    g_signal_connect (GTK_OBJECT (file_selector->ok_button),
        "clicked", G_CALLBACK (s_save_session_from_ui), file_selector);

    g_signal_connect_swapped (GTK_OBJECT (file_selector->ok_button),
        "clicked", G_CALLBACK (gtk_widget_destroy), (gpointer) file_selector);

    g_signal_connect_swapped (GTK_OBJECT (file_selector->cancel_button),
        "clicked", G_CALLBACK (gtk_widget_destroy), (gpointer) file_selector);

    gtk_widget_show ((GtkWidget *) file_selector);
}


void
on_save1_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	if (s_have_filename()) {
		s_save_session(NULL);
	} else {
		on_save_as1_activate (NULL, NULL);
	}
}


void
on_quit1_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
    clean_quit();
}


void
on_undo1_activate                       (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  s_undo();
}

void
on_redo1_activate                       (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  s_redo();
}

void
on_cut1_activate                       (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{

}


void
on_copy1_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{

}


void
on_paste1_activate                     (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{

}


void
on_delete1_activate                    (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{

}


void
on_about1_activate                     (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
    help_message (general_help);
}


void
on_about_prerequisites1_activate       (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
    help_message (prerequisites_help);
}


gboolean
on_frame_l_enter_notify_event          (GtkWidget       *widget,
                                        GdkEventCrossing *event,
                                        gpointer         user_data)
{
  comp_box_enter (0);

  return FALSE;
}


gboolean
on_frame_m_enter_notify_event          (GtkWidget       *widget,
                                        GdkEventCrossing *event,
                                        gpointer         user_data)
{
  comp_box_enter (1);

  return FALSE;
}


gboolean
on_frame_h_enter_notify_event          (GtkWidget       *widget,
                                        GdkEventCrossing *event,
                                        gpointer         user_data)
{
  comp_box_enter (2);

  return FALSE;
}


void
on_help1_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  help_message (help_help);
}


void
on_keys1_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  help_message (keys_help);
}


gboolean
on_window1_key_press_event             (GtkWidget       *widget,
                                        GdkEventKey     *event,
                                        gpointer         user_data)
{
    GtkToggleButton       *bypass;
    gboolean              tmp;
    unsigned int          key = event->keyval, state = event->state;
    int                   scene;


    /*  If a text widget has the focus we don't want to trap key presses.  */

    if (text_focus) return FALSE;


    scene = -1;


    switch (key)
      {

        /*  Bypass  */

      case GDK_b:
        bypass = GTK_TOGGLE_BUTTON (lookup_widget (main_window, 
                                                   "bypass_button"));
        tmp = gtk_toggle_button_get_active (bypass);
        gtk_toggle_button_set_active (bypass, (!tmp));
        break;


        /*  Toggle play  */

      case GDK_space:
	transport_toggle_play();
        break;


        /*  Rewind  */

      case GDK_less:
	transport_set_position(0);
        break;


        /*  FF  */

      case GDK_greater:
	transport_set_position(1000000000);
        break;


        /*  Select scene 1  */

      case GDK_1:
      case GDK_KP_1:
      case GDK_KP_End:
        scene = 0;
        break;


        /*  Select scene 2  */

      case GDK_2:
      case GDK_KP_2:
      case GDK_KP_Down:
        scene = 1;
        break;


        /*  Select scene 3  */

      case GDK_3:
      case GDK_KP_3:
      case GDK_KP_Page_Down:
        scene = 2;
        break;


        /*  Select scene 4  */

      case GDK_4:
      case GDK_KP_4:
      case GDK_KP_Left:
        scene = 3;
        break;


        /*  Select scene 5  */

      case GDK_5:
      case GDK_KP_5:
      case GDK_KP_Begin:
        scene = 4;
        break;


        /*  Select scene 6  */

      case GDK_6:
      case GDK_KP_6:
      case GDK_KP_Right:
        scene = 5;
        break;

 
        /*  Undo  */

      case GDK_u:
        if (state == GDK_CONTROL_MASK) s_undo ();
        break;


        /*  Redo  */
/*
      case GDK_r:
        if (state == GDK_CONTROL_MASK) s_redo ();
        break;
*/


        /*  Switch to tab 1 (HDEQ)  */

      case GDK_F1:
        gtk_notebook_set_current_page (l_notebook1, 0);
        break;


        /*  Switch to tab 2 (30 band EQ)  */

      case GDK_F2:
        gtk_notebook_set_current_page (l_notebook1, 1);
        break;


        /*  Switch to tab 3 (Spectrum)  */

      case GDK_F3:
        gtk_notebook_set_current_page (l_notebook1, 2);
        break;


        /*  Switch to tab 4 (Compressor curves)  */

      case GDK_F4:
        gtk_notebook_set_current_page (l_notebook1, 3);
        break;


        /*  Switch to tab 5 (EQ options)  */

      case GDK_F5:
        gtk_notebook_set_current_page (l_notebook1, 4);
        break;


        /*  Save As session  */

      case GDK_a:
        if (state == GDK_CONTROL_MASK) on_save_as1_activate (NULL, NULL);
        break;
      }


    /*  Check modifiers for scene set and clear.  */

    if (scene >= 0)
      {
        switch (state)
          {
          case 0:
            select_scene (scene, 1);
            break;

          case GDK_MOD1_MASK:
            set_scene (scene);
            break;

          case GDK_CONTROL_MASK:
            clear_scene (scene);
            break;
          }
      }


    //fprintf(stderr,"%s %d %x %x %d\n",__FILE__,__LINE__, key, state, scene);

    return FALSE;
}

/* JACK Ports menubar pulldown */
void
on_jack_ports_activate                 (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{

    // fprintf(stderr, "JACK Ports menu selected.\n");
      iomenu_pull_down_ports(menuitem);
}

void
on_out_trim_scale_value_changed        (GtkRange        *range,
                                        gpointer         user_data)
{
    s_set_value_ui(S_OUT_GAIN, gtk_range_get_adjustment(range)->value);
}

gboolean
scene_warning                          (GtkWidget       *widget,
                                        GdkEventButton  *event,
                                        gpointer         user_data)
{
  set_scene_warning_button ();

  return FALSE;
}
