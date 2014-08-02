#include "StdAfx.h"
#include "SearchPanel.h"

SearchPanel::SearchPanel( QWidget *widget /*= 0*/ ) 
	: m_parent(widget), m_nMusicNum(0), m_nStartPage(0), m_strSearchKey(""), m_bLoadStared(false)
{
	//设置窗口基本属性
	this->resize(400, 662);//设置窗体大小
	this->setWindowFlags(Qt::Tool | Qt::FramelessWindowHint);//去掉窗体边框,不在任务栏显示图标
	this->setAttribute(Qt::WA_TranslucentBackground);//设置背景透明
	this->setWindowIcon(QIcon(":/images/CZPlayer.png"));//设置logo
	this->setWindowTitle(tr("音乐搜索"));

	//导航条
	navigationBarLabel = new QLabel(this);
	navigationBarLabel->setObjectName(tr("navigationBarLabel"));

	//搜索
	searchMusicButton = new QPushButton(this);
	searchMusicButton->setObjectName(tr("searchMusicButton"));
	searchMusicButton->setStyleSheet("QPushButton:!hover{border-image: url(:/images/searchMusicButton.png);}"
										"QPushButton:hover{border-image: url(:/images/searchMusicButton2.png);}"
										"QPushButton:pressed{border-image: url(:/images/searchMusicButton3.png);}");

	//电台
	broadcastingStationButton = new QPushButton(this);
	broadcastingStationButton->setObjectName(tr("broadcastingStationButton"));
	broadcastingStationButton->setStyleSheet("QPushButton:!hover{border-image: url(:/images/broadcastingStationButton.png);}"
		"QPushButton:hover{border-image: url(:/images/broadcastingStationButton2.png);}"
		"QPushButton:pressed{border-image: url(:/images/broadcastingStationButton3.png);}");

	//榜单
	listButton = new QPushButton(this);
	listButton->setObjectName(tr("listButton"));
	listButton->setStyleSheet("QPushButton:!hover{border-image: url(:/images/listButton.png);}"
		"QPushButton:hover{border-image: url(:/images/listButton2.png);}"
		"QPushButton:pressed{border-image: url(:/images/listButton3.png);}");

	//下载列表
	downLoadListButton = new QPushButton(this);
	downLoadListButton->setObjectName(tr("downLoadListButton"));
	downLoadListButton->setStyleSheet("QPushButton:!hover{border-image: url(:/images/downLoadListButton.png);}"
		"QPushButton:hover{border-image: url(:/images/downLoadListButton2.png);}"
		"QPushButton:pressed{border-image: url(:/images/downLoadListButton3.png);}");

	//收藏列表
	favoritesListButton = new QPushButton(this);
	favoritesListButton->setObjectName(tr("favoritesListButton"));
	favoritesListButton->setStyleSheet("QPushButton:!hover{border-image: url(:/images/favoritesListButton.png);}"
		"QPushButton:hover{border-image: url(:/images/favoritesListButton2.png);}"
		"QPushButton:pressed{border-image: url(:/images/favoritesListButton3.png);}");

	//歌词写真
	lyricsPhotoBarButton = new QPushButton(this);
	lyricsPhotoBarButton->setObjectName(tr("lyricsPhotoBarButton"));
	lyricsPhotoBarButton->setStyleSheet("QPushButton:!hover{border-image: url(:/images/lyricsPhotoBarButton.png);}"
		"QPushButton:hover{border-image: url(:/images/lyricsPhotoBarButton2.png);}"
		"QPushButton:pressed{border-image: url(:/images/lyricsPhotoBarButton3.png);}");

	//搜索框
	searchLineEdit = new QLineEdit(this);
	searchLineEdit->setObjectName(tr("searchLineEdit"));
	searchLineEdit->setText(tr("搜索歌手、歌曲、专辑"));
	searchLineEdit->installEventFilter(this);
	//searchLineEdit->setPlaceholderText(tr("搜索歌手、歌曲、专辑"));

	//搜索按钮
	searchButton = new QPushButton(this);
	searchButton->setObjectName(tr("searchButton"));

	//搜索信息显示
	searchInfoLabel = new QLabel(this);
	searchInfoLabel->setObjectName(tr("searchInfoLabel"));
	searchInfoLabel->setText(tr("CZPlayer，极致音乐"));

	searchInfoLabel2 = new QLabel(this);
	searchInfoLabel2->setObjectName(tr("searchInfoLabel"));

	//全选
	QCheckBox *allChecked = new QCheckBox(tr("全选"), this);
	allChecked->setObjectName(tr("allChecked"));
	allChecked->setFont(QFont("微软雅黑", 10));
	allChecked->setStyleSheet("QCheckBox{color:white;}"
		"QCheckBox::indicator{width:10px;height:10px;border: 1px solid white;border-radius:2px}"
		"QCheckBox::indicator:checked {image: url(:/images/checked2.png);}");

	//下载按钮
	downButton = new QPushButton(this);
	downButton->setObjectName(tr("downButton"));
	downButton->setToolTip(tr("下载"));

	//收藏
	collectButton = new QPushButton(this);
	collectButton->setObjectName(tr("collectButton"));
	collectButton->setToolTip(tr("收藏"));

	//搜索列表
	searchList = new SearchList(this);
	searchList->setObjectName(tr("searchList"));

	//for (int i = 0; i < 10; ++i)
	//{
	//	//歌曲名
	//	//QCheckBox *musicNameCheckBox = new QCheckBox("光辉岁月");
	//	//musicNameCheckBox->setObjectName(tr("musicNameCheckBox"));
	//	//musicNameCheckBox->setFont(QFont("微软雅黑", 10));
	//	//musicNameCheckBox->setStyleSheet("QCheckBox{color:white;}"
	//	//	"QCheckBox::indicator{width:10px;height:10px;border: 1px solid white;border-radius:2px}"
	//	//	"QCheckBox::indicator:checked {image: url(:/images/checked2.png);}");

	//	QTableWidgetItem *musicNameItem = new QTableWidgetItem("光辉岁月");
	//	musicNameItem->setTextAlignment(Qt::AlignCenter);
	//	musicNameItem->setFont(QFont("微软雅黑", 10));
	//	musicNameItem->setCheckState(Qt::Unchecked);

	//	//艺人
	//	QTableWidgetItem *artistItem = new QTableWidgetItem("beyond");
	//	artistItem->setTextAlignment(Qt::AlignCenter);
	//	artistItem->setFont(QFont("微软雅黑", 10));

	//	//专辑
	//	QTableWidgetItem *albumItem = new QTableWidgetItem("《海阔天空》");
	//	albumItem->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
	//	albumItem->setFont(QFont("微软雅黑", 10));

	//	//插入播放列表
	//	int currentRows = searchList->rowCount();//返回列表中的行数
	//	searchList->insertRow(currentRows);//从播放列表中的当前行插入
	//	//searchList->setCellWidget(currentRows, 0, musicNameCheckBox);
	//	searchList->setItem(currentRows, 0, musicNameItem);
	//	searchList->setItem(currentRows, 1, artistItem);
	//	searchList->setItem(currentRows, 2, albumItem);
	//}

	//获取QTreeWidget的滚动条
	scrollBar = searchList->verticalScrollBar();
	scrollBar->setObjectName(tr("scrollBar"));

	QSpacerItem *topItem = new QSpacerItem(380, 160, QSizePolicy::Fixed, QSizePolicy::Fixed);
	QSpacerItem *bottomItem = new QSpacerItem(380, 30, QSizePolicy::Fixed, QSizePolicy::Fixed);
	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addItem(topItem);
	mainLayout->addWidget(searchList);
	mainLayout->addItem(bottomItem);
	this->setLayout(mainLayout);

	//定时器
	timer = new QTimer(this);
	timer->start(1000);

	//搜索线程
	searchThread = new SearchThread(searchList);

	//searchLineEdit->setGeometry(27, 7, 250, 30);
	//searchButton->setGeometry(287, 9, 90, 30);
	//searchInfoLabel->setGeometry(11, 50, 380, 15);
	//searchInfoLabel2->setGeometry(11, 625, 380, 15);
	//allChecked->setGeometry(11, 80, 50, 15);
	//downButton->setGeometry(280, 79, 16, 16);
	//collectButton->setGeometry(320, 79, 16, 16);

	navigationBarLabel->setGeometry(5, 10, 390, 33);
	searchMusicButton->setGeometry(7, 12, 51, 29);
	broadcastingStationButton->setGeometry(60, 12, 57, 29);
	listButton->setGeometry(119, 12, 59, 29);
	downLoadListButton->setGeometry(180, 12, 70, 29);
	favoritesListButton->setGeometry(253, 12, 69, 29);
	lyricsPhotoBarButton->setGeometry(325, 12, 68, 29);

	searchLineEdit->setGeometry(27, 77, 250, 30);
	searchButton->setGeometry(287, 79, 90, 30);
	searchInfoLabel->setGeometry(11, 120, 380, 15);
	searchInfoLabel2->setGeometry(11, 625, 380, 15);
	allChecked->setGeometry(11, 150, 50, 15);
	downButton->setGeometry(280, 149, 16, 16);
	collectButton->setGeometry(320, 149, 16, 16);

	connect(searchButton, SIGNAL(clicked()), this, SLOT(slot_SearchMusic()));					//搜索歌曲
	connect(timer, SIGNAL(timeout()), this, SLOT(slot_LineEditFocus()));						//输入框聚焦
	connect(scrollBar, SIGNAL(valueChanged(int)), this, SLOT(slot_ScrollBarEvent(int)));		//滚动条事件
	connect(allChecked, SIGNAL(stateChanged(int)), this, SLOT(slot_CheckBoxStateChanged(int)));	//复选框状态改变
	connect(searchThread, SIGNAL(sig_LoadFinished()), this, SLOT(slot_LoadFinished()));			//加载完成
	connect(searchThread, SIGNAL(sig_SearchNum(int)), this, SLOT(slot_SearchNum(int)));			//搜索到的相关歌曲数量
	connect(searchThread, SIGNAL(sig_SearchTimeout()), this, SLOT(slot_SearchTimeout()));		//搜索超时
}

