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
#include <locale.h>

#include <libvlc-gtk/gtk-libvlc-media-player.h>

#include "lib-gmmkeys.h"
#include "freetuxtv-app.h"
#include "freetuxtv-utils.h"
#include "freetuxtv-i18n.h"
#include "freetuxtv-window-main.h"
#include "freetuxtv-channels-list.h"
#include "freetuxtv-recordings-list.h"
#include "freetuxtv-tv-channels-list.h"
#include "freetuxtv-models.h"
#include "freetuxtv-window-add-channels-group.h"
#include "freetuxtv-player-error-dialog.h"

static void
list_options_add_options (gchar*** pOptions, gchar** options);

static void
list_options_add_option (gchar*** pOptions, gchar* option);

static void
list_options_free (gchar** pOptions);

int
init_user_configuration(FreetuxTVApp *app, GError **error)
{
	int res = 0;
	gboolean bGoOn = TRUE;

	gchar *user_cache_dir;
	user_cache_dir = g_build_filename(g_get_user_cache_dir(), "freetuxtv", NULL);

	gchar *user_img_channels_dir;
	user_img_channels_dir = g_build_filename(g_get_user_data_dir(),
	                                         "freetuxtv", "images", "channels", NULL);

	// Check if user cache directory exists
	if (!g_file_test (user_cache_dir, G_FILE_TEST_EXISTS)){
		res = g_mkdir_with_parents (user_cache_dir,
		                            0744);
		if(res == 0){
			g_log(FREETUXTV_LOG_DOMAIN, G_LOG_LEVEL_INFO,
			      "Directory created: %s\n", user_cache_dir);	
		}else{
			g_log(FREETUXTV_LOG_DOMAIN, G_LOG_LEVEL_CRITICAL,
			      "Cannot create directory: %s\n", user_cache_dir);
			bGoOn = FALSE;
		}
	}
	if(bGoOn){
		if (!g_file_test (user_cache_dir, G_FILE_TEST_IS_DIR)){
			g_log(FREETUXTV_LOG_DOMAIN, G_LOG_LEVEL_CRITICAL,
			      "Not a directory: %s\n", user_cache_dir);
			bGoOn = FALSE;
		}
	}

	// Check if user images directory exists
	if(bGoOn){
		if (!g_file_test (user_img_channels_dir, G_FILE_TEST_EXISTS)){
			res = g_mkdir_with_parents (user_img_channels_dir,
			                            0744);
			if(res == 0){
				g_log(FREETUXTV_LOG_DOMAIN, G_LOG_LEVEL_INFO,
				      "Directory created: %s\n", user_img_channels_dir);	
			}else{
				g_log(FREETUXTV_LOG_DOMAIN, G_LOG_LEVEL_CRITICAL,
				      "Cannot create directory: %s\n", user_img_channels_dir);
				bGoOn = FALSE;
			}
		}
	}
	if(bGoOn){
		if (!g_file_test (user_img_channels_dir, G_FILE_TEST_IS_DIR)){
			g_log(FREETUXTV_LOG_DOMAIN, G_LOG_LEVEL_CRITICAL,
			      "Not a directory: %s\n", user_img_channels_dir);
			bGoOn = FALSE;
		}
	}

	// Create the database if doesn't exist, else try to update the database
	gchar* szScriptFilename;
	szScriptFilename = g_build_filename(app->paths.datadir, "sqlite3-create-tables.sql", NULL);
	DBSync dbsync;
	if (!dbsync_db_exists(&dbsync)){
		dbsync_open_db (&dbsync, error);
		if(*error == NULL){
			dbsync_create_db (&dbsync, szScriptFilename, error);
		}
		dbsync_close_db (&dbsync);
	}else{
		dbsync_open_db (&dbsync, error);
		if(*error == NULL){
			dbsync_update_db(&dbsync, szScriptFilename, error);
		}
		dbsync_close_db (&dbsync);
	}
	if(szScriptFilename){
		g_free(szScriptFilename);
		szScriptFilename = NULL;
	}

	if(*error != NULL){
		g_log(FREETUXTV_LOG_DOMAIN, G_LOG_LEVEL_ERROR,
		      "Error %s\n", (*error)->message);
	}

	g_free(user_cache_dir);
	g_free(user_img_channels_dir);

	return (bGoOn ? 0 : -1);
}

