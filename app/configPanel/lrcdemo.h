#ifndef LRCDEMO_H
#define LRCDEMO_H

#include "head.h"

class LrcDemo : public QLabel
{
    Q_OBJECT

public:
    LrcDemo();
    ~LrcDemo();

public:
    static void setFontYS(QString const &value);
    static void setFontSize(QString const &value);
    static void setFontType(QString const &value);
    static void setLinearGradient(QString const &value);
    static void setLinearGradient2(QString const &value);
    static void setLinearGradient3(QString const &value);
    static void setMaskLinearGradient(QString const &value);
    static void setMaskLinearGradient2(QString const &value);
    static void setMaskLinearGradient3(QString const &value);
    static void setLrcMaskWidth(int const &value);
    static void setLrcDemoShadow(int const &value);

protected:
    void paintEvent(QPaintEvent *event);

private:
    static QLinearGradient linearGradient;//歌词的线性渐变
    static QLinearGradient maskLinearGradient;//歌词的遮罩线性渐变
    static QFont font;
    static qreal lrcMaskWidth;//qreal = float 浮点型，qrand = rand 随机数

    static int shadow;
};

#endif // LRCDEMO_H


































