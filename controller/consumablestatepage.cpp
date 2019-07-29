#include "consumablestatepage.h"
#include "Cslistitem.h"
#include "mainwindow.h"
#include <QListWidget>

ConsumableStatePage::ConsumableStatePage(QObject *parent,CBaseWidget *widget ,MainWindow *wndMain)
    : CSubPage(parent,widget,wndMain)
{
    int iIdx = 0;

    if (gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_Pre_Filter)) //DISP_SM_PreFilterColumn
    {
        aIds[iIdx].iType = 0;
        aIds[iIdx].iId   = DISP_PRE_PACK;
        iIdx++;
    }
    //2018.10.22 ADD
    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_L_EDI_LOOP:
    case MACHINE_L_RO_LOOP:
    case MACHINE_Genie:
    case MACHINE_UP:
    case MACHINE_EDI:
    case MACHINE_RO:
        aIds[iIdx].iType = 0;
        aIds[iIdx].iId   = DISP_AC_PACK;
        iIdx++;
        break;
    case MACHINE_PURIST:
    case MACHINE_ADAPT:
        break;
    }

    //2018 add T-Pack
    if(gGlobalParam.MiscParam.ulMisFlags & (1 << DISP_SM_HP_Water_Cir))
    {
        aIds[iIdx].iType = 0;
        aIds[iIdx].iId   = DISP_T_PACK;
        iIdx++;
    }

    switch(gGlobalParam.iMachineType)
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
        aIds[iIdx].iType = 0;
        aIds[iIdx].iId   = DISP_P_PACK;
        iIdx++;
        break;
    case MACHINE_PURIST:
        break;
    }

    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_L_EDI_LOOP:
        aIds[iIdx].iType = 0;
        aIds[iIdx].iId   = DISP_AT_PACK;
        iIdx++;
        break;
    case MACHINE_L_UP:
    case MACHINE_L_RO_LOOP:
    case MACHINE_Genie:
    case MACHINE_UP:
    case MACHINE_EDI:
    case MACHINE_RO:
    case MACHINE_PURIST:
    case MACHINE_ADAPT:
        break;
    }

    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
        break;
    case MACHINE_L_UP:
    case MACHINE_L_EDI_LOOP:
    case MACHINE_L_RO_LOOP:
    case MACHINE_UP:
    case MACHINE_PURIST:
        aIds[iIdx].iType = 0;
        aIds[iIdx].iId   = DISP_H_PACK;
        iIdx++;
        break;
    case MACHINE_Genie:
    case MACHINE_EDI:
    case MACHINE_RO:
    case MACHINE_ADAPT:
        break;
    }

    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_Genie:
    case MACHINE_UP:
    case MACHINE_PURIST:
    case MACHINE_ADAPT:
          aIds[iIdx].iType = 0;
          aIds[iIdx].iId   = DISP_U_PACK;
          iIdx++;
        break;
    case MACHINE_L_EDI_LOOP:
    case MACHINE_L_RO_LOOP:
    case MACHINE_EDI:
    case MACHINE_RO:
        break;
    }

    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_L_EDI_LOOP:
    case MACHINE_L_RO_LOOP:
    case MACHINE_Genie:
    case MACHINE_EDI:
          aIds[iIdx].iType = 0;
          aIds[iIdx].iId   = DISP_N1_UV;
          iIdx++;
        break;
    case MACHINE_UP:
    case MACHINE_PURIST:
    case MACHINE_ADAPT:
    case MACHINE_RO:
        break;
    }
   
   switch(gGlobalParam.iMachineType)
   {
   case MACHINE_L_Genie:
   case MACHINE_L_UP:
   case MACHINE_L_EDI_LOOP:
   case MACHINE_L_RO_LOOP:
   case MACHINE_Genie:
   case MACHINE_UP:
   case MACHINE_PURIST:
   case MACHINE_ADAPT:
         aIds[iIdx].iType = 0;
         aIds[iIdx].iId   = DISP_N2_UV;
         iIdx++;
       break;
   case MACHINE_EDI:
   case MACHINE_RO:
       break;
   }

    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_L_EDI_LOOP:
    case MACHINE_L_RO_LOOP:
    case MACHINE_Genie:
    case MACHINE_UP:
    case MACHINE_EDI:
    case MACHINE_RO:
          aIds[iIdx].iType = 0;
          aIds[iIdx].iId   = DISP_N3_UV;
          iIdx++;
        break;
    case MACHINE_PURIST:
    case MACHINE_ADAPT:
        break;
    }
  //0628
    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_L_EDI_LOOP:
    case MACHINE_L_RO_LOOP:
    {
        if (gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_TubeUV))
        {
            aIds[iIdx].iType = 0;
            aIds[iIdx].iId   = DISP_N4_UV;
            iIdx++;
        }
        break;
    }
    case MACHINE_UP:
    case MACHINE_PURIST:
    case MACHINE_ADAPT:
    case MACHINE_Genie:
    case MACHINE_RO:
    case MACHINE_EDI:
        break;
    }
    
