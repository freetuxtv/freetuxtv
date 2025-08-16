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

#include "ChannelsGroupsStreamParser.h"

#include <QDebug>

#include "Model/ChannelsGroupInfos.h"

ChannelsGroupsStreamParser::ChannelsGroupsStreamParser(QList<ChannelsGroupSection>& listChannelsGroupSection)
    : m_listChannelsGroupSection(listChannelsGroupSection)
    , m_pCurrentChannelsGroupSection(nullptr)
{
}

ChannelsGroupsStreamParser::~ChannelsGroupsStreamParser()
{
}

bool ChannelsGroupsStreamParser::parse(QIODevice *device)
{
    m_reader.setDevice(device);
    
    if (m_reader.readNextStartElement()) {
        if (m_reader.name() == "channelsgroups") {
            parseChannelsGroupsElement();
        } else {
            m_reader.raiseError(QObject::tr("The file is not a valid channels groups file."));
        }
    }
    
    return !m_reader.hasError();
}

bool ChannelsGroupsStreamParser::parse(const QByteArray &data)
{
    m_reader.addData(data);
    return parse(nullptr);
}

QString ChannelsGroupsStreamParser::errorString() const
{
    return m_reader.errorString();
}

void ChannelsGroupsStreamParser::parseChannelsGroupsElement()
{
    Q_ASSERT(m_reader.isStartElement() && m_reader.name() == "channelsgroups");

    while (m_reader.readNextStartElement()) {
        if (m_reader.name() == "channelsgroupsection") {
            parseChannelsGroupSectionElement();
        } else {
            skipUnknownElement();
        }
    }
}

void ChannelsGroupsStreamParser::parseChannelsGroupSectionElement()
{
    Q_ASSERT(m_reader.isStartElement() && m_reader.name() == "channelsgroupsection");

    ChannelsGroupSection section;
    
    // Read attributes
    QXmlStreamAttributes attributes = m_reader.attributes();
    if (attributes.hasAttribute("id")) {
        section.setId(attributes.value("id").toString());
    }
    if (attributes.hasAttribute("lang")) {
        section.setLang(attributes.value("lang").toString());
    }
    
    m_pCurrentChannelsGroupSection = &section;
    
    while (m_reader.readNextStartElement()) {
        if (m_reader.name() == "channelsgroup") {
            parseChannelsGroupElement();
        } else {
            skipUnknownElement();
        }
    }
    
    m_listChannelsGroupSection.append(section);
    m_pCurrentChannelsGroupSection = nullptr;
}

void ChannelsGroupsStreamParser::parseChannelsGroupElement()
{
    Q_ASSERT(m_reader.isStartElement() && m_reader.name() == "channelsgroup");

    if (!m_pCurrentChannelsGroupSection) {
        skipUnknownElement();
        return;
    }

    ChannelsGroupInfos channelsGroup;
    
    // Read attributes
    QXmlStreamAttributes attributes = m_reader.attributes();
    if (attributes.hasAttribute("name")) {
        channelsGroup.setName(attributes.value("name").toString());
    }
    if (attributes.hasAttribute("url")) {
        channelsGroup.setURI(attributes.value("url").toString());
    }
    if (attributes.hasAttribute("bregex")) {
        channelsGroup.setBRegex(attributes.value("bregex").toString());
    }
    if (attributes.hasAttribute("eregex")) {
        channelsGroup.setERegex(attributes.value("eregex").toString());
    }
    
    m_pCurrentChannelsGroupSection->addChannelsGroup(channelsGroup);
    
    // Skip any child elements
    m_reader.skipCurrentElement();
}

void ChannelsGroupsStreamParser::skipUnknownElement()
{
    Q_ASSERT(m_reader.isStartElement());

    while (m_reader.readNextStartElement()) {
        skipUnknownElement();
    }
}