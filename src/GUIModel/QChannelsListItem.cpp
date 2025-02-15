//
// Created by ebeuque on 07/06/2021.
//

#include <QPixmap>

#include "QChannelsListItem.h"

QChannelsListItem::QChannelsListItem(const QSharedPointer<ChannelsGroupInfos>& pChannelGroupsInfos)
{
	m_pChannelGroupsInfos = pChannelGroupsInfos;
	setText(pChannelGroupsInfos->getName());
	m_bIsPlaying = false;
}

QChannelsListItem::QChannelsListItem(const QSharedPointer<ChannelInfos>& pChannelInfos)
{
	m_pChannelInfos = pChannelInfos;
	setText(pChannelInfos->getName());
	m_bIsPlaying = false;
}

QChannelsListItem::~QChannelsListItem()
{

}

bool QChannelsListItem::isPlaying() const
{
	return m_bIsPlaying;
}

void QChannelsListItem::setIsPlaying(bool bIsPlaying)
{
	m_bIsPlaying = bIsPlaying;
}