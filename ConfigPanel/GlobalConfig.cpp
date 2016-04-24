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
 * \file   GlobalConfig.cpp
 * \author chengxuan   787280310@qq.com
 * \date   2015-01-18
 * \brief  全局配置实现文件。
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

#include "GlobalConfig.h"
#include "Global.h"
#include "TR.h"
#include <QSettings>
#include <QDir>
#include <QFileInfoList>
#ifdef CZ_DEBUG
#include <QDebug>
#endif

/*****************************************************************************
  这里用来检查版本的对应
 *****************************************************************************/
#if GLOBAL_VERSION < 0x030000
#error  "Global version error."
#endif

#if TR_VERSION < 0x030000
#error  "TR version error."
#endif

/*****************************************************************************
 静态成员变量的初始化
*****************************************************************************/
GlobalConfig* GlobalConfig::m_instance = NULL;

/*****************************************************************************
 定义全局变量
*****************************************************************************/
/*常规配置信息界面注册表项名称*/
const QString StrGeneralConfig = "GeneralConfig";
const QString StrAutoPlayMusic = "AutoPlayMusic";
const QString StrAutoRunPlayer = "AutoRunPlayer";
const QString StrAutoGreetings = "AutoGreetings";
const QString StrDefaultPlayer = "DefaultPlayer";

/*下载配置信息界面注册表项名称*/
const QString StrDownloadConfig = "DownloadConfig";
const QString StrMusicPath = "MusicPath";
const QString StrAlbumPath = "AlbumPath";
const QString StrLrcPath = "LrcPath";

/*歌词配置信息界面注册表项名称*/
const QString StrLrcConfig = "LrcConfig";
const QString StrFontName = "FontName";
const QString StrFontSize = "FontSize";
const QString StrFontType = "FontType";
const QString StrNoPlayShang = "NoPlayShang";
const QString StrNoPlayZhong = "NoPlayZhong";
const QString StrNoPlayXia = "NoPlayXia";
const QString StrPlayedShang = "PlayedShang";
const QString StrPlayedZhong = "PlayedZhong";
const QString StrPlayedXia = "PlayedXia";
const QString StrDefaultChoiceColor = "DefaultChoiceColor";
const QString StrShadow = "Shadow";

/*热键配置信息界面注册表项名称*/
const QString StrHotkeyConfig = "HotkeyConfig";
const QString StrConfigOptions = "ConfigOptions";
const QString StrShowHideMainWidget = "ShowHideMainWidget";
const QString StrMinMainWidget = "MinMainWidget";
const QString StrOpenMusicFile = "OpenMusicFile";
const QString StrShowHideMediaWidget = "ShowHideMediaWidget";
const QString StrShowHideMusicList = "ShowHideMusicList";
const QString StrShowHideLrc = "ShowHideLrc";
const QString StrPlayPause = "PlayPause";
const QString StrPlayPre = "PlayPre";
const QString StrPlayNext = "PlayNext";

/*播放控制配置信息注册表项名称*/
const QString StrPlayControlConfig = "PlayControlConfig";
const QString StrMute = "Mute";
const QString StrVolume = "Volume";
const QString StrPlayMode = "PlayMode";

/*常规配置信息默认值*/
const bool AutoPlayMusic = false;
const bool AutoRunPlayer = false;
const bool AutoGreetings = false;
const bool DefaultPlayer = false;

/*下载配置信息默认值*/
const QString MusicPath = "CZPlayer";
const QString AlbumPath = "CZPlayer/albumImages";
const QString LrcPath = "CZPlayer/lrc";

/*歌词配置信息默认值*/
const QString FontName = TR("微软雅黑");
const QString FontSize = "30";
const QString FontType = TR("常规");
const QColor NoPlayShang = QColor(14, 179, 255);
const QColor NoPlayZhong = QColor(114, 232, 255);
const QColor NoPlayXia = QColor(14, 179, 255);
const QColor PlayedShang = QColor(222, 54, 4);
const QColor PlayedZhong = QColor(255, 72, 16);
const QColor PlayedXia = QColor(222, 54, 4);
const QString DefaultChoiceColor = "";
const int Shadow = 255;

/*热键配置信息默认值*/
const QString ConfigOptions = "Ctrl+Alt+F1";
const QString ShowHideMainWidget = "Ctrl+Alt+W";
const QString MinMainWidget = "Ctrl+Alt+M";
const QString OpenMusicFile = "Ctrl+Alt+O";
const QString ShowHideMediaWidget = "Ctrl+Alt+D";
const QString ShowHideMusicList = "Ctrl+Alt+L";
const QString ShowHideLrc = "Ctrl+Alt+R";
const QString PlayPause = "Alt+Space";
const QString PlayPre = "Alt+Left";
const QString PlayNext = "Alt+Right";

//经典白
const QColor NoPlayWhiteShang = QColor(255, 255, 255);
const QColor NoPlayWhiteZhong = QColor(255, 255, 255);
const QColor NoPlayWhiteXia = QColor(255, 255, 255);
const QColor PlayedWhiteShang = QColor(255, 255, 0);
const QColor PlayedWhiteZhong = QColor(255, 255, 0);
const QColor PlayedWhiteXia = QColor(255, 255, 0);

