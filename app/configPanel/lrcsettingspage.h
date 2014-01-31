#ifndef LRCSETTINGSPAGE_H
#define LRCSETTINGSPAGE_H

#include <QtCore>
#include <QtGui>
#include "lrcdemo.h"

class LrcSettingsPage : public QWidget
{
    Q_OBJECT

public:
    LrcSettingsPage(QWidget *parent = 0);
    ~LrcSettingsPage();

public:
    static QString getNoPlayEdit();
    static void setNoPlayEdit(QString const &value);

    static QString getNoPlayEdit2();
    static void setNoPlayEdit2(QString const &value);

    static QString getNoPlayEdit3();
    static void setNoPlayEdit3(QString const &value);

    static QString getPlayedEdit();
    static void setPlayedEdit(QString const &value);

    static QString getPlayedEdit2();
    static void setPlayedEdit2(QString const &value);

    static QString getPlayedEdit3();
    static void setPlayedEdit3(QString const &value);

    static QString getFontCombo();
    static void setFontCombo(QString const &value);

    static QString getFontSizeCombo();
    static void setFontSizeCombo(QString const &value);

    static QString getFontTypeCombo();
    static void setFontTypeCombo(QString const &value);

    static QString getYuSheColor();
    static void setYuSheColor(QString const &value);

    static QString getShadowSlider();
    static void setShadowSlider(int const &value);

private slots:
    void slot_ChangeColor();
    void slot_BackToColor();
    void slot_YuSheColor();
    void slot_ChangeYuSheColor();
    void slot_ChangeLrcDemoFont(QString value);
    void slot_ChangeLrcDemoColor(QString value);
    void slot_ChangelrcDemoShadow(int value);

private:
    static void setColor(QLineEdit *lineEdit, QString const &value);

private:
    QWidget *m_parent;

    LrcDemo *lrcDemo;

    QLabel *fontLabel;
    QLabel *fontSizeLabel;
    QLabel *fontTypeLabel;
    QLabel *shadowLabel;

//    static QPalette palette;

    static QLineEdit *noPlayEdit;
    static QLineEdit *noPlayEdit2;
    static QLineEdit *noPlayEdit3;
    static QLineEdit *playedEdit;
    static QLineEdit *playedEdit2;
    static QLineEdit *playedEdit3;

    static QComboBox *fontCombo;
    static QComboBox *fontSizeCombo;
    static QComboBox *fontTypeCombo;

    static QAction *jingDianWhite;
    static QAction *haiYangBlue;
    static QAction *huoYanRed;
    static QAction *kuXuanBlack;
    static QAction *qiuSeYellow;
    static QAction *langManPurple;
    static QAction *ziRanGreen;

    static QSlider *shadowSlider;
    static QSpinBox *shadowSpinBox;

    QPushButton *changeNoPlayButton;
    QPushButton *changeNoPlayButton2;
    QPushButton *changeNoPlayButton3;
    QPushButton *changePlayedButton;
    QPushButton *changePlayedButton2;
    QPushButton *changePlayedButton3;
    QPushButton *yuSheButton;
    QPushButton *backToButton;
};

#endif // LRCSETTINGSPAGE_H
