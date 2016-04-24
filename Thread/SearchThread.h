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
 * \file   SearchThread.h
 * \author chengxuan   787280310@qq.com
 * \date   2015-03-03
 * \brief  歌曲搜索线程头文件。
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

#ifndef SEARCHTHREAD_H
#define SEARCHTHREAD_H

/*! \def SEARCHTHREAD_VERSION
 *  版本控制宏，对应v3.0.0版本.
 */
#define SEARCHTHREAD_VERSION   0x030000

#include <QThread>
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

/*! 搜索歌曲线程
*/
class SearchThread : public QThread
{
    Q_OBJECT

public:
    /*! 构造函数
     *  \param parent QObject类型，默认为NULL
     */
    SearchThread(QObject *parent = NULL);

    /*! 析构函数.*/
    ~SearchThread();

protected:
    /*! 虚函数，继承自QThread
     */
    virtual void run();
};

/*! 搜索歌曲线程事件处理类
*/
class SearchThreadEvent : public QObject
{
    Q_OBJECT

public:
    /*! 构造函数
     *  \param parent QObject类型，默认为NULL
     */
    SearchThreadEvent(QObject *parent = NULL);

    /*! 析构函数.*/
    ~SearchThreadEvent();

signals:
    /*! 加载音乐信息
    *  \param musicName 歌曲名
    *  \param artist 艺术家
    *  \param album 专辑
    */
    void sigAddMusicInfo(const QString &musicName, const QString &artist, const QString &album);

    /*! 歌曲搜索状态
    *  \param status 状态
    */
    void sigMusicSearchStatus(MusicSearchStatus status);

private slots:
    /*! 搜索歌曲
    *  \param text 关键字文本
    */
    void handleSearchMusic(const QString &text);

    /*! 搜索完成
     *  \param replay 网络回应对象
     */
    void handleSearchFinished(QNetworkReply *replay);

private:
    QNetworkAccessManager *m_searchManager; /*!< 网络管理对象*/
};

#endif // SEARCHTHREAD_H
