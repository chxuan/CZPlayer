#pragma once

#include <Shlobj.h>
#include <Windows.h>

//文件关联类
class FileRelation
{
public:
	FileRelation(void);
	~FileRelation(void);

public:
	static void registerFileRelation(WCHAR *wExt, WCHAR *wAppPath, WCHAR *wAppKey, 
		WCHAR *wDefaultIcon, WCHAR *wDescribe);		//注册文件关联

	static void cancelFileRelation(WCHAR *wExt, WCHAR *wAppKey);	//取消文件关联

	static bool checkFileRelation(const WCHAR* wExt, const WCHAR *wAppKey);		//判断文件关联
};

