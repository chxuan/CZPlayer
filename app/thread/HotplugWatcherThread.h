#ifndef HOTPLUGWATCHERTHTREAD_H
#define HOTPLUGWATCHERTHTREAD_H

#ifndef __GNUC__
#define __PRETTY_FUNCTION__  __FUNCTION__
#endif

#include "head.h"
#include "qdevicewatcher.h"
#include "qdevicechangeevent.h"

class HotplugWatcherThread : public QThread
{
	Q_OBJECT

public:
	HotplugWatcherThread(QObject *parent = 0);
	~HotplugWatcherThread(void);

protected:
	//virtual void run();
	virtual bool event(QEvent *e);

public slots:
	void slot_DeviceAdded(const QString &dev);			//挂载设备
	void slot_DeviceRemoved(const QString &dev);		//移除设备
	void slot_DeviceChanged(const QString &dev);		//改变设备

private:
	QDeviceWatcher *watcher;						//设备监听指针
};

#endif // HOTPLUGWATCHERTHTREAD_H

