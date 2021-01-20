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

bool DatabaseController::loadChannelsGroups(CBOnChannelsGroupLoaded cbOnChannelsGroupLoaded, void* user_data)
{
	bool bRes;

	QSqlQuery query(m_db);
	bRes = query.prepare("SELECT id, position, name, type, uri, bregex, eregex, last_update FROM channels_group ORDER BY position");
	if(!bRes){
		qCritical("[Database] Unable to prepare query to load channels group");
	}

	if(bRes) {
		bRes = query.exec();
		if (bRes){
			while(query.next())
			{
				ChannelsGroupInfos channelsGroupInfos;

				channelsGroupInfos.setId(query.value(0).toInt());

				channelsGroupInfos.setPosition(query.value(1).toInt());
				channelsGroupInfos.setName(query.value(2).toString());

				channelsGroupInfos.setGroupType((ChannelsGroupInfos::GroupType)query.value(3).toInt());

				channelsGroupInfos.setURI(query.value(4).toString());
				//pChannelsGroupInfos->setRequiredISP();

				channelsGroupInfos.setBRegex(query.value(5).toString());
				channelsGroupInfos.setERegex(query.value(6).toString());

				channelsGroupInfos.setNbChannels(0);

				if(cbOnChannelsGroupLoaded){
					bRes = cbOnChannelsGroupLoaded(m_dbInstance, channelsGroupInfos, user_data);
					if(!bRes){
						break;
					}
				}
			}
		}else{
			qCritical("[Database] Unable to prepare query to load channels group");
		}
	}

	return bRes;
}