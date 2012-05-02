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
#include "freetuxtv-gladexml.h"

typedef struct _FreetuxTVWindowChannelPropertiesPrivate FreetuxTVWindowChannelPropertiesPrivate;
struct _FreetuxTVWindowChannelPropertiesPrivate
{
	FreetuxTVApp* app;
	FreetuxTVChannelInfos* pChannelInfos;
};

#define FREETUXTV_WINDOW_CHANNEL_PROPERTIES_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), FREETUXTV_TYPE_WINDOW_CHANNEL_PROPERTIES, FreetuxTVWindowChannelPropertiesPrivate))

G_DEFINE_TYPE (FreetuxTVWindowChannelProperties, freetuxtv_window_channel_properties, GTK_TYPE_BUILDER_DIALOG);

static void
dialog_updateinfos(FreetuxTVWindowChannelProperties *pWindowChannelProperties, FreetuxTVChannelInfos* pChannelInfos);

static void
freetuxtv_window_channel_properties_init (FreetuxTVWindowChannelProperties *object)
{
	FreetuxTVWindowChannelPropertiesPrivate* priv;
	
	priv = FREETUXTV_WINDOW_CHANNEL_PROPERTIES_PRIVATE(object);
	priv->app = NULL;
	priv->pChannelInfos = NULL;
}

static void
freetuxtv_window_channel_properties_finalize (GObject *object)
{
	FreetuxTVWindowChannelPropertiesPrivate* priv;
	
	priv = FREETUXTV_WINDOW_CHANNEL_PROPERTIES_PRIVATE(object);
	priv->app = NULL;

	if(priv->pChannelInfos){
		g_object_unref(priv->pChannelInfos);
		priv->pChannelInfos = NULL;
	}
	
	G_OBJECT_CLASS (freetuxtv_window_channel_properties_parent_class)->finalize (object);
}

static void
freetuxtv_window_channel_properties_class_init (FreetuxTVWindowChannelPropertiesClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);

	g_type_class_add_private (klass, sizeof (FreetuxTVWindowChannelPropertiesPrivate));

	object_class->finalize = freetuxtv_window_channel_properties_finalize;
}


FreetuxTVWindowChannelProperties*
freetuxtv_window_channel_properties_new (GtkWindow *parent, FreetuxTVApp* app, FreetuxTVChannelInfos* pChannelInfos)
{
	g_return_val_if_fail(parent != NULL, NULL);
	g_return_val_if_fail(GTK_IS_WINDOW(parent), NULL);
	g_return_val_if_fail(app != NULL, NULL);
	
	FreetuxTVWindowChannelProperties* pWindowChannelProperties;
	
	gchar* szUiFile = NULL;
	szUiFile = g_build_filename (app->paths.szPathGladeXml, FREETUXTV_GUIFILE_CHANNELPROPERTIES, NULL);
	
	pWindowChannelProperties = g_object_new (FREETUXTV_TYPE_WINDOW_CHANNEL_PROPERTIES,
	    "ui-file", szUiFile,
	    "toplevel-widget-name", "dialogchannelproperties",
	    NULL);

	if(szUiFile){
		g_free(szUiFile);
		szUiFile = NULL;
	}

	// Private members
	FreetuxTVWindowChannelPropertiesPrivate* priv;
	priv = FREETUXTV_WINDOW_CHANNEL_PROPERTIES_PRIVATE(pWindowChannelProperties);
	priv->app = app;

	GtkWindow* pWindow = gtk_builder_window_get_top_window(GTK_BUILDER_WINDOW(pWindowChannelProperties));
	gtk_dialog_add_buttons (GTK_DIALOG(pWindow),
	    "gtk-close", GTK_RESPONSE_CLOSE, NULL);

	// Update from channel
	dialog_updateinfos (pWindowChannelProperties, pChannelInfos);

	return pWindowChannelProperties;
}


static void
dialog_updateinfos(FreetuxTVWindowChannelProperties *pWindowChannelProperties, FreetuxTVChannelInfos* pChannelInfos)
{
	GtkBuilder* builder;
	builder = gtk_builder_object_get_builder(GTK_BUILDER_OBJECT(pWindowChannelProperties));

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
	widget = (GtkWidget *) gtk_builder_get_object (builder, "entry_channelname");
	gtk_entry_set_text(GTK_ENTRY(widget), pChannelInfos->name);
	if(!editable){
#if GTK_API_VERSION == 3
		gtk_editable_set_editable (GTK_EDITABLE(widget), FALSE);
#else
		gtk_entry_set_editable (GTK_ENTRY(widget), FALSE);
#endif
	}

	widget = (GtkWidget *) gtk_builder_get_object (builder, "entry_channeluri");
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

	widget = (GtkWidget *) gtk_builder_get_object (builder, "label_channeldeinterlace");
	if(pChannelInfos->deinterlace_mode){
		gtk_label_set_text(GTK_LABEL(widget), pChannelInfos->deinterlace_mode);
	}else{
		gtk_label_set_text(GTK_LABEL(widget), _("none"));
	}
}
