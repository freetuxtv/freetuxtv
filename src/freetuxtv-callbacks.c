/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8-*- */
/*
 * freetuxtv
 * Copyright (C) FreetuxTV Team's 2008
 * Project homepage : http://code.google.com/p/freetuxtv/
 * 
 * freetuxtv is free software.
 * 
 */

#include "freetuxtv-callbacks.h"
#include "freetuxtv-channels-list.h"
#include "freetuxtv-player.h"

extern GladeXML *gladexml;

void
on_windowmain_destroy (GtkWidget *widget, gpointer *data)
{
	gtk_main_quit();
}

void 
on_menuitemquit_activate (GtkWidget *widget, gpointer *data)
{
	gtk_main_quit();
}

void
on_menuitemaboutdialog_activate (GtkWidget *widget, gpointer *data)
{
	GtkWidget *aboutdialog;
	aboutdialog = glade_xml_get_widget(gladexml,"aboutdialog");
	
	gtk_widget_show(aboutdialog);
}

void
on_buttonclearfilter_clicked (GtkWidget *widget, GdkEventButton *event,
			      gpointer *data)
{
	GtkWidget *entryfilter;
	entryfilter = glade_xml_get_widget(gladexml,"entryfilter");
	
	gtk_entry_set_text(GTK_ENTRY(entryfilter), "");

	GtkWidget *channelslist;
	channelslist = freetuxtv_channels_list_get_from_gladexml ();
	freetuxtv_channels_list_apply_filter (FREETUXTV_CHANNELS_LIST(channelslist));
}

void
on_entryfilter_changed (GtkEntry *entry, gpointer user_data)
{
	GtkWidget *channelslist;
	channelslist = freetuxtv_channels_list_get_from_gladexml ();
	freetuxtv_channels_list_apply_filter (FREETUXTV_CHANNELS_LIST(channelslist));
}

void
on_volumecontrol_value_changed (GtkRange *range, gpointer  user_data)
{
	
        GtkWidget *player;
	player = freetuxtv_player_get_from_gladexml ();
	freetuxtv_player_set_volume (FREETUXTV_PLAYER(player),
				     gtk_range_get_value (range) );
}

