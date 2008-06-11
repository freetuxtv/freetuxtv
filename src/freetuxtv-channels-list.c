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

#include <gtk/gtk.h>
#include <glade/glade.h>
#include <sqlite3.h>
#include <curl/curl.h>
#include <sqlite3.h>

#include "freetuxtv-app.h"
#include "freetuxtv-channels-list.h"
#include "freetuxtv-channels-group.h"
#include "lib-m3uparser.h"

typedef struct _Parsem3uData
{
	gchar *id;
	struct sqlite3 *db;
} Parsem3uData;

typedef struct _CBUserData
{
	FreetuxTVApp *app;
	FreetuxTVChannelsGroup *channelsgroup;
} CBUserData;

static int
channels_group_get_file (FreetuxTVChannelsGroup *self, FreetuxTVApp *app,
			 gchar **filename, gboolean cache);

static void
channels_group_delete_channels (FreetuxTVChannelsGroup *self,
				gpointer user_data);

static void
channels_group_delete_group (FreetuxTVChannelsGroup *self,
			     gpointer user_data);

static void
channels_group_refresh_group (FreetuxTVChannelsGroup *self,
			      gpointer data);

static void
channels_group_reload_channels (FreetuxTVChannelsGroup *self,
				gpointer data);

static GtkWidget*
channels_list_get_channelsgroups_parent (FreetuxTVApp *app);

static int 
on_exec_add_channels_group (void *data, int argc, char **argv, char **colsname);

static int 
on_exec_add_channel (void *data, int argc, char **argv, char **colsname);

static int 
on_parsem3u_add_channel (char *url, int argc, 
			 char **argv, void *user_data);

void
channels_list_add_channels_group (FreetuxTVApp *app,
				  FreetuxTVChannelsGroup *channels_group)
{
	gtk_box_pack_start (GTK_BOX(app->channelsgroups), 
			    GTK_WIDGET(channels_group), FALSE, FALSE, 0);

	g_signal_connect(G_OBJECT(channels_group), "menu-delete-group",
			 G_CALLBACK(channels_group_delete_group), app);

	g_signal_connect(G_OBJECT(channels_group), "menu-delete-channels",
			 G_CALLBACK(channels_group_delete_channels), app);

	g_signal_connect(G_OBJECT(channels_group), "menu-refresh-group",
			 G_CALLBACK(channels_group_refresh_group), app);
}

void
channels_list_apply_filter (FreetuxTVApp *app)
{
	int count = 0;
	GList* tmp;

	GtkWidget *entryfilter;
	gchar *filter;
	entryfilter = glade_xml_get_widget (app->windowmain,
					    "windowmain_entryfilter");
	filter = (gchar *)gtk_entry_get_text (GTK_ENTRY(entryfilter));

	tmp = g_list_first (gtk_container_get_children (GTK_CONTAINER(app->channelsgroups)));
	while (tmp != NULL){
		FreetuxTVChannelsGroup *channels_group;
		channels_group = FREETUXTV_CHANNELS_GROUP (tmp->data);
		count += freetuxtv_channels_group_set_filter (channels_group, 
							      filter);
		tmp = g_list_next (tmp); 
	}
}

