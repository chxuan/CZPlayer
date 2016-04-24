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
* \file   HotKeysSettingsPage.cpp
* \author chengxuan   787280310@qq.com
* \date   2015-01-24
* \brief  热键设置页面实现文件。
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

#include "HotkeysSettingsPage.h"
#include "GlobalConfig.h"
#include "EditHotkeyDialog.h"
#include "NoFocusDelegate.h"
#include "TR.h"
#include <QPushButton>
#include <QTableWidgetItem>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QTableWidget>
#include <QHeaderView>
#ifdef CZ_DEBUG
#include <QDebug>
#endif 

/*****************************************************************************
  这里用来检查版本的对应
 *****************************************************************************/
#if GLOBALCONFIG_VERSION < 0x030000
#error  "GlobalConfig version error."
#endif

#if EDITHOTKEYDIALOG_VERSION < 0x030000
#error  "EditHotkeyDialog version error."
#endif

#if TR_VERSION < 0x030000
#error  "TR version error."
#endif

HotKeysSettingsPage::HotKeysSettingsPage(QWidget *parent) 
	: QWidget(parent),
	m_editHotkeyDialog(NULL),
	m_row(0)
{
	this->createUI();
	this->createConnect();
}

void HotKeysSettingsPage::handleRecoverDefaultSetting()
{
	if (m_settingsHotKey->text() != ConfigOptions)
	{
		m_settingsHotKey->setText(ConfigOptions);
		emit sigHotKeysSettingsChanged();
	}
	/*Else do nothing, and conintue.*/

	if (m_minToTrayHotKey->text() != ShowHideMainWidget)
	{
		m_minToTrayHotKey->setText(ShowHideMainWidget);
		emit sigHotKeysSettingsChanged();
	}
	/*Else do nothing, and conintue.*/
	
	if (m_openMinWidgetHotKey->text() != MinMainWidget)
	{
		m_openMinWidgetHotKey->setText(MinMainWidget);
		emit sigHotKeysSettingsChanged();
	}
	/*Else do nothing, and conintue.*/

	if (m_openMusicFileHotKey->text() != OpenMusicFile)
	{
		m_openMusicFileHotKey->setText(OpenMusicFile);
		emit sigHotKeysSettingsChanged();
	}
	/*Else do nothing, and conintue.*/
	
    if (m_openMediaWidgetHotKey->text() != ShowHideMediaWidget)
	{
        m_openMediaWidgetHotKey->setText(ShowHideMediaWidget);
		emit sigHotKeysSettingsChanged();
	}
	/*Else do nothing, and conintue.*/
	
	if (m_openListWidgetHotKey->text() != ShowHideMusicList)
	{
		m_openListWidgetHotKey->setText(ShowHideMusicList);
		emit sigHotKeysSettingsChanged();
	}
	/*Else do nothing, and conintue.*/
	
	if (m_openLrcHotKey->text() != ShowHideLrc)
	{
		m_openLrcHotKey->setText(ShowHideLrc);
		emit sigHotKeysSettingsChanged();
	}
	/*Else do nothing, and conintue.*/
	
	if (m_pauseHotKey->text() != PlayPause)
	{
		m_pauseHotKey->setText(PlayPause);
		emit sigHotKeysSettingsChanged();
	}
	/*Else do nothing, and conintue.*/
	
	if (m_lastHotKey->text() != PlayPre)
	{
		m_lastHotKey->setText(PlayPre);
		emit sigHotKeysSettingsChanged();
	}
	/*Else do nothing, and conintue.*/
	
	if (m_nextHotKey->text() != PlayNext)
	{
		m_nextHotKey->setText(PlayNext);
		emit sigHotKeysSettingsChanged();
	}
	/*Else do nothing, and conintue.*/
}

void HotKeysSettingsPage::handleEditHotKeys()
{
	QString currentHotkeyName = m_hotKeysTableWidget->item(m_row, 0)->text();
	QString currentHotkey = m_hotKeysTableWidget->item(m_row, 1)->text();
	m_editHotkeyDialog->init(currentHotkeyName, currentHotkey);
	m_editHotkeyDialog->exec();
}

void HotKeysSettingsPage::handleTableClicked(const int &index)
{
	m_row = index;
}

HotKeysSettingsPage::~HotKeysSettingsPage()
{
	if (m_editHotkeyDialog != NULL)
	{
		m_editHotkeyDialog->deleteLater();
		m_editHotkeyDialog = NULL;
	}
	/*Else do nothing, and conintue.*/
}

