//
// Created by ebeuque on 08/02/25.
//

#include "Database/DatabaseController.h"

#include "Global/UserPaths.h"

#include "Controller/ApplicationController.h"
#include "Controller/TVChannelsController.h"

#include "TVChannelInfosListSynchronizer.h"

TVChannelInfosListSynchronizer::TVChannelInfosListSynchronizer(DatabaseInstance& dbInstance, const QString& szLogosURL, SynchronizeProgressCB progressCB)
	: m_dbInstance(dbInstance), m_szLogosURL(szLogosURL), m_progressCB(progressCB)
{

}

TVChannelInfosListSynchronizer::~TVChannelInfosListSynchronizer()
{

}

bool TVChannelInfosListSynchronizer::synchronize (QError& error)
{
	bool bRes;

	// Update the status bar to the process
	QString szText;
	szText = tr("Synchronizing the tv channels list");
	ApplicationController::pushStatusBarMessage("UpdateMsg", szText);
	qDebug("[TVChannelsController] Synchronizing the tv channels list");

	// Delete the TV channel in the database
	DatabaseController dbc(m_dbInstance);
	bRes = dbc.deleteTVChannels(error);

	// Load the list of TV channel in the database for the XML file
	if(bRes){
		QString szFileName;
		QFile file;

		// If exists, load the downloaded file from cache
		szFileName = UserPaths::getUserCacheDir().filePath("tv_channels.dat");

		file.setFileName(szFileName);
		if(!file.exists()){
			// If not exists, fallback to the packaged version of the file
			szFileName = UserPaths::getApplicationSharePath().filePath("tv_channels.xml");
			file.setFileName(szFileName);
		}

		// Parse the file
		bRes = file.open(QIODevice::ReadOnly);
		if(bRes){
			// TODO: Replace with modern QXmlStreamReader-based parser
			// For now, keep the old implementation but add warning
			qWarning("Using deprecated XML parser - should be replaced with QXmlStreamReader");
			
			QXmlInputSource source(&file);

			QXmlSimpleReader reader;
			TVChannelInfosListParserHandler handler(error);
			handler.setUpdater(this);
			reader.setContentHandler(&handler);
			reader.setErrorHandler(&handler);

			bRes = reader.parse(source);
			if(!bRes){
				qCritical("[XML] Error to parse the content of file: %s", qPrintable(szFileName));
			}

			file.close();
		}else{
			qCritical("[XML] Error to open the file: %s", qPrintable(szFileName));
		}

	}

	ApplicationController::popStatusBarMessage("UpdateMsg");

	return bRes;
}

void TVChannelInfosListSynchronizer::onProgress(const QString& szTVChannelName)
{
	#warning "need implement"
}

bool TVChannelInfosListSynchronizer::finalize(const QSharedPointer<TVChannelInfos>& pTVChannelInfos, QError& error)
{
	bool bRes;

	// We have a channel pending, we add it in database
	DatabaseController dbc(m_dbInstance);
	bRes = dbc.addTVChannelInfos(pTVChannelInfos, error);

	// Download logo for the channel if required
	if(bRes){
		QString szLogoName = pTVChannelInfos->getLogoFileName();

		if(!szLogoName.isEmpty() && !m_szLogosURL.isEmpty()){
			QDir szUserImgChannelsDir = UserPaths::getUserImagesChannelsPath();

			#warning "implement this"
			//tvchannels_list_download_logos(cbxmldata->app, cbxmldata->szLogosURL, szUserImgChannelsDir, szLogoName);
		}
	}

	return bRes;
}