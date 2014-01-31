#ifndef GENERALSETTINGSPAGE_H
#define GENERALSETTINGSPAGE_H

#include <QtCore>
#include <QtGui>
#include <QGroupBox>
#include <QCheckBox>

class GeneralSettingsPage : public QWidget
{
    Q_OBJECT

public:
    GeneralSettingsPage(QWidget *parent = 0);
    ~GeneralSettingsPage();

private:
    QWidget *m_parent;

    //QGroupBox *startGB;
//    QCheckBox *autoPlayCB;
//    QCheckBox *autoStartCB;
//    QCheckBox *helloCZPlayerCB;
//    QCheckBox *preWidgetCB;

//    QGroupBox *fileConnect;
//    QCheckBox *defaultPlayer;
};

#endif // GENERALSETTINGSPAGE_H
