//
// Created by ebeuque on 20/01/2021.
//

#include <QVariant>
#include <QSqlQuery>

#include "DatabaseController.h"

DatabaseController::DatabaseController(DatabaseInstance& dbInstance)
	: m_dbInstance(dbInstance), m_db(dbInstance.db())
{

}

DatabaseController::~DatabaseController()
{

}

bool DatabaseController::loadChannelsGroups(ChannelsGroupInfosList& listChannelsGroupInfos)
{
	bool bRes;

	QSqlQuery query(m_db);
	bRes = query.prepare("SELECT id, position, name, type, uri, bregex, eregex, last_update FROM channels_group ORDER BY position");
	if(!bRes){
		qCritical("[Database] Unable to prepare query to load channels group");
	}

	QSharedPointer<ChannelsGroupInfos> pChannelsGroupInfos;

	if(bRes) {
		bRes = query.exec();
		if (bRes){
			while(query.next())
			{
				pChannelsGroupInfos = QSharedPointer<ChannelsGroupInfos>(new ChannelsGroupInfos());

				pChannelsGroupInfos->setId(query.value(0).toInt());

				pChannelsGroupInfos->setPosition(query.value(1).toInt());
				pChannelsGroupInfos->setName(query.value(2).toString());

				pChannelsGroupInfos->setGroupType((ChannelsGroupInfos::GroupType)query.value(3).toInt());

				pChannelsGroupInfos->setURI(query.value(4).toString());
				//pChannelsGroupInfos->setRequiredISP();

				pChannelsGroupInfos->setBRegex(query.value(5).toString());
				pChannelsGroupInfos->setERegex(query.value(6).toString());

				pChannelsGroupInfos->setNbChannels(0);

				listChannelsGroupInfos.append(pChannelsGroupInfos);
			}
		}else{
			qCritical("[Database] Unable to prepare query to load channels group");
		}
	}

	return bRes;
}