void
channels_list_update_from_db (FreetuxTVApp *app)
{
	struct sqlite3 *db;
	int res;
	char *err=0;
	gint ret = 0;
	gchar *query;

	gchar *err_msg = NULL;

	gchar *user_db;
	user_db = g_strconcat(g_get_user_config_dir(), 
			      "/FreetuxTV/freetuxtv.db", NULL);
	
	if(app->channelsgroups != NULL){
		gtk_widget_destroy(GTK_WIDGET(app->channelsgroups));
		app->channelsgroups = NULL;
	}
	
	app->channelsgroups = GTK_VBOX(gtk_vbox_new(FALSE, 0));
	
	gtk_container_add (GTK_CONTAINER(channels_list_get_channelsgroups_parent (app)), 
			   GTK_WIDGET(app->channelsgroups));
	
	/* Ouverture de la BDD */
	res = sqlite3_open(user_db,&db);
	if(res != SQLITE_OK){
		err_msg = g_strdup_printf("L'affichage de la liste des chaînes a échoué.\n\nSQLite a retouné l'erreur :\n%s.",
					  sqlite3_errmsg(db));
		windowmain_show_error (app, err_msg);
		g_free(err_msg);
		
		ret = -1;
	}
	
	if(ret == 0){
		/* Selection des groupes de chaînes */
		query = "SELECT id_channelsgroup, name_channelsgroup, \
                         uri_channelsgroup				      \
                         FROM channels_group";
		res=sqlite3_exec(db, query, on_exec_add_channels_group,
				 (void *)app, &err);
		if(res != SQLITE_OK){
			err_msg = g_strdup_printf("L'affichage de la liste des chaînes a échoué.\n\nSQLite a retouné l'erreur :\n%s.",
						  sqlite3_errmsg(db));
			windowmain_show_error (app, err_msg);
			g_free(err_msg);

			ret = -1;
		}
	}
	
	sqlite3_free(err);
	sqlite3_close(db);

	g_free(user_db);
	
	if(ret == 0){
		gtk_widget_show_all (GTK_WIDGET(app->channelsgroups));
	}
}

void
channels_list_refresh_group (FreetuxTVApp *app,
			     FreetuxTVChannelsGroup *channels_group)
{
	channels_group_refresh_group (channels_group, (gpointer)app);
}

void
on_channel_dbl_clicked (FreetuxTVChannel *channel, gpointer *data)
{
	FreetuxTVApp *app = (FreetuxTVApp *) data;
	
	GtkWidget *statusbar;
	gchar *text;
	
	g_print ("FreetuxTV : launching channel \"%s\" -> %s\n",
		 channel->name, channel->uri);
	
	statusbar = glade_xml_get_widget (app->windowmain,
					  "windowmain_statusbar");
	
	gtk_statusbar_pop (GTK_STATUSBAR(statusbar),
			   gtk_statusbar_get_context_id(GTK_STATUSBAR(statusbar), 
							"PlayChannelMsg"));
	text = g_strconcat ("En cours de lecture : ", channel->name, NULL);
	gtk_statusbar_push (GTK_STATUSBAR(statusbar),
			    gtk_statusbar_get_context_id(GTK_STATUSBAR(statusbar), 
							 "PlayChannelMsg"), 
			    text);
	g_free(text);
	
	freetuxtv_player_play (app->player, channel->uri);
}


