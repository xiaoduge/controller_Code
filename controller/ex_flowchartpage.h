#ifndef EX_FLOWCHARTPAGE_H
#define EX_FLOWCHARTPAGE_H

#include "page.h"
#include "Display.h"

class MainWindow;
class titleBar;
class QLabel;
class QMouseEvent;
class Ex_FlowChartWidget;

class Ex_FlowChartPage : public CPage
{
    Q_OBJECT
public:
    Ex_FlowChartPage(QObject *parent = 0,CBaseWidget *widget = 0 , MainWindow *wndMain = 0);
    virtual void creatTitle();

    virtual void switchLanguage();

    virtual void buildTranslation();

    virtual void initUi();

    virtual void update();

    virtual void fade();

    void updateSwitchInfo();

    void updateRpumpInfo(int iChl);

    void updateGpumpInfo(int iChl);

    void updateRectInfo(int iChl);

    void updateEdiInfo(int iChl);

    void updateWorkState();

    void updTank(int iIndex,float fVolume);
    void updEcoInfo(int iIndex, ECO_INFO_STRU *info);
    void updPressure(int iIndex,float fvalue);
    void updFlowInfo(int iIndex,int Value);
    void updSourceTank(float fvalue);
    void updTOC(float fToc);

    void buildTitles();

public slots:
    void on_navi_clicked(int tmp);
    void updateUnits();

protected:
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);

private:
    titleBar      *m_pTitleBar;
    Ex_FlowChartWidget *m_pFlowChartWidget;

    int   m_iTankLevel;
    float m_fTankLevel;
};


#endif // EX_FLOWCHARTPAGE_H
