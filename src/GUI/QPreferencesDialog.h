//
// Created by ebeuque on 06/10/2020.
//

#ifndef FREETUXTV_QPREFERENCESDIALOG_H
#define FREETUXTV_QPREFERENCESDIALOG_H

#include <QDialog>

class QCheckBox;
class QButtonGroup;
class QRadioButton;
class QLineEdit;
class QLabel;
class QComboBox;

class QPreferencesDialog : public QDialog
{
public:
	QPreferencesDialog();
	virtual ~QPreferencesDialog();

public:
	QPushButton* getButtonCancel() const;
	QPushButton* getButtonValid() const;

private:
	QWidget* createTabGeneral(QWidget* pParent);
	QWidget* createTabLibVLC(QWidget* pParent);
	QWidget* createTabRecordings(QWidget* pParent);
	QWidget* createTabNetwork(QWidget* pParent);

private:
	QPushButton* m_pButtonCancel;
	QPushButton* m_pButtonValid;

	QCheckBox* m_pCheckBoxPlayLastChannelOnStartup;
	QCheckBox* m_pCheckBoxDesktopNotification;
	QCheckBox* m_pCheckBoxIgnoreDiacritics;

	QButtonGroup* m_pButtonGroupConfigFile;
	QRadioButton* m_pButtonConfigFileNone;
	QRadioButton* m_pButtonConfigFileCustom;
	QRadioButton* m_pButtonConfigFileVLCFile;

	QLineEdit* m_pLineEditDirectoryRecording;
	QButtonGroup* m_pButtonGroupTranscode;
	QRadioButton* m_pButtonTranscodeNone;
	QRadioButton* m_pButtonTranscodeCustom;
	QComboBox* m_pComboBoxTranscodeCustom;
	QLabel* m_pLabelTranscodeCmd;

	QLineEdit* m_pLineEditDownloadTimeOut;
	QButtonGroup* m_pButtonProxyMode;
	QRadioButton* m_pButtonProxyNone;
	QRadioButton* m_pButtonProxyCustom;
	QLineEdit* m_pLineEditProxyServer;
	QLineEdit* m_pLineEditProxyPort;
	QComboBox* m_pComboBoxProxyType;
	QCheckBox* m_pCheckBoxProxyAuth;
	QLineEdit* m_pLineEditProxyUsername;
	QLineEdit* m_pLineEditProxyPassword;
};


#endif //FREETUXTV_QPREFERENCESDIALOG_H
