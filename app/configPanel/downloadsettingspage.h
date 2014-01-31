#ifndef DOWNLOADSETTINGSPAGE_H
#define DOWNLOADSETTINGSPAGE_H

#include <QtCore>
#include <QtGui>

class DownloadSettingsPage : public QWidget
{
    Q_OBJECT

public:
    DownloadSettingsPage(QWidget *parent = 0);
    ~DownloadSettingsPage();

public:
    QString static getMusicDir();
    void static setMusicDir(QString const &musicDir);

    QString static getAlbumDir();
    void static setAlbumDir(QString const &albumDir);

    static QString getLrcDir();
    static void setLrcDir(QString const &lrcDir);

private slots:
    void slot_ChangeDir();

private:
    QWidget *m_parent;
    QLabel *musicDir;
    static QLineEdit *musicEdit;
    QPushButton *scanButton;

    QLabel *albumDir;
    static QLineEdit *albumEdit;
    QPushButton *scanButton2;

    QLabel *lrcFileLabel;
    static QLineEdit *lrcFileEdit;
    QPushButton *scanButton3;
};

#endif // DOWNLOADSETTINGSPAGE_H
