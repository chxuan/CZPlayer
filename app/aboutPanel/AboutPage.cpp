#include "StdAfx.h"
#include "AboutPage.h"

AboutPage::AboutPage(void)
{
	strLabel = new QLabel;
	strLabel->setObjectName("strLabel");
	strLabel->setText(tr("基于Qt的多媒体播放器（CZPlayer）\n作品展:http://www.qtcn.org/bbs/read-htm-tid-55824.html\n源代码:https://github.com/chxuan/CZPlayer2.0.git"));

	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addWidget(strLabel);
	mainLayout->addStretch();
	this->setLayout(mainLayout);
}


AboutPage::~AboutPage(void)
{
}
