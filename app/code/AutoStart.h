#pragma once

#include <Windows.h>

//开机自启动类
class AutoStart
{
public:
	AutoStart(void);
	~AutoStart(void);

public:
	static void autoStart(WCHAR *value);			//设置开机自启动
	static void cancelAutoStart(WCHAR *value);		//取消开机自启动
	static bool isAutoStart(WCHAR *value);			//判断程序是否自启动
};

