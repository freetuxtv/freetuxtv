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

#include "freetuxtv-channel.h"
#include "freetuxtv-main-window.h"
#include "freetuxtv-player.h"

G_DEFINE_TYPE (FreetuxTVChannel, freetuxtv_channel, GTK_TYPE_EVENT_BOX);

static void
on_channel_button_press_event (GtkWidget *widget, GdkEventButton *event, 
			       gpointer user_data);

static void
on_channel_notify_event (GtkWidget *widget, GdkEventMotion *event, 
			 gpointer user_data);

static void
freetuxtv_channel_modify_bg (FreetuxTVChannel *widget, 
			     gint red, gint green, gint blue);

GtkWidget *
freetuxtv_channel_new (gchar *name, gchar *uri)
{
	FreetuxTVChannel *self = NULL;
	self = gtk_type_new (freetuxtv_channel_get_type ());
	
	self->name = g_strdup(name);
	self->uri = g_strdup(uri);
	
	/* Evenemment du widget */
	g_signal_connect(G_OBJECT(self),
			 "button-press-event",
			 G_CALLBACK(on_channel_button_press_event),
			 NULL);
	g_signal_connect(G_OBJECT(self),
			 "enter-notify-event",
			 G_CALLBACK(on_channel_notify_event),
			 NULL);
	g_signal_connect(G_OBJECT(self),
			 "leave-notify-event",
			 G_CALLBACK(on_channel_notify_event),
			 NULL);
	
	freetuxtv_channel_modify_bg (self, 0xff, 0xff, 0xff);
	
	GtkWidget *hbox;
	hbox = gtk_hbox_new(FALSE, 0);
	gtk_container_add (GTK_CONTAINER(self), hbox);
	
	/* Ajout du logo */
	self->logo = gtk_image_new ();
	gtk_misc_set_padding (GTK_MISC(self->logo),2,2);
	gtk_box_pack_start (GTK_BOX(hbox), self->logo, FALSE, FALSE, 0);
	
	GtkWidget *vbox;
	vbox = gtk_vbox_new(FALSE, 0);
	gtk_box_pack_start (GTK_BOX(hbox), vbox, TRUE, TRUE, 0);

	/* Ajout du nom du canal */
	GtkWidget *label;
	label = gtk_label_new (g_strconcat("<small>",
					   self->name,
					   "</small>",NULL));
	gtk_label_set_use_markup(GTK_LABEL(label), TRUE);
	gtk_misc_set_alignment (GTK_MISC(label),0,0.5);
	gtk_label_set_ellipsize (GTK_LABEL(label), PANGO_ELLIPSIZE_END);
	gtk_box_pack_start (GTK_BOX(vbox), label, TRUE, TRUE, 0);
	
	
	/* Barre de progression *//*
	GtkWidget *progressbar;
	progressbar = gtk_progress_bar_new ();
	gtk_progress_bar_set_text (GTK_PROGRESS_BAR(progressbar), "12:00 - 14:00");
	gtk_progress_set_text_alignment (GTK_PROGRESS(progressbar), 0, 0.5);
	gtk_progress_bar_set_fraction (GTK_PROGRESS_BAR(progressbar), 0.50);
	gtk_box_pack_start (GTK_BOX(vbox), progressbar, FALSE, FALSE, 0);

	/* Ajout du titre du programme 
	label = gtk_label_new ("<small>Programme TV indisponible</small>");
	gtk_label_set_use_markup (GTK_LABEL(label), TRUE);
	gtk_misc_set_alignment (GTK_MISC(label),0,0.5);
	gtk_label_set_ellipsize (GTK_LABEL(label), PANGO_ELLIPSIZE_END);
	gtk_box_pack_start (GTK_BOX(vbox), label, TRUE, TRUE, 0);*/

	gtk_widget_show_all (GTK_WIDGET(self));

	return GTK_WIDGET(self);
}

