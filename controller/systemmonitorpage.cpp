#include "mainwindow.h"

#include <QPainter>

#include <QScrollBar>

#include <QListWidgetItem>

#include "systemmonitorlistwidgtitem.h"
#include "systemmonitorpage.h"

SystemMonitorPage::SystemMonitorPage(QObject *parent,CBaseWidget *widget ,MainWindow *wndMain) : CSubPage(parent,widget,wndMain)
{
    int iIdx = 0;
    
    aIds[iIdx].iType = SYSMONI_LIST_ITEM_TRIPLE_TEXT;
    aIds[iIdx].iId   = SYSTEMPAGE_ITEM_NAME_DEVICE_STATE;
    aIds[iIdx].num   = 0XFF;
    iIdx++;

    aIds[iIdx].iType = SYSMONI_LIST_ITEM_TRIPLE_TEXT;
    aIds[iIdx].iId   = SYSTEMPAGE_ITEM_NAME_WORKING_STATE;
    aIds[iIdx].num   = 0XFF;
    iIdx++;
    
    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_PURIST:
        break;
    default:
        aIds[iIdx].iType = SYSMONI_LIST_ITEM_DOUBLE_SWITCH;
        aIds[iIdx].iId   = SYSTEMPAGE_ITEM_NAME_E1E2_STATE;
        aIds[iIdx].num   = 0XFF;
        iIdx++;
        break;
    }

    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_PURIST:
        aIds[iIdx].iType = SYSMONI_LIST_ITEM_SINGLE_SWITCH;
        aIds[iIdx].iId   = SYSTEMPAGE_ITEM_NAME_E3E4_STATE;
        aIds[iIdx].num   = 0X1;
        iIdx++;
        break;
        break;
    case MACHINE_ADAPT:
        aIds[iIdx].iType = SYSMONI_LIST_ITEM_SINGLE_SWITCH;
        aIds[iIdx].iId   = SYSTEMPAGE_ITEM_NAME_E3E4_STATE;
        aIds[iIdx].num   = 0X0;
        iIdx++;
        break;
    default:
        aIds[iIdx].iType = SYSMONI_LIST_ITEM_DOUBLE_SWITCH;
        aIds[iIdx].iId   = SYSTEMPAGE_ITEM_NAME_E3E4_STATE;
        aIds[iIdx].num   = 0XFF;
        iIdx++;
        break;

    }   

    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_Genie:
    case MACHINE_UP:
        aIds[iIdx].iType = SYSMONI_LIST_ITEM_DOUBLE_SWITCH;
        aIds[iIdx].iId   = SYSTEMPAGE_ITEM_NAME_E5E6_STATE;
        aIds[iIdx].num   = 0XFF;
        iIdx++;
        break;
    case MACHINE_L_EDI_LOOP:
    case MACHINE_L_RO_LOOP:
    case MACHINE_EDI:
    case MACHINE_RO:
        aIds[iIdx].iType = SYSMONI_LIST_ITEM_SINGLE_SWITCH;
        aIds[iIdx].iId   = SYSTEMPAGE_ITEM_NAME_E5E6_STATE;
        aIds[iIdx].num   = 0X1;
        iIdx++;
        break;
    case MACHINE_PURIST:
    case MACHINE_ADAPT:
        aIds[iIdx].iType = SYSMONI_LIST_ITEM_SINGLE_SWITCH;
        aIds[iIdx].iId   = SYSTEMPAGE_ITEM_NAME_E5E6_STATE;
        aIds[iIdx].num   = 0X0;
        iIdx++;
        break;
    }


    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
        aIds[iIdx].iType = SYSMONI_LIST_ITEM_DOUBLE_SWITCH;
        aIds[iIdx].iId   = SYSTEMPAGE_ITEM_NAME_E9E10_STATE;
        aIds[iIdx].num   = 0XFF;
        iIdx++;
        break;
    case MACHINE_L_EDI_LOOP:
    case MACHINE_L_RO_LOOP:
        aIds[iIdx].iType = SYSMONI_LIST_ITEM_SINGLE_SWITCH;
        aIds[iIdx].iId   = SYSTEMPAGE_ITEM_NAME_E9E10_STATE;
        aIds[iIdx].num   = 0X1;
        iIdx++;
        break;
    case MACHINE_Genie:
    case MACHINE_PURIST:
        aIds[iIdx].iType = SYSMONI_LIST_ITEM_SINGLE_SWITCH;
        aIds[iIdx].iId   = SYSTEMPAGE_ITEM_NAME_E9E10_STATE;
        aIds[iIdx].num   = 0X0;
        iIdx++;
        break;
    }

    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_L_EDI_LOOP:
    case MACHINE_L_RO_LOOP:
    case MACHINE_Genie:
        aIds[iIdx].iType = SYSMONI_LIST_ITEM_DOUBLE_SWITCH_VALUE;
        aIds[iIdx].iId   = SYSTEMPAGE_ITEM_NAME_N1N2_STATE;
        aIds[iIdx].num   = 0XFF;
        iIdx++;
        break;
    case MACHINE_L_UP:
    case MACHINE_UP:
    case MACHINE_PURIST:
    case MACHINE_ADAPT:
        aIds[iIdx].iType = SYSMONI_LIST_ITEM_SINGLE_SWITCH_VALUE;
        aIds[iIdx].iId   = SYSTEMPAGE_ITEM_NAME_N1N2_STATE;
        aIds[iIdx].num   = 0X1;
        iIdx++;
        break;
    case MACHINE_EDI:
        aIds[iIdx].iType = SYSMONI_LIST_ITEM_SINGLE_SWITCH_VALUE;
        aIds[iIdx].iId   = SYSTEMPAGE_ITEM_NAME_N1N2_STATE;
        aIds[iIdx].num   = 0X0;
        iIdx++;
        break;      
    }

    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_L_EDI_LOOP:
    case MACHINE_Genie:
    case MACHINE_EDI:
        aIds[iIdx].iType = SYSMONI_LIST_ITEM_DOUBLE_SWITCH_VALUE;
        aIds[iIdx].iId   = SYSTEMPAGE_ITEM_NAME_N3T1_STATE;
        aIds[iIdx].num   = 0XFF;
        iIdx++;
        break;
    case MACHINE_L_UP:
    case MACHINE_L_RO_LOOP:
    case MACHINE_UP:
    case MACHINE_RO:
    case MACHINE_PURIST:    
        aIds[iIdx].iType = SYSMONI_LIST_ITEM_SINGLE_SWITCH_VALUE;
        aIds[iIdx].iId   = SYSTEMPAGE_ITEM_NAME_N3T1_STATE;
        aIds[iIdx].num   = 0X0;
        iIdx++;
        break;      
    }

    

    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_L_EDI_LOOP:
    case MACHINE_L_RO_LOOP:
        aIds[iIdx].iType = SYSMONI_LIST_ITEM_DOUBLE_SWITCH_VALUE;
        aIds[iIdx].iId   = SYSTEMPAGE_ITEM_NAME_C3C4_STATE;
        aIds[iIdx].num   = 0XFF;
        iIdx++;
        break;
    }

    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_PURIST:
        break;
    default:
        aIds[iIdx].iType = SYSMONI_LIST_ITEM_SINGLE_SWITCH_DOUBLE_VALUE;
        aIds[iIdx].iId   = SYSTEMPAGE_ITEM_NAME_C1_STATE;
        aIds[iIdx].num   = 0XFF;
        iIdx++;
        break;
    }

    aIds[iIdx].iType = SYSMONI_LIST_ITEM_SINGLE_SWITCH_DOUBLE_VALUE;
    aIds[iIdx].iId   = SYSTEMPAGE_ITEM_NAME_C2_STATE;
    aIds[iIdx].num   = 0XFF;
    iIdx++;

    m_iRealNum = iIdx;

    creatTitle();

    initUi();

    buildTranslation();

    m_tankLevel = 0; //ex

}

