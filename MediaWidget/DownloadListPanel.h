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
 * \file   DownloadListPanel.h
 * \author chengxuan   787280310@qq.com
 * \date   2015-02-23
 * \brief  下载列表面板头文件。
 * \version 3.0.0
 *
 * \verbatim
 * 历史
 *          3.0.0   创建,
 *          2015-02-23    by chengxuan
 *
 * \endverbatim
 *
 */

#ifndef DOWNLOADLISTPANEL_H
#define DOWNLOADLISTPANEL_H

/*! \def DOWNLOADLISTPANEL_VERSION
 *  版本控制宏，对应v3.0.0版本.
 */
#define DOWNLOADLISTPANEL_VERSION   0x030000

#include <QWidget>
#include <QList>
#include "StatusDefine.h"

/*****************************************************************************
  这里用来检查版本的对应
 *****************************************************************************/
#if STATUSDEFINE_VERSION < 0x030000
#error  "StatusDefine version error."
#endif

class QLabel;
class QTableWidget;
class QThread;
class QTableWidgetItem;
class QProgressBar;
class QTime;

class DownloadMusicThread;
class DownloadInfo;

/*! 下载面板
*/
class DownloadListPanel : public QWidget
{
    Q_OBJECT

public:
    /*! 构造函数
    *  \param parent 父窗口指针，默认为NULL
    */
    DownloadListPanel(QWidget *parent = NULL);

    /*! 析构函数.*/
    ~DownloadListPanel();

    /*! 设置音乐目录
    *  \param musicPath 音乐目录
    */
    void setMusicPath(const QString &musicPath);

private slots:
    /*! 音乐目录改变
     * \param path 音乐路径
     */
    void handleMusicDirChanged(const QString &path);

    /*! 下载歌曲
    *  \param musicName 音乐名
    *  \param artist 艺术家
    */
    void handleDownloadMusic(const QString &musicName, const QString &artist);

    /*! 下载歌曲状态
     *  \param status 下载状态
     */
    void handleMusicDownloadStatus(MusicDownloadStatus status);

    /*! 更新进度条
     *  \param bytesRead 已经下载的大小
     *  \param totalBytes 总大小
     */
    void handleUpdateDataProgress(const qint64 &bytesRead, const qint64 &totalBytes);

    /*! 音乐文件大小
     *  \param size 音乐文件大小，单位：字节
     */
    void handleMusicSize(const double &size);

private:
    /*! 创建界面
     */
    void createUI();

    /*! 信号与槽
     */
    void createConnect();

    /*! 播放下载完成音乐
     */
    void playAudio();

    /*! 设置表格风格
     */
    void setTableWidgetStyle();

private:
    QTableWidget *m_downloadTable;         /*!< 下载列表*/
    QList<DownloadInfo *> m_musicInfos;    /*!< 歌曲信息集合*/
    QString m_musicPath;                   /*!< 音乐目录*/
};

/*! 下载信息类
*/
class DownloadInfo : public QObject
{
    Q_OBJECT

public:
    /*! 构造函数
     *  \param downloadTable 下载列表指针
    *  \param parent QObject类型，默认为NULL
    */
    DownloadInfo(QTableWidget *downloadTable, QObject *parent = NULL);

    /*! 析构函数.*/
    ~DownloadInfo();

    /*! 得到音乐标题
     *  \return 音乐标题
     */
    QString title() const;

    /*! 设置音乐标题
     *  \param text 音乐标题
     */
    void setTitle(const QString &text);

    /*! 得到音乐状态
     *  \return 音乐状态
     */
    QString status() const;

    /*! 设置音乐状态
     *  \param text 音乐状态
     */
    void setStatus(const QString &text);

    /*! 得到音乐文件大小
     *  \return 音乐文件大小
     */
    QString size() const;

    /*! 设置音乐文件大小
     *  \param text 音乐文件大小
     */
    void setSize(const QString &text);

    /*! 得到下载速度
     *  \return 下载速度
     */
    QString speed() const;

    /*! 设置下载速度
     *  \param text 下载速度
     */
    void setSpeed(const QString &text);

    /*! 得到下载进度
     *  \return 下载进度
     */
    qint64 progress() const;

    /*! 设置下载进度
     *  \param value 下载进度
     */
    void setProgress(const qint64 &value);

    /*! 设置下载进度最大值
     *  \param value 下载进度最大值
     */
    void setProgressMaximum(const qint64 &value);

    /*! 得到下载线程指针
     *  \return 下载线程指针
     */
    QThread *thread() const;

    /*! 设置下载线程指针
     *  \param value 下载线程指针
     */
    void setThread(QThread *thread);

    /*! 得到时间间隔
     *  \return 时间间隔
     */
    int elapsed() const;

private:
    QTableWidgetItem *m_musicTitleItem;     /*!< 音乐标题*/
    QTableWidgetItem *m_musicStatusItem;    /*!< 音乐下载状态*/
    QTableWidgetItem *m_musicSizeItem;      /*!< 音乐文件大小*/
    QTableWidgetItem *m_speedItem;          /*!< 下载速度*/
    QProgressBar *m_progressBar;            /*!< 进度条*/
    QThread *m_thread;                      /*!< 下载线程*/
    QTime *m_time;                          /*!< 时间*/
};

#endif // DOWNLOADLISTPANEL_H
