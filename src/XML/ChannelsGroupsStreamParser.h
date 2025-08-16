/* -*- Mode: C++; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * FreetuxTV
 * Copyright (C) Eric Beuque 2020 <eric.beuque@gmail.com>
 * 
 * FreetuxTV is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * FreetuxTV is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef CHANNELSGROUPSSTREAMPARSER_H
#define CHANNELSGROUPSSTREAMPARSER_H

#include <QList>
#include <QXmlStreamReader>

#include "Model/ChannelsGroupSection.h"

class ChannelsGroupsStreamParser
{
public:
    explicit ChannelsGroupsStreamParser(QList<ChannelsGroupSection>& listChannelsGroupSection);
    virtual ~ChannelsGroupsStreamParser();

    bool parse(QIODevice *device);
    bool parse(const QByteArray &data);
    
    QString errorString() const;

private:
    void parseChannelsGroupsElement();
    void parseChannelsGroupSectionElement();
    void parseChannelsGroupElement();
    void skipUnknownElement();

private:
    QXmlStreamReader m_reader;
    QList<ChannelsGroupSection>& m_listChannelsGroupSection;
    ChannelsGroupSection* m_pCurrentChannelsGroupSection;
};

#endif // CHANNELSGROUPSSTREAMPARSER_H