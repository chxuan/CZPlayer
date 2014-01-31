#ifndef ALBUMTHREAD_H
#define ALBUMTHREAD_H

#include "head.h"

class AlbumThread : public QThread
{
    Q_OBJECT

public:
    AlbumThread(QString name, QString artist, QLabel *albumImage);
    ~AlbumThread();

protected:
    virtual void run();

private slots:
    void slot_GetMusicAlbumJson(QNetworkReply *replay);
    void slot_GetMusicAlbumJson2(QNetworkReply *replay);
    void slot_GetAlbum();
    void slot_ReplayFinished();
    void slot_ReplyError(QNetworkReply::NetworkError networkError);//œ¬‘ÿ¥ÌŒÛ

private:
    void getAlbumFromURL(const QUrl &url, const QString &filePath);
    void startFunc();

private:
    QFile albumFile;
    QString m_musicName;
    QString m_musicArtist;
    QString albumFilePath;
    QLabel *m_albumImage;

    QNetworkAccessManager *albumManager;
    QNetworkAccessManager *albumManager2;
    QNetworkAccessManager *albumManager3;
    QNetworkReply *albumReply;
};

#endif // ALBUMTHREAD_H
