#include "stdafx.h"
#include "downloadlist.h"

DownloadList::DownloadList(QWidget *widget) : parent(widget)
{
    delRow = 0;
    this->setRowCount(0);//初始化的行数为0
    this->setColumnCount(5);//初始化的列数为5

    QStringList headList;
    headList << tr("歌曲名称") << tr("状态") << tr("大小") << tr("速度") << tr("进度");
    this->setHorizontalHeaderLabels(headList);//设置头信息

    this->setSelectionMode(QAbstractItemView::SingleSelection);//设置只能选中单行
    this->setSelectionBehavior(QAbstractItemView::SelectRows);//设置选择习惯为选择行
    this->setEditTriggers(QAbstractItemView::NoEditTriggers);//设置内容不可编辑
    this->horizontalHeader()->setResizeMode(QHeaderView::Stretch);//设置内容自动横向填满可用区域
    this->setFrameStyle(QFrame::NoFrame);//去掉边框
    this->setShowGrid(false);//设置不显示网格
    this->setItemDelegate(new NoFocusDelegate());//去掉选中单元格的虚线
    this->horizontalHeader()->setHighlightSections(false);//点击表时不对表头行光亮（获取焦点）
    this->verticalHeader()->setVisible(false); //设置垂直头不可见
    this->horizontalHeader()->setClickable(false);//不响应鼠标单击

    //设置header样式
    this->horizontalHeader()->setStyleSheet("QHeaderView{background-color: gray;/*background:transparent;*/color:white; font:14px; font-family:'微软雅黑';"
        "border-left:transparent;border:transparent;border-top:transparent;border-bottom:transparent;}");
    this->setStyleSheet("QTableCornerButton::section{background:gray};");//设置表格直角方格样式
    this->verticalHeader()->setStyleSheet("QHeaderView::section{background-color: gray; color:white; font:14px; font-family:'微软雅黑';}");

    connect(this, SIGNAL(cellClicked(int,int)), this, SLOT(slot_TableClicked(int)));
}

//析构函数
DownloadList::~DownloadList()
{
}

//获得选中的行
void DownloadList::slot_TableClicked(int index)
{
    row = index;
}

//删除当前歌曲
void DownloadList::slot_DelCurrentMusic()
{
//     if (row < this->rowCount())
//     {
//         QString musicStatus =  this->item(row, 1)->text();
//         QString musicTitle = this->item(row, 0)->text();
//
//         for (map<int, int>::iterator iter = m_listMap.begin(); iter != m_listMap.end(); ++iter)
//         {
//             if (iter->second == row)
//             {
//                 delRow = iter->first;
//                 break;
//             }
//         }
//
//         if (musicStatus == "正在下载")
//         {
//             this->removeRow(row);
//             qDebug() << "delRow = " << delRow;
//             emit sig_DelCurrentRow(delRow);
//         }
//         else if (musicStatus == "完成")
//         {
//             this->removeRow(row);
//         }
//         else if (musicStatus == "中断")
//         {
//             this->removeRow(row);
//             emit sig_DelCurrentRow(delRow);
//         }
//         emit sig_DelCurrentMusicMapItem(musicTitle);
//     }
}

//暂停下载
void DownloadList::slot_PauseMusicDownload()
{
    if (row < this->rowCount())
    {
        QString musicStatus =  this->item(row, 1)->text();
        if (musicStatus == "正在下载")
        {
            emit sig_PauseDownload(row);
        }
    }
}

//开始下载
void DownloadList::slot_StartMusicDownload()
{

}

//得到listmap
void DownloadList::setMusicListMap(map<int, int> listMap)
{
    m_listMap = listMap;
}

//响应鼠标右键点击事件
void DownloadList::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu;
//     menu.addAction(QIcon(":/images/slot_DelCurrentMusic.png"), tr("删除任务"), this, SLOT(slot_DelCurrentMusic()));
//     menu.addSeparator();
    menu.addAction(QIcon(":/images/sig_PauseDownload.png"), tr("暂停下载"), this, SLOT(slot_PauseMusicDownload()));
    menu.addAction(QIcon(":/images/startDownload.png"), tr("开始下载"), this, SLOT(slot_StartMusicDownload()));
    menu.exec(event->globalPos());//globalPos()为当前鼠标的位置坐标
    event->accept();
}











