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

#include <gtk/gtk.h>
#include <glib/gstdio.h>

#include "freetuxtv-channels-list.h"

#include "freetuxtv-app.h"
#include "freetuxtv-i18n.h"
#include "freetuxtv-window-main.h"
#include "freetuxtv-fileutils.h"
#include "freetuxtv-channel-infos.h"
#include "freetuxtv-channels-group-infos.h"
#include "freetuxtv-cellrenderer-channelslist.h"
#include "freetuxtv-tv-channels-list.h"
#include "freetuxtv-db-sync.h"
#include "freetuxtv-window-add-channels-group.h"
#include "lib-m3uparser.h"

typedef struct _Parsem3uData
{
	FreetuxTVApp *app;
	FreetuxTVChannelsGroupInfos *channels_group_infos;
	DBSync *dbsync;
	GError** error;
} Parsem3uData;

typedef struct _CBIterData
{
	GtkTreeIter *iter;
	GtkTreeIter *iter_parent;
} CBIterData;

enum
{
	CHANNELSGROUP_COLUMN,
	CHANNEL_COLUMN,
	N_COLUMNS
};

enum {
	TYPEGRP_PLAYLIST = 0,
	TYPEGRP_FAVORITE,
	TYPEGRP_COUNT
};


static void 
channels_list_delete_rows (FreetuxTVApp *app, GtkTreePath* path_group,
			   gboolean justchannels);

static int
on_dbsync_add_channels_group(FreetuxTVApp *app, FreetuxTVChannelsGroupInfos* channels_group_infos,
			     DBSync *dbsync, gpointer user_data, GError** error);

static int
on_dbsync_add_channel(FreetuxTVApp *app, FreetuxTVChannelInfos* channel_infos,
		      DBSync *dbsync, gpointer user_data, GError** error);

static void 
on_row_activated_channels_list (GtkTreeView *view, GtkTreePath *path,
				GtkTreeViewColumn  *col, gpointer user_data);

static void 
on_row_displayed_channels_list (GtkTreeViewColumn *col,
				GtkCellRenderer *renderer, GtkTreeModel *model,
				GtkTreeIter *iter, gpointer user_data);

static gboolean
on_button_press_event_channels_list (GtkWidget *treeview, GdkEventButton *event, gpointer user_data);

static void
on_popupmenu_activated_refreshgroup (GtkMenuItem *menuitem, gpointer user_data);

static void
on_popupmenu_activated_deletechannels (GtkMenuItem *menuitem, gpointer user_data);

static void
on_popupmenu_activated_deletegroup (GtkMenuItem *menuitem, gpointer user_data);

static void
on_popupmenu_activated_groupproperties (GtkMenuItem *menuitem, gpointer user_data);

static void
on_popupmenu_activated_addfavourites (GtkMenuItem *menuitem, gpointer user_data);

static void
on_popupmenu_activated_deletechannel (GtkMenuItem *menuitem, gpointer user_data);


static gboolean
on_filter_channels_list (GtkTreeModel *model, GtkTreeIter *iter, gpointer data);

static int 
on_parsem3u_add_channel (char *url, int num, int argc, 
			 char **argv, void *user_data);

static void
channels_group_get_file (FreetuxTVChannelsGroupInfos *self, gchar **filename,
			 gboolean update, GError** error);

static GtkTreePath*
get_favourites_channels_group_new(FreetuxTVApp *app, FreetuxTVChannelsGroupInfos **pChannelsGroupInfos);

static GList*
get_favourites_channels_groups_paths(FreetuxTVApp *app);

static void
channels_list_print(FreetuxTVApp *app);

GQuark freetuxtv_libm3uparse_error = 0;

GQuark
freetuxtv_libm3uparse_error_quark () {
	if (freetuxtv_libm3uparse_error == 0){
		freetuxtv_libm3uparse_error = g_quark_from_string("FREETUXTV_LIBM3UPARSE_ERROR");
	}
	return freetuxtv_libm3uparse_error;
}

void
channels_list_init (FreetuxTVApp *app)
{
	GtkWidget *treeview;
	GtkTreeViewColumn *column;
	GtkCellRenderer *renderer;
	GtkTreeModel *model;
	GtkTreeStore *channelslist;
	GtkTreeSelection *selection;
	
	// TODO : free it
	channelslist = gtk_tree_store_new (N_COLUMNS,
					   FREETUXTV_TYPE_CHANNELS_GROUP_INFOS,
					   FREETUXTV_TYPE_CHANNEL_INFOS);
	app->channelslist = GTK_TREE_MODEL(channelslist);
	
	// Enable filter on the channel list
	model = gtk_tree_model_filter_new (GTK_TREE_MODEL(app->channelslist), NULL);
	gtk_tree_model_filter_set_visible_func (GTK_TREE_MODEL_FILTER (model),
						on_filter_channels_list,
						(gpointer) app, NULL);
	
	treeview = (GtkWidget *)gtk_builder_get_object (app->gui,
							"windowmain_treeviewchannelslist");	
	gtk_tree_view_set_model (GTK_TREE_VIEW(treeview), GTK_TREE_MODEL(model));
	gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(treeview), FALSE);

	selection = gtk_tree_view_get_selection (GTK_TREE_VIEW(treeview));
	gtk_tree_selection_set_mode (selection, GTK_SELECTION_MULTIPLE);

	column = gtk_tree_view_column_new();
	renderer = freetuxtv_cellrenderer_channelslist_new ();
	gtk_tree_view_column_pack_start(column, renderer, FALSE);
	gtk_tree_view_column_set_cell_data_func(column, renderer,
						on_row_displayed_channels_list,
						(gpointer)app, NULL);	
	gtk_tree_view_append_column (GTK_TREE_VIEW (treeview), column);

	g_signal_connect(treeview, "row-activated", G_CALLBACK(on_row_activated_channels_list), app);
	g_signal_connect(treeview, "button-press-event", G_CALLBACK(on_button_press_event_channels_list), app);
}

void
channels_list_load_channels (FreetuxTVApp *app, DBSync* dbsync, GError** error)
{
	g_return_if_fail(dbsync != NULL);
	g_return_if_fail(error != NULL);

	GtkTreeIter iter;
	CBIterData iter_data;
	
	iter_data.iter = &iter;
	iter_data.iter_parent = NULL;

	// Load list of channel from database
	if(*error == NULL){
		gtk_tree_store_clear(GTK_TREE_STORE(app->channelslist));
		dbsync_select_channels_groups (dbsync, app, on_dbsync_add_channels_group, &iter_data, error);
	}
}

void
channels_list_add_channels_group (FreetuxTVApp *app, FreetuxTVChannelsGroupInfos* pChannelsGroupInfos,
                                  GtkTreePath** ppChannelsGroupTreePath, DBSync* dbsync,
                                  GError** error)
{
	g_return_if_fail(dbsync != NULL);
	g_return_if_fail(error != NULL);

	if(*error == NULL){
		dbsync_add_channels_group (dbsync, pChannelsGroupInfos, error);
	}

	// Add group in the treeview
	if(*error == NULL){	
		GtkTreeIter iter_channelsgroup;
		gtk_tree_store_append (GTK_TREE_STORE(app->channelslist), &iter_channelsgroup, NULL);
		gtk_tree_store_set (GTK_TREE_STORE(app->channelslist), &iter_channelsgroup,
		                    CHANNELSGROUP_COLUMN, pChannelsGroupInfos, -1);

		GtkTreePath* path;
		path = gtk_tree_model_get_path(app->channelslist, &iter_channelsgroup);
		
		g_print("added %s\n", gtk_tree_path_to_string (path));

		channels_list_refresh_channels_group(app, path, dbsync, error);

		if(ppChannelsGroupTreePath){
			*ppChannelsGroupTreePath = path;
		}else{
			gtk_tree_path_free(path);
		}
	}

	if(*error == NULL){		
		windowmain_update_statusbar_infos (app);
	}
}

