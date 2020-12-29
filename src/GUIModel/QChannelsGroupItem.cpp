//
// Created by ebeuque on 15/10/2020.
//

#include "Model/ChannelsGroupInfos.h"

#include "QChannelsGroupItem.h"

QChannelsGroupItem::QChannelsGroupItem()
{
	m_pParent = NULL;
	m_pChannelsGroupInfos = NULL;
}

QChannelsGroupItem::QChannelsGroupItem(const ChannelsGroupSection& channelsGroupSection)
{
	m_pParent = NULL;
	m_channelsGroupSection = channelsGroupSection;
}

QChannelsGroupItem::QChannelsGroupItem(const ChannelsGroupInfos& channelsGroupInfos)
{
	m_pParent = NULL;
	m_pChannelsGroupInfos = new ChannelsGroupInfos(channelsGroupInfos);
}

QChannelsGroupItem::~QChannelsGroupItem()
{
	if(m_pChannelsGroupInfos){
		delete m_pChannelsGroupInfos;
		m_pChannelsGroupInfos = NULL;
	}
}

const ChannelsGroupSection& QChannelsGroupItem::getChannelsGroupSection() const
{
	return m_channelsGroupSection;
}

const ChannelsGroupInfos* QChannelsGroupItem::getChannelsGroupInfos() const
{
	return m_pChannelsGroupInfos;
}

void QChannelsGroupItem::setParent(QChannelsGroupItem* parent)
{
	m_pParent = parent;
}

QChannelsGroupItem* QChannelsGroupItem::parent()
{
	return m_pParent;
}

void QChannelsGroupItem::appendRow(QChannelsGroupItem* pChannelsGroupItem)
{
	pChannelsGroupItem->setParent(this);
	return m_listSubItems.append(pChannelsGroupItem);
}

int QChannelsGroupItem::row() const
{
	return m_pParent->findIndex(this);
}

QChannelsGroupItem* QChannelsGroupItem::itemAt(int i) const
{
	return m_listSubItems.at(i);
}

int QChannelsGroupItem::findIndex(const QChannelsGroupItem* pItem) const
{
	int i = -1;
	QList<QChannelsGroupItem *>::const_iterator iter;
	for(iter = m_listSubItems.constBegin(); iter != m_listSubItems.constEnd(); ++iter)
	{
		i++;
		QChannelsGroupItem* pCurItem = (*iter);
		if(pItem == pCurItem) {
			return i;
		}
	}
	return -1;
}

int QChannelsGroupItem::count() const
{
	return m_listSubItems.count();
}

void QChannelsGroupItem::clear()
{
	QList<QChannelsGroupItem *>::iterator iter;
	iter = m_listSubItems.begin();
	while(iter != m_listSubItems.end())
	{
		QChannelsGroupItem* pItem = (*iter);
		pItem->clear();
		iter = m_listSubItems.erase(iter);
	}
}