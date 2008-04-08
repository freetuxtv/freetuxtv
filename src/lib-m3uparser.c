/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8-*- */
/*
 * freetuxtv
 * Copyright (C) FreetuxTV Team's 2008
 * Project homepage : http://code.google.com/p/freetuxtv/
 * 
 * freetuxtv is free software.
 * 
 */

#include <stdlib.h>
#include <string.h>

#include "lib-m3uparser.h"

int
libm3uparser_parse(char *file, 
		   int (*callback)(char *_url, int _argc, 
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
	
	if(callback == NULL){
		return LIBM3UPARSER_INVALID_CALLBACK;
	}

	/* Ouverture du fichier */
	fp = fopen (file, "r");
	
	/* Fichier inexistant */
	if (fp == NULL){
		fclose(fp);
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
					err_callback = callback(textline, 
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
			  char **time, char **title){
	int i;
	for(i=0; i<argc; i++){

		char *begin;
		char *end;
		char *optname;
		char *tmp;
		
		int cars;
		
		if(time != NULL)
			*time = NULL;
		if(title != NULL)
			*title = NULL;
		
		/* Recupere le nom de l'option */
		begin = argv[i];
		end = strchr(argv[i], ':');
		cars = end - begin;
		optname = (char *) malloc ((cars + 1) * sizeof(char));
		strncpy(optname, begin, cars);
		optname[cars] = '\0';
		
		/* Si l'option est EXTINF on recupere le titre */
       		if(strcmp(optname, "#EXTINF") == 0){
			if(time != NULL){
				begin = strchr(argv[i], ':') + 1;
				end = strchr(argv[i], ',');
				cars = end - begin;
				tmp = (char *) malloc ((cars + 1) * sizeof(char));
				strncpy(tmp, begin, cars);
				tmp[cars] = '\0';
				*time = tmp;
			}
			if(title != NULL){
				begin = strchr(argv[i], ',') + 1;
				end = begin + strlen(begin);
				cars = end - begin;
				tmp = (char *) malloc ((cars + 1) * sizeof(char));
				strncpy(tmp, begin, cars);
				tmp[cars] = '\0';
				*title = tmp;
			}   
			free (optname);
			return LIBM3UPARSER_OK;
		}
		free (optname);
	}
	return LIBM3UPARSER_EXTINFO_NOT_FOUND;
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
	case LIBM3UPARSER_CALLBACK_RETURN_ERROR :
		return "the callback function has returned an error";
	default:
		return "bad error number";
	}
}
