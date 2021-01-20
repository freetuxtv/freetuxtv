//
// Created by ebeuque on 15/10/2020.
//

#include "Model/Preferences.h"

#include "Application.h"

Application::Application()
{
	m_pPreferences = new Preferences();
}

Application::~Application()
{
	if(m_pPreferences){
		delete m_pPreferences;
		m_pPreferences = NULL;
	}
}


void Application::setDataDir(const QString& szDataDir)
{
	m_szDataDir = szDataDir;
}

const QString& Application::getDataDir() const
{
	return m_szDataDir;
}

Preferences* Application::getPreferences()
{
	return m_pPreferences;
}

QStandardItemModel* Application::getChannelListModel()
{
	return &m_modelChannelsList;
}