#ifndef MENUPAGE_H
#define MENUPAGE_H

#include <QObject>
#include <QPixmap>
#include <QPalette>
#include <QLabel>
#include <QInputDialog>

#include "basewidget.h"
#include "page.h"
#include "cbitmapbutton.h"
#include <QLineEdit>
#include <QLabel>


class MainWindow;

enum MENU_BTN_NAME
{
    MENU_BTN_WATER_QUALITY_PARAMETER = 0, 
    MENU_BTN_CONSUMPATION_MATERAIL_SATUS,
    MENU_BTN_SYSTEM_MONITOR,
    MENU_BTN_SYSTEM_NOTIFICATION_ALARM,
    MENU_BTN_USER_MANU,
    MENU_BTN_SYSTEM_INFORMATION,
    MENU_BTN_NUMBER
};

#define MENU_PAGE_NUM MENU_BTN_NUMBER


class MainWindow;

class titleBar;

class MenuPage : public CPage
{
    Q_OBJECT
public:
    MenuPage(QObject *parent = 0,CBaseWidget *widget = 0,MainWindow *wndMain=0 );

    virtual void creatTitle();

    virtual  void update();

    virtual void switchLanguage();

    virtual void buildTranslation();
    
    virtual void initUi();
    virtual void mousePressEvent(QMouseEvent *);
    virtual void mouseReleaseEvent(QMouseEvent *);
    virtual void mouseMoveEvent(QMouseEvent *);

    void goAlarm(){on_btn_clicked(MENU_BTN_SYSTEM_NOTIFICATION_ALARM);}

    CPage *getSubPage(int iIdx) {return m_pSubPages[iIdx];}

private:
    void buildTitles();

    void Create_subPage();

    CPage         *m_pSubPages[MENU_PAGE_NUM];

    CBitmapButton *m_pBtns[MENU_BTN_NUMBER];
    
    titleBar      *m_pTitleBar;
    
    QLabel        *m_pLbPageId[4];


public slots:
    void on_btn_clicked(int tmp);
    void on_navi_clicked(int tmp);

};

#endif // RUNPAGE_H
