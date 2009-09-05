/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8-*- */
/*
 * freetuxtv
 * Copyright (C) FreetuxTV Team's 2008
 * Project homepage : http://code.google.com/p/freetuxtv/
 * 
 * freetuxtv is free software.
 * 
 */

#ifndef FREETUXTV_APP_H
#define FREETUXTV_APP_H

#include <gtk/gtk.h>
#include <libnotify/notification.h>

#include "freetuxtv-channel-infos.h"
#include "gtk-libvlc-media-player.h"

G_BEGIN_DECLS

#define FREETUXTV_GLADEXML FREETUXTV_DIR "/freetuxtv.glade"

struct _FreetuxTVApp {
	
	gchar *name;

	GtkBuilder* gui;

	GtkLibVLCMediaPlayer *player;
	GtkTreeModel *channelslist;

	struct {
		gdouble volume;
		gboolean channelonstartup;
		gint lastchannel;
		gboolean windowminimode_stayontop;
		gint windowminimode_width;
		gint windowminimode_height;
		gint logosfiledate;
		gchar* directoryrecordings;
	} config;

	struct {
		GtkTreePath *path_channel;
		gboolean lastchannelonstartup;
		NotifyNotification *notification;
		
		struct {
			gchar* dst_file;
			GTimer *duration;
		} recording;

	} current;

	gboolean debug;	

};
typedef struct _FreetuxTVApp FreetuxTVApp;

void
freetuxtv_play_channel (FreetuxTVApp *app, GtkTreePath* path_channel);

void
freetuxtv_play_media (FreetuxTVApp *app, GtkLibVLCMedia* media);

void
freetuxtv_action_playpause (FreetuxTVApp *app);

void
freetuxtv_action_stop (FreetuxTVApp *app);

void
freetuxtv_action_record (FreetuxTVApp *app);

void
freetuxtv_action_prev (FreetuxTVApp *app);

void
freetuxtv_action_next (FreetuxTVApp *app);

void
freetuxtv_quit (FreetuxTVApp *app);

G_END_DECLS

#endif /* FREETUXTV_APP_H */
