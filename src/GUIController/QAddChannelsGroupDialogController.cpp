//
// Created by ebeuque on 10/10/2020.
//

#include <QDir>
#include <QPushButton>
#include <QTreeView>
#include <QLineEdit>
#include <QMessageBox>

#include "Global/QError.h"
#include "Global/FileUtils.h"
#include "Global/UserPaths.h"

#include "GUI/QAddChannelsGroupDialog.h"

#include "QAddChannelsGroupDialogController.h"

void QAddChannelsGroupDialogController::init(QAddChannelsGroupDialog* pAddChannelsGroupDialog, Preferences* pPreferences)
{
	m_pAddChannelsGroupDialog = pAddChannelsGroupDialog;
	m_pPreferences = pPreferences;

	m_pChannelsGroupListModel = new QStandardItemModel();

	connect(m_pAddChannelsGroupDialog->getButtonCancel(), SIGNAL(clicked()), m_pAddChannelsGroupDialog, SLOT(reject()));
	connect(m_pAddChannelsGroupDialog->getButtonValid(), SIGNAL(clicked()), this, SLOT(onValidButtonClicked()));

	connect(m_pAddChannelsGroupDialog->getButtonChannelsGroupListRefresh(), SIGNAL(clicked()), this, SLOT(onChannelsGroupListRefreshButtonClicked()));

	m_pAddChannelsGroupDialog->getTreeViewChannelsGroupList()->setModel(m_pChannelsGroupListModel);

	// Load channels groups list
	QError error;
	loadChannelsGroupListFromFile(m_pChannelsGroupListModel, &error);
}

void QAddChannelsGroupDialogController::dispose()
{

}

void QAddChannelsGroupDialogController::onValidButtonClicked()
{
	qDebug("onValidButtonClicked");
};

void QAddChannelsGroupDialogController::onChannelsGroupListRefreshButtonClicked()
{
	bool bGoOn = true;

	QError error;

	qInfo("Starting update of the channel's groups list");

	QString szURI = m_pAddChannelsGroupDialog->getLineEditChannelsGroupFile()->text();

	QDir dirPath = UserPaths::getUserCacheDir();
	QString szDstFilePath = dirPath.filePath("channels_groups.dat");

	// Download the file
	qInfo("Downloading the file '%s'", qPrintable(szURI));
	bGoOn = FileUtils::getFile(szURI, szDstFilePath, &m_pPreferences->m_proxy, m_pPreferences->m_iTimeout, &error);

	if(bGoOn){
		qInfo("Updating the list of channel's groups\n");
		bGoOn = loadChannelsGroupListFromFile(m_pChannelsGroupListModel, &error);
	}

	if(!bGoOn){
		QMessageBox::critical(m_pAddChannelsGroupDialog, tr("Error"), error.getErrorMsg());
	}
};


bool QAddChannelsGroupDialogController::loadChannelsGroupListFromFile(QStandardItemModel* pModel, QError *pError)
{
	bool bGoOn = true;



	return bGoOn;
}