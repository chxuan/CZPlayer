#ifndef MINMUSICWIDGET_H
#define MINMUSICWIDGET_H

#include "head.h"

class MinMusicWidget : public QWidget
{
    Q_OBJECT

public:
    MinMusicWidget(QWidget *parent = 0);
    ~MinMusicWidget();

    QPushButton *minPlayButton;
    QPushButton *minPauseButton;
    QPushButton *minStopButton;
    QPushButton *minNextButton;
    QPushButton *minLastButton;
    QPushButton *minOpenFileButton;
    QAction *minMusicLrcAction;

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void contextMenuEvent(QContextMenuEvent *event);

signals:
    void sig_ShowMusicPanel();			//打开主界面

private slots:
    void slot_OpenMainWidget();		//打开主界面
    void slot_HideMinWidget();		//隐藏小界面

private:
    QWidget *m_parent;
    QPoint dragPosition;
};

#endif // MINMUSICWIDGET_H
