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
* \file   LrcSettingsPage.cpp
* \author chengxuan   787280310@qq.com
* \date   2015-01-24
* \brief  歌词设置页面实现文件。
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

#include "LrcSettingsPage.h"
#include "LrcDemo.h"
#include "GlobalConfig.h"
#include "Global.h"
#include "TR.h"
#include <QLabel>
#include <QSpinBox>
#include <QLineEdit>
#include <QSlider>
#include <QPushButton>
#include <QAction>
#include <QGroupBox>
#include <QComboBox>
#include <QHBoxLayout>
#include <QColorDialog>
#include <QMenu>
#include <QFontDatabase>
#include <QColorDialog>
#include <QMessageBox>
#ifdef CZ_DEBUG
#include <QDebug>
#endif

/*****************************************************************************
  这里用来检查版本的对应
 *****************************************************************************/
#if LRCDEMO_VERSION < 0x030000
#error  "LrcDemo version error."
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

LrcSettingsPage::LrcSettingsPage(QWidget *parnet)
	: QWidget(parnet),
	  m_lrcDemo(NULL)
{
	this->createUI();
	this->createConnect();
}

LrcSettingsPage::~LrcSettingsPage()
{
	if (m_lrcDemo != NULL)
	{
		m_lrcDemo->deleteLater();
		m_lrcDemo = NULL;
	}
	/*Else do nothing, and conintue.*/
}

void LrcSettingsPage::init()
{
	GlobalConfig *config = GlobalConfig::instance();
	
	m_fontNameCombo->setCurrentText(config->fontName());
	m_fontSizeCombo->setCurrentText(config->fontSize());
	m_fontTypeCombo->setCurrentText(config->fontType());
	m_noPlayShang = config->noPlayShang();
	this->setLineEditColor(m_noPlayEdit, m_noPlayShang);
	m_noPlayZhong = config->noPlayZhong();
	this->setLineEditColor(m_noPlayEdit2, m_noPlayZhong);
	m_noPlayXia = config->noPlayXia();
	this->setLineEditColor(m_noPlayEdit3, m_noPlayXia);
	m_playedShang = config->playedShang();
	this->setLineEditColor(m_playedEdit, m_playedShang);
	m_playedZhong = config->playedZhong();
	this->setLineEditColor(m_playedEdit2, m_playedZhong);
	m_playedXia = config->playedXia();
	this->setLineEditColor(m_playedEdit3, m_playedXia);
	m_shadowSlider->setValue(config->shadow());
	this->setDefaultChoiceColor(config->defaultChoiceColor());
}

void LrcSettingsPage::apply()
{
	GlobalConfig *config = GlobalConfig::instance();
	config->setFontName(m_fontNameCombo->currentText());
	config->setFontSize(m_fontSizeCombo->currentText());
	config->setFontType(m_fontTypeCombo->currentText());
	config->setNoPlayShang(m_noPlayShang);
	config->setNoPlayZhong(m_noPlayZhong);
	config->setNoPlayXia(m_noPlayXia);
	config->setPlayedShang(m_playedShang);
	config->setPlayedZhong(m_playedZhong);
	config->setPlayedXia(m_playedXia);
	config->setDefaultChoiceColor(m_defaultChoiceColor);
	config->setShadow(m_shadowSlider->value());

	if (!config->save())
	{
		QMessageBox::warning(this, TR("警告"), TR("保存歌词设置配置信息失败！"));
	}
	/*Else do nothing, and contine.*/
}

