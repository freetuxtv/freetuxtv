/*
 * SingletonManager.cpp
 *
 *  Created on: 9 janv. 2013
 *      Author: ebeuque
 */

#include "SingletonManager.h"

SingletonManager SingletonManager::g_instance;

SingletonManager::SingletonManager()
{

}

SingletonManager::~SingletonManager()
{
	execDisposeCallback();
}

SingletonManager& SingletonManager::getInstance()
{
	return g_instance;
}

void SingletonManager::addDisposeCallback(DisposeSingletonFunc func)
{
	g_instance.m_stackDisposeFunc.push(func);
}

void SingletonManager::execDisposeCallback()
{
	while(!g_instance.m_stackDisposeFunc.empty()){
		DisposeSingletonFunc func = g_instance.m_stackDisposeFunc.top();
		func();
		g_instance.m_stackDisposeFunc.pop();
	}
}
