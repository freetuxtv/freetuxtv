//
// Created by ebeuque on 20/10/2020.
//

#include "ChannelsGroupsParserHandler.h"


ChannelsGroupsParserHandler::ChannelsGroupsParserHandler(QList<ChannelsGroupSection>& listChannelsGroupSection)
	: m_listChannelsGroupSection(listChannelsGroupSection)
{
	m_pCurrentChannelsGroupSection = NULL;
}

ChannelsGroupsParserHandler::~ChannelsGroupsParserHandler()
{
	if(m_pCurrentChannelsGroupSection){
		delete m_pCurrentChannelsGroupSection;
		m_pCurrentChannelsGroupSection = NULL;
	}
}

bool ChannelsGroupsParserHandler::startElement(const QString &namespaceURI,
											   const QString &localName,
											   const QString &qName,
											   const QXmlAttributes &attributes)
{
	QString szTmp;

	if(qName == "language"){
		m_pCurrentChannelsGroupSection = new ChannelsGroupSection();

		szTmp = attributes.value("id");
		m_pCurrentChannelsGroupSection->setId(szTmp);

		szTmp = attributes.value("lang");
		m_pCurrentChannelsGroupSection->setLang(QObject::tr(qPrintable(szTmp)));
	}

	if(qName == "channels_group"){
		ChannelsGroupInfos channelsGroupInfos;

		szTmp = attributes.value("name");
		channelsGroupInfos.setName(QObject::tr(qPrintable(szTmp)));

		szTmp = attributes.value("uri");
		channelsGroupInfos.setURI(szTmp);

		szTmp = attributes.value("required_isp");
		channelsGroupInfos.setRequiredISP(szTmp);

		szTmp = attributes.value("bregex");
		channelsGroupInfos.setBRegex(szTmp);

		szTmp = attributes.value("eregex");
		channelsGroupInfos.setERegex(szTmp);

		m_pCurrentChannelsGroupSection->addChannelsGroup(channelsGroupInfos);
	}

	return true;
}

bool ChannelsGroupsParserHandler::endElement(const QString &namespaceURI,
											 const QString &localName,
											 const QString &qName)
{
	if(qName == "language") {
		if (m_pCurrentChannelsGroupSection) {
			m_listChannelsGroupSection.append(*m_pCurrentChannelsGroupSection);
			delete m_pCurrentChannelsGroupSection;
			m_pCurrentChannelsGroupSection = NULL;
		}
	}

	return true;
}

bool ChannelsGroupsParserHandler::characters(const QString &str)
{
	return true;
}

bool ChannelsGroupsParserHandler::fatalError(const QXmlParseException &exception)
{
	qCritical("[XML] Failed to parse XML file");
	return false;
}

