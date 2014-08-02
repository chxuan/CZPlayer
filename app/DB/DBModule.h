#ifndef DBMODULE_H
#define DBMODULE_H

#include "head.h"
#include "MusicListSaveFormatEntity.h"
#include "MusicInfoEntity.h"

class DBModule : public QObject
{
	Q_OBJECT

public:
	DBModule();
	~DBModule();

public:
	static void initDB();																			//初始化数据库
	static bool createConnection();																	//连接数据库
	static void createTable();																		//创建表
	static bool readFilePath(QStringList &list);													//从数据库中读出数据显示到音乐里列表中
	static bool readMusicListSaveFormatEntity(vector<MusicListSaveFormatEntity> &vec);				//从数据库中读出数据显示到m3u文件中
	static bool readMusicInfoEntity(vector<MusicInfoEntity> &vec);									//从数据库读取音乐信息
	static bool clearDB();																			//清空数据库
	static bool delLine(const QString &fileName);													//删除一条记录
	static bool insertLine(const QString &fileName, const QString &time, const QString &filePath);	//插入一行记录
	static bool updateAlbumUrlPath(const QString &fileName, const QString &albumUrlPath);			//更新专辑url
	static bool readAlbumUrlPathFromDB(QString &fileName, QString &albumUrlPath);					//从数据库中读出歌曲名对应的专辑url
};

#endif //DBMODULE_H