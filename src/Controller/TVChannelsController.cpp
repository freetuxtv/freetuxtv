//
// Created by ebeuque on 08/02/25.
//

#include "Global/FileUtils.h"
#include "Global/UserPaths.h"

#include "Model/Preferences.h"

#include "TVChannelsController.h"

bool TVChannelsController::synchronize (DatabaseInstance& dbInstance, const QString& szLogosURL, SynchronizeProgressCB funcCB, void* user_data, QError& error)
{
	TVChannelInfosListSynchronizer sync(dbInstance, szLogosURL, funcCB);
	return sync.synchronize(error);
}

QString TVChannelsController::getTVChannelLogoPath(const QString& szLogoName, bool bNoneIcon)
{
	QString szImgFile;
	QDir dirUserImgChannels;
	bool bFound = false;

	dirUserImgChannels = UserPaths::getUserImagesChannelsPath();

	if(szLogoName != NULL){
		// We look in the user logo directory
		szImgFile = dirUserImgChannels.filePath(szLogoName);
		QFile file(szImgFile);
		if(file.exists()){
			bFound = true;
		}
		if(!bFound){
			szImgFile = UserPaths::getApplicationImagesChannelsPath().filePath(szLogoName);
			if(file.exists()){
				bFound = true;
			}
		}
	}

	if(!bFound){
		if(bNoneIcon){
			szImgFile = dirUserImgChannels.filePath("_none.png");
			QFile file(szImgFile);
			if(file.exists()){
				szImgFile = QString();
			}else{
				szImgFile = UserPaths::getApplicationImagesChannelsPath().filePath("_none.png");
			}
		}
	}

	return szImgFile;
}

QString TVChannelsController::getTVChannelLogoPathForChannel(const QSharedPointer<TVChannelInfos>& pChannelInfos, bool bNoneIcon)
{
	QString szLogoName;
	if(pChannelInfos){
		szLogoName = pChannelInfos->getLogoFileName();
	}
	return getTVChannelLogoPath(szLogoName, bNoneIcon);
}

QString TVChannelsController::getTVChannelLogoPathForRecording(const QSharedPointer<RecordingInfos>& pRecordingInfos, bool bNoneIcon)
{
#warning "fix this"
	QSharedPointer<TVChannelInfos> pChannelInfos;
	//pChannelInfos = channels_list_get_channel_by_id(app, pRecordingInfos->channel_id);
	return getTVChannelLogoPathForChannel(pChannelInfos, bNoneIcon);
}



void TVChannelsController::downloadLogos(const QString& szLogosDirectoryURL, const QDir& dirUserImgChannels, const char* szLogoName, Application* pApplication)
{
	bool bRes;

	QString szLogoURL;
	QString szDstFile;

	if(szLogosDirectoryURL.endsWith("/")){
		szLogoURL = szLogosDirectoryURL + szLogoName;
	}else{
		szLogoURL = szLogosDirectoryURL + "/" + szLogoName;
	}

	QError error;
	szDstFile = dirUserImgChannels.filePath(szLogoName);

	qDebug("[TVChannelsController] Downloading file : %s\n", qPrintable(szLogoURL));
	bRes = FileUtils::getFile(szLogoURL, szDstFile, &pApplication->getPreferences()->m_proxy, pApplication->getPreferences()->m_iTimeout, &error);
	if(!bRes){
		qCritical("[TVChannelsController] Error : %s\n", qPrintable(error.getErrorMsg()));
	}
}