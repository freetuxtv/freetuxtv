//
// Created by ebeuque on 05/10/2020.
//

#include <QCoreApplication>
#include <QAction>
#include <QLineEdit>
#include <QToolButton>
#include <QPushButton>
#include <QSlider>

#include "GUI/QApplicationMainWindow.h"
#include "GUI/QCtrlBarView.h"

#include "QApplicationMainWindowController.h"


QApplicationMainWindowController::QApplicationMainWindowController()
{

}

QApplicationMainWindowController::~QApplicationMainWindowController()
{

}

void QApplicationMainWindowController::init(QApplicationMainWindow* pMainWindow)
{
	m_pMainWindow = pMainWindow;

	QCtrlBarView* pCtrlBarView = m_pMainWindow->getCtrlBarView();

	QAction* pAction;

	// Menu action
	pAction = m_pMainWindow->getActionPreferences();
	connect(pAction, SIGNAL(triggered(bool)), this, SLOT(onPreferencesTriggered()));
	pAction = m_pMainWindow->getActionQuit();
	connect(pAction, SIGNAL(triggered(bool)), this, SLOT(onQuitTriggered()));
	pAction = m_pMainWindow->getActionAddGroup();
	connect(pAction, SIGNAL(triggered(bool)), this, SLOT(onAddGroupTriggered()));
	pAction = m_pMainWindow->getActionSynchronizeTVChannels();
	connect(pAction, SIGNAL(triggered(bool)), this, SLOT(onSynchronizeTVChannelsTriggered()));
	connect(m_pMainWindow->getActionGroupDeinterlace(), SIGNAL(triggered(QAction*)), this, SLOT(onDeinterlaceModeTriggered(QAction*)));
	pAction = m_pMainWindow->getActionAbout();
	connect(pAction, SIGNAL(triggered(bool)), this, SLOT(onAboutTriggered()));

	// Tabs channels
	connect(m_pMainWindow->getLineEditSearch(), SIGNAL(textChanged(const QString&)), this, SLOT(onSearchTextChanged(const QString&)));
	connect(m_pMainWindow->getButtonSearchReset(), SIGNAL(clicked()), this, SLOT(onSearchTextResetClicked()));
	connect(m_pMainWindow->getButtonJumpToChannel(), SIGNAL(clicked()), this, SLOT(onJumpCurrentChannelClicked()));

	// Ctrl bar
	connect(pCtrlBarView->getSliderTime(), SIGNAL(valueChanged(int)), this, SLOT(onSliderTimeChanged(int)));
	connect(pCtrlBarView->getButtonBackward(), SIGNAL(clicked()), this, SLOT(onCtrlBackwardClicked()));
	connect(pCtrlBarView->getButtonPlay(), SIGNAL(clicked()), this, SLOT(onCtrlPlayClicked()));
	connect(pCtrlBarView->getButtonStop(), SIGNAL(clicked()), this, SLOT(onCtrlStopClicked()));
	connect(pCtrlBarView->getButtonRecord(), SIGNAL(clicked()), this, SLOT(onCtrlRecordClicked()));
	connect(pCtrlBarView->getButtonForward(), SIGNAL(clicked()), this, SLOT(onCtrlForwardClicked()));
	connect(pCtrlBarView->getSliderVolume(), SIGNAL(valueChanged(int)), this, SLOT(onSliderVolumeChanged(int)));
	connect(pCtrlBarView->getButtonFullScreen(), SIGNAL(clicked()), this, SLOT(onCtrlFullScreenClicked()));
	connect(pCtrlBarView->getButtonModeMini(), SIGNAL(clicked()), this, SLOT(onCtrlModeMiniClicked()));

	/*
	 * g_signal_connect(G_OBJECT(widget),
					 "focus-in-event",
					 G_CALLBACK(on_windowmain_entryfilter_focusin),
					 app);
	g_signal_connect(G_OBJECT(widget),
					 "focus-out-event",
					 G_CALLBACK(on_windowmain_entryfilter_focusout),
					 app);
					 */

/*
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
	 */


/*
	app->widget.pTrayIcon = gtk_status_icon_new_from_icon_name ("freetuxtv");
	gtk_status_icon_set_visible(app->widget.pTrayIcon, TRUE);
	g_signal_connect(G_OBJECT(app->widget.pTrayIcon), "activate",
					 G_CALLBACK(on_windowmain_trayicon_activate), app);
	g_signal_connect(G_OBJECT(app->widget.pTrayIcon), "popup-menu",
					 G_CALLBACK(on_windowmain_trayicon_popupmenu), app);

	// Window accelerators
	app->widget.pAccelGroup = gtk_accel_group_new ();
	windowmain_add_accelerator (app->widget.pAccelGroup, "F11", "<FreetuxTV>/Fullscreen",
								G_CALLBACK (on_accel_tooglefullscreen), app);
	windowmain_add_accelerator (app->widget.pAccelGroup, "f", "<FreetuxTV>/Fullscreen",
								G_CALLBACK (on_accel_tooglefullscreen), app);
	windowmain_add_accelerator (app->widget.pAccelGroup, "Escape", "<FreetuxTV>/Unfullscreen",
								G_CALLBACK (on_accel_unfullscreen), app);
	windowmain_add_accelerator (app->widget.pAccelGroup, "space", "<FreetuxTV>/PlayPause",
								G_CALLBACK (on_accel_playpause), app);
	windowmain_add_accelerator (app->widget.pAccelGroup, "<Control>Up", "<FreetuxTV>/VolumeUp",
								G_CALLBACK (on_accel_volumeup), app);
	windowmain_add_accelerator (app->widget.pAccelGroup, "<Control>Down", "<FreetuxTV>/VolumeDown",
								G_CALLBACK (on_accel_volumedown), app);
 */
}

void QApplicationMainWindowController::dispose()
{

}


