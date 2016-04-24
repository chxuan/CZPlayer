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
 * \file   ThanksPage.cpp
 * \author chengxuan   787280310@qq.com
 * \date   2014-12-22
 * \brief  感谢页面实现文件。
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

#include "ThanksPage.h"
#include "TR.h"
#include <QVBoxLayout>
#include <QLabel>

/*****************************************************************************
  这里用来检查版本的对应
 *****************************************************************************/
#if TR_VERSION < 0x030000
#error  "TR version error."
#endif

ThanksPage::ThanksPage(QWidget *parent)
	: QWidget(parent)
{
	m_thanksLabel = new QLabel(this);
	m_thanksLabel->setText(TR("感谢：\n		程煊 <787280310@qq.com>"
								"\n		程煊 <787280310@qq.com>"));

	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addWidget(m_thanksLabel);
	mainLayout->addStretch();
	this->setLayout(mainLayout);
}


ThanksPage::~ThanksPage(void)
{
	/*Do nothing.*/
}
