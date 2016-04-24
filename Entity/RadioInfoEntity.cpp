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
 * \file   RadioInfoEntity.cpp
 * \author chengxuan   787280310@qq.com
 * \date   2015-03-06
 * \brief  电台信息实体实现文件。
 * \version 3.0.0
 *
 * \verbatim
 * 历史
 *          3.0.0   创建,
 *          2015-03-06    by chengxuan
 *
 * \endverbatim
 *
 */

#include "RadioInfoEntity.h"

RadioInfoEntity::RadioInfoEntity()
{
    /*Do nothing.*/
}

RadioInfoEntity::RadioInfoEntity(const QString &name, const QString &url)
    : m_name(name),
      m_url(url)
{
    /*Do nothing.*/
}

RadioInfoEntity::~RadioInfoEntity()
{
    /*Do nothing.*/
}

QString RadioInfoEntity::name() const
{
    return m_name;
}

void RadioInfoEntity::setName(const QString &name)
{
    m_name = name;
}

QString RadioInfoEntity::url() const
{
    return m_url;
}

void RadioInfoEntity::setUrl(const QString &url)
{
    m_url = url;
}
