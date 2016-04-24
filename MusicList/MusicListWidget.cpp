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
 * \file   MusicListWidget.cpp
 * \author chengxuan   787280310@qq.com
 * \date   2015-01-08
 * \brief  歌曲列表面板实现文件。
 * \version 3.0.0
 *
 * \verbatim
 * 历史
 *          3.0.0   创建,
 *          2015-01-08    by chengxuan
 *
 * \endverbatim
 *
 */

#include "MusicListWidget.h"
#include "MusicList.h"
#include "ChineseToLetter.h"
#include "Global.h"
#include "TR.h"
#include <QPaintEvent>
#include <QTimer>
#include <QMenu>
#include <QLineEdit>
#include <QPushButton>
#include <QSpacerItem>
#include <QVBoxLayout>
#include <QPainter>
#include <QBitmap>
#include <QDebug>

/*****************************************************************************
  这里用来检查版本的对应
 *****************************************************************************/
#if MUSICLIST_VERSION < 0x030000
#error  "MusicList version error."
#endif

#if CHINESETOLETTER_VERSION < 0x030000
#error  "ChineseToLetter version error."
#endif

#if GLOBAL_VERSION < 0x030000
#error  "Global version error."
#endif

#if TR_VERSION < 0x030000
#error  "TR version error."
#endif

MusicListWidget::MusicListWidget(MusicList *playList, 
								QMap<QString, int> *mapMusicRows,
								QWidget *parent)
    : QWidget(parent),
	m_playList(playList), 
	m_mapMusicRows(mapMusicRows), 
	m_parent(parent)
{
	this->createUI();
	this->createConnect();			
}

MusicListWidget::~MusicListWidget()
{
	/*Do nothing.*/
}

void MusicListWidget::handleOnClickedAddMusic()
{
	m_addMusicMenu->exec(QCursor::pos());
}

void MusicListWidget::handleLineEditFocus()
{
    if (m_searchLineEdit->text().isEmpty() && !m_searchLineEdit->hasFocus())
    {
        m_backButton->setVisible(false);
    }
	/*Else do nothing, and continue.*/
}

void MusicListWidget::handleBackMusicList()
{
    for (int index = 0; index < m_playList->rowCount(); ++index)
    {
        m_playList->showRow(index);
    }
}

void MusicListWidget::handleSearchMusic()
{
    QString text = m_searchLineEdit->text();
    if (text.isEmpty())
    {
        this->handleBackMusicList();
        return;
    }
    /*Else do nothing, and continue.*/

	QMap<QString, int>::iterator begin = m_mapMusicRows->begin();
	QMap<QString, int>::iterator end = m_mapMusicRows->end();
    while (begin != end)
    {
        //将汉字转换成拼音
        QString musicTitleletter = QString::fromStdString(getLetter(begin.key().toLocal8Bit().data()));
        if (begin.key().contains(text) ||
            musicTitleletter.contains(text, Qt::CaseInsensitive))
        {
            m_playList->showRow(begin.value());
        }
        else
        {
            m_playList->hideRow(begin.value());
        }
        ++begin;
    }
}

void MusicListWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QPixmap backgroundImage;
    backgroundImage.load(ImagesDir + "musicList.png");

    this->setMask(backgroundImage.mask());
    painter.drawPixmap(0, 0, 400, 450, backgroundImage);
    event->accept();
}

bool MusicListWidget::eventFilter(QObject *target, QEvent *event)
{
    if (target == m_searchLineEdit)
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

        if (event->type() == QEvent::MouseButtonPress)
        {
            if (m_searchLineEdit->text().isEmpty())
            {
                m_backButton->setVisible(true);
            }
			/*Else do nothing, and continue.*/
            return true;
        }
		/*Else do nothing, and continue.*/
    }

    return QWidget::eventFilter(target, event);
}

