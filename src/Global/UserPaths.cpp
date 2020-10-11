//
// Created by ebeuque on 11/10/2020.
//

#include <QCoreApplication>
#include <QDir>
#include <QStandardPaths>

#include "UserPaths.h"

QDir UserPaths::getUserCacheDir()
{
	QDir dir;
	dir = QStandardPaths::writableLocation(QStandardPaths::GenericCacheLocation);
	return dir.filePath("freetuxtv");

	// TODO: prefer the standard path
	//return QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
}