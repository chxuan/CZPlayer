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
* \file   DownloadSettingsPage.h
* \author chengxuan   787280310@qq.com
* \date   2015-01-24
* \brief  下载设置页面头文件。
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

#ifndef DOWNLOADSETTINGSPAGE_H
#define DOWNLOADSETTINGSPAGE_H

/*! \def DOWNLOADSETTINGSPAGE_VERSION
*  版本控制宏，对应v3.0.0版本.
*/
#define DOWNLOADSETTINGSPAGE_VERSION   0x030000

#include <QWidget>
#include <QString>

class QLabel;
class QLineEdit;
class QPushButton;

/*! 下载设置页面
*/
class DownloadSettingsPage : public QWidget
{
    Q_OBJECT

public:
	/*! 构造函数
	 *  \param parent 父窗口指针，默认为NULL
	 */
    DownloadSettingsPage(QWidget *parent = NULL);

	/*! 析构函数.*/
    ~DownloadSettingsPage();

	/*! 初始页面
	 */
	void init();

	/*! 应用改变
	 */
	void apply();

signals:
	/*! 当下载界面配置信息发生改变时发送该信号
	 */
	void sigDownloadSettingsChanged();

private slots:
	/*! 音乐目录改变
	 */
    void handleMusicDirChanged();

	/*! 专辑目录改变
	 */
	void handleAlbumDirChanged();

	/*! 歌词目录改变
	 */
	void handleLrcDirChanged();

private:
	/*! 创建界面
	*/
	void createUI();

	/*! 信号与槽
	*/
	void createConnect();

private:
	QLabel *m_musicDirLabel;		 /*!< 音乐目录*/
    QLineEdit *m_musicEdit;			 /*!< 音乐目录*/
    QPushButton *m_musicButton;	     /*!< 音乐目录*/

    QLabel *m_albumDirLabel;		 /*!< 专辑目录*/
    QLineEdit *m_albumEdit;			 /*!< 专辑目录*/
    QPushButton *m_albumButton;		 /*!< 专辑目录*/

    QLabel *m_lrcFileLabel;			 /*!< 歌词目录*/
    QLineEdit *m_lrcFileEdit;		 /*!< 歌词目录*/
    QPushButton *m_lrcFileButton;	 /*!< 歌词目录*/
};

#endif // DOWNLOADSETTINGSPAGE_H
