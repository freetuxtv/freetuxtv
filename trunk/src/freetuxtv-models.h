/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8-*- */
/*
 * freetuxtv
 * Copyright (C) FreetuxTV Team's 2008
 * Project homepage : http://code.google.com/p/freetuxtv/
 * 
 * freetuxtv is free software.
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
load_all_models(FreetuxTVApp* app, GError** error);

void
load_model_channels_group_from_file(FreetuxTVApp* app, GError** error);

#endif /* FREETUXTV_MODELS_H */
