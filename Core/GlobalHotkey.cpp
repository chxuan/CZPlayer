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
* \file   GlobalHotKey.cpp
* \author chengxuan   787280310@qq.com
* \date   2015-01-31
* \brief  全局热键实现文件。
* \version 3.0.0
*
* \verbatim
* 历史
*          3.0.0   创建,
*          2015-01-31    by chengxuan
*
* \endverbatim
*
*/

#include "GlobalHotkey.h"
#include "qxtglobalshortcut.h"
#include "GlobalConfig.h"

/*****************************************************************************
  这里用来检查版本的对应
 *****************************************************************************/
#if GLOBALCONFIG_VERSION < 0x030000
#error  "GlobalConfig version error."
#endif

GlobalHotKey::GlobalHotKey(QObject *parent)
	: QObject(NULL)
{
    //选项设置
    m_configOptions = new QxtGlobalShortcut(this);

    //显示/隐藏主界面
    m_showHideMainWidget = new QxtGlobalShortcut(this);

    //迷你模式/正常窗口
    m_minMainWidget = new QxtGlobalShortcut(this);

    //打开音乐文件
    m_openMusicfile = new QxtGlobalShortcut(this);

    //显示/隐藏下载列表
    m_showDownload = new QxtGlobalShortcut(this);

    //显示/隐藏歌曲列表
    m_showMusicList = new QxtGlobalShortcut(this);

    //显示/隐藏桌面歌词
    m_showLrc = new QxtGlobalShortcut(this);

    //播放/暂停
    m_playPause = new QxtGlobalShortcut(this);

    //上一首
    m_last = new QxtGlobalShortcut(this);

    //下一首
    m_nextSc = new QxtGlobalShortcut(this);

	GlobalConfig *config = GlobalConfig::instance();
	connect(config, SIGNAL(sigSettingsHotKeyChanged(QString)), this, SLOT(handleSettingsHotKeyChanged(QString)));
	connect(config, SIGNAL(sigMinToTrayHotKeyChanged(QString)), this, SLOT(handleMinToTrayHotKeyChanged(QString)));
	connect(config, SIGNAL(sigOpenMinWidgetHotKeyChanged(QString)), this, SLOT(handleOpenMinWidgetHotKeyChanged(QString)));
	connect(config, SIGNAL(sigOpenMusicFileHotKeyChanged(QString)), this, SLOT(handleOpenMusicFileHotKeyChanged(QString)));
    connect(config, SIGNAL(sigOpenMediaWidgetHotKeyChanged(QString)), this, SLOT(handleOpenMediaWidgetHotKeyChanged(QString)));
	connect(config, SIGNAL(sigOpenListWidgetHotKeyChanged(QString)), this, SLOT(handleOpenListWidgetHotKeyChanged(QString)));
	connect(config, SIGNAL(sigOpenLrcHotKeyChanged(QString)), this, SLOT(handleOpenLrcHotKeyChanged(QString)));
	connect(config, SIGNAL(sigPauseHotKeyChanged(QString)), this, SLOT(handlePauseHotKeyChanged(QString)));
	connect(config, SIGNAL(sigLastHotKeyChanged(QString)), this, SLOT(handleLastHotKeyChanged(QString)));
	connect(config, SIGNAL(sigNextHotKeyChanged(QString)), this, SLOT(handleNextHotKeyChanged(QString)));

	connect(m_configOptions, SIGNAL(activated()), parent, SLOT(handleConfigOptions()));
	connect(m_showHideMainWidget, SIGNAL(activated()), parent, SLOT(handleShowHideMainWidget()));
	connect(m_minMainWidget, SIGNAL(activated()), parent, SLOT(handleMinMainWidget()));
	connect(m_openMusicfile, SIGNAL(activated()), parent, SLOT(handleOpenFile()));
    connect(m_showDownload, SIGNAL(activated()), parent, SLOT(handleOpenMediaWidget()));
	connect(m_showMusicList, SIGNAL(activated()), parent, SLOT(handleOpenMusicList()));
	connect(m_showLrc, SIGNAL(activated()), parent, SLOT(handleOpenMusicLrc()));
	connect(m_playPause, SIGNAL(activated()), parent, SLOT(handleSetPlayPaused()));
	connect(m_last, SIGNAL(activated()), parent, SLOT(handleSetPre()));
	connect(m_nextSc, SIGNAL(activated()), parent, SLOT(handleSetNext()));
}