void
channels_list_update_channels_group (FreetuxTVApp *app, GtkTreePath *path_group,
				     FreetuxTVChannelsGroupInfos* channels_group_infos)
{
	GError* error = NULL;
	
	// Update the group in the database
	DBSync dbsync;
	dbsync_open_db (&dbsync, &error);
	if(error == NULL){
		dbsync_update_channels_group (&dbsync, channels_group_infos, &error);
	}
	dbsync_close_db(&dbsync);

	// Update the group in the treeview
	if(error == NULL){
		GtkTreeIter iter;
		if(gtk_tree_model_get_iter(app->channelslist, &iter, path_group)){
			gtk_tree_model_row_changed (app->channelslist, path_group, &iter);
		}
		windowmain_update_statusbar_infos (app);
	}

	if(error != NULL){
		windowmain_show_gerror (app, error);
		g_error_free (error);
	}	
}

void
channels_list_refresh_channels_group (FreetuxTVApp *app, GtkTreePath *path_group,
				      DBSync* dbsync, GError** error)
{
	g_return_if_fail(dbsync != NULL);
	g_return_if_fail(error != NULL);

	FreetuxTVChannelsGroupInfos* pChannelsGroupInfos;
	pChannelsGroupInfos = channels_list_get_group (app, path_group);

	gchar *text;

	// Update status bar
	text = g_strdup_printf(_("Update \"%s\" channels list"), pChannelsGroupInfos->name);
	windowmain_statusbar_push (app, "UpdateMsg", text);
	g_free(text);
	g_log(FREETUXTV_LOG_DOMAIN, G_LOG_LEVEL_MESSAGE,
	      "Start updating \"%s\" channels list\n", pChannelsGroupInfos->name);

	if(pChannelsGroupInfos->type == FREETUXTV_CHANNELSGROUP_TYPEGROUP_PLAYLIST){
		// Get the file of the playlist
		text = g_strdup_printf (_("Getting the file : \"%s\""), pChannelsGroupInfos->uri);
		windowmain_statusbar_push (app, "UpdateMsg", text);
		g_free(text);
		g_log(FREETUXTV_LOG_DOMAIN, G_LOG_LEVEL_INFO,
		      "Getting the file \"%s\"\n", pChannelsGroupInfos->uri);

		gchar *filename = NULL;
		channels_group_get_file (pChannelsGroupInfos, &filename, TRUE, error);		
		windowmain_statusbar_pop (app, "UpdateMsg");

		// Delete channels of the channels group in the database 
		if(*error == NULL){
			dbsync_start_update_channels_of_channels_group (dbsync, pChannelsGroupInfos, error);
		}
	
		// Parse the M3U file and add channels in the database
		if(*error == NULL){
			Parsem3uData pdata;
			pdata.channels_group_infos = pChannelsGroupInfos;
			pdata.dbsync = dbsync;
			pdata.app = app;
			pdata.error = error;
			int res = 0;

			g_log(FREETUXTV_LOG_DOMAIN, G_LOG_LEVEL_INFO,
			      "Parsing the file \"%s\"\n", filename);
			res = libm3uparser_parse(filename, &on_parsem3u_add_channel, &pdata);
			if (res != LIBM3UPARSER_OK){		
				if (res != LIBM3UPARSER_CALLBACK_RETURN_ERROR){
					*error = g_error_new (FREETUXTV_LIBM3UPARSE_ERROR,
								  FREETUXTV_LIBM3UPARSE_ERROR_PARSE,
								  _("Error when adding the channels.\n\nM3UParser has returned error :\n%s."),
								  libm3uparser_errmsg(res));
				}
			}
		}

		if(*error == NULL){
			dbsync_end_update_channels_of_channels_group (dbsync, pChannelsGroupInfos, error);
		}
	
		if(filename){
			g_free(filename);
		}
	}

	if(*error == NULL){
		// Update the last update date of the group
		dbsync_update_channels_group_last_update (dbsync,
		    pChannelsGroupInfos, error);
	}

	if(*error == NULL){
		// Delete channels of the channels group in the treeview
		channels_list_delete_rows (app, path_group, TRUE);

		// Display the channels from the database
		CBIterData iter_data;
		GtkTreeIter iter_group;
		GtkTreeIter iter_channel;
		gtk_tree_model_get_iter (GTK_TREE_MODEL(app->channelslist), &iter_group, path_group);
		
		iter_data.iter = &iter_channel;
		iter_data.iter_parent = &iter_group;

		dbsync_select_channels_of_channels_group (dbsync, pChannelsGroupInfos, app, on_dbsync_add_channel, &iter_data, error);
	}

	// Expand the group
	if(*error == NULL){
		GtkWidget *treeview;
		treeview = (GtkWidget *)gtk_builder_get_object (app->gui,
								"windowmain_treeviewchannelslist");
		gtk_tree_view_expand_row (GTK_TREE_VIEW(treeview), path_group, FALSE);
	}

	windowmain_statusbar_pop (app, "UpdateMsg");
}


void
channels_list_delete_channels_channels_group (FreetuxTVApp *app, GtkTreePath *path_group,
					      DBSync* dbsync, GError** error)
{
	g_return_if_fail(dbsync != NULL);
	g_return_if_fail(error != NULL);
	
	FreetuxTVChannelsGroupInfos* channels_group_infos;
	channels_group_infos = channels_list_get_group (app, path_group);
	
	// Delete channels in the database
	if(*error == NULL){
		dbsync_delete_channels_of_channels_group (dbsync, channels_group_infos, error);
	}
	
	// Delete channels in the treeview
	if(*error == NULL){
		channels_list_delete_rows (app, path_group, TRUE);
		windowmain_update_statusbar_infos (app);
	}
}

void
channels_list_delete_channels_group (FreetuxTVApp *app, GtkTreePath *path_group,
				     DBSync* dbsync, GError** error)
{
	g_return_if_fail(dbsync != NULL);
	g_return_if_fail(error != NULL);

	gchar *filename;

	FreetuxTVChannelsGroupInfos* pChannelsGroupInfos;
	pChannelsGroupInfos = channels_list_get_group (app, path_group);
		
	// Delete group in the database
	if(*error == NULL){
		dbsync_delete_channels_group (dbsync, pChannelsGroupInfos, error);
	}

	// Delete the temporary file
	if(pChannelsGroupInfos->type == FREETUXTV_CHANNELSGROUP_TYPEGROUP_PLAYLIST){
		if(*error == NULL){
			channels_group_get_file (pChannelsGroupInfos, &filename, TRUE, error);
	
			g_log(FREETUXTV_LOG_DOMAIN, G_LOG_LEVEL_INFO,
			      "Deleting the file \"%s\"\n", filename);
			if(g_unlink(filename)){
			
			}

			g_free(filename);
		}
	}

	// Delete group in the treeview
	if(*error == NULL){
		channels_list_delete_rows (app, path_group, FALSE);
		windowmain_update_statusbar_infos (app);
	}
}

void
channels_list_delete_channel (FreetuxTVApp *app, GtkTreePath *path_channel,
				     DBSync* dbsync, GError** error)
{
	g_return_if_fail(dbsync != NULL);
	g_return_if_fail(error != NULL);

	FreetuxTVChannelInfos* pChannelInfos;
	pChannelInfos = channels_list_get_channel (app, path_channel);

	// Delete the channel in the database
	if(*error == NULL){
		dbsync_delete_channel (dbsync, pChannelInfos, error);
	}
	
	// Delete channel in the treeview
	if(*error == NULL){
		GtkTreeIter iter;
		gtk_tree_model_get_iter (GTK_TREE_MODEL(app->channelslist), &iter, path_channel);
		if(gtk_tree_store_remove (GTK_TREE_STORE(app->channelslist), &iter)){
			// Update the position on the channel after
			do {
				gtk_tree_model_get (GTK_TREE_MODEL(app->channelslist), &iter, CHANNEL_COLUMN, &pChannelInfos, -1);
				if(pChannelInfos){
					pChannelInfos->position--;
				}
			} while(gtk_tree_model_iter_next (GTK_TREE_MODEL(app->channelslist), &iter));
		};

		pChannelInfos->channels_group->nb_channels--;


		windowmain_update_statusbar_infos (app);
	}
}

