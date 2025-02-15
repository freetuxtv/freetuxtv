//
// Created by ebeuque on 20/01/2021.
//

#ifndef FREETUXTV_MODEL_CHANNELINFOS_H
#define FREETUXTV_MODEL_CHANNELINFOS_H

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

	void setUrl(const QString& szUrl);
	const QString& getUrl() const;

	void setVLCOptions(const QStringList& listOptions);
	const QStringList& getVLCOptions() const;

	void setDeinterlaceMode(const QString& szMode);
	const QString& getDeinterlaceMode() const;

	void setChannelsGroupsInfosId(int iId);
	int getChannelsGroupsInfosId() const;

	// Infos from TV Channels

	void setLogoFileName(const QString& szName);
	const QString& getLogoFileName() const;

private:
	int m_id;
	int m_iPosition;
	QString m_szName;
	QString m_szUrl;

	QStringList m_listVLCOptions;

	QString m_szDeinterlaceMode;

	int m_iChannelsGroupsInfosId;

	QString m_szLogoFileName;

	//QSharedPointer<ChannelsGroupInfos> m_pChannelsGroupInfos;
};

#endif //FREETUXTV_MODEL_CHANNELINFOS_H
