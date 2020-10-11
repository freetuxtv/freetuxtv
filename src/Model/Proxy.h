//
// Created by ebeuque on 11/10/2020.
//

#ifndef FREETUXTV_PROXY_H
#define FREETUXTV_PROXY_H

#include <QString>

class Proxy {
public:
	enum ProxyMode {
		ProxyModeNone = 0,
		ProxyModeManual     = 1
	};

public:
	Proxy();
	virtual ~Proxy();

public:
	int m_iProxyMode;
	QString m_szProxyServer;
	int m_iProxyPort;
	QString m_szProxyType;
	bool m_bProxyAuth;
	QString m_szProxyUser;
	QString m_szProxyPassword;
};

#endif //FREETUXTV_PROXY_H
