#include "stdafx.h"
#include "widget.h"
#include "static.h"
#include "czplayerconfig.h"
#include "ClientBaseOperating.h"
#include "DBModule.h"
#include "SoundControl.h"

const int NullTimerId = -1;

/*

增加数据库功能
                                                         -> 把音乐信息录入数据库(必须把音乐路径录入)
1.运行程序 -> 打开音乐文件 -> 把全部音乐文件加载到媒体源
                                                         -> 把音乐信息显示到音乐表格

                                       -> 把音乐文件显示到音乐表格
2.运行程序 -> 把音乐文件从数据库中读出
                                       -> 根据音乐文件路径查找音乐 -> 把音乐文件加载到媒体源

*/

/*
CTRL_CLOSE_EVENT, CTRL_LOGOFF_EVENT和CTRL_SHUTDOWN_EVENT通常被用来处理一些程序的清理工作，
然后调用ExitProcess API。另外，这三个事件有超时机制，CTRL_CLOSE_EVENT是5秒，另外两个是20秒。
如果程序超时候，系统将会弹出结束进程的对话框。如果用户选择了结束进程，
任何清理工作都不会做，所以应该在超时时间内完成工作
*/
//事件处理回调
BOOL HandlerRoutine(DWORD dwCtrlType)  
{  
	switch (dwCtrlType)  
	{  
	case CTRL_C_EVENT:						//Ctrl+C，系统会发送次消息
		qDebug() << "ctrl+c--程序不会关闭";
		return TRUE;  
	case CTRL_CLOSE_EVENT:					 //当用户关闭Console时，系统会发送次消息
		qDebug() << "ctrl close--正常关闭";
		return TRUE;  
	case CTRL_BREAK_EVENT:					//Ctrl+break，系统会发送次消息
		qDebug() << "CTRL_BREAK_EVENT";  
		return TRUE;
	case CTRL_LOGOFF_EVENT:					//用户退出（注销），系统会发送次消息
		qDebug() << "CTRL_LOGOFF_EVENT--用户退出或注销";  
		return TRUE;
	case CTRL_SHUTDOWN_EVENT:				//系统关闭，系统会发送次消息
		qDebug() << "CTRL_SHUTDOWN_EVENT--系统关闭";  
		return TRUE;
	default: 
		return FALSE;  
	}  
} 

//构造函数
Widget::Widget()
{
	m_strCurrentFilePath = "";
	m_strCurrentMusicName = "";
	m_strCurrentMusicTime = "00:00";
	m_nCurrentMusicRow = 0;
	nTime = 0;
	nPos = 170;

	//安装事件处理
	if (!SetConsoleCtrlHandler((PHANDLER_ROUTINE)HandlerRoutine, TRUE))
	{
		qDebug() << "安装事件处理失败！";
	}

	//step = 0;
	//baseTimer.start(60, this);

	qsrand(time(NULL));							//初始化随机数种子
	ClientBaseOperating::touchConfigFile();		//创建一个配置文件
	readyReadDb = 0;							//没有从数据库中读取
	DBModule::initDB();							//初始化数据库
	ClientBaseOperating::mkdirDownloadDir();	//创建一个下载歌曲目录
	ClientBaseOperating::mkdirAlbumDir();		//创建一个文件夹来存储下载的专辑
	ClientBaseOperating::mkdirLrcDir();			//创建一个下载歌词目录

	m_engine = new Engine(this);				//播放引擎
	this ->createUI();							//创建界面
	this ->connectUI();							//信号与槽
	m_state = NoState;							//设定一个状态
}

//析构函数
Widget::~Widget()
{
	if (searchPanel) {delete searchPanel; searchPanel = 0;}
	if (playList) {delete playList; playList = 0;}
	if (musicListWidget) {delete musicListWidget; musicListWidget = 0;}
	if (lrc) {delete lrc; lrc = 0;}
	if (minMusicWidget) {delete minMusicWidget; minMusicWidget = 0;}

	//terminate:终止线程
	if (hotpugWatcherThread ->isRunning()) {hotpugWatcherThread ->terminate(); delete hotpugWatcherThread; hotpugWatcherThread = 0;}
}

