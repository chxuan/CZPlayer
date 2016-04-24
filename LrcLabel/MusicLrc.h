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
 * \file   MusicLrc.h
 * \author chengxuan   787280310@qq.com
 * \date   2014-12-21
 * \brief  歌词界面头文件。
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

#ifndef MUSICLRC_H
#define MUSICLRC_H

/*! \def MUSICLRC_VERSION
 *  版本控制宏，对应v3.0.0版本.
 */
#define MUSICLRC_VERSION   0x030000

#include <QString>
#include <QPoint>
#include <QPainter>
#include <QLinearGradient>
#include <QFont>
#include <QLabel>

class QTimer;
class QPaintEvent;
class QMouseEvent;
class QContextMenuEvent;

/*! 歌词Label
*/
class MusicLrc : public QLabel
{
    Q_OBJECT

public:
	/*! 构造函数
	 *  \param parent 父窗口指针，默认为NULL
     */
    MusicLrc(QWidget *parent = NULL);

	/*! 析构函数.*/
    ~MusicLrc();

public:
	/*! 开启歌词遮罩，需要指定当前歌词开始与结束之间的时间间隔
     *  \param intervalTime 时间间隔
     */
    void startLrcMask(qint64 intervalTime);

	/*! 停止歌词遮罩
     */
    void stopLrcMask();

	/*! 解锁歌词
     */
    void unlockLrc();


protected:
	/*! 虚函数，重新实现paintEvent函数
     *  \param event QPaintEvent类型
     */
	virtual void paintEvent(QPaintEvent *event);

	/*! 虚函数，重新实现mousePressEvent函数
     *  \param event QMouseEvent类型
     */
	virtual void mousePressEvent(QMouseEvent *event);

	/*! 虚函数，重新实现mouseMoveEvent函数
     *  \param event QMouseEvent类型
     */
	virtual void mouseMoveEvent(QMouseEvent *event);

	/*! 虚函数，重新实现contextMenuEvent函数
     *  \param event QContextMenuEvent类型
     */
	virtual void contextMenuEvent(QContextMenuEvent *event);

signals:
	/*! 歌词已经锁定
     */
    void sigLrcLocked();
	
	/*! 歌词已经解锁
     */
    void sigLrcUnlocked();

    /*! 隐藏桌面歌词
     */
    void sigHideLrc();

private slots:
	/*! 移动歌词已经播放mask
     */
    void handleTimeOut();

	/*! 锁定歌词
     */
    void handleLockLrc();

	/*! 歌词字体改变
	 * \param fontName 字体名称
	 */
	void handleFontNameChanged(const QString &fontName);

	/*! 歌词字体大小改变
	 * \param fontSize 字体大小
	 */
	void handleFontSizeChanged(const QString &fontSize);

	/*! 歌词类型改变
	 * \param fontType 歌词类型
	 */
	void handleFontTypeChanged(const QString &fontType);

	/*! 歌词没有播放上边缘颜色改变
	 * \param color 颜色
	 */
	void handleNoPlayShangChanged(const QColor &color);

	/*! 歌词没有播放中间颜色改变
	 * \param color 颜色
	 */
	void handleNoPlayZhongChanged(const QColor &color);

	/*! 歌词没有播放下边缘颜色改变
	 * \param color 颜色
	 */
	void handleNoPlayXiaChanged(const QColor &color);

	/*! 歌词播放后上边缘颜色改变
	 * \param color 颜色
	 */
	void handlePlayedShangChanged(const QColor &color);

	/*! 歌词播放后中间颜色改变
	 * \param color 颜色
	 */
	void handlePlayedZhongChanged(const QColor &color);

	/*! 歌词播放后下边缘颜色改变
	 * \param color 颜色
	 */
	void handlePlayedXiaChanged(const QColor &color);

	/*! 歌词灰度改变
	 * \param shadow 灰度
	 */
	void handleShadowChanged(const int &shadow);

private:
	/*! 创建界面
	 */
	void createUI();

	/*! 信号与槽
	 */
	void createConnect();

private:
    QTimer *m_timer;								/*!< 定时器*/
    qreal m_lrcMaskWidth;							/*!< 掩码长度*/
    qreal m_lrcMaskWidthInterval;					/*!< 遮罩宽度间隔*/
    QPoint m_dragPosition;							/*!< 坐标*/
    bool m_isLrcLock;								/*!< 歌词锁定旗帜*/

	QLinearGradient m_linearGradient;		/*!< 歌词的线性渐变*/
	QLinearGradient m_maskLinearGradient;	/*!< 歌词的遮罩线性渐变*/
	QFont m_font;							/*!< 字体*/
    int m_shadow;							/*!< 灰度值*/
};

#endif // MUSICLRC_H


































