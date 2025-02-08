//
// Created by ebeuque on 08/02/25.
//

#ifndef FREETUXTV_MODEL_APPLICATIONCONTROLLER_H
#define FREETUXTV_MODEL_APPLICATIONCONTROLLER_H

#include <QString>

class ApplicationController {
public:
	static void pushStatusBarMessage(const QString& szContext, const QString& szMessage);
	static void popStatusBarMessage(const QString& szContext);
};


#endif //FREETUXTV_MODEL_APPLICATIONCONTROLLER_H
