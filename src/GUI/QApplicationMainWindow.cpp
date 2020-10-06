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

#include <stdlib.h>
/*
#include <gtk/gtk.h>
#include <sqlite3.h>
#include <gdk/gdkx.h>
*/
#include <QCoreApplication>
#include <QLabel>
#include <QToolButton>
#include <QLineEdit>
#include <QTreeView>
#include <QPushButton>
#include <QMenuBar>
#include <QStatusBar>
#include <QHBoxLayout>

#include "GUI/QCtrlBarView.h"

#include "QApplicationMainWindow.h"
/*
#include "freetuxtv-window-add-recording.h"
#include "freetuxtv-window-add-channels-group.h"
#include "freetuxtv-window-tv-channels-database.h"

#include "freetuxtv-app.h"
#include "freetuxtv-i18n.h"
#include "freetuxtv-utils.h"
#include "freetuxtv-channels-list.h"
#include "freetuxtv-channels-group-infos.h"
#include "freetuxtv-tv-channels-list.h"
#include "freetuxtv-recordings-list.h"
#include "freetuxtv-db-sync.h"
#include "freetuxtv-models.h"
 */

QApplicationMainWindow::QApplicationMainWindow(QWidget * parent) : QMainWindow(parent)
{
	QWidget *pWidget;

	QMenuBar *pMenuBar = createMenuBar(this);
	setMenuBar(pMenuBar);

	pWidget = createCentralWidget(this);
	setCentralWidget(pWidget);

	QStatusBar* pStatusBar = new QStatusBar();
	setStatusBar(pStatusBar);

	/*
	widget = (GtkWidget *)gtk_builder_get_object (app->gui,
												  "windowmain_buttonclearfilter");
	g_signal_connect(G_OBJECT(widget),
					 "clicked",
					 G_CALLBACK(on_windowmain_buttonclearfilter_clicked),
					 app);

	widget = (GtkWidget *)gtk_builder_get_object (app->gui,
												  "windowmain_entryfilter");
	g_signal_connect(G_OBJECT(widget),
					 "changed",
					 G_CALLBACK(on_windowmain_entryfilter_changed),
					 app);
	g_signal_connect(G_OBJECT(widget),
					 "focus-in-event",
					 G_CALLBACK(on_windowmain_entryfilter_focusin),
					 app);
	g_signal_connect(G_OBJECT(widget),
					 "focus-out-event",
					 G_CALLBACK(on_windowmain_entryfilter_focusout),
					 app);

	widget = (GtkWidget *)gtk_builder_get_object (app->gui,
												  "windowmain_scaletime");
	g_signal_connect(G_OBJECT(widget),
					 "change-value",
					 G_CALLBACK(on_windowmain_valuechanged),
					 app);

	widget = (GtkWidget *)gtk_builder_get_object (app->gui,
												  "windowmain_buttongotocurrent");
	g_signal_connect(G_OBJECT(widget),
					 "clicked",
					 G_CALLBACK(on_windowmain_buttongotocurrent_clicked),
					 app);

	widget = (GtkWidget *)gtk_builder_get_object (app->gui,
												  "windowmain_buttonprevious");
	g_signal_connect(G_OBJECT(widget),
					 "clicked",
					 G_CALLBACK(on_windowmain_buttonprevious_clicked),
					 app);

	widget = (GtkWidget *)gtk_builder_get_object (app->gui,
												  "windowmain_buttonnext");
	g_signal_connect(G_OBJECT(widget),
					 "clicked",
					 G_CALLBACK(on_windowmain_buttonnext_clicked),
					 app);

	widget = (GtkWidget *)gtk_builder_get_object (app->gui,
												  "windowmain_buttonplaypause");
	g_signal_connect(G_OBJECT(widget),
					 "clicked",
					 G_CALLBACK(on_windowmain_buttonplaypause_clicked),
					 app);

	widget = (GtkWidget *)gtk_builder_get_object (app->gui,
												  "windowmain_buttonstop");
	g_signal_connect(G_OBJECT(widget),
					 "clicked",
					 G_CALLBACK(on_windowmain_buttonstop_clicked),
					 app);

	widget = (GtkWidget *)gtk_builder_get_object (app->gui,
												  "windowmain_buttonrecord");
	g_signal_connect(G_OBJECT(widget),
					 "clicked",
					 G_CALLBACK(on_windowmain_buttonrecord_clicked),
					 app);

	widget = (GtkWidget *)gtk_builder_get_object (app->gui,
												  "windowmain_volumecontrol");
	g_signal_connect(G_OBJECT(widget),
					 "value-changed",
					 G_CALLBACK(on_windowmain_volumecontrol_value_changed),
					 app);

	widget = (GtkWidget *)gtk_builder_get_object (app->gui,
												  "windowmain_buttonfullscreen");
	g_signal_connect(G_OBJECT(widget),
					 "clicked",
					 G_CALLBACK(on_windowmain_buttonfullscreen_clicked),
					 app);

	widget = (GtkWidget *)gtk_builder_get_object (app->gui,
												  "windowmain_buttonminimode");
	g_signal_connect(G_OBJECT(widget),
					 "clicked",
					 G_CALLBACK(on_windowmain_buttonminimode_clicked),
					 app);

	// Initialize signals for windowminimode
	widget = (GtkWidget *) gtk_builder_get_object (app->gui,
												   "windowminimode");
	g_signal_connect(G_OBJECT(widget),
					 "delete-event",
					 G_CALLBACK(on_windowmain_deleteevent),
					 app);

	widget = (GtkWidget *) gtk_builder_get_object (app->gui,
												   "windowminimode_buttonnormalmode");
	g_signal_connect(G_OBJECT(widget),
					 "clicked",
					 G_CALLBACK(on_windowminimode_buttonnormalmode_clicked),
					 app);

	widget = (GtkWidget *) gtk_builder_get_object (app->gui,
												   "windowminimode_buttonstayontop");
	g_signal_connect(G_OBJECT(widget),
					 "clicked",
					 G_CALLBACK(on_windowminimode_buttonstayontop_clicked),
					 app);

	widget = (GtkWidget *) gtk_builder_get_object (app->gui,
												   "windowminimode_volumecontrol");
	g_signal_connect(G_OBJECT(widget),
					 "value-changed",
					 G_CALLBACK(on_windowmain_volumecontrol_value_changed),
					 app);

	// Initialize signals for dialogpreferences
	widget = (GtkWidget *)gtk_builder_get_object (app->gui,
												  "dialogpreferences");
	gtk_dialog_add_buttons (GTK_DIALOG(widget),
							"gtk-cancel", GTK_RESPONSE_CANCEL,
							"gtk-apply", GTK_RESPONSE_APPLY, NULL);

	g_signal_connect(G_OBJECT(widget),
					 "response",
					 G_CALLBACK(on_dialogpreferences_response),
					 app);
	g_signal_connect(G_OBJECT(widget),
					 "delete-event",
					 G_CALLBACK(gtk_widget_hide_on_delete),
					 NULL);

	widget = (GtkWidget *)gtk_builder_get_object (app->gui,
												  "dialogpreferences_directoryrecordings");
	g_signal_connect(G_OBJECT(widget),
					 "selection-changed",
					 G_CALLBACK(on_dialogpreferences_directoryrecordings_changed),
					 app);

	widget = (GtkWidget *)gtk_builder_get_object (app->gui,
												  "dialogpreferences_radiotranscodingno");
	g_signal_connect(G_OBJECT(widget),
					 "toggled",
					 G_CALLBACK(on_dialogpreferences_radiotranscoding_toggled),
					 app);

	widget = (GtkWidget *)gtk_builder_get_object (app->gui,
												  "dialogpreferences_radiotranscodingpredefinedformats");
	g_signal_connect(G_OBJECT(widget),
					 "toggled",
					 G_CALLBACK(on_dialogpreferences_radiotranscoding_toggled),
					 app);

	widget = (GtkWidget *)gtk_builder_get_object (app->gui,
												  "dialogpreferences_transcodingformats");
	g_signal_connect(G_OBJECT(widget),
					 "changed",
					 G_CALLBACK(on_dialogpreferences_transcodingformats_changed),
					 app);

	widget = (GtkWidget *)gtk_builder_get_object (app->gui,
												  "dialogpreferences_radioproxyno");
	g_signal_connect(G_OBJECT(widget),
					 "toggled",
					 G_CALLBACK(on_dialogpreferences_radioproxy_toggled),
					 app);

	widget = (GtkWidget *)gtk_builder_get_object (app->gui,
												  "dialogpreferences_radioproxymanual");
	g_signal_connect(G_OBJECT(widget),
					 "toggled",
					 G_CALLBACK(on_dialogpreferences_radioproxy_toggled),
					 app);

	widget = (GtkWidget *)gtk_builder_get_object (app->gui,
												  "dialogpreferences_checkuseauth");
	g_signal_connect(G_OBJECT(widget),
					 "toggled",
					 G_CALLBACK(on_dialogpreferences_checkuseauth_toggled),
					 app);

	// Initialize signals for dialoggroupproperties
	widget = (GtkWidget *)gtk_builder_get_object (app->gui,
												  "dialoggroupproperties");
	gtk_dialog_add_buttons (GTK_DIALOG(widget),
							"gtk-cancel", GTK_RESPONSE_CANCEL, "gtk-apply", GTK_RESPONSE_APPLY, NULL);
	g_signal_connect(G_OBJECT(widget),
					 "response",
					 G_CALLBACK(on_dialoggroupproperties_response),
					 app);
	g_signal_connect(G_OBJECT(widget),
					 "delete-event",
					 G_CALLBACK(gtk_widget_hide_on_delete),
					 NULL);

	// Initialize signals for aboutdialog
	widget =  (GtkWidget *) gtk_builder_get_object (app->gui,
													"aboutdialog");

	gtk_about_dialog_set_version (GTK_ABOUT_DIALOG(widget), VERSION);

	g_signal_connect(G_OBJECT(widget),
					 "response",
					 G_CALLBACK(on_aboutdialog_response),
					 NULL);
	g_signal_connect(G_OBJECT(widget),
					 "delete-event",
					 G_CALLBACK(gtk_widget_hide_on_delete),
					 NULL);

	windowmain_enable_accelerators(app, TRUE);
	 */
}