//海洋蓝
const QColor NoPlayBlueShang = QColor(0, 52, 138);
const QColor NoPlayBlueZhong = QColor(0, 128, 192);
const QColor NoPlayBlueXia = QColor(3, 202, 252);
const QColor PlayedBlueShang = QColor(130, 247, 253);
const QColor PlayedBlueZhong = QColor(255, 255, 255);
const QColor PlayedBlueXia = QColor(3, 233, 252);

//火焰红
const QColor NoPlayRedShang = QColor(255, 172, 0);
const QColor NoPlayRedZhong = QColor(255, 0, 0);
const QColor NoPlayRedXia = QColor(170, 0, 0);
const QColor PlayedRedShang = QColor(255, 255, 164);
const QColor PlayedRedZhong = QColor(255, 255, 0);
const QColor PlayedRedXia = QColor(255, 100, 26);

//酷炫黑
const QColor NoPlayBlackShang = QColor(225, 225, 225);
const QColor NoPlayBlackZhong = QColor(106, 106, 106);
const QColor NoPlayBlackXia = QColor(0, 0, 0);
const QColor PlayedBlackShang = QColor(255, 255, 255);
const QColor PlayedBlackZhong = QColor(0, 255, 255);
const QColor PlayedBlackXia = QColor(128, 255, 255);

//秋色黄
const QColor NoPlayYellowShang = QColor(2, 166, 174);
const QColor NoPlayYellowZhong = QColor(128, 255, 255);
const QColor NoPlayYellowXia = QColor(2, 166, 174);
const QColor PlayedYellowShang = QColor(247, 132, 38);
const QColor PlayedYellowZhong = QColor(255, 255, 0);
const QColor PlayedYellowXia = QColor(255, 128, 0);

//浪漫紫
const QColor NoPlayPurpleShang = QColor(64, 0, 128);
const QColor NoPlayPurpleZhong = QColor(255, 128, 255);
const QColor NoPlayPurpleXia = QColor(64, 0, 128);
const QColor PlayedPurpleShang = QColor(255, 55, 146);
const QColor PlayedPurpleZhong = QColor(255, 243, 134);
const QColor PlayedPurpleXia = QColor(255, 55, 146);

//自然绿
const QColor NoPlayGreenShang = QColor(147, 255, 38);
const QColor NoPlayGreenZhong = QColor(70, 176, 0);
const QColor NoPlayGreenXia = QColor(0, 85, 0);
const QColor PlayedGreenShang = QColor(255, 255, 255);
const QColor PlayedGreenZhong = QColor(154, 255, 17);
const QColor PlayedGreenXia = QColor(255, 255, 0);

/*播放控制配置信息默认值*/
const bool Mute = false;
const int Volume = 100;
enum PlayModeType
{
	RandomPlay = 1,
	CyclePlay = 2
};
enum PlayModeType PlayMode = RandomPlay;

GlobalConfig::GlobalConfig()
	: m_isReleaseResource(false),
	  m_isGeneralSettingsNeedSave(false),
	  m_isDownloadSettingsNeedSave(false),
	  m_isLrcSettingsNeedSave(false),
	  m_isHotkeySettingsNeedSave(false),
	  m_isPlayControlSettingsNeedSave(false),
	  m_isAutoPlayMusic(false),
	  m_isAutoRunPlayer(false),
	  m_isAutoGrettings(false),
	  m_isDefaultPlayer(false)
{
	/*Do nothing.*/
}

GlobalConfig::~GlobalConfig()
{
	/*Do nothing.*/
}

GlobalConfig* GlobalConfig::instance()
{
	if (m_instance == NULL)
	{
		m_instance = new GlobalConfig;
	}
	/*Else do nothing, and continue.*/

	return m_instance;
}

void GlobalConfig::delInstance()
{
	if (m_instance != NULL)
	{
		m_instance->deleteLater();
		m_instance = NULL;
	}
	/*Else do nothing, and continue.*/
}

void GlobalConfig::setReleaseResource(bool isReleaseResource)
{
	m_isReleaseResource = isReleaseResource;
}

bool GlobalConfig::reload()
{
	//加载配置信息之前，先创建注册表
	if (!this->createRegedit())
	{
		return false;
	}
	/*Else do nothing, and continue.*/

	if (!this->reloadGeneralSettingsInfo())
	{
		return false;
	}
	/*Else do nothing, and continue.*/

	if (!this->reloadDownloadSettingsInfo())
	{
		return false;
	}
	/*Else do nothing, and continue.*/

	if (!this->reloadLrcSettingsInfo())
	{
		return false;
	}
	/*Else do nothing, and continue.*/

	if (!this->reloadHotkeySettingsInfo())
	{
		return false;
	}
	/*Else do nothing, and continue.*/

	if (!this->reloadPlayControlSettingsInfo())
	{
		return false;
	}
	/*Else do nothing, and continue.*/

	return true;
}

