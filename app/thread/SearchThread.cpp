#include "StdAfx.h"
#include "SearchThread.h"
#include <parser.h>

SearchThread::SearchThread(QTableWidget *searchList )
	: m_searchList(searchList), m_nMusicNum(0), m_strUrl(""), m_nTimes(0)
{

}

SearchThread::~SearchThread(void)
{
}

void SearchThread::run()
{
}

void SearchThread::startFunc()
{
	m_strUrl = "http://www.xiami.com/app/nineteen/search/key/" + m_strKey + "/page/" + QString::number(m_nStartPage);
	qDebug() << m_strUrl;

	searchManager = new QNetworkAccessManager;
	searchManager ->get(QNetworkRequest(QUrl(m_strUrl)));//得到url
	//信号与槽
	connect(searchManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(slot_LoadFinished(QNetworkReply*)));
}

//加载完成
void SearchThread::slot_LoadFinished( QNetworkReply *replay )
{
	QTextCodec *codec = QTextCodec::codecForName("utf8");		//转换成utf8编码格式
	QString searchStr = codec ->toUnicode(replay ->readAll());
	if (searchStr == "")
	{
		emit sig_SearchTimeout();	//搜索超时
		return;
	}
	searchStr = QUrl::fromPercentEncoding(searchStr.toAscii());	//百分比编码

	//解析Json
	QJson::Parser parser;
	bool ok;
	QVariantMap result = parser.parse(searchStr.toUtf8(), &ok).toMap();
	if (!ok)
	{
		qDebug() << "转换成QVariantMap失败";
		return;
	}

	//搜索到的数量
	m_nMusicNum = result["total"].toInt();
	emit sig_SearchNum(m_nMusicNum);

	//得到结果数组
	QVariantList resultList = result["results"].toList();
	foreach (QVariant var, resultList)
	{
		QVariantMap resultMap = var.toMap();	//得到每一项的map

		//歌曲名
		QCheckBox *musicNameCheckBox = new QCheckBox(resultMap["song_name"].toString());
		musicNameCheckBox ->setObjectName(tr("musicNameCheckBox"));
		musicNameCheckBox ->setToolTip(resultMap["song_name"].toString());
		musicNameCheckBox ->setFont(QFont("微软雅黑", 10));
		musicNameCheckBox ->setStyleSheet("QCheckBox{color:white;}"
			"QCheckBox::indicator{width:10px;height:10px;border: 1px solid white;border-radius:2px}"
			"QCheckBox::indicator:checked {image: url(:/images/checked2.png);}");

		//艺人
		QTableWidgetItem *artistItem = new QTableWidgetItem(resultMap["artist_name"].toString());
		artistItem ->setTextAlignment(Qt::AlignCenter);
		artistItem ->setToolTip(resultMap["artist_name"].toString());
		artistItem ->setFont(QFont("微软雅黑", 10));

		//专辑
		QTableWidgetItem *albumItem = new QTableWidgetItem("《" + resultMap["album_name"].toString() + "》");
		albumItem ->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
		albumItem ->setToolTip("《" + resultMap["album_name"].toString() + "》");
		albumItem ->setFont(QFont("微软雅黑", 10));

		//插入播放列表
		int currentRows = m_searchList ->rowCount();//返回列表中的行数
		m_searchList ->insertRow(currentRows);//从播放列表中的当前行插入
		m_searchList ->setCellWidget(currentRows, 0, musicNameCheckBox);
		m_searchList ->setItem(currentRows, 1, artistItem);
		m_searchList ->setItem(currentRows, 2, albumItem);
	}

	++m_nTimes;
	if (m_nTimes >= 2)
	{
		emit sig_LoadFinished();	//加载完成
		return;
	}
	++m_nStartPage;
	if ((m_nStartPage - 1) * 8 >= m_nMusicNum)//已经加载的数量大于等于总音乐数量则加载完成
	{
		emit sig_LoadFinished();	//加载完成
		return;
	}
	this ->startFunc();				//第二次加载
}

void SearchThread::setParam( QString strKey, int nStartPage )
{
	m_strKey = strKey;
	m_nStartPage = nStartPage;
	m_nMusicNum = 0;
	m_strUrl = "";
	m_nTimes = 0;
}