/*2018/05/21 TOC UV*/
#if 0
    switch(gGlobalParam.iMachineType) /*TOC UV*/
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
        {
            if (gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_HaveTOC))
            {
                aIds[iIdx].iType = 0;
                aIds[iIdx].iId   = DISP_N5_UV;
                iIdx++;
            }
        }
        break;
    }
#endif
    
    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_L_EDI_LOOP:
    case MACHINE_L_RO_LOOP:
    case MACHINE_Genie:
    case MACHINE_UP:
    case MACHINE_EDI:
    case MACHINE_RO:
          aIds[iIdx].iType = 0;
          aIds[iIdx].iId   = DISP_W_FILTER;
          iIdx++;
        break;
    case MACHINE_PURIST:
    case MACHINE_ADAPT:
        break;
    }

    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_Genie:
    case MACHINE_UP:
    case MACHINE_ADAPT:
          aIds[iIdx].iType = 0;
          aIds[iIdx].iId   = DISP_T_B_FILTER;
          iIdx++;
        break;
    case MACHINE_L_EDI_LOOP:
    case MACHINE_L_RO_LOOP:
    case MACHINE_EDI:
    case MACHINE_RO:
    case MACHINE_PURIST:
        break;
    }

    switch(gGlobalParam.iMachineType)
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
          aIds[iIdx].iType = 0;
          aIds[iIdx].iId   = DISP_T_A_FILTER;
          iIdx++;
        break;
    }
    //0628
    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_L_EDI_LOOP:
    case MACHINE_L_RO_LOOP:
    {
        if (gGlobalParam.SubModSetting.ulFlags & DISP_SM_HaveTubeFilter)
        {
            aIds[iIdx].iType = 0;
            aIds[iIdx].iId   = DISP_TUBE_FILTER;
            iIdx++;
        }

        if (gGlobalParam.SubModSetting.ulFlags & DISP_SM_TubeDI)
        {
            aIds[iIdx].iType = 0;
            aIds[iIdx].iId   = DISP_TUBE_DI;
            iIdx++;
        }
        break;
    }
    case MACHINE_Genie:
    case MACHINE_ADAPT:
    case MACHINE_RO:
    case MACHINE_EDI:
    case MACHINE_UP:
    case MACHINE_PURIST:
        break;
    }

    m_realCsNum = iIdx;

    creatTitle();

    initUi();
    
    buildTranslation();

}

void ConsumableStatePage::creatTitle()
{
    CSubPage::creatTitle();

    buildTitles();

    selectTitle(0);
}

void ConsumableStatePage::buildTitles()
{
    QStringList stringList;

    stringList << tr("Consumables Status");

    setTitles(stringList);
}

void ConsumableStatePage::buildTranslation()
{
    
}

static QString decodeTime(unsigned int ulTime)
{
    QString strTime;
    time_t timer = (time_t)ulTime;
    struct tm *tblock = localtime(&timer);

    strTime.sprintf("%04d-%02d-%02d",tblock->tm_year + 1900,tblock->tm_mon + 1,tblock->tm_mday);

    return strTime;
}

static QString decodeDays(unsigned int ulDay)
{
    QString strTime;

    strTime.sprintf("%d",ulDay);

    return strTime;
}