//创建界面
void Widget::createUI()
{
	//设置窗口基本属性
	this ->resize(400, 212);//设置窗体大小
	this ->setWindowFlags(Qt::FramelessWindowHint);//去掉窗体边框
	this ->setAttribute(Qt::WA_TranslucentBackground);//设置背景透明
	this ->setWindowIcon(QIcon(":/images/CZPlayer.png"));//设置logo
	this ->setAcceptDrops(true);//可以拖放文件
	this ->setWindowTitle(tr("CZPlayer"));

	//    qDebug() << "电脑屏幕大小：" << QApplication::desktop() ->width() << ", " << QApplication::desktop() ->height();
	//    qDebug() << "鼠标当前位置：" << QCursor::pos();
	//    qDebug() << "软件界面大小：" << this ->frameGeometry();
	//    qDebug() << "软件界面相对于屏幕的绝对坐标：" << this ->mapToParent(QPoint(0, 0));
	//    qDebug() << "软件界面相对于屏幕的绝对坐标：" << this ->pos();
	//    qDebug() << frameGeometry().topRight();

	//设置界面渐变显示
	QPropertyAnimation *animation = new QPropertyAnimation(this, "windowOpacity");
	animation ->setDuration(1000);
	animation ->setStartValue(0);
	animation ->setEndValue(1);
	animation ->start();

	//专辑封面
	albumImage = new QLabel(this);
	albumImage ->setToolTip(tr("专辑封面"));
	albumImage ->setPixmap(QPixmap(tr(":/images/albumImage.png")));
	movie = new QMovie(tr(":/images/jiaZai.gif"));
	//    albumImage ->setMovie(movie);
	//    movie ->start();

	//信息显示标签
	InfoLabel = new QLabel(this);
	InfoLabel ->setObjectName("InfoLabel");
	InfoLabel ->setText(tr("Welecome to CZPlayer!"));

	//当前播放歌曲
	currentMusicLabel = new QLabel(this);
	currentMusicLabel ->setObjectName(tr("currentMusicLabel"));
	currentMusicLabel ->setText(tr("Welecome to CZPlayer!"));

	//频谱界面
	m_spectrograph = new Spectrograph(this);
	m_spectrograph ->setParams(SpectrumNumBands, SpectrumLowFreq, SpectrumHighFreq);	//初始化频谱参数

	//波形界面
	m_waveform = new Waveform(this);
	if (CZPlayerConfig::getValue("AUDIOSHOW").toString() == "spectrumShow")	//频谱显示
	{
		m_spectrograph ->setVisible(true);
		m_waveform ->setVisible(false);
	}
	else if (CZPlayerConfig::getValue("AUDIOSHOW").toString() == "formShow")	//波形显示
	{
		m_spectrograph ->setVisible(false);
		m_waveform ->setVisible(true);
	}

	//创建控制播放进度的滑块
	seekSlider = new QSlider(Qt::Horizontal, this);
	seekSlider ->setStyleSheet("QSlider::groove:horizontal:!hover{border-image: url(:/images/progressBar.png);}"
		"QSlider::groove:horizontal:hover{border-image: url(:/images/progressBar2.png);}"
		"QSlider::sub-page{border-image: url(:/images/progressBarSub.png);}");
	seekSlider ->setObjectName(tr("seekSlider"));

	//创建音量调节滑块
	volumeSlider = new QSlider(Qt::Horizontal, this);
	volumeSlider ->setStyleSheet("QSlider::groove:horizontal:!hover{border-image: url(:/images/volumeBar.png);}"
		"QSlider::groove:horizontal:hover{border-image: url(:/images/volumeBar2.png);}"
		"QSlider::sub-page{border-image: url(:/images/volumeBarSub.png);}");
	volumeSlider ->setObjectName(tr("volumeSlider"));
	volumeSlider ->setToolTip(tr("音量调节"));
	volumeSlider ->setRange(0, 100);
	volumeSlider ->setValue(CZPlayerConfig::getValue("VOLUMEVALUE").toInt());
	SoundControl::SetVolume(0, CZPlayerConfig::getValue("VOLUMEVALUE").toInt());	//设置音量

	//音量调节按钮
	volumeButton = new QPushButton(this);
	volumeButton ->setObjectName(tr("volumeButton"));
	if (CZPlayerConfig::getValue("SETMUTE").toString() == "false")
	{
		volumeButton ->setToolTip(tr("静音"));
		volumeButton ->setStyleSheet("QPushButton:!hover{border-image: url(:/images/soundButton.png);}"
			"QPushButton:hover{border-image: url(:/images/soundButton2.png);}"
			"QPushButton:pressed{border-image: url(:/images/soundButton3.png);}");
	}
	else
	{
		SoundControl::SetVolume(0, 0);		//静音
		volumeButton ->setToolTip(tr("恢复音量"));
		volumeButton ->setStyleSheet("QPushButton:!hover{border-image: url(:/images/soundButton4.png);}"
			"QPushButton:hover{border-image: url(:/images/soundButton5.png);}"
			"QPushButton:pressed{border-image: url(:/images/soundButton6.png);}");
	}

	//播放模式按钮
	modeButton = new QPushButton(this);
	modeButton ->setObjectName(tr("modeButton"));
	if (ClientBaseOperating::initPlayMode() == "cyclePlay")	//循环播放
	{
		modeButton ->setStyleSheet("QPushButton:!hover{border-image: url(:/images/cyclePlay.png);}"
			"QPushButton:hover{border-image: url(:/images/cyclePlay2.png);}"
			"QPushButton:pressed{border-image: url(:/images/cyclePlay3.png);}");
		modeButton ->setToolTip(tr("循环播放"));
		this ->setMode(cyclePlay);							//设置循环播放模式
	}
	else if (ClientBaseOperating::initPlayMode() == "randomPlay")	//随机播放
	{
		modeButton ->setStyleSheet("QPushButton:!hover{border-image: url(:/images/randomPlay.png);}"
			"QPushButton:hover{border-image: url(:/images/randomPlay2.png);}"
			"QPushButton:pressed{border-image: url(:/images/randomPlay3.png);}");
		modeButton ->setToolTip(tr("随机播放"));
		this ->setMode(randomPlay);							//设置随机播放模式
	}


	//设置显示时间标签
	timeLabel = new QLabel(tr("00:00/00:00"), this);
	timeLabel ->setToolTip(tr("当前时间/总时间"));
	timeLabel ->setObjectName(tr("timeLabel"));
	timeLabel ->setAlignment(Qt::AlignCenter);//设置对齐格式
	//QSizePolicy类是描述水平和垂直修改大小策略的一种属性
	timeLabel ->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);//水平方向上尺寸可扩展，竖直方向已固定

	//显示下载页面
	showButton = new QPushButton(this);
	showButton ->setObjectName(tr("showButton"));
	showButton ->setToolTip(tr("打开搜索面板"));
	showButton ->setStyleSheet("QPushButton:!hover{border-image: url(:/images/showButton.png);}"
		"QPushButton:hover{border-image: url(:/images/showButton2.png);}"
		"QPushButton:pressed{border-image: url(:/images/showButton3.png);}");

	//播放
	playButton = new QPushButton(this);
	playButton ->setToolTip(tr("播放"));
	playButton ->setObjectName(tr("playButton"));
	playButton ->setStyleSheet("QPushButton:!hover{border-image: url(:/images/playButton.png);}"
		"QPushButton:hover{border-image: url(:/images/playButton2.png);}"
		"QPushButton:pressed{border-image: url(:/images/playButton3.png);}");

	//停止
	endButton = new QPushButton(this);
	endButton ->setToolTip(tr("停止"));
	endButton ->setObjectName(tr("endButton"));

	//上一首
	preButton = new QPushButton(this);
	preButton ->setToolTip(tr("上一首"));
	preButton ->setObjectName(tr("preButton"));

	//下一首
	nextButton = new QPushButton(this);
	nextButton ->setToolTip(tr("下一首"));
	nextButton ->setObjectName(tr("nextButton"));

	//打开文件
	openFileButton = new QPushButton(this);
	openFileButton ->setToolTip(tr("打开文件"));
	openFileButton ->setObjectName(tr("openFileButton"));

	//打开播放列表
	musicListButton = new QPushButton(this);
	musicListButton ->setToolTip(tr("打开播放列表"));
	musicListButton ->setObjectName(tr("musicListButton"));

	//桌面歌词
	lrcButton = new QPushButton(this);
	lrcButton ->setToolTip(tr("桌面歌词"));
	lrcButton ->setObjectName(tr("lrcButton"));

	//主界面退出
	closeAction = new QAction(this);
	closeAction ->setIcon(QIcon(":/images/quitAction.png"));
	closeAction ->setText(tr("退出"));

	//主界面最小化
	minAction = new QAction(this);
	minAction ->setIcon(QIcon(":/images/minAction.png"));
	minAction ->setText(tr("最小化"));

	//推盘界面退出程序
	quitAction = new QAction(this);
	quitAction ->setIcon(QIcon(":/images/quitAction.png"));
	quitAction ->setText(tr("退出"));

	//暂停(播放)
	pauseAction = new QAction(this);
	pauseAction ->setIcon(QIcon(":/images/trayPlay.png"));
	pauseAction ->setText(tr("播放"));

	//停止
	stopAction = new QAction(this);
	stopAction ->setIcon(QIcon(":/images/trayStop.png"));
	stopAction ->setText(tr("停止"));

	//上一首
	lastAction = new QAction(this);
	lastAction ->setIcon(QIcon(":/images/trayLast.png"));
	lastAction ->setText(tr("上一首"));

	//下一首
	nextAction = new QAction(this);
	nextAction ->setIcon(QIcon(":/images/trayNext.png"));
	nextAction ->setText(tr("下一首"));

	//解锁歌词
	unlockLrcAction = new QAction(this);
	unlockLrcAction ->setIcon(QIcon(":/images/unlock.png"));
	unlockLrcAction ->setText(tr("解锁歌词"));

	//打开音乐文件
	openFileAction = new QAction(this);
	openFileAction ->setIcon(QIcon(":/images/folder.png"));
	openFileAction ->setText(tr("打开音乐文件"));

	//打开播放列表
	openMusicListAction = new QAction(this);
	openMusicListAction ->setIcon(QIcon(":/images/openMusicList.png"));
	openMusicListAction ->setText(tr("打开播放列表"));

	//打开搜索面板
	openSearchPanelAnimation = new QAction(this);
	openSearchPanelAnimation ->setIcon(QIcon(":/images/openMusicDownload.png"));
	openSearchPanelAnimation ->setText(tr("打开搜索面板"));

	//显示桌面歌词
	openLrcAction = new QAction(this);
	openLrcAction ->setIcon(QIcon(":/images/lrc.png"));
	openLrcAction ->setText(tr("显示桌面歌词"));

	//选项
	configAction = new QAction(this);
	configAction ->setIcon(QIcon(":/images/configButton.png"));
	configAction ->setText(tr("设置"));

	//关于Qt
	aboutQtAction = new QAction(this);
	aboutQtAction ->setIcon(QIcon(":/images/Qt.png"));
	aboutQtAction ->setText(tr("关于Qt"));

	//关于
	aboutAction = new QAction(this);
	aboutAction ->setIcon(QIcon(":/images/CZPlayer2.png"));
	aboutAction ->setText(tr("关于CZPlayer"));

	//循环播放
	cycleAction = new QAction(this);
	cycleAction ->setIcon(QIcon(":/images/cyclePlay.png"));

	//随机播放
	randomAction = new QAction(this);
	randomAction ->setIcon(QIcon(":/images/randomPlay.png"));

	//音频显示
	audioShowMenu = new QMenu(this);
	audioShowMenu ->setTitle(tr("音频显示"));
	audioShowMenu ->setIcon(QIcon(":/images/audioShow.png"));

	//频谱显示
	spectrumAction = new QAction(this);

	//波形显示
	formAction = new QAction(this);

	//创建系统托盘图标
	trayIcon = new QSystemTrayIcon(QIcon(":/images/CZPlayer.png"), this);
	trayIcon ->show();
	trayIcon ->showMessage(tr("CZPlayer"), tr("欢迎登陆www.highway-9.com"));
	trayIcon ->setToolTip(tr("CZPlayer"));

	//创建菜单，系统托盘图标右击出现的菜单
	trayMenu = new QMenu();
	trayMenu ->addAction(quitAction);
	trayMenu ->addAction(unlockLrcAction);
	trayMenu ->addSeparator();
	trayMenu ->addAction(pauseAction);
	trayMenu ->addAction(stopAction);
	trayMenu ->addSeparator();
	trayMenu ->addAction(lastAction);
	trayMenu ->addAction(nextAction);
	trayIcon ->setContextMenu(trayMenu);//在系统托盘上添加菜单
	trayIcon ->show();

	//小界面播放器
	minMusicWidget = new MinMusicWidget(this);
	//播放列表
	playList = new MusicList(this);
	musicListWidget = new MusicListWidget(playList, &m_mapMusicRows, this);
	//歌词
	lrc = new MusicLrc();
	//搜索面板
	searchPanel = new SearchPanel(this);
	//设置界面
	configDialog = new ConfigDialog(this);
	//关于CZPlayer
	aboutPanel = new AboutPanel;
	//全局热键设置
	globalHotKey = new GlobalHotKey(this);

	//设置歌词属性
	ClientBaseOperating::initLrcPropery();
	//开启全局热键
	ClientBaseOperating::initHotKeyPropery();

	//开启设备监听线程
	hotpugWatcherThread = new HotplugWatcherThread;
	//hotpugWatcherThread ->start();//在线程内部已经start

	//设置一个定时器
	timer = new QTimer(this);
	timer ->start(30);
	shortCutIimer = new QTimer(this);
	shortCutIimer ->start(500);
	upDateTimer = new QTimer(this);
	upDateTimer ->start(1000);

	//从数据库中读出信息放入音乐列表中
	this ->readFromDb();

	//设置组件大小和位置
	albumImage ->setGeometry(25, 23, 110, 103);
	m_spectrograph ->setGeometry(155, 47, 190, 78);
	m_waveform ->setGeometry(155, 47, 190, 78);
	InfoLabel ->setGeometry(153, 69, 240, 39);
	timeLabel ->setGeometry(295, 23, 82, 18);
	currentMusicLabel ->setGeometry(170, 4, 290, 11);
	showButton ->setGeometry(365, 55, 17, 17);
	modeButton ->setGeometry(365, 80, 18, 15);

	preButton ->setGeometry(33, 164, 25, 25);
	playButton ->setGeometry(83, 164, 25, 25);
	endButton ->setGeometry(133, 164, 25, 25);
	nextButton ->setGeometry(183, 164, 25, 25);

	seekSlider ->setGeometry(22, 137, 361, 14);
	openFileButton ->setGeometry(152, 21, 49, 23);
	musicListButton ->setGeometry(201, 20, 35, 24);
	lrcButton ->setGeometry(236, 21, 44, 23);
	volumeButton ->setGeometry(249, 170, 16, 16);
	volumeSlider ->setGeometry(273, 170, 108, 15);
}