GlobalHotKey::~GlobalHotKey()
{
	m_configOptions->setEnabled(false);
	m_showHideMainWidget->setEnabled(false);
	m_minMainWidget->setEnabled(false);
	m_openMusicfile->setEnabled(false);
	m_showDownload->setEnabled(false);
	m_showMusicList->setEnabled(false);
	m_showLrc->setEnabled(false);
	m_playPause->setEnabled(false);
	m_last->setEnabled(false);
	m_nextSc->setEnabled(false);
}

void GlobalHotKey::handleSettingsHotKeyChanged(const QString &hotKey)
{
	m_configOptions->setShortcut(QKeySequence(hotKey));
}

void GlobalHotKey::handleMinToTrayHotKeyChanged(const QString &hotKey)
{
	m_showHideMainWidget->setShortcut(QKeySequence(hotKey));
}

void GlobalHotKey::handleOpenMinWidgetHotKeyChanged(const QString &hotKey)
{
	m_minMainWidget->setShortcut(QKeySequence(hotKey));
}

void GlobalHotKey::handleOpenMusicFileHotKeyChanged(const QString &hotKey)
{
	m_openMusicfile->setShortcut(QKeySequence(hotKey));
}

void GlobalHotKey::handleOpenMediaWidgetHotKeyChanged(const QString &hotKey)
{
	m_showDownload->setShortcut(QKeySequence(hotKey));
}

void GlobalHotKey::handleOpenListWidgetHotKeyChanged(const QString &hotKey)
{
	m_showMusicList->setShortcut(QKeySequence(hotKey));
}

void GlobalHotKey::handleOpenLrcHotKeyChanged(const QString &hotKey)
{
	m_showLrc->setShortcut(QKeySequence(hotKey));
}

void GlobalHotKey::handlePauseHotKeyChanged(const QString &hotKey)
{
	m_playPause->setShortcut(QKeySequence(hotKey));
}

void GlobalHotKey::handleLastHotKeyChanged(const QString &hotKey)
{
	m_last->setShortcut(QKeySequence(hotKey));
}

void GlobalHotKey::handleNextHotKeyChanged(const QString &hotKey)
{
	m_nextSc->setShortcut(QKeySequence(hotKey));
}

void GlobalHotKey::setConfigOptionsHotkeyWork(bool isWork)
{
	m_configOptions->setEnabled(isWork);
}

void GlobalHotKey::setShowHideMainWidgetHotkeyWork(bool isWork)
{
	m_showHideMainWidget->setEnabled(isWork);
}

void GlobalHotKey::setMinMainWidgetHotkeyWork(bool isWork)
{
	m_minMainWidget->setEnabled(isWork);
}

void GlobalHotKey::setOpenMusicfileHotkeyWork(bool isWork)
{
	m_openMusicfile->setEnabled(isWork);
}

void GlobalHotKey::setShowDownloadHotkeyWork(bool isWork)
{
	m_showDownload->setEnabled(isWork);
}

void GlobalHotKey::setShowMusicListHotkeyWork(bool isWork)
{
	m_showMusicList->setEnabled(isWork);
}

void GlobalHotKey::setShowLrcHotkeyWork(bool isWork)
{
	m_showLrc->setEnabled(isWork);
}

void GlobalHotKey::setPlayPauseHotkeyWork(bool isWork)
{
	m_playPause->setEnabled(isWork);
}

void GlobalHotKey::setLastHotkeyWork(bool isWork)
{
	m_last->setEnabled(isWork);
}

void GlobalHotKey::setNextHotkeyWork(bool isWork)
{
	m_nextSc->setEnabled(isWork);
}





