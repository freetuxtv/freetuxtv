//
// Created by ebeuque on 06/10/2020.
//

#include <QVBoxLayout>
#include <QFormLayout>
#include <QTabWidget>
#include <QPushButton>
#include <QGroupBox>
#include <QCheckBox>
#include <QButtonGroup>
#include <QRadioButton>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QFileDialog>

#include "QPreferencesDialog.h"

QPreferencesDialog::QPreferencesDialog()
{
	QBoxLayout* pMainLayout = new QVBoxLayout();
	setLayout(pMainLayout);

	QBoxLayout* pTmpLayout;
	QWidget* pTmpWidget;

	{
		QTabWidget* pTabWidget = new QTabWidget();
		pMainLayout->addWidget(pTabWidget);

		pTmpWidget = createTabGeneral(pTabWidget);
		pTabWidget->addTab(pTmpWidget, tr("General"));

		pTmpWidget = createTabLibVLC(pTabWidget);
		pTabWidget->addTab(pTmpWidget, tr("LibVLC"));

		pTmpWidget = createTabRecordings(pTabWidget);
		pTabWidget->addTab(pTmpWidget, tr("Recordings"));

		pTmpWidget = createTabNetwork(pTabWidget);
		pTabWidget->addTab(pTmpWidget, tr("Network"));
	}

	{
		pTmpLayout = new QHBoxLayout();
		pMainLayout->addLayout(pTmpLayout);

		pTmpLayout->addStretch();

		m_pButtonCancel = new QPushButton(tr("Cancel"));
		pTmpLayout->addWidget(m_pButtonCancel);

		m_pButtonValid = new QPushButton(tr("Apply"));
		pTmpLayout->addWidget(m_pButtonValid);
	}
}

QPreferencesDialog::~QPreferencesDialog()
{

}


QWidget* QPreferencesDialog::createTabGeneral(QWidget* pParent)
{
	QWidget* pMainWidget = new QWidget(pParent);

	QBoxLayout* pMainLayout = new QVBoxLayout();
	pMainWidget->setLayout(pMainLayout);

	QGroupBox* pGroupBox;
	QBoxLayout* pTmpLayout;

	// Startup
	{
		pGroupBox = new QGroupBox(tr("Startup"));
		pMainLayout->addWidget(pGroupBox);

		pTmpLayout = new QVBoxLayout();
		pGroupBox->setLayout(pTmpLayout);
		m_pCheckBoxPlayLastChannelOnStartup = new QCheckBox(tr("Play last channel on startup"));
		pTmpLayout->addWidget(m_pCheckBoxPlayLastChannelOnStartup);
	}

	// Notifications
	{
		pGroupBox = new QGroupBox(tr("Notifications"));
		pMainLayout->addWidget(pGroupBox);

		pTmpLayout = new QVBoxLayout();
		pGroupBox->setLayout(pTmpLayout);
		m_pCheckBoxDesktopNotification = new QCheckBox(tr("Enable desktop notifications"));
		pTmpLayout->addWidget(m_pCheckBoxDesktopNotification);
	}

	// Search of channels
	{
		pGroupBox = new QGroupBox(tr("Search of channels"));
		pMainLayout->addWidget(pGroupBox);

		pTmpLayout = new QVBoxLayout();
		pGroupBox->setLayout(pTmpLayout);
		m_pCheckBoxIgnoreDiacritics = new QCheckBox(tr("Ignore diacritics"));
		pTmpLayout->addWidget(m_pCheckBoxIgnoreDiacritics);
	}

	pMainLayout->addStretch();

	return pMainWidget;
}

QWidget* QPreferencesDialog::createTabLibVLC(QWidget* pParent)
{
	QWidget* pMainWidget = new QWidget(pParent);

	QBoxLayout* pMainLayout = new QVBoxLayout();
	pMainWidget->setLayout(pMainLayout);

	QGroupBox* pGroupBox;
	QBoxLayout* pTmpLayout;
	QRadioButton* pTmpRadio;

	// Configuration file
	{
		pGroupBox = new QGroupBox(tr("Configuration file (vlcrc)"));
		pMainLayout->addWidget(pGroupBox);

		m_pButtonGroupConfigFile = new QButtonGroup(pGroupBox);

		pTmpLayout = new QVBoxLayout();
		pGroupBox->setLayout(pTmpLayout);

		m_pButtonConfigFileNone = new QRadioButton(tr("None"));
		m_pButtonGroupConfigFile->addButton(m_pButtonConfigFileNone);
		pTmpLayout->addWidget(m_pButtonConfigFileNone);

		m_pButtonConfigFileCustom = new QRadioButton(tr("Use a custom button"));
		m_pButtonGroupConfigFile->addButton(m_pButtonConfigFileCustom);
		pTmpLayout->addWidget(m_pButtonConfigFileCustom);

		m_pButtonConfigFileVLCFile = new QRadioButton(tr("Use the VLC configuration file"));
		m_pButtonGroupConfigFile->addButton(m_pButtonConfigFileVLCFile);
		pTmpLayout->addWidget(m_pButtonConfigFileVLCFile);

		pTmpLayout->addWidget(new QLabel(tr("Restart the application after changing this option.")));
	}

	pMainLayout->addStretch();

	return pMainWidget;
}

