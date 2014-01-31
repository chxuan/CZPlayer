#ifndef PROXYSETTINGSPAGE_H
#define PROXYSETTINGSPAGE_H

#include <QtCore>
#include <QtGui>

class ProxySettingsPage : public QWidget
{
    Q_OBJECT

public:
    ProxySettingsPage(QWidget *parnet = 0);
    ~ProxySettingsPage();

private:
    QWidget *m_parent;
};

#endif // PROXYSETTINGSPAGE_H
