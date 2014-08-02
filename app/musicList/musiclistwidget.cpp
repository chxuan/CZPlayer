#include "stdafx.h"
#include "musiclistwidget.h"


MusicListWidget::MusicListWidget(MusicList *playList, map<QString, int> *mapMusicRows, QWidget *parent)
    : m_playList(playList), m_mapMusicRows(mapMusicRows), m_parent(parent)
{
    //设置窗口基本属性
    this->resize(400, 450);//设置窗体大小
    this->setWindowFlags(Qt::Tool | Qt::FramelessWindowHint);//去掉窗体边框
    this->setAttribute(Qt::WA_TranslucentBackground);//设置背景透明
    this->setWindowIcon(QIcon(":/images/CZPlayer.png"));//设置logo
    this->setWindowTitle(tr("音乐列表"));

    //搜索框
    searchLineEdit = new QLineEdit(this);
    searchLineEdit->setObjectName(tr("searchLineEdit"));
    searchLineEdit->setText(tr("搜索歌手、歌曲"));
    searchLineEdit->installEventFilter(this);
    //searchLineEdit->setPlaceholderText(tr("搜索歌手、歌曲"));

    //搜索按钮
    searchButton = new QPushButton(this);
    searchButton->setObjectName(tr("searchButton"));

    //返回
    backButton = new QPushButton(this);
    backButton->setObjectName(tr("backButton"));
    backButton->setToolTip(tr("返回音乐列表"));
    backButton->setVisible(false);

    //添加歌曲
    addMusicButton = new QPushButton(this);
    addMusicButton->setObjectName(tr("addMusicButton"));
    addMusicButton->setToolTip(tr("添加歌曲"));

    //定位歌曲
    localeButton = new QPushButton(this);
    localeButton->setObjectName(tr("localeButton"));
    localeButton->setToolTip(tr("定位当前播放歌曲"));

    //删除当前歌曲
    delMusicButton = new QPushButton(this);
    delMusicButton->setObjectName(tr("delMusicButton"));
    delMusicButton->setToolTip(tr("删除当前歌曲"));

    //搜索歌曲
    findButton = new QPushButton(this);
    findButton->setObjectName(tr("findButton"));
    findButton->setToolTip(tr("搜索歌曲"));

    //定时器
    timer = new QTimer(this);
    timer->start(1000);

    QSpacerItem *topItem = new QSpacerItem(380, 37, QSizePolicy::Fixed, QSizePolicy::Fixed);
    QSpacerItem *bottomItem = new QSpacerItem(380, 30, QSizePolicy::Fixed, QSizePolicy::Fixed);
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addItem(topItem);
    mainLayout->addWidget(m_playList);
    mainLayout->addItem(bottomItem);
    this->setLayout(mainLayout);

    searchLineEdit->setGeometry(27, 7, 250, 30);
    searchButton->setGeometry(287, 9, 90, 30);
    backButton->setGeometry(257, 15, 15, 14);
    addMusicButton->setGeometry(60, 417, 25, 25);
    localeButton->setGeometry(145, 417, 25, 25);
    delMusicButton->setGeometry(230, 417, 25, 25);
    findButton->setGeometry(315, 417, 25, 25);

    //信号与槽
    connect(addMusicButton, SIGNAL(clicked()), this, SLOT(slot_OnClickedAddMusic()));		//添加歌曲
    connect(localeButton, SIGNAL(clicked()), m_playList, SLOT(slot_LocateCurrentMusic()));	//定位当前播放歌曲
    connect(delMusicButton, SIGNAL(clicked()), m_playList, SLOT(slot_DelCurrentMusic()));	//删除当前歌曲
    connect(searchButton, SIGNAL(clicked()), this, SLOT(slot_SearchMusic()));				//搜索歌曲
    connect(timer, SIGNAL(timeout()), this, SLOT(slot_LineEditFocus()));					//输入框聚焦
    connect(backButton, SIGNAL(clicked()), this, SLOT(slot_BackMusicList()));				//返回音乐列表
}

MusicListWidget::~MusicListWidget()
{
}

//添加歌曲
void MusicListWidget::slot_OnClickedAddMusic()
{
	QMenu menu;
	menu.addAction(QIcon(":/images/addMusic.png"), tr("添加歌曲"), m_parent, SLOT(slot_OpenFile()));			//添加歌曲
	menu.addAction(QIcon(":/images/folder.png"), tr("添加歌曲文件夹"), m_parent, SLOT(slot_AddMusicDir()));		//添加歌曲文件夹
	menu.addAction(QIcon(":/images/addMusicList.png"), tr("载入播放列表"), m_parent, SLOT(slot_AddMusicList()));//添加歌曲
	menu.exec(QCursor::pos());
}

//输入框聚焦
void MusicListWidget::slot_LineEditFocus()
{
    if ("" == searchLineEdit->text() && !searchLineEdit->hasFocus())
    {
        searchLineEdit->setText(tr("搜索歌手、歌曲"));
        backButton->setVisible(false);
    }
}

//返回音乐列表
void MusicListWidget::slot_BackMusicList()
{
    for (int index = 0; index < m_playList->rowCount(); ++index)
    {
        m_playList->showRow(index);
    }
}

//搜索歌曲
void MusicListWidget::slot_SearchMusic()
{
    QString str = searchLineEdit->text();
    if ("搜索歌手、歌曲" == str)
    {
        return;
    }
    map<QString, int>::iterator begin = m_mapMusicRows->begin();
    map<QString, int>::iterator end = m_mapMusicRows->end();
    while (begin != end)
    {
        if (!(begin->first).contains(str))
        {
            m_playList->hideRow(begin->second);
        }
        if ((begin->first).contains(str))
        {
            m_playList->showRow(begin->second);
        }
        ++begin;
    }
}

//重写paintEvent,添加背景图片
void MusicListWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QPixmap backgroundImage;
    backgroundImage.load(":/images/musicList.png");

    //先通过pix的方法获得图片的过滤掉透明的部分得到的图片，作为loginPanel的不规则边框
    this->setMask(backgroundImage.mask());
    painter.drawPixmap(0, 0, 400, 450, backgroundImage);
    event->accept();
}

//事件过滤器
bool MusicListWidget::eventFilter(QObject *target, QEvent *event)
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
            if ("搜索歌手、歌曲" == searchLineEdit->text())
            {
                searchLineEdit->setText(tr(""));
                backButton->setVisible(true);
            }
            return true;
        }
    }

    return QWidget::eventFilter(target, event);
}