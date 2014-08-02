#include "stdafx.h"
#include "generalsettingspage.h"

QCheckBox* GeneralSettingsPage::autoPlayCB = 0;
QCheckBox* GeneralSettingsPage::autoStartCB = 0;
QCheckBox* GeneralSettingsPage::helloCZPlayerCB = 0;
QCheckBox* GeneralSettingsPage::defaultPlayer = 0;

GeneralSettingsPage::GeneralSettingsPage(QWidget *parent) : m_parent(parent)
{
    QGroupBox *startGB = new QGroupBox(tr("启动时"), this);

    autoPlayCB = new QCheckBox(tr("自动播放歌曲"), this);
	autoPlayCB->setStyleSheet("QCheckBox{color:black;}"
		"QCheckBox::indicator{width:10px;height:10px;border: 1px solid black;border-radius:2px}"
		"QCheckBox::indicator:checked {image: url(:/images/checked2.png);}");

    autoStartCB = new QCheckBox(tr("开机时自动启动CZPlayer"), this);
	autoStartCB->setStyleSheet("QCheckBox{color:black;}"
		"QCheckBox::indicator{width:10px;height:10px;border: 1px solid black;border-radius:2px}"
		"QCheckBox::indicator:checked {image: url(:/images/checked2.png);}");

    helloCZPlayerCB = new QCheckBox(tr("播放“听音乐,找CZPlayer”问候语"), this);
	helloCZPlayerCB->setStyleSheet("QCheckBox{color:black;}"
		"QCheckBox::indicator{width:10px;height:10px;border: 1px solid black;border-radius:2px}"
		"QCheckBox::indicator:checked {image: url(:/images/checked2.png);}");

    QGroupBox *fileConnect = new QGroupBox(tr("文件关联"), this);

    defaultPlayer = new QCheckBox(tr("将CZPlayer作为默认播放器"), this);
	defaultPlayer->setStyleSheet("QCheckBox{color:black;}"
		"QCheckBox::indicator{width:10px;height:10px;border: 1px solid black;border-radius:2px}"
		"QCheckBox::indicator:checked {image: url(:/images/checked2.png);}");

    QGridLayout *startGBLayoutG = new QGridLayout;
    startGBLayoutG->addWidget(autoPlayCB, 0, 0);
    startGBLayoutG->addWidget(autoStartCB, 0, 1);
    startGBLayoutG->addWidget(helloCZPlayerCB, 1, 0);
    startGB->setLayout(startGBLayoutG);

    QVBoxLayout *fileLayout = new QVBoxLayout;
    fileLayout->addWidget(defaultPlayer);
    fileConnect->setLayout(fileLayout);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(startGB);
    mainLayout->addWidget(fileConnect);
    mainLayout->addStretch();
    this->setLayout(mainLayout);
}

GeneralSettingsPage::~GeneralSettingsPage()
{
}

Qt::CheckState GeneralSettingsPage::getAutoPlay()
{
	return autoPlayCB->checkState();
}

void GeneralSettingsPage::setAutoPlay( Qt::CheckState const &state )
{
	autoPlayCB->setCheckState(state);
}

Qt::CheckState GeneralSettingsPage::getAutoStart()
{
	return autoStartCB->checkState();
}

void GeneralSettingsPage::setAutoStart( Qt::CheckState const &state )
{
	autoStartCB->setCheckState(state);
}

Qt::CheckState GeneralSettingsPage::getGreetings()
{
	return helloCZPlayerCB->checkState();
}

void GeneralSettingsPage::setGreetings( Qt::CheckState const &state )
{
	helloCZPlayerCB->setCheckState(state);
}

Qt::CheckState GeneralSettingsPage::getDefaultPlayer()
{
	return defaultPlayer->checkState();
}

void GeneralSettingsPage::setDefaultPlayer( Qt::CheckState const &state )
{
	defaultPlayer->setCheckState(state);
}