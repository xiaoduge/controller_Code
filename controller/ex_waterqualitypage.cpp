#include "ex_waterqualitypage.h"
#include "mainwindow.h"

#include <math.h>
#include <QPainter>

#define toOneDecimal(v) (((float)(int)(v*10))/10)
#define toTwoDecimal(v) (((float)(int)(v*100))/100)


Ex_WaterQualityPage::Ex_WaterQualityPage(QObject *parent,CBaseWidget *widget ,MainWindow *wndMain) : CSubPage(parent,widget,wndMain)
{
    m_showFirst = true;
    for (int iLoop = 0; iLoop < APP_FM_FLOW_METER_NUM; iLoop++)
    {
        m_aulFlowMeter[iLoop] = 0;
    }

    for(int i = 0; i < MSG_NUM; ++i)
    {
        m_historyInfo[i].value1 = 0;
        m_historyInfo[i].value2 = 0;
    }

    creatTitle();
    initUi();
    buildTranslation();   
}

void Ex_WaterQualityPage::creatTitle()
{
    CSubPage::creatTitle();

    buildTitles();

    selectTitle(0);
}

void Ex_WaterQualityPage::buildTitles()
{
    QStringList stringList;

    stringList << tr("Water Quality");

    setTitles(stringList);

}

void Ex_WaterQualityPage::buildTranslation()
{ 
    strMsg[Tap_Cond] = tr("Tap Cond.");
    strMsg[RO_Feed_Cond] = tr("RO Feed Cond.");
    strMsg[RO_Product] = tr("RO Product");
    strMsg[RO_Rejection] = tr("RO Rejection");
    strMsg[EDI_Product] = tr("EDI Product");
    strMsg[RO_Feed_Pressure] = tr("RO Feed Pressure");
    strMsg[RO_Pressure] = tr("RO Pressure");
    strMsg[RO_Product_Rate] = tr("RO Product Rate");
    strMsg[RO_Reject_Rate] = tr("RO Reject Rate");
    strMsg[RO_Feed_Rate] = tr("RO Feed Rate");
    strMsg[Tap_Rate] = tr("Tap Rate");
    strMsg[EDI_Product_Rate] = tr("EDI Product Rate");
    strMsg[EDI_Reject_Rate] = tr("EDI Reject Rate");
    strMsg[Source_Tank_Level] = tr("Source Tank Level");
    strMsg[Pure_Tank_Level] = tr("Pure Tank Level");
    strMsg[HP_Resis] = tr("HP Resis.");
    strMsg[HP_Disp_Rate] = tr("HP Disp. rate");
    strMsg[UP_IN] = tr("UP IN");
    strMsg[UP_Resis] = tr("UP");
    strMsg[TOC_Value] = tr("TOC");
    strMsg[UP_Disp_Rate] = tr("UP Disp. rate");

    strUnitMsg[UNIT_OMG] = "%1 " + tr("omg");
    strUnitMsg[UNIT_USCM] = "%1 " + tr("us/cm");
    strUnitMsg[UNIT_CELSIUS] = "%1 " + tr("Celsius");
    strUnitMsg[UNIT_F] = "%1 " + tr("Fahrenheit");
    strUnitMsg[UNIT_L_MIN] = "%1 " + tr("L/min");
    strUnitMsg[UNIT_G_MIN] = "%1 " + tr("G/min");
    strUnitMsg[UNIT_BAR] = "%1 " + tr("bar");
    strUnitMsg[UNIT_MPA] = "%1 " + tr("mpa");
    strUnitMsg[UNIT_PSI] = "%1 " + tr("psi");
    strUnitMsg[UNIT_PPB] = "%1 " + tr("ppb");
    strUnitMsg[UNIT_PERCENTAGE] = "%1 " + tr("%");
    strUnitMsg[UNIT_VOLUME] = "%1 " + tr("L");

    initTagsArray();
}

void Ex_WaterQualityPage::switchLanguage()
{
    buildTranslation();
    buildTitles();
    selectTitle(titleIndex());
}


void Ex_WaterQualityPage::updAllInfo(void)
{
    updHistoryEcoInfo();
    updHistoryPressure();
    updHistoryFlowInfo();
    updHistoryTank();
    updHistoryTOC();
}

