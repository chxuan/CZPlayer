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
 * \file   SearchPanel.h
 * \author chengxuan   787280310@qq.com
 * \date   2015-02-23
 * \brief  搜索面板头文件。
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

#ifndef SEARCHPANEL_H
#define SEARCHPANEL_H

/*! \def SEARCHPANEL_VERSION
 *  版本控制宏，对应v3.0.0版本.
 */
#define SEARCHPANEL_VERSION   0x030000

#include <QWidget>
#include <QMap>
#include <QString>
#include "StatusDefine.h"

/*****************************************************************************
  这里用来检查版本的对应
 *****************************************************************************/
#if STATUSDEFINE_VERSION < 0x030000
#error  "StatusDefine version error."
#endif

class QLabel;
class QLineEdit;
class QPushButton;
class QCheckBox;
class QTableWidget;
class QTableWidgetItem;
class QEvent;

class SearchThread;
class SearchThreadEvent;

/*! 搜索面板
*/
class SearchPanel : public QWidget
{
    Q_OBJECT

public:
    /*! 构造函数
    *  \param parent 父窗口指针，默认为NULL
    */
    SearchPanel(QWidget *parent = NULL);

    /*! 析构函数.*/
    ~SearchPanel();

protected:
    /*! 虚函数，重新实现eventFilter函数
     *  \param target QObject类型
     *  \param event QEvent类型
     *  \reurn 返回true代表不过滤该事件
     */
    virtual bool eventFilter(QObject *target, QEvent *event);

signals:
    /*! 搜索歌曲
    *  \param text 关键字文本
    */
    void sigSearchMusic(const QString &text);

    /*! 下载收曲
    *  \param musicName 音乐名
    *  \param artist 艺术家
    */
    void sigDownloadMusic(const QString &musicName, const QString &artist);

    /*! 收藏歌曲
    *  \param musicName 音乐名
    *  \param artist 艺术家
    */
    void sigCollectMusic(const QString &musicName, const QString &artist);

private slots:
    /*! 搜索歌曲
     */
    void handleSearchMusic();

    /*! 加载音乐信息
    *  \param musicName 歌曲名
    *  \param artist 艺术家
    *  \param album 专辑
    */
    void handleAddMusicInfo(const QString &musicName, const QString &artist, const QString &album);

    /*! 下载歌曲
     */
    void handleDownloadMusic();

    /*! 收藏歌曲
     */
    void handleCollectMusic();

    /*! 全部选中
     *  \param isChecked 是否被选中
     */
    void handleSelectAllMusic(bool isChecked);

    /*! 选中一首
     *  \param isChecked 是否被选中
     */
    void handleSelectOneMusic(bool isChecked);

    /*! 歌曲搜索状态
    *  \param status 状态
    */
    void handleMusicSearchStatus(MusicSearchStatus status);

private:
    /*! 创建界面
     */
    void createUI();

    /*! 信号与槽
     */
    void createConnect();

    /*! 创建音乐搜索线程
    */
    void createMusicSearchThread();

    /*! 设置表格风格
     */
    void setTableWidgetStyle();

private:
    QLineEdit *m_searchEdit;            /*!< 搜索输入框*/
    QPushButton *m_searchButton;        /*!< 搜索按钮*/
    QLabel *m_searchInfoLabel;          /*!< 搜索提示*/
    QCheckBox *m_allChecked;            /*!< 选择全部歌曲*/
    QPushButton *m_downloadButton;      /*!< 下载歌曲*/
    QPushButton *m_collectButton;       /*!< 收藏歌曲*/
    QTableWidget *m_searchTable;        /*!< 搜索列表*/

    SearchThread *m_searchThread;             /*!< 搜索线程*/
    SearchThreadEvent *m_searchThreadEvent;   /*!< 搜索线程事件处理类*/

    QMap<QCheckBox*, QString> m_searchMap;  /*!< key：QCheckBox类型，value：是艺术家名*/
    bool m_isSearching;                     /*!< 是否正在搜索歌曲*/
};

#endif // SEARCHPANEL_H