bool GlobalConfig::createRegedit()
{
	QSettings config(QSettings::UserScope, StringOrganization, StringApplication);

	config.sync();
	if ((config.status() != QSettings::NoError) || !config.isWritable())
	{
		return false;
	}
	/*Else do nothing, and continue.*/

	//常规界面配置信息
	config.beginGroup(StrGeneralConfig);
	if (config.childKeys().isEmpty())
	{
		config.setValue(StrAutoPlayMusic, AutoPlayMusic);
		config.setValue(StrAutoRunPlayer, AutoRunPlayer);
		config.setValue(StrAutoGreetings, AutoGreetings);
		config.setValue(StrDefaultPlayer, DefaultPlayer);
	}
	/*Else do nothing, and continue.*/
	config.endGroup();

	//下载界面配置信息
	config.beginGroup(StrDownloadConfig);
	if (config.childKeys().isEmpty())
	{
        QString musicPath;
        QString albumPath;
        QString lrcPath;
        bool isFound = false;
        QFileInfoList fileInfoList = QDir::drives();
        if (fileInfoList.size() > 1)
        {
            foreach (QFileInfo fileInfo, fileInfoList)
            {
                QString driveLetter = fileInfo.absoluteFilePath();
                if (!driveLetter.contains('C', Qt::CaseInsensitive))
                {
                    if (fileInfo.isWritable())
                    {
                        musicPath = driveLetter + MusicPath;
                        albumPath = driveLetter + AlbumPath;
                        lrcPath = driveLetter + LrcPath;
                        isFound = true;
                        break;
                    }
                    /*Else do nothing, and continue.*/
                }
                /*Else do nothing, and continue.*/
            }
            if (!isFound)
            {
                QString driveLetter = fileInfoList.at(0).absoluteFilePath();
                musicPath = driveLetter + MusicPath;
                albumPath = driveLetter + AlbumPath;
                lrcPath = driveLetter + LrcPath;
            }
            /*Else do nothing, and continue.*/
        }
        else
        {
            QString driveLetter = fileInfoList.at(0).absoluteFilePath();
            musicPath = driveLetter + MusicPath;
            albumPath = driveLetter + AlbumPath;
            lrcPath = driveLetter + LrcPath;
        }

        config.setValue(StrMusicPath, musicPath);
        config.setValue(StrAlbumPath, albumPath);
        config.setValue(StrLrcPath, lrcPath);
	}
	/*Else do nothing, and continue.*/
	config.endGroup();

	//歌词界面配置信息
	config.beginGroup(StrLrcConfig);
	if (config.childKeys().isEmpty())
	{
		config.setValue(StrFontName, FontName);
		config.setValue(StrFontSize, FontSize);
		config.setValue(StrFontType, FontType);
		config.setValue(StrNoPlayShang, NoPlayShang);
		config.setValue(StrNoPlayZhong, NoPlayZhong);
		config.setValue(StrNoPlayXia, NoPlayXia);
		config.setValue(StrPlayedShang, PlayedShang);
		config.setValue(StrPlayedZhong, PlayedZhong);
		config.setValue(StrPlayedXia, PlayedXia);
		config.setValue(StrDefaultChoiceColor, DefaultChoiceColor);
		config.setValue(StrShadow, Shadow);
	}
	/*Else do nothing, and continue.*/
	config.endGroup();

	//热键界面配置信息
	config.beginGroup(StrHotkeyConfig);
	if (config.childKeys().isEmpty())
	{
		config.setValue(StrConfigOptions, ConfigOptions);
		config.setValue(StrShowHideMainWidget, ShowHideMainWidget);
		config.setValue(StrMinMainWidget, MinMainWidget);
		config.setValue(StrOpenMusicFile, OpenMusicFile);
        config.setValue(StrShowHideMediaWidget, ShowHideMediaWidget);
		config.setValue(StrShowHideMusicList, ShowHideMusicList);
		config.setValue(StrShowHideLrc, ShowHideLrc);
		config.setValue(StrPlayPause, PlayPause);
		config.setValue(StrPlayPre, PlayPre);
		config.setValue(StrPlayNext, PlayNext);
	}
	/*Else do nothing, and continue.*/
	config.endGroup();

	//播放控制信息
	config.beginGroup(StrPlayControlConfig);
	if (config.childKeys().isEmpty())
	{
		config.setValue(StrMute, Mute);
		config.setValue(StrVolume, Volume);
		config.setValue(StrPlayMode, PlayMode);
	}
	/*Else do nothing, and continue.*/
	config.endGroup();

	config.sync();

	return (config.status() == QSettings::NoError);
}

bool GlobalConfig::save()
{
	if (!this->saveGeneralSettingsInfo())
	{
		return false;
	}
	/*Else do nothing, and continue.*/

	if (!this->saveDownloadSettingsInfo())
	{
		return false;
	}
	/*Else do nothing, and continue.*/

	if (!this->saveHotkeysSettingsInfo())
	{
		return false;
	}
	/*Else do nothing, and continue.*/

	if (!this->saveLrcSettingsInfo())
	{
		return false;
	}
	/*Else do nothing, and continue.*/

	if (!this->savePlayControlSettingsInfo())
	{
		return false;
	}
	/*Else do nothing, and continue.*/

	return true;
}