void Ex_WaterQualityPage::updEcoInfo(int iIndex,ECO_INFO_STRU *info)
{
    QString strWaterUnit = strUnitMsg[UNIT_OMG];
    QString strTempUnit = strUnitMsg[UNIT_CELSIUS];

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
                strWaterUnit = strUnitMsg[UNIT_USCM];
                if(fQ < 0.055)
                {
                    fQ = 0.055;
                }
            }


            if (TEMERATURE_UINT_CELSIUS == gGlobalParam.MiscParam.iUint4Temperature)
            {
                fT = info->fTemperature;
            }
            else
            {
                fT = toFahrenheit(info->fTemperature);
                strTempUnit = strUnitMsg[UNIT_F];
            }

            if (DispGetUpQtwFlag() || DispGetUpCirFlag())
            {
                updateValue(m_tags[UP_Resis],
                            strWaterUnit.arg(toOneDecimal(fQ)),
                            strTempUnit.arg(toOneDecimal(fT)));
                m_historyInfo[UP_Resis].value1 = info->fQuality;
                m_historyInfo[UP_Resis].value2 = info->fTemperature;
            }
        }
        break;
    case APP_EXE_I4_NO:
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
                strWaterUnit = strUnitMsg[UNIT_USCM];
                if(fQ < 0.055)
                {
                    fQ = 0.055;
                }
            }

            if (TEMERATURE_UINT_CELSIUS == gGlobalParam.MiscParam.iUint4Temperature)
            {
                fT = info->fTemperature;
            }
            else
            {
                fT = toFahrenheit(info->fTemperature);
                strTempUnit = strUnitMsg[UNIT_F];
            }

            if (DispGetTubeCirFlag())
            {
            }
            else if (DispGetTocCirFlag())
            {
                updateValue(m_tags[HP_Resis],
                            strWaterUnit.arg(toOneDecimal(fQ)),
                            strTempUnit.arg(toOneDecimal(fT)));

                m_historyInfo[HP_Resis].value1 = info->fQuality;
                m_historyInfo[HP_Resis].value2 = info->fTemperature;
            }
            else if (DispGetEdiQtwFlag() //0629
                     &&(MACHINE_PURIST != gGlobalParam.iMachineType)
                     &&(MACHINE_RO != gGlobalParam.iMachineType))
            {
                updateValue(m_tags[HP_Resis],
                            strWaterUnit.arg(toOneDecimal(fQ)),
                            strTempUnit.arg(toOneDecimal(fT)));

                m_historyInfo[HP_Resis].value1 = info->fQuality;
                m_historyInfo[HP_Resis].value2 = info->fTemperature;
            }
            else if (DispGetTankCirFlag()
                && (MACHINE_PURIST != gGlobalParam.iMachineType))
            {
                updateValue(m_tags[HP_Resis],
                            strWaterUnit.arg(toOneDecimal(fQ)),
                            strTempUnit.arg(toOneDecimal(fT)));
                m_historyInfo[HP_Resis].value1 = info->fQuality;
                m_historyInfo[HP_Resis].value2 = info->fTemperature;
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
                strWaterUnit = strUnitMsg[UNIT_USCM];
                if(fQ < 0.055)
                {
                    fQ = 0.055;
                }
            }

            if (TEMERATURE_UINT_CELSIUS == gGlobalParam.MiscParam.iUint4Temperature)
            {
                fT = info->fTemperature;
            }
            else
            {
                fT = toFahrenheit(info->fTemperature);
                strTempUnit = strUnitMsg[UNIT_F];
            }

            if(fQ > 16)
            {
                updateValue(m_tags[EDI_Product],
                            strWaterUnit.arg(">16"),
                            strTempUnit.arg(fT, 0, 'f', 1));
            }
            else
            {
                updateValue(m_tags[EDI_Product],
                            strWaterUnit.arg(fQ, 0, 'f', 1),
                            strTempUnit.arg(fT, 0, 'f', 1));
            }
            m_historyInfo[EDI_Product].value1 = info->fQuality;
            m_historyInfo[EDI_Product].value2 = info->fTemperature;

            if(gGlobalParam.iMachineType == MACHINE_RO)
            {
                if (DispGetEdiQtwFlag() || DispGetTankCirFlag())
                {
                    updateValue(m_tags[HP_Resis],
                                strWaterUnit.arg(fQ, 0, 'f', 1),
                                strTempUnit.arg(fT, 0, 'f', 1));
                    m_historyInfo[HP_Resis].value1 = info->fQuality;
                    m_historyInfo[HP_Resis].value2 = info->fTemperature;
                }
            }
        }
        break;
    case APP_EXE_I2_NO: // RO Out
        {
            float fResidue;
            float fT;

            if (DispGetREJ(&fResidue)
                && (MACHINE_PURIST != gGlobalParam.iMachineType))
            {
                updateValue(m_tags[RO_Rejection],
                            strUnitMsg[UNIT_PERCENTAGE].arg(fResidue, 0, 'f', 0));
                m_historyInfo[RO_Rejection].value1 = fResidue;
            }

            if (TEMERATURE_UINT_CELSIUS == gGlobalParam.MiscParam.iUint4Temperature)
            {
                fT = info->fTemperature;
            }
            else
            {
                fT = toFahrenheit(info->fTemperature);
                strTempUnit = strUnitMsg[UNIT_F];
            }

            if(MACHINE_PURIST != gGlobalParam.iMachineType)
            {
                updateValue(m_tags[RO_Product],
                            strUnitMsg[UNIT_USCM].arg(info->fQuality, 0, 'f', 1),
                            strTempUnit.arg(fT, 0, 'f', 1));

                m_historyInfo[RO_Product].value1 = info->fQuality;
                m_historyInfo[RO_Product].value2 = info->fTemperature;
            }
            else
            {
                updateValue(m_tags[UP_IN],
                            strUnitMsg[UNIT_USCM].arg(info->fQuality, 0, 'f', 1),
                            strTempUnit.arg(fT, 0, 'f', 1));

                m_historyInfo[UP_IN].value1 = info->fQuality;
                m_historyInfo[UP_IN].value2 = info->fTemperature;
            }
        }
        break;
    case APP_EXE_I1_NO: // RO In
        {
            float fT;

            if (TEMERATURE_UINT_CELSIUS == gGlobalParam.MiscParam.iUint4Temperature)
            {
                fT = info->fTemperature;
            }
            else
            {
                fT = toFahrenheit(info->fTemperature);
                strTempUnit = strUnitMsg[UNIT_F];
            }

            if (DispGetInitRunFlag())
            {
                updateValue(m_tags[Tap_Cond],
                            strUnitMsg[UNIT_USCM].arg(info->fQuality, 0, 'f', 1),
                            strTempUnit.arg(fT, 0, 'f', 1));

                m_historyInfo[Tap_Cond].value1 = info->fQuality;
                m_historyInfo[Tap_Cond].value2 = info->fTemperature;
            }
            else
            {
                updateValue(m_tags[RO_Feed_Cond],
                            strUnitMsg[UNIT_USCM].arg(info->fQuality, 0, 'f', 1),
                            strTempUnit.arg(fT, 0, 'f', 1));

                m_historyInfo[RO_Feed_Cond].value1 = info->fQuality;
                m_historyInfo[RO_Feed_Cond].value2 = info->fTemperature;

                updateValue(m_tags[Tap_Cond],
                            strUnitMsg[UNIT_USCM].arg(m_historyInfo[Tap_Cond].value1, 0, 'f', 1),
                            strTempUnit.arg(fT, 0, 'f', 1));
                m_historyInfo[Tap_Cond].value2 = info->fTemperature;
            }


        }
        break;
    }
}

