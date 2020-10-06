//
// Created by ebeuque on 06/10/2020.
//

#include "QPreferencesDialogController.h"

QPreferencesDialogController::QPreferencesDialogController()
{
	m_pPreferencesDialog = NULL;
}

QPreferencesDialogController::~QPreferencesDialogController()
{

}

void QPreferencesDialogController::init(QPreferencesDialog* pPreferencesDialog)
{
	m_pPreferencesDialog = pPreferencesDialog;
}

void QPreferencesDialogController::dispose()
{

}