void LrcSettingsPage::createUI()
{
	//字体
	QGroupBox *fontGB = new QGroupBox(TR("字体"));

	m_fontLabel = new QLabel(this);
	m_fontLabel->setObjectName("m_fontLabel");
	m_fontLabel->setText(TR("字体"));

	m_fontNameCombo = new QComboBox(this);
	QFontDatabase font;
	//获取系统能显示中文的字体
	QStringList fontList = font.families();
	m_fontNameCombo->addItems(fontList);

	//字体大小
	m_fontSizeLabel = new QLabel(this);
	m_fontSizeLabel->setObjectName("m_fontSizeLabel");
	m_fontSizeLabel->setText(TR("字号"));

	m_fontSizeCombo = new QComboBox(this);
	QStringList fontSizeList;
	QString fontSizeStr;
	for (int i = 14; i <= 72; ++i)
	{
		fontSizeStr = QString::number(i, 10);
		fontSizeList << fontSizeStr;
	}
	m_fontSizeCombo->addItems(fontSizeList);

	//字体类型
	m_fontTypeLabel = new QLabel(this);
	m_fontTypeLabel->setObjectName("m_fontTypeLabel");
	m_fontTypeLabel->setText(TR("字形"));

	m_fontTypeCombo = new QComboBox(this);
	QStringList fontTypeList;
	fontTypeList << TR("常规") << TR("粗体") << TR("倾斜") << TR("粗体 倾斜");
	m_fontTypeCombo->addItems(fontTypeList);

	//颜色
	QGroupBox *colorGB = new QGroupBox(TR("颜色"));

	//未播放
	QGroupBox *noPlayGB = new QGroupBox(TR("未播放"));

	m_noPlayEdit = new QLineEdit(this);
	m_noPlayEdit->setObjectName("m_noPlayEdit");
	m_noPlayEdit->setReadOnly(true);

	m_noPlayEdit2 = new QLineEdit(this);
	m_noPlayEdit2->setObjectName("m_noPlayEdit3");
	m_noPlayEdit2->setReadOnly(true);

	m_noPlayEdit3 = new QLineEdit(this);
	m_noPlayEdit3->setObjectName("m_noPlayEdit3");
	m_noPlayEdit3->setReadOnly(true);

	//上部颜色
	m_changeNoPlayButton = new QPushButton(this);
	m_changeNoPlayButton->setObjectName("m_changeNoPlayButton");
	m_changeNoPlayButton->setText(TR("上部颜色"));

	//中部颜色
	m_changeNoPlayButton2 = new QPushButton(this);
	m_changeNoPlayButton2->setObjectName("m_changeNoPlayButton2");
	m_changeNoPlayButton2->setText(TR("中部颜色"));

	//下部颜色
	m_changeNoPlayButton3 = new QPushButton(this);
	m_changeNoPlayButton3->setObjectName("m_changeNoPlayButton3");
	m_changeNoPlayButton3->setText(TR("下部颜色"));

	//已播放
	QGroupBox *playedGB = new QGroupBox(TR("已播放"));

	m_playedEdit = new QLineEdit(this);
	m_playedEdit->setObjectName("m_playedEdit");
	m_playedEdit->setReadOnly(true);

	m_playedEdit2 = new QLineEdit(this);
	m_playedEdit2->setObjectName("m_playedEdit2");
	m_playedEdit2->setReadOnly(true);

	m_playedEdit3 = new QLineEdit(this);
	m_playedEdit3->setObjectName("m_playedEdit3");
	m_playedEdit3->setReadOnly(true);

	//上部颜色
	m_changePlayedButton = new QPushButton(this);
	m_changePlayedButton->setObjectName("m_changePlayedButton");
	m_changePlayedButton->setText(TR("上部颜色"));

	//中部颜色
	m_changePlayedButton2 = new QPushButton(this);
	m_changePlayedButton2->setObjectName("m_changePlayedButton2");
	m_changePlayedButton2->setText(TR("中部颜色"));

	//下部颜色
	m_changePlayedButton3 = new QPushButton(this);
	m_changePlayedButton3->setObjectName("m_changePlayedButton3");
	m_changePlayedButton3->setText(TR("下部颜色"));

	//预设方案
	m_yuSheButton = new QPushButton(this);
	m_yuSheButton->setObjectName("m_yuSheButton");
	m_yuSheButton->setText(TR("预设方案"));

	//恢复默认方案
	m_recoverDefaultColorButton = new QPushButton(this);
	m_recoverDefaultColorButton->setObjectName("m_recoverDefaultColorButton");
	m_recoverDefaultColorButton->setText(TR("恢复默认方案"));

	//灰度显示
	QGroupBox *shadowGB = new QGroupBox(TR("阴影效果"));

	m_shadowLabel = new QLabel(this);
	m_shadowLabel->setObjectName("m_shadowLabel");
	m_shadowLabel->setText(TR("阴影效果"));

	m_shadowSlider = new QSlider(Qt::Horizontal, this);
	m_shadowSlider->setObjectName("m_shadowSlider");
	m_shadowSlider->setRange(0, 255);

	m_shadowSpinBox = new QSpinBox(this);
	m_shadowSpinBox->setObjectName("m_shadowSpinBox");
	m_shadowSpinBox->setRange(0, 255);

	//歌词模板显示
	if (m_lrcDemo == NULL)
	{
		m_lrcDemo = new LrcDemo(this);
		m_lrcDemo->resize(300, 60);
	}
	/*Else do nothing, and continue.*/

	//经典白
	m_jingDianWhite = new QAction(this);
	m_jingDianWhite->setText(TR("经典白"));
	m_jingDianWhite->setIcon(QIcon(ImagesDir + "jingDianWhite.png"));

	//海洋蓝
	m_haiYangBlue = new QAction(this);
	m_haiYangBlue->setText(TR("海洋蓝"));
	m_haiYangBlue->setIcon(QIcon(ImagesDir + "haiYangBlue.png"));

	//火焰红
	m_huoYanRed = new QAction(this);
	m_huoYanRed->setText(TR("火焰红"));
	m_huoYanRed->setIcon(QIcon(ImagesDir + "huoYanRed.png"));

	//酷炫黑
	m_kuXuanBlack = new QAction(this);
	m_kuXuanBlack->setText(TR("酷炫黑"));
	m_kuXuanBlack->setIcon(QIcon(ImagesDir + "kuXuanBlack.png"));

	//秋色黄
	m_qiuSeYellow = new QAction(this);
	m_qiuSeYellow->setText(TR("秋色黄"));
	m_qiuSeYellow->setIcon(QIcon(ImagesDir + "qiuSeYellow.png"));

	//浪漫紫
	m_langManPurple = new QAction(this);
	m_langManPurple->setText(TR("浪漫紫"));
	m_langManPurple->setIcon(QIcon(ImagesDir + "langManPurple.png"));

	//自然绿
	m_ziRanGreen = new QAction(this);
	m_ziRanGreen->setText(TR("自然绿"));
	m_ziRanGreen->setIcon(QIcon(ImagesDir + "ziRanGreen.png"));

	m_presetMenu.addAction(m_jingDianWhite);
	m_presetMenu.addAction(m_haiYangBlue);
	m_presetMenu.addAction(m_huoYanRed);
	m_presetMenu.addAction(m_kuXuanBlack);
	m_presetMenu.addAction(m_qiuSeYellow);
	m_presetMenu.addAction(m_langManPurple);
	m_presetMenu.addAction(m_ziRanGreen);

	QHBoxLayout *fontLayout = new QHBoxLayout;
	fontLayout->addWidget(m_fontLabel);
	fontLayout->addWidget(m_fontNameCombo);
	fontLayout->addStretch();
	fontLayout->addWidget(m_fontSizeLabel);
	fontLayout->addWidget(m_fontSizeCombo);
	fontLayout->addStretch();
	fontLayout->addWidget(m_fontTypeLabel);
	fontLayout->addWidget(m_fontTypeCombo);
	fontGB->setLayout(fontLayout);

	QGridLayout *noPlayLayout = new QGridLayout;
	noPlayLayout->addWidget(m_noPlayEdit, 0, 0);
	noPlayLayout->addWidget(m_changeNoPlayButton, 0, 1);
	noPlayLayout->addWidget(m_noPlayEdit2, 1, 0);
	noPlayLayout->addWidget(m_changeNoPlayButton2, 1, 1);
	noPlayLayout->addWidget(m_noPlayEdit3, 2, 0);
	noPlayLayout->addWidget(m_changeNoPlayButton3, 2, 1);
	noPlayGB->setLayout(noPlayLayout);

	QGridLayout *playedLayout = new QGridLayout;
	playedLayout->addWidget(m_playedEdit, 0, 0);
	playedLayout->addWidget(m_changePlayedButton, 0, 1);
	playedLayout->addWidget(m_playedEdit2, 1, 0);
	playedLayout->addWidget(m_changePlayedButton2, 1, 1);
	playedLayout->addWidget(m_playedEdit3, 2, 0);
	playedLayout->addWidget(m_changePlayedButton3, 2, 1);
	playedGB->setLayout(playedLayout);

	QHBoxLayout *shadowLayout = new QHBoxLayout;
	shadowLayout->addWidget(m_shadowLabel);
	shadowLayout->addWidget(m_shadowSlider);
	shadowLayout->addWidget(m_shadowSpinBox);
	shadowLayout->addStretch();
	shadowGB->setLayout(shadowLayout);

	QHBoxLayout *colorLayout = new QHBoxLayout;
	colorLayout->addWidget(noPlayGB);
	colorLayout->addStretch();
	colorLayout->addWidget(playedGB);

	QHBoxLayout *buttonsLayout = new QHBoxLayout;
	buttonsLayout->addWidget(m_yuSheButton);
	buttonsLayout->addStretch();
	buttonsLayout->addWidget(m_recoverDefaultColorButton);

	QVBoxLayout *colorLayout2 = new QVBoxLayout;
	colorLayout2->addLayout(colorLayout);
	colorLayout2->addLayout(buttonsLayout);
	colorGB->setLayout(colorLayout2);

	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addWidget(fontGB);
	mainLayout->addWidget(colorGB);
	mainLayout->addWidget(shadowGB);
	mainLayout->addWidget(m_lrcDemo);
	this->setLayout(mainLayout);
}