void Ex_WaterQualityPage::updPressure(int iIndex,float fvalue)
{
    if (APP_EXE_PM1_NO == iIndex)
    {
        float fP ;
        QString strUint = strUnitMsg[UNIT_BAR];
        if (PRESSURE_UINT_BAR == gGlobalParam.MiscParam.iUint4Pressure)
        {
            fP = toOneDecimal(fvalue);
        }
        else if (PRESSURE_UINT_MPA == gGlobalParam.MiscParam.iUint4Pressure)
        {
            float temp = toMpa(fvalue);
            fP = toTwoDecimal(temp);
            strUint = strUnitMsg[UNIT_MPA];
        }
        else
        {
            fP = toPsi(fvalue);
            strUint = strUnitMsg[UNIT_PSI];
        }
        QString strValue = strUint.arg(fP);
        updateValue(m_tags[RO_Pressure], strValue);

        m_historyInfo[RO_Pressure].value1 = fvalue;
    }

}

void Ex_WaterQualityPage::updSwPressure(float fvalue)
{
    float fP = fvalue;
    QString strUint = strUnitMsg[UNIT_BAR];

    if (PRESSURE_UINT_BAR == gGlobalParam.MiscParam.iUint4Pressure)
    {
        fP = toOneDecimal(fvalue);
    }
    else if (PRESSURE_UINT_MPA == gGlobalParam.MiscParam.iUint4Pressure)
    {
        float tempValue = toMpa(fvalue);
        fP = toTwoDecimal(tempValue);
        strUint = strUnitMsg[UNIT_MPA];
    }
    else
    {
        fP = toPsi(fvalue);
        strUint = strUnitMsg[UNIT_PSI];
    }

    QString strValue = strUint.arg(fP);
    updateValue(m_tags[RO_Feed_Pressure], strValue);

    m_historyInfo[RO_Feed_Pressure].value1 = fvalue;
}

void Ex_WaterQualityPage::updTOC(float fToc)
{
    QString str = strUnitMsg[UNIT_PPB].arg((int)(fToc + 0.5));
    if(fToc >= 200)
    {
        str = strUnitMsg[UNIT_PPB].arg(">200");
    }
    updateValue(m_tags[TOC_Value], str);
    m_historyInfo[TOC_Value].value1 = fToc;
}

void Ex_WaterQualityPage::initAllValue()
{
    for(int i = 0; i < MSG_NUM; ++i)
    {
        updateValue(m_tags[i], "--");
    }
}

void Ex_WaterQualityPage::initTagsArray()
{
    m_tags[Tap_Cond] = DTags(strMsg[Tap_Cond], 2);
    m_tags[RO_Feed_Cond] = DTags(strMsg[RO_Feed_Cond], 2);
    m_tags[RO_Product] = DTags(strMsg[RO_Product], 2);
    m_tags[RO_Rejection] = DTags(strMsg[RO_Rejection], 1);
    m_tags[EDI_Product] = DTags(strMsg[EDI_Product], 1);

    m_tags[RO_Feed_Pressure] = DTags(strMsg[RO_Feed_Pressure], 1);
    m_tags[RO_Pressure] = DTags(strMsg[RO_Pressure], 1);

    m_tags[RO_Product_Rate] = DTags(strMsg[RO_Product_Rate], 1);
    m_tags[RO_Reject_Rate] = DTags(strMsg[RO_Reject_Rate], 1);
    m_tags[RO_Feed_Rate] = DTags(strMsg[RO_Feed_Rate], 1);
    m_tags[Tap_Rate] = DTags(strMsg[Tap_Rate], 1);
    m_tags[EDI_Product_Rate] = DTags(strMsg[EDI_Product_Rate], 1);
    m_tags[EDI_Reject_Rate] = DTags(strMsg[EDI_Reject_Rate], 1);

    m_tags[Source_Tank_Level] = DTags(strMsg[Source_Tank_Level], 2);
    m_tags[Pure_Tank_Level] = DTags(strMsg[Pure_Tank_Level], 2);
    m_tags[HP_Resis] = DTags(strMsg[HP_Resis], 2);
    m_tags[HP_Disp_Rate] = DTags(strMsg[HP_Disp_Rate], 1);
    m_tags[UP_IN] = DTags(strMsg[UP_IN], 2);
    m_tags[UP_Resis] = DTags(strMsg[UP_Resis], 2);
    m_tags[TOC_Value] = DTags(strMsg[TOC_Value], 1);
    m_tags[UP_Disp_Rate] = DTags(strMsg[UP_Disp_Rate], 1);

    initConfigList();
}

