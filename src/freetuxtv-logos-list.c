/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8-*- */
/*
 * freetuxtv
 * Copyright (C) FreetuxTV Team's 2008
 * Project homepage : http://code.google.com/p/freetuxtv/
 * 
 * freetuxtv is free software.
 * 
 */

#include <gtk/gtk.h>

#include "freetuxtv-logos-list.h"

#include "freetuxtv-app.h"
#include "freetuxtv-i18n.h"


typedef struct _CBXMLData
{
	FreetuxTVApp *app;
	DBSync *dbsync;
	long current_logo;	
} CBXMLData;

static void 
xml_start_cb(GMarkupParseContext *context,
	     const gchar *element_name,
	     const gchar **attribute_names,
	     const gchar **attribute_values,
	     gpointer data,
	     GError **error);

static void 
xml_err_cb(GMarkupParseContext *context,
	   GError *error,
	   gpointer data);


void
logos_list_synchronize (FreetuxTVApp *app, DBSync *dbsync,
			GError** error)
{
	g_return_if_fail(dbsync != NULL);
	g_return_if_fail(error != NULL);
	
	// Mise à jour de la barre de statut
	gchar *text;
	text = g_strdup_printf(_("Synchronizing the logo list"));
	windowmain_statusbar_push (app, "UpdateMsg", text);
	g_free(text);
	g_print("FreetuxTV : Synchronizing the logo list\n");

	// Efface les anciens logos	
	dbsync_delete_channels_logos (dbsync, error);

	if(*error == NULL){
		// Ajout de la liste des logos dans la base de donnée
		CBXMLData cbxmldata;
		cbxmldata.app = app;
		cbxmldata.dbsync = dbsync;
		cbxmldata.current_logo = -1;
		
		gsize filelen;
		static GMarkupParser parser = { xml_start_cb, NULL, NULL, 
						NULL, xml_err_cb };
		GMarkupParseContext *context;
		
		context = g_markup_parse_context_new (&parser, 
						      G_MARKUP_DO_NOT_USE_THIS_UNSUPPORTED_FLAG,
						      &cbxmldata, NULL);
		gchar *xml_data;
		g_file_get_contents (FREETUXTV_DIR "/channels_logos.xml", 
				     &xml_data, &filelen, NULL);
		g_markup_parse_context_parse (context, xml_data, -1, NULL);
	}
		
	windowmain_statusbar_pop (app, "UpdateMsg");
}

gchar*
logos_list_get_channel_logo_filename(FreetuxTVApp *app, 
				     FreetuxTVChannelInfos* channel_infos,
				     gboolean none_icon)
{
	gchar *imgfile = NULL;
	gchar *user_img_channels_dir;
	user_img_channels_dir = g_strconcat(g_get_user_config_dir(), 
					    "/FreetuxTV/images/channels", NULL);
	if(channel_infos->logo_name == NULL){
		if(none_icon){
			imgfile = g_strconcat(user_img_channels_dir, "/_none.png", NULL);	
			if(!g_file_test(imgfile,G_FILE_TEST_EXISTS)){
				g_free(imgfile);
				imgfile = g_strconcat(FREETUXTV_DIR "/images/channels/_none.png", NULL);	
			}
		}
	}else{
		imgfile = g_strconcat(user_img_channels_dir,"/", channel_infos->logo_name, NULL);
		if(!g_file_test(imgfile,G_FILE_TEST_EXISTS)){
			g_free(imgfile);
			imgfile = g_strconcat(FREETUXTV_DIR "/images/channels/", channel_infos->logo_name, NULL);	
		}
	}
	
	g_free(user_img_channels_dir);
	
	return imgfile;
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
	
	gchar *label = NULL;
	gchar *filename = NULL;

	if(g_ascii_strcasecmp(element_name, "logo") == 0){
		label = g_strdup(attribute_values[1]);
		filename = g_strdup(attribute_values[0]);
		dbsync_add_channel_logo (cbxmldata->dbsync, label, filename, 
					 &(cbxmldata->current_logo), error);
		g_free(filename);
		
	}
	
	if(g_ascii_strcasecmp(element_name, "logolabel") == 0){
		label = g_strdup(attribute_values[0]);
		dbsync_add_label_channel_logo (cbxmldata->dbsync, label, 
					       cbxmldata->current_logo, error);
	}

	if(label != NULL){
		if(cbxmldata->app->debug == TRUE){
			g_print("FreetuxTV-debug : Add channel logos '%s' in database\n",
				label);
		}

		g_free(label);
		label = NULL;
	}
}

static void 
xml_err_cb(GMarkupParseContext *context,
	   GError *error,
	   gpointer data)
{
	g_printerr("FreetuxTV : Error parsing XML -> %s\n", error->message);
}
