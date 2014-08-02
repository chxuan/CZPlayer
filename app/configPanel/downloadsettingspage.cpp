#include "stdafx.h"
#include "downloadsettingspage.h"
#include "static.h"

QLineEdit* DownloadSettingsPage::musicEdit = 0;
QLineEdit* DownloadSettingsPage::albumEdit = 0;
QLineEdit* DownloadSettingsPage::lrcFileEdit = 0;

DownloadSettingsPage::DownloadSettingsPage(QWidget *parent) : m_parent(parent)
{
    QGroupBox *dirSettingGB = new QGroupBox(tr("目录设置"));

    musicDir = new QLabel(this);
    musicDir->setObjectName(tr("downloadDir"));
    musicDir->setText(tr("歌曲目录："));

    musicEdit = new QLineEdit(this);
    musicEdit->setObjectName(tr("downloadEdit"));

    scanButton = new QPushButton(this);
    scanButton->setObjectName(tr("scanButton"));
    scanButton->setText(tr("浏览"));

    albumDir = new QLabel(this);
    albumDir->setObjectName(tr("albumDir"));
    albumDir->setText(tr("专辑目录："));

    albumEdit = new QLineEdit(this);
    albumEdit->setObjectName(tr("albumEdit"));

    scanButton2 = new QPushButton(this);
    scanButton2->setObjectName(tr("scanButton2"));
    scanButton2->setText(tr("浏览"));

    lrcFileLabel = new QLabel(this);
    lrcFileLabel->setObjectName(tr("lrcFileLabel"));
    lrcFileLabel->setText(tr("歌词目录："));

    lrcFileEdit = new QLineEdit(this);
    lrcFileEdit->setObjectName(tr("lrcFileEdit"));

    scanButton3 = new QPushButton(this);
    scanButton3->setObjectName(tr("scanButton3"));
    scanButton3->setText(tr("浏览"));

    QHBoxLayout *musicDirLayout = new QHBoxLayout;
    musicDirLayout->addWidget(musicDir);
    musicDirLayout->addWidget(musicEdit);
    musicDirLayout->addWidget(scanButton);

    QHBoxLayout *albumDirLayout = new QHBoxLayout;
    albumDirLayout->addWidget(albumDir);
    albumDirLayout->addWidget(albumEdit);
    albumDirLayout->addWidget(scanButton2);

    QHBoxLayout *lrcDirLayout = new QHBoxLayout;
    lrcDirLayout->addWidget(lrcFileLabel);
    lrcDirLayout->addWidget(lrcFileEdit);
    lrcDirLayout->addWidget(scanButton3);

    QVBoxLayout *dirSettingLayout = new QVBoxLayout;
    dirSettingLayout->addLayout(musicDirLayout);
    dirSettingLayout->addLayout(albumDirLayout);
    dirSettingLayout->addLayout(lrcDirLayout);
    dirSettingGB->setLayout(dirSettingLayout);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(dirSettingGB);
    mainLayout->addStretch();
    this->setLayout(mainLayout);

    connect(scanButton, SIGNAL(clicked()), this, SLOT(slot_ChangeDir()));
    connect(scanButton2, SIGNAL(clicked()), this, SLOT(slot_ChangeDir()));
    connect(scanButton3, SIGNAL(clicked()), this, SLOT(slot_ChangeDir()));
}

DownloadSettingsPage::~DownloadSettingsPage()
{
}

//得到音乐目录
QString DownloadSettingsPage::getMusicDir()
{
    return musicEdit->text();
}

//设置音乐目录
void DownloadSettingsPage::setMusicDir(const QString &musicDir)
{
    musicEdit->setText(musicDir);
}

//得到专辑目录
QString DownloadSettingsPage::getAlbumDir()
{
    return albumEdit->text();
}

//得到歌词目录
QString DownloadSettingsPage::getLrcDir()
{
    return lrcFileEdit->text();
}

//设置歌词目录
void DownloadSettingsPage::setLrcDir(const QString &lrcDir)
{
    lrcFileEdit->setText(lrcDir);
}

//设置专辑目录
void DownloadSettingsPage::setAlbumDir(const QString &albumDir)
{
    albumEdit->setText(albumDir);
}

//更改下载目录
void DownloadSettingsPage::slot_ChangeDir()
{
    QPushButton *button = dynamic_cast<QPushButton *>(sender());
#ifdef _WIN32_WINNT
    QString directory = QFileDialog::getExistingDirectory(this, tr("打开目录"),
                                                    "E:\\",
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);
#else
    QString homePath = QDir::homePath();
    QString directory = QFileDialog::getExistingDirectory(this, tr("打开目录"),
                                                    homePath,
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);
#endif
    if (button == scanButton)
    {
        if (!directory.isEmpty())
        {
            musicEdit->setText(directory);
        }
    }
    else if (button == scanButton2)
    {
        if (!directory.isEmpty())
        {
            albumEdit->setText(directory);
        }
    }
    else if (button == scanButton3)
    {
        if (!directory.isEmpty())
        {
            lrcFileEdit->setText(directory);
        }
    }
}










