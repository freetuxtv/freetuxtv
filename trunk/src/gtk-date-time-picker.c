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

#define _XOPEN_SOURCE /* For strptime */ 
#include <time.h>

#include "gtk-date-time-picker.h"

#include <stdlib.h>

typedef struct _GtkDateTimePickerPrivate GtkDateTimePickerPrivate;
struct _GtkDateTimePickerPrivate
{
	gchar* szDateFormat;

	GtkWidget* date_entry;
	GtkWidget* hour_spinbutton;
	GtkWidget* minute_spinbutton;
	
	GtkWidget* calendar_window;
	GtkWidget* calendar;
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
	SIGNAL_DATETIME_CHANGED,

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
/*
static gboolean
on_dateentry_focusin (GtkWidget *widget, GdkEvent  *event, gpointer user_data);

static gboolean
on_dateentry_focusout (GtkWidget *widget, GdkEvent  *event, gpointer user_data);
*/

static gboolean
on_spinbutton_output (GtkSpinButton *spin, gpointer data);

static void
on_editable_changed (GtkEditable *editable, gpointer user_data);

static gboolean
get_dmy_if_valid(GtkDateTimePicker* picker, gint* day, gint* month, gint* year);

static void
gtk_date_time_picker_init (GtkDateTimePicker *object)
{
	GtkDateTimePickerPrivate* priv;
	priv = GTK_DATE_TIME_PICKER_PRIVATE(object);

	priv->szDateFormat = NULL;
	
	priv->date_entry = NULL;
	priv->hour_spinbutton = NULL;
	priv->minute_spinbutton = NULL;

	priv->calendar_window = NULL;
	priv->calendar = NULL;

	GtkWidget *label, *hbox_time;
	gtk_box_set_spacing(GTK_BOX(object), 2);

	// Date picker
	priv->date_entry = gtk_entry_new ();
	//priv->date_entry = gtk_combo_box_text_new_with_entry ();
	//gtk_entry_set_icon_from_stock (GTK_ENTRY(priv->date_entry), GTK_ENTRY_ICON_SECONDARY, "gtk-edit");
	gtk_entry_set_width_chars (GTK_ENTRY(priv->date_entry), 12);
	g_signal_connect(G_OBJECT(priv->date_entry), "changed",
	    G_CALLBACK(on_editable_changed),
	    (gpointer)object);
	gtk_widget_show(priv->date_entry);
	gtk_box_pack_start(GTK_BOX(object), priv->date_entry, FALSE, FALSE, 0);
	/*
	g_signal_connect(G_OBJECT(priv->date_entry), "focus-in-event",
	    G_CALLBACK(on_dateentry_focusin),
	    (gpointer)object);
	g_signal_connect(G_OBJECT(priv->date_entry), "focus-out-event",
	    G_CALLBACK(on_dateentry_focusout),
	    (gpointer)object);
	g_signal_connect(G_OBJECT(entry), "insert_text",
	    G_CALLBACK(check_date_text_handler),
	    NULL);*/

	// Hour
	hbox_time = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
	gtk_widget_show(hbox_time);
	gtk_box_pack_start(GTK_BOX(object), hbox_time, FALSE, FALSE, 0);
	
	priv->hour_spinbutton = gtk_spin_button_new_with_range (0.0, 23.0, 1.0);
	gtk_spin_button_set_wrap (GTK_SPIN_BUTTON(priv->hour_spinbutton), TRUE);
	g_signal_connect(G_OBJECT(priv->hour_spinbutton), "output",
	    G_CALLBACK(on_spinbutton_output),
	    NULL);
	g_signal_connect(G_OBJECT(priv->hour_spinbutton), "changed",
	    G_CALLBACK(on_editable_changed),
	    (gpointer)object);
	gtk_widget_show(priv->hour_spinbutton);
	gtk_box_pack_start(GTK_BOX(hbox_time), priv->hour_spinbutton, FALSE, FALSE, 0);

	label = gtk_label_new(":");
	gtk_widget_show(label);
	gtk_box_pack_start(GTK_BOX(hbox_time), label, FALSE, FALSE, 0);

	priv->minute_spinbutton = gtk_spin_button_new_with_range (0.0, 59.0, 1.0);
	gtk_spin_button_set_wrap (GTK_SPIN_BUTTON(priv->minute_spinbutton), TRUE);
	g_signal_connect(G_OBJECT(priv->minute_spinbutton), "output",
	    G_CALLBACK(on_spinbutton_output),
	    NULL);
	g_signal_connect(G_OBJECT(priv->minute_spinbutton), "changed",
	    G_CALLBACK(on_editable_changed),
	    (gpointer)object);
	gtk_widget_show(priv->minute_spinbutton);
	gtk_box_pack_start(GTK_BOX(hbox_time), priv->minute_spinbutton, FALSE, FALSE, 0);
	
	//gtk_widget_set_sensitive (GTK_WIDGET(priv->date_entry), FALSE);
	//gtk_widget_set_sensitive (GTK_WIDGET(priv->hour_spinbutton), FALSE);
	//gtk_widget_set_sensitive (GTK_WIDGET(priv->minute_spinbutton), FALSE);
}

static void
gtk_date_time_picker_finalize (GObject *object)
{
	GtkDateTimePickerPrivate* priv;
	priv = GTK_DATE_TIME_PICKER_PRIVATE(object);

	if(priv->calendar_window){
		gtk_widget_destroy(priv->calendar_window);
		priv->calendar_window = NULL;
	}

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
gtk_date_time_picker_datetime_changed (GtkDateTimePicker *self, gpointer user_data)
{
	g_return_if_fail(self != NULL);
	g_return_if_fail(GTK_IS_DATE_TIME_PICKER(self));
	
	GtkDateTimePickerPrivate* priv;
	priv = GTK_DATE_TIME_PICKER_PRIVATE(self);
	
	const gchar* szCurIcon;
	const gchar* szNewIcon;
	szCurIcon = gtk_entry_get_icon_stock (GTK_ENTRY(priv->date_entry), GTK_ENTRY_ICON_SECONDARY);
	
	if(get_dmy_if_valid (self, NULL, NULL, NULL)){
		szNewIcon = GTK_STOCK_YES;
	}else{
		szNewIcon = GTK_STOCK_NO;
	}
	if(szCurIcon == NULL || g_ascii_strcasecmp(szCurIcon, szNewIcon) != 0){
		gtk_entry_set_icon_from_stock (GTK_ENTRY(priv->date_entry),
		    GTK_ENTRY_ICON_SECONDARY, szNewIcon);
	}
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

	klass->datetime_changed = gtk_date_time_picker_datetime_changed;

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

	date_time_picker_signals[SIGNAL_DATETIME_CHANGED] =
		g_signal_new ("datetime-changed",
		    G_OBJECT_CLASS_TYPE (klass),
		    G_SIGNAL_RUN_FIRST,
		    G_STRUCT_OFFSET (GtkDateTimePickerClass, datetime_changed),
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
	g_return_if_fail(picker != NULL);
	g_return_if_fail(GTK_IS_DATE_TIME_PICKER(picker));
	g_return_if_fail(datetime != NULL);
	
	GtkDateTimePickerPrivate* priv;
	priv = GTK_DATE_TIME_PICKER_PRIVATE(picker);
	
	gchar* szTmp;
	
	szTmp = g_date_time_format (datetime, priv->szDateFormat);
	//gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT(priv->date_entry), szTmp);
	//gtk_combo_box_set_active (GTK_COMBO_BOX(priv->date_entry), 0);
	gtk_entry_set_text(GTK_ENTRY(priv->date_entry), szTmp);

	gtk_spin_button_set_value (GTK_SPIN_BUTTON(priv->hour_spinbutton),
	    g_date_time_get_hour (datetime));

	gtk_spin_button_set_value (GTK_SPIN_BUTTON(priv->minute_spinbutton),
	    g_date_time_get_minute (datetime));
}

GDateTime*
gtk_date_time_picker_get_datetime (GtkDateTimePicker* picker, GTimeZone *tz)
{
	g_return_val_if_fail(picker != NULL, NULL);
	g_return_val_if_fail(GTK_IS_DATE_TIME_PICKER(picker), NULL);
	g_return_val_if_fail(tz != NULL, NULL);
	
	GtkDateTimePickerPrivate* priv;
	priv = GTK_DATE_TIME_PICKER_PRIVATE(picker);
	
	GDateTime* datetime = NULL;
	gint year, month, day, hour, minute;

	const gchar *szTmp;
	
	szTmp = gtk_entry_get_text(GTK_ENTRY(priv->hour_spinbutton));
	hour = atoi(szTmp);

	szTmp = gtk_entry_get_text(GTK_ENTRY(priv->minute_spinbutton));
	minute = atoi(szTmp);

	if(get_dmy_if_valid (picker, &day, &month, &year)){
		datetime = g_date_time_new (tz, year, month, day, hour, minute, 0.0);
	}

	return datetime;
}

/*
static gboolean
on_dateentry_focusin (GtkWidget *widget, GdkEvent  *event, gpointer user_data)
{
	GtkDateTimePicker *self;
	self = (GtkDateTimePicker*)user_data;
	
	GtkDateTimePickerPrivate* priv;
	priv = GTK_DATE_TIME_PICKER_PRIVATE(self);
	
	// Calendar
	if(priv->calendar_window == NULL){
		
		priv->calendar_window = gtk_window_new(GTK_WINDOW_POPUP);
		
		GtkWidget *parent = gtk_widget_get_toplevel (widget);
		if (gtk_widget_is_toplevel (parent))
		{
			gtk_window_set_transient_for(GTK_WINDOW(priv->calendar_window), GTK_WINDOW(parent));
		}

		GtkWidget* vbox;
		vbox = gtk_vbox_new(FALSE, 0);

		gtk_box_pack_start(GTK_BOX(vbox), gtk_entry_new(), TRUE, TRUE, 0);

		//gtk_window_set_modal (GTK_WINDOW(priv->calendar_window), TRUE);
		priv->calendar = gtk_calendar_new();
		//gtk_calendar_set_display_options(GTK_CALENDAR(priv->calendar), GTK_CALENDAR_SHOW_DETAILS);
		//gtk_container_add(GTK_CONTAINER(priv->calendar_window), priv->calendar);
		gtk_box_pack_start(GTK_BOX(vbox), priv->calendar, TRUE, TRUE, 0);
		gtk_container_add(GTK_CONTAINER(priv->calendar_window), vbox);
	}
	gtk_window_set_position (GTK_WINDOW(priv->calendar_window), GTK_WIN_POS_MOUSE);
	gtk_widget_show_all(priv->calendar_window);

	return FALSE;
}

static gboolean
on_dateentry_focusout (GtkWidget *widget, GdkEvent  *event, gpointer user_data)
{
	GtkDateTimePicker *self;
	self = (GtkDateTimePicker*)user_data;
	
	GtkDateTimePickerPrivate* priv;
	priv = GTK_DATE_TIME_PICKER_PRIVATE(self);

	if(priv->calendar_window){
		gtk_widget_hide(priv->calendar_window);
	}

	return FALSE;
}
*/

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

static gboolean
on_spinbutton_output (GtkSpinButton *spin, gpointer data)
{
   GtkAdjustment *adj;
   gchar *text;
   int value;
   adj = gtk_spin_button_get_adjustment (spin);
   value = (int)gtk_adjustment_get_value (adj);
   text = g_strdup_printf ("%02d", value);
   gtk_entry_set_text (GTK_ENTRY (spin), text);
   g_free (text);
   
   return TRUE;
}

static void
on_editable_changed (GtkEditable *editable, gpointer user_data)
{
	GtkDateTimePicker *self;
	self = (GtkDateTimePicker*)user_data;
	
	g_signal_emit (
	    G_OBJECT (self),
	    date_time_picker_signals [SIGNAL_DATETIME_CHANGED],
	    0);
}

static gboolean
get_dmy_if_valid(GtkDateTimePicker* picker, gint* day, gint* month, gint* year)
{
	g_return_val_if_fail(picker != NULL, FALSE);
	g_return_val_if_fail(GTK_IS_DATE_TIME_PICKER(picker), FALSE);
	
	GtkDateTimePickerPrivate* priv;
	priv = GTK_DATE_TIME_PICKER_PRIVATE(picker);
	
	struct tm tm;
	gint iyear, imonth, iday;

	gboolean bRes = FALSE;

	const gchar *szDate;
	szDate = gtk_entry_get_text(GTK_ENTRY(priv->date_entry));
	strptime (szDate, priv->szDateFormat, &tm);

	iyear = tm.tm_year + 1900;
	imonth = tm.tm_mon + 1;
	iday = tm.tm_mday;

	GDate* date;
	if(g_date_valid_dmy (iday, imonth, iyear)){
		date = g_date_new_dmy (iday, imonth, iyear);
		if(date){
			gchar szBuf[80];
			g_date_strftime (szBuf, 80, priv->szDateFormat, date);
			if(g_ascii_strcasecmp(szDate, szBuf) == 0){
				bRes = TRUE;
			}

			g_date_free (date);
			date = NULL;
		}
	}

	if(bRes){
		if(day)
			*day = iday;
		if(month)
			*month = imonth;
		if(year)
			*year = iyear;
	}
	
	return bRes;
}