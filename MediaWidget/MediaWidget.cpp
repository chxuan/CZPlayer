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
 * \file   MediaWidget.cpp
 * \author chengxuan   787280310@qq.com
 * \date   2015-02-23
 * \brief  媒体界面实现文件。
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

#include "MediaWidget.h"
#include "SearchPanel.h"
#include "RadioPanel.h"
#include "RankingPanel.h"
#include "DownloadListPanel.h"
#include "CollectionPanel.h"
#include "LrcPortraitPanel.h"
#include "Global.h"
#include "TR.h"
#include <QStackedWidget>
#include <QPushButton>
#include <QLabel>
#include <QPaintEvent>
#include <QPainter>
#include <QPixmap>
#include <QBitmap>
#include <QVBoxLayout>
#include <QSpacerItem>
#ifdef CZ_DEBUG
#include <QDebug>
#endif

/*****************************************************************************
  这里用来检查版本的对应
 *****************************************************************************/
#if SEARCHPANEL_VERSION < 0x030000
#error  "SearchPanel version error."
#endif

#if RADIOPANEL_VERSION < 0x030000
#error  "RadioPanel version error."
#endif

#if RANKINGPANEL_VERSION < 0x030000
#error  "RankingPanel version error."
#endif

#if DOWNLOADLISTPANEL_VERSION < 0x030000
#error  "DownloadListPanel version error."
#endif

#if COLLECTIONPANEL_VERSION < 0x030000
#error  "CollectionPanel version error."
#endif

#if LRCPORTRAITPANEL_VERSION < 0x030000
#error  "LrcPortraitPanel version error."
#endif

#if GLOBAL_VERSION < 0x030000
#error  "Global version error."
#endif

#if TR_VERSION < 0x030000
#error  "TR version error."
#endif

MediaWidget::MediaWidget(QWidget *parent)
    : QWidget(parent),
      m_searchPanel(NULL),
      m_radioPanel(NULL),
      m_rankingPanel(NULL),
      m_downloadListPanel(NULL),
      m_collectionPanel(NULL),
      m_lrcPortraitPanel(NULL),
      m_isSearchButtonClicked(false),
      m_isRadioButtonClicked(false),
      m_isRankingButtonClicked(false),
      m_isDownloadListButtonClicked(false),
      m_isCollectionButtonClicked(false),
      m_isLrcPortraitButtonClicked(false)
{
    this->createUI();
    this->createConnect();
}

MediaWidget::~MediaWidget()
{
    if (m_searchPanel != NULL)
    {
        m_searchPanel->deleteLater();
        m_searchPanel = NULL;
    }
    /*Else do nothing, and continue.*/

    if (m_radioPanel != NULL)
    {
        m_radioPanel->deleteLater();
        m_radioPanel = NULL;
    }
    /*Else do nothing, and continue.*/

    if (m_rankingPanel != NULL)
    {
        m_rankingPanel->deleteLater();
        m_rankingPanel = NULL;
    }
    /*Else do nothing, and continue.*/

    if (m_downloadListPanel != NULL)
    {
        m_downloadListPanel->deleteLater();
        m_downloadListPanel = NULL;
    }
    /*Else do nothing, and continue.*/

    if (m_collectionPanel != NULL)
    {
        m_collectionPanel->deleteLater();
        m_collectionPanel = NULL;
    }
    /*Else do nothing, and continue.*/

    if (m_lrcPortraitPanel != NULL)
    {
        m_lrcPortraitPanel->deleteLater();
        m_lrcPortraitPanel = NULL;
    }
    /*Else do nothing, and continue.*/
}

void MediaWidget::setMusicPath(const QString &musicPath)
{
    m_downloadListPanel->setMusicPath(musicPath);
}

void MediaWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QPixmap backgroundImage;
    backgroundImage.load(ImagesDir + "searchPanel.png");

    this->setMask(backgroundImage.mask());
    painter.drawPixmap(0, 0, 400, 662, backgroundImage);
    event->accept();
}

