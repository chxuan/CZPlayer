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
 * \file   LoadMusicThread.h
 * \author chengxuan   787280310@qq.com
 * \date   2014-12-28
 * \brief  音乐加载线程头文件。
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

#ifndef LOADMUSICTHREAD_H
#define LOADMUSICTHREAD_H

/*! \def LOADMUSICTHREAD_VERSION
 *  版本控制宏，对应v3.0.0版本.
 */
#define LOADMUSICTHREAD_VERSION   0x030000

#include <QThread>
#include <QString>
#include <QList>
#include <QStringList>
#include <QLabel>
#include <QUrl>
#include "MusicInfoEntity.h"

/*! 加载歌曲线程
*/
class LoadMusicThread : public QThread
{
	Q_OBJECT

public:
	/*! 构造函数
	 *  \param parent QObject类型，默认为NULL
	 */
	LoadMusicThread(QObject *parent = NULL);

	/*! 析构函数.*/
    ~LoadMusicThread();

protected:
	/*! 虚函数，继承自QThread
	 */
	virtual void run();
};

/*! 加载歌曲线程事件处理类
*/
class LoadMusicThreadEvent : public QObject
{
	Q_OBJECT

public:
	/*! 构造函数
     *  \param parent QObject类型，默认为NULL
     */
	LoadMusicThreadEvent(QObject *parent = NULL);

	/*! 析构函数.*/
    ~LoadMusicThreadEvent();

signals:
	/*! 加载播放列表错误
	*/
	void sigLoadMusicListError();

	/*! 加载完播放列表
	 */
	void sigLoadMusicListFinished();

	/*! 加载音乐
	 * \param entity 音乐信息实体
	 */
	void sigAddMusic(const MusicInfoEntity &entity);

private slots:
	/*! 打开音乐文件 设置参数
     *  \param musicList 音乐列表信息集合
     */
	void handleStartloadMusic(const QStringList &musicList);

	/*! 拖动音乐文件或音乐格式文件 设置参数
     *  \param urls 音乐url列表
     */
	void handleStartloadMusic(const QList<QUrl> &urls);

	/*! 加载音乐格式文件或文件夹 设置参数
     *  \param strFileOrDir 音乐格式文件或文件夹
     */
	void handleStartloadMusic(const QString &strFileOrDir);

	/*! 从数据库读取音乐文件 设置参数
     */
	void handleStartloadMusic();							

private:
	/*! 开始加载
     */
	void startLoad();

	/*! 打开音乐文件加载模式
     */
	void openMusicModeLoad();								

	/*! 拖动音乐文件或音乐格式文件加载模式
     */
	void dragEnterMusicModeLoad();							

	/*! 打开音乐格式文件加载模式
     */
	void openMusicFormatFileLoad();							

	/*! 数据库读取音乐文件加载模式
     */
	void readFromDBModeLoad();								

	/*! 歌曲文件夹加载模式
     */
	void openMusicDirectoryMode();		

	/*! 通过歌曲文件路径解析歌曲
	 * \param filePath 文件路径
	 */
	void analyzeMusic(const QString &filePath);

private:
	QStringList m_musicList;								/*!< 音乐列表*/
	QList<QUrl> m_urls;										/*!< 音乐url*/
	QString m_musicFormatFile;								/*!< 音乐格式文件*/
	QString m_musicDirectory;								/*!< 音乐文件夹*/

	/*! 加载音乐列表模式*/
	enum LoadMusicMode
	{
		OpenMusic				= 1,		/*!< 打开音乐文件加载模式*/	
		DragEnterMusic			= 2,		/*!< 拖动音乐文件或音乐格式文件加载模式*/	
		OpenMusicFormatFile		= 3,		/*!< 音乐格式文件加载模式*/	
		ReadFromDB				= 4,		/*!< 数据库读取音乐文件加载模式*/	
		OpenDirectory			= 5			/*!< 歌曲文件夹加载模式*/	
	};

	LoadMusicMode m_loadMusicMode;			/*!< 加载音乐列表模式*/
};

#endif //LOADMUSICTHREAD_H