void LrcSettingsPage::createConnect()
{
	connect(m_changeNoPlayButton, SIGNAL(clicked()), this, SLOT(handleChangeColor()));
	connect(m_changeNoPlayButton2, SIGNAL(clicked()), this, SLOT(handleChangeColor()));
	connect(m_changeNoPlayButton3, SIGNAL(clicked()), this, SLOT(handleChangeColor()));
	connect(m_changePlayedButton, SIGNAL(clicked()), this, SLOT(handleChangeColor()));
	connect(m_changePlayedButton2, SIGNAL(clicked()), this, SLOT(handleChangeColor()));
	connect(m_changePlayedButton3, SIGNAL(clicked()), this, SLOT(handleChangeColor()));

	connect(m_jingDianWhite, SIGNAL(triggered()), this, SLOT(handlePresetColorChanged()));
	connect(m_haiYangBlue, SIGNAL(triggered()), this, SLOT(handlePresetColorChanged()));
	connect(m_huoYanRed, SIGNAL(triggered()), this, SLOT(handlePresetColorChanged()));
	connect(m_kuXuanBlack, SIGNAL(triggered()), this, SLOT(handlePresetColorChanged()));
	connect(m_qiuSeYellow, SIGNAL(triggered()), this, SLOT(handlePresetColorChanged()));
	connect(m_langManPurple, SIGNAL(triggered()), this, SLOT(handlePresetColorChanged()));
	connect(m_ziRanGreen, SIGNAL(triggered()), this, SLOT(handlePresetColorChanged()));

	connect(m_yuSheButton, SIGNAL(clicked()), 
		this, SLOT(handlePresetColor()));
	connect(m_recoverDefaultColorButton, SIGNAL(clicked()),
		this, SLOT(handleRecoverDefaultSetting()));

	connect(m_fontNameCombo, SIGNAL(currentTextChanged(QString)), 
		this, SLOT(handleFontNameChanged(QString)));
	connect(m_fontSizeCombo, SIGNAL(currentTextChanged(QString)),
		this, SLOT(handleFontSizeChanged(QString)));
	connect(m_fontTypeCombo, SIGNAL(currentTextChanged(QString)),
		this, SLOT(handleFontTypeChanged(QString)));

	connect(m_noPlayEdit, SIGNAL(textChanged(QString)), 
		this, SLOT(handleNoPlayShangColorChanged()));
	connect(m_noPlayEdit2, SIGNAL(textChanged(QString)),
		this, SLOT(handleNoPlayZhongColorChanged()));
	connect(m_noPlayEdit3, SIGNAL(textChanged(QString)),
		this, SLOT(handleNoPlayXiaColorChanged()));

	connect(m_playedEdit, SIGNAL(textChanged(QString)),
		this, SLOT(handlePlayedShangColorChanged()));
	connect(m_playedEdit2, SIGNAL(textChanged(QString)),
		this, SLOT(handlePlayedZhongColorChanged()));
	connect(m_playedEdit3, SIGNAL(textChanged(QString)),
		this, SLOT(handlePlayedXiaColorChanged()));

	//slider和spinbox同步更新值
	connect(m_shadowSlider, SIGNAL(valueChanged(int)), m_shadowSpinBox, SLOT(setValue(int)));
	connect(m_shadowSpinBox, SIGNAL(valueChanged(int)), m_shadowSlider, SLOT(setValue(int)));
	connect(m_shadowSlider, SIGNAL(valueChanged(int)), this, SLOT(handleShadowChanged(int)));
}

