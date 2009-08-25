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
#include "gtk-libvlc-media-player.h"

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
	app->config.volume = 0.75;
	app->config.logosfiledate = 0;
	app->config.directoryrecord = g_strdup(g_get_home_dir());
	app->current.path_channel = NULL;
	app->current.lastchannelonstartup = FALSE;
	app->current.recording.dst_file = NULL;
	app->current.recording.duration = NULL;
	app->debug = FALSE;
	

	/* Création de la fenêtre */
	app->windowmain = glade_xml_new (FREETUXTV_GLADEXML,
					 "windowmain", NULL);
	glade_xml_signal_autoconnect (app->windowmain);

	/* Ajout du widget du lecteur */	
	eventboxplayer = glade_xml_get_widget (app->windowmain,
					       "windowmain_eventboxplayer");
	GtkLibVLCInstance* instance;
	instance = gtk_libvlc_instance_new(NULL);
	app->player = GTK_LIBVLC_MEDIA_PLAYER(gtk_libvlc_media_player_new(instance));
	gtk_widget_show(GTK_WIDGET(app->player));
	g_object_unref(G_OBJECT(instance));
	gtk_container_add (GTK_CONTAINER(eventboxplayer), GTK_WIDGET(app->player));
	
	/* Initialise la liste des chaines */
	channels_list_init (app);
	/* Connexion des signaux */
	widget = glade_xml_get_widget (app->windowmain,
				       "windowmain");
	
	widget = glade_xml_get_widget (app->windowmain,
				       "windowmain");
	g_signal_connect(G_OBJECT(widget),
			 "delete-event",
			 G_CALLBACK(on_windowmain_deleteevent),
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
				       "windowmain_buttongotocurrent");
	g_signal_connect(G_OBJECT(widget),
			 "clicked",
			 G_CALLBACK(on_windowmain_buttongotocurrent_clicked),
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
			widget = glade_xml_get_widget (app->windowmain,
						       "windowmain_volumecontrol");
			gtk_range_set_value (GTK_RANGE(widget), app->config.volume);
			gtk_libvlc_media_player_set_volume (app->player, app->config.volume);
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
freetuxtv_action_play_channel (FreetuxTVApp *app, GtkTreePath* path_channel)
{
	FreetuxTVChannelInfos *channel_infos;

	channel_infos = channels_list_get_channel(app, path_channel);
	
	if(app->debug){
		g_print("FreetuxTV-debug : freetuxtv_action_play_channel(%s)\n", channel_infos->name);
	}
	gchar *text;
	if(!gtk_libvlc_media_player_is_recording(app->player)){

		channels_list_set_playing(app, path_channel);
		g_print("FreetuxTV-debug : Launching channel '%s' -> %s\n", channel_infos->name, channel_infos->url);

		text = g_strdup_printf (_("Playing : %s"), channel_infos->name);
		windowmain_statusbar_push (app, "PlayChannelMsg", text);
		
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
		
		GtkLibVLCMedia *media;
		media = gtk_libvlc_media_new(channel_infos->url);
		gtk_libvlc_media_player_clear_media_list(app->player);
		gtk_libvlc_media_player_add_media(app->player, media);
		g_object_unref(media);
		gtk_libvlc_media_player_play(app->player);
	}
	if(app->debug){
		g_print("FreetuxTV-debug : end freetuxtv_action_play_channel(%s)\n", channel_infos->name);
	}
}

void
freetuxtv_action_playpause_channel (FreetuxTVApp *app)
{	
	if(app->debug){
		g_print("FreetuxTV-debug : freetuxtv_action_playpause_channel()\n");
	}
	if(app->current.path_channel != NULL){
		FreetuxTVChannelInfos* channel_infos;
		channel_infos = channels_list_get_channel (app, app->current.path_channel);

		g_print("FreetuxTV-debug : current channel %s\n", channel_infos->name);
		GtkLibVLCState state = gtk_libvlc_media_player_get_state(app->player);
		if(state != GTK_LIBVLC_STATE_PLAYING){
			GtkLibVLCMedia *media;
			media = gtk_libvlc_media_new(channel_infos->url);
			gtk_libvlc_media_player_clear_media_list(app->player);
			gtk_libvlc_media_player_add_media(app->player, media);
			g_object_unref(media);
			gtk_libvlc_media_player_play(app->player);
		}
		windowmain_display_buttons (app, WINDOW_MODE_PLAYING);
	}	
	if(app->debug){
		g_print("FreetuxTV-debug : exit freetuxtv_action_playpause_channel()\n");
	}
}

void
freetuxtv_action_stop_channel (FreetuxTVApp *app)
{
	if(app->debug){
		g_print("FreetuxTV-debug : freetuxtv_action_stop_channel()\n");
	}
	gchar *text;
	if(app->current.path_channel != NULL){
		FreetuxTVChannelInfos* channel_infos;
		channel_infos = channels_list_get_channel (app, app->current.path_channel);
		
		if(gtk_libvlc_media_player_get_state(app->player) == GTK_LIBVLC_STATE_PLAYING){

			gboolean recording;
			recording = gtk_libvlc_media_player_is_recording(app->player);
			if(recording){
				text = g_strdup_printf (_("Playing : %s"), channel_infos->name);
			}else{
				text = g_strdup_printf (_("Stopping channel : %s"), channel_infos->name);
			}
			windowmain_statusbar_push (app, "PlayChannelMsg", text);
			g_free(text);
	
			windowmain_display_buttons (app, WINDOW_MODE_STOPPED);
			gtk_libvlc_media_player_stop (app->player);

			if(recording){
				freetuxtv_action_playpause_channel (app);				
			}
		}
	}
	if(app->debug){
		g_print("FreetuxTV-debug : exit freetuxtv_action_stop_channel()\n");
	}
}

void
freetuxtv_action_record_channel (FreetuxTVApp *app)
{
	if(app->debug){
		g_print("FreetuxTV-debug : freetuxtv_action_record_channel()\n");
	}
	gchar *text;

	if(gtk_libvlc_media_player_get_state(app->player) == GTK_LIBVLC_STATE_PLAYING
	   && !gtk_libvlc_media_player_is_recording(app->player)){
		
		FreetuxTVChannelInfos* channel_infos;
		channel_infos = channels_list_get_channel (app, app->current.path_channel);

		// Send notification to desktop
		gchar *imgfile;
		imgfile = logos_list_get_channel_logo_filename(app, channel_infos, TRUE);
		
		notify_notification_update (app->current.notification, channel_infos->name,
					    _("is recording"), imgfile);
		if (!notify_notification_show (app->current.notification, NULL)) {
			g_printerr("FreetuxTV : Failed to send notification\n");
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

                app->current.recording.dst_file = g_strconcat(app->config.directoryrecord, "/", channel_infos->name, " - ",
							      g_time_val_to_iso8601(&now), ".mpg", NULL);
		gtk_libvlc_media_player_record_current (app->player, app->current.recording.dst_file);
		
		text = g_strdup_printf (_("Recording : %s"), app->current.recording.dst_file);
		windowmain_statusbar_push (app, "PlayChannelMsg", text);
		g_free(text);
	}
}

void
freetuxtv_action_prev_channel (FreetuxTVApp *app)
{
	if(app->debug){
		g_print("FreetuxTV-debug : freetuxtv_action_prev_channel()\n");
	}
	gboolean ret;
	GtkTreePath* path_prev_channel;
	if (app->current.path_channel != NULL) {
		if(!gtk_libvlc_media_player_is_recording(app->player)){
			ret = channels_list_get_prev_channel (app, &path_prev_channel);
			if(ret){			
				freetuxtv_action_play_channel(app, path_prev_channel);
			}
		}
	}
}

void
freetuxtv_action_next_channel (FreetuxTVApp *app)
{
	if(app->debug){
		g_print("FreetuxTV-debug : freetuxtv_action_next_channel()\n");
	}
	gboolean ret;
	GtkTreePath* path_next_channel;
	if (app->current.path_channel != NULL) {
		if(!gtk_libvlc_media_player_is_recording(app->player)){
			ret = channels_list_get_next_channel (app, &path_next_channel);
			if(ret){
				freetuxtv_action_play_channel(app, path_next_channel);
			}
		}
	}
}

void
freetuxtv_action_quit (FreetuxTVApp *app)
{
	if(app->debug){
		g_print("FreetuxTV-debug : freetuxtv_action_quit()\n");
	}
	GKeyFile *keyfile;
	char *contents, *filename;

	// Stop the current channel
	freetuxtv_action_stop_channel(app);
	
	// Save FreetuxTV state
	keyfile = g_key_file_new ();	
	
	g_key_file_set_double (keyfile, "general",
			       "volume",
			       app->config.volume);
	g_key_file_set_boolean (keyfile, "general",
				"channel_on_startup",
				app->config.channelonstartup);
	if(app->current.path_channel != NULL){
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


static gchar*
format_time(gint seconds)
{
	const gint s = seconds % 60;
	const gint m = (seconds - s) / 60;
	const gint h = (seconds - m) / (60*60);

	return g_strdup_printf(_("%02dh%02dm%02ds"), h, m, s);
}

static gchar*
format_size(glong size)
{
	if(size < 1){
		return g_strdup_printf(_("%ld byte"), size);
	}else if(size < 1000){
		return g_strdup_printf(_("%ld bytes"), size);
	}else if(size < 1000000){
		return g_strdup_printf(_("%1.1f kB"), size/1000.0);		
	}else if(size < 1000000000){
		return g_strdup_printf(_("%1.1f MB"), size/1000000.0);	
	}else{
		return g_strdup_printf(_("%1.1f GB"), size/1000000000.0);		
	}
}

gboolean
increase_progress_timeout (FreetuxTVApp *app)
{
	if(gtk_libvlc_media_player_is_recording (app->player)){	
		if(app->current.recording.duration != NULL){

			FreetuxTVChannelInfos* channel;
			channel = channels_list_get_channel (app, app->current.path_channel);

			gint second;
			second = (gint)g_timer_elapsed (app->current.recording.duration, NULL);

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
	
	g_print("FreetuxTV : Load channels list\n", LIBVLC_VERSION_MAJOR, LIBVLC_VERSION_MINOR);
	channels_list_load_channels (app);
	app->current.lastchannelonstartup = FALSE;

	// Scroll to the current channel
	if(app->current.path_channel != NULL){
		channels_list_set_playing (app, app->current.path_channel);
	}
	
	mmkeys = g_mmkeys_new ("FreetuxTV");
	g_mmkeys_activate (mmkeys);
	
	g_signal_connect(G_OBJECT(mmkeys),
			 "mm_key_pressed",
			 G_CALLBACK(on_freetuxtv_mm_key_pressed),
			 app);
	
	// Pour les traitements à interval régulier
	g_timeout_add(1000, (GSourceFunc) increase_progress_timeout, app);

	gtk_main();
	
	g_mmkeys_deactivate (mmkeys);
	g_object_unref(G_OBJECT(app->current.notification));
	notify_uninit();  

	return 0;
}
