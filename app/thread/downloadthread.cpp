#include "stdafx.h"
#include <QtXML>
#include "downloadthread.h"
#include "static.h"
#include "czplayerconfig.h"

DownloadThread::DownloadThread(QString musicName, QString musicArtist, DownloadList *downloadList)
    : m_musicName(musicName), m_musicArtist(musicArtist), m_downloadList(downloadList)
{
    times = 0;
    currentTime = 0;

    //设置一个计时器
    timer = new QTimer;
    timer2 = new QTimer;//判断网络是否连接
    time = new QTime;
    connect(timer, SIGNAL(timeout()), this, SLOT(slot_GetNetSpeed()));
    connect(timer2, SIGNAL(timeout()), this, SLOT(slot_NetWorkInterrupt()));

    QString urlStr = "http://box.zhangmen.baidu.com/x?op=12&count=1&title=" + m_musicName + "$$" + m_musicArtist + "$$$";
    manager = new QNetworkAccessManager;
    manager->get(QNetworkRequest(QUrl(urlStr)));//得到url
    //信号与槽
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(slot_GetMusicXML(QNetworkReply*)));
    connect(downloadList, SIGNAL(sig_DelCurrentRow(int)), this, SLOT(slot_DelCurrentThread(int)));//删除当前行
    connect(downloadList, SIGNAL(sig_PauseDownload(int)), this, SLOT(slot_PauseMusicDownload(int)));//暂停下载
}

DownloadThread::~DownloadThread()
{
}

void DownloadThread::run()
{
}

