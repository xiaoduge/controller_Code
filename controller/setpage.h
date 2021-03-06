#ifndef SETPAGE_H
#define SETPAGE_H

#include "subpage.h"

class MainWindow;
class DPushButton;

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

struct ButtonInfo
{
    QString m_text;
    QString m_fileName;
};

class SetPage : public CSubPage //CPage
{
    Q_OBJECT
public:
    SetPage(QObject *parent = 0,CBaseWidget *widget = 0,MainWindow *wndMain=0 );

    virtual void creatTitle();

    virtual void switchLanguage();

    virtual void buildTranslation();
    
    virtual void initUi();

    CPage *getSubPage(int iIdx) { return m_pSubPages[iIdx];}

    void toInitializePage();

    void setSuperPage(bool bShow);

private:
    void buildTitles();

    void createSubPage();

    void checkLoginInfo();

private:
    CPage *m_pSubPages[SET_PAGE_NUM];
    DPushButton *m_pBtns[SET_BTN_NUMBER];

public slots:
    void on_btn_clicked();

};

#endif // SETPAGE_H
