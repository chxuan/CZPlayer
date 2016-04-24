DEPENDPATH  += $$PWD
INCLUDEPATH += $$PWD

HEADERS += \
    $$PWD/AudioStream.h \
    $$PWD/BasicPlayer.h \
    $$PWD/CriticalSection.h \
    $$PWD/FastFourierTransform.h \
    $$PWD/IO.h \
    $$PWD/SoundControl.h \
    $$PWD/SpectroGraph.h \
    $$PWD/Stream.h \
    $$PWD/Thread.h \
    $$PWD/Utils.h

SOURCES += \
    $$PWD/AudioStream.cpp \
    $$PWD/BasicPlayer.cpp \
    $$PWD/FastFourierTransform.cpp \
    $$PWD/IO.cpp \
    $$PWD/SoundControl.cpp \
    $$PWD/SpectroGraph.cpp \
    $$PWD/Stream.cpp \
    $$PWD/Utils.cpp \
    $$PWD/WMWrapper.cpp

#使用libmad库
INCLUDEPATH += $$PWD/../3rdParty/libmad/include
CONFIG(release, debug|release): LIBS += -L$$PWD/../3rdParty/libmad/lib/   -llibmad
CONFIG(debug, debug|release): LIBS += -L$$PWD/../3rdParty/libmad/lib/   -llibmadd

#使用libogg库
INCLUDEPATH += $$PWD/../3rdParty/libogg/include
CONFIG(release, debug|release): LIBS += -L$$PWD/../3rdParty/libogg/lib/   -llibogg
CONFIG(debug, debug|release): LIBS += -L$$PWD/../3rdParty/libogg/lib/   -lliboggd

#使用libvorbis库
INCLUDEPATH += $$PWD/../3rdParty/libvorbis/include
CONFIG(release, debug|release): LIBS += -L$$PWD/../3rdParty/libvorbis/lib/   -llibvorbis
CONFIG(debug, debug|release): LIBS += -L$$PWD/../3rdParty/libvorbis/lib/   -llibvorbisd

#使用dx播放相关库
LIBS += -ldsound
LIBS += -ldxguid
LIBS += -lstrsafe
LIBS += -lwmvcore
LIBS += -lmsimg32
LIBS += -lmsacm32
LIBS += -lWinmm
LIBS += -lole32
