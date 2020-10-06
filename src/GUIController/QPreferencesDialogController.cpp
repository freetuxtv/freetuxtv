//
// Created by ebeuque on 06/10/2020.
//

#include <QPushButton>

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

	connect(m_pPreferencesDialog->getButtonCancel(), SIGNAL(clicked()), m_pPreferencesDialog, SLOT(reject()));
	connect(m_pPreferencesDialog->getButtonValid(), SIGNAL(clicked()), this, SLOT(onValidButtonClicked()));
}

void QPreferencesDialogController::dispose()
{

}

void QPreferencesDialogController::loadPreferences(Preferences* pPreferences)
{

	//m_pPreferencesDialog->get

	/*
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


void QPreferencesDialogController::onValidButtonClicked()
{
	qDebug("onValidButtonClicked");

	m_pPreferencesDialog->accept();
}