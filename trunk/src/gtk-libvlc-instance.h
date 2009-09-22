/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8-*- */

#ifndef GTK_LIBVLC_INSTANCE_H
#define GTK_LIBVLC_INSTANCE_H

#include <glib-object.h>

#include "gtk-libvlc-include.h"

G_BEGIN_DECLS

#define GTK_TYPE_LIBVLC_INSTANCE            (gtk_libvlc_instance_get_type ())
#define GTK_LIBVLC_INSTANCE(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_LIBVLC_INSTANCE, GtkLibVLCInstance))
#define GTK_LIBVLC_INSTANCE_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GTK_TYPE_LIBVLC_INSTANCE, GtkLibVLCInstanceClass))
#define GTK_IS_LIBVLC_INSTANCE(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_LIBVLC_INSTANCE))
#define GTK_IS_LIBVLC_INSTANCE_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_TYPE_LIBVLC_INSTANCE))
#define GTK_LIBVLC_INSTANCE_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GTK_TYPE_LIBVLC_INSTANCE, GtkLibVLCInstanceClass))

typedef struct _GtkLibVLCInstance GtkLibVLCInstance;
typedef struct _GtkLibVLCInstanceClass GtkLibVLCInstanceClass;

struct _GtkLibVLCInstance
{
	GObject parent;

	libvlc_instance_t *libvlc_instance;	
};

struct _GtkLibVLCInstanceClass
{
	GObjectClass parent_class;
};

GType
gtk_libvlc_instance_get_type (void);

GtkLibVLCInstance*
gtk_libvlc_instance_new (const gchar* vlc_args[]);

const gchar*
gtk_libvlc_get_libvlc_version (gint *major, gint *minor, gint *revision);

G_END_DECLS

#endif /* GTK_LIBVLC_INSTANCE_H */
