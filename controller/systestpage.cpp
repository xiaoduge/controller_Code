#include "systestpage.h"

#include "titlebar.h"

#include "mainwindow.h"

#include <QPainter>

#include <QScrollBar>

#include <QListWidgetItem>
#include "Display.h"
#include "notify.h"

SysTestPage::SysTestPage(QObject *parent,CBaseWidget *widget ,MainWindow *wndMain) : CSubPage(parent,widget,wndMain)
{
    int iIdx = 0;

    for (iIdx = 0; iIdx < DISP_NOT_NUM; iIdx++)
    {
        m_aiIndex[iIdx] = -1;
    }

    iIdx = 0;

    switch(gGlobalParam.iMachineType)/*进水阀ON/OFF*/
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_L_EDI_LOOP:
    case MACHINE_L_RO_LOOP:
    case MACHINE_Genie:
    case MACHINE_UP:
    case MACHINE_EDI:
    case MACHINE_RO:
    case MACHINE_ADAPT:
        aSwitchs[iIdx].type = SYSTEM_TEST_ITEM_TYPE_SWITCH;
        aSwitchs[iIdx].id   = APP_EXE_E1_NO;            
        iIdx++;
        break;
    }

    switch(gGlobalParam.iMachineType)/*RO冲洗阀ON/OFF*/
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_L_EDI_LOOP:
    case MACHINE_L_RO_LOOP:
    case MACHINE_Genie:
    case MACHINE_UP:
    case MACHINE_EDI:
    case MACHINE_RO:
    case MACHINE_ADAPT:
        aSwitchs[iIdx].type = SYSTEM_TEST_ITEM_TYPE_SWITCH;
        aSwitchs[iIdx].id   = APP_EXE_E2_NO;            
        iIdx++;
        break;
    }

    switch(gGlobalParam.iMachineType)/*RO产水阀ON/OFF*/
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_L_EDI_LOOP:
    case MACHINE_L_RO_LOOP:
    case MACHINE_Genie:
    case MACHINE_UP:
    case MACHINE_EDI:
    case MACHINE_RO:
    case MACHINE_ADAPT:
        aSwitchs[iIdx].type = SYSTEM_TEST_ITEM_TYPE_SWITCH;
        aSwitchs[iIdx].id   = APP_EXE_E3_NO;            
        iIdx++;
        break;
    }

    switch(gGlobalParam.iMachineType)/*纯水进水电磁阀ON/OFF*/
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_L_EDI_LOOP:
    case MACHINE_L_RO_LOOP:
    case MACHINE_Genie:
    case MACHINE_UP:
    case MACHINE_EDI:
    case MACHINE_RO:
    case MACHINE_PURIST:
        aSwitchs[iIdx].type = SYSTEM_TEST_ITEM_TYPE_SWITCH;
        aSwitchs[iIdx].id   = APP_EXE_E4_NO;            
        iIdx++;
        break;
    }

    switch(gGlobalParam.iMachineType)/*UP循环电磁阀ON/OFF*/
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_Genie:
    case MACHINE_UP:
    case MACHINE_PURIST:
    case MACHINE_ADAPT:
        aSwitchs[iIdx].type = SYSTEM_TEST_ITEM_TYPE_SWITCH;
        aSwitchs[iIdx].id   = APP_EXE_E5_NO;            
        iIdx++;
        break;
    }    

    switch(gGlobalParam.iMachineType)/*HP循环电磁阀 ON/OFF*/
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_L_EDI_LOOP:
    case MACHINE_L_RO_LOOP:
    case MACHINE_Genie:
    case MACHINE_UP:
    case MACHINE_EDI:
    case MACHINE_RO:
        aSwitchs[iIdx].type = SYSTEM_TEST_ITEM_TYPE_SWITCH;
        aSwitchs[iIdx].id   = APP_EXE_E6_NO;            
        iIdx++;
        break;
    } 

    switch(gGlobalParam.iMachineType)/*TOC循环阀ON/OFF*/
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_L_EDI_LOOP:
    case MACHINE_L_RO_LOOP:
    case MACHINE_Genie:
    case MACHINE_UP:
    //case MACHINE_PURIST:
    //case MACHINE_ADAPT:
        aSwitchs[iIdx].type = SYSTEM_TEST_ITEM_TYPE_SWITCH;
        aSwitchs[iIdx].id   = APP_EXE_E9_NO;            
        iIdx++;
        break;
    } 

    switch(gGlobalParam.iMachineType)/*原水阀ON/OFF*/
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_L_EDI_LOOP:
    case MACHINE_L_RO_LOOP:
    case MACHINE_Genie:
    case MACHINE_UP:
    case MACHINE_EDI:
    case MACHINE_RO:
    case MACHINE_ADAPT:
        aSwitchs[iIdx].type = SYSTEM_TEST_ITEM_TYPE_SWITCH;
        aSwitchs[iIdx].id   = APP_EXE_E10_NO;            
        iIdx++;
        break;
    } 