bool GlobalConfig::reloadGeneralSettingsInfo()
{
	QSettings config(QSettings::UserScope, StringOrganization, StringApplication);
	config.sync();
	if (config.status() != QSettings::NoError)
	{
		return false;
	}
	/*Else do nothing, and continue.*/

	config.beginGroup(StrGeneralConfig);
	
	this->setAutoPlayMusic(config.value(StrAutoPlayMusic, AutoPlayMusic).toBool());
	this->setAutoRunPlayer(config.value(StrAutoRunPlayer, AutoRunPlayer).toBool());
	this->setAutoGreetings(config.value(StrAutoGreetings, AutoGreetings).toBool());
	this->setDefaultPlayer(config.value(StrDefaultPlayer, DefaultPlayer).toBool());
	config.endGroup();

	m_isGeneralSettingsNeedSave = false;
	return true;
}

bool GlobalConfig::reloadDownloadSettingsInfo()
{
	QSettings config(QSettings::UserScope, StringOrganization, StringApplication);
	config.sync();
	if (config.status() != QSettings::NoError)
	{
		return false;
	}
	/*Else do nothing, and continue.*/

	config.beginGroup(StrDownloadConfig);
	this->setMusicPath(config.value(StrMusicPath, MusicPath).toString());
	this->setAlbumPath(config.value(StrAlbumPath, AlbumPath).toString());
	this->setLrcPath(config.value(StrLrcPath, m_lrcPath).toString());
	config.endGroup();

	m_isDownloadSettingsNeedSave = false;
	return true;
}

bool GlobalConfig::reloadLrcSettingsInfo()
{
	QSettings config(QSettings::UserScope, StringOrganization, StringApplication);
	config.sync();
	if (config.status() != QSettings::NoError)
	{
		return false;
	}
	/*Else do nothing, and continue.*/

	config.beginGroup(StrLrcConfig);
	this->setFontName(config.value(StrFontName, FontName).toString());
	this->setFontSize(config.value(StrFontSize, FontSize).toString());
	this->setFontType(config.value(StrFontType, FontType).toString());
	this->setNoPlayShang(config.value(StrNoPlayShang, NoPlayShang).value<QColor>());
	this->setNoPlayZhong(config.value(StrNoPlayZhong, NoPlayZhong).value<QColor>());
	this->setNoPlayXia(config.value(StrNoPlayXia, NoPlayXia).value<QColor>());
	this->setPlayedShang(config.value(StrPlayedShang, PlayedShang).value<QColor>());
	this->setPlayedZhong(config.value(StrPlayedZhong, PlayedZhong).value<QColor>());
	this->setPlayedXia(config.value(StrPlayedXia, PlayedXia).value<QColor>());
	this->setDefaultChoiceColor(config.value(StrDefaultChoiceColor, DefaultChoiceColor).toString());
	bool ok;
	int shadow = config.value(StrShadow, Shadow).toInt(&ok);
	if (!ok)
	{
#ifdef CZ_DEBUG
		qDebug() << "To int error.";
		qDebug() << __FUNCTION__ << __FUNCTION__ << __LINE__;
		shadow = Shadow;
#endif 
	}
	/*Else do nothing, and conintue.*/
	this->setShadow(shadow);
	config.endGroup();

	m_isLrcSettingsNeedSave = false;
	return true;
}

bool GlobalConfig::reloadHotkeySettingsInfo()
{
	QSettings config(QSettings::UserScope, StringOrganization, StringApplication);
	config.sync();
	if (config.status() != QSettings::NoError)
	{
		return false;
	}
	/*Else do nothing, and continue.*/

	config.beginGroup(StrHotkeyConfig);
	this->setSettingsHotKey(config.value(StrConfigOptions, ConfigOptions).toString());
	this->setMinToTrayHotKey(config.value(StrShowHideMainWidget, ShowHideMainWidget).toString());
	this->setOpenMinWidgetHotKey(config.value(StrMinMainWidget, MinMainWidget).toString());
	this->setOpenMusicFileHotKey(config.value(StrOpenMusicFile, OpenMusicFile).toString());
    this->setOpenMediaWidgetHotKey(config.value(StrShowHideMediaWidget, ShowHideMediaWidget).toString());
	this->setOpenListWidgetHotKey(config.value(StrShowHideMusicList, ShowHideMusicList).toString());
	this->setOpenLrcHotKey(config.value(StrShowHideLrc, ShowHideLrc).toString());
	this->setPauseHotKey(config.value(StrPlayPause, PlayPause).toString());
	this->setLastHotKey(config.value(StrPlayPre, PlayPre).toString());
	this->setNextHotKey(config.value(StrPlayNext, PlayNext).toString());
	config.endGroup();

	m_isHotkeySettingsNeedSave = false;
	return true;
}

