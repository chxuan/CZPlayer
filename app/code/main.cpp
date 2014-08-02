#include "stdafx.h"
#include "widget.h"
#include "static.h"
#include <QApplication>
#include <qtsingleapplication.h>

//中文支持
void initqt_zh_CN()
{
    //设置编码格式
#ifdef _WIN32_WINNT
    QTextCodec::setCodecForCStrings(QTextCodec::codecForLocale());
    QTextCodec::setCodecForTr(QTextCodec::codecForLocale());
#else
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("gbk"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("gbk"));
#endif
    QTranslator *translator = new QTranslator();
    translator->load(QString(":/qt_zh_CN/qt_zh_CN"), QCoreApplication::applicationDirPath(), NULL, NULL);
    qApp->installTranslator(translator);
}

int main(int argc, char *argv[])
{
	QtSingleApplication app(argc, argv);
	//QApplication app(argc, argv);
	if (app.isRunning())
	{
		//qDebug() << "程序已经启动！";
		return EXIT_SUCCESS;
	}
    app.setApplicationName("CZPlayer");
	
    initqt_zh_CN();//中文支持
    //connectFileToProgram();

    //加载设置qss样式表
    QApplication::setStyle(new QPlastiqueStyle);
    QFile file(":/qss/musicPlayer.qss");
    file.open(QFile::ReadOnly);
    app.setStyleSheet(file.readAll());
    file.close();

    //显示主界面
    Widget w(argv[1]);
    w.show();

    return app.exec();
}
