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
#include "freetuxtv-i18n.h"
#include "freetuxtv-channels-list.h"
#include "freetuxtv-channels-group.h"
#include "lib-m3uparser.h"

typedef struct _Parsem3uData
{
	FreetuxTVApp *app;
	gchar *id;
	struct sqlite3 *db;
	gchar *bregex;
	gchar *eregex;	
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
static void
channels_group_get_regexp (FreetuxTVChannelsGroup *self,
			   gchar *bregexp, gchar *eregexp);

static GtkWidget*
channels_list_get_channelsgroups_parent (FreetuxTVApp *app);

static int 
on_exec_add_channels_group (void *data, int argc, char **argv, char **colsname);

static int 
on_exec_add_channel (void *data, int argc, char **argv, char **colsname);

static int 
on_exec_get_group_infos (void *data, int argc, char **argv, char **colsname);

static int 
on_parsem3u_add_channel (char *url, int num, int argc, 
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
		err_msg = g_strdup_printf(_("Error when displaying the channels.\n\nSQLite has returned error :\n%s."),
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
			err_msg = g_strdup_printf(_("Error when displaying the channels.\n\nSQLite has returned error :\n%s."),
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
		gtk_widget_show (GTK_WIDGET(app->channelsgroups));
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
	
	gchar *text;
	
	g_print ("FreetuxTV : launching channel \"%s\" -> %s\n",
		 channel->name, channel->uri);
	text = g_strdup_printf (_("Playing : %s"), channel->name);
	windowmain_statusbar_push (app, "PlayChannelMsg", text);
	g_free(text);
	
	app->current.channel = channel;

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
		err_msg = g_strdup_printf(_("Error when displaying the channels of the group \"%s\".\n\nSQLite has returned error :\n%s."),
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
                                 WHERE channelsgroup_channel=%s\
                                 ORDER BY order_channel",
				self->id);
		res = sqlite3_exec(db, query, on_exec_add_channel,
				   (void *)cbuserdata, &err);
		sqlite3_free(query);
		
		if(res != SQLITE_OK){
			err_msg = g_strdup_printf(_("Error when displaying the channels of the group \"%s\".\n\nSQLite has returned error :\n%s."),
						  self->name,
						  sqlite3_errmsg(db));
			windowmain_show_error (app, err_msg);
			g_free(err_msg);
			
			ret = -1;
		}

	}
		
	if(ret == 0){
		freetuxtv_channels_group_set_collapsed (self, FREETUXTV_CHANNELS_GROUP_EXPANDED);
		/*
		self->state = FREETUXTV_CHANNELS_GROUP_EXPANDED;
		gtk_arrow_set (GTK_ARROW(self->arrow), 
			       GTK_ARROW_DOWN,
			       GTK_SHADOW_NONE);*/
		
		gtk_widget_show_all (GTK_WIDGET(self->channels));
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
		err_msg = g_strdup_printf(_("Cannot open database.\n\nSQLite has returned error :\n%s."),
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

			err_msg = g_strdup_printf(_("Error when deleting the channels of the group \"%s\".\n\nSQLite has returned error :\n%s."),
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
		err_msg = g_strdup_printf(_("Cannot open database.\n\nSQLite has returned error :\n%s."),
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

			err_msg = g_strdup_printf(_("Error when deleting the group \"%s\".\n\nSQLite has returned error :\n%s."),
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
	gchar *query;

	gchar *err_msg = NULL;

	gchar *user_db;

	user_db = g_strconcat(g_get_user_config_dir(), 
			      "/FreetuxTV/freetuxtv.db", NULL);

	/* Mise à jour de la barre de statut */
	gchar *text;
	text = g_strdup_printf(_("Update \"%s\" channels list"), self->name);
	windowmain_statusbar_push (app, "UpdateMsg", text);
	g_free(text);
	g_print("FreetuxTV : Start updating \"%s\" channels list\n", self->name);

	/* Ouverture de la BDD */
	res = sqlite3_open(user_db, &db);
	if(res != SQLITE_OK){
		err_msg = g_strdup_printf(_("Cannot open database.\n\nSQLite has returned error :\n%s."),
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
		query = sqlite3_mprintf("DELETE FROM channel WHERE channelsgroup_channel=%q",
					self->id);
		res = sqlite3_exec(db, query, NULL, 0, &err);
		sqlite3_free(query);
		if(res != SQLITE_OK){

			err_msg = g_strdup_printf(_("Error when deleting the channels of the group \"%s\".\n\nSQLite has returned error :\n%s."),
						  self->name,
						  sqlite3_errmsg(db));
			windowmain_show_error (app, err_msg);
			g_free(err_msg);
			
			ret = -1;
		}
	}

	Parsem3uData *pdata;
	pdata = g_new0(Parsem3uData, 1);
	pdata->id = self->id;
	pdata->db = db;
	pdata->app = app;

	if (ret == 0){
		/* Recupère les informations du groupe */
		query = sqlite3_mprintf("SELECT bregex_channelsgroup, eregex_channelsgroup FROM channels_group WHERE id_channelsgroup=%q",
					self->id);
		res = sqlite3_exec(db, query, on_exec_get_group_infos,
				   (void *)pdata, &err);
		sqlite3_free(query);
		if(res != SQLITE_OK){
			err_msg = g_strdup_printf(_("Error when loading informations of the group \"%s\".\n\nSQLite has returned error :\n%s."),
						  self->name,
						  sqlite3_errmsg(db));
			windowmain_show_error (app, err_msg);
			g_free(err_msg);
			
			ret = -1;			
		}		
	}

	if (ret == 0){
		g_print("FreetuxTV : Parsing the file \"%s\"\n", file);
		/* Parse le fichier M3U et ajoute les chaînes dans la BDD */
		res = libm3uparser_parse(file, &on_parsem3u_add_channel, pdata);
		if (res < 0 ){
			
			if (res == LIBM3UPARSER_CALLBACK_RETURN_ERROR){

				err_msg = g_strdup_printf(_("Error when adding the channels.\n\nM3UParser has returned error :\n%s."),
							  libm3uparser_errmsg(ret));
				windowmain_show_error (app, err_msg);
				g_free(err_msg);
				ret = -1;

				
			}else{
				
				err_msg = g_strdup_printf(_("Error when adding the channels.\n\nM3UParser has returned error :\n%s."),
							  sqlite3_errmsg(db));
				windowmain_show_error (app, err_msg);
				g_free(err_msg);
				ret = -1;
			}			
		}
	}
	g_free (pdata);

	sqlite3_free(err);
	sqlite3_close(db);
	
	g_free(user_db);

	if (ret == 0){
		channels_group_reload_channels (self, app);
	}
	windowmain_statusbar_pop (app, "UpdateMsg");
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
		gchar *text;
		text = g_strdup_printf (_("Getting the file : \"%s\""), 
					self->uri);
		windowmain_statusbar_push (app, "UpdateMsg", text);
		g_free(text);
		g_print("FreetuxTV : Getting the file \"%s\"\n", self->uri);

		gchar *groupfile;
		groupfile = g_strconcat(g_get_user_config_dir(), 
					"/FreetuxTV/cache/playlist-group-",
					self->id, ".dat", NULL);
		
		if(cache){
			/* Téléchargement du fichier */
			CURL *session = curl_easy_init(); 
			curl_easy_setopt(session, CURLOPT_URL, self->uri);
			curl_easy_setopt(session, CURLOPT_TIMEOUT, 10);
			// curl_easy_setopt(session, CURLOPT_VERBOSE, 1);
			curl_easy_setopt(session, CURLOPT_FOLLOWLOCATION, 1);
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

				err_msg = g_strdup_printf(_("Error when downloading the playlist of the group \"%s\" from URL -> %s.\n\nCURL has returned error :\n%s."),
							  self->name,
							  self->uri,
							  curl_easy_strerror(curlcode));
				windowmain_show_error (app, err_msg);
				g_free(err_msg);
				ret = -1;
			}

		}

		*file = groupfile;
		
		windowmain_statusbar_pop (app, "UpdateMsg");
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

	// Lit eventuellement la chaine
	if(cbuserdata->app->current.lastchannelonstartup == TRUE
	   && cbuserdata->app->config.lastchannel != NULL){
		if(g_ascii_strcasecmp(argv[0], cbuserdata->app->config.lastchannel) == 0){
			on_channel_dbl_clicked (channel, (void*)cbuserdata->app);
		}
	}

	return 0;
}

static int 
on_exec_get_group_infos (void *data, int argc, char **argv, char **colsname)
{
	
	Parsem3uData *cbuserdata = (Parsem3uData *) data;	
	cbuserdata->bregex = g_strdup_printf("^%s", (argv[0]!=NULL?argv[0]:""));
	cbuserdata->eregex = g_strdup_printf("%s$", (argv[1]!=NULL?argv[1]:""));
	return 0;
}

static int 
on_parsem3u_add_channel (char *url, int num, int argc, 
			 char **argv, void *user_data)
{
	Parsem3uData *data = (Parsem3uData *) user_data;

	struct sqlite3 *db = data->db;
	gchar* query;
	int res;
	char *err=0;
	gchar *name;

	gchar *err_msg;
	

	res = libm3uparser_get_extinfo (argc, argv, NULL, &name);
	if(res == LIBM3UPARSER_EXTINFO_NOT_FOUND){
		name = g_strdup_printf("Inconnu");
	}else{
		GRegex* bregex;
		GRegex* eregex;
		gchar *tmp;
		
		bregex = g_regex_new (data->bregex, 0, 0, NULL);
		eregex = g_regex_new (data->eregex, 0, 0, NULL);

		tmp = g_regex_replace (bregex, name, -1, 0, "", 0, NULL);
		g_free(name);
		
		name = g_regex_replace (eregex, tmp, -1, 0, "", 0, NULL);
		g_free(tmp);		
		
		g_regex_unref (bregex);
		g_regex_unref (eregex);
		
	}
	query = sqlite3_mprintf("INSERT INTO channel (name_channel, order_channel, idchannellogo_channel, uri_channel, channelsgroup_channel) values ('%q',%d,(SELECT id_channellogo FROM channel_logo WHERE label_channellogo='%q' OR id_channellogo = (SELECT idchannellogo_labelchannellogo FROM label_channellogo WHERE label_labelchannellogo='%q')),'%q','%q');", 
				name, num, name, name, url, data->id);
	g_free(name);
	res=sqlite3_exec(db, query, NULL, 0, &err);
	sqlite3_free(query);
	if(res != SQLITE_OK){

		err_msg = g_strdup_printf(_("Error when adding the channel \"%s\".\n\nSQLite has returned error :\n%s."),
					  name, sqlite3_errmsg(db));
		windowmain_show_error (data->app, err_msg);
		g_free(err_msg);

		return -1;
	}
	return 0;
}
