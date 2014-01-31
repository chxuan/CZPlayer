#ifndef TONEGENERATOR_H
#define TONEGENERATOR_H

#include <QtCore/qglobal.h>
#include "spectrum.h"

QT_FORWARD_DECLARE_CLASS(QAudioFormat)
QT_FORWARD_DECLARE_CLASS(QByteArray)

/**
 * Generate a sine wave
 */
void generateTone(const SweptTone &tone, const QAudioFormat &format, QByteArray &buffer);

#endif // TONEGENERATOR_H

