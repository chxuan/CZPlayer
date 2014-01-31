#include "stdafx.h"
#include "musicdownload.h"

MusicDownload::MusicDownload(QWidget *widget) : parent(widget)
{
    //设置窗口基本属性
    this ->resize(400, 212);//设置窗体大小
    this ->setWindowFlags(Qt::Tool | Qt::FramelessWindowHint);//去掉窗体边框,不在任务栏显示图标
    this ->setAttribute(Qt::WA_TranslucentBackground);//设置背景透明
    this ->setWindowIcon(QIcon(":/images/CZPlayer.png"));//设置logo
    this ->setWindowTitle(tr("音乐下载"));

    //歌曲
    musicEditLabel = new QLabel(this);
    musicEditLabel ->setObjectName(tr("musicEditLabel"));
    musicEditLabel ->setText(tr("歌曲:"));

    //歌曲输入框
    musicEdit = new QLineEdit(this);
    musicEdit ->setObjectName(tr("musicEdit"));
    //musicEdit ->setMaxLength(0);//设置输入最大长度

    //艺术家
    artistEditLabel = new QLabel(this);
    artistEditLabel ->setObjectName(tr("artistEditLabel"));
    artistEditLabel ->setText(tr("歌手:"));

    //艺术家输入框
    artistEdit = new QLineEdit(this);
    artistEdit ->setObjectName(tr("artistEdit"));

    //下载
    downloadButton = new QPushButton(this);
    downloadButton ->setObjectName(tr("downloadButton"));
    downloadButton ->setToolTip(tr("下载歌曲"));

    //下载列表
    downloadList = new DownloadList(this);
    downloadList ->setObjectName(tr("downloadList"));

    //设置组件事件过滤器
    artistEdit ->installEventFilter(this);
    musicEdit ->installEventFilter(this);

    QSpacerItem *topItem = new QSpacerItem(380, 40, QSizePolicy::Fixed, QSizePolicy::Fixed);
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout ->addItem(topItem);
    mainLayout ->addWidget(downloadList);
    this ->setLayout(mainLayout);

    musicEditLabel ->setGeometry(15, 20, 40, 16);
    musicEdit ->setGeometry(55, 12, 120, 31);
    artistEditLabel ->setGeometry(195, 20, 40, 16);
    artistEdit ->setGeometry(236, 12, 80, 31);
    downloadButton ->setGeometry(335, 13, 55, 33);

    connect(downloadButton, SIGNAL(clicked()), this, SLOT(slot_DownloadMusic()));
}

MusicDownload::~MusicDownload()
{
}

//重写paintEvent,添加背景图片
void MusicDownload::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QPixmap backgroundImage;
    backgroundImage.load(":/images/mainBg.png");

    //先通过pix的方法获得图片的过滤掉透明的部分得到的图片，作为loginPanel的不规则边框
    this ->setMask(backgroundImage.mask());
    painter.drawPixmap(0, 0, 400, 212, backgroundImage);
    event ->accept();
}

//下载按钮回调
void MusicDownload::slot_DownloadMusic()
{
    QString musicName = musicEdit ->text();//歌曲名
    QString musicArtist = artistEdit ->text();//歌手

    if (musicName == "" || musicArtist == "")
    {
		QMessageBox::information(NULL, tr("信息"), tr("歌曲名或歌手名不能为空！"), QMessageBox::Yes);
        return;
    }

    //QString musicTitle = musicArtist + "-" + musicName;
    //map<QString, int>::iterator musicIter = musicMap.find(musicTitle);
    //if (musicIter != musicMap.end())//找到了相同歌曲名
    //{
    //    return;
    //}

    //static int times = 0;
    //++times;
    //musicMap.insert(make_pair(musicTitle, times));

    DownloadThread *downloadThread = new DownloadThread(musicName, musicArtist, downloadList);
    downloadThread ->start();//开启一个线程
//    static int index = 0;
//    threadMap.insert(make_pair(index, downloadThread));
//    ++index;

    connect(downloadThread, SIGNAL(sig_DelCurrentMusicMapItem(QString)), this, SLOT(slot_DelCurrentMapItem(QString)));//删除当前音乐mapIter
    connect(downloadList, SIGNAL(sig_DelCurrentMusicMapItem(QString)), this, SLOT(slot_DelCurrentMapItem(QString)));//删除当前音乐mapIter
    //connect(downloadList, SIGNAL(delCurrentRow(int)), this, SLOT(delCurrentThread(int)));//删除当前线程
}

//删除当前音乐mapIter
void MusicDownload::slot_DelCurrentMapItem(QString musicTitle)
{
    map<QString, int>::iterator musicMapIter = musicMap.find(musicTitle);
    if (musicMapIter != musicMap.end())
    {
        musicMap.erase(musicMapIter);
    }
}

////删除当前线程
//void MusicDownload::delCurrentThread(int row)
//{
//    qDebug() << "row = " << row;
//    map<int, DownloadThread*>::iterator threadMapIter = threadMap.find(row);
//    if (threadMapIter != threadMap.end())
//    {
//        //threadMap.erase(threadMapIter);
////        DownloadThread *thread = threadMapIter ->second;
////        thread ->deleteLater();
//        qDebug() << "true";
//    }
//}

//事件过滤器
bool MusicDownload::eventFilter(QObject *target, QEvent *event)
{
    if (target == artistEdit || target == musicEdit)
    {
        if (event ->type() == QEvent::KeyPress)
        {
            QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
            if (keyEvent ->key() == Qt::Key_Return)
            {
                downloadButton ->click();//选中下载按钮
                return true;
            }
        }
    }

    return QWidget::eventFilter(target, event);
}








