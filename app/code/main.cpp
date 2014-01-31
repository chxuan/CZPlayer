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
    translator ->load(QString(":/qt_zh_CN/qt_zh_CN"), QCoreApplication::applicationDirPath(), NULL, NULL);
    qApp ->installTranslator(translator);
}

//文件关联
//void connectFileToProgram()
//{
//    QSettings *ukenvFileReg = new QSettings("HKEY_CLASSES_ROOT\\.{fileType}", QSettings::NativeFormat);   //

//    //判断UKEnv类型是否已在注册表中，并关联了正确的打开方式（程序打开方式），没有则写入
//    QString currentValue = ukenvFileReg->value("Default").toString();

//    if (currentValue.isEmpty() || currentValue != "{Program_Type}")
//    {
//        //QMessageBox::information(NULL,"information","Unregistered or Uncorrect");
//        ukenvFileReg->setValue("Default", "{Program_Type}");
//        QSettings *ukenvIconReg = new QSettings("HKEY_CLASSES_ROOT\\.{fileType}\\DefaultIcon", QSettings::NativeFormat);
//        ukenvIconReg->setValue("Default", QString(qApp->argv()[0]) + ",0");   //将当前运行的程序图标置入
//        delete ukenvIconReg;

//        QSettings *ukenvProgramReg = new QSettings("HKEY_CLASSES_ROOT\\{Program_Type}\\shell\\open\\command", QSettings::NativeFormat);
//        ukenvProgramReg->setValue("Default", QString(qApp->argv()[0]) + " %1"); //将当前运行的程序路径置入
//        delete ukenvProgramReg;
//    }
//    delete ukenvFileReg;
//}

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
    Widget w;
    w.show();

    return app.exec();
}
