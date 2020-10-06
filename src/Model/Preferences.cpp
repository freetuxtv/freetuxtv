//
// Created by ebeuque on 07/10/2020.
//

#include "Preferences.h"

Preferences::Preferences()
{
	m_bChannelOnStartup = true;
	m_bEnableNotifications = true;
	m_bIgnoreDiacritics = true;

	// Preferences LibVLC
	m_iLibVLCConfigFileMode = 0;

	// Preferences recordings
	m_iTranscodingMode = 0;

	// Preferences network
	m_iTimeout = 20;
	m_iProxyMode = 0;
	m_iProxyPort = -1;
	m_bProxyAuth = false;
}

Preferences::~Preferences()
{

}