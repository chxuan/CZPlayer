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
 * \file   main.cpp
 * \author chengxuan   787280310@qq.com
 * \date   2014-12-21
 * \brief  程序入口。
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

#include "MainWidget.h"
#include "TR.h"
#include <QApplication>
#include <QFile>
#include <QTextCodec>
#include <QString>
#include <QTranslator>
#include "qtsingleapplication.h"

/*****************************************************************************
  这里用来检查版本的对应
 *****************************************************************************/
#if TR_VERSION < 0x030000
#error  "TR version error."
#endif

/*****************************************************************************
  定义全局变量
 *****************************************************************************/
const QString AppName = "CZPlayer";								/*!< 应用程序名*/
const QString QssFilePath = ":/qss/CZPlayer.qss";           	/*!< qss文件路径*/

/*! \fn   main 程序入口函数
    \param argc 参数个数
    \param argv 指向参数字符串的指针
    \return 正常退出返回0，否则返回非0值
    */
int main(int argc, char *argv[])
{
#if 1
    QtSingleApplication app(argc, argv);
	if (app.isRunning())
	{
		return EXIT_SUCCESS;
	}
	/*Else do nothing, and continue.*/
#else
    QApplication app(argc, argv);
#endif
	app.setApplicationName(AppName);

	//加载设置qss样式表
	QFile file(QssFilePath);
	file.open(QFile::ReadOnly);
    app.setStyleSheet(TR(file.readAll()));
	file.close();

	//显示主界面
    MainWidget mainWidget(argv[1]);
    mainWidget.show();
	int result = app.exec();
	return result;
}
