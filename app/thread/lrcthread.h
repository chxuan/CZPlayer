#ifndef LRCTHREAD_H
#define LRCTHREAD_H

#include "head.h"

class LrcThread : public QThread
{
    Q_OBJECT

public:
    LrcThread(QString musicName, QString musicArtist, QString lrcFileName, QLabel *lrc);
    ~LrcThread();

protected:
    virtual void run();

signals:
	void sig_ResolveLrc();//解析歌词信号

private slots:
	void slot_GetMusicXML(QNetworkReply *replay);//得到MusicXML
	void slot_GetLrc(QNetworkReply *replay);//得到歌词

private:
	void startFunc();
	void getLrcFromURL(const QUrl &url);

private:
	QFile lrcFile;
	QString m_musicName;
	QString m_musicArtist;
	QString m_lrcFilePath;

	QLabel *m_lrc;

	QNetworkAccessManager *lrcManager;
	QNetworkAccessManager *lrcManager2;
};

#endif // LRCTHREAD_H
