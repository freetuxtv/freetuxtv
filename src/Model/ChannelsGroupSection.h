//
// Created by ebeuque on 20/10/2020.
//

#ifndef FREETUXTV_CHANNELSGROUPSECTION_H
#define FREETUXTV_CHANNELSGROUPSECTION_H

#include <QString>
#include <QList>

#include "Model/ChannelsGroupInfos.h"

class ChannelsGroupSection
{
public:
	ChannelsGroupSection();
	virtual ~ChannelsGroupSection();

	void setId(const QString& szId);
	const QString& getId() const;

	void setLang(const QString& szLang);
	const QString& getLang() const;

	void addChannelsGroup(const ChannelsGroupInfos& channelsGroup);
	const QList<ChannelsGroupInfos>& getChannelsGroupList() const;

private:
	QString m_szId;
	QString m_szLang;

	QList<ChannelsGroupInfos> m_listChannelsGroup;
};


#endif //FREETUXTV_CHANNELSGROUPSECTION_H
