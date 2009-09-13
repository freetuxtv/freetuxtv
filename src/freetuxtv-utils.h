/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8-*- */
/*
 * freetuxtv
 * Copyright (C) FreetuxTV Team's 2008
 * Project homepage : http://code.google.com/p/freetuxtv/
 * 
 * freetuxtv is free software.
 * 
 */

#ifndef FREETUXTV_UTILS_H
#define FREETUXTV_UTILS_H

#include <gtk/gtk.h>

#include "freetuxtv-app.h"

gchar*
format_time(gint seconds);

gchar*
format_time2(gint seconds);

gchar*
format_size(glong size);

gchar*
get_recording_options(FreetuxTVApp *app, gchar* base_filename, gboolean from_preview, gchar **out_filename);


#endif /* FREETUXTV_UTILS_H */
