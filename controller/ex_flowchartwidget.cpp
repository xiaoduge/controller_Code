#include "ex_flowchartwidget.h"
#include "mainwindow.h"
#include "cminterface.h"

#include <QPainter>
#include <QTimer>

Ex_FlowChartWidget::Ex_FlowChartWidget(QWidget *parent) :
    QWidget(parent)
{
    loadPixmap();
    initDefaultValue();
    initUI();

    for(int i = 0; i < DEVICE_STATE_NUM; i++)
    {
        m_deviceState[i] = false;
    }
    m_updateTimerID = this->startTimer(1000);
    m_updPump = 0;
}

void Ex_FlowChartWidget::updateSwitchInfo()
{
    unsigned int ulTempMaks = DispGetSwitchState(APP_EXE_SWITCHS_MASK);

    m_deviceState[E1_STATE] = ulTempMaks & (1 << APP_EXE_E1_NO) ? true : false;
    m_deviceState[E2_STATE] = ulTempMaks & (1 << APP_EXE_E2_NO) ? true : false;
    m_deviceState[E3_STATE] = ulTempMaks & (1 << APP_EXE_E3_NO) ? true : false;
    m_deviceState[E4_STATE] = ulTempMaks & (1 << APP_EXE_E4_NO) ? true : false;
    m_deviceState[E5_STATE] = ulTempMaks & (1 << APP_EXE_E5_NO) ? true : false;
    m_deviceState[E6_STATE] = ulTempMaks & (1 << APP_EXE_E6_NO) ? true : false;
    m_deviceState[E9_STATE] = ulTempMaks & (1 << APP_EXE_E9_NO) ? true : false;
    m_deviceState[E10_STATE] = ulTempMaks & (1 << APP_EXE_E10_NO) ? true : false;

    m_deviceState[N1_STATE] = ulTempMaks & (1 << APP_EXE_N1_NO) ? true : false;
    m_deviceState[N2_STATE] = ulTempMaks & (1 << APP_EXE_N2_NO) ? true : false;
    m_deviceState[N3_STATE] = ulTempMaks & (1 << APP_EXE_N3_NO) ? true : false;
    m_deviceState[EDI_STATE] = ulTempMaks & (1 << APP_EXE_T1_NO) ? true : false;


    m_deviceState[C1_STATE] = ulTempMaks & (1 << APP_EXE_C1_NO) ? true : false;
    m_deviceState[C2_STATE] = ulTempMaks & (1 << APP_EXE_C2_NO) ? true : false;
    m_deviceState[C3_STATE] = ulTempMaks & (1 << APP_EXE_C3_NO) ? true : false;
    m_deviceState[C4_STATE] = ulTempMaks & (1 << APP_EXE_C4_NO) ? true : false;

    if(ulTempMaks & (1 << APP_EXE_N1_NO))
    {
        DispGetOtherCurrent(APP_EXE_N1_NO,&m_detectionPara[N1_DETECTION].iValueI);
    }
    if(ulTempMaks & (1 << APP_EXE_N2_NO))
    {
        DispGetOtherCurrent(APP_EXE_N2_NO,&m_detectionPara[N2_DETECTION].iValueI);
    }
    if(ulTempMaks & (1 << APP_EXE_N3_NO))
    {
        DispGetOtherCurrent(APP_EXE_N3_NO,&m_detectionPara[N3_DETECTION].iValueI);
    }
    if(ulTempMaks & (1 << APP_EXE_T1_NO))
    {
        DispGetOtherCurrent(APP_EXE_T1_NO,&m_detectionPara[EDI_DETECTION].iValueI);
    }
    if(ulTempMaks & (1 << APP_EXE_C1_NO))
    {
        DispGetRPumpRti(APP_EXE_C1_NO,&m_detectionPara[C1_DETECTION].iValueV, &m_detectionPara[C1_DETECTION].iValueI);
    }
    if(ulTempMaks & (1 << APP_EXE_C2_NO))
    {
        DispGetRPumpRti(APP_EXE_C2_NO,&m_detectionPara[C2_DETECTION].iValueV, &m_detectionPara[C2_DETECTION].iValueI);
    }
    if(ulTempMaks & (1 << APP_EXE_C3_NO))
    {
        DispGetOtherCurrent(APP_EXE_C3_NO,&m_detectionPara[C3_DETECTION].iValueI);
    }
    if(ulTempMaks & (1 << APP_EXE_C4_NO))
    {
        DispGetOtherCurrent(APP_EXE_C4_NO,&m_detectionPara[C4_DETECTION].iValueI);
    }

}

void Ex_FlowChartWidget::updateRpumpInfo(int iChl)
{
    unsigned int ulTempMaks = DispGetSwitchState(APP_EXE_SWITCHS_MASK);
    switch(iChl)
    {
    case 0:
        m_deviceState[C1_STATE] = ulTempMaks & (1 << APP_EXE_C1_NO) ? true : false;
        if(ulTempMaks & (1 << APP_EXE_C1_NO))
        {
            DispGetRPumpRti(APP_EXE_C1_NO,&m_detectionPara[C1_DETECTION].iValueV, &m_detectionPara[C1_DETECTION].iValueI);
        }
        break;
    case 1:
        m_deviceState[C2_STATE] = ulTempMaks & (1 << APP_EXE_C2_NO) ? true : false;
        if(ulTempMaks & (1 << APP_EXE_C2_NO))
        {
            DispGetRPumpRti(APP_EXE_C2_NO,&m_detectionPara[C2_DETECTION].iValueV, &m_detectionPara[C2_DETECTION].iValueI);
        }
        break;
    default:
        break;
    }

}

void Ex_FlowChartWidget::updateGpumpInfo(int iChl)
{
    switch(iChl)
    {
    case 0:
        DispGetOtherCurrent(APP_EXE_C3_NO,&m_detectionPara[C3_DETECTION].iValueI);
        break;
    case 1:
        DispGetOtherCurrent(APP_EXE_C4_NO,&m_detectionPara[C4_DETECTION].iValueI);
        break;
    default:
        break;
    }
}

void Ex_FlowChartWidget::updateRectInfo(int iChl)
{
    switch(iChl)
    {
    case 0:
        DispGetOtherCurrent(APP_EXE_N1_NO,&m_detectionPara[N1_DETECTION].iValueI);
        break;
    case 1:
        DispGetOtherCurrent(APP_EXE_N2_NO,&m_detectionPara[N2_DETECTION].iValueI);
        break;
    case 2:
        DispGetOtherCurrent(APP_EXE_N3_NO,&m_detectionPara[N3_DETECTION].iValueI);
        break;
    default:
        break;
    }
}

void Ex_FlowChartWidget::updateEdiInfo(int iChl)
{
    switch(iChl)
    {
    case 0:
        DispGetOtherCurrent(APP_EXE_T1_NO,&m_detectionPara[EDI_DETECTION].iValueI);
        break;
    default:
        break;
    }
}

void Ex_FlowChartWidget::updTank(int iIndex,float fVolume)
{
    m_iTankLevel = iIndex;
    m_fTankLevel = fVolume;
}

void Ex_FlowChartWidget::updEcoInfo(int iIndex, ECO_INFO_STRU *info)
{
    float fQ;
    float fT;

    switch(iIndex)
    {
    case APP_EXE_I5_NO:
    case APP_EXE_I4_NO:
    case APP_EXE_I3_NO:
        if (CONDUCTIVITY_UINT_OMG == gGlobalParam.MiscParam.iUint4Conductivity)
        {
            fQ = info->fQuality;
        }
        else
        {
            fQ = toConductivity(info->fQuality);
        }
        break;
    default:
        fQ = info->fQuality;
        break;
    }

    if (TEMERATURE_UINT_CELSIUS == gGlobalParam.MiscParam.iUint4Temperature)
    {
        fT = info->fTemperature;
    }
    else
    {
        fT = toFahrenheit(info->fTemperature);
    }

    switch(iIndex)
    {
    case APP_EXE_I5_NO:
        {

            if (DispGetUpQtwFlag() || DispGetUpCirFlag())
            {
                m_waterInfo[I5_VALUE].fQuality = fQ;
                m_waterInfo[I5_VALUE].fTemp = fT;
            }
        }
        break;
    case APP_EXE_I4_NO:
        {
            if (DispGetTubeCirFlag())
            {
            }
            else if (DispGetTocCirFlag())
            {
                m_waterInfo[I4_VALUE].fQuality = fQ;
                m_waterInfo[I4_VALUE].fTemp = fT;
            }
            else if (DispGetEdiQtwFlag() //0629
                     &&(MACHINE_PURIST != gGlobalParam.iMachineType)
                     &&(MACHINE_RO != gGlobalParam.iMachineType))
            {
                m_waterInfo[I4_VALUE].fQuality = fQ;
                m_waterInfo[I4_VALUE].fTemp = fT;
            }
            else if (DispGetTankCirFlag()
                && (MACHINE_PURIST != gGlobalParam.iMachineType))
            {
                m_waterInfo[I4_VALUE].fQuality = fQ;
                m_waterInfo[I4_VALUE].fTemp = fT;
            }
        }
        break;
    case APP_EXE_I3_NO:
        {
            if(gGlobalParam.iMachineType != MACHINE_RO)
            {
                m_waterInfo[I3_VALUE].fQuality = fQ;
                m_waterInfo[I3_VALUE].fTemp = fT;
            }
            else
            {
                if (DispGetEdiQtwFlag() || DispGetTankCirFlag())
                {
                    m_waterInfo[I3_VALUE].fQuality = fQ;
                    m_waterInfo[I3_VALUE].fTemp = fT;
                }
            }
        }
        break;
    case APP_EXE_I2_NO: // RO Out
        {
            float fResidue;

            if (DispGetREJ(&fResidue)
                && (MACHINE_PURIST != gGlobalParam.iMachineType))
            {
                m_fResidue = fResidue;
            }

            m_waterInfo[I2_VALUE].fQuality = fQ;
            m_waterInfo[I2_VALUE].fTemp = fT;

        }
        break;
    case APP_EXE_I1_NO: // RO In
        {
            m_waterInfo[I1_VALUE].fQuality = fQ;
            m_waterInfo[I1_VALUE].fTemp = fT;
        }
        break;
    default:
        break;
    }
}

void Ex_FlowChartWidget::updPressure(int iIndex, float fvalue)
{
    if (APP_EXE_PM1_NO == iIndex)
    {

        if (PRESSURE_UINT_BAR == gGlobalParam.MiscParam.iUint4Pressure)
        {
            m_fWorkPressure = fvalue;
        }
        else if (PRESSURE_UINT_MPA == gGlobalParam.MiscParam.iUint4Pressure)
        {
            m_fWorkPressure = toMpa(fvalue);
        }
        else
        {
            m_fWorkPressure = toPsi(fvalue);
        }
    }
}

void Ex_FlowChartWidget::updFlowInfo(int iIndex,int iValue)
{
    switch(iIndex)
    {
    case APP_FM_FM1_NO:
    {
        if (DispGetUpQtwFlag()
            || DispGetUpCirFlag()
            || DispGetEdiQtwFlag()
            || DispGetTankCirFlag())
        {
            m_flowRate[S1_VALUE] = iValue*1.0/1000;
        }

        else
        {
            m_flowRate[S1_VALUE] = 0;
        }
        break;
   }
   case APP_FM_FM2_NO:
        m_flowRate[S2_VALUE] = (60.0*iValue)/1000;
        break;
   case APP_FM_FM3_NO:
        m_flowRate[S3_VALUE] = (60.0*iValue)/1000;
        break;
   case APP_FM_FM4_NO:
        m_flowRate[S4_VALUE] = (60.0*iValue)/1000;
        break;
   }
}

void Ex_FlowChartWidget::updSourceTank(float fvalue)
{
}

void Ex_FlowChartWidget::updTOC(float fToc)
{
    m_fToc = fToc;
}

void Ex_FlowChartWidget::setTitleText(const QString &text)
{
    m_strWorkStatus[0] = text;
}

void Ex_FlowChartWidget::setInfo1(const QString &text)
{
    m_strWorkStatus[1] = text;
}

void Ex_FlowChartWidget::setInfo2(const QString &text)
{
    m_strWorkStatus[2] = text;
}

void Ex_FlowChartWidget::setInfo3(const QString &text)
{
    m_strWorkStatus[3] = text;
}

