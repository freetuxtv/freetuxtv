/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4-*- */
/*
 * FreetuxTV is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or 
 * (at your option) any later version.
 *
 * FreetuxTV is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Glade; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#ifndef FREETUXTV_MODELS_H
#define FREETUXTV_MODELS_H

#include "freetuxtv-app.h"

enum {
	MODEL_CHANNELSGROUP_NAME,
	MODEL_CHANNELSGROUP_URI,
	MODEL_CHANNELSGROUP_REQUIREDISP,
	MODEL_CHANNELSGROUP_BREGEX,
	MODEL_CHANNELSGROUP_EREGEX,
	MODEL_CHANNELSGROUP_N_COLUMNS
};

void
load_model_channels_group_from_file(FreetuxTVApp* app, GtkTreeModel *model, GError** error);

#endif /* FREETUXTV_MODELS_H */
