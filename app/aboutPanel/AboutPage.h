#pragma once

#include "head.h"

class AboutPage : public QWidget
{
	Q_OBJECT

public:
	AboutPage(void);
	~AboutPage(void);

	QLabel *strLabel;
};

