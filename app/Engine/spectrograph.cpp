#include "stdafx.h"
#include "spectrograph.h"

const int NullTimerId = -1;
const int NullIndex = -1;
const int BarSelectionInterval = 2000;	//频谱bar间隔的选择

Spectrograph::Spectrograph(QWidget *parent) : QLabel(parent) 
	, m_barSelected(NullIndex) , m_timerId(NullTimerId) , m_lowFreq(0.0) , m_highFreq(0.0)
{
	//设置频谱label的大小
	this->resize(190, 78);
}

Spectrograph::~Spectrograph()
{
}

//设置频谱参数
void Spectrograph::setParams(int numBars, double lowFreq, double highFreq)
{
    Q_ASSERT(numBars > 0);
    Q_ASSERT(highFreq > lowFreq);
    m_bars.resize(numBars);
    m_lowFreq = lowFreq;		//频谱下界
    m_highFreq = highFreq;		//频谱上界
    this->updateBars();		//更新频谱
}

//重新实现时间事件
//void Spectrograph::timerEvent(QTimerEvent *event)
//{
//    Q_ASSERT(event->timerId() == m_timerId);
//    Q_UNUSED(event) // suppress warnings in release builds
//    killTimer(m_timerId);
//    m_timerId = NullTimerId;
//    m_barSelected = NullIndex;
//    update();
//}

//绘制频谱
void Spectrograph::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QPainter painter(this);
	//QPixmap backgroundImage;
	//backgroundImage.load(":/images/screen.png");
	////先通过pix的方法获得图片的过滤掉透明的部分得到的图片，作为loginPanel的不规则边框
	//this->setMask(backgroundImage.mask());
	//painter.drawPixmap(0, 0, 190, 78, backgroundImage);

    const int numBars = m_bars.count();

	QColor barColor(5, 184, 204);		//频谱bar颜色
	QColor clipColor(255, 0, 0);		//频谱被截断后的颜色

    barColor = barColor.lighter();
    barColor.setAlphaF(0.75);		//设置alpha通道
    clipColor.setAlphaF(0.75);

    //绘制频谱
    if (numBars)
	{
        //计算宽度的条和空白
		const int widgetWidth = this->width();										//频谱widget宽度
        const int barPlusGapWidth = widgetWidth / numBars;							//每一个频谱加空白间隙的宽度
        const int barWidth = 0.8 * barPlusGapWidth;									//每一个频谱bar的宽度
        const int gapWidth = barPlusGapWidth - barWidth;							//每一个空白间隙宽度
        const int paddingWidth = widgetWidth - numBars * (barWidth + gapWidth);		//边缘宽度
        const int leftPaddingWidth = (paddingWidth + gapWidth) / 2;					//左边缘宽度
        const int barHeight = this->height() - 2 * gapWidth;						//每一个频谱bar的高度

		//绘制每一个频谱bar
        for (int i = 0; i < numBars; ++i)
		{
            const double value = m_bars[i].value;		//vlaue的值在0到1之间
            Q_ASSERT(value >= 0.0 && value <= 1.0);

            QRect bar = rect();
			//设置频谱bar的位置和大小
            bar.setLeft(rect().left() + leftPaddingWidth + (i * (gapWidth + barWidth)));
            bar.setWidth(barWidth);
            bar.setTop(rect().top() + gapWidth + (1.0 - value) * barHeight);
            bar.setBottom(rect().bottom() - gapWidth);

            QColor color = barColor;
            if (m_bars[i].clipped)
			{
                color = clipColor;
			}
			
			//设置颜色渐变
			//QLinearGradient linearGradient(bar.topLeft(), bar.bottomRight());
			//linearGradient.setColorAt(0.1, QColor(247, 104, 9));
			//linearGradient.setColorAt(1.0, QColor(238, 17, 128)); 
			//painter.fillRect(bar, QBrush(linearGradient));

            painter.fillRect(bar, color);
        }
    }
	event->accept();
}

//刷新频谱bar
void Spectrograph::updateBars()
{
	m_bars.fill(Bar());

	//遍历频谱参数
	for (FrequencySpectrum::const_iterator iter = m_spectrum.begin(); iter != m_spectrum.end(); ++iter)
	{
		const FrequencySpectrum::Element element = *iter;
		//如果经过快速傅里叶过的频率如果在【0.0-1.0】之间
		if (element.frequency >= m_lowFreq && element.frequency < m_highFreq) 
		{
			Bar &bar = m_bars[barIndex(element.frequency)];
			bar.value = qMax(bar.value, element.amplitude);	//频谱bar振幅范围
			bar.clipped |= element.clipped;
		}
	}
	update();
}

//重新实现鼠标点击事件
//void Spectrograph::mousePressEvent(QMouseEvent *event)
//{
//    const QPoint pos = event->pos();
//    const int index = m_bars.count() * (pos.x() - rect().left()) / rect().width();
//    this->selectBar(index);		//选中当前频谱bar
//}

//选中频谱bar
//void Spectrograph::selectBar(int index) 
//{
//	const QPair<double, double> frequencyRange = barRange(index);
//	const QString message = QString("%1 - %2 Hz")
//		.arg(frequencyRange.first)
//		.arg(frequencyRange.second);
//	emit infoMessage(message, BarSelectionInterval);
//
//	if (NullTimerId != m_timerId)
//	{
//		killTimer(m_timerId);
//	}
//	m_timerId = startTimer(BarSelectionInterval);
//	m_barSelected = index;
//	update();
//}

//重置频谱
void Spectrograph::reset()
{
    m_spectrum.reset();
    this->spectrumChanged(m_spectrum);	//改变频谱
}

//改变频谱
void Spectrograph::spectrumChanged(const FrequencySpectrum &spectrum)
{
    m_spectrum = spectrum;
    this->updateBars();	//刷新频谱
}

//索引频谱bar
int Spectrograph::barIndex(double frequency) const
{
    Q_ASSERT(frequency >= m_lowFreq && frequency < m_highFreq);
    const double bandWidth = (m_highFreq - m_lowFreq) / m_bars.count();
    const int index = (frequency - m_lowFreq) / bandWidth;
    if(index <0 || index >= m_bars.count())
	{
        Q_ASSERT(false);
	}
    return index;
}

//频谱bar的范围
QPair<double, double> Spectrograph::barRange(int index) const
{
    Q_ASSERT(index >= 0 && index < m_bars.count());
    const double bandWidth = (m_highFreq - m_lowFreq) / m_bars.count();
    return QPair<double, double>(index * bandWidth, (index+1) * bandWidth);
}


