//
// Created by ebeuque on 10/10/2020.
//

#include <QPushButton>

#include "GUI/QAddChannelsGroupDialog.h"

#include "QAddChannelsGroupDialogController.h"

void QAddChannelsGroupDialogController::init(QAddChannelsGroupDialog* pAddChannelsGroupDialog)
{
	m_pAddChannelsGroupDialog = pAddChannelsGroupDialog;

	connect(m_pAddChannelsGroupDialog->getButtonCancel(), SIGNAL(clicked()), m_pAddChannelsGroupDialog, SLOT(reject()));
	connect(m_pAddChannelsGroupDialog->getButtonValid(), SIGNAL(clicked()), this, SLOT(onValidButtonClicked()));
}

void QAddChannelsGroupDialogController::dispose()
{

}

void QAddChannelsGroupDialogController::onValidButtonClicked()
{

};