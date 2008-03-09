/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4-*- */
/*
 * freetuxtv
 * Copyright (C) FreetuxTV Team's 2008
 * Project homepage : http://code.google.com/p/freetuxtv/
 * 
 * freetuxtv is free software.
 * 
 */

#ifndef FREETUXTV_CONFIG_H
#define FREETUXTV_CONFIG_H

#include <glib.h>

extern gchar *FREETUXTV_DIR;
extern gchar *FREETUXTV_CONFIG_DIR;
extern gchar *FREETUXTV_IMG_DIR;
extern gchar *FREETUXTV_IMG_CHANNELS_DIR;

extern gchar *FREETUXTV_USER_DIR;
extern gchar *FREETUXTV_USER_DB;
extern gchar *FREETUXTV_USER_IMG_DIR;
extern gchar *FREETUXTV_USER_IMG_CHANNELS_DIR;

void
freetuxtv_config_init();

void
freetuxtv_config_free();

#endif /* FREETUXTV_CONFIG_H */
