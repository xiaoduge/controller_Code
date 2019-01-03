#include "waterqualitypage.h"


#include "mainwindow.h"

#include <math.h>


#include <QPainter>

#define toOneDecimal(v) (((float)(int)(v*10))/10)
#define toTwoDecimal(v) (((float)(int)(v*100))/100)


QRect wqeditstru[WQ_TYPE_NUM] =
{ 
    QRect(47 , 60,300,200),
    QRect(47 ,300,300,200),
    QRect(400, 60,300,500),
};

WaterQualityPage::WaterQualityPage(QObject *parent,CBaseWidget *widget ,MainWindow *wndMain) : CSubPage(parent,widget,wndMain)
{
    int iTsIdx = 0,iUsIdx = 0,iEsIdx = 0;
    int iLoop;
    
    for (iLoop = 0; iLoop < APP_FM_FLOW_METER_NUM; iLoop++)
    {
        m_aulFlowMeter[iLoop] = 0;
    }

    for (iLoop = 0; iLoop < TANK_NUM; iLoop++)
    {
        aTsIndex[iLoop] = 0XFF;
    }

     for (iLoop = 0; iLoop < UP_NUM; iLoop++)
     {
         aUsIndex[iLoop] = 0XFF;
     }
    
    for (iLoop = 0; iLoop < EDI_NUM; iLoop++)
    {
        aEsIndex[iLoop] = 0XFF;
    }

     switch(gGlobalParam.iMachineType)
     {
     case MACHINE_ADAPT:
        break;
     case MACHINE_PURIST:
         aTsIds[iTsIdx].iId   = TANK_LEVEL;
         aTsIndex[TANK_LEVEL] = iTsIdx;
         iTsIdx++;
         aTsIds[iTsIdx].iId   = TANK_VALUE;
         aTsIndex[TANK_VALUE] = iTsIdx;
         iTsIdx++;
         break;
     default:
         aTsIds[iTsIdx].iId     = TANK_LEVEL;
         aTsIndex[TANK_LEVEL]   = iTsIdx;
         iTsIdx++;
         aTsIds[iTsIdx].iId     = TANK_VALUE;
         aTsIndex[TANK_VALUE]   = iTsIdx;
         iTsIdx++;
         aTsIds[iTsIdx].iId     = CIR_WATER_QUA;
         aTsIndex[CIR_WATER_QUA]    = iTsIdx;
         iTsIdx++;
         aTsIds[iTsIdx].iId     = CIR_SPEED;
         aTsIndex[CIR_SPEED]    = iTsIdx;
         iTsIdx++;
         break;
     }

     m_aiRealNum[WQ_TYPE_TANK] = iTsIdx;

     switch(gGlobalParam.iMachineType)
     {
     case MACHINE_L_EDI_LOOP:
     case MACHINE_L_RO_LOOP:  
     case MACHINE_EDI:
     case MACHINE_RO:
        break;
     case MACHINE_PURIST:
     {
        aUsIds[iUsIdx].iId   = UP_WATER;
        aUsIndex[UP_WATER]   = iUsIdx;
        iUsIdx++;
        if(gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_HaveTOC))
        {
            aUsIds[iUsIdx].iId   = UP_TOC;
            aUsIndex[UP_TOC]     = iUsIdx;
            iUsIdx++;
        }

        aUsIds[iUsIdx].iId          = UP_PRODUCT_TEMT;
        aUsIndex[UP_PRODUCT_TEMT]   = iUsIdx;
        iUsIdx++;
        aUsIds[iUsIdx].iId   = UP_IN_QUA;
        aUsIndex[UP_IN_QUA]  = iUsIdx;
        iUsIdx++;
        aUsIds[iUsIdx].iId   = UP_SPEED;
        aUsIndex[UP_SPEED]   = iUsIdx;
        iUsIdx++;
        break;
     }
     default:
     {
        aUsIds[iUsIdx].iId   = UP_WATER;
        aUsIndex[UP_WATER]   = iUsIdx;
        iUsIdx++;
        if(gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_HaveTOC))
        {
            aUsIds[iUsIdx].iId   = UP_TOC;
            aUsIndex[UP_TOC]     = iUsIdx;
            iUsIdx++;
        }

        aUsIds[iUsIdx].iId   = UP_PRODUCT_TEMT;
        aUsIndex[UP_PRODUCT_TEMT]       = iUsIdx;
        iUsIdx++;
        aUsIds[iUsIdx].iId   = UP_SPEED;
        aUsIndex[UP_SPEED]   = iUsIdx;
        iUsIdx++;
        break;
     }
     }

      m_aiRealNum[WQ_TYPE_UP] = iUsIdx;

    switch(gGlobalParam.iMachineType)
     {
     case MACHINE_L_Genie:
     case MACHINE_L_EDI_LOOP:
     case MACHINE_Genie:
     case MACHINE_EDI:
        aEsIds[iEsIdx].iId             = EDI_PRODUCT_QUALITY;
        aEsIndex[EDI_PRODUCT_QUALITY]  = iEsIdx;
        iEsIdx++;
        aEsIds[iEsIdx].iId       = EDI_PRODUCT_T;
        aEsIndex[EDI_PRODUCT_T]  = iEsIdx;
        iEsIdx++;
        break;
     }   

     switch(gGlobalParam.iMachineType)
     {
     case MACHINE_PURIST:
        break;
     default:
        aEsIds[iEsIdx].iId            = RO_PRODUCT_QUALITY;
        aEsIndex[RO_PRODUCT_QUALITY]  = iEsIdx;
        iEsIdx++;
        aEsIds[iEsIdx].iId      = RO_PRODUCT_T;
        aEsIndex[RO_PRODUCT_T]  = iEsIdx;
        iEsIdx++;
        aEsIds[iEsIdx].iId          = RO_RESIDUE_RATIO;
        aEsIndex[RO_RESIDUE_RATIO]  = iEsIdx;
        iEsIdx++;
        aEsIds[iEsIdx].iId           = TAP_WATER_QUALITY;
        aEsIndex[TAP_WATER_QUALITY]  = iEsIdx;
        iEsIdx++;
        aEsIds[iEsIdx].iId             = RO_WATER_IN_QUALITY;
        aEsIndex[RO_WATER_IN_QUALITY]  = iEsIdx;
        iEsIdx++;
        aEsIds[iEsIdx].iId             = RO_WATER_IN_TEMPT;
        aEsIndex[RO_WATER_IN_TEMPT]    = iEsIdx;
        iEsIdx++;
        aEsIds[iEsIdx].iId            = TAP_WATER_PRESSURE;
        aEsIndex[TAP_WATER_PRESSURE]  = iEsIdx;
        iEsIdx++;
        aEsIds[iEsIdx].iId          = RO_WORK_PRESSURE;
        aEsIndex[RO_WORK_PRESSURE]  = iEsIdx;
        iEsIdx++;
        break;
     }  

     switch(gGlobalParam.iMachineType)
     {
     case MACHINE_L_Genie:
     case MACHINE_L_UP:
     case MACHINE_L_EDI_LOOP:
     case MACHINE_L_RO_LOOP:
        aEsIds[iEsIdx].iId                = RO_PRODUCT_WATER_SPEED;
        aEsIndex[RO_PRODUCT_WATER_SPEED]  = iEsIdx;
        iEsIdx++;
        aEsIds[iEsIdx].iId                = RO_DISCARD_WATER_SPEED;
        aEsIndex[RO_DISCARD_WATER_SPEED]  = iEsIdx;
        iEsIdx++;
        aEsIds[iEsIdx].iId           = RO_WATER_IN_SPEED;
        aEsIndex[RO_WATER_IN_SPEED]  = iEsIdx;
        iEsIdx++;
        aEsIds[iEsIdx].iId         = TAP_WATER_SPEED;
        aEsIndex[TAP_WATER_SPEED]  = iEsIdx;
        iEsIdx++;
        break;
     }

     switch(gGlobalParam.iMachineType)
     {
     case MACHINE_L_Genie:
     case MACHINE_L_EDI_LOOP:
        aEsIds[iEsIdx].iId              = EDI_PROC_WATER_SPEED;
        aEsIndex[EDI_PROC_WATER_SPEED]  = iEsIdx;
        iEsIdx++;
        aEsIds[iEsIdx].iId                 = EDI_DISCARD_WATER_SPEED;
        aEsIndex[EDI_DISCARD_WATER_SPEED]  = iEsIdx;
        iEsIdx++;
        break;
     }
      
      m_aiRealNum[WQ_TYPE_EDI] = iEsIdx;
      
      creatTitle();
  
      initUi();    
      
      buildTranslation();
  
      init_up_msg();
  
      init_tank_msg();
  
      init_edi_msg();
      
      updEcoInfo(APP_EXE_I1_NO,m_wndMain->getEco(APP_EXE_I1_NO),true);
      updEcoInfo(APP_EXE_I2_NO,m_wndMain->getEco(APP_EXE_I2_NO),true);
      updEcoInfo(APP_EXE_I3_NO,m_wndMain->getEco(APP_EXE_I3_NO),true);
      updEcoInfo(APP_EXE_I4_NO,m_wndMain->getEco(APP_EXE_I4_NO),true);
      updEcoInfo(APP_EXE_I5_NO,m_wndMain->getEco(APP_EXE_I5_NO),true);

      //2018.10.22 add
      update_edi_msg(RO_RESIDUE_RATIO, 0);
      updSwPressure(0);
      updPressure(APP_EXE_PM1_NO, 0);
      for(iLoop = 0; iLoop < APP_FM_FLOW_METER_NUM; iLoop++)
      {
          updFlowInfo(iLoop, 0);
      }
      //
}

