#ifndef DOWNLOADTHREAD_H
#define DOWNLOADTHREAD_H

#include "downloadlist.h"
#include "head.h"

class DownloadThread : public QThread
{
    Q_OBJECT

public:
    DownloadThread(QString musicName,
                   QString musicArtist,
                   DownloadList *downloadList);
    ~DownloadThread();

protected:
    virtual void run();

signals:
    void sig_DelCurrentMusicMapItem(QString musicTitle);//删除当前音乐mapIter
    //void musicListMap(map<int, int> listMap);

private slots:
    void slot_GetMusicXML(QNetworkReply *replay);//得到歌曲XML格式
    void slot_GetMusic();//得到歌曲
    void slot_ReplayFinished();//请求完成
    void slot_UpdateDataProgress(qint64,qint64);//更新进度条
    void slot_GetNetSpeed();//得到网速
    void slot_DelCurrentThread(int row);//删除当前线程
    void slot_PauseMusicDownload(int row);
    void slot_ReplyError(QNetworkReply::NetworkError networkError);//下载错误
    void slot_NetWorkInterrupt();//网络中断

private:
    void getMusicFromURL(const QUrl &url, const QString &filePath);

    QNetworkAccessManager *manager;
    QNetworkAccessManager *musicManager;
    QNetworkReply *musicReply;

    QFile musicFile;
    QTimer *timer;
    QTimer *timer2;
    QTime *time;

    QString m_musicName;
    QString m_musicArtist;
    QString m_musicFileName;
    DownloadList *m_downloadList;

    QProgressBar *progressBar;
    QTableWidgetItem *musicTitleItem;
    QTableWidgetItem *musicStatusItem;
    QTableWidgetItem *musicSizeItem;
    QTableWidgetItem *speedItem;
    map<int, DownloadThread*> threadMap;
    map<int, int> listMap;
    //QMultiMap<int, DownloadThread*> threadMap;

    int times;
    double speedD;
    int currentTime;
};

#endif // DOWNLOADTHREAD_H
