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

#ifndef MAINWIDGET_H
#define MAINWIDGET_H

/*! \def MAINWIDGET_VERSION
 *  版本控制宏，对应v3.0.0版本.
 */
#define MAINWIDGET_VERSION   0x030000

#include <vector>
using namespace std;
#include <QWidget>
#include <QString>
#include <QMap>
#include <QPoint>
#include <QUrl>
#include <QSystemTrayIcon>
#include "signal_slot.h"
#include "StatusDefine.h"
#include "MusicInfoEntity.h"

/*****************************************************************************
  这里用来检查版本的对应
 *****************************************************************************/
#if STATUSDEFINE_VERSION < 0x030000
#error  "StatusDefine version error."
#endif

#if MUSICINFOENTITY_VERSION < 0x030000
#error  "MusicInfoEntity version error."
#endif

class QPushButton;
class QLabel;
class QMenu;
class QAction;
class QSlider;
class QTimer;
class QPaintEvent;
class QMouseEvent;
class QContextMenuEvent;
class QDragEnterEvent;
class QDropEvent;

class CBasicPlayer;
class Spectrograph;
class MinMusicWidget;
class MusicList;
class MusicListWidget;
class MusicLrc;
class MediaWidget;
class ConfigDialog;
class SharedDialog;
class AboutPanel;
class GlobalHotKey;
class LoadMusicThread;
class LoadMusicThreadEvent;
class AutoStartHandle;
class FileRelationHandle;

/*****************************************************************************
  这里用来检查版本的对应
 *****************************************************************************/

/*! 播放器主界面
*/
class MainWidget : public QWidget
{
	Q_OBJECT

public:
	 /*! 构造函数
     *  \param argvMusicPath 设备指针
	 *  \param parent 父窗口指针，默认为NULL
     */
    MainWidget(const char *argvMusicPath, QWidget *parent = NULL);

	/*! 析构函数.*/
    ~MainWidget();

protected:
	/*! 虚函数，重新实现paintEvent函数
     *  \param event QPaintEvent类型
     */
	virtual void paintEvent(QPaintEvent *event);

	/*! 虚函数，重新实现mousePressEvent函数
     *  \param event QMouseEvent类型
     */
	virtual void mousePressEvent(QMouseEvent *event);

	/*! 虚函数，重新实现mouseMoveEvent函数
     *  \param event QMouseEvent类型
     */
	virtual void mouseMoveEvent(QMouseEvent *event);

	/*! 虚函数，重新实现contextMenuEvent函数
     *  \param event QContextMenuEvent类型
     */
	virtual void contextMenuEvent(QContextMenuEvent *event);

	/*! 虚函数，重新实现dragEnterEvent函数
     *  \param event QDragEnterEvent类型
     */
	virtual void dragEnterEvent(QDragEnterEvent *event);

	/*! 虚函数，重新实现dropEvent函数
     *  \param event QDropEvent类型
     */
	virtual void dropEvent(QDropEvent *event);

signals:
	/*! 显示主界面
     */
	void sigShowMainWidget();	

	/*! 频谱改变
	 * \param vecFrequency 频谱数据
     */
	void sigSpectrumChanged(const vector<float> &vecFrequency);

	/*! 打开音乐文件 设置参数
	*  \param musicList 音乐列表信息集合
	*/
	void sigStartloadMusic(const QStringList &musicList);

	/*! 拖动音乐文件或音乐格式文件 设置参数
	*  \param urls 音乐url列表
	*/
	void sigStartloadMusic(const QList<QUrl> &urls);

	/*! 加载音乐格式文件或文件夹 设置参数
	*  \param strFileOrDir 音乐格式文件或文件夹
	*/
	void sigStartloadMusic(const QString &strFileOrDir);

	/*! 从数据库读取音乐文件 设置参数
	*/
	void sigStartloadMusic();

private slots:
	/*! 更新时间和歌词
     */
	void handleUpdateTime();																	

