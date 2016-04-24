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
 * \file   SearchPanel.cpp
 * \author chengxuan   787280310@qq.com
 * \date   2015-02-23
 * \brief  搜索面板实现文件。
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

#include "SearchPanel.h"
#include "SearchThread.h"
#include "NoFocusDelegate.h"
#include "Global.h"
#include "TR.h"
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QCheckBox>
#include <QTableWidget>
#include <QHeaderView>
#include <QTableWidgetItem>
#include <QSpacerItem>
#include <QEvent>
#include <QKeyEvent>
#ifdef CZ_DEBUG
#include <QDebug>
#endif

/*****************************************************************************
  这里用来检查版本的对应
 *****************************************************************************/
#if SEARCHTHREAD_VERSION < 0x030000
#error  "SearchThread version error."
#endif

#if GLOBAL_VERSION < 0x030000
#error  "Global version error."
#endif

#if TR_VERSION < 0x030000
#error  "TR version error."
#endif

SearchPanel::SearchPanel(QWidget *parent)
    : QWidget(parent),
      m_searchThread(NULL),
      m_searchThreadEvent(NULL),
      m_isSearching(false)
{
    this->createUI();
    this->createConnect();
    this->createMusicSearchThread();
}

SearchPanel::~SearchPanel()
{
    if (m_searchThreadEvent != NULL)
    {
        m_searchThreadEvent->deleteLater();
        m_searchThreadEvent = NULL;
    }
    /*Else do nothing, and continue.*/

    if (m_searchThread != NULL)
    {
        if (m_searchThread->isRunning())
        {
            m_searchThread->quit();
            m_searchThread->wait();
        }
        /*Else do nothing, and continue.*/
        m_searchThread->deleteLater();
        m_searchThread = NULL;
    }
    /*Else do nothing, and continue.*/

    m_searchMap.clear();
}

bool SearchPanel::eventFilter(QObject *target, QEvent *event)
{
    if (target == m_searchEdit)
    {
        if (event->type() == QEvent::KeyPress)
        {
            QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
            if (keyEvent->key() == Qt::Key_Return)
            {
                m_searchButton->click();
                return true;
            }
            /*Else do nothing, and continue.*/
        }
        /*Else do nothing, and continue.*/
    }
    /*Else do nothing, and continue.*/

    return QWidget::eventFilter(target, event);
}

void SearchPanel::handleSearchMusic()
{
    QString text = m_searchEdit->text();
    if (text.isEmpty() || m_isSearching)
    {
        return;
    }
    /*Else do nothing, and continue.*/

    m_isSearching = true;
    m_searchInfoLabel->setText(TR("正在搜索，请稍后..."));
    m_allChecked->setCheckState(Qt::Unchecked);
    m_searchMap.clear();

    //清空搜索列表
    m_searchTable->clear();
    while (m_searchTable->rowCount() > 0)
    {
        m_searchTable->removeRow(0);
    }

    emit sigSearchMusic(text);
}

void SearchPanel::handleAddMusicInfo(const QString &musicName, const QString &artist, const QString &album)
{
    //歌曲名
    QCheckBox *musicNameCheckBox = new QCheckBox(musicName);
    musicNameCheckBox->setObjectName(tr("musicNameCheckBox"));
    musicNameCheckBox->setFont(QFont("微软雅黑", 10));
    musicNameCheckBox->setStyleSheet(QString("QCheckBox{color:white;}"
                                        "QCheckBox::indicator{width:10px;height:10px;border: 1px solid white;border-radius:2px}"
                                        "QCheckBox::indicator:checked {image: url(%1checked2.png);}").arg(ImagesDir));
    connect(musicNameCheckBox, SIGNAL(clicked(bool)), this, SLOT(handleSelectOneMusic(bool)));

    //艺人
    QTableWidgetItem *artistItem = new QTableWidgetItem(artist);
    artistItem->setTextAlignment(Qt::AlignCenter);
    artistItem->setFont(QFont(TR("微软雅黑"), 10));
    m_searchMap[musicNameCheckBox] = artistItem->text();

    //专辑
    QTableWidgetItem *albumItem = new QTableWidgetItem(album);
    albumItem->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    albumItem->setFont(QFont(TR("微软雅黑"), 10));

    QString tipInfo = TR("歌曲名：%1\n艺术家：%2\n专辑：%3").arg(musicName).arg(artist).arg(album);
    musicNameCheckBox->setToolTip(tipInfo);
    artistItem->setToolTip(tipInfo);
    albumItem->setToolTip(tipInfo);

    //插入播放列表
    int currentRows = m_searchTable->rowCount();
    m_searchTable->insertRow(currentRows);
    m_searchTable->setCellWidget(currentRows, 0, musicNameCheckBox);
    m_searchTable->setItem(currentRows, 1, artistItem);
    m_searchTable->setItem(currentRows, 2, albumItem);
}

