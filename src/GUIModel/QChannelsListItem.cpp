//
// Created by ebeuque on 07/06/2021.
//

#include "QChannelsListItem.h"

QChannelsListItem::QChannelsListItem(const QSharedPointer<ChannelsGroupInfos>& pChannelGroupsInfos)
{
	m_pChannelGroupsInfos = pChannelGroupsInfos;
	setText(pChannelGroupsInfos->getName());
}

QChannelsListItem::QChannelsListItem(const QSharedPointer<ChannelInfos>& pChannelInfos)
{
	m_pChannelInfos = pChannelInfos;
	setText(pChannelInfos->getName());
}

QChannelsListItem::~QChannelsListItem()
{

}