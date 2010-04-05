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

// Define constant for the list of field in the database

// Table tvchannel
#define DB_TVCHANNEL                    "tvchannel"
#define DB_TVCHANNEL_ID                 "id"
#define DB_TVCHANNEL_NAME               "name"
#define DB_TVCHANNEL_LOGOFILENAME       "logo_filename"

// Table label_tvchannel
#define DB_LABELTVCHANNEL               "label_tvchannel"
#define DB_LABELTVCHANNEL_ID            "id"
#define DB_LABELTVCHANNEL_LABEL         "label"
#define DB_LABELTVCHANNEL_TVCHANNELID   "tvchannel_id"

// Table channels_group
#define DB_CHANNELSGROUP                "channels_group"
#define DB_CHANNELSGROUP_ID             "id"
#define DB_CHANNELSGROUP_RANK           "rank"
#define DB_CHANNELSGROUP_NAME           "name"
#define DB_CHANNELSGROUP_TYPE           "type"
#define DB_CHANNELSGROUP_URI            "uri"
#define DB_CHANNELSGROUP_BREGEX         "bregex"
#define DB_CHANNELSGROUP_EREGEX         "eregex"

// Table channel
#define DB_CHANNEL                     "channel"
#define DB_CHANNEL_ID                  "id"
#define DB_CHANNEL_NAME                "name"
#define DB_CHANNEL_RANK                "rank"
#define DB_CHANNEL_URI                 "uri"
#define DB_CHANNEL_VLCOPTIONS          "vlc_options"
#define DB_CHANNEL_CHANNELGROUPID      "channelsgroup_id"
#define DB_CHANNEL_TVCHANNELID         "tvchannel_id"

static int 
on_exec_channels_group (void *data, int argc, char **argv, char **colsname);

static int 
on_exec_channel (void *data, int argc, char **argv, char **colsname);

static void
dbsync_link_tvchannel_to_channels (DBSync *dbsync, gchar *label, glong id_tvchannel,
				   GError** error);

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
	g_return_if_fail(error != NULL);
	g_return_if_fail(*error == NULL);

	gchar *user_db;
	int res;

	user_db = g_strconcat(g_get_user_config_dir(), 
			      "/FreetuxTV/freetuxtv.db", NULL);
	
	// Open the database if not open
	g_print("DBSync : Open database\n");
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
	g_return_if_fail(dbsync->db_link != NULL);

	g_print("DBSync : Close database\n");
	sqlite3_close(dbsync->db_link);
	dbsync->db_link = NULL;
}

gboolean
dbsync_db_exists(DBSync *dbsync)
{
	g_return_val_if_fail(dbsync != NULL, FALSE);

	gchar *user_db;
	gboolean res = FALSE;

	user_db = g_strconcat(g_get_user_config_dir(), 
			      "/FreetuxTV/freetuxtv.db", NULL);
	
	if (g_file_test (user_db, G_FILE_TEST_IS_REGULAR)){
		res = TRUE;
	}
	return res;
}

void
dbsync_create_db (DBSync *dbsync, GError** error)
{
	g_return_if_fail(dbsync != NULL);
	g_return_if_fail(dbsync->db_link != NULL);
	g_return_if_fail(error != NULL);
	g_return_if_fail(*error == NULL);

	gchar *query;
	gchar *db_err = NULL;
	int res;
	
	// Load file containing the database creation queries
	gchar* filename;
	gsize filelen;
	filename = FREETUXTV_DIR "/sqlite3-create-tables.sql";	
	
	res = g_file_get_contents (filename, &query, &filelen, error);
	if (res){		
		res = sqlite3_exec(dbsync->db_link, query, NULL, 0, &db_err);
		if(res != SQLITE_OK){
			g_printerr("Sqlite3 : %s\n",
				   sqlite3_errmsg(dbsync->db_link));
			g_printerr("FreetuxTV : Cannot create tables\n");
			
			*error = g_error_new (FREETUXTV_DBSYNC_ERROR,
					      FREETUXTV_DBSYNC_ERROR_EXEC_QUERY,
					      _("Error when creating the database.\n\nSQLite has returned error :\n%s."),
					      sqlite3_errmsg(dbsync->db_link));
			sqlite3_free(db_err);
		}
	}
	
	g_free(query);	
}

