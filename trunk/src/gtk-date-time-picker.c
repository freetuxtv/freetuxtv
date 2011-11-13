/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * freetuxtv
 * Copyright (C) Eric Beuque 2011 <eric.beuque@gmail.com>
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

#include "gtk-date-time-picker.h"

typedef struct _GtkDateTimePickerPrivate GtkDateTimePickerPrivate;
struct _GtkDateTimePickerPrivate
{
	GtkCalendar* calendar_widget;

	gchar* szDateFormat;

	GtkWidget* date_entry;
	GtkWidget* hour_spinbutton;
	GtkWidget* minute_spinbutton;
};

#define GTK_DATE_TIME_PICKER_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), GTK_TYPE_DATE_TIME_PICKER, GtkDateTimePickerPrivate))

enum
{
	PROP_0,

	PROP_DATE_FORMAT,
	PROP_SHOW_DATE,
	PROP_SHOW_TIME
};

enum
{
	DATE_CHANGED,

	LAST_SIGNAL
};


static guint date_time_picker_signals[LAST_SIGNAL] = { 0 };

G_DEFINE_TYPE (GtkDateTimePicker, gtk_date_time_picker, GTK_TYPE_HBOX);

/*
static void
check_date_text_handler (
    GtkEntry *entry, const gchar *text,
    gint length, gint *position, gpointer data);
	*/

static void
gtk_date_time_picker_init (GtkDateTimePicker *object)
{
	GtkDateTimePickerPrivate* priv;
	priv = GTK_DATE_TIME_PICKER_PRIVATE(object);

	priv->szDateFormat = NULL;
	
	priv->date_entry = NULL;
	priv->hour_spinbutton = NULL;
	priv->minute_spinbutton = NULL;

	GtkWidget *label, *hbox_time;
	gtk_box_set_spacing(GTK_BOX(object), 2);

	// Date picker
	priv->date_entry = gtk_entry_new ();
	gtk_entry_set_icon_from_stock (GTK_ENTRY(priv->date_entry), GTK_ENTRY_ICON_SECONDARY, "gtk-edit");
	gtk_entry_set_width_chars (GTK_ENTRY(priv->date_entry), 12);
	gtk_widget_show(priv->date_entry);
	gtk_box_pack_start(GTK_BOX(object), priv->date_entry, FALSE, FALSE, 0);

	/*
	g_signal_connect(G_OBJECT(entry), "insert_text",
	    G_CALLBACK(check_date_text_handler),
	    NULL);*/

	// Hour
	hbox_time = gtk_hbox_new(FALSE, 0);
	gtk_widget_show(hbox_time);
	gtk_box_pack_start(GTK_BOX(object), hbox_time, FALSE, FALSE, 0);
	
	priv->hour_spinbutton = gtk_spin_button_new_with_range (0.0, 23.0, 1.0);
	gtk_widget_show(priv->hour_spinbutton);
	gtk_box_pack_start(GTK_BOX(hbox_time), priv->hour_spinbutton, FALSE, FALSE, 0);

	label = gtk_label_new(":");
	gtk_widget_show(label);
	gtk_box_pack_start(GTK_BOX(hbox_time), label, FALSE, FALSE, 0);

	priv->minute_spinbutton = gtk_spin_button_new_with_range (0.0, 59.0, 1.0);
	gtk_widget_show(priv->minute_spinbutton);
	gtk_box_pack_start(GTK_BOX(hbox_time), priv->minute_spinbutton, FALSE, FALSE, 0);

	gtk_widget_set_sensitive (GTK_WIDGET(priv->date_entry), FALSE);
	gtk_widget_set_sensitive (GTK_WIDGET(priv->hour_spinbutton), FALSE);
	gtk_widget_set_sensitive (GTK_WIDGET(priv->minute_spinbutton), FALSE);
}

static void
gtk_date_time_picker_finalize (GObject *object)
{
	GtkDateTimePickerPrivate* priv;
	priv = GTK_DATE_TIME_PICKER_PRIVATE(object);

	if(priv->szDateFormat){
		g_free(priv->szDateFormat);
		priv->szDateFormat = NULL;
	}
	
	G_OBJECT_CLASS (gtk_date_time_picker_parent_class)->finalize (object);
}

static void
gtk_date_time_picker_set_property (GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec)
{
	g_return_if_fail (GTK_IS_DATE_TIME_PICKER (object));
	
	GtkDateTimePickerPrivate* priv;
	priv = GTK_DATE_TIME_PICKER_PRIVATE(object);

	switch (prop_id)
	{
		case PROP_DATE_FORMAT:
			if(priv->szDateFormat){
				g_free(priv->szDateFormat);
				priv->szDateFormat = NULL;
			}
			priv->szDateFormat = g_strdup(g_value_get_string(value));
			break;
		case PROP_SHOW_DATE:
			/* TODO: Add setter for "show-date" property here */
			break;
		case PROP_SHOW_TIME:
			/* TODO: Add setter for "show-time" property here */
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
			break;
	}
}

