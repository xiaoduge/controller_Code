#include "ex_flowchartpage.h"
#include <QMouseEvent>

#include "navigatorbar.h"
#include "mainwindow.h"

Ex_FlowChartPage::Ex_FlowChartPage(QObject *parent, CBaseWidget *widget, MainWindow *wndMain)
                                    : CPage(parent, widget, wndMain)
{
    m_pNaviBar = new navigatorBar(m_widget);

    m_pNaviBar->setGeometry(0,0,800,55);
}

void Ex_FlowChartPage::creatTitle()
{
    CPage::creatTitle();
    buildTitles();
    selectTitle(0);
}

void Ex_FlowChartPage::buildTitles()
{
    QStringList stringList;
    stringList << tr("Flow Chart");
    setTitles(stringList);
}

void Ex_FlowChartPage::switchLanguage()
{
    buildTranslation();
    buildTitles();
    selectTitle(titleIndex());
}

void Ex_FlowChartPage::buildTranslation()
{
}

void Ex_FlowChartPage::initUi()
{
}

void Ex_FlowChartPage::update()
{
}

void Ex_FlowChartPage::fade()
{
}

void Ex_FlowChartPage::mousePressEvent(QMouseEvent *e)
{
    if (e->x() < 800
        && e->y() < 50)
    {
        m_pNaviBar->display(true);
    }

    if (!m_lstFlag)
    {
        m_lstX = e->x();
        m_lstY = e->y();
        m_lstFlag = 1;
        m_curX = e->x();
        m_curY = e->y();
    }
}

void Ex_FlowChartPage::mouseReleaseEvent(QMouseEvent *e)
{
    if (abs(m_curX - m_lstX) >= PAGE_X_DIMENSION
        && abs(m_curY - m_lstY) <= PAGE_Y_DIMENSION)
    {

        m_wndMain->naviPage(0,m_curX - m_lstX > 0 ? 1 : 0);
    }

    m_lstFlag = 0;
}

void Ex_FlowChartPage::mouseMoveEvent(QMouseEvent *e)
{
    if (0 == e->x()
        && 0 == e->y())
    {
       return;
    }
    m_curX = e->x();
    m_curY = e->y();
}



