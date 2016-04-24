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
* \file   UpdatePage.cpp
* \author chengxuan   787280310@qq.com
* \date   2015-01-24
* \brief  软件更新页面实现文件。
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

#include "UpdatePage.h"
#include "TR.h"
#include <Windows.h>
#include <QLabel>
#include <QPushButton>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QProcess>
#include <QMessageBox>
#ifdef CZ_DEBUG
#include <QDebug>
#endif

/*****************************************************************************
  这里用来检查版本的对应
 *****************************************************************************/
#if TR_VERSION < 0x030000
#error  "TR version error."
#endif

/*****************************************************************************
全局变量定义
*****************************************************************************/
const QString AutoUpdatePath = "./AutoUpdate/AutoUpdate.exe";	/*!< 在线升级程序路径*/

UpdatePage::UpdatePage(QWidget *parent) 
	: QWidget(parent)
{
	this->createUI();
	this->createConnect();
}

UpdatePage::~UpdatePage(void)
{
	/*Do nothing.*/
}

void UpdatePage::createUI()
{
	QGroupBox *updateGB = new QGroupBox(TR("软件更新"), this);

	//更新信息
	m_updateInfoLabel = new QLabel(this);
	m_updateInfoLabel->setText(TR("您现在使用的版本是V3.0 更新时间:2014年07月18日"));

	//开始更新按钮
	m_updateButton = new QPushButton(this);
	m_updateButton->setText(TR("手动检查最新版本"));

	QHBoxLayout *hLayout = new QHBoxLayout;
	hLayout->addWidget(m_updateInfoLabel);
	hLayout->addStretch();

	QHBoxLayout *hLayout2 = new QHBoxLayout;
	hLayout2->addWidget(m_updateButton);
	hLayout2->addStretch();

	QVBoxLayout *vLayout = new QVBoxLayout;
	vLayout->addLayout(hLayout);
	vLayout->addLayout(hLayout2);
	updateGB->setLayout(vLayout);

	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addWidget(updateGB);
	mainLayout->addStretch();
	this->setLayout(mainLayout);
}

void UpdatePage::createConnect()
{
	connect(m_updateButton, SIGNAL(clicked()), this, SLOT(handleUpdate()));
}

void UpdatePage::handleUpdate()
{
    unsigned long processId = GetCurrentProcessId();
    QStringList idList;
    idList << QString::number(processId);

    if (!QProcess::startDetached(AutoUpdatePath, idList))
    {
        QMessageBox::information(this, TR("信息"), TR("启动在线升级程序失败，请确认程序路径是否正确！"), QMessageBox::Yes);
    }
    /*Else do nothing, and continue.*/
}