//信号与槽
void Widget::connectUI()
{
	connect(openFileButton, SIGNAL(clicked()), this, SLOT(slot_OpenFile()));									//打开音乐文件
	connect(musicListButton, SIGNAL(clicked()), this, SLOT(slot_OpenMusicList()));								//打开音乐列表界面
	connect(lrcButton, SIGNAL(clicked()), this, SLOT(slot_OpenMusicLrc()));										//打开歌词
	connect(showButton, SIGNAL(clicked()), this, SLOT(slot_OpenSearchPanel()));								//打开歌曲下载界面
	connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
		this, SLOT(slot_TrayIconActivated(QSystemTrayIcon::ActivationReason)));									//推盘图标被激活后进行处理
	connect(playList, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(slot_TableDoubleClicked(int, int)));		//鼠标左键双击播放列表
	connect(playList, SIGNAL(sig_PlayListClean()), this, SLOT(slot_ClearSources()));							//清空媒体源
	connect(playList, SIGNAL(sig_RowSelected(int, QString)), this, SLOT(slot_DelSelectSource(int, QString)));	//删除选中的源
	connect(lrc, SIGNAL(sig_LrcLocked()), this, SLOT(slot_ShowLrcLocked()));									//显示歌词已经锁定信息
	connect(lrc, SIGNAL(sig_LrcUnlocked()), this, SLOT(slot_ShowLrcUnlocked()));								//显示歌词已经解锁信息
	connect(unlockLrcAction, SIGNAL(triggered()), this, SLOT(slot_UnlockLrc()));								//解锁歌词
	connect(playButton, SIGNAL(clicked()), this, SLOT(slot_SetPlayPaused()));									//播放/暂停
	connect(endButton, SIGNAL(clicked()), this, SLOT(slot_SetStop()));											//停止
	connect(preButton, SIGNAL(clicked()), this, SLOT(slot_SetPre()));											//上一首
	connect(nextButton, SIGNAL(clicked()), this, SLOT(slot_SetNext()));											//下一首
	connect(m_engine, SIGNAL(sig_Finished()), this, SLOT(slot_Finished()));										//播放完成
	connect(minAction, SIGNAL(triggered()), this, SLOT(slot_MinToTray()));										//最小化
	connect(closeAction, SIGNAL(triggered()), this, SLOT(slot_CloseAllProgress()));								//退出程序
	connect(quitAction, SIGNAL(triggered()), this, SLOT(slot_CloseAllProgress()));								//退出程序
	connect(pauseAction, SIGNAL(triggered()), this, SLOT(slot_SetPlayPaused()));								//暂停(播放)
	connect(stopAction, SIGNAL(triggered()), this, SLOT(slot_SetStop()));										//停止
	connect(lastAction, SIGNAL(triggered()), this, SLOT(slot_SetPre()));										//上一首
	connect(nextAction, SIGNAL(triggered()), this, SLOT(slot_SetNext()));										//下一首
	connect(timer, SIGNAL(timeout()), this, SLOT(slot_TimeOut()));												//设置一个定时器
	connect(shortCutIimer, SIGNAL(timeout()), this, SLOT(slot_ShortCutsetDisable()));							//设置全局热键禁用/启用
	connect(upDateTimer, SIGNAL(timeout()), this, SLOT(slot_UpdateTime()));										//更新时间和歌词
	connect(openFileAction, SIGNAL(triggered()), this, SLOT(slot_OpenFile()));									//打开音乐文件
	connect(openMusicListAction, SIGNAL(triggered()), this, SLOT(slot_OpenMusicList()));						//打开音乐播放列表
	connect(openSearchPanelAnimation, SIGNAL(triggered()), this, SLOT(slot_OpenSearchPanel()));				//打开音乐下载列表
	connect(openLrcAction, SIGNAL(triggered()), this, SLOT(slot_OpenMusicLrc()));								//显示桌面歌词
	connect(configAction, SIGNAL(triggered()), this, SLOT(slot_ConfigCZPlayer()));								//设置
	connect(aboutAction, SIGNAL(triggered()), this, SLOT(slot_AboutCZPlayer()));								//关于CZPlayer
	connect(aboutQtAction, SIGNAL(triggered()), this, SLOT(slot_AboutQt()));									//关于Qt
	connect(minMusicWidget ,SIGNAL(sig_ShowMusicPanel()), this, SLOT(slot_ShowMusicPanel()));					//显示主界面
	connect(musicListWidget, SIGNAL(sig_AddMusic()), this, SLOT(slot_OpenFile()));								//添加歌曲
	connect(musicListWidget, SIGNAL(sig_AddMusicList()), this, SLOT(slot_AddMusicList()));						//添加播放列表
	connect(this, SIGNAL(sig_ShowMainWidget()), minMusicWidget, SLOT(slot_OpenMainWidget()));					//发送到迷你界面显示主界面
	connect(volumeButton, SIGNAL(clicked()), this, SLOT(slot_SetMute()));										//设置静音
    connect(volumeSlider, SIGNAL(valueChanged(int)), this, SLOT(slot_SetVolume(int)));							//设置声音
	connect(modeButton, SIGNAL(clicked()), this, SLOT(slot_ShowModeMenu()));									//显示模式菜单
	connect(cycleAction, SIGNAL(triggered()), this, SLOT(slot_CyclePlay()));									//循环播放
	connect(randomAction, SIGNAL(triggered()), this, SLOT(slot_RandomPlay()));									//随机播放
	connect(spectrumAction, SIGNAL(triggered()), this, SLOT(slot_SpectrumShow()));								//频谱显示
	connect(formAction, SIGNAL(triggered()), this, SLOT(slot_FormShow()));										//波形显示

	////状态改变
	//connect(m_engine, SIGNAL(stateChanged(QAudio::State,QAudio::State)), this, SLOT(slot_StateChanged(QAudio::State,QAudio::State)));
	////格式改变
	connect(m_engine, SIGNAL(formatChanged(const QAudioFormat &)), this, SLOT(slot_FormatChanged(const QAudioFormat &)));
	////显示信息
	//connect(m_engine, SIGNAL(infoMessage(QString, int)), this, SLOT(slot_InfoMessage(QString, int)));
	////提示错误消息
	//connect(m_engine, SIGNAL(errorMessage(QString, QString)), this, SLOT(slot_ErrorMessage(QString, QString)));	//错误信息
	////显示信息
	//connect(m_spectrograph, SIGNAL(infoMessage(QString, int)), this, SLOT(slot_InfoMessage(QString, int)));

	//频谱改变
	connect(m_engine, SIGNAL(spectrumChanged(long long, long long, const FrequencySpectrum &)), this, SLOT(slot_SpectrumChanged(long long, long long, const FrequencySpectrum &)));
	//波形改变
	connect(m_engine, SIGNAL(bufferChanged(long long, long long, const QByteArray &)), m_waveform, SLOT(bufferChanged(long long, long long, const QByteArray &)));
	connect(m_engine, SIGNAL(recordPositionChanged(long long)), m_waveform, SLOT(audioPositionChanged(long long)));
	connect(m_engine, SIGNAL(playPositionChanged(long long)), m_waveform, SLOT(audioPositionChanged(long long)));
}


//从数据库中读出音乐文件信息放入音乐播放列表
void Widget::readFromDb()
{
	QStringList list;
	DBModule::readFromDb(list);
	if (list.isEmpty())
	{
		return;
	}
	this ->loadMusicList(list);	//加载歌曲列表
}

//设置一个定时器
void Widget::slot_TimeOut()
{
    if (lrc ->text().isEmpty())
    {
        lrc ->stopLrcMask();
    }

    //如果播放列表为空，则播放按钮不能用
    if (playList ->rowCount() == 0)
    {
		playButton ->setEnabled(false);
		endButton  ->setEnabled(false);
		preButton ->setEnabled(false);
		nextButton ->setEnabled(false);

		minMusicWidget ->minPlayButton ->setEnabled(false);
		minMusicWidget ->minPauseButton ->setEnabled(false);
		minMusicWidget ->minStopButton ->setEnabled(false);
		minMusicWidget ->minNextButton ->setEnabled(false);
		minMusicWidget ->minLastButton ->setEnabled(false);

		pauseAction ->setIcon(QIcon(":/images/trayPlay.png"));
		pauseAction ->setText(tr("播放"));
		pauseAction ->setEnabled(false);
		stopAction ->setEnabled(false);
		lastAction ->setEnabled(false);
		nextAction ->setEnabled(false);
    }
    else
    {
        playButton ->setEnabled(true);
        endButton  ->setEnabled(true);
        preButton ->setEnabled(true);
        nextButton ->setEnabled(true);

        minMusicWidget ->minPlayButton ->setEnabled(true);
        minMusicWidget ->minPauseButton ->setEnabled(true);
        minMusicWidget ->minStopButton ->setEnabled(true);
        minMusicWidget ->minNextButton ->setEnabled(true);
        minMusicWidget ->minLastButton ->setEnabled(true);

        pauseAction ->setEnabled(true);
        stopAction ->setEnabled(true);
        lastAction ->setEnabled(true);
        nextAction ->setEnabled(true);
    }

	if (nPos == -290)
	{
		nPos = 400;
	}
	currentMusicLabel ->setGeometry(nPos, 7, 290, 11);
	nPos = nPos - 1;
}

//选项设置
void Widget::slot_ConfigOptions()
{
    if (configDialog ->isHidden())
    {
        this ->slot_ConfigCZPlayer();
    }
}

//显示/隐藏主界面
void Widget::slot_ShowHideMainWidget()
{
    this ->slot_MinToTray();
    if (this ->isHidden())
    {
        this ->slot_TrayIconActivated(QSystemTrayIcon::Trigger);
    }
}

//迷你模式/正常窗口
void Widget::slot_MinMainWidget()
{
    this ->slot_OpenMinWidget();
    if (this ->isHidden())
    {
        emit sig_ShowMainWidget();//显示主界面
    }
}

//打开音乐文件
void Widget::slot_OpenMusicfile()
{
    this ->slot_OpenFile();
}

//显示/隐藏下载列表
void Widget::slot_ShowDownload()
{
    this ->slot_OpenSearchPanel();
}

//显示/隐藏歌曲列表
void Widget::slot_ShowMusicList()
{
    this ->slot_OpenMusicList();
}

//显示/隐藏桌面歌词
void Widget::slot_ShowLrc()
{
    this ->slot_OpenMusicLrc();
}

//播放/暂停
void Widget::slot_PlayPause()
{
    this ->slot_SetPlayPaused();
}

//上一首
void Widget::slot_Last()
{
    this ->slot_SetPre();
}

//下一首
void Widget::slot_Next()
{
    this ->slot_SetNext();
}

//打开文件
void Widget::slot_OpenFile()
{
    //可以同时打开多个音频文件
#ifdef _WIN32_WINNT
    QString WINPATH = CZPlayerConfig::getValue("MUSICDIR_WIN").toString();
    QStringList list = QFileDialog::getOpenFileNames(this, tr("打开音乐文件"),
                                                     WINPATH, tr("music Files(*.wav)"));
#else
    QString X11PATH =  QDir::homePath() + CZPlayerConfig::getValue("MUSICDIR_X11").toString();
    QStringList list = QFileDialog::getOpenFileNames(this, tr("打开音乐文件"),
                                                     X11PATH, tr("music Files(*.wav)"));
#endif

    if (list.isEmpty())
    {
        return;
    }
	this ->loadMusicList(list);	//加载歌曲列表
}