void WaterQualityPage::creatTitle()
{
    CSubPage::creatTitle();

    buildTitles();
    
    selectTitle(0);
}

void WaterQualityPage::buildTitles()
{
    QStringList stringList;

    stringList << tr("Water Quality");

    setTitles(stringList);

}

void WaterQualityPage::buildTranslation()
{
    int iIdx;

    for (iIdx = 0; iIdx < m_aiRealNum[WQ_TYPE_TANK]; iIdx++)
    {
        switch(aTsIds[iIdx].iId)
        {
        case TANK_LEVEL:
            tankmsg[iIdx] = tr("Tank Level")    + " : %1" + "%" ;
            break;
        case TANK_VALUE:
            tankmsg[iIdx] = tr("Volume")  + " : %1" + tr("L");
            break;
        case CIR_WATER_QUA:  
            if ((CONDUCTIVITY_UINT_OMG == gGlobalParam.MiscParam.iUint4Conductivity))
            {
                tankmsg[iIdx]  = tr("HP Resis.") + " : %1" + tr("omg");
            }
            else
            {
                tankmsg[iIdx]  = tr("HP Resis.") + " : %1" + tr("us/cm");
            }
            break;
        case CIR_SPEED:
            if ((FLOW_VELOCITY_UINT_L_PER_MIN == gGlobalParam.MiscParam.iUint4FlowVelocity))
            {
                tankmsg[iIdx]  = tr("HP Disp. rate")  + " : %1" + tr("L/min");
            }
            else
            {
                tankmsg[iIdx]  = tr("HP Disp. rate")  + " : %1" + tr("G/min");
            }
            break;
        }   
    }

    for (iIdx = 0; iIdx < m_aiRealNum[WQ_TYPE_UP]; iIdx++)
    {
        switch(aUsIds[iIdx].iId)
        {
        case UP_WATER:
            if ((CONDUCTIVITY_UINT_OMG == gGlobalParam.MiscParam.iUint4Conductivity))
            {
                upmsg[iIdx] = tr("UP") + " : %1" + tr("omg");
            }
            else
            {
                upmsg[iIdx] = tr("UP") + " : %1" + tr("us/cm");
            }
            break;
        case UP_TOC:
            upmsg[iIdx]   = tr("TOC")      + " : %1"  + tr("ppb")  ;
            break;
        case UP_PRODUCT_TEMT:
            if ((TEMERATURE_UINT_CELSIUS == gGlobalParam.MiscParam.iUint4Temperature))
            {
                upmsg[iIdx]     = tr("Temp.")     + " : %1" + tr("Celsius");
            }
            else
            {
                upmsg[iIdx]     = tr("Temp.")     + " : %1" + tr("Fahrenheit");
            }
            break;
        case UP_SPEED:
            if ((FLOW_VELOCITY_UINT_L_PER_MIN == gGlobalParam.MiscParam.iUint4FlowVelocity))
            {
                upmsg[iIdx] = tr("UP Disp. rate") + " : %1" + tr("L/min");
            }
            else
            {
                upmsg[iIdx] = tr("UP Disp. rate") + " : %1" + tr("G/min");
            }
            break;
        case UP_IN_QUA:
            if (MACHINE_PURIST == gGlobalParam.iMachineType)
            {
                upmsg[iIdx] = tr("UP IN") + " : %1" + tr("us/cm");
            }
            else
            {
                 if ((CONDUCTIVITY_UINT_OMG == gGlobalParam.MiscParam.iUint4Conductivity))
                 {
                     upmsg[iIdx] = tr("UP IN") + " : %1" + tr("Omg");
                 }
                 else
                 {
                     upmsg[iIdx] = tr("UP IN") + " : %1" + tr("us/cm");
                 }
            }
            break;
            
        }   
    }
    
    for (iIdx = 0; iIdx < m_aiRealNum[WQ_TYPE_EDI]; iIdx++)
    {
        switch(aEsIds[iIdx].iId)
        {
        case EDI_PRODUCT_QUALITY:  
            if ((CONDUCTIVITY_UINT_OMG == gGlobalParam.MiscParam.iUint4Conductivity))
            {
                edimsg[iIdx]     = tr("EDI Product")+ " : %1" + tr("Omg");
            }
            else
            {
                edimsg[iIdx]     = tr("EDI Product")+ " : %1" + tr("us/cm");
            }
            break;
        case EDI_PRODUCT_T:
            if ((TEMERATURE_UINT_CELSIUS == gGlobalParam.MiscParam.iUint4Temperature))
            {
                edimsg[iIdx]   = tr("EDI Temp.")     + " : %1" + tr("Celsius");
            }
            else
            {
                edimsg[iIdx]   = tr("EDI Temp.")     + " : %1" + tr("Fahrenheit");
            }
            break;   
       case RO_PRODUCT_QUALITY:
            edimsg[iIdx]   = tr("RO Product")    + " : %1" + tr("us/cm");
            break;
       case RO_PRODUCT_T:
            if ((TEMERATURE_UINT_CELSIUS == gGlobalParam.MiscParam.iUint4Temperature))
            {
                edimsg[iIdx]   = tr("RO Temp.")   + " : %1" + tr("Celsius");
            }
            else
            {
                edimsg[iIdx]   = tr("RO Temp.")   + " : %1" + tr("Fahrenheit");
            }
            break; 
        case RO_RESIDUE_RATIO:
            edimsg[iIdx]        = tr("RO Rejection")             + " : %1" + "%";
            break;
        case TAP_WATER_QUALITY:
            edimsg[iIdx]       = tr("Tap Cond.")                  + " : %1" + tr("us/cm");
            break;
        case RO_WATER_IN_QUALITY:
            edimsg[iIdx]     = tr("RO Feed Cond.")                + " : %1" + tr("us/cm");
            break;
        case RO_WATER_IN_TEMPT:
            if ((TEMERATURE_UINT_CELSIUS == gGlobalParam.MiscParam.iUint4Temperature))
            {
                edimsg[iIdx]   = tr("RO Feed Temp.")   + " : %1" + tr("Celsius");
            }
            else
            {
                edimsg[iIdx]   = tr("RO Feed Temp.")   + " : %1" + tr("Fahrenheit");
            }
            break;
        case TAP_WATER_PRESSURE:
            if ((PRESSURE_UINT_BAR == gGlobalParam.MiscParam.iUint4Pressure))
            {
                edimsg[iIdx]      = tr("RO Feed Pressure")         + " : %1" + tr("bar");
            }
            else if (PRESSURE_UINT_MPA == gGlobalParam.MiscParam.iUint4Pressure)
            {
                edimsg[iIdx]      = tr("RO Feed Pressure")         + " : %1" + tr("mpa");
            }
            else
            {
                edimsg[iIdx]      = tr("RO Feed Pressure")         + " : %1" + tr("psi");
            }
            break;
        case RO_WORK_PRESSURE:
            if (PRESSURE_UINT_BAR == gGlobalParam.MiscParam.iUint4Pressure)
            {
                edimsg[iIdx]        = tr("RO Pressure")           + " : %1" + tr("bar");
            }
            else if (PRESSURE_UINT_MPA == gGlobalParam.MiscParam.iUint4Pressure)
            {
                edimsg[iIdx]        = tr("RO Pressure")           + " : %1" + tr("mpa");
            }
            else
            {
                edimsg[iIdx]        = tr("RO Pressure")           + " : %1" + tr("psi");
            }
            break;
        case RO_PRODUCT_WATER_SPEED:
             if ((FLOW_VELOCITY_UINT_L_PER_MIN == gGlobalParam.MiscParam.iUint4FlowVelocity))
            {
                edimsg[iIdx]  = tr("RO Product Rate")     + " : %1" + tr("L/h");
            }
            else
            {
                edimsg[iIdx]  = tr("RO Product Rate")     + " : %1" + tr("G/h");
            }
            break;
        case RO_DISCARD_WATER_SPEED:
             if ((FLOW_VELOCITY_UINT_L_PER_MIN == gGlobalParam.MiscParam.iUint4FlowVelocity))
            {
                edimsg[iIdx]  = tr("RO Reject Rate")  + " : %1" + tr("L/h");
            }
            else
            {
                edimsg[iIdx]  = tr("RO Reject Rate")  + " : %1" + tr("G/h");
            }
            break;
        case RO_WATER_IN_SPEED:
             if ((FLOW_VELOCITY_UINT_L_PER_MIN == gGlobalParam.MiscParam.iUint4FlowVelocity))
            {
                edimsg[iIdx]       = tr("RO Feed rate")       + " : %1" + tr("L/h");
            }
            else
            {
                edimsg[iIdx]       = tr("RO Feed rate")       + " : %1" + tr("G/h");
            }
            break;
        case TAP_WATER_SPEED:
            if ((FLOW_VELOCITY_UINT_L_PER_MIN == gGlobalParam.MiscParam.iUint4FlowVelocity))
            {
                edimsg[iIdx]         = tr("Tap Rate")         + " : %1" + tr("L/h");
            }
            else
            {
                edimsg[iIdx]         = tr("Tap Rate")         + " : %1" + tr("G/h");
            }
            break;
        case EDI_PROC_WATER_SPEED:
             if ((FLOW_VELOCITY_UINT_L_PER_MIN == gGlobalParam.MiscParam.iUint4FlowVelocity))
            {
                edimsg[iIdx]    = tr("EDI Product Rate") + " : %1" + tr("L/h");
            }
            else
            {
                edimsg[iIdx]    = tr("EDI Product Rate") + " : %1" + tr("G/h");
            }
            break;
        case EDI_DISCARD_WATER_SPEED:
            if ((FLOW_VELOCITY_UINT_L_PER_MIN == gGlobalParam.MiscParam.iUint4FlowVelocity))
            {
                edimsg[iIdx] = tr("EDI Reject Rate") + " : %1" + tr("L/h");
            }
            else
            {
                edimsg[iIdx] = tr("EDI Reject Rate") + " : %1" + tr("G/h");
            }
            break;
        }   
    }
}