#if 0
    switch(gGlobalParam.iMachineType) /*E7 ON/OFF*/
    {
    case MACHINE_L_EDI_LOOP:
    case MACHINE_L_RO_LOOP:
    case MACHINE_EDI:
    case MACHINE_RO:
        break;
    default:
        aSwitchs[iIdx].type = SYSTEM_TEST_ITEM_TYPE_HO_SWITCH;
        aSwitchs[iIdx].id   = APP_EXE_HO_E7_NO;            
        iIdx++;
        break;
    }

    switch(gGlobalParam.iMachineType)/*E8 ON/OFF*/
    {
    case MACHINE_PURIST:
        break;
    default:
        aSwitchs[iIdx].type = SYSTEM_TEST_ITEM_TYPE_HO_SWITCH;
        aSwitchs[iIdx].id   = APP_EXE_HO_E8_NO;            
        iIdx++;
        break;
    }
#endif

    switch(gGlobalParam.iMachineType)/*C3原水增压泵*/
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_L_EDI_LOOP:
    case MACHINE_L_RO_LOOP:
    case MACHINE_Genie:
    case MACHINE_UP:
    case MACHINE_EDI:
    case MACHINE_RO:
    case MACHINE_ADAPT:
        aSwitchs[iIdx].type = SYSTEM_TEST_ITEM_TYPE_SWITCH_AND_VALUE;
        aSwitchs[iIdx].id   = APP_EXE_C3_NO;            
        m_aiIndex[DISP_NOT_GPUMP] = iIdx;
        iIdx++;
        break;
    } 

    switch(gGlobalParam.iMachineType)/*C4管路循环泵 管路UV*/
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_L_EDI_LOOP:
    case MACHINE_L_RO_LOOP:
        aSwitchs[iIdx].type = SYSTEM_TEST_ITEM_TYPE_SWITCH_AND_VALUE;
        aSwitchs[iIdx].id   = APP_EXE_C4_NO;   
        if (-1 == m_aiIndex[DISP_NOT_GPUMP])
        {
            m_aiIndex[DISP_NOT_GPUMP] = iIdx - (APP_EXE_C4_NO - APP_EXE_C3_NO);
        }
        iIdx++;
        break;
    } 

    switch(gGlobalParam.iMachineType)/*254UV   ON/OFF   电流：000*/
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_L_EDI_LOOP:
    case MACHINE_L_RO_LOOP:
    case MACHINE_Genie:
    case MACHINE_UP:
    case MACHINE_EDI:
    case MACHINE_RO:
        aSwitchs[iIdx].type = SYSTEM_TEST_ITEM_TYPE_SWITCH_AND_VALUE;
        aSwitchs[iIdx].id   = APP_EXE_N1_NO;            
        m_aiIndex[DISP_NOT_RECT] = iIdx;
        iIdx++;
        break;
    } 
    switch(gGlobalParam.iMachineType)/*185UV   ON/OFF   电流：000*/
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_L_EDI_LOOP:
    case MACHINE_L_RO_LOOP:
    case MACHINE_Genie:
    case MACHINE_UP:
    case MACHINE_PURIST:
    case MACHINE_ADAPT:
        aSwitchs[iIdx].type = SYSTEM_TEST_ITEM_TYPE_SWITCH_AND_VALUE;
        aSwitchs[iIdx].id   = APP_EXE_N2_NO; 
        if (-1 == m_aiIndex[DISP_NOT_RECT])
        {
            m_aiIndex[DISP_NOT_RECT] = iIdx - (APP_EXE_N2_NO - APP_EXE_N1_NO);
        }
        iIdx++;
        break;
    } 

    switch(gGlobalParam.iMachineType)/*水箱UV  ON/OFF   电流：000*/
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_L_EDI_LOOP:
    case MACHINE_L_RO_LOOP:
    case MACHINE_Genie:
    case MACHINE_UP:
    case MACHINE_EDI:
    case MACHINE_RO:
        aSwitchs[iIdx].type = SYSTEM_TEST_ITEM_TYPE_SWITCH_AND_VALUE;
        aSwitchs[iIdx].id   = APP_EXE_N3_NO;            
        if (-1 == m_aiIndex[DISP_NOT_RECT])
        {
            m_aiIndex[DISP_NOT_RECT] = iIdx - (APP_EXE_N3_NO - APP_EXE_N1_NO);
        }
        iIdx++;
        break;
    } 

    switch(gGlobalParam.iMachineType)/*EDI 电源  ON/OFF   电流：000*/
    {
    case MACHINE_L_Genie:
    case MACHINE_L_EDI_LOOP:
    case MACHINE_Genie:
    case MACHINE_EDI:
        aSwitchs[iIdx].type = SYSTEM_TEST_ITEM_TYPE_SWITCH_AND_VALUE;
        aSwitchs[iIdx].id   = APP_EXE_T1_NO;            
        m_aiIndex[DISP_NOT_EDI] = iIdx;
        iIdx++;
        break;
    } 

    switch(gGlobalParam.iMachineType)/*RO泵 OFF/低/中/高 电压：000电流：000*/
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_L_EDI_LOOP:
    case MACHINE_L_RO_LOOP:
    case MACHINE_Genie:
    case MACHINE_UP:
    case MACHINE_EDI:
    case MACHINE_RO:
    case MACHINE_ADAPT:
        aSwitchs[iIdx].type = SYSTEM_TEST_ITEM_TYPE_RADIO_AND_VALUE;
        aSwitchs[iIdx].id   = APP_EXE_C1_NO;            
        m_aiIndex[DISP_NOT_RPUMP] = iIdx;
        iIdx++;
        break;
    } 

    switch(gGlobalParam.iMachineType)/*UP/HP泵 OFF/低/中/高 电压：000电流：000*/
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_L_EDI_LOOP:
    case MACHINE_L_RO_LOOP:
    case MACHINE_Genie:
    case MACHINE_UP:
    case MACHINE_EDI:
    case MACHINE_RO:
    case MACHINE_PURIST:
    case MACHINE_ADAPT:
        aSwitchs[iIdx].type = SYSTEM_TEST_ITEM_TYPE_RADIO_AND_VALUE;
        aSwitchs[iIdx].id   = APP_EXE_C2_NO;            
        if (-1 == m_aiIndex[DISP_NOT_RPUMP])
        {
            m_aiIndex[DISP_NOT_RPUMP] = iIdx - (APP_EXE_C2_NO - APP_EXE_C1_NO);
        }
        iIdx++;
        break;
    }

    m_iRealItems = iIdx;

    creatTitle();

    initUi();

    buildTranslation();

}

