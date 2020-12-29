//
// Created by ebeuque on 15/10/2020.
//

#ifndef FREETUXTV_QCHANNELSGROUPITEM_H
#define FREETUXTV_QCHANNELSGROUPITEM_H

#include "Model/ChannelsGroupSection.h"

class ChannelsGroupInfos;

class QChannelsGroupItem
{
public:
	QChannelsGroupItem();
	QChannelsGroupItem(const ChannelsGroupSection& channelsGroupSection);
	QChannelsGroupItem(const ChannelsGroupInfos& channelsGroupItem);
	virtual ~QChannelsGroupItem();


	const ChannelsGroupSection& getChannelsGroupSection() const;
	const ChannelsGroupInfos* getChannelsGroupInfos() const;

	void setParent(QChannelsGroupItem* parent);
	QChannelsGroupItem* parent();

	void appendRow(QChannelsGroupItem* pChannelsGroupItem);
	int row() const;

	QChannelsGroupItem* itemAt(int i) const;
	int findIndex(const QChannelsGroupItem* pItem) const;

	int count() const;
	void clear();

private:
	QChannelsGroupItem* m_pParent;

	ChannelsGroupSection m_channelsGroupSection;
	ChannelsGroupInfos* m_pChannelsGroupInfos;

	QList<QChannelsGroupItem*> m_listSubItems;
};


#endif //FREETUXTV_QCHANNELSGROUPITEM_H
