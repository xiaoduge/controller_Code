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


class Sterilize: public QObject
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

class SterilizePage : public CSubPage
{
    Q_OBJECT

public:
    SterilizePage(QObject *parent = 0,CBaseWidget *widget = 0 , MainWindow *wndMain = 0);
    
    virtual void creatTitle();

    virtual void switchLanguage();

    virtual void buildTranslation();

    virtual void initUi();

    virtual void update();

    virtual void fade();
    
    void updateRunInfo(int index,bool bRun);

    void updateRfidInfo(int iRfId);
    
public slots:

    void on_btn_clicked(int);
    
private:

    void setBackColor();
    
    void createControl();

    void buildTitles();

    Sterilize    m_aSterilize[DISP_CLEAN_NUM];

};

#endif // STERILIZEPAGE_H
