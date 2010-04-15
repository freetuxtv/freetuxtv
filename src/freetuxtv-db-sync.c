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

#include <glib.h>
#include <sqlite3.h>

#include "freetuxtv-db-sync.h"
#include "freetuxtv-tv-channel-infos.h"

/*
typedef struct {
	FreetuxTVApp *app;
	DBSync *dbsync;
	int (*callback)();
	gpointer user_data;
	GError** error;

	gpointer cb_data1;

}CBDBSync;
*/

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
#define DB_CHANNELSGROUP_POSITION       "position"
#define DB_CHANNELSGROUP_NAME           "name"
#define DB_CHANNELSGROUP_TYPE           "type"
#define DB_CHANNELSGROUP_URI            "uri"
#define DB_CHANNELSGROUP_BREGEX         "bregex"
#define DB_CHANNELSGROUP_EREGEX         "eregex"
#define DB_CHANNELSGROUP_LASTUPDATE     "last_update"

// Table channel
#define DB_CHANNEL                     "channel"
#define DB_CHANNEL_ID                  "id"
#define DB_CHANNEL_NAME                "name"
#define DB_CHANNEL_POSITION            "position"
#define DB_CHANNEL_URI                 "uri"
#define DB_CHANNEL_VLCOPTIONS          "vlc_options"
#define DB_CHANNEL_CHANNELGROUPID      "channelsgroup_id"
#define DB_CHANNEL_TVCHANNELID         "tvchannel_id"

static void
dbsync_add_label_tvchannel (DBSync *dbsync, gchar* label, glong id_tvchannel, 
    GError** error);

static void
dbsync_link_tvchannel_to_channels_from_label (DBSync *dbsync, gchar *label,
    glong id_tvchannel, GError** error);

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
    int (*callback)(FreetuxTVApp *app, FreetuxTVChannelsGroupInfos* pChannelsGroupInfos,
	    DBSync *dbsync, gpointer user_data, GError** error),
    gpointer user_data, GError** error)
{
	g_return_if_fail(dbsync != NULL);
	g_return_if_fail(dbsync->db_link != NULL);
	g_return_if_fail(error != NULL);
	g_return_if_fail(*error == NULL);
	g_return_if_fail(app != NULL);

	gchar *zQuery;
	int res;

	zQuery = sqlite3_mprintf("\
	    SELECT %s, %s, %s, %s, %s, %s, %s \
	    FROM %s \
	    ORDER BY %s",
	    // SELECT
	    DB_CHANNELSGROUP_ID, DB_CHANNELSGROUP_POSITION, DB_CHANNELSGROUP_NAME,
	    DB_CHANNELSGROUP_TYPE, DB_CHANNELSGROUP_URI, DB_CHANNELSGROUP_BREGEX, DB_CHANNELSGROUP_EREGEX,
	    // FROM
	    DB_CHANNELSGROUP,
	    // ORDER BY
	    DB_CHANNELSGROUP_POSITION);

	sqlite3_stmt *pStmt;
	FreetuxTVChannelsGroupInfos* pChannelsGroupInfos;
	gchar *zName, *zURI;
	gchar *zBregex, *zEregex;
	gint id, position;
	
	res = sqlite3_prepare_v2(dbsync->db_link, zQuery, -1, &pStmt, NULL);
	sqlite3_free(zQuery);
	if(res == SQLITE_OK) {
		while(sqlite3_step(pStmt) == SQLITE_ROW) {
			// Create new channels group
			zName = (gchar*)sqlite3_column_text(pStmt, 2);
			zURI = (gchar*)sqlite3_column_text(pStmt, 4);
			pChannelsGroupInfos = freetuxtv_channels_group_infos_new (zName, zURI);

			// Set channels group id
			id = sqlite3_column_int(pStmt, 0);
			freetuxtv_channels_group_infos_set_id (pChannelsGroupInfos, id);
			
			// Set channels group position
			position = sqlite3_column_int(pStmt, 1);
			freetuxtv_channels_group_infos_set_position (pChannelsGroupInfos, position);

			// Set the channels group regex option
			zBregex = (gchar*)sqlite3_column_text(pStmt, 5);
			zEregex = (gchar*)sqlite3_column_text(pStmt, 6);
			freetuxtv_channels_group_infos_set_regex (pChannelsGroupInfos, zBregex, zEregex);

			// Call the callback
			if(callback){
				callback(app, pChannelsGroupInfos, dbsync, user_data, error);
			}

			// Unref the channels group
			g_object_unref(pChannelsGroupInfos);
		}
	}else{
		*error = g_error_new (FREETUXTV_DBSYNC_ERROR,
		    FREETUXTV_DBSYNC_ERROR_EXEC_QUERY,
		    _("Error when displaying the channels.\n\nSQLite has returned error :\n%s."),
		    sqlite3_errmsg(dbsync->db_link));
	}
	sqlite3_finalize(pStmt);
}

