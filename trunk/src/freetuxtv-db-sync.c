/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8-*- */
/*
 * freetuxtv
 * Copyright (C) FreetuxTV Team's 2008
 * Project homepage : http://code.google.com/p/freetuxtv/
 * 
 * freetuxtv is free software.
 * 
 */

#include <glib.h>
#include <sqlite3.h>

#include "freetuxtv-db-sync.h"

typedef struct {
	FreetuxTVApp *app;
	DBSync *dbsync;
	int (*callback)();
	gpointer user_data;
	GError** error;

	gpointer cb_data1;

}CBDBSync;

static int 
on_exec_channels_group (void *data, int argc, char **argv, char **colsname);

static int 
on_exec_channel (void *data, int argc, char **argv, char **colsname);

GQuark freetuxtv_dbsync_error = 0;

GQuark
freetuxtv_dbsync_error_quark () {
	if (freetuxtv_dbsync_error == 0){
		freetuxtv_dbsync_error = g_quark_from_string("FREETUXTV_DBSYNC_ERROR");
	}
	return freetuxtv_dbsync_error;
}

void
dbsync_open_db(DBSync *dbsync, GError** error)
{
	g_return_if_fail(dbsync != NULL);
	g_return_if_fail(dbsync->db_link != NULL);

	gchar *user_db;
	int res;

	user_db = g_strconcat(g_get_user_config_dir(), 
			      "/FreetuxTV/freetuxtv.db", NULL);
	
	// Open the database if not open
	g_print("Open database\n");
	res = sqlite3_open(user_db, &(dbsync->db_link));
	if(res != SQLITE_OK){
		if(error != NULL){
			// sqlite3_errmsg return const char*, no need to free it
			*error = g_error_new (FREETUXTV_DBSYNC_ERROR,
					      FREETUXTV_DBSYNC_ERROR_OPEN,
					      _("Cannot open database.\n\nSQLite has returned error :\n%s."),
					      sqlite3_errmsg(dbsync->db_link));
		}
	}
	
	g_free(user_db);
}

void
dbsync_close_db (DBSync *dbsync)
{
	g_return_if_fail(dbsync != NULL);

	g_print("Close database\n");
	sqlite3_close(dbsync->db_link);
	dbsync->db_link == NULL;
}

void
dbsync_select_channels_groups (DBSync *dbsync, FreetuxTVApp *app,
			       int (*callback)(FreetuxTVApp *app, FreetuxTVChannelsGroupInfos* channels_group_infos,
					       DBSync *dbsync, gpointer user_data, GError** error),
			       gpointer user_data, GError** error)
{
	g_return_if_fail(dbsync != NULL);
	g_return_if_fail(app != NULL);

	gchar *query;
	gchar *db_err = 0;
	int res;
	gboolean no_err = TRUE;
	CBDBSync cb_data;

	cb_data.app = app;
	cb_data.dbsync = dbsync;
	cb_data.callback = callback;
	cb_data.user_data = user_data;
	cb_data.error = error;

	if(error != NULL){
		if(*error != NULL){
			no_err = FALSE;
		}
	}

	if(no_err){
		query = "SELECT id_channelsgroup, name_channelsgroup, uri_channelsgroup, \
                           bregex_channelsgroup, eregex_channelsgroup \
                         FROM channels_group";		
		res = sqlite3_exec(dbsync->db_link, query, on_exec_channels_group, &cb_data, &db_err);
		if(res != SQLITE_OK){
			no_err = FALSE;
			if(error != NULL){
				*error = g_error_new (FREETUXTV_DBSYNC_ERROR,
						      FREETUXTV_DBSYNC_ERROR_EXEC_QUERY,
						      _("Error when displaying the channels.\n\nSQLite has returned error :\n%s."),
						      sqlite3_errmsg(dbsync->db_link));
			}
		}
	}	
}

void
dbsync_select_channels_of_channels_group (DBSync *dbsync,
					  FreetuxTVChannelsGroupInfos* channels_group_infos,
					  FreetuxTVApp *app,
					  int (*callback)(FreetuxTVApp *app, 
							  FreetuxTVChannelInfos* channel_infos,
							  DBSync *dbsync, gpointer user_data, GError** error),
					  gpointer user_data, GError** error)
{
	g_return_if_fail(dbsync != NULL);
	g_return_if_fail(app != NULL);

	gchar *query;
	gchar *db_err = 0;
	int res;
	gboolean no_err = TRUE;
	CBDBSync cb_data;

	cb_data.app = app;
	cb_data.dbsync = dbsync;
	cb_data.callback = callback;
	cb_data.user_data = user_data;
	cb_data.error = error;
	cb_data.cb_data1 = channels_group_infos;

	if(error != NULL){
		if(*error != NULL){
			no_err = FALSE;
		}
	}
	
	if(no_err){
		query = sqlite3_mprintf("SELECT id_channel, name_channel, filename_channellogo, \
                           uri_channel FROM channel LEFT JOIN channel_logo \
                           ON id_channellogo=idchannellogo_channel \
                           WHERE channelsgroup_channel=%d\
                           ORDER BY order_channel",
					channels_group_infos->id);
		res = sqlite3_exec(dbsync->db_link, query, on_exec_channel, &cb_data, &db_err);
		sqlite3_free(query);
		
		if(res != SQLITE_OK){
			no_err = FALSE;
			if(error != NULL){
				*error = g_error_new (FREETUXTV_DBSYNC_ERROR,
						      FREETUXTV_DBSYNC_ERROR_EXEC_QUERY,
						      _("Error when displaying the channels.\n\nSQLite has returned error :\n%s."),
						      sqlite3_errmsg(dbsync->db_link));
			}
		}
	}	
}