void ConsumableStatePage:: update()
{
    int tmp;
    QString strTmp;
    int iIdx = 0;

    QString astrNames[] = {
        tr(" "),
        tr(" "),
    };
   
    for (iIdx = 0; iIdx < m_realCsNum; iIdx++)
    {
        switch(aIds[iIdx].iId)
        {
        case DISP_PRE_PACK:
            /* for preprocess column */
            tmp = gCMUsage.info.aulCms[DISP_PRE_PACKLIFEL] ;
            strTmp = astrNames[0] + "  " + QString::number(tmp) + "L";
            m_pCslistItem[iIdx]->setValue(strTmp);
        
            strTmp = tr("Installation Date ") + decodeTime(gCMUsage.info.aulCms[DISP_PRE_PACKLIFEDAY]);
            m_pCslistItem[iIdx]->setInstDate(strTmp);
        
            tmp = gGlobalParam.CMParam.aulCms[DISP_PRE_PACKLIFEDAY] - 
                (DispGetCurSecond() - gCMUsage.info.aulCms[DISP_PRE_PACKLIFEDAY])/ DISP_DAYININSECOND;
            strTmp = tr("Replace in ") + decodeDays(tmp) + QString(" ")+ tr("days");
            m_pCslistItem[iIdx]->setChangeDate(strTmp);

            strTmp = tr("Cat No.:") + gGlobalParam.cmSn.aCn[DISP_PRE_PACK];
            m_pCslistItem[iIdx]->setCatNo(strTmp);

            strTmp = tr("Lot No.:") + gGlobalParam.cmSn.aLn[DISP_PRE_PACK];
            m_pCslistItem[iIdx]->setLotNo(strTmp);

            
            if (gCMUsage.ulUsageState & (1 << DISP_PRE_PACKLIFEDAY) 
                || gCMUsage.ulUsageState & (1 << DISP_PRE_PACKLIFEL))
            {
                m_pCslistItem[iIdx]->updateState(1);
            }
            else
            {
                m_pCslistItem[iIdx]->updateState(0);
            } 
            
            /*Preporcess column */
            m_pCslistItem[iIdx]->setName(tr("Prefilter"));
            break;
        case DISP_AC_PACK:
            /* for preprocess column */
            tmp = gCMUsage.info.aulCms[DISP_AC_PACKLIFEL] ;
            strTmp = astrNames[0] + "  " + QString::number(tmp) + "L";
            m_pCslistItem[iIdx]->setValue(strTmp);

            strTmp = tr("Installation Date ") + decodeTime(gCMUsage.info.aulCms[DISP_AC_PACKLIFEDAY]);
            m_pCslistItem[iIdx]->setInstDate(strTmp);

            tmp = gGlobalParam.CMParam.aulCms[DISP_AC_PACKLIFEDAY] -
                (DispGetCurSecond() - gCMUsage.info.aulCms[DISP_AC_PACKLIFEDAY])/ DISP_DAYININSECOND;
            strTmp = tr("Replace in ") + decodeDays(tmp) + QString(" ")+ tr("days");
            m_pCslistItem[iIdx]->setChangeDate(strTmp);

            strTmp = tr("Cat No.:") + gGlobalParam.cmSn.aCn[DISP_AC_PACK];
            m_pCslistItem[iIdx]->setCatNo(strTmp);

            strTmp = tr("Lot No.:") + gGlobalParam.cmSn.aLn[DISP_AC_PACK];
            m_pCslistItem[iIdx]->setLotNo(strTmp);


            if (gCMUsage.ulUsageState & (1 << DISP_AC_PACKLIFEDAY)
                || gCMUsage.ulUsageState & (1 << DISP_AC_PACKLIFEL))
            {
                m_pCslistItem[iIdx]->updateState(1);
            }
            else
            {
                m_pCslistItem[iIdx]->updateState(0);
            }
            /*Preporcess column */
            m_pCslistItem[iIdx]->setName(tr("AC Pack"));
            break;
        case DISP_T_PACK:
            /* T-Pack */

            /*
            tmp = gCMUsage.info.aulCms[DISP_T_PACKLIFEL] ;
            strTmp = astrNames[0] + "  " + QString::number(tmp) + "L";
            m_pCslistItem[iIdx]->setValue(strTmp);
            */

            strTmp = tr("Installation Date ") + decodeTime(gCMUsage.info.aulCms[DISP_T_PACKLIFEDAY]);
            m_pCslistItem[iIdx]->setInstDate(strTmp);

            tmp = gGlobalParam.CMParam.aulCms[DISP_T_PACKLIFEDAY] -
                (DispGetCurSecond() - gCMUsage.info.aulCms[DISP_T_PACKLIFEDAY])/ DISP_DAYININSECOND;
            strTmp = tr("Replace in ") + decodeDays(tmp) + QString(" ")+ tr("days");
            m_pCslistItem[iIdx]->setChangeDate(strTmp);

            strTmp = tr("Cat No.:") + gGlobalParam.cmSn.aCn[DISP_T_PACK];
            m_pCslistItem[iIdx]->setCatNo(strTmp);

            strTmp = tr("Lot No.:") + gGlobalParam.cmSn.aLn[DISP_T_PACK];
            m_pCslistItem[iIdx]->setLotNo(strTmp);


            if (gCMUsage.ulUsageState & (1 << DISP_T_PACKLIFEDAY)
                || gCMUsage.ulUsageState & (1 << DISP_T_PACKLIFEL))
            {
                m_pCslistItem[iIdx]->updateState(1);
            }
            else
            {
                m_pCslistItem[iIdx]->updateState(0);
            }
            m_pCslistItem[iIdx]->setName(tr("T Pack"));
            break;
        case DISP_P_PACK:
        
            /* for DISP_P_PACK column */
            tmp = gCMUsage.info.aulCms[DISP_P_PACKLIFEL] ;
            strTmp = astrNames[0] + "  " + QString::number(tmp) + "L";
            m_pCslistItem[iIdx]->setValue(strTmp);
            
            strTmp = tr("Installation Date ") + decodeTime(gCMUsage.info.aulCms[DISP_P_PACKLIFEDAY]);
            m_pCslistItem[iIdx]->setInstDate(strTmp);
            
            tmp = gGlobalParam.CMParam.aulCms[DISP_P_PACKLIFEDAY] - 
                (DispGetCurSecond() - gCMUsage.info.aulCms[DISP_P_PACKLIFEDAY])/ DISP_DAYININSECOND;
            strTmp = tr("Replace in ") + decodeDays(tmp) + QString(" ") + tr("days");
            m_pCslistItem[iIdx]->setChangeDate(strTmp);
            
            strTmp = tr("Cat No.:") + gGlobalParam.cmSn.aCn[DISP_P_PACK];
            m_pCslistItem[iIdx]->setCatNo(strTmp);
            
            strTmp = tr("Lot No.:") + gGlobalParam.cmSn.aLn[DISP_P_PACK];
            m_pCslistItem[iIdx]->setLotNo(strTmp);
                
            if (gCMUsage.ulUsageState & (1 << DISP_P_PACKLIFEDAY) 
                || gCMUsage.ulUsageState & (1 << DISP_P_PACKLIFEL))
            {
                m_pCslistItem[iIdx]->updateState(1);
            }
            else
            {
                m_pCslistItem[iIdx]->updateState(0);
            }
            m_pCslistItem[iIdx]->setName(tr("P PACK "));
            break; 
        case DISP_U_PACK:
            
            /* for DISP_U_PACK column */
            tmp = gCMUsage.info.aulCms[DISP_U_PACKLIFEL] ;
            strTmp = astrNames[0] + "  " + QString::number(tmp) + "L";
            m_pCslistItem[iIdx]->setValue(strTmp);
            
            strTmp = tr("Installation Date ") + decodeTime(gCMUsage.info.aulCms[DISP_U_PACKLIFEDAY]);
            m_pCslistItem[iIdx]->setInstDate(strTmp);
            
            tmp = gGlobalParam.CMParam.aulCms[DISP_U_PACKLIFEDAY] - 
                (DispGetCurSecond() - gCMUsage.info.aulCms[DISP_U_PACKLIFEDAY])/ DISP_DAYININSECOND;
            strTmp = tr("Replace in ") + decodeDays(tmp) + QString(" ") + tr("days");
            m_pCslistItem[iIdx]->setChangeDate(strTmp);
            
            strTmp = tr("Cat No.:") + gGlobalParam.cmSn.aCn[DISP_U_PACK];
            m_pCslistItem[iIdx]->setCatNo(strTmp);
            
            strTmp = tr("Lot No.:") + gGlobalParam.cmSn.aLn[DISP_U_PACK];
            m_pCslistItem[iIdx]->setLotNo(strTmp);
                
            if (gCMUsage.ulUsageState & (1 << DISP_U_PACKLIFEDAY) 
                || gCMUsage.ulUsageState & (1 << DISP_U_PACKLIFEL))
            {
                m_pCslistItem[iIdx]->updateState(1);
            }
            else
            {
                m_pCslistItem[iIdx]->updateState(0);
            }
            m_pCslistItem[iIdx]->setName(tr("U PACK "));
            break;     
        case DISP_AT_PACK:
            
            /* for DISP_AT_PACK column */
            tmp = gCMUsage.info.aulCms[DISP_AT_PACKLIFEL] ;
            strTmp = astrNames[0] + "  " + QString::number(tmp) + "L";
            m_pCslistItem[iIdx]->setValue(strTmp);
            
            strTmp = tr("Installation Date ") + decodeTime(gCMUsage.info.aulCms[DISP_AT_PACKLIFEDAY]);
            m_pCslistItem[iIdx]->setInstDate(strTmp);
            
            tmp = gGlobalParam.CMParam.aulCms[DISP_AT_PACKLIFEDAY] - 
                (DispGetCurSecond() - gCMUsage.info.aulCms[DISP_AT_PACKLIFEDAY])/ DISP_DAYININSECOND;
            strTmp = tr("Replace in ") + decodeDays(tmp) + QString(" ") + tr("days");
            m_pCslistItem[iIdx]->setChangeDate(strTmp);
            
            strTmp = tr("Cat No.:") + gGlobalParam.cmSn.aCn[DISP_AT_PACK];
            m_pCslistItem[iIdx]->setCatNo(strTmp);
            
            strTmp = tr("Lot No.:") + gGlobalParam.cmSn.aLn[DISP_AT_PACK];
            m_pCslistItem[iIdx]->setLotNo(strTmp);
                
            if (gCMUsage.ulUsageState & (1 << DISP_AT_PACKLIFEDAY) 
                || gCMUsage.ulUsageState & (1 << DISP_AT_PACKLIFEL))
            {
                m_pCslistItem[iIdx]->updateState(1);
            }
            else
            {
                m_pCslistItem[iIdx]->updateState(0);
            }
            m_pCslistItem[iIdx]->setName(tr("AT PACK "));
            break;  
        case DISP_H_PACK:
            
            /* for DISP_AT_PACK column */
            tmp = gCMUsage.info.aulCms[DISP_H_PACKLIFEL] ;
            strTmp = astrNames[0] + "  " + QString::number(tmp) + "L";
            m_pCslistItem[iIdx]->setValue(strTmp);
            
            strTmp = tr("Installation Date ") + decodeTime(gCMUsage.info.aulCms[DISP_H_PACKLIFEDAY]);
            m_pCslistItem[iIdx]->setInstDate(strTmp);
            
            tmp = gGlobalParam.CMParam.aulCms[DISP_H_PACKLIFEDAY] - 
                (DispGetCurSecond() - gCMUsage.info.aulCms[DISP_H_PACKLIFEDAY])/ DISP_DAYININSECOND;
            strTmp = tr("Replace in ") + decodeDays(tmp) + QString(" ") + tr("days");
            m_pCslistItem[iIdx]->setChangeDate(strTmp);
            
            strTmp = tr("Cat No.:") + gGlobalParam.cmSn.aCn[DISP_H_PACK];
            m_pCslistItem[iIdx]->setCatNo(strTmp);
            
            strTmp = tr("Lot No.:") + gGlobalParam.cmSn.aLn[DISP_H_PACK];
            m_pCslistItem[iIdx]->setLotNo(strTmp);
                
            if (gCMUsage.ulUsageState & (1 << DISP_H_PACKLIFEDAY) 
                || gCMUsage.ulUsageState & (1 << DISP_H_PACKLIFEL))
            {
                m_pCslistItem[iIdx]->updateState(1);
            }
            else
            {
                m_pCslistItem[iIdx]->updateState(0);
            }
            m_pCslistItem[iIdx]->setName(tr("H PACK "));
            break;              
        case DISP_N1_UV:
            
            /* for DISP_N1_UV column */
            tmp = gCMUsage.info.aulCms[DISP_N1_UVLIFEHOUR] ;
            strTmp = astrNames[1] + "  " + QString::number(tmp) + "H";
            m_pCslistItem[iIdx]->setValue(strTmp);
            
            strTmp = tr("Installation Date ") + decodeTime(gCMUsage.info.aulCms[DISP_N1_UVLIFEDAY]);
            m_pCslistItem[iIdx]->setInstDate(strTmp);
            
            tmp = gGlobalParam.CMParam.aulCms[DISP_N1_UVLIFEDAY] - 
                (DispGetCurSecond() - gCMUsage.info.aulCms[DISP_N1_UVLIFEDAY])/ DISP_DAYININSECOND;
            strTmp = tr("Replace in ") + decodeDays(tmp) + QString(" ") + tr("days");
            m_pCslistItem[iIdx]->setChangeDate(strTmp);

            strTmp = tr("Cat No.:") + gGlobalParam.cmSn.aCn[DISP_N1_UV];
            m_pCslistItem[iIdx]->setCatNo(strTmp);
            
            strTmp = tr("Lot No.:") + gGlobalParam.cmSn.aLn[DISP_N1_UV];
            m_pCslistItem[iIdx]->setLotNo(strTmp);
            
            if (gCMUsage.ulUsageState & (1 << DISP_N1_UVLIFEDAY) 
                || gCMUsage.ulUsageState & (1 << DISP_N1_UVLIFEHOUR))
            {
                m_pCslistItem[iIdx]->updateState(1);
            }
            else
            {
                m_pCslistItem[iIdx]->updateState(0);
            }
            m_pCslistItem[iIdx]->setName(tr("254 UV"));
            break;  
        case DISP_N2_UV:
            
            /* for DISP_N2_UV column */
            tmp = gCMUsage.info.aulCms[DISP_N2_UVLIFEHOUR] ;
            strTmp = astrNames[1] + "  " + QString::number(tmp) + "H";
            m_pCslistItem[iIdx]->setValue(strTmp);
            
            strTmp = tr("Installation Date ") + decodeTime(gCMUsage.info.aulCms[DISP_N2_UVLIFEDAY]);
            m_pCslistItem[iIdx]->setInstDate(strTmp);
            
            tmp = gGlobalParam.CMParam.aulCms[DISP_N2_UVLIFEDAY] - 
                (DispGetCurSecond() - gCMUsage.info.aulCms[DISP_N2_UVLIFEDAY])/ DISP_DAYININSECOND;
            strTmp = tr("Replace in ") + decodeDays(tmp) + QString(" ") + tr("days");
            m_pCslistItem[iIdx]->setChangeDate(strTmp);

            strTmp = tr("Cat No.:") + gGlobalParam.cmSn.aCn[DISP_N2_UV];
            m_pCslistItem[iIdx]->setCatNo(strTmp);
            
            strTmp = tr("Lot No.:") + gGlobalParam.cmSn.aLn[DISP_N2_UV];
            m_pCslistItem[iIdx]->setLotNo(strTmp);
            
            if (gCMUsage.ulUsageState & (1 << DISP_N2_UVLIFEDAY) 
                || gCMUsage.ulUsageState & (1 << DISP_N2_UVLIFEHOUR))
            {
                m_pCslistItem[iIdx]->updateState(1);
            }
            else
            {
                m_pCslistItem[iIdx]->updateState(0);
            }
            m_pCslistItem[iIdx]->setName(tr("185 UV"));
            break;  
        case DISP_N3_UV:
            
            /* for DISP_N3_UV column */
            tmp = gCMUsage.info.aulCms[DISP_N3_UVLIFEHOUR] ;
            strTmp = astrNames[1] + "  " + QString::number(tmp) + "H";
            m_pCslistItem[iIdx]->setValue(strTmp);
            
            strTmp = tr("Installation Date ") + decodeTime(gCMUsage.info.aulCms[DISP_N3_UVLIFEDAY]);
            m_pCslistItem[iIdx]->setInstDate(strTmp);
            
            tmp = gGlobalParam.CMParam.aulCms[DISP_N3_UVLIFEDAY] - 
                (DispGetCurSecond() - gCMUsage.info.aulCms[DISP_N3_UVLIFEDAY])/ DISP_DAYININSECOND;
            strTmp = tr("Replace in ") + decodeDays(tmp) + QString(" ") + tr("days");
            m_pCslistItem[iIdx]->setChangeDate(strTmp);

            strTmp = tr("Cat No.:") + gGlobalParam.cmSn.aCn[DISP_N3_UV];
            m_pCslistItem[iIdx]->setCatNo(strTmp);
            
            strTmp = tr("Lot No.:") + gGlobalParam.cmSn.aLn[DISP_N3_UV];
            m_pCslistItem[iIdx]->setLotNo(strTmp);
            
            if (gCMUsage.ulUsageState & (1 << DISP_N3_UVLIFEDAY) 
                || gCMUsage.ulUsageState & (1 << DISP_N3_UVLIFEHOUR))
            {
                m_pCslistItem[iIdx]->updateState(1);
            }
            else
            {
                m_pCslistItem[iIdx]->updateState(0);
            }
            m_pCslistItem[iIdx]->setName(tr("TANK UV"));
            break;  
        case DISP_N4_UV:
            
            /* for DISP_N4_UV column */
            tmp = gCMUsage.info.aulCms[DISP_N4_UVLIFEHOUR] ;
            strTmp = astrNames[1] + "  " + QString::number(tmp) + "H";
            m_pCslistItem[iIdx]->setValue(strTmp);
            
            strTmp = tr("Installation Date ") + decodeTime(gCMUsage.info.aulCms[DISP_N4_UVLIFEDAY]);
            m_pCslistItem[iIdx]->setInstDate(strTmp);

            tmp = gGlobalParam.CMParam.aulCms[DISP_N4_UVLIFEDAY] - 
                (DispGetCurSecond() - gCMUsage.info.aulCms[DISP_N4_UVLIFEDAY])/ DISP_DAYININSECOND;
            strTmp = tr("Replace in ") + decodeDays(tmp) + QString(" ") + tr("days");
            m_pCslistItem[iIdx]->setChangeDate(strTmp);

            strTmp = tr("Cat No.:") + gGlobalParam.cmSn.aCn[DISP_N4_UV];
            m_pCslistItem[iIdx]->setCatNo(strTmp);
            
            strTmp = tr("Lot No.:") + gGlobalParam.cmSn.aLn[DISP_N4_UV];
            m_pCslistItem[iIdx]->setLotNo(strTmp);
            
            if (gCMUsage.ulUsageState & (1 << DISP_N4_UVLIFEDAY) 
                || gCMUsage.ulUsageState & (1 << DISP_N4_UVLIFEHOUR))
            {
                m_pCslistItem[iIdx]->updateState(1);
            }
            else
            {
                m_pCslistItem[iIdx]->updateState(0);
            }
            m_pCslistItem[iIdx]->setName(tr("TUBE UV"));
            break;  
        case DISP_N5_UV:
            
            /* for DISP_N5_UV column */
            tmp = gCMUsage.info.aulCms[DISP_N5_UVLIFEHOUR] ;
            strTmp = astrNames[1] + "  " + QString::number(tmp) + "H";
            m_pCslistItem[iIdx]->setValue(strTmp);
            
            strTmp = tr("Installation Date ") + decodeTime(gCMUsage.info.aulCms[DISP_N5_UVLIFEDAY]);
            m_pCslistItem[iIdx]->setInstDate(strTmp);
            
            tmp = gGlobalParam.CMParam.aulCms[DISP_N5_UVLIFEDAY] - 
                (DispGetCurSecond() - gCMUsage.info.aulCms[DISP_N5_UVLIFEDAY])/ DISP_DAYININSECOND;
            strTmp = tr("Replace in ") + decodeDays(tmp) + QString(" ") + tr("days");
            m_pCslistItem[iIdx]->setChangeDate(strTmp);

            strTmp = tr("Cat No.:") + gGlobalParam.cmSn.aCn[DISP_N5_UV];
            m_pCslistItem[iIdx]->setCatNo(strTmp);
            
            strTmp = tr("Lot No.:") + gGlobalParam.cmSn.aLn[DISP_N5_UV];
            m_pCslistItem[iIdx]->setLotNo(strTmp);
            
            if (gCMUsage.ulUsageState & (1 << DISP_N5_UVLIFEDAY) 
                || gCMUsage.ulUsageState & (1 << DISP_N5_UVLIFEHOUR))
            {
                m_pCslistItem[iIdx]->updateState(1);
            }
            else
            {
                m_pCslistItem[iIdx]->updateState(0);
            }
            m_pCslistItem[iIdx]->setName(tr("TOC UV"));
            break;              
        case DISP_W_FILTER:
            
            /* for DISP_W_FILTER column */
            strTmp = tr("Installation Date ") + decodeTime(gCMUsage.info.aulCms[DISP_W_FILTERLIFE]);
            m_pCslistItem[iIdx]->setInstDate(strTmp);
            
            tmp = gGlobalParam.CMParam.aulCms[DISP_W_FILTERLIFE] - 
                (DispGetCurSecond() - gCMUsage.info.aulCms[DISP_W_FILTERLIFE])/ DISP_DAYININSECOND;
            strTmp = tr("Replace in ") + decodeDays(tmp) + QString(" ") + tr("days");
            m_pCslistItem[iIdx]->setChangeDate(strTmp);

            strTmp = tr("Cat No.:") + gGlobalParam.cmSn.aCn[DISP_W_FILTER];
            m_pCslistItem[iIdx]->setCatNo(strTmp);
            
            strTmp = tr("Lot No.:") + gGlobalParam.cmSn.aLn[DISP_W_FILTER];
            m_pCslistItem[iIdx]->setLotNo(strTmp);
            
            if (gCMUsage.ulUsageState & (1 << DISP_W_FILTERLIFE) 
                || gCMUsage.ulUsageState & (1 << DISP_W_FILTERLIFE))
            {
                m_pCslistItem[iIdx]->updateState(1);
            }
            else
            {
                m_pCslistItem[iIdx]->updateState(0);
            }
            m_pCslistItem[iIdx]->setName(tr("Tank Vent Filter"));
            break;  
            
        case DISP_T_B_FILTER:
            /* for DISP_T_B_FILTER column */
            strTmp = tr("Installation Date ") + decodeTime(gCMUsage.info.aulCms[DISP_T_B_FILTERLIFE]);
            m_pCslistItem[iIdx]->setInstDate(strTmp);
            
            tmp = gGlobalParam.CMParam.aulCms[DISP_T_B_FILTERLIFE] - 
                (DispGetCurSecond() - gCMUsage.info.aulCms[DISP_T_B_FILTERLIFE])/ DISP_DAYININSECOND;
            strTmp = tr("Replace in ") + decodeDays(tmp) + QString(" ") + tr("days");
            m_pCslistItem[iIdx]->setChangeDate(strTmp);

            strTmp = tr("Cat No.:") + gGlobalParam.cmSn.aCn[DISP_T_B_FILTER];
            m_pCslistItem[iIdx]->setCatNo(strTmp);
            
            strTmp = tr("Lot No.:") + gGlobalParam.cmSn.aLn[DISP_T_B_FILTER];
            m_pCslistItem[iIdx]->setLotNo(strTmp);
            
            if (gCMUsage.ulUsageState & (1 << DISP_T_B_FILTERLIFE) 
                || gCMUsage.ulUsageState & (1 << DISP_T_B_FILTERLIFE))
            {
                m_pCslistItem[iIdx]->updateState(1);
            }
            else
            {
                m_pCslistItem[iIdx]->updateState(0);
            }
            m_pCslistItem[iIdx]->setName(tr("Final Fliter B"));
            break;  
        case DISP_T_A_FILTER:
            /* for DISP_T_A_FILTER column */
            strTmp = tr("Installation Date ") + decodeTime(gCMUsage.info.aulCms[DISP_T_A_FILTERLIFE]);
            m_pCslistItem[iIdx]->setInstDate(strTmp);
            
            tmp = gGlobalParam.CMParam.aulCms[DISP_T_A_FILTERLIFE] - 
                (DispGetCurSecond() - gCMUsage.info.aulCms[DISP_T_A_FILTERLIFE])/ DISP_DAYININSECOND;
            strTmp = tr("Replace in ") + decodeDays(tmp) + QString(" ") + tr("days");
            m_pCslistItem[iIdx]->setChangeDate(strTmp);

            strTmp = tr("Cat No.:") + gGlobalParam.cmSn.aCn[DISP_T_A_FILTER];
            m_pCslistItem[iIdx]->setCatNo(strTmp);
            
            strTmp = tr("Lot No.:") + gGlobalParam.cmSn.aLn[DISP_T_A_FILTER];
            m_pCslistItem[iIdx]->setLotNo(strTmp);
            
            if (gCMUsage.ulUsageState & (1 << DISP_T_A_FILTERLIFE) 
                || gCMUsage.ulUsageState & (1 << DISP_T_A_FILTERLIFE))
            {
                m_pCslistItem[iIdx]->updateState(1);
            }
            else
            {
                m_pCslistItem[iIdx]->updateState(0);
            }
            m_pCslistItem[iIdx]->setName(tr("Final Fliter A"));
            break;  
            
        case DISP_TUBE_FILTER:
            /* for DISP_T_A_FILTER column */
            strTmp = tr("Installation Date ") + decodeTime(gCMUsage.info.aulCms[DISP_TUBE_FILTERLIFE]);
            m_pCslistItem[iIdx]->setInstDate(strTmp);
            
            tmp = gGlobalParam.CMParam.aulCms[DISP_TUBE_FILTERLIFE] - 
                (DispGetCurSecond() - gCMUsage.info.aulCms[DISP_TUBE_FILTERLIFE])/ DISP_DAYININSECOND;
            strTmp = tr("Replace in ") + decodeDays(tmp) + QString(" ") + tr("days");
            m_pCslistItem[iIdx]->setChangeDate(strTmp);

            strTmp = tr("Cat No.:") + gGlobalParam.cmSn.aCn[DISP_TUBE_FILTER];
            m_pCslistItem[iIdx]->setCatNo(strTmp);
            
            strTmp = tr("Lot No.:") + gGlobalParam.cmSn.aLn[DISP_TUBE_FILTER];
            m_pCslistItem[iIdx]->setLotNo(strTmp);
            
            if (gCMUsage.ulUsageState & (1 << DISP_TUBE_FILTERLIFE) 
                || gCMUsage.ulUsageState & (1 << DISP_TUBE_FILTERLIFE))
            {
                m_pCslistItem[iIdx]->updateState(1);
            }
            else
            {
                m_pCslistItem[iIdx]->updateState(0);
            }
            m_pCslistItem[iIdx]->setName(tr("Loop Filter"));
            break;  
            
        case DISP_TUBE_DI:
            /* for DISP_TUBE_DI column */
            strTmp = tr("Installation Date ") + decodeTime(gCMUsage.info.aulCms[DISP_TUBE_DI_LIFE]);
            m_pCslistItem[iIdx]->setInstDate(strTmp);
            
            tmp = gGlobalParam.CMParam.aulCms[DISP_TUBE_DI_LIFE] - 
                (DispGetCurSecond() - gCMUsage.info.aulCms[DISP_TUBE_DI_LIFE])/ DISP_DAYININSECOND;
            strTmp = tr("Replace in ") + decodeDays(tmp) + QString(" ") + tr("days");
            m_pCslistItem[iIdx]->setChangeDate(strTmp);

            strTmp = tr("Cat No.:") + gGlobalParam.cmSn.aCn[DISP_TUBE_DI];
            m_pCslistItem[iIdx]->setCatNo(strTmp);
            
            strTmp = tr("Lot No.:") + gGlobalParam.cmSn.aLn[DISP_TUBE_DI];
            m_pCslistItem[iIdx]->setLotNo(strTmp);
            
            if (gCMUsage.ulUsageState & (1 << DISP_TUBE_DI_LIFE) 
                || gCMUsage.ulUsageState & (1 << DISP_TUBE_DI_LIFE))
            {
                m_pCslistItem[iIdx]->updateState(1);
            }
            else
            {
                m_pCslistItem[iIdx]->updateState(0);
            }
            m_pCslistItem[iIdx]->setName(tr("Loop DI"));                
            break;  
        }
    }

}
void ConsumableStatePage::switchLanguage()
{
    buildTranslation();

    buildTitles();

    selectTitle(titleIndex());
}

