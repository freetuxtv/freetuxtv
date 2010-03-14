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

#ifndef FREETUXTV_DB_SYNC_H
#define FREETUXTV_DB_SYNC_H

#include <glib.h>
#include <sqlite3.h>

#include "freetuxtv-app.h"
#include "freetuxtv-i18n.h"
#include "freetuxtv-channel-infos.h"
#include "freetuxtv-channels-group-infos.h"

typedef struct _DBSync DBSync;
struct _DBSync
{
	struct sqlite3 *db_link;
};

#define FREETUXTV_DBSYNC_ERROR freetuxtv_dbsync_error_quark ()
typedef enum
{
	FREETUXTV_DBSYNC_ERROR_OPEN,
	FREETUXTV_DBSYNC_ERROR_EXEC_QUERY
} FreetuxTVDBSyncError;

GQuark
freetuxtv_dbsync_error_quark ();

void
dbsync_open_db (DBSync *dbsync, GError** error);

void
dbsync_close_db (DBSync *dbsync);

gboolean
dbsync_db_exists(DBSync *dbsync);

void
dbsync_create_db (DBSync *dbsync, GError** error);

void
dbsync_select_channels_groups (DBSync *dbsync,
			       FreetuxTVApp *app,
			       int (*callback)(FreetuxTVApp *app, FreetuxTVChannelsGroupInfos* channels_group_infos,
					       DBSync *dbsync, gpointer user_data, GError** error),
			       gpointer user_data, GError** error);

void
dbsync_select_channels_of_channels_group (DBSync *dbsync,
					  FreetuxTVChannelsGroupInfos* channels_group_infos,
					  FreetuxTVApp *app,
					  int (*callback)(FreetuxTVApp *app, 
							  FreetuxTVChannelInfos* channel_infos,
							  DBSync *dbsync, gpointer user_data, GError** error),
					  gpointer user_data, GError** error);

void
dbsync_add_channel (DBSync *dbsync,
		    FreetuxTVChannelInfos* channel_infos,
		    GError** error);

void
dbsync_add_channels_group (DBSync *dbsync,
			   FreetuxTVChannelsGroupInfos* channels_group_infos,
			   GError** error);

void
dbsync_update_channels_group (DBSync *dbsync,
			      FreetuxTVChannelsGroupInfos* channels_group_infos,
			      GError** error);

void
dbsync_delete_channels_group (DBSync *dbsync,
			      FreetuxTVChannelsGroupInfos* channels_group_infos,
			      GError** error);

void
dbsync_delete_channels_of_channels_group (DBSync *dbsync,
					  FreetuxTVChannelsGroupInfos* channels_group_infos,
					  GError** error);

void
dbsync_delete_channels_logos (DBSync *dbsync, GError** error);

void
dbsync_add_channel_logo (DBSync *dbsync, gchar* label, gchar* filename, 
			 glong *id, GError** error);

void
dbsync_add_label_channel_logo (DBSync *dbsync, gchar* label, glong id_logo, 
			       GError** error);

#endif /* FREETUXTV_DB_SYNC_H */
