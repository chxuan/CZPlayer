#include "StdAfx.h"
#include "LoadMusicThread.h"
#include "DBModule.h"
#include "wavfile.h"

LoadMusicThread::LoadMusicThread(void)
{

}

LoadMusicThread::~LoadMusicThread(void)
{

}

void LoadMusicThread::run()
{
	this->startFunc();
}

//打开音乐文件 设置参数
void LoadMusicThread::setParam( const QStringList &musicList, map<QString, int> *mapMusicRows, map<QString, QString> *mapMusicFilePath, QTableWidget *tableWidget )
{
	m_MusicList = musicList;
	m_mapMusicRows = mapMusicRows;
	m_mapMusicFilePath = mapMusicFilePath;
	m_TableWidget = tableWidget;
	loadMusicMode = LoadMusicThread::openMusic;				//打开音乐文件加载模式
}

//拖动音乐文件或音乐格式文件 设置参数
void LoadMusicThread::setParam( const QList<QUrl> &urls, map<QString, int> *mapMusicRows, map<QString, QString> *mapMusicFilePath, QTableWidget *tableWidget )
{
	m_urls = urls;
	m_mapMusicRows = mapMusicRows;
	m_mapMusicFilePath = mapMusicFilePath;
	m_TableWidget = tableWidget;
	loadMusicMode = LoadMusicThread::dragEnterMusic;		//拖动音乐文件或音乐格式文件加载模式
}

//加载音乐格式文件或文件夹 设置参数
void LoadMusicThread::setParam( const QString &strFileOrDir, map<QString, int> *mapMusicRows, map<QString, QString> *mapMusicFilePath, QTableWidget *tableWidget )
{
	QFileInfo fileInfo(strFileOrDir);
	if (fileInfo.isFile())
	{
		m_strMusicFormatFile = strFileOrDir;
		m_mapMusicRows = mapMusicRows;
		m_mapMusicFilePath = mapMusicFilePath;
		m_TableWidget = tableWidget;
		loadMusicMode = LoadMusicThread::openMusicFormatFile;	//音乐格式文件加载模式
	}
	else if (fileInfo.isDir())
	{
		m_strMusicDirectory = strFileOrDir;
		m_mapMusicRows = mapMusicRows;
		m_mapMusicFilePath = mapMusicFilePath;
		m_TableWidget = tableWidget;
		loadMusicMode = LoadMusicThread::openDirectory;			//歌曲文件夹加载模式
	}
}

//从数据库读取音乐文件 设置参数
void LoadMusicThread::setParam( map<QString, int> *mapMusicRows, map<QString, QString> *mapMusicFilePath, QTableWidget *tableWidget )
{
	m_mapMusicRows = mapMusicRows;
	m_mapMusicFilePath = mapMusicFilePath;
	m_TableWidget = tableWidget;
	loadMusicMode = LoadMusicThread::readFromDB;			//数据库读取音乐文件加载模式
}

void LoadMusicThread::startFunc()
{
	if (loadMusicMode == LoadMusicThread::openMusic)
	{
		this->openMusicModeLoad();
	}
	else if (loadMusicMode == LoadMusicThread::dragEnterMusic)
	{
		this->dragEnterMusicModeLoad();
	}
	else if (loadMusicMode == LoadMusicThread::openMusicFormatFile)
	{
		this->openMusicFormatFileLoad();
	}
	else if (loadMusicMode == LoadMusicThread::readFromDB)
	{
		this->readFromDBModeLoad();
	}
	else if (loadMusicMode == LoadMusicThread::openDirectory)
	{
		this->openMusicDirectoryMode();
	}

	emit sig_LoadMusicListFinished();	//加载完播放列表
	this->deleteLater();
}

