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
#include <libdbevolution/db-evolution-instance.h>

#include "freetuxtv-db-sync.h"
#include "freetuxtv-utils.h"

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

#define SQLITE_FORMAT_DATETIME          "%Y-%m-%d %H:%M:%S"
#define SQLITE_FORMAT_DATETIME00        "%Y-%m-%d %H:%M:00"

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
#define DB_CHANNEL_DEINTERLACEMODE     "deinterlace_mode"
#define DB_CHANNEL_UPDATED             "updated"
#define DB_CHANNEL_CHANNELGROUPID      "channelsgroup_id"
#define DB_CHANNEL_TVCHANNELID         "tvchannel_id"

// Table recording
#define DB_RECORDING                    "recording"
#define DB_RECORDING_ID                 "id"
#define DB_RECORDING_TITLE              "title"
#define DB_RECORDING_STATUS             "status"
#define DB_RECORDING_BEGINTIME	        "begin_date"
#define DB_RECORDING_ENDTIME	        "end_date"
#define DB_RECORDING_FILENAME	        "filename"
#define DB_RECORDING_CHANNELID	        "channel_id"

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

#define FIRST_DB_VERSION		"0.1.0.1"

static gchar*
dbsync_get_current_db_version (gpointer user_data, GError** error);

static void
dbsync_set_current_db_version (gchar* szVersion, gpointer user_data,
                               GError** error);

static int
dbsync_compare_db_version (gchar* szVersion1, gchar* szVersion2,
                           gpointer user_data, GError** error);

static gboolean
dbsync_exec_query(const gchar* szQuery, gpointer user_data,
                  GError** error);

void
dbsync_open_db(DBSync *dbsync, GError** error)
{
	g_return_if_fail(dbsync != NULL);
	g_return_if_fail(error != NULL);
	g_return_if_fail(*error == NULL);

	gchar *szPathConfigDir;
	gchar *szPathDB;
	int res;

	szPathConfigDir = g_build_filename(g_get_user_config_dir(), "FreetuxTV", NULL);
	if (!g_file_test(szPathConfigDir, G_FILE_TEST_EXISTS)) { 
          res = g_mkdir_with_parents (szPathConfigDir, 0744);
          if(res == 0){
              g_log(FREETUXTV_LOG_DOMAIN, G_LOG_LEVEL_INFO,
			      "Directory created: %s\n", szPathConfigDir);
          } else {
			  g_log(FREETUXTV_LOG_DOMAIN, G_LOG_LEVEL_CRITICAL,
			      "Cannot create directory: %s\n", szPathConfigDir);
          }
	}

	szPathDB = g_build_filename(g_get_user_config_dir(),
	    "FreetuxTV", "freetuxtv.db", NULL);

	// Open the database if not open
	g_log(FREETUXTV_LOG_DOMAIN, G_LOG_LEVEL_INFO,
	      "DBSync open database\n");
	res = sqlite3_open(szPathDB, &(dbsync->db_link));
	if(res != SQLITE_OK){
		if(error != NULL){
			// sqlite3_errmsg return const char*, no need to free it
			*error = g_error_new (FREETUXTV_DBSYNC_ERROR,
			    FREETUXTV_DBSYNC_ERROR_OPEN,
			    _("Cannot open database.\n\nSQLite has returned error:\n%s."),
			    sqlite3_errmsg(dbsync->db_link));
		}
	}

	g_free(szPathDB);
	g_free(szPathConfigDir);
}

void
dbsync_close_db (DBSync *dbsync)
{
	g_return_if_fail(dbsync != NULL);
	g_return_if_fail(dbsync->db_link != NULL);

	g_log(FREETUXTV_LOG_DOMAIN, G_LOG_LEVEL_INFO,
	      "DBSync close database\n");
	sqlite3_close(dbsync->db_link);
	dbsync->db_link = NULL;
}

gboolean
dbsync_db_exists(DBSync *dbsync)
{
	g_return_val_if_fail(dbsync != NULL, FALSE);

	gchar *user_db;
	gboolean res = FALSE;

	user_db = g_build_filename(g_get_user_config_dir(), 
	                           "FreetuxTV", "freetuxtv.db", NULL);

	if (g_file_test (user_db, G_FILE_TEST_IS_REGULAR)){
		res = TRUE;
	}
	return res;
}

void
dbsync_create_db (DBSync *dbsync, gchar* szScriptFilename, GError** error)
{
	g_return_if_fail(dbsync != NULL);
	g_return_if_fail(dbsync->db_link != NULL);
	g_return_if_fail(error != NULL);
	g_return_if_fail(*error == NULL);
	
	DbEvolutionInstance* pDbEvolutionInstance;	
	pDbEvolutionInstance = db_evolution_instance_new(szScriptFilename, freetuxtv_log);

	if(pDbEvolutionInstance){

		pDbEvolutionInstance->get_current_db_version_func = NULL;
		pDbEvolutionInstance->set_current_db_version_func = dbsync_set_current_db_version;
		pDbEvolutionInstance->compare_db_version_func = NULL;
		pDbEvolutionInstance->exec_query_func = dbsync_exec_query;
		
		db_evolution_instance_do_creation (pDbEvolutionInstance,
		                                    dbsync, error);
	}
	
	g_object_unref (pDbEvolutionInstance);
}

