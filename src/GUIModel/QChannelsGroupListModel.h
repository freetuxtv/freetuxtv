//
// Created by ebeuque on 15/10/2020.
//

#ifndef FREETUXTV_QCHANNELSGROUPLISTMODEL_H
#define FREETUXTV_QCHANNELSGROUPLISTMODEL_H

#include <QAbstractItemModel>
#include <QList>

class QChannelsGroupItem;

class QChannelsGroupListModel : public QAbstractItemModel
{
public:
	QChannelsGroupListModel();
	virtual ~QChannelsGroupListModel();

	void appendRow(QChannelsGroupItem* pItem);
	void clear();

protected:
	QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
	QModelIndex parent(const QModelIndex &child) const override;
	int rowCount(const QModelIndex &parent) const override;
	int columnCount(const QModelIndex &parent) const override;
	QVariant data(const QModelIndex &index, int role) const override;

private:
	QChannelsGroupItem* m_pRootItem;
};


#endif //FREETUXTV_QCHANNELSGROUPLISTMODEL_H
