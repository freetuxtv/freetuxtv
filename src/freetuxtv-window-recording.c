/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4-*- */
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
#include <stdlib.h>

#include "freetuxtv-window-recording.h"

#include "freetuxtv-app.h"
#include "freetuxtv-gladexml.h"
#include "freetuxtv-utils.h"


typedef struct _FreetuxTVWindowRecordingPrivate FreetuxTVWindowRecordingPrivate;
struct _FreetuxTVWindowRecordingPrivate
{
	FreetuxTVApp* app;
	GtkBuilder* pBuilder;
};

#define FREETUXTV_WINDOW_RECORDING_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), FREETUXTV_TYPE_WINDOW_RECORDING, FreetuxTVWindowRecordingPrivate))

G_DEFINE_TYPE (FreetuxTVWindowRecording, freetuxtv_window_recording, G_TYPE_OBJECT);

static gboolean
dialog_init (FreetuxTVWindowRecording *pWindowRecording, GtkWindow *parent);

static void
dialog_updateinfos(FreetuxTVWindowRecording *pWindowRecording, GTimeVal* pTimeRef);

static void
on_dialogaddrecording_response (GtkDialog *dialog, gint response_id, gpointer user_data);

static void
on_entry_duration_changed(GtkEditable *self, gpointer user_data);

static void
freetuxtv_window_recording_init (FreetuxTVWindowRecording *object)
{
	FreetuxTVWindowRecordingPrivate* priv;

	priv = FREETUXTV_WINDOW_RECORDING_PRIVATE(object);

	priv->pBuilder = NULL;
}

static void
freetuxtv_window_recording_finalize (GObject *object)
{
	G_OBJECT_CLASS (freetuxtv_window_recording_parent_class)->finalize (object);

	FreetuxTVWindowRecordingPrivate* priv;
	FreetuxTVWindowRecordingClass* klass;

	priv = FREETUXTV_WINDOW_RECORDING_PRIVATE(object);
	klass = FREETUXTV_WINDOW_RECORDING_GET_CLASS(object);

	priv->app = NULL;

	if(priv->pBuilder){
		g_object_unref (priv->pBuilder);
		priv->pBuilder = NULL;
	}
}

static void
freetuxtv_window_recording_class_init (FreetuxTVWindowRecordingClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	//GObjectClass* parent_class = G_OBJECT_CLASS (klass);

	g_type_class_add_private (klass, sizeof (FreetuxTVWindowRecordingPrivate));

	object_class->finalize = freetuxtv_window_recording_finalize;
}

FreetuxTVWindowRecording*
freetuxtv_window_recording_new (GtkWindow *parent, FreetuxTVApp* app)
{
	FreetuxTVWindowRecording* pWindowRecording;
	FreetuxTVWindowRecordingPrivate* priv;

	gchar* szUiFile = NULL;

	pWindowRecording = g_object_new (FREETUXTV_TYPE_WINDOW_RECORDING, NULL);

	priv = FREETUXTV_WINDOW_RECORDING_PRIVATE(pWindowRecording);

	priv->app = app;

	szUiFile = g_build_filename (app->paths.szPathGladeXml, FREETUXTV_GUIFILE_ADDRECORDING, NULL);

	priv->pBuilder = gtk_builder_new ();
	gtk_builder_add_from_file (priv->pBuilder, szUiFile, NULL);

	// Initialize dialog
	dialog_init (pWindowRecording, parent);

	if(szUiFile){
		g_free(szUiFile);
		szUiFile = NULL;
	}

	return pWindowRecording;
}

gint
freetuxtv_window_recording_run (
    FreetuxTVWindowRecording* pWindowRecording,
    FreetuxTVChannelInfos* pChannelInfos,
    GtkTreePath* pPath)
{
	g_return_val_if_fail(pWindowRecording != NULL, GTK_RESPONSE_NONE);
	g_return_val_if_fail(FREETUXTV_IS_WINDOW_RECORDING(pWindowRecording), GTK_RESPONSE_NONE);

	FreetuxTVWindowRecordingPrivate* priv;
	priv = FREETUXTV_WINDOW_RECORDING_PRIVATE(pWindowRecording);

	gint res;

	GtkDialog *dialog;
	dialog = (GtkDialog *) gtk_builder_get_object (priv->pBuilder, FREETUXTV_GUI_DIALOG_ADDRECORDING);

	GtkWidget *widget;
	//	GObject *object;
	
	GTimeVal time_now;
	g_get_current_time (&time_now);

	// Set the value in the fields
	widget = (GtkWidget *) gtk_builder_get_object (priv->pBuilder, "label_channel");
	gtk_label_set_text(GTK_LABEL(widget), pChannelInfos->name);
	
	widget = (GtkWidget *) gtk_builder_get_object (priv->pBuilder, "entry_title");

	gchar* szTmp;
	gchar* szEndText;
	szEndText = g_time_val_to_string(&time_now, "%d/%m/%y %H:%M");
	szTmp = g_strdup_printf("%s - %s", pChannelInfos->name, szEndText);
	gtk_entry_set_text(GTK_ENTRY(widget), szTmp);
	g_free(szTmp);
	g_free(szEndText);

	// Update display
	dialog_updateinfos(pWindowRecording, &time_now);

	// Display the dialog
	res = gtk_dialog_run(dialog);

	return res;
}

