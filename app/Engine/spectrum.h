#ifndef SPECTRUM_H
#define SPECTRUM_H

#include <QtCore>
#include "utils.h"
#include "fftreal_wrapper.h"

//用于计算频谱音频样本数
const int SpectrumLengthSamples		 = PowerOfTwo<FFTLengthPowerOfTwo>::Result;
const int    SpectrumNumBands		 = 25;						//频谱显示方格个数
const double  SpectrumLowFreq        = 0.0;						//频谱下界,单位HZ
const double  SpectrumHighFreq       = 1000.0;					//频谱上节,单位HZ
const long long WaveformWindowDuration = 500 * 1000;			//微秒波形窗口大小

//waveform的长度，以字节为单位
//在理想情况下，这些将匹配QAudio*::bufferSize()，但是这不是
//用，直到QAudio*::start()被调用，而我们
//为了初始化波形显示需要这个值。
//因此，我们只要选择一个合理的值
const int   WaveformTileLength  = 4096;
const double SpectrumAnalyserMultiplier = 0.15;					//用于计算频谱栏的高度
const int   NullMessageTimeout      = -1;						//禁用消息超时


//-----------------------------------------------------------------------------
// Types and data structures
//-----------------------------------------------------------------------------

enum WindowFunction
{
    NoWindow,
    HannWindow
};

const WindowFunction DefaultWindowFunction = HannWindow;

struct Tone 
{
    Tone(double freq = 0.0, double amp = 0.0)
    :   frequency(freq), amplitude(amp)
    { }

    // Start and end frequencies for swept tone generation
    double   frequency;

    // Amplitude in range [0.0, 1.0]
    double   amplitude;
};

struct SweptTone 
{
    SweptTone(double start = 0.0, double end = 0.0, double amp = 0.0)
    :   startFreq(start), endFreq(end), amplitude(amp)
    { 
		Q_ASSERT(end >= start);
	}

    SweptTone(const Tone &tone)
    :   startFreq(tone.frequency), endFreq(tone.frequency), amplitude(tone.amplitude)
    {
	}

    // Start and end frequencies for swept tone generation
    double   startFreq;
    double   endFreq;

    // Amplitude in range [0.0, 1.0]
    double   amplitude;
};


//-----------------------------------------------------------------------------
// Macros
//-----------------------------------------------------------------------------

// Macro which connects a signal to a slot, and which causes application to
// abort if the connection fails.  This is intended to catch programming errors
// such as mis-typing a signal or slot name.  It is necessary to write our own
// macro to do this - the following idiom
//     Q_ASSERT(connect(source, signal, receiver, slot));
// will not work because Q_ASSERT compiles to a no-op in release builds.

#define connect(source, signal, receiver, slot) \
    if(!connect(source, signal, receiver, slot)) \
        qt_assert_x(Q_FUNC_INFO, "connect failed", __FILE__, __LINE__);

// Handle some dependencies between macros defined in the .pro file

#ifdef DISABLE_WAVEFORM
#undef SUPERIMPOSE_PROGRESS_ON_WAVEFORM
#endif

#endif // SPECTRUM_H