void Ex_FlowChartWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_L_EDI_LOOP:
    case MACHINE_L_RO_LOOP:
        break;
    case MACHINE_Genie:
        if (gGlobalParam.MiscParam.ulMisFlags & (1 << DISP_SM_HP_Water_Cir))
        {
            paintGenie_G_HPCIR(painter);
        }
        else
        {
            paintGenie_G(painter);
        }
        paintGenie_G_Tags(painter);
        painterGenie_G_Value(painter);
        break;

    case MACHINE_UP:
            if(gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_HaveTOC)
              && gGlobalParam.MiscParam.ulMisFlags & (1 << DISP_SM_HP_Water_Cir))
            {
                paintGenie_U_TOC_HPCIR(painter);
                paintGenie_U_Tags(painter, true);
                painterGenie_U_Value(painter, true, true);
            }
            else if(gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_HaveTOC)
                    && !(gGlobalParam.MiscParam.ulMisFlags & (1 << DISP_SM_HP_Water_Cir)))
            {
                paintGenie_U_TOC(painter);
                paintGenie_U_Tags(painter, true);
                painterGenie_U_Value(painter, false, true);
            }
            else if(gGlobalParam.MiscParam.ulMisFlags & (1 << DISP_SM_HP_Water_Cir)
                    && !(gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_HaveTOC)))
            {
                paintGenie_U_HPCIR(painter);
                paintGenie_U_Tags(painter);
                painterGenie_U_Value(painter, true);
            }
            else
            {
                paintGenie_U(painter);
                paintGenie_U_Tags(painter);
                painterGenie_U_Value(painter);
            }
            break;
    case MACHINE_EDI:
        if(gGlobalParam.MiscParam.ulMisFlags & (1 << DISP_SM_HP_Water_Cir))
        {
            paintGenie_E_HPCIR(painter);
            paintGenie_E_Tags(painter);
            painterGenie_E_Value(painter, true);
        }
        else
        {
            paintGenie_E(painter);
            paintGenie_E_Tags(painter);
            painterGenie_E_Value(painter);
        }
        break;
    case MACHINE_RO:
        if(gGlobalParam.MiscParam.ulMisFlags & (1 << DISP_SM_HP_Water_Cir))
        {
            paintGenie_R_HPCIR(painter);
            paintGenie_R_Tags(painter);
            painterGenie_R_Value(painter, true);
        }
        else
        {
            paintGenie_R(painter);
            paintGenie_R_Tags(painter);
            painterGenie_R_Value(painter);
        }
        break;
    case MACHINE_PURIST:
        if(gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_HaveTOC))
        {
            paintGenie_PURIST_TOC(painter);
            paintGenie_PURIST_Tags(painter, true);
            painterGenie_PURIST_Value(painter, true);
        }
        else
        {
            paintGenie_PURIST(painter);
            paintGenie_PURIST_Tags(painter);
            painterGenie_PURIST_Value(painter);
        }
        break;
    case MACHINE_ADAPT:
        if(gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_HaveTOC))
        {
            paintGenie_A_TOC(painter);
            paintGenie_A_Tags(painter, true);
            painterGenie_A_Value(painter, true);
        }
        else
        {
            paintGenie_A(painter);
            paintGenie_A_Tags(painter);
            painterGenie_A_Value(painter);
        }
        break;
    default:
        break;
    }
    paintWorkStatus(painter);

    return QWidget::paintEvent(event);
}

void Ex_FlowChartWidget::timerEvent(QTimerEvent *event)
{
    if(m_updateTimerID == event->timerId())
    {
        repaint();
    }
}

void Ex_FlowChartWidget::paintGenie_G(QPainter &painter)
{
    painter.drawPixmap(0, 15, m_backgroundPix[GENIE_G]);
    paintGenie_G_TankLevel(painter);
    if(m_isUpdate)
    {
        if(m_deviceState[N1_STATE]) painter.drawPixmap(347, 181, m_devicePix[UV_ON_H]); //N1
        if(m_deviceState[N2_STATE]) painter.drawPixmap(535, 390, m_devicePix[UV_ON_H]); //N2

        if(m_deviceState[N3_STATE])
        {
            painter.drawPixmap(323, 332,  m_devicePix[UV_ON_V]); //TANK UV
        }
        else
        {
            painter.drawPixmap(323, 332,  m_devicePix[UV_OFF_V]);
        }

        if(0 == m_updPump)
        {
            if(m_deviceState[C1_STATE]) painter.drawPixmap(191, 79, m_devicePix[PUMP_ON1]); //C1 RO
            if(m_deviceState[C2_STATE]) painter.drawPixmap(526, 429,m_devicePix[PUMP_ON1]); //C2 CIR
            m_updPump = 1;
        }
        else if(1 == m_updPump)
        {
            if(m_deviceState[C1_STATE]) painter.drawPixmap(191, 79, m_devicePix[PUMP_ON2]); //C1 RO
            if(m_deviceState[C2_STATE]) painter.drawPixmap(526, 429,m_devicePix[PUMP_ON2]); //C2 CIR
            m_updPump = 2;
        }

        else
        {
            m_updPump = 0;
        }

        if(m_deviceState[E1_STATE]) painter.drawPixmap(568, 89, m_devicePix[VALVE_ON]);  //E1
        if(m_deviceState[E2_STATE]) painter.drawPixmap(125, 374, m_devicePix[VALVE_ON]); //E2
        if(m_deviceState[E3_STATE]) painter.drawPixmap(166, 272, m_devicePix[VALVE_E3_H2]); //E3

        if(!m_deviceState[E3_STATE] && !m_deviceState[E2_STATE] && m_deviceState[E1_STATE])
        {
            painter.drawPixmap(166, 272, m_devicePix[VALVE_E3_H1]); //E3
        }

        if(m_deviceState[E4_STATE]) painter.drawPixmap(401, 437, m_devicePix[VALVE_ON]); //E4
        if(m_deviceState[E5_STATE]) painter.drawPixmap(528, 468, m_devicePix[VALVE_ON]); //E5
        if(m_deviceState[E6_STATE]) painter.drawPixmap(598, 175, m_devicePix[VALVE_ON]); //E6
        if(DispGetUpQtwFlag()) painter.drawPixmap(650, 364, m_devicePix[VALVE_ON]); //E7 UP
        if(DispGetEdiQtwFlag()) painter.drawPixmap(650, 175, m_devicePix[VALVE_ON]); //E8 HP

        if(m_deviceState[E9_STATE])painter.drawPixmap(598, 410, m_devicePix[VALVE_E3_V2]); //E9
        if(!m_deviceState[E9_STATE] && m_deviceState[E5_STATE])
        {
            painter.drawPixmap(598, 410, m_devicePix[VALVE_E3_V1]); //E9
        }

        if(m_deviceState[EDI_STATE]) painter.drawPixmap(218, 177, m_devicePix[EDI_ON]);

        m_isUpdate = false;
    }
    else
    {
        if(0 == m_updPump)
        {
            if(m_deviceState[C1_STATE]) painter.drawPixmap(191, 79, m_devicePix[PUMP_ON1]); //C1 RO
            if(m_deviceState[C2_STATE]) painter.drawPixmap(526, 429,m_devicePix[PUMP_ON1]); //C2 CIR
            m_updPump = 1;
        }
        else if(1 == m_updPump)
        {
            if(m_deviceState[C1_STATE]) painter.drawPixmap(191, 79, m_devicePix[PUMP_ON2]); //C1 RO
            if(m_deviceState[C2_STATE]) painter.drawPixmap(526, 429,m_devicePix[PUMP_ON2]); //C2 CIR
            m_updPump = 2;
        }

        else
        {
            m_updPump = 0;
        }

        painter.drawPixmap(323, 332,  m_devicePix[UV_OFF_V]);
        m_isUpdate = true;
    }
}

void Ex_FlowChartWidget::paintGenie_G_HPCIR(QPainter &painter)
{
    painter.drawPixmap(0, 15, m_backgroundPix[GENIE_G_HPCIR]);
    paintGenie_G_TankLevel(painter);
    if(m_isUpdate)
    {
        if(m_deviceState[N1_STATE]) painter.drawPixmap(347, 181, m_devicePix[UV_ON_H]); //N1
        if(m_deviceState[N2_STATE]) painter.drawPixmap(535, 390, m_devicePix[UV_ON_H]); //N2

        if(m_deviceState[N3_STATE])
        {
            painter.drawPixmap(323, 332,  m_devicePix[UV_ON_V]); //TANK UV
        }
        else
        {
            painter.drawPixmap(323, 332,  m_devicePix[UV_OFF_V]);
        }

        if(0 == m_updPump)
        {
            if(m_deviceState[C1_STATE]) painter.drawPixmap(191, 79, m_devicePix[PUMP_ON1]); //C1 RO
            if(m_deviceState[C2_STATE]) painter.drawPixmap(526, 429,m_devicePix[PUMP_ON1]); //C2 CIR
            m_updPump = 1;
        }
        else if(1 == m_updPump)
        {
            if(m_deviceState[C1_STATE]) painter.drawPixmap(191, 79, m_devicePix[PUMP_ON2]); //C1 RO
            if(m_deviceState[C2_STATE]) painter.drawPixmap(526, 429,m_devicePix[PUMP_ON2]); //C2 CIR
            m_updPump = 2;
        }

        else
        {
            m_updPump = 0;
        }

        if(m_deviceState[E1_STATE]) painter.drawPixmap(568, 89, m_devicePix[VALVE_ON]);  //E1
        if(m_deviceState[E2_STATE]) painter.drawPixmap(125, 374, m_devicePix[VALVE_ON]); //E2
        if(m_deviceState[E3_STATE]) painter.drawPixmap(166, 272, m_devicePix[VALVE_E3_H2]); //E3
        if(!m_deviceState[E3_STATE] && !m_deviceState[E2_STATE] && m_deviceState[E1_STATE])
        {
            painter.drawPixmap(166, 272, m_devicePix[VALVE_E3_H1]); //E3
        }
        if(m_deviceState[E4_STATE]) painter.drawPixmap(401, 437, m_devicePix[VALVE_ON]); //E4
        if(m_deviceState[E5_STATE]) painter.drawPixmap(528, 468, m_devicePix[VALVE_ON]); //E5
        if(m_deviceState[E6_STATE]) painter.drawPixmap(598, 175, m_devicePix[VALVE_ON]); //E6
        if(DispGetUpQtwFlag()) painter.drawPixmap(650, 364, m_devicePix[VALVE_ON]); //E7 UP
        if(DispGetEdiQtwFlag()) painter.drawPixmap(650, 175, m_devicePix[VALVE_ON]); //E8 HP

        if(m_deviceState[E9_STATE])painter.drawPixmap(598, 410, m_devicePix[VALVE_E3_V2]); //E9
        if(!m_deviceState[E9_STATE] && m_deviceState[E5_STATE])
        {
            painter.drawPixmap(598, 410, m_devicePix[VALVE_E3_V1]); //E9
        }

        if(m_deviceState[EDI_STATE]) painter.drawPixmap(218, 177, m_devicePix[EDI_ON]);

        m_isUpdate = false;
    }
    else
    {
        if(0 == m_updPump)
        {
            if(m_deviceState[C1_STATE]) painter.drawPixmap(191, 79, m_devicePix[PUMP_ON1]); //C1 RO
            if(m_deviceState[C2_STATE]) painter.drawPixmap(526, 429,m_devicePix[PUMP_ON1]); //C2 CIR
            m_updPump = 1;
        }
        else if(1 == m_updPump)
        {
            if(m_deviceState[C1_STATE]) painter.drawPixmap(191, 79, m_devicePix[PUMP_ON2]); //C1 RO
            if(m_deviceState[C2_STATE]) painter.drawPixmap(526, 429,m_devicePix[PUMP_ON2]); //C2 CIR
            m_updPump = 2;
        }

        else
        {
            m_updPump = 0;
        }

        painter.drawPixmap(323, 332,  m_devicePix[UV_OFF_V]);
        m_isUpdate = true;
    }
}

