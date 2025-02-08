//
// Created by ebeuque on 08/02/25.
//

#ifndef FREETUXTV_CHANNELSGROUPINFOSLIST_H
#define FREETUXTV_CHANNELSGROUPINFOSLIST_H

#include <QList>
#include <QSharedPointer>

#include "ChannelsGroupInfos.h"

class ChannelsGroupInfosList : public QList< QSharedPointer<ChannelsGroupInfos> >
{
	public:
	ChannelsGroupInfosList();
	virtual ~ChannelsGroupInfosList();
};

#endif //FREETUXTV_CHANNELSGROUPINFOSLIST_H
