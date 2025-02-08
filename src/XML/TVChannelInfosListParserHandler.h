//
// Created by ebeuque on 08/02/25.
//

#ifndef FREETUXTV_XML_TVCHANNELINFOSLISTPARSERHANDLER_H
#define FREETUXTV_XML_TVCHANNELINFOSLISTPARSERHANDLER_H

#include <QSharedPointer>
#include <QXmlDefaultHandler>

#include "Global/QError.h"

#include "Model/TVChannelInfos.h"

class TVChannelsInfosListParserUpdater
{
public:
	virtual void onProgress(const QString& szTVChannelName) = 0;
	virtual bool finalize(const QSharedPointer<TVChannelInfos>& pTVChannelInfos, QError& error) = 0;
};

class TVChannelInfosListParserHandler : public QXmlDefaultHandler
{
public:
	TVChannelInfosListParserHandler(QError& error);
	virtual ~TVChannelInfosListParserHandler();

	void setUpdater(TVChannelsInfosListParserUpdater* pUpdater);

protected:
	virtual bool startElement(const QString &namespaceURI,
							  const QString &localName,
							  const QString &qName,
							  const QXmlAttributes &attributes);
	virtual bool endElement(const QString &namespaceURI,
							const QString &localName,
							const QString &qName);
	virtual bool characters(const QString &str);
	//virtual bool fatalError(const QXmlParseException &exception);

private:
	QError& m_error;
	TVChannelsInfosListParserUpdater* m_pUpdater;

	QString m_szCurrentElementName;

	QSharedPointer<TVChannelInfos> m_pCurrentTVChannelInfos;
};

#endif //FREETUXTV_XML_TVCHANNELINFOSLISTPARSERHANDLER_H