void QApplicationMainWindowController::onPreferencesTriggered()
{
	qDebug("onPreferencesTriggered");
	/*
	FreetuxTVApp *app = (FreetuxTVApp *) user_data;

	GtkWidget *widget;
	widget = (GtkWidget *) gtk_builder_get_object (app->gui,
												   "dialogpreferences");
	gtk_widget_show(widget);

	GtkTreeModel *model;
	GtkTreeIter iter;

	gchar* szTmp = NULL;

	// General
	widget = (GtkWidget *) gtk_builder_get_object (app->gui,
												   "dialogpreferences_channelonstartup");
	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(widget), app->prefs.channelonstartup);

	widget = (GtkWidget *) gtk_builder_get_object (app->gui,
												   "dialogpreferences_enablenotifications");
	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(widget), app->prefs.enable_notifications);

	widget = (GtkWidget *) gtk_builder_get_object (app->gui,
												   "dialogpreferences_ignorediacritics");
	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(widget), app->prefs.ignore_diacritics);

	// LibVLC
	switch(app->prefs.libvlcconfigfile_mode){
		case 0:
			widget = (GtkWidget *) gtk_builder_get_object (app->gui,
														   "dialogpreferences_radioconfigfilenone");
			gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(widget), TRUE);
			break;
		case 1:
			widget = (GtkWidget *) gtk_builder_get_object (app->gui,
														   "dialogpreferences_radioconfigfilecustom");
			gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(widget), TRUE);
			break;
		case 2:
			widget = (GtkWidget *) gtk_builder_get_object (app->gui,
														   "dialogpreferences_radioconfigfilevlc");
			gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(widget), TRUE);
		default:
			break;
	}

	// Recordings
	widget = (GtkWidget *) gtk_builder_get_object (app->gui,
												   "dialogpreferences_directoryrecordings");
	gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (widget),
										 app->prefs.directoryrecordings);

	switch(app->prefs.transcoding_mode){
		case 0:
			widget = (GtkWidget *) gtk_builder_get_object (app->gui,
														   "dialogpreferences_radiotranscodingno");
			gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(widget), TRUE);
			break;
		case 1:
			widget = (GtkWidget *) gtk_builder_get_object (app->gui,
														   "dialogpreferences_radiotranscodingpredefinedformats");
			gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(widget), TRUE);
			break;
		default:
			break;
	}

	model = (GtkTreeModel *) gtk_builder_get_object (app->gui, "liststore_transcodeformat");
	widget = (GtkWidget *) gtk_builder_get_object (app->gui,
												   "dialogpreferences_transcodingformats");
	gtk_tree_model_get_iter_from_string (model, &iter, app->prefs.transcoding_format);
	gtk_combo_box_set_active_iter (GTK_COMBO_BOX(widget), &iter);

	// Network

	widget = (GtkWidget *) gtk_builder_get_object (app->gui,
												   "dialogpreferences_entrytimeout");
	szTmp = g_strdup_printf("%d", app->prefs.timeout);
	gtk_entry_set_text (GTK_ENTRY(widget), szTmp);
	g_free(szTmp);
	szTmp = NULL;

	switch(app->prefs.proxy.proxy_mode){
		case 0:
			widget = (GtkWidget *) gtk_builder_get_object (app->gui,
														   "dialogpreferences_radioproxyno");
			gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(widget), TRUE);
			break;
		case 1:
			widget = (GtkWidget *) gtk_builder_get_object (app->gui,
														   "dialogpreferences_radioproxymanual");
			gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(widget), TRUE);
			break;
		default:
			break;
	}

	widget = (GtkWidget *) gtk_builder_get_object (app->gui,
												   "dialogpreferences_entryproxyserver");
	gtk_entry_set_text (GTK_ENTRY(widget), app->prefs.proxy.proxy_server);

	widget = (GtkWidget *) gtk_builder_get_object (app->gui,
												   "dialogpreferences_entryproxyport");
	gtk_entry_set_text (GTK_ENTRY(widget), app->prefs.proxy.proxy_port);

	widget = (GtkWidget *) gtk_builder_get_object (app->gui,
												   "dialogpreferences_entryproxyport");
	gtk_entry_set_text (GTK_ENTRY(widget), app->prefs.proxy.proxy_port);

	switch(app->prefs.proxy.proxy_mode){
		case G_PROXY_MODE_NONE:
			widget = (GtkWidget *) gtk_builder_get_object (app->gui,
														   "dialogpreferences_radioproxyno");
			gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(widget), TRUE);
			break;
		case G_PROXY_MODE_MANUAL:
			widget = (GtkWidget *) gtk_builder_get_object (app->gui,
														   "dialogpreferences_radioproxymanual");
			gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(widget), TRUE);
			break;
		default:
			break;
	}

	model = (GtkTreeModel *) gtk_builder_get_object (app->gui, "liststore_proxytype");
	widget = (GtkWidget *) gtk_builder_get_object (app->gui,
												   "dialogpreferences_comboboxproxytype");
	gtk_tree_model_get_iter_from_string (model, &iter, app->prefs.proxy.proxy_type);
	gtk_combo_box_set_active_iter (GTK_COMBO_BOX(widget), &iter);

	widget = (GtkWidget *) gtk_builder_get_object (app->gui,
												   "dialogpreferences_checkuseauth");
	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(widget), app->prefs.proxy.proxy_use_auth);

	widget = (GtkWidget *) gtk_builder_get_object (app->gui,
												   "dialogpreferences_entryproxyusername");
	gtk_entry_set_text (GTK_ENTRY(widget), app->prefs.proxy.proxy_username);

	widget = (GtkWidget *) gtk_builder_get_object (app->gui,
												   "dialogpreferences_entryproxypassword");
	gtk_entry_set_text (GTK_ENTRY(widget), app->prefs.proxy.proxy_password);


	dialogpreferences_update_view (app);
	*/
}

void QApplicationMainWindowController::onQuitTriggered()
{
	QCoreApplication::quit();
}

void QApplicationMainWindowController::onAddGroupTriggered()
{
	qDebug("onAddGroupTriggered");
	/*
	FreetuxTVApp *app = (FreetuxTVApp *) user_data;

	GError* error = NULL;

	FreetuxTVWindowAddChannelsGroup* pWindowAddChannelsGroups;

	GtkWindow* pWindow;
	GtkWidget* pParent;
	pParent = (GtkWidget *) gtk_builder_get_object (app->gui, "windowmain");

	pWindowAddChannelsGroups = freetuxtv_window_add_channels_group_new (
			GTK_WINDOW(pParent), app, &error);

	if(error == NULL){
		pWindow = gtk_builder_window_get_top_window (GTK_BUILDER_WINDOW(pWindowAddChannelsGroups));
		gtk_widget_show(GTK_WIDGET(pWindow));

		GtkTreePath** ppCurrentTreePath = g_new0 (GtkTreePath*, 1);

		g_signal_connect(G_OBJECT(pWindowAddChannelsGroups), "channels-group-added",
						 G_CALLBACK(on_channels_group_added), ppCurrentTreePath);
		g_signal_connect(G_OBJECT(pWindowAddChannelsGroups), "channels-added",
						 G_CALLBACK(on_channels_added), ppCurrentTreePath);
		g_signal_connect(G_OBJECT(pWindowAddChannelsGroups), "destroy",
						 G_CALLBACK(on_window_add_channels_group_destroy), ppCurrentTreePath);
	}

	if(error != NULL){
		windowmain_show_gerror (app, error);
		g_error_free (error);
	}
	*/
}

