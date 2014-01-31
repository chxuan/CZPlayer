#include "stdafx.h"
#include "musiclrc.h"

QLinearGradient MusicLrc::linearGradient;//歌词的线性渐变
QLinearGradient MusicLrc::maskLinearGradient;//歌词的遮罩线性渐变
QFont MusicLrc::font;
int MusicLrc::shadow;

MusicLrc::MusicLrc()
{
    flag = 0;//没有锁定歌词

    //去掉窗体边框和总是显示在屏幕最前面,但要和showNormal一起用
    this ->setWindowFlags(Qt::Tool | Qt::WindowStaysOnTopHint
                          | Qt::FramelessWindowHint);//去掉窗体边框,不在任务栏显示图标
    this ->setAttribute(Qt::WA_TranslucentBackground);//设置背景透明
    this ->setWindowIcon(QIcon(":/images/CZPlayer.png"));//设置logo
    this ->resize(1000, 60);

    //设置歌词位置
    double width = QApplication::desktop() ->width();
    double height = QApplication::desktop() ->height();
    this ->move(QPoint(width / 2 - 800 / 2, height - 90));

    //歌词的线性渐变填充
    linearGradient.setStart(0, 10);//填充的起点坐标
    linearGradient.setFinalStop(0, 40);//填充的终点坐标
    //第一个参数终点坐标，相对于我们上面的区域而言，按照比例进行计算
//    linearGradient.setColorAt(0.1, QColor(14, 179, 255));
//    linearGradient.setColorAt(0.5, QColor(114, 232, 255));
//    linearGradient.setColorAt(0.9, QColor(14, 179, 255));

    //遮罩的线性渐变填充
    maskLinearGradient.setStart(0, 10);
    maskLinearGradient.setFinalStop(0, 40);
//    maskLinearGradient.setColorAt(0.1, QColor(222, 54, 4));
//    maskLinearGradient.setColorAt(0.5, QColor(255, 72, 16));
//    maskLinearGradient.setColorAt(0.9, QColor(222, 54, 4));

//    //设置字体
//    font.setFamily("微软雅黑");
//    //font.setBold(true);
//    font.setPointSize(30);

    //设置定时器
    timer = new QTimer(this);

    lrcMaskWidth = 0;
    lrcMaskWidthInterval = 0;

    connect(timer, SIGNAL(timeout()), this, SLOT(slot_TimeOut()));
}

MusicLrc::~MusicLrc()
{
	if (timer) {timer ->stop(); delete timer; timer = 0;}
}

//开启歌词遮罩，需要指定当前歌词开始与结束之间的时间间隔
void MusicLrc::startLrcMask(qint64 intervalTime)
{
    //这里设置每隔30毫秒更新一次遮罩的宽度，因为如果更新太平凡
    //会增加cpu占用率，而如果时间间隔太大，则动画效果就不流畅了
    qreal count = intervalTime / 30;

    //获取遮罩每次需要增加的宽度，这里的800是部件的固定宽度
    lrcMaskWidthInterval = 1000 / count;
    lrcMaskWidth = 0;
    timer ->start(30);
}

//停止歌词遮罩
void MusicLrc::stopLrcMask()
{
    timer ->stop();
    lrcMaskWidth = 0;
    this ->update();
}

void MusicLrc::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setFont(font);

    //先绘制底层文字，作为阴影，这样会使显示效果更加清晰，且更有质感
    painter.setPen(QColor(0, 0, 0, shadow));
    painter.drawText(1, 1, 1000, 60, Qt::AlignLeft, text());//左对齐

    //再在上面绘制渐变文字
    painter.setPen(QPen(linearGradient, 0));
    painter.drawText(0, 0, 1000, 60, Qt::AlignLeft, text());

    //设置歌词遮罩
    painter.setPen(QPen(maskLinearGradient, 0));
    painter.drawText(0, 0, lrcMaskWidth, 60, Qt::AlignLeft, text());
    event ->accept();
}

//重写mousePressEvent和mouseMoveEvent实现歌词的移动
void MusicLrc::mousePressEvent(QMouseEvent *event)
{
    if (flag == 0)//没有开启歌词锁定
    {
        if (event ->button() == Qt::LeftButton)
        {
            dragPosition = event ->globalPos() - frameGeometry().topLeft();
            event ->accept();
        }
    }
}