void
dbsync_update_db (DBSync *dbsync, gchar* szScriptFilename, GError** error)
{	
	g_return_if_fail(dbsync != NULL);
	g_return_if_fail(dbsync->db_link != NULL);
	g_return_if_fail(error != NULL);
	g_return_if_fail(*error == NULL);
	
	DbEvolutionInstance* pDbEvolutionInstance;	
	pDbEvolutionInstance = db_evolution_instance_new(szScriptFilename, freetuxtv_log);

	if(pDbEvolutionInstance){

		pDbEvolutionInstance->get_current_db_version_func = dbsync_get_current_db_version;
		pDbEvolutionInstance->set_current_db_version_func = dbsync_set_current_db_version;
		pDbEvolutionInstance->compare_db_version_func = dbsync_compare_db_version;
		pDbEvolutionInstance->exec_query_func = dbsync_exec_query;
		
		db_evolution_instance_do_evolution (pDbEvolutionInstance,
		                                    dbsync, error);
	}
	
	g_object_unref (pDbEvolutionInstance);
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
	gint id, position, type;
	
	res = sqlite3_prepare_v2(dbsync->db_link, zQuery, -1, &pStmt, NULL);
	sqlite3_free(zQuery);
	if(res == SQLITE_OK) {
		while(sqlite3_step(pStmt) == SQLITE_ROW) {
			// Create new channels group
			zName = (gchar*)sqlite3_column_text(pStmt, 2);
			type = sqlite3_column_int(pStmt, 3);
			zURI = (gchar*)sqlite3_column_text(pStmt, 4);
			pChannelsGroupInfos = freetuxtv_channels_group_infos_new (zName,
				type);
			freetuxtv_channels_group_infos_set_uri (pChannelsGroupInfos, zURI);

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
		    _("Error when displaying the channels.\n\nSQLite has returned error:\n%s."),
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
	    SELECT %s.%s, %s.%s, %s.%s, %s.%s, %s.%s, %s.%s, %s.%s \
	    FROM %s LEFT JOIN %s ON %s.%s=%s.%s \
	    WHERE %s.%s=%d \
	    ORDER BY %s.%s",
	    // SELECT
	    DB_CHANNEL, DB_CHANNEL_ID, DB_CHANNEL, DB_CHANNEL_NAME, DB_CHANNEL, DB_CHANNEL_POSITION,
	    DB_TVCHANNEL, DB_TVCHANNEL_LOGOFILENAME, DB_CHANNEL, DB_CHANNEL_URI,
	    DB_CHANNEL, DB_CHANNEL_VLCOPTIONS, DB_CHANNEL, DB_CHANNEL_DEINTERLACEMODE,
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
	gchar *zDeinterlaceMode;
	gint id, position;
	
	res = sqlite3_prepare_v2(dbsync->db_link, zQuery, -1, &pStmt, NULL);
	sqlite3_free(zQuery);
	if(res == SQLITE_OK) {
		while(sqlite3_step(pStmt) == SQLITE_ROW) {
			// Create new channel
			zName = (gchar*)sqlite3_column_text(pStmt, 1);
			zURI = (gchar*)sqlite3_column_text(pStmt, 4);
			
			pChannelInfos = freetuxtv_channel_infos_new (zName, zURI);

			// Set channel id
			id = sqlite3_column_int(pStmt, 0);
			freetuxtv_channel_infos_set_id (pChannelInfos, id);

			// Set the position
			position = sqlite3_column_int(pStmt, 2);
			freetuxtv_channel_infos_set_position (pChannelInfos, position);
			
			
			// Set channel logo
			zLogo = (gchar*)sqlite3_column_text(pStmt, 3);
			if(zLogo != NULL){
				freetuxtv_channel_infos_set_logo(pChannelInfos, zLogo);
			}

			// Set channel VLC options
			zVlcOptions = (gchar*)sqlite3_column_text(pStmt, 5);
			if(zVlcOptions){
				dazVlcOptions = g_strsplit(zVlcOptions, "\n", 0);
				freetuxtv_channel_infos_set_vlcoptions(pChannelInfos, dazVlcOptions);
				g_strfreev(dazVlcOptions);
			}

			// Set deinterlace mode
			zDeinterlaceMode = (gchar*)sqlite3_column_text(pStmt, 6);
			freetuxtv_channel_infos_set_deinterlace_mode (pChannelInfos, zDeinterlaceMode);

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
		    _("Error when displaying the channels.\n\nSQLite has returned error:\n%s."),
		    sqlite3_errmsg(dbsync->db_link));
	}
	sqlite3_finalize(pStmt);
}

void
dbsync_add_channel (DBSync *dbsync,
    FreetuxTVChannelInfos* pChannelInfos,
    gboolean update,
    GError** error)
{
	g_return_if_fail(dbsync != NULL);
	g_return_if_fail(dbsync->db_link != NULL);
	g_return_if_fail(error != NULL);
	g_return_if_fail(*error == NULL);
	g_return_if_fail(pChannelInfos != NULL);
	g_return_if_fail(FREETUXTV_IS_CHANNEL_INFOS(pChannelInfos));

	gchar *query;
	gchar *db_err = NULL;
	int res;
	sqlite3_stmt *pStmt;

	gchar* vlc_options = NULL;
	gint id_channel = -1;

	if(pChannelInfos->vlc_options){
		vlc_options = g_strjoinv ("\n", pChannelInfos->vlc_options);
	}

	if(update){
		// We look if we have a channel in the group with the same URI
		query = sqlite3_mprintf("\
			SELECT %s FROM %s WHERE %s='%q' AND %s=%d AND %s=0 \
			ORDER BY %s.%s;",
			// SELECT
			DB_CHANNEL_ID,
			// FROM
			DB_CHANNEL,
			// WHERE
			DB_CHANNEL_URI, pChannelInfos->url,
			// AND
			DB_CHANNEL_CHANNELGROUPID, pChannelInfos->channels_group->id,
			// AND
			DB_CHANNEL_UPDATED,
			// ORDER BY
			DB_CHANNEL, DB_CHANNEL_POSITION
			);
		res = sqlite3_prepare_v2(dbsync->db_link, query, -1, &pStmt, NULL);
		sqlite3_free(query);
		if(res == SQLITE_OK) {
			while(sqlite3_step(pStmt) == SQLITE_ROW && id_channel==-1) {
				id_channel = (gint)sqlite3_column_int(pStmt, 0);
			}
		}else{
			*error = g_error_new (FREETUXTV_DBSYNC_ERROR,
				FREETUXTV_DBSYNC_ERROR_EXEC_QUERY,
				_("Error when getting the channel.\n\nSQLite has returned error:\n%s."),
				sqlite3_errmsg(dbsync->db_link));
		}
		sqlite3_finalize(pStmt);
	}
	
	if(id_channel == -1){
		// If not found we just add the channel
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
			pChannelInfos->name, pChannelInfos->position, pChannelInfos->url, vlc_options,
			//  (SELECT
			DB_TVCHANNEL, DB_TVCHANNEL_ID,
			//   FROM
			DB_TVCHANNEL,
			//   WHERE
			pChannelInfos->name, DB_TVCHANNEL, DB_TVCHANNEL_NAME,
			//   OR
			DB_TVCHANNEL, DB_TVCHANNEL_ID,
			//     (SELECT
			DB_LABELTVCHANNEL, DB_LABELTVCHANNEL_TVCHANNELID, DB_LABELTVCHANNEL,
			//      WHERE
			pChannelInfos->name, DB_LABELTVCHANNEL, DB_LABELTVCHANNEL_LABEL,
			//      ORDER BY
			DB_LABELTVCHANNEL, DB_LABELTVCHANNEL_LABEL,
			//     ) 
			//   ORDER BY
			DB_TVCHANNEL, DB_TVCHANNEL_NAME, 
			//   )
			pChannelInfos->channels_group->id
			// )
			);
	}else{
		query = sqlite3_mprintf("\
			UPDATE %s SET %s='%q', %s=%d, %s=%Q, %s=1 \
			WHERE %s=%d",
			// UPDATE
			DB_CHANNEL,
			// SET
			DB_CHANNEL_NAME, pChannelInfos->name,
		    DB_CHANNEL_POSITION, pChannelInfos->position,
			DB_CHANNEL_VLCOPTIONS, vlc_options,
		    DB_CHANNEL_UPDATED,
		    // WHERE
			DB_CHANNEL_ID, id_channel
			);
	}

	res = sqlite3_exec(dbsync->db_link, query, NULL, NULL, &db_err);
	sqlite3_free(query);

	if(vlc_options){
		g_free(vlc_options);
	}

	if(res != SQLITE_OK){
		*error = g_error_new (FREETUXTV_DBSYNC_ERROR,
		    FREETUXTV_DBSYNC_ERROR_EXEC_QUERY,
		    _("Error when adding the channel \"%s\".\n\nSQLite has returned error:\n%s."),
		    pChannelInfos->name, sqlite3_errmsg(dbsync->db_link));
		sqlite3_free(db_err);
	}else{
		if(id_channel == -1){
			freetuxtv_channel_infos_set_id (pChannelInfos,
				(int)sqlite3_last_insert_rowid(dbsync->db_link));
		}else{
			freetuxtv_channel_infos_set_id (pChannelInfos, id_channel);
		}
	}	
}

void
dbsync_delete_channel (DBSync *dbsync,
    FreetuxTVChannelInfos* pChannelInfos,
    GError** error)
{
	g_return_if_fail(dbsync != NULL);
	g_return_if_fail(dbsync->db_link != NULL);
	g_return_if_fail(error != NULL);
	g_return_if_fail(*error == NULL);
	g_return_if_fail(pChannelInfos != NULL);
	g_return_if_fail(FREETUXTV_IS_CHANNEL_INFOS(pChannelInfos));

	gchar *query;
	gchar *db_err = NULL;
	int res;

	// Delete the channels group
	query = sqlite3_mprintf("DELETE FROM %s WHERE %s=%d",
	    // DELETE FROM
	    DB_CHANNEL,
	    // WHERE
	    DB_CHANNEL_ID, pChannelInfos->id);
	res = sqlite3_exec(dbsync->db_link, query, NULL, NULL, &db_err);
	sqlite3_free(query);

	if(res != SQLITE_OK){
		*error = g_error_new (FREETUXTV_DBSYNC_ERROR,
		    FREETUXTV_DBSYNC_ERROR_EXEC_QUERY,
		    _("Error when deleting the channel \"%s\".\n\nSQLite has returned error:\n%s."),
		    pChannelInfos->name,
		    sqlite3_errmsg(dbsync->db_link));
		sqlite3_free(db_err);
	}

	if(*error == NULL){
		// Update the position of the channels group after
		query = sqlite3_mprintf("UPDATE %s SET %s=%s-1 WHERE %s>%d AND %s=%d",
			// UPDATE
			DB_CHANNEL,
		    // SET
		    DB_CHANNEL_POSITION, DB_CHANNEL_POSITION,
			// WHERE
			DB_CHANNEL_POSITION, pChannelInfos->position,
		    // AND
		    DB_CHANNEL_CHANNELGROUPID, pChannelInfos->channels_group->id);
		res = sqlite3_exec(dbsync->db_link, query, NULL, NULL, &db_err);
		sqlite3_free(query);

		if(res != SQLITE_OK){
			*error = g_error_new (FREETUXTV_DBSYNC_ERROR,
				FREETUXTV_DBSYNC_ERROR_EXEC_QUERY,
				_("Error when updating the position of the channels in group \"%s\".\n\nSQLite has returned error:\n%s."),
				pChannelInfos->channels_group->name,
				sqlite3_errmsg(dbsync->db_link));
			sqlite3_free(db_err);
		}
	}
}

int
dbsync_get_channel_id_by_name (DBSync *dbsync,
    const gchar* szChannelName,
    GError** error)
{
	g_return_val_if_fail(dbsync != NULL, -1);
	g_return_val_if_fail(dbsync->db_link != NULL, -1);
	g_return_val_if_fail(error != NULL, -1);
	g_return_val_if_fail(*error == NULL, -1);

	int res;
	gchar *zQuery;
	int id = -1;
	sqlite3_stmt *pStmt;

	// First, we look for a channel with name which match exactly
	if(id == -1){
		zQuery = sqlite3_mprintf("\
			SELECT %s.%s \
			FROM %s, %s \
			WHERE %s.%s=%s.%s \
			AND %s.%s='%q' \
			ORDER BY %s.%s, %s.%s \
		    LIMIT 1",
			// SELECT
			DB_CHANNEL, DB_CHANNEL_ID,
			// FROM
			DB_CHANNELSGROUP, DB_CHANNEL,
			// WHERE
			DB_CHANNELSGROUP, DB_CHANNELSGROUP_ID, DB_CHANNEL, DB_CHANNEL_CHANNELGROUPID,
		    // AND
            DB_CHANNEL, DB_CHANNEL_NAME, szChannelName,
			// ORDER BY
			DB_CHANNELSGROUP, DB_CHANNELSGROUP_POSITION, DB_CHANNEL, DB_CHANNEL_POSITION);

		res = sqlite3_prepare_v2(dbsync->db_link, zQuery, -1, &pStmt, NULL);
		sqlite3_free(zQuery);
		if(res == SQLITE_OK) {
			while(sqlite3_step(pStmt) == SQLITE_ROW) {
				// Set channels group id
				id = sqlite3_column_int(pStmt, 0);
			}
		}else{
			*error = g_error_new (FREETUXTV_DBSYNC_ERROR,
				FREETUXTV_DBSYNC_ERROR_EXEC_QUERY,
				_("Error when getting the channel by name.\n\nSQLite has returned error:\n%s."),
				sqlite3_errmsg(dbsync->db_link));
		}
		sqlite3_finalize(pStmt);
	}
	
	// Next, we look for a TV channel with name which match exactly
	if(id == -1){
		zQuery = sqlite3_mprintf("\
			SELECT %s.%s \
			FROM %s, %s, %s \
			WHERE %s.%s=%s.%s \
			AND %s.%s=%s.%s \
			AND %s.%s='%q' \
			ORDER BY %s.%s, %s.%s \
		    LIMIT 1",
			// SELECT
			DB_CHANNEL, DB_CHANNEL_ID,
			// FROM
			DB_CHANNELSGROUP, DB_CHANNEL, DB_TVCHANNEL,
			// WHERE
			DB_CHANNELSGROUP, DB_CHANNELSGROUP_ID, DB_CHANNEL, DB_CHANNEL_CHANNELGROUPID,
			// AND
			DB_CHANNEL, DB_CHANNEL_TVCHANNELID, DB_TVCHANNEL, DB_TVCHANNEL_ID,
		    // AND
            DB_TVCHANNEL, DB_TVCHANNEL_NAME, szChannelName,
			// ORDER BY
			DB_CHANNELSGROUP, DB_CHANNELSGROUP_POSITION, DB_CHANNEL, DB_CHANNEL_POSITION);

		res = sqlite3_prepare_v2(dbsync->db_link, zQuery, -1, &pStmt, NULL);
		sqlite3_free(zQuery);
		if(res == SQLITE_OK) {
			while(sqlite3_step(pStmt) == SQLITE_ROW) {
				// Set channels group id
				id = sqlite3_column_int(pStmt, 0);
			}
		}else{
			*error = g_error_new (FREETUXTV_DBSYNC_ERROR,
				FREETUXTV_DBSYNC_ERROR_EXEC_QUERY,
				_("Error when getting the channel by name.\n\nSQLite has returned error:\n%s."),
				sqlite3_errmsg(dbsync->db_link));
		}
		sqlite3_finalize(pStmt);
	}
	
	return id;
}

void
dbsync_update_channel_deinterlace_mode (DBSync *dbsync,
    FreetuxTVChannelInfos* pChannelInfos,
    gchar *mode,
    GError** error)
{
	g_return_if_fail(dbsync != NULL);
	g_return_if_fail(dbsync->db_link != NULL);
	g_return_if_fail(error != NULL);
	g_return_if_fail(*error == NULL);
	g_return_if_fail(pChannelInfos != NULL);
	g_return_if_fail(FREETUXTV_IS_CHANNEL_INFOS(pChannelInfos));

	gchar *query;
	gchar *db_err = NULL;
	int res;

	// Update the group
	query = sqlite3_mprintf("UPDATE %s SET %s=%Q WHERE %s=%d",
	    // UPDATE
	    DB_CHANNEL,
	    // SET
	    DB_CHANNEL_DEINTERLACEMODE, mode,
	    // WHERE
	    DB_CHANNEL_ID, pChannelInfos->id);
	res = sqlite3_exec(dbsync->db_link, query, NULL, NULL, &db_err);
	sqlite3_free(query);

	if(res != SQLITE_OK){
		*error = g_error_new (FREETUXTV_DBSYNC_ERROR,
		    FREETUXTV_DBSYNC_ERROR_EXEC_QUERY,
		    _("Error when updating the channel \"%s\".\n\nSQLite has returned error:\n%s."),
		    pChannelInfos->name, sqlite3_errmsg(dbsync->db_link));
		sqlite3_free(db_err);
	}

	if(*error == NULL){
		freetuxtv_channel_infos_set_deinterlace_mode (pChannelInfos, mode);
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
	    pChannelsGroupInfos->type,
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
		    _("Error when adding the group \"%s\" in database.\n\nSQLite has returned error:\n%s."),
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
				_("Error when adding the group \"%s\" in database.\n\nSQLite has returned error:\n%s."),
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
		    _("Error when updating the group \"%s\".\n\nSQLite has returned error:\n%s."),
		    channels_group_infos->name, sqlite3_errmsg(dbsync->db_link));
		sqlite3_free(db_err);
	}
}