//添加播放列表
void Widget::slot_AddMusicList()
{
	//只能打开一个播放列表文件
#ifdef _WIN32_WINNT
	QString WINPATH = CZPlayerConfig::getValue("MUSICDIR_WIN").toString();
	QString fileName = QFileDialog::getOpenFileName(this, tr("载入播放列表"),
		WINPATH, tr("music List(*.m3u *.pls)"));
#else
	QString X11PATH =  QDir::homePath() + CZPlayerConfig::getValue("MUSICDIR_X11").toString();
	QString fileName = QFileDialog::getOpenFileName(this, tr("载入播放列表"),
		WINPATH, tr("music List(*.m3u *.pls)"));
#endif

	if (fileName != "")
	{
		QString strSuffixName = fileName.right(3);//得到后缀名
		////获取当前媒体源列表的大小
		//int index = sources.size();
		char buf[256];
		string sBuf;
		int i = 0;
		int n = 1;
		ifstream file(fileName.toStdString());
		while (!file.eof())
		{
			file.getline(buf, 256, '\n');
			sBuf = buf;

			if (i == 0)		//读取表头
			{
				if (sBuf != "#EXTM3U" && sBuf != "[playlist]")
				{
					QMessageBox::information(NULL, tr("信息"), tr("不是有效的m3u或pls文件！"), QMessageBox::Yes);
					return;
				}
			}
			else
			{
				if ((strSuffixName == "m3u" || strSuffixName == "M3U") && i % 2 == 0)
				{
					this ->loadMusicList(QString::fromStdString(sBuf));	//加载歌曲列表
				}
				else if ((strSuffixName == "pls" || strSuffixName == "PLS") && i == 1 + (n - 1) * 3)
				{
					if (!QString::fromStdString(sBuf).contains("NumberOfEntries"))
					{
						++n;
						if (QString::fromStdString(sBuf).contains("="))
						{
							QStringList list = QString::fromStdString(sBuf).split("=");
							QString filePath = list.at(1);
							this ->loadMusicList(filePath);	//加载歌曲列表
						}
					}
				}
			}
			++i;
		}
		file.close();
	}
}

//加载歌曲列表
void Widget::loadMusicList( const QStringList &list )
{
	foreach (QString filePath, list)
	{
		this ->loadMusicList(filePath);
	}
}

//加载歌曲列表
void Widget::loadMusicList( const QString &filePath )
{
	//序号
	QTableWidgetItem *rowItem = new QTableWidgetItem;
	rowItem ->setTextAlignment(Qt::AlignCenter);
	rowItem ->setFont(QFont("微软雅黑", 10));

	//歌曲
	QString fileName = QFileInfo(filePath).baseName();
	QTableWidgetItem *fileNameItem = new QTableWidgetItem(fileName);
	fileNameItem ->setTextAlignment(Qt::AlignCenter);
	fileNameItem ->setFont(QFont("微软雅黑", 10));

	//时间
	WavFile wavFile;
	int nMusicTime = wavFile.getMusicTime(filePath);
	QTime time(0, nMusicTime / 60, nMusicTime % 60);
	QTableWidgetItem *timeItem = new QTableWidgetItem(time.toString("mm:ss"));
	timeItem ->setTextAlignment(Qt::AlignCenter);
	timeItem ->setFont(QFont("微软雅黑", 10));

	//插入播放列表
	int currentRows = playList ->rowCount();//返回列表中的行数
	playList ->insertRow(currentRows);//从播放列表中的当前行插入
	rowItem ->setText(QString::number(currentRows + 1));
	playList ->setItem(currentRows, 0, rowItem);
	playList ->setItem(currentRows, 1, fileNameItem);
	playList ->setItem(currentRows, 2, timeItem);

	//更改时间列间隔
	if (currentRows + 1 <= 12)
	{
		playList ->horizontalHeader() ->resizeSection(3, 80);
	}
	else
	{
		playList ->horizontalHeader() ->resizeSection(3, 65);
	}

	//将音乐信息加入vector
	m_mapMusicRows.insert(make_pair(fileName, currentRows));
	m_mapMusicFilePath.insert(make_pair(fileName, filePath));

	//放入数据库
	DBModule::insertLine(fileName, time.toString("mm:ss"), filePath);
}

//开启小界面
void Widget::slot_OpenMinWidget()
{
    if (minMusicWidget ->isHidden())
    {
        QPropertyAnimation *minWidgetdAnimation = new QPropertyAnimation(minMusicWidget, "windowOpacity");
        minWidgetdAnimation ->setDuration(500);
        minWidgetdAnimation ->setStartValue(0);
        minWidgetdAnimation ->setEndValue(1);
        minWidgetdAnimation ->start();
        minMusicWidget ->show();

        QPropertyAnimation *mainWidgetdAnimation = new QPropertyAnimation(this, "windowOpacity");
        mainWidgetdAnimation ->setDuration(500);
        mainWidgetdAnimation ->setStartValue(1);
        mainWidgetdAnimation ->setEndValue(0);
        mainWidgetdAnimation ->start();
        connect(mainWidgetdAnimation, SIGNAL(finished()), this, SLOT(slot_HideMainWidget()));

        QPropertyAnimation *searchPanelAnimation = new QPropertyAnimation(searchPanel, "windowOpacity");
        searchPanelAnimation ->setDuration(500);
        searchPanelAnimation ->setStartValue(1);
        searchPanelAnimation ->setEndValue(0);
        searchPanelAnimation ->start();
        connect(searchPanelAnimation, SIGNAL(finished()), this, SLOT(slot_HideSearchPanel()));

        QPropertyAnimation *musicListAnimation = new QPropertyAnimation(musicListWidget, "windowOpacity");
        musicListAnimation ->setDuration(500);
        musicListAnimation ->setStartValue(1);
        musicListAnimation ->setEndValue(0);
        musicListAnimation ->start();
        connect(musicListAnimation, SIGNAL(finished()), this, SLOT(slot_HideMusicList()));
    }
}

//打开播放列表
void Widget::slot_OpenMusicList()
{
    if (musicListWidget ->isHidden())
    {
        musicListWidget ->move(frameGeometry().bottomLeft());//显示在主界面的下方
        QPropertyAnimation *musicListAnimation = new QPropertyAnimation(musicListWidget, "windowOpacity");
        musicListAnimation ->setDuration(500);
        musicListAnimation ->setStartValue(0);
        musicListAnimation ->setEndValue(1);
        musicListAnimation ->start();
        musicListWidget ->show();
        openMusicListAction ->setText(tr("关闭播放列表"));
        musicListShowFlag = 1;
    }
    else
    {
        QPropertyAnimation *musicListAnimation = new QPropertyAnimation(musicListWidget, "windowOpacity");
        musicListAnimation ->setDuration(500);
        musicListAnimation ->setStartValue(1);
        musicListAnimation ->setEndValue(0);
        musicListAnimation ->start();
        connect(musicListAnimation, SIGNAL(finished()), this, SLOT(slot_HideMusicList()));
        openMusicListAction ->setText(tr("打开播放列表"));
        musicListShowFlag = 0;
    }
}

//打开歌词
void Widget::slot_OpenMusicLrc()
{
    if (lrc ->isHidden())
    {
        //lrc ->show();
        lrc ->showNormal();
        openLrcAction ->setText(tr("关闭桌面歌词"));
        minMusicWidget ->minMusicLrcAction ->setText(tr("关闭桌面歌词"));
    }
    else
    {
        lrc ->hide();
        openLrcAction ->setText(tr("显示桌面歌词"));
        minMusicWidget ->minMusicLrcAction ->setText(tr("显示桌面歌词"));
    }
}

//打开下载歌曲界面
void Widget::slot_OpenSearchPanel()
{
    if (searchPanel ->isHidden())
    {
        searchPanel ->move(frameGeometry().topRight());//显示在主界面的右方
        QPropertyAnimation *searchPanelAnimation = new QPropertyAnimation(searchPanel, "windowOpacity");
        searchPanelAnimation ->setDuration(500);
        searchPanelAnimation ->setStartValue(0);
        searchPanelAnimation ->setEndValue(1);
        searchPanelAnimation ->start();
        searchPanel ->show();
        openSearchPanelAnimation ->setText(tr("关闭搜索面板"));
        showButton ->setToolTip(tr("关闭搜索面板"));
        showButton ->setStyleSheet("QPushButton:!hover{border-image: url(:/images/hideButton.png);}"
                                   "QPushButton:hover{border-image: url(:/images/hideButton2.png);}"
                                   "QPushButton:pressed{border-image: url(:/images/hideButton3.png);}");
        musicDownloadShowFlag = 1;
    }
    else
    {
        QPropertyAnimation *searchPanelAnimation = new QPropertyAnimation(searchPanel, "windowOpacity");
        searchPanelAnimation ->setDuration(500);
        searchPanelAnimation ->setStartValue(1);
        searchPanelAnimation ->setEndValue(0);
        searchPanelAnimation ->start();
        connect(searchPanelAnimation, SIGNAL(finished()), this, SLOT(slot_HideSearchPanel()));
        openSearchPanelAnimation ->setText(tr("打开搜索面板"));
        showButton ->setToolTip(tr("打开搜索面板"));
        showButton ->setStyleSheet("QPushButton:!hover{border-image: url(:/images/showButton.png);}"
                                   "QPushButton:hover{border-image: url(:/images/showButton2.png);}"
                                   "QPushButton:pressed{border-image: url(:/images/showButton3.png);}");
        musicDownloadShowFlag = 0;
    }
}

//隐藏搜索界面
void Widget::slot_HideSearchPanel()
{
    searchPanel ->hide();
}

//隐藏音乐播放器列表
void Widget::slot_HideMusicList()
{
    musicListWidget ->hide();
}

//隐藏主界面
void Widget::slot_HideMainWidget()
{
    this ->hide();
}

//显示主界面
void Widget::slot_ShowMusicPanel()
{
    if (musicDownloadShowFlag == 1)
    {
        this ->slot_OpenSearchPanel();
    }
    if (musicListShowFlag == 1)
    {
        this ->slot_OpenMusicList();
    }
}

//设置
void Widget::slot_ConfigCZPlayer()
{
    ConfigDialog::contentsWidget ->setCurrentRow(0);//选中第一页
	ClientBaseOperating::initConfigDialog();//初始化设置对话框
    configDialog ->exec();//模态显示对话框
}

//关于Qt
void Widget::slot_AboutQt()
{
    QMessageBox::aboutQt(this, "关于Qt");
}

//关于CZPlayer
void Widget::slot_AboutCZPlayer()
{
	AboutPanel::tabWidget ->setCurrentIndex(0);
	aboutPanel ->exec();//模态显示对话框
}

