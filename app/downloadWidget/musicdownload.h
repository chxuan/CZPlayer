#ifndef MUSICDOWNLOAD_H
#define MUSICDOWNLOAD_H

#include "downloadlist.h"
#include "downloadthread.h"
#include "head.h"

class MusicDownload: public QWidget
{
    Q_OBJECT

public:
    MusicDownload(QWidget *widget = 0);
    ~MusicDownload();

protected:
    void paintEvent(QPaintEvent *event);
    bool eventFilter(QObject *target, QEvent *event);

private slots:
    void slot_DownloadMusic();//获取歌曲名，歌手名，开始下载
    void slot_DelCurrentMapItem(QString musicTitle);//删除当前音乐mapIter
//    void delCurrentThread(int row);//删除当前线程

private:
    QWidget *parent;
    DownloadList *downloadList;
    QLabel *musicEditLabel;
    QLabel *artistEditLabel;
    QLineEdit *musicEdit;
    QLineEdit *artistEdit;
    QPushButton *downloadButton;
    map<QString, int> musicMap;
    map<int, DownloadThread*> threadMap;
};

#endif // MUSICDOWNLOAD_H
