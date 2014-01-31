#include "stdafx.h"
#include "DBModule.h"

DBModule::DBModule()
{

}

DBModule::~DBModule()
{

}

//初始化数据库
void DBModule::initDB()
{
	//判断数据库文件是否存在
	bool create = !QFile::exists("CZPlayer.sqlite");

	if (!DBModule::createConnection())
	{
		QMessageBox::warning(0, tr("提示"), tr("连接数据库错误!"));
		return;
	}
	if (create)
	{
		DBModule::createTable();
	}
}

//连接数据库
bool DBModule::createConnection()
{
	QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
	db.setDatabaseName("CZPlayer.sqlite");//若数据库文件不存在，则自动创建一个

	if (!db.open())
	{
		QMessageBox::warning(0, tr("数据库打开错误!"),
			db.lastError().databaseText());
		return false;
	}
	return true;
}

//如果没有创建表，则创建
void DBModule::createTable()
{
	QSqlQuery query;
	query.exec("drop table musicList");

	query.exec("create table musicList (fileName varchar(50) primary key not null, "
		"time varchar(10), filePath varchar(50))");
}

//从数据库中读出音乐文件信息放入音乐播放列表
bool DBModule::readFromDb( QStringList &list )
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

	return false;
}

//从数据库中读出数据显示到m3u文件中
bool DBModule::readFromDb2( vector<MusicListSaveFormatEntity> &vec )
{
	QSqlQuery query;
	query.exec("select fileName, time, filePath from musicList");

	while (query.next())
	{
		MusicListSaveFormatEntity entity(query.value(0).toString().toStdString(),
									query.value(1).toString().toStdString(),
									query.value(2).toString().toStdString());
		vec.push_back(entity);
	}

	if (!vec.empty())
	{
		return true;
	}

	return false;
}

//清空数据库
bool DBModule::clearDB()
{
	QSqlQuery query;
	return query.exec("delete from musicList");
}

//删除一条记录
bool DBModule::delLine( const QString &fileName )
{
	QSqlQuery query;
	query.prepare("delete from musicList where fileName = ?");
	query.addBindValue(fileName);
	return query.exec();
}

//插入一行记录
bool DBModule::insertLine( const QString &fileName, const QString &time, const QString &filePath )
{
	QSqlQuery query;
	query.prepare("insert into musicList values(?, ?, ?)");
	query.addBindValue(fileName);
	query.addBindValue(time);
	query.addBindValue(filePath);
	return query.exec();
}