void SystemMonitorPage::creatTitle()
{
    CSubPage::creatTitle();

    buildTitles();

    selectTitle(0);
}

void SystemMonitorPage::buildTitles()
{
    QStringList stringList;

    stringList << tr("Components Status");

    setTitles(stringList);

}

void SystemMonitorPage::buildTranslation()
{
    int iLoop;

    QString strName;

    for(iLoop = 0 ; iLoop < m_iRealNum ; iLoop++)
    {
        switch(aIds[iLoop].iId)
        {
        case SYSTEMPAGE_ITEM_NAME_DEVICE_STATE:
            strName = tr("Device Connect");
            m_apSysMonitorListItem[iLoop]->setName1(strName);
            break;
        case SYSTEMPAGE_ITEM_NAME_WORKING_STATE:
            strName = tr("Runing Status");
            m_apSysMonitorListItem[iLoop]->setName1(strName);
            break;
        case SYSTEMPAGE_ITEM_NAME_E1E2_STATE:
            strName = tr("RO Inlet Valve");
            m_apSysMonitorListItem[iLoop]->setName1(strName);

            strName = tr("RO Reject Valve");
            m_apSysMonitorListItem[iLoop]->setName2(strName);
            break;
        case SYSTEMPAGE_ITEM_NAME_E3E4_STATE:
            strName = tr("RO Drain Valve");
            m_apSysMonitorListItem[iLoop]->setName1(strName);

            strName = tr("HP Inlet Valve");
            m_apSysMonitorListItem[iLoop]->setName2(strName);
            break;
        case SYSTEMPAGE_ITEM_NAME_E5E6_STATE:
            strName = tr("UP Recir. Valve");
            m_apSysMonitorListItem[iLoop]->setName1(strName);

            strName = tr("HP Recir. Valve");
            m_apSysMonitorListItem[iLoop]->setName2(strName);
            break;
        case SYSTEMPAGE_ITEM_NAME_E9E10_STATE:
            strName = tr("TOC Flush Valve");
            m_apSysMonitorListItem[iLoop]->setName1(strName);

            strName = tr("Tap Inlet Valve");
            m_apSysMonitorListItem[iLoop]->setName2(strName);
            
            break;
        case SYSTEMPAGE_ITEM_NAME_N1N2_STATE:
            strName = tr("254 UV Lamp");
            m_apSysMonitorListItem[iLoop]->setName1(strName);

            strName = tr("185 UV Lamp");
            m_apSysMonitorListItem[iLoop]->setName2(strName);
            break;
        case SYSTEMPAGE_ITEM_NAME_N3T1_STATE:
            strName = tr("TANK UV Lamp");
            m_apSysMonitorListItem[iLoop]->setName1(strName);

            strName = tr("EDI Module");
            m_apSysMonitorListItem[iLoop]->setName2(strName);
            break;
        case SYSTEMPAGE_ITEM_NAME_C3C4_STATE:
            strName = tr("Feed Pump");
            m_apSysMonitorListItem[iLoop]->setName1(strName);

            strName = tr("Dist. Pump");
            m_apSysMonitorListItem[iLoop]->setName2(strName);
            break;
        case SYSTEMPAGE_ITEM_NAME_C1_STATE:
            strName = tr("RO Pump");
            m_apSysMonitorListItem[iLoop]->setName1(strName);
            break;
        case SYSTEMPAGE_ITEM_NAME_C2_STATE:
            strName = tr("Recir. Pump");
            m_apSysMonitorListItem[iLoop]->setName1(strName);
            break;
        }
    }

}

