/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * main.c
 * Copyright (C) Eric Beuque 2010 <eric.beuque@gmail.com>
 * 
 * FreetuxTV is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * FreetuxTV is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <glib/gstdio.h>

#include <stdlib.h>
#include <gtk/gtk.h>
#include <libnotify/notify.h>

#include "lib-gmmkeys.h"
#include "freetuxtv-app.h"
#include "freetuxtv-utils.h"
#include "freetuxtv-i18n.h"
#include "freetuxtv-window-main.h"
#include "freetuxtv-window-recording.h"
#include "freetuxtv-channels-list.h"
#include "freetuxtv-recordings-list.h"
#include "freetuxtv-tv-channels-list.h"
#include "freetuxtv-models.h"
#include "freetuxtv-window-add-channels-group.h"
#include <libvlc-gtk/gtk-libvlc-media-player.h>

int
init_user_configuration(GError **error)
{
	int res;

	gchar *user_cache_dir;
	user_cache_dir = g_strconcat(g_get_user_config_dir(), 
				     "/FreetuxTV/cache", NULL);

	gchar *user_img_channels_dir;
	user_img_channels_dir = g_strconcat(g_get_user_config_dir(), 
					    "/FreetuxTV/images/channels", NULL);
	
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
	
	/* Create the database if doesn't exist,
	 else try to update the database */
	DBSync dbsync;
	if (!dbsync_db_exists(&dbsync)){
		dbsync_open_db (&dbsync, error);
		if(*error == NULL){
			dbsync_create_db (&dbsync, error);
		}
		dbsync_close_db (&dbsync);
	}else{
		dbsync_open_db (&dbsync, error);
		if(*error == NULL){
			dbsync_update_db(&dbsync, error);
		}
		dbsync_close_db (&dbsync);
	}

	if(*error != NULL){
		g_print("FreetuxTV : Error %s\n", (*error)->message);
	}
	
	g_free(user_cache_dir);
	g_free(user_img_channels_dir);

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
			if(i>0 && i<2){
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

		if(app->current.recording.max_duration > 0){
			GTimeVal max_time;
			GTimeVal now_time;
		       
			g_get_current_time (&now_time);
			

			g_time_val_copy(&(app->current.recording.time_begin), &max_time);
			
			g_time_val_add_seconds(&max_time, app->current.recording.max_duration * 60);
			
			if(g_time_val_compare(&now_time, &max_time) > 0){
				g_print("FreetuxTV : Stopping recording\n");
				freetuxtv_action_stop(app);
			}
			
		}
	}

	GtkLibvlcState state = gtk_libvlc_media_player_get_state(app->player);
	switch(state){
	case GTK_LIBVLC_STATE_PAUSED :
	case GTK_LIBVLC_STATE_PLAYING :
		windowmain_timebar_update (app, gtk_libvlc_media_player_get_time(app->player), 
					   gtk_libvlc_media_player_get_length(app->player),
					   gtk_libvlc_media_player_get_position(app->player));
		break;
	default:
		break;	
	}
	
	return TRUE;
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
	GError* error = NULL;
	
	GtkWidget *widget;
	widget = (GtkWidget *) gtk_builder_get_object (app->gui,
						       "splashscreen_version");
	gtk_label_set_text(GTK_LABEL(widget), VERSION);
	
	g_print("FreetuxTV : Using user configuration dir : %s\n", g_get_user_config_dir());

	// Initializing user configuration
	if(error == NULL){
		splashscreen_statusbar_push (app, _("Initializing user configuration..."));
		init_user_configuration(&error);
		splashscreen_statusbar_pop (app);
	}

	// Loading user configuration from .ini file
	if(error == NULL){
		splashscreen_statusbar_push (app, _("Loading user configuration..."));
		load_user_configuration(app);
		splashscreen_statusbar_pop (app);
	}
	
	// Open database
	DBSync dbsync;
	dbsync_open_db (&dbsync, &error);

	// Synchronizing the list of logos if file modified
	if(error == NULL){
		splashscreen_statusbar_push (app, _("Synchronizing the list of logos..."));
		struct stat file_stat;
		if(g_stat (FREETUXTV_DIR "/tv_channels.xml", &file_stat) == 0){
			if(app->config.logosfiledate < (gint)file_stat.st_mtime){
				tvchannels_list_synchronize(app, &dbsync, &error);
				if(error == NULL){
					app->config.logosfiledate = (gint)file_stat.st_mtime;	
				}
			}
		}
		splashscreen_statusbar_pop (app);
	}

	// Initialise l'interface
	windowmain_display_buttons (app, WINDOW_MODE_STOPPED);	
	
	// Loading the models
	if(error == NULL){
		g_print("FreetuxTV : Loading models\n");
		splashscreen_statusbar_push (app, _("Loading models..."));
		load_all_models (app, &error);
		splashscreen_statusbar_pop (app);
	}

	// Loading the list of channels
	if(error == NULL){
		g_print("FreetuxTV : Loading the list of channels\n");
		splashscreen_statusbar_push (app, _("Loading the list of channels..."));
		channels_list_load_channels (app, &dbsync, &error);
		splashscreen_statusbar_pop (app);
	}

	// Loading the list of recordings
	g_print("FreetuxTV : Loading the list of recordings\n");
	splashscreen_statusbar_push (app, _("Loading the list of recordings..."));
	recordings_list_load_recordings (app);
	splashscreen_statusbar_pop (app);

	// Showing the main window
	g_print("FreetuxTV : Showing the main window, hide splashscreen\n");
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

	// Display add group window if no channels group installed
	int nb_channelsgroup;
	nb_channelsgroup = gtk_tree_model_iter_n_children (app->channelslist, NULL);
	if(nb_channelsgroup == 0){	
		FreetuxTVWindowAddChannelsGroup* pWindowAddChannelsGroups;
		gint res;
	
		pWindowAddChannelsGroups = freetuxtv_window_add_channels_group_new (app);
		res = freetuxtv_window_add_channels_group_run (pWindowAddChannelsGroups);

		g_object_unref(pWindowAddChannelsGroups);
		pWindowAddChannelsGroups = NULL;
	}

	// Update statut bar
	windowmain_update_statusbar_infos (app);
	
	// Close database
	dbsync_close_db(&dbsync);

	// Start internal timer
	g_timeout_add(1000, (GSourceFunc) increase_progress_timeout, app);

	if(error != NULL){
		g_print("FreetuxTV Error : %s\n", error->message);
		g_error_free(error);
		return FALSE;
	}

	return TRUE;
}