void LrcSettingsPage::setLineEditColor(QLineEdit *lineEdit, const QColor &color)
{
	lineEdit->setText(QString("rgb(%1,%2,%3)").arg(color.red())
		.arg(color.green())
		.arg(color.blue()));
	lineEdit->setStyleSheet(QString("background-color: rgb(%1, %2, %3)").arg(color.red())
		.arg(color.green())
		.arg(color.blue()));
}

void LrcSettingsPage::handleChangeColor()
{
	QPushButton *button = dynamic_cast<QPushButton *>(sender());
	QColor preColor;

	if (button == m_changeNoPlayButton)
	{
		preColor = m_noPlayShang;
	}
	else if (button == m_changeNoPlayButton2)
	{
		preColor = m_noPlayZhong;
	}
	else if (button == m_changeNoPlayButton3)
	{
		preColor = m_noPlayXia;
	}
	else if (button == m_changePlayedButton)
	{
		preColor = m_playedShang;
	}
	else if (button == m_changePlayedButton2)
	{
		preColor = m_playedZhong;
	}
	else if (button == m_changePlayedButton3)
	{
		preColor = m_playedXia;
	}
	/*Else do nothing, and conintue.*/

	QColor color = QColorDialog::getColor(preColor, this, TR("选择颜色"),
		QColorDialog::DontUseNativeDialog);
	if (color.isValid())
	{
		QString colorStr = QString("rgb(%1,%2,%3)").arg(color.red())
			.arg(color.green())
			.arg(color.blue());
		QString colorStyleSheet = QString("background-color: rgb(%1, %2, %3)").arg(color.red())
			.arg(color.green())
			.arg(color.blue());

		if (button == m_changeNoPlayButton)
		{
			m_noPlayShang = color;
			m_noPlayEdit->setText(colorStr);
			m_noPlayEdit->setStyleSheet(colorStyleSheet);
		}
		else if (button == m_changeNoPlayButton2)
		{
			m_noPlayZhong = color;
			m_noPlayEdit2->setText(colorStr);
			m_noPlayEdit2->setStyleSheet(colorStyleSheet);
		}
		else if (button == m_changeNoPlayButton3)
		{
			m_noPlayXia = color;
			m_noPlayEdit3->setText(colorStr);
			m_noPlayEdit3->setStyleSheet(colorStyleSheet);
		}
		else if (button == m_changePlayedButton)
		{
			m_playedShang = color;
			m_playedEdit->setText(colorStr);
			m_playedEdit->setStyleSheet(colorStyleSheet);
		}
		else if (button == m_changePlayedButton2)
		{
			m_playedZhong = color;
			m_playedEdit2->setText(colorStr);
			m_playedEdit2->setStyleSheet(colorStyleSheet);
		}
		else if (button == m_changePlayedButton3)
		{
			m_playedXia = color;
			m_playedEdit3->setText(colorStr);
			m_playedEdit3->setStyleSheet(colorStyleSheet);
		}
		/*Else do nothing, and conintue.*/
	}
	/*Else do nothing, and conintue.*/
}