void MediaWidget::handleSearchButton()
{
    if (m_isSearchButtonClicked)
    {
        return;
    }
    /*Else do nothing, and continue.*/

    this->recoverButtonStyle();
    m_searchButton->setStyleSheet(QString("QPushButton:!hover{border-image: url(%1searchMusicButton3.png);}"
                                        "QPushButton:hover{border-image: url(%1searchMusicButton3.png);}"
                                        "QPushButton:pressed{border-image: url(%1searchMusicButton3.png);}").arg(ImagesDir));
    m_pagesWidget->setCurrentWidget(m_searchPanel);
    m_isSearchButtonClicked = true;
}

void MediaWidget::handleRadioButton()
{
    if (m_isRadioButtonClicked)
    {
        return;
    }
    /*Else do nothing, and continue.*/

    this->recoverButtonStyle();
    m_radioButton->setStyleSheet(QString("QPushButton:!hover{border-image: url(%1broadcastingStationButton3.png);}"
                                        "QPushButton:hover{border-image: url(%1broadcastingStationButton3.png);}"
                                        "QPushButton:pressed{border-image: url(%1broadcastingStationButton3.png);}").arg(ImagesDir));
    m_pagesWidget->setCurrentWidget(m_radioPanel);
    m_isRadioButtonClicked = true;
}

void MediaWidget::handleRankingButton()
{
    if (m_isRankingButtonClicked)
    {
        return;
    }
    /*Else do nothing, and continue.*/

    this->recoverButtonStyle();
    m_rankingButton->setStyleSheet(QString("QPushButton:!hover{border-image: url(%1listButton3.png);}"
                                        "QPushButton:hover{border-image: url(%1listButton3.png);}"
                                        "QPushButton:pressed{border-image: url(%1listButton3.png);}").arg(ImagesDir));
    m_pagesWidget->setCurrentWidget(m_rankingPanel);
    m_isRankingButtonClicked = true;
}

void MediaWidget::handleDownloadListButton()
{
    if (m_isDownloadListButtonClicked)
    {
        return;
    }
    /*Else do nothing, and continue.*/

    this->recoverButtonStyle();
    m_downLoadListButton->setStyleSheet(QString("QPushButton:!hover{border-image: url(%1downLoadListButton3.png);}"
                                        "QPushButton:hover{border-image: url(%1downLoadListButton3.png);}"
                                        "QPushButton:pressed{border-image: url(%1downLoadListButton3.png);}").arg(ImagesDir));
    m_pagesWidget->setCurrentWidget(m_downloadListPanel);
    m_isDownloadListButtonClicked = true;
}

void MediaWidget::handleCollectionButton()
{
    if (m_isCollectionButtonClicked)
    {
        return;
    }
    /*Else do nothing, and continue.*/

    this->recoverButtonStyle();
    m_collectionButton->setStyleSheet(QString("QPushButton:!hover{border-image: url(%1favoritesListButton3.png);}"
                                        "QPushButton:hover{border-image: url(%1favoritesListButton3.png);}"
                                        "QPushButton:pressed{border-image: url(%1favoritesListButton3.png);}").arg(ImagesDir));
    m_pagesWidget->setCurrentWidget(m_collectionPanel);
    m_isCollectionButtonClicked = true;
}

void MediaWidget::handleLrcPortraitButton()
{
    if (m_isLrcPortraitButtonClicked)
    {
        return;
    }
    /*Else do nothing, and continue.*/

    this->recoverButtonStyle();
    m_lrcPortraitButton->setStyleSheet(QString("QPushButton:!hover{border-image: url(%1lyricsPhotoBarButton3.png);}"
                                        "QPushButton:hover{border-image: url(%1lyricsPhotoBarButton3.png);}"
                                        "QPushButton:pressed{border-image: url(%1lyricsPhotoBarButton3.png);}").arg(ImagesDir));
    m_pagesWidget->setCurrentWidget(m_lrcPortraitPanel);
    m_isLrcPortraitButtonClicked = true;
}