void MusicListWidget::createUI()
{
	this->resize(400, 450);
	this->setWindowFlags(Qt::Tool | Qt::FramelessWindowHint);
	this->setAttribute(Qt::WA_TranslucentBackground);
	this->setWindowIcon(QIcon(ImagesDir + "CZPlayer.png"));
	this->setWindowTitle(TR("音乐列表"));

	//搜索框
	m_searchLineEdit = new QLineEdit(this);
	m_searchLineEdit->setObjectName("searchLineEdit");
    m_searchLineEdit->setPlaceholderText(TR("搜索歌手、歌曲"));
	m_searchLineEdit->installEventFilter(this);

	//搜索按钮
	m_searchButton = new QPushButton(this);
	m_searchButton->setObjectName("searchButton");

	//返回
	m_backButton = new QPushButton(this);
	m_backButton->setObjectName("backButton");
	m_backButton->setToolTip(TR("返回音乐列表"));
	m_backButton->setVisible(false);

	//添加歌曲
	m_addMusicButton = new QPushButton(this);
	m_addMusicButton->setObjectName("addMusicButton");
	m_addMusicButton->setToolTip(TR("添加歌曲"));

	//定位歌曲
	m_localeButton = new QPushButton(this);
	m_localeButton->setObjectName("localeButton");
	m_localeButton->setToolTip(TR("定位当前播放歌曲"));

	//删除当前歌曲
	m_delMusicButton = new QPushButton(this);
	m_delMusicButton->setObjectName("delMusicButton");
	m_delMusicButton->setToolTip(TR("删除当前歌曲"));

	//搜索歌曲
	m_findButton = new QPushButton(this);
	m_findButton->setObjectName("findButton");
	m_findButton->setToolTip(TR("搜索歌曲"));

	//定时器
	m_editFocusTimer = new QTimer(this);
	m_editFocusTimer->start(1000);

	QSpacerItem *topItem = new QSpacerItem(380, 37, QSizePolicy::Fixed, QSizePolicy::Fixed);
	QSpacerItem *bottomItem = new QSpacerItem(380, 30, QSizePolicy::Fixed, QSizePolicy::Fixed);
	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addItem(topItem);
	mainLayout->addWidget(m_playList);
	mainLayout->addItem(bottomItem);
	this->setLayout(mainLayout);

	//添加歌曲菜单
	m_addMusicMenu = new QMenu(this);
	m_addMusicMenu->addAction(QIcon(ImagesDir + "addMusic.png"), TR("添加歌曲"), m_parent, SLOT(handleOpenFile()));
	m_addMusicMenu->addAction(QIcon(ImagesDir + "folder.png"), TR("添加歌曲文件夹"), m_parent, SLOT(handleAddMusicDir()));
	m_addMusicMenu->addAction(QIcon(ImagesDir + "addMusicList.png"), TR("载入播放列表"), m_parent, SLOT(handleAddMusicList()));

	m_searchLineEdit->setGeometry(27, 7, 250, 30);
	m_searchButton->setGeometry(287, 9, 90, 30);
	m_backButton->setGeometry(257, 15, 15, 14);
	m_addMusicButton->setGeometry(60, 417, 25, 25);
	m_localeButton->setGeometry(145, 417, 25, 25);
	m_delMusicButton->setGeometry(230, 417, 25, 25);
	m_findButton->setGeometry(315, 417, 25, 25);
}

void MusicListWidget::createConnect()
{
	connect(m_addMusicButton, SIGNAL(clicked()), this, SLOT(handleOnClickedAddMusic()));		
	connect(m_localeButton, SIGNAL(clicked()), m_playList, SLOT(handleLocateCurrentMusic()));	
	connect(m_delMusicButton, SIGNAL(clicked()), m_playList, SLOT(handleDelCurrentMusic()));	
	connect(m_searchButton, SIGNAL(clicked()), this, SLOT(handleSearchMusic()));				
	connect(m_editFocusTimer, SIGNAL(timeout()), this, SLOT(handleLineEditFocus()));				
	connect(m_backButton, SIGNAL(clicked()), this, SLOT(handleBackMusicList()));
}
