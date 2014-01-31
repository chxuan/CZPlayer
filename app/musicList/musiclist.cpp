#include "stdafx.h"
#include "nofocusdelegate.h"
#include "musiclist.h"
#include "DBModule.h"
#include "MusicListSaveFormatEntity.h"

MusicList::MusicList(QWidget *widget) : QTableWidget(widget), parent(widget)
{
    //设置窗口基本属性
    this ->resize(380, 360);//设置窗体大小
    //this ->setWindowFlags(Qt::FramelessWindowHint);//去掉窗体边框
    this ->setWindowFlags(Qt::Tool | Qt::FramelessWindowHint);//去掉窗体边框,不在任务栏显示图标

    //this ->setAttribute(Qt::WA_TranslucentBackground);//设置背景透明
    this ->setWindowIcon(QIcon(":/images/CZPlayer.png"));//设置logo
    this ->setWindowTitle(tr("音乐播放列表"));

    this ->setRowCount(0);//初始化的行数为0
    this ->setColumnCount(3);//初始化的列数为3

    QStringList headList;
    headList << tr("序号") << tr("歌曲") << tr("时长");
    this ->setHorizontalHeaderLabels(headList);//设置头信息

    this ->horizontalHeader() ->setVisible(false);
    this ->verticalHeader() ->setVisible(false);
    //this ->horizontalHeader() ->setResizeMode(QHeaderView::Stretch);//设置内容自动横向填满可用区域
    this ->horizontalHeader() ->resizeSection(0, 30);
    this ->horizontalHeader() ->resizeSection(1, 270);
    this ->horizontalHeader() ->resizeSection(2, 65);
    this ->horizontalHeader() ->setHighlightSections(false);//点击表时不对表头行光亮（获取焦点）
    this ->horizontalHeader() ->setClickable(false);//不响应鼠标单击
    this ->setSelectionMode(QAbstractItemView::SingleSelection);//设置只能选中单行
    this ->setSelectionBehavior(QAbstractItemView::SelectRows);//设置选择习惯为选择行
    this ->setEditTriggers(QAbstractItemView::NoEditTriggers);//设置内容不可编辑
    this ->setShowGrid(false);//设置不显示网格
    this ->setItemDelegate(new NoFocusDelegate());//去掉选中单元格的虚线

//    QTableWidgetItem *columnHeadItem = this ->horizontalHeaderItem(0);
//    columnHeadItem ->setFont(QFont("幼圆"));
//    columnHeadItem ->setBackground(Qt::gray);
//    columnHeadItem ->setTextColor(Qt::white);

//    QTableWidgetItem *columnHeadItem2 = this ->horizontalHeaderItem(1);
//    columnHeadItem2 ->setFont(QFont("幼圆"));
//    columnHeadItem2 ->setBackground(Qt::gray);
//    columnHeadItem2 ->setTextColor(Qt::white);

//    QTableWidgetItem *columnHeadItem3 = this ->horizontalHeaderItem(2);
//    columnHeadItem3 ->setFont(QFont("幼圆"));
//    columnHeadItem3 ->setBackground(Qt::gray);
//    columnHeadItem3 ->setTextColor(Qt::white);

    //设置header样式
    this ->horizontalHeader() ->setStyleSheet("QHeaderView::section{background: gray; color:white; font-family:'微软雅黑'}");
    this ->setStyleSheet("QTableCornerButton::section{background:gray};");//设置表格直角方格样式
    this ->verticalHeader() ->setStyleSheet("QHeaderView::section{background: gray; color:white; font-family:'微软雅黑';}");

    clearMusicList = new QAction(this);
    clearMusicList ->setIcon(QIcon(":/images/clearMusicList.png"));
    clearMusicList ->setText(tr("清空列表"));

    closeAction = new QAction(this);
    closeAction ->setIcon(QIcon(":/images/quitAction"));
    closeAction ->setText(tr("关闭音乐列表"));

    delCurrentMusicAction = new QAction(this);
    delCurrentMusicAction ->setIcon(QIcon(":/images/delCurrentMusic.png"));
    delCurrentMusicAction ->setText(tr("删除当前歌曲"));

	saveListAction = new QAction(this);
	saveListAction ->setIcon(QIcon(":/images/saveList.png"));
	saveListAction ->setText(tr("保存列表"));

    connect(clearMusicList, SIGNAL(triggered()), this, SLOT(slot_ClearAllMusicList()));
    connect(closeAction, SIGNAL(triggered()), parent, SLOT(slot_OpenMusicList()));
    connect(delCurrentMusicAction, SIGNAL(triggered()), this, SLOT(slot_DelCurrentMusic()));
	connect(saveListAction, SIGNAL(triggered()), this, SLOT(slot_SaveList()));
    connect(this, SIGNAL(cellClicked(int,int)), this, SLOT(slot_TableClicked(int)));
}

MusicList::~MusicList()
{
}

