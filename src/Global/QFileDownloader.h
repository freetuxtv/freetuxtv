//
// Created by ebeuque on 11/10/2020.
//

#ifndef FREETUXTV_QFILEDOWNLOADER_H
#define FREETUXTV_QFILEDOWNLOADER_H

#include <QObject>

#include <QNetworkAccessManager>

#include "Model/Proxy.h"

class QNetworkReply;

class QFileDownloader : public QObject
{
	Q_OBJECT
public:
	QFileDownloader(const QUrl& url, QObject *parent = 0);
	virtual ~QFileDownloader();

	void setTimeoutMs(int iTimeoutMs);
	void setProxy(Proxy* pProxy);

public:
	bool download();
	QByteArray downloadedData() const;

signals:
	void downloaded();

private slots:
	void fileDownloaded(QNetworkReply* pReply);

private:
	QUrl m_url;
	int m_iTimeoutMs;
	Proxy *m_pProxy;

	QNetworkAccessManager m_networkAccessManager;
	QByteArray m_downloadedData;
};


#endif //FREETUXTV_QFILEDOWNLOADER_H
