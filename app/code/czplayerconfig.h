#ifndef CZPLAYERCONFIG_H
#define CZPLAYERCONFIG_H

#include "head.h"
#include <QVariant>
#include <QSettings>

class CZPlayerConfig : public QObject
{
    Q_OBJECT

public:
    CZPlayerConfig(QObject *parent = 0);
    ~CZPlayerConfig();

    //读取配置文件
    static QVariant getValue(const QString &key);
    //写配置文件
    static void setValue(const QString &key, const QVariant &value);

private:
    static QSettings *m_CZPlayerSetting;
    QObject *m_parent;
};

#endif // CZPLAYERCONFIG_H