	/*! 开启迷你界面
     */
	void handleOpenMinWidget();																

	/*! 打开音乐文件
     */
	void handleOpenFile();																	

	/*! 添加播放列表
     */
	void handleAddMusicList();																

	/*! 添加歌曲文件夹
     */
	void handleAddMusicDir();																

	/*! 打开音乐播放列表
     */
	void handleOpenMusicList();			

	/*! 打开歌词
     */
	void handleOpenMusicLrc();																

    /*! 打开媒体界面
     */
    void handleOpenMediaWidget();

    /*! 隐藏媒体界面
     */
    void handleHideMediaWidget();

	/*! 隐藏音乐播放器列表
     */
	void handleHideMusicList();																

	/*! 隐藏主界面
     */
	void handleHideMainWidget();																

	/*! 显示主界面
     */
	void handleShowMusicPanel();																

	/*! 设置
     */
	void handleConfigCZPlayer();																

	/*! 关于CZPlayer
     */
	void handleAboutCZPlayer();		

	/*! 关于Qt
     */
	void handleAboutQt();																	

	/*! 播放/暂停
     */
	void handleSetPlayPaused();																

	/*! 播放
     */
	void handleSetPlay();																	

	/*! 暂停
     */
	void handleSetPause();																	

	/*! 停止
     */
	void handleSetStop();																	

	/*! 上一首
     */
	void handleSetPre();																		

	/*! 下一首
     */
	void handleSetNext();																	

	/*! 双击媒体库选中播放该音乐
	 *  \param row 播放列表行
	 *  \param column 播放列表列
     */
	void handleTableDoubleClicked(const int &row, const int &column);									

	/*! 清空源
     */
	void handleClearSources();																

	/*! 删除选中源和数据库相应歌曲
	*  \param row 播放列表行
	*  \param musicName 音乐名称
     */
	void handleDelSelectSource(const int &row, const QString &musicName);									

	/*! 系统托盘激活处理
	 *  \param activationReason QSystemTrayIcon::ActivationReason类型
     */
	void handleTrayIconActivated(QSystemTrayIcon::ActivationReason activationReason);		

	/*! 最小化
     */
	void handleShowMinSize();																

	/*! 最小化到推盘
     */
	void handleMinToTray();																	

	/*! 显示歌词已经锁定信息
     */
	void handleShowLrcLocked();																

	/*! 显示歌词已经解锁信息
     */
	void handleShowLrcUnlocked();															

	/*! 解锁歌词
     */
	void handleUnlockLrc();																	

	/*! 退出程序
     */
	void handleCloseAllProgress();															

	/*! 定时器
     */
	void handleTimeOut();	

	/*! 自动升级
	*/
	void handleAutoUpdate();

	/*! 选项设置
     */
	void handleConfigOptions();																

	/*! 显示/隐藏主界面
     */
	void handleShowHideMainWidget();															

	/*! 迷你模式/正常窗口
     */
	void handleMinMainWidget();																

	/*! 设置声音
	 *  \param value 声音大小
     */
	void handleSetVolume(const int &value);															

	/*! 设置静音
     */
	void handleSetMute();																	

	/*! 显示模式菜单
     */
	void handleShowModeMenu();																

	/*! 循环播放
     */
	void handleCyclePlay();																	

	/*! 随机播放
     */
	void handleRandomPlay();																	

	/*! 专辑下载完成信号
	 *  \param musicName 音乐名称
	 *  \param albumUrl 专辑Url
     */
	void handleAlbumDownloadFinished(const QString &musicName, const QString &albumUrl);				

	/*! 分享该歌曲
	 *  \param musicName 音乐名称
     */
	void handleSharedCurrentMusic(const QString &musicName);										

	/*! 加载完播放列表
     */
	void handleLoadMusicListFinished();														

	/*! 加载播放列表错误
     */
	void handleLoadMusicListError();	