void SystemMonitorPage::switchLanguage()
{
    buildTranslation();

    buildTitles();

    selectTitle(titleIndex());
}

void SystemMonitorPage::update()
{
    int iLoop;

    m_ulSwitchMask = DispGetSwitchState(APP_EXE_SWITCHS_MASK);

    for(iLoop = 0 ; iLoop < m_iRealNum ; iLoop++)
    {
        switch(aIds[iLoop].iId)
        {
        case SYSTEMPAGE_ITEM_NAME_DEVICE_STATE:
            updateDeviceState();
            break;
        case SYSTEMPAGE_ITEM_NAME_WORKING_STATE:
            updateWorkState();
            break;
        case SYSTEMPAGE_ITEM_NAME_E1E2_STATE:
            m_apSysMonitorListItem[iLoop]->setSwitch1(m_ulSwitchMask & (1 << APP_EXE_E1_NO) ? true : false);
            m_apSysMonitorListItem[iLoop]->setSwitch2(m_ulSwitchMask & (1 << APP_EXE_E2_NO) ? true : false);
            break;
        case SYSTEMPAGE_ITEM_NAME_E3E4_STATE:
            m_apSysMonitorListItem[iLoop]->setSwitch1(m_ulSwitchMask & (1 << APP_EXE_E3_NO) ? true : false);
            m_apSysMonitorListItem[iLoop]->setSwitch2(m_ulSwitchMask & (1 << APP_EXE_E4_NO) ? true : false);
            break;
        case SYSTEMPAGE_ITEM_NAME_E5E6_STATE:
            m_apSysMonitorListItem[iLoop]->setSwitch1(m_ulSwitchMask & (1 << APP_EXE_E5_NO) ? true : false);
            m_apSysMonitorListItem[iLoop]->setSwitch2(m_ulSwitchMask & (1 << APP_EXE_E6_NO) ? true : false);
            break;
        case SYSTEMPAGE_ITEM_NAME_E9E10_STATE:
            m_apSysMonitorListItem[iLoop]->setSwitch1(m_ulSwitchMask & (1 << APP_EXE_E9_NO) ? true : false);
            m_apSysMonitorListItem[iLoop]->setSwitch2(m_ulSwitchMask & (1 << APP_EXE_E10_NO) ? true : false);
            break;
        case SYSTEMPAGE_ITEM_NAME_N1N2_STATE:
            {
                int iTmpData;
                QString strTemp;
                
                DispGetOtherCurrent(APP_EXE_N1_NO,&iTmpData);
                m_apSysMonitorListItem[iLoop]->setSwitch1(m_ulSwitchMask & (1 << APP_EXE_N1_NO) ? true : false);
                strTemp.sprintf("%d mA",iTmpData);
                m_apSysMonitorListItem[iLoop]->setValue1(strTemp);

                
                DispGetOtherCurrent(APP_EXE_N2_NO,&iTmpData);
                m_apSysMonitorListItem[iLoop]->setSwitch2(m_ulSwitchMask & (1 << APP_EXE_N2_NO) ? true : false);
                strTemp.sprintf("%d mA",iTmpData);
                m_apSysMonitorListItem[iLoop]->setValue2(strTemp);
            }
            break;
        case SYSTEMPAGE_ITEM_NAME_N3T1_STATE:
            {
                int iTmpData;
                QString strTemp;
                
                DispGetOtherCurrent(APP_EXE_N3_NO,&iTmpData);
                m_apSysMonitorListItem[iLoop]->setSwitch1(m_ulSwitchMask & (1 << APP_EXE_N3_NO) ? true : false);
                strTemp.sprintf("%d mA",iTmpData);
                m_apSysMonitorListItem[iLoop]->setValue1(strTemp);
                
                DispGetOtherCurrent(APP_EXE_T1_NO,&iTmpData);
                m_apSysMonitorListItem[iLoop]->setSwitch2(m_ulSwitchMask & (1 << APP_EXE_T1_NO) ? true : false);
                strTemp.sprintf("%d mA",iTmpData);
                m_apSysMonitorListItem[iLoop]->setValue2(strTemp);
            }
            break;
        case SYSTEMPAGE_ITEM_NAME_C3C4_STATE:
            {
                int iTmpData;
                QString strTemp;
                DispGetOtherCurrent(APP_EXE_C3_NO,&iTmpData);
                m_apSysMonitorListItem[iLoop]->setSwitch1(m_ulSwitchMask & (1 << APP_EXE_C3_NO) ? true : false);
                strTemp.sprintf("%d mA",iTmpData);
                m_apSysMonitorListItem[iLoop]->setValue1(strTemp);
                
                DispGetOtherCurrent(APP_EXE_C4_NO,&iTmpData);
                m_apSysMonitorListItem[iLoop]->setSwitch2(m_ulSwitchMask & (1 << APP_EXE_C4_NO) ? true : false);
                strTemp.sprintf("%d mA",iTmpData);
                m_apSysMonitorListItem[iLoop]->setValue2(strTemp);
            }
            break;
        case SYSTEMPAGE_ITEM_NAME_C1_STATE:
            {
                int iTmpDataI,iTmpDataV;
                QString strTemp;
                
                DispGetRPumpRti(APP_EXE_C1_NO,&iTmpDataV,&iTmpDataI);                
                m_apSysMonitorListItem[iLoop]->setSwitch1(m_ulSwitchMask & (1 << APP_EXE_C1_NO) ? true : false);
                strTemp.sprintf("%d mA",iTmpDataI);
                m_apSysMonitorListItem[iLoop]->setValue1(strTemp);

                strTemp.sprintf("%d V",iTmpDataV/1000);
                m_apSysMonitorListItem[iLoop]->setValue2(strTemp);
            }
            break;
        case SYSTEMPAGE_ITEM_NAME_C2_STATE:
            {
                int iTmpDataI,iTmpDataV;
                QString strTemp;

                DispGetRPumpRti(APP_EXE_C2_NO,&iTmpDataV,&iTmpDataI);                
                m_apSysMonitorListItem[iLoop]->setSwitch1(m_ulSwitchMask & (1 << APP_EXE_C2_NO) ? true : false);
                strTemp.sprintf("%d mA",iTmpDataI);
                m_apSysMonitorListItem[iLoop]->setValue1(strTemp);

                strTemp.sprintf("%d V",iTmpDataV/1000);
                m_apSysMonitorListItem[iLoop]->setValue2(strTemp);
            }
            break;
        }
    }
    {
        unsigned char buf[16];
       
        /* do something here */
        DISP_CMD_SWITCH_STATE_REPORT_STRU *cmd = (DISP_CMD_SWITCH_STATE_REPORT_STRU *)buf;
        cmd->iRptFlag = 1;
        DispCmdEntry(DISP_CMD_SWITCH_REPORT,buf,sizeof(DISP_CMD_SWITCH_STATE_REPORT_STRU));
    }        
}

