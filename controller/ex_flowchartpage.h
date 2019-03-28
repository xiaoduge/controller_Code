#ifndef EX_FLOWCHARTPAGE_H
#define EX_FLOWCHARTPAGE_H

#include "page.h"

class MainWindow;
class navigatorBar;
class QLabel;
class QMouseEvent;

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

    void updateDeviceState();

    void updateTankLevel(int level); //ex

    void createList();

    void setBackColor();

    void buildTitles();

    //

    //
protected:
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);

private:
    navigatorBar *m_pNaviBar;
    QLabel        *m_pLbPageId[5];
};


#endif // EX_FLOWCHARTPAGE_H
