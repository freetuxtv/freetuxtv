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

typedef enum {
	WINDOW_MODE_STOPPED,
	WINDOW_MODE_PLAYING,
	WINDOW_MODE_PAUSED,
	WINDOW_MODE_RECORDING
} FreetuxTVWindowMode;

gboolean
on_windowmain_deleteevent (GtkWidget *widget, GdkEvent *event, gpointer *data);

void
on_windowmain_menuitempreferences_activate (GtkMenuItem *menuitem,
					    gpointer user_data);

void
on_windowmain_menuitemgroupsadd_activate (GtkMenuItem *menuitem,
					  gpointer user_data);

void
on_windowmain_menuitemupdatelogos_activate (GtkMenuItem *menuitem,
					    gpointer user_data);

void
on_windowmain_menuitemquit_activate (GtkMenuItem *menuitem,
				     gpointer user_data);

void
on_windowmain_menuitemaboutdialog_activate (GtkMenuItem *menuitem,
					    gpointer user_data);

void
on_windowmain_buttonclearfilter_clicked (GtkButton *button,
					 gpointer user_data);

void
on_windowmain_buttonprevious_clicked (GtkButton *button,
				      gpointer user_data);

void
on_windowmain_buttonnext_clicked (GtkButton *button,
				  gpointer user_data);

void
on_windowmain_buttonstop_clicked (GtkButton *button,
				  gpointer user_data);

void
on_windowmain_buttonrecord_clicked (GtkButton *button,
				    gpointer user_data);

void
on_windowmain_buttonplaypause_clicked (GtkButton *button,
				  gpointer user_data);

void
on_windowmain_buttonfullscreen_clicked (GtkButton *button,
					gpointer user_data);

void
on_windowmain_buttonminimode_clicked (GtkButton *button,
				      gpointer user_data);

void
on_windowmain_entryfilter_changed (GtkEntry *entry,
				   gpointer user_data);

void
on_windowmain_volumecontrol_value_changed (GtkRange *range,
					   gpointer user_data);

void
on_windowminimode_buttonnormalmode_clicked (GtkButton *button,
					    gpointer user_data);

void
on_windowminimode_buttonstayontop_clicked (GtkButton *button,
				       gpointer user_data);

void
on_dialogaddgroup_add_clicked (GtkButton *button,
			       gpointer   user_data);
void
on_dialogaddgroup_cancel_clicked (GtkButton *button,
				  gpointer user_data);

void
on_aboutdialog_response (GtkDialog *dialog,
			 gint response_id,
			 gpointer user_data);

void
on_dialogpreferences_response (GtkDialog *dialog,
			       gint response_id,
			       gpointer   user_data);

void
windowmain_display_buttons (FreetuxTVApp *app, FreetuxTVWindowMode mode);

void
windowmain_show_error (FreetuxTVApp *app, gchar *msg);

void
windowmain_statusbar_push (FreetuxTVApp *app, gchar *context, gchar *msg);

void
windowmain_statusbar_pop (FreetuxTVApp *app, gchar *context);

void
windowminimode_set_from_config (FreetuxTVApp *app);

#endif /* FREETUXTV_WINDOW_MAIN_H */
