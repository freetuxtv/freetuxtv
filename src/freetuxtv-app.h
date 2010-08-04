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
#define FREETUXTV_GLADEXML			FREETUXTV_UI_DIR "/freetuxtv.glade"

#define FREETUXTV_LOG_DOMAIN			"FreetuxTV"

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

		// Prefs network
		gint proxy_mode;
		gchar* proxy_server;
		gchar* proxy_port;
		gchar* proxy_type;
		gboolean proxy_use_auth;
		gchar* proxy_username;
		gchar* proxy_password;
		
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
		gchar* open_channel_name;
		gint open_channel_id;
		GtkTreePath *path_channel;
		gboolean autoplay_channel;
		NotifyNotification *notification;
		
		gboolean is_recording;
		struct {
			gchar* dst_file;
			GTimeVal time_begin;
			gint max_duration;
			GTimer *duration;
		} recording;

	} current;

	struct {
		GtkWidget* pMenuDeinterlaceDisable;
		GtkWidget* pMenuDeinterlaceBlend;
		GtkWidget* pMenuDeinterlaceBob;
		GtkWidget* pMenuDeinterlaceDiscard;
		GtkWidget* pMenuDeinterlaceLinear;
		GtkWidget* pMenuDeinterlaceMean;
		GtkWidget* pMenuDeinterlaceX;
	} widget;
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
freetuxtv_action_deinterlace (FreetuxTVApp *app, const gchar* mode,
                              GError** error);

void
freetuxtv_quit (FreetuxTVApp *app);

void
freetuxtv_log (const gchar *log_domain, GLogLevelFlags log_level,
               const gchar *message, gpointer user_data);

G_END_DECLS

#endif /* FREETUXTV_APP_H */