void SystemMonitorPage::fade()
{
    unsigned char buf[16];
   
    /* do something here */
    DISP_CMD_SWITCH_STATE_REPORT_STRU *cmd = (DISP_CMD_SWITCH_STATE_REPORT_STRU *)buf;
    cmd->iRptFlag = 0;
    DispCmdEntry(DISP_CMD_SWITCH_REPORT,buf,sizeof(DISP_CMD_SWITCH_STATE_REPORT_STRU));

}

void SystemMonitorPage::createList()
{
    int iLoop;

    m_pListWidget = new QListWidget((QWidget *)m_widget);

    m_pListWidget->setStyleSheet("background-color:transparent");
    m_pListWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    m_pListWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_pListWidget->setFrameShape(QListWidget::NoFrame);

    m_pListWidget->setGeometry(QRect(0,55 ,SYSMONI_ITEM_WIDTH,535));


    for(iLoop = 0 ; iLoop < m_iRealNum ; iLoop++)
    {
        m_apListWidgetIem[iLoop] = new QListWidgetItem;
        m_apListWidgetIem[iLoop]->setSizeHint(QSize(SYSMONI_ITEM_WIDTH , SYSMONI_ITEM_HEIGHT));

        m_apSysMonitorListItem[iLoop] = new SystemMonitorListWidgtItem(NULL,aIds[iLoop].iType,aIds[iLoop].iId,aIds[iLoop].num);

//        m_apSysMonitorListItem[iLoop]->setSwitchImage(gpGlobalPixmaps[GLOBAL_BMP_SWITCH_ON],gpGlobalPixmaps[GLOBAL_BMP_SWITCH_OFF]);
         m_apSysMonitorListItem[iLoop]->setSwitchImage(gpGlobalPixmaps[GLOBAL_BMP_DEVICE_ON],gpGlobalPixmaps[GLOBAL_BMP_DEVICE_OFF]);

        m_pListWidget->insertItem(iLoop,m_apListWidgetIem[iLoop]);

        m_pListWidget->setItemWidget(m_apListWidgetIem[iLoop] , m_apSysMonitorListItem[iLoop]);

        connect(m_pListWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(ItemClicked(QListWidgetItem *)));
    }

}

