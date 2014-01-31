#pragma once

#include "head.h"

class SearchList : public QTableWidget
{
	Q_OBJECT

public:
	SearchList(QWidget *widget = 0);
	~SearchList(void);

private:
	QWidget *m_parent;
};

