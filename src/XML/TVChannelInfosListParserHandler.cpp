//
// Created by ebeuque on 08/02/25.
//

#include "TVChannelInfosListParserHandler.h"

TVChannelInfosListParserHandler::TVChannelInfosListParserHandler(QError& error) : m_error(error)
{
	m_pUpdater = nullptr;
}

TVChannelInfosListParserHandler::~TVChannelInfosListParserHandler()
{

}

void TVChannelInfosListParserHandler::setUpdater(TVChannelsInfosListParserUpdater* pUpdater)
{
	m_pUpdater = pUpdater;
}

bool TVChannelInfosListParserHandler::startElement(const QString &namespaceURI, const QString &localName, const QString &qName, const QXmlAttributes &attributes)
{
	QString szName;

	m_szCurrentElementName = qName;

	if(qName == "tvchannel"){
		m_pCurrentTVChannelInfos = QSharedPointer<TVChannelInfos>(new TVChannelInfos());
		szName = attributes.value(0);
		m_pCurrentTVChannelInfos->setName(szName);

		if(m_pUpdater){
			m_pUpdater->onProgress(szName);
		}
	}

	if(!szName.isEmpty()){
		qDebug("[Parser] Add TV channel '%s' in database", qPrintable(szName));
	}
}

bool TVChannelInfosListParserHandler::endElement(const QString &namespaceURI, const QString &localName, const QString &qName)
{
	bool bRes = true;

	QString szUserImgChannelsDir;

	if(qName == "tvchannel"){
		if(m_pCurrentTVChannelInfos){
			if(m_pUpdater){
				bRes = m_pUpdater->finalize(m_pCurrentTVChannelInfos, m_error);
			}

			m_pCurrentTVChannelInfos.clear();
		}
	}

	return bRes;
}

bool TVChannelInfosListParserHandler::characters(const QString &str)
{
	// Add the logo filename
	if(m_szCurrentElementName == "logo_filename"){
		m_pCurrentTVChannelInfos->setLogoFileName(str);
	}

	// Add a label for the channel
	if(m_szCurrentElementName == "label"){
		m_pCurrentTVChannelInfos->addLabel(str);
	}
	return true;
}