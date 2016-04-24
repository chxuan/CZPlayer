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
 * \file   MusicLrc.cpp
 * \author chengxuan   787280310@qq.com
 * \date   2014-12-21
 * \brief  歌词界面实现文件。
 * \version 3.0.0
 *
 * \verbatim
 * 历史
 *          3.0.0   创建,
 *          2014-12-21    by chengxuan
 *
 * \endverbatim
 *
 */

#include "MusicLrc.h"
#include "TR.h"
#include "GlobalConfig.h"
#include "Global.h"
#include <QApplication>
#include <QMenu>
#include <QIcon>
#include <QDesktopWidget>
#include <QMessagebox>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QContextMenuEvent>
#include <QTimer>
#ifdef CZ_DEBUG
#include <QDebug>
#endif

/*****************************************************************************
  这里用来检查版本的对应
 *****************************************************************************/
#if GLOBALCONFIG_VERSION < 0x030000
#error  "GlobalConfig version error."
#endif

#if GLOBAL_VERSION < 0x030000
#error  "Global version error."
#endif

#if TR_VERSION < 0x030000
#error  "TR version error."
#endif

MusicLrc::MusicLrc( QWidget *parent /*= NULL*/ )
	: QLabel(parent),
	m_isLrcLock(false),
	m_lrcMaskWidth(0.0),
	m_lrcMaskWidthInterval(0.0),
	m_shadow(255)
{
	this->createUI();
	this->createConnect();
}

MusicLrc::~MusicLrc()
{
	if (m_timer->isActive()) 
	{
		m_timer->stop();
	}
	/*Else do nothing, and continue.*/
}

void MusicLrc::createUI()
{
	//去掉窗体边框和总是显示在屏幕最前面,但要和showNormal一起用
	this->setWindowFlags(Qt::Tool | Qt::WindowStaysOnTopHint
		| Qt::FramelessWindowHint);			//去掉窗体边框,不在任务栏显示图标
	this->setAttribute(Qt::WA_TranslucentBackground);			//设置背景透明
	this->setWindowIcon(QIcon(ImagesDir + "CZPlayer.png"));		//设置logo
	this->resize(1000, 60);

	//设置歌词位置
	double width = QApplication::desktop()->width();
	double height = QApplication::desktop()->height();
	this->move(QPoint(width / 2 - 800 / 2, height - 90));

	//歌词的线性渐变填充
	m_linearGradient.setStart(0, 10);//填充的起点坐标
	m_linearGradient.setFinalStop(0, 40);//填充的终点坐标

	//遮罩的线性渐变填充
	m_maskLinearGradient.setStart(0, 10);
	m_maskLinearGradient.setFinalStop(0, 40);

	//设置定时器
	m_timer = new QTimer;
}

void MusicLrc::createConnect()
{
	connect(m_timer, SIGNAL(timeout()), this, SLOT(handleTimeOut()));

	GlobalConfig *config = GlobalConfig::instance();
	connect(config, SIGNAL(sigFontNameChanged(QString)), this, SLOT(handleFontNameChanged(QString)));
	connect(config, SIGNAL(sigFontSizeChanged(QString)), this, SLOT(handleFontSizeChanged(QString)));
	connect(config, SIGNAL(sigFontTypeChanged(QString)), this, SLOT(handleFontTypeChanged(QString)));
	connect(config, SIGNAL(sigNoPlayShangChanged(QColor)), this, SLOT(handleNoPlayShangChanged(QColor)));
	connect(config, SIGNAL(sigNoPlayZhongChanged(QColor)), this, SLOT(handleNoPlayZhongChanged(QColor)));
	connect(config, SIGNAL(sigNoPlayXiaChanged(QColor)), this, SLOT(handleNoPlayXiaChanged(QColor)));
	connect(config, SIGNAL(sigPlayedShangChanged(QColor)), this, SLOT(handlePlayedShangChanged(QColor)));
	connect(config, SIGNAL(sigPlayedZhongChanged(QColor)), this, SLOT(handlePlayedZhongChanged(QColor)));
	connect(config, SIGNAL(sigPlayedXiaChanged(QColor)), this, SLOT(handlePlayedXiaChanged(QColor)));
	connect(config, SIGNAL(sigShadowChanged(int)), this, SLOT(handleShadowChanged(int)));
}

void MusicLrc::startLrcMask(qint64 intervalTime)
{
    //这里设置每隔30毫秒更新一次遮罩的宽度，因为如果更新太平凡
    //会增加cpu占用率，而如果时间间隔太大，则动画效果就不流畅了
    qreal count = intervalTime / 30;

    //获取遮罩每次需要增加的宽度，这里的800是部件的固定宽度
    m_lrcMaskWidthInterval = 1000 / count;
    m_lrcMaskWidth = 0;
    m_timer->start(30);
}

void MusicLrc::stopLrcMask()
{
    m_timer->stop();
    m_lrcMaskWidth = 0;
    this->update();
}

