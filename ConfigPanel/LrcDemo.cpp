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
* \file   LrcDemo.cpp
* \author chengxuan   787280310@qq.com
* \date   2015-01-24
* \brief  歌词模板实现文件。
* \version 3.0.0
*
* \verbatim
* 历史
*          3.0.0   创建,
*          2015-01-24    by chengxuan
*
* \endverbatim
*
*/

#include "LrcDemo.h"
#include "TR.h"
#include <QPaintEvent>
#include <QPainter>

/*****************************************************************************
  这里用来检查版本的对应
 *****************************************************************************/
#if TR_VERSION < 0x030000
#error  "TR version error."
#endif

LrcDemo::LrcDemo(QWidget *parent)
	: QLabel(parent),
	  m_lrcMaskWidth(0),
	  m_shadow(255)
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->resize(500, 60);
	this->setText(TR("CZPlayer,传播好音乐"));
    this->setAlignment(Qt::AlignCenter);

    //歌词的线性渐变填充
    m_linearGradient.setStart(0, 10);//填充的起点坐标

    //遮罩的线性渐变填充
    m_maskLinearGradient.setStart(0, 10);
    m_maskLinearGradient.setFinalStop(0, 40);

    this->update();
}

LrcDemo::~LrcDemo()
{
	/*Do nothing.*/
}

void LrcDemo::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setFont(m_font);
	
    //先绘制底层文字，作为阴影，这样会使显示效果更加清晰，且更有质感
    painter.setPen(QColor(0, 0, 0, m_shadow));
    painter.drawText(1, 1, 500, 60, Qt::AlignLeft, text());

    //再在上面绘制渐变文字
    painter.setPen(QPen(m_linearGradient, 0));
    painter.drawText(0, 0, 500, 60, Qt::AlignLeft, text());

    //设置歌词遮罩
    painter.setPen(QPen(m_maskLinearGradient, 0));
    painter.drawText(0, 0, m_lrcMaskWidth, 60, Qt::AlignLeft, text());
    event->accept();
}

void LrcDemo::setFontName(const QString &fontName)
{
	m_font.setFamily(fontName);
	this->update();
}

void LrcDemo::setFontSize(const QString &fontSize)
{
	bool ok;
	int size = fontSize.toInt(&ok);
	if (ok)
	{
		m_font.setPointSize(size);
		this->setLrcMaskWidth(size * 13 / 2);
	}
	else
	{
		m_font.setPointSize(30);
		this->setLrcMaskWidth(30 * 13 / 2);
	}
	this->update();
}

void LrcDemo::setFontType(const QString &fontType)
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

void LrcDemo::setLinearGradient(const QColor &color)
{
	m_linearGradient.setColorAt(0.1, color);
	this->update();
}

void LrcDemo::setLinearGradient2(const QColor &color)
{
	m_linearGradient.setColorAt(0.5, color);
	this->update();
}

void LrcDemo::setLinearGradient3(const QColor &color)
{
	m_linearGradient.setColorAt(0.9, color);
	this->update();
}

void LrcDemo::setMaskLinearGradient(const QColor &color)
{
	m_maskLinearGradient.setColorAt(0.1, color);
	this->update();
}

void LrcDemo::setMaskLinearGradient2(const QColor &color)
{
	m_maskLinearGradient.setColorAt(0.5, color);
	this->update();
}

void LrcDemo::setMaskLinearGradient3(const QColor &color)
{
	m_maskLinearGradient.setColorAt(0.9, color);
	this->update();
}

void LrcDemo::setLrcDemoShadow(const int &shadow)
{
	m_shadow = shadow;
	this->update();
}

void LrcDemo::setLrcMaskWidth(const double &maskWidth)
{
	m_lrcMaskWidth = maskWidth;
	this->update();
}


























