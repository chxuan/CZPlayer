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
* \file   AutoStart.cpp
* \author chengxuan   787280310@qq.com
* \date   2015-05-01
* \brief  开机自启动实现文件。
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

#include "AutoStart.h"
#include <tchar.h>
#include <QString>
#ifdef CZ_DEBUG
#include <QDebug>
#endif

/*****************************************************************************
全局变量定义
*****************************************************************************/
/*! 打开注册表:路径如下*/
const QString Path = "Software\\Microsoft\\Windows\\CurrentVersion\\Run";

AutoStart::AutoStart()
{
    /*Do nothing.*/
}

AutoStart::~AutoStart()
{
    /*Do nothing.*/
}

void AutoStart::autoStart(WCHAR *value)
{
	WCHAR system[MAX_PATH];		//系统目录路径
	WCHAR filePath[MAX_PATH];	//要开机运行的文件的完整路径
	WCHAR fileName[MAX_PATH];	//文件名(\CZPlayer.exe)

	//得到当前执行文件的全路径
	HMODULE hModule = GetModuleHandle(NULL);
	GetModuleFileName(hModule, filePath, sizeof(filePath));

	//得到文件名
	for (int i = lstrlen(filePath) - 1; i >= 0; --i)
	{
		if (filePath[i] == '\\')
		{
			lstrcpy(fileName, &filePath[i]);
			break;
		}
        /*Else do nothing, and continue.*/
	}

	//得到系统文件所在目录的路径，如c:\windows\system32
	GetSystemDirectory(system, sizeof(system));

	//形成要复制到的全路径，如c:\windows\system32\CZPlayer.exe
	wcscat_s(system, fileName);

	//自我复制到目标路径
	CopyFile(filePath, system, false);

	//写入注册表，以便开机自动运行
	HKEY hKey;

    //打开注册表
    if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, (wchar_t*)Path.utf16(), 0, KEY_SET_VALUE, &hKey ) == ERROR_SUCCESS)
	{
		//新增一个值,名称随意命名,值为要开机运行的文件的完整路径
		RegSetValueEx(hKey, value, 0, REG_SZ, (const unsigned char*)system, sizeof(system));
		//关闭注册表:
		RegCloseKey(hKey);

#ifdef CZ_DEBUG
        qDebug() << "设置开机自启动成功！";
#endif
	}
	else
	{
#ifdef CZ_DEBUG
        qDebug() << "打开注册表失败！";
        qDebug() << __FILE__ << __FUNCTION__ << __LINE__;
#endif
	}
}

void AutoStart::cancelAutoStart(WCHAR *value)
{
	WCHAR system[MAX_PATH];		//系统目录路径
	WCHAR filePath[MAX_PATH];	//要开机运行的文件的完整路径
	WCHAR fileName[MAX_PATH];	//文件名(\CZPlayer.exe)

	//得到当前执行文件的全路径
	HMODULE hModule = GetModuleHandle(NULL);
	GetModuleFileName(hModule, filePath, sizeof(filePath));

	//得到文件名
	for (int i = lstrlen(filePath) - 1; i >= 0; --i)
	{
		if (filePath[i] == '\\')
		{
			lstrcpy(fileName, &filePath[i]);
			break;
		}
        /*Else do nothing, and continue.*/
	}

	//得到系统文件所在目录的路径，如c:\windows\system32
	GetSystemDirectory(system, sizeof(system));

	//形成要复制到的全路径，如c:\windows\system32\CZPlayer.exe
	wcscat_s(system, fileName);

	HKEY hKey;
	DeleteFile(filePath);	//删除对应的文件
    //打开注册表
    if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, (wchar_t*)Path.utf16(), 0, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS)
	{
		//删除键值
		RegDeleteValue(hKey, value);
		//关闭注册表
		RegCloseKey(hKey);

#ifdef CZ_DEBUG
        qDebug() << "取消开机自启动成功！";
#endif
	}
	else
	{
#ifdef CZ_DEBUG
        qDebug() << "打开注册表失败！";
        qDebug() << __FILE__ << __FUNCTION__ << __LINE__;
#endif
	}
}

bool AutoStart::isAutoStart(WCHAR *value)
{
	WCHAR system[MAX_PATH];		//系统目录路径
	WCHAR filePath[MAX_PATH];	//要开机运行的文件的完整路径
	WCHAR fileName[MAX_PATH];	//文件名(\CZPlayer.exe)

	//得到当前执行文件的全路径
	HMODULE hModule = GetModuleHandle(NULL);
	GetModuleFileName(hModule, filePath, sizeof(filePath));

	//得到文件名
	for (int i = lstrlen(filePath) - 1; i >= 0; --i)
	{
		if (filePath[i] == '\\')
		{
			lstrcpy(fileName, &filePath[i]);
			break;
		}
        /*Else do nothing, and continue.*/
	}

	//得到系统文件所在目录的路径，如c:\windows\system32
	GetSystemDirectory(system, sizeof(system));

	//形成要复制到的全路径，如c:\windows\system32\CZPlayer.exe
	wcscat_s(system, fileName);

	HKEY hKey;
	bool bIsAutoStart =false;
    //打开注册表
    if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, (wchar_t*)Path.utf16(), 0, KEY_READ, &hKey) == ERROR_SUCCESS)
	{
		WCHAR wData[MAX_PATH];
		DWORD dwLen = MAX_PATH * sizeof(WCHAR);
		if (RegQueryValueEx(hKey, value, 0, 0, (BYTE*)wData, &dwLen) == ERROR_SUCCESS)
		{
			bIsAutoStart =  true;				//该程序设置了自启动
		}
		else
		{
			bIsAutoStart =  false;				//该程序没有设置自启动
		}
	}
	else
	{
#ifdef CZ_DEBUG
        qDebug() << "打开注册表失败！";
        qDebug() << __FILE__ << __FUNCTION__ << __LINE__;
#endif
	}

	RegCloseKey(hKey);
	return bIsAutoStart;
}
