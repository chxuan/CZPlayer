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
 * \file   RadioInfoEntity.h
 * \author chengxuan   787280310@qq.com
 * \date   2015-03-06
 * \brief  电台信息实体头文件。
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

#ifndef RADIOINFOENTITY_H
#define RADIOINFOENTITY_H

/*! \def RADIOINFOENTITY_VERSION
 *  版本控制宏，对应v3.0.0版本.
 */
#define RADIOINFOENTITY_VERSION   0x030000

#include <QString>

/*! 电台信息实体
*/
class RadioInfoEntity
{
public:
    /*! 构造函数.*/
    RadioInfoEntity();

    /*! 构造函数
     * \param name 网络电台名
     * \param url 网络电台url
     */
    RadioInfoEntity(const QString &name, const QString &url);

    /*! 析构函数.*/
    ~RadioInfoEntity();

    /*! 得到网络电台名
     * \return 电台名
     */
    QString name() const;

    /*! 设置网络电台名
     * \param name 电台名
     */
    void setName(const QString &name);

    /*! 得到网络电台url
     * \return 电台url
     */
    QString url() const;

    /*! 设置网络电台url
     * \param name 电台url
     */
    void setUrl(const QString &url);

private:
    QString m_name;     /*!< 网络电台名*/
    QString m_url;      /*!< 网络电台url*/
};

#endif // RADIOINFOENTITY_H
