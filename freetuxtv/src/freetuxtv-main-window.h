/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4-*- */
/*
 * freetuxtv
 * Copyright (C) FreetuxTV Team's 2008
 * Project homepage : http://code.google.com/p/freetuxtv/
 * 
 * freetuxtv is free software.
 * 
 */

#ifndef FREETUXTV_MAIN_WINDOW_H
#define FREETUXTV_MAIN_WINDOW_H

#include <glib-object.h>
#include <gtk/gtk.h>

#include "freetuxtv-channels-list.h"
#include "freetuxtv-player.h"
#include "freetuxtv-statusbar.h"
#include "freetuxtv-channel.h"

G_BEGIN_DECLS

#define FREETUXTV_TYPE_MAIN_WINDOW            (freetuxtv_main_window_get_type ())
#define FREETUXTV_MAIN_WINDOW(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), FREETUXTV_TYPE_MAIN_WINDOW, FreetuxTVMainWindow))
#define FREETUXTV_MAIN_WINDOW_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), FREETUXTV_TYPE_MAIN_WINDOW, FreetuxTVMainWindowClass))
#define FREETUXTV_IS_MAIN_WINDOW(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), FREETUXTV_TYPE_MAIN_WINDOW))
#define FREETUXTV_IS_MAIN_WINDOW_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), FREETUXTV_TYPE_MAIN_WINDOW))
#define FREETUXTV_MAIN_WINDOW_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), FREETUXTV_TYPE_MAIN_WINDOW, FreetuxTVMainWindowClass))

typedef struct _FreetuxTVMainWindow FreetuxTVMainWindow;
typedef struct _FreetuxTVMainWindowClass FreetuxTVMainWindowClass;

struct _FreetuxTVMainWindow
{
	GtkWindow parent;

	FreetuxTVPlayer *player;
	FreetuxTVChannelsList *channelslist;
	FreetuxTVStatusbar *statusbar;

	FreetuxTVChannel *current_channel;
};

struct _FreetuxTVMainWindowClass
{
	GtkWindowClass parent_class;
};

GType
freetuxtv_main_window_get_type (void);

GtkWidget *
freetuxtv_main_window_new ();

FreetuxTVMainWindow *
freetuxtv_main_window_get_from_widget (GtkWidget *self);

void freetuxtv_main_window_play_channel (FreetuxTVMainWindow *self, 
										 FreetuxTVChannel *channel);

G_END_DECLS

#endif /* FREETUXTV_MAIN_WINDOW_H */
