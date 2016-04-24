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
 * \file   MusicList.cpp
 * \author chengxuan   787280310@qq.com
 * \date   2015-01-07
 * \brief  音乐列表实现文件。
 * \version 3.0.0
 *
 * \verbatim
 * 历史
 *          3.0.0   创建,
 *          2015-01-07    by chengxuan
 *
 * \endverbatim
 *
 */

#include "MusicList.h"
#include "NoFocusDelegate.h"
#include "DBModule.h"
#include "MusicListSaveFormatEntity.h"
#include "Global.h"
#include "TR.h"
#include <QHeaderView>
#include <QContextMenuEvent>
#include <QMenu>
#include <QMessageBox>
#include <QFileDialog>
#include <QTime>
#include <QTextStream>
#ifdef CZ_DEBUG
#include <QDebug>
#endif

/*****************************************************************************
  这里用来检查版本的对应
 *****************************************************************************/
#if DBMODULE_VERSION < 0x030000
#error  "DBModule version error."
#endif

#if MUSICLISTSAVEFORMATENTITY_VERSION < 0x030000
#error  "MusicListSaveFormatEntity version error."
#endif

#if GLOBAL_VERSION < 0x030000
#error  "Global version error."
#endif

#if TR_VERSION < 0x030000
#error  "TR version error."
#endif

MusicList::MusicList(QWidget *parent) 
	: QTableWidget(parent), 
	  m_parent(parent),  
	  m_perviousColorRow(0),
	  m_currentMusicRow(-1),
	  m_row(-1)
{
	this->createUI();
	this->createConnect();
}

MusicList::~MusicList()
{
	/*Do nothing.*/
}

void MusicList::handleClearAllMusicList()
{
	if (this->rowCount())
	{
		if (QMessageBox::Yes == QMessageBox::information(this, TR("信息"),
			TR("是否要清空播放列表？"), QMessageBox::Yes | QMessageBox::No))
		{
			while (this->rowCount())
			{
				removeRow(0);
			}
			emit sigPlayListCleanFinshed();
		}
		/*Else do nothing, and continue.*/
	}
	else
	{
		QMessageBox::information(this, TR("信息"), TR("当前播放列表为空！"), QMessageBox::Yes);
	}
}

void MusicList::handleDelCurrentMusic()
{
	if (m_row < 0)
	{
		return;
	}
	/*Else do nothing, and continue.*/

	if (this->rowCount())
    {
        QString musicName = this->item(m_row, 1)->text();
		if (QMessageBox::Yes == QMessageBox::information(this, TR("信息"),
			TR("是否删除《%1》？").arg(musicName), QMessageBox::Yes | QMessageBox::No))
        {
            if (m_row < this->rowCount())
            {
                this->removeRow(m_row);
				emit sigRowSelected(m_row, musicName);//数据库中删除
				//更新播放列表序号
                this->updateMusicList();
            }
			/*Else do nothing, and continue.*/
        }
		/*Else do nothing, and continue.*/
    }
	else
	{
		QMessageBox::information(this, TR("信息"), TR("当前播放列表为空！"), QMessageBox::Yes);
	}
}

void MusicList::updateMusicList()
{
    for (int index = m_row; index < this->rowCount(); ++index)
    {
        QTableWidgetItem *rowItem = this->item(index, 0);
        rowItem->setText(QString::number(index + 1));
    }
}

void MusicList::handleTableClicked(const int &index)
{
    m_row = index;
}

void MusicList::handleTableRowEnter( const int &row, const int &col )
{
	(void)col;
	QTableWidgetItem *item = NULL;

	//还原上一行的颜色
	item = this->item(m_perviousColorRow, 0);
	
	if (item != NULL)
	{
		for (int i = 0; i < this->columnCount(); ++i)
		{
			QTableWidgetItem *item = this->item(m_perviousColorRow, i);
			item->setFont(QFont(TR("微软雅黑"), 10, QFont::Normal));
			item->setBackgroundColor(QColor(0, 0, 0, 0));
		}
	}
	/*Else do nothing, and continue.*/

	//设置当前行颜色
	this->setRowColor(row, QColor(5, 184, 204));

	m_perviousColorRow = row;
}

void MusicList::setRowColor( const int &row, const QColor &color )
{
	for (int i = 0; i < this->columnCount(); ++i)
	{
		QTableWidgetItem *item = this->item(row, i);
		item->setFont(QFont(TR("微软雅黑"), 10, QFont::Bold));
		item->setBackgroundColor(color);
	}
}

void MusicList::contextMenuEvent(QContextMenuEvent *event)
{
    m_rightHandMenu->exec(event->globalPos());
    event->accept();
}

void MusicList::handleLocateCurrentMusic()
{
	if (m_currentMusicRow < 0)
	{
		return;
	}
	/*Else do nothing, and continue.*/

	if (m_currentMusicRow == 0)
	{
		this->selectRow(m_currentMusicRow + 1);
	}
	else
	{
		this->selectRow(m_currentMusicRow - 1);
	}
	this->selectRow(m_currentMusicRow);
}