//加载歌曲列表
void LoadMusicThread::loadMusicList( const QString &filePath )
{
	//序号
	QTableWidgetItem *rowItem = new QTableWidgetItem;
	rowItem->setTextAlignment(Qt::AlignCenter);
	rowItem->setFont(QFont("微软雅黑", 10));

	//歌曲
	QString fileName = QFileInfo(filePath).baseName();
	QTableWidgetItem *fileNameItem = new QTableWidgetItem(fileName);
	fileNameItem->setTextAlignment(Qt::AlignCenter);
	fileNameItem->setFont(QFont("微软雅黑", 10));

	//时间
	WavFile wavFile;
	int nMusicTime = wavFile.getMusicTime(filePath);
	QTime time(0, nMusicTime / 60, nMusicTime % 60);
	QTableWidgetItem *timeItem = new QTableWidgetItem(time.toString("mm:ss"));
	timeItem->setTextAlignment(Qt::AlignCenter);
	timeItem->setFont(QFont("微软雅黑", 10));

	//插入播放列表
	int currentRows = m_TableWidget->rowCount();//返回列表中的行数
	m_TableWidget->insertRow(currentRows);//从播放列表中的当前行插入
	rowItem->setText(QString::number(currentRows + 1));
	m_TableWidget->setItem(currentRows, 0, rowItem);
	m_TableWidget->setItem(currentRows, 1, fileNameItem);
	m_TableWidget->setItem(currentRows, 2, timeItem);

	//更改时间列间隔
	if (currentRows + 1 <= 12)
	{
		m_TableWidget->horizontalHeader()->resizeSection(3, 80);
	}
	else
	{
		m_TableWidget->horizontalHeader()->resizeSection(3, 65);
	}

	//将音乐信息加入vector
	m_mapMusicRows->insert(make_pair(fileName, currentRows));
	m_mapMusicFilePath->insert(make_pair(fileName, filePath));

	//放入数据库
	DBModule::insertLine(fileName, time.toString("mm:ss"), filePath);
}

//打开音乐文件加载模式
void LoadMusicThread::openMusicModeLoad()
{
	foreach (QString filePath, m_MusicList)
	{
		this->loadMusicList(filePath);
	}
}

//拖动音乐文件或音乐格式文件加载模式
void LoadMusicThread::dragEnterMusicModeLoad()
{
	foreach (QUrl url, m_urls)
	{
		QString filePath = url.toLocalFile();

		QFileInfo fileInfo(filePath);
		
		if (fileInfo.isFile())	//加载文件
		{
			QString strSuffixName = filePath.right(3);//得到后缀名
			//加载播放列表
			if (strSuffixName == "m3u" || strSuffixName == "M3U" || strSuffixName == "pls" || strSuffixName == "PLS")
			{
				char buf[256];
				string sBuf;
				int i = 0;
				int n = 1;
				ifstream file(filePath.toStdString());
				while (!file.eof())
				{
					file.getline(buf, 256, '\n');
					sBuf = buf;

					if (i == 0)		//读取表头
					{
						if (sBuf != "#EXTM3U" && sBuf != "[playlist]")
						{
							emit sig_LoadMusicListError();
							this->deleteLater();
						}
					}
					else
					{
						if ((strSuffixName == "m3u" || strSuffixName == "M3U") && i % 2 == 0)
						{
							this->loadMusicList(QString::fromStdString(sBuf));	//加载歌曲列表
						}
						else if ((strSuffixName == "pls" || strSuffixName == "PLS") && i == 1 + (n - 1) * 3)
						{
							if (!QString::fromStdString(sBuf).contains("NumberOfEntries"))
							{
								++n;
								if (QString::fromStdString(sBuf).contains("="))
								{
									QStringList list = QString::fromStdString(sBuf).split("=");
									QString filePath = list.at(1);
									this->loadMusicList(filePath);	//加载歌曲列表
								}
							}
						}
					}
					++i;
				}
				file.close();
			}
			else	//加载单首歌曲
			{
				this->loadMusicList(filePath);
			}
		}
		else if (fileInfo.isDir())	//加载文件夹
		{
			m_strMusicDirectory = filePath;
			this->openMusicDirectoryMode();
		}
	}
}

