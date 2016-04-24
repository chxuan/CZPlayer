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
* \file   EditHotkeyDialog.h
* \author chengxuan   787280310@qq.com
* \date   2015-01-24
* \brief  热键编辑对话框头文件。
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

#ifndef EDITHOTKEYDIALOG_H
#define EDITHOTKEYDIALOG_H

/*! \def EDITHOTKEYDIALOG_VERSION
*  版本控制宏，对应v3.0.0版本.
*/
#define EDITHOTKEYDIALOG_VERSION   0x030000

#include <QDialog>

class QEvent;
class QLineEdit;
class QLabel;
class QPushButton;

/*! 热键编辑对话框
*/
class EditHotkeyDialog : public QDialog
{
	Q_OBJECT

public:
	/*! 构造函数
	 *  \param parent 父窗口指针，默认为NULL
	 */
	EditHotkeyDialog(QWidget *parent = NULL);

	/*! 析构函数.*/
	~EditHotkeyDialog();

	/*! 初始化界面
	 *  \param currentHotKeyName 当前热键名称
	 *  \param currentHotKey 当前热键
	 */
	void init(const QString &currentHotKeyName, const QString &currentHotKey);

protected:
	/*! 虚函数，重新实现eventFilter函数
	 *  \param target QObject类型
	 *  \param event QEvent类型
	 *  \reurn 返回true代表不过滤该事件
	 */
	virtual bool eventFilter(QObject *target, QEvent *event);

signals:
	/*! 热键编辑完成
   	 * \param hotKeyName 热键名
	 * \param hotKey 热键
	 */
	void sigEditHotkeyFinished(const QString &hotKeyName, const QString &hotKey);

private slots:
	/*! 热键编辑完成
	 */
	void handleEditHotkeyFinished();

private:
	/*! 创建界面
	*/
	void createUI();

	/*! 信号与槽
	*/
	void createConnect();

	/*! 重置热键设置成功相关变量
	 */
	void hotKeySuccessReset();

	/*! 重置热键释放相关变量
	 */
	void keyReleaseReset();

private:
	QLabel *m_label;			/*!< 热键名称*/
	QLineEdit *m_edit;		    /*!< 编辑热键*/
	QPushButton *m_okButton;	/*!< 确定按钮*/
	QPushButton *m_cancelButton;/*!< 取消按钮*/

	bool m_isHeadKeyPress;		/*!< 热键头是否按下*/
	bool m_isTailkeyPress;		/*!< 热键尾是否按下*/
	bool m_isHotkeySuccess;		/*!< 热键是否按下*/

	bool m_isCtrlKeyPress;		/*!< Ctrl键是否按下*/
	bool m_isAltKeyPress;		/*!< Alt键是否按下*/
	bool m_isShiftKeyPress;		/*!< Shift键是否按下*/

	QString m_currentHotKeyName;/*!< 热键名称*/
	QString m_currentHotKey;	/*!< 热键*/
};

#endif //EDITHOTKEYDIALOG_H

