//
// Created by ebeuque on 20/01/2021.
//

#include "ChannelInfos.h"

ChannelInfos::ChannelInfos()
{
	m_id = 0;
	m_iPosition = 0;
	m_iChannelsGroupsInfosId = 0;
}

ChannelInfos::~ChannelInfos()
{

}

void ChannelInfos::setId(int iId)
{
	m_id = iId;
}

int ChannelInfos::getId() const
{
	return m_id;
}

void ChannelInfos::setPosition(int iPosition)
{
	m_iPosition = iPosition;
}

int ChannelInfos::getPosition() const
{
	return m_iPosition;
}

void ChannelInfos::setName(const QString& szName)
{
	m_szName = szName;
}

const QString& ChannelInfos::getName() const
{
	return m_szName;
}

void ChannelInfos::setUrl(const QString& szUrl)
{
	m_szUrl = szUrl;
}

const QString& ChannelInfos::getUrl() const
{
	return m_szUrl;
}

void ChannelInfos::setVLCOptions(const QStringList& listOptions)
{
	m_listVLCOptions = listOptions;
}

const QStringList& ChannelInfos::getVLCOptions() const
{
	return m_listVLCOptions;
}

void ChannelInfos::setDeinterlaceMode(const QString& szMode)
{
	m_szDeinterlaceMode = szMode;
}

const QString& ChannelInfos::getDeinterlaceMode() const
{
	return m_szDeinterlaceMode;
}

void ChannelInfos::setChannelsGroupsInfosId(int iId)
{
	m_iChannelsGroupsInfosId = iId;
}

int ChannelInfos::getChannelsGroupsInfosId() const
{
	return m_iChannelsGroupsInfosId;
}

void ChannelInfos::setLogoFileName(const QString& szName)
{
	m_szLogoFileName = szName;
}

const QString& ChannelInfos::getLogoFileName() const
{
	return m_szLogoFileName;
}