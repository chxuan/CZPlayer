#pragma once

#include "head.h"

class AuthorPage : public QWidget
{
	Q_OBJECT

public:
	AuthorPage(void);
	~AuthorPage(void);

private:
	QLabel *mainAuthorLabel;
	QLabel *plugAuthorLabel;
	QLabel *artAuthorLabel;
};

