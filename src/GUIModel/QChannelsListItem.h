//
// Created by ebeuque on 07/06/2021.
//

#ifndef FREETUXTV_QCHANNELSLISTITEM_H
#define FREETUXTV_QCHANNELSLISTITEM_H

#include <QStandardItem>

#include "Model/ChannelInfos.h"
#include "Model/ChannelsGroupInfos.h"

class QChannelsListItem : public QStandardItem
{
public:
	QChannelsListItem(const QSharedPointer<ChannelsGroupInfos>& pChannelGroupsInfos);
	QChannelsListItem(const QSharedPointer<ChannelInfos>& pChannelInfos);
	virtual ~QChannelsListItem();

	bool isPlaying() const;
	void setIsPlaying(bool bIsPlaying);

public:
	QSharedPointer<ChannelsGroupInfos> m_pChannelGroupsInfos;
	QSharedPointer<ChannelInfos> m_pChannelInfos;

	bool m_bIsPlaying;

};


#endif //FREETUXTV_QCHANNELSLISTITEM_H