void MediaWidget::createUI()
{
    //设置窗口基本属性
    this->resize(400, 662);
    this->setWindowFlags(Qt::Tool | Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);

    //导航条
    m_navigationBarLabel = new QLabel(this);
    m_navigationBarLabel->setObjectName("navigationBarLabel");

    //搜索
    m_searchButton = new QPushButton(this);
    m_searchButton->setStyleSheet(QString("QPushButton:!hover{border-image: url(%1searchMusicButton.png);}"
                                        "QPushButton:hover{border-image: url(%1searchMusicButton2.png);}"
                                        "QPushButton:pressed{border-image: url(%1searchMusicButton3.png);}").arg(ImagesDir));

    //电台
    m_radioButton = new QPushButton(this);
    m_radioButton->setStyleSheet(QString("QPushButton:!hover{border-image: url(%1broadcastingStationButton.png);}"
                                        "QPushButton:hover{border-image: url(%1broadcastingStationButton2.png);}"
                                        "QPushButton:pressed{border-image: url(%1broadcastingStationButton3.png);}").arg(ImagesDir));

    //榜单
    m_rankingButton = new QPushButton(this);
    m_rankingButton->setStyleSheet(QString("QPushButton:!hover{border-image: url(%1listButton.png);}"
                                        "QPushButton:hover{border-image: url(%1listButton2.png);}"
                                        "QPushButton:pressed{border-image: url(%1listButton3.png);}").arg(ImagesDir));

    //下载列表
    m_downLoadListButton = new QPushButton(this);
    m_downLoadListButton->setStyleSheet(QString("QPushButton:!hover{border-image: url(%1downLoadListButton.png);}"
                                        "QPushButton:hover{border-image: url(%1downLoadListButton2.png);}"
                                        "QPushButton:pressed{border-image: url(%1downLoadListButton3.png);}").arg(ImagesDir));

    //收藏列表
    m_collectionButton = new QPushButton(this);
    m_collectionButton->setStyleSheet(QString("QPushButton:!hover{border-image: url(%1favoritesListButton.png);}"
                                        "QPushButton:hover{border-image: url(%1favoritesListButton2.png);}"
                                        "QPushButton:pressed{border-image: url(%1favoritesListButton3.png);}").arg(ImagesDir));

    //歌词写真
    m_lrcPortraitButton = new QPushButton(this);
    m_lrcPortraitButton->setStyleSheet(QString("QPushButton:!hover{border-image: url(%1lyricsPhotoBarButton.png);}"
                                        "QPushButton:hover{border-image: url(%1lyricsPhotoBarButton2.png);}"
                                        "QPushButton:pressed{border-image: url(%1lyricsPhotoBarButton3.png);}").arg(ImagesDir));

    //搜索
    if (m_searchPanel == NULL)
    {
        m_searchPanel = new SearchPanel(this);
    }
    /*Else do nothing, and continue.*/

    //电台
    if (m_radioPanel == NULL)
    {
        m_radioPanel = new RadioPanel(this);
    }
    /*Else do nothing, and continue.*/

    //榜单
    if (m_rankingPanel == NULL)
    {
        m_rankingPanel = new RankingPanel(this);
    }
    /*Else do nothing, and continue.*/

    //下载列表
    if (m_downloadListPanel == NULL)
    {
        m_downloadListPanel = new DownloadListPanel(this);
    }
    /*Else do nothing, and continue.*/

    //收藏列表
    if (m_collectionPanel == NULL)
    {
        m_collectionPanel = new CollectionPanel(this);
    }
    /*Else do nothing, and continue.*/

    //歌词写真
    if (m_lrcPortraitPanel == NULL)
    {
        m_lrcPortraitPanel = new LrcPortraitPanel(this);
    }
    /*Else do nothing, and continue.*/

    //多页显示
    m_pagesWidget = new QStackedWidget;
    m_pagesWidget->addWidget(m_searchPanel);
    m_pagesWidget->addWidget(m_radioPanel);
    m_pagesWidget->addWidget(m_rankingPanel);
    m_pagesWidget->addWidget(m_downloadListPanel);
    m_pagesWidget->addWidget(m_collectionPanel);
    m_pagesWidget->addWidget(m_lrcPortraitPanel);

    m_navigationBarLabel->setGeometry(5, 10, 390, 33);
    m_searchButton->setGeometry(7, 12, 51, 29);
    m_radioButton->setGeometry(60, 12, 57, 29);
    m_rankingButton->setGeometry(119, 12, 59, 29);
    m_downLoadListButton->setGeometry(180, 12, 70, 29);
    m_collectionButton->setGeometry(253, 12, 69, 29);
    m_lrcPortraitButton->setGeometry(325, 12, 68, 29);

    QSpacerItem *spacerItem = new QSpacerItem(this->width() - 50, 40, QSizePolicy::Fixed, QSizePolicy::Fixed);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addItem(spacerItem);
    mainLayout->addWidget(m_pagesWidget);
    this->setLayout(mainLayout);

    this->handleSearchButton();
}