void SysTestPage::creatTitle()
{
    CSubPage::creatTitle();

    buildTitles();

    selectTitle(0);
}

void SysTestPage::buildTitles()
{
    QStringList stringList;

    stringList << tr("Test");

    setTitles(stringList);

}

void SysTestPage::buildTranslation()
{
    int iLoop;

    QStringList strList;
    strList << tr("OFF");
    strList << tr("Low");
    strList << tr("Middle");
    strList << tr("High");
    
    for (iLoop = 0; iLoop < m_iRealItems; iLoop++)
    {
        switch(aSwitchs[iLoop].id)
        {
        case APP_EXE_E1_NO:
            /*
            E1  RO进水电磁阀     ON/OFF
            */
            m_pSysTestlistItem[iLoop]->setName(tr("RO Inlet Valve"));
            break;
        case APP_EXE_E2_NO:
            /*
            RO冲洗阀  ON/OFF
            */
            m_pSysTestlistItem[iLoop]->setName(tr("RO Reject Valve"));
            break;
        case APP_EXE_E3_NO:
            /*
            RO产水阀  ON/OFF
            */
            m_pSysTestlistItem[iLoop]->setName(tr("RO Drain Valve"));
            break;
        case APP_EXE_E4_NO:
            /*
            纯水进水电磁阀ON/OFF
            */
            switch(gGlobalParam.iMachineType)
             {
             case MACHINE_L_Genie:
             case MACHINE_L_UP:
             case MACHINE_Genie:
             case MACHINE_UP:
             case MACHINE_PURIST:
             case MACHINE_ADAPT:
                 m_pSysTestlistItem[iLoop]->setName(tr("UP Inlet Valve"));
                 break;
             case MACHINE_L_EDI_LOOP: 
             case MACHINE_L_RO_LOOP:
             case MACHINE_EDI:
             case MACHINE_RO:
                 m_pSysTestlistItem[iLoop]->setName(tr("HP Inlet Valve"));
                 break;
             }
            break;
        case APP_EXE_E5_NO:
            /*
            UP循环电磁阀ON/OFF
            */
            m_pSysTestlistItem[iLoop]->setName(tr("UP Recirculation Valve"));
            break;
        case APP_EXE_E6_NO:
            /*
            HP循环电磁阀 ON/OFF
            */
            m_pSysTestlistItem[iLoop]->setName(tr("HP Recirculation Valve"));
            break;
        case APP_EXE_E9_NO:
            /*
            TOC循环阀ON/OFF
            */
            m_pSysTestlistItem[iLoop]->setName(tr("TOC Flush Valve"));
            
            break;
        case APP_EXE_E10_NO:
            /*
            原水阀 ON/OFF
            */
            m_pSysTestlistItem[iLoop]->setName(tr("Feed Water Valve"));
            break;
        case APP_EXE_C3_NO:
            /*
            C3  原水增压泵  
            */
            m_pSysTestlistItem[iLoop]->setName(tr("Feed Pump"));
            m_pSysTestlistItem[iLoop]->setP1Name(tr("Current"));
            
            break;
        case APP_EXE_C4_NO:
            /*
            C4  管路循环泵  
            */
            m_pSysTestlistItem[iLoop]->setName(tr("Dist. Pump"));
            m_pSysTestlistItem[iLoop]->setP1Name(tr("Current"));
            break;             
        case APP_EXE_N1_NO:
            /*
            254UV   ON/OFF   电流：000
            */
            m_pSysTestlistItem[iLoop]->setName(tr("254 UV Lamp"));
            m_pSysTestlistItem[iLoop]->setP1Name(tr("Current"));
            break;
        case APP_EXE_N2_NO:
            /*
            185UV   ON/OFF   电流：000
            */
            m_pSysTestlistItem[iLoop]->setName(tr("185 UV Lamp"));
            m_pSysTestlistItem[iLoop]->setP1Name(tr("Current"));
            break;
        case APP_EXE_N3_NO:
            /*
            水箱UV  ON/OFF   电流：000
            */
            m_pSysTestlistItem[iLoop]->setName(tr("Tank UV Lamp"));
            m_pSysTestlistItem[iLoop]->setP1Name(tr("Current"));
            break;
        case APP_EXE_T1_NO:
            /*
            EDI 电源   ON/OFF   电流：000
            */
            m_pSysTestlistItem[iLoop]->setName(tr("EDI Power"));
            m_pSysTestlistItem[iLoop]->setP1Name(tr("Current"));
            break;
           
        case APP_EXE_C1_NO:
            /*
            RO泵     OFF/低/中/高         电压：000电流：000          
            */
            m_pSysTestlistItem[iLoop]->setName(tr("RO Pump"));
            m_pSysTestlistItem[iLoop]->setP1Name(tr("Current"));
            m_pSysTestlistItem[iLoop]->setP2Name(tr("Voltage"));
            m_pSysTestlistItem[iLoop]->setRadioName(strList);
            
            break;
        case APP_EXE_C2_NO:
            /*
            UP泵      OFF/低/中/高        电压：000电流：000                    
            */
            switch(gGlobalParam.iMachineType)
             {
             case MACHINE_L_Genie:
             case MACHINE_L_UP:
             case MACHINE_Genie:
             case MACHINE_UP:
             case MACHINE_PURIST:
             case MACHINE_ADAPT:
                 m_pSysTestlistItem[iLoop]->setName(tr("UP Pump"));
                 break;
             case MACHINE_L_EDI_LOOP: 
             case MACHINE_L_RO_LOOP:
             case MACHINE_EDI:
             case MACHINE_RO:
                 m_pSysTestlistItem[iLoop]->setName(tr("HP Pump"));
                 break;
             }
            m_pSysTestlistItem[iLoop]->setP1Name(tr("Current"));
            m_pSysTestlistItem[iLoop]->setP2Name(tr("Voltage"));
            m_pSysTestlistItem[iLoop]->setRadioName(strList);  
            break;
        case APP_EXE_HO_E7_NO:
            m_pSysTestlistItem[iLoop]->setName(tr("UP Product Valve"));
            break;
        case APP_EXE_HO_E8_NO:
            m_pSysTestlistItem[iLoop]->setName(tr("HP Product Valve"));
            break;
        }
    }

#if 0

    /*
    UP产水阀ON/OFF
    */
    m_pSysTestlistItem[4]->setName(tr("UP Out Valve"));
    
    /*
    原水泵  ON/OFF
    */
    m_pSysTestlistItem[5]->setName(tr("Source Pump"));
    
    
    /*
    TOC循环阀ON/OFF
    */
    m_pSysTestlistItem[7]->setName(tr("TOC Cir Valve"));
    
    /*
    HP产水阀ON/OFF
    */
    m_pSysTestlistItem[9]->setName(tr("HP Out Valve"));

    /*
    分配泵  ON/OFF
    */
    m_pSysTestlistItem[11]->setName(tr("Allocated Pump"));

    /*
    管路UV   ON/OFF
    */
    m_pSysTestlistItem[12]->setName(tr("Tube UV Light"));

    /* Vo=0.792*（1+41.2K/Rwb ,Rwb=0~10k */
#endif

}

