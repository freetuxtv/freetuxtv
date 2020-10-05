/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * main.c
 * Copyright (C) Eric Beuque 2010 <eric.beuque@gmail.com>
 * 
 * FreetuxTV is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * FreetuxTV is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef FREETUXTV_WINDOW_MAIN_H
#define FREETUXTV_WINDOW_MAIN_H

#include <QMainWindow>

class QActionGroup;

//#include "freetuxtv-app.h"

typedef enum {
	WINDOW_MODE_STOPPED,
	WINDOW_MODE_PLAYING,
	WINDOW_MODE_PAUSED,
	WINDOW_MODE_RECORDING
} FreetuxTVWindowMode;

class QApplicationMainWindow : public QMainWindow
{
public:
	QApplicationMainWindow(QWidget * parent = NULL);
	virtual ~QApplicationMainWindow();

public:
/*

	void display_buttons (FreetuxTVApp *app, FreetuxTVWindowMode mode);

	void show_error (FreetuxTVApp *app, gchar *msg);

	void show_gerror (FreetuxTVApp *app, GError *error);

	void statusbar_push (FreetuxTVApp *app, gchar *context, gchar *msg);

	void statusbar_pop (FreetuxTVApp *app, gchar *context);

	void update_statusbar_infos (FreetuxTVApp *app);

	void timebar_update (FreetuxTVApp *app, glong time_ms, glong length_ms, gfloat position);

	void enable_accelerators(FreetuxTVApp *app, gboolean enable);

	void show_dialog_addrecordings(FreetuxTVApp *app, FreetuxTVChannelInfos* pChannelInfos, GError** error);
 */
	QAction* getActionPreferences() const;
	QAction* getActionQuit() const;
	QAction* getActionAddGroup() const;
	QAction* getActionSynchronizeTVChannels() const;

	QActionGroup* getActionGroupDeinterlace() const;
	QAction* getActionDeinterlaceDisable() const;
	QAction* getActionDeinterlaceBlend() const;
	QAction* getActionDeinterlaceBob() const;
	QAction* getActionDeinterlaceDiscard() const;
	QAction* getActionDeinterlaceLinear() const;
	QAction* getActionDeinterlaceMean() const;
	QAction* getActionDeinterlaceX() const;

	QAction* getActionAbout() const;

private:
	QMenuBar* createMenuBar(QWidget* parent);

private:
	// Menu FreetuxTV
	QAction* m_pActionPreferences;
	QAction* m_pActionQuit;

	// Menu Channels
	QAction* m_pActionAddGroup;
	QAction* m_pActionSynchronizeTVChannels;

	// Menu Video
	QActionGroup* m_pActionGroupDeinterlace;
	QAction* m_pActionDeinterlaceDisable;
	QAction* m_pActionDeinterlaceBlend;
	QAction* m_pActionDeinterlaceBob;
	QAction* m_pActionDeinterlaceDiscard;
	QAction* m_pActionDeinterlaceLinear;
	QAction* m_pActionDeinterlaceMean;
	QAction* m_pActionDeinterlaceX;

	// Menu Help
	QAction* m_pActionAbout;
};

#endif /* FREETUXTV_WINDOW_MAIN_H */
