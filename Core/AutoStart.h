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
* \file   AutoStart.h
* \author chengxuan   787280310@qq.com
* \date   2015-05-01
* \brief  开机自启动头文件。
* \version 3.0.0
*
* \verbatim
* 历史
*          3.0.0   创建,
*          2015-05-01    by chengxuan
*
* \endverbatim
*
*/

#ifndef AUTOSTART_H
#define AUTOSTART_H

/*! \def AUTOSTART_VERSION
*  版本控制宏，对应v3.0.0版本.
*/
#define AUTOSTART_VERSION   0x030000

#include <Windows.h>

/*! 开机自启动类
 */
class AutoStart
{
public:
    /*! 构造函数.*/
    AutoStart();

    /*! 析构函数.*/
    ~AutoStart();

public:
    /*! 设置开机自启动
     * \param value 注册表值
     */
    static void autoStart(WCHAR *value);

    /*! 取消开机自启动
     * \param value 注册表值
     */
    static void cancelAutoStart(WCHAR *value);

    /*! 判断程序是否自启动
     * \param value 注册表值
     * \return true代表开机自启动
     */
    static bool isAutoStart(WCHAR *value);
};

#endif // AUTOSTART_H

