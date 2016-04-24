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
 * \file   AuthorPage.h
 * \author chengxuan   787280310@qq.com
 * \date   2014-12-22
 * \brief  作者页面头文件。
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

#ifndef AUTHORPAGE_H
#define AUTHORPAGE_H

/*! \def AUTHORPAGE_VERSION
 *  版本控制宏，对应v3.0.0版本.
 */
#define AUTHORPAGE_VERSION   0x030000

#include <QWidget>

class QLabel;

/*! 作者页面
 */
class AuthorPage : public QWidget
{
	Q_OBJECT

public:
	/*! 构造函数
	 *  \param parent 父窗口指针，默认为NULL
     */
	AuthorPage(QWidget *parent = NULL);

	/*! 析构函数.*/
	~AuthorPage(void);

private:
	QLabel *m_mainAuthorLabel;		/*!< 核心开发*/
	QLabel *m_plugAuthorLabel;		/*!< 插件开发*/
	QLabel *m_artAuthorLabel;		/*!< 美术*/
};

#endif //AUTHORPAGE_H

