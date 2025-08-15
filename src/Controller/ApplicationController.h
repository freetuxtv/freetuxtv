//
// Created by ebeuque on 08/02/25.
//

#ifndef FREETUXTV_MODEL_APPLICATIONCONTROLLER_H
#define FREETUXTV_MODEL_APPLICATIONCONTROLLER_H

#include <QString>
#include <QStack>
#include <QMap>

class QStatusBar;

class ApplicationController {
public:
	static void setStatusBar(QStatusBar* statusBar);
	static void pushStatusBarMessage(const QString& szContext, const QString& szMessage);
	static void popStatusBarMessage(const QString& szContext);

private:
	static QStatusBar* s_statusBar;
	static QMap<QString, QStack<QString>> s_messageStacks;
	static void updateStatusBar();
};


#endif //FREETUXTV_MODEL_APPLICATIONCONTROLLER_H
