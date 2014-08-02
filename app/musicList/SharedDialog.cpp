#include "StdAfx.h"
#include "SharedDialog.h"

SharedDialog::SharedDialog(QWidget *parent) : m_parent(parent)
{
	//设置窗口基本属性
	this->resize(420, 280);//设置窗体大小
	this->setMinimumSize(420, 280);
	this->setMaximumSize(420, 280);
	this->setWindowTitle(tr("分享歌曲"));

	//专辑图片
	albumLabel = new QLabel(this);
	albumLabel->setToolTip(tr("专辑图片"));

	//分享文本输入框
	textEdit = new QTextEdit(this);
	textEdit->setObjectName(tr("textEdit"));

	//分享到
	sharedLabel = new QLabel(this);
	sharedLabel->setObjectName(tr("sharedLabel"));
	sharedLabel->setText(tr("分享到"));

	//新浪微博
	xinlangweiboCheckBox = new QCheckBox(this);
	xinlangweiboCheckBox->setToolTip(tr("分享到新浪微博"));
	xinlangweiboCheckBox->setStyleSheet("QCheckBox{color:black;}"
		"QCheckBox::indicator{width:10px;height:10px;border: 1px solid black;border-radius:2px}"
		"QCheckBox::indicator:checked {image: url(:/images/checked2.png);}");
	xinlangweiboLabel = new QLabel(this);
	xinlangweiboLabel->setToolTip(tr("分享到新浪微博"));
	xinlangweiboLabel->setObjectName(tr("xinlangweiboLabel"));

	//腾讯微博
	tengxunweiboCheckBox = new QCheckBox(this);
	tengxunweiboCheckBox->setToolTip(tr("分享到腾讯微博"));
	tengxunweiboCheckBox->setStyleSheet("QCheckBox{color:black;}"
		"QCheckBox::indicator{width:10px;height:10px;border: 1px solid black;border-radius:2px}"
		"QCheckBox::indicator:checked {image: url(:/images/checked2.png);}");
	tengxunweiboLabel = new QLabel(this);
	tengxunweiboLabel->setToolTip(tr("分享到腾讯微博"));
	tengxunweiboLabel->setObjectName(tr("tengxunweiboLabel"));

	//QQ空间
	qqkongjianCheckBox = new QCheckBox(this);
	qqkongjianCheckBox->setToolTip(tr("分享到QQ空间"));
	qqkongjianCheckBox->setStyleSheet("QCheckBox{color:black;}"
		"QCheckBox::indicator{width:10px;height:10px;border: 1px solid black;border-radius:2px}"
		"QCheckBox::indicator:checked {image: url(:/images/checked2.png);}");
	qqkongjianLabel = new QLabel(this);
	qqkongjianLabel->setToolTip(tr("分享到QQ空间"));
	qqkongjianLabel->setObjectName(tr("qqkongjianLabel"));

	//人人网
	renrenCheckBox = new QCheckBox(this);
	renrenCheckBox->setToolTip(tr("分享到人人网"));
	renrenCheckBox->setStyleSheet("QCheckBox{color:black;}"
		"QCheckBox::indicator{width:10px;height:10px;border: 1px solid black;border-radius:2px}"
		"QCheckBox::indicator:checked {image: url(:/images/checked2.png);}");
	renrenLabel = new QLabel(this);
	renrenLabel->setToolTip(tr("分享到人人网"));
	renrenLabel->setObjectName(tr("renrenLabel"));

	//开心网
	kaixinCheckBox = new QCheckBox(this);
	kaixinCheckBox->setToolTip(tr("分享到开心网"));
	kaixinCheckBox->setStyleSheet("QCheckBox{color:black;}"
		"QCheckBox::indicator{width:10px;height:10px;border: 1px solid black;border-radius:2px}"
		"QCheckBox::indicator:checked {image: url(:/images/checked2.png);}");
	kaixinLabel = new QLabel(this);
	kaixinLabel->setToolTip(tr("分享到开心网"));
	kaixinLabel->setObjectName(tr("kaixinLabel"));

	//豆瓣网
	doubanCheckBox = new QCheckBox(this);
	doubanCheckBox->setToolTip(tr("分享到豆瓣网"));
	doubanCheckBox->setStyleSheet("QCheckBox{color:black;}"
		"QCheckBox::indicator{width:10px;height:10px;border: 1px solid black;border-radius:2px}"
		"QCheckBox::indicator:checked {image: url(:/images/checked2.png);}");
	doubanLabel = new QLabel(this);
	doubanLabel->setToolTip(tr("分享到豆瓣网"));
	doubanLabel->setObjectName(tr("doubanLabel"));

	//搜狐微博
	souhuweiboCheckBox = new QCheckBox(this);
	souhuweiboCheckBox->setToolTip(tr("分享到搜狐微博"));
	souhuweiboCheckBox->setStyleSheet("QCheckBox{color:black;}"
		"QCheckBox::indicator{width:10px;height:10px;border: 1px solid black;border-radius:2px}"
		"QCheckBox::indicator:checked {image: url(:/images/checked2.png);}");
	souhuweiboLabel = new QLabel(this);
	souhuweiboLabel->setToolTip(tr("分享到搜狐微博"));
	souhuweiboLabel->setObjectName(tr("souhuweiboLabel"));

	//QQ好友
	qqhaoyouCheckBox = new QCheckBox(this);
	qqhaoyouCheckBox->setToolTip(tr("分享到QQ好友"));
	qqhaoyouCheckBox->setStyleSheet("QCheckBox{color:black;}"
		"QCheckBox::indicator{width:10px;height:10px;border: 1px solid black;border-radius:2px}"
		"QCheckBox::indicator:checked {image: url(:/images/checked2.png);}");
	qqhaoyouLabel = new QLabel(this);
	qqhaoyouLabel->setToolTip(tr("分享到QQ好友"));
	qqhaoyouLabel->setObjectName(tr("qqhaoyouLabel"));

	//分享按钮
	shareButton = new QPushButton(this);
	shareButton->setText(tr("分享"));

	//取消按钮
	cancelButton = new QPushButton(this);
	cancelButton->setText(tr("取消"));

	albumLabel->setGeometry(25, 20, 110, 103);
	textEdit->setGeometry(150, 20, 250, 103);
	sharedLabel->setGeometry(25, 150, 100, 20);
	xinlangweiboCheckBox->setGeometry(80, 150, 20, 20);
	xinlangweiboLabel->setGeometry(95, 143, 30, 30);
	tengxunweiboCheckBox->setGeometry(145, 150, 20, 20);
	tengxunweiboLabel->setGeometry(160, 143, 30, 30);
	qqkongjianCheckBox->setGeometry(210, 150, 20, 20);
	qqkongjianLabel->setGeometry(225, 143, 30, 30);
	renrenCheckBox->setGeometry(275, 150, 20, 20);
	renrenLabel->setGeometry(290, 143, 30, 30);
	kaixinCheckBox->setGeometry(340, 150, 20, 20);
	kaixinLabel->setGeometry(355, 143, 30, 30);
	doubanCheckBox->setGeometry(80, 200, 20, 20);
	doubanLabel->setGeometry(95, 193, 30, 30);
	souhuweiboCheckBox->setGeometry(145, 200, 20, 20);
	souhuweiboLabel->setGeometry(160, 193, 30, 30);
	qqhaoyouCheckBox->setGeometry(210, 200, 20, 20);
	qqhaoyouLabel->setGeometry(225, 193, 30, 30);
	shareButton->setGeometry(240, 240, 80, 25);
	cancelButton->setGeometry(330, 240, 80, 25);

	//复选框状态改变
	connect(xinlangweiboCheckBox, SIGNAL(stateChanged(int)), this, SLOT(slot_CheckBoxStateChanged(int)));
	connect(tengxunweiboCheckBox, SIGNAL(stateChanged(int)), this, SLOT(slot_CheckBoxStateChanged(int)));
	connect(qqkongjianCheckBox, SIGNAL(stateChanged(int)), this, SLOT(slot_CheckBoxStateChanged(int)));
	connect(renrenCheckBox, SIGNAL(stateChanged(int)), this, SLOT(slot_CheckBoxStateChanged(int)));
	connect(kaixinCheckBox, SIGNAL(stateChanged(int)), this, SLOT(slot_CheckBoxStateChanged(int)));
	connect(doubanCheckBox, SIGNAL(stateChanged(int)), this, SLOT(slot_CheckBoxStateChanged(int)));
	connect(souhuweiboCheckBox, SIGNAL(stateChanged(int)), this, SLOT(slot_CheckBoxStateChanged(int)));
	connect(qqhaoyouCheckBox, SIGNAL(stateChanged(int)), this, SLOT(slot_CheckBoxStateChanged(int)));

	connect(shareButton, SIGNAL(clicked()), this, SLOT(slot_SharedMusic()));
	connect(cancelButton, SIGNAL(clicked()), this, SLOT(hide()));
}

