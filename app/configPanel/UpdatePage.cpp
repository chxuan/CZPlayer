#include "StdAfx.h"
#include "UpdatePage.h"

UpdatePage::UpdatePage(QWidget *parent) : m_parent(parent)
{
	QGroupBox *updateGB = new QGroupBox(tr("软件更新"), this);

	updateLabel = new QLabel(this);
	updateLabel->setText(tr("您现在使用的版本是V2.0 更新时间:2014年07月18日"));

	updateButton = new QPushButton(this);
	updateButton->setText(tr("手动检查最新版本"));

	QHBoxLayout *hLayout = new QHBoxLayout;
	hLayout->addWidget(updateLabel);
	hLayout->addStretch();

	QHBoxLayout *hLayout2 = new QHBoxLayout;
	hLayout2->addWidget(updateButton);
	hLayout2->addStretch();

	QVBoxLayout *vLayout = new QVBoxLayout;
	//vLayout->addWidget(updateLabel);
	//vLayout->addWidget(updateButton);
	vLayout->addLayout(hLayout);
	vLayout->addLayout(hLayout2);
	updateGB->setLayout(vLayout);

	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addWidget(updateGB);
	mainLayout->addStretch();
	this->setLayout(mainLayout);
}


UpdatePage::~UpdatePage(void)
{
}
