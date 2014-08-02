#include "stdafx.h"
#include "lrcdemo.h"

QLinearGradient LrcDemo::linearGradient;//歌词的线性渐变
QLinearGradient LrcDemo::maskLinearGradient;//歌词的遮罩线性渐变
QFont LrcDemo::font;
qreal LrcDemo::lrcMaskWidth = 0;
int LrcDemo::shadow;

LrcDemo::LrcDemo()
{
    this->setWindowFlags(Qt::FramelessWindowHint);//去掉窗体边框
    this->setAttribute(Qt::WA_TranslucentBackground);//设置背景透明
    this->resize(500, 60);
    this->setText(tr("CZPlayer,传播好音乐"));
    this->setAlignment(Qt::AlignCenter);

    //歌词的线性渐变填充
    linearGradient.setStart(0, 10);//填充的起点坐标
//    linearGradient.setFinalStop(0, 40);//填充的终点坐标
//    linearGradient.setColorAt(0.1, QColor(14, 179, 255));
//    linearGradient.setColorAt(0.5, QColor(114, 232, 255));
//    linearGradient.setColorAt(0.9, QColor(14, 179, 255));

    //遮罩的线性渐变填充
    maskLinearGradient.setStart(0, 10);
    maskLinearGradient.setFinalStop(0, 40);
//    maskLinearGradient.setColorAt(0.1, QColor(222, 54, 4));
//    maskLinearGradient.setColorAt(0.5, QColor(255, 72, 16));
//    maskLinearGradient.setColorAt(0.9, QColor(222, 54, 4));

    //lrcMaskWidth += 13 * 30 / 2;
    this->update();
}

LrcDemo::~LrcDemo()
{
}

void LrcDemo::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setFont(font);

    //先绘制底层文字，作为阴影，这样会使显示效果更加清晰，且更有质感
    painter.setPen(QColor(0, 0, 0, shadow));
    painter.drawText(1, 1, 500, 60, Qt::AlignLeft, text());//左对齐

    //再在上面绘制渐变文字
    painter.setPen(QPen(linearGradient, 0));
    painter.drawText(0, 0, 500, 60, Qt::AlignLeft, text());

    //设置歌词遮罩
    painter.setPen(QPen(maskLinearGradient, 0));
    painter.drawText(0, 0, lrcMaskWidth, 60, Qt::AlignLeft, text());
    event->accept();
}

//设置字体
void LrcDemo::setFontYS(const QString &value)
{
    font.setFamily(value);
}

//设置字体大小
void LrcDemo::setFontSize(const QString &value)
{
    font.setPointSize(value.toInt());
}

//设置字体类型
void LrcDemo::setFontType(const QString &value)
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
void LrcDemo::setLinearGradient(const QString &value)
{
    QStringList shangList = value.split(" ");
    int shangR = shangList.at(0).toInt();
    int shangG = shangList.at(1).toInt();
    int shangB = shangList.at(2).toInt();
    linearGradient.setColorAt(0.1, QColor(shangR, shangG, shangB));
}

void LrcDemo::setLinearGradient2(const QString &value)
{
    QStringList zhongList = value.split(" ");
    int zhongR = zhongList.at(0).toInt();
    int zhongG = zhongList.at(1).toInt();
    int zhongB = zhongList.at(2).toInt();
    linearGradient.setColorAt(0.5, QColor(zhongR, zhongG, zhongB));
}

void LrcDemo::setLinearGradient3(const QString &value)
{
    QStringList xiaList = value.split(" ");
    int xiaR = xiaList.at(0).toInt();
    int xiaG = xiaList.at(1).toInt();
    int xiaB = xiaList.at(2).toInt();
    linearGradient.setColorAt(0.9, QColor(xiaR, xiaG, xiaB));
}

//设置歌词已播放颜色
void LrcDemo::setMaskLinearGradient(const QString &value)
{
    QStringList shangList = value.split(" ");
    int shangR = shangList.at(0).toInt();
    int shangG = shangList.at(1).toInt();
    int shangB = shangList.at(2).toInt();
    maskLinearGradient.setColorAt(0.1, QColor(shangR, shangG, shangB));
}

void LrcDemo::setMaskLinearGradient2(const QString &value)
{
    QStringList zhongList = value.split(" ");
    int zhongR = zhongList.at(0).toInt();
    int zhongG = zhongList.at(1).toInt();
    int zhongB = zhongList.at(2).toInt();
    maskLinearGradient.setColorAt(0.5, QColor(zhongR, zhongG, zhongB));
}

void LrcDemo::setMaskLinearGradient3(const QString &value)
{
    QStringList xiaList = value.split(" ");
    int xiaR = xiaList.at(0).toInt();
    int xiaG = xiaList.at(1).toInt();
    int xiaB = xiaList.at(2).toInt();
    maskLinearGradient.setColorAt(0.9, QColor(xiaR, xiaG, xiaB));
}

//设置mask长度
void LrcDemo::setLrcMaskWidth(const int &value)
{
    lrcMaskWidth = value;
}

//改变歌词灰度
void LrcDemo::setLrcDemoShadow(const int &value)
{
    shadow = value;
}


