//打开音乐格式文件加载模式
void LoadMusicThread::openMusicFormatFileLoad()
{
	QString strSuffixName = m_strMusicFormatFile.right(3);//得到后缀名
	////获取当前媒体源列表的大小
	//int index = sources.size();
	char buf[256];
	string sBuf;
	int i = 0;
	int n = 1;
	ifstream file(m_strMusicFormatFile.toStdString());
	while (!file.eof())
	{
		file.getline(buf, 256, '\n');
		sBuf = buf;

		if (i == 0)		//读取表头
		{
			if (sBuf != "#EXTM3U" && sBuf != "[playlist]")
			{
				emit sig_LoadMusicListError();
				this->deleteLater();
			}
		}
		else
		{
			if ((strSuffixName == "m3u" || strSuffixName == "M3U") && i % 2 == 0)
			{
				this->loadMusicList(QString::fromStdString(sBuf));	//加载歌曲列表
			}
			else if ((strSuffixName == "pls" || strSuffixName == "PLS") && i == 1 + (n - 1) * 3)
			{
				if (!QString::fromStdString(sBuf).contains("NumberOfEntries"))
				{
					++n;
					if (QString::fromStdString(sBuf).contains("="))
					{
						QStringList list = QString::fromStdString(sBuf).split("=");
						QString filePath = list.at(1);
						this->loadMusicList(filePath);	//加载歌曲列表
					}
				}
			}
		}
		++i;
	}
	file.close();
}

//数据库读取音乐文件加载模式
void LoadMusicThread::readFromDBModeLoad()
{
	vector<MusicInfoEntity> vec;
	if (DBModule::readMusicInfoEntity(vec))
	{
		foreach (MusicInfoEntity entity, vec)
		{
			this->loadMusicList(entity);	//加载歌曲列表
		}
	}
}

//加载歌曲列表
void LoadMusicThread::loadMusicList( const QStringList &list )
{
	foreach (QString filePath, list)
	{
		this->loadMusicList(filePath);
	}
}

//加载歌曲列表
void LoadMusicThread::loadMusicList( const MusicInfoEntity &entity )
{
	//序号
	QTableWidgetItem *rowItem = new QTableWidgetItem;
	rowItem->setTextAlignment(Qt::AlignCenter);
	rowItem->setFont(QFont("微软雅黑", 10));

	//歌曲
	QTableWidgetItem *fileNameItem = new QTableWidgetItem(QString::fromStdString(entity.getFileName()));
	fileNameItem->setTextAlignment(Qt::AlignCenter);
	fileNameItem->setFont(QFont("微软雅黑", 10));

	//时间
	QTableWidgetItem *timeItem = new QTableWidgetItem(QString::fromStdString(entity.getTime()));
	timeItem->setTextAlignment(Qt::AlignCenter);
	timeItem->setFont(QFont("微软雅黑", 10));

	//插入播放列表
	int currentRows = m_TableWidget->rowCount();//返回列表中的行数
	m_TableWidget->insertRow(currentRows);//从播放列表中的当前行插入
	rowItem->setText(QString::number(currentRows + 1));
	m_TableWidget->setItem(currentRows, 0, rowItem);
	m_TableWidget->setItem(currentRows, 1, fileNameItem);
	m_TableWidget->setItem(currentRows, 2, timeItem);

	//更改时间列间隔
	if (currentRows + 1 <= 12)
	{
		m_TableWidget->horizontalHeader()->resizeSection(3, 80);
	}
	else
	{
		m_TableWidget->horizontalHeader()->resizeSection(3, 65);
	}

	//将音乐信息加入vector
	m_mapMusicRows->insert(make_pair(QString::fromStdString(entity.getFileName()), currentRows));
	m_mapMusicFilePath->insert(make_pair(QString::fromStdString(entity.getFileName()), QString::fromStdString(entity.getFilePath())));

	//放入数据库
	//DBModule::insertLine(fileName, time.toString("mm:ss"), filePath);
}

//歌曲文件夹加载模式
void LoadMusicThread::openMusicDirectoryMode()
{
	QDir dir(m_strMusicDirectory);
	QFileInfoList fileInfoList = dir.entryInfoList(QDir::Files | QDir::Hidden | QDir::NoDotAndDotDot | QDir::NoSymLinks);
	foreach (QFileInfo fileInfo, fileInfoList)
	{
		if (fileInfo.isFile() && (fileInfo.absoluteFilePath().right(3) == "wav" || fileInfo.absoluteFilePath().right(3) == "WAV"))
		{
			this->loadMusicList(fileInfo.absoluteFilePath());
		}
	}
}