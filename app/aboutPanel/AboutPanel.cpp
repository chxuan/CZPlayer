#include "StdAfx.h"
#include "AboutPanel.h"

QTabWidget* AboutPanel::tabWidget = 0;

AboutPanel::AboutPanel(void)
{
	//设置窗口基本属性
	this->resize(530, 535);//设置窗体大小
	this->setMinimumSize(530, 535);
	this->setWindowTitle(tr("关于CZPlayer"));

	logoLabel = new QLabel;
	logoLabel->setPixmap(QPixmap(tr(":/images/CZPlayerWhite.png")));

	//关于页面
	aboutPage = new AboutPage;

	//作者页面
	authorPage = new AuthorPage;

	//感谢页面
	thanksPage = new ThanksPage;

	tabWidget = new QTabWidget;
	tabWidget->addTab(aboutPage, tr("关于"));
	tabWidget->addTab(authorPage, tr("作者"));
	tabWidget->addTab(thanksPage, tr("感谢"));

	okButton = new QPushButton;
	okButton->setText(tr("确定"));

	QHBoxLayout *bottomLayout = new QHBoxLayout;
	bottomLayout->addStretch();
	bottomLayout->addWidget(okButton);

	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addWidget(logoLabel);
	mainLayout->addWidget(tabWidget);
	mainLayout->addLayout(bottomLayout);
	this->setLayout(mainLayout);

	connect(okButton, SIGNAL(clicked()), this, SLOT(slot_CloseAboutPanel()));
}


AboutPanel::~AboutPanel(void)
{
}

void AboutPanel::slot_CloseAboutPanel()
{
	this->close();
}
