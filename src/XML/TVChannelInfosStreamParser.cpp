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

#include "TVChannelInfosStreamParser.h"

#include <QDebug>

TVChannelInfosStreamParser::TVChannelInfosStreamParser(QList<TVChannelInfos>& listTVChannelInfos)
    : m_listTVChannelInfos(listTVChannelInfos)
{
}

TVChannelInfosStreamParser::~TVChannelInfosStreamParser()
{
}

bool TVChannelInfosStreamParser::parse(QIODevice *device)
{
    m_reader.setDevice(device);
    
    if (m_reader.readNextStartElement()) {
        if (m_reader.name().toString() == "channels") {
            parseChannelsElement();
        } else {
            m_reader.raiseError(QObject::tr("The file is not a valid channels file."));
        }
    }
    
    return !m_reader.hasError();
}

bool TVChannelInfosStreamParser::parse(const QByteArray &data)
{
    m_reader.addData(data);
    return parse(nullptr);
}

QString TVChannelInfosStreamParser::errorString() const
{
    return m_reader.errorString();
}

void TVChannelInfosStreamParser::parseChannelsElement()
{
    Q_ASSERT(m_reader.isStartElement() && m_reader.name().toString() == "channels");

    while (m_reader.readNextStartElement()) {
        if (m_reader.name().toString() == "channel") {
            parseChannelElement();
        } else {
            skipUnknownElement();
        }
    }
}

void TVChannelInfosStreamParser::parseChannelElement()
{
    Q_ASSERT(m_reader.isStartElement() && m_reader.name().toString() == "channel");

    TVChannelInfos channelInfo;
    
    // Read attributes
    QXmlStreamAttributes attributes = m_reader.attributes();
    if (attributes.hasAttribute("name")) {
        channelInfo.setName(attributes.value("name").toString());
    }
    if (attributes.hasAttribute("logo")) {
        channelInfo.setLogoFileName(attributes.value("logo").toString());
    }
    
    // Read child elements
    while (m_reader.readNextStartElement()) {
        if (m_reader.name().toString() == "name") {
            channelInfo.setName(m_reader.readElementText());
        } else if (m_reader.name().toString() == "logo") {
            channelInfo.setLogoFileName(m_reader.readElementText());
        } else if (m_reader.name().toString() == "label") {
            channelInfo.addLabel(m_reader.readElementText());
        } else {
            skipUnknownElement();
        }
    }
    
    m_listTVChannelInfos.append(channelInfo);
}

void TVChannelInfosStreamParser::skipUnknownElement()
{
    Q_ASSERT(m_reader.isStartElement());

    while (m_reader.readNextStartElement()) {
        skipUnknownElement();
    }
}