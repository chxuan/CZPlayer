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
 * \file   MinMusicWidget.h
 * \author chengxuan   787280310@qq.com
 * \date   2014-12-21
 * \brief  迷你界面头文件。
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

#ifndef MINMUSICWIDGET_H
#define MINMUSICWIDGET_H

/*! \def MINMUSICWIDGET_VERSION
 *  版本控制宏，对应v3.0.0版本.
 */
#define MINMUSICWIDGET_VERSION   0x030000

#include <QWidget>
#include <QPoint>

class QPaintEvent;
class QMouseEvent;
class QContextMenuEvent;
class QPushButton;
class QLabel;
class QMenu;

/*! 播放器迷你界面
*/
class MinMusicWidget : public QWidget
{
    Q_OBJECT

public:
	/*! 构造函数
	 *  \param mainWidget 主界面
	 *  \param parent 父窗口指针，默认为NULL
     */
    MinMusicWidget(QWidget *mainWidget, QWidget *parent = NULL);

	/*! 析构函数.*/
    ~MinMusicWidget();

	/*! 设置迷你界面按钮是否可用
	 *  \param isWork true代表可用，否则不可用
     */
	void setWork(bool isWork);

	/*! 设置歌词Action文本
	 *  \param text 文本内容
     */
	void setMinMusicLrcActionText(const QString &text);

	/*! 迷你界面更改专辑图片
	 * \param albumFilePath 专辑路径
     */
	void changeAlbumImage(const QString &albumFilePath);

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
	/*! 打开主界面
     */
    void sigShowMusicPanel();		

private slots:
	/*! 打开主界面
     */
    void handleOpenMainWidget();	

private:
	/*! 创建界面
     */
	void createUI();				

	/*! 信号与槽
     */
	void createConnect();	

private:
    QWidget *m_mainWidget;						/*!< 主界面*/
    QPoint m_dragPosition;						/*!< 坐标点*/

	QLabel *m_albumImage;						/*!< 专辑图片*/

	QPushButton *m_minPlayButton;				/*!< 播放*/
	QPushButton *m_minPauseButton;				/*!< 暂停*/
	QPushButton *m_minStopButton;				/*!< 停止*/
	QPushButton *m_minNextButton;				/*!< 下一曲*/
	QPushButton *m_minLastButton;				/*!< 上一曲*/
	QPushButton *m_minOpenFileButton;			/*!< 打开文件*/
	QAction *m_minMusicLrcAction;				/*!< 显示歌词*/
	QMenu *m_rightHandMenu;						/*!< 右键菜单*/
};

#endif // MINMUSICWIDGET_H