void
load_user_configuration(FreetuxTVApp *app)
{
	GKeyFile *keyfile;
	int i;
	gboolean b;
	gdouble d;
	gchar *str;
	char *filename;
	GError *err = NULL;

	// Loading FreetuxTV State
	filename = g_build_filename (g_get_user_config_dir(),
	                             "FreetuxTV", "config.ini", NULL);

	g_log(FREETUXTV_LOG_DOMAIN, G_LOG_LEVEL_INFO,
	      "Loading config file %s\n", filename);
	keyfile = g_key_file_new ();
	if (g_key_file_load_from_file (keyfile, filename, G_KEY_FILE_NONE, NULL) == FALSE)
	{
		g_log(FREETUXTV_LOG_DOMAIN, G_LOG_LEVEL_INFO,
		      "Cannot load config file\n");
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

		b = g_key_file_get_boolean (keyfile, "general",
		                            "ignore_diacritics", &err);
		if (err != NULL) {
			g_error_free (err);
			err = NULL;
		}else{
			app->prefs.ignore_diacritics = b;		
		}

		i = g_key_file_get_integer (keyfile, "libvlc",
		                            "configfile_mode", &err);
		if (err != NULL) {
			g_error_free (err);
			err = NULL;
		}else{
			if(i>=0 && i<=2){
				app->prefs.libvlcconfigfile_mode = i;
			}
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
			if(i>=0 && i<=1){
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

		i = g_key_file_get_integer (keyfile, "network",
		                            "timeout", &err);
		if (err != NULL) {
			g_error_free (err);
			err = NULL;
		}else{
			app->prefs.timeout = i;
		}

		i = g_key_file_get_integer (keyfile, "network",
		                            "proxy_mode", &err);
		if (err != NULL) {
			g_error_free (err);
			err = NULL;
		}else{
			if(i>0 && i<2){
				app->prefs.proxy.proxy_mode = i;
			}
		}
		
		str = g_key_file_get_string (keyfile, "network",
		                             "proxy_server", &err);
		if (err != NULL) {
			g_error_free (err);
			err = NULL;
		}else{
			app->prefs.proxy.proxy_server = str;				
		}
		
		str = g_key_file_get_string (keyfile, "network",
		                             "proxy_port", &err);
		if (err != NULL) {
			g_error_free (err);
			err = NULL;
		}else{
			app->prefs.proxy.proxy_port = str;				
		}
		
		str = g_key_file_get_string (keyfile, "network",
		                             "proxy_type", &err);
		if (err != NULL) {
			g_error_free (err);
			err = NULL;
		}else{
			app->prefs.proxy.proxy_type = str;				
		}

		b = g_key_file_get_boolean (keyfile, "network",
		                            "proxy_use_auth", &err);
		if (err != NULL) {
			g_error_free (err);
			err = NULL;
		}else{
			app->prefs.proxy.proxy_use_auth = b;		
		}
		
		str = g_key_file_get_string (keyfile, "network",
		                             "proxy_username", &err);
		if (err != NULL) {
			g_error_free (err);
			err = NULL;
		}else{
			app->prefs.proxy.proxy_username = str;				
		}
		
		str = g_key_file_get_string (keyfile, "network",
		                             "proxy_password", &err);
		if (err != NULL) {
			g_error_free (err);
			err = NULL;
		}else{
			app->prefs.proxy.proxy_password = str;				
		}

		// Load current config
		// b = g_key_file_get_boolean (keyfile, "windowminimode",
		//                             "stay_on_top", &err);
		// if (err != NULL) {
		// 	g_error_free (err);
		// 	err = NULL;
		// }else{
		// 	app->config.windowminimode_stayontop = b;		
		// }
		
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
}

gboolean
increase_progress_timeout (FreetuxTVApp *app)
{
	GError* error = NULL;

	GList *list = NULL;
	FreetuxTVRecordingInfos* pRecordingInfos = NULL;

	if(!app->current.is_recording){

		// Get the list of record to process
		recordings_list_getrecordings_toprocess(app, &list, &error);
		if(error == NULL && list){
			pRecordingInfos = (FreetuxTVRecordingInfos*)list->data;
			if(pRecordingInfos){
				// We record only the first found
				g_log(FREETUXTV_LOG_DOMAIN, G_LOG_LEVEL_MESSAGE,
					  "Staring recording %s\n", pRecordingInfos->szTitle);
				pRecordingInfos = (FreetuxTVRecordingInfos*)list->data;

				freetuxtv_action_start_recording (app, pRecordingInfos, &error);
			}
		}
		g_list_free (list);
	}else{
		
		GTimeVal now_time;
		g_get_current_time (&now_time);
		gint64 now64 = g_time_val_to_int64(&now_time);
		
		pRecordingInfos = app->current.recording.pRecordingInfo;
		if(pRecordingInfos){

			FreetuxTVChannelInfos* pChannelInfos;
			pChannelInfos = channels_list_get_channel (app, app->current.pPathChannel);

			gint second_elapsed;
			second_elapsed = now_time.tv_sec - app->current.recording.time_begin.tv_sec;
			// gtk_libvlc_media_player_set_time (app->player, second*1000);

			struct stat buf;
			long file_size = 0;
			if(g_stat(pRecordingInfos->szFileName, &buf) == 0){
				file_size = buf.st_size;
			}

			gchar *format = format_time(second_elapsed);
			gchar *size_text = format_size(file_size);
			gchar *text;
			gchar *szTmp;

			szTmp = g_path_get_basename (pRecordingInfos->szFileName);
			text = g_strdup_printf (_("Recording: %s (%s) -> %s (%s)"),
			    pChannelInfos->name, format, szTmp, size_text);
			g_free(szTmp);
			windowmain_statusbar_pop (app, "RecordChannelMsg");
			windowmain_statusbar_push (app, "RecordChannelMsg", text);
			g_free(text);

			if(freetuxtv_recording_infos_is_time_greater(pRecordingInfos, now64)){
				freetuxtv_action_stop_recording (app, app->current.recording.pRecordingInfo, &error);
			}
		}
	}

	GtkLibvlcState state = gtk_libvlc_media_player_get_state(app->player, &error);
	switch(state){
		case GTK_LIBVLC_STATE_PAUSED :
		case GTK_LIBVLC_STATE_PLAYING :
			windowmain_timebar_update (app,
			    gtk_libvlc_media_player_get_time(app->player, &error),
			    gtk_libvlc_media_player_get_length(app->player, &error),
			    gtk_libvlc_media_player_get_position(app->player, &error));
			break;
		default:
			break;	
	}

	if(error != NULL){
		g_error_free (error);
		error = NULL;
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

static void
on_channels_group_added (
	    FreetuxTVWindowAddChannelsGroup *pWindowAddChannelsGroup,
	    FreetuxTVChannelsGroupInfos* pChannelsGroupInfos,
	    DBSync *dbsync, GError** error,
	    gpointer user_data)
{
	g_log(FREETUXTV_LOG_DOMAIN, G_LOG_LEVEL_INFO,
	      "Received signal for channels group added '%s'\n", pChannelsGroupInfos->name);

	FreetuxTVApp *app;
	app = freetuxtv_window_add_channels_group_get_app(pWindowAddChannelsGroup);

	GtkTreePath** ppTreePath = (GtkTreePath**)user_data;

	if(ppTreePath){
		gtk_tree_path_free (*ppTreePath);
		*ppTreePath = NULL;
	}
		
	channels_list_ui_add_channels_group (app, pChannelsGroupInfos, ppTreePath);
}

static void
on_channels_added (
	    FreetuxTVWindowAddChannelsGroup *pWindowAddChannelsGroup,
	    FreetuxTVChannelsGroupInfos* pChannelsGroupInfos,
	    DBSync *dbsync, GError** error,
	    gpointer user_data)
{
	g_log(FREETUXTV_LOG_DOMAIN, G_LOG_LEVEL_INFO,
	      "Received signal for channels added in group '%s'\n", pChannelsGroupInfos->name);

	FreetuxTVApp *app;
	app = freetuxtv_window_add_channels_group_get_app(pWindowAddChannelsGroup);

	GtkTreePath** ppTreePath = (GtkTreePath**)user_data;

	if(ppTreePath){
		channels_list_reload_channels_of_channels_group (app, dbsync, *ppTreePath, error);
	}
}

#if GTK_API_VERSION == 3
static void
on_window_add_channels_group_destroy (GtkWidget *widget, gpointer user_data)
#else
static void
on_window_add_channels_group_destroy (GtkObject *object, gpointer user_data)
#endif
{
	GtkTreePath** ppCurrentTreePath = (GtkTreePath**)user_data;

	if(ppCurrentTreePath){
		if(*ppCurrentTreePath){
			gtk_tree_path_free (*ppCurrentTreePath);
			*ppCurrentTreePath = NULL;
		}
		
		g_free(ppCurrentTreePath);
		ppCurrentTreePath = NULL;
	}
}

static gboolean
showplayer_error (gpointer data)
{
	FreetuxTVApp *app = (FreetuxTVApp *)data;

	g_log(FREETUXTV_LOG_DOMAIN, G_LOG_LEVEL_INFO, "Displaying error window\n");

	GtkWidget* pParent;
	pParent = (GtkWidget *) gtk_builder_get_object (app->gui, "windowmain");

	GtkWidget* dialog = app->widget.pPlayerErrorDialog;
	if(dialog == NULL){
		dialog = freetuxtv_player_error_dialog_new (
			GTK_WINDOW(pParent), GTK_DIALOG_DESTROY_WITH_PARENT, app);
		app->widget.pPlayerErrorDialog = dialog;
	}

	
	FreetuxTVChannelInfos *pChannelInfos;
	pChannelInfos = channels_list_get_channel(app, app->current.pPathChannel);

	gchar* message = "";

	gchar* db_url = "http://database.freetuxtv.net/";
	
	if(pChannelInfos != NULL){
		message = g_strdup_printf (_("An error occurred while playing channel '%s' : %s\n\n\
If the problem persist, you can report that the link is dead at %s"),
		    pChannelInfos->name, pChannelInfos->url, db_url);
	}else{
		message = _("An error occurred");
	}
	
	freetuxtv_player_error_dialog_set_message (
		FREETUXTV_PLAYER_ERROR_DIALOG(dialog), message);
	gtk_widget_show(dialog);
		
	return FALSE;
}

static void on_mediaplayer_event_occurred (
    GtkLibvlcMediaPlayer *self,
    GtkLibvlcEventType type_event,
    gpointer user_data)
{
	FreetuxTVApp *app = (FreetuxTVApp *)user_data;
	
	if(type_event == GTK_LIBVLC_EVENT_MP_ENCOUTEREDERROR){
		g_idle_add (showplayer_error, app);
	}
}

static void synchronize_progress_cb(gchar* szTVChannelName, void* user_data)
{
	FreetuxTVApp* app = (FreetuxTVApp*)user_data;

	gchar* szTmp = g_strdup_printf(_("Synchronizing the list of tv channels: %s..."), szTVChannelName);
	
	splashscreen_statusbar_pop (app);
	splashscreen_statusbar_push (app, szTmp);

	if(szTmp){
		g_free(szTmp);
		szTmp = NULL;
	}
}

static gboolean
splashscreen_app_init(gpointer data)
{
	FreetuxTVApp *app = (FreetuxTVApp *)data;
	GError* error = NULL;

	GtkWidget *widget;
	GtkWidget *eventboxplayer;
	GtkWidget *pMainWindow = NULL;

	widget = (GtkWidget *)gtk_builder_get_object (app->gui,
		                                          "splashscreen_image");
	gchar* pImgSplashScreen;
	pImgSplashScreen = g_build_filename(app->paths.datadir, "splashscreen.png", NULL);
	gtk_image_set_from_file (GTK_IMAGE(widget), pImgSplashScreen);
	g_free(pImgSplashScreen);

	gchar* szConfigDir;
	szConfigDir = g_build_filename(g_get_user_config_dir(), "FreetuxTV", NULL);
	g_log(FREETUXTV_LOG_DOMAIN, G_LOG_LEVEL_INFO,
	      "Using user configuration dir: %s\n", szConfigDir);

	// Initializing user configuration
	if(error == NULL){
		splashscreen_statusbar_push (app, _("Initializing user configuration..."));
		init_user_configuration(app, &error);
		splashscreen_statusbar_pop (app);
	}

	// Loading user configuration from .ini file
	if(error == NULL){
		splashscreen_statusbar_push (app, _("Loading user configuration..."));
		load_user_configuration(app);
		splashscreen_statusbar_pop (app);
	}

	// Add player to UI
	if(error == NULL){
		g_log(FREETUXTV_LOG_DOMAIN, G_LOG_LEVEL_INFO,
		    "Creating media player widget\n");
		eventboxplayer = (GtkWidget *)gtk_builder_get_object (app->gui,
		    "windowmain_eventboxplayer");
		GtkLibvlcInstance* instance;
		gchar **options = NULL;
		gchar* newoption;

		// Initialize LibVLC instance
		g_log(FREETUXTV_LOG_DOMAIN, G_LOG_LEVEL_INFO,
		    "Initializing LibVLC instance\n");

		gint major, minor;
		gtk_libvlc_get_libvlc_version(&major, &minor, NULL);
		//list_options_add_option(&options, "-vvv"); // verbose mode
		if(major>=1 || (major == 1 && minor>= 1)){
			list_options_add_option(&options, "--no-xlib"); // Tell VLC to not use Xlib
		}
		if(app->prefs.libvlcconfigfile_mode == 0){
			// No config file
			list_options_add_option(&options, "--ignore-config");
			list_options_add_option(&options, "--no-video-title-show");
		}else if(app->prefs.libvlcconfigfile_mode == 1){
			// Custom config file
			list_options_add_option(&options, "--no-ignore-config");
			newoption = g_strdup_printf("--config=%s/vlcrc", szConfigDir);
			list_options_add_option(&options, newoption);
			g_free(newoption);
			newoption = NULL;
		}else if(app->prefs.libvlcconfigfile_mode == 2){
			// VLC config file
			list_options_add_option(&options, "--no-ignore-config");
		}

		instance = gtk_libvlc_instance_new((const gchar**)options, freetuxtv_log, &error);
		if(error == NULL){
			app->player = GTK_LIBVLC_MEDIA_PLAYER(gtk_libvlc_media_player_new(instance, NULL));
			g_signal_connect(G_OBJECT(app->player),
                 "event-occurred",
                 G_CALLBACK(on_mediaplayer_event_occurred),
                 app);

			gtk_widget_show(GTK_WIDGET(app->player));
			g_object_unref(G_OBJECT(instance));
			gtk_container_add (GTK_CONTAINER(eventboxplayer), GTK_WIDGET(app->player));

			gtk_libvlc_media_player_set_accel_group (app->player, app->widget.pAccelGroup);
		}

		list_options_free(options);
		options = NULL;
	}

	// Open database
	DBSync dbsync;
	gboolean bDbOpen = FALSE;
	if(error == NULL){
		dbsync_open_db (&dbsync, &error);
		bDbOpen = TRUE;
	}

	// Synchronizing the list of tv channels if file modified
	if(error == NULL){
		splashscreen_statusbar_push (app, _("Synchronizing the list of tv channels..."));
		struct stat file_stat;
		gchar* filename;
		filename = g_build_filename(app->paths.datadir, "tv_channels.xml", NULL);
		if(g_stat (filename, &file_stat) == 0){
			if(app->config.logosfiledate < (gint)file_stat.st_mtime){
				tvchannels_list_synchronize(app, &dbsync, &synchronize_progress_cb, app, &error);
				if(error == NULL){
					app->config.logosfiledate = (gint)file_stat.st_mtime;	
				}
			}
		}
		g_free(filename);
		filename = NULL;
		splashscreen_statusbar_pop (app);
	}

	// Initialise l'interface
	windowmain_display_buttons (app, WINDOW_MODE_STOPPED);

	// Look for a channel to open
	if(error == NULL){
		if(app->current.open_channel_name){
			app->current.open_channel_id = dbsync_get_channel_id_by_name (&dbsync,
			    app->current.open_channel_name, &error);
		}
	}


	// Loading the list of channels
	if(error == NULL){
		g_log(FREETUXTV_LOG_DOMAIN, G_LOG_LEVEL_INFO,
		    "Loading the list of channels\n");
		splashscreen_statusbar_push (app, _("Loading the list of channels..."));
		channels_list_load_channels (app, &dbsync, &error);
		splashscreen_statusbar_pop (app);
	}

	// Loading the list of recordings
	if(error == NULL){
		g_log(FREETUXTV_LOG_DOMAIN, G_LOG_LEVEL_INFO,
		    "Loading the list of recordings\n");
		splashscreen_statusbar_push (app, _("Loading the list of recordings..."));
		recordings_list_load_recordings (app, &dbsync, &error);
		recordings_list_updatestatus(app, &dbsync, &error);
		splashscreen_statusbar_pop (app);
	}
	
	// Close database
	if(bDbOpen){
		dbsync_close_db(&dbsync);
	}

	// Showing the main window
	if(error == NULL){
		g_log(FREETUXTV_LOG_DOMAIN, G_LOG_LEVEL_INFO,
		    "Showing the main window, hide splashscreen\n");
		widget = (GtkWidget *)gtk_builder_get_object (app->gui,
		    "splashscreen");
		gtk_widget_hide(widget);

		widget = (GtkWidget *)gtk_builder_get_object (app->gui,
		    "windowmain");
		gtk_widget_show(widget);
		pMainWindow = widget;
	}

	// Set the sound level of the media player
	if(error == NULL){
		widget = (GtkWidget *)gtk_builder_get_object (app->gui,
		    "windowmain_volumecontrol");
		gtk_range_set_value (GTK_RANGE(widget), app->config.volume);
		gtk_libvlc_media_player_set_volume (app->player, app->config.volume, NULL);
	}

	// Play the last channel if needed
	if(error == NULL){
		if(app->current.pPathChannel != NULL){
			freetuxtv_play_channel (app, app->current.pPathChannel, &error);
		}
	}

	GtkWindow* pWindow;
	
	// Display add group window if no channels group installed
	if(error == NULL){
		int nb_channelsgroup;
		nb_channelsgroup = gtk_tree_model_iter_n_children (app->channelslist, NULL);
		if(nb_channelsgroup == 0){	
			FreetuxTVWindowAddChannelsGroup* pWindowAddChannelsGroups;

			pWindowAddChannelsGroups = freetuxtv_window_add_channels_group_new (GTK_WINDOW(pMainWindow), app, &error);

			if(error == NULL){
				pWindow = gtk_builder_window_get_top_window (GTK_BUILDER_WINDOW(pWindowAddChannelsGroups));
				gtk_widget_show(GTK_WIDGET(pWindow));

				GtkTreePath** ppCurrentTreePath = g_new0 (GtkTreePath*, 1);

				g_signal_connect(G_OBJECT(pWindowAddChannelsGroups), "channels-group-added",
					G_CALLBACK(on_channels_group_added), ppCurrentTreePath);
				g_signal_connect(G_OBJECT(pWindowAddChannelsGroups), "channels-added",
					G_CALLBACK(on_channels_added), ppCurrentTreePath);
				g_signal_connect(G_OBJECT(pWindowAddChannelsGroups), "destroy",
					G_CALLBACK(on_window_add_channels_group_destroy), ppCurrentTreePath);
			}
		}
	}

	// Update statut bar
	if(error == NULL){
		windowmain_update_statusbar_infos (app);
	}

	// Start internal timer
	if(error == NULL){
		g_timeout_add(1000, (GSourceFunc) increase_progress_timeout, app);
	}

	g_free(szConfigDir);
	szConfigDir = NULL;

	if(error != NULL){
		g_log(FREETUXTV_LOG_DOMAIN, G_LOG_LEVEL_CRITICAL,
		      "Error : %s\n", error->message);
		windowmain_show_gerror(app, error);
		g_error_free(error);		
		return FALSE;
	}
	
#if GTK_API_VERSION == 3
	return FALSE;
#else
	return TRUE;
#endif
}

static FreetuxTVApp *
freetuxtv_app_create_app (const gchar* szDataDir)
{
	FreetuxTVApp *app;

	app = g_new0 (FreetuxTVApp, 1);

	app->name = "FreetuxTV";
	app->player = NULL;

	// Set app paths
	if(szDataDir){
		app->paths.datadir = g_strdup(szDataDir);
	}else{
		app->paths.datadir = g_strdup(FREETUXTV_DIR);
	}
	app->paths.gladexmlfile = g_build_filename(app->paths.datadir, "ui", "freetuxtv.glade", NULL);
	app->paths.szPathGladeXml = g_build_filename(app->paths.datadir, "ui", NULL);
	
	// Loads default configuration
	app->prefs.channelonstartup = TRUE;
	app->prefs.enable_notifications = TRUE;
	app->prefs.ignore_diacritics = TRUE;
	app->prefs.libvlcconfigfile_mode = 0;
	app->prefs.directoryrecordings = g_strdup(g_get_home_dir());
	app->prefs.transcoding_mode = 0;
	app->prefs.transcoding_format = g_strdup("0");
	app->prefs.timeout = 20;
	app->prefs.proxy.proxy_mode = G_PROXY_MODE_NONE;
	app->prefs.proxy.proxy_server = g_strdup("");
	app->prefs.proxy.proxy_port = g_strdup("");
	app->prefs.proxy.proxy_type = g_strdup("0");
	app->prefs.proxy.proxy_use_auth = FALSE;
	app->prefs.proxy.proxy_username = g_strdup("");
	app->prefs.proxy.proxy_password = g_strdup("");

	app->config.windowminimode_stayontop = FALSE;
	app->config.windowminimode_width = 320;
	app->config.windowminimode_height = 240;
	app->config.lastchannel = -1;
	app->config.volume = 0.75;
	app->config.logosfiledate = 0;

	app->current.open_channel_name = NULL;
	app->current.open_channel_id = -1;
	app->current.pPathChannel = NULL;
	app->current.autoplay_channel = FALSE;
	app->current.is_recording = FALSE;
	app->current.recording.pRecordingInfo = NULL;
	app->current.bIsAccelGroupActivated = FALSE;

	// Create UI from file
	g_log(FREETUXTV_LOG_DOMAIN, G_LOG_LEVEL_INFO,
	      "Loading user interface from path %s\n", app->paths.szPathGladeXml);
	app->gui = gtk_builder_new ();
	gtk_builder_add_from_file (app->gui, app->paths.gladexmlfile, NULL);

	// Initialize UI
	g_log(FREETUXTV_LOG_DOMAIN, G_LOG_LEVEL_INFO,
	      "Initializing user interface\n");
	windowmain_init(app);
	//windowrecording_init(app);
	channels_list_init(app);
	recordings_list_init(app);

	// Player error dialog
	app->widget.pPlayerErrorDialog = NULL;

	return app;

}

static void
freetuxtv_app_destroy_app (FreetuxTVApp** pApp)
{
	g_return_if_fail(pApp != NULL);

	FreetuxTVApp* app = *pApp;

	if(app->paths.datadir){
		g_free(app->paths.datadir);
		app->paths.datadir = NULL;
	}
	if(app->paths.gladexmlfile){
		g_free(app->paths.gladexmlfile);
		app->paths.gladexmlfile = NULL;
	}

	if(app->prefs.directoryrecordings){
		g_free(app->prefs.directoryrecordings);
		app->prefs.directoryrecordings = NULL;
	}

	if(app->prefs.transcoding_format){
		g_free(app->prefs.transcoding_format);
		app->prefs.transcoding_format = NULL;
	}

	if(app->prefs.proxy.proxy_server){
		g_free(app->prefs.proxy.proxy_server);
		app->prefs.proxy.proxy_server = NULL;
	}

	if(app->prefs.proxy.proxy_port){
		g_free(app->prefs.proxy.proxy_port);
		app->prefs.proxy.proxy_port = NULL;
	}

	if(app->prefs.proxy.proxy_type){
		g_free(app->prefs.proxy.proxy_type);
		app->prefs.proxy.proxy_type = NULL;
	}

	if(app->prefs.proxy.proxy_username){
		g_free(app->prefs.proxy.proxy_username);
		app->prefs.proxy.proxy_username = NULL;
	}

	if(app->prefs.proxy.proxy_password){
		g_free(app->prefs.proxy.proxy_password);
		app->prefs.proxy.proxy_password = NULL;
	}

	if(app->gui){
		g_object_unref(app->gui);
		app->gui = NULL;
	}

	if(app->widget.pPlayerErrorDialog){
		gtk_widget_destroy (app->widget.pPlayerErrorDialog);
		app->widget.pPlayerErrorDialog = NULL;
	}
	
	g_free(app);
	app = NULL;
}

static void
list_options_add_options (gchar*** pOptions, gchar** options)
{
	if(pOptions){
		gchar* szOriginalOptions = NULL;
		gchar* szNewOptions = NULL;
		gchar* szFinalOptions = NULL;

		if(*pOptions){
			szOriginalOptions = g_strjoinv("\t", *pOptions);
		}
		if(options){
			szNewOptions = g_strjoinv("\t", options);
		}
		if(szOriginalOptions && szNewOptions){
			szFinalOptions = g_strconcat(szOriginalOptions, "\t", szNewOptions, NULL);
		}else{
			if(szOriginalOptions){
				szFinalOptions = g_strdup(szOriginalOptions);
			}
			if(szNewOptions){
				szFinalOptions = g_strdup(szNewOptions);
			}
		}

		if(*pOptions){
			g_strfreev(*pOptions);
			*pOptions = NULL;
		}
		if(szFinalOptions){
			*pOptions = g_strsplit(szFinalOptions, "\t", 0);
		}

		if(szOriginalOptions){
			g_free(szOriginalOptions);
			szOriginalOptions = NULL;
		}

		if(szNewOptions){
			g_free(szNewOptions);
			szNewOptions = NULL;
		}

		if(szFinalOptions){
			g_free(szFinalOptions);
			szFinalOptions = NULL;
		}
	}
}

static void
list_options_add_option (gchar*** pOptions, gchar* option)
{
	gchar *options[2];
	options[0] = option;
	options[1] = NULL;

	list_options_add_options (pOptions, options);
}

static void
list_options_free (gchar** pOptions)
{
	if(pOptions){
		g_strfreev (pOptions);
		pOptions = NULL;
	}
}

void
freetuxtv_play_channel (FreetuxTVApp *app, GtkTreePath* path_channel, GError** error)
{
	FreetuxTVChannelInfos *pChannelInfos;

	pChannelInfos = channels_list_get_channel(app, path_channel);

	g_log(FREETUXTV_LOG_DOMAIN, G_LOG_LEVEL_DEBUG,
	      "freetuxtv_play_channel(%s)\n", pChannelInfos->name);

	gchar *text;
	if(!app->current.is_recording){

		g_log(FREETUXTV_LOG_DOMAIN, G_LOG_LEVEL_MESSAGE,
		      "Launching channel '%s' at '%s' -> %s\n",
		      pChannelInfos->name, gtk_tree_path_to_string(path_channel), pChannelInfos->url);
		channels_list_set_playing(app, path_channel);		

		text = g_strdup_printf (_("Playing: %s"), pChannelInfos->name);
		windowmain_statusbar_push (app, "PlayChannelMsg", text);

		// Send notification to desktop
		gchar *imgfile;
		imgfile = tvchannels_list_get_tvchannel_logo_path_for_channel(app, pChannelInfos, TRUE);

		if(app->prefs.enable_notifications){
			notify_notification_update (app->current.notification, pChannelInfos->name,
			                            _("is playing"), imgfile);
			if (!notify_notification_show (app->current.notification, NULL)) {
				g_log(FREETUXTV_LOG_DOMAIN, G_LOG_LEVEL_WARNING,
				      "Failed to send notification\n");
			}
		}
		g_free(imgfile);
		g_free(text);

		windowmain_display_buttons (app, WINDOW_MODE_PLAYING);

		g_log(FREETUXTV_LOG_DOMAIN, G_LOG_LEVEL_DEBUG,
		      "Creating the media to play\n");

		GtkLibvlcMedia *media;
		media = gtk_libvlc_media_new(pChannelInfos->url);
		gtk_libvlc_media_set_options(media, pChannelInfos->vlc_options);

		if(pChannelInfos->deinterlace_mode){
			gchar* szMode;

			szMode = g_strdup_printf(":deinterlace-mode=%s", pChannelInfos->deinterlace_mode);
			
			gtk_libvlc_media_add_option(media, ":deinterlace=1");
			gtk_libvlc_media_add_option(media, szMode);

			if(szMode){
				g_free(szMode);
				szMode = NULL;
			}

			if(g_strcasecmp (pChannelInfos->deinterlace_mode, "blend") == 0){
				gtk_check_menu_item_set_active (GTK_CHECK_MENU_ITEM(app->widget.pMenuDeinterlaceBlend), TRUE);
			}else if(g_strcasecmp (pChannelInfos->deinterlace_mode, "bob") == 0){
				gtk_check_menu_item_set_active (GTK_CHECK_MENU_ITEM(app->widget.pMenuDeinterlaceBob), TRUE);
			}else if(g_strcasecmp (pChannelInfos->deinterlace_mode, "discard") == 0){
				gtk_check_menu_item_set_active (GTK_CHECK_MENU_ITEM(app->widget.pMenuDeinterlaceDiscard), TRUE);
			}else if(g_strcasecmp (pChannelInfos->deinterlace_mode, "linear") == 0){
				gtk_check_menu_item_set_active (GTK_CHECK_MENU_ITEM(app->widget.pMenuDeinterlaceLinear), TRUE);
			}else if(g_strcasecmp (pChannelInfos->deinterlace_mode, "mean") == 0){
				gtk_check_menu_item_set_active (GTK_CHECK_MENU_ITEM(app->widget.pMenuDeinterlaceMean), TRUE);
			}else if(g_strcasecmp (pChannelInfos->deinterlace_mode, "x") == 0){
				gtk_check_menu_item_set_active (GTK_CHECK_MENU_ITEM(app->widget.pMenuDeinterlaceX), TRUE);
			}
		}else{
			gtk_libvlc_media_add_option(media, ":deinterlace=0");
			gtk_check_menu_item_set_active (GTK_CHECK_MENU_ITEM(app->widget.pMenuDeinterlaceDisable), TRUE);
		}

		gtk_libvlc_media_player_clear_media_list(app->player);
		gtk_libvlc_media_player_add_media(app->player, media);
		g_object_unref(media);

		gchar **options = NULL;
		list_options_add_option (&options, ":access=timeshift");
		list_options_add_option (&options, ":input-timeshift-granularity=50");

		// Manual mode for proxy
		if(app->prefs.proxy.proxy_mode == G_PROXY_MODE_MANUAL){
			gchar* szUser = NULL;

			if(g_strcasecmp ("0", app->prefs.proxy.proxy_type) == 0){
				// Mode HTTP
				if(app->prefs.proxy.proxy_use_auth){
					szUser = g_strconcat(app->prefs.proxy.proxy_username, "@", NULL);
				}else{
					szUser = g_strdup("");
				}
				text = g_strdup_printf(":http-proxy=http://%s%s:%s/", szUser,
				                       app->prefs.proxy.proxy_server, app->prefs.proxy.proxy_port);
				list_options_add_option(&options, text);
				g_free(text);
				text = NULL;
				if(szUser){
					g_free(szUser);
					szUser = NULL;
				}
				if(app->prefs.proxy.proxy_use_auth){
					text = g_strdup_printf(":http-proxy-pwd=%s", app->prefs.proxy.proxy_password);
					list_options_add_option(&options, text);
					g_free(text);
					text = NULL;
				}	
			}else if(g_strcasecmp ("1", app->prefs.proxy.proxy_type) == 0){
				// Mode SOCK
				text = g_strdup_printf(":socks=%s:%s", app->prefs.proxy.proxy_server, app->prefs.proxy.proxy_port);
				list_options_add_option(&options, text);
				g_free(text);
				
				if(app->prefs.proxy.proxy_use_auth){
					text = g_strdup_printf(":socks-user=%s", app->prefs.proxy.proxy_username);
					list_options_add_option(&options, text);
					g_free(text);
					text = NULL;

					text = g_strdup_printf(":socks-pwd=%s", app->prefs.proxy.proxy_password);
					list_options_add_option(&options, text);
					g_free(text);
					text = NULL;
				}
			}
		}

		gtk_libvlc_media_player_play(app->player, options, error);

		list_options_free(options);
		options = NULL;
	}
	g_log(FREETUXTV_LOG_DOMAIN, G_LOG_LEVEL_DEBUG,
	      "end freetuxtv_play_channel(%s)\n", pChannelInfos->name);
}

void
freetuxtv_play_media (FreetuxTVApp *app, GtkLibvlcMedia* media, GError** error)
{
	g_log(FREETUXTV_LOG_DOMAIN, G_LOG_LEVEL_DEBUG,
	      "freetuxtv_play_media(%s)\n", media->mrl);

	windowmain_display_buttons (app, WINDOW_MODE_PLAYING);

	channels_list_set_playing(app, NULL);

	gtk_libvlc_media_player_clear_media_list(app->player);
	gtk_libvlc_media_player_add_media(app->player, media);
	g_object_unref(media);
	gtk_libvlc_media_player_play(app->player, NULL, error);

	g_log(FREETUXTV_LOG_DOMAIN, G_LOG_LEVEL_DEBUG,
	      "end freetuxtv_play_media(%s)\n", media->mrl);
}

void
freetuxtv_action_playpause (FreetuxTVApp *app, GError** error)
{
	g_log(FREETUXTV_LOG_DOMAIN, G_LOG_LEVEL_DEBUG,
	      "freetuxtv_action_playpause()\n");

	FreetuxTVChannelInfos* pChannelInfos;

	GtkLibvlcState state = gtk_libvlc_media_player_get_state(app->player, error);
	switch(state){
		case GTK_LIBVLC_STATE_PAUSED :
			gtk_libvlc_media_player_pause(app->player, NULL);
			windowmain_display_buttons (app, WINDOW_MODE_PLAYING);
			break;
		case GTK_LIBVLC_STATE_PLAYING :
			if(gtk_libvlc_media_player_can_pause(app->player, NULL)){
				gtk_libvlc_media_player_pause(app->player, NULL);
			}
			windowmain_display_buttons (app, WINDOW_MODE_PAUSED);
			break;
		default:
			if(app->current.pPathChannel != NULL){
				pChannelInfos = channels_list_get_channel (app, app->current.pPathChannel);

				g_log(FREETUXTV_LOG_DOMAIN, G_LOG_LEVEL_DEBUG,
					  "Current channel %s\n", pChannelInfos->name);
				GtkLibvlcMedia *media;
				media = gtk_libvlc_media_new(pChannelInfos->url);
				gtk_libvlc_media_player_clear_media_list(app->player);
				gtk_libvlc_media_player_add_media(app->player, media);
				g_object_unref(media);
				gtk_libvlc_media_player_play(app->player, NULL, error);

				windowmain_display_buttons (app, WINDOW_MODE_PLAYING);
			}
	}

	g_log(FREETUXTV_LOG_DOMAIN, G_LOG_LEVEL_DEBUG,
	      "end freetuxtv_action_playpause()\n");
}

void
freetuxtv_action_stop (FreetuxTVApp *app, GError** error)
{
	g_log(FREETUXTV_LOG_DOMAIN, G_LOG_LEVEL_DEBUG,
	      "freetuxtv_action_stop()\n");

	gchar *text;

	GtkLibvlcState state = gtk_libvlc_media_player_get_state(app->player, error);
	if(state == GTK_LIBVLC_STATE_PLAYING || state == GTK_LIBVLC_STATE_PAUSED){
		if(app->current.pPathChannel != NULL){
			FreetuxTVChannelInfos* channel_infos;
			channel_infos = channels_list_get_channel (app, app->current.pPathChannel);

			if(app->current.is_recording){
				text = g_strdup_printf (_("Playing: %s"), channel_infos->name);
			}else{
				text = g_strdup_printf (_("Stopping channel: %s"), channel_infos->name);
			}
			windowmain_statusbar_push (app, "PlayChannelMsg", text);
			g_free(text);
		}

		windowmain_display_buttons (app, WINDOW_MODE_STOPPED);
		gtk_libvlc_media_player_stop (app->player, error);
	}

	g_log(FREETUXTV_LOG_DOMAIN, G_LOG_LEVEL_DEBUG,
	      "end freetuxtv_action_stop()\n");
}

void
freetuxtv_action_start_recording (FreetuxTVApp *app, FreetuxTVRecordingInfos* pRecordingInfos, GError** error)
{
	g_return_if_fail(error != NULL);
	g_return_if_fail(*error == NULL);

	g_log(FREETUXTV_LOG_DOMAIN, G_LOG_LEVEL_MESSAGE,
	      "Starting recording => %s\n", pRecordingInfos->szTitle);

	DBSync dbsync;
	
//	FREETUXTV_RECORDING_STATUS oldstatus;
	FreetuxTVChannelInfos* pChannelInfos;

	gchar *szTmp;
	gchar *szText;
	gchar *szImgFile;
	gchar *szBaseFilename;
	gchar *szDstFilename;

	if(!app->current.is_recording) {
		g_get_current_time (&(app->current.recording.time_begin));

		pChannelInfos = channels_list_get_channel_by_id(app, pRecordingInfos->channel_id);

		// Send notification to desktop
		szImgFile = tvchannels_list_get_tvchannel_logo_path_for_channel(app, pChannelInfos, TRUE);

		if(app->prefs.enable_notifications){
		   notify_notification_update (app->current.notification, pChannelInfos->name,
		       _("is recording"), szImgFile);
		   if (!notify_notification_show (app->current.notification, NULL)) {
			   g_log(FREETUXTV_LOG_DOMAIN, G_LOG_LEVEL_WARNING,
			       "Failed to send notification\n");
		   }
		}
		g_free(szImgFile);

		windowmain_display_buttons (app, WINDOW_MODE_RECORDING);

		// File name

		// Get the current date string
		time_t rawtime;
		struct tm * timeinfo;
		char buffer [80];

		time (&rawtime);
		timeinfo = localtime ( &rawtime );

		strftime (buffer, 80, "%a %Y-%m-%d %H:%M:%S", timeinfo);
		
		szBaseFilename = g_strconcat(pChannelInfos->name, " - ", buffer, NULL);

		gchar *options[2];
		options[0] = get_recording_options(app, szBaseFilename, FALSE, &szDstFilename);
		options[1] = NULL;

		// TODO : if not the current channel, we create a new media 
		if(TRUE){
			GtkLibvlcMedia *media;
			media = gtk_libvlc_media_new(pChannelInfos->url);
			gtk_libvlc_media_player_clear_media_list(app->player);
			gtk_libvlc_media_player_add_media(app->player, media);
			g_object_unref(media);
	    }
		
		gtk_libvlc_media_player_play (app->player, options, error);

		g_free(szBaseFilename);
		g_free(options[0]);

		// Mark the recording as the current recording
		app->current.is_recording = TRUE;
		app->current.recording.pRecordingInfo = pRecordingInfos;

		szTmp = g_path_get_basename (szDstFilename);
		szText = g_strdup_printf (_("Recording: %s"), szTmp);
		windowmain_statusbar_push (app, "PlayChannelMsg", szText);
		g_free(szTmp);
		g_free(szText);

		// Update recording in database
		dbsync_open_db (&dbsync, error);
		if(*error == NULL){
			freetuxtv_recording_infos_set_filename (pRecordingInfos, szDstFilename);
			freetuxtv_recording_infos_set_status (pRecordingInfos, FREETUXTV_RECORDING_STATUS_PROCESSING);
			dbsync_update_recording (&dbsync, pRecordingInfos, error);
		}
		if(*error == NULL){
			recordings_list_refresh (app);
		}
		dbsync_close_db(&dbsync);

		g_free(szDstFilename);
		szDstFilename = NULL;
	}
}


void
freetuxtv_action_stop_recording (FreetuxTVApp *app, FreetuxTVRecordingInfos* pRecordingInfos, GError** error)
{
	g_return_if_fail(error != NULL);
	g_return_if_fail(*error == NULL);

	g_log(FREETUXTV_LOG_DOMAIN, G_LOG_LEVEL_MESSAGE,
	      "Stopping recording => %s\n", pRecordingInfos->szTitle);

	DBSync dbsync;
	
	freetuxtv_action_stop(app, error);

	// Update recording in database
	if(*error == NULL){
		dbsync_open_db (&dbsync, error);
		if(*error == NULL){
			freetuxtv_recording_infos_set_status (pRecordingInfos, FREETUXTV_RECORDING_STATUS_FINISHED);
			dbsync_update_recording (&dbsync, pRecordingInfos, error);
		}
		if(*error == NULL){
			recordings_list_refresh (app);
		}
		dbsync_close_db(&dbsync);
	}

	app->current.recording.pRecordingInfo = NULL;
	app->current.is_recording = FALSE;

	freetuxtv_action_playpause (app, error);
}

void
freetuxtv_action_prev (FreetuxTVApp *app, GError** error)
{
	g_log(FREETUXTV_LOG_DOMAIN, G_LOG_LEVEL_DEBUG,
	      "freetuxtv_action_prev()\n");

	gboolean ret;
	GtkTreePath* path_prev_channel;
	if (app->current.pPathChannel != NULL) {
		if(!app->current.is_recording){
			ret = channels_list_get_prev_channel (app, &path_prev_channel);
			if(ret){			
				freetuxtv_play_channel (app, path_prev_channel, error);
			}
		}
	}
}

void
freetuxtv_action_next (FreetuxTVApp *app, GError** error)
{
	g_log(FREETUXTV_LOG_DOMAIN, G_LOG_LEVEL_DEBUG,
	      "freetuxtv_action_next()\n");

	gboolean ret;
	GtkTreePath* path_next_channel;
	if (app->current.pPathChannel != NULL) {
		if(!app->current.is_recording){
			ret = channels_list_get_next_channel (app, &path_next_channel);
			if(ret){
				freetuxtv_play_channel (app, path_next_channel, error);
			}
		}
	}
}

void
freetuxtv_action_deinterlace (FreetuxTVApp *app, const gchar* mode,
                              GError** error)
{
	g_log(FREETUXTV_LOG_DOMAIN, G_LOG_LEVEL_DEBUG,
	      "freetuxtv_action_deinterlace()\n");

	 gtk_libvlc_media_player_set_deinterlace (app->player, mode, error);
}

gboolean
freetuxtv_quit (FreetuxTVApp *app, GtkWindow* parent)
{
	GError *error = NULL;

	g_log(FREETUXTV_LOG_DOMAIN, G_LOG_LEVEL_DEBUG,
	      "freetuxtv_action_quit()\n");

	GKeyFile *keyfile;
	char *contents, *filename;

	GtkWidget* dialog = NULL;

	gboolean bQuit = TRUE;

	gboolean bIsPlaying;
	bIsPlaying = (gtk_libvlc_media_player_get_state(app->player, &error) == GTK_LIBVLC_STATE_PLAYING);

	if(bIsPlaying && app->current.is_recording){
		bQuit = FALSE;

		dialog = gtk_message_dialog_new (parent,
		                                 GTK_DIALOG_MODAL, GTK_MESSAGE_QUESTION,
		                                 GTK_BUTTONS_YES_NO,
		                                 _("Are you sure you want to quit?"));
		if(gtk_dialog_run (GTK_DIALOG(dialog)) == GTK_RESPONSE_YES){
			bQuit = TRUE;
		}

		gtk_widget_destroy(dialog);
		dialog = NULL;
		
	}

	if(bQuit){
		// Stop the current channel
		freetuxtv_action_stop(app, &error);	

		keyfile = g_key_file_new ();

		// Save prefs
		g_key_file_set_boolean (keyfile, "general",
			                    "channel_on_startup",
			                    app->prefs.channelonstartup);
		g_key_file_set_boolean (keyfile, "general",
			                    "enable_notifications",
			                    app->prefs.enable_notifications);
		g_key_file_set_boolean (keyfile, "general",
			                    "ignore_diacritics",
			                    app->prefs.ignore_diacritics);

		g_key_file_set_integer (keyfile, "libvlc",
			                    "configfile_mode",
			                    app->prefs.libvlcconfigfile_mode);
		
		g_key_file_set_string (keyfile, "general",
			                   "directory_record",
			                   app->prefs.directoryrecordings);

		g_key_file_set_integer (keyfile, "general",
			                    "transcoding_mode",
			                    app->prefs.transcoding_mode);

		g_key_file_set_string (keyfile, "general",
			                   "transcoding_format",
			                   app->prefs.transcoding_format);

		g_key_file_set_integer (keyfile, "network",
			                    "timeout",
			                    app->prefs.timeout);
		g_key_file_set_integer (keyfile, "network",
			                    "proxy_mode",
			                    app->prefs.proxy.proxy_mode);
		g_key_file_set_string (keyfile, "network",
			                   "proxy_server",
			                   app->prefs.proxy.proxy_server);
		g_key_file_set_string (keyfile, "network",
			                   "proxy_port",
			                   app->prefs.proxy.proxy_port);
		g_key_file_set_string (keyfile, "network",
			                   "proxy_type",
			                   app->prefs.proxy.proxy_type);
		g_key_file_set_boolean (keyfile, "network",
			                    "proxy_use_auth",
			                    app->prefs.proxy.proxy_use_auth);
		g_key_file_set_string (keyfile, "network",
			                   "proxy_username",
			                   app->prefs.proxy.proxy_username);
		g_key_file_set_string (keyfile, "network",
			                   "proxy_password",
			                   app->prefs.proxy.proxy_password);

		// Save current config
		g_key_file_set_double (keyfile, "general",
			                   "volume",
			                   app->config.volume);


		if(app->current.pPathChannel != NULL && bIsPlaying){
			FreetuxTVChannelInfos* channel_infos;
			channel_infos = channels_list_get_channel(app, app->current.pPathChannel);
			g_key_file_set_integer (keyfile, "general",
				                    "last_channel",
				                    channel_infos->id);
			gtk_tree_path_free(app->current.pPathChannel);
		}
		g_key_file_set_integer (keyfile, "general",
			                    "logos_file_date",
			                    app->config.logosfiledate);

		//g_key_file_set_boolean (keyfile, "windowminimode",
		//    "stay_on_top",
		//    app->config.windowminimode_stayontop);
		g_key_file_set_integer (keyfile, "windowminimode",
			                    "width",
			                    app->config.windowminimode_width);
		g_key_file_set_integer (keyfile, "windowminimode",
			                    "height",
			                    app->config.windowminimode_height);

		contents = g_key_file_to_data (keyfile, NULL, NULL);
		g_key_file_free (keyfile);
		filename = g_build_filename (g_get_user_config_dir(),
			                         "FreetuxTV", "config.ini", NULL);
		g_log(FREETUXTV_LOG_DOMAIN, G_LOG_LEVEL_INFO,
			  "Writing config file %s\n", filename);
		if (!g_file_set_contents (filename, contents, -1, NULL)){
			g_log(FREETUXTV_LOG_DOMAIN, G_LOG_LEVEL_CRITICAL,
				  "Error when writing config file\n");
		}

		g_free (filename);
		g_free (contents);

		// Dispose the app
		windowmain_dispose (app);

		if(error != NULL){
			windowmain_show_gerror (app, error);
			g_error_free (error);
		}

		gtk_main_quit();
	}

	return bQuit;
}

static void
on_freetuxtv_mm_key_pressed (GMMKeys *mmkeys, GMMKeysButton button, FreetuxTVApp* app)
{
	GError* error = NULL;

	g_log(FREETUXTV_LOG_DOMAIN, G_LOG_LEVEL_DEBUG,
	      "on_freetuxtv_mm_key_pressed()\n");

	switch(button){
		case GMMKEYS_BUTTON_PLAY :
		case GMMKEYS_BUTTON_PAUSE :
			freetuxtv_action_playpause (app, &error);		
			break;
		case GMMKEYS_BUTTON_STOP :
			freetuxtv_action_stop (app, &error);
			break;
		case GMMKEYS_BUTTON_PREV :
			freetuxtv_action_prev (app, &error);
			break;
		case GMMKEYS_BUTTON_NEXT :
			freetuxtv_action_next (app, &error);
			break;
	}

	if(error){
		g_error_free (error);
		error = NULL;
	}
}

void
freetuxtv_log (const gchar *log_domain, GLogLevelFlags log_level,
               const gchar *message, gpointer user_data)
{
	gboolean debug = (gboolean)(user_data ? *((gboolean*)user_data) : FALSE);
	const gchar* levelmsg = "";
	gchar* szDomain;

	gboolean bPrint = TRUE;

	switch(log_level){
		case G_LOG_LEVEL_ERROR :
			levelmsg = "ERROR";
			break;
		case G_LOG_LEVEL_CRITICAL :
			levelmsg = "CRITICAL";
			break;
		case G_LOG_LEVEL_WARNING :
			levelmsg = "WARNING";
			break;
		case G_LOG_LEVEL_MESSAGE :
			levelmsg = "MESSAGE";
			break;
		case G_LOG_LEVEL_INFO :
			levelmsg = "INFO";
			break;
		case G_LOG_LEVEL_DEBUG :
			levelmsg = "DEBUG";
			if(!debug){
				bPrint = FALSE;
			}
			break;
		default :
			break;
	}
	if(bPrint){
		if(log_domain){
			szDomain = g_strconcat("[", log_domain, "]", NULL);
		}else{
			szDomain = g_strdup("");
		}
		g_print("%-15s%-10s : %s", szDomain, levelmsg, message);
		if(szDomain){
			g_free(szDomain);
			szDomain = NULL;
		}
	}
}

static void
show_help()
{
	g_print("FreetuxTV %s (Compiled with LibVLC version %s)\n", VERSION, gtk_libvlc_get_libvlc_version(NULL,NULL,NULL));
	g_print("Usage: freetuxtv [OPTION...]\n");
	g_print("\n");
	g_print("Options:\n");
	g_print("  -h, --help        display this help and exit\n");
	g_print("      --version     output version information and exit\n");
	g_print("\n");
	g_print("Channel:\n");
	g_print("  --open-channel CHANNEL_NAME\n");
	g_print("                    look for a channel corresponding to the name and \n");
	g_print("                    play it if found (take first in the channels list)\n");
	g_print("\n");
	g_print("Debug options:\n");
	g_print("      --datadir DATA_DIR\n");
	g_print("                    set data directory\n");
	g_print("  -v, --verbose     display a more detailled trace\n");
	g_print("\n");
	g_print("Report bugs to %s.\n", PACKAGE_BUGREPORT);
}

static void
show_version()
{
	g_print("FreetuxTV %s (Compiled with LibVLC version %s)\n", VERSION, gtk_libvlc_get_libvlc_version(NULL,NULL,NULL));
	g_print("\n");
	g_print("Copyright (C) Eric Beuque 2010 <eric.beuque@gmail.com>\n");
	g_print("This is free software; see the source for copying conditions.  There is NO\n");
	g_print("warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.\n");
    g_print("\n");
    g_print("Written by Eric Beuque\n");
}

static void
show_error(gchar *szErrMsg)
{
	g_printerr("FreetuxTV %s (Compiled with LibVLC version %s)\n", VERSION, gtk_libvlc_get_libvlc_version(NULL,NULL,NULL));
	g_printerr("freetuxtv: %s\n", szErrMsg);
	g_printerr("Try `freetuxtv --help' for more information.\n");
}

int
main (int argc, char *argv[])
{	
	FreetuxTVApp *app;

	GMMKeys* mmkeys;

	gboolean bStopParseArgs = FALSE;
	gboolean bQuit = FALSE;
	gchar*   szErrMsg = NULL;
	
	gboolean bShowHelp = FALSE;
	gboolean bShowVersion = FALSE;
	gboolean bTraceDebug = FALSE;
	gboolean bOpenChannel = FALSE;
//	gchar* szUri;
	gchar* szChannelName = NULL;
	gchar* szDataDir = NULL;

	int p;
	int idLogHandler;

	// Process programs args
	if(argc>1){
		for(p=1; p<argc && !bStopParseArgs; p++){
			if(argv[p][0] == '-'){
				// This should be an option
				if(g_ascii_strcasecmp("-h", argv[p]) == 0 || g_ascii_strcasecmp("--help", argv[p]) == 0){
					// Found the show help option
					bShowHelp = TRUE;
					bStopParseArgs = TRUE;
				}else if(g_ascii_strcasecmp("--version", argv[p]) == 0){
					// Found the version option
					bShowVersion = TRUE;
					bStopParseArgs = TRUE;
				}else if(g_ascii_strcasecmp("-v", argv[p]) == 0 || g_ascii_strcasecmp("--verbose", argv[p]) == 0){
					// Found the verbose option
					bTraceDebug = TRUE;
				}else if(g_ascii_strcasecmp("--open-channel", argv[p]) == 0){
					// Found the open channel option
					if(p+1<argc){
						bOpenChannel = TRUE;
						szChannelName = argv[p+1];
					}else{
						szErrMsg = g_strdup_printf("missing mandatory argument to '%s", argv[p]);
						bStopParseArgs = FALSE;
					}
				}else if(g_ascii_strcasecmp("--datadir", argv[p]) == 0){
					// Found the datadir option
					if(p+1<argc){
						szDataDir = argv[p+1];
					}else{
						szErrMsg = g_strdup_printf("missing mandatory argument to '%s", argv[p]);
						bStopParseArgs = FALSE;
					}
				}else{
					szErrMsg = g_strdup_printf("unknown option '%s'", argv[p]);
					bStopParseArgs = FALSE;
				}
			}else{
				if(p == argc-1){
					// If the last args isn't an option, it should be an URI 
					// TODO : Allow to play the URI
					// szUri = argv[p];
				}else{
					szErrMsg = g_strdup_printf("invalid option '%s'", argv[p]);
				}
			}
		}
	}

	if(szErrMsg){
		bQuit = TRUE;
		show_error (szErrMsg);
		g_free(szErrMsg);
		szErrMsg = NULL;
	}

	if(bShowHelp){
		show_help();
		bQuit = TRUE;
	}

	if(bShowVersion){
		show_version();
		bQuit = TRUE;
	}

	if(!bQuit){
		// Initialize log handler
		idLogHandler = g_log_set_handler (FREETUXTV_LOG_DOMAIN,
			                              G_LOG_LEVEL_MASK,
			                              freetuxtv_log, &bTraceDebug);

#ifdef ENABLE_NLS
		bindtextdomain (GETTEXT_PACKAGE, PACKAGE_LOCALE_DIR);
		bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
		textdomain (GETTEXT_PACKAGE);
#endif

		g_log(FREETUXTV_LOG_DOMAIN, G_LOG_LEVEL_INFO,
		    "Compiled with GTK%d\n", GTK_API_VERSION);		
		g_log(FREETUXTV_LOG_DOMAIN, G_LOG_LEVEL_INFO,
		    "Compiled with LibVLC version %s\n", gtk_libvlc_get_libvlc_version(NULL,NULL,NULL));
		g_log(FREETUXTV_LOG_DOMAIN, G_LOG_LEVEL_INFO,
		    "Compiled with libnotify version %d.%d.%d\n", LIBNOTIFY_VERSION_MAJOR, LIBNOTIFY_VERSION_MINOR, LIBNOTIFY_VERSION_REVISION);

#if GTK_API_VERSION == 3
		setlocale(LC_ALL, "");
#else
		gtk_set_locale ();
#endif
		gtk_init (&argc, &argv);

		g_log(FREETUXTV_LOG_DOMAIN, G_LOG_LEVEL_INFO,
		    "Loading FreetuxTV %s\n", VERSION);
		app = freetuxtv_app_create_app (szDataDir);
		if (app != NULL) {

			// Initialize notifications
			notify_init("FreetuxTV");

#if LIBNOTIFY_VERSION_MAJOR == 0 && LIBNOTIFY_VERSION_MINOR < 7
			app->current.notification = notify_notification_new ("FreetuxTV", NULL, NULL, NULL);
#else
			app->current.notification = notify_notification_new ("FreetuxTV", NULL, NULL);
#endif

			mmkeys = g_mmkeys_new ("FreetuxTV", freetuxtv_log);
			g_mmkeys_activate (mmkeys);

			g_signal_connect(G_OBJECT(mmkeys),
				             "mm_key_pressed",
				             G_CALLBACK(on_freetuxtv_mm_key_pressed),
				             app);

			// Set the channel name to open
			if(bOpenChannel){
				app->current.open_channel_name = szChannelName;
			}
			
#if GTK_API_VERSION == 3
			g_idle_add(splashscreen_app_init, app);
#else
			gtk_init_add (splashscreen_app_init, app);
#endif

			gtk_main ();

			g_mmkeys_deactivate (mmkeys);
			g_object_unref(G_OBJECT(app->current.notification));
			notify_uninit();

			freetuxtv_app_destroy_app (&app);
			app = NULL;
		}

		g_log_remove_handler (FREETUXTV_LOG_DOMAIN, idLogHandler);
	}

	return 0;
}