SearchPanel::~SearchPanel(void)
{
}

//搜索歌曲
void SearchPanel::slot_SearchMusic()
{
	m_strSearchKey = searchLineEdit->text();	//获取搜索输入框的内容
	if (m_strSearchKey == "" || m_strSearchKey == "搜索歌手、歌曲、专辑")
	{
		QMessageBox::information(NULL, tr("信息"), tr("歌曲名或歌手名不能为空！"), QMessageBox::Yes);
		return;
	}

	//清空搜索列表
	searchList->clear();
	while (searchList->rowCount() > 0)
	{
		searchList->removeRow(0);
	}

	//初始化参数
	m_nStartPage = 1;		//加载开始位置(加载第一页)
	m_nMusicNum = 0;
	m_bLoadStared = false;
	searchInfoLabel->setText(tr("正在搜索，请稍等......"));
	searchInfoLabel2->setText(tr(""));

	//搜索线程
	//SearchThread *searchThread = new SearchThread(m_strSearchKey, m_nStartPage, searchList);
	//connect(searchThread, SIGNAL(sig_LoadFinished()), this, SLOT(slot_LoadFinished()));
	//connect(searchThread, SIGNAL(sig_SearchNum(int)), this, SLOT(slot_SearchNum(int)));
	//searchThread->start();//开启线程
	searchThread->setParam(m_strSearchKey, m_nStartPage);
	searchThread->startFunc();
}

