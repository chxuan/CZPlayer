/***************************************************************************
*   Copyright (C) 2012-2015 Highway-9 Studio.                             *
*   787280310@qq.com									                   *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
*   This program is distributed in the hope that it will be useful,       *
*   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
*   GNU General Public License for more details.                          *
*                                                                         *
*   You should have received a copy of the GNU General Public License     *
*   along with this program; if not, write to the                         *
*   Free Software Foundation, Inc.,                                       *
*   CUIT Highway-9 Studio, China.									       *
***************************************************************************/

/*!
* \file   MainWidget.h
* \author chengxuan   787280310@qq.com
* \date   2014-12-21
* \brief  播放器主界面头文件。
* \version 3.0.0
*
* \verbatim
* 历史
*          3.0.0   创建,
*          2014-12-21    by chengxuan
*
* \endverbatim
*
*/

#include <windows.h>
#include <time.h>

#include <QCoreApplication>
#include <QPushButton>
#include <QLabel>
#include <QMenu>
#include <QAction>
#include <QSlider>
#include <QTimer>
#include <QSystemTrayIcon>
#include <QDesktopServices>
#include <QDesktopWidget>
#include <QPropertyAnimation>
#include <QLinearGradient>
#include <QFileDialog>
#include <QTime>
#include <QMessageBox>
#include <QDropEvent>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QContextMenuEvent>
#include <QDragEnterEvent>
#include <QList>
#include <QUrl>
#include <QHeaderView>
#include <QMimeData>
#include <QListWidget>
#include <QBitmap>
#include <QPainter>
#include <QProcess>
#include <QMediaPlayer>
#ifdef CZ_DEBUG
#include <QDebug>
#endif

#include "MainWidget.h"
#include "DBModule.h"
#include "SoundControl.h"
#include "AlbumThread.h"
#include "LrcThread.h"
#include "LoadMusicThread.h"
#include "BasicPlayer.h"
#include "SpectroGraph.h"
#include "MediaWidget.h"
#include "MusicList.h"
#include "MusicLrc.h"
#include "MinMusicWidget.h"
#include "MusicListWidget.h"
#include "ConfigDialog.h"
#include "SharedDialog.h"
#include "AboutPanel.h"
#include "GlobalHotkey.h"
#include "GlobalConfig.h"
#include "Global.h"
#include "TR.h"
#include "AutoStartHandle.h"
#include "FileRelationHandle.h"

/*****************************************************************************
  这里用来检查版本的对应
 *****************************************************************************/
#if DBMODULE_VERSION < 0x030000
#error  "DBModule version error."
#endif

#if SOUNDCONTROL_VERSION < 0x030000
#error  "SoundControl version error."
#endif

#if ALBUMTHREAD_VERSION < 0x030000
#error  "AlbumThread version error."
#endif

#if SOUNDCONTROL_VERSION < 0x030000
#error  "SoundControl version error."
#endif

#if LRCTHREAD_VERSION < 0x030000
#error  "LrcThread version error."
#endif

#if LOADMUSICTHREAD_VERSION < 0x030000
#error  "LoadMusicThread version error."
#endif

#if BASICPLAYER_VERSION < 0x030000
#error  "BasicPlayer version error."
#endif

#if SPECTROGRAPH_VERSION < 0x030000
#error  "SpectroGraph version error."
#endif

#if MEDIAWIDGET_VERSION < 0x030000
#error  "MediaWidget version error."
#endif

#if MUSICLIST_VERSION < 0x030000
#error  "MusicList version error."
#endif

#if MUSICLRC_VERSION < 0x030000
#error  "MusicLrc version error."
#endif

#if MINMUSICWIDGET_VERSION < 0x030000
#error  "MinMusicWidget version error."
#endif

#if MUSICLISTWIDGET_VERSION < 0x030000
#error  "MusicListWidget version error."
#endif

#if CONFIGDIALOG_VERSION < 0x030000
#error  "ConfigDialog version error."
#endif

#if SHAREDDIALOG_VERSION < 0x030000
#error  "SharedDialog version error."
#endif

#if ABOUTPANEL_VERSION < 0x030000
#error  "AboutPanel version error."
#endif

#if GLOBALHOTKEY_VERSION < 0x030000
#error  "GlobalHotkey version error."
#endif

#if GLOBALCONFIG_VERSION < 0x030000
#error  "GlobalConfig version error."
#endif

#if GLOBAL_VERSION < 0x030000
#error  "Global version error."
#endif

#if TR_VERSION < 0x030000
#error  "TR version error."
#endif

#if AUTOSTARTHANDLE_VERSION < 0x030000
#error  "AutoStartHandle version error."
#endif

#if FILERELATIONHANDLE_VERSION < 0x030000
#error  "FileRelationHandle version error."
#endif

/*****************************************************************************
全局变量定义
*****************************************************************************/
const QString AppName = "CZPlayer";														/*!< 应用程序名*/
const QString WelecomeInfo = "Welecome to CZPlayer!";									/*!< 欢迎信息*/
const QString WelecomeUrl = TR("欢迎登陆%1").arg("http://www.qtcn.org/bbs/read-htm-tid-55824.html");	/*!< 欢迎Url*/
const QString AutoUpdatePath = "./AutoUpdate/AutoUpdate.exe";							/*!< 在线升级程序路径*/

/*! 得到音乐播放时间，目前仅支持mp3、wma、wav格式
*  \param szPath 音乐路径
*/
inline static unsigned long getMusicTime(WCHAR *szPath)
{
	MCIERROR mciError;
	MCI_OPEN_PARMS mciOpen;
	MCI_STATUS_PARMS mciStatusParms;

	mciOpen.lpstrElementName = szPath;
	mciError = mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT, (DWORD)(LPVOID)&mciOpen);
	if (mciError)
	{
		return 0;
	}
	/*Else do nothing, and continue.*/

	mciStatusParms.dwItem = MCI_STATUS_LENGTH;
	mciSendCommand(mciOpen.wDeviceID, MCI_STATUS, MCI_WAIT | MCI_STATUS_ITEM, (DWORD)(LPVOID)&mciStatusParms);
	mciSendCommand(mciOpen.wDeviceID, MCI_CLOSE, 0, NULL);

	return mciStatusParms.dwReturn / 1000;
}

MainWidget::MainWidget(const char *argvMusicPath, QWidget *parent)
	: QWidget(parent),
	m_minMusicWidget(NULL),
	m_playList(NULL),
	m_musicListWidget(NULL),
	m_lrc(NULL),
    m_mediaWidget(NULL),
	m_configDialog(NULL),
	m_sharedDialog(NULL),
	m_aboutPanel(NULL),
	m_globalHotKey(NULL),
	m_loadMusicThread(NULL),
	m_loadMusicThreadEvent(NULL),
    m_autoStartHandle(NULL),
    m_fileRelationHandle(NULL),
	m_basicPlayer(NULL),
	m_spectrograph(NULL),
	m_isMute(false),
	m_musicListShowFlag(false),
	m_musicDownloadShowFlag(false),
	m_currentFilePath(""),
	m_currentMusicName(""),
	m_currentMusicTime("00:00"),
	m_currentMusicRow(0),
	m_time(0),
	m_pos(170),
	m_isStarupLoadMusic(true),
	m_state(NoState)
{
	m_argvMusicPath = argvMusicPath;			//启动播放器的参数
	this->changeWorkDir();						//改变工作目录
	qsrand(time(NULL));							//初始化随机数种子
	DBModule::initDB();							//初始化数据库
	this->createUI();							//创建界面
	this->createConnect();						//信号与槽
	this->playGreetings();						//播放问候语
	this->createMusicloadThread();				//创建音乐列表加载线程
}

MainWidget::~MainWidget()
{
	this->releaseResource();
}

