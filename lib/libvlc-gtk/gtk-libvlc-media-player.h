/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * freetuxtv
 * Copyright (C) Eric Beuque 2010 <eric.beuque@gmail.com>
 * 
 * freetuxtv is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * freetuxtv is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _GTK_LIBVLC_MEDIA_PLAYER_H_
#define _GTK_LIBVLC_MEDIA_PLAYER_H_

#include <glib-object.h>
#include <gtk/gtk.h>

#include <libvlc-gtk/gtk-libvlc-include.h>
#include <libvlc-gtk/gtk-libvlc-instance.h>
#include <libvlc-gtk/gtk-libvlc-media.h>

G_BEGIN_DECLS

#define GTK_TYPE_LIBVLC_MEDIA_PLAYER             (gtk_libvlc_media_player_get_type ())
#define GTK_LIBVLC_MEDIA_PLAYER(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_LIBVLC_MEDIA_PLAYER, GtkLibvlcMediaPlayer))
#define GTK_LIBVLC_MEDIA_PLAYER_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), GTK_TYPE_LIBVLC_MEDIA_PLAYER, GtkLibvlcMediaPlayerClass))
#define GTK_IS_LIBVLC_MEDIA_PLAYER(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_LIBVLC_MEDIA_PLAYER))
#define GTK_IS_LIBVLC_MEDIA_PLAYER_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_TYPE_LIBVLC_MEDIA_PLAYER))
#define GTK_LIBVLC_MEDIA_PLAYER_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), GTK_TYPE_LIBVLC_MEDIA_PLAYER, GtkLibvlcMediaPlayerClass))

typedef struct _GtkLibvlcMediaPlayerClass GtkLibvlcMediaPlayerClass;
typedef struct _GtkLibvlcMediaPlayer GtkLibvlcMediaPlayer;

struct _GtkLibvlcMediaPlayerClass
{
	GtkWidgetClass parent_class;

	void (*event_occurred) (
	    GtkLibvlcMediaPlayer *self,
	    GtkLibvlcEventType type_event,
	    gpointer user_data);
};

struct _GtkLibvlcMediaPlayer
{
	GtkWidget parent_instance;

	GtkLibvlcInstance* libvlc_instance;
	
	GtkTreeStore *media_list;
};

// States of the media player
typedef enum {
	GTK_LIBVLC_STATE_NOTHING_SPECIAL,
	GTK_LIBVLC_STATE_OPENING,
	GTK_LIBVLC_STATE_BUFFERING,
	GTK_LIBVLC_STATE_PLAYING,
	GTK_LIBVLC_STATE_PAUSED,
	GTK_LIBVLC_STATE_STOPPED,
	GTK_LIBVLC_STATE_ENDED,
	GTK_LIBVLC_STATE_ERROR
} GtkLibvlcState;

enum {
	GTK_LIBVLC_MODEL_MEDIA_COLUMN,
	GTK_LIBVLC_MODEL_NB_COLUMN
};

GType gtk_libvlc_media_player_get_type (void) G_GNUC_CONST;

GtkWidget*
gtk_libvlc_media_player_new (GtkLibvlcInstance* libvlc_instance, GError** error);

void
gtk_libvlc_media_player_add_media (GtkLibvlcMediaPlayer *self, GtkLibvlcMedia *media);

GtkLibvlcMedia*
gtk_libvlc_media_player_get_current_media (GtkLibvlcMediaPlayer *self);

GtkLibvlcMedia*
gtk_libvlc_media_player_get_media_from_path (GtkLibvlcMediaPlayer *self, GtkTreePath *path);

void
gtk_libvlc_media_player_clear_media_list(GtkLibvlcMediaPlayer *self);

void
gtk_libvlc_media_player_play (GtkLibvlcMediaPlayer *self, gchar **options, GError** error);

void
gtk_libvlc_media_player_play_media_at_path (GtkLibvlcMediaPlayer *self, GtkTreePath *path, gchar **options, GError** error);

void
gtk_libvlc_media_player_play_next (GtkLibvlcMediaPlayer *self, gchar **options, GError** error);

void
gtk_libvlc_media_player_pause (GtkLibvlcMediaPlayer *self, GError** error);

gboolean
gtk_libvlc_media_player_can_pause (GtkLibvlcMediaPlayer *self, GError** error);

void
gtk_libvlc_media_player_stop (GtkLibvlcMediaPlayer *self, GError** error);

void
gtk_libvlc_media_player_set_volume (GtkLibvlcMediaPlayer *self, gdouble volume, GError** error);

gdouble
gtk_libvlc_media_player_get_volume (GtkLibvlcMediaPlayer *self, GError** error);

void
gtk_libvlc_media_player_set_mute (GtkLibvlcMediaPlayer *self, gboolean mute, GError** error);

gboolean
gtk_libvlc_media_player_get_mute (GtkLibvlcMediaPlayer *self, GError** error);

void
gtk_libvlc_media_player_set_fullscreen (GtkLibvlcMediaPlayer *self, gboolean fullscreen, GError** error);

gboolean
gtk_libvlc_media_player_get_fullscreen (GtkLibvlcMediaPlayer *self);

void
gtk_libvlc_media_player_toggle_fullscreen (GtkLibvlcMediaPlayer *self, GError** error);

void
gtk_libvlc_media_player_set_accel_group (GtkLibvlcMediaPlayer *self, GtkAccelGroup *accel_group);

gboolean
gtk_libvlc_media_player_is_playing (GtkLibvlcMediaPlayer *self, GError** error);

GtkLibvlcState
gtk_libvlc_media_player_get_state (GtkLibvlcMediaPlayer *self, GError** error);

glong
gtk_libvlc_media_player_get_length(GtkLibvlcMediaPlayer *self, GError** error);

glong
gtk_libvlc_media_player_get_time(GtkLibvlcMediaPlayer *self, GError** error);

void
gtk_libvlc_media_player_set_time(GtkLibvlcMediaPlayer *self, glong time, GError** error);

gfloat
gtk_libvlc_media_player_get_position(GtkLibvlcMediaPlayer *self, GError** error);

void
gtk_libvlc_media_player_set_position(GtkLibvlcMediaPlayer *self, gfloat position, GError** error);

gboolean
gtk_libvlc_media_player_is_seekable (GtkLibvlcMediaPlayer *self, GError** error);

void
gtk_libvlc_media_player_set_deinterlace (GtkLibvlcMediaPlayer *self, const gchar* mode, GError** error);

const gchar*
gtk_libvlc_media_player_state_tostring (GtkLibvlcState state);

void
gtk_libvlc_media_player_set_play_next_at_end (GtkLibvlcMediaPlayer *self, gboolean b);

void
gtk_libvlc_media_player_set_loop (GtkLibvlcMediaPlayer *self, gboolean b);


GtkLibvlcInstance*
gtk_libvlc_media_player_get_instance (GtkLibvlcMediaPlayer *self);

G_END_DECLS

#endif /* _GTK_LIBVLC_MEDIA_PLAYER_H_ */
