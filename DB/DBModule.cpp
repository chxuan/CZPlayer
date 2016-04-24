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
 * \file   DBModule.cpp
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

#include "DBModule.h"
#include "TR.h"
#include <QtSql>
#include <QMessageBox>
#include <QFile>
#include <QMessageBox>

/*****************************************************************************
  这里用来检查版本的对应
 *****************************************************************************/
#if TR_VERSION < 0x030000
#error  "TR version error."
#endif

/*****************************************************************************
  全局变量定义
 *****************************************************************************/
const QString DBType = "QSQLITE";			/*!< 数据库类型*/
const QString DBFile = "CZPlayer.sqlite";	/*!< 数据库文件*/

DBModule::DBModule()
{
	/*Do nothing.*/
}

DBModule::~DBModule()
{
	/*Do nothing.*/
}

void DBModule::initDB()
{
	//判断数据库文件是否存在
	bool create = !QFile::exists(DBFile);

	if (!DBModule::createConnection())
	{
		QMessageBox::warning(NULL, TR("提示"), TR("连接数据库错误!"));
		return;
	}
	/*Else do nothing, and continue.*/

	if (create)
	{
		DBModule::createTable();
	}
	/*Else do nothing, and continue.*/
}

bool DBModule::createConnection()
{
	QSqlDatabase db = QSqlDatabase::addDatabase(DBType);
	db.setDatabaseName(DBFile);//若数据库文件不存在，则自动创建一个

	if (!db.open())
	{
		QMessageBox::warning(0, TR("数据库打开错误!"), db.lastError().databaseText());
		return false;
	}
	/*Else do nothing, and continue.*/

	return true;
}

void DBModule::createTable()
{
	QSqlQuery query;
	query.exec("drop table musicList");

	query.exec("create table musicList (fileName varchar(50) primary key not null, "
		"time varchar(10), filePath varchar(50), albumUrlPath varchar(100))");
}

bool DBModule::readFilePath( QStringList &list )
{
	QSqlQuery query;
	query.exec("select filePath from musicList");

	while (query.next())
	{
		QString filePath = query.value(0).toString();
		list << filePath;
	}

	if (!list.isEmpty())
	{
		return true;
	}
	/*Else do nothing, and continue.*/

	return false;
}

bool DBModule::readMusicListSaveFormatEntity( QList<MusicListSaveFormatEntity> &list )
{
	QSqlQuery query;
	query.exec("select fileName, time, filePath from musicList");

	while (query.next())
	{
		MusicListSaveFormatEntity entity(query.value(0).toString(),
										 query.value(1).toString(),
										 query.value(2).toString());
		list.append(entity);
	}

	if (!list.isEmpty())
	{
		return true;
	}
	/*Else do nothing, and continue.*/

	return false;
}

bool DBModule::clearDB()
{
	QSqlQuery query;
	return query.exec("delete from musicList");
}

bool DBModule::delLine( const QString &fileName )
{
	QSqlQuery query;
	query.prepare("delete from musicList where fileName = ?");
	query.addBindValue(fileName);
	return query.exec();
}

bool DBModule::insertLine( const QString &fileName, const QString &time, const QString &filePath )
{
	QSqlQuery query;
	query.prepare("insert into musicList values(?, ?, ?, ?)");
	query.addBindValue(fileName);
	query.addBindValue(time);
	query.addBindValue(filePath);
	query.addBindValue("");
	return query.exec();
}

bool DBModule::updateAlbumUrlPath( const QString &fileName, const QString &albumUrlPath )
{
	QSqlQuery query;
	query.prepare("update musicList set albumUrlPath = ? where fileName = ?");
	query.addBindValue(albumUrlPath);
	query.addBindValue(fileName);
	return query.exec();
}

bool DBModule::readAlbumUrlPathFromDB( const QString &fileName, QString &albumUrlPath )
{
	QSqlQuery query;
	query.prepare("select albumUrlPath from musicList where fileName = ?");
	query.addBindValue(fileName);
	query.exec();

	while (query.next())
	{
		albumUrlPath = query.value(0).toString();
		break;
	}

	return true;
}

bool DBModule::readMusicInfoEntity( QList<MusicInfoEntity> &list )
{
	QSqlQuery query;
	query.exec("select fileName, time, filePath from musicList");

	while (query.next())
	{
		MusicInfoEntity entity(query.value(0).toString(),
								query.value(1).toString(),
								query.value(2).toString());
		list.append(entity);
	}

	if (!list.isEmpty())
	{
		return true;
	}
	/*Else do nothing, and continue.*/

	return false;
}