void SystemMonitorPage::setBackColor()
{
    QSize size(width(),height());

    QImage image_bg = QImage(size, QImage::Format_ARGB32);

    QPainter p(&image_bg);

    p.fillRect(image_bg.rect(), QColor(228, 231, 240));

    QPalette pal(m_widget->palette());

    pal.setBrush(m_widget->backgroundRole(),QBrush(image_bg));

    m_widget->setAutoFillBackground(true);
    m_widget->setPalette(pal);
}

void SystemMonitorPage::initUi()
{
    setBackColor();
    createList();
}

void SystemMonitorPage::ItemClicked(QListWidgetItem * item)
{

}

void SystemMonitorPage::updateSwitchInfo()
{
   unsigned int ulTempMaks = DispGetSwitchState(APP_EXE_SWITCHS_MASK);

   unsigned int ulDelta = ulTempMaks ^ m_ulSwitchMask;

   qDebug() << "updateSwitchInfo" << ulDelta << m_ulSwitchMask;

   if (ulDelta)
   {
        int iLoop;

        m_ulSwitchMask = ulTempMaks;
        
        for(iLoop = 0 ; iLoop < m_iRealNum ; iLoop++)
        {
            switch(aIds[iLoop].iId)
            {
            case SYSTEMPAGE_ITEM_NAME_E1E2_STATE:
                if (ulDelta & (1 << APP_EXE_E1_NO))
                {
                    m_apSysMonitorListItem[iLoop]->setSwitch1(m_ulSwitchMask & (1 << APP_EXE_E1_NO) ? true : false);
                }

                if (ulDelta & (1 << APP_EXE_E2_NO))
                {
                    m_apSysMonitorListItem[iLoop]->setSwitch2(m_ulSwitchMask & (1 << APP_EXE_E2_NO) ? true : false);
                }
                break;
            case SYSTEMPAGE_ITEM_NAME_E3E4_STATE:
                if (ulDelta & (1 << APP_EXE_E3_NO)) m_apSysMonitorListItem[iLoop]->setSwitch1(m_ulSwitchMask & (1 << APP_EXE_E3_NO) ? true : false);
                if (ulDelta & (1 << APP_EXE_E4_NO)) m_apSysMonitorListItem[iLoop]->setSwitch2(m_ulSwitchMask & (1 << APP_EXE_E4_NO) ? true : false);
                break;
            case SYSTEMPAGE_ITEM_NAME_E5E6_STATE:
                if (ulDelta & (1 << APP_EXE_E5_NO)) m_apSysMonitorListItem[iLoop]->setSwitch1(m_ulSwitchMask & (1 << APP_EXE_E5_NO) ? true : false);
                if (ulDelta & (1 << APP_EXE_E6_NO)) m_apSysMonitorListItem[iLoop]->setSwitch2(m_ulSwitchMask & (1 << APP_EXE_E6_NO) ? true : false);
                break;
            case SYSTEMPAGE_ITEM_NAME_E9E10_STATE:
                if (ulDelta & (1 << APP_EXE_E9_NO)) m_apSysMonitorListItem[iLoop]->setSwitch1(m_ulSwitchMask & (1 << APP_EXE_E9_NO) ? true : false);
                if (ulDelta & (1 << APP_EXE_E10_NO)) m_apSysMonitorListItem[iLoop]->setSwitch2(m_ulSwitchMask & (1 << APP_EXE_E10_NO) ? true : false);
                break;
            case SYSTEMPAGE_ITEM_NAME_N1N2_STATE:
                {
                    int iTmpData;
                    QString strTemp;

                    if (ulDelta & (1 << APP_EXE_N1_NO)) 
                    {
                        DispGetOtherCurrent(APP_EXE_N1_NO,&iTmpData);
                        m_apSysMonitorListItem[iLoop]->setSwitch1(m_ulSwitchMask & (1 << APP_EXE_N1_NO) ? true : false);
                        strTemp.sprintf("%d mA",iTmpData);
                        m_apSysMonitorListItem[iLoop]->setValue1(strTemp);
                    }
    
                    
                    if (ulDelta & (1 << APP_EXE_N2_NO))
                    {
                        DispGetOtherCurrent(APP_EXE_N2_NO,&iTmpData);
                        m_apSysMonitorListItem[iLoop]->setSwitch2(m_ulSwitchMask & (1 << APP_EXE_N2_NO) ? true : false);
                        strTemp.sprintf("%d mA",iTmpData);
                        m_apSysMonitorListItem[iLoop]->setValue2(strTemp);
                    }
                }
                break;
            case SYSTEMPAGE_ITEM_NAME_N3T1_STATE:
                {
                    int iTmpData;
                    QString strTemp;
                    
                    if (ulDelta & (1 << APP_EXE_N3_NO)) 
                    {
                        DispGetOtherCurrent(APP_EXE_N3_NO,&iTmpData);
                        m_apSysMonitorListItem[iLoop]->setSwitch1(m_ulSwitchMask & (1 << APP_EXE_N3_NO) ? true : false);
                        strTemp.sprintf("%d mA",iTmpData);
                        m_apSysMonitorListItem[iLoop]->setValue1(strTemp);
                    }
                    
                    if (ulDelta & (1 << APP_EXE_T1_NO)) 
                    {
                        DispGetOtherCurrent(APP_EXE_T1_NO,&iTmpData);
                        m_apSysMonitorListItem[iLoop]->setSwitch2(m_ulSwitchMask & (1 << APP_EXE_T1_NO) ? true : false);
                        strTemp.sprintf("%d mA",iTmpData);
                        m_apSysMonitorListItem[iLoop]->setValue2(strTemp);
                    }
                }
                break;
            case SYSTEMPAGE_ITEM_NAME_C3C4_STATE:
                {
                    int iTmpData;
                    QString strTemp;
                    
                    if (ulDelta & (1 << APP_EXE_C3_NO)) 
                    {
                        DispGetOtherCurrent(APP_EXE_C3_NO,&iTmpData);
                        m_apSysMonitorListItem[iLoop]->setSwitch1(m_ulSwitchMask & (1 << APP_EXE_C3_NO) ? true : false);
                        strTemp.sprintf("%d mA",iTmpData);
                        m_apSysMonitorListItem[iLoop]->setValue1(strTemp);
                    }
                    
                    if (ulDelta & (1 << APP_EXE_C4_NO)) 
                    {
                        DispGetOtherCurrent(APP_EXE_C4_NO,&iTmpData);
                        m_apSysMonitorListItem[iLoop]->setSwitch2(m_ulSwitchMask & (1 << APP_EXE_C4_NO) ? true : false);
                        strTemp.sprintf("%d mA",iTmpData);
                        m_apSysMonitorListItem[iLoop]->setValue2(strTemp);
                    }
                }
                break;
            case SYSTEMPAGE_ITEM_NAME_C1_STATE:
                {
                    int iTmpDataI,iTmpDataV;
                    QString strTemp;
                    
                    if (ulDelta & (1 << APP_EXE_C1_NO)) 
                    {
                        DispGetRPumpRti(APP_EXE_C1_NO,&iTmpDataV,&iTmpDataI);                
                        m_apSysMonitorListItem[iLoop]->setSwitch1(m_ulSwitchMask & (1 << APP_EXE_C1_NO) ? true : false);
                        strTemp.sprintf("%d mA",iTmpDataI);
                        m_apSysMonitorListItem[iLoop]->setValue1(strTemp);
        
                        strTemp.sprintf("%d V",iTmpDataV/1000);
                        m_apSysMonitorListItem[iLoop]->setValue2(strTemp);
                    }
                }
                break;
            case SYSTEMPAGE_ITEM_NAME_C2_STATE:
                {
                    int iTmpDataI,iTmpDataV;
                    QString strTemp;

    
                    if (ulDelta & (1 << APP_EXE_C2_NO)) 
                    {
                        DispGetRPumpRti(APP_EXE_C2_NO,&iTmpDataV,&iTmpDataI);                
                        m_apSysMonitorListItem[iLoop]->setSwitch1(m_ulSwitchMask & (1 << APP_EXE_C2_NO) ? true : false);
                        strTemp.sprintf("%d mA",iTmpDataI);
                        m_apSysMonitorListItem[iLoop]->setValue1(strTemp);
        
                        strTemp.sprintf("%d V",iTmpDataV/1000);
                        m_apSysMonitorListItem[iLoop]->setValue2(strTemp);
                    }
                }
                break;
            }
        }   
   }
}

