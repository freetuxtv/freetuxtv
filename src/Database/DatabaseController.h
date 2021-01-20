//
// Created by ebeuque on 20/01/2021.
//

#ifndef FREETUXTV_DATABASECONTROLLER_H
#define FREETUXTV_DATABASECONTROLLER_H

#include "Database/DatabaseInstance.h"

#include "Model/ChannelsGroupInfos.h"

class DatabaseController
{
public:
	DatabaseController(DatabaseInstance& dbInstance);
	virtual ~DatabaseController();

public:
	bool loadChannelsGroups(ChannelsGroupInfosList& listChannelsGroupInfos);

private:
	DatabaseInstance& m_dbInstance;
	QSqlDatabase& m_db;
};


#endif //FREETUXTV_DATABASECONTROLLER_H
