#ifndef DOWNLOADLIST_H
#define DOWNLOADLIST_H

#include "nofocusdelegate.h"
#include "head.h"

class DownloadList : public QTableWidget
{
    Q_OBJECT

public:
    DownloadList(QWidget *widget = 0);
    ~DownloadList();
    void setMusicListMap(map<int, int> listMap);

protected:
    void contextMenuEvent(QContextMenuEvent *event);

signals:
    void sig_DelCurrentRow(int row);
    void sig_DelCurrentMusicMapItem(QString musicTitle);//删除当前音乐mapIter
    void sig_PauseDownload(int row);//暂停下载

private slots:
    void slot_TableClicked(int index);//获取选中行
    void slot_DelCurrentMusic();//删除当前行
    void slot_PauseMusicDownload();//暂停下载
    void slot_StartMusicDownload();//开始下载

private:
    //void setThreadMap()
    map<int, int> m_listMap;

    QWidget *parent;
    int row;
    int delRow;
};

#endif // DOWNLOADLIST_H