//解析LRC歌词，在stateChanged()函数的State::Play处(解析当前播放歌的歌词)
//和slot_Finished()函数中调用了该函数(开始解析下一首歌的歌词)
void Widget::resolveLrc()
{
	lrcMap.clear();

	//加载专辑图片
	if (m_strCurrentMusicName.contains(" - "))//包含该字符串（格式为：光辉岁月 - 黄家驹）
	{
		//打开专辑图片
		QStringList ablumList = m_strCurrentMusicName.split(" - ");
		QString name = ablumList.at(1);//歌曲名
		QString artist = ablumList.at(0);//歌手

#ifdef _WIN32_WINNT
		QString albumImagesName = CZPlayerConfig::getValue("ALBUMDIR_WIN").toString();
#else
		QString albumImagesName = QDir::homePath() + CZPlayerConfig::getValue("ALBUMDIR_X11").toString();
#endif

		QString albumFileName = albumImagesName + "/" + m_strCurrentMusicName + ".jpg";
		QFile fileAlbum(albumFileName);
		if (fileAlbum.open(QIODevice::ReadOnly))//打开文件成功
		{
			//由于下载出错，没能下载完
			if (fileAlbum.size() <  4500)
			{
				//下载专辑图片
				AlbumThread *albumThread = new AlbumThread(name, artist, albumImage);
				albumThread ->start();//开启一个线程
			}
			else
			{
				albumImage ->setPixmap(QPixmap(albumFileName));
			}
		}
		else
		{
			//下载专辑图片
			AlbumThread *albumThread = new AlbumThread(name, artist, albumImage);
			albumThread ->start();//开启一个线程
		}
	}
	else
	{
		albumImage ->setPixmap(QPixmap(tr(":/images/albumImage.png")));
	}

#ifdef _WIN32_WINNT
	QString lrcDirName = CZPlayerConfig::getValue("LRCDIR_WIN").toString();
#else
	QString lrcDirName = QDir::homePath() + CZPlayerConfig::getValue("LRCDIR_X11").toString();
#endif
	QString lrcFileName = lrcDirName + "/" + m_strCurrentMusicName + ".lrc";
	m_lrcFileName = lrcFileName;

	//打开歌词
	QFile file(lrcFileName);
	if (!file.open(QIODevice::ReadOnly))//下载歌曲文件
	{
		lrc ->setText(m_strCurrentMusicName + tr("----正在下载歌词文件!"));

		if (m_strCurrentMusicName.contains(" - "))
		{
			QStringList list = m_strCurrentMusicName.split(" - ");
			QString musicName = list.at(1);//歌曲名
			QString musicArtist = list.at(0);//歌手

			//下载歌词
			LrcThread *lrcThread = new LrcThread(musicName, musicArtist, m_lrcFileName, lrc);
			lrcThread ->start();//开启一个线程
			connect(lrcThread, SIGNAL(sig_ResolveLrc()), this, SLOT(slot_ResolveLrc()));
			return;
		}
	}

	/*解析歌词*/

	QString allText = QString(file.readAll());
	file.close();

	//将歌词安行分为歌词列表
	QStringList lrcLines = allText.split("\n");

	//这个是时间标签的格式[00:05.54]
	//正则表达式\d{2}表示匹配两个数字
	QRegExp rx("\\[\\d{2}:\\d{2}\\.\\d{2}\\]");

	foreach (QString oneLine, lrcLines)
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

			//把每一行的时间和歌词插入到lrcMap中
			lrcMap.insert(totalTime, temp);
			pos += rx.matchedLength();
			pos = rx.indexIn(oneLine, pos);//匹配全部
		}
	}

	//如果lrcMap为空
	if (lrcMap.isEmpty())
	{
		lrc ->setText(m_strCurrentMusicName + tr("----歌词文件内容错误!"));
		return;
	}
}

//调用解析歌词
void Widget::slot_ResolveLrc()
{
	this ->resolveLrc();//解析当前下载歌词
}

//播放/暂停
void Widget::slot_SetPlayPaused()
{
	if (m_state == Widget::Play)							//当前状态为播放状态
	{
		m_engine ->suspend();							//暂停播放
		this ->setState(Pause);							//设置播放状态
	}
	else if (m_state == Widget::Pause)						//当前状态为暂停状态
	{
		m_engine ->slot_StartPlayback();				//开始播放
		this ->setState(Play);							//设置播放状态
	}
	else if (m_state == Widget::Stop)						//当前状态为停止状态
	{
		this ->reset();									//重置
		m_engine ->loadFile(m_strCurrentFilePath);		//播放引擎加载文件
		m_engine ->slot_StartPlayback();				//开始播放
		this ->setState(Play);							//设置状态
	}
	else if (m_state == Widget::NoState)				//当前状态为无状态
	{
		QTableWidgetItem *item = playList ->item(0, 1);
		map<QString, QString>::iterator iter2 = m_mapMusicFilePath.find(item ->text());
		if (iter2 != m_mapMusicFilePath.end())
		{
			this ->reset();									//重置
			nTime = 0;				

			m_engine ->loadFile(iter2 ->second);			//播放引擎加载文件
			m_engine ->slot_StartPlayback();				//开始播放
			this ->setState(Play);							//设置状态
			m_strCurrentMusicName = item ->text();			//当前播放的歌曲名
			m_strCurrentFilePath = iter2 ->second;			//当前播放的歌曲路径
			m_nCurrentMusicRow = 0;							//当前播放的歌曲行号
			m_strCurrentMusicTime = playList ->item(m_nCurrentMusicRow, 2) ->text();//当前播放的歌曲时间
			this ->stateChanged();							//状态改变，更新界面
			this ->rowChanged();							//选中播放媒体行
			playList ->setCurrentMusicRow(m_nCurrentMusicRow);	//设置当前播放歌曲行号
		}
	}
	this ->stateChanged();								//状态改变，更新界面
}

//播放
void Widget::slot_SetPlay()
{
	if (m_state == Widget::Pause)						//当前状态为暂停状态
	{
		m_engine ->slot_StartPlayback();				//开始播放
		this ->setState(Play);							//设置播放状态
	}
	else if (m_state == Widget::Stop)					//当前状态为停止状态
	{
		this ->reset();									//重置
		m_engine ->loadFile(m_strCurrentFilePath);		//播放引擎加载文件
		m_engine ->slot_StartPlayback();				//开始播放
		this ->setState(Play);							//设置状态
	}
	else if (m_state == Widget::NoState)				//当前状态为无状态
	{
		QTableWidgetItem *item = playList ->item(0, 1);
		map<QString, QString>::iterator iter2 = m_mapMusicFilePath.find(item ->text());
		if (iter2 != m_mapMusicFilePath.end())
		{
			this ->reset();									//重置
			nTime = 0;				

			m_engine ->loadFile(iter2 ->second);			//播放引擎加载文件
			m_engine ->slot_StartPlayback();				//开始播放
			this ->setState(Play);							//设置状态
			m_strCurrentMusicName = item ->text();			//当前播放的歌曲名
			m_strCurrentFilePath = iter2 ->second;			//当前播放的歌曲路径
			m_nCurrentMusicRow = 0;							//当前播放的歌曲行号
			m_strCurrentMusicTime = playList ->item(m_nCurrentMusicRow, 2) ->text();//当前播放的歌曲时间
			this ->stateChanged();							//状态改变，更新界面
			this ->rowChanged();							//选中播放媒体行
			playList ->setCurrentMusicRow(m_nCurrentMusicRow);	//设置当前播放歌曲行号
		}
	}
	this ->stateChanged();								//状态改变，更新界面
}

//暂停
void Widget::slot_SetPause()
{
	if (m_state == Widget::Play)							//当前状态为播放状态
	{
		m_engine ->suspend();							//暂停播放
		this ->setState(Pause);							//设置播放状态
	}
	this ->stateChanged();								//状态改变，更新界面
}

//停止播放
void Widget::slot_SetStop()
{
	if (m_state == Widget::Play || m_state == Widget::Pause)
	{
		this ->reset();
		this ->setState(Stop);
		this ->stateChanged();							//状态改变，更新界面
	}
}

//播放上一首
void Widget::slot_SetPre()
{
	albumImage ->setPixmap(QPixmap(tr("")));
	albumImage ->setMovie(movie);
	movie ->start();

	//循环播放模式
	if (m_mode == Widget::cyclePlay)
	{
		if (m_nCurrentMusicRow - 1 != -1)
		{
			QTableWidgetItem *item = playList ->item(m_nCurrentMusicRow - 1, 1);
			map<QString, QString>::iterator iter2 = m_mapMusicFilePath.find(item ->text());
			if (iter2 != m_mapMusicFilePath.end())
			{
				this ->reset();									//重置
				nTime = 0;

				m_engine ->loadFile(iter2 ->second);			//播放引擎加载文件
				m_engine ->slot_StartPlayback();				//开始播放
				this ->setState(Play);							//设置状态
				m_strCurrentMusicName = item ->text();			//当前播放的歌曲名
				m_strCurrentFilePath = iter2 ->second;			//当前播放的歌曲路径
				m_nCurrentMusicRow = m_nCurrentMusicRow - 1;	//当前播放的歌曲行号
				m_strCurrentMusicTime = playList ->item(m_nCurrentMusicRow, 2) ->text();//当前播放的歌曲时间
				this ->stateChanged();							//状态改变，更新界面
				this ->rowChanged();							//选中播放媒体行
				playList ->setCurrentMusicRow(m_nCurrentMusicRow);	//设置当前播放歌曲行号
			}
		}
		else	//播放最后一首
		{
			QTableWidgetItem *item = playList ->item(playList ->rowCount() - 1, 1);
			map<QString, QString>::iterator iter2 = m_mapMusicFilePath.find(item ->text());
			if (iter2 != m_mapMusicFilePath.end())
			{
				this ->reset();									//重置
				nTime = 0;

				m_engine ->loadFile(iter2 ->second);			//播放引擎加载文件
				m_engine ->slot_StartPlayback();				//开始播放
				this ->setState(Play);							//设置状态
				m_strCurrentMusicName = item ->text();			//当前播放的歌曲名
				m_strCurrentFilePath = iter2 ->second;			//当前播放的歌曲路径
				m_nCurrentMusicRow = playList ->rowCount() - 1;	//当前播放的歌曲行号
				m_strCurrentMusicTime = playList ->item(m_nCurrentMusicRow, 2) ->text();//当前播放的歌曲时间
				this ->stateChanged();							//状态改变，更新界面
				this ->rowChanged();							//选中播放媒体行
				playList ->setCurrentMusicRow(m_nCurrentMusicRow);	//设置当前播放歌曲行号
			}
		}
	}
	//随机播放
	else if (m_mode == Widget::randomPlay)
	{
		int nRow = qrand() % playList ->rowCount();				//生成随机播放的行数
		QTableWidgetItem *item = playList ->item(nRow, 1);
		map<QString, QString>::iterator iter2 = m_mapMusicFilePath.find(item ->text());
		if (iter2 != m_mapMusicFilePath.end())
		{
			this ->reset();									//重置
			nTime = 0;				

			m_engine ->loadFile(iter2 ->second);			//播放引擎加载文件
			m_engine ->slot_StartPlayback();				//开始播放
			this ->setState(Play);							//设置状态
			m_strCurrentMusicName = item ->text();			//当前播放的歌曲名
			m_strCurrentFilePath = iter2 ->second;			//当前播放的歌曲路径
			m_nCurrentMusicRow = nRow;						//当前播放的歌曲行号
			m_strCurrentMusicTime = playList ->item(m_nCurrentMusicRow, 2) ->text();//当前播放的歌曲时间
			this ->stateChanged();							//状态改变，更新界面
			this ->rowChanged();							//选中播放媒体行
			playList ->setCurrentMusicRow(m_nCurrentMusicRow);	//设置当前播放歌曲行号
		}
	}
}

