/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8-*- */
/*
 * freetuxtv
 * Copyright (C) FreetuxTV Team's 2008
 * Project homepage : http://code.google.com/p/freetuxtv/
 * 
 * freetuxtv is free software.
 * 
 */

#ifndef FREETUXTV_FILEUTILS_H
#define FREETUXTV_FILEUTILS_H

#include <glib.h>

#define FREETUXTV_CURL_ERROR freetuxtv_curl_error_quark ()
typedef enum
{
	FREETUXTV_CURL_ERROR_GET,
} FreetuxTVCurlError;

GQuark
freetuxtv_curl_error_quark ();

void
freetuxtv_fileutils_get_file (gchar* url, gchar* dst_file, GError **error);

#endif /* FREETUXTV_FILEUTILS_H */