void
dbsync_select_channels_groups (DBSync *dbsync, FreetuxTVApp *app,
			       int (*callback)(FreetuxTVApp *app, FreetuxTVChannelsGroupInfos* channels_group_infos,
					       DBSync *dbsync, gpointer user_data, GError** error),
			       gpointer user_data, GError** error)
{
	g_return_if_fail(dbsync != NULL);
	g_return_if_fail(dbsync->db_link != NULL);
	g_return_if_fail(error != NULL);
	g_return_if_fail(*error == NULL);
	g_return_if_fail(app != NULL);

	gchar *query;
	gchar *db_err = NULL;
	int res;
	CBDBSync cb_data;

	cb_data.app = app;
	cb_data.dbsync = dbsync;
	cb_data.callback = callback;
	cb_data.user_data = user_data;
	cb_data.error = error;

	query = sqlite3_mprintf("SELECT %s, %s, %s, %s, %s, %s, %s \
                                 FROM %s \
                                 ORDER BY %s",
				// SELECT
				DB_CHANNELSGROUP_ID, DB_CHANNELSGROUP_RANK, DB_CHANNELSGROUP_NAME,
				DB_CHANNELSGROUP_TYPE, DB_CHANNELSGROUP_URI, DB_CHANNELSGROUP_BREGEX, DB_CHANNELSGROUP_EREGEX,
				// FROM
				DB_CHANNELSGROUP,
				// ORDER BY
				DB_CHANNELSGROUP_RANK);
	res = sqlite3_exec(dbsync->db_link, query, on_exec_channels_group, &cb_data, &db_err);
	sqlite3_free(query);
	if(res != SQLITE_OK){
		*error = g_error_new (FREETUXTV_DBSYNC_ERROR,
				      FREETUXTV_DBSYNC_ERROR_EXEC_QUERY,
				      _("Error when displaying the channels.\n\nSQLite has returned error :\n%s."),
				      sqlite3_errmsg(dbsync->db_link));
		sqlite3_free(db_err);
		
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
	g_return_if_fail(dbsync->db_link != NULL);
	g_return_if_fail(error != NULL);
	g_return_if_fail(*error == NULL);
	g_return_if_fail(app != NULL);

	gchar *query;
	gchar *db_err = NULL;
	int res;
	CBDBSync cb_data;

	cb_data.app = app;
	cb_data.dbsync = dbsync;
	cb_data.callback = callback;
	cb_data.user_data = user_data;
	cb_data.error = error;
	cb_data.cb_data1 = channels_group_infos;

	query = sqlite3_mprintf("SELECT %s.%s, %s.%s, %s.%s, %s.%s, %s.%s \
                                 FROM %s LEFT JOIN %s ON %s.%s=%s.%s \
                                 WHERE %s.%s=%d \
                                 ORDER BY %s.%s",
				// SELECT
				DB_CHANNEL, DB_CHANNEL_ID, DB_CHANNEL, DB_CHANNEL_NAME,
				DB_TVCHANNEL, DB_TVCHANNEL_LOGOFILENAME, DB_CHANNEL, DB_CHANNEL_URI,
				DB_CHANNEL, DB_CHANNEL_VLCOPTIONS,
				// FROM
				DB_CHANNEL, DB_TVCHANNEL,
				// ON
				DB_CHANNEL, DB_CHANNEL_TVCHANNELID, DB_TVCHANNEL, DB_TVCHANNEL_ID,
				// WHERE
				DB_CHANNEL, DB_CHANNEL_CHANNELGROUPID, channels_group_infos->id,
				// ORDER BY
				DB_CHANNEL, DB_CHANNEL_RANK);
	res = sqlite3_exec(dbsync->db_link, query, on_exec_channel, &cb_data, &db_err);
	sqlite3_free(query);

	if(res != SQLITE_OK){
		*error = g_error_new (FREETUXTV_DBSYNC_ERROR,
				      FREETUXTV_DBSYNC_ERROR_EXEC_QUERY,
				      _("Error when displaying the channels.\n\nSQLite has returned error :\n%s."),
				      sqlite3_errmsg(dbsync->db_link));
		sqlite3_free(db_err);
	}
}

void
dbsync_add_channel (DBSync *dbsync,
		    FreetuxTVChannelInfos* channel_infos,
		    GError** error)
{
	g_return_if_fail(dbsync != NULL);
	g_return_if_fail(dbsync->db_link != NULL);
	g_return_if_fail(error != NULL);
	g_return_if_fail(*error == NULL);
	g_return_if_fail(channel_infos != NULL);
	g_return_if_fail(FREETUXTV_IS_CHANNEL_INFOS(channel_infos));

	gchar *query;
	gchar *db_err = NULL;
	int res;

	gchar* vlc_options = NULL;

	if(channel_infos->vlc_options){
		vlc_options = g_strjoinv ("\n", channel_infos->vlc_options);
	}

	// Add the channel
	query = sqlite3_mprintf("INSERT INTO %s (%s, %s, %s, %s, %s, %s) \
                                 VALUES ('%q',%d,'%q', %Q, \
                                   (SELECT %s.%s FROM %s \
                                    WHERE ('%q' LIKE %s.%s||'%%') OR %s.%s = \
                                      (SELECT %s.%s FROM %s \
                                       WHERE ('%q' LIKE %s.%s||'%%') \
                                       ORDER BY %s.%s DESC \
                                    ) \
                                    ORDER BY %s.%s DESC \
                                 ),'%d');",
				// INSERT INTO
				DB_CHANNEL,
				// (
				DB_CHANNEL_NAME, DB_CHANNEL_RANK, DB_CHANNEL_URI,
				DB_CHANNEL_VLCOPTIONS, DB_CHANNEL_TVCHANNELID, DB_CHANNEL_CHANNELGROUPID,
				// ) VALUES (
				channel_infos->name, channel_infos->order, channel_infos->url, vlc_options,
				// (SELECT
				DB_TVCHANNEL, DB_TVCHANNEL_ID,
				// FROM
				DB_TVCHANNEL,
				// WHERE
				channel_infos->name, DB_TVCHANNEL, DB_TVCHANNEL_NAME,
				// OR
				DB_TVCHANNEL, DB_TVCHANNEL_ID,
				// (SELECT
				DB_LABELTVCHANNEL, DB_LABELTVCHANNEL_TVCHANNELID, DB_LABELTVCHANNEL,
				// WHERE
				channel_infos->name, DB_LABELTVCHANNEL, DB_LABELTVCHANNEL_LABEL,
				// ORDER BY
				DB_LABELTVCHANNEL, DB_LABELTVCHANNEL_LABEL,
				// ) ORDER BY
				DB_TVCHANNEL, DB_TVCHANNEL_NAME,
				channel_infos->channels_group->id);
	res = sqlite3_exec(dbsync->db_link, query, NULL, NULL, &db_err);
	//g_print("%s\n", query);
	sqlite3_free(query);

	if(vlc_options){
		g_free(vlc_options);
	}
	
	if(res != SQLITE_OK){
		*error = g_error_new (FREETUXTV_DBSYNC_ERROR,
				      FREETUXTV_DBSYNC_ERROR_EXEC_QUERY,
				      _("Error when adding the channel \"%s\".\n\nSQLite has returned error :\n%s."),
				      channel_infos->name, sqlite3_errmsg(dbsync->db_link));
		sqlite3_free(db_err);
	}else{
		freetuxtv_channel_infos_set_id (channel_infos,
						(int)sqlite3_last_insert_rowid(dbsync->db_link));
	}	
}

void
dbsync_add_channels_group (DBSync *dbsync,
			   FreetuxTVChannelsGroupInfos* channels_group_infos,
			   GError** error)
{
	g_return_if_fail(dbsync != NULL);
	g_return_if_fail(dbsync->db_link != NULL);
	g_return_if_fail(error != NULL);
	g_return_if_fail(*error == NULL);
	g_return_if_fail(channels_group_infos != NULL);
	g_return_if_fail(FREETUXTV_IS_CHANNELS_GROUP_INFOS(channels_group_infos));

	gchar *query;
	gchar *db_err = NULL;
	int res;
	
	// Add the group
	query = sqlite3_mprintf("INSERT INTO %s (%s, %s, %s, %s, %s, %s) \
                                 VALUES ((SELECT IFNULL(MAX(%s), 0) + 1 FROM %s), '%q', %d, %Q, %Q, %Q);",
				// INSERT INTO
				DB_CHANNELSGROUP,
				// (
				DB_CHANNELSGROUP_RANK,
				DB_CHANNELSGROUP_NAME,
				DB_CHANNELSGROUP_TYPE,
				DB_CHANNELSGROUP_URI,
				DB_CHANNELSGROUP_BREGEX,
				DB_CHANNELSGROUP_EREGEX,
				// ) VALUES (
				DB_CHANNELSGROUP_RANK, DB_CHANNELSGROUP, // rank
				channels_group_infos->name,
				0,
				channels_group_infos->uri,
				channels_group_infos->bregex,
				channels_group_infos->eregex
				);
	
	res = sqlite3_exec(dbsync->db_link, query, NULL, NULL, &db_err);
	sqlite3_free(query);
	
	if(res != SQLITE_OK){
		*error = g_error_new (FREETUXTV_DBSYNC_ERROR,
				      FREETUXTV_DBSYNC_ERROR_EXEC_QUERY,
				      _("Cannot add the group \"%s\" in database.\n\nSQLite has returned error :\n%s."),
				      channels_group_infos->name,
				      sqlite3_errmsg(dbsync->db_link));
		sqlite3_free(db_err);
	}else{
		freetuxtv_channels_group_infos_set_id (channels_group_infos,
						       (int)sqlite3_last_insert_rowid(dbsync->db_link));
	}
}

void
dbsync_update_channels_group (DBSync *dbsync,
			      FreetuxTVChannelsGroupInfos* channels_group_infos,
			      GError** error)
{
	g_return_if_fail(dbsync != NULL);
	g_return_if_fail(dbsync->db_link != NULL);
	g_return_if_fail(error != NULL);
	g_return_if_fail(*error == NULL);
	g_return_if_fail(channels_group_infos != NULL);
	g_return_if_fail(FREETUXTV_IS_CHANNELS_GROUP_INFOS(channels_group_infos));

	gchar *query;
	gchar *db_err = NULL;
	int res;
	
	// Update the group
	query = sqlite3_mprintf("UPDATE %s SET %s='%q', %s=%Q, %s=%Q, %s=%Q \
                                 WHERE %s=%d",
				// UPDATE
				DB_CHANNELSGROUP,
				// SET
				DB_CHANNELSGROUP_NAME, channels_group_infos->name,
				DB_CHANNELSGROUP_URI, channels_group_infos->uri,
				DB_CHANNELSGROUP_BREGEX, channels_group_infos->bregex,
				DB_CHANNELSGROUP_EREGEX, channels_group_infos->eregex,
				// WHERE
				DB_CHANNELSGROUP_ID, channels_group_infos->id);
	res = sqlite3_exec(dbsync->db_link, query, NULL, NULL, &db_err);
	sqlite3_free(query);
	
	if(res != SQLITE_OK){
		*error = g_error_new (FREETUXTV_DBSYNC_ERROR,
				      FREETUXTV_DBSYNC_ERROR_EXEC_QUERY,
				      _("Error when updating the group \"%s\".\n\nSQLite has returned error :\n%s."),
				      channels_group_infos->name, sqlite3_errmsg(dbsync->db_link));
		sqlite3_free(db_err);
	}	
}


void
dbsync_delete_channels_group (DBSync *dbsync,
			      FreetuxTVChannelsGroupInfos* channels_group_infos,
			      GError** error)
{
	g_return_if_fail(dbsync != NULL);
	g_return_if_fail(dbsync->db_link != NULL);
	g_return_if_fail(error != NULL);
	g_return_if_fail(*error == NULL);
	g_return_if_fail(channels_group_infos != NULL);
	g_return_if_fail(FREETUXTV_IS_CHANNELS_GROUP_INFOS(channels_group_infos));

	gchar *query;
	gchar *db_err = NULL;
	int res;
	
	// Delete the channels group
	query = sqlite3_mprintf("DELETE FROM %s WHERE %s=%d",
				// DELETE FROM
				DB_CHANNELSGROUP,
				// WHERE
				DB_CHANNELSGROUP_ID, channels_group_infos->id);
	res = sqlite3_exec(dbsync->db_link, query, NULL, NULL, &db_err);
	sqlite3_free(query);
	
	if(res != SQLITE_OK){
		*error = g_error_new (FREETUXTV_DBSYNC_ERROR,
				      FREETUXTV_DBSYNC_ERROR_EXEC_QUERY,
				      _("Error when deleting the group \"%s\".\n\nSQLite has returned error :\n%s."),
				      channels_group_infos->name,
				      sqlite3_errmsg(dbsync->db_link));
		sqlite3_free(db_err);
	}
}

void
dbsync_delete_channels_of_channels_group (DBSync *dbsync,
					  FreetuxTVChannelsGroupInfos* channels_group_infos,
					  GError** error)
{
	g_return_if_fail(dbsync != NULL);
	g_return_if_fail(dbsync->db_link != NULL);
	g_return_if_fail(error != NULL);
	g_return_if_fail(*error == NULL);
	g_return_if_fail(channels_group_infos != NULL);
	g_return_if_fail(FREETUXTV_IS_CHANNELS_GROUP_INFOS(channels_group_infos));

	gchar *query;
	gchar *db_err = NULL;
	int res;

	// Delete the channels of the channels group
	query = sqlite3_mprintf("DELETE FROM %s WHERE %s=%d",
				// DELETE FROM
				DB_CHANNEL,
				// WHERE
				DB_CHANNEL_CHANNELGROUPID, channels_group_infos->id);
	res = sqlite3_exec(dbsync->db_link, query, NULL, NULL, &db_err);
	sqlite3_free(query);
	
	if(res != SQLITE_OK){
		*error = g_error_new (FREETUXTV_DBSYNC_ERROR,
				      FREETUXTV_DBSYNC_ERROR_EXEC_QUERY,
				      _("Error when deleting the channels of the group \"%s\".\n\nSQLite has returned error :\n%s."),
				      channels_group_infos->name,
				      sqlite3_errmsg(dbsync->db_link));
		sqlite3_free(db_err);
	}
}

void
dbsync_delete_tvchannels (DBSync *dbsync, GError** error)
{
	g_return_if_fail(dbsync != NULL);
	g_return_if_fail(dbsync->db_link != NULL);
	g_return_if_fail(error != NULL);
	g_return_if_fail(*error == NULL);

	gchar *query;
	gchar *db_err = NULL;
	int res;

	// Delete the channels logos
	query = sqlite3_mprintf("DELETE FROM %s", DB_TVCHANNEL);
	res = sqlite3_exec(dbsync->db_link, query, NULL, NULL, &db_err);
	sqlite3_free(query);	
	if(res != SQLITE_OK){
		*error = g_error_new (FREETUXTV_DBSYNC_ERROR,
				      FREETUXTV_DBSYNC_ERROR_EXEC_QUERY,
				      _("Error when deleting the tv channels list.\n\nSQLite has returned error :\n%s."),
				      sqlite3_errmsg(dbsync->db_link));
		sqlite3_free(db_err);
	}	
}

void
dbsync_add_tvchannel (DBSync *dbsync, gchar* label, gchar* filename, 
		      glong *id, GError** error)
{
	g_return_if_fail(dbsync != NULL);
	g_return_if_fail(dbsync->db_link != NULL);
	g_return_if_fail(error != NULL);
	g_return_if_fail(*error == NULL);
	g_return_if_fail(id != NULL);

	gchar *query;
	gchar *db_err = NULL;
	int res;

	// Add the channel logo
	query = sqlite3_mprintf("INSERT INTO %s (%s, %s) VALUES ('%q','%q');",
				// INSERT INTO
				DB_TVCHANNEL,
				// (
				DB_TVCHANNEL_NAME, DB_TVCHANNEL_LOGOFILENAME,
				// ) VALUES (
				label, filename);
	res = sqlite3_exec(dbsync->db_link, query, NULL, NULL, &db_err);
	sqlite3_free(query);	
	if(res != SQLITE_OK){
		*error = g_error_new (FREETUXTV_DBSYNC_ERROR,
				      FREETUXTV_DBSYNC_ERROR_EXEC_QUERY,
				      _("Error when adding the tv channel '%s'.\n\nSQLite has returned error :\n%s."),
				      label, sqlite3_errmsg(dbsync->db_link));
		sqlite3_free(db_err);
	}else{
		*id = sqlite3_last_insert_rowid(dbsync->db_link);
		dbsync_link_tvchannel_to_channels (dbsync, label, *id, error);	
	}
}

void
dbsync_add_label_tvchannel (DBSync *dbsync, gchar* label, glong id_tvchannel, 
			    GError** error)
{
	g_return_if_fail(dbsync != NULL);
	g_return_if_fail(dbsync->db_link != NULL);
	g_return_if_fail(error != NULL);
	g_return_if_fail(*error == NULL);

	gchar *query;
	gchar *db_err = NULL;
	int res;

	// Add the label of channel logo
	query = sqlite3_mprintf("INSERT INTO %s (%s, %s) values ('%q', '%ld');",
				// INSERT INTO
				DB_LABELTVCHANNEL,
				// (
				DB_LABELTVCHANNEL_LABEL, DB_LABELTVCHANNEL_TVCHANNELID,
				// ) VALUES (
				label, id_tvchannel);
	res = sqlite3_exec(dbsync->db_link, query, NULL, NULL, &db_err);
	sqlite3_free(query);	
	if(res != SQLITE_OK){
		*error = g_error_new (FREETUXTV_DBSYNC_ERROR,
				      FREETUXTV_DBSYNC_ERROR_EXEC_QUERY,
				      _("Error when adding the label of channel logo '%s'.\n\nSQLite has returned error :\n%s."),
				      label, sqlite3_errmsg(dbsync->db_link));
		sqlite3_free(db_err);
	}else{
		dbsync_link_tvchannel_to_channels (dbsync, label, id_tvchannel, error);	
	}
}


static int 
on_exec_channels_group (void *data, int argc, char **argv, char **colsname)
{
	CBDBSync *cb_data = (CBDBSync *)data;

	FreetuxTVChannelsGroupInfos* channels_group_infos;
	int id = g_ascii_strtoll (argv[0], NULL, 10);
	channels_group_infos = freetuxtv_channels_group_infos_new (argv[2], argv[4]);
	freetuxtv_channels_group_infos_set_id (channels_group_infos, id);
	freetuxtv_channels_group_infos_set_regex (channels_group_infos, argv[5], argv[6]);

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
	gchar** vlc_options;
		
	int id = g_ascii_strtoll (argv[0], NULL, 10);
	channel_infos = freetuxtv_channel_infos_new (argv[1], argv[3]);
	freetuxtv_channel_infos_set_id (channel_infos, id);

	if(argv[2] != NULL){
		freetuxtv_channel_infos_set_logo(channel_infos, argv[2]);
	}
	
	if(argv[4]){
		vlc_options = g_strsplit(argv[4], "\n", 0);
		freetuxtv_channel_infos_set_vlcoptions(channel_infos, vlc_options);
		g_strfreev(vlc_options);
	}

	freetuxtv_channel_infos_set_channels_group(channel_infos, channels_group_infos);
	channels_group_infos->nb_channels++;

	int res = 0;
	res = cb_data->callback(cb_data->app, channel_infos, cb_data->dbsync, cb_data->user_data, cb_data->error);
	
	return res;
}

static void
dbsync_link_tvchannel_to_channels (DBSync *dbsync, gchar *label, glong id_tvchannel,
				   GError** error)
{
	g_return_if_fail(dbsync != NULL);
	g_return_if_fail(dbsync->db_link != NULL);
	g_return_if_fail(error != NULL);
	g_return_if_fail(*error == NULL);

	gchar *query;
	gchar *db_err = NULL;
	int res;

	// Link logo to channels 
	query = sqlite3_mprintf("UPDATE %s SET %s=%ld WHERE %s LIKE '%q%%';",
				// UPDATE
				DB_CHANNEL,
				// SET
				DB_CHANNEL_TVCHANNELID, id_tvchannel,
				// WHERE
				DB_CHANNEL_NAME, label);
	res = sqlite3_exec(dbsync->db_link, query, NULL, NULL, &db_err);
	sqlite3_free(query);	
	if(res != SQLITE_OK){
		*error = g_error_new (FREETUXTV_DBSYNC_ERROR,
				      FREETUXTV_DBSYNC_ERROR_EXEC_QUERY,
				      _("Error when linking the channel logo '%s'.\n\nSQLite has returned error :\n%s."),
				      label, sqlite3_errmsg(dbsync->db_link));
		sqlite3_free(db_err);
	}	
}