void
dbsync_switch_position_channels_group (DBSync *dbsync,
    FreetuxTVChannelsGroupInfos* pChannelsGroupInfosA,
    FreetuxTVChannelsGroupInfos* pChannelsGroupInfosB,
    GError** error)
{
	g_return_if_fail(dbsync != NULL);
	g_return_if_fail(dbsync->db_link != NULL);
	g_return_if_fail(error != NULL);
	g_return_if_fail(*error == NULL);
	g_return_if_fail(pChannelsGroupInfosA != NULL);
	g_return_if_fail(FREETUXTV_IS_CHANNELS_GROUP_INFOS(pChannelsGroupInfosA));
	g_return_if_fail(pChannelsGroupInfosB != NULL);
	g_return_if_fail(FREETUXTV_IS_CHANNELS_GROUP_INFOS(pChannelsGroupInfosB));

	gchar *query;
	gchar *db_err = NULL;
	int res;

	int tmp;

	// Update the group
	query = sqlite3_mprintf("UPDATE %s SET %s=%d WHERE %s=%d",
	    // UPDATE
	    DB_CHANNELSGROUP,
	    // SET
	    DB_CHANNELSGROUP_POSITION, pChannelsGroupInfosB->position,
	    // WHERE
	    DB_CHANNELSGROUP_ID, pChannelsGroupInfosA->id);
	res = sqlite3_exec(dbsync->db_link, query, NULL, NULL, &db_err);
	sqlite3_free(query);

	if(res != SQLITE_OK){
		*error = g_error_new (FREETUXTV_DBSYNC_ERROR,
		    FREETUXTV_DBSYNC_ERROR_EXEC_QUERY,
		    _("Error when updating the group \"%s\".\n\nSQLite has returned error:\n%s."),
		    pChannelsGroupInfosA->name, sqlite3_errmsg(dbsync->db_link));
		sqlite3_free(db_err);
	}

	if(*error == NULL){
		tmp = pChannelsGroupInfosA->position;
		pChannelsGroupInfosA->position = pChannelsGroupInfosB->position;
		
		// Update the group
		query = sqlite3_mprintf("UPDATE %s SET %s=%d WHERE %s=%d",
			// UPDATE
			DB_CHANNELSGROUP,
			// SET
			DB_CHANNELSGROUP_POSITION, tmp,
			// WHERE
			DB_CHANNELSGROUP_ID, pChannelsGroupInfosB->id);
		res = sqlite3_exec(dbsync->db_link, query, NULL, NULL, &db_err);
		sqlite3_free(query);

		if(res != SQLITE_OK){
			*error = g_error_new (FREETUXTV_DBSYNC_ERROR,
				FREETUXTV_DBSYNC_ERROR_EXEC_QUERY,
				_("Error when updating the group \"%s\".\n\nSQLite has returned error:\n%s."),
				pChannelsGroupInfosB->name, sqlite3_errmsg(dbsync->db_link));
			sqlite3_free(db_err);
		}
	}

	if(*error == NULL){
		pChannelsGroupInfosB->position = tmp;
	}
}

