//
// Created by ebeuque on 15/10/2020.
//

#include "Model/ChannelsGroupInfos.h"
#include "Model/ChannelsGroupSection.h"

#include "GUIModel/QChannelsGroupItem.h"

#include "QChannelsGroupListModel.h"

QChannelsGroupListModel::QChannelsGroupListModel()
{
	m_pRootItem = new QChannelsGroupItem();
}

QChannelsGroupListModel::~QChannelsGroupListModel()
{
	if(m_pRootItem){
		delete m_pRootItem;
		m_pRootItem = NULL;
	}
}

QModelIndex QChannelsGroupListModel::index(int row, int column, const QModelIndex &parent) const
{
	QModelIndex retIndex;

	if (!hasIndex(row, column, parent)) {
		return QModelIndex();
	}

	QChannelsGroupItem *pParentItem;
	if (!parent.isValid()) {
		pParentItem = m_pRootItem;
	}else {
		pParentItem = static_cast<QChannelsGroupItem*>(parent.internalPointer());
	}

	QChannelsGroupItem *pItem = pParentItem->itemAt(row);
	if (pItem) {
		retIndex = createIndex(row, column, pItem);
	}else {
		retIndex = QModelIndex();
	}

	return retIndex;
}

QModelIndex QChannelsGroupListModel::parent(const QModelIndex &child) const
{
	if (!child.isValid()) {
		return QModelIndex();
	}

	QChannelsGroupItem *pItem = static_cast<QChannelsGroupItem *>(child.internalPointer());
	QChannelsGroupItem *pParentItem = pItem->parent();

	QModelIndex parentIndex;
	if (!pParentItem || pParentItem == m_pRootItem){
		parentIndex = QModelIndex();
	}else{
		parentIndex = createIndex(pParentItem->row(), 0, pParentItem);
	}

	return parentIndex;
}

int QChannelsGroupListModel::rowCount(const QModelIndex &parent) const
{
	int iRowCount;
	QChannelsGroupItem *pParentItem = static_cast<QChannelsGroupItem*>(parent.internalPointer());
	if(pParentItem) {
		iRowCount = pParentItem->count();
	}else {
		iRowCount = m_pRootItem->count();
	}
	return iRowCount;
}

int QChannelsGroupListModel::columnCount(const QModelIndex &parent) const
{
	return 3;
}

QVariant QChannelsGroupListModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid()){
		return QVariant();
	}

	if (role != Qt::DisplayRole) {
		return QVariant();
	}

	QChannelsGroupItem* pItem = static_cast<QChannelsGroupItem*>(index.internalPointer());
	if(index.parent() == QModelIndex()) {
		if(index.column() == 0) {
			return pItem->getChannelsGroupSection().getLang();
		}
	}else {
		if(index.column() == 0) {
			return pItem->getChannelsGroupInfos()->getName();
		}
		if(index.column() == 1) {
			return pItem->getChannelsGroupInfos()->getRequiredISP();
		}
		if(index.column() == 2) {
			return pItem->getChannelsGroupInfos()->getURI();
		}
	}

	return QVariant();
}

void QChannelsGroupListModel::appendRow(QChannelsGroupItem* pSection)
{
	m_pRootItem->appendRow(pSection);
}

void QChannelsGroupListModel::clear()
{
	m_pRootItem->clear();
}