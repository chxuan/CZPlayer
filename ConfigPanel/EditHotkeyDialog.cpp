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
* \file   EditHotkeyDialog.cpp
* \author chengxuan   787280310@qq.com
* \date   2015-01-24
* \brief  热键编辑对话框实现文件。
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

#include "EditHotkeyDialog.h"
#include "TR.h"
#include <QLineEdit>
#include <QHBoxLayout>
#include <QEvent>
#include <QKeyEvent>
#include <QLabel>
#include <QPushButton>
#ifdef CZ_DEBUG
#include <QDebug>
#endif 

/*****************************************************************************
  这里用来检查版本的对应
 *****************************************************************************/
#if TR_VERSION < 0x030000
#error  "TR version error."
#endif

EditHotkeyDialog::EditHotkeyDialog(QWidget *parent)
	: QDialog(parent),
	m_isHeadKeyPress(false),
	m_isTailkeyPress(false),
	m_isHotkeySuccess(false),
	m_isCtrlKeyPress(false),
	m_isAltKeyPress(false),
	m_isShiftKeyPress(false)
{
	this->createUI();
	this->createConnect();
}

EditHotkeyDialog::~EditHotkeyDialog()
{
	/*Do nothing.*/
}

void EditHotkeyDialog::createUI()
{
	this->setWindowTitle(TR("编辑热键"));
	//this->setWindowFlags(Qt::Drawer);	//取消对话框图标

	m_label = new QLabel(this);
	m_edit = new QLineEdit(this);
	m_edit->setReadOnly(true);
	m_edit->installEventFilter(this);
	QHBoxLayout *edithLayout = new QHBoxLayout;
	edithLayout->addWidget(m_label);
	edithLayout->addWidget(m_edit);

	m_cancelButton = new QPushButton(TR("取消"), this);
	m_okButton = new QPushButton(TR("确定"), this);
	QHBoxLayout *buttonhLayout = new QHBoxLayout;
	buttonhLayout->addStretch();
	buttonhLayout->addWidget(m_cancelButton);
	buttonhLayout->addWidget(m_okButton);

	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addLayout(edithLayout);
	mainLayout->addLayout(buttonhLayout);
	this->setLayout(mainLayout);
}

void EditHotkeyDialog::createConnect()
{
	connect(m_cancelButton, SIGNAL(clicked()), this, SLOT(close()));
	connect(m_okButton, SIGNAL(clicked()), this, SLOT(handleEditHotkeyFinished()));
}

void EditHotkeyDialog::init(const QString &currentHotKeyName, const QString &currentHotKey)
{
	m_currentHotKeyName = currentHotKeyName;
	m_currentHotKey = currentHotKey;
	m_label->setText(m_currentHotKeyName + TR("："));
	m_edit->setText(m_currentHotKey);
}

void EditHotkeyDialog::handleEditHotkeyFinished()
{
	emit sigEditHotkeyFinished(m_currentHotKeyName, m_edit->text());
	this->close();
}

