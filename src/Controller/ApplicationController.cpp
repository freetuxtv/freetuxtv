//
// Created by ebeuque on 08/02/25.
//

#include "ApplicationController.h"

#include <QStatusBar>

// Static member definitions
QStatusBar* ApplicationController::s_statusBar = nullptr;
QMap<QString, QStack<QString>> ApplicationController::s_messageStacks;

void ApplicationController::setStatusBar(QStatusBar* statusBar)
{
	s_statusBar = statusBar;
}

void ApplicationController::pushStatusBarMessage(const QString& szContext, const QString& szMessage)
{
	if (!s_statusBar) {
		return;
	}

	s_messageStacks[szContext].push(szMessage);
	updateStatusBar();
}

void ApplicationController::popStatusBarMessage(const QString& szContext)
{
	if (!s_statusBar) {
		return;
	}

	if (s_messageStacks.contains(szContext) && !s_messageStacks[szContext].isEmpty()) {
		s_messageStacks[szContext].pop();
		if (s_messageStacks[szContext].isEmpty()) {
			s_messageStacks.remove(szContext);
		}
	}
	updateStatusBar();
}

void ApplicationController::updateStatusBar()
{
	if (!s_statusBar) {
		return;
	}

	// Find the most recent message from any context
	QString currentMessage;
	for (auto it = s_messageStacks.begin(); it != s_messageStacks.end(); ++it) {
		if (!it.value().isEmpty()) {
			currentMessage = it.value().top();
			break; // Use the first non-empty stack found
		}
	}

	if (currentMessage.isEmpty()) {
		s_statusBar->clearMessage();
	} else {
		s_statusBar->showMessage(currentMessage);
	}
}