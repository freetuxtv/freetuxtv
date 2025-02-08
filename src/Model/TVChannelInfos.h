//
// Created by ebeuque on 08/02/25.
//

#ifndef FREETUXTV_MODEL_TVCHANNELINFOS_H
#define FREETUXTV_MODEL_TVCHANNELINFOS_H

#include <QString>
#include <QStringList>

class TVChannelInfos
{
public:
	TVChannelInfos();
	virtual ~TVChannelInfos();

	void setId(int iId);
	int getId() const;

	void setName(const QString& szName);
	const QString& getName() const;

	void setLogoFileName(const QString& szName);
	const QString& getLogoFileName() const;

	void addLabel(const QString& szLabel);
	const QStringList& getLabels() const;

private:
	int m_iId;
	QString m_szName;
	QString m_szLogoFileName;
	QStringList m_listLabels;
};

#endif //FREETUXTV_MODEL_TVCHANNELINFOS_H
