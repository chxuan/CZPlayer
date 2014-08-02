#include "stdafx.h"
#include "spectrumanalyser.h"
#include "utils.h"

#include <QtCore/qmath.h>
#include <QtCore/qmetatype.h>
#include <QtMultimedia/QAudioFormat>
#include <QThread>
#include "fftreal_wrapper.h"

//频谱分析线程
SpectrumAnalyserThread::SpectrumAnalyserThread(QObject *parent) : QObject(parent)
#ifndef DISABLE_FFT
    ,   m_fft(new FFTRealWrapper)
#endif
    ,   m_numSamples(SpectrumLengthSamples)
    ,   m_windowFunction(DefaultWindowFunction)
    ,   m_window(SpectrumLengthSamples, 0.0)
    ,   m_input(SpectrumLengthSamples, 0.0)
    ,   m_output(SpectrumLengthSamples, 0.0)
    ,   m_spectrum(SpectrumLengthSamples)
#ifdef SPECTRUM_ANALYSER_SEPARATE_THREAD
    ,   m_thread(new QThread(this))
#endif
{
#ifdef SPECTRUM_ANALYSER_SEPARATE_THREAD
    // moveToThread() cannot be called on a QObject with a parent
    setParent(0);
    moveToThread(m_thread);
    m_thread->start();
#endif
    calculateWindow();
}

SpectrumAnalyserThread::~SpectrumAnalyserThread()
{
#ifndef DISABLE_FFT
    delete m_fft;
#endif
}

void SpectrumAnalyserThread::setWindowFunction(WindowFunction type)
{
    m_windowFunction = type;
    calculateWindow();
}

void SpectrumAnalyserThread::calculateWindow()
{
    for (int i=0; i<m_numSamples; ++i) {
        DataType x = 0.0;

        switch (m_windowFunction) {
        case NoWindow:
            x = 1.0;
            break;
        case HannWindow:
            x = 0.5 * (1 - qCos((2 * M_PI * i) / (m_numSamples - 1)));
            break;
        default:
            Q_ASSERT(false);
        }

        m_window[i] = x;
    }
}

//计算频谱
void SpectrumAnalyserThread::calculateSpectrum(const QByteArray &buffer, int inputFrequency, int bytesPerSample)
{
#ifndef DISABLE_FFT
    Q_ASSERT(buffer.size() == m_numSamples * bytesPerSample);

    // Initialize data array
    const char *ptr = buffer.constData();
    for (int i=0; i<m_numSamples; ++i)
	{
        const qint16 pcmSample = *reinterpret_cast<const qint16*>(ptr);
        // Scale down to range [-1.0, 1.0]
        const DataType realSample = pcmToReal(pcmSample);
        const DataType windowedSample = realSample * m_window[i];
        m_input[i] = windowedSample;
        ptr += bytesPerSample;
    }

    // Calculate the FFT快速傅里叶变换
    m_fft->calculateFFT(m_output.data(), m_input.data());

    // Analyze output to obtain amplitude and phase for each frequency
    for (int i = 2; i <= m_numSamples / 2; ++i) 
	{
        // Calculate frequency of this complex sample
        m_spectrum[i].frequency = double(i * inputFrequency) / (m_numSamples);

        const double real = m_output[i];
        double imag = 0.0;
        if (i > 0 && i < m_numSamples / 2)
		{
            imag = m_output[m_numSamples / 2 + i];
		}

        const double magnitude = sqrt(real * real + imag * imag);
        double amplitude = SpectrumAnalyserMultiplier * log(magnitude);

        // Bound amplitude to [0.0, 1.0]
        m_spectrum[i].clipped = (amplitude > 1.0);
        amplitude = qMax(double(0.0), amplitude);
        amplitude = qMin(double(1.0), amplitude);
        m_spectrum[i].amplitude = amplitude;
    }
#endif

    emit calculationComplete(m_spectrum);	//发射计算完成信号
}

