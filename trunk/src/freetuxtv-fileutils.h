/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8-*- */
/*
 * FreetuxTV is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or 
 * (at your option) any later version.
 *
 * FreetuxTV is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Glade; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
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

typedef enum _GProxyMode {
	G_PROXY_MODE_NONE       = 0,
	G_PROXY_MODE_MANUAL     = 1
} GProxyMode;

typedef struct _GProxyStruct {
	GProxyMode proxy_mode;
	gchar* proxy_server;
	gchar* proxy_port;
	gchar* proxy_type;
	gboolean proxy_use_auth;
	gchar* proxy_username;
	gchar* proxy_password;
} GProxyStruct;

void
freetuxtv_fileutils_get_file (const gchar* url, const gchar* dst_file, const GProxyStruct* pProxySctruct, int timeout, GError **error);

gchar*
gproxystruct_to_string(const GProxyStruct* pProxySctruct, gboolean protocol, gboolean server, gboolean auth);


#endif /* FREETUXTV_FILEUTILS_H */