void SearchPanel::handleDownloadMusic()
{    
    QMapIterator<QCheckBox*, QString> i(m_searchMap);
    while (i.hasNext())
    {
        i.next();
        QCheckBox *checkBox = i.key();
        if (checkBox->checkState() == Qt::Checked)
        {
            QString text = checkBox->text();
            if (text.contains(QChar(10)))
            {
                QStringList list = text.split(QChar(10));
                emit sigDownloadMusic(list.at(0), i.value());
            }
            else
            {
                emit sigDownloadMusic(checkBox->text(), i.value());
            }
        }
        /*Else do nothing, and continue.*/
    }
}

void SearchPanel::handleCollectMusic()
{
    QMapIterator<QCheckBox*, QString> i(m_searchMap);
    while (i.hasNext())
    {
        i.next();
        QCheckBox *checkBox = i.key();
        if (checkBox->checkState() == Qt::Checked)
        {
            emit sigCollectMusic(checkBox->text(), i.value());
        }
        /*Else do nothing, and continue.*/
    }
}

void SearchPanel::handleSelectAllMusic(bool isChecked)
{
    if (isChecked)
    {
        QMapIterator<QCheckBox*, QString> i(m_searchMap);
        while (i.hasNext())
        {
            i.next();
            QCheckBox *checkBox = i.key();
            checkBox->setCheckState(Qt::Checked);
        }
    }
    else
    {
        QMapIterator<QCheckBox*, QString> i(m_searchMap);
        while (i.hasNext())
        {
            i.next();
            QCheckBox *checkBox = i.key();
            checkBox->setCheckState(Qt::Unchecked);
        }
    }
}

void SearchPanel::handleSelectOneMusic(bool isChecked)
{
    if (isChecked)
    {
        bool isFound = false;
        QMapIterator<QCheckBox*, QString> i(m_searchMap);
        while (i.hasNext())
        {
            i.next();
            QCheckBox *checkBox = i.key();
            if (checkBox->checkState() == Qt::Unchecked)
            {
                m_allChecked->setCheckState(Qt::Unchecked);
                isFound = true;
                break;
            }
            /*Else do nothing, and continue.*/
        }
        if (!isFound)
        {
            m_allChecked->setCheckState(Qt::Checked);
        }
        /*Else do nothing, and continue.*/
    }
    else
    {
        m_allChecked->setCheckState(Qt::Unchecked);
    }
}

void SearchPanel::handleMusicSearchStatus(MusicSearchStatus status)
{
    m_isSearching = false;
    if (status == MusicSearchSuccess)
    {
        m_searchInfoLabel->setText(TR("搜索歌曲完成！"));
    }
    else if (status == MusicSearchFailed)
    {
        m_searchInfoLabel->setText(TR("搜索歌曲失败，请检查网络是否正常！"));
    }
    else
    {
#ifdef CZ_DEBUG
        qDebug() << "Nof found this status, status = " << status;
        qDebug() << __FILE__ << __FUNCTION__ << __LINE__;
#endif
    }
}

