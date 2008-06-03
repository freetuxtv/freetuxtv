/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8-*- */
/*
 * freetuxtv
 * Copyright (C) FreetuxTV Team's 2008
 * Project homepage : http://code.google.com/p/freetuxtv/
 * 
 * freetuxtv is free software.
 * 
 */

#ifndef FREETUXTV_WINDOW_MAIN_H
#define FREETUXTV_WINDOW_MAIN_H

#include "freetuxtv-channel.h"

void 
freetuxtv_window_main_play_channel (FreetuxTVChannel *channel);

void
on_windowmain_destroy (GtkWidget *widget, gpointer *data);

void
on_windowmain_menuitemquit_activate (GtkMenuItem *menuitem,
				     gpointer user_data);

void
on_windowmain_menuitemaboutdialog_activate (GtkMenuItem *menuitem,
					    gpointer user_data);

void
on_windowmain_buttonclearfilter_clicked (GtkButton *button,
					 gpointer *data);

void
on_windowmain_buttonstop_clicked (GtkButton *button,
				  gpointer *data);

void
on_windowmain_entryfilter_changed (GtkEntry *entry,
				   gpointer user_data);

void
on_windowmain_volumecontrol_value_changed (GtkRange *range,
					   gpointer  user_data);

void
on_windowmain_menuitemgroupsadd_activate (GtkMenuItem *menuitem,
					  gpointer user_data);

void
on_dialogaddgroup_add_clicked (GtkButton *button,
			       gpointer   user_data);
void
on_dialogaddgroup_cancel_clicked (GtkButton *button,
				  gpointer   user_data);

void
on_aboutdialog_response (GtkDialog *dialog,
			 gpointer   user_data);

#endif /* FREETUXTV_WINDOW_MAIN_H */