void MediaWidget::createConnect()
{
    connect(m_searchButton, SIGNAL(clicked()), this, SLOT(handleSearchButton()));
    connect(m_radioButton, SIGNAL(clicked()), this, SLOT(handleRadioButton()));
    connect(m_rankingButton, SIGNAL(clicked()), this, SLOT(handleRankingButton()));
    connect(m_downLoadListButton, SIGNAL(clicked()), this, SLOT(handleDownloadListButton()));
    connect(m_collectionButton, SIGNAL(clicked()), this, SLOT(handleCollectionButton()));
    connect(m_lrcPortraitButton, SIGNAL(clicked()), this, SLOT(handleLrcPortraitButton()));

    connect(m_searchPanel, SIGNAL(sigDownloadMusic(QString,QString)), m_downloadListPanel, SLOT(handleDownloadMusic(QString,QString)));
    connect(m_searchPanel, SIGNAL(sigCollectMusic(QString,QString)), m_collectionPanel, SLOT(handleAddCollectMusic(QString,QString)));
}

void MediaWidget::recoverButtonStyle()
{
    if (m_isSearchButtonClicked)
    {
        m_searchButton->setStyleSheet(QString("QPushButton:!hover{border-image: url(%1searchMusicButton.png);}"
                                            "QPushButton:hover{border-image: url(%1searchMusicButton2.png);}"
                                            "QPushButton:pressed{border-image: url(%1searchMusicButton3.png);}").arg(ImagesDir));
        m_isSearchButtonClicked = false;
    }
    else if (m_isRadioButtonClicked)
    {
        m_radioButton->setStyleSheet(QString("QPushButton:!hover{border-image: url(%1broadcastingStationButton.png);}"
                                            "QPushButton:hover{border-image: url(%1broadcastingStationButton2.png);}"
                                            "QPushButton:pressed{border-image: url(%1broadcastingStationButton3.png);}").arg(ImagesDir));
        m_isRadioButtonClicked = false;
    }
    else if (m_isRankingButtonClicked)
    {
        m_rankingButton->setStyleSheet(QString("QPushButton:!hover{border-image: url(%1listButton.png);}"
                                            "QPushButton:hover{border-image: url(%1listButton2.png);}"
                                            "QPushButton:pressed{border-image: url(%1listButton3.png);}").arg(ImagesDir));
        m_isRankingButtonClicked = false;
    }
    else if (m_isDownloadListButtonClicked)
    {
        m_downLoadListButton->setStyleSheet(QString("QPushButton:!hover{border-image: url(%1downLoadListButton.png);}"
                                            "QPushButton:hover{border-image: url(%1downLoadListButton2.png);}"
                                            "QPushButton:pressed{border-image: url(%1downLoadListButton3.png);}").arg(ImagesDir));
        m_isDownloadListButtonClicked = false;
    }
    else if (m_isCollectionButtonClicked)
    {
        m_collectionButton->setStyleSheet(QString("QPushButton:!hover{border-image: url(%1favoritesListButton.png);}"
                                            "QPushButton:hover{border-image: url(%1favoritesListButton2.png);}"
                                            "QPushButton:pressed{border-image: url(%1favoritesListButton3.png);}").arg(ImagesDir));
        m_isCollectionButtonClicked = false;
    }
    else if (m_isLrcPortraitButtonClicked)
    {
        m_lrcPortraitButton->setStyleSheet(QString("QPushButton:!hover{border-image: url(%1lyricsPhotoBarButton.png);}"
                                            "QPushButton:hover{border-image: url(%1lyricsPhotoBarButton2.png);}"
                                            "QPushButton:pressed{border-image: url(%1lyricsPhotoBarButton3.png);}").arg(ImagesDir));
        m_isLrcPortraitButtonClicked = false;
    }
    /*Else do nothing, and continue.*/
}
