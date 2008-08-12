/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8-*- */
/*
 * freetuxtv
 * Copyright (C) FreetuxTV Team's 2008
 * Project homepage : http://code.google.com/p/freetuxtv/
 * 
 * freetuxtv is free software.
 * 
 */

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#ifndef INTERNATIONALIZATION_H
#define INTERNATIONALIZATION_H

#include <libintl.h>
#include <locale.h>

#define _(text) gettext(text)

#endif /* INTERNATIONALIZATION_H */
