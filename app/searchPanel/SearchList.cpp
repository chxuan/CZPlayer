#include "StdAfx.h"
#include "SearchList.h"
#include "nofocusdelegate.h"

SearchList::SearchList( QWidget *widget /*= 0*/ ) : QTableWidget(widget), m_parent(widget)
{
	this->setRowCount(0);//初始化的行数为0
	this->setColumnCount(3);//初始化的列数为3

	QStringList headList;
	headList << tr("歌曲") << tr("艺人") << tr("专辑");
	this->setHorizontalHeaderLabels(headList);//设置头信息

	this->horizontalHeader()->setVisible(false);//设置水平头不可见
	this->verticalHeader()->setVisible(false); //设置垂直头不可见
	this->setSelectionMode(QAbstractItemView::SingleSelection);//设置只能选中单行
	this->setSelectionBehavior(QAbstractItemView::SelectRows);//设置选择习惯为选择行
	this->setEditTriggers(QAbstractItemView::NoEditTriggers);//设置内容不可编辑
	this->horizontalHeader()->setResizeMode(QHeaderView::Stretch);//设置内容自动横向填满可用区域
	this->setFrameStyle(QFrame::NoFrame);//去掉边框
	this->setShowGrid(false);//设置不显示网格
	this->setItemDelegate(new NoFocusDelegate());//去掉选中单元格的虚线
	this->horizontalHeader()->setHighlightSections(false);//点击表时不对表头行光亮（获取焦点）
	this->horizontalHeader()->setClickable(false);//不响应鼠标单击

	//设置header样式
	this->horizontalHeader()->setStyleSheet("QHeaderView{background-color: gray;/*background:transparent;*/color:white; font:14px; font-family:'微软雅黑';"
		"border-left:transparent;border:transparent;border-top:transparent;border-bottom:transparent;}");
	this->setStyleSheet("QTableCornerButton::section{background:gray};");//设置表格直角方格样式
	this->verticalHeader()->setStyleSheet("QHeaderView::section{background-color: gray; color:white; font:14px; font-family:'微软雅黑';}");
}

SearchList::~SearchList(void)
{
}
