#ifndef STERILIZEPAGE_H
#define STERILIZEPAGE_H

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

#include "Display.h"


class MainWindow;


class SystemConfigItem: public QObject
{

public:
    QWidget       *BackWidget;
    QLabel        *laBackNo;
    QLabel        *laName;
    
    QLabel        *laPromptFrame;
    QLabel        *laPrompt;

    QLabel        *laMtTime;
    QLabel        *laNextMtTime;

    CBitmapButton *btnClean;

};

class SystemConfigPage : public CSubPage
{
    Q_OBJECT

public:
    SterilizePage(QObject *parent = 0,CBaseWidget *widget = 0 , MainWindow *wndMain = 0);
    
    virtual void creatTitle();

    virtual void switchLanguage();

    virtual void buildTranslation();

    virtual void initUi();

    void Set_Back_Color();

    void Create_control();
    
    
public slots:

    void on_btn_clicked(int);
    
private:

    void buildTitles();

    Sterilize    m_aSterilize[DISP_CLEAN_NUM];

};

#endif // STERILIZEPAGE_H
