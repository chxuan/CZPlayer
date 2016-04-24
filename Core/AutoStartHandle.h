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
* \file   AutoStartHandle.h
* \author chengxuan   787280310@qq.com
* \date   2015-05-02
* \brief  开机自启动处理器头文件。
* \version 3.0.0
*
* \verbatim
* 历史
*          3.0.0   创建,
*          2015-05-02    by chengxuan
*
* \endverbatim
*
*/

#ifndef AUTOSTARTHANDLE_H
#define AUTOSTARTHANDLE_H

/*! \def AUTOSTARTHANDLE_VERSION
*  版本控制宏，对应v3.0.0版本.
*/
#define AUTOSTARTHANDLE_VERSION   0x030000

#include <QObject>

/*! 开机自启动处理器
 */
class AutoStartHandle : public QObject
{
    Q_OBJECT

public:
    /*! 构造函数
     *  \param parent 父窗口指针，默认为NULL
     */
    AutoStartHandle(QObject *parent = NULL);

    /*! 析构函数.*/
    ~AutoStartHandle();

private slots:
    /*! 开机自启动
     *  \param isAutoStart true:开机自启动，false:取消开机自启动
     */
    void handleAutoStart(bool isAutoStart);

private:
    /*! 信号与槽
     */
    void createConnect();
};

#endif // AUTOSTARTHANDLE_H
