#include "stdafx.h"
#include "configdialog.h"
#include "static.h"
#include "czplayerconfig.h"
#include "musiclrc.h"
#include "globalhotkey.h"

QListWidget* ConfigDialog::contentsWidget = 0;

ConfigDialog::ConfigDialog(QWidget *parent) : m_parent(parent)
{
    //设置窗口基本属性
    this ->resize(665, 482);//设置窗体大小
    this ->setMinimumSize(665, 482);
    this ->setWindowTitle(tr("CZPlayer 设置"));

    //设置选项
    contentsWidget = new QListWidget;
    contentsWidget ->setViewMode(QListView::ListMode);//IconMode
    contentsWidget ->setIconSize(QSize(96, 84));
    contentsWidget ->setMovement(QListView::Static);
    contentsWidget ->setMaximumWidth(128);
    contentsWidget ->setSpacing(12);
    this ->createIcons();

    generalSettingsPage = new GeneralSettingsPage(this);
    downloadSettingsPage = new DownloadSettingsPage(this);
    lrcSettingsPage = new LrcSettingsPage(this);
    hotKeysSettingsPage = new HotKeysSettingsPage(this);
    proxySettingsPage = new ProxySettingsPage(this);

    //多页显示
    pagesWidget = new QStackedWidget;
    pagesWidget ->addWidget(generalSettingsPage);//基本设置
    pagesWidget ->addWidget(downloadSettingsPage);//下载设置
    pagesWidget ->addWidget(lrcSettingsPage);//歌词设置
    pagesWidget ->addWidget(hotKeysSettingsPage);//热键设置
    pagesWidget ->addWidget(proxySettingsPage);//代理设置
    contentsWidget ->setCurrentRow(0);

    //确定按钮
    okButton = new QPushButton(this);
    okButton ->setObjectName(tr("okButton"));
    okButton ->setText(tr("确定"));

    //取消按钮
    cancelButton = new QPushButton(this);
    cancelButton ->setObjectName(tr("cancelButton"));
    cancelButton ->setText(tr("取消"));

    //应用按钮
    appButton = new QPushButton(this);
    appButton ->setObjectName(tr("appButton"));
    appButton ->setText(tr("应用"));

    QHBoxLayout *horizontalLayout = new QHBoxLayout;
    horizontalLayout ->addWidget(contentsWidget);
    horizontalLayout ->addWidget(pagesWidget);

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout ->addStretch();
    buttonsLayout ->addWidget(okButton);
    buttonsLayout ->addWidget(cancelButton);
    buttonsLayout ->addWidget(appButton);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout ->addLayout(horizontalLayout);
    mainLayout ->addStretch();
    mainLayout ->addLayout(buttonsLayout);
    this ->setLayout(mainLayout);

    connect(cancelButton, SIGNAL(clicked()), this, SLOT(hide()));
    connect(okButton, SIGNAL(clicked()), this, SLOT(slot_OkFunc()));
    connect(appButton, SIGNAL(clicked()), this, SLOT(slot_AppFunc()));
}

ConfigDialog::~ConfigDialog()
{
}

//创建图标
void ConfigDialog::createIcons()
{
    //常规设置
    QListWidgetItem *generalSettingsItem = new QListWidgetItem(contentsWidget);
    generalSettingsItem ->setIcon(QIcon(":/images/generalSettingsButton.png"));
    generalSettingsItem ->setText(tr("常规设置"));
    generalSettingsItem ->setTextAlignment(Qt::AlignCenter);
    generalSettingsItem ->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    //下载设置
    QListWidgetItem *downloadSettingsItem = new QListWidgetItem(contentsWidget);
    downloadSettingsItem ->setIcon(QIcon(":/images/downloadSettingsButton.png"));
    downloadSettingsItem ->setText(tr("下载设置"));
    downloadSettingsItem ->setTextAlignment(Qt::AlignCenter);
    downloadSettingsItem ->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    //歌词设置
    QListWidgetItem *lrcSettingsItem = new QListWidgetItem(contentsWidget);
    lrcSettingsItem ->setIcon(QIcon(":/images/lrcSettingsButton.png"));
    lrcSettingsItem ->setText(tr("歌词设置"));
    lrcSettingsItem ->setTextAlignment(Qt::AlignCenter);
    lrcSettingsItem ->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    //热键设置
    QListWidgetItem *hotKeysSettingsItem = new QListWidgetItem(contentsWidget);
    hotKeysSettingsItem ->setIcon(QIcon(":/images/hotKeysSettingsButton.png"));
    hotKeysSettingsItem ->setText(tr("热键设置"));
    hotKeysSettingsItem ->setTextAlignment(Qt::AlignCenter);
    hotKeysSettingsItem ->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    //代理设置
    QListWidgetItem *proxySettingsItem = new QListWidgetItem(contentsWidget);
    proxySettingsItem ->setIcon(QIcon(":/images/proxySettingsButton.png"));
    proxySettingsItem ->setText(tr("代理设置"));
    proxySettingsItem ->setTextAlignment(Qt::AlignCenter);
    proxySettingsItem ->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    connect(contentsWidget,
            SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),
            this, SLOT(slot_ChangePage(QListWidgetItem*,QListWidgetItem*)));
}