//清空音乐播放列表
void MusicList::slot_ClearAllMusicList()
{
	if (this ->rowCount())
	{
		if (QMessageBox::Yes == QMessageBox::information(NULL, tr("信息"),
			tr("是否要清空播放列表？"), QMessageBox::Yes | QMessageBox::No))
		{
			while (this ->rowCount())
			{
				removeRow(0);
			}
			emit sig_PlayListClean();//清空完后，发送清空成功信号
		}
	}
	else
	{
		QMessageBox::information(NULL, tr("信息"), tr("当前播放列表为空！"), QMessageBox::Yes);
	}
}

//删除当前歌曲
void MusicList::slot_DelCurrentMusic()
{
    if (this ->rowCount())
    {
        if (QMessageBox::Yes == QMessageBox::information(NULL, tr("信息"),
                tr("是否删除该歌曲？"), QMessageBox::Yes | QMessageBox::No))
        {
            if (row < this ->rowCount())
            {
                QString musicName =  this ->item(row, 1) ->text();
                this ->removeRow(row);
                emit sig_RowSelected(row, musicName);//数据库中删除
                //更新播放列表序号
                this ->updateMusicList();
            }
        }
    }
	else
	{
		QMessageBox::information(NULL, tr("信息"), tr("当前播放列表为空！"), QMessageBox::Yes);
	}
}

//更新播放列表序号
void MusicList::updateMusicList()
{
    for (int index = row; index < this ->rowCount(); ++index)
    {
        QTableWidgetItem *rowItem = this ->item(index, 0);
        rowItem ->setText(QString::number(index + 1));
    }
}

//获得选中的行
void MusicList::slot_TableClicked(int index)
{
    row = index;
}

//响应鼠标右键点击事件
void MusicList::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu;
    menu.addAction(QIcon(":/images/locateButton.png"), tr("定位到当前播放歌曲"), this, SLOT(slot_LocateCurrentMusic()));
    menu.addSeparator();
    menu.addAction(delCurrentMusicAction);
    menu.addAction(clearMusicList);
	menu.addAction(saveListAction);
    menu.addSeparator();
    menu.addAction(closeAction);
    menu.exec(event ->globalPos());//返回鼠标指针的全局位置
    event ->accept();
}

//定位到当前歌曲
void MusicList::slot_LocateCurrentMusic()
{
	if (m_nCurrentMusicRow == 0)
	{
		this ->selectRow(m_nCurrentMusicRow + 1);
	}
	else
	{
		this ->selectRow(m_nCurrentMusicRow - 1);
	}
	this ->selectRow(m_nCurrentMusicRow);
}

//保存当前列表
void MusicList::slot_SaveList()
{
	QString fileName = QFileDialog::getSaveFileName(this, tr("保存播放列表"), tr("播放列表"), tr("*.m3u *.pls"));
	if (!fileName.isEmpty())
	{
		ofstream file(fileName.toStdString());

		QString strSuffixName = fileName.right(3);//得到后缀名
		if (strSuffixName == "m3u" || strSuffixName == "M3U")
		{
			file << "#EXTM3U" << endl;			//表示一个M3U文件
		}
		else if (strSuffixName == "pls" || strSuffixName == "PLS")
		{
			file << "[playlist]" << endl;		//表示一个pls文件
		}

		vector<MusicListSaveFormatEntity> vec;
		if (DBModule::readFromDb2(vec))
		{
			int i = 0;
			foreach (MusicListSaveFormatEntity entity, vec)
			{
				++i;
				QTime qTime;
				int nTime = qTime.secsTo(QTime::fromString(QString::fromStdString(entity.getTime()), "mm:ss"));

				if (strSuffixName == "m3u" || strSuffixName == "M3U")
				{
					QString sFileName = QString::fromStdString(entity.getFileName());
					QString strMusicName = "";
					if (sFileName.contains(" - "))
					{
						strMusicName = sFileName.split(" - ").at(1);
					}
					file << "#EXTINF:" << nTime << "," << strMusicName.toStdString() << "\n" << entity.getFilePath() << endl;
				}
				else if (strSuffixName == "pls" || strSuffixName == "PLS")
				{
					QString sFileName = QString::fromStdString(entity.getFileName());
					QString strMusicName = "";
					if (sFileName.contains(" - "))
					{
						strMusicName = sFileName.split(" - ").at(1);
					}
					file << "File" << i << "=" << entity.getFilePath() << "\n"
						 << "Title" << i << "=" << strMusicName.toStdString() << "\n"
						 << "Length" << i << "=" << nTime << endl;
				}
			}
			if (strSuffixName == "pls" || strSuffixName == "PLS")
			{
				file << "NumberOfEntries=" << i <<  endl;
			}
		}
	}
}

//设置当前歌曲播放的行号
void MusicList::setCurrentMusicRow( int row )
{
	m_nCurrentMusicRow = row;
}
