#include "stdafx.h"
#include "spectrum.h"
#include "utils.h"
#include <QByteArray>
#include <QtMultimedia/QAudioFormat>
#include <QtCore/qmath.h>
#include <QtCore/qendian.h>

void generateTone(const SweptTone &tone, const QAudioFormat &format, QByteArray &buffer)
{
    Q_ASSERT(isPCMS16LE(format));

    const int channelBytes = format.sampleSize() / 8;
    const int sampleBytes = format.channels() * channelBytes;
    int length = buffer.size();
    const int numSamples = buffer.size() / sampleBytes;

    Q_ASSERT(length % sampleBytes == 0);
    Q_UNUSED(sampleBytes) // suppress warning in release builds

    unsigned char *ptr = reinterpret_cast<unsigned char *>(buffer.data());

    double phase = 0.0;

    const double d = 2 * M_PI / format.frequency();

    // We can't generate a zero-frequency sine wave
    const double startFreq = tone.startFreq ? tone.startFreq : 1.0;

    // Amount by which phase increases on each sample
    double phaseStep = d * startFreq;

    // Amount by which phaseStep increases on each sample
    // If this is non-zero, the output is a frequency-swept tone
    const double phaseStepStep = d * (tone.endFreq - startFreq) / numSamples;

    while (length) {
        const double x = tone.amplitude * qSin(phase);
        const qint16 value = realToPcm(x);
        for (int i=0; i<format.channels(); ++i) {
            qToLittleEndian<qint16>(value, ptr);
            ptr += channelBytes;
            length -= channelBytes;
        }

        phase += phaseStep;
        while (phase > 2 * M_PI)
            phase -= 2 * M_PI;
        phaseStep += phaseStepStep;
    }
}