void
dbsync_switch_position_channel (DBSync *dbsync,
    FreetuxTVChannelInfos* pChannelInfosA,
    FreetuxTVChannelInfos* pChannelInfosB,
    GError** error)
{
	g_return_if_fail(dbsync != NULL);
	g_return_if_fail(dbsync->db_link != NULL);
	g_return_if_fail(error != NULL);
	g_return_if_fail(*error == NULL);
	g_return_if_fail(pChannelInfosA != NULL);
	g_return_if_fail(FREETUXTV_IS_CHANNEL_INFOS(pChannelInfosA));
	g_return_if_fail(pChannelInfosB != NULL);
	g_return_if_fail(FREETUXTV_IS_CHANNEL_INFOS(pChannelInfosB));

	gchar *query;
	gchar *db_err = NULL;
	int res;

	int tmp;

	// Update the group
	query = sqlite3_mprintf("UPDATE %s SET %s=%d WHERE %s=%d",
	    // UPDATE
	    DB_CHANNEL,
	    // SET
	    DB_CHANNEL_POSITION, pChannelInfosB->position,
	    // WHERE
	    DB_CHANNEL_ID, pChannelInfosA->id);
	res = sqlite3_exec(dbsync->db_link, query, NULL, NULL, &db_err);
	sqlite3_free(query);

	if(res != SQLITE_OK){
		*error = g_error_new (FREETUXTV_DBSYNC_ERROR,
		    FREETUXTV_DBSYNC_ERROR_EXEC_QUERY,
		    _("Error when updating the channel \"%s\".\n\nSQLite has returned error:\n%s."),
		    pChannelInfosA->name, sqlite3_errmsg(dbsync->db_link));
		sqlite3_free(db_err);
	}

	if(*error == NULL){
		tmp = pChannelInfosA->position;
		pChannelInfosA->position = pChannelInfosB->position;
		
		// Update the group
		query = sqlite3_mprintf("UPDATE %s SET %s=%d WHERE %s=%d",
			// UPDATE
			DB_CHANNEL,
			// SET
			DB_CHANNEL_POSITION, tmp,
			// WHERE
			DB_CHANNEL_ID, pChannelInfosB->id);
		res = sqlite3_exec(dbsync->db_link, query, NULL, NULL, &db_err);
		sqlite3_free(query);

		if(res != SQLITE_OK){
			*error = g_error_new (FREETUXTV_DBSYNC_ERROR,
				FREETUXTV_DBSYNC_ERROR_EXEC_QUERY,
				_("Error when updating the channel \"%s\".\n\nSQLite has returned error:\n%s."),
				pChannelInfosB->name, sqlite3_errmsg(dbsync->db_link));
			sqlite3_free(db_err);
		}
	}

	if(*error == NULL){
		pChannelInfosB->position = tmp;
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
		    _("Error when updating the last update date of the group \"%s\".\n\nSQLite has returned error:\n%s."),
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
		    _("Error when deleting the group \"%s\".\n\nSQLite has returned error:\n%s."),
		    channels_group_infos->name,
		    sqlite3_errmsg(dbsync->db_link));
		sqlite3_free(db_err);
	}

	if(*error == NULL){
		// Update the position of the channels group after
		query = sqlite3_mprintf("UPDATE %s SET %s=%s-1 WHERE %s>%d",
			// UPDATE
			DB_CHANNELSGROUP,
		    // SET
		    DB_CHANNELSGROUP_POSITION, DB_CHANNELSGROUP_POSITION,
			// WHERE
			DB_CHANNELSGROUP_POSITION, channels_group_infos->position);
		res = sqlite3_exec(dbsync->db_link, query, NULL, NULL, &db_err);
		sqlite3_free(query);

		if(res != SQLITE_OK){
			*error = g_error_new (FREETUXTV_DBSYNC_ERROR,
				FREETUXTV_DBSYNC_ERROR_EXEC_QUERY,
				_("Error when updating the position of the groups.\n\nSQLite has returned error:\n%s."),
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
		    _("Error when deleting the channels of the group \"%s\".\n\nSQLite has returned error:\n%s."),
		    channels_group_infos->name,
		    sqlite3_errmsg(dbsync->db_link));
		sqlite3_free(db_err);
	}
}