void WaterQualityPage::switchLanguage()
{
    buildTranslation();

    buildTitles();

    selectTitle(titleIndex());
}

void WaterQualityPage::update_up_msg(int isIdx,float fValue)
{

    QString tmpMsg , Name , Value;

    QTextCursor cursor;

    
    if (!isVisible())
    {
        return;
    }

    int iIdx = aUsIndex[isIdx];

    if (iIdx >= m_aiRealNum[WQ_TYPE_UP])
    {
        return;
    }
     
    m_edit4Up[iIdx]->setText("");

    cursor = m_edit4Up[iIdx]->textCursor();

    switch(isIdx)
    {
    case UP_TOC:
    {
        if(fValue > 200)
        {
//            tmpMsg = QString(" > 200");
            tmpMsg = upmsg[iIdx].arg(" > 200");
        }
        else
        {
            tmpMsg = upmsg[iIdx].arg((int)fValue);
        }
//        tmpMsg = upmsg[iIdx].arg((int)fValue);
        break;
    }
    case UP_WATER:
        if(1 > fValue)
        {
            tmpMsg = upmsg[iIdx].arg(fValue,0,'f',3);
        }
        else
        {
            tmpMsg = upmsg[iIdx].arg(fValue,0,'f',1);
        }
        break;
    default:
        tmpMsg = upmsg[iIdx].arg(fValue,0,'f',1);
        break;
    }

    if(-1 == tmpMsg.indexOf(":"))
    {
        Name = tmpMsg;

        Value = "";
    }
    else
    {
        Name = tmpMsg.mid(0, tmpMsg.indexOf(":")+1);

        Value = tmpMsg.mid(tmpMsg.indexOf(":") + 1) ;
    }

    cursor.movePosition(QTextCursor::Start,QTextCursor::MoveAnchor);

    m_edit4Up[iIdx]->setTextCursor(cursor);

    cursor.setCharFormat(charFormatName);
    cursor.insertText(Name); 
    
    cursor.setCharFormat(charFormatValue);
    cursor.insertText(Value); 

}

