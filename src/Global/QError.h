//
// Created by ebeuque on 10/10/2020.
//

#ifndef FREETUXTV_QERROR_H
#define FREETUXTV_QERROR_H

#include <QString>

class QError
{
public:
	QError();
	virtual ~QError();

	bool hasError() const;
	const QString& getErrorMsg() const;

	QError& operator= (const QString& szMsg);


private:
	QString m_szErrorMsg;
};


#endif //FREETUXTV_QERROR_H
