//
// Created by ebeuque on 20/01/2021.
//

#ifndef FREETUXTV_DATABASECONTROLLER_H
#define FREETUXTV_DATABASECONTROLLER_H

#include "Global/QError.h"

#include "Database/DatabaseInstance.h"

#include "Model/ChannelInfos.h"
#include "Model/ChannelsGroupInfos.h"

typedef bool (*CBOnChannelsGroupLoaded)(DatabaseInstance& m_dbInstance, const ChannelsGroupInfos& channelGroupInfos, void* user_data, QError& error);
typedef bool (*CBOnChannelsLoaded)(DatabaseInstance& m_dbInstance, const ChannelInfos& channelInfos, void* user_data, QError& error);

class DatabaseController : public QObject
{
	Q_OBJECT
public:
	DatabaseController(DatabaseInstance& dbInstance);
	virtual ~DatabaseController();

public:
	bool loadChannelsGroups(CBOnChannelsGroupLoaded cbOnChannelsGroupLoaded, void* user_data, QError& error);

	bool loadChannels(int iChannelsGroupId, CBOnChannelsLoaded cbOnChannelsLoaded, void* user_data, QError& error);

private:
	DatabaseInstance& m_dbInstance;
	QSqlDatabase& m_db;
};


#endif //FREETUXTV_DATABASECONTROLLER_H
