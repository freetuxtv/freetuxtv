/*
 * SingletonManager.h
 *
 *  Created on: 9 janv. 2013
 *      Author: ebeuque
 */

#ifndef SINGLETONMANAGER_H_
#define SINGLETONMANAGER_H_

#include <stack>

typedef void (*DisposeSingletonFunc)();

class SingletonManager {
private:
	SingletonManager();
public:
	virtual ~SingletonManager();

	static SingletonManager& getInstance();

public:
	static void addDisposeCallback(DisposeSingletonFunc func);
	static void execDisposeCallback();

private:
	static SingletonManager g_instance;

	std::stack<DisposeSingletonFunc> m_stackDisposeFunc;
};

#endif /* SINGLETONMANAGER_H_ */