bool GlobalConfig::reloadPlayControlSettingsInfo()
{
	QSettings config(QSettings::UserScope, StringOrganization, StringApplication);
	config.sync();
	if (config.status() != QSettings::NoError)
	{
		return false;
	}
	/*Else do nothing, and continue.*/

	config.beginGroup(StrPlayControlConfig);
	this->setMute(config.value(StrMute, Mute).toBool());

	bool ok;
	int volume = config.value(StrVolume, Volume).toInt(&ok);
	if (!ok)
	{
#ifdef CZ_DEBUG
		qDebug() << "To int error.";
		qDebug() << __FUNCTION__ << __FUNCTION__ << __LINE__;
		volume = Volume;
#endif 
	}
	/*Else do nothing, and conintue.*/
	this->setVolume(volume);
	
	int playMode = config.value(StrPlayMode, PlayMode).toInt(&ok);
	if (!ok)
	{
#ifdef CZ_DEBUG
		qDebug() << "To int error.";
		qDebug() << __FUNCTION__ << __FUNCTION__ << __LINE__;
		playMode = PlayMode;
#endif 
	}
	/*Else do nothing, and conintue.*/
	this->setPlayMode((enum PlayModeType)playMode);
	config.endGroup();

	m_isPlayControlSettingsNeedSave = false;
	return true;
}

bool GlobalConfig::saveGeneralSettingsInfo()
{
	if (!m_isGeneralSettingsNeedSave)
	{
		return true;
	}
	/*Else do nothing, and continue.*/

	QSettings config(QSettings::UserScope, StringOrganization, StringApplication);

	config.sync();
	if ((config.status() != QSettings::NoError) || !config.isWritable())
	{
		return false;
	}
	/*Else do nothing, and continue.*/

	config.beginGroup(StrGeneralConfig);
	config.setValue(StrAutoPlayMusic, m_isAutoPlayMusic);
	config.setValue(StrAutoRunPlayer, m_isAutoRunPlayer);
	config.setValue(StrAutoGreetings, m_isAutoGrettings);
	config.setValue(StrDefaultPlayer, m_isDefaultPlayer);
	config.endGroup();
	config.sync();

	m_isGeneralSettingsNeedSave = false;

	return (config.status() == QSettings::NoError);
}

bool GlobalConfig::saveDownloadSettingsInfo()
{
	if (!m_isDownloadSettingsNeedSave)
	{
		return true;
	}
	/*Else do nothing, and continue.*/

	QSettings config(QSettings::UserScope, StringOrganization, StringApplication);

	config.sync();
	if ((config.status() != QSettings::NoError) || !config.isWritable())
	{
		return false;
	}
	/*Else do nothing, and continue.*/

	config.beginGroup(StrDownloadConfig);
	config.setValue(StrMusicPath, m_musicPath);
	config.setValue(StrAlbumPath, m_albumPath);
	config.setValue(StrLrcPath, m_lrcPath);
	config.endGroup();
	config.sync();

	m_isDownloadSettingsNeedSave = false;

	return (config.status() == QSettings::NoError);
}

bool GlobalConfig::saveHotkeysSettingsInfo()
{
	if (!m_isHotkeySettingsNeedSave)
	{
		return true;
	}
	/*Else do nothing, and continue.*/

	QSettings config(QSettings::UserScope, StringOrganization, StringApplication);

	config.sync();
	if ((config.status() != QSettings::NoError) || !config.isWritable())
	{
		return false;
	}
	/*Else do nothing, and continue.*/

	config.beginGroup(StrHotkeyConfig);
	config.setValue(StrConfigOptions, m_settingsHotKey);
	config.setValue(StrShowHideMainWidget, m_minToTrayHotKey);
	config.setValue(StrMinMainWidget, m_openMinWidgetHotKey);
	config.setValue(StrOpenMusicFile, m_openMusicFileHotKey);
    config.setValue(StrShowHideMediaWidget, m_openMediaWidgetHotKey);
	config.setValue(StrShowHideMusicList, m_openListWidgetHotKey);
	config.setValue(StrShowHideLrc, m_openLrcHotKey);
	config.setValue(StrPlayPause, m_pauseHotKey);
	config.setValue(StrPlayPre, m_lastHotKey);
	config.setValue(StrPlayNext, m_nextHotKey);
	config.endGroup();
	config.sync();

	m_isHotkeySettingsNeedSave = false;

	return (config.status() == QSettings::NoError);
}

