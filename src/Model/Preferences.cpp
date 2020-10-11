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
}

Preferences::~Preferences()
{

}