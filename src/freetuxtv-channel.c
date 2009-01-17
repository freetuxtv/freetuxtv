/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8-*- */
/*
 * freetuxtv
 * Copyright (C) FreetuxTV Team's 2008
 * Project homepage : http://code.google.com/p/freetuxtv/
 * 
 * freetuxtv is free software.
 * 
 */

#include "freetuxtv-channel.h"

G_DEFINE_TYPE (FreetuxTVChannel, freetuxtv_channel, GTK_TYPE_EVENT_BOX);

static void
on_channel_button_press_event (GtkWidget *widget, GdkEventButton *event, 
			       gpointer user_data);

static void
on_channel_notify_event (GtkWidget *widget, GdkEventMotion *event, 
			 gpointer user_data);

static void
freetuxtv_channel_modify_bg (FreetuxTVChannel *widget, gboolean hover);

enum {
  DBL_CLICKED,
  LAST_SIGNAL
};

static guint channel_signals[LAST_SIGNAL];

GtkWidget *
freetuxtv_channel_new (gchar *id, gchar *name, gchar *uri)
{
	FreetuxTVChannel *self = NULL;
	self = gtk_type_new (freetuxtv_channel_get_type ());

	self->id = g_strdup(id);
	self->name = g_strdup(name);
	self->uri = g_strdup(uri);
	
	/* Evenemment du widget */
	g_signal_connect(G_OBJECT(self),
			 "button-press-event",
			 G_CALLBACK(on_channel_button_press_event),
			 NULL);
	g_signal_connect(G_OBJECT(self),
			 "enter-notify-event",
			 G_CALLBACK(on_channel_notify_event),
			 NULL);
	g_signal_connect(G_OBJECT(self),
			 "leave-notify-event",
			 G_CALLBACK(on_channel_notify_event),
			 NULL);

	GtkWidget *hbox;
	hbox = gtk_hbox_new(FALSE, 0);
	gtk_container_add (GTK_CONTAINER(self), hbox);
	gtk_widget_set_tooltip_text (GTK_WIDGET(hbox), self->name);
	
	/* Ajout du logo */
	self->logo = GTK_IMAGE(gtk_image_new ());
	gtk_misc_set_padding (GTK_MISC(self->logo),2,2);
	gtk_box_pack_start (GTK_BOX(hbox), GTK_WIDGET(self->logo), FALSE, FALSE, 0);
	
	GtkWidget *vbox;
	vbox = gtk_vbox_new(FALSE, 0);
	gtk_box_pack_start (GTK_BOX(hbox), vbox, TRUE, TRUE, 0);

	/* Ajout du nom du canal */		
	self->label_name = gtk_label_new (g_strconcat("<small>",
						 self->name,
						 "</small>",NULL));
	gtk_label_set_use_markup(GTK_LABEL(self->label_name), TRUE);
	gtk_misc_set_alignment (GTK_MISC(self->label_name),0,0.5);
	gtk_label_set_ellipsize (GTK_LABEL(self->label_name), PANGO_ELLIPSIZE_END);
	gtk_box_pack_start (GTK_BOX(vbox), self->label_name, TRUE, TRUE, 0);

	/* Barre de progression *//*
	GtkWidget *progressbar;
	progressbar = gtk_progress_bar_new ();
	gtk_progress_bar_set_text (GTK_PROGRESS_BAR(progressbar), "12:00 - 14:00");
	gtk_progress_set_text_alignment (GTK_PROGRESS(progressbar), 0, 0.5);
	gtk_progress_bar_set_fraction (GTK_PROGRESS_BAR(progressbar), 0.50);
	gtk_box_pack_start (GTK_BOX(vbox), progressbar, FALSE, FALSE, 0);

	/* Ajout du titre du programme 
	label = gtk_label_new ("<small>Programme TV indisponible</small>");
	gtk_label_set_use_markup (GTK_LABEL(label), TRUE);
	gtk_misc_set_alignment (GTK_MISC(label),0,0.5);
	gtk_label_set_ellipsize (GTK_LABEL(label), PANGO_ELLIPSIZE_END);
	gtk_box_pack_start (GTK_BOX(vbox), label, TRUE, TRUE, 0);*/

	freetuxtv_channel_set_state (self, FALSE);

	gtk_widget_show_all (GTK_WIDGET(self));

	return GTK_WIDGET(self);
}

void
freetuxtv_channel_set_id (FreetuxTVChannel *self, gchar *id)
{
	g_free(self->id);
	self->id = id;
}

void
freetuxtv_channel_set_logo (FreetuxTVChannel *self, gchar *file)
{
	gtk_image_set_from_file (self->logo, file);
	gtk_widget_show(GTK_WIDGET(self->logo));
}