void MainWidget::createUI()
{
	//设置窗口基本属性
	this->resize(400, 212);									//设置窗体大小
	this->setWindowFlags(Qt::FramelessWindowHint);			//去掉窗体边框
	this->setAttribute(Qt::WA_TranslucentBackground);		//设置背景透明
	this->setWindowIcon(QIcon(ImagesDir + "CZPlayer.png"));	//设置logo
	this->setAcceptDrops(true);								//可以拖放文件
	this->setWindowTitle(AppName);

	//专辑封面
	m_albumImage = new QLabel(this);
	m_albumImage->setToolTip(TR("专辑封面"));
	m_albumImage->setPixmap(QPixmap(ImagesDir + "albumImage.png"));

	//信息显示标签
	m_infoLabel = new QLabel(this);
	m_infoLabel->setObjectName("m_infoLabel");
	m_infoLabel->setText(WelecomeInfo);

	//当前播放歌曲
	m_currentMusicLabel = new QLabel(this);
	m_currentMusicLabel->setObjectName("m_currentMusicLabel");
	m_currentMusicLabel->setText(WelecomeInfo);

	//频谱界面
	if (m_spectrograph == NULL)
	{
		m_spectrograph = new Spectrograph(this);
		m_spectrograph->setParams(0.0, 1000.0);	//初始化频谱参数
	}
	/*Else do nothing, and continue.*/

	//创建控制播放进度的滑块
	m_seekSlider = new QSlider(Qt::Horizontal, this);
	m_seekSlider->setObjectName("m_seekSlider");

	//创建音量调节滑块
	m_volumeSlider = new QSlider(Qt::Horizontal, this);
	m_volumeSlider->setObjectName("m_volumeSlider");
	m_volumeSlider->setToolTip(TR("音量调节"));
	m_volumeSlider->setRange(0, 100);

	//音量调节按钮
	m_volumeButton = new QPushButton(this);
	m_volumeButton->setObjectName("m_volumeButton");

	//播放模式按钮
	m_modeButton = new QPushButton(this);
	m_modeButton->setObjectName("m_modeButton");

	//设置显示时间标签
	m_timeLabel = new QLabel("00:00/00:00", this);
	m_timeLabel->setToolTip(TR("当前时间/总时间"));
	m_timeLabel->setObjectName("m_timeLabel");
	m_timeLabel->setAlignment(Qt::AlignCenter);
	m_timeLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    //显示媒体页面
	m_showButton = new QPushButton(this);
	m_showButton->setObjectName("m_showButton");
    m_showButton->setToolTip(TR("打开媒体面板"));
	m_showButton->setStyleSheet(QString("QPushButton:!hover{border-image: url(%1showButton.png);}"
		"QPushButton:hover{border-image: url(%1showButton2.png);}"
		"QPushButton:pressed{border-image: url(%1showButton3.png);}").arg(ImagesDir));

	//播放
	m_playButton = new QPushButton(this);
	m_playButton->setToolTip(TR("播放"));
	m_playButton->setObjectName("m_playButton");
	m_playButton->setStyleSheet(QString("QPushButton:!hover{border-image: url(%1playButton.png);}"
		"QPushButton:hover{border-image: url(%1playButton2.png);}"
		"QPushButton:pressed{border-image: url(%1playButton3.png);}").arg(ImagesDir));

	//停止
	m_endButton = new QPushButton(this);
	m_endButton->setToolTip(TR("停止"));
	m_endButton->setObjectName("m_endButton");

	//上一首
	m_preButton = new QPushButton(this);
	m_preButton->setToolTip(TR("上一首"));
	m_preButton->setObjectName("m_preButton");

	//下一首
	m_nextButton = new QPushButton(this);
	m_nextButton->setToolTip(TR("下一首"));
	m_nextButton->setObjectName("m_nextButton");

	//打开文件
	m_openFileButton = new QPushButton(this);
	m_openFileButton->setToolTip(TR("打开文件"));
	m_openFileButton->setObjectName("m_openFileButton");

	//打开播放列表
	m_musicListButton = new QPushButton(this);
	m_musicListButton->setToolTip(TR("打开播放列表"));
	m_musicListButton->setObjectName("m_musicListButton");

	//桌面歌词
	m_lrcButton = new QPushButton(this);
	m_lrcButton->setToolTip(TR("桌面歌词"));
	m_lrcButton->setObjectName("m_lrcButton");

	//主界面退出
	m_closeAction = new QAction(TR("退出"), this);
	m_closeAction->setIcon(QIcon(ImagesDir + "quitAction.png"));

	//主界面最小化
	m_minAction = new QAction(TR("最小化"), this);
	m_minAction->setIcon(QIcon(ImagesDir + "minAction.png"));

	//推盘界面退出程序
	m_quitAction = new QAction(TR("退出"), this);
	m_quitAction->setIcon(QIcon(ImagesDir + "quitAction.png"));

	//暂停(播放)
	m_pauseAction = new QAction(TR("播放"), this);
	m_pauseAction->setIcon(QIcon(ImagesDir + "trayPlay.png"));

	//停止
	m_stopAction = new QAction(TR("停止"), this);
	m_stopAction->setIcon(QIcon(ImagesDir + "trayStop.png"));

	//上一首
	m_lastAction = new QAction(TR("上一首"), this);
	m_lastAction->setIcon(QIcon(ImagesDir + "trayLast.png"));

	//下一首
	m_nextAction = new QAction(TR("下一首"), this);
	m_nextAction->setIcon(QIcon(ImagesDir + "trayNext.png"));

	//解锁歌词
	m_unlockLrcAction = new QAction(TR("解锁歌词"), this);
	m_unlockLrcAction->setIcon(QIcon(ImagesDir + "unlock.png"));

	//打开音乐文件
	m_openFileAction = new QAction(TR("打开音乐文件"), this);
	m_openFileAction->setIcon(QIcon(ImagesDir + "addMusic.png"));

	//打开播放列表
	m_openMusicListAction = new QAction(TR("打开播放列表"), this);
	m_openMusicListAction->setIcon(QIcon(ImagesDir + "openMusicList.png"));

    //打开媒体面板
    m_openMediaWidgetAction = new QAction(TR("打开媒体面板"), this);
    m_openMediaWidgetAction->setIcon(QIcon(ImagesDir + "openMusicDownload.png"));

	//显示桌面歌词
	m_openLrcAction = new QAction(TR("显示桌面歌词"), this);
	m_openLrcAction->setIcon(QIcon(ImagesDir + "lrc.png"));

	//自动升级
	m_autoUpdateAction = new QAction(TR("在线升级"), this);
	m_autoUpdateAction->setIcon(QIcon(ImagesDir + "updateButton.png"));

	//设置
	m_configAction = new QAction(TR("设置"), this);
	m_configAction->setIcon(QIcon(ImagesDir + "configButton.png"));

	//关于Qt
	m_aboutQtAction = new QAction(TR("关于Qt"), this);
	m_aboutQtAction->setIcon(QIcon(ImagesDir + "Qt.png"));

	//关于
	m_aboutAction = new QAction(TR("关于CZPlayer"), this);
	m_aboutAction->setIcon(QIcon(ImagesDir + "CZPlayer2.png"));

	//循环播放
	m_cycleAction = new QAction(this);
	m_cycleAction->setIcon(QIcon(ImagesDir + "cyclePlay.png"));

	//随机播放
	m_randomAction = new QAction(this);
	m_randomAction->setIcon(QIcon(ImagesDir + "randomPlay.png"));

	//创建系统托盘图标
	m_trayIcon = new QSystemTrayIcon(QIcon(ImagesDir + "CZPlayer.png"), this);
	m_trayIcon->show();
	m_trayIcon->showMessage(AppName, WelecomeUrl);
	m_trayIcon->setToolTip(AppName);

	//创建菜单
	this->createMenus();

	//小界面播放器
	if (m_minMusicWidget == NULL)
	{
		m_minMusicWidget = new MinMusicWidget(this, NULL);
	}
	/*Else do nothing, and continue.*/

	//播放列表
	if (m_playList == NULL)
	{
		m_playList = new MusicList(this);
	}
	/*Else do nothing, and continue.*/

	//播放列表
	if (m_musicListWidget == NULL)
	{
		m_musicListWidget = new MusicListWidget(m_playList, &m_mapMusicRows, this);
	}
	/*Else do nothing, and continue.*/

	//歌词
	if (m_lrc == NULL)
	{
		m_lrc = new MusicLrc;
	}
	/*Else do nothing, and continue.*/

    //媒体面板
    if (m_mediaWidget == NULL)
	{
        m_mediaWidget = new MediaWidget(this);
	}
	/*Else do nothing, and continue.*/

	//设置界面
	if (m_configDialog == NULL)
	{
		m_configDialog = new ConfigDialog(this);
	}
	/*Else do nothing, and continue.*/

	//分享界面
	if (m_sharedDialog == NULL)
	{
		m_sharedDialog = new SharedDialog(this);
	}
	/*Else do nothing, and continue.*/

	//关于CZPlayer
	if (m_aboutPanel == NULL)
	{
		m_aboutPanel = new AboutPanel(this);
	}
	/*Else do nothing, and continue.*/

	//全局热键设置
	if (m_globalHotKey == NULL)
	{
		m_globalHotKey = new GlobalHotKey(this);
	}
	/*Else do nothing, and continue.*/

    //自启动处理器
    if (m_autoStartHandle == NULL)
    {
        m_autoStartHandle = new AutoStartHandle;
    }
    /*Else do nothing, and continue.*/

    //文件关联处理器
    if (m_fileRelationHandle == NULL)
    {
        m_fileRelationHandle = new FileRelationHandle;
    }
    /*Else do nothing, and continue.*/

	//设置一个定时器
	m_timer = new QTimer(this);
	m_timer->start(30);
	m_upDateTimer = new QTimer(this);
	m_upDateTimer->start(1000);

	//设置组件大小和位置
	m_albumImage->setGeometry(25, 23, 110, 103);
	m_spectrograph->setGeometry(155, 47, 190, 78);
	m_infoLabel->setGeometry(153, 69, 240, 39);
	m_timeLabel->setGeometry(295, 23, 82, 18);
	m_currentMusicLabel->setGeometry(170, 3, 500, 15);
	m_showButton->setGeometry(365, 55, 17, 17);
	m_modeButton->setGeometry(365, 80, 18, 15);

	m_preButton->setGeometry(33, 164, 25, 25);
	m_playButton->setGeometry(83, 164, 25, 25);
	m_endButton->setGeometry(133, 164, 25, 25);
	m_nextButton->setGeometry(183, 164, 25, 25);

	m_seekSlider->setGeometry(25, 129, 361, 30);
	m_openFileButton->setGeometry(152, 21, 49, 23);
	m_musicListButton->setGeometry(201, 20, 35, 24);
	m_lrcButton->setGeometry(236, 21, 44, 23);
	m_volumeButton->setGeometry(249, 168, 16, 16);
	m_volumeSlider->setGeometry(277, 161, 108, 30);
}

void MainWidget::createMenus()
{
	//创建菜单，系统托盘图标右击出现的菜单
	m_trayMenu = new QMenu(this);
	m_trayMenu->addAction(m_quitAction);
	m_trayMenu->addAction(m_unlockLrcAction);
	m_trayMenu->addSeparator();
	m_trayMenu->addAction(m_pauseAction);
	m_trayMenu->addAction(m_stopAction);
	m_trayMenu->addSeparator();
	m_trayMenu->addAction(m_lastAction);
	m_trayMenu->addAction(m_nextAction);
	m_trayIcon->setContextMenu(m_trayMenu);//在系统托盘上添加菜单
	m_trayIcon->show();

	//右键菜单
	m_rightHandMenu = new QMenu(this);
	m_rightHandMenu->addAction(m_closeAction);
	m_rightHandMenu->addAction(m_minAction);
	m_rightHandMenu->addAction(QIcon(ImagesDir + "minMainWidget.png"), TR("迷你模式"), this, SLOT(handleOpenMinWidget()));
	m_rightHandMenu->addSeparator();
	m_rightHandMenu->addAction(m_openFileAction);
	m_rightHandMenu->addAction(m_openMusicListAction);
    m_rightHandMenu->addAction(m_openMediaWidgetAction);
	m_rightHandMenu->addAction(m_openLrcAction);
	m_rightHandMenu->addSeparator();
	m_rightHandMenu->addAction(m_autoUpdateAction);
	m_rightHandMenu->addAction(m_configAction);
	m_rightHandMenu->addAction(m_aboutAction);
	m_rightHandMenu->addAction(m_aboutQtAction);

	//播放模式菜单
	m_playModeMenu = new QMenu(this);
	m_playModeMenu->addAction(m_cycleAction);
	m_playModeMenu->addAction(m_randomAction);
}

