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
 * \file   SharedDialog.h
 * \author chengxuan   787280310@qq.com
 * \date   2015-01-06
 * \brief  歌曲分享对话框头文件。
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

#ifndef SHAREDDIALOG_H
#define SHAREDDIALOG_H

/*! \def SHAREDDIALOG_VERSION
 *  版本控制宏，对应v3.0.0版本.
 */
#define SHAREDDIALOG_VERSION   0x030000

#include <QDialog>
#include <QString>

class QLabel;
class QTextEdit;
class QCheckBox;
class QPushButton;

/*! 歌曲分享对话框
*/
class SharedDialog : public QDialog
{
	Q_OBJECT

public:
	/*! 构造函数
	 *  \param parent 父窗口指针，默认为NULL
     */
	SharedDialog(QWidget *parent = NULL);

	/*! 析构函数.*/
	~SharedDialog(void);

public:
	/*! 初始化分享对话框
	 *  \param albumImagePath 专辑图片路径
	 *  \param musicName 音乐名
	 *  \param albumUrlPath 专辑url
     */
	void init(const QString &albumImagePath, 
				const QString &musicName, 
				const QString &albumUrlPath);		

private slots:
	/*! 复选框状态改变
	 *  \param state 状态
     */
	void handleCheckBoxStateChanged(int state);									

	/*! 分享歌曲
     */
	void handleSharedMusic();													

private:
	QString m_albumUrlPath;					/*!< 专辑图片路径*/
	QString m_musicName;					/*!< 音乐名*/

	QLabel *m_albumLabel;					/*!< 专辑*/
	QTextEdit *m_textEdit;					/*!< 文本*/
	QLabel *m_sharedLabel;					/*!< 分享提示*/

	QCheckBox *m_xinlangweiboCheckBox;		/*!< 新浪微博*/
	QLabel *m_xinlangweiboLabel;			/*!< 新浪微博*/

	QCheckBox *m_tengxunweiboCheckBox;		/*!< 腾讯微博*/
	QLabel *m_tengxunweiboLabel;			/*!< 腾讯微博*/

	QCheckBox *m_qqkongjianCheckBox;		/*!< QQ空间*/
	QLabel *m_qqkongjianLabel;				/*!< QQ空间*/

	QCheckBox *m_renrenCheckBox;			/*!< 人人网*/
	QLabel *m_renrenLabel;					/*!< 人人网*/

	QCheckBox *m_kaixinCheckBox;			/*!< 开心网*/
	QLabel *m_kaixinLabel;					/*!< 开心网*/

	QCheckBox *m_doubanCheckBox;			/*!< 豆瓣网*/
	QLabel *m_doubanLabel;					/*!< 豆瓣网*/

	QCheckBox *m_souhuweiboCheckBox;		/*!< 搜狐微博*/
	QLabel *m_souhuweiboLabel;				/*!< 搜狐微博*/

	QCheckBox *m_qqhaoyouCheckBox;			/*!< QQ好友*/
	QLabel *m_qqhaoyouLabel;				/*!< QQ好友*/

	QPushButton *m_shareButton;				/*!< 分享按钮*/
	QPushButton *m_cancelButton;			/*!< 取消按钮*/

private:
	/*! 打开浏览器
	 *  \param url url链接
     */
	void openBrowser(const QString &url);						

	/*! 创建界面
     */
	void createUI();				

	/*! 信号与槽
     */
	void createConnect();	
};

#endif //SHAREDDIALOG_H