static void
channels_group_reload_channels (FreetuxTVChannelsGroup *self,
				gpointer data)
{
	FreetuxTVApp *app = (FreetuxTVApp *) data;

	struct sqlite3 *db;
	int res;
	char *err=0;
	gint ret = 0;
	gchar *query;
	
	gchar *err_msg = NULL;

	gchar *user_db;
	user_db = g_strconcat(g_get_user_config_dir(), 
			      "/FreetuxTV/freetuxtv.db", NULL);
	
	/* Ouverture de la BDD */
	res = sqlite3_open(user_db,&db);
	if(res != SQLITE_OK){		
		err_msg = g_strdup_printf("L'affichage des chaînes du groupe \"%s\" a échoué.\n\nSQLite a retouné l'erreur :\n%s.",
					  self->name,
					  sqlite3_errmsg(db));
		windowmain_show_error (app, err_msg);
		g_free(err_msg);
		
		ret = -1;
	}

	if(ret == 0){
		freetuxtv_channels_group_delete_channels (self);
	
		/* Selection des channels */
		CBUserData *cbuserdata;
		cbuserdata = g_new0(CBUserData, 1);
		cbuserdata->app = app;
		cbuserdata->channelsgroup = self;
		
		query = sqlite3_mprintf("SELECT id_channel, name_channel, \
                                 filename_channellogo,			\
                                 uri_channel FROM channel LEFT JOIN channel_logo \
                                 ON id_channellogo=idchannellogo_channel \
                                 WHERE channelsgroup_channel=%s",
				self->id);
		res = sqlite3_exec(db, query, on_exec_add_channel,
				   (void *)cbuserdata, &err);
		g_free(query);
		
		if(res != SQLITE_OK){
			err_msg = g_strdup_printf("L'affichage des chaînes du groupe \"%s\" a échoué.\n\nSQLite a retouné l'erreur :\n%s.",
						  self->name,
						  sqlite3_errmsg(db));
			windowmain_show_error (app, err_msg);
			g_free(err_msg);
			
			ret = -1;
		}

	}
		
	if(ret == 0){
		/*
		  freetuxtv_channels_group_set_collapsed (self, FREETUXTV_CHANNELS_GROUP_EXPANDED);*/
		
		self->state = FREETUXTV_CHANNELS_GROUP_EXPANDED;
		gtk_arrow_set (GTK_ARROW(self->arrow), 
			       GTK_ARROW_DOWN,
			       GTK_SHADOW_NONE);
		
		gtk_widget_show_all (GTK_WIDGET(self->channels_widget));
	}
	
	sqlite3_free(err);
	sqlite3_close(db);
	g_free(user_db);
	
}
static void
channels_group_delete_channels (FreetuxTVChannelsGroup *self,
				gpointer user_data)
{
	FreetuxTVApp *app = (FreetuxTVApp *) user_data;
	
	struct sqlite3 *db;
	int res;
	char *err = 0;
	gint ret = 0;
	gchar *query;

	gchar *err_msg = NULL;
	
	gchar *user_db;
	user_db = g_strconcat(g_get_user_config_dir(), 
			      "/FreetuxTV/freetuxtv.db", NULL);

	/* Ouverture de la BDD */
	res = sqlite3_open(user_db, &db);
	if(res != SQLITE_OK){
		err_msg = g_strdup_printf("Impossible d'ouvrir la base de données.\n\nSQLite a retouné l'erreur :\n%s.",
					  sqlite3_errmsg(db));
		windowmain_show_error (app, err_msg);
		g_free(err_msg);

		ret = -1;
	}
	
	if(ret == 0){
		/* Effacement des chaînes du groupe */
		query = sqlite3_mprintf("DELETE FROM channel WHERE channelsgroup_channel=%q",
					self->id);
		res = sqlite3_exec(db, query, NULL, 0, &err);
		sqlite3_free(query);
		if(res != SQLITE_OK){

			err_msg = g_strdup_printf("La suppression des chaînes du groupe \"%s\" a échoué.\n\nSQLite a retouné l'erreur :\n%s.",
						  self->name,
						  sqlite3_errmsg(db));
			windowmain_show_error (app, err_msg);
			g_free(err_msg);
			
			ret = -1;
		}
	}
	
	if(ret == 0){
		/* Effacement du widget */
		freetuxtv_channels_group_delete_channels (self);
	}

	sqlite3_free(err);
	sqlite3_close(db);
	
	g_free(user_db);	
	
}

