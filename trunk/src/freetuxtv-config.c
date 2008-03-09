/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4-*- */
/*
 * freetuxtv
 * Copyright (C) FreetuxTV Team's 2008
 * Project homepage : http://code.google.com/p/freetuxtv/
 * 
 * freetuxtv is free software.
 * 
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "freetuxtv-config.h"

gchar *FREETUXTV_DIR;
gchar *FREETUXTV_CONFIG_DIR;
gchar *FREETUXTV_IMG_DIR;
gchar *FREETUXTV_IMG_CHANNELS_DIR;

gchar *FREETUXTV_USER_DIR;
gchar *FREETUXTV_USER_DB;
gchar *FREETUXTV_USER_IMG_DIR;
gchar *FREETUXTV_USER_IMG_CHANNELS_DIR;

void
freetuxtv_config_init ()
{
	FREETUXTV_DIR = g_strconcat(".", NULL);
	FREETUXTV_CONFIG_DIR = g_strconcat(FREETUXTV_DIR, "/config", NULL);
	FREETUXTV_IMG_DIR = g_strconcat(FREETUXTV_DIR, "/images", NULL);
	FREETUXTV_IMG_CHANNELS_DIR = g_strconcat(FREETUXTV_IMG_DIR, "/channels", NULL);

	FREETUXTV_USER_DIR = g_strconcat(getenv("HOME"), "/.freetuxtv", NULL);
	FREETUXTV_USER_DB = g_strconcat(FREETUXTV_USER_DIR, "/freetuxtv.db", NULL);
	FREETUXTV_USER_IMG_DIR = g_strconcat(FREETUXTV_USER_DIR, "/images", NULL);
	FREETUXTV_USER_IMG_CHANNELS_DIR = g_strconcat(FREETUXTV_USER_IMG_DIR, "/channels", NULL);
}

void
freetuxtv_config_free ()
{
	g_free (FREETUXTV_IMG_DIR);
	g_free (FREETUXTV_IMG_CHANNELS_DIR);

	g_free (FREETUXTV_USER_DIR );
	g_free (FREETUXTV_USER_DB);
	g_free (FREETUXTV_USER_IMG_DIR);
	g_free (FREETUXTV_USER_IMG_CHANNELS_DIR);
}
