#ifndef EX_FACTORYTESTPAGE_H
#define EX_FACTORYTESTPAGE_H


#include "subpage.h"
#include "ctrlapplication.h"


class MainWindow;
class QPushButton;
class QLabel;
class QLineEdit;
class QFrame;

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


public:
    Ex_FactoryTestPage(QObject *parent = 0,CBaseWidget *widget = 0 , MainWindow *wndMain = 0);

    virtual void creatTitle();
    virtual void switchLanguage();
    virtual void buildTranslation();
    virtual void initUi();
    void show(bool bShow);

    void updateFlow(int iIndex, int value);
    void updatePressure(int iIndex,float fvalue);
    void updTank(int iLevel,float fVolume);
    void updSoureTank(int iLevel,float fVolume);

private:
    void buildTitles();
    void createControl();

private slots:
    void on_flowBtn_clicked();
    void on_iLoopBtn_clicked();
    void on_keyBtn_clicked();

private:
    bool isFlow;
    bool isPressure;
    QFrame* m_pFrame[FTESTPAGE_NUM];
    QPushButton* m_pBtn[FTESTPAGE_NUM];
    QLabel* m_pAreaLab[FTESTPAGE_NUM];

    QLineEdit* m_pFlowDisplayLE[FLOW_DISPLAY_NUM];
    QLabel* m_pFlowDisplayLab[FLOW_DISPLAY_NUM];

    QLineEdit* m_pPreDisplayLE[PRESSURE_DISPLAY_NUM];
    QLabel* m_pPreDisplayLab[PRESSURE_DISPLAY_NUM];

};

extern CtrlApplication *gApp;


#endif // EX_FACTORYTESTPAGE_H