bool GlobalConfig::saveLrcSettingsInfo()
{
	if (!m_isLrcSettingsNeedSave)
	{
		return true;
	}
	/*Else do nothing, and continue.*/

	QSettings config(QSettings::UserScope, StringOrganization, StringApplication);

	config.sync();
	if ((config.status() != QSettings::NoError) || !config.isWritable())
	{
		return false;
	}
	/*Else do nothing, and continue.*/

	config.beginGroup(StrLrcConfig);
	config.setValue(StrFontName, m_fontName);
	config.setValue(StrFontSize, m_fontSize);
	config.setValue(StrFontType, m_fontType);
	config.setValue(StrNoPlayShang, m_noPlayShang);
	config.setValue(StrNoPlayZhong, m_noPlayZhong);
	config.setValue(StrNoPlayXia, m_noPlayXia);
	config.setValue(StrPlayedShang, m_playedShang);
	config.setValue(StrPlayedZhong, m_playedZhong);
	config.setValue(StrPlayedXia, m_playedXia);
	config.setValue(StrDefaultChoiceColor, m_defaultChoiceColor);
	config.setValue(StrShadow, m_shadow);
	config.endGroup();
	config.sync();

	m_isLrcSettingsNeedSave = false;

	return (config.status() == QSettings::NoError);
}

bool GlobalConfig::savePlayControlSettingsInfo()
{
	if (!m_isPlayControlSettingsNeedSave)
	{
		return true;
	}
	/*Else do nothing, and continue.*/

	QSettings config(QSettings::UserScope, StringOrganization, StringApplication);

	config.sync();
	if ((config.status() != QSettings::NoError) || !config.isWritable())
	{
		return false;
	}
	/*Else do nothing, and continue.*/

	config.beginGroup(StrPlayControlConfig);
	config.setValue(StrMute, m_isMute);
	config.setValue(StrVolume, m_volume);
	config.setValue(StrPlayMode, m_playMode);
	config.endGroup();
	config.sync();

	m_isPlayControlSettingsNeedSave = false;

	return (config.status() == QSettings::NoError);
}

void GlobalConfig::setAutoPlayMusic(bool isAutoPlayMusic)
{
	if (m_isAutoPlayMusic != isAutoPlayMusic)
	{
		m_isAutoPlayMusic = isAutoPlayMusic;
		m_isGeneralSettingsNeedSave = true;
		emit sigAutoPlayMusicChanged(m_isAutoPlayMusic);
	}
	/*Else do nothing, and continue.*/
}

void GlobalConfig::setAutoRunPlayer(bool isAutoRunPlayer)
{
	if (m_isAutoRunPlayer != isAutoRunPlayer)
	{
		m_isAutoRunPlayer = isAutoRunPlayer;
		m_isGeneralSettingsNeedSave = true;
		emit sigAutoRunPlayerChanged(m_isAutoRunPlayer);
	}
	/*Else do nothing, and continue.*/
}

void GlobalConfig::setAutoGreetings(bool isAutoGrettings)
{
	if (m_isAutoGrettings != isAutoGrettings)
	{
		m_isAutoGrettings = isAutoGrettings;
		m_isGeneralSettingsNeedSave = true;
		emit sigAutoGreetingsChanged(m_isAutoGrettings);
	}
	/*Else do nothing, and continue.*/
}

void GlobalConfig::setDefaultPlayer(bool isDefaultPlayer)
{
	if (m_isDefaultPlayer != isDefaultPlayer)
	{
		m_isDefaultPlayer = isDefaultPlayer;
		m_isGeneralSettingsNeedSave = true;
		emit sigDefaultPlayerChanged(m_isDefaultPlayer);
	}
	/*Else do nothing, and continue.*/
}

bool GlobalConfig::autoPlayMusic() const
{
	return m_isAutoPlayMusic;
}

bool GlobalConfig::autoRunPlayer() const
{
	return m_isAutoRunPlayer;
}

bool GlobalConfig::autoGreetings() const
{
	return m_isAutoGrettings;
}

bool GlobalConfig::defaultPlayer() const
{
	return m_isDefaultPlayer;
}

void GlobalConfig::setMusicPath(const QString &path)
{
	if (m_musicPath != path)
	{
		m_musicPath = path;
		m_isDownloadSettingsNeedSave = true;
		emit sigMusicPathChanged(m_musicPath);
	}
	/*Else do nothing, and continue.*/
}

void GlobalConfig::setAlbumPath(const QString &path)
{
	if (m_albumPath != path)
	{
		m_albumPath = path;
		m_isDownloadSettingsNeedSave = true;
		emit sigAlbumPathChanged(m_albumPath);
	}
	/*Else do nothing, and continue.*/
}

void GlobalConfig::setLrcPath(const QString &path)
{
	if (m_lrcPath != path)
	{
		m_lrcPath = path;
		m_isDownloadSettingsNeedSave = true;
		emit sigLrcPathChanged(m_lrcPath);
	}
	/*Else do nothing, and continue.*/
}

QString GlobalConfig::musicPath() const
{
	return m_musicPath;
}

QString GlobalConfig::albumPath() const
{
	return m_albumPath;
}

QString GlobalConfig::lrcPath() const
{
	return m_lrcPath;
}

void GlobalConfig::setSettingsHotKey(const QString &hotKey)
{
	if (m_settingsHotKey != hotKey)
	{
		m_settingsHotKey = hotKey;
		m_isHotkeySettingsNeedSave = true;
		emit sigSettingsHotKeyChanged(m_settingsHotKey);
	}
	/*Else do nothing, and continue.*/
}

