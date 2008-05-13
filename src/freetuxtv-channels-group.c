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

#include <sqlite3.h>
#include <glib.h>
#include <gtk/gtk.h>
#include <glade/glade.h>
#include <curl/curl.h>

#include "freetuxtv-main-window.h"
#include "freetuxtv-channels-group.h"
#include "freetuxtv-channels-list.h"
#include "freetuxtv-channel.h"
#include "lib-m3uparser.h"

G_DEFINE_TYPE (FreetuxTVChannelsGroup, freetuxtv_channels_group, GTK_TYPE_VBOX);

extern GladeXML *gladexml;

static int 
on_exec_add_channel (void *data, int argc, char **argv, char **colsname);

typedef struct _Parsem3uData
{
	gchar *id;
	struct sqlite3 *db;
} Parsem3uData;

static int 
on_parsem3u_add_channel (char *url, int argc, 
			 char **argv, void *user_data);

static void
on_click_refresh (GtkWidget *widget, GdkEventButton *event, 
		gpointer user_data);

static void
on_click_arrow (GtkWidget *widget, GdkEventButton *event, 
		gpointer user_data);

static int
get_group_file (FreetuxTVChannelsGroup *self, gchar **filename, gboolean cache);

GtkWidget *
freetuxtv_channels_group_new (gchar *id, gchar *name, gchar *uri)
{
	FreetuxTVChannelsGroup *self = NULL;
	
	self = gtk_type_new (freetuxtv_channels_group_get_type ());
	gtk_box_set_homogeneous(GTK_BOX(self),FALSE);
	gtk_box_set_spacing(GTK_BOX(self),0);
	
	self->id = g_strdup(id);
	self->name = g_strdup(name);
	self->uri = g_strdup(uri);
 
	GdkColor color;
	color.pixel = 0;
	color.red   = 0xcf * 0x100;
	color.green = 0xe0 * 0x100;
	color.blue  = 0xf5 * 0x100;

	GtkWidget *eventbox = gtk_event_box_new ();
	gtk_widget_modify_bg(GTK_WIDGET(eventbox), GTK_STATE_NORMAL, &color);
	gtk_box_pack_start (GTK_BOX(self), eventbox, FALSE, FALSE, 0);

	GtkWidget *hbox = gtk_hbox_new (FALSE, 0);
	gtk_container_add (GTK_CONTAINER(eventbox), hbox);
	
	/* Fleche pour l'expand */
	eventbox = gtk_event_box_new ();
	gtk_widget_modify_bg(GTK_WIDGET(eventbox), GTK_STATE_NORMAL, &color);
	g_signal_connect(G_OBJECT(eventbox), "button-press-event",
			 G_CALLBACK(on_click_arrow), NULL);
	gtk_box_pack_start (GTK_BOX(hbox), eventbox, FALSE, FALSE, 0);	
	GtkWidget *arrow = gtk_arrow_new (GTK_ARROW_DOWN, GTK_SHADOW_IN);
	self->arrow = arrow;
	gtk_container_add (GTK_CONTAINER(eventbox), arrow);

	/* Nom du groupe */
	GtkWidget *label = gtk_label_new (self->name);
	gtk_misc_set_padding (GTK_MISC(label),3,3);
	gtk_misc_set_alignment (GTK_MISC(label),0,0.5);
	gtk_label_set_ellipsize (GTK_LABEL(label), PANGO_ELLIPSIZE_END);
	gtk_box_pack_start (GTK_BOX(hbox), label, TRUE, TRUE, 0);

	/* Bouton pour rafraichir la liste des chaines */
	eventbox = gtk_event_box_new ();
	gtk_widget_modify_bg(GTK_WIDGET(eventbox), GTK_STATE_NORMAL, &color);
	g_signal_connect(G_OBJECT(eventbox), "button-press-event",
			 G_CALLBACK(on_click_refresh), NULL);
	gtk_box_pack_start (GTK_BOX(hbox), eventbox, FALSE, FALSE, 0);	
	GtkWidget *image = gtk_image_new_from_stock (GTK_STOCK_REFRESH, GTK_ICON_SIZE_BUTTON);
	gtk_container_add (GTK_CONTAINER(eventbox), image);

	/* Liste des chaines du groupe */
	self->channels_widget = gtk_vbox_new(FALSE, 1);
	gtk_box_pack_start (GTK_BOX(self), 
			    self->channels_widget, FALSE, FALSE, 0);
	freetuxtv_channels_group_reload_channels (self);
	
	gtk_widget_show_all (GTK_WIDGET(self));
	
	return GTK_WIDGET(self);
}

