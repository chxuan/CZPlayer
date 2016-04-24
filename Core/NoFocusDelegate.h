#ifndef NOFOCUSDELEGATE_H
#define NOFOCUSDELEGATE_H

#include <QStyledItemDelegate>
#include <QModelIndex>
#include <QPainter>

class NoFocusDelegate : public QStyledItemDelegate
{
	Q_OBJECT

public:
	NoFocusDelegate(QStyledItemDelegate *parent = 0);
	~NoFocusDelegate();

protected:
	void paint(QPainter* painter, const QStyleOptionViewItem & option, const QModelIndex &index) const
	{
		QStyleOptionViewItem itemOption(option);
		if (itemOption.state & QStyle::State_HasFocus)
		{
			itemOption.state = itemOption.state ^ QStyle::State_HasFocus;
		}

		QStyledItemDelegate::paint(painter, itemOption, index);
	}

private:
    QStyledItemDelegate *m_parent;
};

#endif // NOFOCUSDELEGATE_H
