//
// Created by ebeuque on 20/10/2020.
//

#include "ChannelsGroupSection.h"

ChannelsGroupSection::ChannelsGroupSection()
{

}

ChannelsGroupSection::~ChannelsGroupSection()
{

}

void ChannelsGroupSection::setId(const QString& szId)
{
	m_szId = szId;
}

const QString& ChannelsGroupSection::getId() const
{
	return m_szId;
}

void ChannelsGroupSection::setLang(const QString& szLang)
{
	m_szLang = szLang;
}

const QString& ChannelsGroupSection::getLang() const
{
	return m_szLang;
}

void ChannelsGroupSection::addChannelsGroup(const ChannelsGroupInfos& channelsGroup)
{
	m_listChannelsGroup.append(channelsGroup);
}

const QList<ChannelsGroupInfos>& ChannelsGroupSection::getChannelsGroupList() const
{
	return m_listChannelsGroup;
}