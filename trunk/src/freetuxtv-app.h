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

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#ifndef FREETUXTV_APP_H
#define FREETUXTV_APP_H

#include <gtk/gtk.h>
#include <libnotify/notification.h>

#include <libvlc-gtk/gtk-libvlc-media-player.h>

#include "freetuxtv-channel-infos.h"

G_BEGIN_DECLS

/* For testing propose use the local (not installed) ui file */
#define FREETUXTV_DIR      PACKAGE_DATA_DIR "/" PACKAGE
//#define FREETUXTV_DIR     "data"

#define FREETUXTV_UI_DIR			FREETUXTV_DIR "/ui"
#define FREETUXTV_GLADEXML			FREETUXTV_UI_DIR "/freetuxtv.ui"

struct _FreetuxTVApp {
	
	gchar *name;

	GtkBuilder* gui;

	GtkLibvlcMediaPlayer *player;
	GtkTreeModel *channelslist;

	struct {		
		// Prefs general
		gboolean channelonstartup;
		gboolean enable_notifications;
		
		// Prefs recordings
		gchar* directoryrecordings;
		gint transcoding_mode;
		gchar* transcoding_format;
		
	} prefs;

	struct {
		gdouble volume;

		gint lastchannel;
		gboolean windowminimode_stayontop;
		gint windowminimode_width;
		gint windowminimode_height;
		gint logosfiledate;		
	} config;

	struct {
		GtkTreePath *path_channel;
		gboolean lastchannelonstartup;
		NotifyNotification *notification;
		
		gboolean is_recording;
		struct {
			gchar* dst_file;
			GTimeVal time_begin;
			gint max_duration;
			GTimer *duration;
		} recording;

	} current;
	
	gboolean debug;	

};
typedef struct _FreetuxTVApp FreetuxTVApp;

void
freetuxtv_play_channel (FreetuxTVApp *app, GtkTreePath* path_channel, GError** error);

void
freetuxtv_play_media (FreetuxTVApp *app, GtkLibvlcMedia* media, GError** error);

void
freetuxtv_action_playpause (FreetuxTVApp *app, GError** error);

void
freetuxtv_action_stop (FreetuxTVApp *app, GError** error);

void
freetuxtv_action_record (FreetuxTVApp *app, GError** error);

void
freetuxtv_action_prev (FreetuxTVApp *app, GError** error);

void
freetuxtv_action_next (FreetuxTVApp *app, GError** error);

void
freetuxtv_quit (FreetuxTVApp *app);

G_END_DECLS

#endif /* FREETUXTV_APP_H */
