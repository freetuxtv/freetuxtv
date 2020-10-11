//
// Created by ebeuque on 07/10/2020.
//

#ifndef FREETUXTV_PREFERENCES_H
#define FREETUXTV_PREFERENCES_H

#include <QString>

#include "Model/Proxy.h"

class Preferences
{
public:
	Preferences();
	virtual ~Preferences();

public:
	// Preferences general
	bool m_bChannelOnStartup;
	bool m_bEnableNotifications;
	bool m_bIgnoreDiacritics;

	// Preferences LibVLC
	int m_iLibVLCConfigFileMode;

	// Preferences recordings
	QString m_szDirectoryRecordings;
	int m_iTranscodingMode;
	QString m_szTranscodingFormat;

	// Preferences network
	int m_iTimeout;
	Proxy m_proxy;
};


#endif //FREETUXTV_PREFERENCES_H
