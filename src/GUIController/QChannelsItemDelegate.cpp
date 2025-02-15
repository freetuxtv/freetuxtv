//
// Created by ebeuque on 07/06/2021.
//

#include <QStyleOptionViewItem>

#include "Model/ChannelInfos.h"

#include "GUIModel/QChannelsListItem.h"

#include "QChannelsItemDelegate.h"

void QChannelsItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	/*
	if (index.data().canConvert<StarRating>()) {
		StarRating starRating = qvariant_cast<StarRating>(index.data());

		if (option.state & QStyle::State_Selected)
			painter->fillRect(option.rect, option.palette.highlight());

		starRating.paint(painter, option.rect, option.palette,
						 StarRating::EditMode::ReadOnly);
	} else {
		QStyledItemDelegate::paint(painter, option, index);
	}*/
	QStyledItemDelegate::paint(painter, option, index);
}

QWidget *QChannelsItemDelegate::createEditor(QWidget *parent,
									const QStyleOptionViewItem &option,
									const QModelIndex &index) const

{
	/*
	if (index.data().canConvert<StarRating>()) {
		StarEditor *editor = new StarEditor(parent);
		connect(editor, &StarEditor::editingFinished,
				this, &StarDelegate::commitAndCloseEditor);
		return editor;
	}*/
	return QStyledItemDelegate::createEditor(parent, option, index);
}

void QChannelsItemDelegate::commitAndCloseEditor()
{
	//StarEditor *editor = qobject_cast<StarEditor *>(sender());
	//emit commitData(editor);
	//emit closeEditor(editor);
}

void QChannelsItemDelegate::setEditorData(QWidget *editor,
								 const QModelIndex &index) const
{
	/*
	if (index.data().canConvert<StarRating>()) {
		StarRating starRating = qvariant_cast<StarRating>(index.data());
		StarEditor *starEditor = qobject_cast<StarEditor *>(editor);
		starEditor->setStarRating(starRating);
	} else {
		QStyledItemDelegate::setEditorData(editor, index);
	}
	 */
	QStyledItemDelegate::setEditorData(editor, index);
}

void QChannelsItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
								const QModelIndex &index) const
{
	/*
	if (index.data().canConvert<StarRating>()) {
		StarEditor *starEditor = qobject_cast<StarEditor *>(editor);
		model->setData(index, QVariant::fromValue(starEditor->starRating()));
	} else {
		QStyledItemDelegate::setModelData(editor, model, index);
	}
	 */
	QStyledItemDelegate::setModelData(editor, model, index);
}

QSize QChannelsItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	QStandardItemModel *model = (QStandardItemModel *) index.model();
	QChannelsListItem *pItem = (QChannelsListItem*)model->itemFromIndex(index);

	int coefHeight = 2;
	if(pItem->m_pChannelInfos){
		coefHeight = 1;
	}

	/*
	if (index.data().canConvert<QChannelsListItem>()) {
		QChannelsListItem starRating = qvariant_cast<QChannelsListItem>(index.data());
		return starRating.sizeHint();
	}
*/
	QSize size = QStyledItemDelegate::sizeHint(option, index);
	size.setHeight(size.height() * coefHeight);

	return size;
}

void QChannelsItemDelegate::initStyleOption(QStyleOptionViewItem *option, const QModelIndex &index) const
{
	QStyledItemDelegate::initStyleOption(option, index);

	QStandardItemModel *model = (QStandardItemModel *) index.model();
	QChannelsListItem *pItem = (QChannelsListItem*)model->itemFromIndex(index);

	// Display playing item in bold
	option->font.setBold(pItem->isPlaying());
	option->font.setItalic(pItem->isPlaying());

	if(pItem->m_pChannelInfos) {
		option->rect.setLeft(0);
	}
}