QApplicationMainWindow::~QApplicationMainWindow()
{
	/*
	windowmain_enable_accelerators(app, FALSE);

#if GTK_API_VERSION == 3
	g_object_unref (app->widget.pAccelGroup);
#else
	gtk_accel_group_unref (app->widget.pAccelGroup);
#endif
	app->widget.pAccelGroup = NULL;

	if(app->widget.pTrayIcon){
		g_object_unref(app->widget.pTrayIcon);
		app->widget.pTrayIcon = NULL;
	}
	 */
}

QMenuBar* QApplicationMainWindow::createMenuBar(QWidget* parent)
{
	QMenuBar *pMenuBar = NULL;

	// Initialize menu bar
	pMenuBar = new QMenuBar (this);
	QMenu *pMenu;
	QMenu *pSubmenu;

	//GSList* pRadioGroup = NULL;

	// Menu FreetuxTV
	{
		pMenu = pMenuBar->addMenu(tr("&FreetuxTV"));

		m_pActionPreferences = new QAction(QIcon::fromTheme("document-properties"), tr("&Preferences"));
		pMenu->addAction(m_pActionPreferences);

		pMenu->addSeparator();

		m_pActionQuit = new QAction(QIcon::fromTheme("application-exit"), tr("&Quit"));
		pMenu->addAction(m_pActionQuit);
	}

	// Menu Channels
	{
		pMenu = pMenuBar->addMenu(tr("&Channels"));

		m_pActionAddGroup = new QAction(tr("&Add a group"));
		m_pActionAddGroup->setToolTip(tr("Add/create a channels group from different sources"));
		pMenu->addAction(m_pActionAddGroup);

		m_pActionSynchronizeTVChannels = new QAction(QIcon::fromTheme("view-refresh"), tr("&Synchronize TV channels database"));
		m_pActionSynchronizeTVChannels->setToolTip(
				tr("Synchronize the TV channels list in the local database from the XML database"));
		pMenu->addAction(m_pActionSynchronizeTVChannels);
	}

	// Menu Video
	{
		pMenu = pMenuBar->addMenu(tr("&Video"));

		// SubMenu : Deinterlace
		pSubmenu = pMenu->addMenu(tr("&Deinterlace"));

		m_pActionGroupDeinterlace = new QActionGroup(parent);

		m_pActionDeinterlaceDisable = m_pActionGroupDeinterlace->addAction(tr("Disable"));
		m_pActionDeinterlaceDisable->setCheckable(true);
		m_pActionDeinterlaceDisable->setChecked(true);
		pSubmenu->addAction(m_pActionDeinterlaceDisable);
		pSubmenu->addSeparator();
		m_pActionDeinterlaceBlend = m_pActionGroupDeinterlace->addAction(tr("Blend"));
		m_pActionDeinterlaceBlend->setCheckable(true);
		pSubmenu->addAction(m_pActionDeinterlaceBlend);
		m_pActionDeinterlaceBob = m_pActionGroupDeinterlace->addAction(tr("Bob"));
		m_pActionDeinterlaceBob->setCheckable(true);
		pSubmenu->addAction(m_pActionDeinterlaceBob);
		m_pActionDeinterlaceDiscard = m_pActionGroupDeinterlace->addAction(tr("Discard"));
		m_pActionDeinterlaceDiscard->setCheckable(true);
		pSubmenu->addAction(m_pActionDeinterlaceDiscard);
		m_pActionDeinterlaceLinear = m_pActionGroupDeinterlace->addAction(tr("Linear"));
		m_pActionDeinterlaceLinear->setCheckable(true);
		pSubmenu->addAction(m_pActionDeinterlaceLinear);
		m_pActionDeinterlaceMean = m_pActionGroupDeinterlace->addAction(tr("Mean"));
		m_pActionDeinterlaceMean->setCheckable(true);
		pSubmenu->addAction(m_pActionDeinterlaceMean);
		m_pActionDeinterlaceX = m_pActionGroupDeinterlace->addAction(tr("X"));
		m_pActionDeinterlaceX->setCheckable(true);
		pSubmenu->addAction(m_pActionDeinterlaceX);
	}

	// Menu Help
	{
		pMenu = pMenuBar->addMenu(tr("&Help"));

		m_pActionAbout = new QAction(QIcon::fromTheme("help-about"), tr("&About"));
		pMenu->addAction(m_pActionAbout);
	}

	return pMenuBar;
}