void QApplicationMainWindowController::onSynchronizeTVChannelsTriggered()
{
	qDebug("onSynchronizeTVChannelsTriggered");
	/*
	FreetuxTVApp *app = (FreetuxTVApp *) user_data;

	GError* error = NULL;

	GtkWindow* pWindow;
	GtkWidget* pParent;
	pParent = (GtkWidget *) gtk_builder_get_object (app->gui, "windowmain");

	FreetuxTVWindowTVChannelsDatabase* pWindowTVChannelsDatabase;
	pWindowTVChannelsDatabase = freetuxtv_window_tv_channels_database_new(GTK_WINDOW(pParent), app);

	if(error == NULL){
		pWindow = gtk_builder_window_get_top_window (GTK_BUILDER_WINDOW(pWindowTVChannelsDatabase));
		gtk_widget_show(GTK_WIDGET(pWindow));
	}

	if(error != NULL){
		windowmain_show_gerror (app, error);
		g_error_free (error);
	}
	*/
}

void QApplicationMainWindowController::onDeinterlaceModeTriggered(QAction *action)
{
	qDebug("onDeinterlaceModeTriggered");

	const char* szMode = NULL;
	if(action == m_pMainWindow->getActionDeinterlaceDisable()){
		szMode = NULL;
	}else if(action == m_pMainWindow->getActionDeinterlaceBlend()){
		szMode = "blenc";
	}else if(action == m_pMainWindow->getActionDeinterlaceBob()){
		szMode = "bob";
	}else if(action == m_pMainWindow->getActionDeinterlaceDiscard()){
		szMode = "discard";
	}else if(action == m_pMainWindow->getActionDeinterlaceLinear()){
		szMode = "linear";
	}else if(action == m_pMainWindow->getActionDeinterlaceMean()){
		szMode = "mean";
	}else if(action == m_pMainWindow->getActionDeinterlaceX()){
		szMode = "x";
	}

/*
	FreetuxTVApp *app = (FreetuxTVApp *) user_data;

	GError* error = NULL;
	DBSync dbsync;
	FreetuxTVChannelInfos* pChannel;

	if(gtk_check_menu_item_get_active (GTK_CHECK_MENU_ITEM(menuitem))){
		freetuxtv_action_deinterlace (app, mode, &error);

		if(app->current.pPathChannel){
			if(error == NULL){
				dbsync_open_db (&dbsync, &error);
			}

			if(error == NULL){
				pChannel = channels_list_get_channel (app, app->current.pPathChannel);
				dbsync_update_channel_deinterlace_mode (&dbsync, pChannel, (gchar*)mode, &error);
			}

			dbsync_close_db(&dbsync);
		}
	}

	if(error != NULL){
		windowmain_show_gerror (app, error);
		g_error_free (error);
	}
*/

}

void QApplicationMainWindowController::onAboutTriggered()
{
	qDebug("onAboutTriggered");

	/*
		FreetuxTVApp *app = (FreetuxTVApp *) user_data;
		GtkWidget *widget;
		widget =  (GtkWidget *) gtk_builder_get_object (app->gui,
														"aboutdialog");
		gtk_widget_show(widget);
	*/
}


void QApplicationMainWindowController::onSearchTextChanged(const QString& szText)
{
	qDebug("onSearchTextChanged");

	/*
	FreetuxTVApp *app = (FreetuxTVApp *) user_data;

	GtkWidget *treeview;
	GtkTreeModel *model;
	treeview = (GtkWidget *) gtk_builder_get_object (app->gui,
	                                                 "windowmain_treeviewchannelslist");

	model = gtk_tree_view_get_model (GTK_TREE_VIEW(treeview));
	gtk_tree_model_filter_refilter (GTK_TREE_MODEL_FILTER(model));

	const gchar* text;
	text = gtk_entry_get_text (entry);

	if(g_ascii_strcasecmp(text, "") == 0){
		gtk_tree_view_collapse_all (GTK_TREE_VIEW(treeview));
		channels_list_set_playing (app, app->current.pPathChannel);
	}else{
		gtk_tree_view_expand_all (GTK_TREE_VIEW(treeview));
	}
	*/
}

void QApplicationMainWindowController::onSearchTextResetClicked()
{
	m_pMainWindow->getLineEditSearch()->clear();

	/*
	FreetuxTVApp *app = (FreetuxTVApp *) user_data;

	GtkWidget *treeview;
	GtkTreeModel *model;
	treeview =  (GtkWidget *) gtk_builder_get_object (app->gui,
													  "windowmain_treeviewchannelslist");

	model = gtk_tree_view_get_model (GTK_TREE_VIEW(treeview));
	gtk_tree_model_filter_refilter (GTK_TREE_MODEL_FILTER(model));

	gtk_tree_view_collapse_all (GTK_TREE_VIEW(treeview));
	channels_list_set_playing (app, app->current.pPathChannel);
	 */

}

void QApplicationMainWindowController::onJumpCurrentChannelClicked()
{
	qDebug("onJumpCurrentChannelClicked");
	/*
	FreetuxTVApp *app = (FreetuxTVApp *) user_data;
	channels_list_set_playing(app, app->current.pPathChannel);
	*/
}

void QApplicationMainWindowController::onSliderTimeChanged(int value)
{
	qDebug("onSliderTimeChanged");
	/*
	FreetuxTVApp *app = (FreetuxTVApp *) user_data;

	GError* error = NULL;

	//g_print("change %f\n", value); // TODO
	if(!gtk_libvlc_media_player_is_playing(app->player, &error)){
		gtk_libvlc_media_player_play(app->player, NULL, &error);
	}
	if(value >= 0.0 && value <= 1.0){
		gtk_libvlc_media_player_set_position(app->player, value, &error);
	}

	if(error != NULL){
		g_error_free (error);
		error = NULL;
	}
	return FALSE;
	 */
}

void QApplicationMainWindowController::onCtrlPlayClicked()
{
	qDebug("onCtrlPlayClicked");

	/*
	GError* error = NULL;

	FreetuxTVApp *app = (FreetuxTVApp *) user_data;
	freetuxtv_action_playpause (app, &error);

	if(error != NULL){
		windowmain_show_gerror (app, error);
		g_error_free (error);
	}
	*/
}

