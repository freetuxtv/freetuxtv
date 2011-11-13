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
#include "gtk-date-time-picker.h"

#include "freetuxtv-window-add-recording.h"

#include "freetuxtv-app.h"
#include "freetuxtv-gladexml.h"
#include "freetuxtv-utils.h"
#include "freetuxtv-i18n.h"


typedef struct _FreetuxTVWindowRecordingPrivate FreetuxTVWindowRecordingPrivate;
struct _FreetuxTVWindowRecordingPrivate
{
	FreetuxTVApp* app;
	FreetuxTVChannelInfos* pChannelInfos;

	GtkWidget* pWidgetTimeBegin;
	GtkWidget* pWidgetTimeEnd;
};

#define FREETUXTV_WINDOW_RECORDING_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), FREETUXTV_TYPE_WINDOW_RECORDING, FreetuxTVWindowRecordingPrivate))

G_DEFINE_TYPE (FreetuxTVWindowRecording, freetuxtv_window_recording, GTK_TYPE_BUILDER_DIALOG);

static void
dialog_updateinfos(FreetuxTVWindowRecording *pWindowRecording, GDateTime* datetime_ref);

static void
on_entry_duration_changed(GtkEditable *self, gpointer user_data);

static void
freetuxtv_window_recording_init (FreetuxTVWindowRecording *object)
{
	FreetuxTVWindowRecordingPrivate* priv;

	priv = FREETUXTV_WINDOW_RECORDING_PRIVATE(object);

	priv->app = NULL;
	priv->pChannelInfos = NULL;

	priv->pWidgetTimeBegin = NULL;
	priv->pWidgetTimeEnd = NULL;
}

static void
freetuxtv_window_recording_finalize (GObject *object)
{
	FreetuxTVWindowRecordingPrivate* priv;

	priv = FREETUXTV_WINDOW_RECORDING_PRIVATE(object);

	priv->app = NULL;

	if(priv->pChannelInfos){
		g_object_unref(priv->pChannelInfos);
		priv->pChannelInfos = NULL;
	}
	
	G_OBJECT_CLASS (freetuxtv_window_recording_parent_class)->finalize (object);
}

static void
freetuxtv_window_recording_class_init (FreetuxTVWindowRecordingClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);

	g_type_class_add_private (klass, sizeof (FreetuxTVWindowRecordingPrivate));

	object_class->finalize = freetuxtv_window_recording_finalize;
}

FreetuxTVWindowRecording*
freetuxtv_window_recording_new (GtkWindow *parent, FreetuxTVApp* app, FreetuxTVChannelInfos* pChannelInfos)
{
	g_return_val_if_fail(parent != NULL, NULL);
	g_return_val_if_fail(GTK_IS_WINDOW(parent), NULL);
	g_return_val_if_fail(app != NULL, NULL);
	
	FreetuxTVWindowRecording* pWindowRecording;
	GtkWidget *widget;
	
	gchar* szUiFile = NULL;
	szUiFile = g_build_filename (app->paths.szPathGladeXml, FREETUXTV_GUIFILE_ADDRECORDING, NULL);
	
	pWindowRecording = g_object_new (FREETUXTV_TYPE_WINDOW_RECORDING,
	    "ui-file", szUiFile,
	    "toplevel-widget-name", "dialogaddrecording",
	    NULL);

	if(szUiFile){
		g_free(szUiFile);
		szUiFile = NULL;
	}

	GtkBuilder* builder;
	builder = gtk_builder_dialog_get_builder(GTK_BUILDER_DIALOG(pWindowRecording));

	// Private members
	FreetuxTVWindowRecordingPrivate* priv;
	priv = FREETUXTV_WINDOW_RECORDING_PRIVATE(pWindowRecording);
	priv->app = app;

	priv->pChannelInfos = pChannelInfos;
	g_object_unref(priv->pChannelInfos);
	
	// Set the parent
	gtk_window_set_transient_for (GTK_WINDOW(pWindowRecording), parent);
	gtk_window_set_position (GTK_WINDOW(pWindowRecording), GTK_WIN_POS_CENTER_ON_PARENT);

	gtk_dialog_add_buttons (GTK_DIALOG(pWindowRecording),
	    "gtk-cancel", GTK_RESPONSE_CANCEL,
	    "gtk-ok", GTK_RESPONSE_OK, NULL);

	// Signal to connect instance
	widget =  (GtkWidget *) gtk_builder_get_object (builder,
	    "entry_duration");
	g_signal_connect(G_OBJECT(widget),
	    "changed",
	    G_CALLBACK(on_entry_duration_changed),
	    pWindowRecording);

	// Add the date time picker
	widget = (GtkWidget *) gtk_builder_get_object (builder, "hbox_begintime");
	priv->pWidgetTimeBegin = gtk_date_time_picker_new (I18N_DATE_FORMAT_LONG);
	gtk_box_pack_start(GTK_BOX(widget), priv->pWidgetTimeBegin, FALSE, FALSE, 0);
	gtk_widget_show(priv->pWidgetTimeBegin);
	
	widget = (GtkWidget *) gtk_builder_get_object (builder, "hbox_endtime");
	priv->pWidgetTimeEnd = gtk_date_time_picker_new (I18N_DATE_FORMAT_LONG);
	gtk_box_pack_start(GTK_BOX(widget), priv->pWidgetTimeEnd, FALSE, FALSE, 0);
	gtk_widget_show(priv->pWidgetTimeEnd);

	// Set default value in the fields
	GDateTime *datetime_now = g_date_time_new_now_local();

	widget = (GtkWidget *) gtk_builder_get_object (builder, "label_channel");
	gtk_label_set_text(GTK_LABEL(widget), pChannelInfos->name);
	
	widget = (GtkWidget *) gtk_builder_get_object (builder, "entry_title");

	gchar* szTmp;
	gchar* szEndText;
	szEndText = g_date_time_format(datetime_now, I18N_DATETIME_FORMAT_SHORT);
	szTmp = g_strdup_printf("%s - %s", pChannelInfos->name, szEndText);
	gtk_entry_set_text(GTK_ENTRY(widget), szTmp);
	g_free(szTmp);
	g_free(szEndText);

	// Update display
	dialog_updateinfos(pWindowRecording, datetime_now);

	g_date_time_unref (datetime_now);
	datetime_now = NULL;

	return pWindowRecording;
}