QWidget* QApplicationMainWindow::createCentralWidget(QWidget* parent)
{
	QWidget* pMainWidget = new QWidget(parent);

	QBoxLayout* pLayout = new QHBoxLayout();
	//pLayout->setContentsMargins(2, 2, 2, 2);
	pMainWidget->setLayout(pLayout);

	QWidget* pTmpWidget;

	// Tabs
	{
		QTabWidget *pTab = new QTabWidget();
		pTab->setTabPosition(QTabWidget::West);

		pTmpWidget = createTabChannelsView(pTab);
		pTab->addTab(pTmpWidget, tr("Channels"));
		pTab->addTab(new QWidget(), tr("Recordings"));

		pLayout->addWidget(pTab, 1);
	}

	// Video
	{
		pTmpWidget = createVideoView(pMainWidget);
		pLayout->addWidget(pTmpWidget, 4);
	}

	return pMainWidget;
}


QWidget* QApplicationMainWindow::createTabChannelsView(QWidget* parent)
{
	QWidget* pMainWidget = new QWidget(parent);

	QBoxLayout* pMainLayout = new QVBoxLayout();
	pMainWidget->setLayout(pMainLayout);

	QBoxLayout* pTmpLayout;
	{
		pTmpLayout = new QHBoxLayout();
		pMainLayout->addLayout(pTmpLayout);

		pTmpLayout->addWidget(new QLabel(tr("Search:")));

		// Search area
		m_pLineEditSearch = new QLineEdit(pMainWidget);
		pTmpLayout->addWidget(m_pLineEditSearch);

		// Search button
		m_pButtonSearchReset = new QToolButton(pMainWidget);
		m_pButtonSearchReset->setIcon(QIcon::fromTheme("edit-clear"));
		pTmpLayout->addWidget(m_pButtonSearchReset);
	}

	{
		m_pTreeviewChannels = new QTreeView(pMainWidget);
		pMainLayout->addWidget(m_pTreeviewChannels);
	}

	{
		m_pButtonJumpToChannel = new QPushButton(tr("Jump to current channel"), pMainWidget);
		pMainLayout->addWidget(m_pButtonJumpToChannel);
	}

	return pMainWidget;
}