gboolean
channels_list_get_prev_channel (FreetuxTVApp *app,
				GtkTreePath **path_prev_channel)
{
	GtkWidget *treeview;
	GtkTreeModel *model;
		
	GtkTreePath *path;
	GtkTreeIter iter_channel;
	
	treeview = (GtkWidget *) gtk_builder_get_object(app->gui,
							"windowmain_treeviewchannelslist");
	model = gtk_tree_view_get_model (GTK_TREE_VIEW(treeview));

	if (app->current.path_channel != NULL) {
		path = gtk_tree_path_copy(app->current.path_channel);
		if(gtk_tree_path_prev (path)){
			if(gtk_tree_model_get_iter (GTK_TREE_MODEL(app->channelslist), &iter_channel, path)){
				*path_prev_channel = path;
				return TRUE;
			}
		}
		gtk_tree_path_free(path);	       
	}
	return FALSE;
}

gboolean
channels_list_get_next_channel (FreetuxTVApp *app,
				GtkTreePath ** path_next_channel)
{
	GtkWidget *treeview;
	GtkTreeModel *model;
		
	GtkTreePath *path;
	GtkTreeIter iter_channel;
	
	treeview = (GtkWidget *) gtk_builder_get_object(app->gui,
							"windowmain_treeviewchannelslist");
	model = gtk_tree_view_get_model (GTK_TREE_VIEW(treeview));

	if (app->current.path_channel != NULL) {
		path = gtk_tree_path_copy(app->current.path_channel);
		gtk_tree_path_next (path);
		if(gtk_tree_model_get_iter (GTK_TREE_MODEL(app->channelslist), &iter_channel, path)){
			*path_next_channel = path;
			return TRUE;
		}
		gtk_tree_path_free(path);	       
	}
	return FALSE;
}

gint
channels_list_get_channels_count (FreetuxTVApp *app)
{
	gint channels_count = 0;

	GtkTreeIter iter;
	GtkTreeModel *model;

	model = app->channelslist;

	if(gtk_tree_model_get_iter_first (model, &iter)){

		do {
			channels_count += gtk_tree_model_iter_n_children (model, &iter);
		}while(gtk_tree_model_iter_next(model, &iter));
	}

	return channels_count;
}

void
channels_list_set_playing(FreetuxTVApp *app, GtkTreePath *path_channel)
{
	GtkWidget *treeview = NULL;
	GtkTreeIter iter;

	GtkTreeModel *model = NULL;
	GtkTreePath* filter_path;;

	treeview = (GtkWidget *) gtk_builder_get_object(app->gui,
							"windowmain_treeviewchannelslist");
	
	model = gtk_tree_view_get_model (GTK_TREE_VIEW(treeview));
	
	if(path_channel != NULL){
		if(app->current.path_channel != path_channel){
			if(app->current.path_channel != NULL){
				gtk_tree_model_get_iter (model, &iter, app->current.path_channel);		
				gtk_tree_model_row_changed (model, app->current.path_channel, &iter);
				gtk_tree_path_free(app->current.path_channel);
			}
			app->current.path_channel = gtk_tree_path_copy(path_channel);
			gtk_tree_model_get_iter (model, &iter, app->current.path_channel);
			gtk_tree_model_row_changed (model, app->current.path_channel, &iter);
		}

		gtk_tree_view_expand_to_path (GTK_TREE_VIEW(treeview), app->current.path_channel);

		filter_path = gtk_tree_model_filter_convert_child_path_to_path (GTK_TREE_MODEL_FILTER(model), app->current.path_channel);
		
		if(filter_path != NULL){
			gtk_tree_view_scroll_to_cell (GTK_TREE_VIEW(treeview), filter_path, NULL,
						      TRUE, 0.5, 0.5);
			gtk_tree_view_set_cursor (GTK_TREE_VIEW(treeview), filter_path, NULL, FALSE);
			//gtk_tree_view_set_cursor (GTK_TREE_VIEW(treeview), filter_path, NULL, FALSE);	
			//gtk_widget_grab_focus (treeview);
			gtk_tree_view_expand_to_path (GTK_TREE_VIEW(treeview), filter_path);
		}
	}else{
		GtkTreePath* pTmpPath = app->current.path_channel;
		if(pTmpPath != NULL){
			app->current.path_channel = NULL;
			gtk_tree_model_get_iter (model, &iter, pTmpPath);		
			gtk_tree_model_row_changed (model, pTmpPath, &iter);
			gtk_tree_path_free(pTmpPath);
		}
	}
}

FreetuxTVChannelInfos*
channels_list_get_channel(FreetuxTVApp *app, GtkTreePath* path_channel)
{
	FreetuxTVChannelInfos* channel_infos;
	GtkTreeIter iter;
	g_return_val_if_fail(gtk_tree_model_get_iter (GTK_TREE_MODEL(app->channelslist), &iter, path_channel) == TRUE, NULL);
	gtk_tree_model_get(GTK_TREE_MODEL(app->channelslist), &iter, CHANNEL_COLUMN, &channel_infos, -1);
	return channel_infos;
}


FreetuxTVChannelsGroupInfos*
channels_list_get_group(FreetuxTVApp *app, GtkTreePath* path_group)
{
	FreetuxTVChannelsGroupInfos* channels_group_infos;
	GtkTreeIter iter;
	g_return_val_if_fail(gtk_tree_model_get_iter (GTK_TREE_MODEL(app->channelslist), &iter, path_group) == TRUE, NULL);
	gtk_tree_model_get(GTK_TREE_MODEL(app->channelslist), &iter, CHANNELSGROUP_COLUMN, &channels_group_infos, -1);
	return channels_group_infos;	
}

static void 
channels_list_delete_rows (FreetuxTVApp *app, GtkTreePath* path_group,
			   gboolean justchannels)
{
	GtkTreeIter iter_channelsgroup;

	FreetuxTVChannelsGroupInfos* channels_group_infos = NULL;
	
	// Get the group from the selected path
	gtk_tree_model_get_iter (GTK_TREE_MODEL(app->channelslist), &iter_channelsgroup, path_group);
	
	GtkTreeIter* iter_current;
	iter_current = gtk_tree_iter_copy (&iter_channelsgroup);
	
	// Delete the channels of the group
	if(gtk_tree_model_iter_has_child (GTK_TREE_MODEL(app->channelslist), &iter_channelsgroup)){
		GtkTreeIter iter_tmp;
		gtk_tree_model_iter_children (GTK_TREE_MODEL(app->channelslist), &iter_tmp, &iter_channelsgroup);
		while(gtk_tree_store_remove (GTK_TREE_STORE(app->channelslist), &iter_tmp)){};
	}

	// Delete the group from the treeview
	if(!justchannels){
		// Update the position of the group after
		if(iter_current){
			do{
				gtk_tree_model_get (GTK_TREE_MODEL(app->channelslist), iter_current, CHANNELSGROUP_COLUMN, &channels_group_infos, -1);
				if(channels_group_infos){
					channels_group_infos->position--;
				}
			}while(gtk_tree_model_iter_next (GTK_TREE_MODEL(app->channelslist), iter_current));
		}
		
		gtk_tree_store_remove (GTK_TREE_STORE(app->channelslist), &iter_channelsgroup);
	}

	if(iter_current){
		gtk_tree_iter_free (iter_current);
	}
}