void SysTestPage::switchLanguage()
{
    buildTranslation();

    buildTitles();

    selectTitle(titleIndex());
}

void SysTestPage::createList()
{

    QColor colors[] = {QColor(200,200,188),QColor(228, 231, 240)};

    listWidget = new QListWidget((QWidget *)m_widget);

    listWidget->setStyleSheet("background-color:transparent");
    listWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    listWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //listWidget->setFrameShape(QListWidget::NoFrame);

    listWidget->setGeometry(QRect(10,55 ,SYSTEM_TEST_LIST_WIDGET_WIDTH,500));

    for(int iLoop = 0 ; iLoop < m_iRealItems ; iLoop++)
    {
        listWidgetIem[iLoop] = new QListWidgetItem;
        listWidgetIem[iLoop]->setSizeHint(QSize(600 , SYSTEM_TEST_LIST_WIDGET_HEIGHT));

        m_pSysTestlistItem[iLoop] = new Systestlistwidgtitem(m_widget,this,aSwitchs[iLoop].type,iLoop);

        listWidgetIem[iLoop]->setBackground(colors[iLoop % 2]);

        listWidget->insertItem(iLoop,listWidgetIem[iLoop]);

        listWidget->setItemWidget(listWidgetIem[iLoop] , m_pSysTestlistItem[iLoop]);

        connect(m_pSysTestlistItem[iLoop], SIGNAL(combActionChange(int)), this, SLOT(on_item_cmb_changed(int)));

    }

    listWidget->setStyleSheet(
                 "QListWidget{background-color:transparent;}"  
                 "QListWidget::item:!selected{}"  
                 "QListWidget::item:selected:active{background:#F2F2F2;color:#19649F;}"  
                 "QListWidget::item:selected{background:#F2F2F2;color:#19649F;}" 
                 );
    
    connect(listWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(ItemClicked(QListWidgetItem*)));

}

