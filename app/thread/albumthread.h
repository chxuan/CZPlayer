#ifndef ALBUMTHREAD_H
#define ALBUMTHREAD_H

#include "head.h"

class AlbumThread : public QThread
{
	Q_OBJECT

public:
	AlbumThread(QString musicName, QString name, QString artist, QLabel *albumImage);
	~AlbumThread();

protected:
	virtual void run();

signals:
	void sig_AlbumDownloadFinished(QString strMusicName, QString strAlbumUrl);	//专辑下载完成信号

	private slots:
		void slot_GetMusicAlbumJson(QNetworkReply *replay);
		void slot_GetMusicAlbumJson2(QNetworkReply *replay);
		void slot_GetAlbum();
		void slot_ReplayFinished();
		void slot_ReplyError(QNetworkReply::NetworkError networkError);//下载错误

private:
	void getAlbumFromURL(const QUrl &url, const QString &filePath);
	void startFunc();

private:
	QFile albumFile;
	QString m_name;
	QString m_musicArtist;
	QString albumFilePath;
	QString m_strAlbumUrl;
	QString m_strMusicName;
	QLabel *m_albumImage;

	QNetworkAccessManager *albumManager;
	QNetworkAccessManager *albumManager2;
	QNetworkAccessManager *albumManager3;
	QNetworkReply *albumReply;
};

#endif // ALBUMTHREAD_H