void
freetuxtv_channels_group_add_channel (FreetuxTVChannelsGroup *self,
				      FreetuxTVChannel *channel)
{
	gtk_box_pack_start (GTK_BOX(self->channels_widget),
			    GTK_WIDGET(channel),
			    FALSE, FALSE, 0);
}

void
freetuxtv_channels_group_change_collasped (FreetuxTVChannelsGroup *self)
{
	if(self->collapsed == 'N') {
		freetuxtv_channels_group_set_collasped (self, 'Y');
	}else{
		freetuxtv_channels_group_set_collasped (self, 'N');	
	}	
}

void
freetuxtv_channels_group_set_collasped (FreetuxTVChannelsGroup *self,
					gchar mode)
{
	GtkWidget *entryfilter;
	gchar *filter;

	self->collapsed = mode;
	if(self->collapsed == 'N') {
		gtk_arrow_set (GTK_ARROW(self->arrow), 
			       GTK_ARROW_DOWN,
			       GTK_SHADOW_NONE);
		
	
		entryfilter = glade_xml_get_widget (gladexml, "entryfilter");
		filter = (gchar *)gtk_entry_get_text (GTK_ENTRY(entryfilter));

		freetuxtv_channels_group_apply_filter (self, filter);
	}else{
		gtk_arrow_set (GTK_ARROW(self->arrow), 
			       GTK_ARROW_RIGHT,
			       GTK_SHADOW_NONE);
		gtk_widget_hide_all (GTK_WIDGET(self->channels_widget));	
	}
}

int
freetuxtv_channels_group_apply_filter (FreetuxTVChannelsGroup *self, gchar *filter)
{
	int count = 0;
	GList* tmp;
	tmp = g_list_first (gtk_container_get_children (GTK_CONTAINER(self->channels_widget)));
	gtk_widget_show_all (GTK_WIDGET(self));
	while (tmp != NULL){
		FreetuxTVChannel *channel;
		channel = FREETUXTV_CHANNEL (tmp->data);
		count += freetuxtv_channel_apply_filter (channel, filter);
		tmp = g_list_next (tmp); 
	}
	if( count == 0 && g_ascii_strcasecmp(filter, "") != 0){
		self->collapsed = 'Y';
		gtk_arrow_set (GTK_ARROW(self->arrow), 
			       GTK_ARROW_RIGHT,
			       GTK_SHADOW_NONE);
		gtk_widget_hide_all (GTK_WIDGET(self));
	}else{
		self->collapsed = 'N';
		gtk_arrow_set (GTK_ARROW(self->arrow), 
			       GTK_ARROW_DOWN,
			       GTK_SHADOW_NONE);	
	}
	return count;
}

FreetuxTVChannelsGroup *
freetuxtv_channels_group_get_from_widget (GtkWidget *self)
{
	g_return_val_if_fail(self != NULL, NULL);
	g_return_val_if_fail(GTK_IS_WIDGET(self), NULL);

	if(FREETUXTV_IS_CHANNELS_GROUP(self)){
		return FREETUXTV_CHANNELS_GROUP(self);
	}else{
		return freetuxtv_channels_group_get_from_widget (gtk_widget_get_parent(self));
	}
}

