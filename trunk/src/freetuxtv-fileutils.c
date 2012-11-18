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

void
freetuxtv_fileutils_get_file (const gchar* url, const gchar* dst_file, const GProxyStruct* pProxySctruct, int timeout, GError **error)
{
	g_return_if_fail(url != NULL);
	g_return_if_fail(dst_file != NULL);
	g_return_if_fail(error != NULL);
	g_return_if_fail(*error == NULL);
	
	gchar **uriv;

	gchar* szProxyServeur = NULL;
	gchar* szProxyAuth = NULL;

	uriv = g_strsplit (url, "//", 2);
	
	if( g_ascii_strcasecmp (uriv[0], "http:") == 0 || g_ascii_strcasecmp (uriv[0], "https:") == 0){
		// Download the file
		CURL *session = curl_easy_init(); 
		curl_easy_setopt(session, CURLOPT_URL, url);
		if(timeout > 0){
			curl_easy_setopt(session, CURLOPT_TIMEOUT, timeout);
		}
		// curl_easy_setopt(session, CURLOPT_VERBOSE, 1);
		curl_easy_setopt(session, CURLOPT_FOLLOWLOCATION, 1);

		if(pProxySctruct){
			if(pProxySctruct->proxy_mode == G_PROXY_MODE_MANUAL){
				
				szProxyServeur = gproxystruct_to_string(pProxySctruct, FALSE, TRUE, FALSE);
				curl_easy_setopt(session, CURLOPT_PROXY, szProxyServeur);
				if(pProxySctruct->proxy_use_auth){
					szProxyAuth = gproxystruct_to_string(pProxySctruct, FALSE, FALSE, TRUE);
					curl_easy_setopt(session, CURLOPT_PROXYUSERPWD, szProxyAuth);
				}
			}
		}
		
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
					      _("Error when downloading the file from URL -> %s.\n\nCURL has returned error:\n%s."),
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

	if(szProxyServeur){
		g_free(szProxyServeur);
		szProxyServeur = NULL;
	}

	if(szProxyAuth){
		g_free(szProxyAuth);
		szProxyAuth = NULL;
	}

	g_strfreev (uriv);

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
