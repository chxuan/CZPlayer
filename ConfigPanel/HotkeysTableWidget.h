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
* \file   HotKeysTableWidget.h
* \author chengxuan   787280310@qq.com
* \date   2015-01-24
* \brief  热键页面头文件。
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

#ifndef HOTKEYSTABLEWIDGET_H
#define HOTKEYSTABLEWIDGET_H

/*! \def HOTKEYSTABLEWIDGET_VERSION
*  版本控制宏，对应v3.0.0版本.
*/
#define HOTKEYSTABLEWIDGET_VERSION   0x030000

#include <QTableWidget>

/*! 热键页面
*/
class HotKeysTableWidget : public QTableWidget
{
    Q_OBJECT

public:
	/*! 构造函数
	 *  \param parent 父窗口指针，默认为NULL
	 */
    HotKeysTableWidget(QWidget *parent = NULL);

	/*! 析构函数.*/
    ~HotKeysTableWidget();
};

#endif // HOTKEYSTABLEWIDGET_H
