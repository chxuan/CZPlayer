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
 * \file   GlobalConfig.h
 * \author chengxuan   787280310@qq.com
 * \date   2015-01-18
 * \brief  全局配置头文件。
 * \version 3.0.0
 *
 * \verbatim
 * 历史
 *          3.0.0   创建,
 *          2015-01-18    by chengxuan
 *
 * \endverbatim
 *
 */

#ifndef GLOBAL_H
#define GLOBAL_H

/*! \def GLOBAL_VERSION
*  版本控制宏，对应v3.0.0版本.
*/
#define GLOBAL_VERSION   0x030000

#include <QString>

/*! 组织字符串, 用于QSettings*/
extern const char *StringOrganization;
/*! 应用程序字符串, 用于QSettings*/
extern const char *StringApplication;

extern const QString ImagesDir;         	/*!< 图片文件夹*/

#endif // GLOBAL_H
