/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4-*- */
/*
 * freetuxtv
 * Copyright (C) FreetuxTV Team's 2008
 * Project homepage : http://code.google.com/p/freetuxtv/
 * 
 * freetuxtv is free software.
 * 
 */

#include <gtk/gtk.h>
#include <sqlite3.h>

#include "freetuxtv-config.h"
#include "freetuxtv-main-window.h"

int
init_app()
{

	struct sqlite3 *db;
	int res;
	char *err=0;

	freetuxtv_config_init();

	/* Verifie si le repertoire de configuration existe */
	if (! g_file_test (FREETUXTV_USER_IMG_CHANNELS_DIR, 
					   G_FILE_TEST_EXISTS)){
		res = g_mkdir_with_parents (FREETUXTV_USER_IMG_CHANNELS_DIR,
									0744);
		if(res == 0){
			g_printerr ("FreetuxTV : Directory created : %s\n",
						FREETUXTV_USER_IMG_CHANNELS_DIR);	
		}else{
			g_printerr ("FreetuxTV : Cannot create directory : %s\n",
						FREETUXTV_USER_IMG_CHANNELS_DIR);
		}
	}
	if (! g_file_test (FREETUXTV_USER_IMG_CHANNELS_DIR,
					   G_FILE_TEST_IS_DIR)){
		g_printerr ("FreetuxTV : Not a directory : %s\n",
					FREETUXTV_USER_IMG_CHANNELS_DIR);
		return -1;
	}
	
	/* Creation de la BDD si inexistante */
	if (! g_file_test (FREETUXTV_USER_DB, G_FILE_TEST_IS_REGULAR)){
		
		res = sqlite3_open(FREETUXTV_USER_DB,&db);
		if(res != SQLITE_OK){
			g_printerr ("Sqlite3 : %s\n",
						sqlite3_errmsg(db));
			g_printerr ("FreetuxTV : Cannot open database %s\n",
						FREETUXTV_USER_DB);
			sqlite3_close(db);
			return -1;
		}
		
		/* Lecture du fichier contenant les requetes de création de la base */
		gchar* filename;
		filename = g_strconcat(FREETUXTV_CONFIG_DIR,
							   "/sqlite3-create-tables.sql",NULL);
		gchar* sql_query;
		gsize filelen;
		
		if (g_file_get_contents (filename, &sql_query, &filelen, NULL)){
			
			res=sqlite3_exec(db,sql_query,NULL,0,&err);
			if(res != SQLITE_OK){
				fprintf(stderr,
					"Sqlite3 : %s\n%s\n",
						sqlite3_errmsg(db),sql_query);
				fprintf(stderr,
						"FreetuxTV : Cannot create tables in %s\n",
						FREETUXTV_USER_DB);
				sqlite3_free(err);
			}
		}else{
			fprintf(stderr,
				"FreetuxTV : Cannot read file %s\n",
				FREETUXTV_CONFIG_DIR);
		}
		
		sqlite3_close(db);	
	}
	return 0;
}

int main (int argc, char *argv[])
{

	gtk_init (&argc, &argv);

	init_app();
	
	FreetuxTVMainWindow *freetuxtv = NULL;
	freetuxtv = FREETUXTV_MAIN_WINDOW(freetuxtv_main_window_new());

	gtk_main ();

	freetuxtv_config_free ();
	
	return 0;
}
