/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * freetuxtv
 * Copyright (C) Eric Beuque 2010 <eric.beuque@gmail.com>
 * 
 * freetuxtv is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * freetuxtv is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <gtk/gtk.h>

#include "freetuxtv-tv-channels-list.h"

#include "freetuxtv-app.h"
#include "freetuxtv-i18n.h"
#include "freetuxtv-window-main.h"
#include "freetuxtv-tv-channel-infos.h"
#include "freetuxtv-channels-list.h"

typedef struct _CBXMLData
{
	FreetuxTVApp *app;
	DBSync *dbsync;
	FreetuxTVTvChannelInfos *tv_channels_infos;
	SynchronizeProgressCB progressCB;
	void* progressCBUserData;
} CBXMLData;

static void 
xml_start_cb(GMarkupParseContext *context,
    const gchar *element_name,
    const gchar **attribute_names,
    const gchar **attribute_values,
    gpointer data,
    GError **error);

static void 
xml_end_cb(GMarkupParseContext *context,
    const gchar *element_name,
    gpointer user_data,
    GError **error);

static void 
xml_text_cb(GMarkupParseContext *context,
    const gchar *text,
    gsize text_len,  
    gpointer user_data,
    GError **error);

void
tvchannels_list_synchronize (FreetuxTVApp *app, DBSync *dbsync,
    SynchronizeProgressCB funcCB, void* user_data, GError** error)
{
	g_return_if_fail(dbsync != NULL);
	g_return_if_fail(error != NULL);

	// Update the status bar to the the process
	gchar *text;
	text = g_strdup_printf(_("Synchronizing the tv channels list"));
	windowmain_statusbar_push (app, "UpdateMsg", text);
	g_free(text);
	g_log(FREETUXTV_LOG_DOMAIN, G_LOG_LEVEL_MESSAGE,
	      "Synchronizing the tv channels list\n");

	// Delete the TV channel in the database
	dbsync_delete_tvchannels (dbsync, error);

	if(*error == NULL){
		// Add the list of TV channel in the data base for the XML file
		CBXMLData cbxmldata;
		cbxmldata.app = app;
		cbxmldata.dbsync = dbsync;
		cbxmldata.tv_channels_infos = NULL;
		cbxmldata.progressCB = funcCB;
		cbxmldata.progressCBUserData = user_data;

		gsize filelen;
		static GMarkupParser parser = { xml_start_cb, xml_end_cb, xml_text_cb, 
			NULL, NULL };
		GMarkupParseContext *context;

		context = g_markup_parse_context_new (&parser, 
		    G_MARKUP_DO_NOT_USE_THIS_UNSUPPORTED_FLAG,
		    &cbxmldata, NULL);
		gchar *xml_data;
		gchar *filename;
		filename = g_build_filename(app->paths.datadir, "tv_channels.xml", NULL);
		g_file_get_contents (filename, &xml_data, &filelen, NULL);
		g_free(filename);
		filename = NULL;
		g_markup_parse_context_parse (context, xml_data, -1, error);
	}

	windowmain_statusbar_pop (app, "UpdateMsg");
}


static gchar*
tvchannels_list_get_tvchannel_logo_path(FreetuxTVApp *app, 
    gchar* szLogoName, gboolean bNoneIcon)
{
	gchar *szImgFile = NULL;
	gchar *szUserImgChannelsDir;
	gboolean bFound = FALSE;
	
	szUserImgChannelsDir = g_build_filename(g_get_user_data_dir(), 
	    "freetuxtv", "images", "channels", NULL);
	
	if(szLogoName != NULL){
		// We look in the user logo directory
		szImgFile = g_build_filename(szUserImgChannelsDir, szLogoName, NULL);
		if(g_file_test(szImgFile, G_FILE_TEST_EXISTS)){
			bFound = TRUE;
		}else{
			g_free(szImgFile);
			szImgFile = NULL;
		}
		if(!bFound){
			szImgFile = g_build_filename(app->paths.datadir, "images", "channels", szLogoName, NULL);
			if(g_file_test(szImgFile, G_FILE_TEST_EXISTS)){
				bFound = TRUE;
			}else{
				g_free(szImgFile);
				szImgFile = NULL;
			}
		}
	}

	if(!bFound){
		if(bNoneIcon){
			szImgFile = g_build_filename(szUserImgChannelsDir, "_none.png", NULL);	
			if(g_file_test(szImgFile, G_FILE_TEST_EXISTS)){
				szImgFile = NULL;
			}else{
				g_free(szImgFile);
				szImgFile = g_build_filename(app->paths.datadir, "images", "channels", "_none.png", NULL);
			}
		}
	}

	g_free(szUserImgChannelsDir);
	szUserImgChannelsDir = NULL;

	return szImgFile;
}