void SearchPanel::createUI()
{
    //搜索框
    m_searchEdit = new QLineEdit(this);
    m_searchEdit->setObjectName(tr("searchLineEdit"));
    m_searchEdit->installEventFilter(this);
    m_searchEdit->setPlaceholderText(TR("搜索歌手、歌曲、专辑"));
    m_searchEdit->setFixedSize(250, 30);

    //搜索按钮
    m_searchButton = new QPushButton(this);
    m_searchButton->setObjectName(tr("searchButton"));
    m_searchButton->setFixedSize(90, 30);

    QHBoxLayout *searchEditLayout = new QHBoxLayout;
    searchEditLayout->addWidget(m_searchEdit);
    searchEditLayout->addWidget(m_searchButton);

    //搜索信息显示
    m_searchInfoLabel = new QLabel(this);
    m_searchInfoLabel->setObjectName(tr("searchInfoLabel"));
    m_searchInfoLabel->setText(TR("CZPlayer，极致音乐"));

    //全选
    m_allChecked = new QCheckBox(TR("全选"), this);
    m_allChecked->setFont(QFont(TR("微软雅黑"), 10));
    m_allChecked->setStyleSheet(QString("QCheckBox{color:white;}"
                                        "QCheckBox::indicator{width:10px;height:10px;border: 1px solid white;border-radius:2px}"
                                        "QCheckBox::indicator:checked {image: url(%1checked2.png);}").arg(ImagesDir));

    //下载按钮
    m_downloadButton = new QPushButton(this);
    m_downloadButton->setObjectName(tr("downButton"));
    m_downloadButton->setToolTip(TR("下载"));
    m_downloadButton->setFixedSize(16, 16);

    //收藏
    m_collectButton = new QPushButton(this);
    m_collectButton->setObjectName(tr("collectButton"));
    m_collectButton->setToolTip(TR("收藏"));
    m_collectButton->setFixedSize(16, 16);

    //搜索列表
    m_searchTable = new QTableWidget(this);
    m_searchTable->setObjectName(tr("searchList"));
    this->setTableWidgetStyle();

    QSpacerItem *spacerItem = new QSpacerItem(50, 10, QSizePolicy::Fixed, QSizePolicy::Fixed);

    QHBoxLayout *buttonhLayout = new QHBoxLayout;
    buttonhLayout->addWidget(m_allChecked);
    buttonhLayout->addStretch();
    buttonhLayout->addWidget(m_downloadButton);
    buttonhLayout->addWidget(m_collectButton);
    buttonhLayout->addItem(spacerItem);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(searchEditLayout);
    mainLayout->addWidget(m_searchInfoLabel);
    mainLayout->addLayout(buttonhLayout);
    mainLayout->addWidget(m_searchTable);
    //mainLayout->addStretch();
    this->setLayout(mainLayout);
}

void SearchPanel::createConnect()
{
    connect(m_searchButton, SIGNAL(clicked()), this, SLOT(handleSearchMusic()));
    connect(m_downloadButton, SIGNAL(clicked()), this, SLOT(handleDownloadMusic()));
    connect(m_collectButton, SIGNAL(clicked()), this, SLOT(handleCollectMusic()));
    connect(m_allChecked, SIGNAL(clicked(bool)), this, SLOT(handleSelectAllMusic(bool)));
}

void SearchPanel::createMusicSearchThread()
{
    //创建音乐搜索线程
    if (m_searchThread == NULL)
    {
        m_searchThread = new SearchThread;
        m_searchThread->start();

        if (m_searchThreadEvent == NULL)
        {
            m_searchThreadEvent = new SearchThreadEvent;
            m_searchThreadEvent->moveToThread(m_searchThread);

            qRegisterMetaType<MusicSearchStatus>("MusicSearchStatus");
            connect(this, SIGNAL(sigSearchMusic(QString)), m_searchThreadEvent, SLOT(handleSearchMusic(QString)), Qt::QueuedConnection);
            connect(m_searchThreadEvent, SIGNAL(sigAddMusicInfo(QString,QString,QString)), this, SLOT(handleAddMusicInfo(QString,QString,QString)), Qt::QueuedConnection);
            connect(m_searchThreadEvent, SIGNAL(sigMusicSearchStatus(MusicSearchStatus)), this, SLOT(handleMusicSearchStatus(MusicSearchStatus)), Qt::QueuedConnection);
        }
        /*Else do nothing, and continue.*/
    }
    /*Else do nothing, and continue.*/
}

void SearchPanel::setTableWidgetStyle()
{
    m_searchTable->setRowCount(0);
    m_searchTable->setColumnCount(3);

    m_searchTable->horizontalHeader()->setVisible(false);                       //设置水平头不可见
    m_searchTable->verticalHeader()->setVisible(false);                         //设置垂直头不可见
    m_searchTable->setSelectionMode(QAbstractItemView::SingleSelection);        //设置只能选中单行
    m_searchTable->setSelectionBehavior(QAbstractItemView::SelectRows);         //设置选择习惯为选择行
    m_searchTable->setEditTriggers(QAbstractItemView::NoEditTriggers);          //设置内容不可编辑
    m_searchTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);     //设置内容自动横向填满可用区域
    m_searchTable->setFrameStyle(QFrame::NoFrame);                              //去掉边框
    m_searchTable->setShowGrid(false);                                          //设置不显示网格
    m_searchTable->setItemDelegate(new NoFocusDelegate());                      //去掉选中单元格的虚线
    m_searchTable->horizontalHeader()->setHighlightSections(false);             //点击表时不对表头行光亮（获取焦点）
}
