/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8-*- */

#ifndef GTK_LIBVLC_MEDIA_H
#define GTK_LIBVLC_MEDIA_H

#include <glib-object.h>

G_BEGIN_DECLS

#define GTK_TYPE_LIBVLC_MEDIA            (gtk_libvlc_media_get_type ())
#define GTK_LIBVLC_MEDIA(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_LIBVLC_MEDIA, GtkLibVLCMedia))
#define GTK_LIBVLC_MEDIA_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GTK_TYPE_LIBVLC_MEDIA, GtkLibVLCMediaClass))
#define GTK_IS_LIBVLC_MEDIA(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_LIBVLC_MEDIA))
#define GTK_IS_LIBVLC_MEDIA_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_TYPE_LIBVLC_MEDIA))
#define GTK_LIBVLC_MEDIA_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GTK_TYPE_LIBVLC_MEDIA, GtkLibVLCMediaClass))

typedef struct _GtkLibVLCMedia GtkLibVLCMedia;
typedef struct _GtkLibVLCMediaClass GtkLibVLCMediaClass;

struct _GtkLibVLCMedia
{
	GObject parent;
	
	gchar *mrl;
};

struct _GtkLibVLCMediaClass
{
	GObjectClass parent_class;
};

GType
gtk_libvlc_media_get_type (void);

GtkLibVLCMedia*
gtk_libvlc_media_new (gchar* mrl);

G_END_DECLS

#endif /* GTK_LIBVLC_MEDIA_H */
