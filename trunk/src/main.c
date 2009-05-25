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

#include <glib/gstdio.h>

#include <stdlib.h>
#include <gtk/gtk.h>
#include <glade/glade.h>
#include <sqlite3.h>
#include <libnotify/notification.h>

#include "lib-gmmkeys.h"
#include "freetuxtv-app.h"
#include "freetuxtv-i18n.h"
#include "freetuxtv-window-main.h"
#include "freetuxtv-channels-list.h"
#include "freetuxtv-logos-list.h"
#include "freetuxtv-player.h"

int
init_app()
{

	struct sqlite3 *db;
	int res;
	char *err=0;
	
	gchar *user_cache_dir;
	user_cache_dir = g_strconcat(g_get_user_config_dir(), 
				     "/FreetuxTV/cache", NULL);

	gchar *user_img_channels_dir;
	user_img_channels_dir = g_strconcat(g_get_user_config_dir(), 
					    "/FreetuxTV/images/channels", NULL);
	gchar *user_db;
	user_db = g_strconcat(g_get_user_config_dir(), 
			      "/FreetuxTV/freetuxtv.db", NULL);
	
	/* Verifie si le repertoire des images utilisateur existe */
	if (! g_file_test (user_cache_dir, 
			   G_FILE_TEST_EXISTS)){
		res = g_mkdir_with_parents (user_cache_dir,
					    0744);
		if(res == 0){
			g_printerr ("FreetuxTV : Directory created : %s\n",
				    user_cache_dir);	
		}else{
			g_printerr ("FreetuxTV : Cannot create directory : %s\n",
				    user_cache_dir);
			return -1;
		}
	}
	if (! g_file_test (user_cache_dir,
			   G_FILE_TEST_IS_DIR)){
		g_printerr ("FreetuxTV : Not a directory : %s\n",
			    user_cache_dir);
		return -1;
	}

	/* Verifie si le repertoire des images utilisateur existe */
	if (! g_file_test (user_img_channels_dir, 
			   G_FILE_TEST_EXISTS)){
		res = g_mkdir_with_parents (user_img_channels_dir,
					    0744);
		if(res == 0){
			g_printerr ("FreetuxTV : Directory created : %s\n",
				    user_img_channels_dir);	
		}else{
			g_printerr ("FreetuxTV : Cannot create directory : %s\n",
				    user_img_channels_dir);
			return -1;
		}
	}
	if (! g_file_test (user_img_channels_dir,
			   G_FILE_TEST_IS_DIR)){
		g_printerr ("FreetuxTV : Not a directory : %s\n",
			    user_img_channels_dir);
		return -1;
	}
	
	/* Creation de la BDD si inexistante */
	if (! g_file_test (user_db, G_FILE_TEST_IS_REGULAR)){
		
		res = sqlite3_open(user_db, &db);
		if(res != SQLITE_OK){
			g_printerr ("Sqlite3 : %s\n",
				    sqlite3_errmsg(db));
			g_printerr ("FreetuxTV : Cannot open database %s\n",
				    user_db);
			sqlite3_close(db);
			return -1;
		}
		
		/* Lecture du fichier contenant les requetes de création de la base */
		gchar* filename;
		filename = FREETUXTV_DIR "/sqlite3-create-tables.sql";
		gchar* sql_query;
		gsize filelen;
		
		if (g_file_get_contents (filename, &sql_query, &filelen, NULL)){
			
			res=sqlite3_exec(db, sql_query, NULL, 0, &err);
			if(res != SQLITE_OK){
				g_printerr("Sqlite3 : %s\n%s\n",
					   sqlite3_errmsg(db), sql_query);
				g_printerr("FreetuxTV : Cannot create tables in %s\n",
					   user_db);
				sqlite3_free(err);
			}
		}else{
			g_printerr("FreetuxTV : Cannot read file %s\n", filename);
		}
		
		
		g_free(sql_query);		
		sqlite3_close(db);
	}

	g_free(user_cache_dir);
	g_free(user_img_channels_dir);
	g_free(user_db);

	return 0;
}

