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
 * \file   DownloadMusicThread.h
 * \author chengxuan   787280310@qq.com
 * \date   2015-03-03
 * \brief  歌曲下载线程头文件。
 * \version 3.0.0
 *
 * \verbatim
 * 历史
 *          3.0.0   创建,
 *          2015-03-03    by chengxuan
 *
 * \endverbatim
 *
 */

#ifndef DOWNLOADMUSICTHREAD_H
#define DOWNLOADMUSICTHREAD_H

/*! \def DOWNLOADMUSICTHREAD_VERSION
 *  版本控制宏，对应v3.0.0版本.
 */
#define DOWNLOADMUSICTHREAD_VERSION   0x030000

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

/*! 歌曲下载线程
*/
class DownloadMusicThread : public QThread
{
    Q_OBJECT

public:
    /*! 构造函数
     *  \param musicName 音乐名称
     *  \param musicArtist 音乐艺术家
     *  \param musicPath 存放音乐的路径
     *  \param parent QObject类型，默认为NULL
     */
    DownloadMusicThread(const QString &musicName, const QString artist, const QString &musicPath, QObject *parent = NULL);

    /*! 析构函数.*/
    ~DownloadMusicThread();

protected:
    /*! 虚函数，继承自QThread
     */
    virtual void run();

signals:
    /*! 下载歌曲状态
     *  \param status 下载状态
     */
    void sigMusicDownloadStatus(MusicDownloadStatus status);

    /*! 更新进度条
     *  \param bytesRead 已经下载的大小
     *  \param totalBytes 总大小
     */
    void sigUpdateDataProgress(const qint64 &bytesRead, const qint64 &totalBytes);

    /*! 音乐文件大小
     *  \param size 音乐文件大小，单位：字节
     */
    void sigMusicSize(const double &size);

private slots:
    /*! 得到歌曲XML格式
    *  \param replay 网络回应对象
    */
    void handleGetMusicXML(QNetworkReply *replay);

    /*! 保存歌曲
     */
    void handleSaveMusic();

    /*! 请求完成
    */
    void handleReplayFinished();

    /*! 处理错误
     */
    void handleReplyError(QNetworkReply::NetworkError networkError);

private:
    /*! 设置音乐url
    *  \param url 音乐url
    */
    void setMusicURL(const QUrl &url);

    /*! 网络错误代码转换成字符串
     *  \param networkError 网络错误代码
     */
    QString errorCodeToString(QNetworkReply::NetworkError networkError);

private:
    QString m_musicName;                        /*!< 音乐名*/
    QString m_artist;                           /*!< 艺术家*/
    QString m_filePath;                         /*!< 文件路径*/
    QFile m_musicFile;							/*!< 音乐文件对象*/

    QNetworkAccessManager *m_musicManager;		/*!< 网络管理对象*/
    QNetworkAccessManager *m_musicManager2;		/*!< 网络管理对象*/
    QNetworkReply *m_musicReply;				/*!< 网络回应对象*/

    int m_times;                                /*!< 下载计数*/
};

#endif // DOWNLOADMUSICTHREAD_H
