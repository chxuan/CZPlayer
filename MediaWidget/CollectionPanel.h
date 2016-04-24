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
 * \file   CollectionPanel.h
 * \author chengxuan   787280310@qq.com
 * \date   2015-02-23
 * \brief  收藏面板头文件。
 * \version 3.0.0
 *
 * \verbatim
 * 历史
 *          3.0.0   创建,
 *          2015-02-23    by chengxuan
 *
 * \endverbatim
 *
 */

#ifndef COLLECTIONPANEL_H
#define COLLECTIONPANEL_H

/*! \def COLLECTIONPANEL_VERSION
 *  版本控制宏，对应v3.0.0版本.
 */
#define COLLECTIONPANEL_VERSION   0x030000

#include <QWidget>

class QLabel;
class QTableWidget;

/*! 收藏面板
*/
class CollectionPanel : public QWidget
{
    Q_OBJECT

public:
    /*! 构造函数
    *  \param parent 父窗口指针，默认为NULL
    */
    CollectionPanel(QWidget *parent = NULL);

    /*! 析构函数.*/
    ~CollectionPanel();

private slots:
    /*! 构造函数
    *  \param musicName 音乐名
    *  \param artist 艺术家
    */
    void handleAddCollectMusic(const QString &musicName, const QString &artist);

private:
    /*! 创建界面
     */
    void createUI();

    /*! 信号与槽
     */
    void createConnect();

    /*! 设置表格风格
     */
    void setTableWidgetStyle();

private:
    QTableWidget *m_collectTable;      /*!< 搜藏列表*/
};

#endif // COLLECTIONPANEL_H
