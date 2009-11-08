/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8-*- */
/*
 * freetuxtv
 * Copyright (C) FreetuxTV Team's 2008
 * Project homepage : http://code.google.com/p/freetuxtv/
 * 
 * freetuxtv is free software.
 * 
 */

#include <glib.h>
#include <gio/gio.h>
#include <curl/curl.h>

#include "freetuxtv-fileutils.h"

#include "freetuxtv-i18n.h"

GQuark freetuxtv_curl_error = 0;

GQuark
freetuxtv_curl_error_quark () {
	if (freetuxtv_curl_error == 0){
		freetuxtv_curl_error = g_quark_from_string("FREETUXTV_CURL_ERROR");
	}
	return freetuxtv_curl_error;
}

void
freetuxtv_fileutils_get_file (gchar* url, gchar* dst_file, GError **error)
{
	g_return_if_fail(url != NULL);
	g_return_if_fail(dst_file != NULL);
	g_return_if_fail(error != NULL);
	g_return_if_fail(*error == NULL);
	
	gchar **uriv;

	uriv = g_strsplit (url, "//", 2);
	
	if( g_ascii_strcasecmp (uriv[0], "http:") == 0 || g_ascii_strcasecmp (uriv[0], "https:") == 0){
		// Download the file
		CURL *session = curl_easy_init(); 
		curl_easy_setopt(session, CURLOPT_URL, url);
		curl_easy_setopt(session, CURLOPT_TIMEOUT, 10);
		// curl_easy_setopt(session, CURLOPT_VERBOSE, 1);
		curl_easy_setopt(session, CURLOPT_FOLLOWLOCATION, 1);
		FILE * fp = fopen(dst_file, "w"); 
		curl_easy_setopt(session,
				 CURLOPT_WRITEDATA, fp); 
		curl_easy_setopt(session,
				 CURLOPT_WRITEFUNCTION, fwrite);
		CURLcode curlcode;
		curlcode = curl_easy_perform(session);
		fclose(fp);
		curl_easy_cleanup(session);
		
		if(curlcode != 0){
			*error = g_error_new (FREETUXTV_CURL_ERROR,
					      FREETUXTV_CURL_ERROR_GET,
					      _("Error when downloading the file from URL -> %s.\n\nCURL has returned error :\n%s."),
					      url, curl_easy_strerror(curlcode));
		}
		
	}else{
		GFile *source;
		GFile *destination;

		source = g_file_new_for_path (url);
		destination = g_file_new_for_path (dst_file);

		g_file_copy (source, destination, G_FILE_COPY_OVERWRITE,
			     NULL, NULL, NULL, error);
		g_object_unref(source);
		g_object_unref(destination);
	}
	

	g_strfreev (uriv);

}