void
freetuxtv_channel_set_state (FreetuxTVChannel *self, FreetuxTVChannelStateType state){
	self->state = state;
	freetuxtv_channel_modify_bg (FREETUXTV_CHANNEL(self), FALSE);
}

gint
freetuxtv_channel_show_if_filter (FreetuxTVChannel *self, gchar *filter)
{
	
	gchar *channel = g_utf8_strdown (self->name,-1);
	gchar *search = g_strdup_printf("^.*%s.*$",
					g_utf8_strdown (filter,-1));
	
	GRegex *regex;
	regex = g_regex_new (search, 0, 0 ,NULL);
	if (g_regex_match (regex, channel, 0, NULL)){
		gtk_widget_show_all (GTK_WIDGET(self));
		g_regex_unref (regex);
		return 1;
	}else{
		gtk_widget_hide_all (GTK_WIDGET(self));
		g_regex_unref (regex);
		return 0;
	}
}

static void
on_channel_button_press_event (GtkWidget *widget, GdkEventButton *event, 
			       gpointer user_data)
{
	FreetuxTVChannel *self = FREETUXTV_CHANNEL(widget);
	if (event->type==GDK_2BUTTON_PRESS) {
		/* Envoi du signal dbl-clicked */
		g_signal_emit (G_OBJECT (widget),
			       channel_signals [DBL_CLICKED],
			       0
			       );
	}
}

static void
on_channel_notify_event (GtkWidget *widget, GdkEventMotion *event, 
			 gpointer user_data)
{
	if(event->type == GDK_ENTER_NOTIFY ){
		freetuxtv_channel_modify_bg (FREETUXTV_CHANNEL(widget), TRUE);
	}
	if(event->type == GDK_LEAVE_NOTIFY ){
		freetuxtv_channel_modify_bg (FREETUXTV_CHANNEL(widget), FALSE);
	}
}

static void
freetuxtv_channel_modify_bg (FreetuxTVChannel *widget, gboolean hover){	
	GtkRcStyle *rc_style;
	GtkStyle*style = gtk_rc_get_style(GTK_WIDGET(widget));
	rc_style = gtk_rc_style_new();

	switch(widget->state){
	case FREETUXTV_CHANNEL_STATE_NORMAL:
		if(!hover){
			rc_style->bg[GTK_STATE_NORMAL] = style->base[GTK_STATE_NORMAL];
			rc_style->fg[GTK_STATE_NORMAL] = style->text[GTK_STATE_NORMAL];
		}else{
			rc_style->bg[GTK_STATE_NORMAL] = style->base[GTK_STATE_SELECTED];
			rc_style->fg[GTK_STATE_NORMAL] = style->text[GTK_STATE_SELECTED];
		}
		break;
	case FREETUXTV_CHANNEL_STATE_PLAYING:
		rc_style->bg[GTK_STATE_NORMAL] = style->bg[GTK_STATE_SELECTED];
		rc_style->fg[GTK_STATE_NORMAL] = style->text[GTK_STATE_SELECTED];
		break;
	}
	rc_style->color_flags[GTK_STATE_NORMAL] |= GTK_RC_BG;
	rc_style->color_flags[GTK_STATE_NORMAL] |= GTK_RC_FG;
	gtk_widget_modify_style (GTK_WIDGET(widget), rc_style);
	gtk_widget_modify_style (GTK_WIDGET(widget->label_name), rc_style);
	gtk_rc_style_unref(rc_style);


}

static void
freetuxtv_channel_init (FreetuxTVChannel *object)
{
	object->name="";
	object->uri="";
	object->logo = NULL;
	object->state = FREETUXTV_CHANNEL_STATE_NORMAL;
	object->label_name = NULL;
}

static void
freetuxtv_channel_finalize (GObject *object)
{
	
}

static void
freetuxtv_channel_class_init (FreetuxTVChannelClass *klass)
{
	GObjectClass *gobject_class = G_OBJECT_CLASS (klass);
	gobject_class->finalize = freetuxtv_channel_finalize;

	/* Enregistrements des signaux du widget */
	channel_signals [DBL_CLICKED] = g_signal_new ("dbl-clicked",
						      G_TYPE_FROM_CLASS (klass),
						      G_SIGNAL_RUN_FIRST | G_SIGNAL_ACTION,
						      G_STRUCT_OFFSET (FreetuxTVChannelClass, dbl_clicked),
						      NULL, NULL,
						      g_cclosure_marshal_VOID__VOID,
						      G_TYPE_NONE,
						      0
						      );
}