//播放下一首
void Widget::slot_SetNext()
{
	albumImage ->setPixmap(QPixmap(tr("")));
	albumImage ->setMovie(movie);
	movie ->start();

	//循环播放模式
	if (m_mode == Widget::cyclePlay)
	{
		if (m_nCurrentMusicRow + 1 != playList ->rowCount())
		{
			QTableWidgetItem *item = playList ->item(m_nCurrentMusicRow + 1, 1);
			map<QString, QString>::iterator iter2 = m_mapMusicFilePath.find(item ->text());
			if (iter2 != m_mapMusicFilePath.end())
			{
				this ->reset();									//重置
				nTime = 0;				

				m_engine ->loadFile(iter2 ->second);			//播放引擎加载文件
				m_engine ->slot_StartPlayback();				//开始播放
				this ->setState(Play);							//设置状态
				m_strCurrentMusicName = item ->text();			//当前播放的歌曲名
				m_strCurrentFilePath = iter2 ->second;			//当前播放的歌曲路径
				m_nCurrentMusicRow = m_nCurrentMusicRow + 1;	//当前播放的歌曲行号
				m_strCurrentMusicTime = playList ->item(m_nCurrentMusicRow, 2) ->text();//当前播放的歌曲时间
				this ->stateChanged();							//状态改变，更新界面
				this ->rowChanged();							//选中播放媒体行
				playList ->setCurrentMusicRow(m_nCurrentMusicRow);	//设置当前播放歌曲行号
			}
		}
		else	//重新循环列表
		{
			QTableWidgetItem *item = playList ->item(0, 1);
			map<QString, QString>::iterator iter2 = m_mapMusicFilePath.find(item ->text());
			if (iter2 != m_mapMusicFilePath.end())
			{
				this ->reset();									//重置
				nTime = 0;				

				m_engine ->loadFile(iter2 ->second);			//播放引擎加载文件
				m_engine ->slot_StartPlayback();				//开始播放
				this ->setState(Play);							//设置状态
				m_strCurrentMusicName = item ->text();			//当前播放的歌曲名
				m_strCurrentFilePath = iter2 ->second;			//当前播放的歌曲路径
				m_nCurrentMusicRow = 0;							//当前播放的歌曲行号
				m_strCurrentMusicTime = playList ->item(m_nCurrentMusicRow, 2) ->text();//当前播放的歌曲时间
				this ->stateChanged();							//状态改变，更新界面
				this ->rowChanged();							//选中播放媒体行
				playList ->setCurrentMusicRow(m_nCurrentMusicRow);	//设置当前播放歌曲行号
			}
		}
	}
	//随机播放
	else if (m_mode == Widget::randomPlay)
	{
		int nRow = qrand() % playList ->rowCount();				//生成随机播放的行数
		QTableWidgetItem *item = playList ->item(nRow, 1);
		map<QString, QString>::iterator iter2 = m_mapMusicFilePath.find(item ->text());
		if (iter2 != m_mapMusicFilePath.end())
		{
			this ->reset();									//重置
			nTime = 0;				

			m_engine ->loadFile(iter2 ->second);			//播放引擎加载文件
			m_engine ->slot_StartPlayback();				//开始播放
			this ->setState(Play);							//设置状态
			m_strCurrentMusicName = item ->text();			//当前播放的歌曲名
			m_strCurrentFilePath = iter2 ->second;			//当前播放的歌曲路径
			m_nCurrentMusicRow = nRow;						//当前播放的歌曲行号
			m_strCurrentMusicTime = playList ->item(m_nCurrentMusicRow, 2) ->text();//当前播放的歌曲时间
			this ->stateChanged();							//状态改变，更新界面
			this ->rowChanged();							//选中播放媒体行
			playList ->setCurrentMusicRow(m_nCurrentMusicRow);	//设置当前播放歌曲行号
		}
	}
}

//鼠标左键双击音乐播放列表选中行
void Widget::slot_TableDoubleClicked(int row, int /*column*/)
{
	albumImage ->setPixmap(QPixmap(tr("")));
	albumImage ->setMovie(movie);
	movie ->start();

	QTableWidgetItem *item = playList ->item(row, 1);
	map<QString, QString>::iterator iter = m_mapMusicFilePath.find(item ->text());
	if (iter != m_mapMusicFilePath.end())
	{
		this ->reset();									//重置
		nTime = 0;

		m_engine ->loadFile(iter ->second);				//播放引擎加载文件
		m_engine ->slot_StartPlayback();				//开始播放
		this ->setState(Play);							//设置状态
		m_strCurrentMusicName = item ->text();			//当前播放的歌曲名
		m_strCurrentFilePath = iter ->second;			//当前播放的歌曲路径
		m_nCurrentMusicRow = row;						//当前播放的歌曲行号
		m_strCurrentMusicTime = playList ->item(m_nCurrentMusicRow, 2) ->text();//当前播放的歌曲时间
		this ->stateChanged();							//状态改变，更新界面
		playList ->setCurrentMusicRow(m_nCurrentMusicRow);	//设置当前播放歌曲行号
	}
}

//播放完成
void Widget::slot_Finished()
{
	this ->slot_SetNext();	//播放下一首
}

//选中当前播放媒体源的行
void Widget::rowChanged()
{
	playList ->selectRow(m_nCurrentMusicRow);
}

//清空媒体源并且删除数据库全部数据和map
void Widget::slot_ClearSources()
{
	//停止当前播放
	this ->slot_SetStop();

	if (!DBModule::clearDB())//清空数据库
	{
		QMessageBox::information(NULL, tr("信息"), tr("清空数据库失败！"), QMessageBox::Yes);
	}

    m_mapMusicRows.clear();
    albumImage ->setPixmap(QPixmap(tr(":/images/albumImage.png")));
	currentMusicLabel ->setText(tr("Welecome to CZPlayer!"));
}

//删除数据库相应的数据
void Widget::slot_DelSelectSource(int row, QString musicName)
{
    //如果删除的当前行为正在播放的那一行，则停止该音乐的播放
	if (row == m_nCurrentMusicRow)
	{
		this ->slot_SetStop();
	}
	if (!DBModule::delLine(musicName))//删除一行
	{
		QMessageBox::information(NULL, tr("信息"), tr("删除该歌曲失败！"), QMessageBox::Yes);
		return;
	}
    //更新map
    this ->updateMap();
}

//更新map
void Widget::updateMap()
{
    m_mapMusicRows.clear();
    for (int index = 0; index < playList ->rowCount(); ++index)
    {
        QTableWidgetItem *musicNameItem = playList ->item(index, 1);
        QString musicName = musicNameItem ->text();//歌曲名
        m_mapMusicRows.insert(make_pair(musicName, index));
    }
}

//当媒体播放快结束时，会发送aboutToFininsh()信号
//void Widget::slot_AboutMusicFinish()
//{
//    int index = sources.indexOf(mediaObject ->currentSource()) + 1;
//    if (sources.size() > index)
//    {
//        mediaObject ->setCurrentSource(sources.at(index));
//        mediaObject ->play();
//        lrc ->stopLrcMask();//停止歌词遮罩
//        resolveLrc(sources.at(index).fileName());//解析下一首歌的歌词
//    }
//    else
//    {
//        mediaObject ->enqueue(sources.at(0));
//        mediaObject ->play();
//        lrc ->stopLrcMask();//停止歌词遮罩
//        resolveLrc(sources.at(0).fileName());//解析下一首歌的歌词
//        QString fileName = QFileInfo(sources.at(0).fileName()).baseName();
//    }
//}

//播放状态改变(更新界面)
void Widget::stateChanged()
{
	//当新状态为播放状态时，更改一些控件的状态
	if (m_state == Widget::Play)
	{
		playButton ->setStyleSheet("QPushButton:!hover{border-image: url(:/images/stopButton.png);}"
			"QPushButton:hover{border-image: url(:/images/stopButton2.png);}"
			"QPushButton:pressed{border-image: url(:/images/stopButton3.png);}");
		playButton ->setToolTip(tr("暂停"));
		pauseAction ->setIcon(QIcon(":/images/trayPause.png"));
		pauseAction ->setText(tr("暂停"));
		InfoLabel ->setVisible(false);
		trayIcon ->setToolTip(tr("正在播放:%1").arg(m_strCurrentMusicName));
		currentMusicLabel ->setText(tr("正在播放：") + m_strCurrentMusicName);
		this ->resolveLrc();//解析正在播放的歌曲的歌词
	}
	else if (m_state == Widget::Pause)	//新状态为暂停状态
	{
		playButton ->setStyleSheet("QPushButton:!hover{border-image: url(:/images/playButton.png);}"
			"QPushButton:hover{border-image: url(:/images/playButton2.png);}"
			"QPushButton:pressed{border-image: url(:/images/playButton3.png);}");
		playButton ->setToolTip(tr("播放"));
		pauseAction ->setIcon(QIcon(":/images/trayPlay.png"));
		pauseAction ->setText(tr("播放"));
		InfoLabel ->setText(false);
		trayIcon ->setToolTip(tr("CZPlayer"));
		currentMusicLabel ->setText(tr("暂停播放：") + m_strCurrentMusicName);
		lrc ->stopLrcMask();
		lrc ->setText(m_strCurrentMusicName);
	}
	else if (m_state == Widget::Stop)	//新状态为停止播放
	{
		playButton ->setStyleSheet("QPushButton:!hover{border-image: url(:/images/playButton.png);}"
			"QPushButton:hover{border-image: url(:/images/playButton2.png);}"
			"QPushButton:pressed{border-image: url(:/images/playButton3.png);}");
		playButton ->setToolTip(tr("播放"));
		pauseAction ->setIcon(QIcon(":/images/trayPlay.png"));
		pauseAction ->setText(tr("播放"));
		InfoLabel ->setVisible(true);
		trayIcon ->setToolTip(tr("CZPlayer"));
		InfoLabel ->setText(tr("Welcome to CZPlayer!"));
		currentMusicLabel ->setText(tr("停止播放：") + m_strCurrentMusicName);
		lrc ->stopLrcMask();
		lrc ->setText(tr("CZPlayer"));
	}
	this ->slot_UpdateTime();	//更新时间和歌词
}

