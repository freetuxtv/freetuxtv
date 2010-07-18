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

#ifndef _GTK_LIBVLC_MEDIA_H_
#define _GTK_LIBVLC_MEDIA_H_

#include <glib-object.h>

G_BEGIN_DECLS

#define GTK_TYPE_LIBVLC_MEDIA             (gtk_libvlc_media_get_type ())
#define GTK_LIBVLC_MEDIA(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_LIBVLC_MEDIA, GtkLibvlcMedia))
#define GTK_LIBVLC_MEDIA_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), GTK_TYPE_LIBVLC_MEDIA, GtkLibvlcMediaClass))
#define GTK_IS_LIBVLC_MEDIA(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_LIBVLC_MEDIA))
#define GTK_IS_LIBVLC_MEDIA_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_TYPE_LIBVLC_MEDIA))
#define GTK_LIBVLC_MEDIA_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), GTK_TYPE_LIBVLC_MEDIA, GtkLibvlcMediaClass))

typedef struct _GtkLibvlcMediaClass GtkLibvlcMediaClass;
typedef struct _GtkLibvlcMedia GtkLibvlcMedia;

struct _GtkLibvlcMediaClass
{
	GObjectClass parent_class;
};

struct _GtkLibvlcMedia
{
	GObject parent_instance;

	gchar* mrl;
};

GType gtk_libvlc_media_get_type (void) G_GNUC_CONST;

GtkLibvlcMedia*
gtk_libvlc_media_new (gchar* mrl);

void
gtk_libvlc_media_set_options (GtkLibvlcMedia* media, gchar** options);

void
gtk_libvlc_media_add_option (GtkLibvlcMedia* media, gchar* option);

void
gtk_libvlc_media_add_options (GtkLibvlcMedia* media, gchar** options);

const gchar**
gtk_libvlc_media_get_options (GtkLibvlcMedia* media);

G_END_DECLS

#endif /* _GTK_LIBVLC_MEDIA_H_ */
