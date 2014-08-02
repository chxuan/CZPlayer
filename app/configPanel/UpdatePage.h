#pragma once

#include <QtCore>
#include <QtGui>

class UpdatePage : public QWidget
{
	Q_OBJECT

public:
	UpdatePage(QWidget *parent = 0);
	~UpdatePage(void);

private:
	QWidget *m_parent;

	QLabel *updateLabel;
	QPushButton *updateButton;
};

