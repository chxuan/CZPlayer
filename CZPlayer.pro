#-------------------------------------------------
#
# Project created by QtCreator 2015-02-12T13:54:45
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

DEFINES += CZ_DEBUG

#生成的可执行文件路径
TARGET = ../../CZPlayer/ExecDir/CZPlayer
TEMPLATE = app

include($$PWD/MainWidget/MainWidget.pri)
include($$PWD/MinWidget/MinWidget.pri)
include($$PWD/AboutPanel/AboutPanel.pri)
include($$PWD/ConfigPanel/ConfigPanel.pri)
include($$PWD/Core/Core.pri)
include($$PWD/DB/DB.pri)
include($$PWD/Engine/Engine.pri)
include($$PWD/Entity/Entity.pri)
include($$PWD/LrcLabel/LrcLabel.pri)
include($$PWD/MusicList/MusicList.pri)
include($$PWD/MediaWidget/MediaWidget.pri)
include($$PWD/Thread/Thread.pri)

#使用全局热键库
include($$PWD/3rdParty/qxtglobalshortcut/qxtglobalshortcut.pri)
#使用单例库
include($$PWD/3rdParty/qtsingleapplication/qtsingleapplication.pri)

SOURCES += main.cpp

HEADERS  +=

RESOURCES += \
    Resources/CZPlayer.qrc

RC_FILE += $$PWD/Resources/CZPlayer.rc