QWidget* QApplicationMainWindow::createVideoView(QWidget* parent)
{
	QWidget* pMainWidget = new QWidget(parent);

	QBoxLayout* pMainLayout = new QVBoxLayout();
	pMainLayout->setContentsMargins(0,0,0,0);
	pMainWidget->setLayout(pMainLayout);

	m_pVideoView = new QWidget(pMainWidget);
	m_pVideoView->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
	m_pVideoView->setStyleSheet("background-color:black;");
	pMainLayout->addWidget(m_pVideoView);

	m_pCtrlBarView = new QCtrlBarView(pMainWidget);
	pMainLayout->addWidget(m_pCtrlBarView);

	return pMainWidget;
}

QAction* QApplicationMainWindow::getActionPreferences() const
{
	return m_pActionPreferences;
}

QAction* QApplicationMainWindow::getActionQuit() const
{
	return m_pActionQuit;
}

QAction* QApplicationMainWindow::getActionAddGroup() const
{
	return m_pActionAddGroup;
}

QAction* QApplicationMainWindow::getActionSynchronizeTVChannels() const
{
	return m_pActionSynchronizeTVChannels;
}

QActionGroup* QApplicationMainWindow::getActionGroupDeinterlace() const
{
	return m_pActionGroupDeinterlace;
}

