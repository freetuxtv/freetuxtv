//
// Created by ebeuque on 08/02/25.
//

#ifndef FREETUXTV_CONTROLLER_TVCHANNELSCONTROLLER_H
#define FREETUXTV_CONTROLLER_TVCHANNELSCONTROLLER_H

#include <QString>
#include <QDir>
#include <QSharedPointer>

#include "Model/TVChannelInfos.h"
#include "Model/ChannelInfos.h"
#include "Model/RecordingInfos.h"

#include "Database/DatabaseInstance.h"

#include "Global/Application.h"
#include "Global/QError.h"

#include "TVChannelInfosListSynchronizer.h"

class TVChannelsController : public QObject
{
	Q_OBJECT
public:
	static bool synchronize (DatabaseInstance& dbInstance, const QString& szLogosURL, SynchronizeProgressCB funcCB, void* user_data, QError& error);

	static QString getTVChannelLogoPathForTVChannel(const QSharedPointer<TVChannelInfos>& pChannelInfos, bool bNoneIcon);
	static QString getTVChannelLogoPathForChannel(const QSharedPointer<ChannelInfos>& pChannelInfos, bool bNoneIcon);
	static QString getTVChannelLogoPathForRecording(const QSharedPointer<RecordingInfos>& pRecordingInfos, bool bNoneIcon);

private:
	static QString getTVChannelLogoPath(const QString& szLogoFileName, bool bNoneIcon);
	static void downloadLogos(const QString& szLogosDirectoryURL, const QDir& dirUserImgChannels, const char* szLogoName, Application* pApplication);
};


#endif //FREETUXTV_CONTROLLER_TVCHANNELSCONTROLLER_H
