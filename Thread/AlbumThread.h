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
 * \file   AlbumThread.h
 * \author chengxuan   787280310@qq.com
 * \date   2014-12-24
 * \brief  专辑下载线程头文件。
 * \version 3.0.0
 *
 * \verbatim
 * 历史
 *          3.0.0   创建,
 *          2014-12-24    by chengxuan
 *
 * \endverbatim
 *
 */

#ifndef ALBUMTHREAD_H
#define ALBUMTHREAD_H

/*! \def ALBUMTHREAD_VERSION
 *  版本控制宏，对应v3.0.0版本.
 */
#define ALBUMTHREAD_VERSION   0x030000

#include <QThread>
#include <QFile>
#include <QString>
#include <QNetworkReply>
#include "StatusDefine.h"

/*****************************************************************************
  这里用来检查版本的对应
 *****************************************************************************/
#if STATUSDEFINE_VERSION < 0x030000
#error  "StatusDefine version error."
#endif

class QNetworkAccessManager;
class QLabel;

/*! 专辑下载线程
*/
class AlbumThread : public QThread
{
	Q_OBJECT

public:
	/*! 构造函数
	 *  \param fileName 音乐文件名
	 *  \param musicName 音乐名称
	 *  \param musicArtist 音乐艺术家
	 *  \param albumFilePath 专辑文件路径
     *  \param parent QObject类型，默认为NULL
     */
	AlbumThread(const QString &fileName, 
				const QString &musicName, 
				const QString &musicArtist, 
				const QString &albumFilePath,
				QObject *parent = NULL);

	/*! 析构函数.*/
	~AlbumThread();

protected:
	/*! 虚函数，继承自QThread
     */
	virtual void run();

signals:
	/*! 下载专辑状态
     *  \param fileName 音乐文件名
	 *  \param albumUrl 专辑Url
	 *  \param albumFilePath 专辑文件路径
	 *  \param status 下载状态
     */
	void sigAlbumDownloadStatus(const QString &fileName, 
								const QString &albumUrl, 
								const QString &albumFilePath, 
								AlbumDownloadStatus status);

private slots:
	/*! 得到专辑Json格式数据
     *  \param replay 网络回应对象
     */
	void handleGetMusicAlbumJson(QNetworkReply *replay);

	/*! 得到专辑Json格式数据
     *  \param replay 网络回应对象
     */
	void handleGetMusicAlbumJson2(QNetworkReply *replay);

	/*! 保存专辑
     */
	void handleSaveAlbum();

	/*! 专辑专辑下载完成
     */
	void handleReplayFinished();

	/*! 处理错误
     */
	void handleReplyError(QNetworkReply::NetworkError networkError);

private:
	/*! 设置专辑Url
     *  \param url 专辑Url
     */
	void setAlbumFromURL(const QUrl &url);

	/*! 网络错误代码转换成字符串
     *  \param networkError 网络错误代码
     */
	QString errorCodeToString(QNetworkReply::NetworkError networkError);

private:
	QFile m_albumFile;							/*!< 专辑文件对象*/
	QString m_fileName;							/*!< 音乐文件名*/
	QString m_musicName;						/*!< 音乐名称*/
	QString m_musicArtist;						/*!< 音乐艺术家*/
	QString m_albumFilePath;					/*!< 保存的专辑文件路径*/
	QString m_albumUrl;							/*!< 网络专辑URL*/

	QNetworkAccessManager *m_albumManager;		/*!< 网络管理对象*/
	QNetworkAccessManager *m_albumManager2;		/*!< 网络管理对象*/
	QNetworkAccessManager *m_albumManager3;		/*!< 网络管理对象*/
	QNetworkReply *m_albumReply;				/*!< 网络回应对象*/
};

#endif // ALBUMTHREAD_H
