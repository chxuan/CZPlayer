#ifndef MUSICLISTWIDGET_H
#define MUSICLISTWIDGET_H


#include "musiclist.h"
#include "head.h"

//∏Ë«˙¡–±Ì√Ê∞Â
class MusicListWidget : public QWidget
{
    Q_OBJECT

public:
    MusicListWidget(MusicList *playList,
                    map<QString, int> *mapMusicRows,
                    QWidget *parent = 0);
    ~MusicListWidget();

protected:
    void paintEvent(QPaintEvent *event);
    bool eventFilter(QObject *target, QEvent *event);

private slots:
    void slot_OnClickedAddMusic();
    void slot_SearchMusic();
    void slot_LineEditFocus();
    void slot_BackMusicList();

private:
    QWidget *m_parent;
    MusicList *m_playList;
    QTimer *timer;
    map<QString, int> *m_mapMusicRows;

    QLineEdit *searchLineEdit;
    QPushButton *searchButton;
    QPushButton *addMusicButton;
    QPushButton *localeButton;
    QPushButton *delMusicButton;
    QPushButton *findButton;
    QPushButton *backButton;
};

#endif // MUSICLISTWIDGET_H
