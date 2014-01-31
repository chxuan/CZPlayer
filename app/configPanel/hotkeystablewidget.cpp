#include "stdafx.h"
#include "hotkeystablewidget.h"
#include "nofocusdelegate.h"

HotKeysTableWidget::HotKeysTableWidget(QWidget *parent) : m_parent(parent)
{
    //设置窗口基本属性
    this ->setRowCount(0);//初始化的行数为0
    this ->setColumnCount(2);//初始化的列数为3

    QStringList headList;
    headList << tr("动作") << tr("全局热键");
    this ->setHorizontalHeaderLabels(headList);//设置头信息

//    this ->horizontalHeader() ->setVisible(false);
    this ->verticalHeader() ->setVisible(false);
    this ->horizontalHeader() ->setResizeMode(QHeaderView::Stretch);//设置内容自动横向填满可用区域
//    this ->horizontalHeader() ->resizeSection(0, 30);
//    this ->horizontalHeader() ->resizeSection(1, 170);
//    this ->horizontalHeader() ->resizeSection(2, 100);
//    this ->horizontalHeader() ->resizeSection(3, 65);
    this ->horizontalHeader() ->setHighlightSections(false);//点击表时不对表头行光亮（获取焦点）
    this ->horizontalHeader() ->setClickable(false);//不响应鼠标单击
    this ->setSelectionMode(QAbstractItemView::SingleSelection);//设置只能选中单行
    this ->setSelectionBehavior(QAbstractItemView::SelectRows);//设置选择习惯为选择行
    //this ->setEditTriggers(QAbstractItemView::NoEditTriggers);//设置内容不可编辑
    this ->setShowGrid(false);//设置不显示网格
    this ->setItemDelegate(new NoFocusDelegate());//去掉选中单元格的虚线

    this ->setStyleSheet("color: black;");
}

HotKeysTableWidget::~HotKeysTableWidget()
{
}
