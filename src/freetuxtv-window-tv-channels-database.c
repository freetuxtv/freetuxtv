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

#include "freetuxtv-fileutils.h"

#include "gtk-progress-dialog.h"

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

static void synchronize_progress_cb(gchar* szTVChannelName, void* user_data)
{
	GtkProgressDialog* pProgressDialog = (GtkProgressDialog*)user_data;

	gchar* szTmp = g_strdup_printf(_("Updating TV channel: %s"), szTVChannelName);
	
	gtk_progress_dialog_set_text(pProgressDialog, szTmp);
	gtk_progress_dialog_pulse(pProgressDialog);

	if(szTmp){
		g_free(szTmp);
		szTmp = NULL;
	}
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

	gchar* szTmp = NULL;
	
	gboolean bSynchronize = FALSE;
	gboolean bDownloadFile = FALSE;
	gboolean bUpdateLogos = FALSE;

	const gchar *szDatabaseUrl;
	const gchar *szLogosUrl;
	gchar *szDstFile = NULL;

	GtkWidget* pWidget;
	GtkWindow* pParent;
	
	// Check if must synchronize
	pWidget = (GtkWidget *) gtk_builder_get_object (builder, "checkbutton_synchronize");
	bSynchronize = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(pWidget));
	
	// Check if must update file
	pWidget = (GtkWidget *) gtk_builder_get_object (builder, "checkbutton_download");
	bDownloadFile = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(pWidget));
	pWidget = (GtkWidget *) gtk_builder_get_object (builder, "entry_url");
	szDatabaseUrl = gtk_entry_get_text(GTK_ENTRY(pWidget));

	// Check if must update logos
	pWidget = (GtkWidget *) gtk_builder_get_object (builder, "checkbutton_logos");
	bUpdateLogos = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(pWidget));
	pWidget = (GtkWidget *) gtk_builder_get_object (builder, "entry_logosurl");
	szLogosUrl = gtk_entry_get_text(GTK_ENTRY(pWidget));

	pParent = gtk_builder_window_get_top_window(GTK_BUILDER_WINDOW(pWindowTVChannelsDatabase));

	// Start progress dialog
	GtkProgressDialog* pProgressDialog = NULL;
	gdouble progress = 0.0;
	if(bSynchronize){
		pProgressDialog = gtk_progress_dialog_new(pParent);
		gtk_progress_dialog_set_title(pProgressDialog, _("TV channels synchronisation"));
		gtk_progress_dialog_set_percent(pProgressDialog, progress);
		gtk_widget_show(GTK_WIDGET(pProgressDialog));
	}

	// Do download file
	if(bDownloadFile && (error == NULL)){
		szDstFile = g_build_filename(g_get_user_cache_dir(), "freetuxtv", "tv_channels.dat", NULL);

		szTmp = g_strdup_printf(_("Downloading the file '%s'"), szDatabaseUrl);
		g_log(FREETUXTV_LOG_DOMAIN, G_LOG_LEVEL_INFO,
		    "Downloading the file '%s'\n", szDatabaseUrl);
		
		gtk_progress_dialog_set_text(pProgressDialog, szTmp);
		if(szTmp){
			g_free(szTmp);
			szTmp = NULL;
		}

		freetuxtv_fileutils_get_file (szDatabaseUrl, szDstFile, &(priv->app->prefs.proxy), priv->app->prefs.timeout, &error);
	}

	// Do synchronize
	if(bSynchronize && (error == NULL)){
		gtk_progress_dialog_set_text(pProgressDialog, _("Synchronizing TV channels from file"));
		dbsync_open_db (&dbsync, &error);

		if(error == NULL){
			tvchannels_list_synchronize (priv->app, &dbsync,
			                             (bUpdateLogos ? szLogosUrl : NULL),
			                             synchronize_progress_cb, (void*)pProgressDialog, &error);			
		}

		if(error == NULL){
			channels_list_load_channels (priv->app, &dbsync, &error);
		}

		dbsync_close_db(&dbsync);

		gtk_progress_dialog_set_percent(pProgressDialog, 0.90);
	}

	if(szDstFile){
		g_free(szDstFile);
		szDstFile = NULL;
	}
	
	// On error we destroy the progress dialog view
	if(pProgressDialog){
		if(error != NULL){
			gtk_widget_destroy (GTK_WIDGET(pProgressDialog));
			pProgressDialog = NULL;
		}else{
			gtk_progress_dialog_set_percent(pProgressDialog, 1.0);
			gtk_progress_dialog_set_button_close_enabled(pProgressDialog, TRUE);
		}
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