SharedDialog::~SharedDialog(void)
{
}

//初始化分享对话框
void SharedDialog::initSharedDialog( QString albumImagePath, QString strMusicName, QString strAlbumUrlPath )
{
	if (albumImagePath == ":/images/albumImage.png")
	{
		albumLabel->setPixmap(QPixmap(albumImagePath));
	}
	else
	{
		//将图片转换成label大小来显示
		QImage image(albumImagePath);
		QImage newImg = image.scaled(albumLabel->width(), albumLabel->height());
		albumLabel->setPixmap(QPixmap::fromImage(newImg));
	}

	QString strText = "在CZPlayer上听到的这首《" + strMusicName + "》好听，快来听听看。";
	textEdit->setText(strText);
	m_strMusicName = strMusicName;
	m_strAlbumUrlPath = strAlbumUrlPath;

	xinlangweiboCheckBox->setCheckState(Qt::Checked);
	tengxunweiboCheckBox->setCheckState(Qt::Unchecked);
	qqkongjianCheckBox->setCheckState(Qt::Unchecked);
	renrenCheckBox->setCheckState(Qt::Unchecked);
	kaixinCheckBox->setCheckState(Qt::Unchecked);
	doubanCheckBox->setCheckState(Qt::Unchecked);
	souhuweiboCheckBox->setCheckState(Qt::Unchecked);
	qqhaoyouCheckBox->setCheckState(Qt::Unchecked);
}

