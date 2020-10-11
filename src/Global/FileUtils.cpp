//
// Created by ebeuque on 11/10/2020.
//

#include <QFile>
#include <QNetworkAccessManager>

#include "Global/QFileDownloader.h"

#include "FileUtils.h"


bool FileUtils::getFile (const QUrl& url, const QString& szDstFilePath, Proxy* pProxy, int iTimeout, QError* pError)
{
	bool bRes = true;

	if((url.scheme() == "http") || (url.scheme() == "https"))
	{
		QFileDownloader fileDownload(url);
		fileDownload.setTimeoutMs(iTimeout*1000);
		fileDownload.setProxy(pProxy);
		bRes = fileDownload.download();

		if(bRes) {
			QFile dstFile(szDstFilePath);
			bRes = dstFile.open(QIODevice::WriteOnly);
			if (bRes) {
				bRes = dstFile.write(fileDownload.downloadedData());
				if(!bRes){
					*pError = QObject::tr("Unable to write file %0").arg(szDstFilePath);
				}
				dstFile.close();
			}else{
				*pError = QObject::tr("Unable to open file %0 for writing").arg(szDstFilePath);
			}
		}

		if(!bRes){
			*pError = QObject::tr("Error when downloading the file from URL -> %0.").arg(url.toString());
		}

	}else{
		QFile file(url.toString());
		bRes = file.copy(szDstFilePath);
		if(!bRes){
			if(pError) {
				*pError = QObject::tr("Unable to copy the file given");
			}
		}
	}

	return bRes;
}