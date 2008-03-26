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

#include "freetuxtv-m3uparser.h"

/* Parse le fichier */
int freetuxtv_m3uparser_parse(char* file,void* callback,void* data){

	int retour = OK;
	int compteur=0;

	// ouverture du fichier
	FILE* fp = fopen (file, "r");
  	char* line = NULL;
  	size_t len = 0;
  	ssize_t read;

	if (fp == NULL){
		// fichier inexistant
		return FILE_NOT_FOUND;
	}
	while ((read = getline(&line, &len, fp)) != -1) {
    		retour = freetuxtv_m3uparser_parse_line(line,callback,data);
    		if (retour != OK)
      			break;
		compteur++;
  	}
	printf("Nombre d'enregistrements : %d \n",compteur);
	fclose(fp);
	return retour;

}
/* Parse une ligne */
int freetuxtv_m3uparser_parse_line(char* buffer,void* callback,void *data){
	char* c = buffer;
	char* c2,*c3;
	char* artiste,*nom,*nomChaine;
	char* optionVlc;
	
	const char *extm3u="#EXTM3U";
	const char *extinf="#EXTINF";
	const char *extvlcopt="#EXTVLCOPT";

	// si la ligne commence par un # 
	if(*c=='#'){
		// on passe les #
		for(c = buffer; *c!='#';c++){
			// passage sur #
			printf("mange#");		
		}
		if(!strncmp(c,extm3u,strlen(extm3u))){
			// on passe 
			printf("Fichier M3U \n");
		}
		if(!strncmp(c,extinf,strlen(extinf))){
			// lecture des infos EXTINF
			printf("Infos : %s",c);
			// + decoupage nom artiste 
			// decoupage artiste apres #EXTINF:
			c = strchr(buffer,':');
			artiste = c + 1;
			// jusqu'à ','
			c2 = strchr(buffer,',');
			*c2 = '\0';
			// nom
			nom = c2 + 1;
			// sous decoupage special free
			c3 = strchr(nom,'-');
			nomChaine = c3 + 1 ;
			printf("Artiste : %s Nom : %s NomChaine : %s",artiste,nom,nomChaine);
			
		}
		if(!strncmp(c,extvlcopt,strlen(extvlcopt))){
			// lecture des options VLC
			c = strchr(buffer,':');
			c++;
			optionVlc = c;
			printf("Options VLC : %s",c);

		}
		
	}else{	
		// ici on a que l'url
		printf("%s",buffer);
	}
	
	return OK;
}

char* freetuxtv_m3uparser_errmsg(int err){

	char retour[128];
	sprintf(retour,"%s%d","Erreur numéro : ",err);

	switch(err){
		case 0 : return "OK\n";
		case -1 : return "Fichier non trouvé\n";
		case -2 : return "Erreur de structure\n";
		default: return retour;
	}
}