QWidget* QPreferencesDialog::createTabRecordings(QWidget* pParent)
{
	QWidget* pMainWidget = new QWidget(pParent);

	QBoxLayout* pMainLayout = new QVBoxLayout();
	pMainWidget->setLayout(pMainLayout);

	QGroupBox* pGroupBox;
	QBoxLayout* pTmpLayout;
	QFormLayout* pTmpFormLayout;
	QRadioButton* pTmpRadio;

	// Destination
	{
		pGroupBox = new QGroupBox(tr("Destination"));
		pMainLayout->addWidget(pGroupBox);

		pTmpFormLayout = new QFormLayout();
		pGroupBox->setLayout(pTmpFormLayout);

		m_pLineEditDirectoryRecording = new QLineEdit();
		pTmpFormLayout->addRow(tr("Directory records:"), m_pLineEditDirectoryRecording);
	}

	// Transcoding options
	{
		pGroupBox = new QGroupBox(tr("Transcoding options"));
		pMainLayout->addWidget(pGroupBox);

		m_pButtonGroupTranscode = new QButtonGroup(pGroupBox);

		pTmpLayout = new QVBoxLayout();
		pGroupBox->setLayout(pTmpLayout);

		m_pButtonTranscodeNone = new QRadioButton(tr("No transcoding"));
		m_pButtonGroupTranscode->addButton(m_pButtonTranscodeNone);
		pTmpLayout->addWidget(m_pButtonTranscodeNone);

		m_pButtonTranscodeCustom = new QRadioButton(tr("Predefined transcoding format"));
		m_pButtonGroupTranscode->addButton(m_pButtonTranscodeCustom);
		pTmpLayout->addWidget(m_pButtonTranscodeCustom);

		{
			pTmpFormLayout = new QFormLayout();
			pTmpLayout->addLayout(pTmpFormLayout);

			m_pComboBoxTranscodeCustom = new QComboBox();
			m_pComboBoxTranscodeCustom->addItem("Video - Theora + Vorbis (OGG)");
			m_pComboBoxTranscodeCustom->addItem("Video - H.264 + MPGA (MP4)");
			m_pComboBoxTranscodeCustom->addItem("Video - MPEG-2 + MPGA (TS)");
			pTmpFormLayout->addRow(tr("Format:"), m_pComboBoxTranscodeCustom);
		}

		m_pLabelTranscodeCmd = new QLabel("sout:");
		pTmpLayout->addWidget(m_pLabelTranscodeCmd);
	}

	pMainLayout->addStretch();

	return pMainWidget;
}

QWidget* QPreferencesDialog::createTabNetwork(QWidget* pParent)
{
	QWidget* pMainWidget = new QWidget(pParent);

	QBoxLayout* pMainLayout = new QVBoxLayout();
	pMainWidget->setLayout(pMainLayout);

	QGroupBox* pGroupBox;
	QBoxLayout* pTmpLayout;
	QFormLayout* pTmpFormLayout;
	QRadioButton* pTmpRadio;

	// Connection
	{
		pGroupBox = new QGroupBox(tr("Connection"));
		pMainLayout->addWidget(pGroupBox);

		pTmpFormLayout = new QFormLayout();
		pGroupBox->setLayout(pTmpFormLayout);

		m_pLineEditDownloadTimeOut = new QLineEdit();
		pTmpFormLayout->addRow(tr("Download timeout (seconds):"), m_pLineEditDownloadTimeOut);
	}

	// Proxy
	{
		pGroupBox = new QGroupBox(tr("Proxy"));
		pMainLayout->addWidget(pGroupBox);

		m_pButtonProxyMode = new QButtonGroup(pGroupBox);

		pTmpLayout = new QVBoxLayout();
		pGroupBox->setLayout(pTmpLayout);

		m_pButtonProxyNone = new QRadioButton(tr("No proxy"));
		m_pButtonProxyMode->addButton(m_pButtonProxyNone);
		pTmpLayout->addWidget(m_pButtonProxyNone);

		m_pButtonProxyCustom = new QRadioButton(tr("Manual proxy configuration"));
		m_pButtonProxyMode->addButton(m_pButtonProxyCustom);
		pTmpLayout->addWidget(m_pButtonProxyCustom);

		{
			pTmpFormLayout = new QFormLayout();
			pTmpLayout->addLayout(pTmpFormLayout);

			m_pLineEditProxyServer = new QLineEdit();
			pTmpFormLayout->addRow(tr("Proxy server:"), m_pLineEditProxyServer);

			m_pLineEditProxyPort = new QLineEdit();
			pTmpFormLayout->addRow(tr("Proxy port:"), m_pLineEditProxyPort);

			m_pComboBoxProxyType = new QComboBox();
			m_pComboBoxProxyType->addItem("HTTP");
			m_pComboBoxProxyType->addItem("SOCKS");
			pTmpFormLayout->addRow(tr("Proxy type:"), m_pComboBoxProxyType);


			m_pCheckBoxProxyAuth = new QCheckBox(tr("Use authentication"));
			pTmpFormLayout->addRow(QString(), m_pCheckBoxProxyAuth);

			m_pLineEditProxyUsername = new QLineEdit();
			pTmpFormLayout->addRow(tr("Username:"), m_pLineEditProxyUsername);

			m_pLineEditProxyPassword = new QLineEdit();
			pTmpFormLayout->addRow(tr("Password:"), m_pLineEditProxyPassword);
		}
	}

	pMainLayout->addStretch();

	return pMainWidget;
}