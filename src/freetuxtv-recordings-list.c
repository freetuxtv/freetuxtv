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

#include "freetuxtv-recordings-list.h"

#include "freetuxtv-app.h"
#include "freetuxtv-recording-infos.h"
#include "freetuxtv-cellrenderer-recordingslist.h"
#include "freetuxtv-tv-channels-list.h"
#include "freetuxtv-window-main.h"
#include "freetuxtv-utils.h"

enum
{
	RECORDING_COLUMN,
	N_COLUMNS
};

static gboolean
on_filter_recording_list_in_progress (GtkTreeModel *model, GtkTreeIter *iter, gpointer data);

static gboolean
on_filter_recording_list_terminated (GtkTreeModel *model, GtkTreeIter *iter, gpointer data);

static void
on_row_activated_recordings_list (GtkTreeView *view, GtkTreePath *path,
    GtkTreeViewColumn  *col, gpointer user_data);

static int
on_dbsync_add_recording (FreetuxTVApp *app,
    FreetuxTVRecordingInfos* pRecordingInfos,
    DBSync *dbsync, gpointer user_data, GError** error);

static void 
on_row_displayed_recordings_list(GtkTreeViewColumn *col,
    GtkCellRenderer *renderer, GtkTreeModel *model,
    GtkTreeIter *iter, gpointer user_data);

void
recordings_list_init(FreetuxTVApp *app)
{
	GtkWidget *widget;
	GtkCellRenderer *renderer;
	GtkTreeViewColumn *column;
	GtkTreeModel * filter;

	GtkListStore *liststore;
	liststore = gtk_list_store_new (N_COLUMNS, FREETUXTV_TYPE_RECORDING_INFOS);
	app->pRecordingList = GTK_TREE_MODEL(liststore);
	
	// Set the in progress recordings list view
	widget = (GtkWidget *)gtk_builder_get_object (app->gui,
	    "windowmain_treeviewrecordings");
	filter = gtk_tree_model_filter_new (app->pRecordingList, NULL);
	gtk_tree_model_filter_set_visible_func (GTK_TREE_MODEL_FILTER(filter),
	    on_filter_recording_list_in_progress, (gpointer)app, NULL);
	gtk_tree_view_set_model (GTK_TREE_VIEW(widget), filter);
	g_signal_connect(widget, "row-activated",
	    G_CALLBACK(on_row_activated_recordings_list), app);

	column = gtk_tree_view_column_new();
	renderer = freetuxtv_cellrenderer_recordingslist_new ();
	gtk_tree_view_column_pack_start(column, renderer, FALSE);
	gtk_tree_view_column_set_cell_data_func(column, renderer,
	    on_row_displayed_recordings_list,
	    (gpointer)app, NULL);	
	gtk_tree_view_append_column (GTK_TREE_VIEW (widget), column);

	// Set the in progress recordings list view
	widget = (GtkWidget *)gtk_builder_get_object (app->gui,
	    "windowmain_treeviewfinishedrecordings");
	filter = gtk_tree_model_filter_new (app->pRecordingList, NULL);
	gtk_tree_model_filter_set_visible_func (GTK_TREE_MODEL_FILTER(filter),
	    on_filter_recording_list_terminated, (gpointer)app, NULL);
	gtk_tree_view_set_model (GTK_TREE_VIEW(widget), filter);
	g_signal_connect(widget, "row-activated",
	    G_CALLBACK(on_row_activated_recordings_list), app);
	
	column = gtk_tree_view_column_new();
	renderer = freetuxtv_cellrenderer_recordingslist_new ();
	gtk_tree_view_column_pack_start(column, renderer, FALSE);
	gtk_tree_view_column_set_cell_data_func(column, renderer,
	    on_row_displayed_recordings_list,
	    (gpointer)app, NULL);	
	gtk_tree_view_append_column (GTK_TREE_VIEW (widget), column);
}

void
recordings_list_load_recordings(FreetuxTVApp *app, DBSync* dbsync, GError** error)
{
	// Loads recordings from database
	GtkTreeIter iter;

	GtkListStore *pListStore;
	pListStore = GTK_LIST_STORE(app->pRecordingList);
	gtk_list_store_clear (pListStore);

	dbsync_select_recordings (dbsync, app, on_dbsync_add_recording, &iter, error);
}

