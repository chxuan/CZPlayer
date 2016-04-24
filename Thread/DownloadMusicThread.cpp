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
 * \file   DownloadMusicThread.cpp
 * \author chengxuan   787280310@qq.com
 * \date   2015-03-03
 * \brief  歌曲下载线程实现文件。
 * \version 3.0.0
 *
 * \verbatim
 * 历史
 *          3.0.0   创建,
 *          2015-03-03    by chengxuan
 *
 * \endverbatim
 *
 */

#include "DownloadMusicThread.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QTextCodec>
#include <QEventLoop>
#include <QtXML>
#ifdef CZ_DEBUG
#include <QDebug>
#endif

DownloadMusicThread::DownloadMusicThread(const QString &musicName, const QString artist, const QString &musicPath, QObject *parent)
    : m_musicName(musicName),
      m_artist(artist),
      QThread(parent),
      m_musicManager(NULL),
      m_musicManager2(NULL),
      m_musicReply(NULL),
      m_times(0)
{
    m_filePath = QString("%1/%2 - %3.mp3").arg(musicPath).arg(m_musicName).arg(m_artist);
}

DownloadMusicThread::~DownloadMusicThread()
{
    if (m_musicReply != NULL)
    {
        m_musicReply->deleteLater();
        m_musicReply = NULL;
    }
    /*Else do nothing, and continue.*/

    if (m_musicManager != NULL)
    {
        m_musicManager->deleteLater();
        m_musicManager = NULL;
    }
    /*Else do nothing, and continue.*/

    if (m_musicManager2 != NULL)
    {
        m_musicManager2->deleteLater();
        m_musicManager2 = NULL;
    }
    /*Else do nothing, and continue.*/
}

void DownloadMusicThread::run()
{
    QString url = "http://box.zhangmen.baidu.com/x?op=12&count=1&title=" + m_musicName + "$$" + m_artist + "$$$";
    m_musicManager = new QNetworkAccessManager;
    m_musicManager->get(QNetworkRequest(QUrl(url)));
    connect(m_musicManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(handleGetMusicXML(QNetworkReply*)));
    this->exec();
}

void DownloadMusicThread::handleGetMusicXML(QNetworkReply *replay)
{
    QTextCodec *codec = QTextCodec::codecForName("utf8");
    QString musicStr = codec->toUnicode(replay->readAll());

    if (replay != NULL)
    {
        replay->deleteLater();
        replay = NULL;
    }
    /*Else do nothing, and continue.*/
    if (m_musicManager != NULL)
    {
        m_musicManager->deleteLater();
        m_musicManager = NULL;
    }
    /*Else do nothing, and continue.*/

    //没有连接到网络
    if (musicStr.isEmpty())
    {
#ifdef CZ_DEBUG
        qDebug() << "Downlaod music error.";
        qDebug() << __FILE__ << __FUNCTION__ << __LINE__;
#endif
        emit sigMusicDownloadStatus(MusicDownloadFailed);
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
        qDebug() << "Downlaod music error.";
        qDebug() << __FILE__ << __FUNCTION__ << __LINE__;
#endif
        emit sigMusicDownloadStatus(MusicDownloadFailed);
        return;
    }
    /*Else do nothing, and continue.*/

    QDomElement root = doc.documentElement();
    if (root.tagName() != "result")
    {
        emit sigMusicDownloadStatus(MusicDownloadFailed);
        return;
    }
    /*Else do nothing, and continue.*/

    //获取音乐url
    QString firstPartUrl;	//前半部分
    QString lastPartUrl;	//后半部分
    QDomNodeList nodeList = root.childNodes();//获得root所有子节点的列表
    for (int i = 0; i < nodeList.count(); ++i)
    {
        QDomNode node = nodeList.at(i);
        if (node.isNull())
        {
            emit sigMusicDownloadStatus(MusicDownloadFailed);
            return;
        }
        /*Else do nothing , and continue*/

        if (node.nodeType() != QDomNode::ElementNode)
        {
            continue;
        }
        /*Else do nothing , and continue*/

        QDomElement element = node.toElement();
        if (element.tagName() == "count")
        {
            if (element.text() == "0")//返回元素文本
            {
                emit sigMusicDownloadStatus(MusicDownloadFailed);
                return;
            }
            /*Else do nothing , and continue*/
        }
        else if (element.tagName() == "url")
        {
            QDomNodeList nodeList2 = element.childNodes();//获得url所有子节点的列表
            for (int i = 0; i < nodeList2.count(); ++i)
            {
                QDomNode node2 = nodeList2.at(i);
                if (node2.isNull())
                {
                    emit sigMusicDownloadStatus(MusicDownloadFailed);
                    return;
                }
                /*Else do nothing , and continue*/

                if (node.nodeType() != QDomNode::ElementNode)
                {
                    continue;
                }
                /*Else do nothing , and continue*/

                QDomElement element2 = node2.toElement();
                if (element2.tagName() == "encode")
                {
                    firstPartUrl = element2.text();
                }
                else if (element2.tagName() == "decode")
                {
                    lastPartUrl = element2.text();
                    break;
                }
                /*Else do nothing , and continue*/
            }
            break;
        }
        /*Else do nothing , and continue*/
    }

    //判断是否包含xcode，防止歌曲下载错误！
    if (!lastPartUrl.contains("xcode"))
    {
        emit sigMusicDownloadStatus(MusicDownloadFailed);
        return;
    }
    /*Else do nothing , and continue*/

    //音乐url
    QString url = firstPartUrl + lastPartUrl;
    this->setMusicURL(QUrl(url));
}

