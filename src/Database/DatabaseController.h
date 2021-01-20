//
// Created by ebeuque on 20/01/2021.
//

#ifndef FREETUXTV_DATABASECONTROLLER_H
#define FREETUXTV_DATABASECONTROLLER_H

#include "Database/DatabaseInstance.h"

#include "Model/ChannelsGroupInfos.h"

typedef bool (*CBOnChannelsGroupLoaded)(DatabaseInstance& m_dbInstance, const ChannelsGroupInfos& channelGroupInfos, void* user_data);

class DatabaseController
{
public:
	DatabaseController(DatabaseInstance& dbInstance);
	virtual ~DatabaseController();

public:
	bool loadChannelsGroups(CBOnChannelsGroupLoaded cbOnChannelsGroupLoaded, void* user_data);

private:
	DatabaseInstance& m_dbInstance;
	QSqlDatabase& m_db;
};


#endif //FREETUXTV_DATABASECONTROLLER_H
