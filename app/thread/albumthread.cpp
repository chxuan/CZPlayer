#include "stdafx.h"
#include <parser.h>
#include "albumthread.h"
#include "static.h"
#include "czplayerconfig.h"

AlbumThread::AlbumThread(QString musicName, QString name, QString artist, QLabel *albumImage)
	: m_strMusicName(musicName), m_name(name), m_musicArtist(artist), m_albumImage(albumImage)
{
	//    QString urlStr = "http://geci.me/api/lyric/" + m_name + "/" + m_musicArtist;
	//    albumManager = new QNetworkAccessManager;
	//    albumManager->get(QNetworkRequest(QUrl(urlStr)));//得到url
	//    //信号与槽
	//    connect(albumManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(getMusicAlbumXML(QNetworkReply*)));
	this->startFunc();
}

AlbumThread::~AlbumThread()
{
}

void AlbumThread::run()
{
}

//开始下载
void AlbumThread::startFunc()
{
	QString urlStr = "http://geci.me/api/lyric/" + m_name + "/" + m_musicArtist;
	albumManager = new QNetworkAccessManager;
	albumManager->get(QNetworkRequest(QUrl(urlStr)));//得到url
	//信号与槽
	connect(albumManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(slot_GetMusicAlbumJson(QNetworkReply*)));
}

//得到音乐专辑（json格式）
void AlbumThread::slot_GetMusicAlbumJson(QNetworkReply *replay)
{
	QTextCodec *codec = QTextCodec::codecForName("utf8");//转换成utf8编码格式
	QString albumStr = codec->toUnicode(replay->readAll());

	//没有连接到网络
	if (albumStr == "")
	{
		qDebug() << "下载超时，请检查您的网络或者本机防火墙设置";
		emit sig_AlbumDownloadFinished(m_strMusicName, "");	//专辑下载完成
		m_albumImage->setPixmap(QPixmap(tr(":/images/albumImage.png")));
		return;
	}

	//解析Json
	QJson::Parser parser;
	bool ok;
	QVariantMap result = parser.parse(albumStr.toUtf8(), &ok).toMap();
	if (!ok)
	{
		qDebug() << "转换成QVariantMap失败";
		emit sig_AlbumDownloadFinished(m_strMusicName, "");	//专辑下载完成
		m_albumImage->setPixmap(QPixmap(tr(":/images/albumImage.png")));
		return;
	}

	//判断json里面是否包含url
	if (result["count"].toInt() == 0)
	{
		emit sig_AlbumDownloadFinished(m_strMusicName, "");	//专辑下载完成
		m_albumImage->setPixmap(QPixmap(tr(":/images/albumImage.png")));
		qDebug() << "没有下载到专辑";
		return;
	}

	//得到专辑aid
	QString sAid = "";
	QVariantList resultList = result["result"].toList();
	foreach (QVariant var, resultList)
	{
		QVariantMap resultMap = var.toMap();
		sAid = resultMap["aid"].toString();
		break;
	}

	QString urlStr = "http://geci.me/api/cover/" + sAid;
	//qDebug() << urlStr;
	albumManager2 = new QNetworkAccessManager;
	albumManager2->get(QNetworkRequest(QUrl(urlStr)));//得到url
	//信号与槽
	connect(albumManager2, SIGNAL(finished(QNetworkReply*)), this, SLOT(slot_GetMusicAlbumJson2(QNetworkReply*)));
	replay->deleteLater();
}

