#pragma once

#include "head.h"
#include "MusicInfoEntity.h"

class LoadMusicThread : public QThread
{
	Q_OBJECT

public:
	LoadMusicThread(void);
	~LoadMusicThread(void);

public:
	//打开音乐文件 设置参数
	void setParam(const QStringList &musicList, map<QString, int> *mapMusicRows, map<QString, QString> *mapMusicFilePath, QTableWidget *tableWidget);
	//拖动音乐文件或音乐格式文件 设置参数
	void setParam(const QList<QUrl> &urls, map<QString, int> *mapMusicRows, map<QString, QString> *mapMusicFilePath, QTableWidget *tableWidget);
	//加载音乐格式文件或文件夹 设置参数
	void setParam(const QString &strFileOrDir, map<QString, int> *mapMusicRows, map<QString, QString> *mapMusicFilePath, QTableWidget *tableWidget);
	//从数据库读取音乐文件 设置参数
	void setParam(map<QString, int> *mapMusicRows, map<QString, QString> *mapMusicFilePath, QTableWidget *tableWidget);

protected:
	virtual void run();

signals:
	void sig_LoadMusicListFinished();						//加载完播放列表
	void sig_LoadMusicListError();							//加载播放列表错误

private:
	void startFunc();
	void loadMusicList(const QString &filePath);			//加载歌曲列表
	void loadMusicList(const QStringList &list);
	void loadMusicList(const MusicInfoEntity &entity);

	void openMusicModeLoad();								//打开音乐文件加载模式
	void dragEnterMusicModeLoad();							//拖动音乐文件或音乐格式文件加载模式
	void openMusicFormatFileLoad();							//打开音乐格式文件加载模式
	void readFromDBModeLoad();								//数据库读取音乐文件加载模式
	void openMusicDirectoryMode();							//歌曲文件夹加载模式

private:
	QStringList m_MusicList;
	QList<QUrl> m_urls;
	QString m_strMusicFormatFile;
	QString m_strMusicDirectory;
	QTableWidget *m_TableWidget;

	map<QString, int> *m_mapMusicRows;						//key：歌曲名，value：行号
	map<QString, QString> *m_mapMusicFilePath;				//key：歌曲名，value：音乐路径

	//加载音乐列表模式
	enum _LoadMusicMode
	{
		openMusic = 1,
		dragEnterMusic = 2,
		openMusicFormatFile = 3,
		readFromDB = 4,
		openDirectory = 5
	};

	_LoadMusicMode loadMusicMode;
};

