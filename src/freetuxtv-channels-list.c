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

#include <gtk/gtk.h>

#include "freetuxtv-channels-list.h"

#include "freetuxtv-app.h"
#include "freetuxtv-i18n.h"
#include "freetuxtv-channel-infos.h"
#include "freetuxtv-channels-group-infos.h"
#include "freetuxtv-cellrenderer-channelslist.h"
#include "freetuxtv-logos-list.h"
#include "freetuxtv-db-sync.h"
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

typedef struct _CBUserData
{
	FreetuxTVApp *app;
	int nb;
	GtkTreeIter *iter_channelsgroup;
	GtkTreeIter *iter_channel; 
} CBUserData;

enum
{
	CHANNELSGROUP_COLUMN,
	CHANNEL_COLUMN,
	N_COLUMNS
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
on_row_activated_channels_list (GtkTreeView        *view, GtkTreePath *path,
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

static gboolean
on_filter_channels_list (GtkTreeModel *model, GtkTreeIter *iter, gpointer data);


static int 
on_exec_get_group_infos (void *data, int argc, char **argv, char **colsname);

static int 
on_parsem3u_add_channel (char *url, int num, int argc, 
			 char **argv, void *user_data);

static void
channels_group_get_file (FreetuxTVChannelsGroupInfos *self, gchar **filename,
			 gboolean cache, GError** error);

static void
channels_list_display_channels (FreetuxTVApp *app);

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
channels_list_add_channels_group (FreetuxTVApp *app, FreetuxTVChannelsGroupInfos* channels_group_infos,
				  DBSync* dbsync, GError** error)
{
	g_return_if_fail(dbsync != NULL);
	g_return_if_fail(error != NULL);
	
	if(*error == NULL){
		dbsync_add_channels_group (dbsync, channels_group_infos, error);
	}

	// Add group in the treeview
	if(*error == NULL){	
		GtkTreeIter iter_channelsgroup;
		gtk_tree_store_append (GTK_TREE_STORE(app->channelslist), &iter_channelsgroup, NULL);
		gtk_tree_store_set (GTK_TREE_STORE(app->channelslist), &iter_channelsgroup,
				    CHANNELSGROUP_COLUMN, channels_group_infos, -1);

		GtkTreePath* path;
		path = gtk_tree_model_get_path(app->channelslist, &iter_channelsgroup);
		
		channels_list_refresh_channels_group(app, path, dbsync, error);
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

	FreetuxTVChannelsGroupInfos* channels_group_infos;
	channels_group_infos = channels_list_get_group (app, path_group);

	gchar *text;

	// Update status bar
	text = g_strdup_printf(_("Update \"%s\" channels list"), channels_group_infos->name);
	windowmain_statusbar_push (app, "UpdateMsg", text);
	g_free(text);
	g_print("FreetuxTV : Start updating \"%s\" channels list\n", channels_group_infos->name);

	// Get the file of the playlist
	text = g_strdup_printf (_("Getting the file : \"%s\""), channels_group_infos->uri);
	windowmain_statusbar_push (app, "UpdateMsg", text);
	g_free(text);
	g_print("FreetuxTV : Getting the file \"%s\"\n", channels_group_infos->uri);

	char *file;
	channels_group_get_file (channels_group_infos, &file, TRUE, error);		
	windowmain_statusbar_pop (app, "UpdateMsg");

	// Delete channels of the channels group in the database 
	if(*error == NULL){
		dbsync_delete_channels_of_channels_group (dbsync, channels_group_infos, error);
	}
	
	// Parse the M3U file and add channels in the database
	if(*error == NULL){
		Parsem3uData pdata;
		pdata.channels_group_infos = channels_group_infos;
		pdata.dbsync = dbsync;
		pdata.app = app;
		pdata.error = error;
		int res = 0;

		g_print("FreetuxTV : Parsing the file \"%s\"\n", file);
		res = libm3uparser_parse(file, &on_parsem3u_add_channel, &pdata);
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
		// Delete channels of the channels group in the treeview
		channels_list_delete_rows (app, path_group, TRUE);

		// Display the channels from the database
		CBIterData iter_data;
		GtkTreeIter iter_group;
		GtkTreeIter iter_channel;
		gtk_tree_model_get_iter (GTK_TREE_MODEL(app->channelslist), &iter_group, path_group);
		
		iter_data.iter = &iter_channel;
		iter_data.iter_parent = &iter_group;

		dbsync_select_channels_of_channels_group (dbsync, channels_group_infos, app, on_dbsync_add_channel, &iter_data, error);
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

	FreetuxTVChannelsGroupInfos* channels_group_infos;
	channels_group_infos = channels_list_get_group (app, path_group);
		
	// Delete group in the database
	if(*error == NULL){
		dbsync_delete_channels_group (dbsync, channels_group_infos, error);
	}

	// Delete group in the treeview
	if(*error == NULL){
		channels_list_delete_rows (app, path_group, FALSE);
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
	GtkWidget *treeview;
	GtkTreeIter iter;

	GtkTreeModel *model;
	GtkTreePath* filter_path;

	if(path_channel != NULL){
		treeview = (GtkWidget *) gtk_builder_get_object(app->gui,
								"windowmain_treeviewchannelslist");
		
		model = gtk_tree_view_get_model (GTK_TREE_VIEW(treeview));
		
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
	}
}

FreetuxTVChannelInfos*
channels_list_get_channel(FreetuxTVApp *app, GtkTreePath* path_channel)
{
	FreetuxTVChannelInfos* channel_infos;
	GtkTreeIter iter;
	g_return_if_fail(gtk_tree_model_get_iter (GTK_TREE_MODEL(app->channelslist), &iter, path_channel) == TRUE);
	gtk_tree_model_get(GTK_TREE_MODEL(app->channelslist), &iter, CHANNEL_COLUMN, &channel_infos, -1);
	return channel_infos;
}


FreetuxTVChannelsGroupInfos*
channels_list_get_group(FreetuxTVApp *app, GtkTreePath* path_group)
{
	FreetuxTVChannelsGroupInfos* channels_group_infos;
	GtkTreeIter iter;
	g_return_if_fail(gtk_tree_model_get_iter (GTK_TREE_MODEL(app->channelslist), &iter, path_group) == TRUE);
	gtk_tree_model_get(GTK_TREE_MODEL(app->channelslist), &iter, CHANNELSGROUP_COLUMN, &channels_group_infos, -1);
	return channels_group_infos;	
}

static void 
channels_list_delete_rows (FreetuxTVApp *app, GtkTreePath* path_group,
			   gboolean justchannels)
{
	GtkTreeIter iter_channelsgroup;
	
	// Selectionne le groupe
	gtk_tree_model_get_iter (GTK_TREE_MODEL(app->channelslist), &iter_channelsgroup, path_group);
	
	// Supprime les chaine du modele si il y en a
	if(gtk_tree_model_iter_has_child (GTK_TREE_MODEL(app->channelslist), &iter_channelsgroup)){
		GtkTreeIter iter_tmp;
		gtk_tree_model_iter_children (GTK_TREE_MODEL(app->channelslist), &iter_tmp, &iter_channelsgroup);
		while(gtk_tree_store_remove (GTK_TREE_STORE(app->channelslist), &iter_tmp)){};
	}

	// Supprime tout le groupe
	if(!justchannels){
		gtk_tree_store_remove (GTK_TREE_STORE(app->channelslist), &iter_channelsgroup);
	}
	
}

static void
channels_list_display_channels (FreetuxTVApp *app)
{
	GtkWidget *treeview;
	treeview = (GtkWidget *) gtk_builder_get_object(app->gui,
							"windowmain_treeviewchannelslist");
	gtk_tree_view_expand_all (GTK_TREE_VIEW(treeview));
}

static void
channels_group_get_file (FreetuxTVChannelsGroupInfos *self, gchar **file,
			 gboolean cache, GError** error)
{
	gchar *groupfile;
	groupfile = g_strdup_printf("%s/FreetuxTV/cache/playlist-group-%d.dat",
				    g_get_user_config_dir(), self->id);
	freetuxtv_fileutils_get_file (self->uri, groupfile, error);

	*file = groupfile;
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
on_dbsync_add_channel(FreetuxTVApp *app, FreetuxTVChannelInfos* channel_infos,
		      DBSync *dbsync, gpointer user_data, GError** error)
{
	gboolean no_err = TRUE;
	
	if(error != NULL){
		if(*error != NULL){
			no_err = FALSE;
		}
	}

	if(app->debug == TRUE){
		g_print("FreetuxTV-debug : Add channel %d->'%s' to the model\n",
			channel_infos->id,
			channel_infos->name);
	}
	
	if(no_err){
		CBIterData *iter_data = (CBIterData *)user_data;

		gtk_tree_store_append (GTK_TREE_STORE(app->channelslist),
				       iter_data->iter, iter_data->iter_parent);
		gtk_tree_store_set (GTK_TREE_STORE(app->channelslist), iter_data->iter,
				    CHANNEL_COLUMN, channel_infos, -1);

		// Get the path of the channel to play, if needed
		if(app->current.lastchannelonstartup == TRUE
		   && app->config.lastchannel != -1){
			if(channel_infos->id == app->config.lastchannel){
				GtkTreePath* path;
				path = gtk_tree_model_get_path(GTK_TREE_MODEL(app->channelslist), iter_data->iter);
				app->current.path_channel = path;
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
	
	GtkTreeIter iter;
	gtk_tree_model_get_iter (GTK_TREE_MODEL(app->channelslist), &iter, path);

	FreetuxTVChannelsGroupInfos* channels_group_infos;
	FreetuxTVChannelInfos* channel_infos;

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
		freetuxtv_play_channel (app, model_path);		
	}
	gtk_tree_path_free(model_path);
}

static void 
on_row_displayed_channels_list(GtkTreeViewColumn *col,
			       GtkCellRenderer *renderer, GtkTreeModel *model,
			       GtkTreeIter *iter, gpointer user_data)
{
	FreetuxTVApp *app = (FreetuxTVApp *) user_data;
	
	FreetuxTVChannelsGroupInfos* channels_group_infos = NULL;
	FreetuxTVChannelInfos* channel_infos = NULL;
	
	GdkPixbuf* logo;
	GdkColor color;	

	// Si on veut afficher un groupe
	gtk_tree_model_get(model, iter, CHANNELSGROUP_COLUMN, &channels_group_infos, -1);
	if(channels_group_infos != NULL){
		g_object_set(renderer, "type", CELLRENDERER_TYPE_CHANNELS_GROUP, "name", channels_group_infos->name, "visible", TRUE, NULL);
	}else{
		gtk_tree_model_get(model, iter, CHANNEL_COLUMN, &channel_infos, -1);  

		gchar *imgfile;
		imgfile = logos_list_get_channel_logo_filename(app, channel_infos, TRUE);

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
		gint nb_channels = 0;
		gint nb_groups = 0;
		gint nb_selected = 0;
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
			FreetuxTVChannelsGroupInfos* channels_group_infos;
			FreetuxTVChannelInfos* channel_infos;
			gtk_tree_model_get_iter (GTK_TREE_MODEL(app->channelslist), &iter, real_path);
			gtk_tree_model_get(GTK_TREE_MODEL(app->channelslist), &iter, CHANNELSGROUP_COLUMN, &channels_group_infos, -1);
			if(channels_group_infos != NULL){
				nb_groups++;	
			}else{
				gtk_tree_model_get(GTK_TREE_MODEL(app->channelslist), &iter, CHANNEL_COLUMN, &channel_infos, -1);
				if(channel_infos != NULL){
					nb_channels++;
				}
			}			

			// Iterate next path
			iterator = g_list_next(iterator);
			
		}

		GtkWidget *menu = NULL;
		GtkWidget *menuitem;
		GtkWidget *image;
		
		if(nb_groups > 0 && nb_channels == 0){
			menu = gtk_menu_new();
			
			// Refresh, delete and delete group channels is only for channels groups
			menuitem = gtk_image_menu_item_new_from_stock ("gtk-refresh", NULL);
			gtk_menu_append (GTK_MENU (menu), menuitem);					
			g_signal_connect(G_OBJECT(menuitem), "activate",
					 G_CALLBACK(on_popupmenu_activated_refreshgroup), app);
			gtk_widget_show (menuitem);
			
			menuitem = gtk_separator_menu_item_new();
			gtk_menu_append (GTK_MENU (menu), menuitem);
			gtk_widget_show (menuitem);
			
			menuitem = gtk_image_menu_item_new_with_label(_("Delete group's channels"));
			image = gtk_image_new_from_stock (GTK_STOCK_DELETE, GTK_ICON_SIZE_MENU);
			gtk_image_menu_item_set_image (GTK_IMAGE_MENU_ITEM (menuitem), image);
			gtk_menu_append (GTK_MENU (menu), menuitem);
			g_signal_connect(G_OBJECT(menuitem), "activate",
					 G_CALLBACK(on_popupmenu_activated_deletechannels), app);
			gtk_widget_show (menuitem);
			
			menuitem = gtk_image_menu_item_new_from_stock ("gtk-delete", NULL);
			gtk_menu_append (GTK_MENU (menu), menuitem);
			g_signal_connect(G_OBJECT(menuitem), "activate",
						 G_CALLBACK(on_popupmenu_activated_deletegroup), app);
			gtk_widget_show (menuitem);
						
			// Group properties is only if one channels groups is selected 
			if(nb_groups == 1){
				menuitem = gtk_separator_menu_item_new();
				gtk_menu_append (GTK_MENU (menu), menuitem);
				gtk_widget_show (menuitem);
				
				menuitem = gtk_image_menu_item_new_from_stock ("gtk-properties", NULL);
				gtk_menu_append (GTK_MENU (menu), menuitem);
				g_signal_connect(G_OBJECT(menuitem), "activate",
						 G_CALLBACK(on_popupmenu_activated_groupproperties), app);
				gtk_widget_show (menuitem);
			}
		}

		if(menu){
			gtk_widget_show(menu);
			
			gtk_menu_popup(GTK_MENU(menu), NULL, NULL, NULL, NULL,
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
	
	FreetuxTVChannelsGroupInfos* group;

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
		group = channels_list_get_group(app, real_path);

		// Set the value in the fields
		widget = (GtkWidget *) gtk_builder_get_object (app->gui,
							       "dialoggroupproperties_name");
		gtk_entry_set_text(GTK_ENTRY(widget), group->name);
		
		widget = (GtkWidget *) gtk_builder_get_object (app->gui,
							       "dialoggroupproperties_uri");
		gtk_entry_set_text(GTK_ENTRY(widget), group->uri);
		
		if(group->bregex != NULL){
			widget = (GtkWidget *) gtk_builder_get_object (app->gui,
								       "dialoggroupproperties_bregex");
			gtk_entry_set_text(GTK_ENTRY(widget), group->bregex);
		}
		
		if(group->eregex != NULL){
			widget = (GtkWidget *) gtk_builder_get_object (app->gui,
								       "dialoggroupproperties_eregex");
			gtk_entry_set_text(GTK_ENTRY(widget), group->eregex);
		}
		
		widget = (GtkWidget *) gtk_builder_get_object (app->gui,
							       "dialoggroupproperties_nbchannels");
		gchar* str;
		str = g_strdup_printf("%d", group->nb_channels);
		gtk_label_set_text(GTK_LABEL(widget), str);
		g_free(str);
		
		// Display the window
		widget = (GtkWidget *) gtk_builder_get_object (app->gui,
							       "dialoggroupproperties");
		gtk_widget_show (widget);
		
		g_object_set_data (G_OBJECT(widget), "ChannelsGroup", group);
		g_object_set_data (G_OBJECT(widget), "PathChannelsGroup", real_path);
	}
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
		channels_group_infos->nb_channels_visible == 0;
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

	gchar* query;
	int res;
	char *err=0;
	gchar *name;	

	res = libm3uparser_get_extinfo (argc, argv, NULL, &name);
	if(res == LIBM3UPARSER_EXTINFO_NOT_FOUND){
		name = g_strdup_printf("Inconnu");
	}else{
		GRegex* gregex;
		gchar *tmp;
		gchar *regex;
		
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
	}
	
	g_strstrip(name);	
	
	if(data->app->debug == TRUE){
		g_print("FreetuxTV-debug : Add channel '%s' in database\n", name);
	}

	FreetuxTVChannelInfos* channel_infos;
	channel_infos = freetuxtv_channel_infos_new(name, url);
	freetuxtv_channel_infos_set_order(channel_infos, num);
	freetuxtv_channel_infos_set_channels_group(channel_infos, channels_group_infos);

	dbsync_add_channel (data->dbsync, channel_infos, data->error);

	g_object_unref(channel_infos);

	return 0;
}
