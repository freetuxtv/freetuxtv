/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4-*- */
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

#ifndef FREETUXTV_UTILS_H
#define FREETUXTV_UTILS_H

#include <gtk/gtk.h>

#include "freetuxtv-app.h"

gchar*
format_time(gint seconds);

gchar*
format_time2(gint seconds);

gchar*
format_size(glong size);

gchar*
get_recording_options(FreetuxTVApp *app, gchar* szBaseFilename, gboolean from_preview, gchar **pszOutFilename);

gint64
g_time_val_to_int64 (GTimeVal *timeval);

gchar*
g_time_int64_to_string(gint64 time, const gchar* format);

gint64
g_string_to_time_int64(const gchar* strtime, const gchar* format);

void
g_time_int64_add_seconds (gint64 *time, glong seconds);

void
g_time_val_add_seconds (GTimeVal *timeval, glong seconds);

gint
g_time_val_compare (GTimeVal *timeval1, GTimeVal *timeval2);

void
g_time_val_copy (GTimeVal *timeval_src, GTimeVal *timeval_dest);

gchar*
g_time_val_to_string(GTimeVal *timeval, const gchar* format);

gchar*
g_utf8_removediacritics(const gchar *str, gssize len);

void
g_print_datetime (GDateTime *datetime);


#endif /* FREETUXTV_UTILS_H */
