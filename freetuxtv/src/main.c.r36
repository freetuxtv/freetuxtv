/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8-*- */
/*
 * freetuxtv
 * Copyright (C) FreetuxTV Team's 2008
 * Project homepage : http://code.google.com/p/freetuxtv/
 * 
 * freetuxtv is free software.
 * 
 * You may redistribute it and/or modify it under the terms of the
 * GNU General Public License, as published by the Free Software
 * Foundation; either version 2 of the License, or (at your option)
 * any later version.
 * 
 * freetuxtv is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with brasero.  If not, write to:
 * 	The Free Software Foundation, Inc.,
 * 	51 Franklin Street, Fifth Floor
 * 	Boston, MA  02110-1301, USA.
 */

#include <sys/stat.h>

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
	struct stat sb;

	freetuxtv_config_init();

	/* Verifie si le repertoire de configuration existe */
	res = stat(FREETUXTV_USER_DIR, &sb);
	if(res == -1){
		/* Creation du repertoire de configuration */
		res = mkdir(FREETUXTV_USER_DIR, S_IRWXU | S_IRGRP | S_IROTH);
		if(res == -1){
			perror("mkdir");
			fprintf(stderr,
				"FreetuxTV : Cannot create FreetuxTV user directory : %s\n",
				FREETUXTV_USER_DIR);
			return -1;
		}else{
			fprintf(stderr,
				"FreetuxTV : User directory created : %s\n",
				FREETUXTV_USER_DIR);
		}
		/* Creation du repertoire d'images de l'utilisateur */
		res = mkdir(FREETUXTV_USER_IMG_DIR, S_IRWXU | S_IRGRP | S_IROTH);
		if(res == -1){
			perror("mkdir");
			fprintf(stderr,
				"FreetuxTV : Cannot create FreetuxTV images user directory : %s\n",
				FREETUXTV_USER_IMG_DIR);
			return -1;
		}else{
			fprintf(stderr,
				"FreetuxTV : Images user directory created : %s\n",
				FREETUXTV_USER_IMG_DIR);
		}
		/* Creation du repertoire d'images des chaines de l'utilisateur */
		res = mkdir(FREETUXTV_USER_IMG_CHANNELS_DIR, S_IRWXU | S_IRGRP | S_IROTH);
		if(res == -1){
			perror("mkdir");
			fprintf(stderr,
				"FreetuxTV : Cannot create FreetuxTV images channels user directory : %s\n",
				FREETUXTV_USER_IMG_CHANNELS_DIR);
			return -1;
		}else{
			fprintf(stderr,
				"FreetuxTV : Images channels user directory created : %s\n",
				FREETUXTV_USER_IMG_CHANNELS_DIR);
		}
	}else{
		if(!S_ISDIR(sb.st_mode)){
			fprintf(stderr,
				"FreetuxTV : %s is not a directory\n",
				FREETUXTV_USER_DIR);
			return -1;
		}
	}
	
	/* Creation de la BDD si inexistante */
	res = sqlite3_open(FREETUXTV_USER_DB,&db);
	if(res != SQLITE_OK){
		fprintf(stderr,
			"Sqlite3 : %s\n",
			sqlite3_errmsg(db));
		fprintf(stderr,
			"FreetuxTV : Cannot open database %s\n",
			FREETUXTV_USER_DB);
		sqlite3_close(db);
		return -1;
	}

	/* Creation des tables de la base */
	res=sqlite3_exec(db,FREETUXTV_SQL_CREATE_TABLES,NULL,0,&err);
	if(res != SQLITE_OK){
		fprintf(stderr,
			"Sqlite3 : %s\n%s\n",
			sqlite3_errmsg(db),FREETUXTV_SQL_CREATE_TABLES);
		fprintf(stderr,
			"FreetuxTV : Cannot create tables in %s\n",
			FREETUXTV_USER_DB);
		sqlite3_free(err);
	}

	sqlite3_close(db);
	return 0;
}

int main (int argc, char *argv[])
{
	
	init_app();

	gtk_init (&argc, &argv);
	
	FreetuxTVMainWindow *freetuxtv = NULL;
	freetuxtv = FREETUXTV_MAIN_WINDOW(freetuxtv_main_window_new());

	gtk_main ();
	
	return 0;
}
