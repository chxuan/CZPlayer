#ifndef CLIENTBASEOPERATING_H
#define CLIENTBASEOPERATING_H

#include "head.h"

class ClientBaseOperating : public QObject
{
	Q_OBJECT

public:
	ClientBaseOperating();
	~ClientBaseOperating();

public:
	static void initConfig();					//初始化配置
	static void initConfigDialog();				//初始化设置对话框

	static void initLrcPropery();				//设置歌词属性
	static void initHotKeyPropery();			//开启全局热键
	static QString initPlayMode();				//初始化播放模式

private:
	static void playVoice();							//播放问候语
	static void touchConfigFile();						//判断配置文件是否存在
	static void mkdirDownloadDir();						//创建一个下载歌曲目录
	static void mkdirAlbumDir();						//创建一个文件夹来存储下载的专辑
	static void mkdirLrcDir();							//创建一个下载歌词目录

	static void initGeneralSettings();			//初始化常规设置页
	static void initDownloadConfig();			//初始化下载页
	static void initLrcConfig();				//初始化歌词页
	static void initHotKeyConfig();				//初始化全局热键页
};

#endif // CLIENTBASEOPERATING_H