static void
channels_group_delete_group (FreetuxTVChannelsGroup *self,
			     gpointer user_data)
{
	FreetuxTVApp *app = (FreetuxTVApp *) user_data;
	
	struct sqlite3 *db;
	int res;
	char *err = 0;
	gint ret = 0;
	gchar *query;

	gchar *err_msg = NULL;
	
	gchar *user_db;
	user_db = g_strconcat(g_get_user_config_dir(), 
			      "/FreetuxTV/freetuxtv.db", NULL);

	/* Ouverture de la BDD */
	res = sqlite3_open(user_db, &db);
	if(res != SQLITE_OK){
		err_msg = g_strdup_printf("Impossible d'ouvrir la base de données.\n\nSQLite a retouné l'erreur :\n%s.",
					  sqlite3_errmsg(db));
		windowmain_show_error (app, err_msg);
		g_free(err_msg);

		ret = -1;
	}
	
	if(ret == 0){
		/* Effacement du groupe */
		query = sqlite3_mprintf("DELETE FROM channels_group WHERE id_channelsgroup=%q",
					self->id);
		res = sqlite3_exec(db, query, NULL, 0, &err);
		sqlite3_free(query);
		if(res != SQLITE_OK){

			err_msg = g_strdup_printf("La suppression du groupe \"%s\" a échoué.\n\nSQLite a retouné l'erreur :\n%s.",
						  self->name,
						  sqlite3_errmsg(db));
			windowmain_show_error (app, err_msg);
			g_free(err_msg);
			
			ret = -1;
		}
	}
	
	if(ret == 0){
		/* Effacement du widget */
		freetuxtv_channels_group_delete_channels (self);	
		gtk_widget_destroy (GTK_WIDGET(self));
	}

	sqlite3_free(err);
	sqlite3_close(db);
	
	g_free(user_db);	
	
}

static void
channels_group_refresh_group (FreetuxTVChannelsGroup *self,
			      gpointer data)
{
	FreetuxTVApp *app = (FreetuxTVApp *) data;

	struct sqlite3 *db;
	int res;
	char *err = 0;
	gint ret = 0;

	gchar *err_msg = NULL;

	gchar *user_db;
	user_db = g_strconcat(g_get_user_config_dir(), 
			      "/FreetuxTV/freetuxtv.db", NULL);

	/* Mise à jour de la barre de statut */
	GtkWidget *statusbar;
	gchar *text;
	statusbar = glade_xml_get_widget (app->windowmain,
					  "windowmain_statusbar");
	text = g_strconcat ("Mise à jour des chaines de \"", self->name,"\"", NULL);
	gtk_statusbar_push (GTK_STATUSBAR(statusbar), 
			    gtk_statusbar_get_context_id(GTK_STATUSBAR(statusbar),
							 "UpdateMsg"),
			    text);
	g_main_iteration(FALSE);
	g_free(text);
	
	/* Ouverture de la BDD */
	res = sqlite3_open(user_db, &db);
	if(res != SQLITE_OK){
		err_msg = g_strdup_printf("Impossible d'ouvrir la base de données.\n\nSQLite a retouné l'erreur :\n%s.",
					  sqlite3_errmsg(db));
		windowmain_show_error (app, err_msg);
		g_free(err_msg);

		ret = -1;
	}
	
	char *file;
	if(ret == 0){
		res = channels_group_get_file (self, app, &file, TRUE);
		if (res < 0 || file == NULL ){
			ret = -1;
		}
	}

	if (ret == 0){

		/* Effacement des chaînes du groupe */
		gchar *query;
		query = sqlite3_mprintf("DELETE FROM channel WHERE channelsgroup_channel=%q",
					self->id);
		res = sqlite3_exec(db, query, NULL, 0, &err);
		sqlite3_free(query);
		if(res != SQLITE_OK){

			err_msg = g_strdup_printf("La suppression des chaînes du groupe \"%s\" a échoué.\n\nSQLite a retouné l'erreur : %s.",
						  self->name,
						  sqlite3_errmsg(db));
			windowmain_show_error (app, err_msg);
			g_free(err_msg);
			
			ret = -1;
		}
	}

	if (ret == 0){
		/* Parse le fichier M3U et ajoute les chaînes dans la BDD */
		Parsem3uData *data;
		data = g_new0(Parsem3uData, 1);
		data->id = self->id;
		data->db = db;
		res = libm3uparser_parse(file, &on_parsem3u_add_channel, data);
		if (res < 0 ){
			
			if (res == LIBM3UPARSER_CALLBACK_RETURN_ERROR){
				
				err_msg = g_strdup_printf("L'ajout des chaînes n'a pas correctement fonctionné.\n\nSQLite a retouné l'erreur : %s.",
							  sqlite3_errmsg(db));
				windowmain_show_error (app, err_msg);
				g_free(err_msg);
				ret = -1;
			}else{

				err_msg = g_strdup_printf("L'ajout des chaînes n'a pas correctement fonctionné.\n\nM3UParser a retouné l'erreur : %s.",
							  libm3uparser_errmsg(ret));
				windowmain_show_error (app, err_msg);
				g_free(err_msg);
				ret = -1;
			}			
		}
		g_free (data);
	}
	
	sqlite3_free(err);
	sqlite3_close(db);
	
	g_free(user_db);

	if (ret == 0){
		channels_group_reload_channels (self, app);
		gtk_statusbar_pop (GTK_STATUSBAR(statusbar),
				   gtk_statusbar_get_context_id(GTK_STATUSBAR(statusbar), 
								"UpdateMsg"));
	}
}