static FreetuxTVApp *
freetuxtv_app_create_app ()
{
	FreetuxTVApp *app;

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
	windowrecording_init(app);
	channels_list_init(app);
	recordings_list_init(app);

	// Add player to UI	
	g_print("FreetuxTV : Creating media player widget\n");
	eventboxplayer = (GtkWidget *)gtk_builder_get_object (app->gui,
							      "windowmain_eventboxplayer");
	GtkLibvlcInstance* instance;
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
		imgfile = tvchannels_list_get_tvchannel_logo_path(app, channel_infos, TRUE);
		
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
		
		GtkLibvlcMedia *media;
		media = gtk_libvlc_media_new(channel_infos->url);
		gtk_libvlc_media_set_options(media, channel_infos->vlc_options);
		
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
freetuxtv_play_media (FreetuxTVApp *app, GtkLibvlcMedia* media)
{
	
	if(app->debug){
		g_print("FreetuxTV-debug : freetuxtv_play_media(%s)\n", media->mrl);
	}
	
	windowmain_display_buttons (app, WINDOW_MODE_PLAYING);

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

		GtkLibvlcState state = gtk_libvlc_media_player_get_state(app->player);
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
			GtkLibvlcMedia *media;
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
		imgfile = tvchannels_list_get_tvchannel_logo_path(app, channel_infos, TRUE);
		
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

int
main (int argc, char *argv[])
{
 	FreetuxTVApp *app;
	
	GMMKeys* mmkeys;

	int p;

#ifdef ENABLE_NLS
	bindtextdomain (GETTEXT_PACKAGE, PACKAGE_LOCALE_DIR);
	bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
	textdomain (GETTEXT_PACKAGE);
#endif

	g_print("FreetuxTV : Compiled with LibVLC version %s\n", gtk_libvlc_get_libvlc_version(NULL,NULL,NULL));
	
	gtk_set_locale ();
	gtk_init (&argc, &argv);

	g_print("FreetuxTV : Loading FreetuxTV %s\n", VERSION);
	app = freetuxtv_app_create_app ();
	if (app == NULL)
		return 1;
	// Process programs args
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

	gtk_main ();

	g_mmkeys_deactivate (mmkeys);
	g_object_unref(G_OBJECT(app->current.notification));
	notify_uninit();

	return 0;
}