static FreetuxTVApp *
freetuxtv_app_create_app ()
{
	FreetuxTVApp *app;

	GtkWidget *windowmain;
	GtkWidget *widget;

	GtkWidget *scrolledwindowchannels;	

	GtkWidget *eventboxplayer;

	app = g_new0 (FreetuxTVApp, 1);

	app->name = "FreetuxTV";


	/* Chargement de la configuration par defaut */
	app->config.windowminimode_stayontop = FALSE;
	app->config.windowminimode_width = 320;
	app->config.windowminimode_height = 240;
	app->config.channelonstartup = TRUE;
	app->config.lastchannel = -1;
	app->config.volume = 75.0;
	app->config.logosfiledate = 0;
	app->config.directoryrecord = g_strdup(g_get_home_dir());
	app->current.lastchannelonstartup = FALSE;
	app->debug = FALSE;

	/* Création de la fenêtre */
	app->windowmain = glade_xml_new (FREETUXTV_GLADEXML,
					 "windowmain", NULL);
	glade_xml_signal_autoconnect (app->windowmain);

	/* Ajout du widget du lecteur */
	eventboxplayer = glade_xml_get_widget (app->windowmain,
					       "windowmain_eventboxplayer");
	app->player = FREETUXTV_PLAYER(freetuxtv_player_new ());
	gtk_container_add (GTK_CONTAINER(eventboxplayer), GTK_WIDGET(app->player));
	
	/* Initialise la liste des chaines */
	channels_list_init (app);
	/* Connexion des signaux */
	widget = glade_xml_get_widget (app->windowmain,
				       "windowmain");
	
	widget = glade_xml_get_widget (app->windowmain,
				       "windowmain");
	g_signal_connect(G_OBJECT(widget),
			 "destroy",
			 G_CALLBACK(on_windowmain_destroy),
			 app);
	
	widget = glade_xml_get_widget (app->windowmain,
				       "windowmain_menuitempreferences");
	g_signal_connect(G_OBJECT(widget),
			 "activate",
			 G_CALLBACK(on_windowmain_menuitempreferences_activate),
			 app);

	widget = glade_xml_get_widget (app->windowmain,
				       "windowmain_menuitemquit");
	g_signal_connect(G_OBJECT(widget),
			 "activate",
			 G_CALLBACK(on_windowmain_menuitemquit_activate),
			 app);
	
	widget = glade_xml_get_widget (app->windowmain,
				       "windowmain_menuitemgroupsadd");
	g_signal_connect(G_OBJECT(widget),
			 "activate",
			 G_CALLBACK(on_windowmain_menuitemgroupsadd_activate),
			 app);
	
	widget = glade_xml_get_widget (app->windowmain,
				       "windowmain_menuitemupdatelogos");
	g_signal_connect(G_OBJECT(widget),
			 "activate",
			 G_CALLBACK(on_windowmain_menuitemupdatelogos_activate),
			 app);

	widget = glade_xml_get_widget (app->windowmain,
				       "windowmain_buttonclearfilter");
	g_signal_connect(G_OBJECT(widget),
			 "clicked",
			 G_CALLBACK(on_windowmain_buttonclearfilter_clicked),
			 app);

	widget = glade_xml_get_widget (app->windowmain,
				       "windowmain_entryfilter");
	g_signal_connect(G_OBJECT(widget),
			 "changed",
			 G_CALLBACK(on_windowmain_entryfilter_changed),
			 app);
	
	widget = glade_xml_get_widget (app->windowmain,
				       "windowmain_buttonprevious");
	g_signal_connect(G_OBJECT(widget),
			 "clicked",
			 G_CALLBACK(on_windowmain_buttonprevious_clicked),
			 app);
	
	widget = glade_xml_get_widget (app->windowmain,
				       "windowmain_buttonnext");
	g_signal_connect(G_OBJECT(widget),
			 "clicked",
			 G_CALLBACK(on_windowmain_buttonnext_clicked),
			 app);

	widget = glade_xml_get_widget (app->windowmain,
				       "windowmain_buttonplaypause");
	g_signal_connect(G_OBJECT(widget),
			 "clicked",
			 G_CALLBACK(on_windowmain_buttonplaypause_clicked),
			 app);

	widget = glade_xml_get_widget (app->windowmain,
				       "windowmain_buttonstop");
	g_signal_connect(G_OBJECT(widget),
			 "clicked",
			 G_CALLBACK(on_windowmain_buttonstop_clicked),
			 app);

	widget = glade_xml_get_widget (app->windowmain,
				       "windowmain_buttonrecord");
	g_signal_connect(G_OBJECT(widget),
			 "clicked",
			 G_CALLBACK(on_windowmain_buttonrecord_clicked),
			 app);
	
	widget = glade_xml_get_widget (app->windowmain,
				       "windowmain_volumecontrol");
	g_signal_connect(G_OBJECT(widget),
			 "value-changed",
			 G_CALLBACK(on_windowmain_volumecontrol_value_changed),
			 app);

	widget = glade_xml_get_widget (app->windowmain,
				       "windowmain_buttonfullscreen");
	g_signal_connect(G_OBJECT(widget),
			 "clicked",
			 G_CALLBACK(on_windowmain_buttonfullscreen_clicked),
			 app);

	widget = glade_xml_get_widget (app->windowmain,
				       "windowmain_buttonminimode");
	g_signal_connect(G_OBJECT(widget),
			 "clicked",
			 G_CALLBACK(on_windowmain_buttonminimode_clicked),
			 app);

	// Load FreetuxTV State	
	GKeyFile *keyfile;
	int i;
	gboolean b;
	gdouble d;
	gchar *str;
	char *filename;
	GError *err = NULL;

	filename = g_build_filename (g_get_user_config_dir(),
				     "FreetuxTV/config.ini", NULL);
	g_print("FreetuxTV : Loading config file %s\n", filename);
	keyfile = g_key_file_new ();
	if (g_key_file_load_from_file (keyfile, filename,
				       G_KEY_FILE_NONE, NULL) == FALSE) {
		g_printerr("FreetuxTV : Error when loading config file\n");
		g_free (filename);
	} else {
		g_free (filename);

		b = g_key_file_get_boolean (keyfile, "windowminimode",
					    "stay_on_top", &err);
		if (err != NULL) {
			g_error_free (err);
			err = NULL;
		}else{
			app->config.windowminimode_stayontop = b;		
		}

		i = g_key_file_get_integer (keyfile, "windowminimode",
					    "width", &err);
		if (err != NULL) {
			g_error_free (err);
			err = NULL;
		}else{
			app->config.windowminimode_width = i;
		}


		i = g_key_file_get_integer (keyfile, "windowminimode",
					    "height", &err);
		if (err != NULL) {
			g_error_free (err);
			err = NULL;
		}else{
			app->config.windowminimode_height = i;
		}

		
		d = g_key_file_get_double (keyfile, "general",
					   "volume", &err);
		if (err != NULL) {
			g_error_free (err);
			err = NULL;
		}else{
			app->config.volume = d;			
			widget = glade_xml_get_widget (app->windowmain,
						       "windowmain_volumecontrol");
			gtk_range_set_value (GTK_RANGE(widget), app->config.volume);
			freetuxtv_player_set_volume (app->player, app->config.volume);
		}
		
		b = g_key_file_get_boolean (keyfile, "general",
					    "channel_on_startup", &err);
		if (err != NULL) {
			g_error_free (err);
			err = NULL;
		}else{
			app->config.channelonstartup = b;		
		}		
		
		i = g_key_file_get_integer (keyfile, "general",
					    "last_channel", &err);
		if (err != NULL) {
			g_error_free (err);
			err = NULL;
		}else{
			app->config.lastchannel = i;		
		}
		
		i = g_key_file_get_integer (keyfile, "general",
					    "logos_file_date", &err);
		if (err != NULL) {
			g_error_free (err);
			err = NULL;
		}else{
			app->config.logosfiledate = i;		
		}		
		
		str = g_key_file_get_string (keyfile, "general",
					     "directory_record", &err);
		if (err != NULL) {
			g_error_free (err);
			err = NULL;
		}else{
			app->config.directoryrecord = str;		
		}

		g_key_file_free (keyfile);
	}

	return app;
	
}

