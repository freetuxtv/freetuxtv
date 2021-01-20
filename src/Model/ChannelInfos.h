//
// Created by ebeuque on 20/01/2021.
//

#ifndef FREETUXTV_CHANNELINFOS_H
#define FREETUXTV_CHANNELINFOS_H

#include <QStringList>

class ChannelInfos
{
public:
	ChannelInfos();
	virtual ~ChannelInfos();

public:
	void setId(int iId);
	int getId() const;

	void setPosition(int iPosition);
	int getPosition() const;

	void setName(const QString& szName);
	const QString& getName() const;

private:
	int m_id;
	int m_iPosition;
	QString m_szName;
	QString m_szUrl;

	QString m_szLogoName;

	QStringList listVLCOptions;

	QString m_szDeinterlaceMode;
};


#endif //FREETUXTV_CHANNELINFOS_H
