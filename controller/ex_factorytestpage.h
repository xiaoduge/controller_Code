#ifndef EX_FACTORYTESTPAGE_H
#define EX_FACTORYTESTPAGE_H


#include "subpage.h"
#include "ctrlapplication.h"


class MainWindow;
class QPushButton;
class QLabel;
class DLineEdit;
class QFrame;
class QComboBox;
class QTextBrowser;

class Ex_FactoryTestPage : public CSubPage
{
    Q_OBJECT
public:
    enum FTEST_ENUM
    {
        FTESTPAGE_FLOW = 0,
        FTESTPAGE_ILOOP,
//        FTESTPAGE_KEY,
        FTESTPAGE_NUM
    };
    enum FLOW_DISPLAY_ENUM
    {
        S1_DISPLAY = 0,
        S2_DISPLAY,
        S3_DISPLAY,
        S4_DISPLAY,
        FLOW_DISPLAY_NUM
    };
    enum ILOOP_DISPLAY_ENUM
    {
        P1_DISPLAY = 0,
        P2_DISPLAY,
        P3_DISPLAY,
        PRESSURE_DISPLAY_NUM
    };
    enum TABPAGE
    {
        FACTORY_PAGE_RFID = 0,
        FACTORY_PAGE_FLOW,
        FACTORY_PAGE_UPDWIFI,
        FACTORY_PAGE_NUM
    };

    enum RFIDPAGECONFIG
    {
        CONFIG_CAT = 0,
        CONFIG_LOT,
        CONFIG_INSTALLDATE,
        CONFIG_VOLUMEOFUSE,
        CONFIG_NUM
    };


public:
    Ex_FactoryTestPage(QObject *parent = 0,CBaseWidget *widget = 0 , MainWindow *wndMain = 0);

    virtual void creatTitle();
    virtual void switchLanguage();
    virtual void buildTranslation();
    virtual void initUi();
    void show(bool bShow);

    void update();
    void fade();

    void updateFlow(int iIndex, int value);
    void updatePressure(int iIndex,float fvalue);
    void updTank(int iLevel,float fVolume);
    void updSourceTank(int iLevel,float fVolume);

    //RFID
    void updateRFIDInfo(int iRfId);

    //wifi
    void updateWifiTestMsg(const QString& msg);

private:
    void buildTitles();
    void setBackColor();

    void initFlowTestPage();
    void initRFIDTestPage();
    void initUpdateWifiPage();

private slots:
    void on_flowBtn_clicked();
    void on_iLoopBtn_clicked();
    void on_keyBtn_clicked();

    void on_writeBtn_clicked();
    void on_readBtn_clicked();
    void on_clearBtn_clicked();

    void on_clearWifiMsgBtn_clicked();

private:
    bool isFlow;
    bool isPressure;

    //
    QTabWidget* m_tabWidget;
    QWidget* m_mainWidget;
    QWidget* m_pageWidget[FACTORY_PAGE_NUM];

    //
    QFrame* m_pFrame[FTESTPAGE_NUM];
    QPushButton* m_pBtn[FTESTPAGE_NUM];
    QLabel* m_pAreaLab[FTESTPAGE_NUM];

    DLineEdit* m_pFlowDisplayLE[FLOW_DISPLAY_NUM];
    QLabel* m_pFlowDisplayLab[FLOW_DISPLAY_NUM];

    DLineEdit* m_pPreDisplayLE[PRESSURE_DISPLAY_NUM];
    QLabel* m_pPreDisplayLab[PRESSURE_DISPLAY_NUM];

    //
    QLabel* m_pConfigLabel[CONFIG_NUM];
    DLineEdit* m_pConfigLineEdit[CONFIG_NUM];

    QLabel* m_pIndexLabel;
    QComboBox* m_pIndexCombo;

    QPushButton *m_pWriteBtn;
    QPushButton *m_pReadBtn;
    QPushButton *m_pClearBtn;

    //update wifi Library
    QPushButton *m_pClearWifiMsgBtn;
    QTextBrowser *m_pWifiMsgTBrowser;
};

extern CtrlApplication *gApp;


#endif // EX_FACTORYTESTPAGE_H
