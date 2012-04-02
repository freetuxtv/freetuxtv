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

enum {
	TYPERECORDING_INPROGRESS = 0,
	TYPERECORDING_TERMINATED,
	TYPERECORDING_COUNT
};

static gboolean
on_filter_recording_list_in_progress (GtkTreeModel *model, GtkTreeIter *iter, gpointer data);

static gboolean
on_filter_recording_list_terminated (GtkTreeModel *model, GtkTreeIter *iter, gpointer data);

static void
on_row_activated_recordings_list (GtkTreeView *view, GtkTreePath *path,
    GtkTreeViewColumn  *col, gpointer user_data);

static gboolean
on_button_press_event_recordings_list (GtkWidget *treeview, GdkEventButton *event, gpointer user_data);

static void
on_popupmenu_activated_recordingdelete (GtkMenuItem *menuitem, gpointer user_data);

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
	g_signal_connect(widget, "button-press-event",
	    G_CALLBACK(on_button_press_event_recordings_list), app);

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
	g_signal_connect(widget, "button-press-event",
	    G_CALLBACK(on_button_press_event_recordings_list), app);
	
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

FreetuxTVRecordingInfos*
recordings_list_get_recording(FreetuxTVApp *app, GtkTreePath* pPathRecording)
{
	FreetuxTVRecordingInfos* pRecordingInfos;
	GtkTreeIter iter;
	g_return_val_if_fail(gtk_tree_model_get_iter (GTK_TREE_MODEL(app->pRecordingList), &iter, pPathRecording) == TRUE, NULL);
	gtk_tree_model_get(GTK_TREE_MODEL(app->pRecordingList), &iter, RECORDING_COLUMN, &pRecordingInfos, -1);
	return pRecordingInfos;
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

void
recordings_list_delete_recording (FreetuxTVApp *app, GtkTreePath *pPathRecording, gboolean bWithFile,
    DBSync* dbsync, GError** error)
{
	g_return_if_fail(dbsync != NULL);
	g_return_if_fail(error != NULL);

	FreetuxTVRecordingInfos* pRecordingInfos;
	pRecordingInfos = recordings_list_get_recording (app, pPathRecording);

	gchar* szFileName = g_strdup(pRecordingInfos->szFileName);

	// Delete the channel in the database
	if(*error == NULL){
		dbsync_delete_recording (dbsync, pRecordingInfos, error);
	}
	
	// Delete channel in the treeview
	if(*error == NULL){
		GtkTreeIter iter;
		gtk_tree_model_get_iter (GTK_TREE_MODEL(app->pRecordingList), &iter, pPathRecording);
		if(gtk_list_store_remove (GTK_LIST_STORE(app->pRecordingList), &iter)){
		};

		windowmain_update_statusbar_infos (app);
	}

	// Move the file to trash if needed
	if(*error == NULL && bWithFile){
		if(szFileName){
			GFile *source;
			source = g_file_new_for_path (szFileName);

			if(g_file_query_exists(source, NULL)){
				g_log(FREETUXTV_LOG_DOMAIN, G_LOG_LEVEL_INFO,
					  "Moving '%s' to the trash\n", szFileName);
					g_file_trash (source, NULL, error);
			}
			g_object_unref(source);
		}
	}
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


static void
on_popupmenu_selectiondone (GtkMenuShell *menushell, gpointer user_data)
{
	gtk_widget_destroy (GTK_WIDGET(menushell));
}

static gboolean
on_button_press_event_recordings_list (GtkWidget *treeview, GdkEventButton *event, gpointer user_data)
{

	FreetuxTVApp *app = (FreetuxTVApp *) user_data;

	if (event->type == GDK_BUTTON_PRESS && event->button == 3) {

		GtkTreeSelection *selection;
		GtkTreeModel* model_filter;
		GList *list;
		gint nbRecordingsTab[TYPERECORDING_COUNT+1] = {0, 0, 0};

		GtkTreePath *path;
		GtkTreePath *path_selected;

		model_filter = gtk_tree_view_get_model(GTK_TREE_VIEW(treeview));
		
		// Get the selection
		selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview));
		
		// Select the path where user has clicked if not selected
		if (gtk_tree_view_get_path_at_pos(GTK_TREE_VIEW(treeview),
						  (gint) event->x, (gint) event->y,
						  &path_selected, NULL, NULL, NULL)){
			if(!gtk_tree_selection_path_is_selected (selection, path_selected)){
				gtk_tree_selection_unselect_all (selection);
				gtk_tree_selection_select_path(selection, path_selected);
			}
			gtk_tree_path_free(path_selected);
			path_selected = NULL;
		}

		// Get the final list of selected item
		list = gtk_tree_selection_get_selected_rows (selection, &model_filter);
		
		//nbTotalGroupsVisible = gtk_tree_model_iter_n_children (GTK_TREE_MODEL(model_filter), NULL);

		// Count number of elements selected by category
		GList* iterator = NULL;
		iterator = g_list_first (list);
		while(iterator != NULL){
			GtkTreePath *real_path;
			GtkTreeIter iter;

			// Get the real path
			path = (GtkTreePath*)iterator->data;
			real_path = gtk_tree_model_filter_convert_path_to_child_path(GTK_TREE_MODEL_FILTER(model_filter), path);
			
			// Look the type of the data in the path
			FreetuxTVRecordingInfos* pRecordingInfo;
			gtk_tree_model_get_iter (GTK_TREE_MODEL(app->pRecordingList), &iter, real_path);
			gtk_tree_model_get(GTK_TREE_MODEL(app->pRecordingList), &iter, RECORDING_COLUMN, &pRecordingInfo, -1);
			if(pRecordingInfo != NULL){
				switch(pRecordingInfo->status)
				{
					case FREETUXTV_RECORDING_STATUS_PROCESSING :
					case FREETUXTV_RECORDING_STATUS_WAITING :
						nbRecordingsTab[TYPERECORDING_INPROGRESS]++;
						break;
					default:
						nbRecordingsTab[TYPERECORDING_TERMINATED]++;
						break;
				}
				nbRecordingsTab[TYPERECORDING_COUNT]++;
			}

			// Iterate next path
			iterator = g_list_next(iterator);
		}

		GtkWidget *pMenu = NULL;
		GtkWidget *pMenuItem;

		if(nbRecordingsTab[TYPERECORDING_COUNT] > 0){
			pMenu = gtk_menu_new();
			g_signal_connect(G_OBJECT(pMenu), "selection-done",
			    G_CALLBACK(on_popupmenu_selectiondone), app);

			// Delete channels from favourites group
			pMenuItem = gtk_image_menu_item_new_from_stock ("gtk-delete", NULL);
#if GTK_API_VERSION == 3
			gtk_menu_shell_append (GTK_MENU_SHELL (pMenu), pMenuItem);
#else
			gtk_menu_append (GTK_MENU (pMenu), pMenuItem);
#endif
			g_signal_connect(G_OBJECT(pMenuItem), "activate",
			    G_CALLBACK(on_popupmenu_activated_recordingdelete), app);
			gtk_widget_show (pMenuItem);
		}			 

		if(pMenu){
			gtk_widget_show(pMenu);
			
			gtk_menu_popup(GTK_MENU(pMenu), NULL, NULL, NULL, NULL,
				       (event != NULL) ? event->button : 0,
				       gdk_event_get_time((GdkEvent*)event));
		}

		return TRUE;
	}

	return FALSE;
}