void
freetuxtv_action_play_channel (FreetuxTVApp *app, FreetuxTVChannelInfos *channel_infos)
{
	gchar *text;
	if(!freetuxtv_player_is_recording(app->player)){
		text = g_strdup_printf (_("Playing : %s"), channel_infos->name);
		windowmain_statusbar_push (app, "PlayChannelMsg", text);
		
		app->current.channel = channel_infos;
		
		// Send notification to desktop
		gchar *imgfile;
		imgfile = logos_list_get_channel_logo_filename(app, channel_infos, TRUE);
		
		notify_notification_update (app->current.notification, channel_infos->name,
					    _("is playing"), imgfile);
		if (!notify_notification_show (app->current.notification, NULL)) {
			g_printerr("FreetuxTV : Failed to send notification\n");
		}
		g_free(imgfile);
		g_free(text);
		
		windowmain_display_buttons (app, WINDOW_MODE_PLAYING);
		
		freetuxtv_player_play (app->player, channel_infos);
	}
}

void
freetuxtv_action_playpause_channel (FreetuxTVApp *app)
{	
	if(app->current.channel != NULL){
		if(!freetuxtv_player_is_playing (app->player)){
			freetuxtv_action_play_channel (app, app->current.channel);
		}
	}
}

void
freetuxtv_action_stop_channel (FreetuxTVApp *app)
{
	gchar *text;
	if(app->current.channel != NULL){
		if(freetuxtv_player_is_playing(app->player)){

			gboolean recording;
			recording = freetuxtv_player_is_recording(app->player);
			
			text = g_strdup_printf (_("Stopping channel : %s"), app->current.channel->name);
			windowmain_statusbar_push (app, "PlayChannelMsg", text);
			g_free(text);
	
			windowmain_display_buttons (app, WINDOW_MODE_STOPPED);
			freetuxtv_player_stop (app->player);

			if(recording){
				freetuxtv_action_playpause_channel (app);				
			}
		}
	}
}

