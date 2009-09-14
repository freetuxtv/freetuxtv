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
#include <sqlite3.h>
#include <libnotify/notification.h>

#include "lib-gmmkeys.h"
#include "freetuxtv-app.h"
#include "freetuxtv-utils.h"
#include "freetuxtv-i18n.h"
#include "freetuxtv-window-main.h"
#include "freetuxtv-channels-list.h"
#include "freetuxtv-recordings-list.h"
#include "freetuxtv-logos-list.h"
#include "gtk-libvlc-media-player.h"

int
init_user_configuration()
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

void
load_user_configuration(FreetuxTVApp *app)
{
	GtkWidget *widget;

	GKeyFile *keyfile;
	int i;
	gboolean b;
	gdouble d;
	gchar *str;
	char *filename;
	GError *err = NULL;

	// Loading FreetuxTV State
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

		// Load prefs
		b = g_key_file_get_boolean (keyfile, "general",
					    "channel_on_startup", &err);
		if (err != NULL) {
			g_error_free (err);
			err = NULL;
		}else{
			app->prefs.channelonstartup = b;		
		}
		
		b = g_key_file_get_boolean (keyfile, "general",
					    "enable_notifications", &err);
		if (err != NULL) {
			g_error_free (err);
			err = NULL;
		}else{
			app->prefs.enable_notifications = b;		
		}
		
		str = g_key_file_get_string (keyfile, "general",
					     "directory_record", &err);
		if (err != NULL) {
			g_error_free (err);
			err = NULL;
		}else{
			app->prefs.directoryrecordings = str;		
		}

		i = g_key_file_get_integer (keyfile, "general",
					    "transcoding_mode", &err);
		if (err != NULL) {
			g_error_free (err);
			err = NULL;
		}else{
			if(i>0 & i<2){
				app->prefs.transcoding_mode = i;
			}
		}

		str = g_key_file_get_string (keyfile, "general",
					    "transcoding_format", &err);
		if (err != NULL) {
			g_error_free (err);
			err = NULL;
		}else{
			app->prefs.transcoding_format = str;				
		}

		// Load current config
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
			if(i>0){
				app->config.windowminimode_width = i;
			}
		}


		i = g_key_file_get_integer (keyfile, "windowminimode",
					    "height", &err);
		if (err != NULL) {
			g_error_free (err);
			err = NULL;
		}else{
			if(i>0){
				app->config.windowminimode_height = i;
			}
		}

		
		d = g_key_file_get_double (keyfile, "general",
					   "volume", &err);
		if (err != NULL) {
			g_error_free (err);
			err = NULL;
		}else{
			app->config.volume = d;			
			widget = (GtkWidget *)gtk_builder_get_object (app->gui,
								      "windowmain_volumecontrol");
			gtk_range_set_value (GTK_RANGE(widget), app->config.volume);
			gtk_libvlc_media_player_set_volume (app->player, app->config.volume);
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

		g_key_file_free (keyfile);
	}

	// Check if needs to load a channel on startup	
	app->current.lastchannelonstartup = FALSE;
	if(app->prefs.channelonstartup == TRUE
	   && app->config.lastchannel != -1){
		app->current.lastchannelonstartup = TRUE;
	}

}

static void
splashscreen_statusbar_push (FreetuxTVApp *app, gchar *msg)
{
	int context_id;

	GtkWidget *statusbar;
	statusbar = (GtkWidget *) gtk_builder_get_object (app->gui,
							  "splashscreen_statusbar");
	context_id = gtk_statusbar_get_context_id(GTK_STATUSBAR(statusbar), 
						  "Infos");
	gtk_statusbar_push (GTK_STATUSBAR(statusbar), 
			    context_id,
			    msg);
	while (g_main_context_iteration(NULL, FALSE)){}
}

void
splashscreen_statusbar_pop (FreetuxTVApp *app)
{	
	int context_id;

	GtkWidget *statusbar;
	statusbar = (GtkWidget *) gtk_builder_get_object (app->gui,
							  "splashscreen_statusbar");
	context_id = gtk_statusbar_get_context_id(GTK_STATUSBAR(statusbar), 
						  "Infos");
	gtk_statusbar_pop (GTK_STATUSBAR(statusbar),  context_id);		
}