void WaterQualityPage::updAllInfo(void)
{
    if (CONDUCTIVITY_UINT_OMG == gGlobalParam.MiscParam.iUint4Conductivity)
    {
        update_up_msg(UP_WATER,m_aUpHistoryEco[UP_WATER].fShowInfo);
        update_tank_msg(CIR_WATER_QUA,m_aTankHistoryEco[CIR_WATER_QUA].fShowInfo);       
        update_edi_msg(EDI_PRODUCT_QUALITY,toOneDecimal(m_aEDIHistoryEco[EDI_PRODUCT_QUALITY].fShowInfo));
    }
    else
    {
        update_up_msg(UP_WATER,toConductivity(m_aUpHistoryEco[UP_WATER].fShowInfo));
        update_tank_msg(CIR_WATER_QUA,toConductivity(m_aTankHistoryEco[CIR_WATER_QUA].fShowInfo));
        update_edi_msg(EDI_PRODUCT_QUALITY,toOneDecimal(toConductivity(m_aTankHistoryEco[EDI_PRODUCT_QUALITY].fShowInfo)));
    }

    if (TEMERATURE_UINT_CELSIUS == gGlobalParam.MiscParam.iUint4Temperature)
    {
        update_up_msg(UP_PRODUCT_TEMT,m_aUpHistoryEco[UP_PRODUCT_TEMT].fShowInfo);
        update_edi_msg(EDI_PRODUCT_T,toOneDecimal(m_aEDIHistoryEco[EDI_PRODUCT_T].fShowInfo));
        update_edi_msg(RO_PRODUCT_T,toOneDecimal(m_aEDIHistoryEco[RO_PRODUCT_T].fShowInfo));
        update_edi_msg(RO_WATER_IN_TEMPT ,toOneDecimal(m_aEDIHistoryEco[RO_WATER_IN_TEMPT].fShowInfo));   
    }
    else
    {
        update_up_msg(UP_PRODUCT_TEMT,toFahrenheit(m_aUpHistoryEco[UP_PRODUCT_TEMT].fShowInfo));
        update_edi_msg(EDI_PRODUCT_T,toFahrenheit(toOneDecimal(m_aEDIHistoryEco[EDI_PRODUCT_T].fShowInfo)));
        update_edi_msg(RO_PRODUCT_T,toFahrenheit(toOneDecimal(m_aEDIHistoryEco[RO_PRODUCT_T].fShowInfo)));
        update_edi_msg(RO_WATER_IN_TEMPT,toFahrenheit(toOneDecimal(m_aEDIHistoryEco[RO_WATER_IN_TEMPT].fShowInfo)));
    }  

    
    if ((FLOW_VELOCITY_UINT_GAL_PER_MIN == gGlobalParam.MiscParam.iUint4FlowVelocity))
    {
        update_up_msg(UP_SPEED,toOneDecimal((toGallon(1))*m_aUpHistoryEco[UP_SPEED].fShowInfo));
        update_tank_msg(CIR_SPEED,toOneDecimal((toGallon(1))*m_aTankHistoryEco[CIR_SPEED].fShowInfo));
        update_edi_msg(RO_WATER_IN_SPEED,toTwoDecimal((toGallon(1))*m_aEDIHistoryEco[RO_WATER_IN_SPEED].fShowInfo));
        update_edi_msg(RO_PRODUCT_WATER_SPEED,toTwoDecimal((toGallon(1))*m_aEDIHistoryEco[RO_PRODUCT_WATER_SPEED].fShowInfo));
        update_edi_msg(TAP_WATER_SPEED,toTwoDecimal((toGallon(1))*m_aEDIHistoryEco[TAP_WATER_SPEED].fShowInfo));
        update_edi_msg(EDI_PROC_WATER_SPEED,toTwoDecimal((toGallon(1))*m_aEDIHistoryEco[EDI_PROC_WATER_SPEED].fShowInfo));
        update_edi_msg(EDI_DISCARD_WATER_SPEED,toTwoDecimal((toGallon(1))*m_aEDIHistoryEco[EDI_DISCARD_WATER_SPEED].fShowInfo));
        update_edi_msg(RO_DISCARD_WATER_SPEED,toTwoDecimal((toGallon(1))*m_aEDIHistoryEco[RO_DISCARD_WATER_SPEED].fShowInfo));
    }
    else
    {
        update_up_msg(UP_SPEED,toOneDecimal( 1.0 * m_aUpHistoryEco[UP_SPEED].fShowInfo));
        update_tank_msg(CIR_SPEED,toOneDecimal( 1.0 *m_aTankHistoryEco[CIR_SPEED].fShowInfo));
        update_edi_msg(RO_WATER_IN_SPEED,toTwoDecimal( 1.0 *m_aEDIHistoryEco[RO_WATER_IN_SPEED].fShowInfo));
        update_edi_msg(RO_PRODUCT_WATER_SPEED,toTwoDecimal( 1.0 *m_aEDIHistoryEco[RO_PRODUCT_WATER_SPEED].fShowInfo));
        update_edi_msg(TAP_WATER_SPEED,toTwoDecimal( 1.0 *m_aEDIHistoryEco[TAP_WATER_SPEED].fShowInfo));
        update_edi_msg(EDI_PROC_WATER_SPEED,toTwoDecimal( 1.0 *m_aEDIHistoryEco[EDI_PROC_WATER_SPEED].fShowInfo));
        update_edi_msg(EDI_DISCARD_WATER_SPEED,toTwoDecimal( 1.0 *m_aEDIHistoryEco[EDI_DISCARD_WATER_SPEED].fShowInfo));
        update_edi_msg(RO_DISCARD_WATER_SPEED,toTwoDecimal( 1.0 *m_aEDIHistoryEco[RO_DISCARD_WATER_SPEED].fShowInfo));
    }

    if (PRESSURE_UINT_BAR == gGlobalParam.MiscParam.iUint4Pressure)
    {
        update_edi_msg(RO_WORK_PRESSURE,toOneDecimal(m_aEDIHistoryEco[RO_WORK_PRESSURE].fShowInfo));
        update_edi_msg(TAP_WATER_PRESSURE,toOneDecimal(m_aEDIHistoryEco[TAP_WATER_PRESSURE].fShowInfo));
    }
    else if (PRESSURE_UINT_MPA == gGlobalParam.MiscParam.iUint4Pressure)
    {
        update_edi_msg(RO_WORK_PRESSURE,toTwoDecimal(toMpa(m_aEDIHistoryEco[RO_WORK_PRESSURE].fShowInfo)));
        update_edi_msg(TAP_WATER_PRESSURE,toTwoDecimal(toMpa(m_aEDIHistoryEco[TAP_WATER_PRESSURE].fShowInfo)));
    }
    else
    {
        update_edi_msg(RO_WORK_PRESSURE,toPsi(m_aEDIHistoryEco[RO_WORK_PRESSURE].fShowInfo));
        update_edi_msg(TAP_WATER_PRESSURE,toPsi(m_aEDIHistoryEco[TAP_WATER_PRESSURE].fShowInfo));
    }
    
    {
        update_tank_msg(TANK_LEVEL,m_aTankHistoryEco[TANK_LEVEL].fShowInfo);
        update_tank_msg(TANK_VALUE,m_aTankHistoryEco[TANK_VALUE].fShowInfo);
    }
    
    update_edi_msg(RO_PRODUCT_QUALITY,toOneDecimal(m_aEDIHistoryEco[RO_PRODUCT_QUALITY].fShowInfo));
    update_edi_msg(RO_RESIDUE_RATIO, m_aEDIHistoryEco[RO_RESIDUE_RATIO].fShowInfo);
    update_edi_msg(TAP_WATER_QUALITY,m_aEDIHistoryEco[TAP_WATER_QUALITY].fShowInfo);
    update_edi_msg(RO_WATER_IN_QUALITY,m_aEDIHistoryEco[RO_WATER_IN_QUALITY].fShowInfo);

    update_up_msg(UP_TOC, m_aUpHistoryEco[UP_TOC].fShowInfo);
    
}

