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

#include <stdlib.h>
#include <gtk/gtk.h>
#include <glade/glade.h>
#include <sqlite3.h>

#include "freetuxtv-channels-list.h"
#include "freetuxtv-player.h"

static void xml_start_cb(GMarkupParseContext *context,
			 const gchar *element_name,
			 const gchar **attribute_names,
			 const gchar **attribute_values,
			 gpointer data,
			 GError **error)
{
	struct sqlite3 *db = (sqlite3*) data;
	gchar* sql_query = "";
	int res;
	char *err=0;
	
	if(g_ascii_strcasecmp(element_name, "logo") == 0){
		
		sql_query = sqlite3_mprintf("INSERT INTO channel_logo (label_channellogo, filename_channellogo) values ('%q','%q');", 
					    attribute_values[1],
					    attribute_values[0]);
		res=sqlite3_exec(db, sql_query, NULL, 0, &err);
		if(res != SQLITE_OK){
			g_printerr("Sqlite3 : %s\n%s\n",
				   sqlite3_errmsg(db), sql_query);
			sqlite3_free(err);
		}
		sqlite3_free(sql_query);		
	}
	if(g_ascii_strcasecmp(element_name, "logolabel") == 0){
		
		sql_query = sqlite3_mprintf("INSERT INTO label_channellogo (label_labelchannellogo, idchannellogo_labelchannellogo) values ('%q', (SELECT MAX(id_channellogo) FROM channel_logo));",
					    attribute_values[0]);
		res=sqlite3_exec(db, sql_query, NULL, 0, &err);
		if(res != SQLITE_OK){
			g_printerr("Sqlite3 : %s\n%s\n",
				   sqlite3_errmsg(db), sql_query);
			sqlite3_free(err);
		}
		sqlite3_free(sql_query);
		
	}
	//g_print("%s\n", sql_query);
}

static void xml_err_cb(GMarkupParseContext *context,
		       GError *error,
		       gpointer data)
{
	g_printerr("FreetuxTV : Error parsing XML -> %s\n", error->message);
}

int
init_app()
{

	struct sqlite3 *db;
	int res;
	char *err=0;
	
	gchar *user_cache_dir;
	user_cache_dir = g_strconcat(g_get_user_config_dir(), 
				     "/FreetuxTV/cache", NULL);

	gchar *user_img_channels_dir;
	user_img_channels_dir = g_strconcat(g_get_user_config_dir(), 
					    "/FreetuxTV/images/channels", NULL);
	gchar *user_db;
	user_db = g_strconcat(g_get_user_config_dir(), 
			      "/FreetuxTV/freetuxtv.db", NULL);
	
	/* Verifie si le repertoire des images utilisateur existe */
	if (! g_file_test (user_cache_dir, 
			   G_FILE_TEST_EXISTS)){
		res = g_mkdir_with_parents (user_cache_dir,
					    0744);
		if(res == 0){
			g_printerr ("FreetuxTV : Directory created : %s\n",
				    user_cache_dir);	
		}else{
			g_printerr ("FreetuxTV : Cannot create directory : %s\n",
				    user_cache_dir);
			return -1;
		}
	}
	if (! g_file_test (user_cache_dir,
			   G_FILE_TEST_IS_DIR)){
		g_printerr ("FreetuxTV : Not a directory : %s\n",
			    user_cache_dir);
		return -1;
	}

	/* Verifie si le repertoire des images utilisateur existe */
	if (! g_file_test (user_img_channels_dir, 
			   G_FILE_TEST_EXISTS)){
		res = g_mkdir_with_parents (user_img_channels_dir,
					    0744);
		if(res == 0){
			g_printerr ("FreetuxTV : Directory created : %s\n",
				    user_img_channels_dir);	
		}else{
			g_printerr ("FreetuxTV : Cannot create directory : %s\n",
				    user_img_channels_dir);
			return -1;
		}
	}
	if (! g_file_test (user_img_channels_dir,
			   G_FILE_TEST_IS_DIR)){
		g_printerr ("FreetuxTV : Not a directory : %s\n",
			    user_img_channels_dir);
		return -1;
	}
	
	/* Creation de la BDD si inexistante */
	if (! g_file_test (user_db, G_FILE_TEST_IS_REGULAR)){
		
		res = sqlite3_open(user_db, &db);
		if(res != SQLITE_OK){
			g_printerr ("Sqlite3 : %s\n",
				    sqlite3_errmsg(db));
			g_printerr ("FreetuxTV : Cannot open database %s\n",
				    user_db);
			sqlite3_close(db);
			return -1;
		}
		
		/* Lecture du fichier contenant les requetes de création de la base */
		gchar* filename;
		filename = FREETUXTV_DIR "/sqlite3-create-tables.sql";
		gchar* sql_query;
		gsize filelen;
		
		if (g_file_get_contents (filename, &sql_query, &filelen, NULL)){
			
			res=sqlite3_exec(db, sql_query, NULL, 0, &err);
			if(res != SQLITE_OK){
				g_printerr("Sqlite3 : %s\n%s\n",
					   sqlite3_errmsg(db), sql_query);
				g_printerr("FreetuxTV : Cannot create tables in %s\n",
					   user_db);
				sqlite3_free(err);
			}
		}else{
			g_printerr("FreetuxTV : Cannot read file %s\n", filename);
		}
		
		
		g_free(sql_query);
		
		/* Ajout de la liste des logos dans la base de donnée */
		static GMarkupParser parser = { xml_start_cb, NULL, NULL, 
						NULL, xml_err_cb };
		GMarkupParseContext *context;
		
		context = g_markup_parse_context_new (&parser, 
						      G_MARKUP_DO_NOT_USE_THIS_UNSUPPORTED_FLAG,
						      db, NULL);
		gchar *xml_data;
		g_file_get_contents (FREETUXTV_DIR "/channel_logos.xml", 
				     &xml_data, &filelen, NULL);
		g_markup_parse_context_parse (context, xml_data, -1, NULL);
		
		sqlite3_close(db);
	}

	g_free(user_cache_dir);
	g_free(user_img_channels_dir);
	g_free(user_db);

	return 0;
}

GladeXML *gladexml;

int main (int argc, char *argv[])
{
	
	GtkWidget *windowmain;
	GtkWidget *filterentry;

	GtkWidget *scrolledwindowchannels;
	GtkWidget *channelslist;	

	GtkWidget *eventboxplayer;
	GtkWidget *player;

	init_app();

	gtk_init(&argc, &argv);
	gladexml = glade_xml_new (FREETUXTV_DIR "/freetuxtv.glade", NULL, NULL);

	glade_xml_signal_autoconnect (gladexml);
	windowmain = glade_xml_get_widget (gladexml,"windowmain");
	
	/* Ajout du widget de la liste des chaines */
	channelslist = freetuxtv_channels_list_new ();
	scrolledwindowchannels = glade_xml_get_widget (gladexml,
						       "windowmain_scrolledwindowchannels");
	gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(scrolledwindowchannels),
					      channelslist);

	/* Ajout du widget du lecteur */
	eventboxplayer = glade_xml_get_widget (gladexml, "windowmain_eventboxplayer");
	player = freetuxtv_player_new ();
	gtk_container_add (GTK_CONTAINER(eventboxplayer), player);

	gtk_widget_show(windowmain);
	gtk_main();
	
	return 0;
}