static gboolean
splashscreen_app_init(gpointer data)
{
	FreetuxTVApp *app = (FreetuxTVApp *)data;
	
	GtkWidget *widget;
	widget = (GtkWidget *) gtk_builder_get_object (app->gui,
						       "splashscreen_version");
	gtk_label_set_text(GTK_LABEL(widget), VERSION);

	// Initializing user configuration
	splashscreen_statusbar_push (app, _("Initializing user configuration..."));
	init_user_configuration();
	splashscreen_statusbar_pop (app);

	// Loading user configuration form .ini file
	splashscreen_statusbar_push (app, _("Loading user configuration..."));
	load_user_configuration(app);
	splashscreen_statusbar_pop (app);	
	
	// Synchronizing the list of logos if file modified
	splashscreen_statusbar_push (app, _("Synchronizing the list of logos..."));
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
	splashscreen_statusbar_pop (app);

	// Initialise l'interface
	windowmain_display_buttons (app, WINDOW_MODE_STOPPED);
	
	// Loading the list of channels
	g_print("FreetuxTV : Loading the list of channels\n");
	splashscreen_statusbar_push (app, _("Loading the list of channels..."));
	channels_list_load_channels (app);
	splashscreen_statusbar_pop (app);

	// Loading the list of recordings
	g_print("FreetuxTV : Loading the list of recordings\n");
	splashscreen_statusbar_push (app, _("Loading the list of recordings..."));
	recordings_list_load_recordings (app);
	splashscreen_statusbar_pop (app);

	// Showing the main window
	g_print("FreetuxTV : Showing the main window\n");
	widget = (GtkWidget *)gtk_builder_get_object (app->gui,
						      "splashscreen");
	gtk_widget_hide(widget);
	
	widget = (GtkWidget *)gtk_builder_get_object (app->gui,
						      "windowmain");
	gtk_widget_show(widget);

	// Play the last channel if needed
	if(app->current.path_channel != NULL){
		freetuxtv_play_channel (app, app->current.path_channel);
	}
}

static FreetuxTVApp *
freetuxtv_app_create_app ()
{
	FreetuxTVApp *app;

	GtkWidget *widget;

	GtkWidget *scrolledwindowchannels;	

	GtkWidget *eventboxplayer;

	app = g_new0 (FreetuxTVApp, 1);

	app->name = "FreetuxTV";

	// Loads default configuration
	app->prefs.channelonstartup = TRUE;
	app->prefs.enable_notifications = TRUE;
	app->prefs.directoryrecordings = g_strdup(g_get_home_dir());
	app->prefs.transcoding_mode = 0;
	app->prefs.transcoding_format = "0";

	app->config.windowminimode_stayontop = FALSE;
	app->config.windowminimode_width = 320;
	app->config.windowminimode_height = 240;
	app->config.lastchannel = -1;
	app->config.volume = 0.75;
	app->config.logosfiledate = 0;
	app->current.path_channel = NULL;
	app->current.lastchannelonstartup = FALSE;
	app->current.is_recording = FALSE;
	app->current.recording.dst_file = NULL;
	app->current.recording.duration = NULL;
	app->debug = FALSE;	

	// Create UI from file
	g_print("FreetuxTV : Loading user interface from file %s\n", FREETUXTV_GLADEXML);
	app->gui = gtk_builder_new ();
	gtk_builder_add_from_file (app->gui, FREETUXTV_GLADEXML, NULL);

	// Initialize UI
	g_print("FreetuxTV : Initializing user interface\n");
	windowmain_init(app);
	channels_list_init(app);
	recordings_list_init(app);

	// Add player to UI	
	g_print("FreetuxTV : Creating media player widget\n");
	eventboxplayer = (GtkWidget *)gtk_builder_get_object (app->gui,
							      "windowmain_eventboxplayer");
	GtkLibVLCInstance* instance;
	const gchar *options[] = {"--no-video-title-show"};

	instance = gtk_libvlc_instance_new(options);
	app->player = GTK_LIBVLC_MEDIA_PLAYER(gtk_libvlc_media_player_new(instance));
	
	gtk_widget_show(GTK_WIDGET(app->player));
	g_object_unref(G_OBJECT(instance));
	gtk_container_add (GTK_CONTAINER(eventboxplayer), GTK_WIDGET(app->player));

	return app;
	
}