void WaterQualityPage::updEcoInfo(int iIndex,ECO_INFO_STRU *info,bool bForceUpd)
{
    
    switch(iIndex)
    {
    case APP_EXE_I5_NO:
        {
            float fQ ;
            float fT;
        
            if (CONDUCTIVITY_UINT_OMG == gGlobalParam.MiscParam.iUint4Conductivity)
            {
                fQ = info->fQuality;
            }
            else
            {
                fQ = toConductivity(info->fQuality);
            }

            
            if (TEMERATURE_UINT_CELSIUS == gGlobalParam.MiscParam.iUint4Temperature)
            {
                fT = info->fTemperature;
            }
            else
            {
                fT = toFahrenheit(info->fTemperature);
            }  
            
            if (DispGetUpQtwFlag() || DispGetUpCirFlag())
            { 
                update_up_msg(UP_WATER,toOneDecimal(fQ));
                m_aUpHistoryEco[UP_WATER].fShowInfo = info->fQuality;
                
                update_up_msg(UP_PRODUCT_TEMT,toOneDecimal(fT));
                m_aUpHistoryEco[UP_PRODUCT_TEMT].fShowInfo = info->fTemperature;
            }
            else if (bForceUpd) 
            {
                update_up_msg(UP_WATER,toOneDecimal(fQ));
                m_aUpHistoryEco[UP_WATER].fShowInfo = info->fQuality;
                
                update_up_msg(UP_PRODUCT_TEMT,toOneDecimal(fT));
                m_aUpHistoryEco[UP_PRODUCT_TEMT].fShowInfo = info->fTemperature;

            }
        }
        break;
    case APP_EXE_I4_NO:
        {
            float fQ ;
            
            if (CONDUCTIVITY_UINT_OMG == gGlobalParam.MiscParam.iUint4Conductivity)
            {
                fQ = info->fQuality;
            }
            else
            {
                fQ = toConductivity(info->fQuality);
            }
            
            if (DispGetTubeCirFlag())
            {
            }
            else if (DispGetTocCirFlag())
            {
                update_tank_msg(CIR_WATER_QUA,toOneDecimal(fQ));
                m_aTankHistoryEco[CIR_WATER_QUA].fShowInfo = info->fQuality;
            } 
            else if (DispGetEdiQtwFlag() //0629
                     &&(MACHINE_PURIST != gGlobalParam.iMachineType)
                     &&(MACHINE_RO != gGlobalParam.iMachineType))
            {
                update_tank_msg(CIR_WATER_QUA,toOneDecimal(fQ));
                m_aTankHistoryEco[CIR_WATER_QUA].fShowInfo = info->fQuality;
            }
            else if (DispGetTankCirFlag()
                && (MACHINE_PURIST != gGlobalParam.iMachineType))
            {
//                update_tank_msg(CIR_WATER_QUA,toOneDecimal(fQ));
//                m_aTankHistoryEco[CIR_WATER_QUA].fShowInfo = info->fQuality;
            }
            else if (bForceUpd
                     && (MACHINE_PURIST != gGlobalParam.iMachineType)
                     && (MACHINE_RO != gGlobalParam.iMachineType))
            {
                update_tank_msg(CIR_WATER_QUA,toOneDecimal(fQ));
                m_aTankHistoryEco[CIR_WATER_QUA].fShowInfo = info->fQuality;
            }
        }
        break;
    case APP_EXE_I3_NO:
        {
            float fQ ;
            float fT;
            if (CONDUCTIVITY_UINT_OMG == gGlobalParam.MiscParam.iUint4Conductivity)
            {
                fQ = info->fQuality;
            }
            else
            {
                fQ = toConductivity(info->fQuality);
            }
            
            if (TEMERATURE_UINT_CELSIUS == gGlobalParam.MiscParam.iUint4Temperature)
            {
                fT = info->fTemperature;
            }
            else
            {
                fT = toFahrenheit(info->fTemperature);
            }              
            update_edi_msg(EDI_PRODUCT_QUALITY,toOneDecimal(fQ));
            m_aEDIHistoryEco[EDI_PRODUCT_QUALITY].fShowInfo = info->fQuality;
            
            update_edi_msg(EDI_PRODUCT_T,toOneDecimal(fT));
            m_aEDIHistoryEco[EDI_PRODUCT_T].fShowInfo = info->fTemperature;

            if(gGlobalParam.iMachineType == MACHINE_RO)
            {
                if (DispGetEdiQtwFlag() || DispGetTankCirFlag())
                {
                    update_tank_msg(CIR_WATER_QUA,toOneDecimal(fQ));
                    m_aTankHistoryEco[CIR_WATER_QUA].fShowInfo = info->fQuality;
                }
            }
        }
        break;
    case APP_EXE_I2_NO: // RO Out
        {
            //float fReverse = (info->fQuality != 0) ? 1 / info->fQuality : 9999;
            float fResidue;
            float fT;
                        
            if (DispGetREJ(&fResidue)
                && (MACHINE_PURIST != gGlobalParam.iMachineType))
            {
                update_edi_msg(RO_RESIDUE_RATIO,(fResidue));
                m_aEDIHistoryEco[RO_RESIDUE_RATIO].fShowInfo = fResidue;
            }

            if (TEMERATURE_UINT_CELSIUS == gGlobalParam.MiscParam.iUint4Temperature)
            {
                fT = info->fTemperature;
            }
            else
            {
                fT = toFahrenheit(info->fTemperature);
            }
            
            if(MACHINE_PURIST != gGlobalParam.iMachineType)
            {
            
                update_edi_msg(RO_PRODUCT_QUALITY,toOneDecimal(info->fQuality));
                m_aEDIHistoryEco[RO_PRODUCT_QUALITY].fShowInfo = info->fQuality;
                
                update_edi_msg(RO_PRODUCT_T,toOneDecimal(fT));
                m_aEDIHistoryEco[RO_PRODUCT_T].fShowInfo = info->fTemperature;
            }
            else
            {
                update_up_msg(UP_IN_QUA,toOneDecimal(info->fQuality));
                m_aUpHistoryEco[UP_IN_QUA].fShowInfo = info->fQuality;
                
                update_up_msg(UP_PRODUCT_TEMT,toOneDecimal(fT));
                m_aUpHistoryEco[UP_PRODUCT_TEMT].fShowInfo = info->fTemperature;
            }
        }
        break;
    case APP_EXE_I1_NO: // RO In
        {
            //float fReverse = (info->fQuality != 0) ? 1 / info->fQuality : 2000;
            float fT;
            
            if (DispGetInitRunFlag())
            {
                update_edi_msg(TAP_WATER_QUALITY,(info->fQuality));
                
                m_aEDIHistoryEco[TAP_WATER_QUALITY].fShowInfo = info->fQuality;
            }
            else
            {
                update_edi_msg(RO_WATER_IN_QUALITY,info->fQuality);
                
                m_aEDIHistoryEco[RO_WATER_IN_QUALITY].fShowInfo = info->fQuality;
            }

            if (TEMERATURE_UINT_CELSIUS == gGlobalParam.MiscParam.iUint4Temperature)
            {
                fT = info->fTemperature;
            }
            else
            {
                fT = toFahrenheit(info->fTemperature);
            }              

            update_edi_msg(RO_WATER_IN_TEMPT ,toOneDecimal(fT));   
            m_aEDIHistoryEco[RO_WATER_IN_TEMPT].fShowInfo = info->fTemperature;
        }
        break;
    }
}