void Ex_WaterQualityPage::initConfigList()
{
    m_cfglist.clear();
    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
        m_cfglist.append(m_tags[Tap_Cond]);
        m_cfglist.append(m_tags[RO_Feed_Cond]);
        m_cfglist.append(m_tags[RO_Product]);
        m_cfglist.append(m_tags[RO_Rejection]);
        m_cfglist.append(m_tags[EDI_Product]);
        m_cfglist.append(m_tags[RO_Feed_Pressure]);
        m_cfglist.append(m_tags[RO_Pressure]);
        m_cfglist.append(m_tags[RO_Product_Rate]);
        m_cfglist.append(m_tags[RO_Reject_Rate]);
        m_cfglist.append(m_tags[RO_Feed_Rate]);
        m_cfglist.append(m_tags[Tap_Rate]);
        m_cfglist.append(m_tags[EDI_Product_Rate]);
        m_cfglist.append(m_tags[EDI_Reject_Rate]);
        if(gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_HaveB3))
        {
            m_cfglist.append(m_tags[Source_Tank_Level]);
        }
        m_cfglist.append(m_tags[Pure_Tank_Level]);
        m_cfglist.append(m_tags[HP_Resis]);
        m_cfglist.append(m_tags[HP_Disp_Rate]);

        m_cfglist.append(m_tags[UP_Resis]);
        if(gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_HaveTOC))
        {
            m_cfglist.append(m_tags[TOC_Value]);
        }
        m_cfglist.append(m_tags[UP_Disp_Rate]);
        break;
    case MACHINE_L_UP:
        m_cfglist.append(m_tags[Tap_Cond]);
        m_cfglist.append(m_tags[RO_Feed_Cond]);
        m_cfglist.append(m_tags[RO_Product]);
        m_cfglist.append(m_tags[RO_Rejection]);

        m_cfglist.append(m_tags[RO_Feed_Pressure] );
        m_cfglist.append(m_tags[RO_Pressure]);

        m_cfglist.append(m_tags[RO_Product_Rate]);
        m_cfglist.append(m_tags[RO_Reject_Rate]);
        m_cfglist.append(m_tags[RO_Feed_Rate]);
        m_cfglist.append(m_tags[Tap_Rate]);
        if(gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_HaveB3))
        {
            m_cfglist.append(m_tags[Source_Tank_Level]);
        }
        m_cfglist.append(m_tags[Pure_Tank_Level]);
        m_cfglist.append(m_tags[HP_Resis]);

        m_cfglist.append(m_tags[UP_Resis]);
        m_cfglist.append(m_tags[HP_Disp_Rate]);
        if(gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_HaveTOC))
        {
            m_cfglist.append(m_tags[TOC_Value]);
        }
        m_cfglist.append(m_tags[UP_Disp_Rate]);
        break;
    case MACHINE_L_EDI_LOOP:
        m_cfglist.append(m_tags[Tap_Cond]);
        m_cfglist.append(m_tags[RO_Feed_Cond]);
        m_cfglist.append(m_tags[RO_Product]);
        m_cfglist.append(m_tags[RO_Rejection]);
        m_cfglist.append(m_tags[EDI_Product]);
        m_cfglist.append(m_tags[RO_Feed_Pressure]);
        m_cfglist.append(m_tags[RO_Pressure]);
        m_cfglist.append(m_tags[RO_Product_Rate]);
        m_cfglist.append(m_tags[RO_Reject_Rate]);
        m_cfglist.append(m_tags[RO_Feed_Rate]);
        m_cfglist.append(m_tags[Tap_Rate]);
        m_cfglist.append(m_tags[EDI_Product_Rate]);
        m_cfglist.append(m_tags[EDI_Reject_Rate]);

        if(gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_HaveB3))
        {
            m_cfglist.append(m_tags[Source_Tank_Level]);
        }
        m_cfglist.append(m_tags[Pure_Tank_Level]);
        m_cfglist.append(m_tags[HP_Resis]);
        m_cfglist.append(m_tags[HP_Disp_Rate]);
        break;
    case MACHINE_L_RO_LOOP:
        m_cfglist.append(m_tags[Tap_Cond]);
        m_cfglist.append(m_tags[RO_Feed_Cond]);
        m_cfglist.append(m_tags[RO_Product]);
        m_cfglist.append(m_tags[RO_Rejection]);

        m_cfglist.append(m_tags[RO_Feed_Pressure]);
        m_cfglist.append(m_tags[RO_Pressure]);
        m_cfglist.append(m_tags[RO_Product_Rate]);
        m_cfglist.append(m_tags[RO_Reject_Rate]);
        m_cfglist.append(m_tags[RO_Feed_Rate]);
        m_cfglist.append(m_tags[Tap_Rate]);

        if(gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_HaveB3))
        {
            m_cfglist.append(m_tags[Source_Tank_Level]);
        }
        m_cfglist.append(m_tags[Pure_Tank_Level]);
        m_cfglist.append(m_tags[HP_Resis]);
        m_cfglist.append(m_tags[HP_Disp_Rate]);
        break;
    case MACHINE_Genie:
        m_cfglist.append(m_tags[Tap_Cond]);
        m_cfglist.append(m_tags[RO_Feed_Cond]);
        m_cfglist.append(m_tags[RO_Product]);
        m_cfglist.append(m_tags[RO_Rejection]);
        m_cfglist.append(m_tags[EDI_Product]);
        m_cfglist.append(m_tags[RO_Feed_Pressure]);
        m_cfglist.append(m_tags[RO_Pressure]);
        m_cfglist.append(m_tags[Pure_Tank_Level]);
        m_cfglist.append(m_tags[HP_Resis]);
        m_cfglist.append(m_tags[HP_Disp_Rate]);
        m_cfglist.append(m_tags[UP_Resis]);
        if(gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_HaveTOC))
        {
            m_cfglist.append(m_tags[TOC_Value]);
        }
        m_cfglist.append(m_tags[UP_Disp_Rate]);
        break;
    case MACHINE_UP:
        m_cfglist.append(m_tags[Tap_Cond]);
        m_cfglist.append(m_tags[RO_Feed_Cond]);
        m_cfglist.append(m_tags[RO_Product]);
        m_cfglist.append(m_tags[RO_Rejection]);

        m_cfglist.append(m_tags[RO_Feed_Pressure]);
        m_cfglist.append(m_tags[RO_Pressure]);

        m_cfglist.append(m_tags[Pure_Tank_Level]);
        m_cfglist.append(m_tags[HP_Resis]);
        m_cfglist.append(m_tags[HP_Disp_Rate]);
        m_cfglist.append(m_tags[UP_Resis]);
        if(gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_HaveTOC))
        {
            m_cfglist.append(m_tags[TOC_Value]);
        }
        m_cfglist.append(m_tags[UP_Disp_Rate]);
        break;
    case MACHINE_EDI:
        m_cfglist.append(m_tags[Tap_Cond]);
        m_cfglist.append(m_tags[RO_Feed_Cond]);
        m_cfglist.append(m_tags[RO_Product]);
        m_cfglist.append(m_tags[RO_Rejection]);
        m_cfglist.append(m_tags[EDI_Product]);
        m_cfglist.append(m_tags[RO_Feed_Pressure]);
        m_cfglist.append(m_tags[RO_Pressure]);
        m_cfglist.append(m_tags[Pure_Tank_Level]);
        m_cfglist.append(m_tags[HP_Resis]);
        m_cfglist.append(m_tags[HP_Disp_Rate]);
        break;
    case MACHINE_RO:
        m_cfglist.append(m_tags[Tap_Cond]);
        m_cfglist.append(m_tags[RO_Feed_Cond]);
        m_cfglist.append(m_tags[RO_Product]);
        m_cfglist.append(m_tags[RO_Rejection]);

        m_cfglist.append(m_tags[RO_Feed_Pressure]);
        m_cfglist.append(m_tags[RO_Pressure]);

        m_cfglist.append(m_tags[Pure_Tank_Level]);
        m_cfglist.append(m_tags[HP_Resis]);

        m_cfglist.append(m_tags[HP_Disp_Rate]);
        break;
    case MACHINE_ADAPT:
        m_cfglist.append(m_tags[Tap_Cond]);
        m_cfglist.append(m_tags[RO_Feed_Cond]);
        m_cfglist.append(m_tags[RO_Product]);
        m_cfglist.append(m_tags[RO_Rejection]);
        m_cfglist.append(m_tags[EDI_Product]);
        m_cfglist.append(m_tags[RO_Feed_Pressure]);
        m_cfglist.append(m_tags[RO_Pressure]);

        m_cfglist.append(m_tags[HP_Resis]);
        m_cfglist.append(m_tags[HP_Disp_Rate]);
        m_cfglist.append(m_tags[UP_Resis]);
        if(gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_HaveTOC))
        {
            m_cfglist.append(m_tags[TOC_Value]);
        }
        m_cfglist.append(m_tags[UP_Disp_Rate]);
        break;
    case MACHINE_PURIST:
        m_cfglist.append(m_tags[Pure_Tank_Level]);
        m_cfglist.append(m_tags[UP_IN]);
        m_cfglist.append(m_tags[UP_Resis]);
        if(gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_HaveTOC))
        {
            m_cfglist.append(m_tags[TOC_Value]);
        }
        m_cfglist.append(m_tags[UP_Disp_Rate]);
        break;
    default:
        break;
    }
    m_pQualityWidget->setConfigList(m_cfglist);
}