//更新时间和歌词
void Widget::slot_UpdateTime()
{
	//当新状态为播放状态时，更改一些控件的状态
	if (m_state == Widget::Play)
	{
		//更新滚动条
		QTime qTime;
		int ntotalTime = qTime.secsTo(QTime::fromString(m_strCurrentMusicTime, "mm:ss"));
		seekSlider ->setRange(0, ntotalTime);
		seekSlider ->setValue(nTime);

		//更新时间
		 QTime currentTime(0, nTime / 60, nTime % 60);
		 timeLabel ->setText(currentTime.toString("mm:ss") + "/" + m_strCurrentMusicTime);
		 if (currentTime.toString("mm:ss") != m_strCurrentMusicTime)
		 {
			++nTime;
		 }
	}
	else if (m_state == Widget::Pause)	//新状态为暂停状态
	{
		//更新滚动条
		QTime qTime;
		int ntotalTime = qTime.secsTo(QTime::fromString(m_strCurrentMusicTime, "mm:ss"));
		seekSlider ->setRange(0, ntotalTime);
		seekSlider ->setValue(nTime);

		//更新时间
		QTime currentTime(0, nTime / 60, nTime % 60);
		timeLabel ->setText(currentTime.toString("mm:ss") + "/" + m_strCurrentMusicTime);
	}
	else if (m_state == Widget::Stop)	//新状态为停止播放
	{
		seekSlider ->setValue(0);
		timeLabel ->setText(tr("00:00/00:00"));
		nTime = 0;
	}

	//获取当前时间对应的歌词
	if (!lrcMap.isEmpty())
	{
		//获取当前时间在歌词中的前后两个时间点
		long long previousTime = 0;
		long long laterTime = 0;

		//keys()方法返回lrcMap列表
		foreach (long long value, lrcMap.keys())
		{
			if (nTime >= value / 1000)
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
		QString currentLrc = lrcMap.value(previousTime);

		// 如果是新的一行歌词，那么重新开始显示歌词遮罩
		if (currentLrc != lrc ->text())
		{
			lrc ->setText(currentLrc);
			long long intervalTime = laterTime - previousTime;//时间间隔
			lrc ->startLrcMask(intervalTime);//开启歌词遮罩
		}
	}
	else
	{
		//lrc ->setText(m_strCurrentMusicName + tr("----未找到歌词文件!"));
		lrc ->setText("Welcome to CZPlayer!");
	}
}

//最小化到托盘
void Widget::slot_MinToTray()
{
    if (musicListWidget ->isVisible())
    {
        QPropertyAnimation *musicListAnimation = new QPropertyAnimation(musicListWidget, "windowOpacity");
        musicListAnimation ->setDuration(500);
        musicListAnimation ->setStartValue(1);
        musicListAnimation ->setEndValue(0);
        musicListAnimation ->start();
        connect(musicListAnimation, SIGNAL(finished()), this, SLOT(slot_HideMusicList()));
    }
    if (searchPanel ->isVisible())
    {
        QPropertyAnimation *searchPanelAnimation = new QPropertyAnimation(searchPanel, "windowOpacity");
        searchPanelAnimation ->setDuration(500);
        searchPanelAnimation ->setStartValue(1);
        searchPanelAnimation ->setEndValue(0);
        searchPanelAnimation ->start();
        connect(searchPanelAnimation, SIGNAL(finished()), this, SLOT(slot_HideSearchPanel()));
    }
    if (this ->isVisible())
    {
        QPropertyAnimation *mainWidgetAnimation = new QPropertyAnimation(this, "windowOpacity");
        mainWidgetAnimation ->setDuration(500);
        mainWidgetAnimation ->setStartValue(1);
        mainWidgetAnimation ->setEndValue(0);
        mainWidgetAnimation ->start();
        connect(mainWidgetAnimation, SIGNAL(finished()), this, SLOT(slot_HideMainWidget()));
        trayIcon ->showMessage(tr("CZPlayer"), tr("单击我重新回到主界面"));
    }
}

//关闭所有程序
void Widget::slot_CloseAllProgress()
{
    //    mediaObject ->stop();
    //    sources.clear();

    QPropertyAnimation *mainWidgetAnimation = new QPropertyAnimation(this, "windowOpacity");
    mainWidgetAnimation ->setDuration(500);
    mainWidgetAnimation ->setStartValue(1);
    mainWidgetAnimation ->setEndValue(0);
    mainWidgetAnimation ->start();
    connect(mainWidgetAnimation, SIGNAL(finished()), this, SLOT(close()));

    QPropertyAnimation *musicListAnimation = new QPropertyAnimation(musicListWidget, "windowOpacity");
    musicListAnimation ->setDuration(500);
    musicListAnimation ->setStartValue(1);
    musicListAnimation ->setEndValue(0);
    musicListAnimation ->start();
    connect(musicListAnimation, SIGNAL(finished()), this, SLOT(close()));

    QPropertyAnimation *searchPanelAnimation = new QPropertyAnimation(searchPanel, "windowOpacity");
    searchPanelAnimation ->setDuration(500);
    searchPanelAnimation ->setStartValue(1);
    searchPanelAnimation ->setEndValue(0);
    searchPanelAnimation ->start();
    connect(searchPanelAnimation, SIGNAL(finished()), this, SLOT(close()));

    QPropertyAnimation *lrcAnimation = new QPropertyAnimation(lrc, "windowOpacity");
    lrcAnimation ->setDuration(500);
    lrcAnimation ->setStartValue(1);
    lrcAnimation ->setEndValue(0);
    lrcAnimation ->start();
    connect(lrcAnimation, SIGNAL(finished()), this, SLOT(close()));

    minMusicWidget ->close();
    //    QPropertyAnimation *minWidgetAnimation = new QPropertyAnimation(minMusicWidget, "windowOpacity");
    //    minWidgetAnimation ->setDuration(500);
    //    minWidgetAnimation ->setStartValue(1);
    //    minWidgetAnimation ->setEndValue(0);
    //    minWidgetAnimation ->start();
    //    connect(minWidgetAnimation, SIGNAL(finished()), this, SLOT(close()));
}

//系统托盘图标被激活
void Widget::slot_TrayIconActivated(QSystemTrayIcon::ActivationReason activationReason)
{
    if (activationReason == QSystemTrayIcon::Trigger)
    {
        QPropertyAnimation *mainWidgetAnimation = new QPropertyAnimation(this, "windowOpacity");
        mainWidgetAnimation ->setDuration(500);
        mainWidgetAnimation ->setStartValue(0);
        mainWidgetAnimation ->setEndValue(1);
        mainWidgetAnimation ->start();
        this ->show();//显示主界面
        this ->slot_ShowMusicPanel();
    }
}

//最小化
void Widget::slot_ShowMinSize()
{
    this ->showMinimized();
    musicListWidget ->showMinimized();
    searchPanel ->showMinimized();
}

//显示歌词已经锁定信息
void Widget::slot_ShowLrcLocked()
{
    trayIcon ->showMessage(tr("提示"), tr("你的桌面歌词已经锁定,点击播放器图标右键可以解锁"));
}

//显示歌词已经解锁信息
void Widget::slot_ShowLrcUnlocked()
{
    trayIcon ->showMessage(tr("提示"), tr("你的桌面歌词已经解锁"));
}

//解锁歌词
void Widget::slot_UnlockLrc()
{
    lrc ->unlockLrc();//解锁歌词
}

////定位到当前歌曲
//void Widget::locateCurrentMusic()
//{
//    int index = sources.indexOf(mediaObject ->currentSource());
//    playList ->selectRow(index);
//}

//设置全局热键禁用/启用
void Widget::slot_ShortCutsetDisable()
{
	if (minMusicWidget ->isHidden())
	{
		//启用以下热键
		GlobalHotKey::showHideMainWidgetDisabled(false);
		GlobalHotKey::showDownloadDisabled(false);
		GlobalHotKey::showMusicListDisabled(false);
	}
	else
	{
		//禁用以下热键
		GlobalHotKey::showHideMainWidgetDisabled(true);
		GlobalHotKey::showDownloadDisabled(true);
		GlobalHotKey::showMusicListDisabled(true);
	}
}

//主界面鼠标右键点击事件
void Widget::contextMenuEvent(QContextMenuEvent *event)
{
	if (CZPlayerConfig::getValue("AUDIOSHOW").toString() == "spectrumShow")	//频谱显示
	{
		spectrumAction ->setText(tr("显示频谱 √"));
		formAction ->setText(tr("显示波形"));
	}
	else if (CZPlayerConfig::getValue("AUDIOSHOW").toString() == "formShow")	//波形显示
	{
		spectrumAction ->setText(tr("显示频谱"));
		formAction ->setText(tr("显示波形 √"));
	}

	audioShowMenu ->addAction(spectrumAction);
	audioShowMenu ->addAction(formAction);

    QMenu menu;
    menu.addAction(closeAction);
    menu.addAction(minAction);
    menu.addAction(QIcon(":/images/minMainWidget.png"), tr("迷你模式"), this, SLOT(slot_OpenMinWidget()));
	menu.addMenu(audioShowMenu);
    menu.addSeparator();
    menu.addAction(openFileAction);
    menu.addAction(openMusicListAction);
    menu.addAction(openSearchPanelAnimation);
    menu.addAction(openLrcAction);
    menu.addSeparator();
    menu.addAction(configAction);
	menu.addAction(aboutAction);
    menu.addAction(aboutQtAction);
    menu.exec(event ->globalPos());//globalPos()为当前鼠标的位置坐标
    //menu.exec(QCursor::pos());
    event ->accept();
}

//拖进事件
void Widget::dragEnterEvent(QDragEnterEvent *event)
{
    QList<QUrl> urls = event ->mimeData() ->urls();
    foreach (QUrl url, urls)
    {
        QString filePath = url.toLocalFile();
        QStringList fileList = filePath.split('.');
        QString kzName = fileList.at(1);
        if (kzName == "wav" || kzName == "WAV" || kzName == "m3u" || kzName == "M3U" || kzName == "pls" || kzName == "PLS")
        {
            event ->accept();
        }
        else
        {
            event ->ignore();
        }
    }
}

//放下事件
void Widget::dropEvent(QDropEvent *event)
{
    QList<QUrl> urls = event ->mimeData() ->urls();
    if (urls.isEmpty())
    {
        return;
    }

	foreach (QUrl url, urls)
	{
		QString filePath = url.toLocalFile();

		QString strSuffixName = filePath.right(3);//得到后缀名
		//加载播放列表
		if (strSuffixName == "m3u" || strSuffixName == "M3U" || strSuffixName == "pls" || strSuffixName == "PLS")
		{
			char buf[256];
			string sBuf;
			int i = 0;
			int n = 1;
			ifstream file(filePath.toStdString());
			while (!file.eof())
			{
				file.getline(buf, 256, '\n');
				sBuf = buf;

				if (i == 0)		//读取表头
				{
					if (sBuf != "#EXTM3U" && sBuf != "[playlist]")
					{
						QMessageBox::information(NULL, tr("信息"), tr("不是有效的m3u或pls文件！"), QMessageBox::Yes);
						return;
					}
				}
				else
				{
					if ((strSuffixName == "m3u" || strSuffixName == "M3U") && i % 2 == 0)
					{
						this ->loadMusicList(QString::fromStdString(sBuf));	//加载歌曲列表
					}
					else if ((strSuffixName == "pls" || strSuffixName == "PLS") && i == 1 + (n - 1) * 3)
					{
						if (!QString::fromStdString(sBuf).contains("NumberOfEntries"))
						{
							++n;
							if (QString::fromStdString(sBuf).contains("="))
							{
								QStringList list = QString::fromStdString(sBuf).split("=");
								QString filePath = list.at(1);
								this ->loadMusicList(filePath);	//加载歌曲列表
							}
						}
					}
				}
				++i;
			}
			file.close();
		}
		else	//加载单首歌曲
		{
			this ->loadMusicList(filePath);
		}
	}

    event ->accept();
}

//重写paintEvent,添加背景图片
void Widget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QPixmap backgroundImage;
    backgroundImage.load(":/images/mainBg3.png");

    //先通过pix的方法获得图片的过滤掉透明的部分得到的图片，作为loginPanel的不规则边框
    this ->setMask(backgroundImage.mask());
    painter.drawPixmap(0, 0, 400, 212, backgroundImage);

	////正弦表
	//static const int sineTable[16] = { 0, 38, 71, 92, 100, 92, 71, 38,	0, -38, -71, -92, -100, -92, -71, -38};

	//QFontMetrics metrics(font());	//字体指标
	//int x = (430 - metrics.width(lrcText)) / 2;
	//int y = (250 + metrics.ascent() - metrics.descent()) / 2;//上升 - 降落
	//QColor color;

	//for (int i = 0; i < lrcText.size(); ++i)
	//{
	//	int index = (step + i) % 16;
	//	color.setHsv((15 - index) * 16, 255, 191);
	//	painter.setPen(color);
	//	painter.drawText(x, y - ((sineTable[index] * metrics.height()) / 400), QString(lrcText[i]));
	//	x += metrics.width(lrcText[i]);
	//}

    event ->accept();
}

