//
// Created by ebeuque on 11/10/2020.
//

#include <QNetworkReply>
#include <QNetworkProxy>
#include <QEventLoop>
#include <QTimer>

#include "QFileDownloader.h"

QFileDownloader::QFileDownloader(const QUrl& url, QObject *parent)
{
	m_url = url;
	m_iTimeoutMs = 0;
	m_pProxy = NULL;

	connect(&m_networkAccessManager, SIGNAL (finished(QNetworkReply*)),
			this, SLOT (fileDownloaded(QNetworkReply*))
	);
}

QFileDownloader::~QFileDownloader()
{

}

void QFileDownloader::setTimeoutMs(int iTimeoutMs)
{
	m_iTimeoutMs = iTimeoutMs;
}

void QFileDownloader::setProxy(Proxy* pProxy)
{
	m_pProxy = pProxy;
}

bool QFileDownloader::download()
{
	bool bRes = true;

	QNetworkRequest request(m_url);
	// Add support for redirect attribute
	request.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);

	// Define an event loop
	QEventLoop loop;
	connect(this, SIGNAL (downloaded()), &loop, SLOT (quit()));

	// Proxy
	if(m_pProxy && m_pProxy->m_iProxyMode == Proxy::ProxyModeManual)
	{
		QNetworkProxy networkProxy;
		networkProxy.setHostName(m_pProxy->m_szProxyServer);
		networkProxy.setPort(m_pProxy->m_iProxyPort);
		if(m_pProxy->m_szProxyType == "HTTP"){
			networkProxy.setType(QNetworkProxy::HttpProxy);
		}
		if(m_pProxy->m_szProxyType == "SOCK"){
			networkProxy.setType(QNetworkProxy::Socks5Proxy);
		}

		if(m_pProxy->m_bProxyAuth) {
			networkProxy.setUser(m_pProxy->m_szProxyUser);
			networkProxy.setPassword(m_pProxy->m_szProxyPassword);
		}
		m_networkAccessManager.setProxy(networkProxy);
	}

	// Start request
	QNetworkReply* pReply = m_networkAccessManager.get(request);

	// Set timeout
	QTimer timer;
	if(m_iTimeoutMs > 0) {
		timer.setSingleShot(true);
		connect(&timer, SIGNAL (timeout()), &loop, SLOT (quit()));
		timer.start(m_iTimeoutMs);
	}

	// Start event loop
	loop.exec();

	return bRes;
}

void QFileDownloader::fileDownloaded(QNetworkReply* pReply)
{
	m_downloadedData = pReply->readAll();
	//emit a signal
	pReply->deleteLater();
	emit downloaded();
}

QByteArray QFileDownloader::downloadedData() const
{
	return m_downloadedData;
}