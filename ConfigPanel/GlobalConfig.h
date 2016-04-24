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
 * \file   GlobalConfig.h
 * \author chengxuan   787280310@qq.com
 * \date   2015-01-18
 * \brief  全局配置头文件。
 * \version 3.0.0
 *
 * \verbatim
 * 历史
 *          3.0.0   创建,
 *          2015-01-18    by chengxuan
 *
 * \endverbatim
 *
 */

#ifndef GLOBALCONFIG_H
#define GLOBALCONFIG_H

/*! \def GLOBALCONFIG_VERSION
 *  版本控制宏，对应v3.0.0版本.
 */
#define GLOBALCONFIG_VERSION   0x030000

#include <QObject>
#include <QString>
#include <QColor>

/*歌词配置信息默认值*/
extern const QString FontName;
extern const QString FontSize;
extern const QString FontType;
extern const QColor NoPlayShang;
extern const QColor NoPlayZhong;
extern const QColor NoPlayXia;
extern const QColor PlayedShang;
extern const QColor PlayedZhong;
extern const QColor PlayedXia;
extern const QString DefaultChoiceColor;
extern const int Shadow;

/*热键配置信息默认值*/
extern const QString ConfigOptions;
extern const QString ShowHideMainWidget;
extern const QString MinMainWidget;
extern const QString OpenMusicFile;
extern const QString ShowHideMediaWidget;
extern const QString ShowHideMusicList;
extern const QString ShowHideLrc;
extern const QString PlayPause;
extern const QString PlayPre;
extern const QString PlayNext;

//经典白
extern const QColor NoPlayWhiteShang;
extern const QColor NoPlayWhiteZhong;
extern const QColor NoPlayWhiteXia;
extern const QColor PlayedWhiteShang;
extern const QColor PlayedWhiteZhong;
extern const QColor PlayedWhiteXia;

//海洋蓝
extern const QColor NoPlayBlueShang;
extern const QColor NoPlayBlueZhong;
extern const QColor NoPlayBlueXia;
extern const QColor PlayedBlueShang;
extern const QColor PlayedBlueZhong;
extern const QColor PlayedBlueXia;

//火焰红
extern const QColor NoPlayRedShang;
extern const QColor NoPlayRedZhong;
extern const QColor NoPlayRedXia;
extern const QColor PlayedRedShang;
extern const QColor PlayedRedZhong;
extern const QColor PlayedRedXia;

//酷炫黑
extern const QColor NoPlayBlackShang;
extern const QColor NoPlayBlackZhong;
extern const QColor NoPlayBlackXia;
extern const QColor PlayedBlackShang;
extern const QColor PlayedBlackZhong;
extern const QColor PlayedBlackXia;

//秋色黄
extern const QColor NoPlayYellowShang;
extern const QColor NoPlayYellowZhong;
extern const QColor NoPlayYellowXia;
extern const QColor PlayedYellowShang;
extern const QColor PlayedYellowZhong;
extern const QColor PlayedYellowXia;

//浪漫紫
extern const QColor NoPlayPurpleShang;
extern const QColor NoPlayPurpleZhong;
extern const QColor NoPlayPurpleXia;
extern const QColor PlayedPurpleShang;
extern const QColor PlayedPurpleZhong;
extern const QColor PlayedPurpleXia;

//自然绿
extern const QColor NoPlayGreenShang;
extern const QColor NoPlayGreenZhong;
extern const QColor NoPlayGreenXia;
extern const QColor PlayedGreenShang;
extern const QColor PlayedGreenZhong;
extern const QColor PlayedGreenXia;

Q_DECLARE_METATYPE(enum PlayModeType)

/*! 全局配置
 */
class GlobalConfig : public QObject
{
	Q_OBJECT

public:
	/*! 获取全局对象
	\return 返回从注册表加载信息配置对象
	*/
	static GlobalConfig* instance();

