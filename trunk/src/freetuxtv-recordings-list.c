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
	
	GDir *dir = g_dir_open (app->prefs.directoryrecordings, 0, NULL);
	if (dir){
		const gchar *filename = NULL;
		gchar *url = NULL;		
		while (filename = g_dir_read_name (dir)) {
			if(g_regex_match_simple("^.*\\.(ts|ogg|mpg|avi|mp4)$", filename, G_REGEX_CASELESS, 0)){
				GtkTreeIter iter;
				url = g_strconcat(app->prefs.directoryrecordings, "/", filename, NULL);
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

	GtkLibvlcMedia *media;
	media = gtk_libvlc_media_new (mrl);

	freetuxtv_play_media (app, media);
	
	g_object_unref(media);
	
	g_free(filename);
	g_free(mrl);

}
