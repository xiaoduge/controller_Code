#ifndef SERVICEPAGE_H
#define SERVICEPAGE_H

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

enum SERVICE_BTN_NAME
{  
    SERVICE_BTN_INSTALL = 0,
    SERVICE_BTN_MANAGERCONFIG,
    SET_BTN_USER_CFG,
    SET_BTN_HISTORY_RECORD,
    SET_BTN_PERMISSION, //Permission
    SERVICE_BTN_STERILIZE,
    SET_BTN_SYSTEM_ALLOCATION,
    SET_BTN_SERVICE,
    SERVICE_BTN_NUMBER
};

#define SERVICE_PAGE_NUM SERVICE_BTN_NUMBER

class titleBar;

class ServicePage : public CPage
{
    Q_OBJECT
public:
    ServicePage(QObject *parent = 0,CBaseWidget *widget = 0,MainWindow *wndMain=0 );

    virtual void creatTitle();

    virtual  void update();

    virtual void switchLanguage();

    virtual void buildTranslation();
    
    virtual void initUi();
    virtual void mousePressEvent(QMouseEvent *);
    virtual void mouseReleaseEvent(QMouseEvent *);
    virtual void mouseMoveEvent(QMouseEvent *);

    CPage *getSubPage(int iIdx) { return m_pSubPages[iIdx];}

private:
    void buildTitles();
    void Create_subPage();
    int checkUserInfo(const QString& userName, const QString& passWord);

private:
    CPage *m_pSubPages[SERVICE_PAGE_NUM];
    CBitmapButton *m_pBtns[SERVICE_BTN_NUMBER];   
    titleBar      *m_pTitleBar;
    QLabel        *m_pLbPageId[3];
    

public slots:
    void on_btn_clicked(int tmp);
    void on_navi_clicked(int tmp);

};

#endif // RUNPAGE_H