//复选框状态改变
void SharedDialog::slot_CheckBoxStateChanged( int state )
{
	if (state == Qt::Checked)			//选中
	{
		QCheckBox *checkBox = dynamic_cast<QCheckBox*>(sender());
		if (checkBox == xinlangweiboCheckBox)
		{
			tengxunweiboCheckBox->setCheckState(Qt::Unchecked);
			qqkongjianCheckBox->setCheckState(Qt::Unchecked);
			renrenCheckBox->setCheckState(Qt::Unchecked);
			kaixinCheckBox->setCheckState(Qt::Unchecked);
			doubanCheckBox->setCheckState(Qt::Unchecked);
			souhuweiboCheckBox->setCheckState(Qt::Unchecked);
			qqhaoyouCheckBox->setCheckState(Qt::Unchecked);
		}
		else if (checkBox == tengxunweiboCheckBox)
		{
			xinlangweiboCheckBox->setCheckState(Qt::Unchecked);
			qqkongjianCheckBox->setCheckState(Qt::Unchecked);
			renrenCheckBox->setCheckState(Qt::Unchecked);
			kaixinCheckBox->setCheckState(Qt::Unchecked);
			doubanCheckBox->setCheckState(Qt::Unchecked);
			souhuweiboCheckBox->setCheckState(Qt::Unchecked);
			qqhaoyouCheckBox->setCheckState(Qt::Unchecked);
		}
		else if (checkBox == qqkongjianCheckBox)
		{
			xinlangweiboCheckBox->setCheckState(Qt::Unchecked);
			tengxunweiboCheckBox->setCheckState(Qt::Unchecked);
			renrenCheckBox->setCheckState(Qt::Unchecked);
			kaixinCheckBox->setCheckState(Qt::Unchecked);
			doubanCheckBox->setCheckState(Qt::Unchecked);
			souhuweiboCheckBox->setCheckState(Qt::Unchecked);
			qqhaoyouCheckBox->setCheckState(Qt::Unchecked);
		}
		else if (checkBox == renrenCheckBox)
		{
			xinlangweiboCheckBox->setCheckState(Qt::Unchecked);
			tengxunweiboCheckBox->setCheckState(Qt::Unchecked);
			qqkongjianCheckBox->setCheckState(Qt::Unchecked);
			kaixinCheckBox->setCheckState(Qt::Unchecked);
			doubanCheckBox->setCheckState(Qt::Unchecked);
			souhuweiboCheckBox->setCheckState(Qt::Unchecked);
			qqhaoyouCheckBox->setCheckState(Qt::Unchecked);
		}
		else if (checkBox == kaixinCheckBox)
		{
			xinlangweiboCheckBox->setCheckState(Qt::Unchecked);
			tengxunweiboCheckBox->setCheckState(Qt::Unchecked);
			qqkongjianCheckBox->setCheckState(Qt::Unchecked);
			renrenCheckBox->setCheckState(Qt::Unchecked);
			doubanCheckBox->setCheckState(Qt::Unchecked);
			souhuweiboCheckBox->setCheckState(Qt::Unchecked);
			qqhaoyouCheckBox->setCheckState(Qt::Unchecked);
		}
		else if (checkBox == doubanCheckBox)
		{
			xinlangweiboCheckBox->setCheckState(Qt::Unchecked);
			tengxunweiboCheckBox->setCheckState(Qt::Unchecked);
			qqkongjianCheckBox->setCheckState(Qt::Unchecked);
			renrenCheckBox->setCheckState(Qt::Unchecked);
			kaixinCheckBox->setCheckState(Qt::Unchecked);
			souhuweiboCheckBox->setCheckState(Qt::Unchecked);
			qqhaoyouCheckBox->setCheckState(Qt::Unchecked);
		}
		else if (checkBox == souhuweiboCheckBox)
		{
			xinlangweiboCheckBox->setCheckState(Qt::Unchecked);
			tengxunweiboCheckBox->setCheckState(Qt::Unchecked);
			qqkongjianCheckBox->setCheckState(Qt::Unchecked);
			renrenCheckBox->setCheckState(Qt::Unchecked);
			kaixinCheckBox->setCheckState(Qt::Unchecked);
			doubanCheckBox->setCheckState(Qt::Unchecked);
			qqhaoyouCheckBox->setCheckState(Qt::Unchecked);
		}
		else if (checkBox == qqhaoyouCheckBox)
		{
			xinlangweiboCheckBox->setCheckState(Qt::Unchecked);
			tengxunweiboCheckBox->setCheckState(Qt::Unchecked);
			qqkongjianCheckBox->setCheckState(Qt::Unchecked);
			renrenCheckBox->setCheckState(Qt::Unchecked);
			kaixinCheckBox->setCheckState(Qt::Unchecked);
			doubanCheckBox->setCheckState(Qt::Unchecked);
			souhuweiboCheckBox->setCheckState(Qt::Unchecked);
		}
	}
}