	/*! 删除全局对象
	\note 请一定在程序退出前调用 delInstance() ，否则将导致程序混乱
	*/
	static void delInstance();

	/*! 重新加载配置信息
	 * \return true代表成功，否则失败
	 */
	bool reload();

	/*! 保存配置信息
	 * \return true代表成功，否则失败
	 */
	bool save();

	/*! 是否程序正在析构，主程序调用
	 * \return isReleaseResource代表正在析构，否则没有正在析构
	 */
    void setReleaseResource(bool isReleaseResource);

	/*****************************************************************************
	 常规设置get/set函数
	*****************************************************************************/

	/*! 设置启动播放器时是否自动播放音乐
	 * \param isAutoPlayMusic true代表自动播放，否则取消
	 */
	void setAutoPlayMusic(bool isAutoPlayMusic);

	/*! 设置开机是否自动运行播放器
	 * \param isAutoRunPlayer true代表自动运行，否则取消
	 */
	void setAutoRunPlayer(bool isAutoRunPlayer);

	/*! 设置启动播放器时是否自动打招呼
	 * \param isAutoGrettings true代表自动打招呼，否则取消
	 */
	void setAutoGreetings(bool isAutoGrettings);

	/*! 设置为默认播放器
	 * \param isDefaultPlayer true代表设置为默认播放器，否则取消
	 */
	void setDefaultPlayer(bool isDefaultPlayer);

	/*! 是否启动播放器时自动播放音乐
	* \return true代表自动播放，否则不是
	*/
	bool autoPlayMusic() const;

	/*! 是否开机是否自动运行播放器
	* \return true代表自动运行，否则不是
	*/
	bool autoRunPlayer() const;

	/*! 是否启动播放器时是否自动打招呼
	* \return true代表自动打招呼，否则不是
	*/
	bool autoGreetings() const;

	/*! 是否为默认播放器
	* \return true代表默认播放器，否则不是
	*/
	bool defaultPlayer() const;

	/*****************************************************************************
	 下载设置get/set函数
	*****************************************************************************/

	/*! 设置音乐路径
	 * \param path 音乐路径
	 */
	void setMusicPath(const QString &path);

	/*! 设置专辑路径
	 * \param path 专辑路径
	 */
	void setAlbumPath(const QString &path);

	/*! 保存歌词路径
	 * \param path 歌词路径
	 */
	void setLrcPath(const QString &path);

	/*! 得到音乐路径
	* \return 音乐路径
	*/
	QString musicPath() const;

	/*! 得到专辑路径
	* \return 专辑路径
	*/
	QString albumPath() const;

	/*! 得到歌词路径
	* \return 歌词路径
	*/
	QString lrcPath() const;

	/*****************************************************************************
	 热键设置get/set函数
	*****************************************************************************/

	/*! 设置选项设置热键
	 * \param hotKey 选项设置热键
	 */
	void setSettingsHotKey(const QString &hotKey);

	/*! 设置显示/隐藏主界面热键
	 * \param hotKey 显示/隐藏主界面热键
	 */
	void setMinToTrayHotKey(const QString &hotKey);

	/*! 设置迷你模式/正常窗口热键
	 * \param hotKey 迷你模式/正常窗口热键
	 */
	void setOpenMinWidgetHotKey(const QString &hotKey);

	/*! 设置打开音乐文件热键
	 * \param hotKey 打开音乐文件热键
	 */
	void setOpenMusicFileHotKey(const QString &hotKey);

    /*! 设置显示/隐藏媒体面板热键
	 * \param hotKey 显示/隐藏下载列表热键
	 */
    void setOpenMediaWidgetHotKey(const QString &hotKey);

	/*! 设置显示/隐藏歌曲列表热键
	 * \param hotKey 显示/隐藏歌曲列表热键
	 */
	void setOpenListWidgetHotKey(const QString &hotKey);

	/*! 设置显示/隐藏桌面歌词热键
	 * \param hotKey 显示/隐藏桌面歌词热键
	 */
	void setOpenLrcHotKey(const QString &hotKey);