void LrcSettingsPage::handleFontNameChanged(const QString &fontName)
{
	m_lrcDemo->setFontName(fontName);
	emit sigLrcSettingsChanged();
}

void LrcSettingsPage::handleFontSizeChanged(const QString &fontSize)
{
	m_lrcDemo->setFontSize(fontSize);
	emit sigLrcSettingsChanged();
}

void LrcSettingsPage::handleFontTypeChanged(const QString &fontType)
{
	m_lrcDemo->setFontType(fontType);
	emit sigLrcSettingsChanged();
}

void LrcSettingsPage::handleNoPlayShangColorChanged()
{
	m_lrcDemo->setLinearGradient(m_noPlayShang);
	emit sigLrcSettingsChanged();
}

void LrcSettingsPage::handleNoPlayZhongColorChanged()
{
	m_lrcDemo->setLinearGradient2(m_noPlayZhong);
	emit sigLrcSettingsChanged();
}

void LrcSettingsPage::handleNoPlayXiaColorChanged()
{
	m_lrcDemo->setLinearGradient3(m_noPlayXia);
	emit sigLrcSettingsChanged();
}

void LrcSettingsPage::handlePlayedShangColorChanged()
{
	m_lrcDemo->setMaskLinearGradient(m_playedShang);
	emit sigLrcSettingsChanged();
}

void LrcSettingsPage::handlePlayedZhongColorChanged()
{
	m_lrcDemo->setMaskLinearGradient2(m_playedZhong);
	emit sigLrcSettingsChanged();
}

void LrcSettingsPage::handlePlayedXiaColorChanged()
{
	m_lrcDemo->setMaskLinearGradient3(m_playedXia);
	emit sigLrcSettingsChanged();
}

void LrcSettingsPage::handlePresetColor()
{
	m_presetMenu.exec(QCursor::pos());
}

void LrcSettingsPage::handleRecoverDefaultSetting()
{
	m_fontNameCombo->setCurrentText(FontName);
	m_fontSizeCombo->setCurrentText(FontSize);
	m_fontTypeCombo->setCurrentText(FontType);
	m_noPlayShang = NoPlayShang;
	this->setLineEditColor(m_noPlayEdit, m_noPlayShang);
	m_noPlayZhong = NoPlayZhong;
	this->setLineEditColor(m_noPlayEdit2, m_noPlayZhong);
	m_noPlayXia = NoPlayXia;
	this->setLineEditColor(m_noPlayEdit3, m_noPlayXia);
	m_playedShang = PlayedShang;
	this->setLineEditColor(m_playedEdit, m_playedShang);
	m_playedZhong = PlayedZhong;
	this->setLineEditColor(m_playedEdit2, m_playedZhong);
	m_playedXia = PlayedXia;
	this->setLineEditColor(m_playedEdit3, m_playedXia);
	m_shadowSlider->setValue(Shadow);

	m_defaultChoiceColor = "";
	m_jingDianWhite->setText(TR("经典白"));
	m_haiYangBlue->setText(TR("海洋蓝"));
	m_huoYanRed->setText(TR("火焰红"));
	m_kuXuanBlack->setText(TR("酷炫黑"));
	m_qiuSeYellow->setText(TR("秋色黄"));
	m_langManPurple->setText(TR("浪漫紫"));
	m_ziRanGreen->setText(TR("自然绿"));
}

