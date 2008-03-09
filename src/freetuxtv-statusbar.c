/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4-*- */
/*
 * freetuxtv
 * Copyright (C) FreetuxTV Team's 2008
 * Project homepage : http://code.google.com/p/freetuxtv/
 * 
 * freetuxtv is free software.
 * 
 */

#include "freetuxtv-statusbar.h"

G_DEFINE_TYPE (FreetuxTVStatusbar, freetuxtv_statusbar, GTK_TYPE_STATUSBAR);

GtkWidget *
freetuxtv_statusbar_new ()
{
	FreetuxTVStatusbar *statusbar = NULL;
	statusbar = gtk_type_new (freetuxtv_statusbar_get_type ());	

	gtk_statusbar_set_has_resize_grip (GTK_STATUSBAR (statusbar), TRUE);
	gtk_statusbar_push (GTK_STATUSBAR (statusbar), 0, 
						"Bienvenue dans FreetuxTV");
	
	gtk_widget_show_all (GTK_WIDGET(statusbar));

	return GTK_WIDGET(statusbar);
}

void
freetuxtv_statusbar_update_channel_info (FreetuxTVStatusbar *self, 
										 FreetuxTVChannel *channel)
{
	gchar *text = g_strconcat("En cours de lecture : ",channel->name,NULL);
	gtk_statusbar_push (GTK_STATUSBAR (self), 0, text);
}

static void
freetuxtv_statusbar_init (FreetuxTVStatusbar *object)
{

}

static void
freetuxtv_statusbar_finalize (GObject *object)
{
	
}

static void
freetuxtv_statusbar_class_init (FreetuxTVStatusbarClass *klass)
{
	GObjectClass *gobject_class = G_OBJECT_CLASS (klass);
	gobject_class->finalize = freetuxtv_statusbar_finalize;
}
