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
 * \file   LoadMusicThread.cpp
 * \author chengxuan   787280310@qq.com
 * \date   2014-12-28
 * \brief  音乐加载线程实现文件。
 * \version 3.0.0
 *
 * \verbatim
 * 历史
 *          3.0.0   创建,
 *          2014-12-28    by chengxuan
 *
 * \endverbatim
 *
 */

#include "LoadMusicThread.h"
#include "DBModule.h"
#include "TR.h"
#include <Windows.h>
#include <QTime>
#include <QDir>
#include <QFileInfo>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QDateTime>
#include <QTime>
#include <QTextStream>
#ifdef CZ_DEBUG
#include <QDebug>
#endif 

/*! 得到音乐播放时间，目前仅支持mp3、wma、wav格式
 *  \param szPath 音乐文件路径
 */
static unsigned long getMusicTime(WCHAR *szPath)
{
	MCIERROR mciError;
	MCI_OPEN_PARMS mciOpen;  
	MCI_STATUS_PARMS mciStatusParms;

	mciOpen.lpstrElementName = szPath; 
	mciError = mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT, (DWORD)(LPVOID)&mciOpen);
	if (mciError)
	{
		return 0;
	}
	/*Else do nothing, and continue.*/

	mciStatusParms.dwItem = MCI_STATUS_LENGTH;  
	mciSendCommand(mciOpen.wDeviceID, MCI_STATUS, MCI_WAIT | MCI_STATUS_ITEM, (DWORD)(LPVOID)&mciStatusParms);  
	mciSendCommand(mciOpen.wDeviceID, MCI_CLOSE, 0, NULL);

	return mciStatusParms.dwReturn / 1000; 
}

LoadMusicThread::LoadMusicThread(QObject *parent)
	: QThread(parent)
{
	/*Do nothing.*/
}

LoadMusicThread::~LoadMusicThread(void)
{
	/*Do nothing.*/
}

void LoadMusicThread::run()
{
	this->exec();
}

LoadMusicThreadEvent::LoadMusicThreadEvent(QObject *parent /*= NULL*/)
	: QObject(parent)
{
	/*Do nothing.*/
}

LoadMusicThreadEvent::~LoadMusicThreadEvent(void)
{
	/*Do nothing.*/
}

void LoadMusicThreadEvent::handleStartloadMusic(const QStringList &musicList)
{
	m_musicList = musicList;
	m_loadMusicMode = LoadMusicThreadEvent::OpenMusic;
	this->startLoad();
}

void LoadMusicThreadEvent::handleStartloadMusic(const QList<QUrl> &urls)
{
	m_urls = urls;
	m_loadMusicMode = LoadMusicThreadEvent::DragEnterMusic;
	this->startLoad();
}

void LoadMusicThreadEvent::handleStartloadMusic(const QString &strFileOrDir)
{
	QFileInfo fileInfo(strFileOrDir);
	if (fileInfo.isFile())
	{
		m_musicFormatFile = strFileOrDir;
		m_loadMusicMode = LoadMusicThreadEvent::OpenMusicFormatFile;
	}
	else if (fileInfo.isDir())
	{
		m_musicDirectory = strFileOrDir;
		m_loadMusicMode = LoadMusicThreadEvent::OpenDirectory;
	}
	/*Else do nothing, and continue.*/
	this->startLoad();
}

void LoadMusicThreadEvent::handleStartloadMusic()
{
	m_loadMusicMode = LoadMusicThreadEvent::ReadFromDB;
	this->startLoad();
}

void LoadMusicThreadEvent::startLoad()
{
	if (m_loadMusicMode == LoadMusicThreadEvent::OpenMusic)
	{
		this->openMusicModeLoad();
	}
	else if (m_loadMusicMode == LoadMusicThreadEvent::DragEnterMusic)
	{
		this->dragEnterMusicModeLoad();
	}
	else if (m_loadMusicMode == LoadMusicThreadEvent::OpenMusicFormatFile)
	{
		this->openMusicFormatFileLoad();
	}
	else if (m_loadMusicMode == LoadMusicThreadEvent::ReadFromDB)
	{
		this->readFromDBModeLoad();
	}
	else if (m_loadMusicMode == LoadMusicThreadEvent::OpenDirectory)
	{
		this->openMusicDirectoryMode();
	}
	/*Else do nothing, and continue.*/

	emit sigLoadMusicListFinished();
}

void LoadMusicThreadEvent::openMusicModeLoad()
{
	foreach (QString filePath, m_musicList)
	{
		this->analyzeMusic(filePath);
	}
}