void HotKeysSettingsPage::createUI()
{
	QGroupBox *hotKeysGB = new QGroupBox(TR("热键设置"));

	//热键表
    m_hotKeysTableWidget = new QTableWidget(this);
    this->setTableWidgetStyle();

	//编辑热键对话框
	if (m_editHotkeyDialog == NULL)
	{
		m_editHotkeyDialog = new EditHotkeyDialog(this);
	}
	/*Else do nothing, and conintue.*/

	//选项设置
	QTableWidgetItem *settingsAction = new QTableWidgetItem(TR("选项设置"));
	settingsAction->setTextAlignment(Qt::AlignCenter);
	m_settingsHotKey = new QTableWidgetItem;
	m_settingsHotKey->setTextAlignment(Qt::AlignCenter);
	m_hotKeysTableWidget->insertRow(0);//从播放列表中的当前行插入
	m_hotKeysTableWidget->setItem(0, 0, settingsAction);
	m_hotKeysTableWidget->setItem(0, 1, m_settingsHotKey);

	//显示/隐藏主界面
	QTableWidgetItem *minToTrayAction = new QTableWidgetItem(TR("显示/隐藏主界面"));
	minToTrayAction->setTextAlignment(Qt::AlignCenter);
	m_minToTrayHotKey = new QTableWidgetItem;
	m_minToTrayHotKey->setTextAlignment(Qt::AlignCenter);
	m_hotKeysTableWidget->insertRow(1);//从播放列表中的当前行插入
	m_hotKeysTableWidget->setItem(1, 0, minToTrayAction);
	m_hotKeysTableWidget->setItem(1, 1, m_minToTrayHotKey);

	//迷你模式/正常窗口
	QTableWidgetItem *openMinWidgetAction = new QTableWidgetItem(TR("迷你模式/正常窗口"));
	openMinWidgetAction->setTextAlignment(Qt::AlignCenter);
	m_openMinWidgetHotKey = new QTableWidgetItem;
	m_openMinWidgetHotKey->setTextAlignment(Qt::AlignCenter);
	m_hotKeysTableWidget->insertRow(2);//从播放列表中的当前行插入
	m_hotKeysTableWidget->setItem(2, 0, openMinWidgetAction);
	m_hotKeysTableWidget->setItem(2, 1, m_openMinWidgetHotKey);

	//打开音乐文件
	QTableWidgetItem *openMusicFileAction = new QTableWidgetItem(TR("打开音乐文件"));
	openMusicFileAction->setTextAlignment(Qt::AlignCenter);
	m_openMusicFileHotKey = new QTableWidgetItem;
	m_openMusicFileHotKey->setTextAlignment(Qt::AlignCenter);
	m_hotKeysTableWidget->insertRow(3);//从播放列表中的当前行插入
	m_hotKeysTableWidget->setItem(3, 0, openMusicFileAction);
	m_hotKeysTableWidget->setItem(3, 1, m_openMusicFileHotKey);

    //显示/隐藏媒体面板
    QTableWidgetItem *openMediaWidgetAction = new QTableWidgetItem(TR("显示/隐藏媒体面板"));
    openMediaWidgetAction->setTextAlignment(Qt::AlignCenter);
    m_openMediaWidgetHotKey = new QTableWidgetItem;
    m_openMediaWidgetHotKey->setTextAlignment(Qt::AlignCenter);
	m_hotKeysTableWidget->insertRow(4);//从播放列表中的当前行插入
    m_hotKeysTableWidget->setItem(4, 0, openMediaWidgetAction);
    m_hotKeysTableWidget->setItem(4, 1, m_openMediaWidgetHotKey);

	//显示/隐藏歌曲列表
	QTableWidgetItem *openListWidgetAction = new QTableWidgetItem(TR("显示/隐藏歌曲列表"));
	openListWidgetAction->setTextAlignment(Qt::AlignCenter);
	m_openListWidgetHotKey = new QTableWidgetItem;
	m_openListWidgetHotKey->setTextAlignment(Qt::AlignCenter);
	m_hotKeysTableWidget->insertRow(5);//从播放列表中的当前行插入
	m_hotKeysTableWidget->setItem(5, 0, openListWidgetAction);
	m_hotKeysTableWidget->setItem(5, 1, m_openListWidgetHotKey);

	//显示/隐藏桌面歌词
	QTableWidgetItem *openLrcAction = new QTableWidgetItem(TR("显示/隐藏桌面歌词"));
	openLrcAction->setTextAlignment(Qt::AlignCenter);
	m_openLrcHotKey = new QTableWidgetItem;
	m_openLrcHotKey->setTextAlignment(Qt::AlignCenter);
	m_hotKeysTableWidget->insertRow(6);//从播放列表中的当前行插入
	m_hotKeysTableWidget->setItem(6, 0, openLrcAction);
	m_hotKeysTableWidget->setItem(6, 1, m_openLrcHotKey);

	//播放/暂停
	QTableWidgetItem *pauseAction = new QTableWidgetItem(TR("播放/暂停"));
	pauseAction->setTextAlignment(Qt::AlignCenter);
	m_pauseHotKey = new QTableWidgetItem;
	m_pauseHotKey->setTextAlignment(Qt::AlignCenter);
	m_hotKeysTableWidget->insertRow(7);//从播放列表中的当前行插入
	m_hotKeysTableWidget->setItem(7, 0, pauseAction);
	m_hotKeysTableWidget->setItem(7, 1, m_pauseHotKey);

	//上一首
	QTableWidgetItem *lastAction = new QTableWidgetItem(TR("上一首"));
	lastAction->setTextAlignment(Qt::AlignCenter);
	m_lastHotKey = new QTableWidgetItem;
	m_lastHotKey->setTextAlignment(Qt::AlignCenter);
	m_hotKeysTableWidget->insertRow(8);//从播放列表中的当前行插入
	m_hotKeysTableWidget->setItem(8, 0, lastAction);
	m_hotKeysTableWidget->setItem(8, 1, m_lastHotKey);

	//下一首
	QTableWidgetItem *nextAction = new QTableWidgetItem(TR("下一首"));
	nextAction->setTextAlignment(Qt::AlignCenter);
	m_nextHotKey = new QTableWidgetItem;
	m_nextHotKey->setTextAlignment(Qt::AlignCenter);
	m_hotKeysTableWidget->insertRow(9);//从播放列表中的当前行插入
	m_hotKeysTableWidget->setItem(9, 0, nextAction);
	m_hotKeysTableWidget->setItem(9, 1, m_nextHotKey);

	//编辑
	m_editButton = new QPushButton(this);
	m_editButton->setObjectName("m_editButton");
	m_editButton->setText(TR("编辑热键"));

	//恢复默认
	m_recoverDefaultSettingButton = new QPushButton(this);
	m_recoverDefaultSettingButton->setObjectName("m_recoverDefaultSettingButton");
	m_recoverDefaultSettingButton->setText(TR("恢复默认热键"));

	QHBoxLayout *buttonsLayout = new QHBoxLayout;
	buttonsLayout->addWidget(m_editButton);
	buttonsLayout->addStretch();
	buttonsLayout->addWidget(m_recoverDefaultSettingButton);

	QVBoxLayout *hotKeysLayout = new QVBoxLayout;
	hotKeysLayout->addWidget(m_hotKeysTableWidget);
	hotKeysLayout->addLayout(buttonsLayout);
	hotKeysGB->setLayout(hotKeysLayout);

	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addWidget(hotKeysGB);
	this->setLayout(mainLayout);
}

