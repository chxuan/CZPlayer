#ifndef GENERALSETTINGSPAGE_H
#define GENERALSETTINGSPAGE_H

#include <QtCore>
#include <QtGui>

class GeneralSettingsPage : public QWidget
{
    Q_OBJECT

public:
    GeneralSettingsPage(QWidget *parent = 0);
    ~GeneralSettingsPage();
public:
	static Qt::CheckState getAutoPlay();
	static void setAutoPlay(Qt::CheckState const &state);

	static Qt::CheckState getAutoStart();
	static void setAutoStart(Qt::CheckState const &state);

	static Qt::CheckState getGreetings();
	static void setGreetings(Qt::CheckState const &state);

	static Qt::CheckState getDefaultPlayer();
	static void setDefaultPlayer(Qt::CheckState const &state);

private:
    QWidget *m_parent;

    static QCheckBox *autoPlayCB;
    static QCheckBox *autoStartCB;
    static QCheckBox *helloCZPlayerCB;
    static QCheckBox *defaultPlayer;
};

#endif // GENERALSETTINGSPAGE_H