//得到音乐（xml格式）
void DownloadThread::slot_GetMusicXML(QNetworkReply *replay)
{
    QTextCodec *codec = QTextCodec::codecForName("utf8");//转换成utf8编码格式
    QString musicStr = codec->toUnicode(replay->readAll());

    //没有连接到网络
    if (musicStr == "")
    {
        QMessageBox::information(NULL, tr("信息"), tr("下载超时，请检查您的网络或者本机防火墙设置！"), QMessageBox::Yes);
        QString musicTitle = m_musicArtist + "-" + m_musicName;
        emit sig_DelCurrentMusicMapItem(musicTitle);
        return;
    }

	QString errorStr;
	int errorLine;
	int errorColumn;

	QDomDocument doc;
	if (!doc.setContent(musicStr, false, &errorStr, &errorLine, &errorColumn))
	{
		qDebug() << "在第" << errorLine << "行，第" << errorColumn << "列，读取字符串到doc中错误：" << errorStr;
        QMessageBox::information(NULL, tr("信息"), tr("没有找到该歌曲！"), QMessageBox::Yes);
		return;
	}

	QDomElement root = doc.documentElement();//获取根元素
	if (root.tagName() != "result")
	{
		QMessageBox::information(NULL, tr("信息"), tr("没有找到该歌曲！"), QMessageBox::Yes);
		return;
	}

	//获取音乐url
	QString sFirstPartUrl;	//前半部分
	QString sLastPartUrl;	//后半部分
	QDomNodeList nodeList = root.childNodes();//获得root所有子节点的列表
	for (int i = 0; i < nodeList.count(); ++i)
	{
		QDomNode node = nodeList.at(i);
		if (!node.isNull())//如果节点不为空
		{
			if (node.isElement())//如果节点是元素
			{
				QDomElement element = node.toElement();//转换成元素
				if (element.tagName() == "count")
				{
					qDebug() << "element.text() = " << element.text();
					if (element.text() == "0")//返回元素文本
					{
						QMessageBox::information(NULL, tr("信息"), tr("没有找到该歌曲！"), QMessageBox::Yes);
						return;
					}
				}
				else if (element.tagName() == "url")
				{
					QDomNodeList nodeList2 = element.childNodes();//获得url所有子节点的列表
					for (int i = 0; i < nodeList2.count(); ++i)
					{
						QDomNode node2 = nodeList2.at(i);
						if (!node2.isNull())//如果节点不为空
						{
							if (node2.isElement())//如果节点是元素
							{
								QDomElement element2 = node2.toElement();//转换成元素
								if (element2.tagName() == "encode")
								{
									//qDebug() << element2.text();
									sFirstPartUrl = element2.text();
								}
								else if (element2.tagName() == "decode")
								{
									//qDebug() << element2.text();
									sLastPartUrl = element2.text();
									break;
								}
							}
							else
							{
								QMessageBox::information(NULL, tr("信息"), tr("没有找到该歌曲！"), QMessageBox::Yes);
								return;
							}
						}
						else
						{
							QMessageBox::information(NULL, tr("信息"), tr("没有找到该歌曲！"), QMessageBox::Yes);
							return;
						}
					}
					break;
				}
			}
			else
			{
				QMessageBox::information(NULL, tr("信息"), tr("没有找到该歌曲！"), QMessageBox::Yes);
				return;
			}
		}
		else
		{
			QMessageBox::information(NULL, tr("信息"), tr("没有找到该歌曲！"), QMessageBox::Yes);
			return;
		}
	}

	//判断是否包含xcode，防止歌曲下载错误！
	if (!sLastPartUrl.contains("xcode"))
	{
		QMessageBox::information(NULL, tr("信息"), tr("没有找到该歌曲！"), QMessageBox::Yes);
		return;
	}

    //音乐url
    QString musicUrl = sFirstPartUrl + sLastPartUrl;
	//qDebug() << musicUrl;
#ifdef _WIN32_WINNT
    QString WINPATH = CZPlayerConfig::getValue("MUSICDIR_WIN").toString();
    QString musicFilePath = WINPATH + "/" + m_musicArtist + " - " + m_musicName + ".mp3";
#else
    QString X11PATH = QDir::homePath() + CZPlayerConfig::getValue("MUSICDIR_X11").toString();
    QString musicFilePath = X11PATH + "/" + m_musicArtist + " - " + m_musicName + ".mp3";
#endif

    //插入下载列表
    int currentRows = m_downloadList->rowCount();//返回列表中的行数
    m_downloadList->insertRow(currentRows);//从下载列表中的当前行插入

    //进度条
    progressBar = new QProgressBar;
    progressBar->setObjectName(tr("progressBar"));
    timer->start(1000);
    time->start();

    //歌曲名称
    musicTitleItem = new QTableWidgetItem;
    musicTitleItem->setText(m_musicArtist + "-" + m_musicName);
    musicTitleItem->setTextAlignment(Qt::AlignCenter);
    musicTitleItem->setToolTip(m_musicArtist + "-" + m_musicName);

    //歌曲状态
    musicStatusItem = new QTableWidgetItem;
    musicStatusItem->setTextAlignment(Qt::AlignCenter);
    musicStatusItem->setText(tr("正在下载"));

    //大小
    musicSizeItem = new QTableWidgetItem;
    musicSizeItem->setTextAlignment(Qt::AlignCenter);

    //网速
    speedItem = new QTableWidgetItem;
    speedItem->setTextAlignment(Qt::AlignCenter);

    m_downloadList->setItem(currentRows, 0, musicTitleItem);
    m_downloadList->setItem(currentRows, 1, musicStatusItem);
    m_downloadList->setItem(currentRows, 2, musicSizeItem);
    m_downloadList->setItem(currentRows, 3, speedItem);
    m_downloadList->setCellWidget(currentRows, 4, progressBar);

    static int index = 0;
    threadMap.insert(make_pair(index, this));
    listMap.insert(make_pair(index, currentRows));
    //emit musicListMap(listMap);
    m_downloadList->setMusicListMap(listMap);
    ++index;

    this->getMusicFromURL(musicUrl, musicFilePath);
    replay->deleteLater();//最后要释放reply对象
}

//下载歌曲
void DownloadThread::getMusicFromURL(const QUrl &url, const QString &filePath)
{
    musicFile.setFileName(filePath);
    musicFile.open(QIODevice::WriteOnly);

    musicManager = new QNetworkAccessManager;
    QEventLoop loop;
    musicReply = musicManager->get(QNetworkRequest(QUrl(url)));

    connect(musicReply, SIGNAL(downloadProgress(qint64, qint64)), this, SLOT(slot_UpdateDataProgress(qint64, qint64)));
    connect(musicReply, SIGNAL(finished()), this, SLOT(slot_ReplayFinished()));
    connect(musicReply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(slot_ReplyError(QNetworkReply::NetworkError)));
    connect(musicReply, SIGNAL(readyRead()), this, SLOT(slot_GetMusic()));//实现了数据分段下载保存，
                                                                     //这样下载完所有数据再保存，要节省很多内存
    loop.exec();
}

