#ifndef HOTKEYSSETTINGSPAGE_H
#define HOTKEYSSETTINGSPAGE_H

#include <QtCore>
#include <QtGui>
#include "hotkeystablewidget.h"

class HotKeysSettingsPage : public QWidget
{
    Q_OBJECT

public:
    HotKeysSettingsPage(QWidget *parent = 0);
    ~HotKeysSettingsPage();

    static QString getSettingsHotKey();
    static void setSettingsHotKey(QString const &value);

    static QString getMinToTrayHotKey();
    static void setMinToTrayHotKey(QString const &value);

    static QString getOpenMinWidgetHotKey();
    static void setOpenMinWidgetHotKey(QString const &value);

    static QString getOpenMusicFileHotKey();
    static void setOpenMusicFileHotKey(QString const &value);

    static QString getOpenDownloadWidgetHotKey();
    static void setOpenDownloadWidgetHotKey(QString const &value);

    static QString getOpenListWidgetHotKey();
    static void setOpenListWidgetHotKey(QString const &value);

    static QString getOpenLrcHotKey();
    static void setOpenLrcHotKey(QString const &value);

    static QString getPauseHotKey();
    static void setPauseHotKey(QString const &value);

    static QString getLastHotKey();
    static void setLastHotKey(QString const &value);

    static QString getNextHotKey();
    static void setNextHotKey(QString const &value);

private slots:
    void slot_BackSetting();//恢复默认方案
	void slot_EditHotKeys();//编辑热键

private:
    QWidget *m_parent;

    static HotKeysTableWidget *hotKeysTableWidget;
    QPushButton *editButton;
    QPushButton *backSettingButton;

    static QTableWidgetItem *settingsHotKey;
    static QTableWidgetItem *minToTrayHotKey;
    static QTableWidgetItem *openMinWidgetHotKey;
    static QTableWidgetItem *openMusicFileHotKey;
    static QTableWidgetItem *openDownloadWidgetHotKey;
    static QTableWidgetItem *openListWidgetHotKey;
    static QTableWidgetItem *openLrcHotKey;
    static QTableWidgetItem *pauseHotKey;
    static QTableWidgetItem *lastHotKey;
    static QTableWidgetItem *nextHotKey;
};

#endif // HOTKEYSSETTINGSPAGE_H