void WaterQualityPage::updPressure(int iIndex,float fvalue)
{

    if (APP_EXE_PM1_NO == iIndex)
    {
        float fP ;
        if (PRESSURE_UINT_BAR == gGlobalParam.MiscParam.iUint4Pressure)
        {
            fP = fvalue ;
            
            update_edi_msg(RO_WORK_PRESSURE,toOneDecimal(fP));
        }
        else if (PRESSURE_UINT_MPA == gGlobalParam.MiscParam.iUint4Pressure)
        {
            fP = toMpa(fvalue);
            update_edi_msg(RO_WORK_PRESSURE,toTwoDecimal(fP));
        
        }
        else
        {
            fP = toPsi(fvalue);
            update_edi_msg(RO_WORK_PRESSURE,fP);
        }

        m_aEDIHistoryEco[RO_WORK_PRESSURE].fShowInfo = fvalue;
    }
}

void WaterQualityPage::updSwPressure(float fvalue)
{
    float fP = fvalue;
    
    if (PRESSURE_UINT_BAR == gGlobalParam.MiscParam.iUint4Pressure)
    {
        fP = fvalue ;
        update_edi_msg(TAP_WATER_PRESSURE,toOneDecimal(fP));
    }
    else if (PRESSURE_UINT_MPA == gGlobalParam.MiscParam.iUint4Pressure)
    {
        fP = toMpa(fvalue);
        update_edi_msg(TAP_WATER_PRESSURE,toTwoDecimal(fP));
    }
    else
    {
        fP = toPsi(fvalue);
        
        update_edi_msg(TAP_WATER_PRESSURE,(fP));
    }

    m_aEDIHistoryEco[TAP_WATER_PRESSURE].fShowInfo = fvalue;
}

void WaterQualityPage::updTOC(float fToc)
{
//    UP_TOC
    update_up_msg(UP_TOC, fToc);
    m_aUpHistoryEco[UP_TOC].fShowInfo = fToc;

}

void WaterQualityPage::updFlowInfo(int iIndex,int iValue)
{
    float fUnit = 1.0;

    
    m_aulFlowMeter[iIndex] = iValue;

   if ((FLOW_VELOCITY_UINT_GAL_PER_MIN == gGlobalParam.MiscParam.iUint4FlowVelocity))
   {
       fUnit =  toGallon(1);
   }

   switch(iIndex)
   {
   case APP_FM_FM1_NO:
   {
        if (DispGetUpQtwFlag()
            || DispGetUpCirFlag())
        {
            if (0 != m_aTankHistoryEco[CIR_SPEED].fShowInfo)
            {
                update_tank_msg(CIR_SPEED,0);
                m_aTankHistoryEco[CIR_SPEED].fShowInfo = 0;
            }
            update_up_msg(UP_SPEED,toOneDecimal(fUnit*(iValue*1.0)/1000));      
            m_aUpHistoryEco[UP_SPEED].fShowInfo = ((iValue*1.0)/1000);
        }
        else if(DispGetEdiQtwFlag()) //0629
        {
            if (0 != m_aUpHistoryEco[UP_SPEED].fShowInfo)
            {
                update_up_msg(UP_SPEED,0);
                m_aUpHistoryEco[UP_SPEED].fShowInfo = 0;
            }
            update_tank_msg(CIR_SPEED,toOneDecimal(fUnit*(iValue*1.0)/1000));
            m_aTankHistoryEco[CIR_SPEED].fShowInfo = ((iValue*1.0)/1000);
        }
        else
        {
            if (0 != m_aUpHistoryEco[UP_SPEED].fShowInfo) 
            {
                update_up_msg(UP_SPEED,0);
                m_aUpHistoryEco[UP_SPEED].fShowInfo = 0;    
            }
            if (0 != m_aTankHistoryEco[CIR_SPEED].fShowInfo) 
            {
                update_tank_msg(CIR_SPEED,0);
                m_aTankHistoryEco[CIR_SPEED].fShowInfo = 0;    
            }            
        }
        break;
   }
   case APP_FM_FM2_NO:
        update_edi_msg(RO_WATER_IN_SPEED,toTwoDecimal(fUnit*(60.0*iValue)/1000));
        m_aEDIHistoryEco[RO_WATER_IN_SPEED].fShowInfo = ((60.0*iValue)/1000);
        break;
   case APP_FM_FM3_NO:
        update_edi_msg(RO_PRODUCT_WATER_SPEED,toTwoDecimal(fUnit*(60.0*iValue)/1000));
        m_aEDIHistoryEco[RO_PRODUCT_WATER_SPEED].fShowInfo = ((60.0*iValue)/1000);
        
        update_edi_msg(TAP_WATER_SPEED,toTwoDecimal(fUnit*(60.0*(m_aulFlowMeter[APP_FM_FM3_NO] + m_aulFlowMeter[APP_FM_FM4_NO]))/1000));
        m_aEDIHistoryEco[TAP_WATER_SPEED].fShowInfo = ((60.0*(m_aulFlowMeter[APP_FM_FM3_NO] + m_aulFlowMeter[APP_FM_FM4_NO]))/1000);

        update_edi_msg(EDI_PROC_WATER_SPEED,toTwoDecimal(fUnit*(60.0*0.8*iValue)/1000)); 
        m_aEDIHistoryEco[EDI_PROC_WATER_SPEED].fShowInfo = ((60.0*0.8*iValue)/1000);

        update_edi_msg(EDI_DISCARD_WATER_SPEED,toTwoDecimal(fUnit*(60.0*0.2*iValue)/1000));
        m_aEDIHistoryEco[EDI_DISCARD_WATER_SPEED].fShowInfo = ((60.0*0.2*iValue)/1000);
        break;
   case APP_FM_FM4_NO:
        update_edi_msg(RO_DISCARD_WATER_SPEED,toTwoDecimal(fUnit*(60.0*iValue)/1000));
        m_aEDIHistoryEco[RO_DISCARD_WATER_SPEED].fShowInfo = ((60.0*iValue)/1000);

        update_edi_msg(TAP_WATER_SPEED,toTwoDecimal(fUnit*(60.0*(m_aulFlowMeter[APP_FM_FM3_NO] + m_aulFlowMeter[APP_FM_FM4_NO]))/1000));
        m_aEDIHistoryEco[TAP_WATER_SPEED].fShowInfo = ((60.0*(m_aulFlowMeter[APP_FM_FM3_NO] + m_aulFlowMeter[APP_FM_FM4_NO]))/1000);
        break;
   }
}