//得到歌曲文件
void DownloadThread::slot_GetMusic()
{
    //得到歌曲文件的大小
    ++times;
    if (times == 1)
    {
        QVariant var = musicReply->header(QNetworkRequest::ContentLengthHeader);
        int size = var.toLongLong();
        double musicSizeD = (double) size / 1048576;
        QString musicSizeStr = QString::number(musicSizeD, 'f', 2) + "MB";
        musicSizeItem->setText(musicSizeStr);
    }

    musicFile.write(musicReply->readAll());

    if (timer2->isActive())
    {
        timer2->stop();
    }
    timer2->start(200000);//如果20秒都好没有下载文件则网络中断
}

//下载歌曲完成
void DownloadThread::slot_ReplayFinished()
{
    musicFile.flush();
    if (musicFile.isOpen())
    {
        musicFile.close();
    }
    musicReply->deleteLater();//最后要释放reply对象
    this->deleteLater();//释放当前线程
}

//每隔1秒更新一下网速
void DownloadThread::slot_GetNetSpeed()
{
    QString speedStr = QString::number(speedD, 'f', 1) + "KB/S";
    speedItem->setText(speedStr);
}

//更新进度条
void DownloadThread::slot_UpdateDataProgress(qint64 bytesRead, qint64 totalBytes)
{
    progressBar->setMaximum(totalBytes);//最大值
    progressBar->setValue(bytesRead);//当前值

//    ++times;
//    if (times == 1)
//    {
//        double musicSizeD = (double) totalBytes / 1048576;
//        QString musicSizeStr = QString::number(musicSizeD, 'f', 2) + "MB";
//        musicSizeItem->setText(musicSizeStr);
//    }

    //得到当前时间
    currentTime = time->elapsed();
    //计算网速
    speedD = bytesRead / currentTime * 1000 / 1024;

    if (bytesRead == totalBytes)
    {
        timer->stop();
        speedItem->setText(tr("0.0KB/S"));
        musicStatusItem->setText(tr("完成"));
    }
}

//删除正在下载的歌曲
void DownloadThread::slot_DelCurrentThread(int row)
{
    map<int, DownloadThread*>::iterator threadMapIter = threadMap.find(row);
    if (threadMapIter != threadMap.end())
    {
        DownloadThread *currentThread = threadMapIter->second;
        if (currentThread == this)
        {
            threadMap.erase(threadMapIter);
            musicManager->deleteLater();
            musicReply->deleteLater();
            //m_downloadList->removeRow(row);
            //m_downloadList->setMusicListMap(listMap);
            if (musicFile.isOpen())
            {
                musicFile.close();
            }
            this->deleteLater();

            map<int, int>::iterator listMapIter = listMap.find(row);
            if (listMapIter != listMap.end())
            {
                qDebug() << "row = " << row;
                listMap.erase(listMapIter);
                m_downloadList->setMusicListMap(listMap);
            }
        }
    }
}

//暂停下载
void DownloadThread::slot_PauseMusicDownload( int row )
{
    map<int, DownloadThread*>::iterator threadMapIter = threadMap.find(row);
    if (threadMapIter != threadMap.end())
    {
        DownloadThread *currentThread = threadMapIter->second;
        if (currentThread == this)
        {
            if (musicReply->isReadable())
            {
                //musicReply->deleteLater();
                musicReply->abort();
                timer->stop();
                timer2->stop();
                speedItem->setText(tr("0.0KB/S"));
                musicStatusItem->setText(tr("暂停"));
            }
        }
    }
}

//下载错误
void DownloadThread::slot_ReplyError(QNetworkReply::NetworkError networkError)
{
    QMessageBox::information(NULL, tr("信息"), tr("下载错误，错误代码：%1").arg(networkError), QMessageBox::Yes);
	qApp->exit(-1);
	//if (musicFile.isOpen())
	//{
	//	musicFile.close();
	//}
 //   musicManager->deleteLater();
 //   musicReply->deleteLater();
 //   this->deleteLater();
}

//网络中断
void DownloadThread::slot_NetWorkInterrupt()
{
    static int i = 0;
    ++i;
    if (i == 1)
    {
        timer->stop();//停止更新网速
        timer2->stop();
        speedItem->setText(tr("0.0KB/S"));
        musicStatusItem->setText(tr("中断"));
        QMessageBox::information(NULL, tr("信息"), tr("请检查网络是否连接！"), QMessageBox::Yes);
    }
}