void MusicList::handleSaveList()
{
	QString fileName = QFileDialog::getSaveFileName(this, TR("保存播放列表"),
		TR("播放列表.m3u"), "*.m3u;;*.pls");
	if (!fileName.isEmpty())
	{
		QFile file(fileName);
		if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
		{
			file.close();
			return;
		}
		/*Else do nothing, and continue.*/
		QTextStream out(&file);

		QString suffixName = fileName.right(3);//得到后缀名
		if (suffixName == "m3u" || suffixName == "M3U")
		{
			out << "#EXTM3U\n";			//表示一个M3U文件
		}
		else if (suffixName == "pls" || suffixName == "PLS")
		{
			out << "[playlist]\n";		//表示一个pls文件
		}
		/*Else do nothing, and continue.*/

		QList<MusicListSaveFormatEntity> list;
		if (DBModule::readMusicListSaveFormatEntity(list))
		{
			int i = 0;
			foreach (MusicListSaveFormatEntity entity, list)
			{
				++i;
				QTime time = QTime::fromString(entity.time(), "mm:ss");
				int second = time.minute() * 60 + time.second();

				if (suffixName == "m3u" || suffixName == "M3U")
				{
					QString fileName = entity.fileName();
					QString musicName = "";
					if (fileName.contains('-'))
					{
						musicName = fileName.split('-').at(1).trimmed();
					}
					/*Else do nothing, and continue.*/
					out << "#EXTINF:" << second << "," << musicName
						<< '\n' << entity.filePath() << '\n';
				}
				else if (suffixName == "pls" || suffixName == "PLS")
				{
					QString fileName = entity.fileName();
					QString musicName = "";
					if (fileName.contains('-'))
					{
						musicName = fileName.split('-').at(1).trimmed();
					}
					/*Else do nothing, and continue.*/
					out << "File" << i << "=" << entity.filePath() << '\n'
						<< "Title" << i << "=" << musicName << '\n'
						<< "Length" << i << "=" << second << '\n';
				}
				/*Else do nothing, and continue.*/
			}
			if (suffixName == "pls" || suffixName == "PLS")
			{
				out << "NumberOfEntries=" << i << '\n';
			}
			/*Else do nothing, and continue.*/
		}
		/*Else do nothing, and continue.*/
		file.close();
	}
	/*Else do nothing, and continue.*/
}

void MusicList::setCurrentMusicRow( const int &row )
{
	m_currentMusicRow = row;
}

void MusicList::handleShareCurrentMusic()
{
	if (this->rowCount())
	{
		if (m_row < this->rowCount())
		{
			emit sigSharedCurrentMusic(this->item(m_row, 1)->text());
		}
		/*Else do nothing, and continue.*/
	}
	else
	{
		QMessageBox::information(this, TR("信息"), TR("当前播放列表为空！"), QMessageBox::Yes);
	}
}

void MusicList::createUI()
{
	//设置窗口基本属性
	this->resize(380, 360);

	this->setWindowIcon(QIcon(ImagesDir + "CZPlayer.png"));
	this->setWindowTitle(TR("音乐播放列表"));

	this->setRowCount(0);		//初始化的行数为0
	this->setColumnCount(3);		//初始化的列数为3

	QStringList headList;
	headList << TR("序号") << TR("歌曲") << TR("时长");
	this->setHorizontalHeaderLabels(headList);//设置头信息

	this->horizontalHeader()->setVisible(false);
	this->verticalHeader()->setVisible(false);
	//this->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);//设置内容自动横向填满可用区域
	this->horizontalHeader()->resizeSection(0, 30);
	this->horizontalHeader()->resizeSection(1, 270);
	this->horizontalHeader()->resizeSection(2, 65);
	this->horizontalHeader()->setHighlightSections(false);			//点击表时不对表头行光亮（获取焦点）
	this->horizontalHeader()->setSectionsClickable(false);			//不响应鼠标单击
	this->setSelectionMode(QAbstractItemView::SingleSelection);		//设置只能选中单行
	this->setSelectionBehavior(QAbstractItemView::SelectRows);		//设置选择习惯为选择行
	this->setEditTriggers(QAbstractItemView::NoEditTriggers);		//设置内容不可编辑
	this->setShowGrid(false);										//设置不显示网格
	this->setItemDelegate(new NoFocusDelegate());					//去掉选中单元格的虚线
	this->setMouseTracking(true);									//开启捕获鼠标功能

	//右键菜单
	m_rightHandMenu = new QMenu(this);
	m_rightHandMenu->addAction(QIcon(ImagesDir + "shareMusic.png"), TR("分享"), this, SLOT(handleShareCurrentMusic()));
	m_rightHandMenu->addAction(QIcon(ImagesDir + "locateButton.png"), TR("定位到当前播放歌曲"), this, SLOT(handleLocateCurrentMusic()));
	m_rightHandMenu->addSeparator();
	m_rightHandMenu->addAction(QIcon(ImagesDir + "delCurrentMusic.png"), TR("删除当前歌曲"), this, SLOT(handleDelCurrentMusic()));
	m_rightHandMenu->addAction(QIcon(ImagesDir + "clearMusicList.png"), TR("清空列表"), this, SLOT(handleClearAllMusicList()));
	m_rightHandMenu->addAction(QIcon(ImagesDir + "saveList.png"), TR("保存列表"), this, SLOT(handleSaveList()));
	m_rightHandMenu->addSeparator();
	m_rightHandMenu->addAction(QIcon(ImagesDir + "quitAction.png"), TR("关闭音乐列表"), m_parent, SLOT(handleOpenMusicList()));
}

void MusicList::createConnect()
{
	connect(this, SIGNAL(cellPressed(int, int)), this, SLOT(handleTableClicked(int)));
	connect(this, SIGNAL(cellEntered(int, int)), this, SLOT(handleTableRowEnter(int, int)));
}