void WaterQualityPage::updTank(int iLevel,float fVolume)
{
    update_tank_msg(TANK_LEVEL,iLevel);
    m_aTankHistoryEco[TANK_LEVEL].fShowInfo = iLevel;
    
    update_tank_msg(TANK_VALUE,fVolume);
    m_aTankHistoryEco[TANK_VALUE].fShowInfo = fVolume;
}

void WaterQualityPage::update_tank_msg(int isIdx,float fValue)
{
    QString tmpMsg , Name , Value;

    QTextCursor cursor ;

    int iIdx = aTsIndex[isIdx];

    if (!isVisible())
    {
        return;
    }

    if (iIdx >= m_aiRealNum[WQ_TYPE_TANK])
    {
        return;
    }
    

    m_edit4Tank[iIdx]->setText("");

    cursor = m_edit4Tank[iIdx]->textCursor();
    
    switch(isIdx)
    {
    case TANK_LEVEL:
    case TANK_VALUE:
        {
            if (!(gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_HaveB2)))
            {
                m_edit4Tank[iIdx]->hide();
            }
            else
            {
                m_edit4Tank[iIdx]->show();
                tmpMsg = tankmsg[iIdx].arg((int)fValue);
            }
        }
        break;
    case CIR_WATER_QUA:
        if(1 > fValue)
        {
            tmpMsg = tankmsg[iIdx].arg(fValue,0,'f',3);
        }
        else
        {
            tmpMsg = tankmsg[iIdx].arg(fValue,0,'f',1);
        }
        break;
    case CIR_SPEED:         
        tmpMsg = tankmsg[iIdx].arg(fValue,0,'f',1);
        break;
    default:           
        tmpMsg = tankmsg[iIdx].arg((int)fValue);
        break;
    }

    cursor.movePosition(QTextCursor::Start,QTextCursor::MoveAnchor);

    m_edit4Tank[iIdx]->setTextCursor(cursor);

    if(-1 == tmpMsg.indexOf(":"))
    {
        Name = tmpMsg;

        Value = "";
    }
    else
    {
        Name = tmpMsg.mid(0, tmpMsg.indexOf(":")+1);

        Value = tmpMsg.mid(tmpMsg.indexOf(":") + 1) ;
    }

    cursor.setCharFormat(charFormatName);
    cursor.insertText(Name); 
    
    cursor.setCharFormat(charFormatValue);
    cursor.insertText(Value); 
}

void WaterQualityPage::update_edi_msg(int isIdx,float fvalue)
{
    QString tmpMsg , Name , Value;
    QTextCursor cursor ;

    int iIdx = aEsIndex[isIdx];

    if (!isVisible())
    {
        return;
    }

    if (iIdx >= m_aiRealNum[WQ_TYPE_EDI])
    {
        return;
    }

     m_edit4Edi[iIdx]->setText("");
   
     cursor = m_edit4Edi[iIdx]->textCursor();
   
     switch(isIdx)
     {
     case TAP_WATER_QUALITY:
     case RO_WATER_IN_QUALITY:
         tmpMsg = edimsg[iIdx].arg((int)fvalue);
         break;
     case EDI_PRODUCT_QUALITY:
        if(1 > fvalue)
        {
            tmpMsg = edimsg[iIdx].arg(fvalue,0,'f',3);
        }
        else
        {
            // > 16显示 >16
            if(fvalue > 16)
            {
                tmpMsg = edimsg[iIdx].arg(">16");
            }
            else
            {
                tmpMsg = edimsg[iIdx].arg(fvalue,0,'f',1);
            }
//            tmpMsg = edimsg[iIdx].arg(fvalue,0,'f',1);
        }
        break;
     default:
         tmpMsg = edimsg[iIdx].arg(fvalue,0,'f',1);
         break;
     }
   
     if(-1 == tmpMsg.indexOf(":"))
     {
         Name = tmpMsg;
   
         Value = "";
     }
     else
     {
         Name = tmpMsg.mid(0, tmpMsg.indexOf(":")+1);
   
         Value = tmpMsg.mid(tmpMsg.indexOf(":") + 1) ;
     }
   
     cursor.movePosition(QTextCursor::Start,QTextCursor::MoveAnchor);
     
      m_edit4Edi[iIdx]->setTextCursor(cursor);
   
     cursor.setCharFormat(charFormatName);
     cursor.insertText(Name);
     
     cursor.setCharFormat(charFormatValue);
     cursor.insertText(Value);

}

void WaterQualityPage::init_up_msg(void)
{

    QString tmpMsg , Name , Value;

    QTextCursor cursor;

    for(int iLoop = 0 ; iLoop <  m_aiRealNum[WQ_TYPE_UP] ; iLoop++)
    {
        cursor = m_edit4Up[iLoop]->textCursor();

        switch(aUsIds[iLoop].iId)
        {
        case UP_TOC:
            tmpMsg = upmsg[iLoop].arg(3);
            break;
        case UP_IN_QUA:
        case UP_WATER:
            tmpMsg = upmsg[iLoop].arg(18.2,0,'f',1);
            break;
        case UP_PRODUCT_TEMT:
            tmpMsg = upmsg[iLoop].arg(25.0,0,'f',1);
            break;
        default:
            tmpMsg = upmsg[iLoop].arg(0.00,0,'f',1);
            break;
        }

        if(-1 == tmpMsg.indexOf(":"))
        {
            Name = tmpMsg;

            Value = "";
        }
        else
        {
            Name = tmpMsg.mid(0, tmpMsg.indexOf(":")+1);

            Value = tmpMsg.mid(tmpMsg.indexOf(":") + 1) ;
        }

        cursor.setCharFormat(charFormatName);
        cursor.insertText(Name); 
        
        cursor.setCharFormat(charFormatValue);
        cursor.insertText(Value); 
    }

}