	/*! 设置播放/暂停热键
	 * \param hotKey 播放/暂停热键
	 */
	void setPauseHotKey(const QString &hotKey);

	/*! 设置上一首热键
	 * \param hotKey 上一首热键
	 */
	void setLastHotKey(const QString &hotKey);

	/*! 设置下一首热键
	 * \param hotKey 下一首热键
	 */
	void setNextHotKey(const QString &hotKey);

	/*! 得到选项设置热键
	 * \return 选项设置热键
	 */
	QString settingsHotKey() const;

	/*! 得到显示/隐藏主界面热键
	 * \return 显示/隐藏主界面热键
	 */
	QString minToTrayHotKey() const;

	/*! 得到迷你模式/正常窗口热键
	 * \return 迷你模式/正常窗口热键
	 */
	QString openMinWidgetHotKey() const;

	/*! 得到打开音乐文件热键
	 * \return 打开音乐文件热键
	 */
	QString openMusicFileHotKey() const;

    /*! 得到显示/隐藏媒体面板热键
	 * \return 显示/隐藏下载列表热键
	 */
    QString openMediaWidgetHotKey() const;

	/*! 得到显示/隐藏歌曲列表热键
	 * \return 显示/隐藏歌曲列表热键
	 */
	QString openListWidgetHotKey() const;

	/*! 得到显示/隐藏桌面歌词热键
	 * \return 显示/隐藏桌面歌词热键
	 */
	QString openLrcHotKey() const;

	/*! 得到播放/暂停热键改变
	 * \return 播放/暂停热键
	 */
	QString pauseHotKey() const;

	/*! 得到上一首热键改变
	 * \return 上一首热键
	 */
	QString lastHotKey() const;

	/*! 得到下一首热键改变
	 * \return 下一首热键
	 */
	QString nextHotKey() const;

	/*****************************************************************************
	 歌词设置get/set函数
	*****************************************************************************/

	/*! 设置歌词字体
	 * \param fontName 字体名称
	 */
	void setFontName(const QString &fontName);

	/*! 设置歌词字体大小
	 * \param fontSize 字体大小
	 */
	void setFontSize(const QString &fontSize);

	/*! 设置歌词类型
	 * \param fontType 歌词类型
	 */
	void setFontType(const QString &fontType);

	/*! 设置歌词没有播放上边缘颜色
	 * \param color 颜色
	 */
	void setNoPlayShang(const QColor &color);

	/*! 设置歌词没有播放中间颜色
	 * \param color 颜色
	 */
	void setNoPlayZhong(const QColor &color);

	/*! 设置歌词没有播放下边缘颜色
	 * \param color 颜色
	 */
	void setNoPlayXia(const QColor &color);

	/*! 设置歌词播放后上边缘颜色
	 * \param color 颜色
	 */
	void setPlayedShang(const QColor &color);

	/*! 设置歌词播放后中间颜色
	 * \param color 颜色
	 */
	void setPlayedZhong(const QColor &color);

	/*! 设置歌词播放后下边缘颜色
	 * \param color 颜色
	 */
	void setPlayedXia(const QColor &color);

	/*! 设置歌词默认颜色颜色
	 * \param color 颜色
	 */
	void setDefaultChoiceColor(const QString &color);

	/*! 设置歌词灰度
	 * \param shadow 灰度
	 */
	void setShadow(const int &shadow);

	/*! 得到歌词字体
	 * \return 字体名称
	 */
	QString fontName() const;

	/*! 得到歌词字体大小
	 * \return 字体大小
	 */
	QString fontSize() const;

	/*! 得到歌词类型
	 * \return 歌词类型
	 */
	QString fontType() const;

	/*! 得到歌词没有播放上边缘颜色
	 * \return 颜色
	 */
	QColor noPlayShang() const;

