/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8-*- */
/*
 * FreetuxTV is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or 
 * (at your option) any later version.
 *
 * FreetuxTV is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Glade; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>

#include "freetuxtv-window-recording.h"

#include "freetuxtv-app.h"

static void
on_windowrecording_response(GtkDialog *dialog,
			    gint response_id,
			    gpointer user_data);

static void
on_windowrecording_entryduration_changed(GtkEditable *editable,
					 gpointer     user_data);

void
windowrecording_init(FreetuxTVApp *app)
{
	GtkWidget *widget;
	
	// Initialize signals for window recording
	widget =  (GtkWidget *) gtk_builder_get_object (app->gui,
							"dialogrecording");
	
	gtk_dialog_add_buttons (GTK_DIALOG(widget),
				"gtk-cancel", GTK_RESPONSE_CANCEL,
				"gtk-ok", GTK_RESPONSE_OK, NULL);
	
	g_signal_connect(G_OBJECT(widget),
			 "response",
			 G_CALLBACK(on_windowrecording_response),
			 app);
	g_signal_connect(G_OBJECT(widget),
			 "delete-event",
			 G_CALLBACK(gtk_widget_hide_on_delete),
			 NULL);
	
	widget =  (GtkWidget *) gtk_builder_get_object (app->gui,
							"dialogrecording_entryduration");
	g_signal_connect(G_OBJECT(widget),
			 "changed",
			 G_CALLBACK(on_windowrecording_entryduration_changed),
			 app);
}

void
windowrecording_updateinfos(FreetuxTVApp *app)
{
	
	GtkWidget *widget;
	GTimeVal time_now;
	const gchar* duration_text;
	int duration;
	gchar* begin_text;
	gchar* end_text;
	int add_time;

	// Get current hour
	g_get_current_time (&time_now);
	
	widget =  (GtkWidget *) gtk_builder_get_object (app->gui,
							"dialogrecording_entryduration");

	duration_text = gtk_entry_get_text(GTK_ENTRY(widget));
	duration = atoi(duration_text);

	app->current.recording.max_duration = duration;

	begin_text = g_time_val_to_string(&time_now, "%T");
	g_time_val_add_seconds (&time_now, duration * 60);
	end_text = g_time_val_to_string(&time_now, "%d/%m/%y %H:%M");
	
	widget =  (GtkWidget *) gtk_builder_get_object (app->gui,
							"dialogrecording_labelendtime");

	gtk_label_set_text(GTK_LABEL(widget), end_text);

	g_free(begin_text);
	g_free(end_text);
	
}

static void
on_windowrecording_response(GtkDialog *dialog,
			    gint response_id,
			    gpointer user_data)
{	
	FreetuxTVApp *app = (FreetuxTVApp*)user_data;
	GtkWidget *widget;
	
	widget =  (GtkWidget *) gtk_builder_get_object (app->gui,
							"dialogrecording");
	gtk_widget_hide(GTK_WIDGET(dialog));
}


static void
on_windowrecording_entryduration_changed(GtkEditable *editable,
				   gpointer     user_data)
{
	FreetuxTVApp *app = (FreetuxTVApp*)user_data;
	windowrecording_updateinfos(app);
}