void
freetuxtv_play_channel (FreetuxTVApp *app, GtkTreePath* path_channel)
{
	FreetuxTVChannelInfos *channel_infos;

	channel_infos = channels_list_get_channel(app, path_channel);
	
	if(app->debug){
		g_print("FreetuxTV-debug : freetuxtv_play_channel(%s)\n", channel_infos->name);
	}
	gchar *text;
	if(!app->current.is_recording){

		g_print("FreetuxTV : Launching channel '%s' at '%s' -> %s\n",
			channel_infos->name, gtk_tree_path_to_string(path_channel), channel_infos->url);
		channels_list_set_playing(app, path_channel);		

		text = g_strdup_printf (_("Playing : %s"), channel_infos->name);
		windowmain_statusbar_push (app, "PlayChannelMsg", text);
		
		// Send notification to desktop
		gchar *imgfile;
		imgfile = logos_list_get_channel_logo_filename(app, channel_infos, TRUE);
		
		if(app->prefs.enable_notifications){
			notify_notification_update (app->current.notification, channel_infos->name,
						    _("is playing"), imgfile);
			if (!notify_notification_show (app->current.notification, NULL)) {
				g_printerr("FreetuxTV : Failed to send notification\n");
			}
		}
		g_free(imgfile);
		g_free(text);
		
		windowmain_display_buttons (app, WINDOW_MODE_PLAYING);
		
		GtkLibVLCMedia *media;
		media = gtk_libvlc_media_new(channel_infos->url);
		gtk_libvlc_media_player_clear_media_list(app->player);
		gtk_libvlc_media_player_add_media(app->player, media);
		g_object_unref(media);

		gchar *options[3];
		options[0] = ":access=timeshift";
		options[1] = ":input-timeshift-granularity=50";
		options[2] = NULL;

		gtk_libvlc_media_player_play(app->player, options);
	}
	if(app->debug){
		g_print("FreetuxTV-debug : end freetuxtv_play_channel(%s)\n", channel_infos->name);
	}
}

void
freetuxtv_play_media (FreetuxTVApp *app, GtkLibVLCMedia* media)
{
	
	if(app->debug){
		g_print("FreetuxTV-debug : freetuxtv_play_media(%s)\n", media->mrl);
	}
	
	gtk_libvlc_media_player_clear_media_list(app->player);
	gtk_libvlc_media_player_add_media(app->player, media);
	g_object_unref(media);
	gtk_libvlc_media_player_play(app->player, NULL);

	if(app->debug){
		g_print("FreetuxTV-debug : end freetuxtv_play_media(%s)\n", media->mrl);
	}
}

void
freetuxtv_action_playpause (FreetuxTVApp *app)
{	
	if(app->debug){
		g_print("FreetuxTV-debug : freetuxtv_action_playpause()\n");
	}
	if(app->current.path_channel != NULL){

		FreetuxTVChannelInfos* channel_infos;

		GtkLibVLCState state = gtk_libvlc_media_player_get_state(app->player);
		switch(state){
		case GTK_LIBVLC_STATE_PAUSED :
			gtk_libvlc_media_player_pause(app->player);
			windowmain_display_buttons (app, WINDOW_MODE_PLAYING);
			break;
		case GTK_LIBVLC_STATE_PLAYING :
			if(gtk_libvlc_media_player_can_pause(app->player)){
				gtk_libvlc_media_player_pause(app->player);
			}
			windowmain_display_buttons (app, WINDOW_MODE_PAUSED);
			break;
		default:
			channel_infos = channels_list_get_channel (app, app->current.path_channel);
			
			g_print("FreetuxTV-debug : current channel %s\n", channel_infos->name);
			GtkLibVLCMedia *media;
			media = gtk_libvlc_media_new(channel_infos->url);
			gtk_libvlc_media_player_clear_media_list(app->player);
			gtk_libvlc_media_player_add_media(app->player, media);
			g_object_unref(media);
			gtk_libvlc_media_player_play(app->player, NULL);
			
			windowmain_display_buttons (app, WINDOW_MODE_PLAYING);	
		}
	}	
	if(app->debug){
		g_print("FreetuxTV-debug : exit freetuxtv_action_playpause()\n");
	}
}