void
dbsync_add_channel (DBSync *dbsync,
		    FreetuxTVChannelInfos* channel_infos,
		    GError** error)
{
	g_return_if_fail(dbsync != NULL);
	g_return_if_fail(channel_infos != NULL);
	g_return_if_fail(FREETUXTV_IS_CHANNEL_INFOS(channel_infos));

	gchar *query;
	gchar *db_err = 0;
	int res;
	gboolean no_err = TRUE;

	if(error != NULL){
		if(*error != NULL){
			no_err = FALSE;
		}
	}
	
	if(no_err){
		// Add the channel
		query = sqlite3_mprintf("INSERT INTO channel \
                               (name_channel, order_channel, idchannellogo_channel, uri_channel, channelsgroup_channel) \
                           VALUES ('%q',%d, \
                               (SELECT id_channellogo FROM channel_logo \
                                WHERE label_channellogo='%q' OR id_channellogo = \
                                   (SELECT idchannellogo_labelchannellogo FROM label_channellogo \
                                    WHERE label_labelchannellogo='%q' \
                                   ) \
                               ),'%q','%d');", 
					channel_infos->name, channel_infos->order, channel_infos->name,
					channel_infos->name, channel_infos->url, channel_infos->channels_group->id);
		res = sqlite3_exec(dbsync->db_link, query, NULL, NULL, &db_err);
		sqlite3_free(query);

		if(res != SQLITE_OK){
			no_err = FALSE;
			if(error != NULL){
				*error = g_error_new (FREETUXTV_DBSYNC_ERROR,
						      FREETUXTV_DBSYNC_ERROR_EXEC_QUERY,
						      _("Error when adding the channel \"%s\".\n\nSQLite has returned error :\n%s."),
						      channel_infos->name, sqlite3_errmsg(dbsync->db_link));
			}
		}else{
			freetuxtv_channel_infos_set_id (channel_infos,
							(int)sqlite3_last_insert_rowid(dbsync->db_link));
		}	
	}
}

void
dbsync_add_channels_group (DBSync *dbsync,
			   FreetuxTVChannelsGroupInfos* channels_group_infos,
			   GError** error)
{
	g_return_if_fail(dbsync != NULL);
	g_return_if_fail(channels_group_infos != NULL);
	g_return_if_fail(FREETUXTV_IS_CHANNELS_GROUP_INFOS(channels_group_infos));

	gchar *query;
	gchar *db_err = 0;
	int res;
	gboolean no_err = TRUE;
	
	if(no_err){
		// Add the group
		query = sqlite3_mprintf("INSERT INTO channels_group (name_channelsgroup, uri_channelsgroup, bregex_channelsgroup, eregex_channelsgroup) VALUES ('%q', %Q, %Q, %Q);", 
					channels_group_infos->name,
					channels_group_infos->uri,
					channels_group_infos->bregex,
					channels_group_infos->eregex
					);
		
		res = sqlite3_exec(dbsync->db_link, query, NULL, NULL, &db_err);
		sqlite3_free(query);

		if(res != SQLITE_OK){
			no_err = FALSE;
			if(error != NULL){
				*error = g_error_new (FREETUXTV_DBSYNC_ERROR,
						      FREETUXTV_DBSYNC_ERROR_EXEC_QUERY,
						      _("Cannot add the group \"%s\" in database.\n\nSQLite has returned error :\n%s."),
						      channels_group_infos->name,
						      sqlite3_errmsg(dbsync->db_link));
			}
		}else{
			freetuxtv_channels_group_infos_set_id (channels_group_infos,
							       (int)sqlite3_last_insert_rowid(dbsync->db_link));
		}
	}
}

void
dbsync_update_channels_group (DBSync *dbsync,
			      FreetuxTVChannelsGroupInfos* channels_group_infos,
			      GError** error)
{
	g_return_if_fail(dbsync != NULL);
	g_return_if_fail(channels_group_infos != NULL);
	g_return_if_fail(FREETUXTV_IS_CHANNELS_GROUP_INFOS(channels_group_infos));

	gchar *query;
	gchar *db_err = 0;
	int res;
	gboolean no_err = TRUE;

	if(error != NULL){
		if(*error != NULL){
			no_err = FALSE;
		}
	}
	
	if(no_err){
		// Update the group
		query = sqlite3_mprintf("UPDATE channels_group SET \
                               name_channelsgroup='%q', uri_channelsgroup=%Q, \
                               bregex_channelsgroup=%Q, eregex_channelsgroup=%Q \
                               WHERE id_channelsgroup=%d", 
					channels_group_infos->name, channels_group_infos->uri,
					channels_group_infos->bregex, channels_group_infos->eregex,
					channels_group_infos->id);
		res = sqlite3_exec(dbsync->db_link, query, NULL, NULL, &db_err);
		sqlite3_free(query);

		if(res != SQLITE_OK){
			no_err = FALSE;
			if(error != NULL){
				*error = g_error_new (FREETUXTV_DBSYNC_ERROR,
						      FREETUXTV_DBSYNC_ERROR_EXEC_QUERY,
						      _("Error when updating the group \"%s\".\n\nSQLite has returned error :\n%s."),
						      channels_group_infos->name, sqlite3_errmsg(dbsync->db_link));
			}
		}	
	}
}


