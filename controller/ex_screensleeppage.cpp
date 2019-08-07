#include "ex_screensleeppage.h"
#include "ExtraDisplay.h"
#include "mainwindow.h"
#include <QLabel>
#include <QPalette>
#include <QTimer>

#include "ex_dsleepwidget.h"


Ex_ScreenSleepPage::Ex_ScreenSleepPage(QObject *parent, CBaseWidget *widget, MainWindow  *wndMain)
    :CPage(parent, widget, wndMain)
{
    buildTranslation();
    initUi();    
}

void Ex_ScreenSleepPage::buildTranslation()
{
    switch(ex_gGlobalParam.Ex_System_Msg.Ex_iCompany)
    {
    case 0:
        buildTranslationTitleRephile();
        break;
    case 1:
        buildTranslationTitleVWR();
        break;
    default:
        break;
    }
}

void Ex_ScreenSleepPage::update()
{
    buildTranslation();

    if(m_pdwidget)
    {
        m_pdwidget->setMsg(m_titleMsg);
    }
}

void Ex_ScreenSleepPage::initUi()
{
    m_pdwidget = new Ex_DSleepWidget(m_titleMsg, 800, 600, m_widget);
    m_pdwidget->setGeometry(0, 0, 800, 600);
}


void Ex_ScreenSleepPage::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    emit pageHide();
}

void Ex_ScreenSleepPage::timerEvent(QTimerEvent *event)
{
    if(event->timerId() == m_timeID)
    {
        int x = randomPos(800);
        int y = randomPos(600) + 30;
        m_pdwidget->setPos(x, y);
        m_pdwidget->update();
    }
}

void Ex_ScreenSleepPage::on_SleepPageShow(bool isShow)
{
    if(isShow)
    {
        m_timeID = startTimer(3000);
    }
    else
    {
        killTimer(m_timeID);
    }
}

int Ex_ScreenSleepPage::randomPos(int max)
{
    qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
    int temp = qrand()%max;
    if(temp < 5)
    {
        temp += 5;
    }
    return temp;
}

void Ex_ScreenSleepPage::buildTranslationTitleRephile()
{
    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
        m_titleMsg = QString("Super-Genie G ") + tr("%1").arg(ex_gGlobalParam.Ex_Machine_Msg.iMachineFlow);
        break;
    case MACHINE_L_UP:
        m_titleMsg = QString("Super-Genie U ") + tr("%1").arg(ex_gGlobalParam.Ex_Machine_Msg.iMachineFlow);
        break;
    case MACHINE_L_EDI_LOOP:
        m_titleMsg = QString("Super-Genie E ") + tr("%1").arg(ex_gGlobalParam.Ex_Machine_Msg.iMachineFlow);
        break;
    case MACHINE_L_RO_LOOP:
        m_titleMsg = QString("Super-Genie R ") + tr("%1").arg(ex_gGlobalParam.Ex_Machine_Msg.iMachineFlow);
        break;
    case MACHINE_Genie:
        m_titleMsg = QString("Genie G ") + tr("%1").arg(ex_gGlobalParam.Ex_Machine_Msg.iMachineFlow);
        break;
    case MACHINE_UP:
        m_titleMsg = QString("Genie U ") + tr("%1").arg(ex_gGlobalParam.Ex_Machine_Msg.iMachineFlow);
        break;
    case MACHINE_EDI:
        m_titleMsg = QString("Genie E ") + tr("%1").arg(ex_gGlobalParam.Ex_Machine_Msg.iMachineFlow);
        break;
    case MACHINE_RO:
        m_titleMsg = QString("Genie R ") + tr("%1").arg(ex_gGlobalParam.Ex_Machine_Msg.iMachineFlow);
        break;
    case MACHINE_PURIST:
        m_titleMsg = QString("Genie PURIST");
        break;
    case MACHINE_ADAPT:
        m_titleMsg = QString("Genie A ") + tr("%1").arg(ex_gGlobalParam.Ex_Machine_Msg.iMachineFlow);
        break;
    default:
        break;
    }
}

void Ex_ScreenSleepPage::buildTranslationTitleVWR()
{
    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
        m_titleMsg = QString("Super-Genie G ") + tr("%1").arg(ex_gGlobalParam.Ex_Machine_Msg.iMachineFlow);
        break;
    case MACHINE_L_UP:
        m_titleMsg = QString("Super-Genie U ") + tr("%1").arg(ex_gGlobalParam.Ex_Machine_Msg.iMachineFlow);
        break;
    case MACHINE_L_EDI_LOOP:
        m_titleMsg = QString("Super-Genie E ") + tr("%1").arg(ex_gGlobalParam.Ex_Machine_Msg.iMachineFlow);
        break;
    case MACHINE_L_RO_LOOP:
        m_titleMsg = QString("Super-Genie R ") + tr("%1").arg(ex_gGlobalParam.Ex_Machine_Msg.iMachineFlow);
        break;
    case MACHINE_Genie:
        m_titleMsg = QString("VWR G ") + tr("%1").arg(ex_gGlobalParam.Ex_Machine_Msg.iMachineFlow);
        break;
    case MACHINE_UP:
        m_titleMsg = QString("VWR U ") + tr("%1").arg(ex_gGlobalParam.Ex_Machine_Msg.iMachineFlow);
        break;
    case MACHINE_EDI:
        m_titleMsg = QString("VWR E ") + tr("%1").arg(ex_gGlobalParam.Ex_Machine_Msg.iMachineFlow);
        break;
    case MACHINE_RO:
        m_titleMsg = QString("VWR R ") + tr("%1").arg(ex_gGlobalParam.Ex_Machine_Msg.iMachineFlow);
        break;
    case MACHINE_PURIST:
        m_titleMsg = QString("VWR P");
        break;
    case MACHINE_ADAPT:
        m_titleMsg = QString("VWR A ") + tr("%1").arg(ex_gGlobalParam.Ex_Machine_Msg.iMachineFlow);
        break;
    default:
        break;
    }
}