	/*! 得到歌词没有播放中间颜色
	 * \return 颜色
	 */
	QColor noPlayZhong() const;

	/*! 得到歌词没有播放下边缘颜色
	 * \return 颜色
	 */
	QColor noPlayXia() const;

	/*! 得到歌词播放后上边缘颜色
	 * \return 颜色
	 */
	QColor playedShang() const;

	/*! 得到歌词播放后中间颜色
	 * \return 颜色
	 */
	QColor playedZhong() const;

	/*! 得到歌词播放后下边缘颜色
	 * \return 颜色
	 */
	QColor playedXia() const;

	/*! 得到歌词默认颜色
	 * \return 颜色
	 */
	QString defaultChoiceColor() const;

	/*! 得到歌词灰度
	 * \return 灰度
	 */
	int shadow() const;

	/*****************************************************************************
	 播放控制设置get/set函数
	*****************************************************************************/

	/*! 设置是否静音
	 * \param isMute true代表静音，否则不静音
	 */
    void setMute(bool isMute);

	/*! 设置音量
	 * \param volume 音量
	 */
	void setVolume(const int &volume);

	/*! 播放模式
	 * \param playMode 播放模式
	 */
	void setPlayMode(enum PlayModeType playMode);

	/*! 是否静音
	 * \return true代表静音，否则不静音
	 */
	bool mute() const;

	/*! 得到音量
	 * \return 音量
	 */
	int volume() const;

	/*! 得到播放类型
	 * \return 播放类型
	 */
	enum PlayModeType playMode() const;

signals:
	/*****************************************************************************
	 常规设置配置信息改变发送的信号
	*****************************************************************************/

	/*! 自动播放音乐设置改变
	 * \param isAutoPlayMusic true代表自动播放，否则取消
	 */
	void sigAutoPlayMusicChanged(bool isAutoPlayMusic);

	/*! 开机是否自动运行播放器设置改变
	 * \param isAutoRunPlayer true代表自动运行，否则取消
	 */
	void sigAutoRunPlayerChanged(bool isAutoRunPlayer);

	/*! 启动播放器时是否自动打招呼设置改变
	 * \param isAutoGrettings true代表自动打招呼，否则取消
	 */
	void sigAutoGreetingsChanged(bool isAutoGrettings);

	/*! 默认播放器设置改变
	 * \param isDefaultPlayer true代表设置为默认播放器，否则取消
	 */
	void sigDefaultPlayerChanged(bool isDefaultPlayer);

	/*****************************************************************************
	 下载设置配置信息改变发送的信号
	*****************************************************************************/

	/*! 音乐路径改变
	 * \param path 音乐路径
	 */
	void sigMusicPathChanged(const QString &path);

	/*! 专辑路径改变
	 * \param path 专辑路径
	 */
	void sigAlbumPathChanged(const QString &path);

	/*! 歌词路径改变
	 * \param path 歌词路径
	 */
	void sigLrcPathChanged(const QString &path);

	/*****************************************************************************
	 热键设置配置信息改变发送的信号
	*****************************************************************************/
	/*! 选项设置热键改变
	 * \param hotKey 选项设置热键
	 */
	void sigSettingsHotKeyChanged(const QString &hotKey);

	/*! 显示/隐藏主界面热键改变
	 * \param hotKey 显示/隐藏主界面热键
	 */
	void sigMinToTrayHotKeyChanged(const QString &hotKey);

	/*! 迷你模式/正常窗口热键改变
	 * \param hotKey 迷你模式/正常窗口热键
	 */
	void sigOpenMinWidgetHotKeyChanged(const QString &hotKey);

	/*! 打开音乐文件热键改变
	 * \param hotKey 打开音乐文件热键
	 */
	void sigOpenMusicFileHotKeyChanged(const QString &hotKey);

    /*! 显示/隐藏媒体面板热键改变
	 * \param hotKey 显示/隐藏下载列表热键
	 */
    void sigOpenMediaWidgetHotKeyChanged(const QString &hotKey);