void SystemMonitorPage::updateRpumpInfo(int iChl)
{
    int iLoop;

    unsigned int ulSwitchMask = DispGetSwitchState(APP_EXE_SWITCHS_MASK);

    for(iLoop = 0 ; iLoop < m_iRealNum ; iLoop++)
    {
        switch(aIds[iLoop].iId)
        {
        case SYSTEMPAGE_ITEM_NAME_C1_STATE:
            {
                int iTmpDataI,iTmpDataV;
                QString strTemp;
                
                if (0 == iChl ) 
                {
                    DispGetRPumpRti(APP_EXE_C1_NO,&iTmpDataV,&iTmpDataI);                
                    m_apSysMonitorListItem[iLoop]->setSwitch1(ulSwitchMask & (1 << APP_EXE_C1_NO) ? true : false);
                    strTemp.sprintf("%d mA",iTmpDataI);
                    m_apSysMonitorListItem[iLoop]->setValue1(strTemp);
    
                    strTemp.sprintf("%d V",iTmpDataV/1000);
                    m_apSysMonitorListItem[iLoop]->setValue2(strTemp);
                }
            }
            break;
        case SYSTEMPAGE_ITEM_NAME_C2_STATE:
            {
                int iTmpDataI,iTmpDataV;
                QString strTemp;

                if (1 == iChl ) 
                {
                    DispGetRPumpRti(APP_EXE_C2_NO,&iTmpDataV,&iTmpDataI);                
                    m_apSysMonitorListItem[iLoop]->setSwitch1(ulSwitchMask & (1 << APP_EXE_C2_NO) ? true : false);
                    strTemp.sprintf("%d mA",iTmpDataI);
                    m_apSysMonitorListItem[iLoop]->setValue1(strTemp);
    
                    strTemp.sprintf("%d V",iTmpDataV/1000);
                    m_apSysMonitorListItem[iLoop]->setValue2(strTemp);
                }
            }
            break;
        }
    }   
}

