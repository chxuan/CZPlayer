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
 * \file   MusicList.h
 * \author chengxuan   787280310@qq.com
 * \date   2015-01-07
 * \brief  音乐列表头文件。
 * \version 3.0.0
 *
 * \verbatim
 * 历史
 *          3.0.0   创建,
 *          2015-01-07    by chengxuan
 *
 * \endverbatim
 *
 */

#ifndef MUSICLIST_H
#define MUSICLIST_H

/*! \def MUSICLIST_VERSION
 *  版本控制宏，对应v3.0.0版本.
 */
#define MUSICLIST_VERSION   0x030000

#include <QTableWidget>
#include <QString>
#include <QPoint>
#include <QColor>

class QContextMenuEvent;
class QMenu;

/*! 歌曲列表
*/
class MusicList : public QTableWidget
{
    Q_OBJECT

public:
	/*! 构造函数
	 *  \param parent 父窗口指针，默认为NULL
     */
    MusicList(QWidget *parent = NULL);

	/*! 析构函数.*/
    ~MusicList();

public:
	/*! 设置当前歌曲播放的行号
	 *  \param row 行号
     */
	void setCurrentMusicRow(const int &row);	

protected:
	/*! 虚函数，重新实现contextMenuEvent函数
     *  \param event QContextMenuEvent类型
     */
    virtual void contextMenuEvent(QContextMenuEvent *event);

signals:
	/*! 清空播放列表完成
     */
    void sigPlayListCleanFinshed();

	/*! 设置当前歌曲播放的行号
	 *  \param row 行号
	 *  \param title 标题
     */
    void sigRowSelected(const int &row, const QString &title);

	/*! 分享当前歌曲
	 *  \param musicName 音乐名
     */
	void sigSharedCurrentMusic(const QString &musicName);

private slots:
	/*! 清空歌曲
     */
    void handleClearAllMusicList();		

	/*! 删除当前歌曲
     */
    void handleDelCurrentMusic();	

	/*! 获取选中行
	 *  \param row 行号
     */
    void handleTableClicked(const int &index);	

	/*! 鼠标滑动到每一行
	 *  \param row 行号
	 *  \param col 列号
     */
	void handleTableRowEnter(const int &row, const int &col);	

	/*! 定位到当前歌曲
     */
    void handleLocateCurrentMusic();		

	/*! 保存当前列表
     */
	void handleSaveList();	

	/*! 分享当前音乐
     */
	void handleShareCurrentMusic();				

private:
	/*! 更新播放列表序号
     */
    void updateMusicList();				

	/*! 设置行颜色
	 *  \param row 行号
	 *  \param color 颜色
     */
	void setRowColor(const int &row, const QColor &color);	

	/*! 创建界面
     */
	void createUI();				

	/*! 信号与槽
     */
	void createConnect();	

private:
    QWidget *m_parent;				/*!< 父窗口*/
    int m_row;						/*!< 行号*/
	int m_currentMusicRow;			/*!< 当前歌曲行号*/
	int m_perviousColorRow;			/*!< 之前被标记了颜色的行号*/
	QMenu *m_rightHandMenu;			/*!< 音乐列表右键菜单*/
};

#endif // MUSICLIST_H
