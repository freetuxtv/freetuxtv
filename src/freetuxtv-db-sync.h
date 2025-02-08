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

#ifndef FREETUXTV_DB_SYNC_H
#define FREETUXTV_DB_SYNC_H

#include <glib.h>
#include <sqlite3.h>

#include "freetuxtv-app.h"
#include "freetuxtv-i18n.h"
#include "freetuxtv-channel-infos.h"
#include "freetuxtv-channels-group-infos.h"
#include "freetuxtv-tv-channel-infos.h"
#include "freetuxtv-recording-infos.h"

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

// Database manipulation function

void
dbsync_open_db (DBSync *dbsync, GError** error);

void
dbsync_close_db (DBSync *dbsync);

gboolean
dbsync_db_exists (DBSync *dbsync);

void
dbsync_create_db (DBSync *dbsync, gchar* szScriptFilename, GError** error);

void
dbsync_update_db (DBSync *dbsync, gchar* szScriptFilename, GError** error);

// Channels groups manipulation function


#endif /* FREETUXTV_DB_SYNC_H */
