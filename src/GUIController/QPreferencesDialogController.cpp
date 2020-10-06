//
// Created by ebeuque on 06/10/2020.
//

#include <QCheckBox>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QRadioButton>
#include <QLabel>
#include <QButtonGroup>

#include "Model/Preferences.h"

#include "GUI/QPreferencesDialog.h"

#include "QPreferencesDialogController.h"

QPreferencesDialogController::QPreferencesDialogController()
{
	m_pPreferencesDialog = NULL;
}

QPreferencesDialogController::~QPreferencesDialogController()
{

}

void QPreferencesDialogController::init(QPreferencesDialog* pPreferencesDialog, Preferences* pPreferences)
{
	m_pPreferencesDialog = pPreferencesDialog;

	loadPreferences(pPreferences);
	updateCtrl();

	connect(m_pPreferencesDialog->getButtonCancel(), SIGNAL(clicked()), m_pPreferencesDialog, SLOT(reject()));
	connect(m_pPreferencesDialog->getButtonValid(), SIGNAL(clicked()), this, SLOT(onValidButtonClicked()));

	connect(m_pPreferencesDialog->getLineEditDirectoryRecording(), SIGNAL(textChanged(const QString&)), this, SLOT(onDirectoryRecordingsChanged()));
	connect(m_pPreferencesDialog->getComboBoxTranscodeCustom(), SIGNAL(activated(int)), this, SLOT(onTranscodingFormatsChanged()));
	connect(m_pPreferencesDialog->getButtonGroupTranscode(), SIGNAL(buttonClicked(QAbstractButton *)), this, SLOT(onTranscodingModeToggled()));
	connect(m_pPreferencesDialog->getButtonGroupProxyMode(), SIGNAL(buttonClicked(QAbstractButton *)), this, SLOT(onProxyModeToggled()));
	connect(m_pPreferencesDialog->getCheckBoxProxyAuth(), SIGNAL(toggled(bool)), this, SLOT(onProxyAuthToggled()));
}

void QPreferencesDialogController::dispose()
{

}

void QPreferencesDialogController::loadPreferences(Preferences* pPreferences)
{
	QString szTmp;

	// General
	m_pPreferencesDialog->getCheckBoxPlayLastChannelOnStartup()->setChecked(pPreferences->m_bChannelOnStartup);
	m_pPreferencesDialog->getCheckBoxDesktopNotification()->setChecked(pPreferences->m_bEnableNotifications);
	m_pPreferencesDialog->getCheckBoxIgnoreDiacritics()->setChecked(pPreferences->m_bIgnoreDiacritics);

	// LibVLC
	switch(pPreferences->m_iLibVLCConfigFileMode)
	{
		case 0:
			m_pPreferencesDialog->getButtonConfigFileNone()->setChecked(true);
			break;
		case 1:
			m_pPreferencesDialog->getButtonConfigFileCustom()->setChecked(true);
			break;
		case 2:
			m_pPreferencesDialog->getButtonConfigFileVLCFile()->setChecked(true);
		default:
			break;
	}

	// Recordings
	m_pPreferencesDialog->getLineEditDirectoryRecording()->setText(pPreferences->m_szDirectoryRecordings);
	switch(pPreferences->m_iTranscodingMode)
	{
		case 0:
			m_pPreferencesDialog->getButtonTranscodeNone()->setChecked(true);
			break;
		case 1:
			m_pPreferencesDialog->getButtonTranscodeCustom()->setChecked(true);
			break;
		default:
			break;
	}
	m_pPreferencesDialog->getComboBoxTranscodeCustom()->setCurrentText(pPreferences->m_szTranscodingFormat);

	// Network
	m_pPreferencesDialog->getLineEditDownloadTimeOut()->setText(QString::number(pPreferences->m_iTimeout));
	switch(pPreferences->m_iProxyMode)
	{
		case 0:
			m_pPreferencesDialog->getButtonProxyNone()->setChecked(true);
			break;
		case 1:
			m_pPreferencesDialog->getButtonProxyCustom()->setChecked(true);
			break;
		default:
			break;
	}
	m_pPreferencesDialog->getLineEditProxyServer()->setText(pPreferences->m_szProxyServer);
	if(pPreferences->m_iProxyPort != -1){
		szTmp = QString::number(pPreferences->m_iProxyPort);
	}
	m_pPreferencesDialog->getLineEditProxyPort()->setText(szTmp);
	m_pPreferencesDialog->getComboBoxProxyType()->setCurrentText(pPreferences->m_szProxyType);
	m_pPreferencesDialog->getCheckBoxProxyAuth()->setChecked(pPreferences->m_bProxyAuth);
	m_pPreferencesDialog->getLineEditProxyUsername()->setText(pPreferences->m_szProxyUser);
	m_pPreferencesDialog->getLineEditProxyPassword()->setText(pPreferences->m_szProxyPassword);
}

void QPreferencesDialogController::updateCtrl()
{
	bool bIsManualProxy = false;
	bool bUseAuth = false;

	// Refresh recording string
	QString szText;
	//szText = get_recording_options(app, "%filename%", TRUE, NULL);
	m_pPreferencesDialog->getLabelTranscodeCmd()->setText(szText);

	// Refresh the network view
	if(m_pPreferencesDialog->getButtonProxyCustom()->isChecked()){
		bIsManualProxy = true;
	}else{
		bIsManualProxy = false;
	}
	if(m_pPreferencesDialog->getCheckBoxProxyAuth()->isChecked()){
		bUseAuth = true;
	}else{
		bUseAuth = false;
	}

	m_pPreferencesDialog->getLineEditProxyServer()->setEnabled(bIsManualProxy);
	m_pPreferencesDialog->getLineEditProxyPort()->setEnabled(bIsManualProxy);
	m_pPreferencesDialog->getComboBoxProxyType()->setEnabled(bIsManualProxy);
	m_pPreferencesDialog->getCheckBoxProxyAuth()->setEnabled(bIsManualProxy);
	m_pPreferencesDialog->getLineEditProxyUsername()->setEnabled(bIsManualProxy && bUseAuth);
	m_pPreferencesDialog->getLineEditProxyPassword()->setEnabled(bIsManualProxy && bUseAuth);
}


void QPreferencesDialogController::onValidButtonClicked()
{
	qDebug("onValidButtonClicked");

/*
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
	*/

	m_pPreferencesDialog->accept();
}

void QPreferencesDialogController::onDirectoryRecordingsChanged ()
{
	updateCtrl();
}

void QPreferencesDialogController::onTranscodingModeToggled ()
{
	updateCtrl();
}

void QPreferencesDialogController::onTranscodingFormatsChanged ()
{
	updateCtrl();
}

void QPreferencesDialogController::onProxyModeToggled ()
{
	updateCtrl();
}

void QPreferencesDialogController::onProxyAuthToggled ()
{
	updateCtrl();
}