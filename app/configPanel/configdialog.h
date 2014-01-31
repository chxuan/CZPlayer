#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H

#include <QtCore>
#include <QtGui>
#include "generalsettingspage.h"
#include "downloadsettingspage.h"
#include "lrcsettingspage.h"
#include "hotkeyssettingspage.h"
#include "proxysettingspage.h"

class ConfigDialog : public QDialog
{
    Q_OBJECT

public:
    ConfigDialog(QWidget *parent = 0);
    ~ConfigDialog();

public:
    static QListWidget *contentsWidget;

//signals:
//    void changeLrcTransparent(int value);//改变歌词透明度

private slots:
    void slot_ChangePage(QListWidgetItem *current, QListWidgetItem *previous);
    void slot_OkFunc();//确定
    void slot_AppFunc();//应用

private:
    void createIcons();
    void downloadAppFunc();//下载页应用
    void lrcAppFunc();//歌词页应用
    void hotKeyAppFunc();//热键页应用
    void mkdirMusicDir();
    void mkdirAlbumDir();
    void mkdirLrcDir();

private:
    QStackedWidget *pagesWidget;

    QPushButton *okButton;//确定按钮
    QPushButton *cancelButton;//取消按钮
    QPushButton *appButton;//应用按钮

    QWidget *m_parent;

    GeneralSettingsPage *generalSettingsPage;//基本设置
    DownloadSettingsPage *downloadSettingsPage;//下载设置
    LrcSettingsPage *lrcSettingsPage;//歌词设置
    HotKeysSettingsPage *hotKeysSettingsPage;//热键设置
    ProxySettingsPage *proxySettingsPage;//代理设置
};

#endif // CONFIGDIALOG_H
