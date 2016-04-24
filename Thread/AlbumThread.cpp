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
 * \file   AlbumThread.cpp
 * \author chengxuan   787280310@qq.com
 * \date   2014-12-24
 * \brief  专辑下载线程实现文件。
 * \version 3.0.0
 *
 * \verbatim
 * 历史
 *          3.0.0   创建,
 *          2014-12-24    by chengxuan
 *
 * \endverbatim
 *
 */

#include "AlbumThread.h"
#include <QTextCodec>
#include <QEventLoop>
#include <QNetworkAccessManager>
#include <QLabel>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonParseError>
#ifdef CZ_DEBUG
#include <QDebug>
#endif

AlbumThread::AlbumThread( const QString &fileName, 
							const QString &musicName, 
							const QString &musicArtist, 
							const QString &albumFilePath, 
							QObject *parent /*= NULL*/ )
	: QThread(parent),
	m_albumUrl(""),
	m_fileName(fileName),
	m_musicName(musicName),
	m_musicArtist(musicArtist),
	m_albumFilePath(albumFilePath),
	m_albumManager(NULL),
	m_albumManager2(NULL),
	m_albumManager3(NULL),
	m_albumReply(NULL)
{
	/*Do nothing.*/
}

AlbumThread::~AlbumThread()
{
	if (m_albumReply != NULL)
	{
		m_albumReply->deleteLater();
		m_albumReply = NULL;
	}
	/*Else do nothing, and continue.*/

	if (m_albumManager != NULL)
	{
		m_albumManager->deleteLater();
		m_albumManager = NULL;
	}
	/*Else do nothing, and continue.*/

	if (m_albumManager2 != NULL)
	{
		m_albumManager2->deleteLater();
		m_albumManager2 = NULL;
	}
	/*Else do nothing, and continue.*/

	if (m_albumManager3 != NULL)
	{
		m_albumManager3->deleteLater();
		m_albumManager3 = NULL;
	}
	/*Else do nothing, and continue.*/
}

void AlbumThread::run()
{
	QString url = "http://geci.me/api/lyric/" + m_musicName + "/" + m_musicArtist;
	m_albumManager = new QNetworkAccessManager;
	connect(m_albumManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(handleGetMusicAlbumJson(QNetworkReply*)));
	m_albumManager->get(QNetworkRequest(QUrl(url)));
	this->exec();
}

void AlbumThread::handleGetMusicAlbumJson(QNetworkReply *replay)
{
	QTextCodec *codec = QTextCodec::codecForName("utf8");
	QString albumStr = codec->toUnicode(replay->readAll());

	if (replay != NULL)
	{
		replay->deleteLater();
		replay = NULL;
	}
	/*Else do nothing, and continue.*/
	if (m_albumManager != NULL)
	{
		m_albumManager->deleteLater();
		m_albumManager = NULL;
	}
	/*Else do nothing, and continue.*/

	//没有连接到网络
    if (albumStr.isEmpty())
	{
#ifdef CZ_DEBUG
		qDebug() << "Downlaod album error.";
		qDebug() << __FILE__ << __FUNCTION__ << __LINE__;
#endif
		emit sigAlbumDownloadStatus(m_fileName, m_albumUrl, m_albumFilePath, AlbumDownloadFailed);
		return;
	}
	/*Else do nothing, and continue.*/

	//解析Json
	QJsonParseError error;
	QJsonDocument jsonDocument = QJsonDocument::fromJson(albumStr.toUtf8(), &error);
	if (error.error != QJsonParseError::NoError)
	{
#ifdef CZ_DEBUG
		qDebug() << "Downlaod album error.";
		qDebug() << __FILE__ << __FUNCTION__ << __LINE__;
#endif
		emit sigAlbumDownloadStatus(m_fileName, m_albumUrl, m_albumFilePath, AlbumDownloadFailed);
		return;
	}
	/*Else do nothing, and continue.*/

	if (!jsonDocument.isObject())
	{
#ifdef CZ_DEBUG
		qDebug() << "Downlaod album error.";
		qDebug() << __FILE__ << __FUNCTION__ << __LINE__;
#endif
		emit sigAlbumDownloadStatus(m_fileName, m_albumUrl, m_albumFilePath, AlbumDownloadFailed);
		return;
	}
	/*Else do nothing, and continue.*/

	QVariantMap result = jsonDocument.toVariant().toMap();
	//判断json里面是否包含url
	if (result["count"].toString() == "0")
	{
#ifdef CZ_DEBUG
		qDebug() << "Not found album url.";
		qDebug() << __FILE__ << __FUNCTION__ << __LINE__;
#endif
		emit sigAlbumDownloadStatus(m_fileName, m_albumUrl, m_albumFilePath, AlbumDownloadFailed);
		return;
	}
	/*Else do nothing, and continue.*/

	//得到专辑aid
	QString aid = "";
	QVariantList resultList = result["result"].toList();
	foreach(QVariant var, resultList)
	{
		QVariantMap resultMap = var.toMap();
		aid = resultMap["aid"].toString();
		break;
	}

	QString url = "http://geci.me/api/cover/" + aid;
	m_albumManager2 = new QNetworkAccessManager;
	m_albumManager2->get(QNetworkRequest(QUrl(url)));
	connect(m_albumManager2, SIGNAL(finished(QNetworkReply*)), this, SLOT(handleGetMusicAlbumJson2(QNetworkReply*)));
}