void GlobalConfig::setMinToTrayHotKey(const QString &hotKey)
{
	if (m_minToTrayHotKey != hotKey)
	{
		m_minToTrayHotKey = hotKey;
		m_isHotkeySettingsNeedSave = true;
		emit sigMinToTrayHotKeyChanged(m_minToTrayHotKey);
	}
	/*Else do nothing, and continue.*/
}

void GlobalConfig::setOpenMinWidgetHotKey(const QString &hotKey)
{
	if (m_openMinWidgetHotKey != hotKey)
	{
		m_openMinWidgetHotKey = hotKey;
		m_isHotkeySettingsNeedSave = true;
		emit sigOpenMinWidgetHotKeyChanged(m_openMinWidgetHotKey);
	}
	/*Else do nothing, and continue.*/
}

void GlobalConfig::setOpenMusicFileHotKey(const QString &hotKey)
{
	if (m_openMusicFileHotKey != hotKey)
	{
		m_openMusicFileHotKey = hotKey;
		m_isHotkeySettingsNeedSave = true;
		emit sigOpenMusicFileHotKeyChanged(m_openMusicFileHotKey);
	}
	/*Else do nothing, and continue.*/
}

void GlobalConfig::setOpenMediaWidgetHotKey(const QString &hotKey)
{
    if (m_openMediaWidgetHotKey != hotKey)
	{
        m_openMediaWidgetHotKey = hotKey;
		m_isHotkeySettingsNeedSave = true;
        emit sigOpenMediaWidgetHotKeyChanged(m_openMediaWidgetHotKey);
	}
	/*Else do nothing, and continue.*/
}

void GlobalConfig::setOpenListWidgetHotKey(const QString &hotKey)
{
	if (m_openListWidgetHotKey != hotKey)
	{
		m_openListWidgetHotKey = hotKey;
		m_isHotkeySettingsNeedSave = true;
		emit sigOpenListWidgetHotKeyChanged(m_openListWidgetHotKey);
	}
	/*Else do nothing, and continue.*/
}

void GlobalConfig::setOpenLrcHotKey(const QString &hotKey)
{
	if (m_openLrcHotKey != hotKey)
	{
		m_openLrcHotKey = hotKey;
		m_isHotkeySettingsNeedSave = true;
		emit sigOpenLrcHotKeyChanged(m_openLrcHotKey);
	}
	/*Else do nothing, and continue.*/
}

void GlobalConfig::setPauseHotKey(const QString &hotKey)
{
	if (m_pauseHotKey != hotKey)
	{
		m_pauseHotKey = hotKey;
		m_isHotkeySettingsNeedSave = true;
		emit sigPauseHotKeyChanged(m_pauseHotKey);
	}
	/*Else do nothing, and continue.*/
}

void GlobalConfig::setLastHotKey(const QString &hotKey)
{
	if (m_lastHotKey != hotKey)
	{
		m_lastHotKey = hotKey;
		m_isHotkeySettingsNeedSave = true;
		emit sigLastHotKeyChanged(m_lastHotKey);
	}
	/*Else do nothing, and continue.*/
}

void GlobalConfig::setNextHotKey(const QString &hotKey)
{
	if (m_nextHotKey != hotKey)
	{
		m_nextHotKey = hotKey;
		m_isHotkeySettingsNeedSave = true;
		emit sigNextHotKeyChanged(m_nextHotKey);
	}
	/*Else do nothing, and continue.*/
}

QString GlobalConfig::settingsHotKey() const
{
	return m_settingsHotKey;
}

QString GlobalConfig::minToTrayHotKey() const
{
	return m_minToTrayHotKey;
}

QString GlobalConfig::openMinWidgetHotKey() const
{
	return m_openMinWidgetHotKey;
}

QString GlobalConfig::openMusicFileHotKey() const
{
	return m_openMusicFileHotKey;
}

QString GlobalConfig::openMediaWidgetHotKey() const
{
    return m_openMediaWidgetHotKey;
}

QString GlobalConfig::openListWidgetHotKey() const
{
	return m_openListWidgetHotKey;
}

QString GlobalConfig::openLrcHotKey() const
{
	return m_openLrcHotKey;
}

QString GlobalConfig::pauseHotKey() const
{
	return m_pauseHotKey;
}

QString GlobalConfig::lastHotKey() const
{
	return m_lastHotKey;
}

QString GlobalConfig::nextHotKey() const
{
	return m_nextHotKey;
}

void GlobalConfig::setFontName(const QString &fontName)
{
	if (m_fontName != fontName)
	{
		m_fontName = fontName;
		m_isLrcSettingsNeedSave = true;
		emit sigFontNameChanged(m_fontName);
	}
	/*Else do nothing, and continue.*/
}

void GlobalConfig::setFontSize(const QString &fontSize)
{
	if (m_fontSize != fontSize)
	{
		m_fontSize = fontSize;
		m_isLrcSettingsNeedSave = true;
		emit sigFontSizeChanged(m_fontSize);
	}
	/*Else do nothing, and continue.*/
}