void
dbsync_delete_channels_group (DBSync *dbsync,
			      FreetuxTVChannelsGroupInfos* channels_group_infos,
			      GError** error)
{
	g_return_if_fail(dbsync->db_link != NULL);
	g_return_if_fail(channels_group_infos != NULL);
	g_return_if_fail(FREETUXTV_IS_CHANNELS_GROUP_INFOS(channels_group_infos));

	gchar *query;
	gchar *db_err = 0;
	int res;
	gboolean no_err = TRUE;
	
	if(no_err){
		// Delete the channels group
		query = sqlite3_mprintf("DELETE FROM channels_group WHERE id_channelsgroup=%d",
					channels_group_infos->id);
		res = sqlite3_exec(dbsync->db_link, query, NULL, NULL, &db_err);
		sqlite3_free(query);

		if(res != SQLITE_OK){
			no_err = FALSE;
			if(error != NULL){
				*error = g_error_new (FREETUXTV_DBSYNC_ERROR,
						      FREETUXTV_DBSYNC_ERROR_EXEC_QUERY,
						      _("Error when deleting the group \"%s\".\n\nSQLite has returned error :\n%s."),
						      channels_group_infos->name,
						      sqlite3_errmsg(dbsync->db_link));
			}
		}
	}
}

void
dbsync_delete_channels_of_channels_group (DBSync *dbsync,
					  FreetuxTVChannelsGroupInfos* channels_group_infos,
					  GError** error)
{
	g_return_if_fail(dbsync->db_link != NULL);
	g_return_if_fail(channels_group_infos != NULL);
	g_return_if_fail(FREETUXTV_IS_CHANNELS_GROUP_INFOS(channels_group_infos));

	gchar *query;
	gchar *db_err = 0;
	int res;
	gboolean no_err = TRUE;
	
	if(no_err){
		// Delete the channels of the channels group
		query = sqlite3_mprintf("DELETE FROM channel WHERE channelsgroup_channel=%d",
					channels_group_infos->id);
		res = sqlite3_exec(dbsync->db_link, query, NULL, NULL, &db_err);
		sqlite3_free(query);

		if(res != SQLITE_OK){
			no_err = FALSE;
			if(error != NULL){
				*error = g_error_new (FREETUXTV_DBSYNC_ERROR,
						      FREETUXTV_DBSYNC_ERROR_EXEC_QUERY,
						      _("Error when deleting the channels of the group \"%s\".\n\nSQLite has returned error :\n%s."),
						      channels_group_infos->name,
						      sqlite3_errmsg(dbsync->db_link));
			}
		}
	}
}

static int 
on_exec_channels_group (void *data, int argc, char **argv, char **colsname)
{
	CBDBSync *cb_data = (CBDBSync *)data;

	FreetuxTVChannelsGroupInfos* channels_group_infos;
	int id = g_ascii_strtoll (argv[0], NULL, 10);
	channels_group_infos = freetuxtv_channels_group_infos_new (argv[1],argv[2]);
	freetuxtv_channels_group_infos_set_id (channels_group_infos, id);
	freetuxtv_channels_group_infos_set_regex (channels_group_infos, argv[3], argv[4]);

	int res = 0;
	res = cb_data->callback(cb_data->app, channels_group_infos, cb_data->dbsync, cb_data->user_data, cb_data->error);

	return res;
}

static int 
on_exec_channel (void *data, int argc, char **argv, char **colsname)
{
	CBDBSync *cb_data = (CBDBSync *)data;
	FreetuxTVChannelsGroupInfos* channels_group_infos = (FreetuxTVChannelsGroupInfos*)cb_data->cb_data1;

	FreetuxTVChannelInfos* channel_infos;
	int id = g_ascii_strtoll (argv[0], NULL, 10);
	channel_infos = freetuxtv_channel_infos_new (argv[1], argv[3]);
	freetuxtv_channel_infos_set_id (channel_infos, id);

	if(argv[2] != NULL){
		freetuxtv_channel_infos_set_logo(channel_infos, argv[2]);
	}
	
	freetuxtv_channel_infos_set_channels_group(channel_infos, channels_group_infos);
	channels_group_infos->nb_channels++;

	int res = 0;
	res = cb_data->callback(cb_data->app, channel_infos, cb_data->dbsync, cb_data->user_data, cb_data->error);
	
	return res;
}
