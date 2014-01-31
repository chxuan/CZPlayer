#include "stdafx.h"
#include <QtXml>
#include "lrcthread.h"
#include "static.h"
#include "czplayerconfig.h"

LrcThread::LrcThread( QString musicName, QString musicArtist, QString lrcFileName, QLabel *lrc ) 
	: m_musicName(musicName), m_musicArtist(musicArtist), m_lrcFilePath(lrcFileName), m_lrc(lrc)
{
	this ->startFunc();
}

LrcThread::~LrcThread()
{
}

void LrcThread::run()
{
}

//开始下载
void LrcThread::startFunc()
{
	QString urlStr = "http://box.zhangmen.baidu.com/x?op=12&count=1&title=" + m_musicName + "$$" + m_musicArtist + "$$$";
	lrcManager = new QNetworkAccessManager;
	lrcManager ->get(QNetworkRequest(QUrl(urlStr)));//得到url
	//信号与槽
	connect(lrcManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(slot_GetMusicXML(QNetworkReply*)));
}

//得到音乐（xml格式）
void LrcThread::slot_GetMusicXML(QNetworkReply *replay)
{
	QTextCodec *codec = QTextCodec::codecForName("utf8");//转换成utf8编码格式
	QString musicStr = codec ->toUnicode(replay ->readAll());

	//没有连接到网络
	if (musicStr == "")
	{
		m_lrc ->setText(m_musicArtist + " - " + m_musicName + tr("----无法连接到网络!"));
		return;
	}

	QString errorStr;
	int errorLine;
	int errorColumn;

	QDomDocument doc;
	if (!doc.setContent(musicStr, false, &errorStr, &errorLine, &errorColumn))
	{
		qDebug() << "在第" << errorLine << "行，第" << errorColumn << "列，读取字符串到doc中错误：" << errorStr;
		m_lrc ->setText(m_musicArtist + " - " + m_musicName + tr("----未找到歌词文件!"));
		return;
	}

	QDomElement root = doc.documentElement();//获取根元素
	if (root.tagName() != "result")
	{
		m_lrc ->setText(m_musicArtist + " - " + m_musicName + tr("----未找到歌词文件!"));
		return;
	}

	//获取lrcid
	QString sLrcID;
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
						m_lrc ->setText(m_musicArtist + " - " + m_musicName + tr("----未找到歌词文件!"));
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
								if (element2.tagName() == "lrcid")
								{
									sLrcID = element2.text();
									break;
								}
							}
							else
							{
								m_lrc ->setText(m_musicArtist + " - " + m_musicName + tr("----未找到歌词文件!"));
								return;
							}
						}
						else
						{
							m_lrc ->setText(m_musicArtist + " - " + m_musicName + tr("----未找到歌词文件!"));
							return;
						}
					}
					break;
				}
			}
			else
			{
				m_lrc ->setText(m_musicArtist + " - " + m_musicName + tr("----未找到歌词文件!"));
				return;
			}
		}
		else
		{
			m_lrc ->setText(m_musicArtist + " - " + m_musicName + tr("----未找到歌词文件!"));
			return;
		}
	}

	//得到歌词所在文件夹
	int nFolder = sLrcID.toInt() / 100;
	QString sFolder = QString::number(nFolder);
	//歌词url
	QString lrcUrl = "http://box.zhangmen.baidu.com/bdlrc/" + sFolder + "/" + sLrcID + ".lrc";

	this ->getLrcFromURL(lrcUrl);
	replay ->deleteLater();//最后要释放reply对象
}

//下载歌词
void LrcThread::getLrcFromURL(const QUrl &url)
{
	lrcFile.setFileName(m_lrcFilePath);
	lrcFile.open(QIODevice::WriteOnly);

	lrcManager2 = new QNetworkAccessManager;
	lrcManager2 ->get(QNetworkRequest(QUrl(url)));

	connect(lrcManager2, SIGNAL(finished(QNetworkReply*)), this, SLOT(slot_GetLrc(QNetworkReply*)));
}

//得到歌词文件
void LrcThread::slot_GetLrc(QNetworkReply *replay)
{
	lrcFile.write(replay ->readAll());
	lrcFile.flush();
	if (lrcFile.isOpen())
	{
		lrcFile.close();
	}

	emit sig_ResolveLrc();		//解析当前下载歌词
	replay ->deleteLater();		//最后要释放reply对象
	this ->deleteLater();		//释放当前线程
}







