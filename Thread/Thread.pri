DEPENDPATH  += $$PWD
INCLUDEPATH += $$PWD

QT       += xml
QT       += network

HEADERS += \
    $$PWD/AlbumThread.h \
    $$PWD/LoadMusicThread.h \
    $$PWD/LrcThread.h \
    $$PWD/SearchThread.h \
    $$PWD/DownloadMusicThread.h

SOURCES += \
    $$PWD/AlbumThread.cpp \
    $$PWD/LoadMusicThread.cpp \
    $$PWD/LrcThread.cpp \
    $$PWD/SearchThread.cpp \
    $$PWD/DownloadMusicThread.cpp