void SystemMonitorPage::updateGpumpInfo(int iChl)
{
    int iLoop;

    for(iLoop = 0 ; iLoop < m_iRealNum ; iLoop++)
    {
        switch(aIds[iLoop].iId)
        {
        case SYSTEMPAGE_ITEM_NAME_C3C4_STATE:
            {
                int iTmpData;
                QString strTemp;

                if ( 0 == iChl ) 
                {
                    DispGetOtherCurrent(APP_EXE_C3_NO,&iTmpData);
                    strTemp.sprintf("%d mA",iTmpData);
                    m_apSysMonitorListItem[iLoop]->setValue1(strTemp);
                }
                else if ( 1 == iChl )
                {
                    DispGetOtherCurrent(APP_EXE_C4_NO,&iTmpData);
                    strTemp.sprintf("%d mA",iTmpData);
                    m_apSysMonitorListItem[iLoop]->setValue2(strTemp);
                }
            }
            break;

        }
    }   

}


void SystemMonitorPage::updateRectInfo(int iChl)
{
    int iLoop;

    for(iLoop = 0 ; iLoop < m_iRealNum ; iLoop++)
    {
        switch(aIds[iLoop].iId)
        {
        case SYSTEMPAGE_ITEM_NAME_N1N2_STATE:
            {
                int iTmpData;
                QString strTemp;

                if ( 0 == iChl ) 
                {
                    DispGetOtherCurrent(APP_EXE_N1_NO,&iTmpData);
                    strTemp.sprintf("%d mA",iTmpData);
                    m_apSysMonitorListItem[iLoop]->setValue1(strTemp);
                }
                else if ( 1 == iChl )
                {
                    DispGetOtherCurrent(APP_EXE_N2_NO,&iTmpData);
                    strTemp.sprintf("%d mA",iTmpData);
                    m_apSysMonitorListItem[iLoop]->setValue2(strTemp);
                }
            }
            break;
        case SYSTEMPAGE_ITEM_NAME_N3T1_STATE:
            {
                int iTmpData;
                QString strTemp;

                if ( 2 == iChl ) 
                {
                    DispGetOtherCurrent(APP_EXE_N3_NO,&iTmpData);
                    strTemp.sprintf("%d mA",iTmpData);
                    m_apSysMonitorListItem[iLoop]->setValue1(strTemp);
                }
            }
            break;
        }
    }   

}