//得到音乐专辑（json格式）
void AlbumThread::slot_GetMusicAlbumJson2(QNetworkReply *replay)
{
	QTextCodec *codec = QTextCodec::codecForName("utf8");//转换成utf8编码格式
	QString albumStr = codec->toUnicode(replay->readAll());

	//没有连接到网络
	if (albumStr == "")
	{
		qDebug() << "无法连接到网络";
		emit sig_AlbumDownloadFinished(m_strMusicName, "");	//专辑下载完成
		m_albumImage->setPixmap(QPixmap(tr(":/images/albumImage.png")));
		return;
	}

	//解析Json
	QJson::Parser parser;
	bool ok;
	QVariantMap result = parser.parse(albumStr.toUtf8(), &ok).toMap();
	if (!ok)
	{
		qDebug() << "转换成QVariantMap失败";
		emit sig_AlbumDownloadFinished(m_strMusicName, "");	//专辑下载完成
		m_albumImage->setPixmap(QPixmap(tr(":/images/albumImage.png")));
		return;
	}

	//    //判断xml里面是否包含url(url错误返回404页面)
	//    QStringList countList = albumStr.split(", \"code\":");
	//    QString countStr = countList.at(0);
	//    QStringList countList2 = countStr.split(": ");
	//    QString countStr2 = countList2.at(1);

	//    if (countStr2 == "0")
	//    {
	//        qDebug() << "没有找到专辑";
	//        return;
	//    }

	//得到专辑url
	QVariantMap resultMap = result["result"].toMap();
	QString sJpg = resultMap["cover"].toString();		//标准图片
	//QString sJpg2 = resultMap["thumb"].toString();	//迷你图片

#ifdef _WIN32_WINNT
	QString albumImagesName = CZPlayerConfig::getValue("ALBUMDIR_WIN").toString();
#else
	QString albumImagesName = QDir::homePath() + CZPlayerConfig::getValue("ALBUMDIR_X11").toString();
#endif

	albumFilePath = albumImagesName + "/" + m_musicArtist + " - " + m_name + ".jpg";

	this->getAlbumFromURL(sJpg, albumFilePath);
	replay->deleteLater();//最后要释放reply对象
}

//下载专辑图片
void AlbumThread::getAlbumFromURL(const QUrl &url, const QString &filePath)
{
	albumFile.setFileName(filePath);
	albumFile.open(QIODevice::WriteOnly);
	m_strAlbumUrl = url.toString();		//保存专辑url

	albumManager3 = new QNetworkAccessManager;
	QEventLoop loop;
	albumReply = albumManager3->get(QNetworkRequest(QUrl(url)));

	connect(albumReply, SIGNAL(finished()), this, SLOT(slot_ReplayFinished()));
	connect(albumReply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(slot_ReplyError(QNetworkReply::NetworkError)));
	connect(albumReply, SIGNAL(readyRead()), this, SLOT(slot_GetAlbum()));//实现了数据分段下载保存，
	//这样下载完所有数据再保存，要节省很多内存
	loop.exec();
}

//得到专辑文件
void AlbumThread::slot_GetAlbum()
{
	albumFile.write(albumReply->readAll());
}

//下载专辑文件完成
void AlbumThread::slot_ReplayFinished()
{
	albumFile.flush();
	if (albumFile.isOpen())
	{
		albumFile.close();
	}

	emit sig_AlbumDownloadFinished(m_strMusicName, m_strAlbumUrl);	//专辑下载完成

	//m_albumImage->setPixmap(QPixmap(albumFilePath));
	//将图片转换成label大小来显示
	QImage image(albumFilePath);
	QImage newImg = image.scaled(m_albumImage->width(), m_albumImage->height());
	m_albumImage->setPixmap(QPixmap::fromImage(newImg));

	albumReply->deleteLater();//最后要释放reply对象
	this->deleteLater();//释放当前线程
}

//下载错误
void AlbumThread::slot_ReplyError(QNetworkReply::NetworkError networkError)
{
	if (networkError != QNetworkReply::NoError)
	{
		this->startFunc();//重新开始下载
		m_albumImage->setPixmap(QPixmap(tr(":/images/albumImage.png")));
		QMessageBox::information(NULL, tr("信息"), tr("下载专辑错误！"), QMessageBox::Yes);
	}

	//    albumManager3->deleteLater();
	//    albumReply->deleteLater();
	//    if (albumFile.isOpen())
	//    {
	//        albumFile.close();
	//    }
	//    this->deleteLater();
	qDebug() << networkError;
}







