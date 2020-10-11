//
// Created by ebeuque on 10/10/2020.
//

#include "QError.h"

QError::QError()
{

}

QError::~QError()
{

}

bool QError::hasError() const
{
	return !m_szErrorMsg.isNull();
}

const QString& QError::getErrorMsg() const
{
	return m_szErrorMsg;
}

QError& QError::operator= (const QString& szMsg)
{
	m_szErrorMsg = szMsg;
	return *this;
}