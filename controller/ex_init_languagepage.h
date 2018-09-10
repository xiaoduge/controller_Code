#ifndef EX_INIT_LANGUAGEPAGE_H
#define EX_INIT_LANGUAGEPAGE_H


#include <QObject>
#include <QPixmap>
#include <QPalette>
#include <QLabel>
#include <QInputDialog>

#include "basewidget.h"
#include "subpage.h"
#include "cbitmapbutton.h"
#include <QLineEdit>
#include <QLabel>
#include <QTextEdit>
#include <QHBoxLayout>

#include "Display.h"

#define Ex_LanguageNum  (APP_LAN_NUM)

class MainWindow;
class QPushButton;

class Ex_Init_Languagepage : public CSubPage
{
    Q_OBJECT

public:
    QString Country[Ex_LanguageNum] =
    {
        QString(":/pic/English.png"),
        QString(":/pic/China.png"),
        QString(":/pic/Spanish.png"),
        QString(":/pic/France.png"),
        QString(":/pic/Germany.png"),
        QString(":/pic/Italian.png"),

    };

    QString CountryName[Ex_LanguageNum] =
    {
        QString("English"),
        QString("Chinese"),
        QString("Spanish"),
        QString("French"),
        QString("German"),
        QString("Italian"),

    };

    QRect backrect[Ex_LanguageNum] =
    {
        QRect(72 , 163 , 202 , 125),
        QRect(299 , 163 , 202 , 125),
        QRect(526 , 163 , 202 , 125),
        QRect(72 , 320 , 202 , 125),
        QRect(299 , 320 , 202 , 125),
        QRect(526 , 320 , 202 , 125),
    };

public:
    Ex_Init_Languagepage(QObject *parent = 0,CBaseWidget *widget = 0 , MainWindow *wndMain = 0);

    virtual void creatTitle();

    virtual void switchLanguage();

    virtual void buildTranslation();

    virtual void initUi();

    virtual void leaveSubPage();

private:

    void buildTitles();
    void setBackColor();

    CBitmapButton *btnLanguage[Ex_LanguageNum];
    QLabel *lbLanguage[Ex_LanguageNum];
    QLabel *lbStr[Ex_LanguageNum];

    int    m_iLanguage;

    QPushButton* m_nextBtn;
    QPushButton* m_saveBtn;

signals:
    void languageSwitchBtnClicked(int);

public slots:
    void on_btn_clicked(int index);

    void on_nextBtn_clicked();
    void on_saveBtn_clicked();

};


#endif // EX_INIT_LANGUAGEPAGE_H
