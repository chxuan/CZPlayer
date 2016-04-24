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
 * \file   LrcThread.cpp
 * \author chengxuan   787280310@qq.com
 * \date   2014-12-28
 * \brief  歌词下载线程头文件。
 * \version 3.0.0
 *
 * \verbatim
 * 历史
 *          3.0.0   创建,
 *          2014-12-28    by chengxuan
 *
 * \endverbatim
 *
 */

#include "LrcThread.h"
#include <QtXml>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QLabel>
#ifdef CZ_DEBUG
#include <QDebug>
#endif

LrcThread::LrcThread( const QString &musicName, 
					const QString &musicArtist, 
					const QString &lrcFilePath, 
					QObject *parent /*= NULL*/ )
	: QThread(parent),
	m_musicName(musicName),
	m_musicArtist(musicArtist),
	m_lrcFilePath(lrcFilePath),
	m_lrcManager(NULL),
	m_lrcManager2(NULL)
{
	/*Do nothing.*/
}

LrcThread::~LrcThread()
{
	if (m_lrcManager != NULL)
	{
		m_lrcManager->deleteLater();
		m_lrcManager = NULL;
	}
	/*Else do nothing, and continue.*/

	if (m_lrcManager2 != NULL)
	{
		m_lrcManager2->deleteLater();
		m_lrcManager2 = NULL;
	}
	/*Else do nothing, and continue.*/
}

void LrcThread::run()
{
	QString url = "http://box.zhangmen.baidu.com/x?op=12&count=1&title=" + m_musicName + "$$" + m_musicArtist + "$$$";
	m_lrcManager = new QNetworkAccessManager;
	m_lrcManager->get(QNetworkRequest(QUrl(url)));
	connect(m_lrcManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(handleGetMusicXML(QNetworkReply*)));
	this->exec();
}

void LrcThread::handleGetMusicXML(QNetworkReply *replay)
{
	QTextCodec *codec = QTextCodec::codecForName("utf8");
	QString musicStr = codec->toUnicode(replay->readAll());

	if (replay != NULL)
	{
		replay->deleteLater();
		replay = NULL;
	}
	/*Else do nothing, and continue.*/
	if (m_lrcManager != NULL)
	{
		m_lrcManager->deleteLater();
		m_lrcManager = NULL;
	}
	/*Else do nothing, and continue.*/

	//没有连接到网络
    if (musicStr.isEmpty())
	{
#ifdef CZ_DEBUG
		qDebug() << "Downlaod lrc error.";
		qDebug() << __FILE__ << __FUNCTION__ << __LINE__;
#endif
		emit sigLrcDownloadStatus(m_musicName, m_musicArtist, m_lrcFilePath, LrcDownloadFailed);
		return;
	}
	/*Else do nothing, and continue.*/

	QString errorStr;
	int errorLine;
	int errorColumn;

	QDomDocument doc;
	if (!doc.setContent(musicStr, false, &errorStr, &errorLine, &errorColumn))
	{
#ifdef CZ_DEBUG
		qDebug() << "Downlaod lrc error.";
		qDebug() << __FILE__ << __FUNCTION__ << __LINE__;
#endif
		emit sigLrcDownloadStatus(m_musicName, m_musicArtist, m_lrcFilePath, LrcDownloadFailed);
		return;
	}
	/*Else do nothing, and continue.*/

	QDomElement root = doc.documentElement();
	if (root.tagName() != "result")
	{
		emit sigLrcDownloadStatus(m_musicName, m_musicArtist, m_lrcFilePath, LrcDownloadFailed);
		return;
	}
	/*Else do nothing, and continue.*/

	//获取lrcid
	QString lrcId;
	QDomNodeList nodeList = root.childNodes();
	for (int i = 0; i < nodeList.count(); ++i)
	{
		QDomNode node = nodeList.at(i);
        if (node.isNull())
		{
            emit sigLrcDownloadStatus(m_musicName, m_musicArtist, m_lrcFilePath, LrcDownloadFailed);
            return;
		}
        /*Else do nothing, and continue.*/

        if (node.nodeType() != QDomNode::ElementNode)
        {
            continue;
        }
        /*Else do nothing , and continue*/

        QDomElement element = node.toElement();
        if (element.tagName() == "count")
        {
            if (element.text() == "0")
            {
                emit sigLrcDownloadStatus(m_musicName, m_musicArtist, m_lrcFilePath, LrcDownloadFailed);
                return;
            }
            /*Else do nothing, and continue.*/
        }
        else if (element.tagName() == "url")
        {
            QDomNodeList nodeList2 = element.childNodes();
            for (int i = 0; i < nodeList2.count(); ++i)
            {
                QDomNode node2 = nodeList2.at(i);
                if (node2.isNull())
                {
                    emit sigLrcDownloadStatus(m_musicName, m_musicArtist, m_lrcFilePath, LrcDownloadFailed);
                    return;
                }
                /*Else do nothing , and continue*/

                if (node2.nodeType() != QDomNode::ElementNode)
                {
                    continue;
                }
                /*Else do nothing , and continue*/

                QDomElement element2 = node2.toElement();
                if (element2.tagName() == "lrcid")
                {
                    lrcId = element2.text();
                    break;
                }
                /*Else do nothing, and continue.*/
            }
            break;
        }
        /*Else do nothing, and continue.*/
	}

	//得到歌词所在文件夹
	bool ok;
	int id = lrcId.toInt(&ok);
	if (!ok)
	{
		emit sigLrcDownloadStatus(m_musicName, m_musicArtist, m_lrcFilePath, LrcDownloadFailed);
		return;
	}
	/*Else do nothing, and continue.*/

	int folder = id / 100;
	//歌词url
	QString lrcUrl = "http://box.zhangmen.baidu.com/bdlrc/" + QString::number(folder) + "/" + lrcId + ".lrc";

	this->setLrcFromURL(lrcUrl);
}

void LrcThread::setLrcFromURL(const QUrl &url)
{
	m_lrcFile.setFileName(m_lrcFilePath);
    if (!m_lrcFile.open(QIODevice::WriteOnly))
    {
        emit sigLrcDownloadStatus(m_musicName, m_musicArtist, m_lrcFilePath, LrcDownloadFailed);
        return;
    }
    /*Else do nothing, and continue.*/

	m_lrcManager2 = new QNetworkAccessManager;
    m_lrcManager2->get(QNetworkRequest(url));
	connect(m_lrcManager2, SIGNAL(finished(QNetworkReply*)), this, SLOT(handleSaveLrc(QNetworkReply*)));
}

void LrcThread::handleSaveLrc(QNetworkReply *replay)
{
	m_lrcFile.write(replay->readAll());
	m_lrcFile.flush();
	if (m_lrcFile.isOpen())
	{
		m_lrcFile.close();
	}
	/*Else do nothing, and continue.*/

	if (replay != NULL)
	{
		replay->deleteLater();
		replay = NULL;
	}
	/*Else do nothing, and continue.*/
	if (m_lrcManager2 != NULL)
	{
		m_lrcManager2->deleteLater();
		m_lrcManager2 = NULL;
	}
	/*Else do nothing, and continue.*/

	emit sigLrcDownloadStatus(m_musicName, m_musicArtist, m_lrcFilePath, LrcDownloadSuccess);
}