void Ex_FlowChartWidget::paintGenie_G_Tags(QPainter &painter)
{
    QFont oldFont = painter.font();
    painter.setFont(QFont("", 12, QFont::Bold));

    painter.drawText(370, 208, "N1");
    painter.drawText(558, 416, "N2");
    painter.drawText(321, 320, "N3");
    painter.drawText(200, 125, "C1");
    painter.drawText(559, 445, "C2");

    painter.drawText(575, 122, "E1");
    painter.drawText(130, 406, "E2");
    painter.drawText(190, 273, "E3");
    painter.drawText(407, 470, "E4");
    painter.drawText(559, 476, "E5");
    painter.drawText(605, 208, "E6");

    painter.drawText(656, 396, "E7");
    painter.drawText(656, 208, "E8");

    painter.drawText(615, 448, "E9");

//    painter.drawText(277, 128, "I1");
//    painter.drawText(120, 312, "I2");
//    painter.drawText(299, 216, "I3");
//    painter.drawText(431, 425, "I4");
//    painter.drawText(512, 326, "I5");
    painter.drawText(223, 250, "EDI");

    painter.setFont(oldFont);
}

void Ex_FlowChartWidget::painterGenie_G_Value(QPainter &painter)
{
    QString strUs  = tr("us");
    int iPrecision = 1;

    QPen oldPen = painter.pen();
    QFont oldFont = painter.font();
    painter.setFont(QFont("", 12, QFont::Bold));
    painter.setPen(QPen(Qt::blue));

    painter.drawText(259, 72,
                     QString::number(m_waterInfo[I1_VALUE].fQuality, 'f', 0) + QString(" ") + strUs);
    painter.drawText(259, 86,
                     QString::number(m_waterInfo[I1_VALUE].fTemp, 'f', 1)  + QString(" ")+ m_strUnit[TEMP_UNIT]);

    painter.drawText(100, 250,
                     QString::number(m_waterInfo[I2_VALUE].fQuality, 'f', 1) + QString(" ") + strUs);
    painter.drawText(100, 264,
                     QString::number(m_waterInfo[I2_VALUE].fTemp, 'f', 1)  + QString(" ")+ m_strUnit[TEMP_UNIT]);

    iPrecision = m_waterInfo[I3_VALUE].fQuality > 1.0 ? 1: 2;
    painter.drawText(289, 155,
                     QString::number(m_waterInfo[I3_VALUE].fQuality, 'f', iPrecision) + QString(" ") + m_strUnit[RES_COND_UNIT]);
    painter.drawText(289, 169,
                     QString::number(m_waterInfo[I3_VALUE].fTemp, 'f', 1)  + QString(" ") + m_strUnit[TEMP_UNIT]);

    iPrecision = m_waterInfo[I4_VALUE].fQuality > 1.0 ? 1: 2;
    painter.drawText(410, 420,
                     QString::number(m_waterInfo[I4_VALUE].fQuality, 'f', iPrecision) + QString(" ") + m_strUnit[RES_COND_UNIT]);
    painter.drawText(410, 434,
                     QString::number(m_waterInfo[I4_VALUE].fTemp, 'f', 1)  + QString(" ") + m_strUnit[TEMP_UNIT]);

    iPrecision = m_waterInfo[I5_VALUE].fQuality > 1.0 ? 1: 2;
    painter.drawText(470, 325,
                     QString::number(m_waterInfo[I5_VALUE].fQuality, 'f', iPrecision) + QString(" ") + m_strUnit[RES_COND_UNIT]);
    painter.drawText(470, 339,
                     QString::number(m_waterInfo[I5_VALUE].fTemp, 'f', 1)  + QString(" ") + m_strUnit[TEMP_UNIT]);

    if (PRESSURE_UINT_MPA == gGlobalParam.MiscParam.iUint4Pressure)
    {
        painter.drawText(51, 372, QString::number(m_fWorkPressure, 'f', 2) + m_strUnit[PRESSURE_UNIT]);
    }
    else
    {
        painter.drawText(51, 372, QString::number(m_fWorkPressure, 'f', 1) + m_strUnit[PRESSURE_UNIT]);
    }

    painter.drawText(481, 470, QString::number(m_flowRate[S1_VALUE], 'f', 1) + " L/min");

    painter.drawText(2, 415,
                     "C1:  " + QString("%1mA %2V").arg(m_detectionPara[C1_DETECTION].iValueI).arg(m_detectionPara[C1_DETECTION].iValueV/1000));
    painter.drawText(2, 430,
                     "C2:  " + QString("%1mA %2V").arg(m_detectionPara[C2_DETECTION].iValueI).arg(m_detectionPara[C2_DETECTION].iValueV/1000));
    painter.drawText(2, 445,
                     "N1:  " + QString("%1mA").arg(m_detectionPara[N1_DETECTION].iValueI));
    painter.drawText(2, 460,
                     "N2:  " + QString("%1mA").arg(m_detectionPara[N2_DETECTION].iValueI));
    painter.drawText(2, 475,
                     "N3:  " + QString("%1mA").arg(m_detectionPara[N3_DETECTION].iValueI));
    painter.drawText(2, 490,
                     "EDI: " + QString("%1mA").arg(m_detectionPara[EDI_DETECTION].iValueI));
    if (gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_HaveTOC))
    {
        painter.drawText(110, 475,
                         tr("TOC") + ": " + QString::number(m_fToc, 'f', 0) + QString("ppb"));
    }

    painter.drawText(110, 490,
                     tr("RO Rejection") + ": " + QString::number(m_fResidue, 'f', 1) + QString("%"));

    painter.drawText(355, 375, QString("%1%").arg(m_iTankLevel));

    painter.setFont(oldFont);
    painter.setPen(oldPen);
}

void Ex_FlowChartWidget::paintGenie_G_TankLevel(QPainter &painter)
{
    QColor levelColor(40, 150, 200);
    QPen oldPen = painter.pen();
    QBrush oldBrush = painter.brush();
    painter.setPen(QPen(levelColor, 1, Qt::DotLine));
    painter.setBrush(QBrush(levelColor));
    painter.setRenderHint(QPainter::Antialiasing, true);

    int iTankLevel = m_iTankLevel > 100 ? 100 : m_iTankLevel;
    double offset = 53*(1 - iTankLevel*1.0/100);
    QRectF tankLevelRect(QPointF(307, 333 + offset),
                         QPointF(349, 386)); //Tank Level
    painter.drawRect(tankLevelRect);

    painter.setPen(oldPen);
    painter.setBrush(oldBrush);
}

void Ex_FlowChartWidget::paintGenie_E(QPainter &painter)
{
    painter.drawPixmap(0, 15, m_backgroundPix[GENIE_E]);
    paintGenie_E_TankLevel(painter);
    if(m_isUpdate)
    {
        if(m_deviceState[N1_STATE]) painter.drawPixmap(347, 181, m_devicePix[UV_ON_H]); //N1
        if(m_deviceState[N3_STATE])
        {
            painter.drawPixmap(323, 332,  m_devicePix[UV_ON_V]); //TANK UV
        }
        else
        {
            painter.drawPixmap(323, 332,  m_devicePix[UV_OFF_V]);
        }

        if(0 == m_updPump)
        {
            if(m_deviceState[C1_STATE]) painter.drawPixmap(191, 79, m_devicePix[PUMP_ON1]); //C1 RO
            if(m_deviceState[C2_STATE]) painter.drawPixmap(493, 429,m_devicePix[PUMP_ON1]); //C2 CIR
            m_updPump = 1;
        }
        else if(1 == m_updPump)
        {
            if(m_deviceState[C1_STATE]) painter.drawPixmap(191, 79, m_devicePix[PUMP_ON2]); //C1 RO
            if(m_deviceState[C2_STATE]) painter.drawPixmap(493, 429,m_devicePix[PUMP_ON2]); //C2 CIR
            m_updPump = 2;
        }

        else
        {
            m_updPump = 0;
        }


        if(m_deviceState[E1_STATE]) painter.drawPixmap(568, 89, m_devicePix[VALVE_ON]);  //E1
        if(m_deviceState[E2_STATE]) painter.drawPixmap(125, 374, m_devicePix[VALVE_ON]); //E2
        if(m_deviceState[E3_STATE]) painter.drawPixmap(166, 272, m_devicePix[VALVE_E3_H2]); //E3
        if(!m_deviceState[E3_STATE] && !m_deviceState[E2_STATE] && m_deviceState[E1_STATE])
        {
            painter.drawPixmap(166, 272, m_devicePix[VALVE_E3_H1]); //E3
        }
        if(m_deviceState[E4_STATE]) painter.drawPixmap(401, 437, m_devicePix[VALVE_ON]); //E4
        if(m_deviceState[E6_STATE]) painter.drawPixmap(528, 272, m_devicePix[VALVE_ON]); //E6

        if(DispGetEdiQtwFlag()) painter.drawPixmap(604, 271, m_devicePix[VALVE_ON]); //E8 HP

        if(m_deviceState[EDI_STATE]) painter.drawPixmap(218, 177, m_devicePix[EDI_ON]);

        m_isUpdate = false;
    }
    else
    {
        if(0 == m_updPump)
        {
            if(m_deviceState[C1_STATE]) painter.drawPixmap(191, 79, m_devicePix[PUMP_ON1]); //C1 RO
            if(m_deviceState[C2_STATE]) painter.drawPixmap(493, 429,m_devicePix[PUMP_ON1]); //C2 CIR
            m_updPump = 1;
        }
        else if(1 == m_updPump)
        {
            if(m_deviceState[C1_STATE]) painter.drawPixmap(191, 79, m_devicePix[PUMP_ON2]); //C1 RO
            if(m_deviceState[C2_STATE]) painter.drawPixmap(493, 429,m_devicePix[PUMP_ON2]); //C2 CIR
            m_updPump = 2;
        }

        else
        {
            m_updPump = 0;
        }

        painter.drawPixmap(323, 332,  m_devicePix[UV_OFF_V]);
        m_isUpdate = true;
    }
}

void Ex_FlowChartWidget::paintGenie_E_HPCIR(QPainter &painter)
{
    painter.drawPixmap(0, 15, m_backgroundPix[GENIE_E_HPCIR]);
    paintGenie_E_TankLevel(painter);
    if(m_isUpdate)
    {
        if(m_deviceState[N1_STATE]) painter.drawPixmap(347, 181, m_devicePix[UV_ON_H]); //N1

        if(m_deviceState[N3_STATE])
        {
            painter.drawPixmap(323, 332,  m_devicePix[UV_ON_V]); //TANK UV
        }
        else
        {
            painter.drawPixmap(323, 332,  m_devicePix[UV_OFF_V]);
        }
        if(0 == m_updPump)
        {
            if(m_deviceState[C1_STATE]) painter.drawPixmap(191, 79, m_devicePix[PUMP_ON1]); //C1 RO
            if(m_deviceState[C2_STATE]) painter.drawPixmap(493, 429,m_devicePix[PUMP_ON1]); //C2 CIR
            m_updPump = 1;
        }
        else if(1 == m_updPump)
        {
            if(m_deviceState[C1_STATE]) painter.drawPixmap(191, 79, m_devicePix[PUMP_ON2]); //C1 RO
            if(m_deviceState[C2_STATE]) painter.drawPixmap(493, 429,m_devicePix[PUMP_ON2]); //C2 CIR
            m_updPump = 2;
        }

        else
        {
            m_updPump = 0;
        }


        if(m_deviceState[E1_STATE]) painter.drawPixmap(568, 89, m_devicePix[VALVE_ON]);  //E1
        if(m_deviceState[E2_STATE]) painter.drawPixmap(125, 374, m_devicePix[VALVE_ON]); //E2
        if(m_deviceState[E3_STATE]) painter.drawPixmap(166, 272, m_devicePix[VALVE_E3_H2]); //E3
        if(!m_deviceState[E3_STATE] && !m_deviceState[E2_STATE] && m_deviceState[E1_STATE])
        {
            painter.drawPixmap(166, 272, m_devicePix[VALVE_E3_H1]); //E3
        }
        if(m_deviceState[E4_STATE]) painter.drawPixmap(401, 437, m_devicePix[VALVE_ON]); //E4
        if(m_deviceState[E6_STATE]) painter.drawPixmap(528, 272, m_devicePix[VALVE_ON]); //E6

        if(DispGetEdiQtwFlag()) painter.drawPixmap(604, 271, m_devicePix[VALVE_ON]); //E8 HP

        if(m_deviceState[EDI_STATE]) painter.drawPixmap(218, 177, m_devicePix[EDI_ON]);

        m_isUpdate = false;
    }
    else
    {
        if(0 == m_updPump)
        {
            if(m_deviceState[C1_STATE]) painter.drawPixmap(191, 79, m_devicePix[PUMP_ON1]); //C1 RO
            if(m_deviceState[C2_STATE]) painter.drawPixmap(493, 429,m_devicePix[PUMP_ON1]); //C2 CIR
            m_updPump = 1;
        }
        else if(1 == m_updPump)
        {
            if(m_deviceState[C1_STATE]) painter.drawPixmap(191, 79, m_devicePix[PUMP_ON2]); //C1 RO
            if(m_deviceState[C2_STATE]) painter.drawPixmap(493, 429,m_devicePix[PUMP_ON2]); //C2 CIR
            m_updPump = 2;
        }

        else
        {
            m_updPump = 0;
        }

        painter.drawPixmap(323, 332,  m_devicePix[UV_OFF_V]);
        m_isUpdate = true;
    }
}

