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
 * \file   LrcPortraitPanel.cpp
 * \author chengxuan   787280310@qq.com
 * \date   2015-02-23
 * \brief  歌词写真面板实现文件。
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

#include "LrcPortraitPanel.h"
#include "TR.h"
#include <QLabel>
#include <QVBoxLayout>

/*****************************************************************************
  这里用来检查版本的对应
 *****************************************************************************/
#if TR_VERSION < 0x030000
#error  "TR version error."
#endif

LrcPortraitPanel::LrcPortraitPanel(QWidget *parent)
    : QWidget(parent)
{
    this->createUI();
    this->createConnect();
}

LrcPortraitPanel::~LrcPortraitPanel()
{
    /*Do nothing.*/
}

void LrcPortraitPanel::createUI()
{
    m_tipLabel = new QLabel(TR("歌词写真正在开发中..."), this);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(m_tipLabel);
    mainLayout->addStretch();
    this->setLayout(mainLayout);
}

void LrcPortraitPanel::createConnect()
{

}