	/*! 音乐目录改变
	 * \param path 音乐路径
	 */
	void handleMusicDirChanged(const QString &path);

	/*! 专辑目录改变
	 * \param path 专辑路径
	 */
	void handleAlbumDirChanged(const QString &path);

	/*! 歌词目录改变
	 * \param path 歌词路径
	 */
	void handleLrcDirChanged(const QString &path);

	/*! 加载音乐
	* \param entity 音乐信息实体
	*/
	void handleAddMusic(const MusicInfoEntity &entity);

	/*! 下载专辑状态
     *  \param fileName 音乐文件名
	 *  \param albumUrl 专辑Url
	 *  \param albumFilePath 专辑文件路径
	 *  \param status 下载状态
     */
	void handleAlbumDownloadStatus(const QString &fileName, 
								   const QString &albumUrl, 
								   const QString &albumFilePath, 
								   AlbumDownloadStatus status);

	/*! 下载歌词状态
     *  \param musicName 音乐名
	 *  \param musicArtist 音乐艺术家
	 *  \param lrcFilePath 歌词文件路径
	 *  \param status 下载状态
     */
	void handleLrcDownloadStatus(const QString &musicName, 
							    const QString &musicArtist, 
								const QString &lrcFilePath, 
								LrcDownloadStatus status);

	/*! 配置对话框关闭
	 */
	void handleConfigDialogClosed();

	/*! 是否静音设置改变
	 * \param isMute true代表静音，否则不静音
	 */
    void handleMuteChanged(bool isMute);

	/*! 音量是否改变
	 * \param volume 音量
	 */
	void handleVolumeChanged(const int &volume);

	/*! 播放类型改变
	 * \param playMode 播放类型
	 */
	void handlePlayModeChanged(enum PlayModeType playMode);

private c_slots:
	/*! 频谱改变
	 * \param vecFrequency 频谱数据
     */
	void handleSpectrumChanged(vector<float> vecFrequency);									

	/*! 播放完成
     */
	void handleFinished();																	

private:
	/*!< 播放状态*/
	enum State
	{
		NoState,			/*!< 无状态*/
		Play,				/*!< 播放*/
		Pause,				/*!< 暂停*/
		Stop				/*!< 停止*/
	};

	/*!< 播放模式*/
	enum Mode
	{
		CyclePlay,			/*!< 循环播放*/
		RandomPlay			/*!< 随机播放*/
	};

private:
	/*! 创建界面
     */
	void createUI();		

	/*! 创建菜单
     */
	void createMenus();	

	/*! 信号与槽
     */
	void createConnect();		

	/*! 创建音乐列表加载线程
	*/
	void createMusicloadThread();

	/*! 释放资源
	*/
	void releaseResource();

	/*! 保存播放控制配置信息
	*/
	void savePlayControlSettingsInfo();

	/*! 结束所有线程，在析构函数调用该函数
	*/
	void terminateAllThread();

	/*! 播放问候语
	*/
	void playGreetings();

	/*! 解析歌词
	 * \param lrcFilePath 歌词文件路径
     */
	void resolveLrc(const QString &lrcFilePath);	

	/*! 显示专辑图片
     */
	void showAlbumImage();	

	/*! 更新map
	 * \param musicName 音乐名称
     */
	void updateMap(const QString &musicName);														

	/*! 播放状态改变(更新界面)
     */
	void stateChanged();																	

	/*! 选中当前播放媒体源的行
     */
	void rowChanged();																		

	/*! 加载歌曲列表
	 * \param filePath 音乐文件路径
     */
	void loadMusicList(const QString &filePath);	

	/*! 判断该歌曲是否有效，如果有效则播放
	 * \param currentMusicName 当前播放的音乐名称
	 * \param currentMusicRow 当前播放的音乐所在媒体库行
	 */
	void findAndPlay(const QString &currentMusicName, const int &currentMusicRow);

