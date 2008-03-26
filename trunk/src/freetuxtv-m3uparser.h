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

#ifndef FREETUXTV_M3UPARSER_H
#define FREETUXTV_M3UPARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum erreur {FILE_NOT_FOUND=-1,OK=0};

/* 
 * Parse un fichier *.m3u
 */
int freetuxtv_m3uparser_parse(char* file,void* callback,void* data);

/* 
 * Parse une ligne
 */
int freetuxtv_m3uparser_parse_line(char* buffer,void* callback,void* data);


/* 
 * Gestion erreur affiche le message en fonction du code d'erreur passé
 */
char* freetuxtv_m3uparser_errmsg(int err);

/* 
 * Fonction appeler pour chaque enregistrement trouvé
 */
void callback(void* data,char* url,int argc,char** argv);

#endif /* FREETUXTV_M3UPARSER_H */
