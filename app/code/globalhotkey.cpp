#include "stdafx.h"
#include "globalhotkey.h"

QxtGlobalShortcut* GlobalHotKey::configOptions = 0;
QxtGlobalShortcut* GlobalHotKey::showHideMainWidget = 0;
QxtGlobalShortcut* GlobalHotKey::minMainWidget = 0;
QxtGlobalShortcut* GlobalHotKey::openMusicfile = 0;
QxtGlobalShortcut* GlobalHotKey::showDownload = 0;
QxtGlobalShortcut* GlobalHotKey::showMusicList = 0;
QxtGlobalShortcut* GlobalHotKey::showLrc = 0;
QxtGlobalShortcut* GlobalHotKey::playPause = 0;
QxtGlobalShortcut* GlobalHotKey::last = 0;
QxtGlobalShortcut* GlobalHotKey::nextSc = 0;

GlobalHotKey::GlobalHotKey(QWidget *parent)
{
    //选项设置
    configOptions = new QxtGlobalShortcut(this);

    //显示/隐藏主界面
    showHideMainWidget = new QxtGlobalShortcut(this);

    //迷你模式/正常窗口
    minMainWidget = new QxtGlobalShortcut(this);

    //打开音乐文件
    openMusicfile = new QxtGlobalShortcut(this);

    //显示/隐藏下载列表
    showDownload = new QxtGlobalShortcut(this);

    //显示/隐藏歌曲列表
    showMusicList = new QxtGlobalShortcut(this);

    //显示/隐藏桌面歌词
    showLrc = new QxtGlobalShortcut(this);

    //播放/暂停
    playPause = new QxtGlobalShortcut(this);

    //上一首
    last = new QxtGlobalShortcut(this);

    //下一首
    nextSc = new QxtGlobalShortcut(this);

    //信号与槽
    connect(configOptions, SIGNAL(activated()), parent, SLOT(slot_ConfigOptions()));
    connect(showHideMainWidget, SIGNAL(activated()), parent, SLOT(slot_ShowHideMainWidget()));
    connect(minMainWidget, SIGNAL(activated()), parent, SLOT(slot_MinMainWidget()));
    connect(openMusicfile, SIGNAL(activated()), parent, SLOT(slot_OpenMusicfile()));
    connect(showDownload, SIGNAL(activated()), parent, SLOT(slot_ShowDownload()));
    connect(showMusicList, SIGNAL(activated()), parent, SLOT(slot_ShowMusicList()));
    connect(showLrc, SIGNAL(activated()), parent, SLOT(slot_ShowLrc()));
    connect(playPause, SIGNAL(activated()), parent, SLOT(slot_PlayPause()));
    connect(last, SIGNAL(activated()), parent, SLOT(slot_Last()));
    connect(nextSc, SIGNAL(activated()), parent, SLOT(slot_Next()));
}

GlobalHotKey::~GlobalHotKey()
{
}

void GlobalHotKey::setConfigOptions(const QString &value)
{
    configOptions ->setShortcut(QKeySequence(value));
}

void GlobalHotKey::setShowHideMainWidget(const QString &value)
{
    showHideMainWidget ->setShortcut(QKeySequence(value));
}

void GlobalHotKey::setMinMainWidget(const QString &value)
{
    minMainWidget ->setShortcut(QKeySequence(value));
}

void GlobalHotKey::setOpenMusicfile(const QString &value)
{
    openMusicfile ->setShortcut(QKeySequence(value));
}

void GlobalHotKey::setShowDownload(const QString &value)
{
    showDownload ->setShortcut(QKeySequence(value));
}

void GlobalHotKey::setShowMusicList(const QString &value)
{
    showMusicList ->setShortcut(QKeySequence(value));
}

void GlobalHotKey::setShowLrc(const QString &value)
{
    showLrc ->setShortcut(QKeySequence(value));
}

void GlobalHotKey::setPlayPause(const QString &value)
{
    playPause ->setShortcut(QKeySequence(value));
}

void GlobalHotKey::setLast(const QString &value)
{
    last ->setShortcut(QKeySequence(value));
}

void GlobalHotKey::setNext(const QString &value)
{
    nextSc ->setShortcut(QKeySequence(value));
}

void GlobalHotKey::configOptionsDisabled( const bool &value )
{
	configOptions ->setDisabled(value);
}

void GlobalHotKey::showHideMainWidgetDisabled( const bool &value )
{
	showHideMainWidget ->setDisabled(value);
}

void GlobalHotKey::minMainWidgetDisabled( const bool &value )
{
	minMainWidget ->setDisabled(value);
}

void GlobalHotKey::openMusicfileDisabled( const bool &value )
{
	openMusicfile ->setDisabled(value);
}

void GlobalHotKey::showDownloadDisabled( const bool &value )
{
	showDownload ->setDisabled(value);
}

void GlobalHotKey::showMusicListDisabled( const bool &value )
{
	showMusicList ->setDisabled(value);
}

void GlobalHotKey::showLrcDisabled( const bool &value )
{
	showLrc ->setDisabled(value);
}

void GlobalHotKey::playPauseDisabled( const bool &value )
{
	playPause ->setDisabled(value);
}

void GlobalHotKey::lastDisabled( const bool &value )
{
	last ->setDisabled(value);
}

void GlobalHotKey::nextDisabled( const bool &value )
{
	nextSc ->setDisabled(value);
}
























