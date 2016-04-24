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
 * \file   DBModule.h
 * \author chengxuan   787280310@qq.com
 * \date   2014-12-21
 * \brief  数据库模块头文件。
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

#ifndef DBMODULE_H
#define DBMODULE_H

/*! \def DBMODULE_VERSION
 *  版本控制宏，对应v3.0.0版本.
 */
#define DBMODULE_VERSION   0x030000

#include "MusicListSaveFormatEntity.h"
#include "MusicInfoEntity.h"
#include <QStringList>
#include <QString>
#include <QList>

/*! 数据库模块
 */
class DBModule
{
public:
	/*! 初始化数据库
	 */
	static void initDB();																			

	/*! 连接数据库
	 * \return true代表成功，否则失败
	 */
	static bool createConnection();																	

	/*! 创建表
	 */
	static void createTable();																		

	/*! 从数据库中读出数据显示到音乐里列表中
	 * \param list 文件路径
	 * \return true代表成功，否则失败
	 */
	static bool readFilePath(QStringList &list);													

	/*! 从数据库中读出数据显示到m3u文件中
	 * \param list 音乐列表格式集合
	 * \return true代表成功，否则失败
	 */
	static bool readMusicListSaveFormatEntity(QList<MusicListSaveFormatEntity> &list);				

	/*! 从数据库读取音乐信息
	 * \param list 音乐信息集合
	 * \return true代表成功，否则失败
	 */
	static bool readMusicInfoEntity(QList<MusicInfoEntity> &list);									

	/*! 清空数据库
	 * \return true代表成功，否则失败
	 */
	static bool clearDB();																			

	/*! 删除一条记录
	 * \param fileName 文件名
	 * \return true代表成功，否则失败
	 */
	static bool delLine(const QString &fileName);													

	/*! 插入一行记录
	 * \param fileName 文件名
	 * \param time 播放时间
	 * \param filePath 文件路径
	 * \return true代表成功，否则失败
	 */
	static bool insertLine(const QString &fileName, const QString &time, const QString &filePath);	

	/*! 更新专辑url
	 * \param fileName 文件名
	 * \param albumUrlPath 专辑url
	 * \return true代表成功，否则失败
	 */
	static bool updateAlbumUrlPath(const QString &fileName, const QString &albumUrlPath);			

	/*! 从数据库中读出歌曲名对应的专辑url
	 * \param fileName 文件名
	 * \param albumUrlPath 专辑url
	 * \return true代表成功，否则失败
	 */
	static bool readAlbumUrlPathFromDB(const QString &fileName, QString &albumUrlPath);					

private:
	/*! 构造函数.*/
	DBModule();

	/*! 析构函数.*/
	~DBModule();
};

#endif //DBMODULE_H
