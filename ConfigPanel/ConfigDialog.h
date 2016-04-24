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
* \file   ConfigDialog.h
* \author chengxuan   787280310@qq.com
* \date   2015-01-24
* \brief  全局配置头文件。
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

#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H

/*! \def CONFIGDIALOG_VERSION
*  版本控制宏，对应v3.0.0版本.
*/
#define CONFIGDIALOG_VERSION   0x030000

#include <QDialog>

class GeneralSettingsPage;
class DownloadSettingsPage;
class LrcSettingsPage;
class HotKeysSettingsPage;
class UpdatePage;
class QListWidget;
class QPushButton;
class QStackedWidget;
class QListWidgetItem;
class QCloseEvent;

/*! 配置对话框
*/
class ConfigDialog : public QDialog
{
    Q_OBJECT

public:
	/*! 构造函数
	 *  \param parent 父窗口指针，默认为NULL
	 */
    ConfigDialog(QWidget *parent = NULL);

	/*! 析构函数.*/
    ~ConfigDialog();

	/*! 初始化配置界面
	*/
	void init();

protected:
	/*! 虚函数，重新实现closeEvent函数
	*  \param event QPaintEvent类型
	*/
	virtual void closeEvent(QCloseEvent *event);

signals:
	/*! 配置对话框关闭
	 */
	void sigConfigDialogClosed();

private slots:
	/*! 页面改变
	 *  \param current 当前页面
	 *  \param previous 之前页面
	 */
    void handleChangePage(QListWidgetItem *current, QListWidgetItem *previous);

	/*! 确定
	 */
    void handleOkFunc();								

	/*! 应用
	 */
    void handleAppFunc();	

	/*! 基本设置页面改变
	 */
	void handleGeneralSettingsChanged();

	/*! 下载设置页面改变
	 */
	void handleDownloadSettingsChanged();

	/*! 歌词设置页面改变
	 */
	void handleLrcSettingsChanged();

	/*! 热键设置页面改变
	 */
	void handleHotKeysSettingsChanged();

private:
	/*! 创建界面
	*/
	void createUI();

	/*! 创建图标
	*/
	void createIcons();

	/*! 信号与槽
	*/
	void createConnect();

private:
	QListWidget *m_contentsWidget;					/*!< 图标widget*/
    QStackedWidget *m_pagesWidget;					/*!< 页面widget*/

    QPushButton *m_okButton;						/*!< 确定按钮*/				
    QPushButton *m_cancelButton;					/*!< 取消按钮*/
    QPushButton *m_appButton;						/*!< 应用按钮*/

    GeneralSettingsPage *m_generalSettingsPage;		/*!< 基本设置*/
    DownloadSettingsPage *m_downloadSettingsPage;	/*!< 下载设置*/
    LrcSettingsPage *m_lrcSettingsPage;				/*!< 歌词设置*/
    HotKeysSettingsPage *m_hotKeysSettingsPage;		/*!< 热键设置*/
	UpdatePage *m_updatePage;						/*!< 更新页面*/

	bool m_isGeneralSettingsChanged;				/*!< 基本设置是否改变*/
	bool m_isDownloadSettingsChanged;				/*!< 下载设置是否改变*/
	bool m_isLrcSettingsChanged;					/*!< 歌词设置是否改变*/
	bool m_isHotKeysSettingsChanged;				/*!< 热键设置是否改变*/
};

#endif // CONFIGDIALOG_H