//重写mousePressEvent和mouseMoveEvent实现窗体的移动
void Widget::mousePressEvent(QMouseEvent *event)
{
    if (event ->button() == Qt::LeftButton)
    {
        dragPosition = event ->globalPos() - frameGeometry().topLeft();
        event ->accept();
    }
}

//重写mousePressEvent和mouseMoveEvent实现窗体的移动
void Widget::mouseMoveEvent(QMouseEvent *event)
{
    if (event ->buttons() == Qt::LeftButton)
    {
        this ->move(event ->globalPos() - dragPosition);//移动窗体
        musicListWidget ->move(frameGeometry().bottomLeft());//移动窗体的时候音乐播放列表跟着移动
        searchPanel ->move(frameGeometry().topRight());//移动窗体的时候音乐下载界面跟着移动
        event ->accept();
    }
}

//设置播放状态
void Widget::setState( State state )
{
	m_state = state;
}

//设置播放模式
void Widget::setMode( Mode mode )
{
	m_mode = mode;
}

//重置
void Widget::reset()
{
	m_engine ->reset();			//重置播放引擎
	m_spectrograph ->reset();	//重置频谱绘制引擎
	m_waveform ->reset();		//重置波形绘制引擎
}

//状态改变
//void Widget::slot_StateChanged( QAudio::State mode, QAudio::State state )
//{
//
//}

//audio格式改变
void Widget::slot_FormatChanged( const QAudioFormat &format )
{
	if (QAudioFormat() != format) 
	{
		//初始化波形绘制引擎
		m_waveform ->initialize(format, WaveformTileLength, WaveformWindowDuration);
	}
}

//频谱改变
void Widget::slot_SpectrumChanged( long long position, long long length, const FrequencySpectrum &spectrum )
{
	m_spectrograph ->spectrumChanged(spectrum);	//改变频谱
}

//信息
void Widget::slot_InfoMessage( const QString &message, int timeoutMs )
{

}

void Widget::slot_ErrorMessage( const QString &heading, const QString &detail )
{

}

//设置声音
void Widget::slot_SetVolume(int value)
{
	if (CZPlayerConfig::getValue("SETMUTE").toString() == "true")
	{
		volumeButton ->setToolTip(tr("静音"));
		volumeButton ->setStyleSheet("QPushButton:!hover{border-image: url(:/images/soundButton.png);}"
			"QPushButton:hover{border-image: url(:/images/soundButton2.png);}"
			"QPushButton:pressed{border-image: url(:/images/soundButton3.png);}");
	}
	SoundControl::SetVolume(0, value);
	CZPlayerConfig::setValue("VOLUMEVALUE", QString::number(value));
	volumeSlider ->setToolTip(tr("当前音量：%1%").arg(value));
}

//设置静音
void Widget::slot_SetMute()
{
	if (CZPlayerConfig::getValue("SETMUTE").toString() == "false")
	{
		SoundControl::SetVolume(0, 0);	//静音
		volumeButton ->setToolTip(tr("恢复音量"));
		volumeButton ->setStyleSheet("QPushButton:!hover{border-image: url(:/images/soundButton4.png);}"
			"QPushButton:hover{border-image: url(:/images/soundButton5.png);}"
			"QPushButton:pressed{border-image: url(:/images/soundButton6.png);}");
		CZPlayerConfig::setValue("SETMUTE", "true");
	}
	else
	{
		SoundControl::SetVolume(0, volumeSlider ->value());	//恢复
		volumeButton ->setToolTip(tr("静音"));
		volumeButton ->setStyleSheet("QPushButton:!hover{border-image: url(:/images/soundButton.png);}"
			"QPushButton:hover{border-image: url(:/images/soundButton2.png);}"
			"QPushButton:pressed{border-image: url(:/images/soundButton3.png);}");
		CZPlayerConfig::setValue("SETMUTE", "false");
	}
}

//显示模式菜单
void Widget::slot_ShowModeMenu()
{
	if (ClientBaseOperating::initPlayMode() == "cyclePlay")	//循环播放
	{
		cycleAction ->setText(tr("循环播放 √"));
		randomAction ->setText(tr("随机播放"));
	}
	else if (ClientBaseOperating::initPlayMode() == "randomPlay")	//随机播放
	{
		cycleAction ->setText(tr("循环播放"));
		randomAction ->setText(tr("随机播放 √"));
	}

	QMenu menu;
	menu.addAction(cycleAction);
	menu.addAction(randomAction);
	menu.exec(QCursor::pos());
}

//循环播放
void Widget::slot_CyclePlay()
{
	CZPlayerConfig::setValue("PLAYMODE", "cyclePlay");
	cycleAction ->setText(tr("循环播放 √"));
	randomAction ->setText(tr("随机播放"));
	modeButton ->setStyleSheet("QPushButton:!hover{border-image: url(:/images/cyclePlay.png);}"
		"QPushButton:hover{border-image: url(:/images/cyclePlay2.png);}"
		"QPushButton:pressed{border-image: url(:/images/cyclePlay3.png);}");
	modeButton ->setToolTip(tr("循环播放"));
	this ->setMode(cyclePlay);					//设置循环播放模式
}

//随机播放
void Widget::slot_RandomPlay()
{
	CZPlayerConfig::setValue("PLAYMODE", "randomPlay");
	cycleAction ->setText(tr("循环播放"));
	randomAction ->setText(tr("随机播放 √"));
	modeButton ->setStyleSheet("QPushButton:!hover{border-image: url(:/images/randomPlay.png);}"
		"QPushButton:hover{border-image: url(:/images/randomPlay2.png);}"
		"QPushButton:pressed{border-image: url(:/images/randomPlay3.png);}");
	modeButton ->setToolTip(tr("随机播放"));
	this ->setMode(randomPlay);					//设置随机播放模式
}

//显示频谱
void Widget::slot_SpectrumShow()
{
	CZPlayerConfig::setValue("AUDIOSHOW", "spectrumShow");
	spectrumAction ->setText(tr("显示频谱 √"));
	formAction ->setText(tr("显示波形"));
	m_spectrograph ->setVisible(true);
	m_waveform ->setVisible(false);
}

//显示波形
void Widget::slot_FormShow()
{
	CZPlayerConfig::setValue("AUDIOSHOW", "formShow");
	spectrumAction ->setText(tr("显示频谱 √"));
	formAction ->setText(tr("显示波形"));
	m_spectrograph ->setVisible(false);
	m_waveform ->setVisible(true);
}