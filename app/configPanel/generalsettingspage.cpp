#include "stdafx.h"
#include "generalsettingspage.h"

GeneralSettingsPage::GeneralSettingsPage(QWidget *parent) : m_parent(parent)
{
    QGroupBox *startGB = new QGroupBox(tr("启动时"));
    QCheckBox *autoPlayCB = new QCheckBox(tr("自动播放歌曲"));
    QCheckBox *autoStartCB = new QCheckBox(tr("开机时自动启动CZPlayer"));
    QCheckBox *helloCZPlayerCB = new QCheckBox(tr("播放“Hello CZPlayer”问候语"));
    QCheckBox *preWidgetCB = new QCheckBox(tr("恢复上次关闭时的界面"));

    QGroupBox *fileConnect = new QGroupBox(tr("文件关联"));
    QCheckBox *defaultPlayer = new QCheckBox(tr("将CZPlayer作为默认播放器"));

    QGridLayout *startGBLayoutG = new QGridLayout;
    startGBLayoutG ->addWidget(autoPlayCB, 0, 0);
    startGBLayoutG ->addWidget(autoStartCB, 0, 1);
    startGBLayoutG ->addWidget(helloCZPlayerCB, 1, 0);
    startGBLayoutG ->addWidget(preWidgetCB, 1, 1);
    startGB ->setLayout(startGBLayoutG);

    QVBoxLayout *fileLayout = new QVBoxLayout;
    fileLayout ->addWidget(defaultPlayer);
    fileConnect ->setLayout(fileLayout);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout ->addWidget(startGB);
    mainLayout ->addWidget(fileConnect);
    mainLayout ->addStretch();
    this ->setLayout(mainLayout);
}

GeneralSettingsPage::~GeneralSettingsPage()
{
}
