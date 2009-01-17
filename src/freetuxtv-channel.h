/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8-*- */
/*
 * freetuxtv
 * Copyright (C) FreetuxTV Team's 2008
 * Project homepage : http://code.google.com/p/freetuxtv/
 * 
 * freetuxtv is free software.
 * 
 */

#ifndef FREETUXTV_CHANNEL_H
#define FREETUXTV_CHANNEL_H

#include <glib-object.h>
#include <gtk/gtk.h>

G_BEGIN_DECLS

#define FREETUXTV_TYPE_CHANNEL            (freetuxtv_channel_get_type ())
#define FREETUXTV_CHANNEL(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), FREETUXTV_TYPE_CHANNEL, FreetuxTVChannel))
#define FREETUXTV_CHANNEL_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), FREETUXTV_TYPE_CHANNEL, FreetuxTVChannelClass))
#define FREETUXTV_IS_CHANNEL(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), FREETUXTV_TYPE_CHANNEL))
#define FREETUXTV_IS_CHANNEL_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), FREETUXTV_TYPE_CHANNEL))
#define FREETUXTV_CHANNEL_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), FREETUXTV_TYPE_CHANNEL, FreetuxTVChannelClass))

typedef struct _FreetuxTVChannel FreetuxTVChannel;
typedef struct _FreetuxTVChannelClass FreetuxTVChannelClass;

typedef enum
{
	FREETUXTV_CHANNEL_STATE_NORMAL,
	FREETUXTV_CHANNEL_STATE_PLAYING
} FreetuxTVChannelStateType;

struct _FreetuxTVChannel
{
	GtkEventBox parent;

	gchar *name;
	gchar *uri;
	gchar *id;
	
	FreetuxTVChannelStateType state;

	GtkImage *logo;
	GtkWidget *label_name;
};

struct _FreetuxTVChannelClass
{
	GtkEventBoxClass parent_class;

	void (* dbl_clicked) (FreetuxTVChannel *channel);
};

GType
freetuxtv_channel_get_type (void);

GtkWidget *
freetuxtv_channel_new (gchar *id, gchar *name, gchar *uri);

void
freetuxtv_channel_set_id (FreetuxTVChannel *self, gchar *id);

void
freetuxtv_channel_set_logo (FreetuxTVChannel *self, gchar *file);

gint
freetuxtv_channel_show_if_filter (FreetuxTVChannel *self, gchar *filter);

G_END_DECLS

#endif /* FREETUXTV_CHANNEL_H */