void MainWidget::createConnect()
{
	//注册类型
	qRegisterMetaType<vector<float>>("vector<float>");

	connect(m_openFileButton, SIGNAL(clicked()), this, SLOT(handleOpenFile()));
	connect(m_musicListButton, SIGNAL(clicked()), this, SLOT(handleOpenMusicList()));
	connect(m_lrcButton, SIGNAL(clicked()), this, SLOT(handleOpenMusicLrc()));
    connect(m_showButton, SIGNAL(clicked()), this, SLOT(handleOpenMediaWidget()));
	connect(m_trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(handleTrayIconActivated(QSystemTrayIcon::ActivationReason)));
	connect(m_playList, SIGNAL(cellDoubleClicked(int, int)), this, SLOT(handleTableDoubleClicked(int, int)));
	connect(m_playList, SIGNAL(sigPlayListCleanFinshed()), this, SLOT(handleClearSources()));
	connect(m_playList, SIGNAL(sigRowSelected(int, QString)), this, SLOT(handleDelSelectSource(int, QString)));
	connect(m_playList, SIGNAL(sigSharedCurrentMusic(QString)), this, SLOT(handleSharedCurrentMusic(QString)));
	connect(m_lrc, SIGNAL(sigLrcLocked()), this, SLOT(handleShowLrcLocked()));
	connect(m_lrc, SIGNAL(sigLrcUnlocked()), this, SLOT(handleShowLrcUnlocked()));
    connect(m_lrc, SIGNAL(sigHideLrc()), this, SLOT(handleOpenMusicLrc()));
	connect(m_unlockLrcAction, SIGNAL(triggered()), this, SLOT(handleUnlockLrc()));
	connect(m_playButton, SIGNAL(clicked()), this, SLOT(handleSetPlayPaused()));
	connect(m_endButton, SIGNAL(clicked()), this, SLOT(handleSetStop()));
	connect(m_preButton, SIGNAL(clicked()), this, SLOT(handleSetPre()));
	connect(m_nextButton, SIGNAL(clicked()), this, SLOT(handleSetNext()));
	connect(m_minAction, SIGNAL(triggered()), this, SLOT(handleMinToTray()));
	connect(m_closeAction, SIGNAL(triggered()), this, SLOT(handleCloseAllProgress()));
	connect(m_quitAction, SIGNAL(triggered()), this, SLOT(handleCloseAllProgress()));
	connect(m_pauseAction, SIGNAL(triggered()), this, SLOT(handleSetPlayPaused()));
	connect(m_stopAction, SIGNAL(triggered()), this, SLOT(handleSetStop()));
	connect(m_lastAction, SIGNAL(triggered()), this, SLOT(handleSetPre()));
	connect(m_nextAction, SIGNAL(triggered()), this, SLOT(handleSetNext()));
	connect(m_timer, SIGNAL(timeout()), this, SLOT(handleTimeOut()));
	connect(m_upDateTimer, SIGNAL(timeout()), this, SLOT(handleUpdateTime()));
	connect(m_openFileAction, SIGNAL(triggered()), this, SLOT(handleOpenFile()));
	connect(m_openMusicListAction, SIGNAL(triggered()), this, SLOT(handleOpenMusicList()));
    connect(m_openMediaWidgetAction, SIGNAL(triggered()), this, SLOT(handleOpenMediaWidget()));
	connect(m_openLrcAction, SIGNAL(triggered()), this, SLOT(handleOpenMusicLrc()));
	connect(m_autoUpdateAction, SIGNAL(triggered()), this, SLOT(handleAutoUpdate()));
	connect(m_configAction, SIGNAL(triggered()), this, SLOT(handleConfigCZPlayer()));
	connect(m_aboutAction, SIGNAL(triggered()), this, SLOT(handleAboutCZPlayer()));
	connect(m_aboutQtAction, SIGNAL(triggered()), this, SLOT(handleAboutQt()));
	connect(m_minMusicWidget, SIGNAL(sigShowMusicPanel()), this, SLOT(handleShowMusicPanel()));
	connect(this, SIGNAL(sigShowMainWidget()), m_minMusicWidget, SLOT(handleOpenMainWidget()));
	connect(m_volumeButton, SIGNAL(clicked()), this, SLOT(handleSetMute()));
	connect(m_volumeSlider, SIGNAL(sliderMoved(int)), this, SLOT(handleSetVolume(int)));
	connect(m_modeButton, SIGNAL(clicked()), this, SLOT(handleShowModeMenu()));
	connect(m_cycleAction, SIGNAL(triggered()), this, SLOT(handleCyclePlay()));
	connect(m_randomAction, SIGNAL(triggered()), this, SLOT(handleRandomPlay()));
	connect(this, SIGNAL(sigSpectrumChanged(vector<float>)), m_spectrograph, SLOT(handleSpectrumChanged(vector<float>)));
	connect(m_configDialog, SIGNAL(sigConfigDialogClosed()), this, SLOT(handleConfigDialogClosed()));

	GlobalConfig *config = GlobalConfig::instance();
	connect(config, SIGNAL(sigMusicPathChanged(QString)), this, SLOT(handleMusicDirChanged(QString)));
	connect(config, SIGNAL(sigAlbumPathChanged(QString)), this, SLOT(handleAlbumDirChanged(QString)));
	connect(config, SIGNAL(sigLrcPathChanged(QString)), this, SLOT(handleLrcDirChanged(QString)));
	connect(config, SIGNAL(sigMuteChanged(bool)), this, SLOT(handleMuteChanged(bool)));
	connect(config, SIGNAL(sigVolumeChanged(int)), this, SLOT(handleVolumeChanged(int)));
	connect(config, SIGNAL(sigPlayModeChanged(enum PlayModeType)), this, SLOT(handlePlayModeChanged(enum PlayModeType)));
	config->reload();
}

void MainWidget::playGreetings()
{
#if 0
	GlobalConfig *config = GlobalConfig::instance();
	if (config->autoGreetings())
	{
        QString filePath = "./CZPlayer.wav";
        if (QFile::exists(filePath))
        {
            QMediaPlayer *mediaPlayer = new QMediaPlayer;
            mediaPlayer->setMedia(QUrl::fromLocalFile(filePath));
            mediaPlayer->setVolume(100);
            mediaPlayer->play();
        }
        /*Else do nothing, and continue.*/
	}
	/*Else do nothing, and continue.*/
#endif
}

void MainWidget::createMusicloadThread()
{
	//创建音乐加载线程
	if (m_loadMusicThread == NULL)
	{
		m_loadMusicThread = new LoadMusicThread;
		m_loadMusicThread->start();

		if (m_loadMusicThreadEvent == NULL)
		{
			m_loadMusicThreadEvent = new LoadMusicThreadEvent;
			m_loadMusicThreadEvent->moveToThread(m_loadMusicThread);

			qRegisterMetaType<MusicInfoEntity>("MusicInfoEntity");
			qRegisterMetaType<QList<QUrl>>("QList<QUrl>");
			connect(this, SIGNAL(sigStartloadMusic()), m_loadMusicThreadEvent, SLOT(handleStartloadMusic()), Qt::QueuedConnection);
			connect(this, SIGNAL(sigStartloadMusic(QStringList)), m_loadMusicThreadEvent, SLOT(handleStartloadMusic(QStringList)), Qt::QueuedConnection);
			connect(this, SIGNAL(sigStartloadMusic(QString)), m_loadMusicThreadEvent, SLOT(handleStartloadMusic(QString)), Qt::QueuedConnection);
			connect(this, SIGNAL(sigStartloadMusic(QList<QUrl>)), m_loadMusicThreadEvent, SLOT(handleStartloadMusic(QList<QUrl>)), Qt::QueuedConnection);

			connect(m_loadMusicThreadEvent, SIGNAL(sigAddMusic(MusicInfoEntity)), this, SLOT(handleAddMusic(MusicInfoEntity)), Qt::QueuedConnection);
			connect(m_loadMusicThreadEvent, SIGNAL(sigLoadMusicListError()), this, SLOT(handleLoadMusicListError()), Qt::QueuedConnection);
			connect(m_loadMusicThreadEvent, SIGNAL(sigLoadMusicListFinished()), this, SLOT(handleLoadMusicListFinished()), Qt::QueuedConnection);

			emit sigStartloadMusic();
		}
		/*Else do nothing, and continue.*/
	}
	/*Else do nothing, and continue.*/
}

void MainWidget::releaseResource()
{
	this->reset();

	if (m_timer->isActive())
	{
		m_timer->stop();
	}
	/*Else do nothing, and continue.*/

	if (m_upDateTimer->isActive())
	{
		m_upDateTimer->stop();
	}
	/*Else do nothing, and continue.*/

    if (m_autoStartHandle != NULL)
    {
        m_autoStartHandle->deleteLater();
        m_autoStartHandle = NULL;
    }
    /*Else do nothing, and continue.*/

    if (m_fileRelationHandle != NULL)
    {
        m_fileRelationHandle->deleteLater();
        m_fileRelationHandle = NULL;
    }
    /*Else do nothing, and continue.*/

	this->terminateAllThread();

	this->savePlayControlSettingsInfo();

	if (m_globalHotKey != NULL)
	{
		m_globalHotKey->deleteLater();
		m_globalHotKey = NULL;
	}
	/*Else do nothing, and continue.*/

	if (m_spectrograph != NULL)
	{
		m_spectrograph->deleteLater();
		m_spectrograph = NULL;
	}
	/*Else do nothing, and continue.*/

	if (m_minMusicWidget != NULL)
	{
		m_minMusicWidget->deleteLater();
		m_minMusicWidget = NULL;
	}
	/*Else do nothing, and continue.*/

	if (m_playList != NULL)
	{
		m_playList->deleteLater();
		m_playList = NULL;
	}
	/*Else do nothing, and continue.*/

	if (m_musicListWidget != NULL)
	{
		m_musicListWidget->deleteLater();
		m_musicListWidget = NULL;
	}
	/*Else do nothing, and continue.*/

	if (m_lrc != NULL)
	{
		m_lrc->deleteLater();
		m_lrc = NULL;
	}
	/*Else do nothing, and continue.*/

    if (m_mediaWidget != NULL)
	{
        m_mediaWidget->deleteLater();
        m_mediaWidget = NULL;
	}
	/*Else do nothing, and continue.*/

	if (m_configDialog != NULL)
	{
		m_configDialog->deleteLater();
		m_configDialog = NULL;
	}
	/*Else do nothing, and continue.*/

	if (m_sharedDialog != NULL)
	{
		m_sharedDialog->deleteLater();
		m_sharedDialog = NULL;
	}
	/*Else do nothing, and continue.*/

	if (m_aboutPanel != NULL)
	{
		m_aboutPanel->deleteLater();
		m_aboutPanel = NULL;
	}
	/*Else do nothing, and continue.*/
}

void MainWidget::terminateAllThread()
{
	while (!m_albumThreads.isEmpty())
	{
		QThread *thread = m_albumThreads.takeFirst();
		if (thread->isRunning())
		{
			thread->terminate();
			thread->wait();
		}
		/*Else do nothing, and continue.*/
		thread->deleteLater();
		thread = NULL;
	}

	while (!m_lrcThreads.isEmpty())
	{
		QThread *thread = m_lrcThreads.takeFirst();
		if (thread->isRunning())
		{
			thread->terminate();
			thread->wait();
		}
		/*Else do nothing, and continue.*/
		thread->deleteLater();
		thread = NULL;
	}

	if (m_loadMusicThreadEvent != NULL)
	{
		m_loadMusicThreadEvent->deleteLater();
		m_loadMusicThreadEvent = NULL;
	}
	/*Else do nothing, and continue.*/

	if (m_loadMusicThread != NULL)
	{
		if (m_loadMusicThread->isRunning())
		{
			m_loadMusicThread->quit();
			m_loadMusicThread->wait();
		}
		/*Else do nothing, and continue.*/
		m_loadMusicThread->deleteLater();
		m_loadMusicThread = NULL;
	}
	/*Else do nothing, and continue.*/
}

void MainWidget::savePlayControlSettingsInfo()
{
	GlobalConfig *config = GlobalConfig::instance();
	config->setReleaseResource(true);
	config->setMute(m_isMute);
	config->setVolume(m_volumeSlider->value());
	if (m_mode == RandomPlay)
	{
		config->setPlayMode((enum PlayModeType)1);
	}
	else if (m_mode == CyclePlay)
	{
		config->setPlayMode((enum PlayModeType)2);
	}
	/*Else do nothing, and continue.*/
	if (!config->save())
	{
		QMessageBox::warning(this, TR("警告"), TR("保存播放控制配置信息失败！"));
	}
	/*Else do nothing, and contine.*/
	config->delInstance();
}

