//
// Created by ebeuque on 10/10/2020.
//

#include <QDir>
#include <QPushButton>
#include <QTreeView>
#include <QLineEdit>
#include <QMessageBox>
#include <QXmlInputSource>

#include "Global/QError.h"
#include "Global/FileUtils.h"
#include "Global/UserPaths.h"
#include "Global/Application.h"

#include "Model/Preferences.h"
#include "XML/ChannelsGroupsParserHandler.h"

#include "GUI/QAddChannelsGroupDialog.h"

#include "QAddChannelsGroupDialogController.h"

void QAddChannelsGroupDialogController::init(QAddChannelsGroupDialog* pAddChannelsGroupDialog, Application* pApplication)
{
	m_pAddChannelsGroupDialog = pAddChannelsGroupDialog;
	m_pApplication = pApplication;

	m_pChannelsGroupListModel = new QStandardItemModel();

	connect(m_pAddChannelsGroupDialog->getButtonCancel(), SIGNAL(clicked()), m_pAddChannelsGroupDialog, SLOT(reject()));
	connect(m_pAddChannelsGroupDialog->getButtonValid(), SIGNAL(clicked()), this, SLOT(onValidButtonClicked()));

	connect(m_pAddChannelsGroupDialog->getButtonChannelsGroupListRefresh(), SIGNAL(clicked()), this, SLOT(onChannelsGroupListRefreshButtonClicked()));

	m_pAddChannelsGroupDialog->getTreeViewChannelsGroupList()->setModel(m_pChannelsGroupListModel);

	// Load channels groups list
	QError error;
	QDir dirPath = UserPaths::getUserCacheDir();
	QString szDstFilePath = dirPath.filePath("channels_groups.dat");
	loadChannelsGroupListFromFile(szDstFilePath, m_pChannelsGroupListModel, &error);
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
	bGoOn = FileUtils::getFile(szURI, szDstFilePath, &m_pApplication->getPreferences()->m_proxy, m_pApplication->getPreferences()->m_iTimeout, &error);

	if(bGoOn){
		qInfo("Updating the list of channel's groups\n");
		bGoOn = loadChannelsGroupListFromFile(szDstFilePath, m_pChannelsGroupListModel, &error);
	}

	if(!bGoOn){
		QMessageBox::critical(m_pAddChannelsGroupDialog, tr("Error"), error.getErrorMsg());
	}
};


bool QAddChannelsGroupDialogController::loadChannelsGroupListFromFile(const QString& szFilePath, QStandardItemModel* pModel, QError *pError)
{
	bool bRes = true;

	QString szTmpFilePath = szFilePath;

	QFile file(szTmpFilePath);
	if(!file.exists()){
		// If not local file, we use the file from
		szTmpFilePath = QDir(m_pApplication->getDataDir()).filePath("channels_groups.xml");
		file.setFileName(szTmpFilePath);
	}

	QList<ChannelsGroupSection> listChannelsGroupSection;

	// Parse the file
	bRes = file.open(QIODevice::ReadOnly);
	if(bRes){
		QXmlInputSource source(&file);

		QXmlSimpleReader reader;
		ChannelsGroupsParserHandler handler(listChannelsGroupSection);
		reader.setContentHandler(&handler);
		reader.setErrorHandler(&handler);

		bRes = reader.parse(source);
		if(!bRes){
			qCritical("[XML] Error to parse the content of file: %s", qPrintable(szTmpFilePath));
		}

		file.close();
	}else{
		qCritical("[XML] Error to open the file: %s", qPrintable(szTmpFilePath));
	}

	// Fill the treeview
	if(bRes){
		// Clear the model
		pModel->clear();
		bRes = fillChannelsGroupModel(listChannelsGroupSection, pModel, pError);
	}

	return bRes;
}

bool QAddChannelsGroupDialogController::fillChannelsGroupModel(const QList<ChannelsGroupSection>& listChannelsGroupSection, QStandardItemModel* pModel, QError *pError)
{
	bool bRes = true;

	QList<ChannelsGroupSection>::const_iterator iter_section;
	QList<ChannelsGroupInfos>::const_iterator iter_group;

	QStandardItem* pSectionItem;
	QStandardItem* pChannelsGroupItem;

	for(iter_section = listChannelsGroupSection.constBegin(); iter_section != listChannelsGroupSection.constEnd(); ++iter_section)
	{
		const ChannelsGroupSection& channelsGroupSection = (*iter_section);
		pSectionItem = new QStandardItem();
		pSectionItem->setText(channelsGroupSection.getLang());
		pModel->appendRow(pSectionItem);

		const QList<ChannelsGroupInfos>& listChannelsGroupInfos = channelsGroupSection.getChannelsGroupList();

		for(iter_group = listChannelsGroupInfos.constBegin(); iter_group != listChannelsGroupInfos.constEnd(); ++iter_group)
		{
			const ChannelsGroupInfos& channelsGroupInfos = (*iter_group);
			pChannelsGroupItem = new QStandardItem();
			pChannelsGroupItem->setText(channelsGroupInfos.getName());
			pSectionItem->appendRow(pChannelsGroupItem);
		}


	}

	return bRes;
}