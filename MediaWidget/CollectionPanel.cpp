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
 * \file   CollectionPanel.cpp
 * \author chengxuan   787280310@qq.com
 * \date   2015-02-23
 * \brief  收藏面板实现文件。
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

#include "CollectionPanel.h"
#include "NoFocusDelegate.h"
#include "TR.h"
#include <QLabel>
#include <QTableWidget>
#include <QHeaderView>
#include <QVBoxLayout>

/*****************************************************************************
  这里用来检查版本的对应
 *****************************************************************************/
#if TR_VERSION < 0x030000
#error  "TR version error."
#endif

CollectionPanel::CollectionPanel(QWidget *parent)
    : QWidget(parent)
{
    this->createUI();
    this->createConnect();
}

CollectionPanel::~CollectionPanel()
{
    /*Do nothing.*/
}

void CollectionPanel::handleAddCollectMusic(const QString &musicName, const QString &artist)
{
    //歌曲名称
    QTableWidgetItem *musicNameItem = new QTableWidgetItem;
    musicNameItem->setTextAlignment(Qt::AlignCenter);
    musicNameItem->setText(musicName);
    musicNameItem->setToolTip(musicName);

    //艺术家
    QTableWidgetItem *artistItem = new QTableWidgetItem;
    artistItem->setTextAlignment(Qt::AlignCenter);
    artistItem->setText(artist);
    artistItem->setToolTip(artist);

    int currentRows = m_collectTable->rowCount();
    m_collectTable->insertRow(currentRows);
    m_collectTable->setItem(currentRows, 0, musicNameItem);
    m_collectTable->setItem(currentRows, 1, artistItem);
}

void CollectionPanel::createUI()
{
    //搜藏列表
    m_collectTable = new QTableWidget(this);
    this->setTableWidgetStyle();

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(m_collectTable);
    this->setLayout(mainLayout);
}

void CollectionPanel::createConnect()
{

}

void CollectionPanel::setTableWidgetStyle()
{
    m_collectTable->setRowCount(0);
    m_collectTable->setColumnCount(2);

    QStringList headList;
    headList << TR("歌曲名称") << TR("艺术家");
    m_collectTable->setHorizontalHeaderLabels(headList);

    m_collectTable->setSelectionMode(QAbstractItemView::SingleSelection);              //设置只能选中单行
    m_collectTable->setSelectionBehavior(QAbstractItemView::SelectRows);               //设置选择习惯为选择行
    m_collectTable->setEditTriggers(QAbstractItemView::NoEditTriggers);                //设置内容不可编辑
    m_collectTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);    //设置内容自动横向填满可用区域
    m_collectTable->setFrameStyle(QFrame::NoFrame);                                    //去掉边框
    m_collectTable->setShowGrid(false);                                                //设置不显示网格
    m_collectTable->setItemDelegate(new NoFocusDelegate());                            //去掉选中单元格的虚线
    m_collectTable->horizontalHeader()->setHighlightSections(false);                   //点击表时不对表头行光亮（获取焦点）
    m_collectTable->verticalHeader()->setVisible(false);                               //设置垂直头不可见
    m_collectTable->horizontalHeader()->setSectionsClickable(false);                   //不响应鼠标单击
}
