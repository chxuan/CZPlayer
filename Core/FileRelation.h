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
* \file   FileRelation.h
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

#ifndef FILERELATION_H
#define FILERELATION_H

/*! \def FILERELATION_VERSION
*  版本控制宏，对应v3.0.0版本.
*/
#define FILERELATION_VERSION   0x030000

#include <Shlobj.h>
#include <Windows.h>

/*! 文件关联类
 */
class FileRelation
{
public:
    /*! 构造函数.*/
    FileRelation();

    /*! 析构函数.*/
    ~FileRelation();

public:
    /*! 注册文件关联
     * \param wExt 要检测的扩展名(例如: ".wav")
     * \param wAppPath 要关联的应用程序名(例如: "C:/MyApp/MyApp.exe")
     * \param wAppKey wExt扩展名在注册表中的键值(例如: "CZPlayer2.WAV")
     * \param wDefaultIcon 扩展名为wAppPath的图标文件(例如: "D:/visual studio 2010 Projects/spectrum/debug/spectrum.exe,0")
     * \param wDescribe 文件类型描述
     */
	static void registerFileRelation(WCHAR *wExt, WCHAR *wAppPath, WCHAR *wAppKey, 
        WCHAR *wDefaultIcon, WCHAR *wDescribe);

    /*! 取消文件关联
     * \param wExt 要检测的扩展名(例如: ".wav")
     * \param wAppKey ExeName扩展名在注册表中的键值(例如: "CZPlayer2.0.WAV")
     */
    static void cancelFileRelation(WCHAR *wExt, WCHAR *wAppKey);

    /*! 判断文件关联
     * \param wExt 要检测的扩展名(例如: ".wav")
     * \param wAppKey ExeName扩展名在注册表中的键值(例如: "CZPlayer2.0.WAV")
     * \return 返回true: 表示已关联，false: 表示未关联
     */
    static bool checkFileRelation(const WCHAR* wExt, const WCHAR *wAppKey);
};

#endif //FILERELATION_H

