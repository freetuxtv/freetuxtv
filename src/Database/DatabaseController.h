//
// Created by ebeuque on 20/01/2021.
//

#ifndef FREETUXTV_DATABASECONTROLLER_H
#define FREETUXTV_DATABASECONTROLLER_H

#include <QSharedPointer>

#include "Global/QError.h"

#include "Database/DatabaseInstance.h"

#include "Model/TVChannelInfos.h"
#include "Model/ChannelInfos.h"
#include "Model/ChannelsGroupInfos.h"

typedef bool (*CBOnChannelsGroupLoaded)(DatabaseInstance& m_dbInstance, const QSharedPointer<ChannelsGroupInfos>& pChannelGroupInfos, void* user_data, QError& error);
typedef bool (*CBOnChannelsLoaded)(DatabaseInstance& m_dbInstance, const QSharedPointer<ChannelInfos>& pChannelInfos, void* user_data, QError& error);

class DatabaseController : public QObject
{
	Q_OBJECT
public:
	DatabaseController(DatabaseInstance& dbInstance);
	virtual ~DatabaseController();

public:
	// About TV Channel
	bool addTVChannelInfos(const QSharedPointer<TVChannelInfos>& pChannelInfos, QError& error);
	bool addTVChannelInfosLabel(int iTVChannelInfos, const QString& szLabel, QError& error);
	bool linkTVChannelToChannelsFromLabel(const QString& szLabel, int iTVChannelInfos, QError& error);
	bool deleteTVChannels(QError& error);

	// About Channels Group
	bool loadChannelsGroups(CBOnChannelsGroupLoaded cbOnChannelsGroupLoaded, void* user_data, QError& error);

	// About Channel
	bool loadChannels(int iChannelsGroupId, CBOnChannelsLoaded cbOnChannelsLoaded, void* user_data, QError& error);
	bool saveChannelInfos(const QSharedPointer<ChannelInfos>& pChannelInfos, bool bUpdate, QError& error);


	/*
	 *
void
dbsync_select_channels_of_channels_group (DBSync *dbsync,
    FreetuxTVChannelsGroupInfos* pChannelsGroupInfos,
    FreetuxTVApp *app,
    int (*callback)(FreetuxTVApp *app,
	    FreetuxTVChannelInfos* pChannelInfos,
	    DBSync *dbsync, gpointer user_data, GError** error),
    gpointer user_data, GError** error);

void
dbsync_update_channels_group_last_update (DBSync *dbsync,
    FreetuxTVChannelsGroupInfos* pChannelsGroupInfos,
    GError** error);

void
dbsync_delete_channels_group (DBSync *dbsync,
    FreetuxTVChannelsGroupInfos* pChannelsGroupInfos,
    GError** error);

void
dbsync_delete_channels_of_channels_group (DBSync *dbsync,
    FreetuxTVChannelsGroupInfos* pChannelsGroupInfos,
    GError** error);

void
dbsync_start_update_channels_of_channels_group (DBSync *dbsync,
    FreetuxTVChannelsGroupInfos* pChannelsGroupInfos,
    GError** error);

void
dbsync_end_update_channels_of_channels_group (DBSync *dbsync,
    FreetuxTVChannelsGroupInfos* pChannelsGroupInfos,
    GError** error);

// Channels manipulation function

void
dbsync_add_channel (DBSync *dbsync,
    FreetuxTVChannelInfos* pChannelInfos,
    gboolean update,
    GError** error);

void
dbsync_delete_channel (DBSync *dbsync,
    FreetuxTVChannelInfos* pChannelInfos,
    GError** error);

int
dbsync_get_channel_id_by_name (DBSync *dbsync,
    const gchar* szChannelName,
    GError** error);

void
dbsync_update_channel_deinterlace_mode (DBSync *dbsync,
    FreetuxTVChannelInfos* pChannelInfos,
    gchar *mode,
    GError** error);

void
dbsync_add_channels_group (DBSync *dbsync,
    FreetuxTVChannelsGroupInfos* pChannelsGroupInfos,
    GError** error);

void
dbsync_update_channels_group (DBSync *dbsync,
    FreetuxTVChannelsGroupInfos* pChannelsGroupInfos,
    GError** error);

void
dbsync_switch_position_channels_group (DBSync *dbsync,
    FreetuxTVChannelsGroupInfos* pChannelsGroupInfosA,
    FreetuxTVChannelsGroupInfos* pChannelsGroupInfosB,
    GError** error);

void
dbsync_switch_position_channel (DBSync *dbsync,
    FreetuxTVChannelInfos* pChannelInfosA,
    FreetuxTVChannelInfos* pChannelInfosB,
    GError** error);

// TVChannels manipulation function

void
dbsync_add_tvchannel (DBSync *dbsync, FreetuxTVTvChannelInfos* pTvChannelInfos,
    GError** error);

// Recordings manipulation function

void
dbsync_add_recording (DBSync *dbsync, FreetuxTVRecordingInfos* pRecordingInfos,
    GError** error);

void
dbsync_select_recordings (DBSync *dbsync,
    FreetuxTVApp *app,
    int (*callback)(FreetuxTVApp *app,
	    FreetuxTVRecordingInfos* pRecordingInfos,
	    DBSync *dbsync, gpointer user_data, GError** error),
    gpointer user_data, GError** error);

void
dbsync_update_recording (DBSync *dbsync,
    FreetuxTVRecordingInfos* pRecordingInfos,
    GError** error);

void
dbsync_delete_recording (DBSync *dbsync,
    FreetuxTVRecordingInfos* pRecordingInfos,
    GError** error);
	 */

private:
	DatabaseInstance& m_dbInstance;
	QSqlDatabase& m_db;
};


#endif //FREETUXTV_DATABASECONTROLLER_H