static GtkWidget*
channels_list_get_channelsgroups_parent (FreetuxTVApp *app)
{
	GtkWidget *widget;
        
        widget = glade_xml_get_widget (app->windowmain,
				       "windowmain_scrolledwindowchannels");
	g_return_if_fail (GTK_IS_SCROLLED_WINDOW(widget));
        widget = gtk_bin_get_child (GTK_BIN(widget));
        widget = gtk_bin_get_child (GTK_BIN(widget));
	return widget;
}

static int
channels_group_get_file (FreetuxTVChannelsGroup *self, FreetuxTVApp *app, gchar **file, gboolean cache)
{
	gchar **uriv;
	int ret = 0;

	gchar *err_msg = NULL;

	uriv = g_strsplit (self->uri, "//", 2);
	*file = NULL;
	if( g_ascii_strcasecmp (uriv[0], "file:") == 0 ){
		*file = g_strdup (uriv[1]);
	}
	if( g_ascii_strcasecmp (uriv[0], "http:") == 0 ){
		
		GtkWidget *windowmain;
		windowmain = glade_xml_get_widget (app->windowmain,
						   "windowmain");

		/* Mise à jour de la barre de statut */				
		GtkWidget *statusbar;
		gchar *text;
		statusbar = glade_xml_get_widget (app->windowmain,
						  "windowmain_statusbar");
		text = g_strconcat ("Récupération du fichier : \"",
				    self->uri,"\"", NULL);
		gtk_statusbar_push (GTK_STATUSBAR(statusbar), 
				    gtk_statusbar_get_context_id(GTK_STATUSBAR(statusbar), 
								 "UpdateMsg"),
				    text);
		g_main_iteration(FALSE);
		g_free(text);

		gchar *groupfile;
		groupfile = g_strconcat(g_get_user_config_dir(), 
					"/FreetuxTV/cache/playlist-group-",
					self->id, ".dat", NULL);;
			
		if(cache){
			/* Téléchargement du fichier */
			CURL *session = curl_easy_init(); 
			curl_easy_setopt(session, CURLOPT_URL, self->uri);
			curl_easy_setopt(session, CURLOPT_TIMEOUT, 10);
			FILE * fp = fopen(groupfile, "w"); 
			curl_easy_setopt(session,
					 CURLOPT_WRITEDATA, fp); 
			curl_easy_setopt(session,
					 CURLOPT_WRITEFUNCTION, fwrite);
			CURLcode curlcode;
			curlcode = curl_easy_perform(session);
			fclose(fp);
			curl_easy_cleanup(session);

			if(curlcode != 0){

				err_msg = g_strdup_printf("La playlist du groupe \"%s\" n'as pas pu être télécharger à partir de l'URL -> %s.\n\nCURL a retourné l'erreur : %s.",
							  self->name,
							  self->uri,
							  curl_easy_strerror(curlcode));
				windowmain_show_error (app, err_msg);
				g_free(err_msg);
				ret = -1;
			}

		}

		*file = groupfile;
		
		gtk_statusbar_pop (GTK_STATUSBAR(statusbar),
				   gtk_statusbar_get_context_id(GTK_STATUSBAR(statusbar), 
								"UpdateMsg"));
	}
	

	g_strfreev (uriv);
	
	return ret;
}


