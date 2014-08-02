#include "stdafx.h"
#include "minmusicwidget.h"

MinMusicWidget::MinMusicWidget(QWidget *parent) : m_parent(parent)
{
    //设置窗口基本属性
    this->resize(200, 159);//设置窗体大小
    this->setWindowFlags(Qt::FramelessWindowHint);//去掉窗体边框
    this->setAttribute(Qt::WA_TranslucentBackground);//设置背景透明
    this->setWindowIcon(QIcon(":/images/CZPlayer.png"));//设置logo
    this->setAcceptDrops(true);//可以拖放文件
    this->setWindowTitle(tr("CZPlayer"));

    //播放
    minPlayButton = new QPushButton(this);
    minPlayButton->setObjectName(tr("minPlayButton"));
    minPlayButton->setToolTip(tr("播放"));

    //暂停
    minPauseButton = new QPushButton(this);
    minPauseButton->setObjectName(tr("minPauseButton"));
    minPauseButton->setToolTip(tr("暂停"));

    //停止
    minStopButton = new QPushButton(this);
    minStopButton->setObjectName(tr("minStopButton"));
    minStopButton->setToolTip(tr("停止"));

    //下一首
    minNextButton = new QPushButton(this);
    minNextButton->setObjectName(tr("minNextButton"));
    minNextButton->setToolTip(tr("下一首"));

    //上一首
    minLastButton = new QPushButton(this);
    minLastButton->setObjectName(tr("minLastButton"));
    minLastButton->setToolTip(tr("上一首"));

    //打开文件
    minOpenFileButton = new QPushButton(this);
    minOpenFileButton->setObjectName(tr("minOpenFileButton"));
    minOpenFileButton->setToolTip(tr("打开文件"));

    //桌面歌词
    minMusicLrcAction = new QAction(this);
    minMusicLrcAction->setIcon(QIcon(":/images/lrc.png"));
    minMusicLrcAction->setText(tr("显示桌面歌词"));

    //设置各个组件的位置和大小
    minPlayButton->setGeometry(2, 33, 37, 37);
    minPauseButton->setGeometry(10, 78, 40, 37);
    minStopButton->setGeometry(45, 109, 41, 37);
    minNextButton->setGeometry(91, 114, 40, 37);
    minLastButton->setGeometry(134, 94, 40, 39);
    minOpenFileButton->setGeometry(161, 55, 39, 39);

    connect(minPlayButton, SIGNAL(clicked()), parent, SLOT(slot_SetPlay()));				//播放
    connect(minPauseButton, SIGNAL(clicked()), parent, SLOT(slot_SetPause()));				//暂停
    connect(minStopButton, SIGNAL(clicked()), parent, SLOT(slot_SetStop()));				//停止
    connect(minNextButton, SIGNAL(clicked()), parent, SLOT(slot_SetNext()));				//下一首
    connect(minLastButton, SIGNAL(clicked()), parent, SLOT(slot_SetPre()));					//上一首
    connect(minOpenFileButton, SIGNAL(clicked()), parent, SLOT(slot_OpenFile()));			//打开文件
    connect(minMusicLrcAction, SIGNAL(triggered()), parent, SLOT(slot_OpenMusicLrc()));		//显示歌词
}

MinMusicWidget::~MinMusicWidget()
{
}

//打开主界面
void MinMusicWidget::slot_OpenMainWidget()
{
    if (m_parent->isHidden())
    {
        QPropertyAnimation *mainWidgetAnimation = new QPropertyAnimation(m_parent, "windowOpacity");
        mainWidgetAnimation->setDuration(500);
        mainWidgetAnimation->setStartValue(0);
        mainWidgetAnimation->setEndValue(1);
        mainWidgetAnimation->start();
        m_parent->show();
        emit sig_ShowMusicPanel();

        QPropertyAnimation *minWidgetAnimation = new QPropertyAnimation(this, "windowOpacity");
        minWidgetAnimation->setDuration(500);
        minWidgetAnimation->setStartValue(1);
        minWidgetAnimation->setEndValue(0);
        minWidgetAnimation->start();
        connect(minWidgetAnimation, SIGNAL(finished()), this, SLOT(slot_HideMinWidget()));
    }
}

//隐藏小界面
void MinMusicWidget::slot_HideMinWidget()
{
    this->hide();
}

//主界面鼠标右键点击事件
void MinMusicWidget::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu;
    menu.addAction(QIcon(":/images/minMainWidget.png"), tr("开启主界面"), this, SLOT(slot_OpenMainWidget()));
    menu.addAction(minMusicLrcAction);
    menu.exec(event->globalPos());//globalPos()为当前鼠标的位置坐标
    //menu.exec(QCursor::pos());
    event->accept();
}

//重写paintEvent,添加背景图片
void MinMusicWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QPixmap backgroundImage;
    backgroundImage.load(":/images/minMainBg2.png");

    //先通过pix的方法获得图片的过滤掉透明的部分得到的图片，作为loginPanel的不规则边框
    this->setMask(backgroundImage.mask());
    painter.drawPixmap(0, 0, 200, 159, backgroundImage);
    event->accept();
}

//重写mousePressEvent和mouseMoveEvent实现窗体的移动
void MinMusicWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        dragPosition = event->globalPos() - frameGeometry().topLeft();
        event->accept();
    }
}

//重写mousePressEvent和mouseMoveEvent实现窗体的移动
void MinMusicWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() == Qt::LeftButton)
    {
        this->move(event->globalPos() - dragPosition);//移动窗体
        event->accept();
    }
}