void QApplicationMainWindowController::onCtrlStopClicked()
{
	qDebug("onCtrlStopClicked");
	/*
	GError* error = NULL;

	FreetuxTVApp *app = (FreetuxTVApp *) user_data;

	if(app->current.is_recording){
		freetuxtv_action_stop_recording(app, app->current.recording.pRecordingInfo, &error);
	}else{
		freetuxtv_action_stop (app, &error);
	}

	if(error != NULL){
		windowmain_show_gerror (app, error);
		g_error_free (error);
	}
	 */
}

void QApplicationMainWindowController::onCtrlRecordClicked()
{
	qDebug("onCtrlRecordClicked");
	/*
	FreetuxTVApp *app = (FreetuxTVApp *) user_data;

	GError* error = NULL;
	FreetuxTVChannelInfos* pChannelInfos;

	// Show properties to the channel corresponding to the path
	pChannelInfos = channels_list_get_channel (app, app->current.pPathChannel);

	windowmain_show_dialog_addrecordings (app, pChannelInfos, &error);

	if(error != NULL){
		windowmain_show_gerror (app, error);
		g_error_free (error);
	}
	 */
}

void QApplicationMainWindowController::onCtrlBackwardClicked()
{
	qDebug("onCtrlBackwardClicked");
	/*
	GError* error = NULL;

	FreetuxTVApp *app = (FreetuxTVApp *) user_data;
	freetuxtv_action_prev (app, &error);

	if(error != NULL){
		windowmain_show_gerror (app, error);
		g_error_free (error);
	}
	 */
}

void QApplicationMainWindowController::onCtrlForwardClicked()
{
	qDebug("onCtrlForwardClicked");
/*
	GError* error = NULL;

	FreetuxTVApp *app = (FreetuxTVApp *) user_data;
	freetuxtv_action_next (app, &error);

	if(error != NULL){
		windowmain_show_gerror (app, error);
		g_error_free (error);
	}
	*/
}

void QApplicationMainWindowController::onSliderVolumeChanged(int value)
{
	qDebug("onSliderVolumeChanged");
	/*
	FreetuxTVApp *app = (FreetuxTVApp *) user_data;

	GError* error = NULL;

	app->config.volume = gtk_range_get_value (range);
	gtk_libvlc_media_player_set_volume (app->player, app->config.volume, NULL);

	if(error != NULL){
		windowmain_show_gerror (app, error);
		g_error_free (error);
	}
	 */
}

void QApplicationMainWindowController::onCtrlFullScreenClicked()
{
	qDebug("onCtrlFullScreenClicked");

/*
	GError* error = NULL;

	FreetuxTVApp *app = (FreetuxTVApp *) user_data;
	gtk_libvlc_media_player_set_fullscreen (app->player, TRUE, &error);

	if(error != NULL){
		windowmain_show_gerror (app, error);
		g_error_free (error);
	}
	*/
}

void QApplicationMainWindowController::onCtrlModeMiniClicked()
{
	qDebug("onCtrlModeMiniClicked");

	/*
	FreetuxTVApp *app = (FreetuxTVApp *) user_data;

	GtkWidget *widget;

	widget = (GtkWidget *) gtk_builder_get_object (app->gui,
												   "windowmain");
	gtk_widget_hide(widget);

	// Display the mini mode window
	widget = (GtkWidget *) gtk_builder_get_object (app->gui,
												   "windowminimode");
	gtk_widget_show(widget);
	gtk_window_set_keep_above (GTK_WINDOW(widget),
							   app->config.windowminimode_stayontop);

	gtk_window_resize (GTK_WINDOW(widget),
					   app->config.windowminimode_width,
					   app->config.windowminimode_height);

	widget = (GtkWidget *) gtk_builder_get_object (app->gui,
												   "windowminimode_buttonstayontop");
	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(widget),
								  app->config.windowminimode_stayontop);

	widget = (GtkWidget *) gtk_builder_get_object (app->gui,
												   "windowminimode_eventboxplayer");
	gtk_widget_reparent (GTK_WIDGET(app->player), widget);
	 */
}

/*
static void
on_windowmain_menuitemmute_activate (GtkMenuItem *menuitem,
									 gpointer user_data)
{
	FreetuxTVApp *app = (FreetuxTVApp *) user_data;
	GError* error = NULL;
	gboolean mute;

	mute = gtk_libvlc_media_player_get_mute (app->player, NULL);
	if(!error){
		gtk_libvlc_media_player_set_mute (app->player, !mute, NULL);
	}

	if(error != NULL){
		windowmain_show_gerror (app, error);
		g_error_free (error);
	}
}
 */


