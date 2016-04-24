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
 * \file   MusicListWidget.h
 * \author chengxuan   787280310@qq.com
 * \date   2015-01-08
 * \brief  歌曲列表面板头文件。
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

#ifndef MUSICLISTWIDGET_H
#define MUSICLISTWIDGET_H

/*! \def MUSICLISTWIDGET_VERSION
 *  版本控制宏，对应v3.0.0版本.
 */
#define MUSICLISTWIDGET_VERSION   0x030000

#include <QWidget>
#include <QMap>

class MusicList;
class QPaintEvent;
class QTimer;
class QLineEdit;
class QPushButton;
class QMenu;

//歌曲列表面板
class MusicListWidget : public QWidget
{
    Q_OBJECT

public:
	/*! 构造函数
	 *  \param playList 音乐列表
	 *  \param mapMusicRows 存放音乐-行map
	 *  \param parent 父窗口指针，默认为NULL
     */
    MusicListWidget(MusicList *playList,
					QMap<QString, int> *mapMusicRows,
                    QWidget *parent = NULL);

	/*! 析构函数.*/
    ~MusicListWidget();

protected:
	/*! 虚函数，重新实现paintEvent函数
     *  \param event QPaintEvent类型
     */
    virtual void paintEvent(QPaintEvent *event);

	/*! 虚函数，重新实现eventFilter函数
     *  \param target QObject对象
	 *  \param event QEvent类型
	 *  \return 返回true代表中断事件的传递
     */
    virtual bool eventFilter(QObject *target, QEvent *event);

private slots:
	/*! 加载音乐
     */
    void handleOnClickedAddMusic();

	/*! 搜索音乐
     */
    void handleSearchMusic();

	/*! 检查焦点是否聚焦到搜索输入框
     */
    void handleLineEditFocus();

	/*! 返回到音乐列表
     */
    void handleBackMusicList();

private:
	/*! 创建界面
     */
	void createUI();				

	/*! 信号与槽
     */
	void createConnect();	

private:
    QWidget *m_parent;						/*!< 父窗口*/
    MusicList *m_playList;					/*!< 音乐列表*/
    QTimer *m_editFocusTimer;				/*!< 检查焦点定时器*/
	QMap<QString, int> *m_mapMusicRows;		/*!< key:音乐名称，value:行*/

    QLineEdit *m_searchLineEdit;			/*!< 搜索输入框*/
    QPushButton *m_searchButton;			/*!< 搜索按钮*/
    QPushButton *m_addMusicButton;			/*!< 添加音乐按钮*/
    QPushButton *m_localeButton;			/*!< 定位歌曲*/
    QPushButton *m_delMusicButton;			/*!< 删除当前歌曲*/
    QPushButton *m_findButton;				/*!< 搜索歌曲*/
    QPushButton *m_backButton;				/*!< 返回音乐列表按钮*/
	QMenu *m_addMusicMenu;					/*!< 添加音乐菜单*/
};

#endif // MUSICLISTWIDGET_H
