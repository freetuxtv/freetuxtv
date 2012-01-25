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

#ifndef _FREETUXTV_PLAYER_ERROR_DIALOG_H_
#define _FREETUXTV_PLAYER_ERROR_DIALOG_H_

#include <glib-object.h>
#include <gtk/gtk.h>

#include "freetuxtv-app.h"
#include "freetuxtv-channel-infos.h"

G_BEGIN_DECLS

#define FREETUXTV_TYPE_PLAYER_ERROR_DIALOG             (freetuxtv_player_error_dialog_get_type ())
#define FREETUXTV_PLAYER_ERROR_DIALOG(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), FREETUXTV_TYPE_PLAYER_ERROR_DIALOG, FreetuxTVPlayerErrorDialog))
#define FREETUXTV_PLAYER_ERROR_DIALOG_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), FREETUXTV_TYPE_PLAYER_ERROR_DIALOG, FreetuxTVPlayerErrorDialogClass))
#define FREETUXTV_IS_PLAYER_ERROR_DIALOG(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), FREETUXTV_TYPE_PLAYER_ERROR_DIALOG))
#define FREETUXTV_IS_PLAYER_ERROR_DIALOG_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), FREETUXTV_TYPE_PLAYER_ERROR_DIALOG))
#define FREETUXTV_PLAYER_ERROR_DIALOG_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), FREETUXTV_TYPE_PLAYER_ERROR_DIALOG, FreetuxTVPlayerErrorDialogClass))

typedef struct _FreetuxTVPlayerErrorDialogClass FreetuxTVPlayerErrorDialogClass;
typedef struct _FreetuxTVPlayerErrorDialog FreetuxTVPlayerErrorDialog;

struct _FreetuxTVPlayerErrorDialogClass
{
	GtkDialogClass parent_class;
};

struct _FreetuxTVPlayerErrorDialog
{
	GtkDialog parent_instance;
};

GType freetuxtv_player_error_dialog_get_type (void) G_GNUC_CONST;

GtkWidget*
freetuxtv_player_error_dialog_new (
    GtkWindow *parent, GtkDialogFlags flags,
    FreetuxTVApp* app);

void
freetuxtv_player_error_dialog_set_message (
    FreetuxTVPlayerErrorDialog* pPlayerErrorDialog, const gchar *message);

G_END_DECLS

#endif /* _FREETUXTV_PLAYER_ERROR_DIALOG_H_ */