int
freetuxtv_channels_group_reload_channels (FreetuxTVChannelsGroup *self)
{
	struct sqlite3 *db;
	int res;
	char *err=0;

	gchar *user_db;
	user_db = g_strconcat(g_get_user_config_dir(), 
			      "/FreetuxTV/freetuxtv.db", NULL);
	
	/* Ouverture de la BDD */
	res = sqlite3_open(user_db,&db);
	if(res != SQLITE_OK){
		g_printerr("Sqlite3 : %s\n",
			   sqlite3_errmsg(db));
		g_printerr("FreetuxTV : Cannot open database %s\n",
			   user_db);
		sqlite3_close(db);
		return -1;
	}
	
	gtk_widget_hide_all (GTK_WIDGET(self->channels_widget));
	gtk_widget_destroy(GTK_WIDGET(self->channels_widget));
	self->channels_widget = gtk_vbox_new(FALSE, 1);
	gtk_box_pack_start (GTK_BOX(self), 
			    self->channels_widget, FALSE, FALSE, 0);
	
	/* Selection des channels */
	gchar *query;
	query = g_strconcat("SELECT id_channel, name_channel, \
                             filename_channellogo, \
                             uri_channel FROM channel LEFT JOIN channel_logo \
                             ON  id_channellogo=idchannellogo_channel \
                             WHERE channelsgroup_channel=",
			    self->id, NULL);
	res = sqlite3_exec(db, query, on_exec_add_channel,
			   (void *)self, &err);
	g_free(query);
	if(res != SQLITE_OK){
		g_printerr("Sqlite3 : %s\n",
			   sqlite3_errmsg(db));
		g_printerr("FreetuxTV : Cannot read channels for group %s in %s\n",
			   self->name,user_db);
	}
	
	sqlite3_free(err);
	sqlite3_close(db);
	
	self->collapsed = 'N';
	gtk_arrow_set (GTK_ARROW(self->arrow), 
		       GTK_ARROW_DOWN,
		       GTK_SHADOW_NONE);
	
	gtk_widget_show_all (GTK_WIDGET(self->channels_widget));

	g_free(user_db);

	return 0;
}

int
freetuxtv_channels_group_update_from_uri (FreetuxTVChannelsGroup *self)
{
	struct sqlite3 *db;
	int res;
	char *err = 0;
	int ret = 0;

	gchar *user_db;
	user_db = g_strconcat(g_get_user_config_dir(), 
			      "/FreetuxTV/freetuxtv.db", NULL);

	/* Mise à jour de la barre de statut */
	GtkWidget *statusbar;
	gchar *text;
	statusbar = glade_xml_get_widget (gladexml, "statusbar");
	text = g_strconcat ("Mise à jour des chaines de \"", self->name,"\"", NULL);
	gtk_statusbar_push (GTK_STATUSBAR(statusbar), 
			    gtk_statusbar_get_context_id(GTK_STATUSBAR(statusbar),
							 "UpdateMsg"),
			    text);
	g_main_iteration(FALSE);
	g_free(text);

	
	GtkWidget *windowmain;
	windowmain = glade_xml_get_widget (gladexml, "windowmain");

	/* Ouverture de la BDD */
	res = sqlite3_open(user_db, &db);
	if(res != SQLITE_OK){
		GtkWidget* dialog;		
		dialog = gtk_message_dialog_new(GTK_WINDOW(windowmain),
						GTK_DIALOG_MODAL, 
						GTK_MESSAGE_ERROR,
						GTK_BUTTONS_OK,
						"Impossible d'ouvrir la base de données.\n\nSQLite a retouné l'erreur : %s.",
						sqlite3_errmsg(db)
						);
		
		gtk_dialog_run(GTK_DIALOG(dialog));
		gtk_widget_destroy(dialog);
		ret = -1;
	}
	
	char *file;
	if(ret >= 0){
		res = get_group_file (self, &file, TRUE);
		if (res < 0){
			ret = -1;
		}
	}

	if (file != NULL && ret >= 0){

		/* Effacement des channels */
		gchar *query;
		query = sqlite3_mprintf("DELETE FROM channel WHERE channelsgroup_channel=%q",
					self->id);
		res = sqlite3_exec(db, query, NULL, 0, &err);
		sqlite3_free(query);
		if(res != SQLITE_OK){
			GtkWidget* dialog;
			dialog = gtk_message_dialog_new(GTK_WINDOW(windowmain),
							GTK_DIALOG_MODAL, 
							GTK_MESSAGE_ERROR,
							GTK_BUTTONS_OK,
							"La suppression des chaînes du groupe \"%s\" a échoué.\n\nSQLite a retouné l'erreur : %s.",
							self->name,
							sqlite3_errmsg(db)
							);
			
			gtk_dialog_run(GTK_DIALOG(dialog));
			gtk_widget_destroy(dialog);
			ret = -1;
		}else{
			/* Parse le fichier M3U et ajoute les chaînes dans la BDD */
			Parsem3uData *data;
			data = g_new0(Parsem3uData, 1);
			data->id = self->id;
			data->db = db;
			res = libm3uparser_parse(file, &on_parsem3u_add_channel, data);
			if (res < 0 ){
				GtkWidget* dialog;
				if (ret == LIBM3UPARSER_CALLBACK_RETURN_ERROR){				
					dialog = gtk_message_dialog_new(GTK_WINDOW(windowmain),
									GTK_DIALOG_MODAL, 
									GTK_MESSAGE_ERROR,
									GTK_BUTTONS_OK,
									"L'ajout des chaînes n'a pas correctement fonctionné.\n\nSQLite a retouné l'erreur : %s.",
									sqlite3_errmsg(db)
									);
				}else{
					dialog = gtk_message_dialog_new(GTK_WINDOW(windowmain),
									GTK_DIALOG_MODAL, 
									GTK_MESSAGE_ERROR,
									GTK_BUTTONS_OK,
									"L'ajout des chaînes n'a pas correctement fonctionné.\n\nM3UParser a retouné l'erreur : %s.",
									libm3uparser_errmsg(ret)
									);
				}
				gtk_dialog_run(GTK_DIALOG(dialog));
				gtk_widget_destroy(dialog);
				ret = -1;
			}
			g_free (data);
		}
	}
	
	sqlite3_free(err);
	sqlite3_close(db);
	
	g_free(user_db);
	freetuxtv_channels_group_reload_channels (self);
	gtk_statusbar_pop (GTK_STATUSBAR(statusbar),
			   gtk_statusbar_get_context_id(GTK_STATUSBAR(statusbar), 
							"UpdateMsg"));
	return ret;
}