void Ex_WaterQualityPage::updateValue(const DTags &t, const QString &value1, const QString &value2)
{
    if(!m_cfglist.contains(t))
    {
        return;
    }
    m_pQualityWidget->updateValue(t, value1, value2);
}

void Ex_WaterQualityPage::updHistoryEcoInfo()
{
    Show_History_Info tempValue[3];
    QString strWaterUnit = strUnitMsg[UNIT_OMG];
    QString strTempUnit = strUnitMsg[UNIT_CELSIUS];

    if (CONDUCTIVITY_UINT_OMG == gGlobalParam.MiscParam.iUint4Conductivity)
    {
        tempValue[0].value1 = m_historyInfo[EDI_Product].value1;
        tempValue[1].value1  = m_historyInfo[HP_Resis].value1;
        tempValue[2].value1  = m_historyInfo[UP_Resis].value1;
    }
    else
    {
        tempValue[0].value1  = toConductivity(m_historyInfo[EDI_Product].value1);
        tempValue[1].value1  = toConductivity(m_historyInfo[HP_Resis].value1);
        tempValue[2].value1  = toConductivity(m_historyInfo[UP_Resis].value1);

        for(int i = 0; i < 3; ++i)
        {
            if(tempValue[i].value1 < 0.055)
            {
                tempValue[i].value1 = 0.055;
            }
        }

        strWaterUnit = strUnitMsg[UNIT_USCM];
    }


    if (TEMERATURE_UINT_CELSIUS == gGlobalParam.MiscParam.iUint4Temperature)
    {
        tempValue[0].value2 = m_historyInfo[EDI_Product].value2;
        tempValue[1].value2 = m_historyInfo[HP_Resis].value2;
        tempValue[2].value2 = m_historyInfo[UP_Resis].value2;

        updateValue(m_tags[RO_Feed_Cond],
                    strUnitMsg[UNIT_USCM].arg(m_historyInfo[RO_Feed_Cond].value1, 0, 'f', 1),
                    strUnitMsg[UNIT_CELSIUS].arg(m_historyInfo[RO_Feed_Cond].value2, 0, 'f', 1));

        updateValue(m_tags[RO_Product],
                    strUnitMsg[UNIT_USCM].arg(m_historyInfo[RO_Product].value1, 0, 'f', 1),
                    strUnitMsg[UNIT_CELSIUS].arg(m_historyInfo[RO_Product].value2, 0, 'f', 1));
    }
    else
    {
        tempValue[0].value2 = toFahrenheit(m_historyInfo[EDI_Product].value2);
        tempValue[1].value2 = toFahrenheit(m_historyInfo[HP_Resis].value2);
        tempValue[2].value2 = toFahrenheit(m_historyInfo[UP_Resis].value2);

        strTempUnit = strUnitMsg[UNIT_F];

        updateValue(m_tags[RO_Feed_Cond],
                    strUnitMsg[UNIT_USCM].arg(m_historyInfo[RO_Feed_Cond].value1, 0, 'f', 1),
                    strUnitMsg[UNIT_F].arg(toFahrenheit(m_historyInfo[RO_Feed_Cond].value2), 0, 'f', 1));

        updateValue(m_tags[RO_Product],
                    strUnitMsg[UNIT_USCM].arg(m_historyInfo[RO_Product].value1, 0, 'f', 1),
                    strUnitMsg[UNIT_F].arg(toFahrenheit(m_historyInfo[RO_Product].value2), 0, 'f', 1));
    }

    if(tempValue[2].value1 > 1)
    {
        updateValue(m_tags[UP_Resis],
                    strWaterUnit.arg(tempValue[2].value1, 0,' f', 1),
                    strTempUnit.arg(tempValue[2].value2, 0, 'f', 1));
    }
    else
    {
        updateValue(m_tags[UP_Resis],
                    strWaterUnit.arg(tempValue[2].value1, 0, 'f',3),
                    strTempUnit.arg(tempValue[2].value2, 0, 'f',1));
    }

    if(tempValue[1].value1 > 1)
    {
        updateValue(m_tags[HP_Resis],
                    strWaterUnit.arg(tempValue[1].value1, 0, 'f',1),
                    strTempUnit.arg(tempValue[1].value2, 0, 'f',1));
    }
    else
    {
        updateValue(m_tags[HP_Resis],
                    strWaterUnit.arg(tempValue[1].value1, 0, 'f',3),
                    strTempUnit.arg(tempValue[1].value2, 0, 'f',1));
    }

    if(tempValue[0].value1 > 1)
    {
        updateValue(m_tags[EDI_Product],
                    strWaterUnit.arg(tempValue[0].value1, 0, 'f',1),
                    strTempUnit.arg(tempValue[0].value2, 0, 'f',1));
    }
    else
    {
        updateValue(m_tags[EDI_Product],
                    strWaterUnit.arg(tempValue[0].value1, 0, 'f',3),
                    strTempUnit.arg(tempValue[0].value2, 0, 'f',1));
    }

    updateValue(m_tags[Tap_Cond],
                strUnitMsg[UNIT_USCM].arg(m_historyInfo[Tap_Cond].value1, 0, 'f', 1),
                strTempUnit.arg(m_historyInfo[Tap_Cond].value2, 0, 'f', 1));

    updateValue(m_tags[RO_Rejection],
                strUnitMsg[UNIT_PERCENTAGE].arg(m_historyInfo[RO_Rejection].value1, 0, 'f', 0));

    if( MACHINE_PURIST == gGlobalParam.iMachineType)
    {
        updateValue(m_tags[UP_IN],
                    strUnitMsg[UNIT_USCM].arg(m_historyInfo[UP_IN].value1, 0, 'f', 1),
                    strTempUnit.arg(m_historyInfo[UP_IN].value1, 0, 'f', 1));
    }

}