void WaterQualityPage::init_tank_msg()
{
    QString tmpMsg , Name , Value;
    QTextCursor cursor ;

    for(int iLoop = 0 ; iLoop <  m_aiRealNum[WQ_TYPE_TANK] ; iLoop++)
    {
        cursor = m_edit4Tank[iLoop]->textCursor();
    
        switch(aTsIds[iLoop].iId)
        {
        case CIR_WATER_QUA:
            tmpMsg = tankmsg[iLoop].arg(18.2,0,'f',1);
            break;
        case CIR_SPEED:         
            tmpMsg = tankmsg[iLoop].arg(0.00,0,'f',1);
            break;
        default:           
            tmpMsg = tankmsg[iLoop].arg(0);
            break;
        }

        if(-1 == tmpMsg.indexOf(":"))
        {
            Name = tmpMsg;

            Value = "";
        }
        else
        {
            Name = tmpMsg.mid(0, tmpMsg.indexOf(":")+1);

            Value = tmpMsg.mid(tmpMsg.indexOf(":") + 1) ;
        }

        cursor.setCharFormat(charFormatName);
        cursor.insertText(Name); 
        
        cursor.setCharFormat(charFormatValue);
        cursor.insertText(Value); 
    }
}

void WaterQualityPage::init_edi_msg()
{
    QString tmpMsg , Name , Value;

    QTextCursor cursor ;

    for(int iLoop = 0 ; iLoop < m_aiRealNum[WQ_TYPE_EDI] ; iLoop++)
    {
        cursor = m_edit4Edi[iLoop]->textCursor();
    
        switch(aEsIds[iLoop].iId)
        {
        case TAP_WATER_QUALITY:
        case RO_WATER_IN_QUALITY:
            tmpMsg = edimsg[iLoop].arg(0);
            break;
        default:
            tmpMsg = edimsg[iLoop].arg(0.00,0,'f',1);
            break;
        }

        if(-1 == tmpMsg.indexOf(":"))
        {
            Name = tmpMsg;

            Value = "";
        }
        else
        {
            Name = tmpMsg.mid(0, tmpMsg.indexOf(":")+1);

            Value = tmpMsg.mid(tmpMsg.indexOf(":") + 1) ;
        }

        cursor.setCharFormat(charFormatName);
        cursor.insertText(Name); 
        cursor.setCharFormat(charFormatValue);
        cursor.insertText(Value); 
    }

}


void WaterQualityPage::setBackColor()
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

void WaterQualityPage::update()
{
    //update_edi_msg(TAP_WATER_PRESSURE,toTwoDecimal(m_wndMain->m_fSourceWaterPressure));
    
    //update_edi_msg(TAP_WATER_QUALITY,m_wndMain->m_fSourceWaterConductivity);

    updAllInfo();

    
}

void WaterQualityPage::initUi()
{
    int iLoop;

    int width,height;

    QRect tmpRect ;

    QFont fontName("Arial",14,QFont::Bold);

    QFont fontValue("Arial",14,0);

    QString colors[] = {"background-color:rgb(200,200,188)","background-color:rgb(228, 231, 240)"};

    setBackColor();

    charFormatName.setFont(fontName);
    
    charFormatValue.setFont(fontValue);
    

    for(iLoop = 0 ; iLoop < WQ_TYPE_NUM ; iLoop++)
    {
        if(MACHINE_ADAPT != gGlobalParam.iMachineType)
        {
            m_lbFrame[iLoop] = new QLabel(m_widget);
            m_lbFrame[iLoop]->setGeometry(wqeditstru[iLoop]);
        }
        else
        {
            m_lbFrame[WQ_TYPE_TANK] = new QLabel(m_widget);
            m_lbFrame[WQ_TYPE_TANK]->setGeometry(wqeditstru[WQ_TYPE_UP]);
            
            m_lbFrame[WQ_TYPE_UP] = new QLabel(m_widget);
            m_lbFrame[WQ_TYPE_UP]->setGeometry(wqeditstru[WQ_TYPE_TANK]);

            m_lbFrame[WQ_TYPE_EDI] = new QLabel(m_widget);
            m_lbFrame[WQ_TYPE_EDI]->setGeometry(wqeditstru[WQ_TYPE_EDI]);

            iLoop = WQ_TYPE_NUM ;
        }

        //m_lbFrame[iLoop]->setStyleSheet("background-color:rgb(234, 237, 246)"); 

    }

    width  = wqeditstru[WQ_TYPE_UP].width();
    height = wqeditstru[WQ_TYPE_UP].height() / UP_NUM;

    for(iLoop = 0 ; iLoop <  m_aiRealNum[WQ_TYPE_UP] ; iLoop++)
    {
        tmpRect.setX(0);
        tmpRect.setY(iLoop * height);
        tmpRect.setWidth(width);
        tmpRect.setHeight(height);
        m_edit4Up[iLoop] = new QTextEdit(m_lbFrame[WQ_TYPE_UP]);
        m_edit4Up[iLoop]->setGeometry(tmpRect);
        //m_edit4Up[iLoop]->setStyleSheet("background-color:transparent");
        m_edit4Up[iLoop]->setStyleSheet(colors[iLoop % 2]); 
            
        m_edit4Up[iLoop]->setReadOnly(true);
    }

    width  = wqeditstru[WQ_TYPE_TANK].width();
    height = wqeditstru[WQ_TYPE_TANK].height() / TANK_NUM;

    for(iLoop = 0 ; iLoop < m_aiRealNum[WQ_TYPE_TANK] ; iLoop++)
    {
        tmpRect.setX(0);
        tmpRect.setY(iLoop * height);
        tmpRect.setWidth(width);
        tmpRect.setHeight(height);
        m_edit4Tank[iLoop] = new QTextEdit(m_lbFrame[WQ_TYPE_TANK]);
        m_edit4Tank[iLoop]->setGeometry(tmpRect);
        //m_edit4Tank[iLoop]->setStyleSheet("background-color:transparent"); 
        m_edit4Tank[iLoop]->setStyleSheet(colors[iLoop % 2]); 
        
        m_edit4Tank[iLoop]->setReadOnly(true);
    }    

    width  = wqeditstru[WQ_TYPE_EDI].width();
    height = wqeditstru[WQ_TYPE_EDI].height() / EDI_NUM;
    
    for(iLoop = 0 ; iLoop < m_aiRealNum[WQ_TYPE_EDI] ; iLoop++)
    {
        tmpRect.setX(0);
        tmpRect.setY(iLoop * height);
        tmpRect.setWidth(width);
        tmpRect.setHeight(height);
        m_edit4Edi[iLoop] = new QTextEdit(m_lbFrame[WQ_TYPE_EDI]);
        m_edit4Edi[iLoop]->setGeometry(tmpRect);
        //m_edit4Edi[iLoop]->setStyleSheet("background-color:transparent"); 
        m_edit4Edi[iLoop]->setStyleSheet(colors[iLoop % 2]); 
        
        m_edit4Edi[iLoop]->setReadOnly(true);
    }    


}
