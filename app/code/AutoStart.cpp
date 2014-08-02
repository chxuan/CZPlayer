#include "StdAfx.h"
#include "AutoStart.h"

#include <iostream>
using namespace std;

AutoStart::AutoStart(void)
{
}


AutoStart::~AutoStart(void)
{
}

//设置开机自启动
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
	}

	//得到系统文件所在目录的路径，如c:\windows\system32
	GetSystemDirectory(system, sizeof(system));

	//形成要复制到的全路径，如c:\windows\system32\CZPlayer.exe
	wcscat_s(system, fileName);

	//自我复制到目标路径
	CopyFile(filePath, system, false);

	//写入注册表，以便开机自动运行
	HKEY hKey;

	//打开注册表:路径如下
	//HEKY_LOCAL_MACHINE\Software\Microsoft\Windows\CurrentVersion\Run
	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("Software\\Microsoft\\Windows\\CurrentVersion\\Run"), 0, KEY_SET_VALUE, &hKey ) == ERROR_SUCCESS)
	{
		//新增一个值,名称随意命名,值为要开机运行的文件的完整路径
		RegSetValueEx(hKey, value, 0, REG_SZ, (const unsigned char*)system, sizeof(system));
		//关闭注册表:
		RegCloseKey(hKey);

		cout << "设置开机自启动成功！" << endl;
	}
	else
	{
		cout << "打开注册表失败" << endl;
	}
}

//取消开机自启动
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
	}

	//得到系统文件所在目录的路径，如c:\windows\system32
	GetSystemDirectory(system, sizeof(system));

	//形成要复制到的全路径，如c:\windows\system32\CZPlayer.exe
	wcscat_s(system, fileName);

	HKEY hKey;
	DeleteFile(filePath);	//删除对应的文件
	//打开注册表:路径如下
	//HEKY_LOCAL_MACHINE\Software\Microsoft\Windows\CurrentVersion\Run
	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("Software\\Microsoft\\Windows\\CurrentVersion\\Run"), 0, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS)
	{
		//删除键值
		RegDeleteValue(hKey, value);
		//关闭注册表
		RegCloseKey(hKey);

		cout << "取消开机自启动成功！" << endl;
	}
	else
	{
		cout << "打开注册表失败" << endl;
	}
}

//判断播放器是否自启动
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
	}

	//得到系统文件所在目录的路径，如c:\windows\system32
	GetSystemDirectory(system, sizeof(system));

	//形成要复制到的全路径，如c:\windows\system32\CZPlayer.exe
	wcscat_s(system, fileName);

	HKEY hKey;
	bool bIsAutoStart =false;
	//打开注册表:路径如下
	//HEKY_LOCAL_MACHINE\Software\Microsoft\Windows\CurrentVersion\Run
	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("Software\\Microsoft\\Windows\\CurrentVersion\\Run"), 0, KEY_READ, &hKey) == ERROR_SUCCESS)
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
		cout << "打开注册表失败" << endl;
	}

	RegCloseKey(hKey);
	return bIsAutoStart;
}