void
freetuxtv_action_record_channel (FreetuxTVApp *app)
{
	gchar* out_filename;
	gchar *text;

	if(freetuxtv_player_is_playing(app->player) &&
	   !freetuxtv_player_is_recording(app->player)){

		// Send notification to desktop
		gchar *imgfile;
		imgfile = logos_list_get_channel_logo_filename(app, app->current.channel, TRUE);
		
		notify_notification_update (app->current.notification, app->current.channel->name,
					    _("is recording"), imgfile);
		if (!notify_notification_show (app->current.notification, NULL)) {
			g_printerr("FreetuxTV : Failed to send notification\n");
		}
		g_free(imgfile);
		g_free(text);
	
		windowmain_display_buttons (app, WINDOW_MODE_RECORDING);

		freetuxtv_player_record_current (app->player, app->config.directoryrecord,
						 &out_filename);
	
		text = g_strdup_printf (_("Recording : %s"), out_filename);
		windowmain_statusbar_push (app, "PlayChannelMsg", text);
		g_free(text);
		g_free(out_filename);
	}
}

void
freetuxtv_action_prev_channel (FreetuxTVApp *app)
{	
	gboolean ret;
	FreetuxTVChannelInfos* prev_channel_infos;
	if (app->current.channel != NULL) {
		if(!freetuxtv_player_is_recording(app->player)){
			ret = channels_list_get_prev_channel (app, app->current.channel, &prev_channel_infos);
			if(ret){			
				freetuxtv_action_play_channel(app, prev_channel_infos);
			}
		}
	}
}

void
freetuxtv_action_next_channel (FreetuxTVApp *app)
{
	gboolean ret;
	FreetuxTVChannelInfos* next_channel_infos;
	if (app->current.channel != NULL) {
		if(!freetuxtv_player_is_recording(app->player)){
			ret = channels_list_get_next_channel (app, app->current.channel, &next_channel_infos);
			if(ret){			
				freetuxtv_action_play_channel(app, next_channel_infos);
			}
		}
	}
}