//分享歌曲
void SharedDialog::slot_SharedMusic()
{
	if (textEdit->toPlainText() == "")
	{
		QMessageBox::information(NULL, tr("信息"), tr("分享的内容不能为空！"), QMessageBox::Yes);
		return;
	}
	if (xinlangweiboCheckBox->checkState() == Qt::Unchecked
		&& tengxunweiboCheckBox->checkState() == Qt::Unchecked
		&& qqkongjianCheckBox->checkState() == Qt::Unchecked
		&& renrenCheckBox->checkState() == Qt::Unchecked
		&& kaixinCheckBox->checkState() == Qt::Unchecked
		&& doubanCheckBox->checkState() == Qt::Unchecked
		&& souhuweiboCheckBox->checkState() == Qt::Unchecked
		&& qqhaoyouCheckBox->checkState() == Qt::Unchecked)
	{
		QMessageBox::information(NULL, tr("信息"), tr("请勾选要分享到的网站！"), QMessageBox::Yes);
		return;
	}

	QString strMyIndex = "http://www.qtcn.org/bbs/read-htm-tid-55824.html";

	if (xinlangweiboCheckBox->checkState() == Qt::Checked)
	{
		qDebug() << "分享到新浪微博";

		QString qStrUrl = "http://service.weibo.com/share/share.php?url=" 
			+ strMyIndex + "&title=" + textEdit->toPlainText() + "&pic=" + m_strAlbumUrlPath;
		this->openBrowser(qStrUrl);
	}
	else if (tengxunweiboCheckBox->checkState() == Qt::Checked)
	{
		qDebug() << "分享到腾讯微博";

		QString qStrUrl = "http://share.v.t.qq.com/index.php?c=share&a=index&url=" 
			+ strMyIndex + "&title=" + textEdit->toPlainText() + "&pic=" + m_strAlbumUrlPath;
		this->openBrowser(qStrUrl);
	}
	else if (qqkongjianCheckBox->checkState() == Qt::Checked)
	{
		qDebug() << "分享到QQ空间";

		QString qStrUrl = "http://sns.qzone.qq.com/cgi-bin/qzshare/cgi_qzshare_onekey?url=" 
			+ strMyIndex + "&title=分享自CZPlayer&desc=" + textEdit->toPlainText() + "&pics=" + m_strAlbumUrlPath;
		this->openBrowser(qStrUrl);
	}
	else if (renrenCheckBox->checkState() == Qt::Checked)
	{
		qDebug() << "分享到人人网";

		QString qStrUrl = "http://widget.renren.com/dialog/share?resourceUrl=" 
			+ strMyIndex + "&description=" + textEdit->toPlainText() + "&pic=" + m_strAlbumUrlPath;
		this->openBrowser(qStrUrl);
	}
	else if (kaixinCheckBox->checkState() == Qt::Checked)
	{
		qDebug() << "分享到开心网";

		QString qStrUrl = "http://www.kaixin001.com/rest/records.php?content="
			+ textEdit->toPlainText() + "&url=" + strMyIndex + "&starid=&aid=100018706&style=11&pic=" + m_strAlbumUrlPath;
		this->openBrowser(qStrUrl);
	}
	else if (doubanCheckBox->checkState() == Qt::Checked)
	{
		qDebug() << "分享到豆瓣网";

		QString qStrUrl = "http://www.douban.com/share/service?href=" + m_strAlbumUrlPath + "&name="
			+ textEdit->toPlainText() + "&text=" + strMyIndex;
		this->openBrowser(qStrUrl);
	}
	else if (souhuweiboCheckBox->checkState() == Qt::Checked)
	{
		qDebug() << "分享到搜狐微博";

		QString qStrUrl = "http://t.sohu.com/third/post.jsp?url=" + strMyIndex + "&title=" + textEdit->toPlainText();
		this->openBrowser(qStrUrl);
	}
	else if (qqhaoyouCheckBox->checkState() == Qt::Checked)
	{
		qDebug() << "分享到QQ好友";

		QString qStrUrl = "http://connect.qq.com/widget/shareqq/index.html?url=" 
			+ strMyIndex + "&desc=" + textEdit->toPlainText() + "&title=" 
			+ m_strMusicName + "&summary=听音乐，找CZPlayer&pics=" + m_strAlbumUrlPath;
		this->openBrowser(qStrUrl);
	}
}

//ascii字符转换成Unicode字符
int SharedDialog::astr_ustr( char *ansistr, WCHAR *unicodestr )
{
	int result = 0;
	try
	{
		int needlen = MultiByteToWideChar(CP_ACP, 0, ansistr, -1, NULL, 0 );
		if( needlen < 0 )
		{
			return needlen;
		}

		result = MultiByteToWideChar(CP_ACP, 0, ansistr, -1, unicodestr, needlen);
		if( result < 0 )
		{
			return result;
		}
		return result;
	}
	catch( ... )
	{

	}
	return result;
}

//打开浏览器
void SharedDialog::openBrowser( QString &qStrUrl )
{
	WCHAR wcUrl[4096];
	this->astr_ustr(const_cast<char*>(qStrUrl.toStdString().c_str()), wcUrl);
	ShellExecute(NULL, _T("open"), wcUrl, NULL, NULL, SW_SHOWNORMAL);
}