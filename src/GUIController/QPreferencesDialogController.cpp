//
// Created by ebeuque on 06/10/2020.
//

#include <QCheckBox>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QRadioButton>
#include <QLabel>

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
}

void QPreferencesDialogController::dispose()
{

}

void QPreferencesDialogController::loadPreferences(Preferences* pPreferences)
{
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
	m_pPreferencesDialog->getLineEditProxyPort()->setText(QString::number(pPreferences->m_iProxyPort));
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

	m_pPreferencesDialog->accept();
}