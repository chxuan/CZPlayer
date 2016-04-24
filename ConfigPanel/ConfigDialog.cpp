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
* \file   ConfigDialog.cpp
* \author chengxuan   787280310@qq.com
* \date   2015-01-24
* \brief  全局配置实现文件。
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

#include "ConfigDialog.h"
#include "GeneralSettingsPage.h"
#include "DownloadSettingsPage.h"
#include "LrcSettingsPage.h"
#include "HotkeysSettingsPage.h"
#include "UpdatePage.h"
#include "Global.h"
#include "TR.h"
#include <QListWidget>
#include <QPushButton>
#include <QStackedWidget>
#include <QHBoxLayout>
#include <QDir>
#include <QCloseEvent>
#ifdef CZ_DEBUG
#include <QDebug>
#endif

/*****************************************************************************
  这里用来检查版本的对应
 *****************************************************************************/
#if GENERALSETTINGSPAGE_VERSION < 0x030000
#error  "GeneralSettingsPage version error."
#endif

#if DOWNLOADSETTINGSPAGE_VERSION < 0x030000
#error  "DownloadSettingsPage version error."
#endif

#if LRCSETTINGSPAGE_VERSION < 0x030000
#error  "LrcSettingsPage version error."
#endif

#if HOTKEYSSETTINGSPAGE_VERSION < 0x030000
#error  "HotkeysSettingsPage version error."
#endif

#if UPDATEPAGE_VERSION < 0x030000
#error  "UpdatePage version error."
#endif

#if GLOBAL_VERSION < 0x030000
#error  "Global version error."
#endif

#if TR_VERSION < 0x030000
#error  "TR version error."
#endif

ConfigDialog::ConfigDialog(QWidget *parent) 
	: QDialog(parent),
	  m_generalSettingsPage(NULL),
	  m_downloadSettingsPage(NULL),
	  m_lrcSettingsPage(NULL),
	  m_hotKeysSettingsPage(NULL),
	  m_updatePage(NULL),
	  m_isGeneralSettingsChanged(false),
	  m_isDownloadSettingsChanged(false),
	  m_isLrcSettingsChanged(false),
	  m_isHotKeysSettingsChanged(false)
{
	this->createUI();
	this->createConnect();
}

ConfigDialog::~ConfigDialog()
{
	//基本设置
	if (m_generalSettingsPage != NULL)
	{
		m_generalSettingsPage->deleteLater();
		m_generalSettingsPage = NULL;
	}
	/*Else do nothing, and continue.*/

	//下载设置
	if (m_downloadSettingsPage != NULL)
	{
		m_downloadSettingsPage->deleteLater();
		m_downloadSettingsPage = NULL;
	}
	/*Else do nothing, and continue.*/

	//歌词设置
	if (m_lrcSettingsPage != NULL)
	{
		m_lrcSettingsPage->deleteLater();
		m_lrcSettingsPage = NULL;
	}
	/*Else do nothing, and continue.*/

	//热键设置
	if (m_hotKeysSettingsPage != NULL)
	{
		m_hotKeysSettingsPage->deleteLater();
		m_hotKeysSettingsPage = NULL;
	}
	/*Else do nothing, and continue.*/

	//更新页面
	if (m_updatePage != NULL)
	{
		m_updatePage->deleteLater();
		m_updatePage = NULL;
	}
	/*Else do nothing, and continue.*/
}

