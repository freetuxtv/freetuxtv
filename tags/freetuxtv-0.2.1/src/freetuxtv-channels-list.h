/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8-*- */
/*
 * freetuxtv
 * Copyright (C) FreetuxTV Team's 2008
 * Project homepage : http://code.google.com/p/freetuxtv/
 * 
 * freetuxtv is free software.
 * 
 */

#ifndef FREETUXTV_CHANNELS_LIST_H
#define FREETUXTV_CHANNELS_LIST_H

#include <glib-object.h>
#include <gtk/gtk.h>

#include "freetuxtv-app.h"
#include "freetuxtv-channels-group.h"
#include "freetuxtv-channel.h"

void
channels_list_add_channels_group (FreetuxTVApp *app,
				  FreetuxTVChannelsGroup *channels_group);

void
channels_list_refresh_group (FreetuxTVApp *app,
			     FreetuxTVChannelsGroup *channels_group);

void
channels_list_apply_filter (FreetuxTVApp *app);

void
channels_list_update_from_db (FreetuxTVApp *app);

void
on_channel_dbl_clicked (FreetuxTVChannel *channel, gpointer *data);

#endif /* FREETUXTV_CHANNELS_LIST_H */