void MainWidget::handleTimeOut()
{
	if (m_lrc->text().isEmpty())
	{
		m_lrc->stopLrcMask();
	}
	/*Else do nothing, and continue.*/

	//如果播放列表为空，则播放按钮不能用
	if (m_playList->rowCount() == 0)
	{
		m_playButton->setEnabled(false);
		m_endButton->setEnabled(false);
		m_preButton->setEnabled(false);
		m_nextButton->setEnabled(false);

		m_minMusicWidget->setWork(false);

		m_pauseAction->setIcon(QIcon(ImagesDir + "trayPlay.png"));
		m_pauseAction->setText(TR("播放"));
		m_pauseAction->setEnabled(false);
		m_stopAction->setEnabled(false);
		m_lastAction->setEnabled(false);
		m_nextAction->setEnabled(false);
	}
	else
	{
		m_playButton->setEnabled(true);
		m_endButton->setEnabled(true);
		m_preButton->setEnabled(true);
		m_nextButton->setEnabled(true);

		m_minMusicWidget->setWork(true);

		m_pauseAction->setEnabled(true);
		m_stopAction->setEnabled(true);
		m_lastAction->setEnabled(true);
		m_nextAction->setEnabled(true);
	}

	if (m_pos == -500)
	{
		m_pos = 400;
	}
	/*Else do nothing, and continue.*/

	m_currentMusicLabel->setGeometry(m_pos, 3, 500, 15);
	m_pos = m_pos - 1;
}

void MainWidget::handleAutoUpdate()
{
	unsigned long processId = GetCurrentProcessId();
	QStringList idList;
	idList << QString::number(processId);
	
	if (!QProcess::startDetached(AutoUpdatePath, idList))
	{
		QMessageBox::information(this, TR("信息"), TR("启动在线升级程序失败，请确认程序路径是否正确！"), QMessageBox::Yes);
	}
	/*Else do nothing, and continue.*/
}

void MainWidget::handleConfigOptions()
{
	if (m_configDialog->isHidden())
	{
		this->handleConfigCZPlayer();
	}
	/*Else do nothing, and continue.*/
}

void MainWidget::handleShowHideMainWidget()
{
	this->handleMinToTray();
	if (this->isHidden())
	{
		this->handleTrayIconActivated(QSystemTrayIcon::Trigger);
	}
	/*Else do nothing, and continue.*/
}

void MainWidget::handleMinMainWidget()
{
	this->handleOpenMinWidget();
	if (this->isHidden())
	{
		emit sigShowMainWidget();
	}
	/*Else do nothing, and continue.*/
}

void MainWidget::handleOpenFile()
{
	QStringList list = QFileDialog::getOpenFileNames(this, TR("打开音乐文件"),
		m_musicPath, "Music Files(*.mp3 *.wma *.wav *.ogg)");
	if (list.isEmpty())
	{
		return;
	}
	/*Else do nothing, and continue.*/

	emit sigStartloadMusic(list);
}

void MainWidget::handleAddMusicList()
{
	QString fileName = QFileDialog::getOpenFileName(this, TR("载入播放列表"),
		m_musicPath, "Music List(*.m3u *.pls)");
	if (!fileName.isEmpty())
	{
		emit sigStartloadMusic(fileName);
	}
	/*Else do nothing, and continue.*/
}

void MainWidget::handleOpenMinWidget()
{
	if (m_minMusicWidget->isHidden())
	{
		QPropertyAnimation *minWidgetdAnimation = new QPropertyAnimation(m_minMusicWidget, "windowOpacity");
		minWidgetdAnimation->setDuration(500);
		minWidgetdAnimation->setStartValue(0);
		minWidgetdAnimation->setEndValue(1);
		minWidgetdAnimation->start();
		m_minMusicWidget->show();

		QPropertyAnimation *mainWidgetdAnimation = new QPropertyAnimation(this, "windowOpacity");
		mainWidgetdAnimation->setDuration(500);
		mainWidgetdAnimation->setStartValue(1);
		mainWidgetdAnimation->setEndValue(0);
		mainWidgetdAnimation->start();
		connect(mainWidgetdAnimation, SIGNAL(finished()), this, SLOT(handleHideMainWidget()));

        QPropertyAnimation *mediaWidgetAnimation = new QPropertyAnimation(m_mediaWidget, "windowOpacity");
        mediaWidgetAnimation->setDuration(500);
        mediaWidgetAnimation->setStartValue(1);
        mediaWidgetAnimation->setEndValue(0);
        mediaWidgetAnimation->start();
        connect(mediaWidgetAnimation, SIGNAL(finished()), this, SLOT(handleHideMediaWidget()));

		QPropertyAnimation *musicListAnimation = new QPropertyAnimation(m_musicListWidget, "windowOpacity");
		musicListAnimation->setDuration(500);
		musicListAnimation->setStartValue(1);
		musicListAnimation->setEndValue(0);
		musicListAnimation->start();
		connect(musicListAnimation, SIGNAL(finished()), this, SLOT(handleHideMusicList()));

		//在显示迷你界面的时候，禁用以下热键
		m_globalHotKey->setShowHideMainWidgetHotkeyWork(false);
		m_globalHotKey->setShowDownloadHotkeyWork(false);
		m_globalHotKey->setShowMusicListHotkeyWork(false);
	}
	else
	{
		//在主界面的时候，启用以下热键
		m_globalHotKey->setShowHideMainWidgetHotkeyWork(true);
		m_globalHotKey->setShowDownloadHotkeyWork(true);
		m_globalHotKey->setShowMusicListHotkeyWork(true);
	}
}

void MainWidget::handleOpenMusicList()
{
	if (m_musicListWidget->isHidden())
	{
		m_musicListWidget->move(frameGeometry().bottomLeft());
		QPropertyAnimation *musicListAnimation = new QPropertyAnimation(m_musicListWidget, "windowOpacity");
		musicListAnimation->setDuration(500);
		musicListAnimation->setStartValue(0);
		musicListAnimation->setEndValue(1);
		musicListAnimation->start();
		m_musicListWidget->show();
		m_openMusicListAction->setText(TR("关闭播放列表"));
		m_musicListShowFlag = true;
	}
	else
	{
		QPropertyAnimation *musicListAnimation = new QPropertyAnimation(m_musicListWidget, "windowOpacity");
		musicListAnimation->setDuration(500);
		musicListAnimation->setStartValue(1);
		musicListAnimation->setEndValue(0);
		musicListAnimation->start();
		connect(musicListAnimation, SIGNAL(finished()), this, SLOT(handleHideMusicList()));
		m_openMusicListAction->setText(TR("打开播放列表"));
		m_musicListShowFlag = false;
	}
}

void MainWidget::handleOpenMusicLrc()
{
	if (m_lrc->isHidden())
	{
		m_lrc->showNormal();
		m_openLrcAction->setText(TR("关闭桌面歌词"));
		m_minMusicWidget->setMinMusicLrcActionText(TR("关闭桌面歌词"));
	}
	else
	{
		m_lrc->hide();
		m_openLrcAction->setText(TR("显示桌面歌词"));
		m_minMusicWidget->setMinMusicLrcActionText(TR("显示桌面歌词"));
	}
}

void MainWidget::handleOpenMediaWidget()
{
    if (m_mediaWidget->isHidden())
	{
        m_mediaWidget->move(frameGeometry().topRight());
        QPropertyAnimation *mediaWidgetAnimation = new QPropertyAnimation(m_mediaWidget, "windowOpacity");
        mediaWidgetAnimation->setDuration(500);
        mediaWidgetAnimation->setStartValue(0);
        mediaWidgetAnimation->setEndValue(1);
        mediaWidgetAnimation->start();
        m_mediaWidget->show();
        m_openMediaWidgetAction->setText(TR("关闭媒体面板"));
        m_showButton->setToolTip(TR("关闭媒体面板"));
		m_showButton->setStyleSheet(QString("QPushButton:!hover{border-image: url(%1hideButton.png);}"
			"QPushButton:hover{border-image: url(%1hideButton2.png);}"
			"QPushButton:pressed{border-image: url(%1hideButton3.png);}").arg(ImagesDir));
		m_musicDownloadShowFlag = true;
	}
	else
	{
        QPropertyAnimation *mediaWidgetAnimation = new QPropertyAnimation(m_mediaWidget, "windowOpacity");
        mediaWidgetAnimation->setDuration(500);
        mediaWidgetAnimation->setStartValue(1);
        mediaWidgetAnimation->setEndValue(0);
        mediaWidgetAnimation->start();
        connect(mediaWidgetAnimation, SIGNAL(finished()), this, SLOT(handleHideMediaWidget()));
        m_openMediaWidgetAction->setText(TR("打开媒体面板"));
        m_showButton->setToolTip(TR("打开媒体面板"));
		m_showButton->setStyleSheet(QString("QPushButton:!hover{border-image: url(%1showButton.png);}"
			"QPushButton:hover{border-image: url(%1showButton2.png);}"
			"QPushButton:pressed{border-image: url(%1showButton3.png);}").arg(ImagesDir));
		m_musicDownloadShowFlag = false;
	}
}

void MainWidget::handleHideMediaWidget()
{
    m_mediaWidget->hide();
}

void MainWidget::handleHideMusicList()
{
	m_musicListWidget->hide();
}

void MainWidget::handleHideMainWidget()
{
	this->hide();
}

void MainWidget::handleShowMusicPanel()
{
	if (m_musicDownloadShowFlag)
	{
        this->handleOpenMediaWidget();
	}
	/*Else do nothing, and continue.*/

	if (m_musicListShowFlag)
	{
		this->handleOpenMusicList();
	}
	/*Else do nothing, and continue.*/

	//在主界面的时候，启用以下热键
	m_globalHotKey->setShowHideMainWidgetHotkeyWork(true);
	m_globalHotKey->setShowDownloadHotkeyWork(true);
	m_globalHotKey->setShowMusicListHotkeyWork(true);
}

void MainWidget::handleConfigCZPlayer()
{
	m_globalHotKey->setConfigOptionsHotkeyWork(false);
	m_globalHotKey->setShowHideMainWidgetHotkeyWork(false);
	m_globalHotKey->setMinMainWidgetHotkeyWork(false);
	m_globalHotKey->setOpenMusicfileHotkeyWork(false);
	m_globalHotKey->setShowDownloadHotkeyWork(false);
	m_globalHotKey->setShowMusicListHotkeyWork(false);
	m_globalHotKey->setShowLrcHotkeyWork(false);
	m_globalHotKey->setPlayPauseHotkeyWork(false);
	m_globalHotKey->setLastHotkeyWork(false);
	m_globalHotKey->setNextHotkeyWork(false);

	m_configDialog->init();
	m_configDialog->exec();
}

