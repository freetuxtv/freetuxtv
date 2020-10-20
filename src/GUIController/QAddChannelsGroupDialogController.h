//
// Created by ebeuque on 10/10/2020.
//

#ifndef FREETUXTV_QADDCHANNELSGROUPDIALOGCONTROLLER_H
#define FREETUXTV_QADDCHANNELSGROUPDIALOGCONTROLLER_H

#include <QObject>
#include <QList>
#include <QStandardItemModel>

#include "Model/ChannelsGroupSection.h"

class Application;

class QError;

class QAddChannelsGroupDialog;

class QAddChannelsGroupDialogController : public QObject
{
	Q_OBJECT
public:
	void init(QAddChannelsGroupDialog* pPreferencesDialog, Application* pApplication);
	void dispose();

private slots:
	void onValidButtonClicked();
	void onChannelsGroupListRefreshButtonClicked();

signals:
	void channelsGroupAdded();
	void channelsAdded();

private:
	bool loadChannelsGroupListFromFile(const QString& szFilePath, QStandardItemModel* pModel, QError *pError);
	bool fillChannelsGroupModel(const QList<ChannelsGroupSection>& listChannelsGroupSection, QStandardItemModel* pModel, QError *pError);

private:
	QAddChannelsGroupDialog* m_pAddChannelsGroupDialog;
	Application* m_pApplication;

	QStandardItemModel* m_pChannelsGroupListModel;
};


#endif //FREETUXTV_QADDCHANNELSGROUPDIALOGCONTROLLER_H