//输入框聚焦
void SearchPanel::slot_LineEditFocus()
{
	if ("" == searchLineEdit->text() && !searchLineEdit->hasFocus())
	{
		searchLineEdit->setText(tr("搜索歌手、歌曲、专辑"));
	}
}

//滚动条事件
void SearchPanel::slot_ScrollBarEvent( int value )
{
	//判断滚动条是否滚动到最下方 ,并且还没有开始加载
	if (scrollBar->maximum() == value && m_bLoadStared == false)
	{
		qDebug() << "滑动到了最底部";
		//判断已经加载的音乐数量是否小于音乐总数
		if ((m_nStartPage - 1) * 8 < m_nMusicNum)
		{
			searchInfoLabel2->setText(tr("正在加载，请稍等......"));

			//搜索线程
			//SearchThread *searchThread = new SearchThread(m_strSearchKey, m_nStartPage, searchList);
			//connect(searchThread, SIGNAL(sig_LoadFinished()), this, SLOT(slot_LoadFinished()));
			//connect(searchThread, SIGNAL(sig_SearchNum(int)), this, SLOT(slot_SearchNum(int)));
			//searchThread->start();	//开启线程
			searchThread->setParam(m_strSearchKey, m_nStartPage);
			searchThread->startFunc();

			m_bLoadStared = true;	//开始加载
		}
	}
}

//搜到的相关歌曲数量
void SearchPanel::slot_SearchNum( int nNum )
{
	searchInfoLabel->setText(tr("搜索“%1”，找到相关歌曲%2首").arg(m_strSearchKey).arg(nNum));
	m_nMusicNum = nNum;		//歌曲数量
}

//加载完成
void SearchPanel::slot_LoadFinished()
{
	m_nStartPage = m_nStartPage + 2;		//加载页数加2
	searchInfoLabel2->setText(tr("已加载歌曲%1首").arg(searchList->rowCount()));
	m_bLoadStared = false;					//没有开始加载

	searchThread->terminate(); 
	//terminate:终止线程
	//QThread *thread = dynamic_cast<QThread *>(sender());
	//if (thread->isRunning()) { thread->terminate(); delete thread; thread = 0; }
}

//搜索超时
void SearchPanel::slot_SearchTimeout()
{
	searchInfoLabel->setText(tr("搜索超时，请检查您的网络或者本机防火墙设置"));
}

//复选框状态改变
void SearchPanel::slot_CheckBoxStateChanged( int state )
{
	if (state == Qt::Checked)			//选中
	{
	}
	else if (state == Qt::Unchecked)	//取消选中
	{
	}
}

//重写paintEvent,添加背景图片
void SearchPanel::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	QPixmap backgroundImage;
	backgroundImage.load(":/images/searchPanel.png");

	//先通过pix的方法获得图片的过滤掉透明的部分得到的图片，作为loginPanel的不规则边框
	this->setMask(backgroundImage.mask());
	painter.drawPixmap(0, 0, 400, 662, backgroundImage);
	event->accept();
}

//事件过滤器
bool SearchPanel::eventFilter(QObject *target, QEvent *event)
{
	if (target == searchLineEdit)
	{
		if (event->type() == QEvent::KeyPress)
		{
			QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
			if (keyEvent->key() == Qt::Key_Return)
			{
				searchButton->click();//选中下载按钮
				return true;
			}
		}

		if (event->type() == QEvent::MouseButtonPress)
		{
			if ("搜索歌手、歌曲、专辑" == searchLineEdit->text())
			{
				searchLineEdit->setText(tr(""));
			}
			return true;
		}
	}

	return QWidget::eventFilter(target, event);
}