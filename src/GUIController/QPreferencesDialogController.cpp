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
	QString szTmp;

	// General
	m_pPreferences->m_bChannelOnStartup = m_pPreferencesDialog->getCheckBoxPlayLastChannelOnStartup()->isChecked();
	m_pPreferences->m_bEnableNotifications = m_pPreferencesDialog->getCheckBoxDesktopNotification()->isChecked();
	m_pPreferences->m_bIgnoreDiacritics = m_pPreferencesDialog->getCheckBoxIgnoreDiacritics()->isChecked();

	// LibVLC
	if(m_pPreferencesDialog->getButtonConfigFileNone()->isChecked()){
		m_pPreferences->m_iLibVLCConfigFileMode = 0;
	}
	if(m_pPreferencesDialog->getButtonConfigFileCustom()->isChecked()){
		m_pPreferences->m_iLibVLCConfigFileMode = 1;
	}
	if(m_pPreferencesDialog->getButtonConfigFileVLCFile()->isChecked()){
		m_pPreferences->m_iLibVLCConfigFileMode = 2;
	}

	// Recordings
	m_pPreferences->m_szDirectoryRecordings = m_pPreferencesDialog->getLineEditDirectoryRecording()->text();
	if(m_pPreferencesDialog->getButtonTranscodeNone()->isChecked()){
		m_pPreferences->m_iTranscodingMode = 0;
	}
	if(m_pPreferencesDialog->getButtonTranscodeCustom()->isChecked()){
		m_pPreferences->m_iTranscodingMode = 1;
	}
	m_pPreferences->m_szTranscodingFormat = m_pPreferencesDialog->getComboBoxTranscodeCustom()->currentText();

	// Network
	szTmp = m_pPreferencesDialog->getLineEditDownloadTimeOut()->text();
	m_pPreferences->m_iTimeout = szTmp.toInt();
	if(m_pPreferencesDialog->getButtonProxyNone()->isChecked()){
		m_pPreferences->m_iProxyMode = 0;
	}
	if(m_pPreferencesDialog->getButtonProxyCustom()->isChecked()){
		m_pPreferences->m_iProxyMode = 1;
	}
	m_pPreferences->m_szProxyServer = m_pPreferencesDialog->getLineEditProxyServer()->text();
	szTmp = m_pPreferencesDialog->getLineEditProxyPort()->text();
	if(szTmp.isNull()) {
		m_pPreferences->m_iProxyPort = -1;
	}else{
		m_pPreferences->m_iProxyPort = szTmp.toInt();
	}
	m_pPreferences->m_szProxyType = m_pPreferencesDialog->getComboBoxProxyType()->currentText();
	m_pPreferences->m_bProxyAuth = m_pPreferencesDialog->getCheckBoxProxyAuth()->isChecked();
	m_pPreferences->m_szProxyUser = m_pPreferencesDialog->getLineEditProxyUsername()->text();
	m_pPreferences->m_szProxyPassword = m_pPreferencesDialog->getLineEditProxyPassword()->text();

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