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
* \file   Spectrograph.h
* \author chengxuan   787280310@qq.com
* \date   2015-02-03
* \brief  频谱绘制头文件。
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

#ifndef SPECTROGRAPH_H
#define SPECTROGRAPH_H

/*! \def SPECTROGRAPH_VERSION
*  版本控制宏，对应v3.0.0版本.
*/
#define SPECTROGRAPH_VERSION   0x030000

#include <QLabel>
#include <QString>
#include <vector>
using namespace std;

class QPaintEvent;

QT_FORWARD_DECLARE_CLASS(QMouseEvent)

/*! 频谱绘制
*/
class Spectrograph : public QLabel
{
    Q_OBJECT

public:
	/*! 构造函数
	 *  \param parent 父窗口指针，默认为NULL
	 */
    Spectrograph(QWidget *parent = NULL);

	/*! 析构函数.*/
    ~Spectrograph();

public:
	/*! 设置参数
	 *  \param lowFreq 频谱下界
	 *  \param highFreq 频谱上节
	 */
    void setParams(const double &lowFreq, const double &highFreq);

	/*! 重置频谱
	 */
	void reset();													
	
protected:
	/*! 虚函数，重新实现paintEvent函数
	 *  \param event QPaintEvent类型
	 */
    virtual void paintEvent(QPaintEvent *event);

public slots:
	/*! 频谱数据改变
	 *  \param vecFrequency 频谱数据
	 */
	void handleSpectrumChanged(const vector<float> &vecFrequency);

private:
    double m_lowFreq;								/*!< 频谱下界*/
    double m_highFreq;								/*!< 频谱上界*/
	vector<float> m_vecFrequency;					/*!< 通过快速傅里叶变换传过来的参数*/
};

#endif // SPECTROGRAPH_H
