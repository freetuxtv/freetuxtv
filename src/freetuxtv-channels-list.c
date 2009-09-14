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
#include <sqlite3.h>
#include <curl/curl.h>

#include "freetuxtv-app.h"
#include "freetuxtv-i18n.h"
#include "freetuxtv-channel-infos.h"
#include "freetuxtv-channels-group-infos.h"
#include "freetuxtv-channels-list.h"
#include "freetuxtv-cellrenderer-channelslist.h"
#include "freetuxtv-logos-list.h"
#include "lib-m3uparser.h"

typedef struct _Parsem3uData
{
	FreetuxTVApp *app;
	gint id;
	struct sqlite3 *db;
	gchar *bregex;
	gchar *eregex;	
} Parsem3uData;

typedef struct _CBUserData
{
	FreetuxTVApp *app;
	int nb;
	GtkTreeIter *iter_channelsgroup;
	GtkTreeIter *iter_channel; 
} CBUserData;

typedef struct _CBGroupData
{
	FreetuxTVApp *app;
	GtkTreePath *path_group;
} CBGroupData;

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
on_exec_add_channels_group (void *data, int argc, char **argv, char **colsname);

static int 
on_exec_add_channel (void *data, int argc, char **argv, char **colsname);

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

static gboolean
on_filter_channels_list (GtkTreeModel *model, GtkTreeIter *iter, gpointer data);


static int 
on_exec_get_group_infos (void *data, int argc, char **argv, char **colsname);

static int 
on_parsem3u_add_channel (char *url, int num, int argc, 
			 char **argv, void *user_data);

static int
channels_group_get_file (FreetuxTVChannelsGroupInfos *self, FreetuxTVApp *app,
			 gchar **filename, gboolean cache);

static void
channels_list_display_channels (FreetuxTVApp *app);