void
dbsync_start_update_channels_of_channels_group (DBSync *dbsync,
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

	// Delete the channels of the channels group
	query = sqlite3_mprintf("UPDATE %s SET %s=0 WHERE %s=%d",
	    // UPDATE
	    DB_CHANNEL,
	    // SET
	    DB_CHANNEL_UPDATED,
	    // WHERE
	    DB_CHANNEL_CHANNELGROUPID, pChannelsGroupInfos->id);
	res = sqlite3_exec(dbsync->db_link, query, NULL, NULL, &db_err);
	sqlite3_free(query);

	if(res != SQLITE_OK){
		*error = g_error_new (FREETUXTV_DBSYNC_ERROR,
		    FREETUXTV_DBSYNC_ERROR_EXEC_QUERY,
		    _("Error when updating the channels of the group \"%s\".\n\nSQLite has returned error:\n%s."),
		    pChannelsGroupInfos->name,
		    sqlite3_errmsg(dbsync->db_link));
		sqlite3_free(db_err);
	}
}

void
dbsync_end_update_channels_of_channels_group (DBSync *dbsync,
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

	// Delete the channels of the channels group
	query = sqlite3_mprintf("DELETE FROM %s WHERE %s=%d AND %s=0",
	    // DELETE FROM
	    DB_CHANNEL,
	    // WHERE
	    DB_CHANNEL_CHANNELGROUPID, pChannelsGroupInfos->id,
	    DB_CHANNEL_UPDATED);
	res = sqlite3_exec(dbsync->db_link, query, NULL, NULL, &db_err);
	sqlite3_free(query);

	if(res != SQLITE_OK){
		*error = g_error_new (FREETUXTV_DBSYNC_ERROR,
		    FREETUXTV_DBSYNC_ERROR_EXEC_QUERY,
		    _("Error when updating the channels of the group \"%s\".\n\nSQLite has returned error:\n%s."),
		    pChannelsGroupInfos->name,
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
		    _("Error when deleting the tv channels list.\n\nSQLite has returned error:\n%s."),
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
		    _("Error when adding the tv channel '%s'.\n\nSQLite has returned error:\n%s."),
		    name, sqlite3_errmsg(dbsync->db_link));
		sqlite3_free(db_err);
	}

	if(*error == NULL){
		// Get the insert id of the TV channel
		id = sqlite3_last_insert_rowid(dbsync->db_link);
		freetuxtv_tv_channel_infos_set_id(tv_channel_infos, id);
		// Link channels which have a near label
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

void
dbsync_add_recording (DBSync *dbsync, FreetuxTVRecordingInfos* pRecordingInfos,
    GError** error)
{
	g_return_if_fail(dbsync != NULL);
	g_return_if_fail(dbsync->db_link != NULL);
	g_return_if_fail(error != NULL);
	g_return_if_fail(*error == NULL);
	g_return_if_fail(pRecordingInfos != NULL);
	g_return_if_fail(FREETUXTV_IS_RECORDING_INFOS(pRecordingInfos));

	gchar *query;
	gchar *db_err = NULL;
	int res;
	int id;

	gchar* szTitle = pRecordingInfos->szTitle;
	gchar* szChannelId = NULL;
	if(pRecordingInfos->channel_id != -1){
		szChannelId = g_strdup_printf("%d", pRecordingInfos->channel_id);
	}

	gchar* szBeginTime = g_time_int64_to_string(pRecordingInfos->beginTime, SQLITE_FORMAT_DATETIME00);
	gchar* szEndTime = g_time_int64_to_string(pRecordingInfos->endTime, SQLITE_FORMAT_DATETIME00);

	// Add the TV channel in the database
	query = sqlite3_mprintf("INSERT INTO %s (%s, %s, %s, %s, %s, %s) VALUES ('%q', %d, %Q, %Q, %Q, %Q);",
	    // INSERT INTO
	    DB_RECORDING,
	    // (
	    DB_RECORDING_TITLE, DB_RECORDING_STATUS, DB_RECORDING_BEGINTIME, DB_RECORDING_ENDTIME,
	    DB_RECORDING_FILENAME, DB_RECORDING_CHANNELID,
	    // ) VALUES (
	    szTitle, pRecordingInfos->status, szBeginTime, szEndTime, NULL, szChannelId
	    // )
	    );
	res = sqlite3_exec(dbsync->db_link, query, NULL, NULL, &db_err);
	sqlite3_free(query);	
	if(res != SQLITE_OK){
		*error = g_error_new (FREETUXTV_DBSYNC_ERROR,
		    FREETUXTV_DBSYNC_ERROR_EXEC_QUERY,
		    _("Error when adding the recording '%s'.\n\nSQLite has returned error:\n%s."),
		    szTitle, sqlite3_errmsg(dbsync->db_link));
		sqlite3_free(db_err);
	}

	if(*error == NULL){
		// Get the insert id of the TV channel
		id = sqlite3_last_insert_rowid(dbsync->db_link);
		pRecordingInfos->id = id;
	}

	if(szChannelId){
		g_free(szChannelId);
		szChannelId = NULL;
	}
}

void
dbsync_select_recordings (DBSync *dbsync,
    FreetuxTVApp *app,
    int (*callback)(FreetuxTVApp *app, 
	    FreetuxTVRecordingInfos* pRecordingInfos,
	    DBSync *dbsync, gpointer user_data, GError** error),
    gpointer user_data, GError** error)
{
	g_return_if_fail(dbsync != NULL);
	g_return_if_fail(dbsync->db_link != NULL);
	g_return_if_fail(error != NULL);
	g_return_if_fail(*error == NULL);
	g_return_if_fail(app != NULL);

	gchar *szQuery;
	int res;	
	
	szQuery = sqlite3_mprintf("\
	    SELECT %s, %s, %s, %s, %s, %s, %s \
	    FROM %s \
	    ORDER BY %s",
	    // SELECT
	    DB_RECORDING_ID, DB_RECORDING_TITLE, DB_RECORDING_STATUS, DB_RECORDING_BEGINTIME, DB_RECORDING_ENDTIME,
	    DB_RECORDING_FILENAME, DB_RECORDING_CHANNELID,
	    // FROM
	    DB_RECORDING,
	    // ORDER BY
	    DB_RECORDING_BEGINTIME);

	sqlite3_stmt *pStmt;
	
	FreetuxTVRecordingInfos* pRecordingInfos;
	
	gchar *szTitle;
	gchar *szBeginTime = 0;
	gchar *szEndTime = 0;
	gint64 beginTime = 0;
	gint64 endTime = 0;
	gint id, channel_id, status;
	gchar *szFileName = NULL;
	
	res = sqlite3_prepare_v2(dbsync->db_link, szQuery, -1, &pStmt, NULL);
	sqlite3_free(szQuery);
	if(res == SQLITE_OK) {
		while(sqlite3_step(pStmt) == SQLITE_ROW) {
			// Create new recording
			szTitle = (gchar*)sqlite3_column_text(pStmt, 1);
			status = sqlite3_column_int(pStmt, 2);
			szBeginTime = (gchar*)sqlite3_column_text(pStmt, 3);
			szEndTime = (gchar*)sqlite3_column_text(pStmt, 4);
			szFileName = (gchar*)sqlite3_column_text(pStmt, 5);
			channel_id = sqlite3_column_int(pStmt, 6);

			beginTime = g_string_to_time_int64(szBeginTime, SQLITE_FORMAT_DATETIME);
			endTime = g_string_to_time_int64(szEndTime, SQLITE_FORMAT_DATETIME);
			
			pRecordingInfos = freetuxtv_recording_infos_new (szTitle, beginTime, endTime, channel_id);
			freetuxtv_recording_infos_set_status (pRecordingInfos, (FREETUXTV_RECORDING_STATUS)status);
			freetuxtv_recording_infos_set_filename (pRecordingInfos, szFileName);

			// Set the ID
			id = sqlite3_column_int(pStmt, 0);
			freetuxtv_recording_infos_set_id(pRecordingInfos, id);
			
			// Call the callback
			if(callback){
				callback(app, pRecordingInfos, dbsync, user_data, error);
			}

			// Unref the channel
			g_object_unref(pRecordingInfos);
		}
	}else{
		*error = g_error_new (FREETUXTV_DBSYNC_ERROR,
		    FREETUXTV_DBSYNC_ERROR_EXEC_QUERY,
		    _("Error when displaying the channels.\n\nSQLite has returned error:\n%s."),
		    sqlite3_errmsg(dbsync->db_link));
	}
	sqlite3_finalize(pStmt);
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
		    _("Error when adding the label of channel logo '%s'.\n\nSQLite has returned error:\n%s."),
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
		    _("Error when linking the TV channel '%s' to channel.\n\nSQLite has returned error:\n%s."),
		    label, sqlite3_errmsg(dbsync->db_link));
		sqlite3_free(db_err);
	}
}