static gboolean
dialog_init (FreetuxTVWindowRecording *pWindowRecording, GtkWindow *parent)
{
	gboolean res = TRUE;

	FreetuxTVWindowRecordingPrivate* priv;
	priv = FREETUXTV_WINDOW_RECORDING_PRIVATE(pWindowRecording);

	FreetuxTVWindowRecordingClass* klass;

	GtkWidget *widget;

	klass = FREETUXTV_WINDOW_RECORDING_GET_CLASS(pWindowRecording);

	// Initialize signals for dialog
	widget = (GtkWidget *)gtk_builder_get_object (priv->pBuilder,
	    FREETUXTV_GUI_DIALOG_ADDRECORDING);
	
	// Set the parent
	gtk_window_set_transient_for (GTK_WINDOW(widget), parent);

	gtk_dialog_add_buttons (GTK_DIALOG(widget),
	    "gtk-cancel", GTK_RESPONSE_CANCEL,
	    "gtk-ok", GTK_RESPONSE_OK, NULL);

	g_signal_connect(G_OBJECT(widget),
	    "response",
	    G_CALLBACK(on_dialogaddrecording_response),
	    pWindowRecording);

	g_signal_connect(G_OBJECT(widget),
	    "delete-event",
	    G_CALLBACK(gtk_widget_hide_on_delete),
	    NULL);

	widget =  (GtkWidget *) gtk_builder_get_object (priv->pBuilder,
	    "entry_duration");
	g_signal_connect(G_OBJECT(widget),
	    "changed",
	    G_CALLBACK(on_entry_duration_changed),
	    pWindowRecording);

	return res;
}

static void
dialog_updateinfos(FreetuxTVWindowRecording *pWindowRecording, GTimeVal* pTimeRef)
{
	GtkWidget *widget;
	const gchar* szDurationText;
	int duration;
	gchar* szBeginText = NULL;
	gchar* szEndText = NULL;
	
	FreetuxTVWindowRecordingPrivate* priv;
	priv = FREETUXTV_WINDOW_RECORDING_PRIVATE(pWindowRecording);

	widget =  (GtkWidget *) gtk_builder_get_object (priv->pBuilder,
	    "entry_duration");

	szDurationText = gtk_entry_get_text(GTK_ENTRY(widget));
	
	duration = atoi(szDurationText);
	priv->app->current.recording.max_duration = duration;

	if(pTimeRef){
		szBeginText = g_time_val_to_string(pTimeRef, "%d/%m/%y %H:%M");
		widget =  (GtkWidget *) gtk_builder_get_object (priv->pBuilder,
			"label_begintime");
		gtk_label_set_text(GTK_LABEL(widget), szBeginText);
		
		g_time_val_add_seconds (pTimeRef, duration * 60);
		szEndText = g_time_val_to_string(pTimeRef, "%d/%m/%y %H:%M");
		widget =  (GtkWidget *) gtk_builder_get_object (priv->pBuilder,
			"label_endtime");
		gtk_label_set_text(GTK_LABEL(widget), szEndText);
	}

	if(szBeginText){
		g_free(szBeginText);
		szBeginText = NULL;
	}
	if(szEndText){
		g_free(szEndText);
		szEndText = NULL;
	}
}

static void
on_dialogaddrecording_response (GtkDialog *self, gint response_id, gpointer user_data)
{
	FreetuxTVWindowRecording* pWindowRecording;
	FreetuxTVWindowRecordingPrivate* priv;

	pWindowRecording = (FreetuxTVWindowRecording*)user_data;
	priv = FREETUXTV_WINDOW_RECORDING_PRIVATE(pWindowRecording);

	gtk_widget_hide(GTK_WIDGET(self));
}

static void
on_entry_duration_changed (GtkEditable *self, gpointer user_data)
{
	FreetuxTVWindowRecording* pWindowRecording;
	pWindowRecording = (FreetuxTVWindowRecording*)user_data;

	// Get current hour
	GTimeVal time_now;
	g_get_current_time (&time_now);
	
	dialog_updateinfos(pWindowRecording, &time_now);
}
