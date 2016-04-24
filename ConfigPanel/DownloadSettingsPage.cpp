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
* \file   DownloadSettingsPage.cpp
* \author chengxuan   787280310@qq.com
* \date   2015-01-24
* \brief  下载设置页面实现文件。
* \version 3.0.0
*
* \verbatim
* 历史
*          3.0.0   创建,
*          2015-01-24    by chengxuan
*
* \endverbatim
*
*/

#include "DownloadSettingsPage.h"
#include "GlobalConfig.h"
#include "TR.h"
#include <QLabel>
#include <QLineEdit>
#include <QPushButton> 
#include <QGroupBox>
#include <QHBoxLayout>
#include <QDir>
#include <QFileDialog>
#include <QMessageBox>
#ifdef CZ_DEBUG
#include <QDebug>
#endif

/*****************************************************************************
  这里用来检查版本的对应
 *****************************************************************************/
#if DOWNLOADSETTINGSPAGE_VERSION < 0x030000
#error  "DownloadSettingsPage version error."
#endif

#if TR_VERSION < 0x030000
#error  "TR version error."
#endif

DownloadSettingsPage::DownloadSettingsPage(QWidget *parent) 
	: QWidget(parent)
{
	this->createUI();
	this->createConnect();
}

DownloadSettingsPage::~DownloadSettingsPage()
{
	/*Do nothing.*/
}

void DownloadSettingsPage::createUI()
{
	QGroupBox *dirSettingGB = new QGroupBox(TR("目录设置"));

	//歌曲目录
	m_musicDirLabel = new QLabel(this);
	m_musicDirLabel->setText(TR("歌曲目录："));
	m_musicEdit = new QLineEdit(this);
	m_musicEdit->setReadOnly(true);
	m_musicButton = new QPushButton(this);
	m_musicButton->setText(TR("浏览"));

	//专辑目录
	m_albumDirLabel = new QLabel(this);
	m_albumDirLabel->setText(TR("专辑目录："));
	m_albumEdit = new QLineEdit(this);
	m_albumEdit->setReadOnly(true);
	m_albumEdit->setObjectName("m_albumEdit");
	m_albumButton = new QPushButton(this);
	m_albumButton->setText(TR("浏览"));

	//歌词目录
	m_lrcFileLabel = new QLabel(this);
	m_lrcFileLabel->setText(TR("歌词目录："));
	m_lrcFileEdit = new QLineEdit(this);
	m_lrcFileEdit->setReadOnly(true);
	m_lrcFileButton = new QPushButton(this);
	m_lrcFileButton->setText(TR("浏览"));

	QHBoxLayout *musicDirLayout = new QHBoxLayout;
	musicDirLayout->addWidget(m_musicDirLabel);
	musicDirLayout->addWidget(m_musicEdit);
	musicDirLayout->addWidget(m_musicButton);

	QHBoxLayout *albumDirLayout = new QHBoxLayout;
	albumDirLayout->addWidget(m_albumDirLabel);
	albumDirLayout->addWidget(m_albumEdit);
	albumDirLayout->addWidget(m_albumButton);

	QHBoxLayout *lrcDirLayout = new QHBoxLayout;
	lrcDirLayout->addWidget(m_lrcFileLabel);
	lrcDirLayout->addWidget(m_lrcFileEdit);
	lrcDirLayout->addWidget(m_lrcFileButton);

	QVBoxLayout *dirSettingLayout = new QVBoxLayout;
	dirSettingLayout->addLayout(musicDirLayout);
	dirSettingLayout->addLayout(albumDirLayout);
	dirSettingLayout->addLayout(lrcDirLayout);
	dirSettingGB->setLayout(dirSettingLayout);

	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addWidget(dirSettingGB);
	mainLayout->addStretch();
	this->setLayout(mainLayout);
}

void DownloadSettingsPage::createConnect()
{
	connect(m_musicButton, SIGNAL(clicked()), this, SLOT(handleMusicDirChanged()));
	connect(m_albumButton, SIGNAL(clicked()), this, SLOT(handleAlbumDirChanged()));
	connect(m_lrcFileButton, SIGNAL(clicked()), this, SLOT(handleLrcDirChanged()));
}

void DownloadSettingsPage::init()
{
	GlobalConfig *config = GlobalConfig::instance();
	m_musicEdit->setText(config->musicPath());
	m_albumEdit->setText(config->albumPath());
	m_lrcFileEdit->setText(config->lrcPath());
}

void DownloadSettingsPage::apply()
{
	GlobalConfig *config = GlobalConfig::instance();

	config->setMusicPath(m_musicEdit->text());
	config->setAlbumPath(m_albumEdit->text());
	config->setLrcPath(m_lrcFileEdit->text());

	if (!config->save())
	{
		QMessageBox::warning(this, TR("警告"), TR("保存下载设置配置信息失败！"));
	}
	/*Else do nothing, and contine.*/
}

void DownloadSettingsPage::handleMusicDirChanged()
{
	QString path = m_musicEdit->text();
	if (path.isEmpty())
	{
		path = QDir::homePath();
	}
	/*Else do nothing, and contine.*/
	
	QString directory = QFileDialog::getExistingDirectory(this, TR("改变歌曲目录"),
		path,
		QFileDialog::ShowDirsOnly
		| QFileDialog::DontResolveSymlinks);

	if (!directory.isEmpty() && directory != m_musicEdit->text())
	{
		m_musicEdit->setText(directory);
		emit sigDownloadSettingsChanged();
	}
	/*Else do nothing, and contine.*/
}

void DownloadSettingsPage::handleAlbumDirChanged()
{
	QString path = m_albumEdit->text();
	if (path.isEmpty())
	{
		path = QDir::homePath();
	}
	/*Else do nothing, and contine.*/

	QString directory = QFileDialog::getExistingDirectory(this, TR("改变专辑目录"),
		path,
		QFileDialog::ShowDirsOnly
		| QFileDialog::DontResolveSymlinks);

	if (!directory.isEmpty() && directory != m_albumEdit->text())
	{
		m_albumEdit->setText(directory);
		emit sigDownloadSettingsChanged();
	}
	/*Else do nothing, and contine.*/
}

void DownloadSettingsPage::handleLrcDirChanged()
{
	QString path = m_lrcFileEdit->text();
	if (path.isEmpty())
	{
		path = QDir::homePath();
	}
	/*Else do nothing, and contine.*/

	QString directory = QFileDialog::getExistingDirectory(this, TR("改变歌词目录"),
		path,
		QFileDialog::ShowDirsOnly
		| QFileDialog::DontResolveSymlinks);

	if (!directory.isEmpty() && directory != m_lrcFileEdit->text())
	{
		m_lrcFileEdit->setText(directory);
		emit sigDownloadSettingsChanged();
	}
	/*Else do nothing, and contine.*/
}



