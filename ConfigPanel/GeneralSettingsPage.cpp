/***************************************************************************
*   Copyright (C) 2012-2015 Highway-9 Studio.                             *
*   787280310@qq.com									                   *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
*   This program is distributed in the hope that it will be useful,       *
*   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
*   GNU General Public License for more details.                          *
*                                                                         *
*   You should have received a copy of the GNU General Public License     *
*   along with this program; if not, write to the                         *
*   Free Software Foundation, Inc.,                                       *
*   CUIT Highway-9 Studio, China.									       *
***************************************************************************/

/*!
* \file   GeneralSettingsPage.cpp
* \author chengxuan   787280310@qq.com
* \date   2015-01-24
* \brief  一般设置页面实现文件。
* \version 3.0.0
*
* \verbatim
* 历史
*          3.0.0   创建,
*          2015-01-24    by chengxuan
*
* \endverbatim
*
*/

#include "GeneralSettingsPage.h"
#include "GlobalConfig.h"
#include "Global.h"
#include "TR.h"
#include <QGridLayout>
#include <QGroupBox>
#include <QCheckBox>
#include <QMessageBox>

/*****************************************************************************
  这里用来检查版本的对应
 *****************************************************************************/
#if GLOBALCONFIG_VERSION < 0x030000
#error  "GlobalConfig version error."
#endif

#if GLOBAL_VERSION < 0x030000
#error  "Global version error."
#endif

#if TR_VERSION < 0x030000
#error  "TR version error."
#endif

GeneralSettingsPage::GeneralSettingsPage(QWidget *parent) 
	: QWidget(parent)
{
	this->createUI();
	this->createConnect();
}

GeneralSettingsPage::~GeneralSettingsPage()
{
	/*Do nothing.*/
}

void GeneralSettingsPage::createUI()
{
	QGroupBox *startGB = new QGroupBox(TR("启动时"), this);

    QString styleSheet = QString("QCheckBox{color:black;}"
                                 "QCheckBox::indicator{width:10px;height:10px;border: 1px solid black;border-radius:2px}"
                                 "QCheckBox::indicator:checked {image: url(%1checked2.png);}").arg(ImagesDir);

	//自动播放
	m_autoPlay = new QCheckBox(TR("自动播放歌曲"), this);
    m_autoPlay->setStyleSheet(styleSheet);

	//开机自启动
	m_autoRun = new QCheckBox(TR("开机时自动启动CZPlayer"), this);
    m_autoRun->setStyleSheet(styleSheet);

	//播放欢迎语
	m_helloCZPlayer = new QCheckBox(TR("播放“听音乐,找CZPlayer”问候语"), this);
    m_helloCZPlayer->setStyleSheet(styleSheet);

	QGroupBox *fileConnect = new QGroupBox(TR("文件关联"), this);

	//设置为默认播放器
	m_defaultPlayer = new QCheckBox(TR("将CZPlayer作为默认播放器"), this);
    m_defaultPlayer->setStyleSheet(styleSheet);

	QGridLayout *startGBLayoutG = new QGridLayout;
	startGBLayoutG->addWidget(m_autoPlay, 0, 0);
	startGBLayoutG->addWidget(m_autoRun, 0, 1);
	startGBLayoutG->addWidget(m_helloCZPlayer, 1, 0);
	startGB->setLayout(startGBLayoutG);

	QVBoxLayout *fileLayout = new QVBoxLayout;
	fileLayout->addWidget(m_defaultPlayer);
	fileConnect->setLayout(fileLayout);

	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addWidget(startGB);
	mainLayout->addWidget(fileConnect);
	mainLayout->addStretch();
	this->setLayout(mainLayout);
}

void GeneralSettingsPage::createConnect()
{
	connect(m_autoPlay, SIGNAL(stateChanged(int)), this, SIGNAL(sigGeneralSettingsChanged()));
	connect(m_autoRun, SIGNAL(stateChanged(int)), this, SIGNAL(sigGeneralSettingsChanged()));
	connect(m_helloCZPlayer, SIGNAL(stateChanged(int)), this, SIGNAL(sigGeneralSettingsChanged()));
	connect(m_defaultPlayer, SIGNAL(stateChanged(int)), this, SIGNAL(sigGeneralSettingsChanged()));
}

void GeneralSettingsPage::init()
{
	GlobalConfig *config = GlobalConfig::instance();
	m_autoPlay->setChecked(config->autoPlayMusic());
	m_autoRun->setChecked(config->autoRunPlayer());
	m_helloCZPlayer->setChecked(config->autoGreetings());
	m_defaultPlayer->setChecked(config->defaultPlayer());
}

void GeneralSettingsPage::apply()
{
	GlobalConfig *config = GlobalConfig::instance();

	config->setAutoPlayMusic(m_autoPlay->checkState() == Qt::CheckState::Checked);
	config->setAutoRunPlayer(m_autoRun->checkState() == Qt::CheckState::Checked);
	config->setAutoGreetings(m_helloCZPlayer->checkState() == Qt::CheckState::Checked);
	config->setDefaultPlayer(m_defaultPlayer->checkState() == Qt::CheckState::Checked);

	if (!config->save())
	{
		QMessageBox::warning(this, TR("警告"), TR("保存常规设置配置信息失败！"));
	}
	/*Else do nothing, and contine.*/
}
