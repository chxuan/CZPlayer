#pragma once

#include "head.h"
#include "SearchList.h"
#include "SearchThread.h"

//搜索面板
class SearchPanel : public QWidget
{
	Q_OBJECT

public:
	SearchPanel(QWidget *widget = 0);
	~SearchPanel(void);

protected:
	void paintEvent(QPaintEvent *event);
	bool eventFilter(QObject *target, QEvent *event);

private slots:
	void slot_SearchMusic();					//搜索歌曲
	void slot_LineEditFocus();					//输入框聚焦
	void slot_ScrollBarEvent(int value);		//滚动条事件
	void slot_SearchNum(int nNum);				//搜到的相关歌曲数量
	void slot_LoadFinished();					//加载完成
	void slot_CheckBoxStateChanged(int state);	//复选框状态改变
	void slot_SearchTimeout();					//搜索超时

private:
	QWidget *m_parent;
	SearchThread *searchThread;
	QLineEdit *searchLineEdit;
	QPushButton *searchButton;
	QLabel *searchInfoLabel;
	QLabel *searchInfoLabel2;
	SearchList *searchList;
	QTimer *timer;
	QCheckBox *allChecked;
	QPushButton *collectButton;
	QPushButton *downButton;
	QScrollBar *scrollBar;
	int m_nMusicNum;
	int m_nStartPage;				//已经加载页面数量
	QString m_strSearchKey;
	bool m_bLoadStared;				//是否已经开始加载

	QLabel *navigationBarLabel;
	QPushButton *searchMusicButton;			//搜索
	QPushButton *broadcastingStationButton;	//电台
	QPushButton *listButton;				//榜单
	QPushButton *downLoadListButton;		//下载列表
	QPushButton *favoritesListButton;		//收藏列表
	QPushButton *lyricsPhotoBarButton;		//歌词写真
};