//重写mousePressEvent和mouseMoveEvent实现歌词的移动
void MusicLrc::mouseMoveEvent(QMouseEvent *event)
{
    if (flag == 0)//没有开启歌词锁定
    {
        if (event ->buttons() == Qt::LeftButton)
        {
            setCursor(Qt::PointingHandCursor);//显示手型
            move(event ->globalPos() - dragPosition);//移动窗体
            event ->accept();
        }
    }
}

//右击事件
void MusicLrc::contextMenuEvent(QContextMenuEvent *event)
{
    if (flag == 0)//没有开启歌词锁定
    {
        QMenu menu;
        menu.addAction(QIcon(":/images/lrc.png"), tr("隐藏歌词"), this, SLOT(hide()));
        menu.addAction(QIcon(":images/lock.png"), tr("锁定歌词"), this, SLOT(slot_LockLrc()));
        menu.exec(event ->globalPos());//globalPos()为当前鼠标的位置坐标
    }
    event ->accept();
}

//锁定歌词
void MusicLrc::slot_LockLrc()
{
    flag = 1;//开启歌词锁定
    emit sig_LrcLocked();//发射歌词已经锁定信号
}

//解锁歌词
void MusicLrc::unlockLrc()
{
    flag = 0;
    emit sig_LrcUnlocked();//发射歌词已经解锁信号
}

//设置字体
void MusicLrc::setFontYS(const QString &value)
{
    font.setFamily(value);
}

//设置字体大小
void MusicLrc::setFontSize(const QString &value)
{
    font.setPointSize(value.toInt());
}

//设置字体类型
void MusicLrc::setFontType(const QString &value)
{
    if (value == "常规")
    {
        font.setBold(false);
        font.setItalic(false);
    }
    else if (value == "粗体")
    {
        font.setBold(true);
        font.setItalic(false);
    }
    else if (value == "倾斜")
    {
        font.setBold(false);
        font.setItalic(true);
    }
    else if (value == "粗体 倾斜")
    {
        font.setBold(true);
        font.setItalic(true);
    }
}

//设置歌词未播放颜色
void MusicLrc::setLinearGradient(const QString &shangValue, const QString &zhongValue, const QString &xiaValue)
{
    QStringList shangList = shangValue.split(" ");
    int shangR = shangList.at(0).toInt();
    int shangG = shangList.at(1).toInt();
    int shangB = shangList.at(2).toInt();
    QStringList zhongList = zhongValue.split(" ");
    int zhongR = zhongList.at(0).toInt();
    int zhongG = zhongList.at(1).toInt();
    int zhongB = zhongList.at(2).toInt();
    QStringList xiaList = xiaValue.split(" ");
    int xiaR = xiaList.at(0).toInt();
    int xiaG = xiaList.at(1).toInt();
    int xiaB = xiaList.at(2).toInt();

    linearGradient.setColorAt(0.1, QColor(shangR, shangG, shangB));
    linearGradient.setColorAt(0.5, QColor(zhongR, zhongG, zhongB));
    linearGradient.setColorAt(0.9, QColor(xiaR, xiaG, xiaB));
}

//设置歌词已播放颜色
void MusicLrc::setMaskLinearGradient(const QString &shangValue, const QString &zhongValue, const QString &xiaValue)
{
    QStringList shangList = shangValue.split(" ");
    int shangR = shangList.at(0).toInt();
    int shangG = shangList.at(1).toInt();
    int shangB = shangList.at(2).toInt();
    QStringList zhongList = zhongValue.split(" ");
    int zhongR = zhongList.at(0).toInt();
    int zhongG = zhongList.at(1).toInt();
    int zhongB = zhongList.at(2).toInt();
    QStringList xiaList = xiaValue.split(" ");
    int xiaR = xiaList.at(0).toInt();
    int xiaG = xiaList.at(1).toInt();
    int xiaB = xiaList.at(2).toInt();

    maskLinearGradient.setColorAt(0.1, QColor(shangR, shangG, shangB));
    maskLinearGradient.setColorAt(0.5, QColor(zhongR, zhongG, zhongB));
    maskLinearGradient.setColorAt(0.9, QColor(xiaR, xiaG, xiaB));
}

//设置歌词阴影效果
void MusicLrc::setLrcShadow(const int &value)
{
    shadow = value;
}

//定时器
void MusicLrc::slot_TimeOut()
{
    //每隔一段固定的时间笼罩的长度就增加一点
    lrcMaskWidth += lrcMaskWidthInterval;
    this ->update();//更新歌词显示，但是并不立即重绘，而是安排一个Paint事件，当返回主循环时由系统来重绘
}


























