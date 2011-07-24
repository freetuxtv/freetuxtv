/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * freetuxtv
 * Copyright (C) Eric Beuque 2010 <eric.beuque@gmail.com>
 * 
 * freetuxtv is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * freetuxtv is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "freetuxtv-window-channel-properties.h"

#include "freetuxtv-i18n.h"

typedef struct _FreetuxTVWindowChannelPropertiesPrivate FreetuxTVWindowChannelPropertiesPrivate;
struct _FreetuxTVWindowChannelPropertiesPrivate
{
	FreetuxTVApp* app;
};

#define FREETUXTV_WINDOW_CHANNEL_PROPERTIES_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), FREETUXTV_TYPE_WINDOW_CHANNEL_PROPERTIES, FreetuxTVWindowChannelPropertiesPrivate))

G_DEFINE_TYPE (FreetuxTVWindowChannelProperties, freetuxtv_window_channel_properties, G_TYPE_OBJECT);

static gboolean
dialog_init (FreetuxTVWindowChannelProperties *pWindowChannelProperties);

static void
on_dialog_response (GtkDialog *dialog, gint response_id, gpointer user_data);

static void
freetuxtv_window_channel_properties_init (FreetuxTVWindowChannelProperties *object)
{
	
}

static void
freetuxtv_window_channel_properties_finalize (GObject *object)
{
	G_OBJECT_CLASS (freetuxtv_window_channel_properties_parent_class)->finalize (object);

	FreetuxTVWindowChannelPropertiesPrivate* priv;
	FreetuxTVWindowChannelPropertiesClass* klass;
	
	priv = FREETUXTV_WINDOW_CHANNEL_PROPERTIES_PRIVATE(object);
	klass = FREETUXTV_WINDOW_CHANNEL_PROPERTIES_GET_CLASS(object);
	
	GtkWidget* widget;
	
	if(klass->on_dialog_response_hid != -1){
		widget = (GtkWidget *) gtk_builder_get_object (priv->app->gui, "dialogchannelproperties");
		g_signal_handler_disconnect (widget, klass->on_dialog_response_hid);
		klass->on_dialog_response_hid = -1;
	}

	priv->app = NULL;
}

static void
freetuxtv_window_channel_properties_class_init (FreetuxTVWindowChannelPropertiesClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);

	g_type_class_add_private (klass, sizeof (FreetuxTVWindowChannelPropertiesPrivate));

	object_class->finalize = freetuxtv_window_channel_properties_finalize;

	klass->initialized = FALSE;
	klass->on_dialog_response_hid = -1;
}


FreetuxTVWindowChannelProperties*
freetuxtv_window_channel_properties_new (FreetuxTVApp* app)
{
	FreetuxTVWindowChannelProperties* pWindowChannelProperties;
	FreetuxTVWindowChannelPropertiesPrivate* priv;
	
	pWindowChannelProperties = g_object_new (FREETUXTV_TYPE_WINDOW_CHANNEL_PROPERTIES, NULL);
	
	priv = FREETUXTV_WINDOW_CHANNEL_PROPERTIES_PRIVATE(pWindowChannelProperties);

	priv->app = app;

	dialog_init (pWindowChannelProperties);

	return pWindowChannelProperties;
}