void MainWidget::handleAboutQt()
{
	QMessageBox::aboutQt(this, TR("关于Qt"));
}

void MainWidget::handleAboutCZPlayer()
{
	m_aboutPanel->init();
	m_aboutPanel->exec();
}

void MainWidget::resolveLrc(const QString &lrcFilePath)
{
	m_lrcMap.clear();

	//打开歌词
	QFile file(lrcFilePath);
	if (!file.open(QIODevice::ReadOnly))
	{
		file.close();
		m_lrc->setText(m_currentMusicName + TR("----正在下载歌词文件!"));

		if (m_currentMusicName.contains('-'))
		{
			QStringList list = m_currentMusicName.split('-');
			QString musicName = list.at(1).trimmed();//歌曲名
			QString musicArtist = list.at(0).trimmed();//歌手
			//下载歌词
			this->downloadLrc(musicName, musicArtist, lrcFilePath);
			return;
		}
		/*Else do nothing, and continue.*/
	}
	/*Else do nothing, and continue.*/

	/*解析歌词*/
	QTextStream in(&file);
	QString allText = in.readAll();
	file.close();

	//将歌词安行分为歌词列表
	QStringList lrcLines = allText.split('\n');

	//这个是时间标签的格式[00:05.54]
	//正则表达式\d{2}表示匹配两个数字
	QRegExp rx("\\[\\d{2}:\\d{2}\\.\\d{2}\\]");

	foreach(QString oneLine, lrcLines)
	{
		QString temp = oneLine;
		temp.replace(rx, "");//用空字符串替换正则表达式中所匹配的地方,这样就获得了歌词文本

		// 然后依次获取当前行中的所有时间标签，并分别与歌词文本存入QMap中
		//indexIn()为返回第一个匹配的位置，如果返回为-1，则表示没有匹配成功
		//正常情况下pos后面应该对应的是歌词文件

		int pos = rx.indexIn(oneLine, 0);
		while (pos != -1)//表示匹配成功
		{
			QString cap = rx.cap(0);//返回第0个表达式匹配的内容

			//将时间标签转换为时间数值，以毫秒为单位
			QRegExp regexp;

			//获得分钟
			regexp.setPattern("\\d{2}(?=:)");
			regexp.indexIn(cap);
			int minute = regexp.cap(0).toInt();

			//获得秒
			regexp.setPattern("\\d{2}(?=\\.)");
			regexp.indexIn(cap);
			int second = regexp.cap(0).toInt();

			//获得毫秒
			regexp.setPattern("\\d{2}(?=\\])");
			regexp.indexIn(cap);
			int milliseSecond = regexp.cap(0).toInt();

			//获得总时间
			long long totalTime = minute * 60000 + second * 1000 + milliseSecond * 10;

			//把每一行的时间和歌词插入到m_lrcMap中
			m_lrcMap.insert(totalTime, temp);
			pos += rx.matchedLength();
			pos = rx.indexIn(oneLine, pos);//匹配全部
		}
	}

	if (m_lrcMap.isEmpty())
	{
		m_lrc->setText(m_currentMusicName + TR("----歌词文件内容错误!"));
		return;
	}
	/*Else do nothing, and continue.*/
}

void MainWidget::handleSetPlayPaused()
{
    if (m_state == MainWidget::Play)
	{
		this->pause();
		this->setState(Pause);
        this->stateChanged();
	}
    else if (m_state == MainWidget::Pause)
	{
		this->play();
		this->setState(Play);
        this->stateChanged();
	}
    else if (m_state == MainWidget::Stop)
	{
		this->reset();
        if (QFile::exists(m_currentFilePath))
		{
			this->play(m_currentFilePath);
			this->setState(Play);
            this->stateChanged();
		}
		else
		{
			QMessageBox::information(this, TR("信息"), TR("无效歌曲路径！"), QMessageBox::Yes);
			return;
		}
	}
    else if (m_state == MainWidget::NoState)
	{
		int row = 0;
		//随机播放
        if (m_mode == MainWidget::RandomPlay)
		{
			row = qrand() % m_playList->rowCount();
		}
        /*Els do nothing, and continue.*/
		QTableWidgetItem *item = m_playList->item(row, 1);
		this->findAndPlay(item->text(), row);
	}
    /*Els do nothing, and continue.*/
}

void MainWidget::handleSetPlay()
{
    if (m_state == MainWidget::Pause)
	{
		this->play();
		this->setState(Play);
        this->stateChanged();
	}
    else if (m_state == MainWidget::Stop)
	{
		this->reset();
        if (QFile::exists(m_currentFilePath))
		{
			this->play(m_currentFilePath);
			this->setState(Play);
            this->stateChanged();
		}
		else
		{
			QMessageBox::information(this, TR("信息"), TR("无效歌曲路径！"), QMessageBox::Yes);
			return;
		}
	}
    else if (m_state == MainWidget::NoState)
	{
		int row = 0;
		//随机播放
        if (m_mode == MainWidget::RandomPlay)
		{
			row = qrand() % m_playList->rowCount();
		}
        /*Else do nothing, and continue.*/
		QTableWidgetItem *item = m_playList->item(row, 1);
		this->findAndPlay(item->text(), row);
	}
    /*Else do nothing, and continue.*/
}

void MainWidget::handleSetPause()
{
    if (m_state == MainWidget::Play)
	{
		this->pause();
		this->setState(Pause);
        this->stateChanged();
	}
	/*Else do nothing, and continue.*/
}

void MainWidget::handleSetStop()
{
    if (m_state == MainWidget::Play || m_state == MainWidget::Pause)
	{
		this->reset();
		this->setState(Stop);
		this->stateChanged();
	}
	/*Else do nothing, and continue.*/
}

void MainWidget::handleSetPre()
{
	//循环播放模式
    if (m_mode == MainWidget::CyclePlay)
	{
		int row = m_currentMusicRow - 1;
		if (row != -1)
		{
			QTableWidgetItem *item = m_playList->item(row, 1);
			this->findAndPlay(item->text(), row);
		}
		else	//播放最后一首
		{
			row = m_playList->rowCount() - 1;
			QTableWidgetItem *item = m_playList->item(row, 1);
			this->findAndPlay(item->text(), row);
		}
	}
	//随机播放
    else if (m_mode == MainWidget::RandomPlay)
	{
		int row = qrand() % m_playList->rowCount();
		QTableWidgetItem *item = m_playList->item(row, 1);
		this->findAndPlay(item->text(), row);
	}
	/*Else do nothing, and continue.*/
}

void MainWidget::handleSetNext()
{
	//循环播放模式
    if (m_mode == MainWidget::CyclePlay)
	{
		int row = m_currentMusicRow + 1;
		if (row != m_playList->rowCount())
		{
			QTableWidgetItem *item = m_playList->item(row, 1);
			this->findAndPlay(item->text(), row);
		}
		else	//重新循环列表
		{
			QTableWidgetItem *item = m_playList->item(0, 1);
			this->findAndPlay(item->text(), 0);
		}
	}
	//随机播放
    else if (m_mode == MainWidget::RandomPlay)
	{
		int row = qrand() % m_playList->rowCount();
		QTableWidgetItem *item = m_playList->item(row, 1);
		this->findAndPlay(item->text(), row);
	}
	/*Else do nothing, and continue.*/
}

void MainWidget::handleTableDoubleClicked(const int &row, const int &column)
{
	(void)column;
	QTableWidgetItem *item = m_playList->item(row, 1);
	this->findAndPlay(item->text(), row);
}

void MainWidget::rowChanged()
{
	m_playList->selectRow(m_currentMusicRow);
}

void MainWidget::handleClearSources()
{
	//停止当前播放
	this->handleSetStop();

	if (!DBModule::clearDB())//清空数据库
	{
		QMessageBox::information(this, TR("信息"), TR("清空数据库失败！"), QMessageBox::Yes);
	}
	/*Else do nothing, and continue.*/

	m_albumImage->setPixmap(QPixmap(ImagesDir + "albumImage.png"));
	m_minMusicWidget->changeAlbumImage(ImagesDir + "albumImage.png");
	m_currentMusicLabel->setText(WelecomeInfo);
	m_mapMusicRows.clear();
	m_mapMusicFilePath.clear();

	//删除专辑文件夹里面的全部专辑
	QDir dir(m_albumPath);
	QFileInfoList fileInfoList = dir.entryInfoList(QDir::Files | QDir::Hidden | QDir::NoDotAndDotDot | QDir::NoSymLinks);
	foreach(QFileInfo fileInfo, fileInfoList)
	{
		if (fileInfo.isFile())
		{
			QFile file(fileInfo.absoluteFilePath());
			file.remove();
		}
		/*Else do nothing, and continue.*/
	}
}

void MainWidget::handleDelSelectSource(const int &row, const QString &musicName)
{
	//如果删除的当前行为正在播放的那一行，则停止该音乐的播放
	if (row == m_currentMusicRow)
	{
		this->handleSetStop();
	}
	/*Else do nothing, and continue.*/

	if (!DBModule::delLine(musicName))//删除一行
	{
		QMessageBox::information(this, TR("信息"), TR("删除该歌曲失败！"), QMessageBox::Yes);
		return;
	}
	/*Else do nothing, and continue.*/

	//更新map
	this->updateMap(musicName);
}

void MainWidget::updateMap(const QString &musicName)
{
	m_mapMusicRows.clear();
	for (int index = 0; index < m_playList->rowCount(); ++index)
	{
		m_mapMusicRows[m_playList->item(index, 1)->text()] = index;
	}

	//删除该歌曲对应的路径
	QMap<QString, QString>::iterator iter = m_mapMusicFilePath.find(musicName);
	if (iter != m_mapMusicFilePath.end())
	{
		m_mapMusicFilePath.erase(iter);
	}
	/*Else do nothing, and continue.*/

	//删除专辑文件夹里面歌曲对应的专辑
	QString albumFileName = m_albumPath + "/" + musicName + ".jpg";
	remove(albumFileName.toStdString().c_str());
}

