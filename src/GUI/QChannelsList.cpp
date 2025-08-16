//
// Created by ebeuque on 15/02/25.
//

#include "QChannelsList.h"

#include <QHeaderView>
#include <QStandardItem>

QChannelsList::QChannelsList(QWidget* parent)
    : QWidget(parent)
    , m_treeView(nullptr)
    , m_model(nullptr)
    , m_dbInstance(nullptr)
{
    setupUI();
    setupModel();
}

QChannelsList::~QChannelsList()
{
}

void QChannelsList::setupUI()
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    
    m_treeView = new QTreeView(this);
    m_treeView->setRootIsDecorated(true);
    m_treeView->setAlternatingRowColors(true);
    m_treeView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_treeView->setHeaderHidden(true);
    
    layout->addWidget(m_treeView);
    
    // Connect signals
    connect(m_treeView->selectionModel(), &QItemSelectionModel::currentChanged,
            this, &QChannelsList::onSelectionChanged);
    connect(m_treeView, &QTreeView::doubleClicked,
            this, &QChannelsList::onItemDoubleClicked);
    connect(m_treeView, &QTreeView::expanded,
            this, &QChannelsList::onItemExpanded);
}

void QChannelsList::setupModel()
{
    m_model = new QStandardItemModel(this);
    m_model->setHorizontalHeaderLabels(QStringList() << tr("Channels"));
    
    m_treeView->setModel(m_model);
}

void QChannelsList::setDatabaseInstance(DatabaseInstance* dbInstance)
{
    m_dbInstance = dbInstance;
}

void QChannelsList::loadChannelsGroups(QError& error)
{
    // TODO: Implement database loading
    // For now, clear the model
    m_model->clear();
    m_model->setHorizontalHeaderLabels(QStringList() << tr("Channels"));
    m_channelsGroups.clear();
    m_channelsByGroup.clear();
}

void QChannelsList::loadChannels(const QSharedPointer<ChannelsGroupInfos>& channelsGroup, QError& error)
{
    if (!channelsGroup) {
        return;
    }
    
    // TODO: Implement database loading of channels for the group
    // For now, just store the group
    if (!m_channelsByGroup.contains(channelsGroup->getId())) {
        m_channelsByGroup[channelsGroup->getId()] = QList<QSharedPointer<ChannelInfos>>();
    }
}

void QChannelsList::refreshChannelsList()
{
    QError error;
    loadChannelsGroups(error);
}

void QChannelsList::addChannelsGroup(const QSharedPointer<ChannelsGroupInfos>& channelsGroup)
{
    if (!channelsGroup) {
        return;
    }
    
    m_channelsGroups.append(channelsGroup);
    
    QStandardItem* groupItem = new QStandardItem(channelsGroup->getName());
    groupItem->setData(QVariant::fromValue(channelsGroup), Qt::UserRole);
    groupItem->setData("group", Qt::UserRole + 1); // Type identifier
    groupItem->setEditable(false);
    
    // Set icon or styling for group
    groupItem->setIcon(QIcon::fromTheme("folder"));
    
    m_model->appendRow(groupItem);
}

void QChannelsList::addChannel(const QSharedPointer<ChannelInfos>& channel, const QSharedPointer<ChannelsGroupInfos>& channelsGroup)
{
    if (!channel || !channelsGroup) {
        return;
    }
    
    // Store channel in the map
    m_channelsByGroup[channelsGroup->getId()].append(channel);
    
    // Find the group item
    QStandardItem* groupItem = findChannelsGroupItem(channelsGroup);
    if (!groupItem) {
        // Group doesn't exist, add it first
        addChannelsGroup(channelsGroup);
        groupItem = findChannelsGroupItem(channelsGroup);
    }
    
    if (groupItem) {
        QStandardItem* channelItem = new QStandardItem(channel->getName());
        channelItem->setData(QVariant::fromValue(channel), Qt::UserRole);
        channelItem->setData("channel", Qt::UserRole + 1); // Type identifier
        channelItem->setEditable(false);
        
        // Set icon for channel
        channelItem->setIcon(QIcon::fromTheme("video-television"));
        
        groupItem->appendRow(channelItem);
    }
}

QStandardItem* QChannelsList::findChannelsGroupItem(const QSharedPointer<ChannelsGroupInfos>& channelsGroup)
{
    if (!channelsGroup) {
        return nullptr;
    }
    
    for (int i = 0; i < m_model->rowCount(); ++i) {
        QStandardItem* item = m_model->item(i);
        if (item) {
            QVariant data = item->data(Qt::UserRole);
            QSharedPointer<ChannelsGroupInfos> storedGroup = data.value<QSharedPointer<ChannelsGroupInfos>>();
            if (storedGroup && storedGroup->getId() == channelsGroup->getId()) {
                return item;
            }
        }
    }
    
    return nullptr;
}

QSharedPointer<ChannelInfos> QChannelsList::getChannelFromIndex(const QModelIndex& index)
{
    if (!index.isValid()) {
        return QSharedPointer<ChannelInfos>();
    }
    
    QStandardItem* item = m_model->itemFromIndex(index);
    if (!item) {
        return QSharedPointer<ChannelInfos>();
    }
    
    QString type = item->data(Qt::UserRole + 1).toString();
    if (type == "channel") {
        QVariant data = item->data(Qt::UserRole);
        return data.value<QSharedPointer<ChannelInfos>>();
    }
    
    return QSharedPointer<ChannelInfos>();
}

QSharedPointer<ChannelsGroupInfos> QChannelsList::getChannelsGroupFromIndex(const QModelIndex& index)
{
    if (!index.isValid()) {
        return QSharedPointer<ChannelsGroupInfos>();
    }
    
    QStandardItem* item = m_model->itemFromIndex(index);
    if (!item) {
        return QSharedPointer<ChannelsGroupInfos>();
    }
    
    QString type = item->data(Qt::UserRole + 1).toString();
    if (type == "group") {
        QVariant data = item->data(Qt::UserRole);
        return data.value<QSharedPointer<ChannelsGroupInfos>>();
    }
    
    return QSharedPointer<ChannelsGroupInfos>();
}

void QChannelsList::onSelectionChanged()
{
    QModelIndex current = m_treeView->currentIndex();
    
    QSharedPointer<ChannelInfos> channel = getChannelFromIndex(current);
    if (channel) {
        emit channelSelected(channel);
        return;
    }
    
    QSharedPointer<ChannelsGroupInfos> channelsGroup = getChannelsGroupFromIndex(current);
    if (channelsGroup) {
        emit channelsGroupSelected(channelsGroup);
    }
}

void QChannelsList::onItemDoubleClicked(const QModelIndex& index)
{
    QSharedPointer<ChannelInfos> channel = getChannelFromIndex(index);
    if (channel) {
        emit channelDoubleClicked(channel);
    }
}

void QChannelsList::onItemExpanded(const QModelIndex& index)
{
    QSharedPointer<ChannelsGroupInfos> channelsGroup = getChannelsGroupFromIndex(index);
    if (channelsGroup) {
        // Load channels for this group if not already loaded
        if (m_channelsByGroup[channelsGroup->getId()].isEmpty()) {
            QError error;
            loadChannels(channelsGroup, error);
        }
    }
}