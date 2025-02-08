//
// Created by ebeuque on 07/06/2021.
//

#ifndef FREETUXTV_QCHANNELSITEMDELEGATE_H
#define FREETUXTV_QCHANNELSITEMDELEGATE_H

#include <QStyledItemDelegate>

class QChannelsItemDelegate : public QStyledItemDelegate
{
	Q_OBJECT
public:
	using QStyledItemDelegate::QStyledItemDelegate;

	void paint(QPainter *painter, const QStyleOptionViewItem &option,
			   const QModelIndex &index) const override;
	QSize sizeHint(const QStyleOptionViewItem &option,
				   const QModelIndex &index) const override;
	QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
						  const QModelIndex &index) const override;
	void setEditorData(QWidget *editor, const QModelIndex &index) const override;
	void setModelData(QWidget *editor, QAbstractItemModel *model,
					  const QModelIndex &index) const override;

private slots:
	void commitAndCloseEditor();
};


#endif //FREETUXTV_QCHANNELSITEMDELEGATE_H
