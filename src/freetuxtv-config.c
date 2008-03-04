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

#include <stdlib.h>
#include <string.h>

#include <stdio.h>

#include "freetuxtv-config.h"

gchar *FREETUXTV_IMG_DIR;
gchar *FREETUXTV_IMG_CHANNELS_DIR;

gchar *FREETUXTV_USER_DIR;
gchar *FREETUXTV_USER_DB;
gchar *FREETUXTV_USER_IMG_DIR;
gchar *FREETUXTV_USER_IMG_CHANNELS_DIR;

void
freetuxtv_config_init ()
{
	FREETUXTV_IMG_DIR = "./images";
	FREETUXTV_IMG_CHANNELS_DIR = g_strconcat(FREETUXTV_IMG_DIR, "/channels", NULL);

	FREETUXTV_USER_DIR = g_strconcat(getenv("HOME"), "/.freetuxtv", NULL);
	FREETUXTV_USER_DB = g_strconcat(FREETUXTV_USER_DIR, "/freetuxtv.db", NULL);
	FREETUXTV_USER_IMG_DIR = g_strconcat(FREETUXTV_USER_DIR, "/images", NULL);
	FREETUXTV_USER_IMG_CHANNELS_DIR = g_strconcat(FREETUXTV_USER_IMG_DIR, "/channels", NULL);
}
