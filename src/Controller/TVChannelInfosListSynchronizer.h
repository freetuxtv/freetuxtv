//
// Created by ebeuque on 08/02/25.
//

#ifndef FREETUXTV_TVCHANNELINFOSLISTSYNCHRONIZER_H
#define FREETUXTV_TVCHANNELINFOSLISTSYNCHRONIZER_H

#include "Database/DatabaseInstance.h"

#include "Global/QError.h"

#include "XML/TVChannelInfosListParserHandler.h"

typedef void (*SynchronizeProgressCB)(const QString& szTVChannelName, void* user_data);

class TVChannelInfosListSynchronizer : public QObject, public TVChannelsInfosListParserUpdater
{
	Q_OBJECT
public:
	TVChannelInfosListSynchronizer(DatabaseInstance& dbInstance, const QString& szLogosURL, SynchronizeProgressCB progressCB);
	virtual ~TVChannelInfosListSynchronizer();

	bool synchronize (QError& error);

protected:
	virtual void onProgress(const QString& szTVChannelName) override;
	virtual bool finalize(const QSharedPointer<TVChannelInfos>& pTVChannelInfos, QError& error) override;

public:
	DatabaseInstance& m_dbInstance;
	const QString m_szLogosURL;
	SynchronizeProgressCB m_progressCB;

};


#endif //FREETUXTV_TVCHANNELINFOSLISTSYNCHRONIZER_H
