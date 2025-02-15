//
// Created by ebeuque on 11/10/2020.
//

#include <QCoreApplication>
#include <QDir>
#include <QStandardPaths>

#include "UserPaths.h"

QDir UserPaths::getUserConfigPath()
{
	QDir dir;
	dir = QStandardPaths::writableLocation(QStandardPaths::GenericConfigLocation);
	return dir.filePath("FreetuxTV");
}

QString UserPaths::getUserDatabasePath()
{
	QDir dir;
	dir = getUserConfigPath();
	return dir.filePath("freetuxtv.db");
}

QDir UserPaths::getUserCacheDir()
{
	QDir dir;
	dir = QStandardPaths::writableLocation(QStandardPaths::GenericCacheLocation);
	return dir.filePath("freetuxtv");

	// TODO: prefer the standard path
	//return QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
}

QDir UserPaths::getUserShareDir()
{
	QDir dir;
	dir = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation);
	return dir.filePath("freetuxtv");
}

QDir UserPaths::getUserImagesPath()
{
	QDir dir = getUserShareDir();
	return dir.filePath("images");
}

QDir UserPaths::getUserImagesChannelsPath()
{
	QDir dir;
	dir = getUserImagesPath();
	#warning "Verify the path here"
	// must be // .local/share/freetuxtv/images/channels
	return dir.filePath("channels");
}

QDir UserPaths::getApplicationSharePath()
{
	QDir dir;

#ifdef QT_DEBUG
	dir.setPath("../data/");
	if(dir.exists("freetuxtv.png")){
		return dir.absolutePath();
	}
#endif

	dir.setPath("/usr/share/freetuxtv");
	return dir;
}

QDir UserPaths::getApplicationImagesPath()
{
	QDir dir = getApplicationSharePath();
	return dir.filePath("images");
}

QDir UserPaths::getApplicationImagesChannelsPath()
{
	QDir dir = getApplicationImagesPath();
	return dir.filePath("channels");
}