	/*! 播放
	 * \param currentFilePath 当前播放的音乐文件路径
	 * \param currentMusicName 当前播放的音乐名称
	 * \param currentMusicRow 当前播放的音乐所在媒体库行
	 * \param currentMusicTime 当前音乐播放的时间
     */
	void play(const QString &currentFilePath, 
			  const QString &currentMusicName, 
			  const int &currentMusicRow, 
			  const QString &currentMusicTime);					

	/*! 设置播放状态
	 * \param state 播放状态
     */
	void setState(State state);																

	/*! 设置播放模式
	 * \param mode 播放模式
     */
	void setMode(Mode mode);																

	/*! 重置
     */
	void reset();																			

	/*! 播放
     */
	void play();																			

	/*! 播放
	 * \param currentFilePath 当前播放的音乐文件路径
     */
	void play(const QString &currentFilePath);											

	/*! 暂停
     */
	void pause();																			

	/*! 改变工作目录
     */
	void changeWorkDir();		

	/*! 更改专辑图片
	 * \param albumFilePath 专辑路径
     */
	void changeAlbumImage(const QString &albumFilePath);

	/*! 下载专辑
	 *  \param fileName 音乐文件名
	 *  \param musicName 音乐名称
	 *  \param musicArtist 音乐艺术家
	 *  \param albumFilePath 专辑文件路径
     */
	void downloadAlbum(const QString &fileName, 
				const QString &musicName, 
				const QString &musicArtist, 
				const QString &albumFilePath);

	/*! 下载歌词
	 *  \param musicName 音乐名称
	 *  \param musicArtist 音乐艺术家
	 *  \param lrcFilePath 歌词路径
     */
	void downloadLrc(const QString &musicName, const QString &musicArtist, const QString &lrcFilePath);

private:
	bool m_musicListShowFlag;						/*!< 是否显示播放列表旗帜*/
	bool m_musicDownloadShowFlag;					/*!< 是否显示下载列表旗帜*/

	QPoint m_dragPosition;							/*!< 坐标点*/
	QTimer *m_timer;								/*!< 定时器*/
	QTimer *m_upDateTimer;							/*!< 更新时间定时器*/

	QList<QThread *> m_albumThreads;				/*!< 专辑线程集合*/
	QList<QThread *> m_lrcThreads;					/*!< 歌词线程集合*/

	MinMusicWidget *m_minMusicWidget;				/*!< 迷你播放界面*/
	MusicList *m_playList;							/*!< 播放列表*/
	MusicListWidget *m_musicListWidget;				/*!< 播放列表*/
	MusicLrc *m_lrc;								/*!< 歌词*/
    MediaWidget *m_mediaWidget;                     /*!< 媒体面板*/
	ConfigDialog *m_configDialog;					/*!< 设置界面*/
	SharedDialog *m_sharedDialog;					/*!< 分享界面*/
	AboutPanel *m_aboutPanel;						/*!< 关于CZPlayer*/
	GlobalHotKey *m_globalHotKey;					/*!< 全局热键*/
	LoadMusicThread *m_loadMusicThread;				/*!< 音乐列表加载线程*/
	LoadMusicThreadEvent *m_loadMusicThreadEvent;	/*!< 音乐列表加载线程事件处理类*/
    AutoStartHandle *m_autoStartHandle;             /*!< 自启动处理器*/
    FileRelationHandle *m_fileRelationHandle;       /*!< 文件关联处理器*/

	QMap<qint64, QString> m_lrcMap;
	QMap<QString, int> m_mapMusicRows;				/*!< key：歌曲名，value：行号*/
	QMap<QString, QString> m_mapMusicFilePath;		/*!< key：歌曲名，value：音乐路径*/

	State m_state;									/*!< 播放状态*/
	Mode m_mode;									/*!< 播放模式*/

	CBasicPlayer *m_basicPlayer;					/*!< 播放引擎*/
	Spectrograph *m_spectrograph;					/*!< 频谱绘制引擎*/

