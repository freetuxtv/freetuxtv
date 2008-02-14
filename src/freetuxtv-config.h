/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8-*- */
/*
 * freetuxtv
 * Copyright (C) FreetuxTV Team's 2008
 * Project homepage : http://code.google.com/p/freetuxtv/
 * 
 * freetuxtv is free software.
 * 
 * You may redistribute it and/or modify it under the terms of the
 * GNU General Public License, as published by the Free Software
 * Foundation; either version 2 of the License, or (at your option)
 * any later version.
 * 
 * freetuxtv is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with brasero.  If not, write to:
 * 	The Free Software Foundation, Inc.,
 * 	51 Franklin Street, Fifth Floor
 * 	Boston, MA  02110-1301, USA.
 */

#ifndef FREETUXTV_CONFIG_H
#define FREETUXTV_CONFIG_H

#include <glib.h>

extern gchar *FREETUXTV_USER_DIR;
extern gchar *FREETUXTV_SQLITE_DB;

#define FREETUXTV_SQL_CREATE_TABLES "\
        CREATE TABLE IF NOT EXISTS channels_group ( \n\
           id_channelsgroup INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,	\n\
           name_channelsgroup VARCHAR(30) NOT NULL, \n\
           logo_channelsgroup VARCHAR(20), \n\
           uri_channelsgroup VARCHAR(255) NOT NULL \n\
        ); \n\
        CREATE TABLE IF NOT EXISTS channel ( \n\
           id_channel INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, \n\
           name_channel VARCHAR(30) NOT NULL, \n\
           logo_channel VARCHAR(20), \n\
           uri_channel VARCHAR(255) NOT NULL, \n\
           channelsgroup_channel INTEGER NOT NULL, \n\
           FOREING KEY channelsgroup_channel REFERENCES channels_group(id_channelsgroup) \n\
        );"

void
freetuxtv_config_init();

#endif /* FREETUXTV_CONFIG_H */