void SysTestPage::setBackColor()
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

void SysTestPage::initUi()
{
    setBackColor();

    createList();
}

void SysTestPage::update()
{
    unsigned char buf[1];
    buf[0] = 1;
    DispCmdEntry(DISP_CMD_ENG_MODE,buf,1);
}

void SysTestPage::fade()
{
    unsigned char buf[1];
    buf[0] = 0;
    DispCmdEntry(DISP_CMD_ENG_MODE,buf,1);
}

void SysTestPage::ItemClicked(QListWidgetItem * item)
{
#if 0
    QListWidget *pTmpListWidget = (QListWidget *)this->sender();
    
    int iIndex = pTmpListWidget->row(item);

    switch (m_pSysTestlistItem[iIndex]->getType())
    {
    case SYSTEM_TEST_ITEM_TYPE_SWITCH:
    case SYSTEM_TEST_ITEM_TYPE_SWITCH_AND_VALUE:
        {
            int state ;
            unsigned char buf[32];
            int iIdx       = 0;
            DISPHANDLE hdl = NULL;
            
            m_pSysTestlistItem[iIndex]->toggleSwitchState();
            state = m_pSysTestlistItem[iIndex]->getSwitchState();

            /* do something here */\
            buf[iIdx++] = ENG_CMD_TYPE_SWITCH;
            buf[iIdx++] = aSwitchs[iIndex].id;
            buf[iIdx++] = state ? 1 : 0;
            hdl = DispCmdEntry(DISP_CMD_ENG_CMD,buf,iIdx);
            
            if (!hdl)
            {
                m_pSysTestlistItem[iIndex]->setSwitchState(!state);
            }
        }
        break;
    case SYSTEM_TEST_ITEM_TYPE_RADIO_AND_VALUE:
        {
            int state ;
            m_pSysTestlistItem[iIndex]->changeCombSelection();
            state = m_pSysTestlistItem[iIndex]->getCombSelection();
        }
        break;
    }

    //m_wndMain->showOptDlg(true);

    printf("list index %d\r\n",iIndex);
#endif
}

