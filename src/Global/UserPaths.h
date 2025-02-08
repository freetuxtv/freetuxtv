//
// Created by ebeuque on 11/10/2020.
//

#ifndef FREETUXTV_USERPATHS_H
#define FREETUXTV_USERPATHS_H

#include <QDir>

class UserPaths {
public:
	static QDir getUserConfigPath();

	static QString getUserDatabasePath();

	static QDir getUserCacheDir();
	static QDir getUserShareDir();

	static QDir getUserImagesPath();
	static QDir getUserImagesChannelsPath();

	static QDir getApplicationSharePath();
	static QDir getApplicationImagesPath();
	static QDir getApplicationImagesChannelsPath();
};


#endif //FREETUXTV_USERPATHS_H
