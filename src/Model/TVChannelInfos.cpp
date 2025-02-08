//
// Created by ebeuque on 08/02/25.
//

#include "TVChannelInfos.h"

TVChannelInfos::TVChannelInfos()
{
	m_iId = 0;
}

TVChannelInfos::~TVChannelInfos()
{

}

void TVChannelInfos::setId(int iId)
{
	m_iId = iId;
}

int TVChannelInfos::getId() const
{
	return m_iId;
}

void TVChannelInfos::setName(const QString& szName)
{
	m_szName = szName;
}

const QString& TVChannelInfos::getName() const
{
	return m_szName;
}

void TVChannelInfos::setLogoFileName(const QString& szName)
{
	m_szLogoFileName = szName;
}

const QString& TVChannelInfos::getLogoFileName() const
{
	return m_szLogoFileName;
}

void TVChannelInfos::addLabel(const QString& szLabel)
{
	m_listLabels.append(szLabel);
}

const QStringList& TVChannelInfos::getLabels() const
{
	return m_listLabels;
}