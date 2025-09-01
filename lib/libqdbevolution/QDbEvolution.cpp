/*
 * QDbEvolution.cpp
 *
 *  Created on: 30 juil. 2012
 *      Author: ebeuque
 */

#include <QFile>

//#include <Toolkit/QtCore/QtCompat.h>

#ifdef USE_NO_QREGEXP
#include <QRegularExpression>
#else
#include <QRegExp>
#endif

#include "QDbEvolution.h"


QDbEvolution::QDbEvolution(const QString& szScriptFilename)
{
	m_szScriptFilename = szScriptFilename;
}

QDbEvolution::~QDbEvolution()
{

}

bool QDbEvolution::hasVersion()
{
	QString szVersion;
	return getCurrentDbVersion(szVersion);
}

bool QDbEvolution::doCreation()
{
	bool bGoOn = true;

    QString szCurrentVersion;
    QString szUpdateValue;
    QString szTmpVersion;
    qInfo("[DbEvolution] Starting database creation");

    // Open the script file
    QFile scriptFile(m_szScriptFilename);
    if(!scriptFile.open(QIODevice::ReadOnly | QIODevice::Text)){
		// file couldn't be opened
		qCritical("[DbEvolution] File could not be opened");
		return false;
    }

    bool bProcess = false;
    bool bEndOfVersion = false;
    QString szQuery;

	while (!scriptFile.atEnd() && bGoOn) {

		QByteArray line = scriptFile.readLine();
		if(line.startsWith(DBEVOLUTION_DBVERSION_TAGS)){
			if(!szTmpVersion.isEmpty()){
				bEndOfVersion = true;
			}
			szTmpVersion = line.mid((int)strlen(DBEVOLUTION_DBVERSION_TAGS));
#ifdef USE_NO_QREGEXP
			szTmpVersion.remove(QRegularExpression("[\r\n]"));
#else
			szTmpVersion.remove(QRegExp("[\r\n]"));
#endif
			bProcess = true;
			if(szCurrentVersion.isEmpty()){
				szCurrentVersion = szTmpVersion;
			}
		}else if(line.startsWith(DBEVOLUTION_SEPARATOR_TAGS)){
			// Just tell to process the last query
			bProcess = true;
		}else if(line.startsWith(DBEVOLUTION_LASTDBVERSION_TAGS)){
			bProcess = false;
		}else if(line.startsWith(DBEVOLUTION_DBUPDATE_TAGS)){
			szUpdateValue = line.mid((int)strlen(DBEVOLUTION_DBUPDATE_TAGS));
#ifdef USE_NO_QREGEXP
			szUpdateValue.remove(QRegularExpression("[\r\n]"));
#else
			szUpdateValue.remove(QRegExp("[\r\n]"));
#endif
			bProcess = true;
		}else if(line.startsWith("--") || line.trimmed().isEmpty()){
			bProcess = false;
		}else{
			szQuery += line;
			bProcess = false;
		}

		if(scriptFile.atEnd()){
			bProcess = true;
			bEndOfVersion = true;
		}

		if(bProcess && szQuery != ""){
			if(!execQuery(szQuery)){
				qCritical("[DbEvolution] Cannot execute query %s", qPrintable(szQuery));
				bGoOn = false;
			}
			szQuery = "";
			bProcess = false;
		}

		if(bGoOn && szUpdateValue != "") {
			if(!handleDbUpdate(szUpdateValue)){
				qCritical("[DbEvolution] Cannot execute update version %s", qPrintable(szUpdateValue));
				bGoOn = false;
			}
			szUpdateValue = "";
		}

		if(bGoOn && bEndOfVersion){
			qInfo("[DbEvolution] Database version %s installed", qPrintable(szCurrentVersion));
			setCurrentDbVersion(szCurrentVersion);
			szCurrentVersion = szTmpVersion;
			bEndOfVersion = false;
		}
	}

    return bGoOn;
}

