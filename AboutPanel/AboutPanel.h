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
 * \file   AboutPanel.h
 * \author chengxuan   787280310@qq.com
 * \date   2014-12-22
 * \brief  关于面板头文件。
 * \version 3.0.0
 *
 * \verbatim
 * 历史
 *          3.0.0   创建,
 *          2014-12-22    by chengxuan
 *
 * \endverbatim
 *
 */

#ifndef ABOUTPANEL_H
#define ABOUTPANEL_H

/*! \def ABOUTPANEL_VERSION
 *  版本控制宏，对应v3.0.0版本.
 */
#define ABOUTPANEL_VERSION   0x030000

#include <QDialog>

class QLabel;
class QPushButton;
class QTabWidget;
class AboutPage;
class AuthorPage;
class ThanksPage;

/*! 关于面板
 */
class AboutPanel : public QDialog
{
	Q_OBJECT

public:
	/*! 构造函数
	 *  \param parent 父窗口指针，默认为NULL
     */
	AboutPanel(QWidget *parent = NULL);

	/*! 析构函数.*/
	~AboutPanel(void);

	/*! 初始化关于对话框.
	 */
	void init();

private:
	QTabWidget *m_tabWidget;		/*!< tab页widget*/
	QPushButton *m_okButton;		/*!< 确定按钮*/
	QLabel *m_logoLabel;			/*!< logo*/

	AboutPage *m_aboutPage;			/*!< 关于页面*/
	AuthorPage *m_authorPage;		/*!< 作者页面*/
	ThanksPage *m_thanksPage;		/*!< 感谢页面*/
};

#endif // ABOUTPANEL_H

