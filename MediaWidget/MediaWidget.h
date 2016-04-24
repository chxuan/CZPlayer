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
 * \file   MediaWidget.h
 * \author chengxuan   787280310@qq.com
 * \date   2015-02-23
 * \brief  媒体界面头文件。
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

#ifndef MEDIAWIDGET_H
#define MEDIAWIDGET_H

/*! \def MEDIAWIDGET_VERSION
 *  版本控制宏，对应v3.0.0版本.
 */
#define MEDIAWIDGET_VERSION   0x030000

#include <QWidget>

class SearchPanel;
class RadioPanel;
class RankingPanel;
class DownloadListPanel;
class CollectionPanel;
class LrcPortraitPanel;
class QStackedWidget;
class QPushButton;
class QLabel;
class QPaintEvent;

/*! 媒体界面
*/
class MediaWidget : public QWidget
{
    Q_OBJECT

public:
    /*! 构造函数
    *  \param parent 父窗口指针，默认为NULL
    */
    MediaWidget(QWidget *parent = NULL);

    /*! 析构函数.*/
    ~MediaWidget();

    /*! 设置音乐目录
    *  \param musicPath 音乐目录
    */
    void setMusicPath(const QString &musicPath);

protected:
    /*! 虚函数，重新实现paintEvent函数
     *  \param event QPaintEvent类型
     */
    virtual void paintEvent(QPaintEvent *event);

private slots:
    /*! 处理搜索按钮点击
     */
    void handleSearchButton();

    /*! 处理搜索按钮点击
     */
    void handleRadioButton();

    /*! 处理榜单按钮点击
     */
    void handleRankingButton();

    /*! 处理下载按钮点击
     */
    void handleDownloadListButton();

    /*! 处理收藏按钮点击
     */
    void handleCollectionButton();

    /*! 处理歌词写真按钮点击
     */
    void handleLrcPortraitButton();

private:
    /*! 创建界面
     */
    void createUI();

    /*! 信号与槽
     */
    void createConnect();

    /*! 恢复之前按钮风格
     */
    void recoverButtonStyle();

private:
    QStackedWidget *m_pagesWidget;					/*!< 页面widget*/

    SearchPanel *m_searchPanel;                     /*!< 搜索*/
    RadioPanel *m_radioPanel;                       /*!< 电台*/
    RankingPanel *m_rankingPanel;                   /*!< 榜单*/
    DownloadListPanel *m_downloadListPanel;         /*!< 下载列表*/
    CollectionPanel *m_collectionPanel;             /*!< 收藏列表*/
    LrcPortraitPanel *m_lrcPortraitPanel;           /*!< 歌词写真*/

    QLabel *m_navigationBarLabel;           /*!< 导航栏*/
    QPushButton *m_searchButton;			/*!< 搜索*/
    QPushButton *m_radioButton;             /*!< 电台*/
    QPushButton *m_rankingButton;			/*!< 榜单*/
    QPushButton *m_downLoadListButton;		/*!< 下载列表*/
    QPushButton *m_collectionButton;		/*!< 收藏列表*/
    QPushButton *m_lrcPortraitButton;		/*!< 歌词写真*/

    bool m_isSearchButtonClicked;           /*!< 搜索按钮按下旗帜*/
    bool m_isRadioButtonClicked;            /*!< 电台按钮按下旗帜*/
    bool m_isRankingButtonClicked;          /*!< 榜单按钮按下旗帜*/
    bool m_isDownloadListButtonClicked;     /*!< 下载列表按钮按下旗帜*/
    bool m_isCollectionButtonClicked;       /*!< 收藏列表按钮按下旗帜*/
    bool m_isLrcPortraitButtonClicked;      /*!< 歌词按钮按下旗帜*/
};

#endif // MEDIAWIDGET_H
