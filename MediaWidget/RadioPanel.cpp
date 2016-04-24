
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
 * \file   RadioPanel.cpp
 * \author chengxuan   787280310@qq.com
 * \date   2015-02-23
 * \brief  电台面板实现文件。
 * \version 3.0.0
 *
 * \verbatim
 * 历史
 *          3.0.0   创建,
 *          2015-02-23    by chengxuan
 *
 * \endverbatim
 *
 */

#include "RadioPanel.h"
#include "NoFocusDelegate.h"
#include "Global.h"
#include "TR.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QTableWidget>
#include <QFile>
#include <QHeaderView>
#include <QtXml>
#include <QUrl>
#include <QPushButton>
#ifdef CZ_DEBUG
//#define XML_DEBUG
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

RadioPanel::RadioPanel(QWidget *parent)
    : QWidget(parent),
      m_currentRow(0),
      m_playStatus(Stoped)
{
    this->createUI();
    this->createConnect();
}

RadioPanel::~RadioPanel()
{
    m_radioPlayer->stop();
}

void RadioPanel::handlePlayRadio(const int &row)
{
    m_currentRow = row;
    m_radioInfo = m_radioInfoMap[m_currentRow];
    m_radioPlayer->stop();
    m_radioPlayer->setMedia(QUrl(m_radioInfo.url()));
    m_radioPlayer->play();
    m_currentRadioLabel->setText(TR("当前播放电台：%1").arg(m_radioInfo.name()));
    m_radioTable->selectRow(m_currentRow);
    m_playButton->setStyleSheet(QString("QPushButton:!hover{border-image: url(%1audioPause.png);}"
        "QPushButton:hover{border-image: url(%1audioPause2.png);}"
        "QPushButton:pressed{border-image: url(%1audioPause.png);}").arg(ImagesDir));
    m_playStatus = Playing;
}

void RadioPanel::handlePlayRadio()
{
    if (m_radioTable->rowCount() == 0)
    {
        return;
    }
    /*Else do nothing, and continue.*/

    if (m_playStatus == Playing)
    {
        m_playButton->setStyleSheet(QString("QPushButton:!hover{border-image: url(%1audioPlay.png);}"
            "QPushButton:hover{border-image: url(%1audioPlay2.png);}"
            "QPushButton:pressed{border-image: url(%1audioPlay.png);}").arg(ImagesDir));
        m_playStatus = Stoped;
        m_radioPlayer->stop();
        m_currentRadioLabel->setText(TR("当前播放电台："));
    }
    else if (m_playStatus == Stoped)
    {
        m_playButton->setStyleSheet(QString("QPushButton:!hover{border-image: url(%1audioPause.png);}"
            "QPushButton:hover{border-image: url(%1audioPause2.png);}"
            "QPushButton:pressed{border-image: url(%1audioPause.png);}").arg(ImagesDir));
        m_playStatus = Playing;

        if (m_radioInfo.name().isEmpty() || m_radioInfo.url().isEmpty())
        {
            this->handlePlayRadio(0);
        }
        else
        {
            m_radioPlayer->stop();
            m_radioPlayer->setMedia(QUrl(m_radioInfo.url()));
            m_radioPlayer->play();
            m_currentRadioLabel->setText(TR("当前播放电台：%1").arg(m_radioInfo.name()));
        }
    }
    /*Else do nothing, and continue.*/
}

void RadioPanel::handlePreRadio()
{
    if (m_radioTable->rowCount() == 0)
    {
        return;
    }
    /*Else do nothing, and continue.*/

    if (m_currentRow + 1 == 1)
    {
        m_currentRow = m_radioTable->rowCount() - 1;

    }
    else
    {
        --m_currentRow;
    }
    this->handlePlayRadio(m_currentRow);
}

void RadioPanel::handleNextRadio()
{
    if (m_radioTable->rowCount() == 0)
    {
        return;
    }
    /*Else do nothing, and continue.*/

    if (m_currentRow + 1 == m_radioTable->rowCount())
    {
        m_currentRow = 0;
    }
    else
    {
        ++m_currentRow;
    }
    this->handlePlayRadio(m_currentRow);
}