bool QDbEvolution::doEvolution()
{
	bool bGoOn = true;

    QString szInitialVersion;
    QString szCurrentVersion;
    QString szNextVersion;
    QString szUpdateValue;
	qInfo("[DbEvolution] Starting database creation");

    // Open the script file
    QFile scriptFile(m_szScriptFilename);
    if(!scriptFile.open(QIODevice::ReadOnly | QIODevice::Text)){
		// file couldn't be opened
		qCritical("[DbEvolution] File could not be opened");
		return false;
    }

    bool bProcess = false;
    bool bEndOfVersion = false;
    bool bInstallCurrentVersion = false;
    bool bInstallNextVersion = false;
    QString szQuery;

    bGoOn = getCurrentDbVersion(szInitialVersion);
    if(bGoOn){
		qInfo("[DbEvolution] Current database version is %s", qPrintable(szInitialVersion));
    }else{
		qCritical("[DbEvolution] Cannot get version");
    }

	while (!scriptFile.atEnd() && bGoOn) {

		QByteArray line = scriptFile.readLine();
		if(line.startsWith(DBEVOLUTION_DBVERSION_TAGS)){
			if(!szNextVersion.isEmpty()){
				bEndOfVersion = true;
			}
			szNextVersion = line.mid((int)strlen(DBEVOLUTION_DBVERSION_TAGS));
#ifdef USE_NO_QREGEXP
			szNextVersion.remove(QRegularExpression("[\r\n]"));
#else
			szNextVersion.remove(QRegExp("[\r\n]"));
#endif
			bProcess = true;
			if(compareDbVersion(szInitialVersion, szNextVersion) < 0){
				bInstallNextVersion = true;
			}
			if(szCurrentVersion.isEmpty()){
				szCurrentVersion = szNextVersion;
				bInstallCurrentVersion = bInstallNextVersion;
			}
		}else if(line.startsWith(DBEVOLUTION_SEPARATOR_TAGS)){
			// Just tell to process the last query
			bProcess = true;
		}else if(line.startsWith(DBEVOLUTION_LASTDBVERSION_TAGS)){
			bProcess = false;
		}else if(line.startsWith(DBEVOLUTION_DBUPDATE_TAGS)){
			szUpdateValue = line.mid((int)strlen(DBEVOLUTION_DBUPDATE_TAGS));
#ifdef USE_NO_QREGEXP
			szUpdateValue.remove(QRegularExpression("[\r\n]"));
#else
			szUpdateValue.remove(QRegExp("[\r\n]"));
#endif
			bProcess = true;
		}else if(line.startsWith("--") || line.trimmed().isEmpty()){
			bProcess = false;
		}else{
			szQuery += line;
			bProcess = false;
		}

		if(scriptFile.atEnd()){
			bProcess = true;
			bEndOfVersion = true;
		}

		if(bProcess && szQuery != ""){
			if(bInstallCurrentVersion){
				notifyDbEvolution(szCurrentVersion);
				if(!execQuery(szQuery)){
					qCritical("[DbEvolution] Cannot execute query %s", qPrintable(szQuery));
					bGoOn = false;
				}
			}
			szQuery = "";
			bProcess = false;
		}

		if(bGoOn && szUpdateValue != "") {
			if(!handleDbUpdate(szUpdateValue)){
				qCritical("[DbEvolution] Cannot execute update version %s", qPrintable(szUpdateValue));
				bGoOn = false;
			}
			szUpdateValue = "";
		}

		if(bGoOn && bEndOfVersion){
			if(bInstallCurrentVersion){
				bGoOn = setCurrentDbVersion(szCurrentVersion);
				if(bGoOn){
					qInfo("[DbEvolution] Installed database version %s", qPrintable(szCurrentVersion));
				}
			}
			szCurrentVersion = szNextVersion;
			bInstallCurrentVersion = bInstallNextVersion;
			szQuery = "";
			bEndOfVersion = false;
		}
	}

    return bGoOn;
}