void Ex_WaterQualityPage::updHistoryPressure()
{
    if (PRESSURE_UINT_BAR == gGlobalParam.MiscParam.iUint4Pressure)
    {
        updateValue(m_tags[RO_Feed_Pressure],
                    strUnitMsg[UNIT_BAR].arg(m_historyInfo[RO_Feed_Pressure].value1, 0, 'f', 1));

        updateValue(m_tags[RO_Pressure],
                    strUnitMsg[UNIT_BAR].arg(m_historyInfo[RO_Pressure].value1, 0, 'f', 1));
    }
    else if (PRESSURE_UINT_MPA == gGlobalParam.MiscParam.iUint4Pressure)
    {
        updateValue(m_tags[RO_Feed_Pressure],
                    strUnitMsg[UNIT_MPA].arg(m_historyInfo[RO_Feed_Pressure].value1, 0, 'f', 1));

        updateValue(m_tags[RO_Pressure],
                    strUnitMsg[UNIT_MPA].arg(m_historyInfo[RO_Pressure].value1, 0, 'f', 1));
    }
    else
    {
        updateValue(m_tags[RO_Feed_Pressure],
                    strUnitMsg[UNIT_PSI].arg(m_historyInfo[RO_Feed_Pressure].value1, 0, 'f', 1));

        updateValue(m_tags[RO_Pressure],
                    strUnitMsg[UNIT_PSI].arg(m_historyInfo[RO_Pressure].value1, 0, 'f', 1));
    }
}

