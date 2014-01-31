#ifndef FREQUENCYSPECTRUM_H
#define FREQUENCYSPECTRUM_H

#include <QtCore/QVector>

/**
	表示频谱为一系列元素，其中每一个的 
	包含一个频率，振幅和相位
 */
//频谱元素集合(通过频谱分析仪对音频进行快速傅里叶变换取得音频的
//频率，相位，振幅，组成一个频谱元素集合再传递给频谱绘制引擎绘制)
class FrequencySpectrum 
{
public:
    FrequencySpectrum(int numPoints = 0);

    struct Element 
	{
        Element() : frequency(0.0), amplitude(0.0), phase(0.0), clipped(false)
        { }
        double frequency;		//赫兹频率
        double amplitude;		//振幅范围[0.0, 1.0]
        double phase;			//相位范围[0.0, 2*PI]
        bool clipped;			//指示是否值时频谱分析被截断
    };

    typedef QVector<Element>::iterator iterator;
    typedef QVector<Element>::const_iterator const_iterator;

    void reset();	//重置

    int count() const;
    Element& operator[](int index);
    const Element& operator[](int index) const;
    iterator begin();
    iterator end();
    const_iterator begin() const;
    const_iterator end() const;

private:
    QVector<Element> m_elements;

};

#endif // FREQUENCYSPECTRUM_H