FreetuxTVRecordingInfos*
freetuxtv_window_recording_get_recording_infos(FreetuxTVWindowRecording* pWindowRecording)
{
	g_return_val_if_fail(pWindowRecording != NULL, NULL);
	g_return_val_if_fail(FREETUXTV_IS_WINDOW_RECORDING(pWindowRecording), NULL);

	FreetuxTVRecordingInfos* pRecordingInfos;
	
	gchar* szTmp;
	int duration;

	FreetuxTVWindowRecordingPrivate* priv;
	priv = FREETUXTV_WINDOW_RECORDING_PRIVATE(pWindowRecording);
	
	GtkBuilder* builder;
	builder = gtk_builder_dialog_get_builder(GTK_BUILDER_DIALOG(pWindowRecording));
	
	GtkWidget *widget;
	widget =  (GtkWidget *) gtk_builder_get_object (builder,
	    "entry_duration");
	szTmp = (gchar*)gtk_entry_get_text(GTK_ENTRY(widget));
	duration = atoi(szTmp);

	widget = (GtkWidget *) gtk_builder_get_object (builder, "entry_title");
	szTmp = (gchar*)gtk_entry_get_text(GTK_ENTRY(widget));

	GTimeVal now;
	gint64 beginTime;
	gint64 endTime;
	g_get_current_time (&now);
	beginTime = g_time_val_to_int64(&now);
	endTime = beginTime;
	g_time_int64_add_seconds (&endTime, duration * 60);
	
    pRecordingInfos = freetuxtv_recording_infos_new(szTmp, beginTime, endTime, priv->pChannelInfos->id);

	return pRecordingInfos;
}

static void
dialog_updateinfos(FreetuxTVWindowRecording *pWindowRecording, GDateTime *datetime_ref)
{
	GtkWidget *widget;
	const gchar* szDurationText;
	int duration;
	gchar* szBeginText = NULL;
	gchar* szEndText = NULL;
	
	FreetuxTVWindowRecordingPrivate* priv;
	priv = FREETUXTV_WINDOW_RECORDING_PRIVATE(pWindowRecording);

	GtkBuilder* builder;
	builder = gtk_builder_dialog_get_builder(GTK_BUILDER_DIALOG(pWindowRecording));	

	widget =  (GtkWidget *) gtk_builder_get_object (builder,
	    "entry_duration");

	szDurationText = gtk_entry_get_text(GTK_ENTRY(widget));
	
	duration = atoi(szDurationText);
	priv->app->current.recording.max_duration = duration;
	
	gtk_date_time_picker_set_datetime(GTK_DATE_TIME_PICKER(priv->pWidgetTimeBegin), datetime_ref);

	GDateTime* datetime_end;
	datetime_end = g_date_time_add_minutes (datetime_ref, duration);
	gtk_date_time_picker_set_datetime(GTK_DATE_TIME_PICKER(priv->pWidgetTimeEnd), datetime_end);
	g_date_time_unref (datetime_end);
	datetime_end = NULL;

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
on_entry_duration_changed (GtkEditable *self, gpointer user_data)
{
	FreetuxTVWindowRecording* pWindowRecording;
	pWindowRecording = (FreetuxTVWindowRecording*)user_data;

	// Get current hour
	GDateTime *datetime_now = g_date_time_new_now_local();
	
	dialog_updateinfos(pWindowRecording, datetime_now);
	
	g_date_time_unref (datetime_now);
	datetime_now = NULL;
}
