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

typedef enum {
	DIALOG_UPDATE_FROM_INIT,
	DIALOG_UPDATE_FROM_BEGIN,
	DIALOG_UPDATE_FROM_END,
	DIALOG_UPDATE_FROM_DURATION
} DIALOG_UPDATE;

static void
dialog_updateinfos(FreetuxTVWindowRecording *pWindowRecording,
    DIALOG_UPDATE origin, GDateTime* datetime_ref);

static void
on_entry_duration_changed(GtkEditable *self, gpointer user_data);

static void
on_begindatetimepicker_changed (GtkDateTimePicker *self, gpointer user_data);

static void
on_enddatetimepicker_changed (GtkDateTimePicker *self, gpointer user_data);

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
	builder = gtk_builder_object_get_builder(GTK_BUILDER_OBJECT(pWindowRecording));

	// Private members
	FreetuxTVWindowRecordingPrivate* priv;
	priv = FREETUXTV_WINDOW_RECORDING_PRIVATE(pWindowRecording);
	priv->app = app;

	priv->pChannelInfos = pChannelInfos;
	g_object_unref(priv->pChannelInfos);

	GtkWindow* pWindow = gtk_builder_window_get_top_window(GTK_BUILDER_WINDOW(pWindowRecording));
	
	// Set the parent
	gtk_window_set_transient_for (GTK_WINDOW(pWindow), parent);
	gtk_window_set_position (GTK_WINDOW(pWindow), GTK_WIN_POS_CENTER_ON_PARENT);

	gtk_dialog_add_buttons (GTK_DIALOG(pWindow),
	    "gtk-cancel", GTK_RESPONSE_CANCEL,
	    "gtk-ok", GTK_RESPONSE_OK, NULL);

	// Signal to connect instance
	widget =  (GtkWidget *) gtk_builder_get_object (builder,
	    "entry_duration");
	gtk_entry_set_text(GTK_ENTRY(widget), "60"); // 60 minutes by default
	g_signal_connect(G_OBJECT(widget),
	    "changed",
	    G_CALLBACK(on_entry_duration_changed),
	    pWindowRecording);

	// Add the date time picker
	widget = (GtkWidget *) gtk_builder_get_object (builder, "hbox_begintime");
	priv->pWidgetTimeBegin = gtk_date_time_picker_new (I18N_DATE_FORMAT_LONG);
	g_signal_connect(G_OBJECT(priv->pWidgetTimeBegin), "datetime-changed",
	    G_CALLBACK(on_begindatetimepicker_changed),
	    pWindowRecording);
	gtk_box_pack_start(GTK_BOX(widget), priv->pWidgetTimeBegin, FALSE, FALSE, 0);
	gtk_widget_show(priv->pWidgetTimeBegin);
	
	widget = (GtkWidget *) gtk_builder_get_object (builder, "hbox_endtime");
	priv->pWidgetTimeEnd = gtk_date_time_picker_new (I18N_DATE_FORMAT_LONG);
	g_signal_connect(G_OBJECT(priv->pWidgetTimeEnd), "datetime-changed",
	    G_CALLBACK(on_enddatetimepicker_changed),
	    pWindowRecording);
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
	dialog_updateinfos(pWindowRecording, DIALOG_UPDATE_FROM_INIT, datetime_now);

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

	FreetuxTVWindowRecordingPrivate* priv;
	priv = FREETUXTV_WINDOW_RECORDING_PRIVATE(pWindowRecording);
	
	GtkBuilder* builder;
	builder = gtk_builder_object_get_builder(GTK_BUILDER_OBJECT(pWindowRecording));
	
	GtkWidget *widget;

	widget = (GtkWidget *) gtk_builder_get_object (builder, "entry_title");
	szTmp = (gchar*)gtk_entry_get_text(GTK_ENTRY(widget));

	GTimeZone *tz;
	tz = g_time_zone_new_local ();

	GDateTime* datetime_begin;
	GDateTime* datetime_end;

	datetime_begin = gtk_date_time_picker_get_datetime (GTK_DATE_TIME_PICKER(priv->pWidgetTimeBegin), tz);
	datetime_end = gtk_date_time_picker_get_datetime (GTK_DATE_TIME_PICKER(priv->pWidgetTimeEnd), tz);
	
	GTimeVal tval;
	gint64 beginTime;
	gint64 endTime;
	//gint64 now64;

	if(datetime_begin && datetime_end){
		//g_get_current_time (&tval);
		//now64 = g_time_val_to_int64 (&tval);
		g_date_time_to_timeval (datetime_begin, &tval);
		beginTime = g_time_val_to_int64(&tval);
		g_date_time_to_timeval (datetime_end, &tval);
		endTime = g_time_val_to_int64(&tval);
		
		pRecordingInfos = freetuxtv_recording_infos_new(szTmp, beginTime, endTime, priv->pChannelInfos->id);
	}

	if(datetime_begin){
		g_date_time_unref(datetime_begin);
		datetime_begin = NULL;
	}

	if(datetime_end){
		g_date_time_unref(datetime_end);
		datetime_end = NULL;
	}

	if(tz){
		g_time_zone_unref (tz);
		tz = NULL;
	}
	
	return pRecordingInfos;
}

