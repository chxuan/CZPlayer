#ifndef SPECTRUMANALYSER_H
#define SPECTRUMANALYSER_H

#include <QByteArray>
#include <QObject>
#include <QVector>

#ifdef DUMP_SPECTRUMANALYSER
#include <QDir>
#include <QFile>
#include <QTextStream>
#endif

#include "frequencyspectrum.h"
#include "spectrum.h"

#ifndef DISABLE_FFT
#include "FFTRealFixLenParam.h"
#endif

QT_FORWARD_DECLARE_CLASS(QAudioFormat)
QT_FORWARD_DECLARE_CLASS(QThread)

class FFTRealWrapper;

class SpectrumAnalyserThreadPrivate;

//频谱线程
class SpectrumAnalyserThread : public QObject
{
    Q_OBJECT
public:
    SpectrumAnalyserThread(QObject *parent);
    ~SpectrumAnalyserThread();

public slots:
    void setWindowFunction(WindowFunction type);
	//计算频谱
    void calculateSpectrum(const QByteArray &buffer, int inputFrequency, int bytesPerSample);

signals:
	//计算完成信号
    void calculationComplete(const FrequencySpectrum &spectrum);

private:
    void calculateWindow();

private:
#ifndef DISABLE_FFT
    FFTRealWrapper*                             m_fft;
#endif

    const int                                   m_numSamples;

    WindowFunction                              m_windowFunction;

#ifdef DISABLE_FFT
    typedef double                               DataType;
#else
    typedef FFTRealFixLenParam::DataType        DataType;
#endif
    QVector<DataType>                           m_window;

    QVector<DataType>                           m_input;
    QVector<DataType>                           m_output;

    FrequencySpectrum                           m_spectrum;

#ifdef SPECTRUM_ANALYSER_SEPARATE_THREAD
    QThread*                                    m_thread;
#endif
};

//频谱分析仪外包装
class SpectrumAnalyser : public QObject
{
    Q_OBJECT
public:
    SpectrumAnalyser(QObject *parent = 0);
    ~SpectrumAnalyser();

#ifdef DUMP_SPECTRUMANALYSER
    void setOutputPath(const QString &outputPath);
#endif

public:
    void setWindowFunction(WindowFunction type);
    void calculate(const QByteArray &buffer, const QAudioFormat &format);
    bool isReady() const;
    void cancelCalculation();

signals:
    void spectrumChanged(const FrequencySpectrum &spectrum);

private slots:
    void calculationComplete(const FrequencySpectrum &spectrum);

private:
    void calculateWindow();

private:
    SpectrumAnalyserThread*    m_thread;
    enum State 
	{
        Idle,
        Busy,
        Cancelled
    };
    State              m_state;

#ifdef DUMP_SPECTRUMANALYSER
    QDir                m_outputDir;
    int                 m_count;
    QFile               m_textFile;
    QTextStream         m_textStream;
#endif
};

#endif // SPECTRUMANALYSER_H