void SysTestPage::on_btn_clicked(int iIndex)
{
    switch (m_pSysTestlistItem[iIndex]->getType())
    {
    case SYSTEM_TEST_ITEM_TYPE_SWITCH:
    case SYSTEM_TEST_ITEM_TYPE_SWITCH_AND_VALUE:
        {
            int state ;
            unsigned char buf[32];
            int iIdx       = 0;
            DISPHANDLE hdl = NULL;
            
            state = m_pSysTestlistItem[iIndex]->getSwitchState();

            /* do something here */
            buf[iIdx++] = ENG_CMD_TYPE_SWITCH;
            buf[iIdx++] = aSwitchs[iIndex].id;
            buf[iIdx++] = state ? 1 : 0;
            hdl = DispCmdEntry(DISP_CMD_ENG_CMD,buf,iIdx);
            
            if (!hdl)
            {
                m_pSysTestlistItem[iIndex]->setSwitchState(!state);
            }
        }
        break;
    case SYSTEM_TEST_ITEM_TYPE_HO_SWITCH:
        {
            int state ;
            int type;
            DB_HANDLER_STRU * hdl;
                        
            state = m_pSysTestlistItem[iIndex]->getSwitchState();

            type = (APP_EXE_HO_E7_NO == aSwitchs[iIndex].id) ? CIR_TYPE_UP : CIR_TYPE_HP;

            hdl = m_wndMain->getDefaultHandler(type);

            if (hdl)
            {
                DispSndHoSystemTest(hdl->address - APP_HAND_SET_BEGIN_ADDRESS,state,APP_PACKET_HO_SYS_TEST_TYPE_TEST);
            }
        }
        break;
    }
    printf("btn index %d\r\n",iIndex);
    
    m_wndMain->prepareKeyStroke();

}