void Ex_FlowChartWidget::paintGenie_E_Tags(QPainter &painter)
{
    QFont oldFont = painter.font();
    painter.setFont(QFont("", 12, QFont::Bold));
    painter.drawText(370, 208, "N1");

    painter.drawText(321, 320, "N3");
    painter.drawText(200, 125, "C1");
    painter.drawText(524, 470, "C2");

    painter.drawText(575, 122, "E1");
    painter.drawText(130, 406, "E2");
    painter.drawText(190, 273, "E3");
    painter.drawText(407, 470, "E4");
    painter.drawText(533, 305, "E6");
    painter.drawText(611, 302, "E8");

    painter.drawText(223, 250, "EDI");

    painter.setFont(oldFont);
}

void Ex_FlowChartWidget::painterGenie_E_Value(QPainter &painter, bool isHPCir)
{
    int iPrecision = 1;
    QString strUs  = tr("us");

    QPen oldPen = painter.pen();
    QFont oldFont = painter.font();
    painter.setFont(QFont("", 12, QFont::Bold));
    painter.setPen(QPen(Qt::blue));


    painter.drawText(259, 72,
                     QString::number(m_waterInfo[I1_VALUE].fQuality, 'f', 0) + QString(" ") + strUs);
    painter.drawText(259, 86,
                     QString::number(m_waterInfo[I1_VALUE].fTemp, 'f', 1)  + QString(" ")+ m_strUnit[TEMP_UNIT]);

    painter.drawText(100, 250,
                     QString::number(m_waterInfo[I2_VALUE].fQuality, 'f', 1) + QString(" ") + strUs);
    painter.drawText(100, 264,
                     QString::number(m_waterInfo[I2_VALUE].fTemp, 'f', 1)  + QString(" ")+ m_strUnit[TEMP_UNIT]);

    iPrecision = m_waterInfo[I3_VALUE].fQuality > 1.0 ? 1: 2;
    painter.drawText(289, 155,
                     QString::number(m_waterInfo[I3_VALUE].fQuality, 'f', iPrecision) + QString(" ") + m_strUnit[RES_COND_UNIT]);
    painter.drawText(289, 169,
                     QString::number(m_waterInfo[I3_VALUE].fTemp, 'f', 1)  + QString(" ") + m_strUnit[TEMP_UNIT]);

    if(isHPCir)
    {
        iPrecision = m_waterInfo[I4_VALUE].fQuality > 1.0 ? 1: 2;
        painter.drawText(598, 412,
                        QString::number(m_waterInfo[I4_VALUE].fQuality, 'f', iPrecision) + QString(" ") + m_strUnit[RES_COND_UNIT]);
        painter.drawText(598, 426,
                        QString::number(m_waterInfo[I4_VALUE].fTemp, 'f', 1)  + QString(" ") + m_strUnit[TEMP_UNIT]);
    }

    if (PRESSURE_UINT_MPA == gGlobalParam.MiscParam.iUint4Pressure)
    {
        painter.drawText(51, 372, QString::number(m_fWorkPressure, 'f', 2) + m_strUnit[PRESSURE_UNIT]);
    }
    else
    {
        painter.drawText(51, 372, QString::number(m_fWorkPressure, 'f', 1) + m_strUnit[PRESSURE_UNIT]);
    }

    painter.drawText(435, 470, QString::number(m_flowRate[S1_VALUE], 'f', 1) + " L/min");
    painter.drawText(355, 375, QString("%1%").arg(m_iTankLevel));

    painter.drawText(2, 430,
                     "C1:  " + QString("%1mA %2V").arg(m_detectionPara[C1_DETECTION].iValueI).arg(m_detectionPara[C1_DETECTION].iValueV/1000));
    painter.drawText(2, 445,
                     "C2:  " + QString("%1mA %2V").arg(m_detectionPara[C2_DETECTION].iValueI).arg(m_detectionPara[C2_DETECTION].iValueV/1000));
    painter.drawText(2, 460,
                     "N1:  " + QString("%1mA").arg(m_detectionPara[N1_DETECTION].iValueI));
    painter.drawText(2, 475,
                     "N3:  " + QString("%1mA").arg(m_detectionPara[N3_DETECTION].iValueI));
    painter.drawText(2, 490,
                     "EDI: " + QString("%1mA").arg(m_detectionPara[EDI_DETECTION].iValueI));

    painter.drawText(110, 490,
                     tr("RO Rejection") + ": " + QString::number(m_fResidue, 'f', 1) + QString("%"));

    painter.setFont(oldFont);
    painter.setPen(oldPen);
}

void Ex_FlowChartWidget::paintGenie_E_TankLevel(QPainter &painter)
{
    QColor levelColor(40, 150, 200);
    QPen oldPen = painter.pen();
    QBrush oldBrush = painter.brush();
    painter.setPen(QPen(levelColor, 1, Qt::DotLine));
    painter.setBrush(QBrush(levelColor));
    painter.setRenderHint(QPainter::Antialiasing, true);

    int iTankLevel = m_iTankLevel > 100 ? 100 : m_iTankLevel;
    double offset = 53*(1 - iTankLevel*1.0/100);
    QRectF tankLevelRect(QPointF(307, 333 + offset),
                         QPointF(349, 386)); //Tank Level
    painter.drawRect(tankLevelRect);

    painter.setPen(oldPen);
    painter.setBrush(oldBrush);
}

void Ex_FlowChartWidget::paintGenie_U(QPainter &painter)
{
    painter.drawPixmap(0, 15, m_backgroundPix[GENIE_U]);
    paintGenie_U_TankLevel(painter);
    if(m_isUpdate)
    {
        if(m_deviceState[N2_STATE]) painter.drawPixmap(487, 290, m_devicePix[UV_ON_H]); //N2
        if(m_deviceState[N3_STATE])
        {
            painter.drawPixmap(280, 332,  m_devicePix[UV_ON_V]); //TANK UV
        }
        else
        {
            painter.drawPixmap(280, 332,  m_devicePix[UV_OFF_V]);
        }
        if(0 == m_updPump)
        {
            if(m_deviceState[C1_STATE]) painter.drawPixmap(192, 77, m_devicePix[PUMP_ON1]); //C1 RO
            if(m_deviceState[C2_STATE]) painter.drawPixmap(527, 427,m_devicePix[PUMP_ON1]); //C2 CIR
            m_updPump = 1;
        }
        else if(1 == m_updPump)
        {
            if(m_deviceState[C1_STATE]) painter.drawPixmap(192, 77, m_devicePix[PUMP_ON2]); //C1 RO
            if(m_deviceState[C2_STATE]) painter.drawPixmap(527, 427,m_devicePix[PUMP_ON2]); //C2 CIR
            m_updPump = 2;
        }

        else
        {
            m_updPump = 0;
        }


        if(m_deviceState[E1_STATE]) painter.drawPixmap(569, 88, m_devicePix[VALVE_ON]);  //E1
        if(m_deviceState[E2_STATE]) painter.drawPixmap(126, 373, m_devicePix[VALVE_ON]); //E2
        if(m_deviceState[E3_STATE]) painter.drawPixmap(167, 271, m_devicePix[VALVE_E3_H2]); //E3
        if(!m_deviceState[E3_STATE] && !m_deviceState[E2_STATE] && m_deviceState[E1_STATE])
        {
            painter.drawPixmap(167, 271, m_devicePix[VALVE_E3_H1]); //E3
        }
        if(m_deviceState[E4_STATE]) painter.drawPixmap(402, 436, m_devicePix[VALVE_ON]); //E4
        if(m_deviceState[E5_STATE]) painter.drawPixmap(529, 467, m_devicePix[VALVE_ON]); //E5
        if(m_deviceState[E6_STATE]) painter.drawPixmap(599, 174, m_devicePix[VALVE_ON]); //E6

        if(DispGetUpQtwFlag()) painter.drawPixmap(651, 363, m_devicePix[VALVE_ON]); //E7 UP
        if(DispGetEdiQtwFlag()) painter.drawPixmap(651, 174, m_devicePix[VALVE_ON]); //E8 HP

        m_isUpdate = false;
    }
    else
    {
        if(0 == m_updPump)
        {
            if(m_deviceState[C1_STATE]) painter.drawPixmap(192, 77, m_devicePix[PUMP_ON1]); //C1 RO
            if(m_deviceState[C2_STATE]) painter.drawPixmap(527, 427, m_devicePix[PUMP_ON1]); //C2 CIR
            m_updPump = 1;
        }
        else if(1 == m_updPump)
        {
            if(m_deviceState[C1_STATE]) painter.drawPixmap(192, 77, m_devicePix[PUMP_ON2]); //C1 RO
            if(m_deviceState[C2_STATE]) painter.drawPixmap(527, 427, m_devicePix[PUMP_ON2]); //C2 CIR
            m_updPump = 2;
        }

        else
        {
            m_updPump = 0;
        }

        painter.drawPixmap(280, 332,  m_devicePix[UV_OFF_V]);
        m_isUpdate = true;
    }
}

void Ex_FlowChartWidget::paintGenie_U_TOC(QPainter &painter)
{
    painter.drawPixmap(0, 15, m_backgroundPix[GENIE_U_TOC]);
    paintGenie_U_TankLevel(painter);
    if(m_isUpdate)
    {
        if(m_deviceState[N2_STATE]) painter.drawPixmap(487, 290, m_devicePix[UV_ON_H]); //N2
        if(m_deviceState[N3_STATE])
        {
            painter.drawPixmap(280, 332,  m_devicePix[UV_ON_V]); //TANK UV
        }
        else
        {
            painter.drawPixmap(280, 332,  m_devicePix[UV_OFF_V]);
        }
        if(0 == m_updPump)
        {
            if(m_deviceState[C1_STATE]) painter.drawPixmap(192, 77, m_devicePix[PUMP_ON1]); //C1 RO
            if(m_deviceState[C2_STATE]) painter.drawPixmap(527, 427,m_devicePix[PUMP_ON1]); //C2 CIR
            m_updPump = 1;
        }
        else if(1 == m_updPump)
        {
            if(m_deviceState[C1_STATE]) painter.drawPixmap(192, 77, m_devicePix[PUMP_ON2]); //C1 RO
            if(m_deviceState[C2_STATE]) painter.drawPixmap(527, 427,m_devicePix[PUMP_ON2]); //C2 CIR
            m_updPump = 2;
        }

        else
        {
            m_updPump = 0;
        }


        if(m_deviceState[E1_STATE]) painter.drawPixmap(569, 88, m_devicePix[VALVE_ON]);  //E1
        if(m_deviceState[E2_STATE]) painter.drawPixmap(126, 373, m_devicePix[VALVE_ON]); //E2
        if(m_deviceState[E3_STATE]) painter.drawPixmap(167, 271, m_devicePix[VALVE_E3_H2]); //E3
        if(!m_deviceState[E3_STATE] && !m_deviceState[E2_STATE] && m_deviceState[E1_STATE])
        {
            painter.drawPixmap(167, 271, m_devicePix[VALVE_E3_H1]); //E3
        }
        if(m_deviceState[E4_STATE]) painter.drawPixmap(402, 436, m_devicePix[VALVE_ON]); //E4
        if(m_deviceState[E5_STATE]) painter.drawPixmap(529, 467, m_devicePix[VALVE_ON]); //E5
        if(m_deviceState[E6_STATE]) painter.drawPixmap(599, 174, m_devicePix[VALVE_ON]); //E6

        if(DispGetUpQtwFlag()) painter.drawPixmap(651, 363, m_devicePix[VALVE_ON]); //E7 UP
        if(DispGetEdiQtwFlag()) painter.drawPixmap(651, 174, m_devicePix[VALVE_ON]); //E8 HP

        if(m_deviceState[E9_STATE]) painter.drawPixmap(557, 285, m_devicePix[VALVE_E3_H3]); //E9
        if(!m_deviceState[E9_STATE] && m_deviceState[E5_STATE])
        {
            painter.drawPixmap(557, 285, m_devicePix[VALVE_E3_H4]); //E9
        }

        m_isUpdate = false;
    }
    else
    {
        if(0 == m_updPump)
        {
            if(m_deviceState[C1_STATE]) painter.drawPixmap(192, 77, m_devicePix[PUMP_ON1]); //C1 RO
            if(m_deviceState[C2_STATE]) painter.drawPixmap(527, 427, m_devicePix[PUMP_ON1]); //C2 CIR
            m_updPump = 1;
        }
        else if(1 == m_updPump)
        {
            if(m_deviceState[C1_STATE]) painter.drawPixmap(192, 77, m_devicePix[PUMP_ON2]); //C1 RO
            if(m_deviceState[C2_STATE]) painter.drawPixmap(527, 427, m_devicePix[PUMP_ON2]); //C2 CIR
            m_updPump = 2;
        }

        else
        {
            m_updPump = 0;
        }

        painter.drawPixmap(280, 332,  m_devicePix[UV_OFF_V]);
        m_isUpdate = true;
    }
}

