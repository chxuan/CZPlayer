#pragma once

#include "head.h"
#include "AboutPage.h"
#include "AuthorPage.h"
#include "ThanksPage.h"

class AboutPanel : public QDialog
{
	Q_OBJECT

public:
	AboutPanel(void);
	~AboutPanel(void);

public:
	static QTabWidget *tabWidget;

private slots:
	void slot_CloseAboutPanel();

private:
	QPushButton *okButton;

	QLabel *logoLabel;

	AboutPage *aboutPage;
	AuthorPage *authorPage;
	ThanksPage *thanksPage;
};

