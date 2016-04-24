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
* \file   GlobalHotKey.h
* \author chengxuan   787280310@qq.com
* \date   2015-01-31
* \brief  全局热键头文件。
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

#ifndef GLOBALHOTKEY_H
#define GLOBALHOTKEY_H

/*! \def GLOBALHOTKEY_VERSION
*  版本控制宏，对应v3.0.0版本.
*/
#define GLOBALHOTKEY_VERSION   0x030000

#include <QObject>
#include <QString>

class QxtGlobalShortcut;

/*! 全局热键
*/
class GlobalHotKey : public QObject
{
    Q_OBJECT

public:
	/*! 构造函数
	 *  \param parent 父窗口指针，默认为NULL
	 */
	GlobalHotKey(QObject *parent = NULL);

	/*! 析构函数.*/
    ~GlobalHotKey();

public:
	/*! 是否启用选项设置热键
	 *  \param isWork true代表启用，否则关闭
	 */
    void setConfigOptionsHotkeyWork(bool isWork);

	/*! 是否启用显示/隐藏主界面热键
	 *  \param isWork true代表启用，否则关闭
	 */
    void setShowHideMainWidgetHotkeyWork(bool isWork);

	/*! 是否启用迷你模式/正常窗口热键
	 *  \param isWork true代表启用，否则关闭
	 */
    void setMinMainWidgetHotkeyWork(bool isWork);

	/*! 是否启用打开音乐文件热键
	 *  \param isWork true代表启用，否则关闭
	 */
    void setOpenMusicfileHotkeyWork(bool isWork);

	/*! 是否启用显示/隐藏下载列表热键
	 *  \param isWork true代表启用，否则关闭
	 */
    void setShowDownloadHotkeyWork(bool isWork);

	/*! 是否启用显示/隐藏歌曲列表热键
	 *  \param isWork true代表启用，否则关闭
	 */
    void setShowMusicListHotkeyWork(bool isWork);

	/*! 是否启用显示/隐藏桌面歌词热键
	 *  \param isWork true代表启用，否则关闭
	 */
    void setShowLrcHotkeyWork(bool isWork);

	/*! 是否启用播放/暂停热键
	 *  \param isWork true代表启用，否则关闭
	 */
    void setPlayPauseHotkeyWork(bool isWork);

	/*! 是否启用上一首热键
	 *  \param isWork true代表启用，否则关闭
	 */
    void setLastHotkeyWork(bool isWork);

	/*! 是否启用下一首热键
	 *  \param isWork true代表启用，否则关闭
	 */
    void setNextHotkeyWork(bool isWork);

private slots:
	/*! 选项设置热键改变
	* \param hotKey 选项设置热键
	*/
	void handleSettingsHotKeyChanged(const QString &hotKey);

	/*! 显示/隐藏主界面热键改变
	* \param hotKey 显示/隐藏主界面热键
	*/
	void handleMinToTrayHotKeyChanged(const QString &hotKey);

	/*! 迷你模式/正常窗口热键改变
	* \param hotKey 迷你模式/正常窗口热键
	*/
	void handleOpenMinWidgetHotKeyChanged(const QString &hotKey);

	/*! 打开音乐文件热键改变
	* \param hotKey 打开音乐文件热键
	*/
	void handleOpenMusicFileHotKeyChanged(const QString &hotKey);

    /*! 显示/隐藏媒体面板热键改变
    * \param hotKey 显示/隐藏媒体面板热键
	*/
    void handleOpenMediaWidgetHotKeyChanged(const QString &hotKey);

	/*! 显示/隐藏歌曲列表热键改变
	* \param hotKey 显示/隐藏歌曲列表热键
	*/
	void handleOpenListWidgetHotKeyChanged(const QString &hotKey);

	/*! 显示/隐藏桌面歌词热键改变
	* \param hotKey 显示/隐藏桌面歌词热键
	*/
	void handleOpenLrcHotKeyChanged(const QString &hotKey);

	/*! 播放/暂停热键改变
	* \param hotKey 播放/暂停热键
	*/
	void handlePauseHotKeyChanged(const QString &hotKey);

	/*! 上一首热键改变
	* \param hotKey 上一首热键
	*/
	void handleLastHotKeyChanged(const QString &hotKey);

	/*! 下一首热键改变
	* \param hotKey 下一首热键
	*/
	void handleNextHotKeyChanged(const QString &hotKey);

private:
	QxtGlobalShortcut *m_configOptions;				/*!< 选项设置热键*/
    QxtGlobalShortcut *m_showHideMainWidget;		/*!< 显示/隐藏主界面热键*/
    QxtGlobalShortcut *m_minMainWidget;				/*!< 迷你模式/正常窗口热键*/
    QxtGlobalShortcut *m_openMusicfile;				/*!< 打开音乐文件热键*/
    QxtGlobalShortcut *m_showDownload;				/*!< 显示/隐藏下载列表热键*/
    QxtGlobalShortcut *m_showMusicList;				/*!< 显示/隐藏歌曲列表热键*/
    QxtGlobalShortcut *m_showLrc;					/*!< 显示/隐藏桌面歌词热键*/
    QxtGlobalShortcut *m_playPause;					/*!< 播放/暂停热键*/
    QxtGlobalShortcut *m_last;						/*!< 上一首热键*/
    QxtGlobalShortcut *m_nextSc;					/*!< 下一首热键*/
};

#endif // GLOBALHOTKEY_H