void Ex_FlowChartWidget::paintGenie_U_HPCIR(QPainter &painter)
{
    painter.drawPixmap(0, 15, m_backgroundPix[GENIE_U_HPCIR]);
    paintGenie_U_TankLevel(painter);
    if(m_isUpdate)
    {
        if(m_deviceState[N2_STATE]) painter.drawPixmap(487, 290, m_devicePix[UV_ON_H]); //N2
        if(m_deviceState[N3_STATE])
        {
            painter.drawPixmap(280, 332,  m_devicePix[UV_ON_V]); //TANK UV
        }
        else
        {
            painter.drawPixmap(280, 332,  m_devicePix[UV_OFF_V]);
        }
        if(0 == m_updPump)
        {
            if(m_deviceState[C1_STATE]) painter.drawPixmap(192, 77, m_devicePix[PUMP_ON1]); //C1 RO
            if(m_deviceState[C2_STATE]) painter.drawPixmap(527, 427,m_devicePix[PUMP_ON1]); //C2 CIR
            m_updPump = 1;
        }
        else if(1 == m_updPump)
        {
            if(m_deviceState[C1_STATE]) painter.drawPixmap(192, 77, m_devicePix[PUMP_ON2]); //C1 RO
            if(m_deviceState[C2_STATE]) painter.drawPixmap(527, 427,m_devicePix[PUMP_ON2]); //C2 CIR
            m_updPump = 2;
        }

        else
        {
            m_updPump = 0;
        }


        if(m_deviceState[E1_STATE]) painter.drawPixmap(569, 88, m_devicePix[VALVE_ON]);  //E1
        if(m_deviceState[E2_STATE]) painter.drawPixmap(126, 373, m_devicePix[VALVE_ON]); //E2
        if(m_deviceState[E3_STATE]) painter.drawPixmap(167, 271, m_devicePix[VALVE_E3_H2]); //E3
        if(!m_deviceState[E3_STATE] && !m_deviceState[E2_STATE] && m_deviceState[E1_STATE])
        {
            painter.drawPixmap(167, 271, m_devicePix[VALVE_E3_H1]); //E3
        }
        if(m_deviceState[E4_STATE]) painter.drawPixmap(402, 436, m_devicePix[VALVE_ON]); //E4
        if(m_deviceState[E5_STATE]) painter.drawPixmap(529, 467, m_devicePix[VALVE_ON]); //E5
        if(m_deviceState[E6_STATE]) painter.drawPixmap(599, 174, m_devicePix[VALVE_ON]); //E6

        if(DispGetUpQtwFlag()) painter.drawPixmap(651, 363, m_devicePix[VALVE_ON]); //E7 UP
        if(DispGetEdiQtwFlag()) painter.drawPixmap(651, 174, m_devicePix[VALVE_ON]); //E8 HP

        m_isUpdate = false;
    }
    else
    {
        if(0 == m_updPump)
        {
            if(m_deviceState[C1_STATE]) painter.drawPixmap(192, 77, m_devicePix[PUMP_ON1]); //C1 RO
            if(m_deviceState[C2_STATE]) painter.drawPixmap(527, 427, m_devicePix[PUMP_ON1]); //C2 CIR
            m_updPump = 1;
        }
        else if(1 == m_updPump)
        {
            if(m_deviceState[C1_STATE]) painter.drawPixmap(192, 77, m_devicePix[PUMP_ON2]); //C1 RO
            if(m_deviceState[C2_STATE]) painter.drawPixmap(527, 427, m_devicePix[PUMP_ON2]); //C2 CIR
            m_updPump = 2;
        }

        else
        {
            m_updPump = 0;
        }

        painter.drawPixmap(280, 332,  m_devicePix[UV_OFF_V]);
        m_isUpdate = true;
    }
}

void Ex_FlowChartWidget::paintGenie_U_TOC_HPCIR(QPainter &painter)
{
    painter.drawPixmap(0, 15, m_backgroundPix[GENIE_U_TOC_HPCIR]);
    paintGenie_U_TankLevel(painter);
    if(m_isUpdate)
    {
        if(m_deviceState[N2_STATE]) painter.drawPixmap(487, 290, m_devicePix[UV_ON_H]); //N2
        if(m_deviceState[N3_STATE])
        {
            painter.drawPixmap(280, 332,  m_devicePix[UV_ON_V]); //TANK UV
        }
        else
        {
            painter.drawPixmap(280, 332,  m_devicePix[UV_OFF_V]);
        }
        if(0 == m_updPump)
        {
            if(m_deviceState[C1_STATE]) painter.drawPixmap(192, 77, m_devicePix[PUMP_ON1]); //C1 RO
            if(m_deviceState[C2_STATE]) painter.drawPixmap(527, 427,m_devicePix[PUMP_ON1]); //C2 CIR
            m_updPump = 1;
        }
        else if(1 == m_updPump)
        {
            if(m_deviceState[C1_STATE]) painter.drawPixmap(192, 77, m_devicePix[PUMP_ON2]); //C1 RO
            if(m_deviceState[C2_STATE]) painter.drawPixmap(527, 427,m_devicePix[PUMP_ON2]); //C2 CIR
            m_updPump = 2;
        }

        else
        {
            m_updPump = 0;
        }


        if(m_deviceState[E1_STATE]) painter.drawPixmap(569, 88, m_devicePix[VALVE_ON]);  //E1
        if(m_deviceState[E2_STATE]) painter.drawPixmap(126, 373, m_devicePix[VALVE_ON]); //E2
        if(m_deviceState[E3_STATE]) painter.drawPixmap(167, 271, m_devicePix[VALVE_E3_H2]); //E3
        if(!m_deviceState[E3_STATE] && !m_deviceState[E2_STATE] && m_deviceState[E1_STATE])
        {
            painter.drawPixmap(167, 271, m_devicePix[VALVE_E3_H1]); //E3
        }
        if(m_deviceState[E4_STATE]) painter.drawPixmap(402, 436, m_devicePix[VALVE_ON]); //E4
        if(m_deviceState[E5_STATE]) painter.drawPixmap(529, 467, m_devicePix[VALVE_ON]); //E5
        if(m_deviceState[E6_STATE]) painter.drawPixmap(599, 174, m_devicePix[VALVE_ON]); //E6

        if(DispGetUpQtwFlag()) painter.drawPixmap(651, 363, m_devicePix[VALVE_ON]); //E7 UP
        if(DispGetEdiQtwFlag()) painter.drawPixmap(651, 174, m_devicePix[VALVE_ON]); //E8 HP

        if(m_deviceState[E9_STATE]) painter.drawPixmap(557, 285, m_devicePix[VALVE_E3_H3]); //E9
        if(!m_deviceState[E9_STATE] && m_deviceState[E5_STATE])
        {
            painter.drawPixmap(557, 285, m_devicePix[VALVE_E3_H4]); //E9
        }

        m_isUpdate = false;
    }
    else
    {
        if(0 == m_updPump)
        {
            if(m_deviceState[C1_STATE]) painter.drawPixmap(192, 77, m_devicePix[PUMP_ON1]); //C1 RO
            if(m_deviceState[C2_STATE]) painter.drawPixmap(527, 427, m_devicePix[PUMP_ON1]); //C2 CIR
            m_updPump = 1;
        }
        else if(1 == m_updPump)
        {
            if(m_deviceState[C1_STATE]) painter.drawPixmap(192, 77, m_devicePix[PUMP_ON2]); //C1 RO
            if(m_deviceState[C2_STATE]) painter.drawPixmap(527, 427, m_devicePix[PUMP_ON2]); //C2 CIR
            m_updPump = 2;
        }

        else
        {
            m_updPump = 0;
        }

        painter.drawPixmap(280, 332,  m_devicePix[UV_OFF_V]);
        m_isUpdate = true;
    }
}

void Ex_FlowChartWidget::paintGenie_U_Tags(QPainter &painter, bool isHaveTOC)
{
    QFont oldFont = painter.font();
    painter.setFont(QFont("", 12, QFont::Bold));

    painter.drawText(506, 316, "N2");
    painter.drawText(278, 318, "N3");
    painter.drawText(200, 125, "C1");
    painter.drawText(559, 465, "C2");

    painter.drawText(575, 122, "E1");
    painter.drawText(130, 406, "E2");
    painter.drawText(190, 273, "E3");
    painter.drawText(407, 470, "E4");
    painter.drawText(559, 496, "E5");
    painter.drawText(605, 208, "E6");

    painter.drawText(656, 396, "E7");
    painter.drawText(656, 208, "E8");

    if(isHaveTOC)
    {
        painter.drawText(575, 323, "E9");
    }

    painter.setFont(oldFont);
}