void
recordings_list_refresh (FreetuxTVApp *app)
{
	GtkWidget *treeview;
	GtkTreeModel *model;
	treeview =  (GtkWidget *) gtk_builder_get_object (app->gui,
		"windowmain_treeviewrecordings");
	model = gtk_tree_view_get_model (GTK_TREE_VIEW(treeview));
	gtk_tree_model_filter_refilter (GTK_TREE_MODEL_FILTER(model));
	treeview =  (GtkWidget *) gtk_builder_get_object (app->gui,
		"windowmain_treeviewfinishedrecordings");
	model = gtk_tree_view_get_model (GTK_TREE_VIEW(treeview));
	gtk_tree_model_filter_refilter (GTK_TREE_MODEL_FILTER(model));
}	

static gboolean
on_filter_recording_list_in_progress (GtkTreeModel *model, GtkTreeIter *iter, gpointer data)
{
	gboolean bRes = FALSE;
	
	// FreetuxTVApp *app = (FreetuxTVApp *) data;
	
	FreetuxTVRecordingInfos* pRecordingInfos;
	
	gtk_tree_model_get(GTK_TREE_MODEL(model), iter, RECORDING_COLUMN, &pRecordingInfos, -1);
	if(pRecordingInfos != NULL){
		switch(pRecordingInfos->status){
		case FREETUXTV_RECORDING_STATUS_WAITING:
		case FREETUXTV_RECORDING_STATUS_PROCESSING:
			bRes = TRUE;
			break;
		default:
			bRes = FALSE;
		}
	}
	return bRes;
}

static gboolean
on_filter_recording_list_terminated (GtkTreeModel *model, GtkTreeIter *iter, gpointer data)
{
	gboolean bRes = TRUE;
	
	// FreetuxTVApp *app = (FreetuxTVApp *) data;
	
	FreetuxTVRecordingInfos* pRecordingInfos;
	
	gtk_tree_model_get(GTK_TREE_MODEL(model), iter, RECORDING_COLUMN, &pRecordingInfos, -1);
	if(pRecordingInfos != NULL){
		switch(pRecordingInfos->status){
		case FREETUXTV_RECORDING_STATUS_WAITING:
		case FREETUXTV_RECORDING_STATUS_PROCESSING:
			bRes = FALSE;
			break;
		default:
			bRes = TRUE;
		}
	}
	return bRes;
}

static void
on_row_activated_recordings_list (GtkTreeView *view, GtkTreePath *path,
    GtkTreeViewColumn  *col, gpointer user_data)
{
	FreetuxTVApp *app = (FreetuxTVApp *) user_data;

	GError* error = NULL;

	GtkTreeModel *model;
	model = gtk_tree_view_get_model (view);

	GtkTreeIter iter;
	gtk_tree_model_get_iter (model, &iter, path);

	FreetuxTVRecordingInfos* pRecordingInfos;
	gtk_tree_model_get (model, &iter, RECORDING_COLUMN, &pRecordingInfos, -1);

	if(pRecordingInfos){

		// If recording finished, we play it
		if(pRecordingInfos->status == FREETUXTV_RECORDING_STATUS_FINISHED){
			GtkLibvlcMedia *media;
			media = gtk_libvlc_media_new (pRecordingInfos->szFileName);

			freetuxtv_play_media (app, media, &error);

			g_object_unref(media);
		}else{
			// TODO : Open the recording details dialog
		}
	}

	if(error != NULL){
		windowmain_show_gerror (app, error);
		g_error_free (error);
	}
}

static int
on_dbsync_add_recording (FreetuxTVApp *app,
    FreetuxTVRecordingInfos* pRecordingInfos,
    DBSync *dbsync, gpointer user_data, GError** error)
{
	GtkTreeIter* iter = (GtkTreeIter*)user_data;
	GtkListStore *pListStore;

	pListStore = GTK_LIST_STORE(app->pRecordingList);

	g_object_ref(pRecordingInfos);

	gtk_list_store_append (pListStore, iter);
	gtk_list_store_set (pListStore, iter,
	    RECORDING_COLUMN, pRecordingInfos,
	    -1);

	return 0;
}

static void 
on_row_displayed_recordings_list(GtkTreeViewColumn *col,
    GtkCellRenderer *renderer, GtkTreeModel *model,
    GtkTreeIter *iter, gpointer user_data)
{
	FreetuxTVApp *app = (FreetuxTVApp *) user_data;

	FreetuxTVRecordingInfos* pRecordingInfos = NULL;
	gchar* szLogoPath = NULL;

	// Si on veut afficher un groupe
	gtk_tree_model_get(model, iter, RECORDING_COLUMN, &pRecordingInfos, -1);
	if(pRecordingInfos != NULL){

		szLogoPath = tvchannels_list_get_tvchannel_logo_path_for_recording (app, pRecordingInfos, TRUE);

		g_object_set(renderer,
		    "title", pRecordingInfos->szTitle,
		    "begintime", pRecordingInfos->beginTime,
		    "endtime", pRecordingInfos->endTime,
		    "status", pRecordingInfos->status,
		    "logopath", szLogoPath,
		    "visible", TRUE, NULL);

		if(szLogoPath){
			g_free(szLogoPath);
			szLogoPath = NULL;
		}
	}
}

