DEPENDPATH  += $$PWD
INCLUDEPATH += $$PWD

HEADERS += \
    $$PWD/GlobalHotkey.h \
    $$PWD/NoFocusDelegate.h \
    $$PWD/signal_slot.h \
    $$PWD/StatusDefine.h \
    $$PWD/TR.h \
    $$PWD/ChineseToLetter.h \
    $$PWD/AutoStart.h \
    $$PWD/FileRelation.h \
    $$PWD/AutoStartHandle.h \
    $$PWD/FileRelationHandle.h

SOURCES += \
    $$PWD/GlobalHotkey.cpp \
    $$PWD/NoFocusDelegate.cpp \
    $$PWD/AutoStart.cpp \
    $$PWD/FileRelation.cpp \
    $$PWD/AutoStartHandle.cpp \
    $$PWD/FileRelationHandle.cpp

#操作注册表相关api依赖库
LIBS += -lAdvapi32
#刷新系统缓存函数：SHChangeNotify的依赖库
LIBS += -lshell32

