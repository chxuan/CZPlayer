#ifndef MUSICLRC_H
#define MUSICLRC_H

#include "head.h"

class MusicLrc : public QLabel
{
    Q_OBJECT

public:
    MusicLrc();
    ~MusicLrc();

public:
    void startLrcMask(qint64 intervalTime);
    void stopLrcMask();
    void unlockLrc();

    static void setFontYS(QString const &value);
    static void setFontSize(QString const &value);
    static void setFontType(QString const &value);
    static void setLinearGradient(QString const &shangValue, QString const &zhongValue, QString const &xiaValue);
    static void setMaskLinearGradient(QString const &shangValue, QString const &zhongValue, QString const &xiaValue);
    static void setLrcShadow(int const &value);

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void contextMenuEvent(QContextMenuEvent *event);

signals:
    void sig_LrcLocked();//歌词已经锁定信号
    void sig_LrcUnlocked();//歌词已经解锁信号

private slots:
    void slot_TimeOut();
    void slot_LockLrc();//锁定歌词

private:
    static QLinearGradient linearGradient;//歌词的线性渐变
    static QLinearGradient maskLinearGradient;//歌词的遮罩线性渐变
    static QFont font;
    QTimer *timer;
    qreal lrcMaskWidth;//qreal = float 浮点型，qrand = rand 随机数
    qreal lrcMaskWidthInterval;//各期遮罩宽度间隔
    QPoint dragPosition;
    qint16 flag;//歌词锁定标志

    static int shadow;
    static QPainter painter;
};

#endif // MUSICLRC_H


































