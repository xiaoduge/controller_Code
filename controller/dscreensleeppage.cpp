#include "dscreensleeppage.h"
#include "exconfig.h"
#include "mainwindow.h"
#include <QLabel>
#include <QPalette>
#include <QTimer>

#include "dsleepwidget.h"


DScreenSleepPage::DScreenSleepPage(QObject *parent, CBaseWidget *widget, MainWindow  *wndMain)
    :CPage(parent, widget, wndMain)
{
    buildTranslation();
    initUi();    
}

void DScreenSleepPage::buildTranslation()
{
    switch(gAdditionalCfgParam.productInfo.iCompany)
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

void DScreenSleepPage::update()
{
    buildTranslation();

    if(m_pdwidget)
    {
        m_pdwidget->setMsg(m_titleMsg);
    }
}

void DScreenSleepPage::initUi()
{
    m_pdwidget = new DSleepWidget(m_titleMsg, 800, 600, m_widget);
    m_pdwidget->setGeometry(0, 0, 800, 600);
}


void DScreenSleepPage::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    emit pageHide();
}

void DScreenSleepPage::timerEvent(QTimerEvent *event)
{
    if(event->timerId() == m_timeID)
    {
        int x = randomPos(800);
        int y = randomPos(600) + 30;
        m_pdwidget->setPos(x, y);
        m_pdwidget->update();
    }
}

void DScreenSleepPage::on_SleepPageShow(bool isShow)
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

int DScreenSleepPage::randomPos(int max)
{
    qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
    int temp = qrand()%max;
    if(temp < 5)
    {
        temp += 5;
    }
    return temp;
}

void DScreenSleepPage::buildTranslationTitleRephile()
{
    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
        m_titleMsg = QString("Super-Genie G ") + tr("%1").arg(gAdditionalCfgParam.machineInfo.iMachineFlow);
        break;
    case MACHINE_L_UP:
        m_titleMsg = QString("Super-Genie U ") + tr("%1").arg(gAdditionalCfgParam.machineInfo.iMachineFlow);
        break;
    case MACHINE_L_EDI_LOOP:
        m_titleMsg = QString("Super-Genie E ") + tr("%1").arg(gAdditionalCfgParam.machineInfo.iMachineFlow);
        break;
    case MACHINE_L_RO_LOOP:
        m_titleMsg = QString("Super-Genie R ") + tr("%1").arg(gAdditionalCfgParam.machineInfo.iMachineFlow);
        break;
    case MACHINE_Genie:
        m_titleMsg = QString("Genie G ") + tr("%1").arg(gAdditionalCfgParam.machineInfo.iMachineFlow);
        break;
    case MACHINE_UP:
        m_titleMsg = QString("Genie U ") + tr("%1").arg(gAdditionalCfgParam.machineInfo.iMachineFlow);
        break;
    case MACHINE_EDI:
        m_titleMsg = QString("Genie E ") + tr("%1").arg(gAdditionalCfgParam.machineInfo.iMachineFlow);
        break;
    case MACHINE_RO:
        m_titleMsg = QString("Genie R ") + tr("%1").arg(gAdditionalCfgParam.machineInfo.iMachineFlow);
        break;
    case MACHINE_PURIST:
        m_titleMsg = QString("Genie PURIST");
        break;
    case MACHINE_ADAPT:
        m_titleMsg = QString("Genie A ") + tr("%1").arg(gAdditionalCfgParam.machineInfo.iMachineFlow);
        break;
    default:
        break;
    }
}

void DScreenSleepPage::buildTranslationTitleVWR()
{
    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
        m_titleMsg = QString("Super-Genie G ") + tr("%1").arg(gAdditionalCfgParam.machineInfo.iMachineFlow);
        break;
    case MACHINE_L_UP:
        m_titleMsg = QString("Super-Genie U ") + tr("%1").arg(gAdditionalCfgParam.machineInfo.iMachineFlow);
        break;
    case MACHINE_L_EDI_LOOP:
        m_titleMsg = QString("Super-Genie E ") + tr("%1").arg(gAdditionalCfgParam.machineInfo.iMachineFlow);
        break;
    case MACHINE_L_RO_LOOP:
        m_titleMsg = QString("Super-Genie R ") + tr("%1").arg(gAdditionalCfgParam.machineInfo.iMachineFlow);
        break;
    case MACHINE_Genie:
        m_titleMsg = QString("VWR G ") + tr("%1").arg(gAdditionalCfgParam.machineInfo.iMachineFlow);
        break;
    case MACHINE_UP:
        m_titleMsg = QString("VWR U ") + tr("%1").arg(gAdditionalCfgParam.machineInfo.iMachineFlow);
        break;
    case MACHINE_EDI:
        m_titleMsg = QString("VWR E ") + tr("%1").arg(gAdditionalCfgParam.machineInfo.iMachineFlow);
        break;
    case MACHINE_RO:
        m_titleMsg = QString("VWR R ") + tr("%1").arg(gAdditionalCfgParam.machineInfo.iMachineFlow);
        break;
    case MACHINE_PURIST:
        m_titleMsg = QString("VWR P");
        break;
    case MACHINE_ADAPT:
        m_titleMsg = QString("VWR A ") + tr("%1").arg(gAdditionalCfgParam.machineInfo.iMachineFlow);
        break;
    default:
        break;
    }
}


