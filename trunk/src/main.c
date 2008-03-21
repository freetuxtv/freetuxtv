/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4-*- */
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

#include "freetuxtv-main-window.h"

int
init_app()
{

	struct sqlite3 *db;
	int res;
	char *err=0;

	gchar *user_img_channels_dir;
	user_img_channels_dir = g_strconcat(g_get_user_config_dir(), 
										"/FreetuxTV/images/channels", NULL);
	gchar *user_db;
	user_db = g_strconcat(g_get_user_config_dir(), 
						  "/FreetuxTV/freetuxtv.db", NULL);

	/* Verifie si le repertoire de configuration existe */
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
		
		res = sqlite3_open(user_db,&db);
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
		
		sqlite3_close(db);	
	}

	g_free(user_img_channels_dir);
	g_free(user_db);

	return 0;
}

int main (int argc, char *argv[])
{

	gtk_init (&argc, &argv);

	init_app();
	
	FreetuxTVMainWindow *freetuxtv = NULL;
	freetuxtv = FREETUXTV_MAIN_WINDOW(freetuxtv_main_window_new());

	gtk_main ();
	
	return 0;
}
