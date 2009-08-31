/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8-*- */
/*
 * freetuxtv
 * Copyright (C) FreetuxTV Team's 2008
 * Project homepage : http://code.google.com/p/freetuxtv/
 * 
 * freetuxtv is free software.
 * 
 */

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>
#include <sqlite3.h>

#include "freetuxtv-app.h"
#include "freetuxtv-i18n.h"


typedef struct _CBXMLData
{
	FreetuxTVApp *app;
	struct sqlite3 *db;
	int current_logo;	
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


gint
logos_list_synchronize (FreetuxTVApp *app)
{
	struct sqlite3 *db;
	int res;
	char *err=0;
	gint ret = 0;
	gchar *query;

	gchar *err_msg = NULL;

	gchar *user_db;
	user_db = g_strconcat(g_get_user_config_dir(), 
			      "/FreetuxTV/freetuxtv.db", NULL);

	res = sqlite3_open(user_db, &db);
	if(res != SQLITE_OK){
		g_printerr ("Sqlite3 : %s\n",
			    sqlite3_errmsg(db));
		g_printerr ("FreetuxTV : Cannot open database %s\n",
			    user_db);
		sqlite3_close(db);
		ret = -1;
	}
	
	if(ret == 0){
		
		// Mise à jour de la barre de statut
		gchar *text;
		text = g_strdup_printf(_("Synchronizing the logo list"));
		windowmain_statusbar_push (app, "UpdateMsg", text);
		g_free(text);
		g_print("FreetuxTV : Synchronizing the logo list\n");

		// Efface les anciens logos
		query = sqlite3_mprintf("DELETE FROM channel_logo");
		res = sqlite3_exec(db, query, NULL, 0, &err);
		if(res != SQLITE_OK){

			err_msg = g_strdup_printf(_("Error when deleting the logos list.\n\nSQLite has returned error :\n%s."),
						  sqlite3_errmsg(db));
			windowmain_show_error (app, err_msg);
			g_free(err_msg);
			ret = -1;
		}else{
			sqlite3_free(query);

			// Ajout de la liste des logos dans la base de donnée
			CBXMLData cbxmldata;
			cbxmldata.app = app;
			cbxmldata.db = db;
			cbxmldata.current_logo = -1;
			
			gsize filelen;
			static GMarkupParser parser = { xml_start_cb, NULL, NULL, 
							NULL, xml_err_cb };
			GMarkupParseContext *context;
			
			context = g_markup_parse_context_new (&parser, 
							      G_MARKUP_DO_NOT_USE_THIS_UNSUPPORTED_FLAG,
							      &cbxmldata, NULL);
			gchar *xml_data;
			g_file_get_contents (FREETUXTV_DIR "/channel_logos.xml", 
					     &xml_data, &filelen, NULL);
			g_markup_parse_context_parse (context, xml_data, -1, NULL);
			
			sqlite3_close(db);
		}
		
		windowmain_statusbar_pop (app, "UpdateMsg");
	}

	return ret;
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
	
	gchar* sql_query = "";
	int res;
	char *err=0;

	gchar *label;

	if(g_ascii_strcasecmp(element_name, "logo") == 0){
		
		label = g_strdup(attribute_values[1]);
		sql_query = sqlite3_mprintf("INSERT INTO channel_logo (label_channellogo, filename_channellogo) values ('%q','%q');", 
					    attribute_values[1],
					    attribute_values[0]);
		res=sqlite3_exec(cbxmldata->db, sql_query, NULL, 0, &err);
		if(res != SQLITE_OK){
			g_printerr("Sqlite3 : %s\n%s\n",
				   sqlite3_errmsg(cbxmldata->db), sql_query);
			sqlite3_free(err);
		}

		cbxmldata->current_logo = sqlite3_last_insert_rowid(cbxmldata->db);

		sqlite3_free(sql_query);		
	}
	
	if(g_ascii_strcasecmp(element_name, "logolabel") == 0){

		label = g_strdup(attribute_values[0]);		
		sql_query = sqlite3_mprintf("INSERT INTO label_channellogo (label_labelchannellogo, idchannellogo_labelchannellogo) values ('%q', (SELECT MAX(id_channellogo) FROM channel_logo));",
					    attribute_values[0]);
		res=sqlite3_exec(cbxmldata->db, sql_query, NULL, 0, &err);
		if(res != SQLITE_OK){
			g_printerr("Sqlite3 : %s\n%s\n",
				   sqlite3_errmsg(cbxmldata->db), sql_query);
			sqlite3_free(err);
		}
		sqlite3_free(sql_query);
		
	}

	if(cbxmldata->current_logo >= 0){
		sql_query = sqlite3_mprintf("UPDATE channel SET idchannellogo_channel=%d WHERE name_channel = '%q';",
					    cbxmldata->current_logo,
					    label);
		res=sqlite3_exec(cbxmldata->db, sql_query, NULL, 0, &err);
		if(res != SQLITE_OK){
			g_printerr("Sqlite3 : %s\n%s\n",
				   sqlite3_errmsg(cbxmldata->db), sql_query);
			sqlite3_free(err);
		}
		sqlite3_free(sql_query);

		if(cbxmldata->app->debug == TRUE){
			g_print("FreetuxTV-debug : Add channel logos '%s' in database\n",
				label);
		}

		g_free(label);
	
	}
}

static void 
xml_err_cb(GMarkupParseContext *context,
	   GError *error,
	   gpointer data)
{
	g_printerr("FreetuxTV : Error parsing XML -> %s\n", error->message);
}
