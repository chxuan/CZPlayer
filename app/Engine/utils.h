#ifndef UTILS_H
#define UTILS_H

#include <QtCore/qglobal.h>
#include <QDebug>

QT_FORWARD_DECLARE_CLASS(QAudioFormat)

//-----------------------------------------------------------------------------
// Miscellaneous utility functions
//-----------------------------------------------------------------------------

long long audioDuration(const QAudioFormat &format, long long bytes);
long long audioLength(const QAudioFormat &format, long long microSeconds);

QString formatToString(const QAudioFormat &format);

double nyquistFrequency(const QAudioFormat &format);

// Scale PCM value to [-1.0, 1.0]
double pcmToReal(qint16 pcm);

// Scale real value in [-1.0, 1.0] to PCM
qint16 realToPcm(double real);

// Check whether the audio format is PCM
bool isPCM(const QAudioFormat &format);

// Check whether the audio format is signed, little-endian, 16-bit PCM
bool isPCMS16LE(const QAudioFormat &format);

// Compile-time calculation of powers of two

template<int N> class PowerOfTwo
{ public: static const int Result = PowerOfTwo<N-1>::Result * 2; };

template<> class PowerOfTwo<0>
{ public: static const int Result = 1; };


//-----------------------------------------------------------------------------
// Debug output
//-----------------------------------------------------------------------------

class NullDebug
{
public:
    template <typename T>
    NullDebug& operator<<(const T&) { return *this; }
};

inline NullDebug nullDebug() { return NullDebug(); }

#endif // UTILS_H