//改变页面
void ConfigDialog::slot_ChangePage(QListWidgetItem *current, QListWidgetItem *previous)
{
    if (!current)
    {
        current = previous;
    }
    pagesWidget ->setCurrentIndex(contentsWidget->row(current));
}

//确定
void ConfigDialog::slot_OkFunc()
{
    this ->slot_AppFunc();
    this ->hide();
}

//应用
void ConfigDialog::slot_AppFunc()
{
    this ->downloadAppFunc();
    this ->lrcAppFunc();
    this ->hotKeyAppFunc();
}

//下载页应用
void ConfigDialog::downloadAppFunc()
{
    //得到下载目录
#ifdef _WIN32_WINNT
    CZPlayerConfig::setValue("MUSICDIR_WIN", DownloadSettingsPage::getMusicDir());
    CZPlayerConfig::setValue("ALBUMDIR_WIN", DownloadSettingsPage::getAlbumDir());
    CZPlayerConfig::setValue("LRCDIR_WIN", DownloadSettingsPage::getLrcDir());
#else
    CZPlayerConfig::setValue("MUSICDIR_X11", DownloadSettingsPage::getMusicDir());
    CZPlayerConfig::setValue("ALBUMDIR_X11", DownloadSettingsPage::getAlbumDir());
    CZPlayerConfig::setValue("LRCDIR_X11", DownloadSettingsPage::getLrcDir());
#endif
    this ->mkdirMusicDir();
    this ->mkdirAlbumDir();
    this ->mkdirLrcDir();
}

//歌词页应用
void ConfigDialog::lrcAppFunc()
{
    //写入配置文件
    CZPlayerConfig::setValue("FONT", LrcSettingsPage::getFontCombo());
    CZPlayerConfig::setValue("FONTSIZE", LrcSettingsPage::getFontSizeCombo());
    CZPlayerConfig::setValue("FONTTYPE", LrcSettingsPage::getFontTypeCombo());
    CZPlayerConfig::setValue("NOPLAYSHANG", LrcSettingsPage::getNoPlayEdit());
    CZPlayerConfig::setValue("NOPLAYZHONG", LrcSettingsPage::getNoPlayEdit2());
    CZPlayerConfig::setValue("NOPLAYXIA", LrcSettingsPage::getNoPlayEdit3());
    CZPlayerConfig::setValue("PLAYEDSHANG", LrcSettingsPage::getPlayedEdit());
    CZPlayerConfig::setValue("PLAYEDZHONG", LrcSettingsPage::getPlayedEdit2());
    CZPlayerConfig::setValue("PLAYEDXIA", LrcSettingsPage::getPlayedEdit3());
    CZPlayerConfig::setValue("YUSHECOLOR", LrcSettingsPage::getYuSheColor());
    CZPlayerConfig::setValue("SHADOW", LrcSettingsPage::getShadowSlider());

    //更改歌词属性
    MusicLrc::setFontYS(CZPlayerConfig::getValue("FONT").toString());
    MusicLrc::setFontSize(CZPlayerConfig::getValue("FONTSIZE").toString());
    MusicLrc::setFontType(CZPlayerConfig::getValue("FONTTYPE").toString());
    MusicLrc::setLinearGradient(CZPlayerConfig::getValue("NOPLAYSHANG").toString(),
                                CZPlayerConfig::getValue("NOPLAYZHONG").toString(),
                                CZPlayerConfig::getValue("NOPLAYXIA").toString());
    MusicLrc::setMaskLinearGradient(CZPlayerConfig::getValue("PLAYEDSHANG").toString(),
                                    CZPlayerConfig::getValue("PLAYEDZHONG").toString(),
                                    CZPlayerConfig::getValue("PLAYEDXIA").toString());
    MusicLrc::setLrcShadow(CZPlayerConfig::getValue("SHADOW").toInt());
}