/*
static void
channels_list_display_channels (FreetuxTVApp *app)
{
	GtkWidget *treeview;
	treeview = (GtkWidget *) gtk_builder_get_object(app->gui,
							"windowmain_treeviewchannelslist");
	gtk_tree_view_expand_all (GTK_TREE_VIEW(treeview));
}
*/

static void
channels_group_get_file (FreetuxTVChannelsGroupInfos *self, gchar **filename,
			 gboolean update, GError** error)
{
	gchar *groupfile;
	groupfile = g_strdup_printf("%s/FreetuxTV/cache/playlist-group-%d.dat",
				    g_get_user_config_dir(), self->id);
	if(update){
		freetuxtv_fileutils_get_file (self->uri, groupfile, error);
	}

	*filename = groupfile;
}

static int
on_dbsync_add_channels_group(FreetuxTVApp *app, FreetuxTVChannelsGroupInfos* channels_group_infos,
			     DBSync *dbsync, gpointer user_data, GError** error)
{

	gboolean no_err = TRUE;
	
	if(error != NULL){
		if(*error != NULL){
			no_err = FALSE;
		}
	}
	
	if(no_err){
		CBIterData *iter_data = (CBIterData *)user_data;		

		gtk_tree_store_append (GTK_TREE_STORE(app->channelslist), iter_data->iter, iter_data->iter_parent);
		gtk_tree_store_set (GTK_TREE_STORE(app->channelslist), iter_data->iter,
				    CHANNELSGROUP_COLUMN, channels_group_infos, -1);

		GtkTreeIter iter;
		CBIterData iter_data2;
		iter_data2.iter_parent = iter_data->iter;
		iter_data2.iter = &iter;
		dbsync_select_channels_of_channels_group (dbsync, channels_group_infos, app, on_dbsync_add_channel, &iter_data2, error);
	}

	return 0;
}

static int
on_dbsync_add_channel(FreetuxTVApp *app, FreetuxTVChannelInfos* pChannelInfos,
		      DBSync *dbsync, gpointer user_data, GError** error)
{
	gboolean no_err = TRUE;
	
	if(error != NULL){
		if(*error != NULL){
			no_err = FALSE;
		}
	}

	g_log(FREETUXTV_LOG_DOMAIN, G_LOG_LEVEL_DEBUG,
	      "Add channel %d->'%s' to the model\n",
	      pChannelInfos->id, pChannelInfos->name);
	
	if(no_err){
		CBIterData *iter_data = (CBIterData *)user_data;

		gtk_tree_store_append (GTK_TREE_STORE(app->channelslist),
				       iter_data->iter, iter_data->iter_parent);
		gtk_tree_store_set (GTK_TREE_STORE(app->channelslist), iter_data->iter,
				    CHANNEL_COLUMN, pChannelInfos, -1);

		// Get the path of the channel to play, if needed
		gboolean match = FALSE;
		if(app->current.path_channel == NULL){
			// If channel name match with required channel
			if(app->current.open_channel_id != -1){
				if(pChannelInfos->id == app->current.open_channel_id){
					match = TRUE;
				}
			}else if(app->prefs.channelonstartup && app->config.lastchannel != -1){
				// If we want to load a channel on startup and the channel match
				if(pChannelInfos->id == app->config.lastchannel){
					match = TRUE;
				}
			}
			if(match){
				GtkTreePath* path;
				path = gtk_tree_model_get_path(GTK_TREE_MODEL(app->channelslist), iter_data->iter);
				app->current.path_channel = path;
				app->current.autoplay_channel = TRUE;
			}
		}
	}

	return 0;
}

static void
on_row_activated_channels_list(GtkTreeView *view, GtkTreePath *path,
			       GtkTreeViewColumn  *col, gpointer user_data)
{
	FreetuxTVApp *app = (FreetuxTVApp *) user_data;
	
	GError* error = NULL;
	
	GtkTreeIter iter;
	gtk_tree_model_get_iter (GTK_TREE_MODEL(app->channelslist), &iter, path);

	FreetuxTVChannelsGroupInfos* channels_group_infos;
	// FreetuxTVChannelInfos* channel_infos;

	GtkTreeModel *model;
	GtkTreePath* model_path;
	model = gtk_tree_view_get_model (GTK_TREE_VIEW(view));
	model_path = gtk_tree_model_filter_convert_path_to_child_path (GTK_TREE_MODEL_FILTER(model), path);

	gtk_tree_model_get(GTK_TREE_MODEL(app->channelslist), &iter, CHANNELSGROUP_COLUMN, &channels_group_infos, -1);
	if(channels_group_infos != NULL){
		if(gtk_tree_view_row_expanded(view, model_path)){
			gtk_tree_view_collapse_row (view, model_path);
		}else{
			gtk_tree_view_expand_row (view, model_path, TRUE);
		}	
	}else{
		freetuxtv_play_channel (app, model_path, &error);		
	}
	gtk_tree_path_free(model_path);

	if(error != NULL){
		windowmain_show_gerror (app, error);
		g_error_free (error);
	}
}

static void 
on_row_displayed_channels_list(GtkTreeViewColumn *col,
			       GtkCellRenderer *renderer, GtkTreeModel *model,
			       GtkTreeIter *iter, gpointer user_data)
{
	FreetuxTVApp *app = (FreetuxTVApp *) user_data;
	
	FreetuxTVChannelsGroupInfos* channels_group_infos = NULL;
	FreetuxTVChannelInfos* channel_infos = NULL;
	
	// GdkPixbuf* logo;
	// GdkColor color;	

	// Si on veut afficher un groupe
	gtk_tree_model_get(model, iter, CHANNELSGROUP_COLUMN, &channels_group_infos, -1);
	if(channels_group_infos != NULL){
		g_object_set(renderer, "type", CELLRENDERER_TYPE_CHANNELS_GROUP, "name", channels_group_infos->name, "visible", TRUE, NULL);
	}else{
		gtk_tree_model_get(model, iter, CHANNEL_COLUMN, &channel_infos, -1);  

		gchar *imgfile;
		imgfile = tvchannels_list_get_tvchannel_logo_path(app, channel_infos, TRUE);

		gboolean is_playing = FALSE;
		if(app->current.path_channel != NULL){
			GtkTreePath* path, *model_path;
			path = gtk_tree_model_get_path(model, iter);
			model_path = gtk_tree_model_filter_convert_path_to_child_path (GTK_TREE_MODEL_FILTER(model), path);
			if(gtk_tree_path_compare(app->current.path_channel, model_path) == 0){
				is_playing = TRUE;
			}
			gtk_tree_path_free(path);
			gtk_tree_path_free(model_path);
		}

		g_object_set(renderer,
			     "type", CELLRENDERER_TYPE_CHANNEL, "name", channel_infos->name,
			     "logo", imgfile, "isplaying", is_playing,
			     "visible", TRUE, NULL);
		
		g_free(imgfile);
	}
}

