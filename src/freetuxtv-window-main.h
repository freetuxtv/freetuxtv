/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8-*- */
/*
 * freetuxtv
 * Copyright (C) FreetuxTV Team's 2008
 * Project homepage : http://code.google.com/p/freetuxtv/
 * 
 * freetuxtv is free software.
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


enum {
	FREETUXTV_RESPONSE_ADD = 1
} FreetuxTVResponseDialog;

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
windowmain_timebar_update (FreetuxTVApp *app, glong time_ms, glong length_ms, gfloat position);

#endif /* FREETUXTV_WINDOW_MAIN_H */