static int 
on_exec_add_channels_group (void *data, int argc, char **argv, char **colsname)
{
	FreetuxTVApp *app = (FreetuxTVApp *) data;

	FreetuxTVChannelsGroup *channels_group;
	channels_group = FREETUXTV_CHANNELS_GROUP (freetuxtv_channels_group_new (argv[0], argv[1], argv[2]));

	channels_list_add_channels_group (app, channels_group);
	channels_group_reload_channels (channels_group, app);
	
	return 0;
}

static int 
on_exec_add_channel (void *data, int argc, char **argv, char **colsname)
{
	CBUserData *cbuserdata = (CBUserData *) data;	

	FreetuxTVChannel *channel;
	channel = FREETUXTV_CHANNEL(freetuxtv_channel_new(argv[0],
							  argv[1],
							  argv[3]));
	
	gchar *imgfile;
	gchar *user_img_channels_dir;
	user_img_channels_dir = g_strconcat(g_get_user_data_dir(), 
					    "/.freetuxtv/images/channels", NULL);
	if(argv[2] == NULL){
		imgfile = g_strconcat(user_img_channels_dir, "/_none.png", NULL);	
		if(!g_file_test(imgfile,G_FILE_TEST_EXISTS)){
			imgfile = g_strconcat(FREETUXTV_DIR "/images/channels/_none.png", NULL);	
		}
	}else{
		imgfile = g_strconcat(user_img_channels_dir,"/", argv[2], NULL);	
		if(!g_file_test(imgfile,G_FILE_TEST_EXISTS)){
			imgfile = g_strconcat(FREETUXTV_DIR "/images/channels/", argv[2], NULL);	
		}
	}
	
	freetuxtv_channel_set_logo (channel, imgfile);
	
	g_free(user_img_channels_dir);
	g_free(imgfile);
	
	freetuxtv_channels_group_add_channel (cbuserdata->channelsgroup, channel);
	
	g_signal_connect(G_OBJECT(channel),
			 "dbl-clicked",
			 G_CALLBACK(on_channel_dbl_clicked),
			 cbuserdata->app);

	return 0;
}

static int 
on_parsem3u_add_channel (char *url, int argc, 
			 char **argv, void *user_data)
{
	Parsem3uData *data = (Parsem3uData *) user_data;

	struct sqlite3 *db = data->db;
	gchar* query;
	int res;
	char *err=0;
	
	gchar *name;
	res = libm3uparser_get_extinfo (argc, argv, NULL, &name);
	if(res == LIBM3UPARSER_EXTINFO_NOT_FOUND){
		name = g_strconcat("Inconnu", NULL);
	}else{
		/* Enleve le numero de chaine s'il est present */
		gchar *tmp = name;
		name = g_strrstr(tmp, " - ");
		if(name != NULL){
			name = g_strdup (name+3);
			g_free (tmp);
		}else{
			name = tmp;
		}
	}
	query = sqlite3_mprintf("INSERT INTO channel (name_channel, idchannellogo_channel, uri_channel, channelsgroup_channel) values ('%q',(SELECT id_channellogo FROM channel_logo WHERE label_channellogo='%q' OR id_channellogo = (SELECT idchannellogo_labelchannellogo FROM label_channellogo WHERE label_labelchannellogo='%q')),'%q','%q');", 
				name, name, name, url, data->id);
	g_free(name);
	res=sqlite3_exec(db, query, NULL, 0, &err);
	sqlite3_free(query);
	if(res != SQLITE_OK){
		return -1;
	}
	return 0;
}