//创建图标
void ConfigDialog::createIcons()
{
    //常规设置
    QListWidgetItem *generalSettingsItem = new QListWidgetItem(m_contentsWidget);
    generalSettingsItem->setIcon(QIcon(ImagesDir + "generalSettingsButton.png"));
    generalSettingsItem->setText(TR("常规设置"));
    generalSettingsItem->setTextAlignment(Qt::AlignCenter);
    generalSettingsItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    //下载设置
    QListWidgetItem *downloadSettingsItem = new QListWidgetItem(m_contentsWidget);
    downloadSettingsItem->setIcon(QIcon(ImagesDir + "downloadSettingsButton.png"));
    downloadSettingsItem->setText(TR("下载设置"));
    downloadSettingsItem->setTextAlignment(Qt::AlignCenter);
    downloadSettingsItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    //歌词设置
    QListWidgetItem *lrcSettingsItem = new QListWidgetItem(m_contentsWidget);
    lrcSettingsItem->setIcon(QIcon(ImagesDir + "lrcSettingsButton.png"));
    lrcSettingsItem->setText(TR("歌词设置"));
    lrcSettingsItem->setTextAlignment(Qt::AlignCenter);
    lrcSettingsItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    //热键设置
    QListWidgetItem *hotKeysSettingsItem = new QListWidgetItem(m_contentsWidget);
    hotKeysSettingsItem->setIcon(QIcon(ImagesDir + "hotKeysSettingsButton.png"));
    hotKeysSettingsItem->setText(TR("热键设置"));
    hotKeysSettingsItem->setTextAlignment(Qt::AlignCenter);
    hotKeysSettingsItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

	//更新页面
	QListWidgetItem *updateItem = new QListWidgetItem(m_contentsWidget);
    updateItem->setIcon(QIcon(ImagesDir + "updateButton.png"));
	updateItem->setText(TR("软件更新"));
	updateItem->setTextAlignment(Qt::AlignCenter);
	updateItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
}

//改变页面
void ConfigDialog::handleChangePage(QListWidgetItem *current, QListWidgetItem *previous)
{
    if (!current)
    {
        current = previous;
    }
	/*Else do nothing, and continue.*/
    m_pagesWidget->setCurrentIndex(m_contentsWidget->row(current));
}

void ConfigDialog::handleOkFunc()
{
    this->handleAppFunc();
	this->close();
}

void ConfigDialog::handleAppFunc()
{
	if (m_isGeneralSettingsChanged)
	{
		m_generalSettingsPage->apply();
	}
	/*Else do nothing, and continue.*/

	if (m_isDownloadSettingsChanged)
	{
		m_downloadSettingsPage->apply();
	}
	/*Else do nothing, and continue.*/

	if (m_isLrcSettingsChanged)
	{
		m_lrcSettingsPage->apply();
	}
	/*Else do nothing, and continue.*/

	if (m_isHotKeysSettingsChanged)
	{
		m_hotKeysSettingsPage->apply();
	}
	/*Else do nothing, and continue.*/

	m_appButton->setEnabled(false);
}

void ConfigDialog::init()
{
	m_contentsWidget->setCurrentRow(0);
	m_generalSettingsPage->init();
	m_downloadSettingsPage->init();
	m_lrcSettingsPage->init();
	m_hotKeysSettingsPage->init();

	m_isGeneralSettingsChanged = false;
	m_isDownloadSettingsChanged = false;
	m_isLrcSettingsChanged = false;
	m_isHotKeysSettingsChanged = false;

	m_appButton->setEnabled(false);
}