static gchar*
dbsync_get_current_db_version (gpointer user_data, GError** error)
{
	DBSync *dbsync = (DBSync*)user_data;
	
	g_return_val_if_fail(dbsync != NULL, NULL);
	g_return_val_if_fail(dbsync->db_link != NULL, NULL);
	g_return_val_if_fail(error != NULL, NULL);
	g_return_val_if_fail(*error == NULL, NULL);

	gchar *query;
	int res;

	gchar* szVersion = NULL;

	// Get the current version of the database
	sqlite3_stmt *pStmt;
	query = sqlite3_mprintf("SELECT dbversion FROM config WHERE id=1");
	res = sqlite3_prepare_v2(dbsync->db_link, query, -1, &pStmt, NULL);
	sqlite3_free(query);
	
	if(res == SQLITE_OK){
		if(sqlite3_step(pStmt) == SQLITE_ROW) {
			szVersion = g_strdup((gchar*)sqlite3_column_text(pStmt, 0));
		}else{
			szVersion = g_strdup(FIRST_DB_VERSION);
		}
		sqlite3_finalize(pStmt);
	}else if(res == SQLITE_ERROR){
		szVersion = g_strdup(FIRST_DB_VERSION);
	}else{
		*error = g_error_new (FREETUXTV_DBSYNC_ERROR,
		    FREETUXTV_DBSYNC_ERROR_EXEC_QUERY,
			_("Error when getting the database version.\n\nSQLite has returned error:\n%s."),
		    sqlite3_errmsg(dbsync->db_link));
	}
	
	return szVersion;
}