static gboolean
on_button_press_event_channels_list (GtkWidget *treeview, GdkEventButton *event, gpointer user_data)
{

	FreetuxTVApp *app = (FreetuxTVApp *) user_data;

	if (event->type == GDK_BUTTON_PRESS && event->button == 3) {
		
		GtkTreeSelection *selection;
		GtkTreeModel* model_filter;
		GList *list;
		gint nb_selected = 0;
		gint nbChannels = 0;
		gint nbGroupsTab[TYPEGRP_COUNT+1] = {0, 0, 0};
		gint nbChannelsForGroupTab[TYPEGRP_COUNT] = {0, 0};
		GtkTreePath *path;
		GtkTreePath *path_selected;

		model_filter = gtk_tree_view_get_model(GTK_TREE_VIEW(treeview));
		
		// Get the selection
		selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview));
		nb_selected = gtk_tree_selection_count_selected_rows(selection);

		// Get the path where user has clicked
		if (gtk_tree_view_get_path_at_pos(GTK_TREE_VIEW(treeview),
						  (gint) event->x, (gint) event->y,
						  &path_selected, NULL, NULL, NULL)){
			if(!gtk_tree_selection_path_is_selected (selection, path_selected)){
				gtk_tree_selection_unselect_all (selection);
				gtk_tree_selection_select_path(selection, path_selected);
			}
			gtk_tree_path_free(path_selected);			
		}
		
		nb_selected = gtk_tree_selection_count_selected_rows(selection);
		list = gtk_tree_selection_get_selected_rows (selection, &model_filter);
		
		GList* iterator = NULL;
		iterator = g_list_first (list);

		while(iterator != NULL){
			GtkTreePath *real_path;
			GtkTreeIter iter;

			// Get the real path
			path = (GtkTreePath*)iterator->data;
			real_path = gtk_tree_model_filter_convert_path_to_child_path(GTK_TREE_MODEL_FILTER(model_filter), path);
			
			// Look the type of the data in the path
			FreetuxTVChannelsGroupInfos* pChannelsGroupInfos;
			FreetuxTVChannelInfos* pChannelInfos;
			gtk_tree_model_get_iter (GTK_TREE_MODEL(app->channelslist), &iter, real_path);
			gtk_tree_model_get(GTK_TREE_MODEL(app->channelslist), &iter, CHANNELSGROUP_COLUMN, &pChannelsGroupInfos, -1);
			if(pChannelsGroupInfos != NULL){
				switch(pChannelsGroupInfos->type)
				{
					case FREETUXTV_CHANNELSGROUP_TYPEGROUP_PLAYLIST :
						nbGroupsTab[TYPEGRP_PLAYLIST]++;
						break;
					case FREETUXTV_CHANNELSGROUP_TYPEGROUP_FAVORITES :
						nbGroupsTab[TYPEGRP_FAVORITE]++;
						break;
				}
				nbGroupsTab[TYPEGRP_COUNT]++;	
			}else{
				gtk_tree_model_get(GTK_TREE_MODEL(app->channelslist), &iter, CHANNEL_COLUMN, &pChannelInfos, -1);
				if(pChannelInfos != NULL){
					switch(pChannelInfos->channels_group->type)
					{
						case FREETUXTV_CHANNELSGROUP_TYPEGROUP_PLAYLIST :
							nbChannelsForGroupTab[TYPEGRP_PLAYLIST]++;
							break;
						case FREETUXTV_CHANNELSGROUP_TYPEGROUP_FAVORITES :
							nbChannelsForGroupTab[TYPEGRP_FAVORITE]++;
							break;
					}
					nbChannels++;
				}
			}			

			// Iterate next path
			iterator = g_list_next(iterator);
			
		}

		GtkWidget *pMenu = NULL;
		GtkWidget *pMenuItem;
		GtkWidget *pSubMenu;
		GtkWidget *pSubMenuItem;
		GtkWidget *pImage;
		
		if(nbGroupsTab[TYPEGRP_COUNT] > 0 && nbChannels == 0){
			pMenu = gtk_menu_new();
			
			// Refresh, delete and delete group channels is only for channels groups
			pMenuItem = gtk_image_menu_item_new_from_stock ("gtk-refresh", NULL);
			gtk_menu_append (GTK_MENU (pMenu), pMenuItem);					
			g_signal_connect(G_OBJECT(pMenuItem), "activate",
				G_CALLBACK(on_popupmenu_activated_refreshgroup), app);
			// Only for type playlist
			if(nbGroupsTab[TYPEGRP_COUNT] != nbGroupsTab[TYPEGRP_PLAYLIST]){
				gtk_widget_set_sensitive (pMenuItem, FALSE);
			}
			gtk_widget_show (pMenuItem);

			// Separator
			pMenuItem = gtk_separator_menu_item_new();
			gtk_menu_append (GTK_MENU (pMenu), pMenuItem);
			gtk_widget_show (pMenuItem);
			
			pMenuItem = gtk_image_menu_item_new_with_label(_("Delete group's channels"));
			pImage = gtk_image_new_from_stock (GTK_STOCK_DELETE, GTK_ICON_SIZE_MENU);
			gtk_image_menu_item_set_image (GTK_IMAGE_MENU_ITEM (pMenuItem), pImage);
			gtk_menu_append (GTK_MENU (pMenu), pMenuItem);
			g_signal_connect(G_OBJECT(pMenuItem), "activate",
				G_CALLBACK(on_popupmenu_activated_deletechannels), app);
			gtk_widget_show (pMenuItem);
			
			pMenuItem = gtk_image_menu_item_new_from_stock ("gtk-delete", NULL);
			gtk_menu_append (GTK_MENU (pMenu), pMenuItem);
			g_signal_connect(G_OBJECT(pMenuItem), "activate",
				G_CALLBACK(on_popupmenu_activated_deletegroup), app);
			gtk_widget_show (pMenuItem);
						
			// Group properties is only if one channels groups is selected
			pMenuItem = gtk_separator_menu_item_new();
			gtk_menu_append (GTK_MENU (pMenu), pMenuItem);
			gtk_widget_show (pMenuItem);
			
			pMenuItem = gtk_image_menu_item_new_from_stock ("gtk-properties", NULL);
			gtk_menu_append (GTK_MENU (pMenu), pMenuItem);
			g_signal_connect(G_OBJECT(pMenuItem), "activate",
				G_CALLBACK(on_popupmenu_activated_groupproperties), app);
			if(nbGroupsTab[TYPEGRP_COUNT] > 1){
				gtk_widget_set_sensitive (pMenuItem, FALSE);
			}
			gtk_widget_show (pMenuItem);
		}

		if(nbGroupsTab[TYPEGRP_COUNT] == 0 && nbChannels > 0){
			pMenu = gtk_menu_new();

			// Add to favourite is only for channels
			pMenuItem = gtk_image_menu_item_new_with_label(_("Add to favourites"));
			pImage = gtk_image_new_from_stock (GTK_STOCK_ADD, GTK_ICON_SIZE_MENU);
			gtk_image_menu_item_set_image (GTK_IMAGE_MENU_ITEM (pMenuItem), pImage);
			gtk_menu_append (GTK_MENU (pMenu), pMenuItem);
			gtk_widget_show (pMenuItem);
			
			// Sub menu to choose the group where add the channel
			pSubMenu = gtk_menu_new();
			gtk_menu_item_set_submenu(GTK_MENU_ITEM(pMenuItem), pSubMenu);
			gtk_widget_show(pSubMenu);
			
			// Add favourites to a new group
			pSubMenuItem = gtk_image_menu_item_new_with_label(_("New group"));
			pImage = gtk_image_new_from_stock (GTK_STOCK_NEW, GTK_ICON_SIZE_MENU);
			gtk_image_menu_item_set_image (GTK_IMAGE_MENU_ITEM (pSubMenuItem), pImage);
			gtk_menu_append (GTK_MENU (pSubMenu), pSubMenuItem);
			g_signal_connect(G_OBJECT(pSubMenuItem), "activate",
				G_CALLBACK(on_popupmenu_activated_addfavourites), app);
			gtk_widget_show (pSubMenuItem);
			
			// Separator
			pSubMenuItem = gtk_separator_menu_item_new ();
			gtk_menu_append (GTK_MENU (pSubMenu), pSubMenuItem);
			gtk_widget_show (pSubMenuItem);
			
			// Add each favourites group in the menu
			GList* pListGroupsPath;
			GtkTreeModel* model;
			FreetuxTVChannelsGroupInfos* pChannelsGroupInfos;
			GtkTreePath* pPath = NULL;
			GtkTreeIter treeiter;

			model = app->channelslist;
			
			pListGroupsPath = get_favourites_channels_groups_paths (app);
			GList* iter = g_list_first (pListGroupsPath);
			while(iter){
				pPath = iter->data;

				gtk_tree_model_get_iter (model, &treeiter, pPath);
				gtk_tree_model_get (model, &treeiter, CHANNELSGROUP_COLUMN, &pChannelsGroupInfos, -1);

				if(pChannelsGroupInfos){
					pSubMenuItem = gtk_image_menu_item_new_with_label(pChannelsGroupInfos->name);
					gtk_menu_append (GTK_MENU (pSubMenu), pSubMenuItem);

					g_object_set_data (G_OBJECT(pSubMenuItem), "FavouritesChannelsGroup", pChannelsGroupInfos);
					g_object_set_data (G_OBJECT(pSubMenuItem), "PathFavouritesChannelsGroup", pPath);
					g_signal_connect(G_OBJECT(pSubMenuItem), "activate",
						             G_CALLBACK(on_popupmenu_activated_addfavourites), app);
				
					gtk_widget_show (pSubMenuItem);
				}
				
				iter = iter->next;
			}
			
			// Separator
			pMenuItem = gtk_separator_menu_item_new();
			gtk_menu_append (GTK_MENU (pMenu), pMenuItem);
			gtk_widget_show (pMenuItem);

			// Delete channels from favourites group
			pMenuItem = gtk_image_menu_item_new_from_stock ("gtk-delete", NULL);
			gtk_menu_append (GTK_MENU (pMenu), pMenuItem);
			g_signal_connect(G_OBJECT(pMenuItem), "activate",
				G_CALLBACK(on_popupmenu_activated_deletechannel), app);
			if(nbChannelsForGroupTab[TYPEGRP_FAVORITE] != nbChannels){
				gtk_widget_set_sensitive (pMenuItem, FALSE);
			}
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
on_popupmenu_activated_refreshgroup (GtkMenuItem *menuitem, gpointer user_data)
{
	FreetuxTVApp *app = (FreetuxTVApp *)user_data;

	GError* error = NULL;

	GtkWidget* treeview;
	GtkTreeModel* model_filter;
	GtkTreeSelection *selection;
	GList *list;
	GList* iterator = NULL;
	GtkTreePath *path;
	GtkTreePath *real_path;
	
	DBSync dbsync;
	dbsync_open_db (&dbsync, &error);

	if(error == NULL){
		treeview =  (GtkWidget *) gtk_builder_get_object (app->gui,
								"windowmain_treeviewchannelslist");
		model_filter = gtk_tree_view_get_model(GTK_TREE_VIEW(treeview));
		
		// Get the selection
		selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview));
		list = gtk_tree_selection_get_selected_rows (selection, &model_filter);
		
		iterator = g_list_first (list);

		while(iterator != NULL && error == NULL){			

			// Get the real path
			path = (GtkTreePath*)iterator->data;
			real_path = gtk_tree_model_filter_convert_path_to_child_path(GTK_TREE_MODEL_FILTER(model_filter), path);
			
			// Refresh the group corresponding to the path
			channels_list_refresh_channels_group(app, real_path, &dbsync, &error);

			iterator = g_list_next(iterator);
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

static void
on_popupmenu_activated_deletechannels (GtkMenuItem *menuitem, gpointer user_data)
{
	FreetuxTVApp *app = (FreetuxTVApp *)user_data;

	GError* error = NULL;

	GtkWidget* treeview;
	GtkTreeModel* model_filter;
	GtkTreeSelection *selection;
	GList *list;
	GList* iterator = NULL;
	GtkTreePath *path;
	GtkTreePath *real_path;
	
	DBSync dbsync;
	dbsync_open_db (&dbsync, &error);

	if(error == NULL){
		treeview =  (GtkWidget *) gtk_builder_get_object (app->gui,
								"windowmain_treeviewchannelslist");
		model_filter = gtk_tree_view_get_model(GTK_TREE_VIEW(treeview));
		
		// Get the selection
		selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview));
		list = gtk_tree_selection_get_selected_rows (selection, &model_filter);
		
		iterator = g_list_first (list);

		while(iterator != NULL && error == NULL){
			
			// Get the real path
			path = (GtkTreePath*)iterator->data;
			real_path = gtk_tree_model_filter_convert_path_to_child_path(GTK_TREE_MODEL_FILTER(model_filter), path);
			
			// Delete the channels of the channels group corresponding to the path
			channels_list_delete_channels_channels_group(app, real_path, &dbsync, &error);

			iterator = g_list_next(iterator);
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


static void
on_popupmenu_activated_deletegroup (GtkMenuItem *menuitem, gpointer user_data)
{
	FreetuxTVApp *app = (FreetuxTVApp *)user_data;

	GError* error = NULL;

	GtkWidget* treeview;
	GtkTreeModel* model_filter;
	GtkTreeSelection *selection;
	GList *list;
	GList* iterator = NULL;
	GtkTreePath *path;
	GtkTreePath *real_path;
	
	DBSync dbsync;
	dbsync_open_db (&dbsync, &error);

	if(error == NULL){
		treeview =  (GtkWidget *) gtk_builder_get_object (app->gui,
								"windowmain_treeviewchannelslist");
		model_filter = gtk_tree_view_get_model(GTK_TREE_VIEW(treeview));
		
		// Get the selection
		selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview));
		list = gtk_tree_selection_get_selected_rows (selection, &model_filter);
		
		iterator = g_list_last (list);

		while(iterator != NULL && error == NULL){			

			// Get the real path
			path = (GtkTreePath*)iterator->data;
			real_path = gtk_tree_model_filter_convert_path_to_child_path(GTK_TREE_MODEL_FILTER(model_filter), path);
			
			// Delete the channels group corresponding to the path
			channels_list_delete_channels_group(app, real_path, &dbsync, &error);

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

static void
on_popupmenu_activated_groupproperties (GtkMenuItem *menuitem, gpointer user_data)
{
	FreetuxTVApp *app = (FreetuxTVApp *)user_data;
	
	GtkWidget *widget;
	GtkWidget* treeview;
	GtkTreeModel* model_filter;
	GtkTreeSelection *selection;
	GList *list;
	GList* iterator = NULL;
	GtkTreePath *path;
	GtkTreePath *real_path;
	
	FreetuxTVChannelsGroupInfos* pChannelsGroupInfos;

	treeview =  (GtkWidget *) gtk_builder_get_object (app->gui,
							  "windowmain_treeviewchannelslist");
	model_filter = gtk_tree_view_get_model(GTK_TREE_VIEW(treeview));
	
	// Get the selection
	selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview));
	list = gtk_tree_selection_get_selected_rows (selection, &model_filter);
	
	iterator = g_list_last (list);
	
	if(iterator != NULL){			
		
		// Get the real path
		path = (GtkTreePath*)iterator->data;
		real_path = gtk_tree_model_filter_convert_path_to_child_path(GTK_TREE_MODEL_FILTER(model_filter), path);
		
		// Show properties to the channels group corresponding to the path
		pChannelsGroupInfos = channels_list_get_group(app, real_path);

		// Set the value in the fields
		widget = (GtkWidget *) gtk_builder_get_object (app->gui,
							       "dialoggroupproperties_name");
		gtk_entry_set_text(GTK_ENTRY(widget), pChannelsGroupInfos->name);

		if(pChannelsGroupInfos->type == FREETUXTV_CHANNELSGROUP_TYPEGROUP_PLAYLIST){
			widget = (GtkWidget *) gtk_builder_get_object (app->gui,
									   "dialoggroupproperties_uri");
			gtk_entry_set_text(GTK_ENTRY(widget), pChannelsGroupInfos->uri);
		
			if(pChannelsGroupInfos->bregex != NULL){
				widget = (GtkWidget *) gtk_builder_get_object (app->gui,
										   "dialoggroupproperties_bregex");
				gtk_entry_set_text(GTK_ENTRY(widget), pChannelsGroupInfos->bregex);
			}
		
			if(pChannelsGroupInfos->eregex != NULL){
				widget = (GtkWidget *) gtk_builder_get_object (app->gui,
										   "dialoggroupproperties_eregex");
				gtk_entry_set_text(GTK_ENTRY(widget), pChannelsGroupInfos->eregex);
			}
			
			widget = (GtkWidget *) gtk_builder_get_object (app->gui,
									   "label34");
			gtk_widget_show (widget);
			widget = (GtkWidget *) gtk_builder_get_object (app->gui,
									   "dialoggroupproperties_uri");
			gtk_widget_show (widget);
			widget = (GtkWidget *) gtk_builder_get_object (app->gui,
									   "frame6");
			gtk_widget_show (widget);
		}

		if(pChannelsGroupInfos->type == FREETUXTV_CHANNELSGROUP_TYPEGROUP_FAVORITES){
			widget = (GtkWidget *) gtk_builder_get_object (app->gui,
									   "label34");
			gtk_widget_hide (widget);
			widget = (GtkWidget *) gtk_builder_get_object (app->gui,
									   "dialoggroupproperties_uri");
			gtk_widget_hide (widget);
			widget = (GtkWidget *) gtk_builder_get_object (app->gui,
									   "frame6");
			gtk_widget_hide (widget);
		}
		
		widget = (GtkWidget *) gtk_builder_get_object (app->gui,
							       "dialoggroupproperties_nbchannels");
		gchar* str;
		str = g_strdup_printf("%d", pChannelsGroupInfos->nb_channels);
		gtk_label_set_text(GTK_LABEL(widget), str);
		g_free(str);
		
		// Display the window
		widget = (GtkWidget *) gtk_builder_get_object (app->gui,
							       "dialoggroupproperties");
		gtk_widget_show (widget);
		
		g_object_set_data (G_OBJECT(widget), "ChannelsGroup", pChannelsGroupInfos);
		g_object_set_data (G_OBJECT(widget), "PathChannelsGroup", real_path);
	}
}

static void
on_popupmenu_activated_addfavourites (GtkMenuItem *menuitem, gpointer user_data)
{
	FreetuxTVApp *app = (FreetuxTVApp *)user_data;

	GError* error = NULL;

	GtkWidget* treeview;
	GtkTreeModel* model_filter;
	GtkTreeSelection *selection;
	GList *list;
	GList* iterator = NULL;
	GtkTreePath *path;
	GtkTreePath *real_path;
	GtkTreeIter treeiter;

	FreetuxTVChannelsGroupInfos* pFavouritesChannelsGroupInfos = NULL;
	GtkTreePath* pFavouritesChannelsGroupInfosPath = NULL;

	FreetuxTVChannelInfos* pOriginalChannelInfos = NULL;
	FreetuxTVChannelInfos* pChannelInfos = NULL;
	
	DBSync dbsync;
	dbsync_open_db (&dbsync, &error);

	treeview =  (GtkWidget *) gtk_builder_get_object (app->gui,
	    "windowmain_treeviewchannelslist");
	model_filter = gtk_tree_view_get_model(GTK_TREE_VIEW(treeview));

	// Get the selection
	selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview));
	list = gtk_tree_selection_get_selected_rows (selection, &model_filter);

	iterator = g_list_last (list);

	// Get the favourites group where add channels
	if(iterator != NULL){
		pFavouritesChannelsGroupInfos = g_object_get_data (G_OBJECT(menuitem), "FavouritesChannelsGroup");
		pFavouritesChannelsGroupInfosPath = g_object_get_data (G_OBJECT(menuitem), "PathFavouritesChannelsGroup");

		if(pFavouritesChannelsGroupInfos == NULL){
			pFavouritesChannelsGroupInfosPath = get_favourites_channels_group_new(app, &pFavouritesChannelsGroupInfos);
		}
	}

	if(pFavouritesChannelsGroupInfos != NULL){
		
		while(iterator != NULL && error == NULL){			

			// Get the real path
			path = (GtkTreePath*)iterator->data;
			real_path = gtk_tree_model_filter_convert_path_to_child_path(GTK_TREE_MODEL_FILTER(model_filter), path);

			gtk_tree_model_get_iter (app->channelslist, &treeiter, real_path);
			gtk_tree_model_get (app->channelslist, &treeiter, CHANNEL_COLUMN, &pOriginalChannelInfos, -1);

			if(pOriginalChannelInfos){
				g_log(FREETUXTV_LOG_DOMAIN, G_LOG_LEVEL_MESSAGE,
				      "Adding '%s' to favourites group '%s'\n", pOriginalChannelInfos->name, pFavouritesChannelsGroupInfos->name);

				// Copy the channel and add it in database
				pChannelInfos = freetuxtv_channel_infos_new (pOriginalChannelInfos->name, pOriginalChannelInfos->url);
				freetuxtv_channel_infos_set_position (pChannelInfos, pFavouritesChannelsGroupInfos->nb_channels+1);
				freetuxtv_channel_infos_set_logo (pChannelInfos, pOriginalChannelInfos->logo_name);
				freetuxtv_channel_infos_set_channels_group (pChannelInfos, pFavouritesChannelsGroupInfos);			
				freetuxtv_channel_infos_set_vlcoptions (pChannelInfos, pOriginalChannelInfos->vlc_options);

				// Add the channels in the groups
				dbsync_add_channel (&dbsync, pChannelInfos, FALSE, &error);

				if(error == NULL){
					pFavouritesChannelsGroupInfos->nb_channels++;

					// Add the channel in the treeview as the last element
					GtkTreeIter iterChannelsGroup;
					GtkTreeIter iterChannel;
					gtk_tree_model_get_iter (app->channelslist, &iterChannelsGroup,
							                 pFavouritesChannelsGroupInfosPath);
			
					gtk_tree_store_append (GTK_TREE_STORE(app->channelslist), &iterChannel, &iterChannelsGroup);
					gtk_tree_store_set (GTK_TREE_STORE(app->channelslist), &iterChannel,
							CHANNEL_COLUMN, pChannelInfos, -1);
				}
			}

			iterator = g_list_previous(iterator);
		}
	}
	
	dbsync_close_db(&dbsync);
	
	if(error != NULL){
		windowmain_show_gerror (app, error);
		g_error_free (error);
	}
}