gint
freetuxtv_window_channel_properties_run (
	FreetuxTVWindowChannelProperties* pWindowChannelProperties,
    FreetuxTVChannelInfos* pChannelInfos,
    GtkTreePath* pPath)
{
	g_return_val_if_fail(pWindowChannelProperties != NULL, GTK_RESPONSE_NONE);
	g_return_val_if_fail(FREETUXTV_IS_WINDOW_CHANNEL_PROPERTIES(pWindowChannelProperties), GTK_RESPONSE_NONE);

	FreetuxTVWindowChannelPropertiesPrivate* priv;
	priv = FREETUXTV_WINDOW_CHANNEL_PROPERTIES_PRIVATE(pWindowChannelProperties);
	
	GtkBuilder *builder;
	builder = priv->app->gui;

	gint res;

	GtkDialog *dialog;
	dialog = (GtkDialog *) gtk_builder_get_object (builder, "dialogchannelproperties");

	GtkWidget *widget;
	GObject *object;

	gboolean editable = FALSE;

	/*
	// Disable editing for now
	switch(pChannelInfos->channels_group->type){
	case FREETUXTV_CHANNELSGROUP_TYPEGROUP_FAVORITES :
		editable = TRUE;
		break;
	default :
		editable = FALSE;
	}
	*/
	
	// Set the value in the fields
	widget = (GtkWidget *) gtk_builder_get_object (builder, "dialogchannelproperties_name");
	gtk_entry_set_text(GTK_ENTRY(widget), pChannelInfos->name);
	if(!editable){
#if GTK_API_VERSION == 3
		gtk_editable_set_editable (GTK_EDITABLE(widget), FALSE);
#else
		gtk_entry_set_editable (GTK_ENTRY(widget), FALSE);
#endif
	}

	widget = (GtkWidget *) gtk_builder_get_object (builder, "dialogchannelproperties_uri");
	gtk_entry_set_text(GTK_ENTRY(widget), pChannelInfos->url);
	if(!editable){
#if GTK_API_VERSION == 3
		gtk_editable_set_editable (GTK_EDITABLE(widget), FALSE);
#else
		gtk_entry_set_editable (GTK_ENTRY(widget), FALSE);
#endif
	}

	// Display the VLC options of the channel
	object = gtk_builder_get_object (builder, "textbuffer_channelvlcoptions");
	gchar *text;
	if(pChannelInfos->vlc_options){
		text = g_strjoinv("\n", pChannelInfos->vlc_options);
	}else{
		text = g_strdup("");
	}
	gtk_text_buffer_set_text (GTK_TEXT_BUFFER(object), text, -1);
	g_free(text);
	if(!editable){
		widget = (GtkWidget *) gtk_builder_get_object (builder, "textview_channelvlcoptions");
		gtk_text_view_set_editable (GTK_TEXT_VIEW(widget), FALSE);
	}

	widget = (GtkWidget *) gtk_builder_get_object (builder, "dialogchannelproperties_deinterlace");
	if(pChannelInfos->deinterlace_mode){
		gtk_label_set_text(GTK_LABEL(widget), pChannelInfos->deinterlace_mode);
	}else{
		gtk_label_set_text(GTK_LABEL(widget), _("none"));
	}

	// Display the dialog
	res = gtk_dialog_run(dialog);

	return res;
}

static gboolean
dialog_init (FreetuxTVWindowChannelProperties *pWindowChannelProperties)
{
	gboolean res = TRUE;

	FreetuxTVWindowChannelPropertiesPrivate* priv;
	priv = FREETUXTV_WINDOW_CHANNEL_PROPERTIES_PRIVATE(pWindowChannelProperties);

	FreetuxTVWindowChannelPropertiesClass* klass;
	
	GtkDialog *dialog;
	GtkWidget *widget;

	GtkBuilder *builder;
	builder = priv->app->gui;

	klass = FREETUXTV_WINDOW_CHANNEL_PROPERTIES_GET_CLASS(pWindowChannelProperties);

	dialog = (GtkDialog *) gtk_builder_get_object (builder,
	    "dialogchannelproperties");
	
	if(!klass->initialized){
		// Initialize signals for dialogchannelproperties
		widget = (GtkWidget *)gtk_builder_get_object (builder,
		    "dialogchannelproperties");

		gtk_dialog_add_buttons (GTK_DIALOG(widget),
		    "gtk-close", GTK_RESPONSE_CLOSE, NULL);
		/*
		gtk_dialog_add_buttons (GTK_DIALOG(widget),
		    "gtk-cancel", GTK_RESPONSE_CANCEL,
		    "gtk-apply", GTK_RESPONSE_APPLY, NULL);
		*/
		
		g_signal_connect(G_OBJECT(widget),
		    "delete-event", G_CALLBACK(gtk_widget_hide_on_delete),
		    NULL);
		
		klass->initialized = TRUE;
	}
	
	klass->on_dialog_response_hid = g_signal_connect(G_OBJECT(dialog),
	    "response",
	    G_CALLBACK(on_dialog_response),
	    pWindowChannelProperties);

	return res;
}

static void
on_dialog_response (GtkDialog *dialog, gint response_id, gpointer user_data)
{
	gtk_widget_hide(GTK_WIDGET(dialog));
}