static void
dbsync_set_current_db_version (gchar* szVersion, gpointer user_data,
                               GError** error)
{
	DBSync *dbsync = (DBSync*)user_data;
	
	g_return_if_fail(dbsync != NULL);
	g_return_if_fail(dbsync->db_link != NULL);
	g_return_if_fail(error != NULL);
	g_return_if_fail(*error == NULL);

	gchar *query;
	gchar *db_err = NULL;
	int res;

	// Update the database version
	res = dbsync_compare_db_version(szVersion, FIRST_DB_VERSION, NULL, error);
	if(*error == NULL && res != 0){
		query = sqlite3_mprintf("REPLACE INTO config (id, dbversion) VALUES (1, '%q');",
				                szVersion);
		res = sqlite3_exec(dbsync->db_link, query, NULL, NULL, &db_err);
		sqlite3_free(query);	
		if(res != SQLITE_OK){
			*error = g_error_new (FREETUXTV_DBSYNC_ERROR,
				FREETUXTV_DBSYNC_ERROR_EXEC_QUERY,
				_("Error when updating the database version.\n\nSQLite has returned error:\n%s."),
				sqlite3_errmsg(dbsync->db_link));
			sqlite3_free(db_err);
		}
	}
}

int
dbsync_compare_db_version (gchar* szVersion1, gchar* szVersion2, 
                           gpointer user_data, GError** error)
{
	int M1, m1, r1, b1;
	int M2, m2, r2, b2;

	sscanf(szVersion1, "%d.%d.%d.%d", &M1, &m1, &r1, &b1);
	sscanf(szVersion2, "%d.%d.%d.%d", &M2, &m2, &r2, &b2);

	if(M1 > M2){
		return 1;
	}
	if(M1 < M2){
		return -1;
	}
	if(m1 > m2){
		return 1;
	}
	if(m1 < m2){
		return -1;
	}
	if(r1 > r2){
		return 1;
	}
	if(r1 < r2){
		return -1;
	}
	if(b1 > b2){
		return 1;
	}
	if(b1 < b2){
		return -1;
	}
	return 0;
}


