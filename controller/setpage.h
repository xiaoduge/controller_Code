#ifndef SETPAGE_H
#define SETPAGE_H

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

enum SET_BTN_NAME
{
    SET_BTN_MAINTAIN_PERIOD = 0, 
    SET_BTN_ALARM_SETTING,
    SET_BTN_SYSTEM_TEST,
    SET_BTN_SYSTEM_PARAMETER_CALIBRATE,
    SET_BTN_SYSTEM_PARAM_CONFIG,
    SET_BTN_SYSTEM_DEVICE_CONFIG,
    SET_BTN_SYSTEM_NETWORK,
    SET_BTN_PERIPHERAL_DEVICE_MANAGER,
    SET_BTN_INITIALIZE,   //Initialize
    SET_BTN_SYSTEM_SUPER, //super power- ex_dcj
    SET_BTN_SYSTEM_FACTORYTEST, //ex
    SET_BTN_NUMBER
};

#define SET_PAGE_NUM SET_BTN_NUMBER

class titleBar;

class SetPage : public CPage
{
    Q_OBJECT
public:
    SetPage(QObject *parent = 0,CBaseWidget *widget = 0,MainWindow *wndMain=0 );

    virtual void creatTitle();

    virtual  void update();

    virtual void switchLanguage();

    virtual void buildTranslation();
    
    virtual void initUi();
    virtual void mousePressEvent(QMouseEvent *);
    virtual void mouseReleaseEvent(QMouseEvent *);
    virtual void mouseMoveEvent(QMouseEvent *);

    CPage *getSubPage(int iIdx) { return m_pSubPages[iIdx];}

    void toInitializePage();

private:
    void buildTitles();

    void createSubPage();

private:
    CPage *m_pSubPages[SET_PAGE_NUM];

    CBitmapButton *m_pBtns[SET_PAGE_NUM];
    
    titleBar      *m_pTitleBar;
    
    QLabel        *m_pLbPageId[4];
    

public slots:
    void on_btn_clicked(int tmp);
    void on_navi_clicked(int tmp);

};

#endif // SETPAGE_H