void MainWidget::stateChanged()
{
	//当新状态为播放状态时，更改一些控件的状态
    if (m_state == MainWidget::Play)
	{
		m_playButton->setStyleSheet(QString("QPushButton:!hover{border-image: url(%1stopButton.png);}"
			"QPushButton:hover{border-image: url(%1stopButton2.png);}"
			"QPushButton:pressed{border-image: url(%1stopButton3.png);}").arg(ImagesDir));
		m_playButton->setToolTip(TR("暂停"));
		m_pauseAction->setIcon(QIcon(ImagesDir + "trayPause.png"));
		m_pauseAction->setText(TR("暂停"));
		m_infoLabel->setVisible(false);
		m_spectrograph->setVisible(true);
		m_trayIcon->setToolTip(TR("正在播放：") + m_currentMusicName);
		m_currentMusicLabel->setText(TR("正在播放：") + m_currentMusicName);
		this->showAlbumImage();
		QString lrcFilePath = m_lrcPath + "/" + m_currentMusicName + ".lrc";
		this->resolveLrc(lrcFilePath);
	}
    else if (m_state == MainWidget::Pause)	//新状态为暂停状态
	{
		m_playButton->setStyleSheet(QString("QPushButton:!hover{border-image: url(%1playButton.png);}"
			"QPushButton:hover{border-image: url(%1playButton2.png);}"
			"QPushButton:pressed{border-image: url(%1playButton3.png);}").arg(ImagesDir));
		m_playButton->setToolTip(TR("播放"));
		m_pauseAction->setIcon(QIcon(ImagesDir + "trayPlay.png"));
		m_pauseAction->setText(TR("播放"));
		m_infoLabel->setText(false);
		m_spectrograph->setVisible(true);
		m_trayIcon->setToolTip(AppName);
		m_currentMusicLabel->setText(TR("暂停播放：") + m_currentMusicName);
		m_lrc->stopLrcMask();
		m_lrc->setText(m_currentMusicName);
	}
    else if (m_state == MainWidget::Stop)	//新状态为停止播放
	{
		m_playButton->setStyleSheet(QString("QPushButton:!hover{border-image: url(%1playButton.png);}"
			"QPushButton:hover{border-image: url(%1playButton2.png);}"
			"QPushButton:pressed{border-image: url(%1playButton3.png);}").arg(ImagesDir));
		m_playButton->setToolTip(TR("播放"));
		m_pauseAction->setIcon(QIcon(ImagesDir + "trayPlay.png"));
		m_pauseAction->setText(TR("播放"));
		m_spectrograph->setVisible(false);
		m_infoLabel->setVisible(true);
		m_trayIcon->setToolTip(AppName);
		m_infoLabel->setText("Welcome to CZPlayer!");
		m_currentMusicLabel->setText(TR("停止播放：") + m_currentMusicName);
		m_lrc->stopLrcMask();
		m_lrc->setText(AppName);
	}
	/*Else do nothing, and continue.*/

	this->handleUpdateTime();	//更新时间和歌词
}

void MainWidget::handleUpdateTime()
{
	//当新状态为播放状态时，更改一些控件的状态
    if (m_state == MainWidget::Play)
	{
		QTime time = QTime::fromString(m_currentMusicTime, "mm:ss");
		int totalTime = time.minute() * 60 + time.second();
		//更新滚动条
		m_seekSlider->setRange(0, totalTime);
		m_seekSlider->setValue(m_time);

		//更新时间
		QTime currentTime(0, m_time / 60, m_time % 60);
		m_timeLabel->setText(currentTime.toString("mm:ss") + "/" + m_currentMusicTime);
		if (currentTime.toString("mm:ss") != m_currentMusicTime)
		{
			++m_time;
		}
		/*Else do nothing, and continue.*/
	}
    else if (m_state == MainWidget::Pause)	//新状态为暂停状态
	{
		//更新滚动条
		QTime time = QTime::fromString(m_currentMusicTime, "mm:ss");
		int totalTime = time.minute() * 60 + time.second();
		m_seekSlider->setRange(0, totalTime);
		m_seekSlider->setValue(m_time);

		//更新时间
		QTime currentTime(0, m_time / 60, m_time % 60);
		m_timeLabel->setText(currentTime.toString("mm:ss") + "/" + m_currentMusicTime);
	}
    else if (m_state == MainWidget::Stop)	//新状态为停止播放
	{
		m_seekSlider->setValue(0);
		m_timeLabel->setText("00:00/00:00");
		m_time = 0;
	}
	/*Else do nothing, and continue.*/

	//获取当前时间对应的歌词
	if (!m_lrcMap.isEmpty())
	{
		//获取当前时间在歌词中的前后两个时间点
		long long previousTime = 0;
		long long laterTime = 0;

		//keys()方法返回m_lrcMap列表
		foreach(long long value, m_lrcMap.keys())
		{
			if (m_time >= value / 1000)
			{
				previousTime = value;
			}
			else
			{
				laterTime = value;
				break;
			}
		}

		//获取当前时间所对应的歌词内容
		QString currentLrc = m_lrcMap.value(previousTime);

		// 如果是新的一行歌词，那么重新开始显示歌词遮罩
		if (currentLrc != m_lrc->text())
		{
			m_lrc->setText(currentLrc);
			long long intervalTime = laterTime - previousTime;
			m_lrc->startLrcMask(intervalTime);
		}
		/*Else do nothing, and continue.*/
	}
	else
	{
		m_lrc->setText("Welcome to CZPlayer!");
	}
}

void MainWidget::handleMinToTray()
{
	if (m_musicListWidget->isVisible())
	{
		QPropertyAnimation *musicListAnimation = new QPropertyAnimation(m_musicListWidget, "windowOpacity");
		musicListAnimation->setDuration(500);
		musicListAnimation->setStartValue(1);
		musicListAnimation->setEndValue(0);
		musicListAnimation->start();
		connect(musicListAnimation, SIGNAL(finished()), this, SLOT(handleHideMusicList()));
	}
	/*Else do nothing, and continue.*/

    if (m_mediaWidget->isVisible())
	{
        QPropertyAnimation *mediaWidgetAnimation = new QPropertyAnimation(m_mediaWidget, "windowOpacity");
        mediaWidgetAnimation->setDuration(500);
        mediaWidgetAnimation->setStartValue(1);
        mediaWidgetAnimation->setEndValue(0);
        mediaWidgetAnimation->start();
        connect(mediaWidgetAnimation, SIGNAL(finished()), this, SLOT(handleHideMediaWidget()));
	}
	/*Else do nothing, and continue.*/

	if (this->isVisible())
	{
		QPropertyAnimation *mainWidgetAnimation = new QPropertyAnimation(this, "windowOpacity");
		mainWidgetAnimation->setDuration(500);
		mainWidgetAnimation->setStartValue(1);
		mainWidgetAnimation->setEndValue(0);
		mainWidgetAnimation->start();
		connect(mainWidgetAnimation, SIGNAL(finished()), this, SLOT(handleHideMainWidget()));
		m_trayIcon->showMessage(AppName, TR("单击我重新回到主界面"));
	}
	/*Else do nothing, and continue.*/
}

void MainWidget::handleCloseAllProgress()
{
	QPropertyAnimation *mainWidgetAnimation = new QPropertyAnimation(this, "windowOpacity");
	mainWidgetAnimation->setDuration(500);
	mainWidgetAnimation->setStartValue(1);
	mainWidgetAnimation->setEndValue(0);
	mainWidgetAnimation->start();
	connect(mainWidgetAnimation, SIGNAL(finished()), this, SLOT(close()));

	QPropertyAnimation *musicListAnimation = new QPropertyAnimation(m_musicListWidget, "windowOpacity");
	musicListAnimation->setDuration(500);
	musicListAnimation->setStartValue(1);
	musicListAnimation->setEndValue(0);
	musicListAnimation->start();
	connect(musicListAnimation, SIGNAL(finished()), this, SLOT(close()));

    QPropertyAnimation *mediaWidgetAnimation = new QPropertyAnimation(m_mediaWidget, "windowOpacity");
    mediaWidgetAnimation->setDuration(500);
    mediaWidgetAnimation->setStartValue(1);
    mediaWidgetAnimation->setEndValue(0);
    mediaWidgetAnimation->start();
    connect(mediaWidgetAnimation, SIGNAL(finished()), this, SLOT(close()));

	QPropertyAnimation *lrcAnimation = new QPropertyAnimation(m_lrc, "windowOpacity");
	lrcAnimation->setDuration(500);
	lrcAnimation->setStartValue(1);
	lrcAnimation->setEndValue(0);
	lrcAnimation->start();
	connect(lrcAnimation, SIGNAL(finished()), this, SLOT(close()));

	m_minMusicWidget->close();
}

void MainWidget::handleTrayIconActivated(QSystemTrayIcon::ActivationReason activationReason)
{
	if (activationReason == QSystemTrayIcon::Trigger)
	{
		QPropertyAnimation *mainWidgetAnimation = new QPropertyAnimation(this, "windowOpacity");
		mainWidgetAnimation->setDuration(500);
		mainWidgetAnimation->setStartValue(0);
		mainWidgetAnimation->setEndValue(1);
		mainWidgetAnimation->start();
		this->show();
		this->handleShowMusicPanel();
	}
	/*Else do nothing, and continue.*/
}

void MainWidget::handleShowMinSize()
{
	this->showMinimized();
	m_musicListWidget->showMinimized();
    m_mediaWidget->showMinimized();
}

void MainWidget::handleShowLrcLocked()
{
	m_trayIcon->showMessage(TR("提示"), TR("你的桌面歌词已经锁定,点击播放器图标右键可以解锁！"));
}

void MainWidget::handleShowLrcUnlocked()
{
	m_trayIcon->showMessage(TR("提示"), TR("你的桌面歌词已经解锁！"));
}

void MainWidget::handleUnlockLrc()
{
	m_lrc->unlockLrc();
}

void MainWidget::contextMenuEvent(QContextMenuEvent *event)
{
	m_rightHandMenu->exec(event->globalPos());
	event->accept();
}

void MainWidget::dragEnterEvent(QDragEnterEvent *event)
{
	QList<QUrl> urls = event->mimeData()->urls();
	foreach(QUrl url, urls)
	{
		QString filePath = url.toLocalFile();

		QFileInfo fileInfo(filePath);
		//可拖进文件
		if (fileInfo.isFile())
		{
            QString suffixName = filePath.right(3);
            if (suffixName == "mp3" || suffixName == "MP3"
                || suffixName == "wma" || suffixName == "WMA"
                || suffixName == "wav" || suffixName == "WAV"
                || suffixName == "ogg" || suffixName == "OGG"
                || suffixName == "m3u" || suffixName == "M3U"
                || suffixName == "pls" || suffixName == "PLS")
			{
				event->accept();
			}
			else
			{
				event->ignore();
			}
		}
		//可拖进文件夹
		else if (fileInfo.isDir())
		{
			event->accept();
		}
		/*Else do nothing, and continue.*/
	}
}

void MainWidget::dropEvent(QDropEvent *event)
{
	QList<QUrl> urls = event->mimeData()->urls();
	if (urls.isEmpty())
	{
		return;
	}
	/*Else do nothing, and continue.*/

	emit sigStartloadMusic(urls);

	event->accept();
}

void MainWidget::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	QPixmap backgroundImage;
	backgroundImage.load(ImagesDir + "mainBg2.png");

	this->setMask(backgroundImage.mask());
	painter.drawPixmap(0, 0, 400, 212, backgroundImage);

	event->accept();
}

void MainWidget::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton)
	{
		m_dragPosition = event->globalPos() - frameGeometry().topLeft();
		event->accept();
	}
	/*Else do nothing, and continue.*/
}

