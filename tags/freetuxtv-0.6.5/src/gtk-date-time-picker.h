/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * freetuxtv
 * Copyright (C) Eric Beuque 2011 <eric.beuque@gmail.com>
 * 
freetuxtv is free software: you can redistribute it and/or modify it
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

#ifndef _GTK_DATE_TIME_PICKER_H_
#define _GTK_DATE_TIME_PICKER_H_

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define GTK_TYPE_DATE_TIME_PICKER             (gtk_date_time_picker_get_type ())
#define GTK_DATE_TIME_PICKER(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_DATE_TIME_PICKER, GtkDateTimePicker))
#define GTK_DATE_TIME_PICKER_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), GTK_TYPE_DATE_TIME_PICKER, GtkDateTimePickerClass))
#define GTK_IS_DATE_TIME_PICKER(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_DATE_TIME_PICKER))
#define GTK_IS_DATE_TIME_PICKER_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_TYPE_DATE_TIME_PICKER))
#define GTK_DATE_TIME_PICKER_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), GTK_TYPE_DATE_TIME_PICKER, GtkDateTimePickerClass))

typedef struct _GtkDateTimePickerClass GtkDateTimePickerClass;
typedef struct _GtkDateTimePicker GtkDateTimePicker;

struct _GtkDateTimePickerClass
{
	GtkHBoxClass parent_class;

	/* Signals */
	void(* datetime_changed) (GtkDateTimePicker *self, gpointer user_data);
};

struct _GtkDateTimePicker
{
	GtkHBox parent_instance;
};

GType gtk_date_time_picker_get_type (void) G_GNUC_CONST;

GtkWidget* gtk_date_time_picker_new (gchar* date_format);

void
gtk_date_time_picker_set_datetime (GtkDateTimePicker* picker, GDateTime *datetime);

GDateTime*
gtk_date_time_picker_get_datetime (GtkDateTimePicker* picker, GTimeZone *tz);

G_END_DECLS

#endif /* _GTK_DATE_TIME_PICKER_H_ */