	/*! 显示/隐藏歌曲列表热键改变
	 * \param hotKey 显示/隐藏歌曲列表热键
	 */
	void sigOpenListWidgetHotKeyChanged(const QString &hotKey);

	/*! 显示/隐藏桌面歌词热键改变
	 * \param hotKey 显示/隐藏桌面歌词热键
	 */
	void sigOpenLrcHotKeyChanged(const QString &hotKey);

	/*! 播放/暂停热键改变
	 * \param hotKey 播放/暂停热键
	 */
	void sigPauseHotKeyChanged(const QString &hotKey);

	/*! 上一首热键改变
	 * \param hotKey 上一首热键
	 */
	void sigLastHotKeyChanged(const QString &hotKey);

	/*! 下一首热键改变
	 * \param hotKey 下一首热键
	 */
	void sigNextHotKeyChanged(const QString &hotKey);

	/*****************************************************************************
	 歌词设置配置信息改变发送的信号
	*****************************************************************************/
	/*! 歌词字体改变
	 * \param fontName 字体名称
	 */
	void sigFontNameChanged(const QString &fontName);

	/*! 歌词字体大小改变
	 * \param fontSize 字体大小
	 */
	void sigFontSizeChanged(const QString &fontSize);

	/*! 歌词类型改变
	 * \param fontType 歌词类型
	 */
	void sigFontTypeChanged(const QString &fontType);

	/*! 歌词没有播放上边缘颜色改变
	 * \param color 颜色
	 */
	void sigNoPlayShangChanged(const QColor &color);

	/*! 歌词没有播放中间颜色改变
	 * \param color 颜色
	 */
	void sigNoPlayZhongChanged(const QColor &color);

	/*! 歌词没有播放下边缘颜色改变
	 * \param color 颜色
	 */
	void sigNoPlayXiaChanged(const QColor &color);

	/*! 歌词播放后上边缘颜色改变
	 * \param color 颜色
	 */
	void sigPlayedShangChanged(const QColor &color);

	/*! 歌词播放后中间颜色改变
	 * \param color 颜色
	 */
	void sigPlayedZhongChanged(const QColor &color);

	/*! 歌词播放后下边缘颜色改变
	 * \param color 颜色
	 */
	void sigPlayedXiaChanged(const QColor &color);

	/*! 歌词默认颜色改变
	 * \param color 颜色
	 */
	void sigDefaultChoiceColorChanged(const QString &color);

	/*! 歌词灰度改变
	 * \param shadow 灰度
	 */
	void sigShadowChanged(const int &shadow);

	/*****************************************************************************
	播放控制配置信息改变发送的信号
	*****************************************************************************/

	/*! 是否静音设置改变
	 * \param isMute true代表静音，否则不静音
	 */
    void sigMuteChanged(bool isMute);

	/*! 音量是否改变
	 * \param volume 音量
	 */
	void sigVolumeChanged(const int &volume);

	/*! 播放类型改变
	 * \param playMode 播放类型
	 */
	void sigPlayModeChanged(enum PlayModeType playMode);

private:
	/*! 构造函数.*/
	GlobalConfig();

	/*! 析构函数.*/
	~GlobalConfig();

	/*! 创建系统配置注册表
	 * \return true代表成功，否则失败
	 */
	bool createRegedit();

	/*! 保存常规配置信息
	 * \return true代表成功，否则失败
	 */
	bool saveGeneralSettingsInfo();

	/*! 保存下载配置信息
	 * \return true代表成功，否则失败
	 */
	bool saveDownloadSettingsInfo();

	/*! 保存热键配置信息
	 * \return true代表成功，否则失败
	 */
	bool saveHotkeysSettingsInfo();

	/*! 保存歌词配置信息
	 * \return true代表成功，否则失败
	 */
	bool saveLrcSettingsInfo();