static void
on_popupmenu_activated_deletechannel (GtkMenuItem *menuitem, gpointer user_data)
{
	FreetuxTVApp *app = (FreetuxTVApp *)user_data;

	GError* error = NULL;

	GtkWidget* treeview;
	GtkTreeModel* model_filter;
	GtkTreeSelection *selection;
	GList *list;
	GList* iterator = NULL;
	GtkTreePath *path;
	GtkTreePath *real_path;
	
	DBSync dbsync;
	dbsync_open_db (&dbsync, &error);

	if(error == NULL){
		treeview =  (GtkWidget *) gtk_builder_get_object (app->gui,
								"windowmain_treeviewchannelslist");
		model_filter = gtk_tree_view_get_model(GTK_TREE_VIEW(treeview));
		
		// Get the selection
		selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview));
		list = gtk_tree_selection_get_selected_rows (selection, &model_filter);
		
		iterator = g_list_last (list);

		while(iterator != NULL && error == NULL){			

			// Get the real path
			path = (GtkTreePath*)iterator->data;
			real_path = gtk_tree_model_filter_convert_path_to_child_path(GTK_TREE_MODEL_FILTER(model_filter), path);
			
			// Delete the channels group corresponding to the path
			channels_list_delete_channel(app, real_path, &dbsync, &error);

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

static GtkTreePath*
get_favourites_channels_group_new(FreetuxTVApp *app, FreetuxTVChannelsGroupInfos **ppChannelsGroupInfos)
{
	GtkTreePath* pTreePath = NULL;
	
	// No favourites group, ask to add one
	FreetuxTVWindowAddChannelsGroup* pWindowAddChannelsGroups;
	gint res;

	pWindowAddChannelsGroups = freetuxtv_window_add_channels_group_new (app);

	int allowedType;
	allowedType = FREETUXTV_WINDOW_ADD_CHANNELS_GROUP_ALLOW_FAVOURITES;
	freetuxtv_window_add_channels_group_set_allowed_type (pWindowAddChannelsGroups, allowedType);
	
	res = freetuxtv_window_add_channels_group_run (pWindowAddChannelsGroups);

	freetuxtv_window_add_channels_group_get_last_added (pWindowAddChannelsGroups,
	                                                    ppChannelsGroupInfos, &pTreePath);

	g_object_unref(pWindowAddChannelsGroups);
	pWindowAddChannelsGroups = NULL;

	return pTreePath;
}

static GList*
get_favourites_channels_groups_paths(FreetuxTVApp *app)
{
	GtkTreeModel* model;
	GtkTreeIter iter;
	GList* pListPath = NULL;
	
	FreetuxTVChannelsGroupInfos* pChannelsGroupInfos = NULL;
	GtkTreePath* pPath = NULL;
	
	model = app->channelslist;
	
	if(gtk_tree_model_get_iter_first (model, &iter)){

		do {
			gtk_tree_model_get (model, &iter, CHANNELSGROUP_COLUMN, &pChannelsGroupInfos, -1);
			if(pChannelsGroupInfos){
				if(pChannelsGroupInfos->type == FREETUXTV_CHANNELSGROUP_TYPEGROUP_FAVORITES){
					pPath = gtk_tree_model_get_path (model, &iter);
					pListPath = g_list_append (pListPath, pPath);
				}
			}
		}while(gtk_tree_model_iter_next(model, &iter));
	}
	
	return pListPath;
}

static gboolean
on_filter_channels_list (GtkTreeModel *model, GtkTreeIter *iter, gpointer data)
{
	FreetuxTVApp *app = (FreetuxTVApp *) data;
	
	FreetuxTVChannelsGroupInfos* channels_group_infos;
	FreetuxTVChannelInfos* channel_infos;

	GtkTreeIter iter_channelsgroup;

	GtkWidget *entryfilter;
	gchar *filter;
	entryfilter = (GtkWidget *) gtk_builder_get_object(app->gui,
							   "windowmain_entryfilter");
	filter = (gchar *)gtk_entry_get_text (GTK_ENTRY(entryfilter));
	
	gtk_tree_model_get(GTK_TREE_MODEL(model), iter, CHANNELSGROUP_COLUMN, &channels_group_infos, -1);
	if(channels_group_infos == NULL){
		gtk_tree_model_get(GTK_TREE_MODEL(model), iter, CHANNEL_COLUMN, &channel_infos, -1);
		
		if(channel_infos != NULL){
			
			// Recupere le groupe parent
			gtk_tree_model_iter_parent(GTK_TREE_MODEL(model), &iter_channelsgroup, iter);
			gtk_tree_model_get(GTK_TREE_MODEL(model), &iter_channelsgroup,
					   CHANNELSGROUP_COLUMN, &channels_group_infos, -1);

			gchar *channel = g_utf8_strdown (channel_infos->name,-1);
			gchar *search = g_strdup_printf("^.*%s.*$",
							g_utf8_strdown (filter,-1));
			GRegex *regex;			
			regex = g_regex_new (search, 0, 0 ,NULL);
			if (g_regex_match (regex, channel, 0, NULL)){
				g_regex_unref (regex);
				if(channels_group_infos != NULL){
					channels_group_infos->nb_channels_visible++;
				}
				return TRUE;
			}else{
				g_regex_unref (regex);
				return FALSE;
			}
			
		}		
	}else{	
		channels_group_infos->nb_channels_visible = 0;
		return TRUE;
		
	}
	return TRUE;
}

static int 
on_parsem3u_add_channel (char *url, int num, int argc, 
			 char **argv, void *user_data)
{
	Parsem3uData *data = (Parsem3uData *) user_data;
	FreetuxTVChannelsGroupInfos* channels_group_infos = data->channels_group_infos;

	int res;
	gchar *name;
	gchar **vlc_options = NULL;

	res = libm3uparser_get_extinfo (argc, argv, NULL, &name);
	if(res == LIBM3UPARSER_EXTINFO_NOT_FOUND){
		name = g_strdup_printf("Inconnu");
	}else{
		GRegex* gregex;
		gchar *tmp;
		gchar *regex;
		
		// Correct channel name with regex defined
		if(channels_group_infos->bregex != NULL){
			regex = g_strdup_printf("^%s", channels_group_infos->bregex);
			gregex = g_regex_new (regex, 0, 0, NULL);
			g_free(regex);
			tmp = g_regex_replace (gregex, name, -1, 0, "", 0, NULL);
			g_regex_unref (gregex);
		}else{
			tmp = g_strdup(name);
		}
		g_free(name);		

		if(channels_group_infos->eregex != NULL){
			regex = g_strdup_printf("%s$", channels_group_infos->eregex);
			gregex = g_regex_new (regex, 0, 0, NULL);
			g_free(regex);
			name = g_regex_replace (gregex, tmp, -1, 0, "", 0, NULL);
			g_regex_unref (gregex);
		}else{
			name = g_strdup(tmp);
		}
		g_free(tmp);

		// Get the VLC options for the channel
		char** ext_data = NULL;
		int ext_count;
		int i;
		libm3uparser_get_extdata (argc, argv, "#EXTVLCOPT", &ext_data, &ext_count);
		if(ext_count>0){
			vlc_options = (gchar**)g_malloc((ext_count+1) * sizeof(gchar*));
			vlc_options[ext_count] = NULL;
			for(i=0; i<ext_count; i++){
				vlc_options[i] = g_strdup_printf(":%s", ext_data[i]);
				g_free(ext_data[i]);
				ext_data[i] = NULL;
			}
			g_free(ext_data);
			ext_data = NULL;
		}
	}
	
	g_strstrip(name);	

	g_log(FREETUXTV_LOG_DOMAIN, G_LOG_LEVEL_DEBUG,
	      "Add channel '%s' in database\n", name);

	FreetuxTVChannelInfos* channel_infos;
	channel_infos = freetuxtv_channel_infos_new(name, url);
	freetuxtv_channel_infos_set_position(channel_infos, num);
	freetuxtv_channel_infos_set_channels_group(channel_infos, channels_group_infos);
	if(vlc_options){
		freetuxtv_channel_infos_set_vlcoptions(channel_infos, vlc_options);
		g_strfreev(vlc_options);
	}

	dbsync_add_channel (data->dbsync, channel_infos, TRUE, data->error);

	g_object_unref(channel_infos);

	return 0;
}

/* For debug only
static void
channels_list_print(FreetuxTVApp *app)
{
	GtkTreeModel* model;
	GtkTreeIter iter;
	GtkTreeIter iter2;
	
	FreetuxTVChannelsGroupInfos* pChannelsGroupInfos;
	FreetuxTVChannelInfos*		 pChannelInfos;
	
	model = app->channelslist;

	if(gtk_tree_model_get_iter_first (model, &iter)){

		// do {
			gtk_tree_model_get (model, &iter, CHANNELSGROUP_COLUMN, &pChannelsGroupInfos, -1);
			if(pChannelsGroupInfos){
				g_print("%d - %s\n", pChannelsGroupInfos->position, pChannelsGroupInfos->name);
			}
			if(gtk_tree_model_iter_has_child(model, &iter)){
				if(gtk_tree_model_iter_children(model, &iter2, &iter)){
					do {
						gtk_tree_model_get (model, &iter2, CHANNEL_COLUMN, &pChannelInfos, -1);
						if(pChannelInfos){
							g_print("\t%d - %s\n", pChannelInfos->position, pChannelInfos->name);
						}
					}while(gtk_tree_model_iter_next(model, &iter2));
				}
			}
		// }while(gtk_tree_model_iter_next(model, &iter));
	}
}
*/