void HotKeysSettingsPage::createConnect()
{
	connect(m_recoverDefaultSettingButton, SIGNAL(clicked()), this, SLOT(handleRecoverDefaultSetting()));
	connect(m_editButton, SIGNAL(clicked()), this, SLOT(handleEditHotKeys()));
	connect(m_hotKeysTableWidget, SIGNAL(cellPressed(int, int)), 
		this, SLOT(handleTableClicked(int)));
	connect(m_hotKeysTableWidget, SIGNAL(cellDoubleClicked(int, int)),
		this, SLOT(handleTableDoubleClicked(int, int)));
	connect(m_editHotkeyDialog, SIGNAL(sigEditHotkeyFinished(QString, QString)),
            this, SLOT(handleEditHotkeyFinished(QString, QString)));
}

void HotKeysSettingsPage::setTableWidgetStyle()
{
    //设置窗口基本属性
    m_hotKeysTableWidget->setRowCount(0);//初始化的行数为0
    m_hotKeysTableWidget->setColumnCount(2);//初始化的列数为2

    QStringList headList;
    headList << TR("动作") << TR("全局热键");
    m_hotKeysTableWidget->setHorizontalHeaderLabels(headList);								//设置头信息

    m_hotKeysTableWidget->verticalHeader()->setVisible(false);
    m_hotKeysTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);	//设置内容自动横向填满可用区域
    m_hotKeysTableWidget->horizontalHeader()->setHighlightSections(false);					//点击表时不对表头行光亮（获取焦点）
    m_hotKeysTableWidget->horizontalHeader()->setSectionsClickable(false);					//不响应鼠标单击
    m_hotKeysTableWidget->setSelectionMode(QAbstractItemView::SingleSelection);				//设置只能选中单行
    m_hotKeysTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);				//设置选择习惯为选择行
    m_hotKeysTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);				//设置内容不可编辑
    m_hotKeysTableWidget->setShowGrid(false);												//设置不显示网格
    m_hotKeysTableWidget->setItemDelegate(new NoFocusDelegate());							//去掉选中单元格的虚线

    m_hotKeysTableWidget->setStyleSheet("color: black;");
}