/*

static void
windowmain_add_accelerator (GtkAccelGroup *p_accel_group,
                            const gchar *accelerator, const gchar *accel_path,
                            GCallback callback, gpointer user_data);

static void
on_windowmain_trayicon_activate(GtkStatusIcon *status_icon, gpointer user_data);

static void
on_windowmain_trayicon_popupmenu(GtkStatusIcon *status_icon, guint button,
                                 guint activate_time, gpointer user_data);


static void
on_windowmain_menuitemmute_activate (GtkMenuItem *menuitem,
                                     gpointer user_data);

static gboolean
on_windowmain_entryfilter_focusin (GtkWidget *widget, GdkEventFocus *event,
                                   gpointer user_data);

static gboolean
on_windowmain_entryfilter_focusout (GtkWidget *widget, GdkEventFocus *event,
                                    gpointer user_data);

static void
on_windowminimode_buttonnormalmode_clicked (GtkButton *button,
                                            gpointer user_data);

static void
on_windowminimode_buttonstayontop_clicked (GtkButton *button,
                                           gpointer user_data);

static void
on_dialogpreferences_directoryrecordings_changed (GtkFileChooser *chooser,
                                                  gpointer user_data);

static void
on_dialogpreferences_radiotranscoding_toggled (GtkToggleButton *togglebutton,
                                               gpointer user_data);

static void
on_dialogpreferences_transcodingformats_changed (GtkComboBox *widget,
                                                 gpointer user_data);

static void
on_dialogpreferences_radioproxy_toggled (GtkToggleButton *togglebutton,
                                         gpointer user_data);

static void
on_dialogpreferences_checkuseauth_toggled (GtkToggleButton *togglebutton,
                                     	   gpointer user_data);

static void
on_dialogpreferences_response (GtkDialog *dialog,
                               gint response_id,
                               gpointer   user_data);

static void
on_dialoggroupproperties_response (GtkDialog *dialog,
                                   gint response_id,
                                   gpointer user_data);

static void
on_aboutdialog_response (GtkDialog *dialog,
                         gint response_id,
                         gpointer user_data);

static void
dialogpreferences_update_view(FreetuxTVApp *app);

static gboolean
on_accel_playpause (GtkAccelGroup *accel_group, GObject *acceleratable, guint keyval,
                    GdkModifierType modifier, gpointer user_data);

static gboolean
on_accel_tooglefullscreen (GtkAccelGroup *accel_group, GObject *acceleratable, guint keyval,
                           GdkModifierType modifier, gpointer user_data);

static gboolean
on_accel_unfullscreen (GtkAccelGroup *accel_group, GObject *acceleratable, guint keyval,
                       GdkModifierType modifier, gpointer user_data);

static gboolean
on_accel_volumeup (GtkAccelGroup *accel_group, GObject *acceleratable, guint keyval,
                   GdkModifierType modifier, gpointer user_data);

static gboolean
on_accel_volumedown (GtkAccelGroup *accel_group, GObject *acceleratable, guint keyval,
    GdkModifierType modifier, gpointer user_data);

static void
on_channels_group_added (
	    FreetuxTVWindowAddChannelsGroup *pWindowAddChannelsGroup,
	    FreetuxTVChannelsGroupInfos* pChannelsGroupInfos,
	    DBSync *dbsync, GError** error,
	    gpointer user_data);


	    static void
on_windowmain_trayicon_activate(GtkStatusIcon *status_icon, gpointer user_data)
{
	FreetuxTVApp *app = (FreetuxTVApp *) user_data;

	GtkWidget *widget;
	widget =  (GtkWidget *) gtk_builder_get_object (app->gui,
	                                                "windowmain");
	widget =  gtk_widget_get_toplevel(GTK_WIDGET(app->player));

	gboolean bIsVisible;

#if GTK_API_VERSION == 3
	bIsVisible = gtk_widget_get_visible(widget);
	if(bIsVisible){
		gtk_widget_set_visible(widget, FALSE);
	}else{
		gtk_widget_set_visible(widget, TRUE);
	}
#else
	bIsVisible = GTK_WIDGET_FLAGS (widget) & GTK_VISIBLE;
	if(bIsVisible){
		gtk_widget_hide(widget);
	}else{
		gtk_widget_show(widget);
	}
#endif
}

static void
on_windowmain_trayicon_popupmenu (GtkStatusIcon *status_icon, guint button,
                                  guint activate_time, gpointer user_data)
{
	GtkWidget *pMenu;
	GtkWidget *pMenuItem;
	FreetuxTVApp *app = (FreetuxTVApp *) user_data;

	gboolean bVolumeDisabled;

	g_return_if_fail (status_icon != NULL);
	g_return_if_fail (GTK_IS_STATUS_ICON (status_icon));

	bVolumeDisabled = gtk_libvlc_media_player_get_mute (app->player, NULL);

	pMenu = gtk_menu_new();

	// Create the menu items
	pMenuItem = gtk_check_menu_item_new_with_label (_("Mute"));
#if GTK_API_VERSION == 3
#else
	gtk_check_menu_item_set_show_toggle (GTK_CHECK_MENU_ITEM(pMenuItem), FALSE);
#endif
	gtk_check_menu_item_set_active (GTK_CHECK_MENU_ITEM(pMenuItem), bVolumeDisabled);
	gtk_menu_shell_append (GTK_MENU_SHELL (pMenu), pMenuItem);
	g_signal_connect(G_OBJECT(pMenuItem),
	                 "activate",
	                 G_CALLBACK(on_windowmain_menuitemmute_activate),
	                 app);
	gtk_widget_show(pMenuItem);

	pMenuItem = gtk_separator_menu_item_new();
	gtk_menu_shell_append (GTK_MENU_SHELL (pMenu), pMenuItem);
	gtk_widget_show(pMenuItem);

	pMenuItem = gtk_image_menu_item_new_from_stock(GTK_STOCK_QUIT, NULL);
	gtk_menu_shell_append (GTK_MENU_SHELL (pMenu), pMenuItem);
	g_signal_connect(G_OBJECT(pMenuItem),
	                 "activate",
	                 G_CALLBACK(on_windowmain_menuitemquit_activate),
	                 app);
	gtk_widget_show(pMenuItem);

	gtk_widget_show(pMenu);

	gtk_menu_popup (GTK_MENU(pMenu), NULL, NULL, gtk_status_icon_position_menu, status_icon, button, activate_time);
}

static gboolean
on_windowmain_entryfilter_focusin (GtkWidget *widget, GdkEventFocus *event,
                                   gpointer user_data)
{
	FreetuxTVApp *app = (FreetuxTVApp *) user_data;
	windowmain_enable_accelerators (app, FALSE);
	return FALSE;
}

static gboolean
on_windowmain_entryfilter_focusout (GtkWidget *widget, GdkEventFocus *event,
                                    gpointer user_data)
{
	FreetuxTVApp *app = (FreetuxTVApp *) user_data;
	windowmain_enable_accelerators (app, TRUE);
	return FALSE;
}

static void
on_channels_group_added (
	    FreetuxTVWindowAddChannelsGroup *pWindowAddChannelsGroup,
	    FreetuxTVChannelsGroupInfos* pChannelsGroupInfos,
	    DBSync *dbsync, GError** error,
	    gpointer user_data)
{
	g_log(FREETUXTV_LOG_DOMAIN, G_LOG_LEVEL_INFO,
	      "Received signal for channels group added '%s'\n", pChannelsGroupInfos->name);

	FreetuxTVApp *app;
	app = freetuxtv_window_add_channels_group_get_app(pWindowAddChannelsGroup);

	GtkTreePath** ppTreePath = (GtkTreePath**)user_data;

	if(ppTreePath){
		gtk_tree_path_free (*ppTreePath);
		*ppTreePath = NULL;
	}

	channels_list_ui_add_channels_group (app, pChannelsGroupInfos, ppTreePath);
}

static void
on_channels_added (
	    FreetuxTVWindowAddChannelsGroup *pWindowAddChannelsGroup,
	    FreetuxTVChannelsGroupInfos* pChannelsGroupInfos,
	    DBSync *dbsync, GError** error,
	    gpointer user_data)
{
	g_log(FREETUXTV_LOG_DOMAIN, G_LOG_LEVEL_INFO,
	      "Received signal for channels added in group '%s'\n", pChannelsGroupInfos->name);

	FreetuxTVApp *app;
	app = freetuxtv_window_add_channels_group_get_app(pWindowAddChannelsGroup);

	GtkTreePath** ppTreePath = (GtkTreePath**)user_data;

	if(ppTreePath){
		channels_list_reload_channels_of_channels_group (app, dbsync, *ppTreePath, error);
	}
}

static void
on_window_add_channels_group_destroy (GtkBuilderWidget *self, gpointer user_data)
{
	GtkTreePath** ppCurrentTreePath = (GtkTreePath**)user_data;

	if(ppCurrentTreePath){
		if(*ppCurrentTreePath){
			gtk_tree_path_free (*ppCurrentTreePath);
			*ppCurrentTreePath = NULL;
		}

		g_free(ppCurrentTreePath);
		ppCurrentTreePath = NULL;
	}
}

static void
on_windowminimode_buttonnormalmode_clicked (GtkButton *button,
                                            gpointer user_data)
{
	FreetuxTVApp *app = (FreetuxTVApp *) user_data;

	GtkWidget *widget;

	widget = (GtkWidget *) gtk_builder_get_object (app->gui,
	                                               "windowminimode");
	gtk_window_get_size (GTK_WINDOW(widget),
	                     &app->config.windowminimode_width,
	                     &app->config.windowminimode_height);

	gtk_widget_hide (widget);

	// Display the normal mode window
	widget = (GtkWidget *) gtk_builder_get_object (app->gui,
	                                               "windowmain");
	gtk_widget_show (widget);

	widget = (GtkWidget *) gtk_builder_get_object (app->gui,
	                                               "windowmain_eventboxplayer");
	gtk_widget_reparent (GTK_WIDGET(app->player), widget);
}

static void
on_windowminimode_buttonstayontop_clicked (GtkButton *button,
                                           gpointer user_data)
{
	FreetuxTVApp *app = (FreetuxTVApp *) user_data;
	GtkWidget *widget;
	gboolean bActive;

	widget = (GtkWidget *) gtk_builder_get_object (app->gui,
	    "windowminimode_buttonstayontop");
	bActive = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(widget));

	if(bActive){
		app->config.windowminimode_stayontop = TRUE;
	}else{
		app->config.windowminimode_stayontop = FALSE;
	}

	widget = (GtkWidget *) gtk_builder_get_object (app->gui,
	                                               "windowminimode");
	gtk_window_set_keep_above (GTK_WINDOW(widget),
	                           app->config.windowminimode_stayontop);
}

static void
on_dialogpreferences_directoryrecordings_changed (GtkFileChooser *chooser,
                                                  gpointer user_data)
{
	FreetuxTVApp *app = (FreetuxTVApp*)user_data;
	dialogpreferences_update_view(app);
}

static void
on_dialogpreferences_radiotranscoding_toggled (GtkToggleButton *togglebutton,
                                               gpointer user_data)
{
	FreetuxTVApp *app = (FreetuxTVApp*)user_data;
	dialogpreferences_update_view(app);
}

static void
on_dialogpreferences_transcodingformats_changed (GtkComboBox *widget,
                                                 gpointer user_data)
{
	FreetuxTVApp *app = (FreetuxTVApp*)user_data;
	dialogpreferences_update_view(app);
}

static void
on_dialogpreferences_radioproxy_toggled (GtkToggleButton *togglebutton,
                                         gpointer user_data)
{
	FreetuxTVApp *app = (FreetuxTVApp*)user_data;
	dialogpreferences_update_view(app);
}

static void
on_dialogpreferences_checkuseauth_toggled (GtkToggleButton *togglebutton,
                                     	   gpointer user_data)
{
	FreetuxTVApp *app = (FreetuxTVApp*)user_data;
	dialogpreferences_update_view(app);
}

static void
on_dialogpreferences_response (GtkDialog *dialog,
                               gint response_id,
                               gpointer user_data)
{
	FreetuxTVApp *app = (FreetuxTVApp*)user_data;
	GtkWidget* widget;
	gboolean bIsOk = TRUE;
	const char* text;

	GtkTreeModel *model;
	GtkTreeIter iter;


	if(response_id == GTK_RESPONSE_APPLY){
		// Get prefs general
		widget = (GtkWidget *) gtk_builder_get_object (app->gui,
		                                               "dialogpreferences_channelonstartup");
		app->prefs.channelonstartup = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget));

		widget = (GtkWidget *) gtk_builder_get_object (app->gui,
		                                               "dialogpreferences_enablenotifications");
		app->prefs.enable_notifications = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget));

		widget = (GtkWidget *) gtk_builder_get_object (app->gui,
		                                               "dialogpreferences_ignorediacritics");
		app->prefs.ignore_diacritics = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget));

		// Get prefs libvlc
		widget = (GtkWidget *) gtk_builder_get_object (app->gui,
		                                               "dialogpreferences_radioconfigfilenone");
		if(gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(widget))){
			app->prefs.libvlcconfigfile_mode = 0;
		}

		widget = (GtkWidget *) gtk_builder_get_object (app->gui,
		                                               "dialogpreferences_radioconfigfilecustom");
		if(gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(widget))){
			app->prefs.libvlcconfigfile_mode = 1;
		}

		widget = (GtkWidget *) gtk_builder_get_object (app->gui,
		                                               "dialogpreferences_radioconfigfilevlc");
		if(gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(widget))){
			app->prefs.libvlcconfigfile_mode = 2;
		}


		// Get prefs recording
		widget = (GtkWidget *) gtk_builder_get_object (app->gui,
		                                               "dialogpreferences_directoryrecordings");
		g_free(app->prefs.directoryrecordings);
		app->prefs.directoryrecordings = gtk_file_chooser_get_current_folder(GTK_FILE_CHOOSER (widget));

		widget = (GtkWidget *) gtk_builder_get_object (app->gui,
		                                               "dialogpreferences_radiotranscodingno");
		if(gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(widget))){
			app->prefs.transcoding_mode = 0;
		}

		widget = (GtkWidget *) gtk_builder_get_object (app->gui,
		                                               "dialogpreferences_radiotranscodingpredefinedformats");
		if(gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(widget))){
			app->prefs.transcoding_mode = 1;
		}

		model = (GtkTreeModel *) gtk_builder_get_object (app->gui, "liststore_transcodeformat");
		widget = (GtkWidget *) gtk_builder_get_object (app->gui,
		                                               "dialogpreferences_transcodingformats");
		gtk_combo_box_get_active_iter (GTK_COMBO_BOX(widget), &iter);
		app->prefs.transcoding_format = gtk_tree_model_get_string_from_iter (model, &iter);

		// Get prefs network
		widget = (GtkWidget *) gtk_builder_get_object (app->gui,
		                                               "dialogpreferences_entrytimeout");
		text = gtk_entry_get_text (GTK_ENTRY(widget));
		app->prefs.timeout = atoi(text);

		widget = (GtkWidget *) gtk_builder_get_object (app->gui,
		                                               "dialogpreferences_radioproxyno");
		if(gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(widget))){
			app->prefs.proxy.proxy_mode = G_PROXY_MODE_NONE;
		}

		widget = (GtkWidget *) gtk_builder_get_object (app->gui,
		                                               "dialogpreferences_radioproxymanual");
		if(gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(widget))){
			app->prefs.proxy.proxy_mode = G_PROXY_MODE_MANUAL;
		}

		widget = (GtkWidget *) gtk_builder_get_object (app->gui,
		                                               "dialogpreferences_entryproxyserver");
		text = gtk_entry_get_text (GTK_ENTRY(widget));
		if(app->prefs.proxy.proxy_server){
			g_free(app->prefs.proxy.proxy_server);
			app->prefs.proxy.proxy_server = NULL;
		}
		app->prefs.proxy.proxy_server = g_strdup(text);

		widget = (GtkWidget *) gtk_builder_get_object (app->gui,
		                                               "dialogpreferences_entryproxyport");
		text = gtk_entry_get_text (GTK_ENTRY(widget));
		if(app->prefs.proxy.proxy_port){
			g_free(app->prefs.proxy.proxy_port);
			app->prefs.proxy.proxy_port = NULL;
		}
		app->prefs.proxy.proxy_port = g_strdup(text);

		model = (GtkTreeModel *) gtk_builder_get_object (app->gui, "liststore_proxytype");
		widget = (GtkWidget *) gtk_builder_get_object (app->gui,
		                                               "dialogpreferences_comboboxproxytype");
		gtk_combo_box_get_active_iter (GTK_COMBO_BOX(widget), &iter);
		app->prefs.proxy.proxy_type = gtk_tree_model_get_string_from_iter (model, &iter);

		widget = (GtkWidget *) gtk_builder_get_object (app->gui,
		                                               "dialogpreferences_checkuseauth");
		app->prefs.proxy.proxy_use_auth = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget));

		widget = (GtkWidget *) gtk_builder_get_object (app->gui,
		                                               "dialogpreferences_entryproxyusername");
		text = gtk_entry_get_text (GTK_ENTRY(widget));
		if(app->prefs.proxy.proxy_username){
			g_free(app->prefs.proxy.proxy_username);
			app->prefs.proxy.proxy_username = NULL;
		}
		app->prefs.proxy.proxy_username = g_strdup(text);

		widget = (GtkWidget *) gtk_builder_get_object (app->gui,
		                                               "dialogpreferences_entryproxypassword");
		text = gtk_entry_get_text (GTK_ENTRY(widget));
		if(app->prefs.proxy.proxy_password){
			g_free(app->prefs.proxy.proxy_password);
			app->prefs.proxy.proxy_password = NULL;
		}
		app->prefs.proxy.proxy_password = g_strdup(text);
	}

	if(bIsOk){
		gtk_widget_hide(GTK_WIDGET(dialog));
	}
}

static void
on_dialoggroupproperties_response (GtkDialog *dialog,
                                   gint response_id,
                                   gpointer user_data)
{
	FreetuxTVApp *app = (FreetuxTVApp *) user_data;
	gchar *errmsg = NULL;

	FreetuxTVChannelsGroupInfos *pChannelsGroupInfos;

	if(response_id == GTK_RESPONSE_APPLY){
		GtkWidget *groupname;
		GtkWidget *groupuri;
		GtkWidget *bregex;
		GtkWidget *eregex;

		const gchar *sgroupname;
		const gchar *sgroupuri;
		const gchar *sbregex;
		const gchar *seregex;

		groupname = (GtkWidget *) gtk_builder_get_object (app->gui,
		                                                  "dialoggroupproperties_name");
		groupuri = (GtkWidget *) gtk_builder_get_object (app->gui,
		                                                 "dialoggroupproperties_uri");
		bregex = (GtkWidget *) gtk_builder_get_object (app->gui,
		                                               "dialoggroupproperties_bregex");
		eregex = (GtkWidget *) gtk_builder_get_object (app->gui,
		                                               "dialoggroupproperties_eregex");

		sgroupname = gtk_entry_get_text(GTK_ENTRY(groupname));
		sgroupuri = gtk_entry_get_text(GTK_ENTRY(groupuri));
		sbregex = gtk_entry_get_text(GTK_ENTRY(bregex));
		seregex = gtk_entry_get_text(GTK_ENTRY(eregex));

		pChannelsGroupInfos = (FreetuxTVChannelsGroupInfos *)g_object_get_data (G_OBJECT(dialog), "ChannelsGroup");

		// Check the field
		if(g_ascii_strcasecmp(sgroupname,"") == 0 && errmsg == NULL){
			errmsg = g_strdup_printf(_("Please enter the group's name !"));
		}

		if(pChannelsGroupInfos->type == FREETUXTV_CHANNELSGROUP_TYPEGROUP_PLAYLIST){
			if(g_ascii_strcasecmp(sgroupuri,"") == 0 && errmsg == NULL){
				errmsg = g_strdup_printf(_("Please enter the group's URI !"));
			}
		}

		if(errmsg != NULL){
			windowmain_show_error (app, errmsg);
		}else{

			if(g_ascii_strcasecmp(sbregex,"") == 0 ){
				bregex = NULL;
			}
			if(g_ascii_strcasecmp(seregex,"") == 0 ){
				seregex = NULL;
			}

			GtkTreePath *path_group;
			path_group = (GtkTreePath *)g_object_get_data (G_OBJECT(dialog), "PathChannelsGroup");

			freetuxtv_channels_group_infos_set_name (pChannelsGroupInfos, (gchar*)sgroupname);

			if(pChannelsGroupInfos->type == FREETUXTV_CHANNELSGROUP_TYPEGROUP_PLAYLIST){
				freetuxtv_channels_group_infos_set_uri (pChannelsGroupInfos, (gchar*)sgroupuri);
				freetuxtv_channels_group_infos_set_regex (pChannelsGroupInfos, (gchar*)sbregex, (gchar*)seregex);
			}

			channels_list_update_channels_group (app, path_group, pChannelsGroupInfos);

			g_object_set_data (G_OBJECT(dialog), "ChannelsGroup", NULL);
			g_object_set_data (G_OBJECT(dialog), "PathChannelsGroup", NULL);

			gtk_widget_hide(GTK_WIDGET(dialog));
		}

		g_free(errmsg);
	}
	if (response_id == GTK_RESPONSE_CANCEL){
		gtk_widget_hide(GTK_WIDGET(dialog));
	}
}

static void
on_aboutdialog_response (GtkDialog *dialog,
                         gint response_id,
                         gpointer user_data)
{
	gtk_widget_hide(GTK_WIDGET(dialog));
}


static void
dialogpreferences_update_view(FreetuxTVApp *app)
{
	GtkWidget* widget;
	gboolean bIsManualProxy = FALSE;
	gboolean bUseAuth = FALSE;

	// Refresh recording string
	widget = (GtkWidget *) gtk_builder_get_object (app->gui,
	                                                   "textbuffer_previewtranscoding");
	gchar *text;
	text = get_recording_options(app, "%filename%", TRUE, NULL);
	gtk_text_buffer_set_text (GTK_TEXT_BUFFER(widget), text, -1);
	g_free(text);

	// Refresh the network view
	widget = (GtkWidget *) gtk_builder_get_object (app->gui,
	                                               "dialogpreferences_radioproxymanual");
	if(gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(widget))){
		bIsManualProxy = TRUE;
	}else{
		bIsManualProxy = FALSE;
	}

	widget = (GtkWidget *) gtk_builder_get_object (app->gui,
	                                               "dialogpreferences_checkuseauth");
	if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget))){
		bUseAuth = TRUE;
	}else{
		bUseAuth = FALSE;
	}

	if(bIsManualProxy){
		// Disable all widget
		widget = (GtkWidget *) gtk_builder_get_object (app->gui,
		                                               "dialogpreferences_entryproxyserver");
		gtk_widget_set_sensitive (widget, TRUE);

		widget = (GtkWidget *) gtk_builder_get_object (app->gui,
		                                               "dialogpreferences_entryproxyport");
		gtk_widget_set_sensitive (widget, TRUE);

		widget = (GtkWidget *) gtk_builder_get_object (app->gui,
		                                               "dialogpreferences_comboboxproxytype");
		gtk_widget_set_sensitive (widget, TRUE);

		widget = (GtkWidget *) gtk_builder_get_object (app->gui,
		                                               "dialogpreferences_checkuseauth");
		gtk_widget_set_sensitive (widget, TRUE);

		widget = (GtkWidget *) gtk_builder_get_object (app->gui,
		                                               "dialogpreferences_entryproxyusername");
		gtk_widget_set_sensitive (widget, bUseAuth);

		widget = (GtkWidget *) gtk_builder_get_object (app->gui,
		                                               "dialogpreferences_entryproxypassword");
		gtk_widget_set_sensitive (widget, bUseAuth);

	}else{
		// Disable all widget
		widget = (GtkWidget *) gtk_builder_get_object (app->gui,
		                                               "dialogpreferences_entryproxyserver");
		gtk_widget_set_sensitive (widget, FALSE);

		widget = (GtkWidget *) gtk_builder_get_object (app->gui,
		                                               "dialogpreferences_entryproxyport");
		gtk_widget_set_sensitive (widget, FALSE);

		widget = (GtkWidget *) gtk_builder_get_object (app->gui,
		                                               "dialogpreferences_comboboxproxytype");
		gtk_widget_set_sensitive (widget, FALSE);

		widget = (GtkWidget *) gtk_builder_get_object (app->gui,
		                                               "dialogpreferences_checkuseauth");
		gtk_widget_set_sensitive (widget, FALSE);

		widget = (GtkWidget *) gtk_builder_get_object (app->gui,
		                                               "dialogpreferences_entryproxyusername");
		gtk_widget_set_sensitive (widget, FALSE);

		widget = (GtkWidget *) gtk_builder_get_object (app->gui,
		                                               "dialogpreferences_entryproxypassword");
		gtk_widget_set_sensitive (widget, FALSE);
	}
}

static gboolean
on_accel_playpause (GtkAccelGroup *accel_group, GObject *acceleratable, guint keyval,
                     GdkModifierType modifier, gpointer user_data)
{
	GError* error = NULL;

	FreetuxTVApp *app = (FreetuxTVApp *) user_data;
	freetuxtv_action_playpause (app, &error);

	if(error != NULL){
		windowmain_show_gerror (app, error);
		g_error_free (error);
	}
	return TRUE;
}

static gboolean
on_accel_tooglefullscreen (GtkAccelGroup *accel_group, GObject *acceleratable, guint keyval,
                           GdkModifierType modifier, gpointer user_data)
{
	GError* error = NULL;

	FreetuxTVApp *app = (FreetuxTVApp *) user_data;
	gtk_libvlc_media_player_toggle_fullscreen (app->player, &error);

	if(error != NULL){
		windowmain_show_gerror (app, error);
		g_error_free (error);
	}
	return TRUE;
}

static gboolean
on_accel_unfullscreen (GtkAccelGroup *accel_group, GObject *acceleratable, guint keyval,
                       GdkModifierType modifier, gpointer user_data)
{
	GError* error = NULL;

	FreetuxTVApp *app = (FreetuxTVApp *) user_data;
	gtk_libvlc_media_player_set_fullscreen (app->player, FALSE, &error);

	if(error != NULL){
		windowmain_show_gerror (app, error);
		g_error_free (error);
	}
	return TRUE;
}

static gboolean
on_accel_volumeup (GtkAccelGroup *accel_group, GObject *acceleratable, guint keyval,
                   GdkModifierType modifier, gpointer user_data)
{
	GError* error = NULL;
	GtkWidget* widget;

	FreetuxTVApp *app = (FreetuxTVApp *) user_data;
	widget = (GtkWidget *)gtk_builder_get_object (app->gui,
	                                              "windowmain_volumecontrol");

	app->config.volume = app->config.volume + 0.10;
	if(app->config.volume >= 1.0){
		app->config.volume = 1.0;
	}
	gtk_range_set_value (GTK_RANGE(widget), app->config.volume);
	gtk_libvlc_media_player_set_volume (app->player, app->config.volume, &error);


	if(error != NULL){
		windowmain_show_gerror (app, error);
		g_error_free (error);
	}
	return TRUE;
}

static gboolean
on_accel_volumedown (GtkAccelGroup *accel_group, GObject *acceleratable, guint keyval,
                     GdkModifierType modifier, gpointer user_data)
{
	GError* error = NULL;
	GtkWidget* widget;

	FreetuxTVApp *app = (FreetuxTVApp *) user_data;
	widget = (GtkWidget *)gtk_builder_get_object (app->gui,
	                                              "windowmain_volumecontrol");

	app->config.volume = app->config.volume - 0.10;
	if(app->config.volume <= 0.0){
		app->config.volume = 0.0;
	}
	gtk_range_set_value (GTK_RANGE(widget), app->config.volume);
	gtk_libvlc_media_player_set_volume (app->player, app->config.volume, &error);


	if(error != NULL){
		windowmain_show_gerror (app, error);
		g_error_free (error);
	}
	return TRUE;
}

*/