void
recordings_list_updatestatus(FreetuxTVApp *app, DBSync* dbsync, GError** error)
{
	g_return_if_fail(error != NULL);
	g_return_if_fail(*error == NULL);
	
	GtkListStore *pListStore;
	pListStore = GTK_LIST_STORE(app->pRecordingList);
	GtkTreeModel *model;
	model = GTK_TREE_MODEL(pListStore);

	GtkTreeIter iter;
	gboolean bGoOn;
	FreetuxTVRecordingInfos* pRecordingInfos = NULL;
	FREETUXTV_RECORDING_STATUS newstatus;
	FREETUXTV_RECORDING_STATUS oldstatus;

	GTimeVal now;
	gint64 time;
	g_get_current_time (&now);
	time = g_time_val_to_int64(&now);

	bGoOn = gtk_tree_model_get_iter_first(model, &iter);
	while(bGoOn){

		gtk_tree_model_get(model, &iter, RECORDING_COLUMN, &pRecordingInfos, -1);

		newstatus = pRecordingInfos->status;

		if(pRecordingInfos->status == FREETUXTV_RECORDING_STATUS_WAITING){
			if(pRecordingInfos->endTime < time){
				newstatus = FREETUXTV_RECORDING_STATUS_SKIPPED;
			}
		}

		if(pRecordingInfos->status == FREETUXTV_RECORDING_STATUS_PROCESSING){
			if(pRecordingInfos->endTime < time){
				newstatus = FREETUXTV_RECORDING_STATUS_ERROR;
			}
		}

		if(newstatus != pRecordingInfos->status){
			oldstatus = pRecordingInfos->status;
			pRecordingInfos->status = newstatus;
			dbsync_update_recording (dbsync, pRecordingInfos, error);
			if(*error){
				pRecordingInfos->status = oldstatus;
				bGoOn = FALSE;
			}
		}

		if(bGoOn){
			bGoOn = gtk_tree_model_iter_next(model, &iter);
		}
	}

	recordings_list_refresh (app);
}


void
recordings_list_getrecordings_toprocess(FreetuxTVApp *app, GList** ppListRecording, GError** error)
{
	g_return_if_fail(error != NULL);
	g_return_if_fail(*error == NULL);
	g_return_if_fail(ppListRecording != NULL);
	
	GtkListStore *pListStore;
	pListStore = GTK_LIST_STORE(app->pRecordingList);
	GtkTreeModel *model;
	model = GTK_TREE_MODEL(pListStore);

	GtkTreeIter iter;
	gboolean bGoOn;
	FreetuxTVRecordingInfos* pRecordingInfos = NULL;
	FREETUXTV_RECORDING_STATUS newstatus;

	GTimeVal now;
	gint64 time;
	g_get_current_time (&now);
	time = g_time_val_to_int64(&now);

	bGoOn = gtk_tree_model_get_iter_first(model, &iter);
	while(bGoOn){

		gtk_tree_model_get(model, &iter, RECORDING_COLUMN, &pRecordingInfos, -1);

		newstatus = pRecordingInfos->status;

		if(pRecordingInfos->status == FREETUXTV_RECORDING_STATUS_WAITING){
			if(pRecordingInfos->endTime < time){
				newstatus = FREETUXTV_RECORDING_STATUS_SKIPPED;
				*ppListRecording = g_list_append(*ppListRecording, (gpointer)pRecordingInfos);
			}
		}

		if(bGoOn){
			bGoOn = gtk_tree_model_iter_next(model, &iter);
		}
	}
}

void
recordings_list_add_recording(FreetuxTVApp *app, DBSync* dbsync, FreetuxTVRecordingInfos* pRecordingInfos, GError** error)
{
	g_return_if_fail(error != NULL);
	g_return_if_fail(*error == NULL);
	g_return_if_fail(dbsync != NULL);
	g_return_if_fail(pRecordingInfos != NULL);
	g_return_if_fail(FREETUXTV_IS_RECORDING_INFOS(pRecordingInfos));
	
	GtkTreeIter iter;
	
	dbsync_add_recording (dbsync, pRecordingInfos, error);

	if(*error == NULL){
		gtk_list_store_append (GTK_LIST_STORE(app->pRecordingList), &iter);
		gtk_list_store_set (GTK_LIST_STORE(app->pRecordingList), &iter, RECORDING_COLUMN, pRecordingInfos, -1);
	}
}