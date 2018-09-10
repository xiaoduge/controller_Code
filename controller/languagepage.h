#ifndef LANGUAGEPAGE_H
#define LANGUAGEPAGE_H

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

#define LanguageNum  (APP_LAN_NUM)

class MainWindow;

class LanguagePage : public CSubPage
{
    Q_OBJECT

public:
    LanguagePage(QObject *parent = 0,CBaseWidget *widget = 0 , MainWindow *wndMain = 0);

    virtual void creatTitle();

    virtual void switchLanguage();

    virtual void buildTranslation();

    virtual void initUi();
    
    virtual void leaveSubPage();

private:

    void buildTitles();

    void setBackColor();


    CBitmapButton *btnLanguage[LanguageNum];
    QLabel *lbLanguage[LanguageNum];
    QLabel *lbStr[LanguageNum];

    int    m_iLanguage;

public slots:
    void on_btn_clicked(int index);

};

#endif // LANGUAGEPAGE_H
