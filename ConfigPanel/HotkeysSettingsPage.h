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
* \file   HotKeysSettingsPage.h
* \author chengxuan   787280310@qq.com
* \date   2015-01-24
* \brief  热键设置页面头文件。
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

#ifndef HOTKEYSSETTINGSPAGE_H
#define HOTKEYSSETTINGSPAGE_H

/*! \def HOTKEYSSETTINGSPAGE_VERSION
*  版本控制宏，对应v3.0.0版本.
*/
#define HOTKEYSSETTINGSPAGE_VERSION   0x030000

#include <QWidget>
#include <QString>

class QTableWidget;
class QPushButton;
class QTableWidgetItem;
class EditHotkeyDialog;

/*! 热键设置页面
*/
class HotKeysSettingsPage : public QWidget
{
    Q_OBJECT

public:
	/*! 构造函数
	 *  \param parent 父窗口指针，默认为NULL
	 */
    HotKeysSettingsPage(QWidget *parent = NULL);

	/*! 析构函数.*/
    ~HotKeysSettingsPage();

	/*! 初始页面
	*/
	void init();

	/*! 应用改变
	*/
	void apply();

signals:
	/*! 当热键界面配置信息发生改变时发送该信号
	 */
	void sigHotKeysSettingsChanged();

private slots:
	/*! 恢复默认方案
	 */
	void handleRecoverDefaultSetting();

	/*! 编辑热键
	*/
	void handleEditHotKeys();

	/*! 获取选中行
	 *  \param row 行号
	 */
	void handleTableClicked(const int &index);

	/*! 热键编辑完成
	 * \param hotKeyName 热键名
	 * \param hotKey 热键
	 */
	void handleEditHotkeyFinished(const QString &hotKeyName, const QString &hotKey);

	/*! 双击热键列表编辑热键
	 *  \param row 热键列表行
	 *  \param column 热键列表列
	 */
	void handleTableDoubleClicked(const int &row, const int &column);

private:
	/*! 创建界面
	*/
	void createUI();

	/*! 信号与槽
	*/
	void createConnect();

    /*! 设置表格风格
     */
    void setTableWidgetStyle();

private:
    QTableWidget *m_hotKeysTableWidget;             	/*!< 热键表*/
	EditHotkeyDialog *m_editHotkeyDialog;				/*!< 热键编辑对话框*/
    QPushButton *m_editButton;							/*!< 编辑热键*/
    QPushButton *m_recoverDefaultSettingButton;			/*!< 恢复默认*/

    QTableWidgetItem *m_settingsHotKey;					/*!< 选项设置*/
    QTableWidgetItem *m_minToTrayHotKey;				/*!< 显示/隐藏主界面*/
    QTableWidgetItem *m_openMinWidgetHotKey;			/*!< 迷你模式/正常窗口*/
    QTableWidgetItem *m_openMusicFileHotKey;			/*!< 打开音乐文件*/
    QTableWidgetItem *m_openMediaWidgetHotKey;          /*!< 显示/隐藏媒体面板*/
    QTableWidgetItem *m_openListWidgetHotKey;			/*!< 显示/隐藏歌曲列表*/
    QTableWidgetItem *m_openLrcHotKey;					/*!< 显示/隐藏桌面歌词*/
    QTableWidgetItem *m_pauseHotKey;					/*!< 播放/暂停*/
    QTableWidgetItem *m_lastHotKey;						/*!< 上一首*/
    QTableWidgetItem *m_nextHotKey;						/*!< 下一首*/

	int m_row;											/*!< 选中行*/
};

#endif // HOTKEYSSETTINGSPAGE_H