//频谱分析仪
SpectrumAnalyser::SpectrumAnalyser(QObject *parent)
    :   QObject(parent)
    ,   m_thread(new SpectrumAnalyserThread(this))
    ,   m_state(Idle)
#ifdef DUMP_SPECTRUMANALYSER
    ,   m_count(0)
#endif
{
    connect(m_thread, SIGNAL(calculationComplete(FrequencySpectrum)),
                    this, SLOT(calculationComplete(FrequencySpectrum)));
}

SpectrumAnalyser::~SpectrumAnalyser()
{

}

#ifdef DUMP_SPECTRUMANALYSER
void SpectrumAnalyser::setOutputPath(const QString &outputDir)
{
    m_outputDir.setPath(outputDir);
    m_textFile.setFileName(m_outputDir.filePath("spectrum.txt"));
    m_textFile.open(QIODevice::WriteOnly | QIODevice::Text);
    m_textStream.setDevice(&m_textFile);
}
#endif

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

void SpectrumAnalyser::setWindowFunction(WindowFunction type)
{
    const bool b = QMetaObject::invokeMethod(m_thread, "setWindowFunction",
                              Qt::AutoConnection,
                              Q_ARG(WindowFunction, type));
    Q_ASSERT(b);
    Q_UNUSED(b) // suppress warnings in release builds
}

//频谱分析仪进行计算
void SpectrumAnalyser::calculate(const QByteArray &buffer,
                         const QAudioFormat &format)
{
    if (isReady())
	{
        Q_ASSERT(isPCMS16LE(format));

        const int bytesPerSample = format.sampleSize() * format.channels() / 8;

#ifdef DUMP_SPECTRUMANALYSER
        m_count++;
        const QString pcmFileName = m_outputDir.filePath(QString("spectrum_%1.pcm").arg(m_count, 4, 10, QChar('0')));
        QFile pcmFile(pcmFileName);
        pcmFile.open(QIODevice::WriteOnly);
        const int bufferLength = m_numSamples * bytesPerSample;
        pcmFile.write(buffer, bufferLength);

        m_textStream << "TimeDomain " << m_count << "\n";
        const qint16* input = reinterpret_cast<const qint16*>(buffer);
        for (int i=0; i<m_numSamples; ++i) {
            m_textStream << i << "\t" << *input << "\n";
            input += format.channels();
        }
#endif

        m_state = Busy;

        // Invoke SpectrumAnalyserThread::calculateSpectrum using QMetaObject.  If
        // m_thread is in a different thread from the current thread, the
        // calculation will be done in the child thread.
        // Once the calculation is finished, a calculationChanged signal will be
        // emitted by m_thread.
        const bool b = QMetaObject::invokeMethod(m_thread, "calculateSpectrum",
                                  Qt::AutoConnection,
                                  Q_ARG(QByteArray, buffer),
                                  Q_ARG(int, format.frequency()),
                                  Q_ARG(int, bytesPerSample));
        Q_ASSERT(b);
        Q_UNUSED(b) // suppress warnings in release builds

#ifdef DUMP_SPECTRUMANALYSER
        m_textStream << "FrequencySpectrum " << m_count << "\n";
        FrequencySpectrum::const_iterator x = m_spectrum.begin();
        for (int i=0; i<m_numSamples; ++i, ++x)
            m_textStream << i << "\t"
                         << x->frequency << "\t"
                         << x->amplitude<< "\t"
                         << x->phase << "\n";
#endif
    }
}

//判断是否已经进行频谱
bool SpectrumAnalyser::isReady() const
{
    return (Idle == m_state);
}

//取消频谱计算
void SpectrumAnalyser::cancelCalculation()
{
    if (Busy == m_state)
	{
        m_state = Cancelled;
	}
}

//计算完成
void SpectrumAnalyser::calculationComplete(const FrequencySpectrum &spectrum)
{
    Q_ASSERT(Idle != m_state);
    if (Busy == m_state)
	{
        emit spectrumChanged(spectrum);	//计算完成后发射频谱改变信号
	}
    m_state = Idle;
}




