/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8-*- */
/*
 * freetuxtv
 * Copyright (C) FreetuxTV Team's 2008
 * Project homepage : http://code.google.com/p/freetuxtv/
 * 
 * freetuxtv is free software.
 * 
 */

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>

#include "freetuxtv-recordings-list.h"

#include "freetuxtv-app.h"

enum
{
	FILENAME_COLUMN,
	MRL_COLUMN,
	N_COLUMNS
};

static void
on_row_activated_recordings_list (GtkTreeView        *view, GtkTreePath *path,
				  GtkTreeViewColumn  *col, gpointer user_data);

void
recordings_list_init(FreetuxTVApp *app)
{
	GtkWidget *widget;
	widget = (GtkWidget *)gtk_builder_get_object (app->gui,
						      "windowmain_treeviewrecordings");
	g_signal_connect(widget, "row-activated", G_CALLBACK(on_row_activated_recordings_list), app);
}

void
recordings_list_load_recordings(FreetuxTVApp *app)
{
	GtkListStore *liststore;
	liststore = (GtkListStore *)gtk_builder_get_object (app->gui,
							    "liststore_recordings");
	gtk_list_store_clear(liststore);
	
	GDir *dir = g_dir_open (app->config.directoryrecordings, 0, NULL);
	if (dir){
		const gchar *filename = NULL;
		gchar *url = NULL;		
		while (filename = g_dir_read_name (dir)) {
			if(g_regex_match_simple(".*.mpg", filename, 0, 0)){
				GtkTreeIter iter;
				url = g_strconcat(app->config.directoryrecordings, "/", filename, NULL);
				gtk_list_store_append(liststore, &iter);
				gtk_list_store_set(liststore, &iter,
						   FILENAME_COLUMN, filename, MRL_COLUMN, url, -1);
			}
		}
		g_dir_close (dir);
	}
}

static void
on_row_activated_recordings_list (GtkTreeView        *view, GtkTreePath *path,
				  GtkTreeViewColumn  *col, gpointer user_data)
{
	FreetuxTVApp *app = (FreetuxTVApp *) user_data;

	GtkTreeModel *model;
	model = gtk_tree_view_get_model (view);
	
	GtkTreeIter iter;
	gtk_tree_model_get_iter (model, &iter, path);
	
	gchar *filename, *mrl;
	gtk_tree_model_get (model, &iter, FILENAME_COLUMN, &filename, MRL_COLUMN, &mrl, -1);

	GtkLibVLCMedia *media;
	media = gtk_libvlc_media_new (mrl);

	freetuxtv_play_media (app, media);
	
	g_object_unref(media);
	
	g_free(filename);
	g_free(mrl);

}
