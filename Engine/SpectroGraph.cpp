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
* \file   Spectrograph.cpp
* \author chengxuan   787280310@qq.com
* \date   2015-02-03
* \brief  频谱绘制实现文件。
* \version 3.0.0
*
* \verbatim
* 历史
*          3.0.0   创建,
*          2015-02-03    by chengxuan
*
* \endverbatim
*
*/

#include "spectrograph.h"
#include <QPaintEvent>
#include <QPainter>
#include <QColor>
#include <QBitmap>

Spectrograph::Spectrograph(QWidget *parent) 
	: QLabel(parent),
	  m_lowFreq(0.0),
	  m_highFreq(0.0)
{
	this->resize(190, 78);
	m_vecFrequency.clear();
}

Spectrograph::~Spectrograph()
{
	/*Do nothing.*/
}

void Spectrograph::setParams(const double &lowFreq, const double &highFreq)
{
    Q_ASSERT(highFreq > lowFreq);
    m_lowFreq = lowFreq;		
    m_highFreq = highFreq;		
}

void Spectrograph::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QPainter painter(this);

#if 0
	QPixmap backgroundImage;
	backgroundImage.load(":/app/images/screen.png");
	//先通过pix的方法获得图片的过滤掉透明的部分得到的图片，作为loginPanel的不规则边框
	this->setMask(backgroundImage.mask());
	painter.drawPixmap(0, 0, 190, 78, backgroundImage);
#endif

	const int numBars = m_vecFrequency.size();

	QColor barColor(5, 184, 204);		//频谱bar颜色

    barColor = barColor.lighter();
    barColor.setAlphaF(0.75);		//设置alpha通道

    //绘制频谱
    if (numBars)
	{
        //计算宽度的条和空白
		const int widgetWidth = this->width();										//频谱widget宽度
        const int barPlusGapWidth = widgetWidth / numBars;							//每一个频谱加空白间隙的宽度
        const int barWidth = 0.8 * barPlusGapWidth;									//每一个频谱bar的宽度
        const int gapWidth = barPlusGapWidth - barWidth;								//每一个空白间隙宽度
        const int paddingWidth = widgetWidth - numBars * (barWidth + gapWidth);		//边缘宽度
        const int leftPaddingWidth = (paddingWidth + gapWidth) / 2;					//左边缘宽度
        const int barHeight = this->height() - 2 * gapWidth;							//每一个频谱bar的高度

		//绘制每一个频谱bar
        for (int i = 0; i < numBars; ++i)
		{
            const double value = m_vecFrequency[i];		//vlaue的值在0到1之间
            Q_ASSERT(value >= 0.0 && value <= 1.0);

            QRect bar = rect();
			//设置频谱bar的位置和大小
            bar.setLeft(rect().left() + leftPaddingWidth + (i * (gapWidth + barWidth)));
            bar.setWidth(barWidth);
            bar.setTop(rect().top() + gapWidth + (1.0 - value) * barHeight);
            bar.setBottom(rect().bottom() - gapWidth);
#if 0
			//设置颜色渐变
			QLinearGradient linearGradient(bar.topLeft(), bar.bottomRight());
			linearGradient.setColorAt(0.1, QColor(247, 104, 9));
			linearGradient.setColorAt(1.0, QColor(238, 17, 128));
			painter.fillRect(bar, QBrush(linearGradient));
#else
			painter.fillRect(bar, barColor);
#endif
        }
    }
	event->accept();
}

void Spectrograph::handleSpectrumChanged( const vector<float> &vecFrequency )
{
	m_vecFrequency = vecFrequency;
	this->update();						
}

void Spectrograph::reset()
{
	m_vecFrequency.clear();
	this->update();
}

