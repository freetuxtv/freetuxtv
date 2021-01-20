/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * freetuxtv
 * Copyright (C) Eric Beuque 2010 <eric.beuque@gmail.com>
 * 
 * freetuxtv is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * freetuxtv is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "ChannelsGroupInfos.h"

ChannelsGroupInfos::ChannelsGroupInfos()
{
	m_id = 0;
	m_iPosition = 0;
	m_iGroupType = Playlist;
	m_iNbChannels = 0;
}

ChannelsGroupInfos::ChannelsGroupInfos(const ChannelsGroupInfos& other)
{
	m_id = other.m_id;
	m_iPosition = other.m_iPosition;
	m_szName = other.m_szName;
	m_szURI = other.m_szURI;
	m_szRequiredISP = other.m_szRequiredISP;

	m_iGroupType = other.m_iGroupType;

	m_szBRegex = other.m_szBRegex;
	m_szERegex = other.m_szERegex;

	m_iNbChannels = other.m_iNbChannels;
}

ChannelsGroupInfos::~ChannelsGroupInfos()
{

}

void ChannelsGroupInfos::setId(int iId)
{
	m_id = iId;
}

int ChannelsGroupInfos::getId() const
{
	return m_id;
}

void ChannelsGroupInfos::setPosition(int iPosition)
{
	m_iPosition = iPosition;
}

int ChannelsGroupInfos::getPosition() const
{
	return m_iPosition;
}

void ChannelsGroupInfos::setName(const QString& szName)
{
	m_szName = szName;
}

const QString& ChannelsGroupInfos::getName() const
{
	return m_szName;
}

void ChannelsGroupInfos::setURI(const QString& szURI)
{
	m_szURI = szURI;
}

const QString& ChannelsGroupInfos::getURI() const
{
	return m_szURI;
}

void ChannelsGroupInfos::setRequiredISP(const QString& szRequiredISP)
{
	m_szRequiredISP = szRequiredISP;
}

const QString& ChannelsGroupInfos::getRequiredISP() const
{
	return m_szRequiredISP;
}


void ChannelsGroupInfos::setGroupType(GroupType iGroupType)
{
	m_iGroupType = iGroupType;
}

ChannelsGroupInfos::GroupType ChannelsGroupInfos::getGroupType() const
{
	return m_iGroupType;
}

void ChannelsGroupInfos::setBRegex(const QString& szRegex)
{
	m_szBRegex = szRegex;
}

const QString& ChannelsGroupInfos::getBRegex() const
{
	return m_szBRegex;
}

void ChannelsGroupInfos::setERegex(const QString& szRegex)
{
	m_szERegex = szRegex;
}

const QString& ChannelsGroupInfos::getERegex() const
{
	return m_szERegex;
}

void ChannelsGroupInfos::setNbChannels(int iNbChannels)
{
	m_iNbChannels = iNbChannels;
}

int ChannelsGroupInfos::getNbChannels() const
{
	return m_iNbChannels;
}

ChannelsGroupInfosList::ChannelsGroupInfosList()
{

}

ChannelsGroupInfosList::~ChannelsGroupInfosList()
{

}