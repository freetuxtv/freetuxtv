//
// Created by ebeuque on 20/10/2020.
//

#ifndef FREETUXTV_CHANNELSGROUPSPARSERHANDLER_H
#define FREETUXTV_CHANNELSGROUPSPARSERHANDLER_H

#include <QList>
#include <QXmlDefaultHandler>

#include "Model/ChannelsGroupSection.h"

class ChannelsGroupsParserHandler : public QXmlDefaultHandler
{
public:
	ChannelsGroupsParserHandler(QList<ChannelsGroupSection>& listChannelsGroupSection);
	virtual ~ChannelsGroupsParserHandler();

protected:
	virtual bool startElement(const QString &namespaceURI,
							  const QString &localName,
							  const QString &qName,
							  const QXmlAttributes &attributes);
	virtual bool endElement(const QString &namespaceURI,
							const QString &localName,
							const QString &qName);
	virtual bool characters(const QString &str);
	virtual bool fatalError(const QXmlParseException &exception);

private:
	ChannelsGroupSection* m_pCurrentChannelsGroupSection;

	QList<ChannelsGroupSection>& m_listChannelsGroupSection;
};


#endif //FREETUXTV_CHANNELSGROUPSPARSERHANDLER_H
