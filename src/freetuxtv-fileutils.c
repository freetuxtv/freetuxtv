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

gchar*
gproxystruct_to_string(const GProxyStruct* pProxySctruct, gboolean protocol, gboolean server, gboolean auth)
{
	gchar* szRes = NULL;
	
	gchar* szProtocole = NULL;
	gchar* szServer = NULL;
	gchar* szAuth = NULL;
	
	if(protocol){
		szProtocole = g_strdup_printf("%s://", pProxySctruct->proxy_type);
	}
	
	if(server){
		szServer = g_strdup_printf("%s:%s", pProxySctruct->proxy_server, pProxySctruct->proxy_port);
	}
	
	if(auth){
		szAuth = g_strdup_printf("%s:%s", pProxySctruct->proxy_username, pProxySctruct->proxy_password);
	}

	if(szProtocole || szServer || szAuth){
		if(szServer && szAuth){
			szRes = g_strdup_printf("%s%s@%s",
			    (szProtocole ? szProtocole : ""), (szAuth ? szAuth : ""),
			    (szServer ? szServer : ""));
		}else{
			szRes = g_strdup_printf("%s%s%s",
			    (szProtocole ? szProtocole : ""), (szAuth ? szAuth : ""),
			    (szServer ? szServer : ""));
		}
	}

	if(szProtocole){
		g_free(szProtocole);
		szProtocole = NULL;
	}

	if(szServer){
		g_free(szServer);
		szServer = NULL;
	}

	if(szAuth){
		g_free(szAuth);
		szAuth = NULL;
	}

	return szRes;
}
