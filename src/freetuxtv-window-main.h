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

#ifndef FREETUXTV_WINDOW_MAIN_H
#define FREETUXTV_WINDOW_MAIN_H

#include "freetuxtv-app.h"

typedef enum {
	WINDOW_MODE_STOPPED,
	WINDOW_MODE_PLAYING,
	WINDOW_MODE_PAUSED,
	WINDOW_MODE_RECORDING
} FreetuxTVWindowMode;

void
windowmain_init(FreetuxTVApp *app);

void
windowmain_display_buttons (FreetuxTVApp *app, FreetuxTVWindowMode mode);

void
windowmain_show_error (FreetuxTVApp *app, gchar *msg);

void
windowmain_show_gerror (FreetuxTVApp *app, GError *error);

void
windowmain_statusbar_push (FreetuxTVApp *app, gchar *context, gchar *msg);

void
windowmain_statusbar_pop (FreetuxTVApp *app, gchar *context);

void
windowmain_update_statusbar_infos (FreetuxTVApp *app);

void
windowmain_timebar_update (FreetuxTVApp *app, glong time_ms, glong length_ms, gfloat position);

#endif /* FREETUXTV_WINDOW_MAIN_H */
