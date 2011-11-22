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

#ifndef _GTK_LIBVLC_INSTANCE_H_
#define _GTK_LIBVLC_INSTANCE_H_

#include <glib-object.h>

#include "gtk-libvlc-include.h"

G_BEGIN_DECLS

#define GTK_TYPE_LIBVLC_INSTANCE             (gtk_libvlc_instance_get_type ())
#define GTK_LIBVLC_INSTANCE(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_LIBVLC_INSTANCE, GtkLibvlcInstance))
#define GTK_LIBVLC_INSTANCE_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), GTK_TYPE_LIBVLC_INSTANCE, GtkLibvlcInstanceClass))
#define GTK_IS_LIBVLC_INSTANCE(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_LIBVLC_INSTANCE))
#define GTK_IS_LIBVLC_INSTANCE_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_TYPE_LIBVLC_INSTANCE))
#define GTK_LIBVLC_INSTANCE_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), GTK_TYPE_LIBVLC_INSTANCE, GtkLibvlcInstanceClass))

typedef struct _GtkLibvlcInstanceClass GtkLibvlcInstanceClass;
typedef struct _GtkLibvlcInstance GtkLibvlcInstance;

struct _GtkLibvlcInstanceClass
{
	GObjectClass parent_class;
};

struct _GtkLibvlcInstance
{
	GObject parent_instance;
};

GType gtk_libvlc_instance_get_type (void) G_GNUC_CONST;

GtkLibvlcInstance*
gtk_libvlc_instance_new (const gchar* vlc_args[], GLogFunc log_func, GError** error);

LIBVLC_INSTANCE_HANDLE
gtk_libvlc_instance_get_libvlc_instance(GtkLibvlcInstance* pLibvlcInstance, GError** error);

const gchar*
gtk_libvlc_get_libvlc_version (gint* major, gint *minor, gint *revision);

G_END_DECLS

#endif /* _GTK_LIBVLC_INSTANCE_H_ */
