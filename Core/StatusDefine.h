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
 * \file   StatusDefine.h
 * \author chengxuan   787280310@qq.com
 * \date   2014-12-25
 * \brief  状态代码定义头文件。
 * \version 3.0.0
 *
 * \verbatim
 * 历史
 *          3.0.0   创建,
 *          2014-12-25    by chengxuan
 *
 * \endverbatim
 *
 */

#ifndef STATUSDEFINE_H
#define STATUSDEFINE_H

/*! \def STATUSDEFINE_VERSION
 *  版本控制宏，对应v3.0.0版本.
 */
#define STATUSDEFINE_VERSION   0x030000

/*! 专辑下载状态*/
enum AlbumDownloadStatus
{
	AlbumDownloadFailed = 1,		/*!< 专辑下载失败*/
	AlbumDownloadSuccess = 2		/*!< 专辑下载成功*/
};

/*! 歌词下载状态*/
enum LrcDownloadStatus
{
	LrcDownloadFailed = 1,		/*!< 歌词下载失败*/
	LrcDownloadSuccess = 2		/*!< 歌词下载成功*/
};

/*! 歌曲搜索状态*/
enum MusicSearchStatus
{
    MusicSearchFailed = 1,		/*!< 歌曲搜索失败*/
    MusicSearchSuccess = 2		/*!< 歌曲搜索成功*/
};

/*! 歌曲下载状态*/
enum MusicDownloadStatus
{
    MusicDownloadFailed = 1,		/*!< 歌曲下载失败*/
    MusicDownloadSuccess = 2		/*!< 歌曲下载成功*/
};

#endif // STATUSDEFINE_H
