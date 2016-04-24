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
 * \file   MusicListSaveFormatEntity.h
 * \author chengxuan   787280310@qq.com
 * \date   2014-12-21
 * \brief  音乐列表保存格式实体头文件。
 * \version 3.0.0
 *
 * \verbatim
 * 历史
 *          3.0.0   创建,
 *          2014-12-21    by chengxuan
 *
 * \endverbatim
 *
 */

#ifndef MUSICLISTSAVEFORMATENTITY_H
#define MUSICLISTSAVEFORMATENTITY_H

/*! \def MUSICLISTSAVEFORMATENTITY_VERSION
 *  版本控制宏，对应v3.0.0版本.
 */
#define MUSICLISTSAVEFORMATENTITY_VERSION   0x030000

#include <QString>

/*! 音乐列表保存格式实体
*/
class MusicListSaveFormatEntity
{
public:
	/*! 构造函数.*/
	MusicListSaveFormatEntity();

	/*! 构造函数
	 * \param fileName 音乐文件名
	 * \param time 播放时间
	 * \param filePath 文件路径
	 */
	MusicListSaveFormatEntity(const QString &fileName, const QString &time, const QString &filePath);

	/*! 析构函数.*/
	~MusicListSaveFormatEntity();

	/*! 得到文件名
	 * \return 文件名
	 */
	QString fileName() const;

	/*! 设置文件名
	 * \param fileName 音乐文件名
	 */
	void setFileName(const QString &fileName);

	/*! 得到音乐播放的时间
	 * \return 时间
	 */
	QString time() const;

	/*! 设置时间
	 * \param time 时间
	 */
	void setTime(const QString &time);

	/*! 得到文件路径
	 * \return 文件路径
	 */
	QString filePath() const;

	/*! 设置文件路径
	 * \param filePath 文件路径
	 */
	void setFilePath(const QString &filePath);

private:
	QString m_fileName;		/*!< 音乐文件名*/
	QString m_time;			/*!< 播放时间*/
	QString m_filePath;		/*!< 文件路径*/
};

#endif //MUSICLISTSAVEFORMATENTITY_H