	/*! 保存播放控制配置信息
	 * \return true代表成功，否则失败
	 */
	bool savePlayControlSettingsInfo();

	/*! 重新加载常规配置信息
	 * \return true代表成功，否则失败
	 */
	bool reloadGeneralSettingsInfo();

	/*! 重新加载下载配置信息
	 * \return true代表成功，否则失败
	 */
	bool reloadDownloadSettingsInfo();

	/*! 重新加载歌词配置信息
	 * \return true代表成功，否则失败
	 */
	bool reloadLrcSettingsInfo();

	/*! 重新加载热键配置信息
	* \return true代表成功，否则失败
	*/
	bool reloadHotkeySettingsInfo();

	/*! 重新加载播放控制配置信息
	* \return true代表成功，否则失败
	*/
	bool reloadPlayControlSettingsInfo();

private:
	static GlobalConfig* m_instance;				  /*!< 全局对象*/
	bool m_isReleaseResource;						  /*!< 是否在析构资源*/

	bool m_isGeneralSettingsNeedSave;			   	  /*!< 用于检查是否需要保存的旗标*/
	bool m_isDownloadSettingsNeedSave;			   	  /*!< 用于检查是否需要保存的旗标*/
	bool m_isLrcSettingsNeedSave;			   		  /*!< 用于检查是否需要保存的旗标*/
	bool m_isHotkeySettingsNeedSave;			   	  /*!< 用于检查是否需要保存的旗标*/
	bool m_isPlayControlSettingsNeedSave;			  /*!< 用于检查是否需要保存的旗标*/

	bool m_isAutoPlayMusic;							  /*!< 启动播放器时是否自动播放音乐*/
	bool m_isAutoRunPlayer;							  /*!< 开机是否自动运行播放器*/
	bool m_isAutoGrettings;							  /*!< 启动播放器时是否自动打招呼*/
	bool m_isDefaultPlayer;							  /*!< 是否是默认播放器*/

	QString m_musicPath;							 /*!< 音乐路径*/
	QString m_albumPath;							 /*!< 专辑路径*/
	QString m_lrcPath;								 /*!< 歌词路径*/

	QString m_settingsHotKey;						 /*!< 选项设置*/
	QString m_minToTrayHotKey;						 /*!< 显示/隐藏主界面*/
	QString m_openMinWidgetHotKey;					 /*!< 迷你模式/正常窗口*/
	QString m_openMusicFileHotKey;					 /*!< 打开音乐文件*/
    QString m_openMediaWidgetHotKey;				 /*!< 显示/隐藏媒体面板*/
	QString m_openListWidgetHotKey;					 /*!< 显示/隐藏歌曲列表*/
	QString m_openLrcHotKey;						 /*!< 显示/隐藏桌面歌词*/
	QString m_pauseHotKey;							 /*!< 播放/暂停*/
	QString m_lastHotKey;							 /*!< 上一首*/
	QString m_nextHotKey;							 /*!< 下一首*/

	QString m_fontName;								/*!< 字体名称*/
	QString m_fontSize;								/*!< 字体大小*/
	QString m_fontType;								/*!< 字体类型*/
	QColor m_noPlayShang;							/*!< 没有播放上边缘颜色*/
	QColor m_noPlayZhong;							/*!< 没有播放中间颜色*/
	QColor m_noPlayXia;								/*!< 没有播放下边缘颜色*/
	QColor m_playedShang;							/*!< 播放过后的上边缘颜色*/
	QColor m_playedZhong;							/*!< 播放过后的中间颜色*/
	QColor m_playedXia;								/*!< 播放过后的下边缘颜色*/
	QString m_defaultChoiceColor;					/*!< 默认颜色*/
	int m_shadow;									/*!< 灰度*/

	bool m_isMute;									/*!< 是否静音*/
	int m_volume;									/*!< 音量大小*/
	enum PlayModeType m_playMode;					/*!< 播放模式*/
};

#endif // GLOBALCONFIG_H