void Ex_FlowChartWidget::painterGenie_U_Value(QPainter &painter, bool isHPCir, bool isHaveTOC)
{
    int iPrecision = 1;
    QString strUs  = tr("us");

    QPen oldPen = painter.pen();
    QFont oldFont = painter.font();
    painter.setFont(QFont("", 12, QFont::Bold));
    painter.setPen(QPen(Qt::blue));

    //
    painter.drawText(259, 72,
                     QString::number(m_waterInfo[I1_VALUE].fQuality, 'f', 0) + QString(" ") + strUs);
    painter.drawText(259, 86,
                     QString::number(m_waterInfo[I1_VALUE].fTemp, 'f', 1)  + QString(" ")+ m_strUnit[TEMP_UNIT]);

    painter.drawText(100, 250,
                     QString::number(m_waterInfo[I2_VALUE].fQuality, 'f', 1) + QString(" ") + strUs);
    painter.drawText(100, 264,
                     QString::number(m_waterInfo[I2_VALUE].fTemp, 'f', 1)  + QString(" ")+ m_strUnit[TEMP_UNIT]);

    if(isHPCir)
    {
        iPrecision = m_waterInfo[I3_VALUE].fQuality > 1.0 ? 1: 2;
        painter.drawText(535, 420,
                         QString::number(m_waterInfo[I3_VALUE].fQuality, 'f', iPrecision) + QString(" ") + m_strUnit[RES_COND_UNIT]);
        painter.drawText(557, 434,
                         QString::number(m_waterInfo[I3_VALUE].fTemp, 'f', 1)  + QString(" ") + m_strUnit[TEMP_UNIT]);
    }

    if(isHaveTOC)
    {
        iPrecision = m_waterInfo[I4_VALUE].fQuality > 1.0 ? 1: 2;
        painter.drawRect(395, 240, 80, 32);
        painter.drawLine(424, 273, 424, 285);
        painter.drawText(400, 254,
                         QString::number(m_waterInfo[I4_VALUE].fQuality, 'f', iPrecision) + QString(" ") + m_strUnit[RES_COND_UNIT]);
        painter.drawText(400, 268,
                         QString::number(m_waterInfo[I4_VALUE].fTemp, 'f', 1)  + QString(" ") + m_strUnit[TEMP_UNIT]);
    }

    iPrecision = m_waterInfo[I5_VALUE].fQuality > 1.0 ? 1: 2;
    painter.drawText(412, 340,
                     QString::number(m_waterInfo[I5_VALUE].fQuality, 'f', iPrecision) + QString(" ") + m_strUnit[RES_COND_UNIT]);
    painter.drawText(412, 354,
                     QString::number(m_waterInfo[I5_VALUE].fTemp, 'f', 1)  + QString(" ") + m_strUnit[TEMP_UNIT]);

    if (PRESSURE_UINT_MPA == gGlobalParam.MiscParam.iUint4Pressure)
    {
        painter.drawText(51, 372, QString::number(m_fWorkPressure, 'f', 2) + m_strUnit[PRESSURE_UNIT]);
    }
    else
    {
        painter.drawText(51, 372, QString::number(m_fWorkPressure, 'f', 1) + m_strUnit[PRESSURE_UNIT]);
    }

    painter.drawText(480, 470, QString::number(m_flowRate[S1_VALUE], 'f', 1) + " L/min");
    painter.drawText(312, 375, QString("%1%").arg(m_iTankLevel));

    painter.drawText(2, 445,
                     "C1:  " + QString("%1mA %2V").arg(m_detectionPara[C1_DETECTION].iValueI).arg(m_detectionPara[C1_DETECTION].iValueV/1000));
    painter.drawText(2, 460,
                     "C2:  " + QString("%1mA %2V").arg(m_detectionPara[C2_DETECTION].iValueI).arg(m_detectionPara[C2_DETECTION].iValueV/1000));
    painter.drawText(2, 475,
                     "N2:  " + QString("%1mA").arg(m_detectionPara[N2_DETECTION].iValueI));
    painter.drawText(2, 490,
                     "N3:  " + QString("%1mA").arg(m_detectionPara[N3_DETECTION].iValueI));

    if (gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_HaveTOC))
    {
        painter.drawText(110, 475,
                         tr("TOC") + ": " + QString::number(m_fToc, 'f', 0) + QString("ppb"));
    }
    painter.drawText(110, 490,
                     tr("RO Rejection") + ": " + QString::number(m_fResidue, 'f', 1) + QString("%"));


    painter.setFont(oldFont);
    painter.setPen(oldPen);
}

void Ex_FlowChartWidget::paintGenie_U_TankLevel(QPainter &painter)
{
    QColor levelColor(40, 150, 200);
    QPen oldPen = painter.pen();
    QBrush oldBrush = painter.brush();
    painter.setPen(QPen(levelColor, 1, Qt::DotLine));
    painter.setBrush(QBrush(levelColor));
    painter.setRenderHint(QPainter::Antialiasing, true);

    int iTankLevel = m_iTankLevel > 100 ? 100 : m_iTankLevel;
    double offset = 53*(1 - iTankLevel*1.0/100);
    QRectF tankLevelRect(QPointF(263, 333 + offset),
                         QPointF(305, 386)); //Tank Level
    painter.drawRect(tankLevelRect);

    painter.setPen(oldPen);
    painter.setBrush(oldBrush);
}

void Ex_FlowChartWidget::paintGenie_R(QPainter &painter)
{
    painter.drawPixmap(0, 15, m_backgroundPix[GENIE_R]);
    paintGenie_R_TankLevel(painter);
    if(m_isUpdate)
    {
        if(m_deviceState[N3_STATE])
        {
            painter.drawPixmap(323, 332,  m_devicePix[UV_ON_V]); //TANK UV
        }
        else
        {
            painter.drawPixmap(323, 332,  m_devicePix[UV_OFF_V]);
        }
        if(0 == m_updPump)
        {
            if(m_deviceState[C1_STATE]) painter.drawPixmap(191, 79, m_devicePix[PUMP_ON1]); //C1 RO
            if(m_deviceState[C2_STATE]) painter.drawPixmap(493, 429,m_devicePix[PUMP_ON1]); //C2 CIR
            m_updPump = 1;
        }
        else if(1 == m_updPump)
        {
            if(m_deviceState[C1_STATE]) painter.drawPixmap(191, 79, m_devicePix[PUMP_ON2]); //C1 RO
            if(m_deviceState[C2_STATE]) painter.drawPixmap(493, 429,m_devicePix[PUMP_ON2]); //C2 CIR
            m_updPump = 2;
        }

        else
        {
            m_updPump = 0;
        }


        if(m_deviceState[E1_STATE]) painter.drawPixmap(568, 90, m_devicePix[VALVE_ON]);  //E1
        if(m_deviceState[E2_STATE]) painter.drawPixmap(125, 374, m_devicePix[VALVE_ON]); //E2
        if(m_deviceState[E3_STATE]) painter.drawPixmap(166, 272, m_devicePix[VALVE_E3_H2]); //E3
        if(!m_deviceState[E3_STATE] && !m_deviceState[E2_STATE] && m_deviceState[E1_STATE])
        {
            painter.drawPixmap(166, 272, m_devicePix[VALVE_E3_H1]); //E3
        }
        if(m_deviceState[E4_STATE]) painter.drawPixmap(401, 437, m_devicePix[VALVE_ON]); //E4
        if(m_deviceState[E6_STATE]) painter.drawPixmap(528, 272, m_devicePix[VALVE_ON]); //E6

        if(DispGetEdiQtwFlag()) painter.drawPixmap(604, 271, m_devicePix[VALVE_ON]); //E8 HP


        m_isUpdate = false;
    }
    else
    {
        if(0 == m_updPump)
        {
            if(m_deviceState[C1_STATE]) painter.drawPixmap(191, 79, m_devicePix[PUMP_ON1]); //C1 RO
            if(m_deviceState[C2_STATE]) painter.drawPixmap(493, 429,m_devicePix[PUMP_ON1]); //C2 CIR
            m_updPump = 1;
        }
        else if(1 == m_updPump)
        {
            if(m_deviceState[C1_STATE]) painter.drawPixmap(191, 79, m_devicePix[PUMP_ON2]); //C1 RO
            if(m_deviceState[C2_STATE]) painter.drawPixmap(493, 429,m_devicePix[PUMP_ON2]); //C2 CIR
            m_updPump = 2;
        }

        else
        {
            m_updPump = 0;
        }

        painter.drawPixmap(323, 332,  m_devicePix[UV_OFF_V]);
        m_isUpdate = true;
    }

}

void Ex_FlowChartWidget::paintGenie_R_HPCIR(QPainter &painter)
{
    painter.drawPixmap(0, 15, m_backgroundPix[GENIE_R_HPCIR]);
    paintGenie_R_TankLevel(painter);
    if(m_isUpdate)
    {
        if(m_deviceState[N3_STATE])
        {
            painter.drawPixmap(323, 332,  m_devicePix[UV_ON_V]); //TANK UV
        }
        else
        {
            painter.drawPixmap(323, 332,  m_devicePix[UV_OFF_V]);
        }
        if(0 == m_updPump)
        {
            if(m_deviceState[C1_STATE]) painter.drawPixmap(191, 79, m_devicePix[PUMP_ON1]); //C1 RO
            if(m_deviceState[C2_STATE]) painter.drawPixmap(493, 429,m_devicePix[PUMP_ON1]); //C2 CIR
            m_updPump = 1;
        }
        else if(1 == m_updPump)
        {
            if(m_deviceState[C1_STATE]) painter.drawPixmap(191, 79, m_devicePix[PUMP_ON2]); //C1 RO
            if(m_deviceState[C2_STATE]) painter.drawPixmap(493, 429,m_devicePix[PUMP_ON2]); //C2 CIR
            m_updPump = 2;
        }

        else
        {
            m_updPump = 0;
        }


        if(m_deviceState[E1_STATE]) painter.drawPixmap(568, 90, m_devicePix[VALVE_ON]);  //E1
        if(m_deviceState[E2_STATE]) painter.drawPixmap(125, 374, m_devicePix[VALVE_ON]); //E2
        if(m_deviceState[E3_STATE]) painter.drawPixmap(166, 272, m_devicePix[VALVE_E3_H2]); //E3
        if(!m_deviceState[E3_STATE] && !m_deviceState[E2_STATE] && m_deviceState[E1_STATE])
        {
            painter.drawPixmap(166, 272, m_devicePix[VALVE_E3_H1]); //E3
        }
        if(m_deviceState[E4_STATE]) painter.drawPixmap(401, 437, m_devicePix[VALVE_ON]); //E4
        if(m_deviceState[E6_STATE]) painter.drawPixmap(528, 272, m_devicePix[VALVE_ON]); //E6

        if(DispGetEdiQtwFlag()) painter.drawPixmap(604, 271, m_devicePix[VALVE_ON]); //E8 HP


        m_isUpdate = false;
    }
    else
    {
        if(0 == m_updPump)
        {
            if(m_deviceState[C1_STATE]) painter.drawPixmap(191, 79, m_devicePix[PUMP_ON1]); //C1 RO
            if(m_deviceState[C2_STATE]) painter.drawPixmap(493, 429,m_devicePix[PUMP_ON1]); //C2 CIR
            m_updPump = 1;
        }
        else if(1 == m_updPump)
        {
            if(m_deviceState[C1_STATE]) painter.drawPixmap(191, 79, m_devicePix[PUMP_ON2]); //C1 RO
            if(m_deviceState[C2_STATE]) painter.drawPixmap(493, 429,m_devicePix[PUMP_ON2]); //C2 CIR
            m_updPump = 2;
        }

        else
        {
            m_updPump = 0;
        }

        painter.drawPixmap(323, 332,  m_devicePix[UV_OFF_V]);
        m_isUpdate = true;
    }
}

void Ex_FlowChartWidget::paintGenie_R_Tags(QPainter &painter)
{
    QFont oldFont = painter.font();
    painter.setFont(QFont("", 12, QFont::Bold));

    painter.drawText(321, 320, "N3");
    painter.drawText(200, 125, "C1");
    painter.drawText(524, 470, "C2");

    painter.drawText(575, 122, "E1");
    painter.drawText(130, 406, "E2");
    painter.drawText(190, 273, "E3");
    painter.drawText(407, 470, "E4");
    painter.drawText(533, 305, "E6");
    painter.drawText(611, 302, "E8");

    painter.setFont(oldFont);
}

void Ex_FlowChartWidget::painterGenie_R_Value(QPainter &painter, bool isHPCir)
{
    int iPrecision = 1;
    QString strUs  = tr("us");

    QPen oldPen = painter.pen();
    QFont oldFont = painter.font();
    painter.setFont(QFont("", 12, QFont::Bold));
    painter.setPen(QPen(Qt::blue));


    painter.drawText(259, 72,
                     QString::number(m_waterInfo[I1_VALUE].fQuality, 'f', 0) + QString(" ") + strUs);
    painter.drawText(259, 86,
                     QString::number(m_waterInfo[I1_VALUE].fTemp, 'f', 1)  + QString(" ")+ m_strUnit[TEMP_UNIT]);

    painter.drawText(100, 250,
                     QString::number(m_waterInfo[I2_VALUE].fQuality, 'f', 1) + QString(" ") + strUs);
    painter.drawText(100, 264,
                     QString::number(m_waterInfo[I2_VALUE].fTemp, 'f', 1)  + QString(" ")+ m_strUnit[TEMP_UNIT]);

    if(isHPCir)
    {
        iPrecision = m_waterInfo[I3_VALUE].fQuality > 1.0 ? 1: 2;
        painter.drawText(598, 412,
                        QString::number(m_waterInfo[I3_VALUE].fQuality, 'f', iPrecision) + QString(" ") + m_strUnit[RES_COND_UNIT]);
        painter.drawText(598, 426,
                        QString::number(m_waterInfo[I3_VALUE].fTemp, 'f', 1)  + QString(" ") + m_strUnit[TEMP_UNIT]);
    }
    if (PRESSURE_UINT_MPA == gGlobalParam.MiscParam.iUint4Pressure)
    {
        painter.drawText(51, 372, QString::number(m_fWorkPressure, 'f', 2) + m_strUnit[PRESSURE_UNIT]);
    }
    else
    {
        painter.drawText(51, 372, QString::number(m_fWorkPressure, 'f', 1) + m_strUnit[PRESSURE_UNIT]);
    }

    painter.drawText(435, 470, QString::number(m_flowRate[S1_VALUE], 'f', 1) + " L/min");
    painter.drawText(355, 375, QString("%1%").arg(m_iTankLevel));

    painter.drawText(2, 460,
                     "C1:  " + QString("%1mA %2V").arg(m_detectionPara[C1_DETECTION].iValueI).arg(m_detectionPara[C1_DETECTION].iValueV/1000));
    painter.drawText(2, 475,
                     "C2:  " + QString("%1mA %2V").arg(m_detectionPara[C2_DETECTION].iValueI).arg(m_detectionPara[C2_DETECTION].iValueV/1000));
    painter.drawText(2, 490,
                     "N3:  " + QString("%1mA").arg(m_detectionPara[N3_DETECTION].iValueI));


    painter.drawText(110, 490,
                     tr("RO Rejection") + ": " + QString::number(m_fResidue, 'f', 1) + QString("%"));
    painter.setFont(oldFont);
    painter.setPen(oldPen);
}