gchar*
tvchannels_list_get_tvchannel_logo_path_for_channel(FreetuxTVApp *app, 
    FreetuxTVChannelInfos* pChannelInfos,
    gboolean bNoneIcon)
{
	gchar* szLogoName = NULL;
	if(pChannelInfos){
	    szLogoName = pChannelInfos->logo_name;
	}
	return tvchannels_list_get_tvchannel_logo_path(app, szLogoName, bNoneIcon);
}

gchar*
tvchannels_list_get_tvchannel_logo_path_for_recording(FreetuxTVApp *app, 
    FreetuxTVRecordingInfos* pRecordingInfos,
    gboolean bNoneIcon)
{
	FreetuxTVChannelInfos* pChannelInfos = NULL;

	pChannelInfos = channels_list_get_channel_by_id(app, pRecordingInfos->channel_id);
	
	return tvchannels_list_get_tvchannel_logo_path_for_channel(app, pChannelInfos, bNoneIcon);
}

static void 
xml_start_cb(GMarkupParseContext *context,
    const gchar *element_name,
    const gchar **attribute_names,
    const gchar **attribute_values,
    gpointer data,
    GError **error)
{
	CBXMLData* cbxmldata = (CBXMLData*)data;

	gchar *name = NULL;

	if(g_ascii_strcasecmp(element_name, "tvchannel") == 0){
		name = (gchar*)attribute_values[0];
		cbxmldata->progressCB(name, cbxmldata->progressCBUserData);
		cbxmldata->tv_channels_infos = freetuxtv_tv_channel_infos_new(name);
	}
	
	if(name != NULL){
		g_log(FREETUXTV_LOG_DOMAIN, G_LOG_LEVEL_DEBUG,
			  "Add TV channel '%s' in database\n", name);
	}	
}

static void 
xml_end_cb(GMarkupParseContext *context,
    const gchar *element_name,
    gpointer user_data,
    GError **error)
{
	CBXMLData* cbxmldata = (CBXMLData*)user_data;

	FreetuxTVTvChannelInfos *tv_channels_infos = NULL;

	if(g_ascii_strcasecmp(element_name, "tvchannel") == 0){
		tv_channels_infos = cbxmldata->tv_channels_infos;
		if(tv_channels_infos){
			// We have a channel pending, we add it in database
			dbsync_add_tvchannel (cbxmldata->dbsync, tv_channels_infos, error);

			g_object_unref(cbxmldata->tv_channels_infos);
			cbxmldata->tv_channels_infos = NULL;
		}
	}
}

static void 
xml_text_cb(GMarkupParseContext *context,
    const gchar *text,
    gsize text_len,  
    gpointer user_data,
    GError **error)
{
	CBXMLData* cbxmldata = (CBXMLData*)user_data;
	
	const gchar *element_name;
	element_name = g_markup_parse_context_get_element(context);

	// Add the logo filename
	if(g_ascii_strcasecmp(element_name, "logo_filename") == 0){
		freetuxtv_tv_channel_infos_set_logo_filename(cbxmldata->tv_channels_infos, (gchar*) text);
	}
	
	// Add a label for the channel
	if(g_ascii_strcasecmp(element_name, "label") == 0){
		freetuxtv_tv_channel_infos_add_label(cbxmldata->tv_channels_infos, (gchar*) text);
	}
	
}