void AlbumThread::handleGetMusicAlbumJson2(QNetworkReply *replay)
{
	QTextCodec *codec = QTextCodec::codecForName("utf8");
	QString albumStr = codec->toUnicode(replay->readAll());

	if (replay != NULL)
	{
		replay->deleteLater();
		replay = NULL;
	}
	/*Else do nothing, and continue.*/
	if (m_albumManager2 != NULL)
	{
		m_albumManager2->deleteLater();
		m_albumManager2 = NULL;
	}
	/*Else do nothing, and continue.*/

	//没有连接到网络
    if (albumStr.isEmpty())
	{
#ifdef CZ_DEBUG
		qDebug() << "Downlaod album error.";
		qDebug() << __FILE__ << __FUNCTION__ << __LINE__;
#endif
		emit sigAlbumDownloadStatus(m_fileName, m_albumUrl, m_albumFilePath, AlbumDownloadFailed);
		return;
	}
	/*Else do nothing, and continue.*/

#if 0
	//判断xml里面是否包含url(url错误返回404页面)
	QStringList countList = albumStr.split(", \"code\":");
	QString countStr = countList.at(0);
	QStringList countList2 = countStr.split(": ");
	QString countStr2 = countList2.at(1);

	if (countStr2 == "0")
	{
#ifdef CZ_DEBUG
		qDebug() << "Downlaod album error.";
		qDebug() << __FILE__ << __FUNCTION__ << __LINE__;
#endif
		emit sigAlbumDownloadStatus(m_fileName, m_albumUrl, AlbumDownloadFailed);
		return;
	}
	/*Else do nothing, and continue.*/
#endif
	
	//解析Json
	QJsonParseError error;
	QJsonDocument jsonDocument = QJsonDocument::fromJson(albumStr.toUtf8(), &error);
	if (error.error != QJsonParseError::NoError)
	{
#ifdef CZ_DEBUG
		qDebug() << "Downlaod album error.";
		qDebug() << __FILE__ << __FUNCTION__ << __LINE__;
#endif
		emit sigAlbumDownloadStatus(m_fileName, m_albumUrl, m_albumFilePath, AlbumDownloadFailed);
		return;
	}
	/*Else do nothing, and continue.*/

	if (!jsonDocument.isObject())
	{
#ifdef CZ_DEBUG
		qDebug() << "Downlaod album error.";
		qDebug() << __FILE__ << __FUNCTION__ << __LINE__;
#endif
		emit sigAlbumDownloadStatus(m_fileName, m_albumUrl, m_albumFilePath, AlbumDownloadFailed);
		return;
	}
	/*Else do nothing, and continue.*/

	//得到专辑url
	QVariantMap result = jsonDocument.toVariant().toMap();
	QVariantMap resultMap = result["result"].toMap();

    QString imageUrl;
#if 1
    imageUrl = resultMap["cover"].toString();		//标准图片
#else
    imageUrl = resultMap["thumb"].toString();		//迷你图片
#endif

    this->setAlbumFromURL(QUrl(imageUrl));
}

void AlbumThread::setAlbumFromURL(const QUrl &url)
{
	m_albumFile.setFileName(m_albumFilePath);
	if (!m_albumFile.open(QIODevice::WriteOnly))
	{
		emit sigAlbumDownloadStatus(m_fileName, m_albumUrl, m_albumFilePath, AlbumDownloadFailed);
		return;
	}
	/*Else do nothing, and continue.*/

	m_albumUrl = url.toString();		

	m_albumManager3 = new QNetworkAccessManager;
	QEventLoop loop;
    m_albumReply = m_albumManager3->get(QNetworkRequest(url));
	connect(m_albumReply, SIGNAL(finished()), this, SLOT(handleReplayFinished()));
	connect(m_albumReply, SIGNAL(error(QNetworkReply::NetworkError)), 
		this, SLOT(handleReplyError(QNetworkReply::NetworkError)));
	//实现了数据分段下载保存，这样下载完所有数据再保存，要节省很多内存
	connect(m_albumReply, SIGNAL(readyRead()), this, SLOT(handleSaveAlbum()));
	loop.exec();
}

void AlbumThread::handleSaveAlbum()
{
	m_albumFile.write(m_albumReply->readAll());
}

void AlbumThread::handleReplayFinished()
{
	if (m_albumReply != NULL)
	{
		m_albumReply->deleteLater();
		m_albumReply = NULL;
	}
	/*Else do nothing, and continue.*/
	if (m_albumManager3 != NULL)
	{
		m_albumManager3->deleteLater();
		m_albumManager3 = NULL;
	}
	/*Else do nothing, and continue.*/

	m_albumFile.flush();
	if (m_albumFile.isOpen())
	{
		m_albumFile.close();
	}
	/*Else do nothing, and continue.*/

	emit sigAlbumDownloadStatus(m_fileName, m_albumUrl, m_albumFilePath, AlbumDownloadSuccess);
}

void AlbumThread::handleReplyError(QNetworkReply::NetworkError networkError)
{
#ifdef CZ_DEBUG
	qDebug() << errorCodeToString(networkError);
#endif
}

QString AlbumThread::errorCodeToString( QNetworkReply::NetworkError networkError )
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