void Ex_FlowChartWidget::paintGenie_R_TankLevel(QPainter &painter)
{
    QColor levelColor(40, 150, 200);
    QPen oldPen = painter.pen();
    QBrush oldBrush = painter.brush();
    painter.setPen(QPen(levelColor, 1, Qt::DotLine));
    painter.setBrush(QBrush(levelColor));
    painter.setRenderHint(QPainter::Antialiasing, true);

    int iTankLevel = m_iTankLevel > 100 ? 100 : m_iTankLevel;
    double offset = 53*(1 - iTankLevel*1.0/100);
    QRectF tankLevelRect(QPointF(307, 333 + offset),
                         QPointF(349, 386)); //Tank Level
    painter.drawRect(tankLevelRect);

    painter.setPen(oldPen);
    painter.setBrush(oldBrush);
}

void Ex_FlowChartWidget::paintGenie_PURIST(QPainter &painter)
{
    painter.drawPixmap(0, 15, m_backgroundPix[GENIE_PURIST]);
    if(m_isUpdate)
    {
        if(m_deviceState[N2_STATE]) painter.drawPixmap(234, 191, m_devicePix[UV_ON_H]); //N2
        if(0 == m_updPump)
        {
            if(m_deviceState[C2_STATE]) painter.drawPixmap(185, 354,m_devicePix[PUMP_ON1]); //C2 CIR
            m_updPump = 1;
        }
        else if(1 == m_updPump)
        {
            if(m_deviceState[C2_STATE]) painter.drawPixmap(185, 354,m_devicePix[PUMP_ON2]); //C2 CIR
            m_updPump = 2;
        }

        else
        {
            m_updPump = 0;
        }


        if(m_deviceState[E4_STATE]) painter.drawPixmap(380, 363, m_devicePix[VALVE_ON]); //E4
        if(m_deviceState[E5_STATE]) painter.drawPixmap(380, 297, m_devicePix[VALVE_ON]); //E5

        if(DispGetUpQtwFlag()) painter.drawPixmap(476, 184, m_devicePix[VALVE_ON]); //E7 UP


        m_isUpdate = false;
    }
    else
    {
        if(0 == m_updPump)
        {
            if(m_deviceState[C2_STATE]) painter.drawPixmap(185, 354,m_devicePix[PUMP_ON1]); //C2 CIR
            m_updPump = 1;
        }
        else if(1 == m_updPump)
        {
            if(m_deviceState[C2_STATE]) painter.drawPixmap(185, 354,m_devicePix[PUMP_ON2]); //C2 CIR
            m_updPump = 2;
        }

        else
        {
            m_updPump = 0;
        }

        m_isUpdate = true;
    }
}

void Ex_FlowChartWidget::paintGenie_PURIST_TOC(QPainter &painter)
{
    painter.drawPixmap(0, 15, m_backgroundPix[GENIE_PURIST_TOC]);
    if(m_isUpdate)
    {
        if(m_deviceState[N2_STATE]) painter.drawPixmap(234, 191, m_devicePix[UV_ON_H]); //N2
        if(0 == m_updPump)
        {
            if(m_deviceState[C2_STATE]) painter.drawPixmap(185, 354,m_devicePix[PUMP_ON1]); //C2 CIR
            m_updPump = 1;
        }
        else if(1 == m_updPump)
        {
            if(m_deviceState[C2_STATE]) painter.drawPixmap(185, 354,m_devicePix[PUMP_ON2]); //C2 CIR
            m_updPump = 2;
        }

        else
        {
            m_updPump = 0;
        }


        if(m_deviceState[E4_STATE]) painter.drawPixmap(380, 363, m_devicePix[VALVE_ON]); //E4
        if(m_deviceState[E5_STATE]) painter.drawPixmap(380, 297, m_devicePix[VALVE_ON]); //E5

        if(DispGetUpQtwFlag()) painter.drawPixmap(476, 184, m_devicePix[VALVE_ON]); //E7 UP
        if(m_deviceState[E9_STATE]) painter.drawPixmap(197, 185, m_devicePix[VALVE_E3_H3]); //E9
        if(!m_deviceState[E9_STATE] && m_deviceState[E5_STATE])
        {
            painter.drawPixmap(197, 185, m_devicePix[VALVE_E3_H5]); //E9
        }

        m_isUpdate = false;
    }
    else
    {
        if(0 == m_updPump)
        {
            if(m_deviceState[C2_STATE]) painter.drawPixmap(185, 354,m_devicePix[PUMP_ON1]); //C2 CIR
            m_updPump = 1;
        }
        else if(1 == m_updPump)
        {
            if(m_deviceState[C2_STATE]) painter.drawPixmap(185, 354,m_devicePix[PUMP_ON2]); //C2 CIR
            m_updPump = 2;
        }

        else
        {
            m_updPump = 0;
        }

        m_isUpdate = true;
    }
}

void Ex_FlowChartWidget::paintGenie_PURIST_Tags(QPainter &painter, bool isHaveTOC)
{
    QFont oldFont = painter.font();
    painter.setFont(QFont("", 12, QFont::Bold));

    painter.drawText(255, 216, "N2");
    painter.drawText(190, 400, "C2");
    painter.drawText(385, 396, "E4");
    painter.drawText(385, 328, "E5");
    painter.drawText(480, 219, "E7");

    if(isHaveTOC)
    {
        painter.drawText(191, 185, "E9");
    }

    painter.setFont(oldFont);
}

void Ex_FlowChartWidget::painterGenie_PURIST_Value(QPainter &painter, bool isHaveTOC)
{
    int iPrecision = 1;
    QString strUs  = tr("us");

    QPen oldPen = painter.pen();
    QFont oldFont = painter.font();
    painter.setFont(QFont("", 12, QFont::Bold));
    painter.setPen(QPen(Qt::blue));
    //
    painter.drawText(301, 341,
                     QString::number(m_waterInfo[I2_VALUE].fQuality, 'f', 1) + QString(" ") + strUs);
    painter.drawText(301, 355,
                     QString::number(m_waterInfo[I2_VALUE].fTemp, 'f', 1)  + QString(" ")+ m_strUnit[TEMP_UNIT]);


     if(isHaveTOC)
     {
         iPrecision = m_waterInfo[I4_VALUE].fQuality > 1.0 ? 1: 2;
         painter.drawText(330, 166,
                          QString::number(m_waterInfo[I4_VALUE].fQuality, 'f', iPrecision) + QString(" ") + m_strUnit[RES_COND_UNIT]);
         painter.drawText(330, 180,
                          QString::number(m_waterInfo[I4_VALUE].fTemp, 'f', 1)  + QString(" ") + m_strUnit[TEMP_UNIT]);
     }

     iPrecision = m_waterInfo[I5_VALUE].fQuality > 1.0 ? 1: 2;
     painter.drawText(420, 166,
                      QString::number(m_waterInfo[I5_VALUE].fQuality, 'f', iPrecision) + QString(" ") + m_strUnit[RES_COND_UNIT]);
     painter.drawText(420, 180,
                      QString::number(m_waterInfo[I5_VALUE].fTemp, 'f', 1)  + QString(" ") + m_strUnit[TEMP_UNIT]);

    painter.drawText(236, 400, QString::number(m_flowRate[S1_VALUE], 'f', 1) + " L/min");

    painter.drawText(2, 475,
                     "C2:  " + QString("%1mA %2V").arg(m_detectionPara[C2_DETECTION].iValueI).arg(m_detectionPara[C2_DETECTION].iValueV/1000));
    painter.drawText(2, 490,
                     "N2:  " + QString("%1mA").arg(m_detectionPara[N2_DETECTION].iValueI));

    if (gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_HaveTOC))
    {
        painter.drawText(110, 490,
                         tr("TOC") + ": " + QString::number(m_fToc, 'f', 0) + QString("ppb"));
    }

    painter.setFont(oldFont);
    painter.setPen(oldPen);
}

void Ex_FlowChartWidget::paintGenie_A(QPainter &painter)
{
    painter.drawPixmap(0, 15, m_backgroundPix[GENIE_A]);
    if(m_isUpdate)
    {
        if(m_deviceState[N2_STATE]) painter.drawPixmap(487, 258, m_devicePix[UV_ON_H]); //N2
        if(0 == m_updPump)
        {
            if(m_deviceState[C1_STATE]) painter.drawPixmap(192, 77, m_devicePix[PUMP_ON1]); //C1 RO
            if(m_deviceState[C2_STATE]) painter.drawPixmap(538, 165,m_devicePix[PUMP_ON1]); //C2 CIR
            m_updPump = 1;
        }
        else if(1 == m_updPump)
        {
            if(m_deviceState[C1_STATE]) painter.drawPixmap(192, 77, m_devicePix[PUMP_ON2]); //C1 RO
            if(m_deviceState[C2_STATE]) painter.drawPixmap(538, 165,m_devicePix[PUMP_ON2]); //C2 CIR
            m_updPump = 2;
        }

        else
        {
            m_updPump = 0;
        }

        if(m_deviceState[E1_STATE]) painter.drawPixmap(569, 88, m_devicePix[VALVE_ON]);  //E1
        if(m_deviceState[E2_STATE]) painter.drawPixmap(126, 373, m_devicePix[VALVE_ON]); //E2
        if(m_deviceState[E3_STATE]) painter.drawPixmap(167, 271, m_devicePix[VALVE_E3_H2]); //E3
        if(!m_deviceState[E3_STATE] && !m_deviceState[E2_STATE] && m_deviceState[E1_STATE])
        {
            painter.drawPixmap(167, 271, m_devicePix[VALVE_E3_H1]); //E3
        }
        if(m_deviceState[E5_STATE]) painter.drawPixmap(402, 436, m_devicePix[VALVE_ON]); //E5

        if(DispGetUpQtwFlag()) painter.drawPixmap(651, 363, m_devicePix[VALVE_ON]); //E7 UP
        if(DispGetEdiQtwFlag()) painter.drawPixmap(651, 174, m_devicePix[VALVE_ON]); //E8 HP

        m_isUpdate = false;
    }
    else
    {
        if(0 == m_updPump)
        {
            if(m_deviceState[C1_STATE]) painter.drawPixmap(192, 77, m_devicePix[PUMP_ON1]); //C1 RO
            if(m_deviceState[C2_STATE]) painter.drawPixmap(538, 165, m_devicePix[PUMP_ON1]); //C2 CIR
            m_updPump = 1;
        }
        else if(1 == m_updPump)
        {
            if(m_deviceState[C1_STATE]) painter.drawPixmap(192, 77, m_devicePix[PUMP_ON2]); //C1 RO
            if(m_deviceState[C2_STATE]) painter.drawPixmap(538, 165, m_devicePix[PUMP_ON2]); //C2 CIR
            m_updPump = 2;
        }

        else
        {
            m_updPump = 0;
        }

        m_isUpdate = true;
    }
}
void Ex_FlowChartWidget::paintGenie_A_TOC(QPainter &painter)
{
    painter.drawPixmap(0, 15, m_backgroundPix[GENIE_A_TOC]);
    if(m_isUpdate)
    {
        if(m_deviceState[N2_STATE]) painter.drawPixmap(487, 258, m_devicePix[UV_ON_H]); //N2
        if(0 == m_updPump)
        {
            if(m_deviceState[C1_STATE]) painter.drawPixmap(192, 77, m_devicePix[PUMP_ON1]); //C1 RO
            if(m_deviceState[C2_STATE]) painter.drawPixmap(538, 165,m_devicePix[PUMP_ON1]); //C2 CIR
            m_updPump = 1;
        }
        else if(1 == m_updPump)
        {
            if(m_deviceState[C1_STATE]) painter.drawPixmap(192, 77, m_devicePix[PUMP_ON2]); //C1 RO
            if(m_deviceState[C2_STATE]) painter.drawPixmap(538, 165,m_devicePix[PUMP_ON2]); //C2 CIR
            m_updPump = 2;
        }

        else
        {
            m_updPump = 0;
        }


        if(m_deviceState[E1_STATE]) painter.drawPixmap(569, 88, m_devicePix[VALVE_ON]);  //E1
        if(m_deviceState[E2_STATE]) painter.drawPixmap(126, 373, m_devicePix[VALVE_ON]); //E2
        if(m_deviceState[E3_STATE]) painter.drawPixmap(167, 271, m_devicePix[VALVE_E3_H2]); //E3
        if(!m_deviceState[E3_STATE] && !m_deviceState[E2_STATE] && m_deviceState[E1_STATE])
        {
            painter.drawPixmap(167, 271, m_devicePix[VALVE_E3_H1]); //E3
        }
        if(m_deviceState[E5_STATE]) painter.drawPixmap(402, 436, m_devicePix[VALVE_ON]); //E5

        if(DispGetUpQtwFlag()) painter.drawPixmap(651, 363, m_devicePix[VALVE_ON]); //E7 UP
        if(DispGetEdiQtwFlag()) painter.drawPixmap(651, 174, m_devicePix[VALVE_ON]); //E8 HP

        if(m_deviceState[E9_STATE]) painter.drawPixmap(557, 253, m_devicePix[VALVE_E3_H3]); //E9
        if(!m_deviceState[E9_STATE] && m_deviceState[E5_STATE])
        {
            painter.drawPixmap(557, 253, m_devicePix[VALVE_E3_H4]); //E9
        }

        m_isUpdate = false;
    }
    else
    {
        if(0 == m_updPump)
        {
            if(m_deviceState[C1_STATE]) painter.drawPixmap(192, 77, m_devicePix[PUMP_ON1]); //C1 RO
            if(m_deviceState[C2_STATE]) painter.drawPixmap(538, 165, m_devicePix[PUMP_ON1]); //C2 CIR
            m_updPump = 1;
        }
        else if(1 == m_updPump)
        {
            if(m_deviceState[C1_STATE]) painter.drawPixmap(192, 77, m_devicePix[PUMP_ON2]); //C1 RO
            if(m_deviceState[C2_STATE]) painter.drawPixmap(538, 165, m_devicePix[PUMP_ON2]); //C2 CIR
            m_updPump = 2;
        }

        else
        {
            m_updPump = 0;
        }

        m_isUpdate = true;
    }
}

