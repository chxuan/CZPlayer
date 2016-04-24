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
* \file   LrcSettingsPage.h
* \author chengxuan   787280310@qq.com
* \date   2015-01-24
* \brief  歌词设置页面头文件。
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

#ifndef LRCSETTINGSPAGE_H
#define LRCSETTINGSPAGE_H

/*! \def LRCSETTINGSPAGE_VERSION
*  版本控制宏，对应v3.0.0版本.
*/
#define LRCSETTINGSPAGE_VERSION   0x030000

#include <QWidget>
#include <QString>
#include <QColor>
#include <QMenu>

class LrcDemo;
class QLabel;
class QSpinBox;
class QLineEdit;
class QSlider;
class QPushButton;
class QAction;
class QComboBox;

/*! 歌词设置页面
*/
class LrcSettingsPage : public QWidget
{
    Q_OBJECT

public:
	/*! 构造函数
	 *  \param parent 父窗口指针，默认为NULL
	 */
    LrcSettingsPage(QWidget *parent = NULL);

	/*! 析构函数.*/
    ~LrcSettingsPage();

	/*! 初始页面
	 */
	void init();

	/*! 应用改变
	 */
	void apply();

signals:
	/*! 当歌词界面配置信息发生改变时发送该信号
	 */
	void sigLrcSettingsChanged();

private slots:
	/*! 改变颜色
	 */
	void handleChangeColor();

	/*! 歌词字体改变
	 * \param fontName 字体名称
	 */
	void handleFontNameChanged(const QString &fontName);

	/*! 歌词字体大小改变
	 * \param fontSize 字体大小
	 */
	void handleFontSizeChanged(const QString &fontSize);

	/*! 歌词类型改变
	 * \param fontType 歌词类型
	 */
	void handleFontTypeChanged(const QString &fontType);

	/*! 歌词没有播放上边缘颜色改变
	 */
	void handleNoPlayShangColorChanged();

	/*! 歌词没有播放中间颜色改变
	 */
	void handleNoPlayZhongColorChanged();

	/*! 歌词没有播放下边缘颜色改变
	 */
	void handleNoPlayXiaColorChanged();

	/*! 歌词播放后上边缘颜色改变
	 */
	void handlePlayedShangColorChanged();

	/*! 歌词播放后中间颜色改变
	 */
	void handlePlayedZhongColorChanged();

	/*! 歌词播放后下边缘颜色改变
	 */
	void handlePlayedXiaColorChanged();

	/*! 选择歌词预设颜色
	 */
	void handlePresetColor();

	/*! 恢复默认设置
	 */
	void handleRecoverDefaultSetting();

	/*! 预设歌词颜色改变
	 */
	void handlePresetColorChanged();

	/*! 歌词灰度改变
	 */
	void handleShadowChanged(const int &shadow);

private:
	/*! 创建界面
	 */
	void createUI();

	/*! 信号与槽
   	 */
	void createConnect();

	/*! 改变QLineEdit颜色
	 *  \param lineEdit QLineEdit指针
	 *  \param color 颜色
	 */
	void setLineEditColor(QLineEdit *lineEdit, const QColor &color);

	/*! 设置预设颜色
	 *  \param color 颜色
	 */
	void setDefaultChoiceColor(const QString &color);

private:
    LrcDemo *m_lrcDemo;								/*!< 歌词Demo*/

    QLabel *m_fontLabel;							/*!< 字体名称*/
	QComboBox *m_fontNameCombo;							/*!< 字体名称*/

    QLabel *m_fontSizeLabel;						/*!< 字体大小*/
	QComboBox *m_fontSizeCombo;						/*!< 字体大小*/

    QLabel *m_fontTypeLabel;						/*!< 字体类型*/
	QComboBox *m_fontTypeCombo;						/*!< 字体类型*/

    QLabel *m_shadowLabel;							/*!< 灰度*/
	QSlider *m_shadowSlider;						/*!< 灰度*/
	QSpinBox *m_shadowSpinBox;						/*!< 灰度*/

    QLineEdit *m_noPlayEdit;						/*!< 没有播放上边缘颜色*/
	QPushButton *m_changeNoPlayButton;				/*!< 没有播放上边缘颜色*/
	QColor m_noPlayShang;							/*!< 没有播放上边缘颜色*/

    QLineEdit *m_noPlayEdit2;						/*!< 没有播放中间颜色*/
	QPushButton *m_changeNoPlayButton2;				/*!< 没有播放中间颜色*/
	QColor m_noPlayZhong;							/*!< 没有播放中间颜色*/

    QLineEdit *m_noPlayEdit3;						/*!< 没有播放下边缘颜色*/
	QPushButton *m_changeNoPlayButton3;				/*!< 没有播放下边缘颜色*/
	QColor m_noPlayXia;								/*!< 没有播放下边缘颜色*/

    QLineEdit *m_playedEdit;						/*!< 播放过后的上边缘颜色*/
	QPushButton *m_changePlayedButton;				/*!< 播放过后的上边缘颜色*/
	QColor m_playedShang;							/*!< 播放过后的上边缘颜色*/

    QLineEdit *m_playedEdit2;						/*!< 播放过后的中间颜色*/
	QPushButton *m_changePlayedButton2;				/*!< 播放过后的中间颜色*/
	QColor m_playedZhong;							/*!< 播放过后的中间颜色*/

    QLineEdit *m_playedEdit3;						/*!< 播放过后的下边缘颜色*/
	QPushButton *m_changePlayedButton3;				/*!< 播放过后的下边缘颜色*/
	QColor m_playedXia;								/*!< 播放过后的下边缘颜色*/
    
	QPushButton *m_yuSheButton;						/*!< 预设颜色*/
	QPushButton *m_recoverDefaultColorButton;		/*!< 恢复默认颜色*/

	QMenu m_presetMenu;								/*!< 预设颜色*/
	QString m_defaultChoiceColor;					/*!< 预设默认颜色*/

    QAction *m_jingDianWhite;						/*!< 经典白*/
    QAction *m_haiYangBlue;							/*!< 海洋蓝*/
    QAction *m_huoYanRed;							/*!< 火焰红*/
    QAction *m_kuXuanBlack;							/*!< 酷炫黑*/
    QAction *m_qiuSeYellow;							/*!< 秋色黄*/
    QAction *m_langManPurple;						/*!< 浪漫紫*/
    QAction *m_ziRanGreen;							/*!< 自然绿*/
};

#endif // LRCSETTINGSPAGE_H
