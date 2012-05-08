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

#ifndef LIB_M3UPARSER_H
#define LIB_M3UPARSER_H

enum libm3uparser_error {
	LIBM3UPARSER_OK = 0,
	LIBM3UPARSER_INVALID_CALLBACK = -1,
	LIBM3UPARSER_FILE_NOT_FOUND = -2,
	LIBM3UPARSER_EXTINFO_NOT_FOUND = -3,
	LIBM3UPARSER_EXTDATA_NOT_FOUND = -4,
	LIBM3UPARSER_CALLBACK_RETURN_ERROR = -5,
	LIBM3UPARSER_INVALID_PARAM = -6
};

int 
libm3uparser_parse(char *file,
		   int (*callback)(char *url, int num, int argc, 
				    char **argv, void *user_data), 
		   void *user_data);

int
libm3uparser_get_extinfo (char argc, char **argv, 
			  char **time, char **title);

int
libm3uparser_get_extdata (char argc, char **argv, char *ext_name,
			  char ***ext_data, int *ext_count);

const char *
libm3uparser_errmsg(int err);

#endif /* LIB_M3UPARSER_H */