bool EditHotkeyDialog::eventFilter(QObject *target, QEvent *event)
{
	if (target == m_edit)
	{
		if (event->type() == QEvent::KeyPress)
		{
			QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
			int key = keyEvent->key();

			switch (key)
			{
			/*Ctrl、Alt、Shift键*/
			case Qt::Key_Control:
				if (m_isTailkeyPress) { break; }

				if (m_isAltKeyPress) { m_edit->setText("Ctrl+Alt+"); m_isAltKeyPress = false; }
				else if (m_isShiftKeyPress) { m_edit->setText("Ctrl+Shift+"); m_isShiftKeyPress = false; }
				else { m_edit->setText("Ctrl+"); }
				m_isCtrlKeyPress = true;
				m_isHeadKeyPress = true;
				m_isHotkeySuccess = false;
				m_okButton->setEnabled(false);
				break;
			case Qt::Key_Alt:
				if (m_isTailkeyPress) { break; }

				if (m_isCtrlKeyPress) { m_edit->setText("Ctrl+Alt+"); m_isCtrlKeyPress = false; }
				else if (m_isShiftKeyPress) { break; }
				else { m_edit->setText("Alt+"); }
				m_isAltKeyPress = true;
				m_isHeadKeyPress = true;
				m_isHotkeySuccess = false;
				m_okButton->setEnabled(false);
				break;
			case Qt::Key_Shift:
				if (m_isTailkeyPress) { break; }

				if (m_isCtrlKeyPress) { m_edit->setText("Ctrl+Shift+"); m_isCtrlKeyPress = false; }
				else if (m_isAltKeyPress) { break; }
				else { m_edit->setText("Shift+"); }
				m_isShiftKeyPress = true;
				m_isHeadKeyPress = true;
				m_isHotkeySuccess = false;
				m_okButton->setEnabled(false);
				break;

			/*字母键*/
			case Qt::Key_A: if (m_isHeadKeyPress) { m_edit->setText(m_edit->text() + "A"); this->hotKeySuccessReset(); } break;
			case Qt::Key_B: if (m_isHeadKeyPress) { m_edit->setText(m_edit->text() + "B"); this->hotKeySuccessReset(); } break;
			case Qt::Key_C: if (m_isHeadKeyPress) { m_edit->setText(m_edit->text() + "C"); this->hotKeySuccessReset(); } break;
			case Qt::Key_D: if (m_isHeadKeyPress) { m_edit->setText(m_edit->text() + "D"); this->hotKeySuccessReset(); } break;
			case Qt::Key_E: if (m_isHeadKeyPress) { m_edit->setText(m_edit->text() + "E"); this->hotKeySuccessReset(); } break;
			case Qt::Key_F: if (m_isHeadKeyPress) { m_edit->setText(m_edit->text() + "F"); this->hotKeySuccessReset(); } break;
			case Qt::Key_G: if (m_isHeadKeyPress) { m_edit->setText(m_edit->text() + "G"); this->hotKeySuccessReset(); } break;
			case Qt::Key_H: if (m_isHeadKeyPress) { m_edit->setText(m_edit->text() + "H"); this->hotKeySuccessReset(); } break;
			case Qt::Key_I: if (m_isHeadKeyPress) { m_edit->setText(m_edit->text() + "I"); this->hotKeySuccessReset(); } break;
			case Qt::Key_J: if (m_isHeadKeyPress) { m_edit->setText(m_edit->text() + "J"); this->hotKeySuccessReset(); } break;
			case Qt::Key_K: if (m_isHeadKeyPress) { m_edit->setText(m_edit->text() + "K"); this->hotKeySuccessReset(); } break;
			case Qt::Key_L: if (m_isHeadKeyPress) { m_edit->setText(m_edit->text() + "L"); this->hotKeySuccessReset(); } break;
			case Qt::Key_M: if (m_isHeadKeyPress) { m_edit->setText(m_edit->text() + "M"); this->hotKeySuccessReset(); } break;
			case Qt::Key_N: if (m_isHeadKeyPress) { m_edit->setText(m_edit->text() + "N"); this->hotKeySuccessReset(); } break;
			case Qt::Key_O: if (m_isHeadKeyPress) { m_edit->setText(m_edit->text() + "O"); this->hotKeySuccessReset(); } break;
			case Qt::Key_P: if (m_isHeadKeyPress) { m_edit->setText(m_edit->text() + "P"); this->hotKeySuccessReset(); } break;
			case Qt::Key_Q: if (m_isHeadKeyPress) { m_edit->setText(m_edit->text() + "Q"); this->hotKeySuccessReset(); } break;
			case Qt::Key_R: if (m_isHeadKeyPress) { m_edit->setText(m_edit->text() + "R"); this->hotKeySuccessReset(); } break;
			case Qt::Key_S: if (m_isHeadKeyPress) { m_edit->setText(m_edit->text() + "S"); this->hotKeySuccessReset(); } break;
			case Qt::Key_T: if (m_isHeadKeyPress) { m_edit->setText(m_edit->text() + "T"); this->hotKeySuccessReset(); } break;
			case Qt::Key_U: if (m_isHeadKeyPress) { m_edit->setText(m_edit->text() + "U"); this->hotKeySuccessReset(); } break;
			case Qt::Key_V: if (m_isHeadKeyPress) { m_edit->setText(m_edit->text() + "V"); this->hotKeySuccessReset(); } break;
			case Qt::Key_W: if (m_isHeadKeyPress) { m_edit->setText(m_edit->text() + "W"); this->hotKeySuccessReset(); } break;
			case Qt::Key_X: if (m_isHeadKeyPress) { m_edit->setText(m_edit->text() + "X"); this->hotKeySuccessReset(); } break;
			case Qt::Key_Y: if (m_isHeadKeyPress) { m_edit->setText(m_edit->text() + "Y"); this->hotKeySuccessReset(); } break;
			case Qt::Key_Z: if (m_isHeadKeyPress) { m_edit->setText(m_edit->text() + "Z"); this->hotKeySuccessReset(); } break;

			/*F键*/
			case Qt::Key_F1: if (m_isHeadKeyPress) { m_edit->setText(m_edit->text() + "F1"); this->hotKeySuccessReset(); } break;
			case Qt::Key_F2: if (m_isHeadKeyPress) { m_edit->setText(m_edit->text() + "F2"); this->hotKeySuccessReset(); } break;
			case Qt::Key_F3: if (m_isHeadKeyPress) { m_edit->setText(m_edit->text() + "F3"); this->hotKeySuccessReset(); } break;
			case Qt::Key_F4: if (m_isHeadKeyPress) { m_edit->setText(m_edit->text() + "F4"); this->hotKeySuccessReset(); } break;
			case Qt::Key_F5: if (m_isHeadKeyPress) { m_edit->setText(m_edit->text() + "F5"); this->hotKeySuccessReset(); } break;
			case Qt::Key_F6: if (m_isHeadKeyPress) { m_edit->setText(m_edit->text() + "F6"); this->hotKeySuccessReset(); } break;
			case Qt::Key_F7: if (m_isHeadKeyPress) { m_edit->setText(m_edit->text() + "F7"); this->hotKeySuccessReset(); } break;
			case Qt::Key_F8: if (m_isHeadKeyPress) { m_edit->setText(m_edit->text() + "F8"); this->hotKeySuccessReset(); } break;
			case Qt::Key_F9: if (m_isHeadKeyPress) { m_edit->setText(m_edit->text() + "F9"); this->hotKeySuccessReset(); } break;
			case Qt::Key_F10: if (m_isHeadKeyPress) { m_edit->setText(m_edit->text() + "F10"); this->hotKeySuccessReset(); } break;
			case Qt::Key_F11: if (m_isHeadKeyPress) { m_edit->setText(m_edit->text() + "F11"); this->hotKeySuccessReset(); } break;
			case Qt::Key_F12: if (m_isHeadKeyPress) { m_edit->setText(m_edit->text() + "F12"); this->hotKeySuccessReset(); } break;

			/*空格键*/
			case Qt::Key_Space: if (m_isHeadKeyPress) { m_edit->setText(m_edit->text() + "Space"); this->hotKeySuccessReset(); } break;
			default:
				break;
			}
			return true;
		}
		/*Else do nothing, and conintue.*/

		if (event->type() == QEvent::KeyRelease)
		{
			QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
			int key = keyEvent->key();

			switch (key)
			{
			/*Ctrl、Alt、Shift键*/
			case Qt::Key_Control: if (m_isCtrlKeyPress && !m_isHotkeySuccess) { this->keyReleaseReset(); } m_okButton->setEnabled(true); break;
			case Qt::Key_Alt: if (m_isAltKeyPress && !m_isHotkeySuccess) { this->keyReleaseReset(); } m_okButton->setEnabled(true); break;
			case Qt::Key_Shift: if (m_isShiftKeyPress && !m_isHotkeySuccess) { this->keyReleaseReset(); } m_okButton->setEnabled(true); break;

			/*字母键*/
			case Qt::Key_A:
			case Qt::Key_B:
			case Qt::Key_C:
			case Qt::Key_D:
			case Qt::Key_E:
			case Qt::Key_F:
			case Qt::Key_G:
			case Qt::Key_H:
			case Qt::Key_I:
			case Qt::Key_J:
			case Qt::Key_K:
			case Qt::Key_L:
			case Qt::Key_M:
			case Qt::Key_N:
			case Qt::Key_O:
			case Qt::Key_P:
			case Qt::Key_Q:
			case Qt::Key_R:
			case Qt::Key_S:
			case Qt::Key_T:
			case Qt::Key_U:
			case Qt::Key_V:
			case Qt::Key_W:
			case Qt::Key_X:
			case Qt::Key_Y:
			case Qt::Key_Z:

			/*F键*/
			case Qt::Key_F1:
			case Qt::Key_F2:
			case Qt::Key_F3:
			case Qt::Key_F4:
			case Qt::Key_F5:
			case Qt::Key_F6:
			case Qt::Key_F7:
			case Qt::Key_F8:
			case Qt::Key_F9:
			case Qt::Key_F10:
			case Qt::Key_F11:
			case Qt::Key_F12:

			/*空格键*/
			case Qt::Key_Space:
				m_isTailkeyPress = false;
				break;

			default:
				break;
			}
			return true;
		}
		/*Else do nothing, and conintue.*/
	}

	return QWidget::eventFilter(target, event);
}

void EditHotkeyDialog::hotKeySuccessReset()
{
	m_isCtrlKeyPress = false;
	m_isAltKeyPress = false;
	m_isShiftKeyPress = false;
	m_isHeadKeyPress = false;
	m_isHotkeySuccess = true;
	m_isTailkeyPress = true;
}

void EditHotkeyDialog::keyReleaseReset()
{
	m_edit->setText(m_currentHotKey);
	m_isCtrlKeyPress = false;
	m_isAltKeyPress = false;
	m_isShiftKeyPress = false;
	m_isHeadKeyPress = false;
}
