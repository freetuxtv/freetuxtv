/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8-*- */
/*
 * freetuxtv
 * Copyright (C) FreetuxTV Team's 2008
 * Project homepage : http://code.google.com/p/freetuxtv/
 * 
 * freetuxtv is free software.
 * 
 * You may redistribute it and/or modify it under the terms of the
 * GNU General Public License, as published by the Free Software
 * Foundation; either version 2 of the License, or (at your option)
 * any later version.
 * 
 * freetuxtv is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with brasero.  If not, write to:
 * 	The Free Software Foundation, Inc.,
 * 	51 Franklin Street, Fifth Floor
 * 	Boston, MA  02110-1301, USA.
 */

#include "freetuxtv-statusbar.h"

G_DEFINE_TYPE (FreetuxTVStatusbar, freetuxtv_statusbar, GTK_TYPE_STATUSBAR);

GtkWidget *
freetuxtv_statusbar_new ()
{
	FreetuxTVStatusbar *statusbar = NULL;
	statusbar = gtk_type_new (freetuxtv_statusbar_get_type ());	

	gtk_statusbar_set_has_resize_grip (GTK_STATUSBAR (statusbar), TRUE);
	gtk_statusbar_push (GTK_STATUSBAR (statusbar), 0, "Bienvenue dans FreetuxTV");
	
	gtk_widget_show_all (GTK_WIDGET(statusbar));

	return GTK_WIDGET(statusbar);
}

void freetuxtv_statusbar_update_channel_info (FreetuxTVStatusbar *self, 
					      FreetuxTVChannel *channel)
{
	gchar *text = g_strconcat("Chaîne en cours : ",channel->name,NULL);
	gtk_statusbar_push (GTK_STATUSBAR (self), 0, text);
}

static void
freetuxtv_statusbar_init (FreetuxTVStatusbar *object)
{

}

static void
freetuxtv_statusbar_finalize (GObject *object)
{
	
}

static void
freetuxtv_statusbar_class_init (FreetuxTVStatusbarClass *klass)
{
	GObjectClass *gobject_class = G_OBJECT_CLASS (klass);
	gobject_class->finalize = freetuxtv_statusbar_finalize;
}
