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
* \file   TR.h
* \author chengxuan   787280310@qq.com
* \date   2015-01-22
* \brief  转码文件。
* \version 3.0.0
*
* \verbatim
* 历史
*          3.0.0   创建,
*          2015-01-22    by chengxuan
*
* \endverbatim
*
*/

#ifndef TR_H
#define TR_H

/*! \def TR_VERSION
*  版本控制宏，对应v3.0.0版本.
*/
#define TR_VERSION   0x030000

#include <QTextCodec>
#include <QString>
#include <QByteArray>

/*! 将字符串转换成GBK编码
*  \param chars 待转码的字符串
*  \return 返回转换过后的字符串
*/
inline static QString TR(const char *chars)
{
	static QTextCodec *codec = QTextCodec::codecForName("GBK");
	return codec->toUnicode(chars);
}

/*! 重载函数，将字符串转换成GBK编码
*  \param chars 待转码的字符串
*  \return 返回转换过后的字符串
*/
inline static QString TR(const QByteArray &chars)
{
	static QTextCodec *codec = QTextCodec::codecForName("GBK");
	return codec->toUnicode(chars);
}

#endif //TR_H
