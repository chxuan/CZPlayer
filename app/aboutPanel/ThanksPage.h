#pragma once

#include "head.h"

class ThanksPage: public QWidget
{
	Q_OBJECT

public:
	ThanksPage(void);
	~ThanksPage(void);

	QLabel *thanksLabel;
};

