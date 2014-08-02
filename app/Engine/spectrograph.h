#ifndef SPECTROGRAPH_H
#define SPECTROGRAPH_H

#include <QtCore>
#include <QtGui>
#include "frequencyspectrum.h"

QT_FORWARD_DECLARE_CLASS(QMouseEvent)

//频谱绘制
class Spectrograph : public QLabel
{
    Q_OBJECT

public:
    Spectrograph(QWidget *parent = 0);
    ~Spectrograph();

public:
	//numBars:频谱显示方格个数
	//lowFreq:频谱下界
	//highFreq:频谱上节
    void setParams(int numBars, double lowFreq, double highFreq);//设置参数
	
protected:
	//重新实现以下event函数
    //void timerEvent(QTimerEvent *event);
    void paintEvent(QPaintEvent *event);
    //void mousePressEvent(QMouseEvent *event);

signals:
    void infoMessage(const QString &message, int intervalMs);	//消息信号

public slots:
    void reset();												//重置
    void spectrumChanged(const FrequencySpectrum &spectrum);	//改变频谱

private:
    int barIndex(double frequency) const;						//索引频谱bar
    QPair<double, double> barRange(int barIndex) const;			//频谱bar的范围
    void updateBars();											//刷新频谱bar
    //void selectBar(int index);									//选中频谱bar

private:
	//频谱bar结构体
    struct Bar 
	{
        Bar() : value(0.0), clipped(false) { }
        double   value;
        bool    clipped;										//频谱bar是否被截断
    };

    QVector<Bar>        m_bars;									//频谱bar vector
    int                 m_barSelected;							//频谱bar是否被选中
    int                 m_timerId;								//时间id
    double              m_lowFreq;								//频谱下界
    double              m_highFreq;								//频谱上界
    FrequencySpectrum   m_spectrum;								//通过快速傅里叶变换传过来的参数

	QPoint dragPosition;
};

#endif // SPECTROGRAPH_H