void
freetuxtv_action_stop (FreetuxTVApp *app)
{
	if(app->debug){
		g_print("FreetuxTV-debug : freetuxtv_action_stop()\n");
	}
	gchar *text;
	if(app->current.path_channel != NULL){
		FreetuxTVChannelInfos* channel_infos;
		channel_infos = channels_list_get_channel (app, app->current.path_channel);
		
		if(gtk_libvlc_media_player_is_playing(app->player)){

			if(app->current.is_recording){
				text = g_strdup_printf (_("Playing : %s"), channel_infos->name);
			}else{
				text = g_strdup_printf (_("Stopping channel : %s"), channel_infos->name);
			}
			windowmain_statusbar_push (app, "PlayChannelMsg", text);
			g_free(text);
	
			windowmain_display_buttons (app, WINDOW_MODE_STOPPED);
			gtk_libvlc_media_player_stop (app->player);			

			if(app->current.is_recording){
				app->current.is_recording = FALSE;
				freetuxtv_action_playpause (app);				
			}
		}
	}
	if(app->debug){
		g_print("FreetuxTV-debug : exit freetuxtv_action_stop()\n");
	}
}

void
freetuxtv_action_record (FreetuxTVApp *app)
{
	if(app->debug){
		g_print("FreetuxTV-debug : freetuxtv_action_record()\n");
	}
	gchar *text;

	if(gtk_libvlc_media_player_is_playing(app->player)
	   && !app->current.is_recording){
		
		FreetuxTVChannelInfos* channel_infos;
		channel_infos = channels_list_get_channel (app, app->current.path_channel);

		// Send notification to desktop
		gchar *imgfile;
		imgfile = logos_list_get_channel_logo_filename(app, channel_infos, TRUE);
		
		if(app->prefs.enable_notifications){
			notify_notification_update (app->current.notification, channel_infos->name,
						    _("is recording"), imgfile);
			if (!notify_notification_show (app->current.notification, NULL)) {
				g_printerr("FreetuxTV : Failed to send notification\n");
			}
		}

		g_free(imgfile);
		windowmain_display_buttons (app, WINDOW_MODE_RECORDING);

		// Timer and file name
		if(app->current.recording.duration != NULL){
			g_timer_stop (app->current.recording.duration);
			g_timer_destroy (app->current.recording.duration);
		}
		app->current.recording.duration = g_timer_new ();

		GTimeVal now;
                g_get_current_time(&now);
		if(app->current.recording.dst_file != NULL){
			g_free(app->current.recording.dst_file);
			app->current.recording.dst_file = NULL;
		}

		gchar *base_filename;
		base_filename = g_strconcat(channel_infos->name, " - ", g_time_val_to_iso8601(&now), NULL);

		gchar *options[2];
		options[0] = get_recording_options(app, base_filename, FALSE, &app->current.recording.dst_file);
		options[1] = NULL;
		
		gtk_libvlc_media_player_play (app->player, options);

		g_free(base_filename);
		g_free(options[0]);
		
		app->current.is_recording = TRUE;

		text = g_strdup_printf (_("Recording : %s"), app->current.recording.dst_file);
		windowmain_statusbar_push (app, "PlayChannelMsg", text);
		g_free(text);
	}
}

void
freetuxtv_action_prev (FreetuxTVApp *app)
{
	if(app->debug){
		g_print("FreetuxTV-debug : freetuxtv_action_prev()\n");
	}
	gboolean ret;
	GtkTreePath* path_prev_channel;
	if (app->current.path_channel != NULL) {
		if(!app->current.is_recording){
			ret = channels_list_get_prev_channel (app, &path_prev_channel);
			if(ret){			
				freetuxtv_play_channel (app, path_prev_channel);
			}
		}
	}
}

void
freetuxtv_action_next (FreetuxTVApp *app)
{
	if(app->debug){
		g_print("FreetuxTV-debug : freetuxtv_action_next()\n");
	}
	gboolean ret;
	GtkTreePath* path_next_channel;
	if (app->current.path_channel != NULL) {
		if(!app->current.is_recording){
			ret = channels_list_get_next_channel (app, &path_next_channel);
			if(ret){
				freetuxtv_play_channel (app, path_next_channel);
			}
		}
	}
}

