#ifndef WIDGET_H
#define WIDGET_H

//#include <Mmdeviceapi.h>
//#include <Audiopolicy.h>

#include "musiclist.h"
#include "musiclrc.h"
#include "musicdownload.h"
#include "SearchPanel.h"
#include "albumthread.h"
#include "lrcthread.h"
#include "HotplugWatcherThread.h"
#include "minmusicwidget.h"
#include "musiclistwidget.h"
#include "configdialog.h"
#include "AboutPanel.h"
#include "globalhotkey.h"
#include "head.h"

#include "engine.h"
#include "frequencyspectrum.h"
#include "spectrograph.h"
#include "waveform.h"
#include "utils.h"

class Widget : public QWidget
{
    Q_OBJECT
    
public:
    Widget();
    ~Widget();

public:
    int getMusicListShow() const;
    void setMusicListShow(int value);

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void contextMenuEvent(QContextMenuEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
	//void timerEvent(QTimerEvent *event);

signals:
    void sig_ShowMainWidget();																//显示主界面

private slots:
	void slot_UpdateTime();																	//更新时间和歌词
	void slot_Finished();																	//播放完成
    void slot_OpenMinWidget();																//开启迷你界面
    void slot_OpenFile();																	//打开音乐文件
	void slot_AddMusicList();																//添加播放列表
    void slot_OpenMusicList();																//打开音乐播放列表
    void slot_OpenMusicLrc();																//打开歌词
    void slot_OpenSearchPanel();															//打开下载歌曲界面
    void slot_HideSearchPanel();															//隐藏搜索界面
    void slot_HideMusicList();																//隐藏音乐播放器列表
    void slot_HideMainWidget();																//隐藏主界面
    void slot_ShowMusicPanel();																//显示主界面
    void slot_ConfigCZPlayer();																//设置
	void slot_AboutCZPlayer();																//关于CZPlayer
    void slot_AboutQt();																	//关于Qt
    void slot_SetPlayPaused();																//播放/暂停
	void slot_SetPlay();																	//播放
	void slot_SetPause();																	//暂停
    void slot_SetStop();																	//停止
    void slot_SetPre();																		//上一首
    void slot_SetNext();																	//下一首
    void slot_TableDoubleClicked(int row, int /*column*/);									//双击媒体库选中播放该音乐
    void slot_ClearSources();																//清空源
    void slot_DelSelectSource(int row, QString musicName);									//删除选中源和数据库相应歌曲
    void slot_TrayIconActivated(QSystemTrayIcon::ActivationReason activationReason);		//系统托盘激活处理
    void slot_ShowMinSize();																//最小化
    void slot_MinToTray();																	//最小化到推盘
    void slot_ShowLrcLocked();																//显示歌词已经锁定信息
    void slot_ShowLrcUnlocked();															//显示歌词已经解锁信息
    void slot_UnlockLrc();																	//解锁歌词
    void slot_CloseAllProgress();															//退出程序
    void slot_TimeOut();																	//定时器
	void slot_ResolveLrc();																	//解析歌词
	void slot_ShortCutsetDisable();															//设置全局热键禁用/启用
    void slot_ConfigOptions();																//选项设置
    void slot_ShowHideMainWidget();															//显示/隐藏主界面
    void slot_MinMainWidget();																//迷你模式/正常窗口
    void slot_OpenMusicfile();																//打开音乐文件
    void slot_ShowDownload();																//显示/隐藏下载列表
    void slot_ShowMusicList();																//显示/隐藏歌曲列表
    void slot_ShowLrc();																	//显示/隐藏桌面歌词
    void slot_PlayPause();																	//播放/暂停
    void slot_Last();																		//上一首
    void slot_Next();																		//下一首
	void slot_SetVolume(int value);															//设置声音
	void slot_SetMute();																	//设置静音
	void slot_ShowModeMenu();																//显示模式菜单
	void slot_CyclePlay();																	//循环播放
	void slot_RandomPlay();																	//随机播放
	void slot_SpectrumShow();																//显示频谱
	void slot_FormShow();																	//显示波形

	//void slot_StateChanged(QAudio::Mode mode, QAudio::State state);									//状态改变
	void slot_FormatChanged(const QAudioFormat &format);												//audio格式改变
	void slot_SpectrumChanged(long long position, long long length, const FrequencySpectrum &spectrum);	//频谱改变
	void slot_InfoMessage(const QString &message, int timeoutMs);										//信息
	void slot_ErrorMessage(const QString &heading, const QString &detail);	

private:
	//播放状态
	enum State
	{
		NoState,			//无状态
		Play,				//播放
		Pause,				//暂停
		Stop				//停止
	};

	//播放模式
	enum Mode
	{
		cyclePlay,			//循环播放
		randomPlay			//随机播放
	};

private:
	void createUI();				//创建界面
	void connectUI();				//信号与槽

    void resolveLrc();																		//解析歌词
	void loadMusicList(const QStringList &list);											//加载歌曲列表
	void loadMusicList(const QString &filePath);											//加载歌曲列表
    void readFromDb();																		//从数据库中读出数据显示到音乐里列表中
    void updateMap();																		//更新map
	void stateChanged();																	//播放状态改变(更新界面)
	void rowChanged();																		//选中当前播放媒体源的行

	void setState(State state);																//设置播放状态
	void setMode(Mode mode);																//设置播放模式
	void reset();																			//重置

private:
    //显示变量（0：关闭，1：开启）
    int musicListShowFlag;
    int musicDownloadShowFlag;

    QPoint dragPosition;
    QTimer *timer;
	QTimer *shortCutIimer;
	QTimer *upDateTimer;

    MinMusicWidget *minMusicWidget;							//迷你播放界面
    MusicList *playList;									//播放列表
    MusicListWidget *musicListWidget;
    MusicLrc *lrc;											//歌词
    //MusicDownload *musicDownload;							//歌曲下载
	SearchPanel *searchPanel;								//歌曲搜索面板
    ConfigDialog *configDialog;								//设置界面
	AboutPanel *aboutPanel;									//关于CZPlayer
    GlobalHotKey *globalHotKey;								//全局热键
	HotplugWatcherThread *hotpugWatcherThread;				//设备监听

    QMap<qint64, QString> lrcMap;
    map<QString, int> m_mapMusicRows;						//key：歌曲名，value：行号
	map<QString, QString> m_mapMusicFilePath;				//key：歌曲名，value：音乐路径

	State m_state;											//播放状态
	Mode m_mode;											//播放模式

	Engine *m_engine;										//播放引擎
	Spectrograph *m_spectrograph;							//频谱绘制引擎
	Waveform *m_waveform;									//波形绘制引擎

	QSlider *seekSlider;	//播放控制条
	QSlider *volumeSlider;	//音量调节条

    QLabel *albumImage;
    QLabel *timeLabel;
	QLabel *InfoLabel;
	QLabel *currentMusicLabel;

    QAction *closeAction;
    QAction *pauseAction;
    QAction *stopAction;
    QAction *lastAction;
    QAction *nextAction;
    QAction *minAction;
    QAction *quitAction;
    QAction *unlockLrcAction;
    QAction *openFileAction;
    QAction *openMusicListAction;
    QAction *openSearchPanelAnimation;
    QAction *openLrcAction;
    QAction *configAction;
	QAction *aboutAction;
    QAction *aboutQtAction;
	QAction *cycleAction;
	QAction *randomAction;
	QMenu *audioShowMenu;
	QAction *spectrumAction;
	QAction *formAction;

    QMenu *trayMenu;										//系统推盘菜单
    QSystemTrayIcon *trayIcon;								//系统托盘图标
    QMovie *movie;

    QPushButton *playButton;
    QPushButton *endButton;
    QPushButton *preButton;
    QPushButton *nextButton;
    QPushButton *openFileButton;
    QPushButton *musicListButton;
    QPushButton *lrcButton;
    QPushButton *showButton;
	QPushButton *volumeButton;
	QPushButton *modeButton;

    QString m_musicName;
    QString m_musicArtist;
    QString m_lrcFileName;
    QString m_title;
    QString m_artist;
    int readyReadDb;
	int nPos;

	QString m_strCurrentFilePath;		//当前播放歌曲的路径
	QString m_strCurrentMusicName;		//当前播放歌曲的名字
	QString m_strCurrentMusicTime;		//当前播放歌曲的时间(mm:ss)
	int m_nCurrentMusicRow;				//当前播放歌曲的行号
	int nTime;							//当前播放歌曲的时间(秒)

	//QBasicTimer baseTimer;
	//QString lrcText;
	//int step;
};

#endif // WIDGET_H