void LoadMusicThreadEvent::dragEnterMusicModeLoad()
{
	foreach (QUrl url, m_urls)
	{
		QString filePath = url.toLocalFile();
		QFileInfo fileInfo(filePath);
		
		if (fileInfo.isFile())	//加载文件
		{
			QString suffixName = filePath.right(3);//得到后缀名
			//加载播放列表
			if (suffixName == "m3u"
				|| suffixName == "M3U"
				|| suffixName == "pls"
				|| suffixName == "PLS")
			{
				int i = 0;
				int n = 1;
				QFile file(filePath);
				if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
				{
					file.close();
					emit sigLoadMusicListError();
					return;
				}
				/*Else do nothing, and continue.*/
				QTextStream in(&file);

				while (!in.atEnd())
				{
					QString line = in.readLine();
					if (i == 0)		//读取表头
					{
						if (line != "#EXTM3U" && line != "[playlist]")
						{
							emit sigLoadMusicListError();
						}
						/*Else do nothing, and continue.*/
					}
					else
					{
						if ((suffixName == "m3u" || suffixName == "M3U") && i % 2 == 0)
						{
							this->analyzeMusic(line);
						}
						else if ((suffixName == "pls" || suffixName == "PLS") && i == 1 + (n - 1) * 3)
						{
							if (!line.contains("NumberOfEntries"))
							{
								++n;
								if (line.contains('='))
								{
									QStringList list = line.split('=');
									this->analyzeMusic(list.at(1));
								}
								/*Else do nothing, and continue.*/
							}
							/*Else do nothing, and continue.*/
						}
					}
					++i;
				}
				file.close();
			}
			else	//加载单首歌曲
			{
				this->analyzeMusic(filePath);
			}
		}
		else if (fileInfo.isDir())	//加载文件夹
		{
			m_musicDirectory = filePath;
			this->openMusicDirectoryMode();
		}
		/*Else do nothing, and continue.*/
	}
}

void LoadMusicThreadEvent::openMusicFormatFileLoad()
{
	QString suffixName = m_musicFormatFile.right(3);//得到后缀名
	int i = 0;
	int n = 1;

	QFile file(m_musicFormatFile);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		file.close();
		emit sigLoadMusicListError();
		return;
	}
	/*Else do nothing, and continue.*/

	QTextStream in(&file);

	while (!in.atEnd())
	{
		QString line = in.readLine();
		if (i == 0)		//读取表头
		{
			if (line != "#EXTM3U" && line != "[playlist]")
			{
				emit sigLoadMusicListError();
			}
			/*Else do nothing, and continue.*/
		}
		else
		{
			if ((suffixName == "m3u" || suffixName == "M3U") && i % 2 == 0)
			{
				this->analyzeMusic(line);
			}
			else if ((suffixName == "pls" || suffixName == "PLS") && i == 1 + (n - 1) * 3)
			{
				if (!line.contains("NumberOfEntries"))
				{
					++n;
					if (line.contains('='))
					{
						QStringList list = line.split('=');
						this->analyzeMusic(list.at(1));
					}
					/*Else do nothing, and continue.*/
				}
				/*Else do nothing, and continue.*/
			}
			/*Else do nothing, and continue.*/
		}
		++i;
	}
	file.close();
}

void LoadMusicThreadEvent::readFromDBModeLoad()
{
	QList<MusicInfoEntity> list;
	if (DBModule::readMusicInfoEntity(list))
	{
		foreach (MusicInfoEntity entity, list)
		{
			emit sigAddMusic(entity);
		}
	}
	/*Else do nothing, and continue.*/
}

void LoadMusicThreadEvent::openMusicDirectoryMode()
{
	QDir dir(m_musicDirectory);
	QFileInfoList fileInfoList = dir.entryInfoList(QDir::Files | QDir::Hidden | QDir::NoDotAndDotDot | QDir::NoSymLinks);
	foreach (QFileInfo fileInfo, fileInfoList)
	{
		QString suffixName = fileInfo.absoluteFilePath().right(3);
		if (fileInfo.isFile() 
			&& (suffixName == "mp3" || suffixName == "MP3"
			|| suffixName == "wma" || suffixName == "WMA"
			|| suffixName == "wav" || suffixName == "WAV"
			|| suffixName == "ogg" || suffixName == "OGG"))
		{
			this->analyzeMusic(fileInfo.absoluteFilePath());
		}
		/*Else do nothing, and continue.*/
	}
}

void LoadMusicThreadEvent::analyzeMusic(const QString &filePath)
{
	//音乐文件名
	QString fileName = QFileInfo(filePath).baseName();

	//时间
	unsigned long musicTime = getMusicTime((wchar_t*)filePath.utf16());
	QTime time(0, musicTime / 60, musicTime % 60);
	QString formatTime = time.toString("mm:ss");

	//放入数据库
	DBModule::insertLine(fileName, formatTime, filePath);

	MusicInfoEntity entity(fileName, formatTime, filePath);
	emit sigAddMusic(entity);
}