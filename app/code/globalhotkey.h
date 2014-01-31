#ifndef GLOBALHOTKEY_H
#define GLOBALHOTKEY_H

#include <QtCore>
#include <QtGui>
#include "qxtglobalshortcut.h"

class GlobalHotKey : public QObject
{
    Q_OBJECT

public:
    GlobalHotKey(QWidget *parent = 0);
    ~GlobalHotKey();

	//设置全局热键
    static void setConfigOptions(const QString &value);
    static void setShowHideMainWidget(const QString &value);
    static void setMinMainWidget(const QString &value);
    static void setOpenMusicfile(const QString &value);
    static void setShowDownload(const QString &value);
    static void setShowMusicList(const QString &value);
    static void setShowLrc(const QString &value);
    static void setPlayPause(const QString &value);
    static void setLast(const QString &value);
    static void setNext(const QString &value);

	//设置全局热键
	static void configOptionsDisabled(const bool &value);
	static void showHideMainWidgetDisabled(const bool &value);
	static void minMainWidgetDisabled(const bool &value);
	static void openMusicfileDisabled(const bool &value);
	static void showDownloadDisabled(const bool &value);
	static void showMusicListDisabled(const bool &value);
	static void showLrcDisabled(const bool &value);
	static void playPauseDisabled(const bool &value);
	static void lastDisabled(const bool &value);
	static void nextDisabled(const bool &value);

private:
    static QxtGlobalShortcut *configOptions;
    static QxtGlobalShortcut *showHideMainWidget;
    static QxtGlobalShortcut *minMainWidget;
    static QxtGlobalShortcut *openMusicfile;
    static QxtGlobalShortcut *showDownload;
    static QxtGlobalShortcut *showMusicList;
    static QxtGlobalShortcut *showLrc;
    static QxtGlobalShortcut *playPause;
    static QxtGlobalShortcut *last;
    static QxtGlobalShortcut *nextSc;
};

#endif // GLOBALHOTKEY_H
