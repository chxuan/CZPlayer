#include "StdAfx.h"
#include "HotplugWatcherThread.h"

HotplugWatcherThread::HotplugWatcherThread(QObject *parent) : QThread(parent)
{
	qDebug("当前线程id:%#x, %s", (unsigned int)QThread::currentThreadId(), __PRETTY_FUNCTION__);
	this ->start();
	this ->moveToThread(this); //Let bool event(QEvent *e) be in another thread

	watcher = new QDeviceWatcher;
	watcher->moveToThread(this);
	watcher->appendEventReceiver(this);

	connect(watcher, SIGNAL(deviceAdded(QString)), this, SLOT(slot_DeviceAdded(QString)));
	connect(watcher, SIGNAL(deviceChanged(QString)), this, SLOT(slot_DeviceChanged(QString)));
	connect(watcher, SIGNAL(deviceRemoved(QString)), this, SLOT(slot_DeviceRemoved(QString)));
	watcher->start();
}


HotplugWatcherThread::~HotplugWatcherThread(void)
{
}

//void HotplugWatcherThread::run()
//{
//
//}

//挂载设备
void HotplugWatcherThread::slot_DeviceAdded( const QString &dev )
{
	//qDebug() << "挂载设备";
	qDebug("当前线程id:%#x, %s, 加载:%s", (unsigned int) QThread::currentThreadId(), __PRETTY_FUNCTION__, qPrintable(dev));
}

//移除设备
void HotplugWatcherThread::slot_DeviceRemoved( const QString &dev )
{
	//qDebug() << "移除设备";
	qDebug("当前线程id:%#x, %s, 移除:%s", (unsigned int) QThread::currentThreadId(), __PRETTY_FUNCTION__, qPrintable(dev));
}

//改变设备
void HotplugWatcherThread::slot_DeviceChanged( const QString &dev )
{
	//qDebug() << "改变设备";
	qDebug("当前线程id:#x, %s, 改变:%s", (unsigned int)QThread::currentThreadId(), __PRETTY_FUNCTION__, qPrintable(dev));
}

bool HotplugWatcherThread::event(QEvent *e)
{
	if (e ->type() == QDeviceChangeEvent::registeredType()) 
	{
		QDeviceChangeEvent *event = dynamic_cast<QDeviceChangeEvent*>(e);
		QString action("改变");

		if (event ->action() == QDeviceChangeEvent::Add)
		{
			action = "挂载";
		}
		else if (event ->action() == QDeviceChangeEvent::Remove)
		{
			action = "移除";
		}

		qDebug("当前线程id:%#x, 类型:%d, %s: %s %s", (unsigned int)QThread::currentThreadId(), e ->type(), __PRETTY_FUNCTION__, qPrintable(action), qPrintable(event ->device()));
		event->accept();
		return true;
	}
	return QObject::event(e);
}