QAction* QApplicationMainWindow::getActionDeinterlaceDisable() const
{
	return m_pActionDeinterlaceDisable;
}

QAction* QApplicationMainWindow::getActionDeinterlaceBlend() const
{
	return m_pActionDeinterlaceBlend;
}

QAction* QApplicationMainWindow::getActionDeinterlaceBob() const
{
	return m_pActionDeinterlaceBob;
}

QAction* QApplicationMainWindow::getActionDeinterlaceDiscard() const
{
	return m_pActionDeinterlaceDiscard;
}

QAction* QApplicationMainWindow::getActionDeinterlaceLinear() const
{
	return m_pActionDeinterlaceLinear;
}

QAction* QApplicationMainWindow::getActionDeinterlaceMean() const
{
	return m_pActionDeinterlaceMean;
}

QAction* QApplicationMainWindow::getActionDeinterlaceX() const
{
	return m_pActionDeinterlaceX;
}

QAction* QApplicationMainWindow::getActionAbout() const
{
	return m_pActionAbout;
}

QLineEdit* QApplicationMainWindow::getLineEditSearch() const
{
	return m_pLineEditSearch;
}

QToolButton* QApplicationMainWindow::getButtonSearchReset() const
{
	return m_pButtonSearchReset;
}

QTreeView* QApplicationMainWindow::getTreeviewChannels() const
{
	return m_pTreeviewChannels;
}