void
channels_list_init (FreetuxTVApp *app)
{
	GtkWidget *treeview;
	GtkTreeViewColumn *column;
	GtkCellRenderer *renderer;
	GtkTreeModel *model;
	GtkTreeStore *channelslist;
	
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
channels_list_load_channels (FreetuxTVApp *app)
{
	struct sqlite3 *db;
	int res;
	char *err=0;
	gint ret = 0;
	gchar *query;

	gchar *err_msg = NULL;

	gchar *user_db;
	user_db = g_strconcat(g_get_user_config_dir(), 
			      "/FreetuxTV/freetuxtv.db", NULL);
	
	/* Ouverture de la BDD */
	res = sqlite3_open(user_db,&db);
	if(res != SQLITE_OK){
		err_msg = g_strdup_printf(_("Error when displaying the channels.\n\nSQLite has returned error :\n%s."),
					  sqlite3_errmsg(db));
		windowmain_show_error (app, err_msg);
		g_free(err_msg);
		
		ret = -1;
	}

	if(ret == 0){
		if(app->debug == TRUE){
			g_print("FreetuxTV-debug : Load all groups from database\n");
		}
		
		// Efface les groupes dans la liste des chaines
		gtk_tree_store_clear(GTK_TREE_STORE(app->channelslist));
		GtkTreeIter iter_channelsgroup;
		
		CBUserData cbuserdata;
		cbuserdata.app = app;
		cbuserdata.nb = 0;
		cbuserdata.iter_channelsgroup = &iter_channelsgroup;

		// Selection des groupes de chaînes 
		query = "SELECT id_channelsgroup, name_channelsgroup, \
                         uri_channelsgroup				      \
                         FROM channels_group";
		res=sqlite3_exec(db, query, on_exec_add_channels_group,
				 (void *)&cbuserdata, &err);
		if(res != SQLITE_OK){
			err_msg = g_strdup_printf(_("Error when displaying the channels.\n\nSQLite has returned error :\n%s."),
						  sqlite3_errmsg(db));
			windowmain_show_error (app, err_msg);
			g_free(err_msg);

			ret = -1;
		}
	}
	
	sqlite3_free(err);
	sqlite3_close(db);

	g_free(user_db);
	
	if(ret == 0){
		channels_list_display_channels (app);
	}
}

void
channels_list_load_channels_group (FreetuxTVApp *app, GtkTreePath *path_group)
{
	FreetuxTVChannelsGroupInfos* channels_group_infos;
	channels_group_infos = channels_list_get_group (app, path_group);
	
	struct sqlite3 *db;
	int res;
	char *err=0;
	gint ret = 0;
	gchar *query;
	
	gchar *err_msg = NULL;

	gchar *user_db;
	user_db = g_strconcat(g_get_user_config_dir(), 
			      "/FreetuxTV/freetuxtv.db", NULL);
	
	// Ouverture de la BDD
	res = sqlite3_open(user_db,&db);
	if(res != SQLITE_OK){		
		err_msg = g_strdup_printf(_("Error when displaying the channels of the group \"%s\".\n\nSQLite has returned error :\n%s."),
					  channels_group_infos->name,
					  sqlite3_errmsg(db));
		windowmain_show_error (app, err_msg);
		g_free(err_msg);
		
		ret = -1;
	}
	
	if(ret == 0){
		if(app->debug == TRUE){
			g_print("FreetuxTV-debug : Load channels group %d->'%s' from database\n",
				channels_group_infos->id,
				channels_group_infos->name);
		}

		GtkTreeIter iter_channelsgroup;
		GtkTreeIter iter_channel;
				
		GtkTreePath *path;
		
		// Selectionne le groupe
		path = gtk_tree_path_new_from_indices(channels_group_infos->rank - 1, -1);
		gtk_tree_model_get_iter (GTK_TREE_MODEL(app->channelslist), &iter_channelsgroup, path);

		// Supprime les lignes attaché au groupe
		channels_list_delete_rows (app, path, TRUE);
		
		// Selection des chaines
		CBUserData cbuserdata;
		cbuserdata.app = app;
		cbuserdata.nb = 0;
		cbuserdata.iter_channelsgroup =  &iter_channelsgroup;
		cbuserdata.iter_channel = &iter_channel;
		
		query = sqlite3_mprintf("SELECT id_channel, name_channel, \
                                 filename_channellogo,			\
                                 uri_channel FROM channel LEFT JOIN channel_logo \
                                 ON id_channellogo=idchannellogo_channel \
                                 WHERE channelsgroup_channel=%d\
                                 ORDER BY order_channel",
					channels_group_infos->id);

		res = sqlite3_exec(db, query, on_exec_add_channel,
				   (void *)&cbuserdata, &err);
		sqlite3_free(query);
		
		if(res != SQLITE_OK){
			err_msg = g_strdup_printf(_("Error when displaying the channels of the group \"%s\".\n\nSQLite has returned error :\n%s."),
						  channels_group_infos->name,
						  sqlite3_errmsg(db));
			windowmain_show_error (app, err_msg);
			g_free(err_msg);
			
			ret = -1;
		}
	}
		
	if(ret == 0){
		GtkWidget *treeview;
		treeview = (GtkWidget *)gtk_builder_get_object (app->gui,
								"windowmain_treeviewchannelslist");
		gtk_tree_view_expand_row (GTK_TREE_VIEW(treeview), path_group, FALSE);
	}
	
	sqlite3_free(err);
	sqlite3_close(db);
	g_free(user_db);
	
}

void
channels_list_add_channels_group (FreetuxTVApp *app, 
				  FreetuxTVChannelsGroupInfos* channels_group_infos)
{
	struct sqlite3 *db;
	int res;
	char *err = 0;
	gint ret = 0;
	gchar *query;

	gchar *err_msg = NULL;

	gchar *user_db;

	user_db = g_strconcat(g_get_user_config_dir(), 
			      "/FreetuxTV/freetuxtv.db", NULL);

	// Ouverture de la BDD
	res = sqlite3_open(user_db, &db);
	if(res != SQLITE_OK){
		err_msg = g_strdup_printf(_("Cannot open database.\n\nSQLite has returned error :\n%s."),
					  sqlite3_errmsg(db));
		windowmain_show_error (app, err_msg);
		g_free(err_msg);

		ret = -1;
	}

	// Ajout le nouveau groupe
	if (ret == 0){
		query = sqlite3_mprintf("INSERT INTO channels_group (name_channelsgroup, uri_channelsgroup, bregex_channelsgroup, eregex_channelsgroup) VALUES ('%q','%q', '%q', '%q');", 
					channels_group_infos->name,
					channels_group_infos->uri,
					channels_group_infos->bregex,
					channels_group_infos->eregex
					);
		res=sqlite3_exec(db, query, NULL, 0, &err);
		sqlite3_free(query);
		if(res != SQLITE_OK){
			err_msg = g_strdup_printf(_("Cannot add the group \"%s\" in database.\n\nSQLite has returned error :\n%s."),
						  channels_group_infos->name,
						  sqlite3_errmsg(db));
			windowmain_show_error (app, err_msg);
			g_free(err_msg);
			ret = -1;
		}else{
			freetuxtv_channels_group_infos_set_id (channels_group_infos,
							       (int)sqlite3_last_insert_rowid(db));
		}

		sqlite3_free(err);
		sqlite3_close(db);
	}

	if (ret == 0){
	
		GtkTreeIter iter_channelsgroup;
	
		GtkTreePath *path;
		gtk_tree_model_get_iter_first(GTK_TREE_MODEL(app->channelslist), &iter_channelsgroup);

		int rank = 1;

		while (gtk_tree_model_iter_next(GTK_TREE_MODEL(app->channelslist), &iter_channelsgroup)){
			rank++;
		}

		freetuxtv_channels_group_infos_set_rank (channels_group_infos,
							 rank + 1);

		gtk_tree_store_append (GTK_TREE_STORE(app->channelslist), &iter_channelsgroup, NULL);
		gtk_tree_store_set (GTK_TREE_STORE(app->channelslist), &iter_channelsgroup,
				    CHANNELSGROUP_COLUMN, channels_group_infos, -1);
		
	}	
}

void
channels_list_update_channels_group (FreetuxTVApp *app, GtkTreePath *path_group)
{
	FreetuxTVChannelsGroupInfos* channels_group_infos;
	channels_group_infos = channels_list_get_group (app, path_group);
	
	struct sqlite3 *db;
	int res;
	char *err = 0;
	gint ret = 0;
	gchar *query;

	gchar *err_msg = NULL;

	gchar *user_db;
	
	user_db = g_strconcat(g_get_user_config_dir(), 
			      "/FreetuxTV/freetuxtv.db", NULL);

	// Mise à jour de la barre de statut
	gchar *text;
	text = g_strdup_printf(_("Update \"%s\" channels list"), channels_group_infos->name);
	windowmain_statusbar_push (app, "UpdateMsg", text);
	g_free(text);
	g_print("FreetuxTV : Start updating \"%s\" channels list\n", channels_group_infos->name);

	// Ouverture de la BDD
	res = sqlite3_open(user_db, &db);
	if(res != SQLITE_OK){
		err_msg = g_strdup_printf(_("Cannot open database.\n\nSQLite has returned error :\n%s."),
					  sqlite3_errmsg(db));
		windowmain_show_error (app, err_msg);
		g_free(err_msg);

		ret = -1;
	}
	
	char *file;
	if(ret == 0){
		res = channels_group_get_file (channels_group_infos, app, &file, TRUE);
		if (res < 0 || file == NULL ){
			ret = -1;
		}
	}

	if (ret == 0){

		// Effacement des chaînes du groupe
		query = sqlite3_mprintf("DELETE FROM channel WHERE channelsgroup_channel=%d",
					channels_group_infos->id);
		res = sqlite3_exec(db, query, NULL, 0, &err);
		sqlite3_free(query);
		if(res != SQLITE_OK){

			err_msg = g_strdup_printf(_("Error when deleting the channels of the group \"%s\".\n\nSQLite has returned error :\n%s."),
						  channels_group_infos->name,
						  sqlite3_errmsg(db));
			windowmain_show_error (app, err_msg);
			g_free(err_msg);
			
			ret = -1;
		}
	}

	Parsem3uData *pdata;
	pdata = g_new0(Parsem3uData, 1);
	pdata->id = channels_group_infos->id;
	pdata->db = db;
	pdata->app = app;

	if (ret == 0){
		// Recupère les informations du groupe
		query = sqlite3_mprintf("SELECT bregex_channelsgroup, eregex_channelsgroup FROM channels_group WHERE id_channelsgroup=%d",
					channels_group_infos->id);
		res = sqlite3_exec(db, query, on_exec_get_group_infos,
				   (void *)pdata, &err);
		sqlite3_free(query);
		if(res != SQLITE_OK){
			err_msg = g_strdup_printf(_("Error when loading informations of the group \"%s\".\n\nSQLite has returned error :\n%s."),
						  channels_group_infos->name,
						  sqlite3_errmsg(db));
			windowmain_show_error (app, err_msg);
			g_free(err_msg);
			
			ret = -1;			
		}		
	}

	if (ret == 0){
		g_print("FreetuxTV : Parsing the file \"%s\"\n", file);
		// Parse le fichier M3U et ajoute les chaînes dans la BDD
		res = libm3uparser_parse(file, &on_parsem3u_add_channel, pdata);
		if (res < 0 ){
			
			if (res == LIBM3UPARSER_CALLBACK_RETURN_ERROR){

				err_msg = g_strdup_printf(_("Error when adding the channels.\n\nM3UParser has returned error :\n%s."),
							  libm3uparser_errmsg(ret));
				windowmain_show_error (app, err_msg);
				g_free(err_msg);
				ret = -1;

				
			}else{
				
				err_msg = g_strdup_printf(_("Error when adding the channels.\n\nM3UParser has returned error :\n%s."),
							  sqlite3_errmsg(db));
				windowmain_show_error (app, err_msg);
				g_free(err_msg);
				ret = -1;
			}			
		}
	}
	g_free (pdata);

	sqlite3_free(err);
	sqlite3_close(db);
	
	g_free(user_db);

	if (ret == 0){		
		channels_list_load_channels_group(app, path_group);
	}
	windowmain_statusbar_pop (app, "UpdateMsg");	
}


void
channels_list_delete_channels_channels_group (FreetuxTVApp *app, GtkTreePath *path_group)
{
	FreetuxTVChannelsGroupInfos* channels_group_infos;
	channels_group_infos = channels_list_get_group (app, path_group);
	
	struct sqlite3 *db;
	int res;
	char *err = 0;
	gint ret = 0;
	gchar *query;

	gchar *err_msg = NULL;
	
	gchar *user_db;
	user_db = g_strconcat(g_get_user_config_dir(), 
			      "/FreetuxTV/freetuxtv.db", NULL);

	// Ouverture de la BDD
	res = sqlite3_open(user_db, &db);
	if(res != SQLITE_OK){
		err_msg = g_strdup_printf(_("Cannot open database.\n\nSQLite has returned error :\n%s."),
					  sqlite3_errmsg(db));
		windowmain_show_error (app, err_msg);
		g_free(err_msg);

		ret = -1;
	}
	
	if(ret == 0){
		// Effacement des chaînes du groupe
		query = sqlite3_mprintf("DELETE FROM channel WHERE channelsgroup_channel=%d",
					channels_group_infos->id);
		res = sqlite3_exec(db, query, NULL, 0, &err);
		sqlite3_free(query);
		if(res != SQLITE_OK){

			err_msg = g_strdup_printf(_("Error when deleting the channels of the group \"%s\".\n\nSQLite has returned error :\n%s."),
						  channels_group_infos->name,
						  sqlite3_errmsg(db));
			windowmain_show_error (app, err_msg);
			g_free(err_msg);
			
			ret = -1;
		}
	}

	sqlite3_free(err);
	sqlite3_close(db);
	
	g_free(user_db);

	if (ret == 0){
		// Supprime les lignes attaché au groupe
		channels_list_delete_rows (app, path_group, TRUE);
	}
}

void
channels_list_delete_channels_group (FreetuxTVApp *app, GtkTreePath *path_group)
{
	FreetuxTVChannelsGroupInfos* channels_group_infos;
	channels_group_infos = channels_list_get_group (app, path_group);
	
	struct sqlite3 *db;
	int res;
	char *err = 0;
	gint ret = 0;
	gchar *query;

	gchar *err_msg = NULL;
	
	gchar *user_db;
	user_db = g_strconcat(g_get_user_config_dir(), 
			      "/FreetuxTV/freetuxtv.db", NULL);

	// Ouverture de la BDD
	res = sqlite3_open(user_db, &db);
	if(res != SQLITE_OK){
		err_msg = g_strdup_printf(_("Cannot open database.\n\nSQLite has returned error :\n%s."),
					  sqlite3_errmsg(db));
		windowmain_show_error (app, err_msg);
		g_free(err_msg);

		ret = -1;
	}
	
	if(ret == 0){
		// Effacement du groupe
		query = sqlite3_mprintf("DELETE FROM channels_group WHERE id_channelsgroup=%d",
					channels_group_infos->id);
		res = sqlite3_exec(db, query, NULL, 0, &err);
		sqlite3_free(query);
		if(res != SQLITE_OK){

			err_msg = g_strdup_printf(_("Error when deleting the group \"%s\".\n\nSQLite has returned error :\n%s."),
						  channels_group_infos->name,
						  sqlite3_errmsg(db));
			windowmain_show_error (app, err_msg);
			g_free(err_msg);
			
			ret = -1;
		}
	}
	
	if (ret == 0){
		// Supprime les lignes attaché au groupe
		channels_list_delete_rows (app, path_group, FALSE);
	}

	sqlite3_free(err);
	sqlite3_close(db);
	
	g_free(user_db);	
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

		filter_path = gtk_tree_model_filter_convert_child_path_to_path (GTK_TREE_MODEL_FILTER(model), app->current.path_channel);
		
		if(filter_path != NULL){
			gtk_tree_view_scroll_to_cell (GTK_TREE_VIEW(treeview), filter_path, NULL,
						      TRUE, 0.5, 0.5);
			gtk_tree_view_set_cursor (GTK_TREE_VIEW(treeview), filter_path, NULL, FALSE);
			//gtk_tree_view_set_cursor (GTK_TREE_VIEW(treeview), filter_path, NULL, FALSE);	
			//gtk_widget_grab_focus (treeview);
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

static int
channels_group_get_file (FreetuxTVChannelsGroupInfos *self, FreetuxTVApp *app, gchar **file, gboolean cache)
{
	gchar **uriv;
	int ret = 0;

	gchar *err_msg = NULL;

	uriv = g_strsplit (self->uri, "//", 2);
	*file = NULL;
	if( g_ascii_strcasecmp (uriv[0], "file:") == 0 ){
		*file = g_strdup (uriv[1]);
	}
	if( g_ascii_strcasecmp (uriv[0], "http:") == 0 ){
		
		GtkWidget *windowmain;
		windowmain = (GtkWidget *) gtk_builder_get_object(app->gui,
								  "windowmain");

		// Mise à jour de la barre de statut
		gchar *text;
		text = g_strdup_printf (_("Getting the file : \"%s\""), 
					self->uri);
		windowmain_statusbar_push (app, "UpdateMsg", text);
		g_free(text);
		g_print("FreetuxTV : Getting the file \"%s\"\n", self->uri);

		gchar *groupfile;
		groupfile = g_strdup_printf("%s/FreetuxTV/cache/playlist-group-%d.dat",
					    g_get_user_config_dir(), self->id);
		
		if(cache){
			// Téléchargement du fichier
			CURL *session = curl_easy_init(); 
			curl_easy_setopt(session, CURLOPT_URL, self->uri);
			curl_easy_setopt(session, CURLOPT_TIMEOUT, 10);
			// curl_easy_setopt(session, CURLOPT_VERBOSE, 1);
			curl_easy_setopt(session, CURLOPT_FOLLOWLOCATION, 1);
			FILE * fp = fopen(groupfile, "w"); 
			curl_easy_setopt(session,
					 CURLOPT_WRITEDATA, fp); 
			curl_easy_setopt(session,
					 CURLOPT_WRITEFUNCTION, fwrite);
			CURLcode curlcode;
			curlcode = curl_easy_perform(session);
			fclose(fp);
			curl_easy_cleanup(session);

			if(curlcode != 0){

				err_msg = g_strdup_printf(_("Error when downloading the playlist of the group \"%s\" from URL -> %s.\n\nCURL has returned error :\n%s."),
							  self->name,
							  self->uri,
							  curl_easy_strerror(curlcode));
				windowmain_show_error (app, err_msg);
				g_free(err_msg);
				ret = -1;
			}

		}

		*file = groupfile;
		
		windowmain_statusbar_pop (app, "UpdateMsg");
	}
	

	g_strfreev (uriv);
	
	return ret;
}

static int 
on_exec_add_channels_group (void *data, int argc, char **argv, char **colsname)
{
	CBUserData *cbuserdata = (CBUserData *) data;
	cbuserdata->nb++;

	FreetuxTVChannelsGroupInfos* channels_group_infos;
	int id = g_ascii_strtoll (argv[0], NULL, 10);
	channels_group_infos = freetuxtv_channels_group_infos_new (argv[1],argv[2]);
	freetuxtv_channels_group_infos_set_id (channels_group_infos, id);
	freetuxtv_channels_group_infos_set_rank (channels_group_infos, cbuserdata->nb);

	gtk_tree_store_append (GTK_TREE_STORE(cbuserdata->app->channelslist), cbuserdata->iter_channelsgroup, NULL);
	gtk_tree_store_set (GTK_TREE_STORE(cbuserdata->app->channelslist), cbuserdata->iter_channelsgroup,
			    CHANNELSGROUP_COLUMN, channels_group_infos, -1);
	GtkTreePath *path;
	path = gtk_tree_model_get_path(GTK_TREE_MODEL(cbuserdata->app->channelslist), cbuserdata->iter_channelsgroup);
	channels_list_load_channels_group (cbuserdata->app, path);
	gtk_tree_path_free(path);

	return 0;
}

static int 
on_exec_add_channel (void *data, int argc, char **argv, char **colsname)
{
	
	CBUserData *cbuserdata = (CBUserData *) data;	
	cbuserdata->nb++;

	FreetuxTVChannelInfos* channel_infos;
	int id = g_ascii_strtoll (argv[0], NULL, 10);
	channel_infos = freetuxtv_channel_infos_new (id, cbuserdata->nb, argv[1], argv[3]);

	if(argv[2] != NULL){
		freetuxtv_channel_infos_set_logo(channel_infos, argv[2]);
	}
	
	if(cbuserdata->app->debug == TRUE){
		g_print("FreetuxTV-debug : Load channel %d->'%s' from database\n",
			channel_infos->id,
			channel_infos->name);
	}
	
	gtk_tree_store_append (GTK_TREE_STORE(cbuserdata->app->channelslist),
			       cbuserdata->iter_channel, cbuserdata->iter_channelsgroup);
	gtk_tree_store_set (GTK_TREE_STORE(cbuserdata->app->channelslist), cbuserdata->iter_channel,
			    CHANNEL_COLUMN, channel_infos, -1);

	// Pointeur de la chaine sur son groupe
	FreetuxTVChannelsGroupInfos* channels_group_infos;	
	gtk_tree_model_get(GTK_TREE_MODEL(cbuserdata->app->channelslist), cbuserdata->iter_channelsgroup, CHANNELSGROUP_COLUMN, &channels_group_infos, -1);
	if(channels_group_infos != NULL){
		channels_group_infos->nb_channels++;
		freetuxtv_channel_infos_set_group (channel_infos, channels_group_infos);
	}

	// Get the path of the channel to play, if needed
	if(cbuserdata->app->current.lastchannelonstartup == TRUE
	   && cbuserdata->app->config.lastchannel != -1){
		if(id == cbuserdata->app->config.lastchannel){
			GtkTreePath* path;
			path = gtk_tree_model_get_path(GTK_TREE_MODEL(cbuserdata->app->channelslist), cbuserdata->iter_channel);
			cbuserdata->app->current.path_channel = path;
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
	
	}else {
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

	if (event->type == GDK_BUTTON_PRESS  &&  event->button == 3) {

		GtkTreeSelection *selection;
		
		selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview));
		
		if (gtk_tree_selection_count_selected_rows(selection)  <= 1) {
			GtkTreePath *path;
			
			// Recupère le chemin de la ligne selectionné
			if (gtk_tree_view_get_path_at_pos(GTK_TREE_VIEW(treeview),
							  (gint) event->x, 
							  (gint) event->y,
							  &path, NULL, NULL, NULL)){
				gtk_tree_selection_unselect_all(selection);
				gtk_tree_selection_select_path(selection, path);

				GtkTreeIter iter;

				FreetuxTVChannelsGroupInfos* channels_group_infos;
				// FreetuxTVChannelInfos* channel_infos;

				gtk_tree_model_get_iter (GTK_TREE_MODEL(app->channelslist), &iter, path);				
				gtk_tree_model_get(GTK_TREE_MODEL(app->channelslist), &iter, CHANNELSGROUP_COLUMN, &channels_group_infos, -1);

				GtkWidget *menu, *menuitem;
				GtkWidget *image;

				// Si on clic sur un groupe
				if(channels_group_infos != NULL){
					
					
					CBGroupData *cbgroupdata;
					cbgroupdata = g_new0(CBGroupData, 1);
					cbgroupdata->app = app;
					cbgroupdata->path_group = path;
					// TODO Function to free cbgroupdata when popup menu close

					menu = gtk_menu_new();
					
					menuitem = gtk_image_menu_item_new_with_label(_("Refresh from the playlist"));
					gtk_widget_set_tooltip_text (GTK_WIDGET(menuitem), channels_group_infos->uri);
					image = gtk_image_new_from_stock (GTK_STOCK_REFRESH, GTK_ICON_SIZE_MENU);
					gtk_image_menu_item_set_image (GTK_IMAGE_MENU_ITEM (menuitem), image);
					gtk_menu_append (GTK_MENU (menu), menuitem);					
					g_signal_connect(G_OBJECT(menuitem), "activate",
							 G_CALLBACK(on_popupmenu_activated_refreshgroup), cbgroupdata);
					gtk_widget_show (menuitem);
						
					menuitem = gtk_image_menu_item_new_with_label(_("Delete group's channels"));
					image = gtk_image_new_from_stock (GTK_STOCK_DELETE, GTK_ICON_SIZE_MENU);
					gtk_image_menu_item_set_image (GTK_IMAGE_MENU_ITEM (menuitem), image);
					gtk_menu_append (GTK_MENU (menu), menuitem);
					g_signal_connect(G_OBJECT(menuitem), "activate",
							 G_CALLBACK(on_popupmenu_activated_deletechannels), cbgroupdata);
					gtk_widget_show (menuitem);
					
					menuitem = gtk_image_menu_item_new_with_label(_("Delete group"));
					image = gtk_image_new_from_stock (GTK_STOCK_DELETE, GTK_ICON_SIZE_MENU);
					gtk_image_menu_item_set_image (GTK_IMAGE_MENU_ITEM (menuitem), image);
					gtk_menu_append (GTK_MENU (menu), menuitem);
					g_signal_connect(G_OBJECT(menuitem), "activate",
							 G_CALLBACK(on_popupmenu_activated_deletegroup), cbgroupdata);
					gtk_widget_show (menuitem);
						
					gtk_widget_show(menu);
										
					gtk_menu_popup(GTK_MENU(menu), NULL, NULL, NULL, NULL,
						       (event != NULL) ? event->button : 0,
						       gdk_event_get_time((GdkEvent*)event));
					
					return TRUE;
					
				}
				
				gtk_tree_path_free(path);
			}
		}
	}

	return FALSE;
}

static void
on_popupmenu_activated_refreshgroup (GtkMenuItem *menuitem, gpointer user_data)
{
	CBGroupData *cbgroupdata = (CBGroupData *)user_data;
	channels_list_update_channels_group(cbgroupdata->app, cbgroupdata->path_group);	
}

static void
on_popupmenu_activated_deletechannels (GtkMenuItem *menuitem, gpointer user_data)
{
	CBGroupData *cbgroupdata = (CBGroupData *)user_data;
	channels_list_delete_channels_channels_group(cbgroupdata->app, cbgroupdata->path_group);	
}


static void
on_popupmenu_activated_deletegroup (GtkMenuItem *menuitem, gpointer user_data)
{
	CBGroupData *cbgroupdata = (CBGroupData *)user_data;
	channels_list_delete_channels_group(cbgroupdata->app, cbgroupdata->path_group);	
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
on_exec_get_group_infos (void *data, int argc, char **argv, char **colsname)
{	
	Parsem3uData *cbuserdata = (Parsem3uData *) data;	
	cbuserdata->bregex = g_strdup_printf("^%s", (argv[0]!=NULL?argv[0]:""));
	cbuserdata->eregex = g_strdup_printf("%s$", (argv[1]!=NULL?argv[1]:""));
	return 0;
}

static int 
on_parsem3u_add_channel (char *url, int num, int argc, 
			 char **argv, void *user_data)
{
	Parsem3uData *data = (Parsem3uData *) user_data;

	struct sqlite3 *db = data->db;
	gchar* query;
	int res;
	char *err=0;
	gchar *name;

	gchar *err_msg;
	

	res = libm3uparser_get_extinfo (argc, argv, NULL, &name);
	if(res == LIBM3UPARSER_EXTINFO_NOT_FOUND){
		name = g_strdup_printf("Inconnu");
	}else{
		GRegex* bregex;
		GRegex* eregex;
		gchar *tmp;
		
		bregex = g_regex_new (data->bregex, 0, 0, NULL);
		eregex = g_regex_new (data->eregex, 0, 0, NULL);

		tmp = g_regex_replace (bregex, name, -1, 0, "", 0, NULL);
		g_free(name);
		
		name = g_regex_replace (eregex, tmp, -1, 0, "", 0, NULL);
		g_free(tmp);		
		
		g_regex_unref (bregex);
		g_regex_unref (eregex);
		
	}
	
	g_strstrip(name);	
	
	if(data->app->debug == TRUE){
		g_print("FreetuxTV-debug : Add channel '%s' in database\n",
			name);
	}

	query = sqlite3_mprintf("INSERT INTO channel (name_channel, order_channel, idchannellogo_channel, uri_channel, channelsgroup_channel) values ('%q',%d,(SELECT id_channellogo FROM channel_logo WHERE label_channellogo='%q' OR id_channellogo = (SELECT idchannellogo_labelchannellogo FROM label_channellogo WHERE label_labelchannellogo='%q')),'%q','%d');", 
				name, num, name, name, url, data->id);
	g_free(name);
	res=sqlite3_exec(db, query, NULL, 0, &err);
	sqlite3_free(query);
	if(res != SQLITE_OK){

		err_msg = g_strdup_printf(_("Error when adding the channel \"%s\".\n\nSQLite has returned error :\n%s."),
					  name, sqlite3_errmsg(db));
		windowmain_show_error (data->app, err_msg);
		g_free(err_msg);

		return -1;
	}
	return 0;
}
