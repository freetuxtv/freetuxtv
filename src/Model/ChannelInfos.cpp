//
// Created by ebeuque on 20/01/2021.
//

#include "ChannelInfos.h"

ChannelInfos::ChannelInfos()
{
	m_id = 0;
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