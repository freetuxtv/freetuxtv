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

#include <gtk/gtk.h>

#include "freetuxtv-main-window.h"

int main (int argc, char *argv[])
{
	gtk_init (&argc, &argv);
	
	FreetuxTVMainWindow *freetuxtv = NULL;
	freetuxtv = FREETUXTV_MAIN_WINDOW(freetuxtv_main_window_new());
	
	gtk_widget_show_all (GTK_WIDGET(freetuxtv));
	
	FreetuxTVChannel *channel;
	channel=freetuxtv_channel_new("France 2",
				      "rtsp://mafreebox.freebox.fr/freeboxtv/stream?id=201");
	freetuxtv_channel_set_logo (channel, "./img/France 2.jpg");
	freetuxtv_channel_set_player (channel, freetuxtv->player);
	freetuxtv_channels_list_add_channel (freetuxtv->channelslist, channel);


	channel=freetuxtv_channel_new("France 3",
				      "rtsp://mafreebox.freebox.fr/freeboxtv/stream?id=202");
	freetuxtv_channel_set_logo (channel, "./img/France 3.jpg");
	freetuxtv_channel_set_player (channel, freetuxtv->player);
	freetuxtv_channels_list_add_channel (freetuxtv->channelslist, channel);

	channel=freetuxtv_channel_new("Canal +",
				      "rtsp://mafreebox.freebox.fr/freeboxtv/stream?id=203");
	freetuxtv_channel_set_logo (channel, "./img/canal+.jpg");
	freetuxtv_channel_set_player (channel, freetuxtv->player);
	freetuxtv_channels_list_add_channel (freetuxtv->channelslist, channel);

	channel=freetuxtv_channel_new("W9",
				      "rtsp://mafreebox.freebox.fr/freeboxtv/stream?id=211");
	freetuxtv_channel_set_logo (channel, "./img/W9.jpg");
	freetuxtv_channel_set_player (channel, freetuxtv->player);
	freetuxtv_channels_list_add_channel (freetuxtv->channelslist, channel);

	channel=freetuxtv_channel_new("T-Shirt Japonais",
				      "file://./tshirt_japonais.mpg");
	freetuxtv_channel_set_player (channel, freetuxtv->player);
	freetuxtv_channel_set_logo (channel, "./img/Shangai Dragon TV.jpg");
	freetuxtv_channels_list_add_channel (freetuxtv->channelslist, channel);
	
	freetuxtv_player_init(freetuxtv->player);

	gtk_main ();

	return 0;


	/*

	FreetuxTVChannel *channel;
	channel=freetuxtv_channel_new("France 2",
				      "rtsp://mafreebox.freebox.fr/freeboxtv/stream?id=201");
	freetuxtv_channel_set_logo (channel, "./img/France 2.jpg");
	freetuxtv_channel_set_player (channel, freetuxtv->player);
	freetuxtv_channels_list_add_channel (freetuxtv->channelslist, channel);


	channel=freetuxtv_channel_new("France 3",
				      "rtsp://mafreebox.freebox.fr/freeboxtv/stream?id=202");
	freetuxtv_channel_set_logo (channel, "./img/France 3.jpg");
	freetuxtv_channel_set_player (channel, freetuxtv->player);
	freetuxtv_channels_list_add_channel (freetuxtv->channelslist, channel);

	channel=freetuxtv_channel_new("Canal +",
				      "rtsp://mafreebox.freebox.fr/freeboxtv/stream?id=203");
	freetuxtv_channel_set_logo (channel, "./img/canal+.jpg");
	freetuxtv_channel_set_player (channel, freetuxtv->player);
	freetuxtv_channels_list_add_channel (freetuxtv->channelslist, channel);

	channel=freetuxtv_channel_new("W9",
				      "rtsp://mafreebox.freebox.fr/freeboxtv/stream?id=211");
	freetuxtv_channel_set_logo (channel, "./img/W9.jpg");
	freetuxtv_channel_set_player (channel, freetuxtv->player);
	freetuxtv_channels_list_add_channel (freetuxtv->channelslist, channel);

	channel=freetuxtv_channel_new("T-Shirt Japonais",
				      "file://./tshirt_japonais.mpg");
	freetuxtv_channel_set_player (channel, freetuxtv->player);
	freetuxtv_channel_set_logo (channel, "./img/Shangai Dragon TV.jpg");
	freetuxtv_channels_list_add_channel (freetuxtv->channelslist, channel);
	
	freetuxtv_player_init(freetuxtv->player);

	/*
	freetuxtv_player_play(freetuxtv->player,"file://./tshirt_japonais.mpg");
	/*freetuxtv_player_play(freetuxtv->player,"http://mafreebox.freebox.fr/freeboxtv/playlist.m3u");*/

}
