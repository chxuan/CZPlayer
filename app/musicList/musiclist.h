#ifndef MUSICLIST_H
#define MUSICLIST_H

#include "head.h"

//歌曲列表
class MusicList : public QTableWidget
{
    Q_OBJECT

public:
    MusicList(QWidget *widget = 0);
    ~MusicList();

public:
	void setCurrentMusicRow(int row);	//设置当前歌曲播放的行号

protected:
    void contextMenuEvent(QContextMenuEvent *event);

signals:
    void sig_PlayListClean();
    void sig_RowSelected(int row, QString title);
	void sig_SharedCurrentMusic(QString strMusicName);

private slots:
    void slot_ClearAllMusicList();				//清空歌曲
    void slot_DelCurrentMusic();				//删除当前歌曲
    void slot_TableClicked(int index);			//获取选中行
	void slot_TableRowEnter(int row, int /*col*/);	//鼠标滑动到每一行
    void slot_LocateCurrentMusic();				//定位到当前歌曲
	void slot_SaveList();						//保存当前列表
	void slot_ShareCurrentMusic();				//分享当前音乐

private:
    void updateMusicList();						//更新播放列表序号
	void setRowColor(int row, QColor color);	//设置行颜色

private:
    QPoint dragPosition;
    QWidget *parent;
    int row;
	int m_nCurrentMusicRow;
	int m_nPerviousColorRow;
};

#endif // MUSICLIST_H