void
dbsync_select_channels_of_channels_group (DBSync *dbsync,
    FreetuxTVChannelsGroupInfos* pChannelsGroupInfos,
    FreetuxTVApp *app,
    int (*callback)(FreetuxTVApp *app, 
	    FreetuxTVChannelInfos* pChannelInfos,
	    DBSync *dbsync, gpointer user_data, GError** error),
    gpointer user_data, GError** error)
{
	g_return_if_fail(dbsync != NULL);
	g_return_if_fail(dbsync->db_link != NULL);
	g_return_if_fail(error != NULL);
	g_return_if_fail(*error == NULL);
	g_return_if_fail(app != NULL);

	gchar *zQuery;
	int res;

	zQuery = sqlite3_mprintf("\
	    SELECT %s.%s, %s.%s, %s.%s, %s.%s, %s.%s \
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
	    DB_CHANNEL, DB_CHANNEL_CHANNELGROUPID, pChannelsGroupInfos->id,
	    // ORDER BY
	    DB_CHANNEL, DB_CHANNEL_POSITION);

	sqlite3_stmt *pStmt;
	FreetuxTVChannelInfos* pChannelInfos;
	gchar *zName, *zURI;
	gchar *zLogo;
	gchar *zVlcOptions;
	gchar** dazVlcOptions;
	gint id;
	
	res = sqlite3_prepare_v2(dbsync->db_link, zQuery, -1, &pStmt, NULL);
	sqlite3_free(zQuery);
	if(res == SQLITE_OK) {
		while(sqlite3_step(pStmt) == SQLITE_ROW) {
			// Create new channel
			zName = (gchar*)sqlite3_column_text(pStmt, 1);
			zURI = (gchar*)sqlite3_column_text(pStmt, 3);
			
			pChannelInfos = freetuxtv_channel_infos_new (zName, zURI);

			// Set channel id
			id = sqlite3_column_int(pStmt, 0);
			freetuxtv_channel_infos_set_id (pChannelInfos, id);
			
			// Set channel logo
			zLogo = (gchar*)sqlite3_column_text(pStmt, 2);
			if(zLogo != NULL){
				freetuxtv_channel_infos_set_logo(pChannelInfos, zLogo);
			}

			// Set channel VLC options
			zVlcOptions = (gchar*)sqlite3_column_text(pStmt, 4);
			if(zVlcOptions){
				dazVlcOptions = g_strsplit(zVlcOptions, "\n", 0);
				freetuxtv_channel_infos_set_vlcoptions(pChannelInfos, dazVlcOptions);
				g_strfreev(dazVlcOptions);
			}

			// Set the group of the channel
			freetuxtv_channel_infos_set_channels_group(pChannelInfos, pChannelsGroupInfos);
			pChannelsGroupInfos->nb_channels++;
			
			// Call the callback
			if(callback){
				callback(app, pChannelInfos, dbsync, user_data, error);
			}

			// Unref the channel
			g_object_unref(pChannelInfos);
		}
	}else{
		*error = g_error_new (FREETUXTV_DBSYNC_ERROR,
		    FREETUXTV_DBSYNC_ERROR_EXEC_QUERY,
		    _("Error when displaying the channels.\n\nSQLite has returned error :\n%s."),
		    sqlite3_errmsg(dbsync->db_link));
	}
	sqlite3_finalize(pStmt);
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
	query = sqlite3_mprintf("\
	    INSERT INTO %s (%s, %s, %s, %s, %s, %s) \
	    VALUES \
	    ('%q',%d,'%q', %Q, (\
		    SELECT %s.%s FROM %s \
		    WHERE ('%q' LIKE %s.%s||'%%') \
		    OR %s.%s = ( \
			    SELECT %s.%s FROM %s \
			    WHERE ('%q' LIKE %s.%s||'%%') \
			    ORDER BY %s.%s DESC ) \
		    ORDER BY %s.%s DESC ) \
		    ,'%d');",
	    // INSERT INTO
	    DB_CHANNEL,
	    // (
	    DB_CHANNEL_NAME, DB_CHANNEL_POSITION, DB_CHANNEL_URI,
	    DB_CHANNEL_VLCOPTIONS, DB_CHANNEL_TVCHANNELID, DB_CHANNEL_CHANNELGROUPID,
	    // ) VALUES (
	    channel_infos->name, channel_infos->position, channel_infos->url, vlc_options,
	    //  (SELECT
	    DB_TVCHANNEL, DB_TVCHANNEL_ID,
	    //   FROM
	    DB_TVCHANNEL,
	    //   WHERE
	    channel_infos->name, DB_TVCHANNEL, DB_TVCHANNEL_NAME,
	    //   OR
	    DB_TVCHANNEL, DB_TVCHANNEL_ID,
	    //     (SELECT
	    DB_LABELTVCHANNEL, DB_LABELTVCHANNEL_TVCHANNELID, DB_LABELTVCHANNEL,
	    //      WHERE
	    channel_infos->name, DB_LABELTVCHANNEL, DB_LABELTVCHANNEL_LABEL,
	    //      ORDER BY
	    DB_LABELTVCHANNEL, DB_LABELTVCHANNEL_LABEL,
	    //     ) 
	    //   ORDER BY
	    DB_TVCHANNEL, DB_TVCHANNEL_NAME, 
	    //   )
	    channel_infos->channels_group->id
	    // )
	    );

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
    FreetuxTVChannelsGroupInfos* pChannelsGroupInfos,
    GError** error)
{
	g_return_if_fail(dbsync != NULL);
	g_return_if_fail(dbsync->db_link != NULL);
	g_return_if_fail(error != NULL);
	g_return_if_fail(*error == NULL);
	g_return_if_fail(pChannelsGroupInfos != NULL);
	g_return_if_fail(FREETUXTV_IS_CHANNELS_GROUP_INFOS(pChannelsGroupInfos));

	gchar *query;
	gchar *db_err = NULL;
	int res;
	int id_group = -1;

	// Add the group
	query = sqlite3_mprintf("INSERT INTO %s (%s, %s, %s, %s, %s, %s) \
	    VALUES ((SELECT IFNULL(MAX(%s), 0) + 1 FROM %s), '%q', %d, %Q, %Q, %Q);",
	    // INSERT INTO
	    DB_CHANNELSGROUP,
	    // (
	    DB_CHANNELSGROUP_POSITION,
	    DB_CHANNELSGROUP_NAME,
	    DB_CHANNELSGROUP_TYPE,
	    DB_CHANNELSGROUP_URI,
	    DB_CHANNELSGROUP_BREGEX,
	    DB_CHANNELSGROUP_EREGEX,
	    // ) VALUES (
	    DB_CHANNELSGROUP_POSITION, DB_CHANNELSGROUP, // rank
	    pChannelsGroupInfos->name,
	    0,
	    pChannelsGroupInfos->uri,
	    pChannelsGroupInfos->bregex,
	    pChannelsGroupInfos->eregex
	    // )
	    );

	res = sqlite3_exec(dbsync->db_link, query, NULL, NULL, &db_err);
	sqlite3_free(query);

	if(res != SQLITE_OK){
		*error = g_error_new (FREETUXTV_DBSYNC_ERROR,
		    FREETUXTV_DBSYNC_ERROR_EXEC_QUERY,
		    _("Error when adding the group \"%s\" in database.\n\nSQLite has returned error :\n%s."),
		    pChannelsGroupInfos->name,
		    sqlite3_errmsg(dbsync->db_link));
		sqlite3_free(db_err);
	}else{
		id_group = sqlite3_last_insert_rowid(dbsync->db_link);
		freetuxtv_channels_group_infos_set_id (pChannelsGroupInfos, id_group);
	}

	// Get the position of the group added
	if(*error == NULL && id_group>=0){
		query = sqlite3_mprintf("\
		    SELECT %s FROM %s \
		    WHERE %s=%d;",
		    // SELECT
		    DB_CHANNELSGROUP_POSITION,
		    // FROM
		    DB_CHANNELSGROUP,
		    // WHERE
		    DB_CHANNELSGROUP_ID, id_group
		    );

		sqlite3_stmt *pStmt;
		int new_position = -1;

		res = sqlite3_prepare_v2(dbsync->db_link, query, -1, &pStmt, NULL);
		if(res == SQLITE_OK) {
			if(sqlite3_step(pStmt) == SQLITE_ROW) {
				new_position = sqlite3_column_int(pStmt, 0);
				freetuxtv_channels_group_infos_set_position (pChannelsGroupInfos, new_position);
			}
		}else{
			*error = g_error_new (FREETUXTV_DBSYNC_ERROR,
				FREETUXTV_DBSYNC_ERROR_EXEC_QUERY,
				_("Error when adding the group \"%s\" in database.\n\nSQLite has returned error :\n%s."),
				pChannelsGroupInfos->name,
				sqlite3_errmsg(dbsync->db_link));
		}
		sqlite3_finalize(pStmt);

		sqlite3_free(query);
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
dbsync_update_channels_group_last_update (DBSync *dbsync,
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
	query = sqlite3_mprintf("\
	    UPDATE %s SET %s=DATETIME('NOW', 'localtime')\
	    WHERE %s=%d",
	    // UPDATE
	    DB_CHANNELSGROUP,
	    // SET
	    DB_CHANNELSGROUP_LASTUPDATE,
	    // WHERE
	    DB_CHANNELSGROUP_ID, channels_group_infos->id);
	res = sqlite3_exec(dbsync->db_link, query, NULL, NULL, &db_err);
	sqlite3_free(query);

	if(res != SQLITE_OK){
		*error = g_error_new (FREETUXTV_DBSYNC_ERROR,
		    FREETUXTV_DBSYNC_ERROR_EXEC_QUERY,
		    _("Error when updating the last update date of the group \"%s\".\n\nSQLite has returned error :\n%s."),
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

	if(*error == NULL){
		// Update the position of the channels group after
		query = sqlite3_mprintf("UPDATE %s SET %s=%s-1 WHERE %s>%d",
			// DELETE FROM
			DB_CHANNELSGROUP,
		    DB_CHANNELSGROUP_POSITION, DB_CHANNELSGROUP_POSITION,
			// WHERE
			DB_CHANNELSGROUP_POSITION, channels_group_infos->position);
		res = sqlite3_exec(dbsync->db_link, query, NULL, NULL, &db_err);
		sqlite3_free(query);

		if(res != SQLITE_OK){
			*error = g_error_new (FREETUXTV_DBSYNC_ERROR,
				FREETUXTV_DBSYNC_ERROR_EXEC_QUERY,
				_("Error when updating the position of the group \"%s\".\n\nSQLite has returned error :\n%s."),
				channels_group_infos->name,
				sqlite3_errmsg(dbsync->db_link));
			sqlite3_free(db_err);
		}
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

	// Delete all TV channels from the database
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
dbsync_add_tvchannel (DBSync *dbsync, FreetuxTVTvChannelInfos* tv_channel_infos,
    GError** error)
{
	g_return_if_fail(dbsync != NULL);
	g_return_if_fail(dbsync->db_link != NULL);
	g_return_if_fail(error != NULL);
	g_return_if_fail(*error == NULL);
	g_return_if_fail(tv_channel_infos != NULL);
	g_return_if_fail(FREETUXTV_IS_TV_CHANNEL_INFOS(tv_channel_infos));

	gchar *query;
	gchar *db_err = NULL;
	int res;
	int id;

	gchar* name;
	gchar* logo_filename;
	GList* cur_label;

	name = (gchar*)freetuxtv_tv_channel_infos_get_name(tv_channel_infos);
	logo_filename = (gchar*)freetuxtv_tv_channel_infos_get_logo_filename(tv_channel_infos);

	// Add the TV channel in the database
	query = sqlite3_mprintf("INSERT INTO %s (%s, %s) VALUES ('%q', %Q);",
	    // INSERT INTO
	    DB_TVCHANNEL,
	    // (
	    DB_TVCHANNEL_NAME, DB_TVCHANNEL_LOGOFILENAME,
	    // ) VALUES (
	    name, logo_filename
	    // )
	    );
	res = sqlite3_exec(dbsync->db_link, query, NULL, NULL, &db_err);
	sqlite3_free(query);	
	if(res != SQLITE_OK){
		*error = g_error_new (FREETUXTV_DBSYNC_ERROR,
		    FREETUXTV_DBSYNC_ERROR_EXEC_QUERY,
		    _("Error when adding the tv channel '%s'.\n\nSQLite has returned error :\n%s."),
		    name, sqlite3_errmsg(dbsync->db_link));
		sqlite3_free(db_err);
	}

	if(*error == NULL){
		// Get the insert id of the TV channel
		id = sqlite3_last_insert_rowid(dbsync->db_link);
		freetuxtv_tv_channel_infos_set_id(tv_channel_infos, id);
		// Link channels who has close label
		dbsync_link_tvchannel_to_channels_from_label (dbsync, name, id, error);
	}
	
	if(*error == NULL){
		// Add the label of the TV channel
		cur_label = freetuxtv_tv_channel_infos_get_labels(tv_channel_infos);
		while(cur_label && *error == NULL){
			dbsync_add_label_tvchannel (dbsync, cur_label->data, id, error);
			cur_label = cur_label->next;
		}
	}
}

static void
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

	// Add an alternative label for the TV channel
	query = sqlite3_mprintf("INSERT INTO %s (%s, %s) values ('%q', '%ld');",
	    // INSERT INTO
	    DB_LABELTVCHANNEL,
	    // (
	    DB_LABELTVCHANNEL_LABEL, DB_LABELTVCHANNEL_TVCHANNELID,
	    // ) VALUES (
	    label, id_tvchannel
	    // )
	    );
	res = sqlite3_exec(dbsync->db_link, query, NULL, NULL, &db_err);
	sqlite3_free(query);	
	if(res != SQLITE_OK){
		*error = g_error_new (FREETUXTV_DBSYNC_ERROR,
		    FREETUXTV_DBSYNC_ERROR_EXEC_QUERY,
		    _("Error when adding the label of channel logo '%s'.\n\nSQLite has returned error :\n%s."),
		    label, sqlite3_errmsg(dbsync->db_link));
		sqlite3_free(db_err);
	}else{
		dbsync_link_tvchannel_to_channels_from_label (dbsync, label, id_tvchannel, error);	
	}
}

static void
dbsync_link_tvchannel_to_channels_from_label (DBSync *dbsync, gchar *label, 
    glong id_tvchannel, GError** error)
{
	g_return_if_fail(dbsync != NULL);
	g_return_if_fail(dbsync->db_link != NULL);
	g_return_if_fail(error != NULL);
	g_return_if_fail(*error == NULL);

	gchar *query;
	gchar *db_err = NULL;
	int res;

	// Link TV channel to channels 
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
		    _("Error when linking the TV channel '%s' to channel.\n\nSQLite has returned error :\n%s."),
		    label, sqlite3_errmsg(dbsync->db_link));
		sqlite3_free(db_err);
	}	
}
