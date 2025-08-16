//
// Created by ebeuque on 15/02/25.
//

#ifndef FREETUXTV_GUI_QCHANNELSLIST_H
#define FREETUXTV_GUI_QCHANNELSLIST_H

#include <QWidget>
#include <QTreeView>
#include <QStandardItemModel>
#include <QVBoxLayout>
#include <QSharedPointer>

#include "Model/ChannelInfos.h"
#include "Model/ChannelsGroupInfos.h"
#include "Database/DatabaseInstance.h"
#include "Global/QError.h"

Q_DECLARE_METATYPE(QSharedPointer<ChannelInfos>)
Q_DECLARE_METATYPE(QSharedPointer<ChannelsGroupInfos>)

class QChannelsList : public QWidget
{
    Q_OBJECT

public:
    explicit QChannelsList(QWidget* parent = nullptr);
    virtual ~QChannelsList();

    void setDatabaseInstance(DatabaseInstance* dbInstance);
    void loadChannelsGroups(QError& error);
    void loadChannels(const QSharedPointer<ChannelsGroupInfos>& channelsGroup, QError& error);
    void refreshChannelsList();
    void addChannelsGroup(const QSharedPointer<ChannelsGroupInfos>& channelsGroup);
    void addChannel(const QSharedPointer<ChannelInfos>& channel, const QSharedPointer<ChannelsGroupInfos>& channelsGroup);

signals:
    void channelSelected(const QSharedPointer<ChannelInfos>& channel);
    void channelDoubleClicked(const QSharedPointer<ChannelInfos>& channel);
    void channelsGroupSelected(const QSharedPointer<ChannelsGroupInfos>& channelsGroup);

private slots:
    void onSelectionChanged();
    void onItemDoubleClicked(const QModelIndex& index);
    void onItemExpanded(const QModelIndex& index);

private:
    void setupUI();
    void setupModel();
    QStandardItem* findChannelsGroupItem(const QSharedPointer<ChannelsGroupInfos>& channelsGroup);
    QSharedPointer<ChannelInfos> getChannelFromIndex(const QModelIndex& index);
    QSharedPointer<ChannelsGroupInfos> getChannelsGroupFromIndex(const QModelIndex& index);

private:
    QTreeView* m_treeView;
    QStandardItemModel* m_model;
    
    DatabaseInstance* m_dbInstance;
    QList<QSharedPointer<ChannelsGroupInfos>> m_channelsGroups;
    QMap<int, QList<QSharedPointer<ChannelInfos>>> m_channelsByGroup;
};

#endif //FREETUXTV_GUI_QCHANNELSLIST_H