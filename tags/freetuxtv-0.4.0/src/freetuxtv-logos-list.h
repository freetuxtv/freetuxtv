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

#ifndef FREETUXTV_LOGOS_LIST_H
#define FREETUXTV_LOGOS_LIST_H

#include "freetuxtv-app.h"
#include "freetuxtv-db-sync.h"

void
logos_list_synchronize (FreetuxTVApp *app, DBSync *dbsync,
			GError** error);

gchar*
logos_list_get_channel_logo_filename(FreetuxTVApp *app, 
				     FreetuxTVChannelInfos* channel_infos,
				     gboolean none_icon);

#endif /* FREETUXTV_LOGOS_LIST_H */
