#include "stdafx.h"
#include "czplayerconfig.h"
#include "static.h"

QSettings* CZPlayerConfig::m_CZPlayerSetting = 0;

CZPlayerConfig::CZPlayerConfig(QObject *parent) : m_parent(parent)
{
}

CZPlayerConfig::~CZPlayerConfig()
{
}

//读取配置文件
QVariant CZPlayerConfig::getValue(const QString &key)
{
    QString configPath = CZPlayerConfigName;
    if (!m_CZPlayerSetting)
    {
        m_CZPlayerSetting = new QSettings(configPath, QSettings::IniFormat);
        m_CZPlayerSetting ->setIniCodec("UTF-8");
    }
    return m_CZPlayerSetting ->value(key);
}

//写配置文件
void CZPlayerConfig::setValue(const QString &key, const QVariant &value)
{
    QString configPath = CZPlayerConfigName;
    if (!m_CZPlayerSetting)
    {
        m_CZPlayerSetting = new QSettings(configPath, QSettings::IniFormat);
        m_CZPlayerSetting ->setIniCodec("UTF-8");
    }
    m_CZPlayerSetting ->setValue(key, value);
}















