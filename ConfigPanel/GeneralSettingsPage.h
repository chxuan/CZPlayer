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
* \file   GeneralSettingsPage.h
* \author chengxuan   787280310@qq.com
* \date   2015-01-24
* \brief  一般设置页面头文件。
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

#ifndef GENERALSETTINGSPAGE_H
#define GENERALSETTINGSPAGE_H

/*! \def GENERALSETTINGSPAGE_VERSION
*  版本控制宏，对应v3.0.0版本.
*/
#define GENERALSETTINGSPAGE_VERSION   0x030000

#include <QWidget>

class QCheckBox;

/*! 一般设置页面
*/
class GeneralSettingsPage : public QWidget
{
    Q_OBJECT

public:
	/*! 构造函数
	 *  \param parent 父窗口指针，默认为NULL
	 */
    GeneralSettingsPage(QWidget *parent = NULL);

	/*! 析构函数.*/
    ~GeneralSettingsPage();

	/*! 初始页面
	*/
	void init();

	/*! 应用改变
	*/
	void apply();

signals:
	/*! 当常规界面配置信息发生改变时发送该信号
	 */
	void sigGeneralSettingsChanged();

private:
	/*! 创建界面
	*/
	void createUI();

	/*! 信号与槽
	*/
	void createConnect();

private:
    QCheckBox *m_autoPlay;			/*!< 自动播放*/
    QCheckBox *m_autoRun;			/*!< 开机自启动*/
    QCheckBox *m_helloCZPlayer;		/*!< 播放欢迎语*/
    QCheckBox *m_defaultPlayer;		/*!< 设置为默认播放器*/
};

#endif // GENERALSETTINGSPAGE_H
