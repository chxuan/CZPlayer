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
 * \file   LrcThread.h
 * \author chengxuan   787280310@qq.com
 * \date   2014-12-28
 * \brief  歌词下载线程头文件。
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

#ifndef LRCTHREAD_H
#define LRCTHREAD_H

/*! \def LRCTHREAD_VERSION
 *  版本控制宏，对应v3.0.0版本.
 */
#define LRCTHREAD_VERSION   0x030000

#include <QThread>
#include <QFile>
#include <QUrl>
#include <QString>
#include "StatusDefine.h"

/*****************************************************************************
  这里用来检查版本的对应
 *****************************************************************************/
#if STATUSDEFINE_VERSION < 0x030000
#error  "StatusDefine version error."
#endif

class QNetworkAccessManager;
class QNetworkReply;
class QLabel;

/*! 歌词下载线程
*/
class LrcThread : public QThread
{
	Q_OBJECT

public:
	/*! 构造函数
	 *  \param musicName 音乐名称
	 *  \param musicArtist 音乐艺术家
	 *  \param lrcFilePath 歌词路径
     *  \param parent QObject类型，默认为NULL
     */
	LrcThread(const QString &musicName, 
		const QString &musicArtist, 
		const QString &lrcFilePath, 
		QObject *parent = NULL);

	/*! 析构函数.*/
	~LrcThread();

protected:
    /*! 虚函数，继承自QThread
     */
	virtual void run();

signals:
	/*! 下载歌词状态
     *  \param musicName 音乐名
	 *  \param musicArtist 音乐艺术家
	 *  \param lrcFilePath 歌词文件路径
	 *  \param status 下载状态
     */
	void sigLrcDownloadStatus(const QString &musicName, 
							    const QString &musicArtist, 
								const QString &lrcFilePath, 
								LrcDownloadStatus status);

private slots:
	/*! 得到MusicXML
	 *  \param replay 网络回应对象
     */
	void handleGetMusicXML(QNetworkReply *replay);

	/*! 保存歌词
	 *  \param replay 网络回应对象
     */
	void handleSaveLrc(QNetworkReply *replay);

private:
	/*! 设置歌词Url
     *  \param url 歌词Url
     */
	void setLrcFromURL(const QUrl &url);

private:
	QFile m_lrcFile;							/*!< 歌词文件对象*/
	QString m_musicName;						/*!< 音乐名称*/
	QString m_musicArtist;						/*!< 音乐艺术家*/
	QString m_lrcFilePath;						/*!< 保存的歌词文件路径*/

	QNetworkAccessManager *m_lrcManager;		/*!< 网络管理对象*/
	QNetworkAccessManager *m_lrcManager2;		/*!< 网络管理对象*/
};

#endif // LRCTHREAD_H
