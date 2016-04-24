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
* \file   LrcDemo.h
* \author chengxuan   787280310@qq.com
* \date   2015-01-24
* \brief  歌词模板头文件。
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

#ifndef LRCDEMO_H
#define LRCDEMO_H

/*! \def LRCDEMO_VERSION
*  版本控制宏，对应v3.0.0版本.
*/
#define LRCDEMO_VERSION   0x030000

#include <QLabel>
#include <QString>
#include <QLinearGradient>
#include <QFont>
#include <QColor>

class QPaintEvent;

/*! 歌词模板
*/
class LrcDemo : public QLabel
{
    Q_OBJECT

public:
	/*! 构造函数
	 *  \param parent 父窗口指针，默认为NULL
	 */
	LrcDemo(QWidget *parent = NULL);

	/*! 析构函数.*/
    ~LrcDemo();

public:
	/*! 设置歌词字体
	 * \param fontName 字体名称
	 */
	void setFontName(const QString &fontName);

	/*! 设置歌词字体大小
	 * \param fontSize 字体大小
	 */
	void setFontSize(const QString &fontSize);

	/*! 设置歌词类型
	 * \param fontType 歌词类型
	 */
	void setFontType(const QString &fontType);

	/*! 设置歌词没有播放上边缘颜色
	 * \param color 颜色
	 */
	void setLinearGradient(const QColor &color);

	/*! 设置歌词没有播放中间颜色
	 * \param color 颜色
	 */
	void setLinearGradient2(const QColor &color);

	/*! 设置歌词没有播放下边缘颜色
	 * \param color 颜色
	 */
	void setLinearGradient3(const QColor &color);

	/*! 设置歌词播放后上边缘颜色
	 * \param color 颜色
	 */
	void setMaskLinearGradient(const QColor &color);

	/*! 设置歌词播放后中间颜色
	 * \param color 颜色
	 */
	void setMaskLinearGradient2(const QColor &color);

	/*! 设置歌词播放后下边缘颜色
	 * \param color 颜色
	 */
	void setMaskLinearGradient3(const QColor &color);

	/*! 设置歌词灰度
	 * \param shadow 灰度
	 */
	void setLrcDemoShadow(const int &shadow);

protected:
	/*! 虚函数，重新实现paintEvent函数
	 *  \param event QPaintEvent类型
	 */
    virtual void paintEvent(QPaintEvent *event);

private:
	/*! 设置mask长度
	 * \param maskWidth mask长度
	 */
	void setLrcMaskWidth(const double &maskWidth);

private:
    QLinearGradient m_linearGradient;			/*!< 歌词的线性渐变*/
    QLinearGradient m_maskLinearGradient;		/*!< 歌词的遮罩线性渐变*/
    QFont m_font;								/*!< 字体*/
	double m_lrcMaskWidth;						/*!< mask长度*/
    int m_shadow;								/*!< 灰度*/
};

#endif // LRCDEMO_H


