void SysTestPage::on_item_cmb_changed(int value)
{
    int iIndex = (value >> 8) & 0XFF;
    int iSel = m_pSysTestlistItem[iIndex]->getCombSelection();

    int iPump = aSwitchs[iIndex].id;

    int tmp = 0X0000;

    if (iPump != APP_EXE_C1_NO
        && iPump != APP_EXE_C2_NO)
    {
        return;
    }

    switch(iSel)
    {
    case 0: 
        /* turn off pump */
        tmp = -1;
        break;
    case 1: 
        /* Low speed */
        tmp = DispConvertVoltage2RPumpSpeed(12);
        break;
    case 2: 
        /* Medium speed */
        tmp = DispConvertVoltage2RPumpSpeed(18);
        
        break;
    case 3: 
        /* High speed */
        tmp = DispConvertVoltage2RPumpSpeed(24);
        break;
    default:
        break;
    }

    {
        unsigned char buf[32];
        int iIdx       = 0;
        DISPHANDLE hdl = NULL;
        

        if (-1 != tmp)
        {
#if 1        
             buf[iIdx++] = ENG_CMD_TYPE_RPUMP;
             buf[iIdx++] = ((iPump - APP_EXE_C1_NO + APP_EXE_HOLD_REG_RPUMP_OFFSET) >> 8) & 0XFF;
             buf[iIdx++] = ((iPump - APP_EXE_C1_NO + APP_EXE_HOLD_REG_RPUMP_OFFSET) >> 0) & 0XFF;
        
             buf[iIdx++] = 0XFF;
             buf[iIdx++] = ((tmp) >> 0) & 0XFF;
             hdl = DispCmdEntry(DISP_CMD_ENG_CMD,buf,iIdx);
#else             
             buf[iIdx++] = ENG_CMD_TYPE_SWITCH;
             buf[iIdx++] = aSwitchs[iIndex].id;
             buf[iIdx++] = 1;
             hdl = DispCmdEntry(DISP_CMD_ENG_CMD,buf,iIdx);
#endif
        }
        else
        {
            buf[iIdx++] = ENG_CMD_TYPE_SWITCH;
            buf[iIdx++] = aSwitchs[iIndex].id;
            buf[iIdx++] = 0;
            hdl = DispCmdEntry(DISP_CMD_ENG_CMD,buf,iIdx);
        }

        if (!hdl)
        {
            qDebug() << "set rpump failed";
        }
    }    
}

void  SysTestPage::updateInfo(int major,int minor,float fValue)
{
    switch(major)
    {
    case DISP_NOT_EDI:
        m_pSysTestlistItem[m_aiIndex[major] + minor]->setP1(QString::number(fValue,'f',1));
        break;
    case DISP_NOT_RECT:
        m_pSysTestlistItem[m_aiIndex[major] + minor]->setP1(QString::number(fValue,'f',1));
        break;
    case DISP_NOT_RPUMP:
        if (minor & (1 << 8))
        {
           m_pSysTestlistItem[m_aiIndex[major] + (minor & 0XFF)]->setP2(QString::number(fValue,'f',1));
        }
        else
        {
            m_pSysTestlistItem[m_aiIndex[major] + (minor & 0XFF)]->setP1(QString::number(fValue,'f',1));
        }
        break;
    case DISP_NOT_GPUMP:
        m_pSysTestlistItem[m_aiIndex[major] + minor]->setP1(QString::number(fValue,'f',1));
        break;
    case DISP_NOT_RF_STATE:
        break;
    }
}