void LrcSettingsPage::handlePresetColorChanged()
{
	QAction *action = dynamic_cast<QAction *>(sender());

	//经典白
	if (action == m_jingDianWhite)
	{
		m_defaultChoiceColor = TR("经典白");
		m_jingDianWhite->setText(TR("经典白 √"));
		m_haiYangBlue->setText(TR("海洋蓝"));
		m_huoYanRed->setText(TR("火焰红"));
		m_kuXuanBlack->setText(TR("酷炫黑"));
		m_qiuSeYellow->setText(TR("秋色黄"));
		m_langManPurple->setText(TR("浪漫紫"));
		m_ziRanGreen->setText(TR("自然绿"));
		m_noPlayShang = NoPlayWhiteShang;
		this->setLineEditColor(m_noPlayEdit, m_noPlayShang);
		m_noPlayZhong = NoPlayWhiteZhong;
		this->setLineEditColor(m_noPlayEdit2, m_noPlayZhong);
		m_noPlayXia = NoPlayWhiteXia;
		this->setLineEditColor(m_noPlayEdit3, m_noPlayXia);
		m_playedShang = PlayedWhiteShang;
		this->setLineEditColor(m_playedEdit, m_playedShang);
		m_playedZhong = PlayedWhiteZhong;
		this->setLineEditColor(m_playedEdit2, m_playedZhong);
		m_playedXia = PlayedWhiteXia;
		this->setLineEditColor(m_playedEdit3, m_playedXia);
	}
	//海洋蓝
	else if (action == m_haiYangBlue)
	{
		m_defaultChoiceColor = TR("海洋蓝");
		m_jingDianWhite->setText(TR("经典白"));
		m_haiYangBlue->setText(TR("海洋蓝 √"));
		m_huoYanRed->setText(TR("火焰红"));
		m_kuXuanBlack->setText(TR("酷炫黑"));
		m_qiuSeYellow->setText(TR("秋色黄"));
		m_langManPurple->setText(TR("浪漫紫"));
		m_ziRanGreen->setText(TR("自然绿"));
		m_noPlayShang = NoPlayBlueShang;
		this->setLineEditColor(m_noPlayEdit, m_noPlayShang);
		m_noPlayZhong = NoPlayBlueZhong;
		this->setLineEditColor(m_noPlayEdit2, m_noPlayZhong);
		m_noPlayXia = NoPlayBlueXia;
		this->setLineEditColor(m_noPlayEdit3, m_noPlayXia);
		m_playedShang = PlayedBlueShang;
		this->setLineEditColor(m_playedEdit, m_playedShang);
		m_playedZhong = PlayedBlueZhong;
		this->setLineEditColor(m_playedEdit2, m_playedZhong);
		m_playedXia = PlayedBlueXia;
		this->setLineEditColor(m_playedEdit3, m_playedXia);
	}
	//火焰红
	else if (action == m_huoYanRed)
	{
		m_defaultChoiceColor = TR("火焰红");
		m_jingDianWhite->setText(TR("经典白"));
		m_haiYangBlue->setText(TR("海洋蓝"));
		m_huoYanRed->setText(TR("火焰红 √"));
		m_kuXuanBlack->setText(TR("酷炫黑"));
		m_qiuSeYellow->setText(TR("秋色黄"));
		m_langManPurple->setText(TR("浪漫紫"));
		m_ziRanGreen->setText(TR("自然绿"));
		m_noPlayShang = NoPlayRedShang;
		this->setLineEditColor(m_noPlayEdit, m_noPlayShang);
		m_noPlayZhong = NoPlayRedZhong;
		this->setLineEditColor(m_noPlayEdit2, m_noPlayZhong);
		m_noPlayXia = NoPlayRedXia;
		this->setLineEditColor(m_noPlayEdit3, m_noPlayXia);
		m_playedShang = PlayedRedShang;
		this->setLineEditColor(m_playedEdit, m_playedShang);
		m_playedZhong = PlayedRedZhong;
		this->setLineEditColor(m_playedEdit2, m_playedZhong);
		m_playedXia = PlayedRedXia;
		this->setLineEditColor(m_playedEdit3, m_playedXia);
	}
	//酷炫黑
	else if (action == m_kuXuanBlack)
	{
		m_defaultChoiceColor = TR("酷炫黑");
		m_jingDianWhite->setText(TR("经典白"));
		m_haiYangBlue->setText(TR("海洋蓝"));
		m_huoYanRed->setText(TR("火焰红"));
		m_kuXuanBlack->setText(TR("酷炫黑 √"));
		m_qiuSeYellow->setText(TR("秋色黄"));
		m_langManPurple->setText(TR("浪漫紫"));
		m_ziRanGreen->setText(TR("自然绿"));
		m_noPlayShang = NoPlayBlackShang;
		this->setLineEditColor(m_noPlayEdit, m_noPlayShang);
		m_noPlayZhong = NoPlayBlackZhong;
		this->setLineEditColor(m_noPlayEdit2, m_noPlayZhong);
		m_noPlayXia = NoPlayBlackXia;
		this->setLineEditColor(m_noPlayEdit3, m_noPlayXia);
		m_playedShang = PlayedBlackShang;
		this->setLineEditColor(m_playedEdit, m_playedShang);
		m_playedZhong = PlayedBlackZhong;
		this->setLineEditColor(m_playedEdit2, m_playedZhong);
		m_playedXia = PlayedBlackXia;
		this->setLineEditColor(m_playedEdit3, m_playedXia);
	}
	//秋色黄
	else if (action == m_qiuSeYellow)
	{
		m_defaultChoiceColor = TR("秋色黄");
		m_jingDianWhite->setText(TR("经典白"));
		m_haiYangBlue->setText(TR("海洋蓝"));
		m_huoYanRed->setText(TR("火焰红"));
		m_kuXuanBlack->setText(TR("酷炫黑"));
		m_qiuSeYellow->setText(TR("秋色黄 √"));
		m_langManPurple->setText(TR("浪漫紫"));
		m_ziRanGreen->setText(TR("自然绿"));
		m_noPlayShang = NoPlayYellowShang;
		this->setLineEditColor(m_noPlayEdit, m_noPlayShang);
		m_noPlayZhong = NoPlayYellowZhong;
		this->setLineEditColor(m_noPlayEdit2, m_noPlayZhong);
		m_noPlayXia = NoPlayYellowXia;
		this->setLineEditColor(m_noPlayEdit3, m_noPlayXia);
		m_playedShang = PlayedYellowShang;
		this->setLineEditColor(m_playedEdit, m_playedShang);
		m_playedZhong = PlayedYellowZhong;
		this->setLineEditColor(m_playedEdit2, m_playedZhong);
		m_playedXia = PlayedYellowXia;
		this->setLineEditColor(m_playedEdit3, m_playedXia);
	}
	//浪漫紫
	else if (action == m_langManPurple)
	{
		m_defaultChoiceColor = TR("浪漫紫");
		m_jingDianWhite->setText(TR("经典白"));
		m_haiYangBlue->setText(TR("海洋蓝"));
		m_huoYanRed->setText(TR("火焰红"));
		m_kuXuanBlack->setText(TR("酷炫黑"));
		m_qiuSeYellow->setText(TR("秋色黄"));
		m_langManPurple->setText(TR("浪漫紫 √"));
		m_ziRanGreen->setText(TR("自然绿"));
		m_noPlayShang = NoPlayPurpleShang;
		this->setLineEditColor(m_noPlayEdit, m_noPlayShang);
		m_noPlayZhong = NoPlayPurpleZhong;
		this->setLineEditColor(m_noPlayEdit2, m_noPlayZhong);
		m_noPlayXia = NoPlayPurpleXia;
		this->setLineEditColor(m_noPlayEdit3, m_noPlayXia);
		m_playedShang = PlayedPurpleShang;
		this->setLineEditColor(m_playedEdit, m_playedShang);
		m_playedZhong = PlayedPurpleZhong;
		this->setLineEditColor(m_playedEdit2, m_playedZhong);
		m_playedXia = PlayedPurpleXia;
		this->setLineEditColor(m_playedEdit3, m_playedXia);
	}
	//自然绿
	else if (action == m_ziRanGreen)
	{
		m_defaultChoiceColor = TR("自然绿");
		m_jingDianWhite->setText(TR("经典白"));
		m_haiYangBlue->setText(TR("海洋蓝"));
		m_huoYanRed->setText(TR("火焰红"));
		m_kuXuanBlack->setText(TR("酷炫黑"));
		m_qiuSeYellow->setText(TR("秋色黄"));
		m_langManPurple->setText(TR("浪漫紫"));
		m_ziRanGreen->setText(TR("自然绿 √"));
		m_noPlayShang = NoPlayGreenShang;
		this->setLineEditColor(m_noPlayEdit, m_noPlayShang);
		m_noPlayZhong = NoPlayGreenZhong;
		this->setLineEditColor(m_noPlayEdit2, m_noPlayZhong);
		m_noPlayXia = NoPlayGreenXia;
		this->setLineEditColor(m_noPlayEdit3, m_noPlayXia);
		m_playedShang = PlayedGreenShang;
		this->setLineEditColor(m_playedEdit, m_playedShang);
		m_playedZhong = PlayedGreenZhong;
		this->setLineEditColor(m_playedEdit2, m_playedZhong);
		m_playedXia = PlayedGreenXia;
		this->setLineEditColor(m_playedEdit3, m_playedXia);
	}
	/*Else do nothing, and conintue.*/
}