static gboolean
dbsync_exec_query(const gchar* szQuery, gpointer user_data,
                  GError** error)
{
	DBSync *dbsync = (DBSync*)user_data;
	
	g_return_val_if_fail(dbsync != NULL, FALSE);
	g_return_val_if_fail(dbsync->db_link != NULL, FALSE);
	g_return_val_if_fail(error != NULL, FALSE);
	g_return_val_if_fail(*error == NULL, FALSE);

	gboolean bRes = FALSE;

	gchar *db_err = NULL;
	int res;

	res = sqlite3_exec(dbsync->db_link, szQuery, NULL, NULL, &db_err);	
	if(res != SQLITE_OK){
		*error = g_error_new (FREETUXTV_DBSYNC_ERROR,
		    FREETUXTV_DBSYNC_ERROR_EXEC_QUERY,
		    _("Error when migrating the database.\n\nSQLite has returned error:\n%s."),
		    sqlite3_errmsg(dbsync->db_link));
		sqlite3_free(db_err);
	}else{
		bRes = TRUE;
	}

	return bRes;
}

void
dbsync_update_recording (DBSync *dbsync,
    FreetuxTVRecordingInfos* pRecordingInfos,
    GError** error)
{
	g_return_if_fail(dbsync != NULL);
	g_return_if_fail(dbsync->db_link != NULL);
	g_return_if_fail(error != NULL);
	g_return_if_fail(*error == NULL);
	g_return_if_fail(pRecordingInfos != NULL);
	g_return_if_fail(FREETUXTV_IS_RECORDING_INFOS(pRecordingInfos));

	gchar *query;
	gchar *db_err = NULL;
	int res;

	// Update the group
	query = sqlite3_mprintf("\
	    UPDATE %s SET %s=%d, %s=%Q\
	    WHERE %s=%d",
	    // UPDATE
	    DB_RECORDING,
	    // SET
	    DB_RECORDING_STATUS, pRecordingInfos->status,
	    DB_RECORDING_FILENAME, pRecordingInfos->szFileName,
	    // WHERE
	    DB_RECORDING_ID, pRecordingInfos->id);
	res = sqlite3_exec(dbsync->db_link, query, NULL, NULL, &db_err);
	sqlite3_free(query);

	if(res != SQLITE_OK){
		*error = g_error_new (FREETUXTV_DBSYNC_ERROR,
		    FREETUXTV_DBSYNC_ERROR_EXEC_QUERY,
		    _("Error when updating the status of the recording \"%s\".\n\nSQLite has returned error:\n%s."),
		    pRecordingInfos->szTitle, sqlite3_errmsg(dbsync->db_link));
		sqlite3_free(db_err);
	}
}

void
dbsync_delete_recording (DBSync *dbsync,
    FreetuxTVRecordingInfos* pRecordingInfos,
    GError** error)
{
	g_return_if_fail(dbsync != NULL);
	g_return_if_fail(dbsync->db_link != NULL);
	g_return_if_fail(error != NULL);
	g_return_if_fail(*error == NULL);
	g_return_if_fail(pRecordingInfos != NULL);
	g_return_if_fail(FREETUXTV_IS_RECORDING_INFOS(pRecordingInfos));

	gchar *query;
	gchar *db_err = NULL;
	int res;

	// Delete the channels group
	query = sqlite3_mprintf("DELETE FROM %s WHERE %s=%d",
	    // DELETE FROM
	    DB_RECORDING,
	    // WHERE
	    DB_RECORDING_ID, pRecordingInfos->id);
	res = sqlite3_exec(dbsync->db_link, query, NULL, NULL, &db_err);
	sqlite3_free(query);

	if(res != SQLITE_OK){
		*error = g_error_new (FREETUXTV_DBSYNC_ERROR,
		    FREETUXTV_DBSYNC_ERROR_EXEC_QUERY,
		    _("Error when deleting the recording \"%s\".\n\nSQLite has returned error:\n%s."),
		    pRecordingInfos->szTitle,
		    sqlite3_errmsg(dbsync->db_link));
		sqlite3_free(db_err);
	}
}