static void
dialog_updateinfos(FreetuxTVWindowRecording *pWindowRecording,
    DIALOG_UPDATE origin, GDateTime *datetime_ref)
{
	GtkWidget *duration_entry;
	const gchar* szDurationText;
	gchar* szNewDurationText = NULL;
	gint duration;
	
	FreetuxTVWindowRecordingPrivate* priv;
	priv = FREETUXTV_WINDOW_RECORDING_PRIVATE(pWindowRecording);

	GtkBuilder* builder;
	builder = gtk_builder_object_get_builder(GTK_BUILDER_OBJECT(pWindowRecording));

	duration_entry =  (GtkWidget *) gtk_builder_get_object (builder,
	    "entry_duration");
	szDurationText = gtk_entry_get_text(GTK_ENTRY(duration_entry));
	duration = atoi(szDurationText);

	GTimeZone *tz;
	GDateTime* datetime_begin = NULL;
	GDateTime* datetime_end = NULL;

	tz = g_time_zone_new_local ();

	switch(origin){
	case DIALOG_UPDATE_FROM_INIT:
		datetime_begin = g_date_time_to_timezone (datetime_ref, tz);
		if(datetime_begin){
			datetime_end =  g_date_time_add_minutes (datetime_begin, duration);
		}
		break;
	case DIALOG_UPDATE_FROM_BEGIN:
	case DIALOG_UPDATE_FROM_DURATION:
		datetime_begin = gtk_date_time_picker_get_datetime (GTK_DATE_TIME_PICKER(priv->pWidgetTimeBegin), tz);
		if(datetime_begin){
			datetime_end = g_date_time_add_minutes (datetime_begin, duration);
		}
		break;
	case DIALOG_UPDATE_FROM_END:
		datetime_begin = gtk_date_time_picker_get_datetime (GTK_DATE_TIME_PICKER(priv->pWidgetTimeBegin), tz);
		if(datetime_begin){
			datetime_end = gtk_date_time_picker_get_datetime (GTK_DATE_TIME_PICKER(priv->pWidgetTimeEnd), tz);
			if(datetime_end){
				GTimeSpan diff = g_date_time_difference (datetime_end, datetime_begin);
				duration = (gint)(diff/(60*G_USEC_PER_SEC)); // GTimeSpan is microseconds and duration is minutes
				szNewDurationText = g_strdup_printf("%d", duration);
			}
		}
		break;
	}

	if(datetime_begin){
		g_signal_handlers_block_by_func (priv->pWidgetTimeBegin,
			(gpointer) on_begindatetimepicker_changed, pWindowRecording);
		gtk_date_time_picker_set_datetime(GTK_DATE_TIME_PICKER(priv->pWidgetTimeBegin), datetime_begin);
		g_signal_handlers_unblock_by_func (priv->pWidgetTimeBegin,
			(gpointer) on_begindatetimepicker_changed, pWindowRecording);
		if(origin == DIALOG_UPDATE_FROM_BEGIN){
			g_signal_stop_emission_by_name (priv->pWidgetTimeBegin, "datetime-changed");
		}
	}

	if(datetime_end){
		g_signal_handlers_block_by_func (priv->pWidgetTimeEnd,
			(gpointer) on_enddatetimepicker_changed, pWindowRecording);
		gtk_date_time_picker_set_datetime(GTK_DATE_TIME_PICKER(priv->pWidgetTimeEnd), datetime_end);
		g_signal_handlers_unblock_by_func (priv->pWidgetTimeEnd,
			(gpointer) on_enddatetimepicker_changed, pWindowRecording);
		if(origin == DIALOG_UPDATE_FROM_END){
			g_signal_stop_emission_by_name (priv->pWidgetTimeEnd, "datetime-changed");
		}
	}

	if(szNewDurationText){
		g_signal_handlers_block_by_func (duration_entry,
			(gpointer) on_entry_duration_changed, pWindowRecording);
		gtk_entry_set_text(GTK_ENTRY(duration_entry), szNewDurationText);
		g_signal_handlers_unblock_by_func (duration_entry,
			(gpointer) on_entry_duration_changed, pWindowRecording);
		g_free(szNewDurationText);
		szNewDurationText = NULL;
	}

	if(datetime_begin){
		g_date_time_unref (datetime_begin);
		datetime_begin = NULL;
	}

	if(datetime_end){
		g_date_time_unref (datetime_end);
		datetime_end = NULL;
	}

	if(tz){
		g_time_zone_unref (tz);
		tz = NULL;
	}
	    
}

static void
on_entry_duration_changed (GtkEditable *self, gpointer user_data)
{
	FreetuxTVWindowRecording* pWindowRecording;
	pWindowRecording = (FreetuxTVWindowRecording*)user_data;
	
	dialog_updateinfos(pWindowRecording, DIALOG_UPDATE_FROM_DURATION, NULL);
}

static void
on_begindatetimepicker_changed (GtkDateTimePicker *self, gpointer user_data)
{
	FreetuxTVWindowRecording* pWindowRecording;
	pWindowRecording = (FreetuxTVWindowRecording*)user_data;

	dialog_updateinfos(pWindowRecording, DIALOG_UPDATE_FROM_BEGIN, NULL);
}

static void
on_enddatetimepicker_changed (GtkDateTimePicker *self, gpointer user_data)
{
	FreetuxTVWindowRecording* pWindowRecording;
	pWindowRecording = (FreetuxTVWindowRecording*)user_data;
	
	dialog_updateinfos(pWindowRecording, DIALOG_UPDATE_FROM_END, NULL);
}