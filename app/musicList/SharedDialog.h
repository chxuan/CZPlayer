#pragma once

#include <Windows.h>
#include <QtCore>
#include <QtGui>

//歌曲分享对话框
class SharedDialog : public QDialog
{
	Q_OBJECT

public:
	SharedDialog(QWidget *parent = 0);
	~SharedDialog(void);

public:
	void initSharedDialog(QString albumImagePath, QString strMusicName, QString strAlbumUrlPath);		//初始化分享对话框

private slots:
	void slot_CheckBoxStateChanged(int state);									//复选框状态改变
	void slot_SharedMusic();													//分享歌曲

private:
	QWidget *m_parent;
	QString m_strAlbumUrlPath;
	QString m_strMusicName;

	QLabel *albumLabel;
	QTextEdit *textEdit;
	QLabel *sharedLabel;

	QCheckBox *xinlangweiboCheckBox;	//新浪微博
	QLabel *xinlangweiboLabel;

	QCheckBox *tengxunweiboCheckBox;	//腾讯微博
	QLabel *tengxunweiboLabel;

	QCheckBox *qqkongjianCheckBox;		//QQ空间
	QLabel *qqkongjianLabel;

	QCheckBox *renrenCheckBox;			//人人网
	QLabel *renrenLabel;

	QCheckBox *kaixinCheckBox;			//开心网
	QLabel *kaixinLabel;

	QCheckBox *doubanCheckBox;			//豆瓣网
	QLabel *doubanLabel;

	QCheckBox *souhuweiboCheckBox;		//搜狐微博
	QLabel *souhuweiboLabel;

	QCheckBox *qqhaoyouCheckBox;		//QQ好友
	QLabel *qqhaoyouLabel;

	QPushButton *shareButton;			//分享按钮
	QPushButton *cancelButton;			//取消按钮

private:
	int astr_ustr( char *ansistr, WCHAR *unicodestr );		//ascii字符转换成Unicode字符
	void openBrowser(QString &qStrUrl);						//打开浏览器
};