void SystemMonitorPage::updateEdiInfo(int iChl)
{
    int iLoop;

    for(iLoop = 0 ; iLoop < m_iRealNum ; iLoop++)
    {
        switch(aIds[iLoop].iId)
        {
        case SYSTEMPAGE_ITEM_NAME_N3T1_STATE:
            {
                int iTmpData;
                QString strTemp;

                if ( 0 == iChl ) 
                {
                    DispGetOtherCurrent(APP_EXE_T1_NO,&iTmpData);
                    strTemp.sprintf("%d mA",iTmpData);
                    m_apSysMonitorListItem[iLoop]->setValue2(strTemp);
                }
            }
            break;

        }
    }   

}

void SystemMonitorPage::updateWorkState()
{
   QString strInfo1 = "UP:  ";
   QString strInfo2 = "HP:  ";
   QString strInfo3;

   //ex
    if((m_tankLevel >= B2_FULL)
        && (gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_HaveB2)))
    {
        strInfo3 = tr("Tank Full");
    }

   //end

   switch(DispGetWorkState())
   {
   case DISP_WORK_STATE_IDLE:
        if (DispGetFwFlag())
        {
            strInfo3 = tr("Fill Feed Tank");
        }         
        break;
   case DISP_WORK_STATE_PREPARE:
       if(NOT_RUNING_STATE_FLUSH == DispGetRunningStateFlag())
       {
           strInfo2 += tr("Flush");
       }
       break;
   case DISP_WORK_STATE_RUN:
        if(NOT_RUNING_STATE_CLEAN == DispGetRunningStateFlag())
        {
            strInfo2 += tr("Rinsing");
        }
        if (DispGetPwFlag()
            && (gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_HaveB2)))
        {
            strInfo3 = tr("Filling");
        }   
      
        break;
   case DISP_WORK_STATE_LPP:
        strInfo3 = tr("LPP");
        break;
   case DISP_WORK_STATE_KP:
        strInfo3 = tr("KP");
        break;
   }
   
   switch(DispGetWorkState4Pw())
   {
   case DISP_WORK_STATE_IDLE:
        strInfo1 +=  tr("Standby");
        strInfo2 +=  tr("Standby");
        break;
   case DISP_WORK_STATE_RUN:
       if (DispGetUpQtwFlag())
       {
            strInfo1 +=  tr("Dispense");
       }
       else if (DispGetUpCirFlag())
       {
           strInfo1 +=  tr("Recirculation");
       }
       else 
       {
           strInfo1 +=  tr("Ready");
       }
       
       if(NOT_RUNING_STATE_NONE == DispGetRunningStateFlag())
       {
           if (MACHINE_FUNCTION_EDI == (MACHINE_FUNCTION_EDI & gaMachineType[gGlobalParam.iMachineType].iFunctions))
           {
               if (DispGetEdiQtwFlag())
               {
                   strInfo2 +=  tr("Dispense");
               }
               else 
               {
                   strInfo2 +=  tr("Ready");
               }
            }
       }
       
       if (DispGetTankCirFlag())
       {
           strInfo3 = tr("Recirculation");
       }             
       break;
   }
   
   if (MACHINE_FUNCTION_UP == (MACHINE_FUNCTION_UP & gaMachineType[gGlobalParam.iMachineType].iFunctions))
   {
       m_apSysMonitorListItem[1]->setValue1(strInfo1);
   }
   if (MACHINE_FUNCTION_EDI == (MACHINE_FUNCTION_EDI & gaMachineType[gGlobalParam.iMachineType].iFunctions))
   {
      m_apSysMonitorListItem[1]->setValue2(strInfo2);
   }
   m_apSysMonitorListItem[1]->setValue3(strInfo3);
}

void SystemMonitorPage::updateDeviceState()
{
    QString strInfo1;
    QString strInfo2;
    QString strInfo3;

    if (m_wndMain->getActiveExeBrds()
        && m_wndMain->getActiveFmBrds())
    {
       strInfo1 = tr("Excution Board OK");
    }
    else
    {
       strInfo1 = tr("Excution Board Fault");
    }

    
    if (m_wndMain->getActiveRfidBrds())
     {
        strInfo2 = tr("RFID OK");
     }
     else
     {
        strInfo2 = tr("RFID Fault");
     }

     switch(m_wndMain->getActiveHandlerBrds())
     {
         case 1:
         {
             strInfo3 = tr("Dispenser OK");
             break;
         }
         case 2:
         {
              strInfo3 = tr("Dispenser Partial");
              break;
          }
          default:
          {
             strInfo3 = tr("Dispenser Fault");
             break;
          }
     }
     
     m_apSysMonitorListItem[0]->setValue1(strInfo1);
     m_apSysMonitorListItem[0]->setValue2(strInfo2);
     m_apSysMonitorListItem[0]->setValue3(strInfo3);
}

void SystemMonitorPage::updateTankLevel(int level)
{
    m_tankLevel = level;
}