void ConsumableStatePage::createList()
{
    int index;
    
    QColor colors[] = {QColor(200,200,188),QColor(228, 231, 240)};
    
    m_listWidget = new QListWidget(m_widget);//m_wndMain->getMainWidget()

    m_listWidget->setStyleSheet("background-color:transparent");

    m_listWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    m_listWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_listWidget->setFrameShape(QListWidget::NoFrame);
    m_listWidget->setGeometry(QRect((800-CS_LIST_WIDGET_WIDTH)/2,55 ,CS_LIST_WIDGET_WIDTH,540));

    for(index = 0 ; index < m_realCsNum ; index++ )
    {
        listWidgetItem[index] = new QListWidgetItem;
        listWidgetItem[index]->setSizeHint(QSize(CS_LIST_WIDGET_WIDTH , CS_LIST_WIDGET_HEIGHT));
        listWidgetItem[index]->setBackground(colors[index % 2]);

        m_pCslistItem[index] = new CsListItem(0,this,aIds[index].iId);

        m_listWidget->insertItem(index,listWidgetItem[index]);

        m_listWidget->setItemWidget(listWidgetItem[index] , m_pCslistItem[index]);
    }
}

void ConsumableStatePage::setBackColor()
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

void ConsumableStatePage::initUi()
{
    setBackColor();

    createList();
}

void ConsumableStatePage::on_btn_clicked(int index)
{
    (void)index;
}