void HotKeysSettingsPage::init()
{
	GlobalConfig *config = GlobalConfig::instance();
	m_settingsHotKey->setText(config->settingsHotKey());
	m_minToTrayHotKey->setText(config->minToTrayHotKey());
	m_openMinWidgetHotKey->setText(config->openMinWidgetHotKey());
	m_openMusicFileHotKey->setText(config->openMusicFileHotKey());
    m_openMediaWidgetHotKey->setText(config->openMediaWidgetHotKey());
	m_openListWidgetHotKey->setText(config->openListWidgetHotKey());
	m_openLrcHotKey->setText(config->openLrcHotKey());
	m_pauseHotKey->setText(config->pauseHotKey());
	m_lastHotKey->setText(config->lastHotKey());
	m_nextHotKey->setText(config->nextHotKey());
}

void HotKeysSettingsPage::apply()
{
	GlobalConfig *config = GlobalConfig::instance();
	config->setSettingsHotKey(m_settingsHotKey->text());
	config->setMinToTrayHotKey(m_minToTrayHotKey->text());
	config->setOpenMinWidgetHotKey(m_openMinWidgetHotKey->text());
	config->setOpenMusicFileHotKey(m_openMusicFileHotKey->text());
    config->setOpenMediaWidgetHotKey(m_openMediaWidgetHotKey->text());
	config->setOpenListWidgetHotKey(m_openListWidgetHotKey->text());
	config->setOpenLrcHotKey(m_openLrcHotKey->text());
	config->setPauseHotKey(m_pauseHotKey->text());
	config->setLastHotKey(m_lastHotKey->text());
	config->setNextHotKey(m_nextHotKey->text());

	if (!config->save())
	{
		QMessageBox::warning(this, TR("警告"), TR("保存热键设置配置信息失败！"));
	}
	/*Else do nothing, and contine.*/
}

void HotKeysSettingsPage::handleEditHotkeyFinished(const QString &hotKeyName, const QString &hotKey)
{
	bool isInvaildHotkey = false;
	for (int i = 0; i < m_hotKeysTableWidget->rowCount(); ++i)
	{
		if (hotKeyName != m_hotKeysTableWidget->item(i, 0)->text() &&
			hotKey == m_hotKeysTableWidget->item(i, 1)->text())
		{
			isInvaildHotkey = true;
			QMessageBox::information(this, TR("信息"), TR("该热键已经存在，请重新编辑该热键！"), QMessageBox::Yes);
			break;
		}
		/*Else do nothing, and conintue.*/
	}

	if (!isInvaildHotkey)
	{
		for (int i = 0; i < m_hotKeysTableWidget->rowCount(); ++i)
		{
			if (hotKeyName == m_hotKeysTableWidget->item(i, 0)->text())
			{
				if (hotKey != m_hotKeysTableWidget->item(i, 1)->text())
				{
					m_hotKeysTableWidget->item(i, 1)->setText(hotKey);
					emit sigHotKeysSettingsChanged();
				}
				/*Else do nothing, and conintue.*/

				break;
			}
			/*Else do nothing, and conintue.*/
		}
	}
}

void HotKeysSettingsPage::handleTableDoubleClicked(const int &row, const int &column)
{
	(void)row;
	(void)column;
	this->handleEditHotKeys();
}






