static void
on_popupmenu_activated_recordingdelete (GtkMenuItem *menuitem, gpointer user_data)
{
	FreetuxTVApp *app = (FreetuxTVApp *)user_data;

	GError* error = NULL;

	GtkWidget* pParent;
	GtkWidget* treeview;
	GtkTreeModel* model_filter;
	GtkTreeSelection *selection;
	GList *list;
	GList* iterator = NULL;
	GtkTreePath *path;
	GtkTreePath *real_path;
	
	DBSync dbsync;
	dbsync_open_db (&dbsync, &error);

	gboolean bDeleteFile = FALSE;

	if(error == NULL){
		treeview =  (GtkWidget *) gtk_builder_get_object (app->gui,
		    "windowmain_treeviewfinishedrecordings");
		model_filter = gtk_tree_view_get_model(GTK_TREE_VIEW(treeview));
		
		// Get the selection
		selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview));
		list = gtk_tree_selection_get_selected_rows (selection, &model_filter);

		if(list == NULL){
			treeview = (GtkWidget *) gtk_builder_get_object (app->gui,
				"windowmain_treeviewrecordings");
			model_filter = gtk_tree_view_get_model(GTK_TREE_VIEW(treeview));
		
			// Get the selection
			selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview));
			list = gtk_tree_selection_get_selected_rows (selection, &model_filter);
		}

		if(list != NULL){
			GtkWidget* dialog;
			pParent = (GtkWidget *) gtk_builder_get_object (app->gui,
				"windowmain");
			dialog = gtk_message_dialog_new (GTK_WINDOW(pParent),
			    GTK_DIALOG_MODAL, GTK_MESSAGE_QUESTION, GTK_BUTTONS_YES_NO,
			    _("Do you want to move the related file on the hard disk to the trash?"));
			int res = gtk_dialog_run(GTK_DIALOG(dialog));
			if(res == GTK_RESPONSE_YES){
				bDeleteFile = TRUE;
			}
			gtk_widget_destroy (dialog);
			dialog = NULL;
		}
		
		iterator = g_list_last (list);

		while(iterator != NULL && error == NULL){			

			// Get the real path
			path = (GtkTreePath*)iterator->data;
			real_path = gtk_tree_model_filter_convert_path_to_child_path(GTK_TREE_MODEL_FILTER(model_filter), path);
			
			// Delete the recordings corresponding to the path
			recordings_list_delete_recording(app, real_path, bDeleteFile, &dbsync, &error);

			iterator = g_list_previous(iterator);
		}
	}
	
	dbsync_close_db(&dbsync);

	if(error == NULL){
		windowmain_update_statusbar_infos (app);
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
			if(freetuxtv_recording_infos_is_time_greater (pRecordingInfos, time)){
				// To late for the recording
				newstatus = FREETUXTV_RECORDING_STATUS_SKIPPED;
				pRecordingInfos->status = newstatus;
				//*ppListRecording = g_list_append(*ppListRecording, (gpointer)pRecordingInfos);
			}else if(freetuxtv_recording_infos_has_time (pRecordingInfos, time)){
				// The recording should be started
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