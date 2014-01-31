#include "StdAfx.h"
#include "AboutPage.h"


AboutPage::AboutPage(void)
{
	strLabel = new QLabel;
	strLabel ->setObjectName("strLabel");
	strLabel ->setText(tr("基于Qt的多媒体播放器（CZPlayer）"));

	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout ->addWidget(strLabel);
	mainLayout ->addStretch();
	this ->setLayout(mainLayout);
}


AboutPage::~AboutPage(void)
{
}