void Ex_FlowChartWidget::paintGenie_A_Tags(QPainter &painter, bool isHaveTOC)
{
    QFont oldFont = painter.font();
    painter.setFont(QFont("", 12, QFont::Bold));

    painter.drawText(508, 285, "N2");
    painter.drawText(200, 125, "C1");
    painter.drawText(543, 208, "C2");
    painter.drawText(575, 122, "E1");
    painter.drawText(130, 406, "E2");
    painter.drawText(190, 273, "E3");
    painter.drawText(407, 470, "E5");
    painter.drawText(656, 396, "E7");
    painter.drawText(656, 208, "E8");

     if(isHaveTOC)
     {
         painter.drawText(581, 285, "E9");
     }

    painter.setFont(oldFont);
}

void Ex_FlowChartWidget::painterGenie_A_Value(QPainter &painter, bool isHaveTOC)
{
    int iPrecision = 1;
    QString strUs  = tr("us");

    QPen oldPen = painter.pen();
    QFont oldFont = painter.font();
    painter.setFont(QFont("", 12, QFont::Bold));
    painter.setPen(QPen(Qt::blue));

    painter.drawText(259, 72,
                     QString::number(m_waterInfo[I1_VALUE].fQuality, 'f', 0) + QString(" ") + strUs);
    painter.drawText(259, 86,
                     QString::number(m_waterInfo[I1_VALUE].fTemp, 'f', 1)  + QString(" ")+ m_strUnit[TEMP_UNIT]);

    painter.drawText(100, 250,
                     QString::number(m_waterInfo[I2_VALUE].fQuality, 'f', 1) + QString(" ") + strUs);
    painter.drawText(100, 264,
                     QString::number(m_waterInfo[I2_VALUE].fTemp, 'f', 1)  + QString(" ")+ m_strUnit[TEMP_UNIT]);

    if(isHaveTOC)
    {
        iPrecision = m_waterInfo[I4_VALUE].fQuality > 1.0 ? 1: 2;
        painter.drawText(400, 235,
                         QString::number(m_waterInfo[I4_VALUE].fQuality, 'f', iPrecision) + QString(" ") + m_strUnit[RES_COND_UNIT]);
        painter.drawText(400, 249,
                         QString::number(m_waterInfo[I4_VALUE].fTemp, 'f', 1)  + QString(" ") + m_strUnit[TEMP_UNIT]);
    }

    iPrecision = m_waterInfo[I5_VALUE].fQuality > 1.0 ? 1: 2;
    painter.drawText(400, 342,
                      QString::number(m_waterInfo[I5_VALUE].fQuality, 'f', iPrecision) + QString(" ") + m_strUnit[RES_COND_UNIT]);
    painter.drawText(400, 356,
                      QString::number(m_waterInfo[I5_VALUE].fTemp, 'f', 1)  + QString(" ") + m_strUnit[TEMP_UNIT]);

    if (PRESSURE_UINT_MPA == gGlobalParam.MiscParam.iUint4Pressure)
    {
        painter.drawText(51, 372, QString::number(m_fWorkPressure, 'f', 2) + m_strUnit[PRESSURE_UNIT]);
    }
    else
    {
        painter.drawText(51, 372, QString::number(m_fWorkPressure, 'f', 1) + m_strUnit[PRESSURE_UNIT]);
    }

    painter.drawText(410, 208, QString::number(m_flowRate[S1_VALUE], 'f', 1) + " L/min");

    painter.drawText(2, 460,
                      "C1:  " + QString("%1mA %2V").arg(m_detectionPara[C1_DETECTION].iValueI).arg(m_detectionPara[C1_DETECTION].iValueV/1000));
    painter.drawText(2, 475,
                      "C2:  " + QString("%1mA %2V").arg(m_detectionPara[C2_DETECTION].iValueI).arg(m_detectionPara[C2_DETECTION].iValueV/1000));
    painter.drawText(2, 490,
                      "N2:  " + QString("%1mA").arg(m_detectionPara[N2_DETECTION].iValueI));

    if (gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_HaveTOC))
    {
        painter.drawText(110, 475,
                         tr("TOC") + ": " + QString::number(m_fToc, 'f', 0) + QString("ppb"));
    }

    painter.drawText(110, 490,
                     tr("RO Rejection") + ": " + QString::number(m_fResidue, 'f', 1) + QString("%"));

    painter.setFont(oldFont);
    painter.setPen(oldPen);
}

void Ex_FlowChartWidget::paintWorkStatus(QPainter &painter)
{
    painter.drawText(20,  10, 150, 40, Qt::AlignLeft, m_strWorkStatus[0] + ":");
    painter.drawText(200, 10, 150, 40, Qt::AlignLeft, m_strWorkStatus[1]);
    painter.drawText(400, 10, 150, 40, Qt::AlignLeft, m_strWorkStatus[2]);
    painter.drawText(600, 10, 150, 40, Qt::AlignLeft, m_strWorkStatus[3]);
}

void Ex_FlowChartWidget::initUI()
{
    setAutoFillBackground(true);
    QPalette palette = this->palette();
    palette.setColor(QPalette::Window, QColor(228, 231, 240));
    this->setPalette(palette);
}

void Ex_FlowChartWidget::initDefaultValue()
{
    m_waterInfo[I1_VALUE].fQuality = 2000;
    m_waterInfo[I1_VALUE].fTemp = 25.0;

    m_waterInfo[I2_VALUE].fQuality = 200;
    m_waterInfo[I2_VALUE].fTemp = 25.0;

    m_waterInfo[I3_VALUE].fQuality = 1.0;
    m_waterInfo[I3_VALUE].fTemp = 25.0;

    m_waterInfo[I4_VALUE].fQuality = 1.0;
    m_waterInfo[I4_VALUE].fTemp = 25.0;

    m_waterInfo[I5_VALUE].fQuality = 18.2;
    m_waterInfo[I5_VALUE].fTemp = 25.0;

    m_fResidue = 0.0;
    m_fToc = 3;

    for(int i = 0; i < DETECTION_NUM; i++)
    {
        m_detectionPara[i].iValueI = 0;
        m_detectionPara[i].iValueV = 0;
    }
    for(int i = 0; i < S_NUM; i++)
    {
        m_flowRate[i] = 0;
    }

    m_fWorkPressure = 0;
    m_iTankLevel = 0;
    m_fTankLevel = 0;
    updateUnits();
}

void Ex_FlowChartWidget::loadPixmap()
{
    QString strBImage[BACK_IMAGE_NUM] =
    {
        ":/flowchart_background/Genie_G.png",
        ":/flowchart_background/Genie_G_HPCIR.png",
        ":/flowchart_background/Genie_E.png",
        ":/flowchart_background/Genie_E_HPCIR.png",
        ":/flowchart_background/Genie_U.png",
        ":/flowchart_background/Genie_U_TOC.png",
        ":/flowchart_background/Genie_U_HPCIR.png",
        ":/flowchart_background/Genie_U_TOC_HPCIR.png",
        ":/flowchart_background/Genie_R.png",
        ":/flowchart_background/Genie_R_HPCIR.png",
        ":/flowchart_background/Genie_PURIST.png",
        ":/flowchart_background/Genie_PURIST_TOC.png",
        ":/flowchart_background/Genie_A.png",
        ":/flowchart_background/Genie_A_TOC.png"
    };
    QString strDImage[DEVICE_IMAGE_NUM] =
    {
        ":/flowchart_device/E_ON.png",
        ":/flowchart_device/E_3_H1.png",
        ":/flowchart_device/E_3_H2.png",
        ":/flowchart_device/E_3_H3.png",
        ":/flowchart_device/E_3_H4.png",
        ":/flowchart_device/E_3_H5.png",
        ":/flowchart_device/E_3_V1.png",
        ":/flowchart_device/E_3_V2.png",
        ":/flowchart_device/Pump1.png",
        ":/flowchart_device/Pump2.png",
        ":/flowchart_device/UVON.png",
        ":/flowchart_device/UVOFF.png",
        ":/flowchart_device/UVON2.png",
        ":/flowchart_device/EDI.png"
    };

    for(int i = 0; i < BACK_IMAGE_NUM; i++)
    {
        m_backgroundPix[i] = QPixmap(strBImage[i]);
    }
    for(int i = 0; i < DEVICE_IMAGE_NUM; i++)
    {
        m_devicePix[i] = QPixmap(strDImage[i]);
    }
}

void Ex_FlowChartWidget::updateUnits()
{
    if (CONDUCTIVITY_UINT_OMG == gGlobalParam.MiscParam.iUint4Conductivity)
    {
        m_strUnit[RES_COND_UNIT] = tr("omg");
    }
    else
    {
        m_strUnit[RES_COND_UNIT] = tr("us");
    }

    if (TEMERATURE_UINT_CELSIUS == gGlobalParam.MiscParam.iUint4Temperature)
    {
        m_strUnit[TEMP_UNIT] = tr("Celsius");
    }
    else
    {
        m_strUnit[TEMP_UNIT] = tr("F");
    }

    if (PRESSURE_UINT_BAR == gGlobalParam.MiscParam.iUint4Pressure)
    {
        m_strUnit[PRESSURE_UNIT] = QString(" bar");
    }
    else if (PRESSURE_UINT_MPA == gGlobalParam.MiscParam.iUint4Pressure)
    {
        m_strUnit[PRESSURE_UNIT] = QString(" MPa");
    }
    else
    {
        m_strUnit[PRESSURE_UNIT] = QString(" psi");
    }
}
