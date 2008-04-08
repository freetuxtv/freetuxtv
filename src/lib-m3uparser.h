/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8-*- */
/*
 * freetuxtv
 * Copyright (C) FreetuxTV Team's 2008
 * Project homepage : http://code.google.com/p/freetuxtv/
 * 
 * freetuxtv is free software.
 * 
 */

#ifndef LIB_M3UPARSER_H
#define LIB_M3UPARSER_H

#include <stdio.h>

enum libm3uparser_error {
	LIBM3UPARSER_OK = 0,
	LIBM3UPARSER_INVALID_CALLBACK = -1,
	LIBM3UPARSER_FILE_NOT_FOUND = -2,
	LIBM3UPARSER_EXTINFO_NOT_FOUND = -3,
	LIBM3UPARSER_CALLBACK_RETURN_ERROR = -4
};

int 
libm3uparser_parse(char *file,
		   int (*callback)(char *url, int argc, 
				    char **argv, void *user_data), 
		   void *user_data);

int
libm3uparser_get_extinfo (char argc, char **argv, 
			  char **time, char **title);

const char *
libm3uparser_errmsg(int err);

#endif /* LIB_M3UPARSER_H */