void MainWidget::mouseMoveEvent(QMouseEvent *event)
{
	if (event->buttons() == Qt::LeftButton)
	{
		this->move(event->globalPos() - m_dragPosition);
		m_musicListWidget->move(frameGeometry().bottomLeft());
        m_mediaWidget->move(frameGeometry().topRight());
		event->accept();
	}
	/*Else do nothing, and continue.*/
}

void MainWidget::setState(State state)
{
	m_state = state;
}

void MainWidget::setMode(Mode mode)
{
	m_mode = mode;
}

void MainWidget::handleSetVolume(const int &value)
{
	if (m_isMute)
	{
		m_volumeButton->setToolTip(TR("静音"));
		m_volumeButton->setStyleSheet(QString("QPushButton:!hover{border-image: url(%1soundButton.png);}"
			"QPushButton:hover{border-image: url(%1soundButton2.png);}"
			"QPushButton:pressed{border-image: url(%1soundButton3.png);}").arg(ImagesDir));
		m_isMute = false;
	}
	/*Else do nothing, and continue.*/

	SoundControl::setVolume(value);
	m_volumeSlider->setToolTip(TR("当前音量：%1%").arg(value));
}

void MainWidget::handleSetMute()
{
	if (!m_isMute)
	{
		SoundControl::setVolume(0);
		m_volumeButton->setToolTip(TR("恢复音量"));
		m_volumeButton->setStyleSheet(QString("QPushButton:!hover{border-image: url(%1soundButton4.png);}"
			"QPushButton:hover{border-image: url(%1soundButton5.png);}"
			"QPushButton:pressed{border-image: url(%1soundButton6.png);}").arg(ImagesDir));
		m_isMute = true;
	}
	else
	{
		SoundControl::setVolume(m_volumeSlider->value());
		m_volumeButton->setToolTip(TR("静音"));
		m_volumeButton->setStyleSheet(QString("QPushButton:!hover{border-image: url(%1soundButton.png);}"
			"QPushButton:hover{border-image: url(%1soundButton2.png);}"
			"QPushButton:pressed{border-image: url(%1soundButton3.png);}").arg(ImagesDir));
		m_isMute = false;
	}
}

void MainWidget::handleShowModeMenu()
{
	//循环播放
	if (m_mode == CyclePlay)
	{
		m_cycleAction->setText(TR("循环播放 √"));
		m_randomAction->setText(TR("随机播放"));
	}
	//随机播放
	else if (m_mode == RandomPlay)
	{
		m_cycleAction->setText(TR("循环播放"));
		m_randomAction->setText(TR("随机播放 √"));
	}
	/*Else do nothing, and continue.*/

	m_playModeMenu->exec(QCursor::pos());
}

void MainWidget::handleCyclePlay()
{
	m_cycleAction->setText(TR("循环播放 √"));
	m_randomAction->setText(TR("随机播放"));
	m_modeButton->setStyleSheet(QString("QPushButton:!hover{border-image: url(%1cyclePlay.png);}"
		"QPushButton:hover{border-image: url(%1cyclePlay2.png);}"
		"QPushButton:pressed{border-image: url(%1cyclePlay3.png);}").arg(ImagesDir));
	m_modeButton->setToolTip(TR("循环播放"));
	this->setMode(CyclePlay);
}

void MainWidget::handleRandomPlay()
{
	m_cycleAction->setText(TR("循环播放"));
	m_randomAction->setText(TR("随机播放 √"));
	m_modeButton->setStyleSheet(QString("QPushButton:!hover{border-image: url(%1randomPlay.png);}"
		"QPushButton:hover{border-image: url(%1randomPlay2.png);}"
		"QPushButton:pressed{border-image: url(%1randomPlay3.png);}").arg(ImagesDir));
	m_modeButton->setToolTip(TR("随机播放"));
	this->setMode(RandomPlay);
}

void MainWidget::handleAlbumDownloadFinished(const QString &musicName, const QString &albumUrl)
{
	DBModule::updateAlbumUrlPath(musicName, albumUrl);
}

void MainWidget::handleSharedCurrentMusic(const QString &musicName)
{
	QString albumUrlPath;
	DBModule::readAlbumUrlPathFromDB(musicName, albumUrlPath);

	if (m_sharedDialog->isHidden())
	{
		QString albumImagePath = m_albumPath + "/" + musicName + ".jpg";
        if (QFile::exists(albumImagePath))
		{
			m_sharedDialog->init(albumImagePath, musicName, albumUrlPath);
		}
		else
		{
			m_sharedDialog->init(ImagesDir + "albumImage.png", musicName, albumUrlPath);
		}

		m_sharedDialog->exec();
	}
	/*Else do nothing, and continue.*/
}

void MainWidget::loadMusicList(const QString &filePath)
{
	//音乐文件名
	QString fileName = QFileInfo(filePath).baseName();

	//时间
	unsigned long musicTime = getMusicTime((wchar_t*)filePath.utf16());
	QTime time(0, musicTime / 60, musicTime % 60);
	QString formatTime = time.toString("mm:ss");

	//放入数据库
	DBModule::insertLine(fileName, formatTime, filePath);

	MusicInfoEntity entity(fileName, formatTime, filePath);
	this->handleAddMusic(entity);
}

void MainWidget::handleLoadMusicListFinished()
{
#ifdef CZ_DEBUG
	qDebug() << "Load music list finished.";
#endif 

    if (m_isStarupLoadMusic)		//是否是启动时加载播放列表
	{
		//当从配置文件读取自动播放标志为true,播放列表不为空并且不是带参数启动播放器时则执行播放功能
		GlobalConfig *config = GlobalConfig::instance();
		if (config->autoPlayMusic() &&
			m_playList->rowCount() != 0 &&
			m_argvMusicPath == "")
		{
			this->handleSetPlayPaused();
		}
		/*Else do nothing, and continue.*/

		//如果是带参数启动播放器,则播放该歌曲(参数为歌曲)
		if (m_argvMusicPath != "")
		{
			//在map里面找该歌曲
			QMap<QString, int>::iterator iter = m_mapMusicRows.find(QFileInfo(m_argvMusicPath).baseName());
			if (iter != m_mapMusicRows.end())
			{
				//模拟双击该行播放音乐,第二个参数无意义
				this->handleTableDoubleClicked(iter.value(), 1);
			}
			else	//当前音乐列表没有这首歌则把该歌曲加入播放列表并且播放
			{
				this->loadMusicList(m_argvMusicPath);
				this->handleTableDoubleClicked(m_playList->rowCount() - 1, 1);
			}
		}
		/*Else do nothing, and continue.*/
	}
	/*Else do nothing, and continue.*/
	m_isStarupLoadMusic = false;
}

void MainWidget::handleLoadMusicListError()
{
	QMessageBox::information(this, TR("信息"), TR("不是有效的m3u或pls文件！"), QMessageBox::Yes);
}

void MainWidget::handleAddMusicDir()
{
	QString directory = QFileDialog::getExistingDirectory(this, TR("请选择文件夹"), m_musicPath);
	if (!directory.isEmpty())
	{
		emit sigStartloadMusic(directory);
	}
	/*Else do nothing, and continue.*/
}

void MainWidget::changeWorkDir()
{
	QDir::setCurrent(QCoreApplication::applicationDirPath());
}

void MainWidget::handleSpectrumChanged(vector<float> vecFrequency)
{
	emit sigSpectrumChanged(vecFrequency);
}

void MainWidget::handleFinished()
{
	m_nextButton->click();
}

void MainWidget::reset()
{
	m_spectrograph->reset();

	//重置播放引擎
	if (m_basicPlayer != NULL)
	{
		CCriticalSection cs;
		cs.Enter();
		m_basicPlayer->Stop();
		//调用Sleep函数的原因是m_basicPlayer->Stop()函数里面线程的终止是异步的，
		//所以要等线程终止后才delete m_basicPlayer
		Sleep(500);
		cs.Leave();

		if (m_basicPlayer != NULL)
		{
			delete m_basicPlayer;
			m_basicPlayer = NULL;
		}
		/*Else do nothing, and continue.*/
	}
	/*Else do nothing, and continue.*/
}

void MainWidget::findAndPlay(const QString &currentMusicName, const int &currentMusicRow)
{
	QMap<QString, QString>::iterator iter = m_mapMusicFilePath.find(currentMusicName);
	if (iter != m_mapMusicFilePath.end())
	{
		this->reset();
		m_time = 0;

        if (QFile::exists(iter.value()))
		{
			this->play(iter.value(), currentMusicName, currentMusicRow, m_playList->item(currentMusicRow, 2)->text());
		}
		else
		{
			QMessageBox::information(this, TR("信息"), TR("无效歌曲路径！"), QMessageBox::Yes);
		}
	}
	/*Else do nothing, and continue.*/
}

void MainWidget::play(const QString &currentFilePath,
	const QString &currentMusicName,
	const int &currentMusicRow,
	const QString &currentMusicTime)
{
    this->play(currentFilePath);						//播放
	this->setState(Play);								//设置状态
	m_currentMusicName = currentMusicName;				//当前播放的歌曲名
	m_currentFilePath = currentFilePath;				//当前播放的歌曲路径
	m_currentMusicRow = currentMusicRow;				//当前播放的歌曲行号
	m_currentMusicTime = currentMusicTime;				//当前播放的歌曲时间
	this->stateChanged();								//状态改变，更新界面
	this->rowChanged();									//选中播放媒体行
	m_playList->setCurrentMusicRow(m_currentMusicRow);	//设置当前播放歌曲行号
}

void MainWidget::play(const QString &currentFilePath)
{
	//在CBasicPlayer构造函数里面主要完成播放线程、频谱分析仪以及文件读写流的构建
	m_basicPlayer = new CBasicPlayer((wchar_t*)currentFilePath.utf16());
    c_connect(m_basicPlayer, sigSpectrumChanged, this, &MainWidget::handleSpectrumChanged);
    c_connect(m_basicPlayer, sigFinished, this, &MainWidget::handleFinished);

	//在GetValid函数里面主要是对音乐文件进行打开已经读取音乐文件数据
	if (!m_basicPlayer->isVaild())
	{
		return;
	}
	/*Else do nothing, and continue.*/

	//播放线程创建好以及已经读取到了音乐文件的数据就可以开始播放了
	m_basicPlayer->Start();
}

void MainWidget::play()
{
	CCriticalSection cs;
	cs.Enter();
	m_basicPlayer->Play();
	cs.Leave();
}

void MainWidget::pause()
{
	CCriticalSection cs;
	cs.Enter();
	m_basicPlayer->Pause();
	cs.Leave();
}