void Ex_WaterQualityPage::updHistoryFlowInfo()
{
    if ((FLOW_VELOCITY_UINT_GAL_PER_MIN == gGlobalParam.MiscParam.iUint4FlowVelocity))
    {
        updateValue(m_tags[HP_Disp_Rate], strUnitMsg[UNIT_G_MIN].arg((toGallon(1))*m_historyInfo[HP_Disp_Rate].value1, 0, 'f', 1));
        updateValue(m_tags[UP_Disp_Rate], strUnitMsg[UNIT_G_MIN].arg((toGallon(1))*m_historyInfo[UP_Disp_Rate].value1, 0, 'f', 1));
        updateValue(m_tags[RO_Feed_Rate], strUnitMsg[UNIT_G_MIN].arg((toGallon(1))*m_historyInfo[RO_Feed_Rate].value1, 0, 'f', 1));
        updateValue(m_tags[RO_Product_Rate], strUnitMsg[UNIT_G_MIN].arg((toGallon(1))*m_historyInfo[RO_Product_Rate].value1, 0, 'f', 1));
        updateValue(m_tags[Tap_Rate], strUnitMsg[UNIT_G_MIN].arg((toGallon(1))*m_historyInfo[Tap_Rate].value1, 0, 'f', 1));
        updateValue(m_tags[EDI_Product_Rate], strUnitMsg[UNIT_G_MIN].arg((toGallon(1))*m_historyInfo[EDI_Product_Rate].value1, 0, 'f', 1));
        updateValue(m_tags[EDI_Reject_Rate], strUnitMsg[UNIT_G_MIN].arg((toGallon(1))*m_historyInfo[EDI_Reject_Rate].value1, 0, 'f', 1));
        updateValue(m_tags[RO_Reject_Rate], strUnitMsg[UNIT_G_MIN].arg((toGallon(1))*m_historyInfo[RO_Reject_Rate].value1, 0, 'f', 1));
    }
    else
    {
        updateValue(m_tags[HP_Disp_Rate], strUnitMsg[UNIT_L_MIN].arg(1.0 * m_historyInfo[HP_Disp_Rate].value1, 0, 'f', 1));
        updateValue(m_tags[UP_Disp_Rate], strUnitMsg[UNIT_L_MIN].arg(1.0 * m_historyInfo[UP_Disp_Rate].value1, 0, 'f', 1));
        updateValue(m_tags[RO_Feed_Rate], strUnitMsg[UNIT_L_MIN].arg(1.0 * m_historyInfo[RO_Feed_Rate].value1, 0, 'f', 1));
        updateValue(m_tags[RO_Product_Rate], strUnitMsg[UNIT_L_MIN].arg(1.0 * m_historyInfo[RO_Product_Rate].value1, 0, 'f', 1));
        updateValue(m_tags[Tap_Rate], strUnitMsg[UNIT_L_MIN].arg(1.0 * m_historyInfo[Tap_Rate].value1, 0, 'f', 1));
        updateValue(m_tags[EDI_Product_Rate], strUnitMsg[UNIT_L_MIN].arg(1.0 * m_historyInfo[EDI_Product_Rate].value1, 0, 'f', 1));
        updateValue(m_tags[EDI_Reject_Rate], strUnitMsg[UNIT_L_MIN].arg(1.0 * m_historyInfo[EDI_Reject_Rate].value1, 0, 'f', 1));
        updateValue(m_tags[RO_Reject_Rate], strUnitMsg[UNIT_L_MIN].arg(1.0 * m_historyInfo[RO_Reject_Rate].value1, 0, 'f', 1));
    }
}

void Ex_WaterQualityPage::updHistoryTank()
{
    updateValue(m_tags[Source_Tank_Level],
                strUnitMsg[UNIT_PERCENTAGE].arg(m_historyInfo[Source_Tank_Level].value1),
                strUnitMsg[UNIT_VOLUME].arg(m_historyInfo[Source_Tank_Level].value2, 0, 'f', 0));

    updateValue(m_tags[Pure_Tank_Level],
                strUnitMsg[UNIT_PERCENTAGE].arg(m_historyInfo[Pure_Tank_Level].value1),
                strUnitMsg[UNIT_VOLUME].arg(m_historyInfo[Pure_Tank_Level].value2, 0, 'f', 0));
}

void Ex_WaterQualityPage::updHistoryTOC()
{
    updateValue(m_tags[TOC_Value], strUnitMsg[UNIT_PPB].arg(m_historyInfo[TOC_Value].value1));
}