void RadioPanel::handleError(QMediaPlayer::Error error)
{
    QString errorStr = this->errorCodeToString(error);
#ifdef CZ_DEBUG
    qDebug() << errorStr;
#endif
}

void RadioPanel::createUI()
{
    //当前电台
    m_currentRadioLabel = new QLabel(this);
    m_currentRadioLabel->setObjectName(tr("searchInfoLabel"));
    m_currentRadioLabel->setText(TR("当前播放电台："));

    //播放/暂停按钮
    m_playButton = new QPushButton(this);
    m_playButton->setFixedSize(32, 32);
    m_playButton->setToolTip(TR("播放"));
    m_playButton->setStyleSheet(QString("QPushButton:!hover{border-image: url(%1audioPlay.png);}"
        "QPushButton:hover{border-image: url(%1audioPlay2.png);}"
        "QPushButton:pressed{border-image: url(%1audioPlay.png);}").arg(ImagesDir));

    //上一首
    m_preButton = new QPushButton(this);
    m_preButton->setFixedSize(32, 32);
    m_preButton->setToolTip(TR("上一首"));
    m_preButton->setStyleSheet(QString("QPushButton:!hover{border-image: url(%1audioRew.png);}"
        "QPushButton:hover{border-image: url(%1audioRew2.png);}"
        "QPushButton:pressed{border-image: url(%1audioRew.png);}").arg(ImagesDir));

    //下一首
    m_nextButton = new QPushButton(this);
    m_nextButton->setFixedSize(32, 32);
    m_nextButton->setToolTip(TR("下一首"));
    m_nextButton->setStyleSheet(QString("QPushButton:!hover{border-image: url(%1audioFwd.png);}"
        "QPushButton:hover{border-image: url(%1audioFwd2.png);}"
        "QPushButton:pressed{border-image: url(%1audioFwd.png);}").arg(ImagesDir));

    QHBoxLayout *buttonhLayout = new QHBoxLayout;
    buttonhLayout->addWidget(m_preButton);
    buttonhLayout->addWidget(m_playButton);
    buttonhLayout->addWidget(m_nextButton);

    //电台列表
    m_radioTable = new QTableWidget(this);
    this->setTableWidgetStyle();

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(m_currentRadioLabel);
    mainLayout->addLayout(buttonhLayout);
    mainLayout->addWidget(m_radioTable);
    this->setLayout(mainLayout);

    QList<RadioInfoEntity> radioInfos = this->readXML("./RadioInfo.xml");
    foreach (RadioInfoEntity entity, radioInfos)
    {
        this->addRadioInfo(entity);
    }

    m_radioPlayer = new QMediaPlayer;
}

void RadioPanel::createConnect()
{
    connect(m_playButton, SIGNAL(clicked()), this, SLOT(handlePlayRadio()));
    connect(m_preButton, SIGNAL(clicked()), this, SLOT(handlePreRadio()));
    connect(m_nextButton, SIGNAL(clicked()), this, SLOT(handleNextRadio()));
    connect(m_radioTable, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(handlePlayRadio(int)));
    connect(m_radioPlayer, SIGNAL(error(QMediaPlayer::Error)), this, SLOT(handleError(QMediaPlayer::Error)));
}

void RadioPanel::addRadioInfo(const RadioInfoEntity &entity)
{
    //电台名称
    QTableWidgetItem *radioNameItem = new QTableWidgetItem;
    radioNameItem->setTextAlignment(Qt::AlignCenter);
    radioNameItem->setText(entity.name());
    radioNameItem->setToolTip(entity.name());

    int currentRows = m_radioTable->rowCount();
    m_radioTable->insertRow(currentRows);
    m_radioTable->setItem(currentRows, 0, radioNameItem);
    m_radioInfoMap[currentRows] = entity;
}