void
freetuxtv_quit (FreetuxTVApp *app)
{
	if(app->debug){
		g_print("FreetuxTV-debug : freetuxtv_quit()\n");
	}
	GKeyFile *keyfile;
	char *contents, *filename;


	gboolean is_playing;
	is_playing = (gtk_libvlc_media_player_get_state(app->player) == GTK_LIBVLC_STATE_PLAYING);

	// Stop the current channel
	freetuxtv_action_stop(app);	
	
	keyfile = g_key_file_new ();

	// Save prefs
	g_key_file_set_boolean (keyfile, "general",
				"channel_on_startup",
				app->prefs.channelonstartup);
	g_key_file_set_boolean (keyfile, "general",
				"enable_notifications",
				app->prefs.enable_notifications);
	
	g_key_file_set_string (keyfile, "general",
			       "directory_record",
			       app->prefs.directoryrecordings);
	g_free(app->prefs.directoryrecordings);

	g_key_file_set_integer (keyfile, "general",
				"transcoding_mode",
				app->prefs.transcoding_mode);

	g_key_file_set_string (keyfile, "general",
				"transcoding_format",
				app->prefs.transcoding_format);

	// Save current config
	g_key_file_set_double (keyfile, "general",
			       "volume",
			       app->config.volume);
	

	if(app->current.path_channel != NULL && is_playing){
		FreetuxTVChannelInfos* channel_infos;
		channel_infos = channels_list_get_channel(app, app->current.path_channel);
		g_key_file_set_integer (keyfile, "general",
					"last_channel",
					channel_infos->id);
		gtk_tree_path_free(app->current.path_channel);
	}
	g_key_file_set_integer (keyfile, "general",
				"logos_file_date",
				app->config.logosfiledate);

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

	gtk_main_quit();
}

static void
on_freetuxtv_mm_key_pressed (GMMKeys *mmkeys, GMMKeysButton button, FreetuxTVApp* app)
{
	if(app->debug){
		g_print("FreetuxTV-debug : on_freetuxtv_mm_key_pressed()\n");
	}
	switch(button){
	case GMMKEYS_BUTTON_PLAY :
	case GMMKEYS_BUTTON_PAUSE :
		freetuxtv_action_playpause (app);		
		break;
	case GMMKEYS_BUTTON_STOP :
		freetuxtv_action_stop (app);
		break;
	case GMMKEYS_BUTTON_PREV :
		freetuxtv_action_prev (app);
		break;
	case GMMKEYS_BUTTON_NEXT :
		freetuxtv_action_next (app);
		break;
	}
}

gboolean
increase_progress_timeout (FreetuxTVApp *app)
{
	if(app->current.is_recording){	
		if(app->current.recording.duration != NULL){

			FreetuxTVChannelInfos* channel;
			channel = channels_list_get_channel (app, app->current.path_channel);

			gint second;
			second = (gint)g_timer_elapsed (app->current.recording.duration, NULL);
			
			// gtk_libvlc_media_player_set_time (app->player, second*1000);
			
			struct stat buf;
			long file_size = 0;
			if(g_stat(app->current.recording.dst_file, &buf) == 0){
				file_size = buf.st_size;
			}

			gchar *format = format_time(second);
			gchar *size_text = format_size(file_size);
			gchar *text;
			text = g_strdup_printf (_("Recording : %s (%s) -> %s (%s)"), channel->name, 
						format, app->current.recording.dst_file, size_text);
			windowmain_statusbar_pop (app, "RecordChannelMsg");
			windowmain_statusbar_push (app, "RecordChannelMsg", text);
			g_free(text);
		}
	}

	GtkLibVLCState state = gtk_libvlc_media_player_get_state(app->player);
	switch(state){
	case GTK_LIBVLC_STATE_PAUSED :
	case GTK_LIBVLC_STATE_PLAYING :
	case GTK_LIBVLC_STATE_ENDED :
		windowmain_timebar_update (app, gtk_libvlc_media_player_get_time(app->player), 
					   gtk_libvlc_media_player_get_length(app->player),
					   gtk_libvlc_media_player_get_position(app->player));
	}
	
	return TRUE;
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

	g_print("FreetuxTV : Compiled with LibVLC version %d.%d.%d\n",
		LIBVLC_VERSION_MAJOR, LIBVLC_VERSION_MINOR, LIBVLC_VERSION_REVISION);	

	gtk_init(&argc, &argv);

	g_print("FreetuxTV : Loading FreetuxTV %s\n", VERSION);
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

	mmkeys = g_mmkeys_new ("FreetuxTV");
	g_mmkeys_activate (mmkeys);
	
	g_signal_connect(G_OBJECT(mmkeys),
			 "mm_key_pressed",
			 G_CALLBACK(on_freetuxtv_mm_key_pressed),
			 app);
	
	gtk_init_add (splashscreen_app_init, app);

	// Pour les traitements à interval régulier
	g_timeout_add(1000, (GSourceFunc) increase_progress_timeout, app);

	gtk_main();

	g_mmkeys_deactivate (mmkeys);
	g_object_unref(G_OBJECT(app->current.notification));
	notify_uninit();  

	return 0;
}
