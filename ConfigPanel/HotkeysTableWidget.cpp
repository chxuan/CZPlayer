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
* \file   HotKeysTableWidget.cpp
* \author chengxuan   787280310@qq.com
* \date   2015-01-24
* \brief  热键页面实现文件。
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

#include "HotkeysTableWidget.h"
#include "NoFocusDelegate.h"
#include "TR.h"
#include <QHeaderView>

/*****************************************************************************
  这里用来检查版本的对应
 *****************************************************************************/
#if TR_VERSION < 0x030000
#error  "TR version error."
#endif

HotKeysTableWidget::HotKeysTableWidget(QWidget *parent) 
	: QTableWidget(parent)
{
    //设置窗口基本属性
    this->setRowCount(0);//初始化的行数为0
    this->setColumnCount(2);//初始化的列数为2

    QStringList headList;
	headList << TR("动作") << TR("全局热键");
    this->setHorizontalHeaderLabels(headList);								//设置头信息

    this->verticalHeader()->setVisible(false);
	this->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);	//设置内容自动横向填满可用区域
    this->horizontalHeader()->setHighlightSections(false);					//点击表时不对表头行光亮（获取焦点）
	this->horizontalHeader()->setSectionsClickable(false);					//不响应鼠标单击
    this->setSelectionMode(QAbstractItemView::SingleSelection);				//设置只能选中单行
    this->setSelectionBehavior(QAbstractItemView::SelectRows);				//设置选择习惯为选择行
    this->setEditTriggers(QAbstractItemView::NoEditTriggers);				//设置内容不可编辑
    this->setShowGrid(false);												//设置不显示网格
    this->setItemDelegate(new NoFocusDelegate());							//去掉选中单元格的虚线

    this->setStyleSheet("color: black;");
}

HotKeysTableWidget::~HotKeysTableWidget()
{
	/*Do nothing.*/
}