void RadioPanel::setTableWidgetStyle()
{
    m_radioTable->setRowCount(0);
    m_radioTable->setColumnCount(1);

    QStringList headList;
    headList << TR("电台名称");
    m_radioTable->setHorizontalHeaderLabels(headList);

    m_radioTable->setSelectionMode(QAbstractItemView::SingleSelection);              //设置只能选中单行
    m_radioTable->setSelectionBehavior(QAbstractItemView::SelectRows);               //设置选择习惯为选择行
    m_radioTable->setEditTriggers(QAbstractItemView::NoEditTriggers);                //设置内容不可编辑
    m_radioTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);    //设置内容自动横向填满可用区域
    m_radioTable->setFrameStyle(QFrame::NoFrame);                                    //去掉边框
    m_radioTable->setShowGrid(false);                                                //设置不显示网格
    m_radioTable->setItemDelegate(new NoFocusDelegate());                            //去掉选中单元格的虚线
    m_radioTable->horizontalHeader()->setHighlightSections(false);                   //点击表时不对表头行光亮（获取焦点）
    m_radioTable->verticalHeader()->setVisible(false);                               //设置垂直头不可见
    m_radioTable->horizontalHeader()->setSectionsClickable(false);                   //不响应鼠标单击
}

QList<RadioInfoEntity> RadioPanel::readXML(const QString &filePath)
{
    //判断文件是否存在
    QFile file;
    bool isExist = file.exists(filePath);
    if (!isExist)
    {
#ifdef XML_DEBUG
        qDebug() << filePath << "Not exits.";
        qDebug() << __FILE__ << __FUNCTION__ << __LINE__;
#endif
        return QList<RadioInfoEntity>();
    }
    /*Else do nothing, and continue.*/

    QString errorStr;
    int errorLine;
    int errorColumn;

    file.setFileName(filePath);
    file.open(QFile::ReadOnly);
    QDomDocument doc;
    if (!doc.setContent(&file, false, &errorStr, &errorLine, &errorColumn))
    {
        file.close();
#ifdef XML_DEBUG
        qDebug() << "Read xml file error, line = " << QString::number(errorLine)
                 << ", column = " << QString::number(errorColumn)
                 << ", error string = " << errorStr;
        qDebug() << __FILE__ << __FUNCTION__ << __LINE__;
#endif
        return QList<RadioInfoEntity>();
    }
    /*Else do nothing , and continue*/

    //获取根元素
    QDomElement root = doc.documentElement();
    if (root.tagName() != "RadioInfo")
    {
        file.close();
        return QList<RadioInfoEntity>();
    }
    /*Else do nothing , and continue*/

    QList<RadioInfoEntity> radioInfos;
    //获得root所有子节点的列表
    QDomNodeList nodeList = root.childNodes();
    for (int i = 0; i < nodeList.count(); ++i)
    {
        QDomNode node = nodeList.at(i);
        if (node.isNull())
        {
            file.close();
            return QList<RadioInfoEntity>();
        }
        /*Else do nothing , and continue*/

        if (node.nodeType() != QDomNode::ElementNode)
        {
            continue;
        }
        /*Else do nothing , and continue*/

        //转换成元素
        QDomElement element = node.toElement();
        if (element.tagName() == "Radio")
        {
            RadioInfoEntity radioInfo(element.attribute("name"), element.text());
            radioInfos.append(radioInfo);
#ifdef XML_DEBUG
            qDebug() << element.attribute("name") << element.text();
#endif
        }
        /*Else do nothing, and continue.*/
    }

    file.close();
    return radioInfos;
}

QString RadioPanel::errorCodeToString(QMediaPlayer::Error error)
{
    QString codeStr;
    switch (error)
    {
    case QMediaPlayer::NoError:
        codeStr = "QMediaPlayer::NoError";
        break;
    case QMediaPlayer::ResourceError:
        codeStr = "QMediaPlayer::ResourceError";
        break;
    case QMediaPlayer::FormatError:
        codeStr = "QMediaPlayer::FormatError";
        break;
    case QMediaPlayer::NetworkError:
        codeStr = "QMediaPlayer::NetworkError";
        break;
    case QMediaPlayer::AccessDeniedError:
        codeStr = "QMediaPlayer::AccessDeniedError";
        break;
    case QMediaPlayer::ServiceMissingError:
        codeStr = "QMediaPlayer::ServiceMissingError";
        break;
    default:
        codeStr = "Unknown error.";
        break;
    }
    return codeStr;
}



