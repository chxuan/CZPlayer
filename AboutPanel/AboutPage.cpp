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
 * \file   AboutPage.cpp
 * \author chengxuan   787280310@qq.com
 * \date   2014-12-22
 * \brief  关于页面实现文件。
 * \version 3.0.0
 *
 * \verbatim
 * 历史
 *          3.0.0   创建,
 *          2014-12-22    by chengxuan
 *
 * \endverbatim
 *
 */

#include "AboutPage.h"
#include "TR.h"
#include <QVBoxLayout>
#include <QLabel>

/*****************************************************************************
  这里用来检查版本的对应
 *****************************************************************************/
#if TR_VERSION < 0x030000
#error  "TR version error."
#endif

AboutPage::AboutPage(QWidget *parent)
	: QWidget(parent)
{
	m_label = new QLabel(this);
	m_label->setText(TR("基于Qt的多媒体播放器（CZPlayer）\n作品展:"
					"http://www.qtcn.org/bbs/read-htm-tid-55824.html\n"
                    "源代码:https://github.com/chxuan/CZPlayer.git"));

	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addWidget(m_label);
	mainLayout->addStretch();
	this->setLayout(mainLayout);
}


AboutPage::~AboutPage(void)
{
	/*Do nothing.*/
}