void MainWidget::handleAlbumDownloadStatus(const QString &fileName,
	const QString &albumUrl,
	const QString &albumFilePath,
	AlbumDownloadStatus status)
{
	QThread *thread = dynamic_cast<QThread *>(sender());
	m_albumThreads.removeOne(thread);
	thread->quit();
	thread->wait();
	thread->deleteLater();

	if (fileName != m_currentMusicName)
	{
		return;
	}
	/*Else do nothing, and continue.*/

	if (status == AlbumDownloadSuccess)
	{
#ifdef CZ_DEBUG
		qDebug() << "Album download success, albumUrl = " << albumUrl;
#endif
		this->changeAlbumImage(albumFilePath);
		m_minMusicWidget->changeAlbumImage(albumFilePath);

		DBModule::updateAlbumUrlPath(fileName, albumUrl);
	}
	else if (status == AlbumDownloadFailed)
	{
#ifdef CZ_DEBUG
		qDebug() << "Album download failed.";
#endif
		m_albumImage->setPixmap(QPixmap(ImagesDir + "albumImage.png"));
		m_minMusicWidget->changeAlbumImage(ImagesDir + "albumImage.png");
	}
	else
	{
#ifdef CZ_DEBUG
		qDebug() << "Not found this status, status code = " << status;
		qDebug() << __FILE__ << __FUNCTION__ << __LINE__;
#endif
	}
}

void MainWidget::showAlbumImage()
{
	//加载专辑图片
	if (m_currentMusicName.contains('-'))//包含该字符串（格式为：光辉岁月 - 黄家驹）
	{
		//打开专辑图片
		QStringList ablumList = m_currentMusicName.split('-');
		QString musicName = ablumList.at(1).trimmed();//歌曲名
		QString musicArtist = ablumList.at(0).trimmed();//歌手
		QString albumFilePath = m_albumPath + "/" + m_currentMusicName + ".jpg";
		QFile fileAlbum(albumFilePath);
		if (fileAlbum.open(QIODevice::ReadOnly))
		{
			fileAlbum.close();
			//由于下载出错，没能下载完
			if (fileAlbum.size() <  4500)
			{
				m_albumImage->setPixmap(QPixmap(ImagesDir + "albumImage.png"));
				m_minMusicWidget->changeAlbumImage(ImagesDir + "albumImage.png");
				//下载专辑
				this->downloadAlbum(m_currentMusicName, musicName, musicArtist, albumFilePath);
			}
			else
			{
				this->changeAlbumImage(albumFilePath);
				m_minMusicWidget->changeAlbumImage(albumFilePath);
			}
		}
		else
		{
			fileAlbum.close();
			m_albumImage->setPixmap(QPixmap(ImagesDir + "albumImage.png"));
			m_minMusicWidget->changeAlbumImage(ImagesDir + "albumImage.png");
			//下载专辑
			this->downloadAlbum(m_currentMusicName, musicName, musicArtist, albumFilePath);
		}
	}
	else
	{
		m_albumImage->setPixmap(QPixmap(ImagesDir + "albumImage.png"));
		m_minMusicWidget->changeAlbumImage(ImagesDir + "albumImage.png");
	}
}

void MainWidget::handleLrcDownloadStatus(const QString &musicName,
	const QString &musicArtist,
	const QString &lrcFilePath,
	LrcDownloadStatus status)
{
	QThread *thread = dynamic_cast<QThread *>(sender());
	m_lrcThreads.removeOne(thread);
	thread->quit();
	thread->wait();
	thread->deleteLater();

	QString fileName = musicArtist + " - " + musicName;
	if (fileName != m_currentMusicName)
	{
		return;
	}
	/*Else do nothing, and continue.*/

	if (status == LrcDownloadSuccess)
	{
		this->resolveLrc(lrcFilePath);
	}
	else if (status == LrcDownloadFailed)
	{
		m_lrc->setText(musicArtist + " - " + musicName + TR("----下载歌词失败!"));
	}
	else
	{
#ifdef CZ_DEBUG
		qDebug() << "Not found this status, status code = " << status;
		qDebug() << __FILE__ << __FUNCTION__ << __LINE__;
#endif
	}
}

void MainWidget::changeAlbumImage(const QString &albumFilePath)
{
	QImage image(albumFilePath);
    if (image.isNull())
    {
        m_albumImage->setPixmap(QPixmap(ImagesDir + "albumImage.png"));
        return;
    }
    /*Else do nothing, and continue.*/

	int width = image.width();
	int height = image.height();
	int newWidth = width > height ? height : width;

	QImage tempImage(newWidth, newWidth, QImage::Format_ARGB32);
	tempImage.fill(Qt::transparent);

	QBrush brush(image);
	QPainter painter(&tempImage);
	painter.setPen(Qt::NoPen);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setBrush(brush);
	painter.drawRect(0, 0, newWidth, newWidth);

	QImage newImage = tempImage.scaled(m_albumImage->width(), m_albumImage->height(),
		Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	m_albumImage->setPixmap(QPixmap::fromImage(newImage));
}

void MainWidget::downloadAlbum(const QString &fileName,
	const QString &musicName,
	const QString &musicArtist,
	const QString &albumFilePath)
{
	AlbumThread *albumDownloadThread = new AlbumThread(fileName, musicName, musicArtist, albumFilePath);
	m_albumThreads.append(albumDownloadThread);
	qRegisterMetaType<AlbumDownloadStatus>("AlbumDownloadStatus");
	connect(albumDownloadThread, SIGNAL(sigAlbumDownloadStatus(QString, QString, QString, AlbumDownloadStatus)),
		this, SLOT(handleAlbumDownloadStatus(QString, QString, QString, AlbumDownloadStatus)), Qt::QueuedConnection);
	albumDownloadThread->start();
}

void MainWidget::downloadLrc(const QString &musicName, const QString &musicArtist, const QString &lrcFilePath)
{
	LrcThread *lrcDownloadThread = new LrcThread(musicName, musicArtist, lrcFilePath);
	m_lrcThreads.append(lrcDownloadThread);
	qRegisterMetaType<LrcDownloadStatus>("LrcDownloadStatus");
	connect(lrcDownloadThread, SIGNAL(sigLrcDownloadStatus(QString, QString, QString, LrcDownloadStatus)),
		this, SLOT(handleLrcDownloadStatus(QString, QString, QString, LrcDownloadStatus)), Qt::QueuedConnection);
	lrcDownloadThread->start();
}

void MainWidget::handleMusicDirChanged(const QString &path)
{
	m_musicPath = path;
    QDir dir;
    dir.mkpath(m_musicPath);
    m_mediaWidget->setMusicPath(m_musicPath);
}

void MainWidget::handleAlbumDirChanged(const QString &path)
{
	m_albumPath = path;
    QDir dir;
    dir.mkpath(m_albumPath);
}

void MainWidget::handleLrcDirChanged(const QString &path)
{
	m_lrcPath = path;
    QDir dir;
    dir.mkpath(m_lrcPath);
}

void MainWidget::handleAddMusic(const MusicInfoEntity &entity)
{
	//序号
	QTableWidgetItem *rowItem = new QTableWidgetItem;
	rowItem->setTextAlignment(Qt::AlignCenter);
	rowItem->setFont(QFont(TR("微软雅黑"), 10));

	//歌曲
	QTableWidgetItem *fileNameItem = new QTableWidgetItem(entity.fileName());
	fileNameItem->setTextAlignment(Qt::AlignCenter);
	fileNameItem->setFont(QFont(TR("微软雅黑"), 10));

	//时间
	QTableWidgetItem *timeItem = new QTableWidgetItem(entity.time());
	timeItem->setTextAlignment(Qt::AlignCenter);
	timeItem->setFont(QFont(TR("微软雅黑"), 10));

	//插入播放列表
	int currentRows = m_playList->rowCount();//返回列表中的行数
	m_playList->insertRow(currentRows);//从播放列表中的当前行插入
	rowItem->setText(QString::number(currentRows + 1));
	m_playList->setItem(currentRows, 0, rowItem);
	m_playList->setItem(currentRows, 1, fileNameItem);
	m_playList->setItem(currentRows, 2, timeItem);

	//更改时间列间隔
	if (currentRows + 1 <= 12)
	{
		m_playList->horizontalHeader()->resizeSection(3, 80);
	}
	else
	{
		m_playList->horizontalHeader()->resizeSection(3, 65);
	}

	//将音乐信息加入QMap
	m_mapMusicRows[entity.fileName()] = currentRows;
	m_mapMusicFilePath[entity.fileName()] = entity.filePath();
}

void MainWidget::handleConfigDialogClosed()
{
	m_globalHotKey->setConfigOptionsHotkeyWork(true);
	m_globalHotKey->setShowHideMainWidgetHotkeyWork(true);
	m_globalHotKey->setMinMainWidgetHotkeyWork(true);
	m_globalHotKey->setOpenMusicfileHotkeyWork(true);
	m_globalHotKey->setShowDownloadHotkeyWork(true);
	m_globalHotKey->setShowMusicListHotkeyWork(true);
	m_globalHotKey->setShowLrcHotkeyWork(true);
	m_globalHotKey->setPlayPauseHotkeyWork(true);
	m_globalHotKey->setLastHotkeyWork(true);
	m_globalHotKey->setNextHotkeyWork(true);
}

void MainWidget::handleMuteChanged(bool isMute)
{
	m_isMute = isMute;
	if (m_isMute)
	{
		SoundControl::setVolume(0);//静音
		m_volumeButton->setToolTip(TR("恢复音量"));
		m_volumeButton->setStyleSheet(QString("QPushButton:!hover{border-image: url(%1soundButton4.png);}"
			"QPushButton:hover{border-image: url(%1soundButton5.png);}"
			"QPushButton:pressed{border-image: url(%1soundButton6.png);}").arg(ImagesDir));
	}
	else
	{
		m_volumeButton->setToolTip(TR("静音"));
		m_volumeButton->setStyleSheet(QString("QPushButton:!hover{border-image: url(%1soundButton.png);}"
			"QPushButton:hover{border-image: url(%1soundButton2.png);}"
			"QPushButton:pressed{border-image: url(%1soundButton3.png);}").arg(ImagesDir));
	}
}

void MainWidget::handleVolumeChanged(const int &volume)
{
	m_volumeSlider->setValue(volume);
	if (!m_isMute)
	{
		SoundControl::setVolume(volume);
	}
	/*Else do nothing, and continue.*/
}

void MainWidget::handlePlayModeChanged(enum PlayModeType playMode)
{
	//随即播放
	if (playMode == 1)
	{
		m_modeButton->setStyleSheet(QString("QPushButton:!hover{border-image: url(%1randomPlay.png);}"
			"QPushButton:hover{border-image: url(%1randomPlay2.png);}"
			"QPushButton:pressed{border-image: url(%1randomPlay3.png);}").arg(ImagesDir));
		m_modeButton->setToolTip(TR("随机播放"));
		this->setMode(RandomPlay);
	}
	//循环播放
	else if (playMode == 2)
	{
		m_modeButton->setStyleSheet(QString("QPushButton:!hover{border-image: url(%1cyclePlay.png);}"
			"QPushButton:hover{border-image: url(%1cyclePlay2.png);}"
			"QPushButton:pressed{border-image: url(%1cyclePlay3.png);}").arg(ImagesDir));
		m_modeButton->setToolTip(TR("循环播放"));
		this->setMode(CyclePlay);
	}
	/*Else do nothing, and continue.*/
}
