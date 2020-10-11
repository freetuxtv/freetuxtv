//
// Created by ebeuque on 11/10/2020.
//

#ifndef FREETUXTV_FILEUTILS_H
#define FREETUXTV_FILEUTILS_H

#include <QUrl>

#include "Global/QError.h"

#include "Model/Proxy.h"

class FileUtils
{
public:
	static bool getFile (const QUrl& url, const QString& szDstFilePath, Proxy* pProxy, int iTimeout, QError* pError);
};


#endif //FREETUXTV_FILEUTILS_H