QPushButton* QApplicationMainWindow::getButtonJumpToChannel() const
{
	return m_pButtonJumpToChannel;
}

void QApplicationMainWindow::closeEvent(QCloseEvent *event)
{
	QCoreApplication::quit();
}

/*
void
windowmain_display_buttons (FreetuxTVApp *app, FreetuxTVWindowMode mode)
{
	GtkWidget *widget;
	gboolean sensitive;
	GtkWidget *image = NULL;
	// Button previous
	widget = (GtkWidget *) gtk_builder_get_object (app->gui,
	                                               "windowmain_buttonprevious");
	switch(mode){
		case WINDOW_MODE_STOPPED :
			if(app->current.pPathChannel != NULL){
				sensitive = TRUE;		
			}else{
				sensitive = FALSE;	
			}
			break;
		case WINDOW_MODE_RECORDING :
			sensitive = FALSE;
			break;
		case WINDOW_MODE_PLAYING :
			sensitive = TRUE;
			break;
		default:
			sensitive = FALSE;
			break;
	}
	gtk_widget_set_sensitive(widget, sensitive);

	// Button next
	widget = (GtkWidget *) gtk_builder_get_object (app->gui,
	                                               "windowmain_buttonnext");
	switch(mode){
		case WINDOW_MODE_STOPPED :
			if(app->current.pPathChannel != NULL){
				sensitive = TRUE;		
			}else{
				sensitive = FALSE;	
			}
			break;
		case WINDOW_MODE_RECORDING :
			sensitive = FALSE;
			break;
		case WINDOW_MODE_PLAYING :
			sensitive = TRUE;		
			break;
		default:
			break;
	}
	gtk_widget_set_sensitive(widget, sensitive);

	// Button play/pause
	widget = (GtkWidget *) gtk_builder_get_object (app->gui,
	                                               "windowmain_buttonplaypause");
	switch(mode){
		case WINDOW_MODE_STOPPED :
			if(app->current.pPathChannel == NULL){
				sensitive = FALSE;
			}else{
				sensitive = TRUE;
			}
			image = gtk_image_new_from_stock (GTK_STOCK_MEDIA_PLAY, GTK_ICON_SIZE_BUTTON);
			break;
		case WINDOW_MODE_PAUSED :
			sensitive = TRUE;
			image = gtk_image_new_from_stock (GTK_STOCK_MEDIA_PLAY, GTK_ICON_SIZE_BUTTON);
			break;
		case WINDOW_MODE_PLAYING :
			sensitive = TRUE;
			image = gtk_image_new_from_stock (GTK_STOCK_MEDIA_PAUSE, GTK_ICON_SIZE_BUTTON);
			break;
		case WINDOW_MODE_RECORDING :
			sensitive = FALSE;
			image = gtk_image_new_from_stock (GTK_STOCK_MEDIA_PAUSE, GTK_ICON_SIZE_BUTTON);
			break;
	}
	gtk_widget_set_sensitive(widget, sensitive);
	if(image){
		gtk_button_set_image (GTK_BUTTON(widget), image);
	}

	// Button stop
	widget = (GtkWidget *) gtk_builder_get_object (app->gui,
	                                               "windowmain_buttonstop");
	switch(mode){
		case WINDOW_MODE_STOPPED :
			sensitive = FALSE;
			break;
		case WINDOW_MODE_PLAYING :
		case WINDOW_MODE_RECORDING :
			sensitive = TRUE;		
			break;
		default:
			break;
	}
	gtk_widget_set_sensitive(widget, sensitive);

	// Button record
	widget = (GtkWidget *) gtk_builder_get_object (app->gui,
	                                               "windowmain_buttonrecord");
	switch(mode){
		case WINDOW_MODE_STOPPED :
		case WINDOW_MODE_RECORDING :
			sensitive = FALSE;
			break;
		case WINDOW_MODE_PLAYING :
			sensitive = TRUE;		
			break;
		default:
			break;
	}
	gtk_widget_set_sensitive(widget, sensitive);
}

void
windowmain_show_error (FreetuxTVApp *app, gchar *msg)
{
	GtkWidget *windowmain;
	GtkWidget* dialog;

	windowmain = (GtkWidget *) gtk_builder_get_object (app->gui,
	                                                   "windowmain");

	dialog = gtk_message_dialog_new(GTK_WINDOW(windowmain),
	                                GTK_DIALOG_MODAL, 
	                                GTK_MESSAGE_ERROR,
	                                GTK_BUTTONS_OK,
	                                msg, NULL);

	gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(dialog);
}

void
windowmain_show_gerror (FreetuxTVApp *app, GError* error)
{
	g_return_if_fail(error != NULL);
	windowmain_show_error (app, error->message);
}

void
windowmain_statusbar_push (FreetuxTVApp *app, gchar *context, gchar *msg)
{
	int context_id;

	GtkWidget *statusbar;
	statusbar = (GtkWidget *) gtk_builder_get_object (app->gui,
	                                                  "windowmain_statusbar");
	context_id = gtk_statusbar_get_context_id(GTK_STATUSBAR(statusbar), 
	                                          context);
	gtk_statusbar_push (GTK_STATUSBAR(statusbar), 
	                    context_id,
	                    msg);
	
	while (g_main_context_iteration(NULL, FALSE)){}
}


void
windowmain_statusbar_pop (FreetuxTVApp *app, gchar *context)
{	
	int context_id;

	GtkWidget *statusbar;
	statusbar = (GtkWidget *) gtk_builder_get_object (app->gui,
	                                                  "windowmain_statusbar");
	context_id = gtk_statusbar_get_context_id(GTK_STATUSBAR(statusbar), 
	                                          context);
	gtk_statusbar_pop (GTK_STATUSBAR(statusbar),
	                   context_id);

	while (g_main_context_iteration(NULL, FALSE)){}
}

void
windowmain_update_statusbar_infos (FreetuxTVApp *app)
{
	GtkWidget *widget;
	widget = (GtkWidget *) gtk_builder_get_object (app->gui,
	                                               "windowmain_channelscount");

	gint channels_count;
	channels_count = channels_list_get_channels_count (app);

	gchar *str;
	str = g_strdup_printf("%d", channels_count);
	gtk_label_set_text(GTK_LABEL(widget), str);
	g_free(str);	
}

void
windowmain_timebar_update (FreetuxTVApp *app, glong time_ms, glong length_ms, gfloat position)
{
	GtkWidget *widget;

	gdouble time_s = time_ms/1000.0;
	gdouble length_s = length_ms/1000.0;

	widget = (GtkWidget *) gtk_builder_get_object (app->gui,
	                                               "windowmain_labeltime");
	gtk_label_set_text(GTK_LABEL(widget), format_time2((glong)time_s));

	widget = (GtkWidget *) gtk_builder_get_object (app->gui,
	                                               "windowmain_labellength");
	gtk_label_set_text(GTK_LABEL(widget), format_time2((glong)length_s));	

	widget = (GtkWidget *) gtk_builder_get_object (app->gui,
	                                               "adjustment_time");
	gtk_adjustment_set_value (GTK_ADJUSTMENT(widget), position);

	widget = (GtkWidget *) gtk_builder_get_object (app->gui,
	                                               "windowmain_hboxtimebar");
	if(gtk_libvlc_media_player_is_seekable(app->player, NULL)){
		gtk_widget_set_sensitive(widget, TRUE);
	}else{
		gtk_widget_set_sensitive(widget, FALSE);
	}
}

void
windowmain_show_dialog_addrecordings(FreetuxTVApp *app, FreetuxTVChannelInfos* pChannelInfos, GError** error)
{
	g_return_if_fail (error != NULL);
	
	GtkWidget* pParent;
	pParent = (GtkWidget *) gtk_builder_get_object (app->gui, "windowmain");

	DBSync dbsync;
	
	// Create and show the window
	FreetuxTVWindowRecording* pWindowRecording;
	FreetuxTVRecordingInfos* pRecordingInfos = NULL;
	gint res;
	
	pWindowRecording = freetuxtv_window_recording_new (GTK_WINDOW(pParent), app, pChannelInfos);
	res = gtk_builder_dialog_run (GTK_BUILDER_DIALOG(pWindowRecording));

	if(res == GTK_RESPONSE_OK){
		pRecordingInfos = freetuxtv_window_recording_get_recording_infos (pWindowRecording);
	}

	// Destroy the window
	gtk_builder_widget_destroy(GTK_BUILDER_WIDGET(pWindowRecording));
	pWindowRecording = NULL;

	if(res == GTK_RESPONSE_OK){
		dbsync_open_db (&dbsync, error);
		if(*error == NULL){
			freetuxtv_recording_infos_set_status(pRecordingInfos, FREETUXTV_RECORDING_STATUS_WAITING);

			recordings_list_add_recording (app, &dbsync, pRecordingInfos, error);
		}
		dbsync_close_db(&dbsync);

		if(*error == NULL){
			GTimeVal now;
			g_get_current_time (&now);
			gint64 now64 = g_time_val_to_int64(&now);

			if(freetuxtv_recording_infos_has_time(pRecordingInfos, now64)){
				freetuxtv_action_start_recording (app, pRecordingInfos, error);
			}
		}
	}

	if(pRecordingInfos){
		g_object_unref (pRecordingInfos);
		pRecordingInfos = NULL;
	}
}
/*
void
windowmain_enable_accelerators(FreetuxTVApp *app, gboolean enable)
{
	g_return_if_fail(app != NULL);
	
	GtkWidget* widget;

	widget = (GtkWidget *)gtk_builder_get_object (app->gui,
	                                              "windowmain");
	if(enable){
		if(!app->current.bIsAccelGroupActivated){
			gtk_window_add_accel_group(GTK_WINDOW(widget), app->widget.pAccelGroup);
		}
	}else{
		if(app->current.bIsAccelGroupActivated){
			gtk_window_remove_accel_group(GTK_WINDOW(widget), app->widget.pAccelGroup);
		}
	}

	widget = (GtkWidget *) gtk_builder_get_object (app->gui,
	                                               "windowminimode");
	if(enable){
		if(!app->current.bIsAccelGroupActivated){
			gtk_window_add_accel_group(GTK_WINDOW(widget), app->widget.pAccelGroup);
		}
	}else{
		if(app->current.bIsAccelGroupActivated){
			gtk_window_remove_accel_group(GTK_WINDOW(widget), app->widget.pAccelGroup);
		}
	}

	app->current.bIsAccelGroupActivated = enable;
}

static void
windowmain_add_accelerator (GtkAccelGroup *p_accel_group,
                           const gchar *accelerator, const gchar *accel_path,
                           GCallback callback, gpointer user_data)
{
	guint key;
	GdkModifierType mods;
	GClosure *closure = NULL;

	gtk_accelerator_parse (accelerator, &key, &mods);
	closure = g_cclosure_new (callback, user_data, NULL);
	gtk_accel_group_connect (p_accel_group, key, mods, GTK_ACCEL_VISIBLE, closure);
	gtk_accel_map_add_entry (accel_path, key, mods);
}


*/