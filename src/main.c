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

#include "freetuxtv-app.h"
#include "freetuxtv-window-main.h"
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

static FreetuxTVApp *
freetuxtv_app_create_app ()
{
	FreetuxTVApp *app;

	GtkWidget *windowmain;
	GtkWidget *widget;

	GtkWidget *scrolledwindowchannels;	

	GtkWidget *eventboxplayer;

	app = g_new0 (FreetuxTVApp, 1);

	app->name = "FreetuxTV";


	/* Chargemetn de la configuration par defaut */
	app->config.windowminimode_stayontop = FALSE;
	app->config.windowminimode_width = 320;
	app->config.windowminimode_height = 240;

	/* Création de la fenêtre */
	app->windowmain = glade_xml_new (FREETUXTV_GLADEXML,
					 "windowmain", NULL);
	glade_xml_signal_autoconnect (app->windowmain);
	
	/* Ajout du widget de la liste des chaines */
	GtkWidget *eventbox;
	GdkColor color;
	eventbox = gtk_event_box_new();
	color.pixel = 0;
	color.red   = 0xdf * 0x100;
	color.green = 0xe0 * 0x100;
	color.blue  = 0xe6 * 0x100;
	gtk_widget_modify_bg(eventbox, GTK_STATE_NORMAL, &color);
	
	scrolledwindowchannels = glade_xml_get_widget (app->windowmain,
						       "windowmain_scrolledwindowchannels");
	gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(scrolledwindowchannels),
					      eventbox);
	app->channelsgroups = NULL;	
	gtk_widget_show_all (eventbox);

	/* Ajout du widget du lecteur */
	eventboxplayer = glade_xml_get_widget (app->windowmain,
					       "windowmain_eventboxplayer");
	app->player = FREETUXTV_PLAYER(freetuxtv_player_new ());
	gtk_container_add (GTK_CONTAINER(eventboxplayer), GTK_WIDGET(app->player));

	/* Connexion des signaux */
	widget = glade_xml_get_widget (app->windowmain,
				       "windowmain_menuitemgroupsadd");
	g_signal_connect(G_OBJECT(widget),
			 "activate",
			 G_CALLBACK(on_windowmain_menuitemgroupsadd_activate),
			 app);

	widget = glade_xml_get_widget (app->windowmain,
				       "windowmain_buttonclearfilter");
	g_signal_connect(G_OBJECT(widget),
			 "clicked",
			 G_CALLBACK(on_windowmain_buttonclearfilter_clicked),
			 app);

	widget = glade_xml_get_widget (app->windowmain,
				       "windowmain_entryfilter");
	g_signal_connect(G_OBJECT(widget),
			 "changed",
			 G_CALLBACK(on_windowmain_entryfilter_changed),
			 app);
	
	widget = glade_xml_get_widget (app->windowmain,
				       "windowmain_buttonstop");
	g_signal_connect(G_OBJECT(widget),
			 "clicked",
			 G_CALLBACK(on_windowmain_buttonstop_clicked),
			 app);
	
	widget = glade_xml_get_widget (app->windowmain,
				       "windowmain_volumecontrol");
	g_signal_connect(G_OBJECT(widget),
			 "value-changed",
			 G_CALLBACK(on_windowmain_volumecontrol_value_changed),
			 app);

	widget = glade_xml_get_widget (app->windowmain,
				       "windowmain_buttonfullscreen");
	g_signal_connect(G_OBJECT(widget),
			 "clicked",
			 G_CALLBACK(on_windowmain_buttonfullscreen_clicked),
			 app);

	widget = glade_xml_get_widget (app->windowmain,
				       "windowmain_buttonminimode");
	g_signal_connect(G_OBJECT(widget),
			 "clicked",
			 G_CALLBACK(on_windowmain_buttonminimode_clicked),
			 app);

	return app;

}

int main (int argc, char *argv[])
{
	
	FreetuxTVApp *app;
	
	init_app();
	
	gtk_init(&argc, &argv);
	
	app = freetuxtv_app_create_app ();
	if (app == NULL)
		return 1;	

	channels_list_update_from_db (app);

	gtk_main();
	
	return 0;
}