void LrcSettingsPage::handleShadowChanged(const int &shadow)
{
	m_lrcDemo->setLrcDemoShadow(shadow);
	emit sigLrcSettingsChanged();
}

void LrcSettingsPage::setDefaultChoiceColor(const QString &color)
{
	m_defaultChoiceColor = color;
	if (color == TR("经典白"))
	{
		m_jingDianWhite->setText(TR("经典白 √"));
		m_haiYangBlue->setText(TR("海洋蓝"));
		m_huoYanRed->setText(TR("火焰红"));
		m_kuXuanBlack->setText(TR("酷炫黑"));
		m_qiuSeYellow->setText(TR("秋色黄"));
		m_langManPurple->setText(TR("浪漫紫"));
		m_ziRanGreen->setText(TR("自然绿"));
	}
	else if (color == TR("海洋蓝"))
	{
		m_jingDianWhite->setText(TR("经典白"));
		m_haiYangBlue->setText(TR("海洋蓝 √"));
		m_huoYanRed->setText(TR("火焰红"));
		m_kuXuanBlack->setText(TR("酷炫黑"));
		m_qiuSeYellow->setText(TR("秋色黄"));
		m_langManPurple->setText(TR("浪漫紫"));
		m_ziRanGreen->setText(TR("自然绿"));
	}
	else if (color == TR("火焰红"))
	{
		m_jingDianWhite->setText(TR("经典白"));
		m_haiYangBlue->setText(TR("海洋蓝"));
		m_huoYanRed->setText(TR("火焰红 √"));
		m_kuXuanBlack->setText(TR("酷炫黑"));
		m_qiuSeYellow->setText(TR("秋色黄"));
		m_langManPurple->setText(TR("浪漫紫"));
		m_ziRanGreen->setText(TR("自然绿"));
	}
	else if (color == TR("酷炫黑"))
	{
		m_jingDianWhite->setText(TR("经典白"));
		m_haiYangBlue->setText(TR("海洋蓝"));
		m_huoYanRed->setText(TR("火焰红"));
		m_kuXuanBlack->setText(TR("酷炫黑 √"));
		m_qiuSeYellow->setText(TR("秋色黄"));
		m_langManPurple->setText(TR("浪漫紫"));
		m_ziRanGreen->setText(TR("自然绿"));
	}
	else if (color == TR("秋色黄"))
	{
		m_jingDianWhite->setText(TR("经典白"));
		m_haiYangBlue->setText(TR("海洋蓝"));
		m_huoYanRed->setText(TR("火焰红"));
		m_kuXuanBlack->setText(TR("酷炫黑"));
		m_qiuSeYellow->setText(TR("秋色黄 √"));
		m_langManPurple->setText(TR("浪漫紫"));
		m_ziRanGreen->setText(TR("自然绿"));
	}
	else if (color == TR("浪漫紫"))
	{
		m_jingDianWhite->setText(TR("经典白"));
		m_haiYangBlue->setText(TR("海洋蓝"));
		m_huoYanRed->setText(TR("火焰红"));
		m_kuXuanBlack->setText(TR("酷炫黑"));
		m_qiuSeYellow->setText(TR("秋色黄"));
		m_langManPurple->setText(TR("浪漫紫 √"));
		m_ziRanGreen->setText(TR("自然绿"));
	}
	else if (color == TR("自然绿"))
	{
		m_jingDianWhite->setText(TR("经典白"));
		m_haiYangBlue->setText(TR("海洋蓝"));
		m_huoYanRed->setText(TR("火焰红"));
		m_kuXuanBlack->setText(TR("酷炫黑"));
		m_qiuSeYellow->setText(TR("秋色黄"));
		m_langManPurple->setText(TR("浪漫紫"));
		m_ziRanGreen->setText(TR("自然绿 √"));
	}
	/*Else do nothing, and conintue.*/
}