void GlobalConfig::setFontType(const QString &fontType)
{
	if (m_fontType != fontType)
	{
		m_fontType = fontType;
		m_isLrcSettingsNeedSave = true;
		emit sigFontTypeChanged(m_fontType);
	}
	/*Else do nothing, and continue.*/
}

void GlobalConfig::setNoPlayShang(const QColor &color)
{
	if (m_noPlayShang != color)
	{
		m_noPlayShang = color;
		m_isLrcSettingsNeedSave = true;
		emit sigNoPlayShangChanged(m_noPlayShang);
	}
	/*Else do nothing, and continue.*/
}

void GlobalConfig::setNoPlayZhong(const QColor &color)
{
	if (m_noPlayZhong != color)
	{
		m_noPlayZhong = color;
		m_isLrcSettingsNeedSave = true;
		emit sigNoPlayZhongChanged(m_noPlayZhong);
	}
	/*Else do nothing, and continue.*/
}

void GlobalConfig::setNoPlayXia(const QColor &color)
{
	if (m_noPlayXia != color)
	{
		m_noPlayXia = color;
		m_isLrcSettingsNeedSave = true;
		emit sigNoPlayXiaChanged(m_noPlayXia);
	}
	/*Else do nothing, and continue.*/
}

void GlobalConfig::setPlayedShang(const QColor &color)
{
	if (m_playedShang != color)
	{
		m_playedShang = color;
		m_isLrcSettingsNeedSave = true;
		emit sigPlayedShangChanged(m_playedShang);
	}
	/*Else do nothing, and continue.*/
}

void GlobalConfig::setPlayedZhong(const QColor &color)
{
	if (m_playedZhong != color)
	{
		m_playedZhong = color;
		m_isLrcSettingsNeedSave = true;
		emit sigPlayedZhongChanged(m_playedZhong);
	}
	/*Else do nothing, and continue.*/
}

void GlobalConfig::setPlayedXia(const QColor &color)
{
	if (m_playedXia != color)
	{
		m_playedXia = color;
		m_isLrcSettingsNeedSave = true;
		emit sigPlayedXiaChanged(m_playedXia);
	}
	/*Else do nothing, and continue.*/
}

void GlobalConfig::setDefaultChoiceColor(const QString &color)
{
	if (m_defaultChoiceColor != color)
	{
		m_defaultChoiceColor = color;
		m_isLrcSettingsNeedSave = true;
		emit sigDefaultChoiceColorChanged(m_defaultChoiceColor);
	}
	/*Else do nothing, and continue.*/
}

void GlobalConfig::setShadow(const int &shadow)
{
	if (m_shadow != shadow)
	{
		m_shadow = shadow;
		m_isLrcSettingsNeedSave = true;
		emit sigShadowChanged(m_shadow);
	}
	/*Else do nothing, and continue.*/
}

QString GlobalConfig::fontName() const
{
	return m_fontName;
}

QString GlobalConfig::fontSize() const
{
	return m_fontSize;
}

QString GlobalConfig::fontType() const
{
	return m_fontType;
}

QColor GlobalConfig::noPlayShang() const
{
	return m_noPlayShang;
}

QColor GlobalConfig::noPlayZhong() const
{
	return m_noPlayZhong;
}

QColor GlobalConfig::noPlayXia() const
{
	return m_noPlayXia;
}

QColor GlobalConfig::playedShang() const
{
	return m_playedShang;
}

QColor GlobalConfig::playedZhong() const
{
	return m_playedZhong;
}

QColor GlobalConfig::playedXia() const
{
	return m_playedXia;
}

QString GlobalConfig::defaultChoiceColor() const
{
	return m_defaultChoiceColor;
}

int GlobalConfig::shadow() const
{
	return m_shadow;
}

void GlobalConfig::setMute(bool isMute)
{
	m_isMute = isMute;
	m_isPlayControlSettingsNeedSave = true;
	if (!m_isReleaseResource)
	{
		emit sigMuteChanged(m_isMute);
	}
	/*Else do nothing, and continue.*/
}

void GlobalConfig::setVolume(const int &volume)
{
	if (m_volume != volume)
	{
		m_volume = volume;
		m_isPlayControlSettingsNeedSave = true;
		if (!m_isReleaseResource)
		{
			emit sigVolumeChanged(m_volume);
		}
		/*Else do nothing, and continue.*/
	}
	/*Else do nothing, and continue.*/
}

void GlobalConfig::setPlayMode(enum PlayModeType playMode)
{
	if (m_playMode != playMode)
	{
		m_playMode = playMode;
		m_isPlayControlSettingsNeedSave = true;
		if (!m_isReleaseResource)
		{
			emit sigPlayModeChanged(m_playMode);
		}
		/*Else do nothing, and continue.*/
	}
	/*Else do nothing, and continue.*/
}

bool GlobalConfig::mute() const
{
	return m_isMute;
}

int GlobalConfig::volume() const
{
	return m_volume;
}

enum PlayModeType GlobalConfig::playMode() const
{
	return m_playMode;
}