void DownloadMusicThread::handleSaveMusic()
{
    //得到歌曲文件的大小
    ++m_times;
    if (m_times == 1)
    {
        QVariant var = m_musicReply->header(QNetworkRequest::ContentLengthHeader);
        bool ok;
        double size = var.toDouble(&ok);
        if (!ok)
        {
            emit sigMusicDownloadStatus(MusicDownloadFailed);
            return;
        }
        /*Else do nothing , and continue*/

        emit sigMusicSize(size);
    }
    /*Else do nothing, and continue.*/

    m_musicFile.write(m_musicReply->readAll());
}

void DownloadMusicThread::handleReplayFinished()
{
    if (m_musicReply != NULL)
    {
        m_musicReply->deleteLater();
        m_musicReply = NULL;
    }
    /*Else do nothing, and continue.*/
    if (m_musicManager2 != NULL)
    {
        m_musicManager2->deleteLater();
        m_musicManager2 = NULL;
    }
    /*Else do nothing, and continue.*/

    m_musicFile.flush();
    if (m_musicFile.isOpen())
    {
        m_musicFile.close();
    }
    /*Else do nothing, and continue.*/

    emit sigMusicDownloadStatus(MusicDownloadSuccess);
}

void DownloadMusicThread::handleReplyError(QNetworkReply::NetworkError networkError)
{
#ifdef CZ_DEBUG
    qDebug() << errorCodeToString(networkError);
#endif
}

void DownloadMusicThread::setMusicURL(const QUrl &url)
{
    m_musicFile.setFileName(m_filePath);
    if (!m_musicFile.open(QIODevice::WriteOnly))
    {
        emit sigMusicDownloadStatus(MusicDownloadFailed);
        return;
    }
    /*Else do nothing, and continue.*/

    m_musicManager2 = new QNetworkAccessManager;
    QEventLoop loop;
    m_musicReply = m_musicManager2->get(QNetworkRequest(url));

    connect(m_musicReply, SIGNAL(downloadProgress(qint64,qint64)), this, SIGNAL(sigUpdateDataProgress(qint64,qint64)));
    connect(m_musicReply, SIGNAL(finished()), this, SLOT(handleReplayFinished()));
    connect(m_musicReply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(handleReplyError(QNetworkReply::NetworkError)));
    connect(m_musicReply, SIGNAL(readyRead()), this, SLOT(handleSaveMusic()));

    loop.exec();
}

QString DownloadMusicThread::errorCodeToString(QNetworkReply::NetworkError networkError)
{
    QString codeStr;
    switch (networkError)
    {
    case QNetworkReply::ConnectionRefusedError:
        codeStr = "QNetworkReply::ConnectionRefusedError";
        break;
    case QNetworkReply::RemoteHostClosedError:
        codeStr = "QNetworkReply::RemoteHostClosedError";
        break;
    case QNetworkReply::HostNotFoundError:
        codeStr = "QNetworkReply::HostNotFoundError";
        break;
    case QNetworkReply::TimeoutError:
        codeStr = "QNetworkReply::TimeoutError";
        break;
    case QNetworkReply::OperationCanceledError:
        codeStr = "QNetworkReply::OperationCanceledError";
        break;
    case QNetworkReply::SslHandshakeFailedError:
        codeStr = "QNetworkReply::SslHandshakeFailedError";
        break;
    case QNetworkReply::TemporaryNetworkFailureError:
        codeStr = "QNetworkReply::TemporaryNetworkFailureError";
        break;
    case QNetworkReply::ProxyConnectionRefusedError:
        codeStr = "QNetworkReply::ProxyConnectionRefusedError";
        break;
    case QNetworkReply::ProxyConnectionClosedError:
        codeStr = "QNetworkReply::ProxyConnectionClosedError";
        break;
    case QNetworkReply::ProxyNotFoundError:
        codeStr = "QNetworkReply::ProxyNotFoundError";
        break;
    case QNetworkReply::ProxyTimeoutError:
        codeStr = "QNetworkReply::ProxyTimeoutError";
        break;
    case QNetworkReply::ProxyAuthenticationRequiredError:
        codeStr = "QNetworkReply::ProxyAuthenticationRequiredError";
        break;
    case QNetworkReply::ContentAccessDenied:
        codeStr = "QNetworkReply::ContentAccessDenied";
        break;
    case QNetworkReply::ContentOperationNotPermittedError:
        codeStr = "QNetworkReply::ContentOperationNotPermittedError";
        break;
    case QNetworkReply::ContentNotFoundError:
        codeStr = "QNetworkReply::ContentNotFoundError";
        break;
    case QNetworkReply::AuthenticationRequiredError:
        codeStr = "QNetworkReply::AuthenticationRequiredError";
        break;
    case QNetworkReply::ContentReSendError:
        codeStr = "QNetworkReply::ContentReSendError";
        break;
    case QNetworkReply::ProtocolUnknownError:
        codeStr = "QNetworkReply::ProtocolUnknownError";
        break;
    case QNetworkReply::ProtocolInvalidOperationError:
        codeStr = "QNetworkReply::ProtocolInvalidOperationError";
        break;
    case QNetworkReply::UnknownNetworkError:
        codeStr = "QNetworkReply::UnknownNetworkError";
        break;
    case QNetworkReply::UnknownProxyError:
        codeStr = "QNetworkReply::UnknownProxyError";
        break;
    case QNetworkReply::UnknownContentError:
        codeStr = "QNetworkReply::UnknownContentError";
        break;
    case QNetworkReply::ProtocolFailure:
        codeStr = "QNetworkReply::ProtocolFailure";
        break;
    default:
        codeStr = "Unknown error.";
        break;
    }
    return codeStr;
}