void
freetuxtv_channel_set_logo (FreetuxTVChannel *self, gchar *file)
{
	gchar *imgfile;
	gchar *user_img_channels_dir;
	user_img_channels_dir = g_strconcat(g_get_user_data_dir(), 
					    "/.freetuxtv/images/channels", NULL);
	if(file == NULL){
		imgfile = g_strconcat(user_img_channels_dir, "/_none.png", NULL);	
		if(!g_file_test(imgfile,G_FILE_TEST_EXISTS)){
			imgfile = g_strconcat(FREETUXTV_DIR "/images/channels/_none.png", NULL);	
		}
	}else{
		imgfile = g_strconcat(user_img_channels_dir,"/",file,NULL);	
		if(!g_file_test(imgfile,G_FILE_TEST_EXISTS)){
			imgfile = g_strconcat(FREETUXTV_DIR "/images/channels/", file, NULL);	
		}
	}
	
	gtk_image_set_from_file (GTK_IMAGE(self->logo), imgfile);
	gtk_widget_show(self->logo);
	
	g_free(user_img_channels_dir);
	g_free(imgfile);
}	

void
freetuxtv_channel_play (FreetuxTVChannel *self)
{
	FreetuxTVMainWindow *main_window;
	main_window = freetuxtv_main_window_get_from_widget (GTK_WIDGET(self));
	
	freetuxtv_main_window_play_channel (main_window, self);
}

gint
freetuxtv_channel_apply_filter (FreetuxTVChannel *self, gchar *filter)
{
	gchar *channel = g_utf8_strdown (self->name,-1);
	gchar *search = g_strconcat("^.*",
				    g_utf8_strdown (filter,-1),
				    ".*$",NULL);
	
	GRegex *regex;
	regex = g_regex_new (search,0,0,NULL);
	if (g_regex_match (regex, channel, 0, NULL)){
		gtk_widget_show_all (GTK_WIDGET(self));
		g_regex_unref (regex);
		return 1;
	}else{
		gtk_widget_hide_all (GTK_WIDGET(self));
		g_regex_unref (regex);
		return 0;
	}
}

static void
on_channel_button_press_event (GtkWidget *widget, GdkEventButton *event, 
			       gpointer user_data)
{
	FreetuxTVChannel *self = FREETUXTV_CHANNEL(widget);
	if (event->type==GDK_2BUTTON_PRESS 
	    || event->type==GDK_3BUTTON_PRESS) {
		g_print ("FreetuxTV : launching channel \"%s\" -> %s\n",
			 self->name, self->uri);	
		freetuxtv_channel_play(self);
	}
}

static void
on_channel_notify_event (GtkWidget *widget, GdkEventMotion *event, 
			 gpointer user_data)
{
	if(event->type == GDK_ENTER_NOTIFY ){
		freetuxtv_channel_modify_bg (FREETUXTV_CHANNEL(widget), 
					     0xFA, 0xF8, 0xB9);
	}
	if(event->type == GDK_LEAVE_NOTIFY ){
		freetuxtv_channel_modify_bg (FREETUXTV_CHANNEL(widget), 
					     0xFF, 0xFF, 0xFF);	
	}
}

static void
freetuxtv_channel_modify_bg (FreetuxTVChannel *widget, 
			     gint red, gint green, gint blue){
	GdkColor color;
	color.pixel = 0;
	color.red   = red * 0x100;
	color.green = green * 0x100;
	color.blue  = blue * 0x100;
	gtk_widget_modify_bg(GTK_WIDGET(widget), GTK_STATE_NORMAL, &color);	
}

static void
freetuxtv_channel_init (FreetuxTVChannel *object)
{
	object->name="";
	object->uri="";
	object->logo = NULL;
}

static void
freetuxtv_channel_finalize (GObject *object)
{
	
}

static void
freetuxtv_channel_class_init (FreetuxTVChannelClass *klass)
{
	GObjectClass *gobject_class = G_OBJECT_CLASS (klass);
	gobject_class->finalize = freetuxtv_channel_finalize;
}