	QSlider *m_seekSlider;							/*!< 播放控制条*/
	QSlider *m_volumeSlider;						/*!< 音量调节条*/
	bool m_isMute;									/*!< 是否静音*/

	QLabel *m_albumImage;							/*!< 专辑图片*/
	QLabel *m_timeLabel;							/*!< 显示播放时间*/
	QLabel *m_infoLabel;							/*!< 显示一些信息*/
	QLabel *m_currentMusicLabel;					/*!< 显示当前播放歌曲*/

	QAction *m_closeAction;							/*!< 关闭*/
	QAction *m_pauseAction;							/*!< 暂停*/
	QAction *m_stopAction;							/*!< 停止*/
	QAction *m_lastAction;							/*!< 上一首*/
	QAction *m_nextAction;							/*!< 下一首*/
	QAction *m_minAction;							/*!< 最小化*/
	QAction *m_quitAction;							/*!< 退出*/
	QAction *m_unlockLrcAction;						/*!< 解锁歌词*/
	QAction *m_openFileAction;						/*!< 打开文件*/
	QAction *m_openMusicListAction;					/*!< 打开播放列表*/
    QAction *m_openMediaWidgetAction;               /*!< 打开媒体面板*/
	QAction *m_openLrcAction;						/*!< 打开歌词*/
	QAction *m_autoUpdateAction;					/*!< 自动升级*/
	QAction *m_configAction;						/*!< 打开配置界面*/
	QAction *m_aboutAction;							/*!< 关于*/
	QAction *m_aboutQtAction;						/*!< 关于Qt*/
	QAction *m_cycleAction;							/*!< 循环播放*/
	QAction *m_randomAction;						/*!< 随机播放*/

	QMenu *m_trayMenu;								/*!< 系统推盘菜单*/
	QSystemTrayIcon *m_trayIcon;					/*!< 系统托盘图标*/
	QMenu *m_rightHandMenu;							/*!< 右键菜单*/
	QMenu *m_playModeMenu;							/*!< 播放模式菜单*/

	QPushButton *m_playButton;						/*!< 播放*/	
	QPushButton *m_endButton;						/*!< 停止*/	
	QPushButton *m_preButton;						/*!< 上一曲*/	
	QPushButton *m_nextButton;						/*!< 下一曲*/	
	QPushButton *m_openFileButton;					/*!< 打开文件*/	
	QPushButton *m_musicListButton;					/*!< 显示播放列表*/	
	QPushButton *m_lrcButton;						/*!< 显示歌词*/	
	QPushButton *m_showButton;						/*!< 显示下载界面*/	
	QPushButton *m_volumeButton;					/*!< 音量调节*/	
	QPushButton *m_modeButton;						/*!< 播放模式*/	

	QString m_musicName;							/*!< 歌曲名称*/	
	QString m_musicArtist;							/*!< 艺术家*/	
	QString m_title;								/*!< 标题*/	
	QString m_artist;								/*!< 艺术家*/	
	int m_pos;										/*!< 跑马灯位置*/	

	QString m_currentFilePath;						/*!< 当前播放歌曲的路径*/	
	QString m_currentMusicName;						/*!< 当前播放歌曲的名字*/	
	QString m_currentMusicTime;						/*!< 当前播放歌曲的时间(mm:ss)*/	
	int m_currentMusicRow;							/*!< 当前播放歌曲的行号*/	
	int m_time;										/*!< 当前播放歌曲的时间(秒)*/	

	bool m_isStarupLoadMusic;						/*!< 是否是启动时加载播放列表*/	
	QString m_argvMusicPath;						/*!< 启动播放器的参数*/	

	QString m_musicPath;							 /*!< 音乐路径*/
	QString m_albumPath;							 /*!< 专辑路径*/
	QString m_lrcPath;								 /*!< 歌词路径*/
};

#endif // MAINWIDGET_H