//热键页应用
void ConfigDialog::hotKeyAppFunc()
{
    //写入配置文件
    CZPlayerConfig::setValue("CONFIGOPTIONS", HotKeysSettingsPage::getSettingsHotKey());
    CZPlayerConfig::setValue("SHOWHIDEMAINWIDGET", HotKeysSettingsPage::getMinToTrayHotKey());
    CZPlayerConfig::setValue("MINMAINWIDGET", HotKeysSettingsPage::getOpenMinWidgetHotKey());
    CZPlayerConfig::setValue("OPENMUSICFILE", HotKeysSettingsPage::getOpenMusicFileHotKey());
    CZPlayerConfig::setValue("SHOWHIDEDOWNLOAD", HotKeysSettingsPage::getOpenDownloadWidgetHotKey());
    CZPlayerConfig::setValue("SHOWHIDEMUSICLIST", HotKeysSettingsPage::getOpenListWidgetHotKey());
    CZPlayerConfig::setValue("SHOWHIDELRC", HotKeysSettingsPage::getOpenLrcHotKey());
    CZPlayerConfig::setValue("PLAYPAUSE", HotKeysSettingsPage::getPauseHotKey());
    CZPlayerConfig::setValue("LAST", HotKeysSettingsPage::getLastHotKey());
    CZPlayerConfig::setValue("NEXT", HotKeysSettingsPage::getNextHotKey());

    //更改热键
    GlobalHotKey::setConfigOptions(CZPlayerConfig::getValue("CONFIGOPTIONS").toString());
    GlobalHotKey::setShowHideMainWidget(CZPlayerConfig::getValue("SHOWHIDEMAINWIDGET").toString());
    GlobalHotKey::setMinMainWidget(CZPlayerConfig::getValue("MINMAINWIDGET").toString());
    GlobalHotKey::setOpenMusicfile(CZPlayerConfig::getValue("OPENMUSICFILE").toString());
    GlobalHotKey::setShowDownload(CZPlayerConfig::getValue("SHOWHIDEDOWNLOAD").toString());
    GlobalHotKey::setShowMusicList(CZPlayerConfig::getValue("SHOWHIDEMUSICLIST").toString());
    GlobalHotKey::setShowLrc(CZPlayerConfig::getValue("SHOWHIDELRC").toString());
    GlobalHotKey::setPlayPause(CZPlayerConfig::getValue("PLAYPAUSE").toString());
    GlobalHotKey::setLast(CZPlayerConfig::getValue("LAST").toString());
    GlobalHotKey::setNext(CZPlayerConfig::getValue("NEXT").toString());
}

//创建一个下载歌曲目录
void ConfigDialog::mkdirMusicDir()
{
    QDir *CZPlayer = new QDir;
    //读取配置文件//创建一个文件夹来存储下载的歌曲
#ifdef _WIN32_WINNT
    QString WINPATH = CZPlayerConfig::getValue("MUSICDIR_WIN").toString();
    bool exist = CZPlayer ->exists(WINPATH);
    if (!exist)
    {
        CZPlayer ->mkdir(WINPATH);
    }
#else
    QString X11PATH =  QDir::homePath() + CZPlayerConfig::getValue("MUSICDIR_X11").toString();
    bool exist = CZPlayer ->exists(X11PATH);
    if (!exist)
    {
        CZPlayer ->mkdir(X11PATH);
    }
#endif
}

//创建一个下载专辑目录
void ConfigDialog::mkdirAlbumDir()
{
    QDir *CZPlayer = new QDir;
    //读取配置文件//创建一个文件夹来存储下载的专辑
#ifdef _WIN32_WINNT
    QString WINPATH = CZPlayerConfig::getValue("ALBUMDIR_WIN").toString();
    bool exist = CZPlayer ->exists(WINPATH);
    if (!exist)
    {
        CZPlayer ->mkdir(WINPATH);
    }
#else
    QString X11PATH =  QDir::homePath() + CZPlayerConfig::getValue("ALBUMDIR_X11").toString();
    bool exist = CZPlayer ->exists(X11PATH);
    if (!exist)
    {
        CZPlayer ->mkdir(X11PATH);
    }
#endif
}

//创建一个下载歌词目录
void ConfigDialog::mkdirLrcDir()
{
    QDir *CZPlayer = new QDir;
    //读取配置文件//创建一个文件夹来存储下载的歌词
#ifdef _WIN32_WINNT
    QString WINPATH = CZPlayerConfig::getValue("LRCDIR_WIN").toString();
    bool exist = CZPlayer ->exists(WINPATH);
    if (!exist)
    {
        CZPlayer ->mkdir(WINPATH);
    }
#else
    QString X11PATH =  QDir::homePath() + CZPlayerConfig::getValue("LRCDIR_X11").toString();
    bool exist = CZPlayer ->exists(X11PATH);
    if (!exist)
    {
        CZPlayer ->mkdir(X11PATH);
    }
#endif
}




















