//
// Created by ebeuque on 10/10/2020.
//

#ifndef FREETUXTV_QADDCHANNELSGROUPDIALOGCONTROLLER_H
#define FREETUXTV_QADDCHANNELSGROUPDIALOGCONTROLLER_H

#include <QObject>
#include <QList>
#include <QStandardItemModel>

#include "GUIModel/QChannelsGroupListModel.h"

#include "Model/ChannelsGroupSection.h"

class Application;

class QError;

class QAddChannelsGroupDialog;
class QChannelsGroupListModel;

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
	bool loadChannelsGroupListFromFile(const QString& szFilePath, QChannelsGroupListModel* pModel, QError *pError);
	bool fillChannelsGroupModel(const QList<ChannelsGroupSection>& listChannelsGroupSection, QChannelsGroupListModel* pModel, QError *pError);

private:
	QAddChannelsGroupDialog* m_pAddChannelsGroupDialog;
	Application* m_pApplication;

	QChannelsGroupListModel* m_pChannelsGroupListModel;
};


#endif //FREETUXTV_QADDCHANNELSGROUPDIALOGCONTROLLER_H
