/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * freetuxtv
 * Copyright (C) Eric Beuque 2012 <eric.beuque@gmail.com>
 * 
freetuxtv is free software: you can redistribute it and/or modify it
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

#include "freetuxtv-window-tv-channels-database.h"

#include "freetuxtv-gladexml.h"
#include "freetuxtv-db-sync.h"
#include "freetuxtv-channels-list.h"
#include "freetuxtv-tv-channels-list.h"
#include "freetuxtv-window-main.h"

typedef struct _FreetuxTVWindowTVChannelsDatabasePrivate FreetuxTVWindowTVChannelsDatabasePrivate;
struct _FreetuxTVWindowTVChannelsDatabasePrivate
{
	FreetuxTVApp* app;
};

#define FREETUXTV_WINDOW_TV_CHANNELS_DATABASE_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), FREETUXTV_TYPE_WINDOW_TV_CHANNELS_DATABASE, FreetuxTVWindowTVChannelsDatabasePrivate))

G_DEFINE_TYPE (FreetuxTVWindowTVChannelsDatabase, freetuxtv_window_tv_channels_database, GTK_TYPE_BUILDER_WINDOW);

static void
freetuxtv_window_tv_channels_database_init (FreetuxTVWindowTVChannelsDatabase *pWindowTVChannelsDatabase)
{
	
}

static void
freetuxtv_window_tv_channels_database_finalize (GObject *object)
{
	G_OBJECT_CLASS (freetuxtv_window_tv_channels_database_parent_class)->finalize (object);
}

static void
freetuxtv_window_tv_channels_database_class_init (FreetuxTVWindowTVChannelsDatabaseClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	//GtkBuilderDialogClass* parent_class = GTK_BUILDER_DIALOG_CLASS (klass);

	g_type_class_add_private (klass, sizeof (FreetuxTVWindowTVChannelsDatabasePrivate));

	object_class->finalize = freetuxtv_window_tv_channels_database_finalize;
}

static gboolean
do_idle_destroy_window (gpointer user_data)
{
	g_return_val_if_fail(GTK_IS_BUILDER_WIDGET(user_data), FALSE);
	gtk_builder_widget_destroy (GTK_BUILDER_WIDGET(user_data));

	return FALSE;
}

static void
on_buttonclose_clicked (GtkButton *button, gpointer user_data)
{
	// We do the destruction after the button clicked event is finished
	g_idle_add (do_idle_destroy_window, user_data);
}

static void
on_buttonapply_clicked (GtkButton *button, gpointer user_data)
{
	FreetuxTVWindowTVChannelsDatabase* pWindowTVChannelsDatabase;
	FreetuxTVWindowTVChannelsDatabasePrivate* priv;

	pWindowTVChannelsDatabase = (FreetuxTVWindowTVChannelsDatabase*)user_data;
	priv = FREETUXTV_WINDOW_TV_CHANNELS_DATABASE_PRIVATE(pWindowTVChannelsDatabase);

	GError* error = NULL;
	
	GtkBuilder *builder;
	builder = gtk_builder_object_get_builder(GTK_BUILDER_OBJECT(pWindowTVChannelsDatabase));
	
	DBSync dbsync;

	gboolean bSynchronize = FALSE;

	// Check if must synchronize
	GtkWidget* pWidget;
	pWidget = (GtkWidget *) gtk_builder_get_object (builder, "checkbutton_synchronize");

	bSynchronize = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(pWidget));

	// Do synchronize
	if(bSynchronize){
		dbsync_open_db (&dbsync, &error);

		if(error == NULL){
			tvchannels_list_synchronize (priv->app, &dbsync, &error);			
		}

		if(error == NULL){
			channels_list_load_channels (priv->app, &dbsync, &error);
		}

		dbsync_close_db(&dbsync);
	}

	if(error != NULL){
		windowmain_show_gerror (priv->app, error);
		g_error_free (error);
	}
}

FreetuxTVWindowTVChannelsDatabase*
freetuxtv_window_tv_channels_database_new (GtkWindow *parent, FreetuxTVApp* app)
{
	g_return_val_if_fail(parent != NULL, NULL);
	g_return_val_if_fail(GTK_IS_WINDOW(parent), NULL);
	g_return_val_if_fail(app != NULL, NULL);
	
	FreetuxTVWindowTVChannelsDatabase* pWindowTVChannelsDatabase;
	GtkWidget *widget;
	GtkWindow *pWindow;
	
	gchar* szUiFile = NULL;
	szUiFile = g_build_filename (app->paths.szPathGladeXml, FREETUXTV_GUIFILE_TVCHANNELSDATABASE, NULL);
	
	pWindowTVChannelsDatabase = g_object_new (FREETUXTV_TYPE_WINDOW_TV_CHANNELS_DATABASE,
	    "ui-file", szUiFile,
	    "toplevel-widget-name", "dialogtvchannelsdatabase",
	    NULL);

	if(szUiFile){
		g_free(szUiFile);
		szUiFile = NULL;
	}

	// Private members
	FreetuxTVWindowTVChannelsDatabasePrivate* priv;
	priv = FREETUXTV_WINDOW_TV_CHANNELS_DATABASE_PRIVATE(pWindowTVChannelsDatabase);
	priv->app = app;

	pWindow = gtk_builder_window_get_top_window (
	    GTK_BUILDER_WINDOW(pWindowTVChannelsDatabase));
	
	// Set the parent
	gtk_window_set_transient_for (pWindow, parent);
	gtk_window_set_position (pWindow, GTK_WIN_POS_CENTER_ON_PARENT);

	// Signal to connect instance
	widget = (GtkWidget *)gtk_builder_object_get_object (
	    GTK_BUILDER_OBJECT(pWindowTVChannelsDatabase),
	    "button_apply");
	g_signal_connect(G_OBJECT(widget),
		"clicked",
		G_CALLBACK(on_buttonapply_clicked),
		pWindowTVChannelsDatabase);
	
	widget = (GtkWidget *)gtk_builder_object_get_object (
	    GTK_BUILDER_OBJECT(pWindowTVChannelsDatabase),
	    "button_close");
	g_signal_connect(G_OBJECT(widget),
		"clicked",
		G_CALLBACK(on_buttonclose_clicked),
	    pWindowTVChannelsDatabase);

	return pWindowTVChannelsDatabase;
}

