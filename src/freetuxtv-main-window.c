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

#include "freetuxtv-main-window.h"
#include "freetuxtv-player.h"

extern GladeXML *gladexml;

void freetuxtv_main_window_play_channel (FreetuxTVChannel *channel)
{
	GtkWidget *player;
	GtkWidget *statusbar;
	gchar *text;
	
	statusbar = glade_xml_get_widget (gladexml, "statusbar");
	
	gtk_statusbar_pop (GTK_STATUSBAR(statusbar),
			   gtk_statusbar_get_context_id(GTK_STATUSBAR(statusbar), 
							"PlayChannelMsg"));
	text = g_strconcat ("En cours de lecture : ", channel->name, NULL);
	gtk_statusbar_push (GTK_STATUSBAR(statusbar),
			    gtk_statusbar_get_context_id(GTK_STATUSBAR(statusbar), 
							 "PlayChannelMsg"), 
			    text);
	g_free(text);
	
	player = freetuxtv_player_get_from_gladexml ();
	freetuxtv_player_play (FREETUXTV_PLAYER(player), channel->uri);
}