void Ex_WaterQualityPage::updFlowInfo(int iIndex,int iValue)
{
    float fUnit = 1.0;
    m_aulFlowMeter[iIndex] = iValue;

    QString strFlowUnit = strUnitMsg[UNIT_L_MIN];

    if ((FLOW_VELOCITY_UINT_GAL_PER_MIN == gGlobalParam.MiscParam.iUint4FlowVelocity))
    {
       fUnit =  toGallon(1);
       strFlowUnit = strUnitMsg[UNIT_G_MIN];
    }

    switch(iIndex)
    {
    case APP_FM_FM1_NO:
    {
        if (DispGetUpQtwFlag()
            || DispGetUpCirFlag())
        {
            if (0 != m_historyInfo[HP_Disp_Rate].value1)
            {
                updateValue(m_tags[HP_Disp_Rate], strFlowUnit.arg(0));
                m_historyInfo[HP_Disp_Rate].value1 = 0;
            }
            updateValue(m_tags[UP_Disp_Rate], strFlowUnit.arg(toOneDecimal(fUnit*(iValue*1.0)/1000)));
            m_historyInfo[UP_Disp_Rate].value1 = ((iValue*1.0)/1000);
        }
        else if(DispGetEdiQtwFlag() || DispGetTankCirFlag()) //0629
        {
            if (0 != m_historyInfo[UP_Disp_Rate].value1)
            {
                updateValue(m_tags[UP_Disp_Rate], strFlowUnit.arg(0));
                m_historyInfo[UP_Disp_Rate].value1 = 0;
            }
            updateValue(m_tags[HP_Disp_Rate], strFlowUnit.arg(toOneDecimal(fUnit*(iValue*1.0)/1000)));

            m_historyInfo[HP_Disp_Rate].value1 = ((iValue*1.0)/1000);
        }
        else
        {
            if (0 != m_historyInfo[UP_Disp_Rate].value1)
            {
                updateValue(m_tags[UP_Disp_Rate], strFlowUnit.arg(0));
                m_historyInfo[UP_Disp_Rate].value1 = 0;
            }
            if (0 != m_historyInfo[HP_Disp_Rate].value1)
            {
                updateValue(m_tags[HP_Disp_Rate], strFlowUnit.arg(0));
                m_historyInfo[HP_Disp_Rate].value1 = 0;
            }
        }
        break;
    }
    case APP_FM_FM2_NO:
        updateValue(m_tags[RO_Feed_Rate], strFlowUnit.arg(toTwoDecimal(fUnit*(60.0*iValue)/1000)));
        m_historyInfo[RO_Feed_Rate].value1 = ((60.0*iValue)/1000);
        break;
   case APP_FM_FM3_NO:
        updateValue(m_tags[RO_Product_Rate], strFlowUnit.arg(toTwoDecimal(fUnit*(60.0*iValue)/1000)));
        m_historyInfo[RO_Product_Rate].value1 = ((60.0*iValue)/1000);

        updateValue(m_tags[Tap_Rate], strFlowUnit.arg(toTwoDecimal(fUnit*(60.0*(m_aulFlowMeter[APP_FM_FM3_NO] + m_aulFlowMeter[APP_FM_FM4_NO]))/1000)));
        m_historyInfo[Tap_Rate].value1 = ((60.0*(m_aulFlowMeter[APP_FM_FM3_NO] + m_aulFlowMeter[APP_FM_FM4_NO]))/1000);

        updateValue(m_tags[EDI_Product_Rate], strFlowUnit.arg(toTwoDecimal(fUnit*(60.0*0.8*iValue)/1000)));
        m_historyInfo[EDI_Product_Rate].value1 = ((60.0*0.8*iValue)/1000);

        updateValue(m_tags[EDI_Reject_Rate], strFlowUnit.arg(toTwoDecimal(fUnit*(60.0*0.2*iValue)/1000)));
        m_historyInfo[EDI_Reject_Rate].value1 = ((60.0*0.2*iValue)/1000);
        break;
   case APP_FM_FM4_NO:
        updateValue(m_tags[RO_Reject_Rate], strFlowUnit.arg(toTwoDecimal(fUnit*(60.0*iValue)/1000)));
        m_historyInfo[RO_Reject_Rate].value1 = ((60.0*iValue)/1000);

        updateValue(m_tags[Tap_Rate], strFlowUnit.arg(toTwoDecimal(fUnit*(60.0*(m_aulFlowMeter[APP_FM_FM3_NO] + m_aulFlowMeter[APP_FM_FM4_NO]))/1000)));
        m_historyInfo[Tap_Rate].value1 = ((60.0*(m_aulFlowMeter[APP_FM_FM3_NO] + m_aulFlowMeter[APP_FM_FM4_NO]))/1000);
        break;
   }
}

void Ex_WaterQualityPage::updTank(int iLevel,float fVolume)
{
    updateValue(m_tags[Pure_Tank_Level],
                strUnitMsg[UNIT_PERCENTAGE].arg(iLevel),
                strUnitMsg[UNIT_VOLUME].arg(fVolume, 0, 'f', 0));
    m_historyInfo[Pure_Tank_Level].value1 = iLevel;
    m_historyInfo[Pure_Tank_Level].value2 = fVolume;
}

void Ex_WaterQualityPage::updSourceTank(int iLevel, float fVolume)
{
    updateValue(m_tags[Source_Tank_Level],
                strUnitMsg[UNIT_PERCENTAGE].arg(iLevel),
                strUnitMsg[UNIT_VOLUME].arg(fVolume, 0, 'f', 0));
    m_historyInfo[Source_Tank_Level].value1 = iLevel;
    m_historyInfo[Source_Tank_Level].value2 = fVolume;
}

void Ex_WaterQualityPage::setBackColor()
{
    QSize size(width(),height());

    QImage image_bg = QImage(size, QImage::Format_ARGB32);

    QPainter p(&image_bg);

    p.fillRect(image_bg.rect(), QColor(228, 231, 240)); //

    QPalette pal(m_widget->palette());

    pal.setBrush(m_widget->backgroundRole(),QBrush(image_bg));

    m_widget->setAutoFillBackground(true);
    m_widget->setPalette(pal);
}

void Ex_WaterQualityPage::update()
{
    if(gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_HaveTOC))
    {
        if(!m_cfglist.contains(m_tags[TOC_Value]))
        {
            initConfigList();
        }
    }
    else
    {
        if(m_cfglist.contains(m_tags[TOC_Value]))
        {
            initConfigList();
        }
    }

    if(gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_HaveB3))
    {
        if(!m_cfglist.contains(m_tags[Source_Tank_Level]))
        {
            initConfigList();
        }
    }
    else
    {
        if(m_cfglist.contains(m_tags[Source_Tank_Level]))
        {
            initConfigList();
        }
    }

    updAllInfo();
}

void Ex_WaterQualityPage::initUi()
{
    setBackColor();

    m_pQualityWidget = new DWaterQualityWidget(m_widget);
    m_pQualityWidget->setGeometry(10, 60, 780, 500);
}