void
freetuxtv_action_quit (FreetuxTVApp *app)
{
	GKeyFile *keyfile;
	char *contents, *filename;
	
	// Save FreetuxTV state
	keyfile = g_key_file_new ();
	
	
	g_key_file_set_double (keyfile, "general",
			       "volume",
			       app->config.volume);
	g_key_file_set_boolean (keyfile, "general",
				"channel_on_startup",
				app->config.channelonstartup);
	if(app->current.channel != NULL){
		g_key_file_set_integer (keyfile, "general",
					"last_channel",
					app->current.channel->id);
	}
	
	g_key_file_set_integer (keyfile, "general",
				"logos_file_date",
				app->config.logosfiledate);
	
	g_key_file_set_string (keyfile, "general",
			       "directory_record",
			       app->config.directoryrecord);
	g_free(app->config.directoryrecord);

	g_key_file_set_boolean (keyfile, "windowminimode",
				"stay_on_top",
				app->config.windowminimode_stayontop);
	g_key_file_set_integer (keyfile, "windowminimode",
				"width",
				app->config.windowminimode_width);
	g_key_file_set_integer (keyfile, "windowminimode",
				"height",
				app->config.windowminimode_height);
	
	contents = g_key_file_to_data (keyfile, NULL, NULL);
	g_key_file_free (keyfile);
	filename = g_build_filename (g_get_user_config_dir(),
				     "FreetuxTV/config.ini", NULL);
	g_print("FreetuxTV : Writing config file %s\n", filename);
	if (!g_file_set_contents (filename, contents, -1, NULL)){
		g_printerr("FreetuxTV : Error when writing config file\n");
	}

	g_free (filename);
	g_free (contents);

	// Stop the current channel
	freetuxtv_action_stop_channel(app);

	gtk_main_quit();
}

static void
on_freetuxtv_mm_key_pressed (GMMKeys *mmkeys, GMMKeysButton button, FreetuxTVApp* app)
{
	switch(button){
	case GMMKEYS_BUTTON_PLAY :
	case GMMKEYS_BUTTON_PAUSE :
		freetuxtv_action_playpause_channel (app);		
		break;
	case GMMKEYS_BUTTON_STOP :
		freetuxtv_action_stop_channel (app);
		break;
	case GMMKEYS_BUTTON_PREV :
		freetuxtv_action_prev_channel (app);
		break;
	case GMMKEYS_BUTTON_NEXT :
		freetuxtv_action_next_channel (app);
		break;
	}
}

int main (int argc, char *argv[])
{
	
	FreetuxTVApp *app;

	GMMKeys* mmkeys;

	int p;

#ifdef ENABLE_NLS
	setlocale (LC_ALL, "");
	bindtextdomain (GETTEXT_PACKAGE, LOCALE_DIR);
	bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
	textdomain (GETTEXT_PACKAGE);
#endif

	g_print("FreetuxTV : Compliled with LibVLC version %d.%d\n", LIBVLC_VERSION_MAJOR, LIBVLC_VERSION_MINOR);
	init_app();
	
	gtk_init(&argc, &argv);
	
	app = freetuxtv_app_create_app ();
	if (app == NULL)
		return 1;
	// Traitement des arguments du programme
	for(p=1; p<argc; p++){
		if(g_ascii_strcasecmp("-v",argv[p]) == 0){
			g_print("FreetuxTV : Enable debug mode\n");
			app->debug = TRUE;
		}
	}
	
	// Initialize notifications
	notify_init("FreetuxTV");
	app->current.notification = notify_notification_new ("FreetuxTV", NULL, NULL, NULL);

	// Synchronize logos list if file modified
	struct stat file_stat;
	gint ret;
	if(g_stat (FREETUXTV_DIR "/channel_logos.xml", &file_stat) == 0){
		if(app->config.logosfiledate < (gint)file_stat.st_mtime){
			ret = logos_list_synchronize(app);
			if(ret == 0){
				app->config.logosfiledate = (gint)file_stat.st_mtime;	
			}
		}
	}

	// Initialise l'interface
	windowmain_display_buttons (app, WINDOW_MODE_STOPPED);

	// Regarde si on charge une chaine au demarrage
	if(app->config.channelonstartup == TRUE
	   && app->config.lastchannel != -1){
		app->current.lastchannelonstartup = TRUE;
	}
	channels_list_load_channels (app);
	app->current.lastchannelonstartup = FALSE;
	
	mmkeys = g_mmkeys_new ("FreetuxTV");
	g_mmkeys_activate (mmkeys);
	
	g_signal_connect(G_OBJECT(mmkeys),
			 "mm_key_pressed",
			 G_CALLBACK(on_freetuxtv_mm_key_pressed),
			 app);
	

	
	gtk_main();
	
	g_mmkeys_deactivate (mmkeys);
	g_object_unref(G_OBJECT(app->current.notification));
	notify_uninit();  

	return 0;
}
