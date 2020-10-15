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

#ifndef _FREETUXTV_CHANNELS_GROUP_INFOS_H_
#define _FREETUXTV_CHANNELS_GROUP_INFOS_H_

#include <QString>

class ChannelsGroupInfos
{
public:
	ChannelsGroupInfos();
	virtual ~ChannelsGroupInfos();

public:
	enum GroupType
	{
		Playlist,
		Favourites
	};

public:
	void setId(int iId);
	int getId() const;

	void setPosition(int iPosition);
	int getPosition() const;

	void setName(const QString& szName);
	const QString& getName() const;

	void setURI(const QString& szURI);
	const QString& getURI() const;

	void setGroupType(GroupType iGroupType);
	GroupType getGroupType() const;

	void setBRegex(const QString& szRegex);
	const QString& getBRegex() const;

	void setERegex(const QString& szRegex);
	const QString& getERegex() const;

	void setNbChannels(int iNbChannels);
	int getNbChannels() const;

private:
	int m_id;
	int m_iPosition;
	QString m_szName;
	QString m_szURI;

	GroupType m_iGroupType;

	QString m_szBRegex;
	QString m_szERegex;

	int m_iNbChannels;
};

#endif /* _FREETUXTV_CHANNELS_GROUP_INFOS_H_ */

