/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8-*- */
/*
 * freetuxtv
 * Copyright (C) FreetuxTV Team's 2008
 * Project homepage : http://code.google.com/p/freetuxtv/
 * 
 * freetuxtv is free software.
 * 
 */

#ifndef FREETUXTV_LOGOS_LIST_H
#define FREETUXTV_LOGOS_LIST_H

#include "freetuxtv-app.h"

gint
logos_list_synchronize (FreetuxTVApp *app);

gchar*
logos_list_get_channel_logo_filename(FreetuxTVApp *app, 
				     FreetuxTVChannelInfos* channel_infos,
				     gboolean none_icon);

#endif /* FREETUXTV_LOGOS_LIST_H */
