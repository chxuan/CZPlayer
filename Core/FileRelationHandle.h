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
* \file   FileRelationHandle.h
* \author chengxuan   787280310@qq.com
* \date   2015-05-02
* \brief  文件关联处理器头文件。
* \version 3.0.0
*
* \verbatim
* 历史
*          3.0.0   创建,
*          2015-05-02    by chengxuan
*
* \endverbatim
*
*/

#ifndef FILERELATIONHANDLE_H
#define FILERELATIONHANDLE_H

/*! \def FILERELATIONHANDLE_VERSION
*  版本控制宏，对应v3.0.0版本.
*/
#define FILERELATIONHANDLE_VERSION   0x030000

#include <QObject>

/*! 文件关联处理器
 */
class FileRelationHandle : public QObject
{
    Q_OBJECT

public:
    /*! 构造函数
     *  \param parent 父窗口指针，默认为NULL
     */
    FileRelationHandle(QObject *parent = NULL);

    /*! 析构函数.*/
    ~FileRelationHandle();

private slots:
    /*! 关联文件
     *  \param isRelationFile true:关联文件，false:取消关联
     */
    void handleFileRelation(bool isRelationFile);

private:
    /*! 信号与槽
     */
    void createConnect();
};

#endif // FILERELATIONHANDLE_H
