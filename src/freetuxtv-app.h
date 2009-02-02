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
#include <glade/glade.h>

#include "freetuxtv-player.h"
#include "freetuxtv-channel.h"

G_BEGIN_DECLS

#define FREETUXTV_GLADEXML FREETUXTV_DIR "/freetuxtv.glade"

struct _FreetuxTVApp {
	
	gchar *name;

	GladeXML *windowmain;
	GladeXML *dialogaddgroup;
	GladeXML *windowminimode;
	GladeXML *dialogpreferences;

	FreetuxTVPlayer *player;
	GtkVBox *channelsgroups;

	struct {
		gdouble volume;
		gboolean channelonstartup;
		gchar *lastchannel;
		gboolean windowminimode_stayontop;
		gint windowminimode_width;
		gint windowminimode_height;
	} config;

	struct {
		FreetuxTVChannel *channel;
		gboolean lastchannelonstartup;
	} current;

};
typedef struct _FreetuxTVApp FreetuxTVApp;

void
freetuxtv_action_play_channel (FreetuxTVApp *app);

void
freetuxtv_action_stop_channel (FreetuxTVApp *app);

void
freetuxtv_action_record_channel (FreetuxTVApp *app);

void
freetuxtv_action_prev_channel (FreetuxTVApp *app);

void
freetuxtv_action_next_channel (FreetuxTVApp *app);

void
freetuxtv_action_quit (FreetuxTVApp *app);

G_END_DECLS

#endif /* FREETUXTV_APP_H */