void ConfigDialog::createUI()
{
	//设置窗口基本属性
	this->resize(665, 482);//设置窗体大小
	this->setMinimumSize(665, 482);
	this->setMaximumSize(665, 482);
	this->setWindowTitle(TR("CZPlayer 设置"));

	//设置选项
    m_contentsWidget = new QListWidget(this);
	m_contentsWidget->setViewMode(QListView::ListMode);//IconMode
	m_contentsWidget->setIconSize(QSize(96, 84));
	m_contentsWidget->setMovement(QListView::Static);
	m_contentsWidget->setMaximumWidth(128);
	m_contentsWidget->setSpacing(12);
	this->createIcons();

	//基本设置
	if (m_generalSettingsPage == NULL)
	{
		m_generalSettingsPage = new GeneralSettingsPage(this);
	}
	/*Else do nothing, and continue.*/

	//下载设置
	if (m_downloadSettingsPage == NULL)
	{
		m_downloadSettingsPage = new DownloadSettingsPage(this);
	}
	/*Else do nothing, and continue.*/

	//歌词设置
	if (m_lrcSettingsPage == NULL)
	{
		m_lrcSettingsPage = new LrcSettingsPage(this);
	}
	/*Else do nothing, and continue.*/

	//热键设置
	if (m_hotKeysSettingsPage == NULL)
	{
		m_hotKeysSettingsPage = new HotKeysSettingsPage(this);
	}
	/*Else do nothing, and continue.*/

	//更新页面
	if (m_updatePage == NULL)
	{
		m_updatePage = new UpdatePage(this);
	}
	/*Else do nothing, and continue.*/

	//多页显示
    m_pagesWidget = new QStackedWidget(this);
	m_pagesWidget->addWidget(m_generalSettingsPage);
	m_pagesWidget->addWidget(m_downloadSettingsPage);
	m_pagesWidget->addWidget(m_lrcSettingsPage);
	m_pagesWidget->addWidget(m_hotKeysSettingsPage);
	m_pagesWidget->addWidget(m_updatePage);
	m_contentsWidget->setCurrentRow(0);

	//确定按钮
	m_okButton = new QPushButton(this);
	m_okButton->setObjectName(TR("m_okButton"));
	m_okButton->setText(TR("确定"));

	//取消按钮
	m_cancelButton = new QPushButton(this);
	m_cancelButton->setObjectName(TR("m_cancelButton"));
	m_cancelButton->setText(TR("取消"));

	//应用按钮
	m_appButton = new QPushButton(this);
	m_appButton->setObjectName(TR("m_appButton"));
	m_appButton->setText(TR("应用"));

	QHBoxLayout *horizontalLayout = new QHBoxLayout;
	horizontalLayout->addWidget(m_contentsWidget);
	horizontalLayout->addWidget(m_pagesWidget);

	QHBoxLayout *buttonsLayout = new QHBoxLayout;
	buttonsLayout->addStretch();
	buttonsLayout->addWidget(m_okButton);
	buttonsLayout->addWidget(m_cancelButton);
	buttonsLayout->addWidget(m_appButton);

	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addLayout(horizontalLayout);
	//mainLayout->addStretch();
	mainLayout->addLayout(buttonsLayout);
	this->setLayout(mainLayout);
}

void ConfigDialog::createConnect()
{
	connect(m_contentsWidget,
		SIGNAL(currentItemChanged(QListWidgetItem*, QListWidgetItem*)),
		this, SLOT(handleChangePage(QListWidgetItem*, QListWidgetItem*)));

	connect(m_cancelButton, SIGNAL(clicked()), this, SLOT(close()));
	connect(m_okButton, SIGNAL(clicked()), this, SLOT(handleOkFunc()));
	connect(m_appButton, SIGNAL(clicked()), this, SLOT(handleAppFunc()));

	connect(m_generalSettingsPage, SIGNAL(sigGeneralSettingsChanged()),
		this, SLOT(handleGeneralSettingsChanged()));
	connect(m_downloadSettingsPage, SIGNAL(sigDownloadSettingsChanged()),
		this, SLOT(handleDownloadSettingsChanged()));
	connect(m_hotKeysSettingsPage, SIGNAL(sigHotKeysSettingsChanged()),
		this, SLOT(handleHotKeysSettingsChanged()));
	connect(m_lrcSettingsPage, SIGNAL(sigLrcSettingsChanged()),
		this, SLOT(handleLrcSettingsChanged()));
}

void ConfigDialog::handleGeneralSettingsChanged()
{
	m_isGeneralSettingsChanged = true;
	m_appButton->setEnabled(true);
}

void ConfigDialog::handleDownloadSettingsChanged()
{
	m_isDownloadSettingsChanged = true;
	m_appButton->setEnabled(true);
}

void ConfigDialog::handleLrcSettingsChanged()
{
	m_isLrcSettingsChanged = true;
	m_appButton->setEnabled(true);
}

void ConfigDialog::handleHotKeysSettingsChanged()
{
	m_isHotKeysSettingsChanged = true;
	m_appButton->setEnabled(true);
}

void ConfigDialog::closeEvent(QCloseEvent *event)
{
	emit sigConfigDialogClosed();
	QDialog::closeEvent(event);
}