static void
gtk_date_time_picker_get_property (GObject *object, guint prop_id, GValue *value, GParamSpec *pspec)
{
	g_return_if_fail (GTK_IS_DATE_TIME_PICKER (object));
	
	GtkDateTimePickerPrivate* priv;
	priv = GTK_DATE_TIME_PICKER_PRIVATE(object);

	switch (prop_id)
	{
		case PROP_DATE_FORMAT:
			g_value_set_string(value, priv->szDateFormat);
			break;
		case PROP_SHOW_DATE:
			/* TODO: Add getter for "show-date" property here */
			break;
		case PROP_SHOW_TIME:
			/* TODO: Add getter for "show-time" property here */
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
			break;
	}
}

static void
gtk_date_time_picker_date_changed (GtkDateTimePicker *self)
{
	/* TODO: Add default signal handler implementation here */
}

static void
gtk_date_time_picker_class_init (GtkDateTimePickerClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	//GtkBinClass* parent_class = GTK_BIN_CLASS (klass);

	g_type_class_add_private (klass, sizeof (GtkDateTimePickerPrivate));

	object_class->finalize = gtk_date_time_picker_finalize;
	object_class->set_property = gtk_date_time_picker_set_property;
	object_class->get_property = gtk_date_time_picker_get_property;

	klass->date_changed = gtk_date_time_picker_date_changed;

	g_object_class_install_property (object_class,
	    PROP_DATE_FORMAT,
	    g_param_spec_string ("date-format",
		    "Date Format",
		    "Date reprensentation format",
		    "%m/%d/%y",
		    G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));

	g_object_class_install_property (object_class,
	    PROP_SHOW_DATE,
	    g_param_spec_boolean ("show-date",
		    "Show date",
		    "Display the date picker",
		    TRUE,
		    G_PARAM_READWRITE));

	g_object_class_install_property (object_class,
	    PROP_SHOW_TIME,
	    g_param_spec_boolean ("show-time",
		    "Show time",
		    "Display the time picker",
		    TRUE,
		    G_PARAM_READWRITE));

	date_time_picker_signals[DATE_CHANGED] =
		g_signal_new ("date-changed",
		    G_OBJECT_CLASS_TYPE (klass),
		    G_SIGNAL_RUN_FIRST,
		    G_STRUCT_OFFSET (GtkDateTimePickerClass, date_changed),
		    NULL, NULL,
		    g_cclosure_marshal_VOID__VOID,
		    G_TYPE_NONE, 0);
}


GtkWidget*
gtk_date_time_picker_new (gchar* date_format)
{
	GtkWidget* self;

	self = (GtkWidget*)g_object_new (GTK_TYPE_DATE_TIME_PICKER,
	    "date-format", date_format, NULL);

	return self;
}

void
gtk_date_time_picker_set_datetime (GtkDateTimePicker* picker, GDateTime *datetime)
{
	GtkDateTimePickerPrivate* priv;
	priv = GTK_DATE_TIME_PICKER_PRIVATE(picker);
	
	gchar* szTmp;
	
	szTmp = g_date_time_format (datetime, priv->szDateFormat);
	gtk_entry_set_text(GTK_ENTRY(priv->date_entry), szTmp);

	gtk_spin_button_set_value (GTK_SPIN_BUTTON(priv->hour_spinbutton),
	    g_date_time_get_hour (datetime));

	gtk_spin_button_set_value (GTK_SPIN_BUTTON(priv->minute_spinbutton),
	    g_date_time_get_minute (datetime));
}

/*
static void
check_date_text_handler (
    GtkEntry *entry, const gchar *text,
    gint length, gint *position, gpointer data)
{
	GtkEditable *editable = GTK_EDITABLE(entry);
	int i, count=0;
	gchar *result = g_new (gchar, length);

	for (i=0; i < length; i++) {
		//if((i>=0 && i<=1) || (i>= && i<=1)
		if (!isalpha(text[i]))
			continue;
		result[count++] = islower(text[i]) ? toupper(text[i]) : text[i];
	}

	if (count > 0) {
		g_signal_handlers_block_by_func (G_OBJECT (editable),
		    G_CALLBACK (check_date_text_handler),
		    data);
		gtk_editable_insert_text (editable, result, count, position);
		g_signal_handlers_unblock_by_func (G_OBJECT (editable),
		    G_CALLBACK (check_date_text_handler),
		    data);
	}
	g_signal_stop_emission_by_name (G_OBJECT (editable), "insert_text");

	g_free (result);
}
*/
