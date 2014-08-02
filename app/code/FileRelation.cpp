#include "StdAfx.h"
#include "FileRelation.h"


FileRelation::FileRelation(void)
{
}


FileRelation::~FileRelation(void)
{
}

// 注册文件关联  
// wExt: 要检测的扩展名(例如: ".wav")  
// wAppPath: 要关联的应用程序名(例如: "C:/MyApp/MyApp.exe")  
// wAppKey: wExt扩展名在注册表中的键值(例如: "CZPlayer2.WAV")  
// wDefaultIcon: 扩展名为wAppPath的图标文件(例如: "D:/visual studio 2010 Projects/spectrum/debug/spectrum.exe,0")  
// wDescribe: 文件类型描述 
void FileRelation::registerFileRelation( WCHAR *wExt, WCHAR *wAppPath, WCHAR *wAppKey, 
	WCHAR *wDefaultIcon, WCHAR *wDescribe )
{
	WCHAR wTemp[MAX_PATH];  
	HKEY hKey;  

	//创建键为.wav,并且设置值为CZPlayer2.WAV
	RegCreateKey(HKEY_CLASSES_ROOT, wExt, &hKey);							//.wav
	RegSetValue(hKey, _T(""), REG_SZ, wAppKey, lstrlen(wAppKey) + 1);		//CZPlayer2.WAV
	RegCloseKey(hKey);  

	//创建键为CZPlayer2.WAV,并且设置值为WAV文件
	RegCreateKey(HKEY_CLASSES_ROOT, wAppKey, &hKey);						 //CZPlayer2.WAV
	RegSetValue(hKey, _T(""), REG_SZ, wDescribe, lstrlen(wDescribe) + 1);	 //WAV文件
	RegCloseKey(hKey);  

	//设置默认图标(比如:D:/visual studio 2010 Projects/spectrum/debug/spectrum.exe,0)
	wsprintf(wTemp, _T("%s\\DefaultIcon"), wAppKey);						 				
	RegCreateKey(HKEY_CLASSES_ROOT, wTemp, &hKey);  
	RegSetValue(hKey, _T(""), REG_SZ, wDefaultIcon, lstrlen(wDefaultIcon) + 1);  
	RegCloseKey(hKey);  

	//SHCNE_ASSOCCHANGED:修改文件关联   
	SHChangeNotify(SHCNE_ASSOCCHANGED, SHCNF_IDLIST, 0, 0);//该句为刷新系统缓存

	wsprintf(wTemp, _T("%s\\Shell\\Open"), wAppKey);  
	RegCreateKey(HKEY_CLASSES_ROOT, wTemp, &hKey);  
	RegSetValue(hKey, _T(""), REG_SZ, _T("使用 CZPlayer 播放(&O)"), strlen("使用 CZPlayer 播放(&O)") + 1);  
	RegCloseKey(hKey);  

	//设置关联命令(比如:"D:/visual studio 2010 Projects/spectrum/debug/spectrum.exe" "%1")
	wsprintf(wTemp, _T("%s\\Shell\\Open\\Command"), wAppKey);  
	RegCreateKey(HKEY_CLASSES_ROOT, wTemp, &hKey);  
	wsprintf(wTemp ,_T("\"%s\" \"%%1\""), wAppPath);  
	RegSetValue(hKey, _T(""), REG_SZ, wTemp, lstrlen(wTemp) + 1);
	RegCloseKey(hKey);
}

//取消文件关联
void FileRelation::cancelFileRelation( WCHAR *wExt, WCHAR *wAppKey )
{
	WCHAR wTemp[MAX_PATH];  
	HKEY hKey;  

	//创建键为.wav,并且设置值为CZPlayer2.WAV
	RegCreateKey(HKEY_CLASSES_ROOT, wExt, &hKey);		//.wav
	RegSetValue(hKey, _T(""), REG_SZ, _T(""), 2);		//CZPlayer2.WAV
	RegCloseKey(hKey);  

	//设置默认图标
	wsprintf(wTemp, _T("%s\\DefaultIcon"), wAppKey);						 				
	RegCreateKey(HKEY_CLASSES_ROOT, wTemp, &hKey);  
	RegSetValue(hKey, _T(""), REG_SZ, _T(""), 2 + 1);  
	RegCloseKey(hKey);  

	//SHCNE_ASSOCCHANGED:修改文件关联   
	SHChangeNotify(SHCNE_ASSOCCHANGED, SHCNF_IDLIST, 0, 0);//该句为刷新系统缓存 
}

// 检测文件关联情况  
// wExt: 要检测的扩展名(例如: ".wav")  
// wAppKey: ExeName扩展名在注册表中的键值(例如: "CZPlayer2.0.WAV")  
// 返回true: 表示已关联，false: 表示未关联 
bool FileRelation::checkFileRelation( const WCHAR* wExt, const WCHAR *wAppKey )
{
	bool isRelation = false;
	HKEY hKey;

	if(RegOpenKey(HKEY_CLASSES_ROOT, wExt, &hKey)==ERROR_SUCCESS)
	{
		WCHAR wData[MAX_PATH];
		DWORD dwLen = MAX_PATH * sizeof(WCHAR);
		if (RegQueryValueEx(hKey, 0, 0, 0, (BYTE*)wData, &dwLen) == ERROR_SUCCESS)
		{
			if (lstrcmp(wData, wAppKey) == 0)
			{
				isRelation = true;
			}
		}

		RegCloseKey(hKey);
		return isRelation;
	}
	return isRelation;
}