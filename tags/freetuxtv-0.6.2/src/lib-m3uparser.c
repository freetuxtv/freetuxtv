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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lib-m3uparser.h"

int
libm3uparser_parse(char *file, 
		   int (*callback)(char *_url, int _num, int _argc, 
				   char **_argv, void *_user_data), 
		   void *user_data)
{
	int argc = 0;
	char **argv = NULL;

	FILE* fp = NULL;
  	char* line = NULL;
  	size_t len = 0;
  	ssize_t read;

	int i;
	int err_callback = 0;

	int num = 0;
	
	if(callback == NULL){
		return LIBM3UPARSER_INVALID_CALLBACK;
	}
	
	/* Ouverture du fichier */
	fp = fopen (file, "r");
	
	/* Fichier inexistant */
	if (fp == NULL){
		return LIBM3UPARSER_FILE_NOT_FOUND;
	}
	
	/* Traitement de chaque ligne */
	while ((read = getline(&line, &len, fp)) != -1 && err_callback >= 0) {
		if(read != 0){
			int endline;
			endline = strcspn (line, "\n\r");
			char *textline;
			textline = (char *) malloc ( (endline + 1) * sizeof(char));
			strncpy (textline, line, (endline + 1));
			textline[endline] = '\0';
			
			if(textline[0] == '#'){
				if(strcmp(textline, "#EXTM3U") != 0){
					argc++;
					if(argc == 1){
						argv = (char **) malloc (sizeof(char *));
					}else{
						argv = (char **) realloc (argv, argc * sizeof(char *));
					}
					argv[argc - 1] = malloc ((endline + 1) * sizeof(char));
					strcpy(argv[argc - 1], textline);
					argv[argc - 1][endline] = '\0';
					
				}
			}else{
				/* Envoi des options et de l'url au callback */
				if(textline[0]!='\n'){
					num++;
					err_callback = callback(textline, num,
								argc, argv, 
								user_data);
					for(i=0; i<argc; i++){
						free(argv[i]);
					}
					free(argv);
					argc = 0;
					argv = NULL;
				}
			}
			free (textline);
			textline = NULL;
		}
		free (line);
		line = NULL;
		len = 0;
	}
	
	free (line);
	line = NULL;
	
	for(i=0; i<argc; i++){
		free(argv[i]);
	}
	free(argv);
	argc = 0;
	argv = NULL;

	fclose(fp);
	
	if(err_callback < 0){
		return LIBM3UPARSER_CALLBACK_RETURN_ERROR;	
	}

	return LIBM3UPARSER_OK;
}

int
libm3uparser_get_extinfo (char argc, char **argv,
			  char **time, char **title)
{
	int res;
	
	char** extdata = NULL;
	int nb;

	int i;
	
	if(time != NULL)
		*time = NULL;
	if(title != NULL)
		*title = NULL;
	
	res = libm3uparser_get_extdata (argc, argv, "#EXTINF", &extdata, &nb);
	
	if(nb>0){
		i = nb -1;

		char *begin;
		char *end;
		char *tmp;
		
		int cars;
		
		if(time != NULL){
			begin = strchr(extdata[i], ':') + 1;
			end = strchr(extdata[i], ',');
			cars = end - begin;
			tmp = (char *) malloc ((cars + 1) * sizeof(char));
			strncpy(tmp, begin, cars);
			tmp[cars] = '\0';
			*time = tmp;
		}
		if(title != NULL){
			begin = strchr(extdata[i], ',') + 1;
			end = begin + strlen(begin);
			cars = end - begin;
			tmp = (char *) malloc ((cars + 1) * sizeof(char));
			strncpy(tmp, begin, cars);
			tmp[cars] = '\0';
			*title = tmp;
		}   
	}

	// Free the tab
	for(i=0; i<nb; i++){
		free(extdata[i]);
	}
	free(extdata);
	
	if(res == LIBM3UPARSER_EXTDATA_NOT_FOUND){
		res = LIBM3UPARSER_EXTINFO_NOT_FOUND;
	}
	
	return res;
}

int
libm3uparser_get_extdata (char argc, char **argv, char *ext_name,
			  char ***ext_data, int *ext_count)
{
	int i;
	int nb = 0;
	
	int res = LIBM3UPARSER_OK;

	char *begin;
	char *end;
	char *optname;
	
	int cars;
	
	if(!ext_data && !ext_count){
		res = LIBM3UPARSER_INVALID_PARAM;
	}
	
	if(res == LIBM3UPARSER_OK){

		res = LIBM3UPARSER_EXTINFO_NOT_FOUND;
		
		for(i=0; i<argc; i++){
			
			/* Get ext name */
			begin = argv[i];
			end = strchr(argv[i], ':');
			if(end){
				cars = end - begin;
				optname = (char *) malloc ((cars + 1) * sizeof(char));
				strncpy(optname, begin, cars);
				optname[cars] = '\0';
			
				/* Compare the name found with the given ext_name */
				if(strcmp(optname, ext_name) == 0){
					nb++;

					*ext_data = (char **) realloc(*ext_data, nb * sizeof(char*));

					// Get current ext data
					begin = end + 1;
					end = argv[i] + strlen(argv[i]);
					cars = end - begin;
	
					(*ext_data)[nb-1] = (char *) malloc ((cars + 1) * sizeof(char));			
					strncpy((*ext_data)[nb-1], begin, cars);
					(*ext_data)[nb-1][cars] = '\0';

					res = LIBM3UPARSER_OK;
				}
				free (optname);
			}
		}
		*ext_count = nb;
	}
	
	return res;
}

const char*
libm3uparser_errmsg(int err){
	switch(err){
	case LIBM3UPARSER_OK :
		return "OK";
	case LIBM3UPARSER_INVALID_CALLBACK :
		return "invalid callback function";
	case LIBM3UPARSER_FILE_NOT_FOUND :
		return "file not found";
	case LIBM3UPARSER_EXTINFO_NOT_FOUND :
		return "M3U EXTINFO option not found in the list";
	case LIBM3UPARSER_EXTDATA_NOT_FOUND :
		return "M3U EXTNAME option not found in the list";		
	case LIBM3UPARSER_CALLBACK_RETURN_ERROR :
		return "the callback function has returned an error";
	case LIBM3UPARSER_INVALID_PARAM :
		return "the function params are not valid";	
	default:
		return "bad error number";
	}
}