void MusicLrc::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
	painter.setFont(m_font);

    //先绘制底层文字，作为阴影，这样会使显示效果更加清晰，且更有质感
	painter.setPen(QColor(0, 0, 0, m_shadow));
	painter.drawText(1, 1, 1000, 60, Qt::AlignLeft, text());

    //再在上面绘制渐变文字
	painter.setPen(QPen(m_linearGradient, 0));
	painter.drawText(0, 0, 1000, 60, Qt::AlignLeft, text());

    //设置歌词遮罩
	painter.setPen(QPen(m_maskLinearGradient, 0));
	painter.drawText(0, 0, m_lrcMaskWidth, 60, Qt::AlignLeft, text());
    event->accept();
}

void MusicLrc::mousePressEvent(QMouseEvent *event)
{
    if (!m_isLrcLock)//没有开启歌词锁定
    {
        if (event->button() == Qt::LeftButton)
        {
            m_dragPosition = event->globalPos() - frameGeometry().topLeft();
            event->accept();
        }
		/*Else do nothing, and continue.*/
    }
	/*Else do nothing, and continue.*/
}

void MusicLrc::mouseMoveEvent(QMouseEvent *event)
{
    if (!m_isLrcLock)//没有开启歌词锁定
    {
        if (event->buttons() == Qt::LeftButton)
        {
            setCursor(Qt::PointingHandCursor);//显示手型
            move(event->globalPos() - m_dragPosition);//移动窗体
            event->accept();
        }
		/*Else do nothing, and continue.*/
    }
	/*Else do nothing, and continue.*/
}

void MusicLrc::contextMenuEvent(QContextMenuEvent *event)
{
    if (!m_isLrcLock)//没有开启歌词锁定
    {
        QMenu menu;
        menu.addAction(QIcon(ImagesDir + "lrc.png"), TR("隐藏歌词"), this, SIGNAL(sigHideLrc()));
        menu.addAction(QIcon(ImagesDir + "lock.png"), TR("锁定歌词"), this, SLOT(handleLockLrc()));
        menu.exec(event->globalPos());
    }
	/*Else do nothing, and continue.*/
    event->accept();
}

void MusicLrc::handleLockLrc()
{
    m_isLrcLock = true;
    emit sigLrcLocked();
}

void MusicLrc::unlockLrc()
{
    m_isLrcLock = false;
    emit sigLrcUnlocked();
}

void MusicLrc::handleTimeOut()
{
    //每隔一段固定的时间笼罩的长度就增加一点
    m_lrcMaskWidth += m_lrcMaskWidthInterval;
	//更新歌词显示，但是并不立即重绘，而是安排一个Paint事件，当返回主循环时由系统来重绘
    this->update();
}

void MusicLrc::handleFontNameChanged(const QString &fontName)
{
	m_font.setFamily(fontName);
	this->update();
}

void MusicLrc::handleFontSizeChanged(const QString &fontSize)
{
	bool ok;
	int size = fontSize.toInt(&ok);
	if (ok)
	{
		m_font.setPointSize(size);
	}
	else
	{
		m_font.setPointSize(30);
	}
	this->update();
}

void MusicLrc::handleFontTypeChanged(const QString &fontType)
{
	if (fontType == TR("常规"))
	{
		m_font.setBold(false);
		m_font.setItalic(false);
	}
	else if (fontType == TR("粗体"))
	{
		m_font.setBold(true);
		m_font.setItalic(false);
	}
	else if (fontType == TR("倾斜"))
	{
		m_font.setBold(false);
		m_font.setItalic(true);
	}
	else if (fontType == TR("粗体 倾斜"))
	{
		m_font.setBold(true);
		m_font.setItalic(true);
	}
	/*Else do nothing, and conintue.*/
	this->update();
}

void MusicLrc::handleNoPlayShangChanged(const QColor &color)
{
	m_linearGradient.setColorAt(0.1, color);
	this->update();
}

void MusicLrc::handleNoPlayZhongChanged(const QColor &color)
{
	m_linearGradient.setColorAt(0.5, color);
	this->update();
}

void MusicLrc::handleNoPlayXiaChanged(const QColor &color)
{
	m_linearGradient.setColorAt(0.9, color);
	this->update();
}

void MusicLrc::handlePlayedShangChanged(const QColor &color)
{
	m_maskLinearGradient.setColorAt(0.1, color);
	this->update();
}

void MusicLrc::handlePlayedZhongChanged(const QColor &color)
{
	m_maskLinearGradient.setColorAt(0.5, color);
	this->update();
}

void MusicLrc::handlePlayedXiaChanged(const QColor &color)
{
	m_maskLinearGradient.setColorAt(0.9, color);
	this->update();
}

void MusicLrc::handleShadowChanged(const int &shadow)
{
	m_shadow = shadow;
	this->update();
}














