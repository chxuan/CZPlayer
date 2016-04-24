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
 * \file   SharedDialog.cpp
 * \author chengxuan   787280310@qq.com
 * \date   2015-01-06
 * \brief  歌曲分享对话框实现文件。
 * \version 3.0.0
 *
 * \verbatim
 * 历史
 *          3.0.0   创建,
 *          2015-01-06    by chengxuan
 *
 * \endverbatim
 *
 */

#include "SharedDialog.h"
#include "Global.h"
#include "TR.h"
#include <QLabel>
#include <QTextEdit>
#include <QCheckBox>
#include <QPushButton>
#include <QMessageBox>
#include <QDesktopServices>
#include <QUrl>
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

SharedDialog::SharedDialog(QWidget *parent) 
	: QDialog(parent)
{
	this->createUI();
	this->createConnect();
}

SharedDialog::~SharedDialog(void)
{
	/*Do nothing.*/
}

void SharedDialog::init( const QString &albumImagePath, const QString &musicName, const QString &albumUrlPath )
{
	if (albumImagePath == ImagesDir + "albumImage.png")
	{
		m_albumLabel->setPixmap(QPixmap(albumImagePath));
	}
	else
	{
		//将图片转换成label大小来显示
		QImage image(albumImagePath);
		QImage newImg = image.scaled(m_albumLabel->width(), m_albumLabel->height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
		m_albumLabel->setPixmap(QPixmap::fromImage(newImg));
	}

	QString text = TR("在CZPlayer上听到的这首《%1》好听，快来听听看。").arg(musicName);
	m_textEdit->setText(text);
	m_musicName = musicName;
	m_albumUrlPath = albumUrlPath;

	m_xinlangweiboCheckBox->setCheckState(Qt::Checked);
	m_tengxunweiboCheckBox->setCheckState(Qt::Unchecked);
	m_qqkongjianCheckBox->setCheckState(Qt::Unchecked);
	m_renrenCheckBox->setCheckState(Qt::Unchecked);
	m_kaixinCheckBox->setCheckState(Qt::Unchecked);
	m_doubanCheckBox->setCheckState(Qt::Unchecked);
	m_souhuweiboCheckBox->setCheckState(Qt::Unchecked);
	m_qqhaoyouCheckBox->setCheckState(Qt::Unchecked);
}

void SharedDialog::handleCheckBoxStateChanged( int state )
{
	if (state == Qt::Checked)			//选中
	{
		QCheckBox *checkBox = dynamic_cast<QCheckBox*>(sender());
		if (checkBox == m_xinlangweiboCheckBox)
		{
			m_tengxunweiboCheckBox->setCheckState(Qt::Unchecked);
			m_qqkongjianCheckBox->setCheckState(Qt::Unchecked);
			m_renrenCheckBox->setCheckState(Qt::Unchecked);
			m_kaixinCheckBox->setCheckState(Qt::Unchecked);
			m_doubanCheckBox->setCheckState(Qt::Unchecked);
			m_souhuweiboCheckBox->setCheckState(Qt::Unchecked);
			m_qqhaoyouCheckBox->setCheckState(Qt::Unchecked);
		}
		else if (checkBox == m_tengxunweiboCheckBox)
		{
			m_xinlangweiboCheckBox->setCheckState(Qt::Unchecked);
			m_qqkongjianCheckBox->setCheckState(Qt::Unchecked);
			m_renrenCheckBox->setCheckState(Qt::Unchecked);
			m_kaixinCheckBox->setCheckState(Qt::Unchecked);
			m_doubanCheckBox->setCheckState(Qt::Unchecked);
			m_souhuweiboCheckBox->setCheckState(Qt::Unchecked);
			m_qqhaoyouCheckBox->setCheckState(Qt::Unchecked);
		}
		else if (checkBox == m_qqkongjianCheckBox)
		{
			m_xinlangweiboCheckBox->setCheckState(Qt::Unchecked);
			m_tengxunweiboCheckBox->setCheckState(Qt::Unchecked);
			m_renrenCheckBox->setCheckState(Qt::Unchecked);
			m_kaixinCheckBox->setCheckState(Qt::Unchecked);
			m_doubanCheckBox->setCheckState(Qt::Unchecked);
			m_souhuweiboCheckBox->setCheckState(Qt::Unchecked);
			m_qqhaoyouCheckBox->setCheckState(Qt::Unchecked);
		}
		else if (checkBox == m_renrenCheckBox)
		{
			m_xinlangweiboCheckBox->setCheckState(Qt::Unchecked);
			m_tengxunweiboCheckBox->setCheckState(Qt::Unchecked);
			m_qqkongjianCheckBox->setCheckState(Qt::Unchecked);
			m_kaixinCheckBox->setCheckState(Qt::Unchecked);
			m_doubanCheckBox->setCheckState(Qt::Unchecked);
			m_souhuweiboCheckBox->setCheckState(Qt::Unchecked);
			m_qqhaoyouCheckBox->setCheckState(Qt::Unchecked);
		}
		else if (checkBox == m_kaixinCheckBox)
		{
			m_xinlangweiboCheckBox->setCheckState(Qt::Unchecked);
			m_tengxunweiboCheckBox->setCheckState(Qt::Unchecked);
			m_qqkongjianCheckBox->setCheckState(Qt::Unchecked);
			m_renrenCheckBox->setCheckState(Qt::Unchecked);
			m_doubanCheckBox->setCheckState(Qt::Unchecked);
			m_souhuweiboCheckBox->setCheckState(Qt::Unchecked);
			m_qqhaoyouCheckBox->setCheckState(Qt::Unchecked);
		}
		else if (checkBox == m_doubanCheckBox)
		{
			m_xinlangweiboCheckBox->setCheckState(Qt::Unchecked);
			m_tengxunweiboCheckBox->setCheckState(Qt::Unchecked);
			m_qqkongjianCheckBox->setCheckState(Qt::Unchecked);
			m_renrenCheckBox->setCheckState(Qt::Unchecked);
			m_kaixinCheckBox->setCheckState(Qt::Unchecked);
			m_souhuweiboCheckBox->setCheckState(Qt::Unchecked);
			m_qqhaoyouCheckBox->setCheckState(Qt::Unchecked);
		}
		else if (checkBox == m_souhuweiboCheckBox)
		{
			m_xinlangweiboCheckBox->setCheckState(Qt::Unchecked);
			m_tengxunweiboCheckBox->setCheckState(Qt::Unchecked);
			m_qqkongjianCheckBox->setCheckState(Qt::Unchecked);
			m_renrenCheckBox->setCheckState(Qt::Unchecked);
			m_kaixinCheckBox->setCheckState(Qt::Unchecked);
			m_doubanCheckBox->setCheckState(Qt::Unchecked);
			m_qqhaoyouCheckBox->setCheckState(Qt::Unchecked);
		}
		else if (checkBox == m_qqhaoyouCheckBox)
		{
			m_xinlangweiboCheckBox->setCheckState(Qt::Unchecked);
			m_tengxunweiboCheckBox->setCheckState(Qt::Unchecked);
			m_qqkongjianCheckBox->setCheckState(Qt::Unchecked);
			m_renrenCheckBox->setCheckState(Qt::Unchecked);
			m_kaixinCheckBox->setCheckState(Qt::Unchecked);
			m_doubanCheckBox->setCheckState(Qt::Unchecked);
			m_souhuweiboCheckBox->setCheckState(Qt::Unchecked);
		}
		/*Else do nothing, and continue.*/
	}
	/*Else do nothing, and continue.*/
}

void SharedDialog::handleSharedMusic()
{
	if (m_textEdit->toPlainText() == "")
	{
		QMessageBox::information(this, TR("信息"), TR("分享的内容不能为空！"), QMessageBox::Yes);
		return;
	}
	/*Else do nothing, and continue.*/

	if (m_xinlangweiboCheckBox->checkState() == Qt::Unchecked
		&& m_tengxunweiboCheckBox->checkState() == Qt::Unchecked
		&& m_qqkongjianCheckBox->checkState() == Qt::Unchecked
		&& m_renrenCheckBox->checkState() == Qt::Unchecked
		&& m_kaixinCheckBox->checkState() == Qt::Unchecked
		&& m_doubanCheckBox->checkState() == Qt::Unchecked
		&& m_souhuweiboCheckBox->checkState() == Qt::Unchecked
		&& m_qqhaoyouCheckBox->checkState() == Qt::Unchecked)
	{
		QMessageBox::information(this, TR("信息"), TR("请勾选要分享到的网站！"), QMessageBox::Yes);
		return;
	}
	/*Else do nothing, and continue.*/

	QString strMyIndex = "http://www.qtcn.org/bbs/read-htm-tid-55824.html";
	QString url;

	if (m_xinlangweiboCheckBox->checkState() == Qt::Checked)
	{
		url = "http://service.weibo.com/share/share.php?url=" 
			+ strMyIndex + "&title=" + m_textEdit->toPlainText() + "&pic=" + m_albumUrlPath;
	}
	else if (m_tengxunweiboCheckBox->checkState() == Qt::Checked)
	{
		url = "http://share.v.t.qq.com/index.php?c=share&a=index&url=" 
			+ strMyIndex + "&title=" + m_textEdit->toPlainText() + "&pic=" + m_albumUrlPath;
	}
	else if (m_qqkongjianCheckBox->checkState() == Qt::Checked)
	{
		url = "http://sns.qzone.qq.com/cgi-bin/qzshare/cgi_qzshare_onekey?url=" 
			+ strMyIndex + "&title=" + TR("分享自") + "CZPlayer&desc=" + m_textEdit->toPlainText() + "&pics=" + m_albumUrlPath;
	}
	else if (m_renrenCheckBox->checkState() == Qt::Checked)
	{
		url = "http://widget.renren.com/dialog/share?resourceUrl=" 
			+ strMyIndex + "&description=" + m_textEdit->toPlainText() + "&pic=" + m_albumUrlPath;
	}
	else if (m_kaixinCheckBox->checkState() == Qt::Checked)
	{
		url = "http://www.kaixin001.com/rest/records.php?content="
			+ m_textEdit->toPlainText() + "&url=" + strMyIndex + "&starid=&aid=100018706&style=11&pic=" + m_albumUrlPath;
	}
	else if (m_doubanCheckBox->checkState() == Qt::Checked)
	{
		url = "http://www.douban.com/share/service?href=" + m_albumUrlPath + "&name="
			+ m_textEdit->toPlainText() + "&text=" + strMyIndex;
	}
	else if (m_souhuweiboCheckBox->checkState() == Qt::Checked)
	{
		url = "http://t.sohu.com/third/post.jsp?url=" + strMyIndex + "&title=" + m_textEdit->toPlainText();
	}
	else if (m_qqhaoyouCheckBox->checkState() == Qt::Checked)
	{
		url = "http://connect.qq.com/widget/shareqq/index.html?url=" 
			+ strMyIndex + "&desc=" + m_textEdit->toPlainText() + "&title=" 
			+ m_musicName + "&summary=" + TR("听音乐，找") + "CZPlayer&pics=" + m_albumUrlPath;
	}
	/*Else do nothing, and continue.*/

	Q_ASSERT(!url.isEmpty());
	this->openBrowser(url);
}

void SharedDialog::openBrowser( const QString &url )
{
	bool ok = QDesktopServices::openUrl(QUrl(url));
	if (!ok)
	{
		QMessageBox::information(this, TR("信息"), TR("打开浏览器失败！"), QMessageBox::Yes);
	}
	/*Else do nothing, and continue.*/
}

void SharedDialog::createUI()
{
	//设置窗口基本属性
	this->resize(420, 280);//设置窗体大小
	this->setMinimumSize(420, 280);
	this->setMaximumSize(420, 280);
	this->setWindowTitle(TR("分享歌曲"));

	//专辑图片
	m_albumLabel = new QLabel(this);
	m_albumLabel->setToolTip(TR("专辑图片"));

	//分享文本输入框
	m_textEdit = new QTextEdit(this);
	m_textEdit->setObjectName("m_textEdit");

	//分享到
	m_sharedLabel = new QLabel(this);
	m_sharedLabel->setObjectName("sharedLabel");
	m_sharedLabel->setText(TR("分享到"));

    QString styleSheet = QString("QCheckBox{color:black;}"
                                 "QCheckBox::indicator{width:10px;height:10px;border: 1px solid black;border-radius:2px}"
                                 "QCheckBox::indicator:checked {image: url(%1checked2.png);}").arg(ImagesDir);

	//新浪微博
	m_xinlangweiboCheckBox = new QCheckBox(this);
	m_xinlangweiboCheckBox->setToolTip(TR("分享到新浪微博"));
    m_xinlangweiboCheckBox->setStyleSheet(styleSheet);
	m_xinlangweiboLabel = new QLabel(this);
	m_xinlangweiboLabel->setToolTip(TR("分享到新浪微博"));
	m_xinlangweiboLabel->setObjectName("xinlangweiboLabel");

	//腾讯微博
	m_tengxunweiboCheckBox = new QCheckBox(this);
	m_tengxunweiboCheckBox->setToolTip(TR("分享到腾讯微博"));
    m_tengxunweiboCheckBox->setStyleSheet(styleSheet);
	m_tengxunweiboLabel = new QLabel(this);
	m_tengxunweiboLabel->setToolTip(TR("分享到腾讯微博"));
	m_tengxunweiboLabel->setObjectName("tengxunweiboLabel");

	//QQ空间
	m_qqkongjianCheckBox = new QCheckBox(this);
	m_qqkongjianCheckBox->setToolTip(TR("分享到QQ空间"));
    m_qqkongjianCheckBox->setStyleSheet(styleSheet);
	m_qqkongjianLabel = new QLabel(this);
	m_qqkongjianLabel->setToolTip(TR("分享到QQ空间"));
	m_qqkongjianLabel->setObjectName("qqkongjianLabel");

	//人人网
	m_renrenCheckBox = new QCheckBox(this);
	m_renrenCheckBox->setToolTip(TR("分享到人人网"));
    m_renrenCheckBox->setStyleSheet(styleSheet);
	m_renrenLabel = new QLabel(this);
	m_renrenLabel->setToolTip(TR("分享到人人网"));
	m_renrenLabel->setObjectName("renrenLabel");

	//开心网
	m_kaixinCheckBox = new QCheckBox(this);
	m_kaixinCheckBox->setToolTip(TR("分享到开心网"));
    m_kaixinCheckBox->setStyleSheet(styleSheet);
	m_kaixinLabel = new QLabel(this);
	m_kaixinLabel->setToolTip(TR("分享到开心网"));
	m_kaixinLabel->setObjectName("kaixinLabel");

	//豆瓣网
	m_doubanCheckBox = new QCheckBox(this);
	m_doubanCheckBox->setToolTip(TR("分享到豆瓣网"));
    m_doubanCheckBox->setStyleSheet(styleSheet);
	m_doubanLabel = new QLabel(this);
	m_doubanLabel->setToolTip(TR("分享到豆瓣网"));
	m_doubanLabel->setObjectName("doubanLabel");

	//搜狐微博
	m_souhuweiboCheckBox = new QCheckBox(this);
	m_souhuweiboCheckBox->setToolTip(TR("分享到搜狐微博"));
    m_souhuweiboCheckBox->setStyleSheet(styleSheet);
	m_souhuweiboLabel = new QLabel(this);
	m_souhuweiboLabel->setToolTip(TR("分享到搜狐微博"));
	m_souhuweiboLabel->setObjectName("souhuweiboLabel");

	//QQ好友
	m_qqhaoyouCheckBox = new QCheckBox(this);
	m_qqhaoyouCheckBox->setToolTip(TR("分享到QQ好友"));
    m_qqhaoyouCheckBox->setStyleSheet(styleSheet);
	m_qqhaoyouLabel = new QLabel(this);
	m_qqhaoyouLabel->setToolTip(TR("分享到QQ好友"));
	m_qqhaoyouLabel->setObjectName("qqhaoyouLabel");

	//分享按钮
	m_shareButton = new QPushButton(this);
	m_shareButton->setText(TR("分享"));

	//取消按钮
	m_cancelButton = new QPushButton(this);
	m_cancelButton->setText(TR("取消"));

	m_albumLabel->setGeometry(25, 20, 110, 103);
	m_textEdit->setGeometry(150, 20, 250, 103);
	m_sharedLabel->setGeometry(25, 150, 100, 20);
	m_xinlangweiboCheckBox->setGeometry(80, 150, 20, 20);
	m_xinlangweiboLabel->setGeometry(95, 143, 30, 30);
	m_tengxunweiboCheckBox->setGeometry(145, 150, 20, 20);
	m_tengxunweiboLabel->setGeometry(160, 143, 30, 30);
	m_qqkongjianCheckBox->setGeometry(210, 150, 20, 20);
	m_qqkongjianLabel->setGeometry(225, 143, 30, 30);
	m_renrenCheckBox->setGeometry(275, 150, 20, 20);
	m_renrenLabel->setGeometry(290, 143, 30, 30);
	m_kaixinCheckBox->setGeometry(340, 150, 20, 20);
	m_kaixinLabel->setGeometry(355, 143, 30, 30);
	m_doubanCheckBox->setGeometry(80, 200, 20, 20);
	m_doubanLabel->setGeometry(95, 193, 30, 30);
	m_souhuweiboCheckBox->setGeometry(145, 200, 20, 20);
	m_souhuweiboLabel->setGeometry(160, 193, 30, 30);
	m_qqhaoyouCheckBox->setGeometry(210, 200, 20, 20);
	m_qqhaoyouLabel->setGeometry(225, 193, 30, 30);
	m_shareButton->setGeometry(240, 240, 80, 25);
	m_cancelButton->setGeometry(330, 240, 80, 25);
}

void SharedDialog::createConnect()
{
	connect(m_xinlangweiboCheckBox, SIGNAL(stateChanged(int)), this, SLOT(handleCheckBoxStateChanged(int)));
	connect(m_tengxunweiboCheckBox, SIGNAL(stateChanged(int)), this, SLOT(handleCheckBoxStateChanged(int)));
	connect(m_qqkongjianCheckBox, SIGNAL(stateChanged(int)), this, SLOT(handleCheckBoxStateChanged(int)));
	connect(m_renrenCheckBox, SIGNAL(stateChanged(int)), this, SLOT(handleCheckBoxStateChanged(int)));
	connect(m_kaixinCheckBox, SIGNAL(stateChanged(int)), this, SLOT(handleCheckBoxStateChanged(int)));
	connect(m_doubanCheckBox, SIGNAL(stateChanged(int)), this, SLOT(handleCheckBoxStateChanged(int)));
	connect(m_souhuweiboCheckBox, SIGNAL(stateChanged(int)), this, SLOT(handleCheckBoxStateChanged(int)));
	connect(m_qqhaoyouCheckBox, SIGNAL(stateChanged(int)), this, SLOT(handleCheckBoxStateChanged(int)));

	connect(m_shareButton, SIGNAL(clicked()), this, SLOT(handleSharedMusic()));
	connect(m_cancelButton, SIGNAL(clicked()), this, SLOT(hide()));
}
