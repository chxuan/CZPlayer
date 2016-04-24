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
* \file   UpdatePage.h
* \author chengxuan   787280310@qq.com
* \date   2015-01-24
* \brief  软件更新页面头文件。
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

#ifndef UPDATEPAGE_H
#define UPDATEPAGE_H

/*! \def UPDATEPAGE_VERSION
*  版本控制宏，对应v3.0.0版本.
*/
#define UPDATEPAGE_VERSION   0x030000

#include <QWidget>

class QLabel;
class QPushButton;

/*! 软件更新页面
*/
class UpdatePage : public QWidget
{
	Q_OBJECT

public:
	/*! 构造函数
	*  \param parent 父窗口指针，默认为NULL
	*/
	UpdatePage(QWidget *parent = NULL);

	/*! 析构函数.*/
	~UpdatePage(void);

private slots:
	/*! 更新软件
	 */
	void handleUpdate();

private:
	/*! 创建界面
	 */
	void createUI();

	/*! 信号与槽
	 */
	void createConnect();

private:
	QLabel *m_updateInfoLabel;		/*!< 更新信息*/
	QPushButton *m_updateButton;	/*!< 开始更新按钮*/
};

#endif //UPDATEPAGE_H

