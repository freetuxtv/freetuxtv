//
// Created by ebeuque on 20/01/2021.
//

#include <QVariant>
#include <QSqlQuery>
#include <QSqlError>

#include "DatabaseController.h"

DatabaseController::DatabaseController(DatabaseInstance& dbInstance)
	: m_dbInstance(dbInstance), m_db(dbInstance.db())
{

}

DatabaseController::~DatabaseController()
{

}

bool DatabaseController::loadChannelsGroups(CBOnChannelsGroupLoaded cbOnChannelsGroupLoaded, void* user_data, QError& error)
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
					bRes = cbOnChannelsGroupLoaded(m_dbInstance, channelsGroupInfos, user_data, error);
					if(!bRes){
						break;
					}
				}
			}
		}else{
			qCritical("[Database] Unable to load channels group");
		}
	}

	return bRes;
}

bool DatabaseController::loadChannels(int iChannelsGroupId, CBOnChannelsLoaded cbOnChannelsLoaded, void* user_data, QError& error)
{
	bool bRes;

	QString szQuery;
	szQuery = "SELECT channel.id, channel.name, channel.position, channel.uri, channel.vlc_options, channel.views, channel.channelsgroup_id, " \
		 " channel.tvchannel_id, channel.deinterlace_mode, channel.updated, " \
		 " tvchannel.logo_filename" \
		 " FROM channel LEFT JOIN tvchannel ON channel.tvchannel_id=tvchannel.id " \
		 " WHERE channel.channelsgroup_id=:channelsgroup_id  " \
		 " ORDER BY channel.position";

	QSqlQuery query(m_db);
	bRes = query.prepare(szQuery);
	if(!bRes){
		qCritical("[Database] Unable to prepare query to load channels");
		error = tr("Error when displaying the channels.\n\nSQLite has returned error:\n%0.").arg(query.lastError().text());
	}

	if(bRes){
		query.bindValue(":channelsgroup_id", iChannelsGroupId);
	}

	if(bRes) {
		bRes = query.exec();
		if (bRes){
			while(query.next())
			{
				ChannelInfos channelInfos;

				channelInfos.setId(query.value(0).toInt());

				channelInfos.setName(query.value(1).toString());
				channelInfos.setPosition(query.value(2).toInt());

				/*
				channelsGroupInfos.setGroupType((ChannelsGroupInfos::GroupType)query.value(3).toInt());

				channelsGroupInfos.setURI(query.value(4).toString());
				//pChannelsGroupInfos->setRequiredISP();

				channelsGroupInfos.setBRegex(query.value(5).toString());
				channelsGroupInfos.setERegex(query.value(6).toString());

				channelsGroupInfos.setNbChannels(0);
				 */

				if(cbOnChannelsLoaded){
					bRes = cbOnChannelsLoaded(m_dbInstance, channelInfos, user_data, error);
					if(!bRes){
						break;
					}
				}


				// Set the group of the channel
				//freetuxtv_channel_infos_set_channels_group(pChannelInfos, pChannelsGroupInfos);
				//pChannelsGroupInfos->nb_channels++;
			}
		}else{
			qCritical("[Database] Unable to load channels group");
			error = tr("Error when displaying the channels.\n\nSQLite has returned error:\n%0.").arg(query.lastError().text());
		}
	}

	return bRes;
}