static void
on_click_refresh (GtkWidget *widget, GdkEventButton *event,
		  gpointer user_data)
{
	FreetuxTVChannelsGroup *self;
	self = freetuxtv_channels_group_get_from_widget (GTK_WIDGET(widget));
	freetuxtv_channels_group_update_from_uri (self);
}

static void
on_click_arrow (GtkWidget *widget, GdkEventButton *event,
		gpointer user_data)
{
	FreetuxTVChannelsGroup *self;
	self = freetuxtv_channels_group_get_from_widget (GTK_WIDGET(widget));
	freetuxtv_channels_group_change_collasped (self);
}

static int 
on_exec_add_channel (void *data, int argc, char **argv, char **colsname)
{
	FreetuxTVChannelsGroup *self = (FreetuxTVChannelsGroup *) data;
	FreetuxTVChannel *channel;
	channel = FREETUXTV_CHANNEL(freetuxtv_channel_new(argv[1],
							  argv[3]));
	
	freetuxtv_channel_set_logo (channel, (gchar *)argv[2]);
	
	freetuxtv_channels_group_add_channel (self, channel);
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

static int
get_group_file (FreetuxTVChannelsGroup *self, gchar **file, gboolean cache)
{
	gchar **uriv;
	int ret = 0;
	uriv = g_strsplit (self->uri, "//", 2);
	*file = NULL;
	if( g_ascii_strcasecmp (uriv[0], "file:") == 0 ){
		*file = g_strdup (uriv[1]);
	}
	if( g_ascii_strcasecmp (uriv[0], "http:") == 0 ){

		GtkWidget *windowmain;
		windowmain = glade_xml_get_widget (gladexml, "windowmain");

		/* Mise à jour de la barre de statut */				
		GtkWidget *statusbar;
		gchar *text;
		statusbar = glade_xml_get_widget (gladexml, "statusbar");
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
				GtkWidget* dialog;
					
				dialog = gtk_message_dialog_new(GTK_WINDOW(windowmain),
								GTK_DIALOG_MODAL, 
								GTK_MESSAGE_ERROR,
								GTK_BUTTONS_OK,
								"La playlist du groupe \"%s\" n'as pas pu être télécharger à partir de l'URL -> %s.\n\nCURL a retourné l'erreur : %s.",
								self->name,
								self->uri,
								curl_easy_strerror(curlcode));
				gtk_dialog_run(GTK_DIALOG(dialog));
				gtk_widget_destroy(dialog);
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

static void
freetuxtv_channels_group_init (FreetuxTVChannelsGroup *object)
{
	object->id=0;
	object->name="";
	object->uri="";
	object->channels_widget = NULL;
	
	object->collapsed = 'N';
	object->arrow = NULL;
}

static void
freetuxtv_channels_group_finalize (GObject *object)
{
	
}

static void
freetuxtv_channels_group_class_init (FreetuxTVChannelsGroupClass *klass)
{
	GObjectClass *gobject_class = G_OBJECT_CLASS (klass);
	gobject_class->finalize = freetuxtv_channels_group_finalize;
}
