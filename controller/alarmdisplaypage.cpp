#include "alarmdisplaypage.h"

#include "titlebar.h"

#include "mainwindow.h"

#include <QPainter>

#include <QScrollBar>

#define MAKEID(major,minor) (((major)<<16) + (minor))

AlarmDisplayPage::AlarmDisplayPage(QObject *parent,CBaseWidget *widget ,MainWindow *wndMain) : CSubPage(parent,widget,wndMain)
{
     int iCsIdx = 0;
     int iAsIds = 0;
     int iLoop;
     int iMask;

    iMask = m_wndMain->getMachineNotifyMask(gGlobalParam.iMachineType,0);
    
    for (iLoop = 0; iLoop <= DISP_ROC12; iLoop++)
    {
       if (iMask & (1 << iLoop))
       {
            aCsIds[iCsIdx++].iId = iLoop;
       }
    }

     m_aiRealNum[ALARM_TYPE_CS] = iCsIdx;

    iMask = m_wndMain->getMachineAlarmMask(gGlobalParam.iMachineType,DISP_ALARM_PART0);
    
    for (iLoop = 0; iLoop < DISP_ALARM_PART0_NUM; iLoop++)
    {
       if (iMask & (1 << iLoop))
       {
            aAsIds[iAsIds++].iId = MAKEID(DISP_ALARM_PART0,iLoop);
       }
    }
    
    iMask = m_wndMain->getMachineAlarmMask(gGlobalParam.iMachineType,DISP_ALARM_PART1);
    
    for (iLoop = 0; iLoop < DISP_ALARM_PART1_NUM; iLoop++)
    {
       if (iMask & (1 << iLoop))
       {
            aAsIds[iAsIds++].iId = MAKEID(DISP_ALARM_PART1,iLoop);
       }
    }

    m_aiRealNum[ALARM_TYPE_AS] = iAsIds;

    creatTitle();

    initUi();
    
    buildTranslation();

}

void AlarmDisplayPage::creatTitle()
{
    CSubPage::creatTitle();

    buildTitles();

    selectTitle(0);
}

void AlarmDisplayPage::buildTitles()
{
    QStringList stringList;

    stringList << tr("Alarms & Alerts");

    setTitles(stringList);
}

void AlarmDisplayPage::buildTranslation()
{
     int iIdx = 0;

     m_lbNotName->setText(tr("Alerts"));
     m_lbAlarmName->setText(tr("Alarms"));

     
     for (iIdx = 0; iIdx < DISP_ALARM_TOTAL_NUM; iIdx++)
     {
         aAsIndex[DISP_ALARM_PART0][iIdx] = 0XFF;
         aAsIndex[DISP_ALARM_PART1][iIdx] = 0XFF;
     }
     
     for (iIdx = 0; iIdx < m_aiRealNum[ALARM_TYPE_AS]; iIdx++)
     {
         switch(aAsIds[iIdx].iId)
         {
             /* Part One */
         case MAKEID(DISP_ALARM_PART0,DISP_ALARM_PART0_254UV_OOP):
             /*
             检查254UV
             */
             m_astrPartAlarm[iIdx] = tr("Check 254 UV");

             aAsIndex[DISP_ALARM_PART0][aAsIds[iIdx].iId - MAKEID(DISP_ALARM_PART0,DISP_ALARM_PART0_254UV_OOP)] = iIdx; 
             break;
         case MAKEID(DISP_ALARM_PART0,DISP_ALARM_PART0_185UV_OOP):
             /*
             检查185UV
             */
             m_astrPartAlarm[iIdx] = tr("Check 185 UV");
             aAsIndex[DISP_ALARM_PART0][aAsIds[iIdx].iId - MAKEID(DISP_ALARM_PART0,DISP_ALARM_PART0_254UV_OOP)] = iIdx; 
             break;
         case MAKEID(DISP_ALARM_PART0,DISP_ALARM_PART0_TANKUV_OOP):
             /*
             检查水箱UV
             */
             m_astrPartAlarm[iIdx] = tr("Check Tank UV");
             aAsIndex[DISP_ALARM_PART0][aAsIds[iIdx].iId - MAKEID(DISP_ALARM_PART0,DISP_ALARM_PART0_254UV_OOP)] = iIdx; 
             break;
         case MAKEID(DISP_ALARM_PART0,DISP_ALARM_PART0_TUBEUV_OOP):
             /*
             检查管路UV
             */
             m_astrPartAlarm[iIdx] = tr("Check TUBE UV");
             aAsIndex[DISP_ALARM_PART0][aAsIds[iIdx].iId - MAKEID(DISP_ALARM_PART0,DISP_ALARM_PART0_254UV_OOP)] = iIdx; 
             break;
         case MAKEID(DISP_ALARM_PART0,DISP_ALARM_PART0_PPACK_OOP):
             /*
             P-Pack脱落
             */
             m_astrPartAlarm[iIdx] = tr("P Pack Not Detected");
             aAsIndex[DISP_ALARM_PART0][aAsIds[iIdx].iId - MAKEID(DISP_ALARM_PART0,DISP_ALARM_PART0_254UV_OOP)] = iIdx; 
             break;
         case MAKEID(DISP_ALARM_PART0,DISP_ALARM_PART0_ATPACK_OOP):
             /*
             AT-Pack脱落
             */
             m_astrPartAlarm[iIdx] = tr("AT Pack Not Detected");
             aAsIndex[DISP_ALARM_PART0][aAsIds[iIdx].iId - MAKEID(DISP_ALARM_PART0,DISP_ALARM_PART0_254UV_OOP)] = iIdx; 
             break;
         case MAKEID(DISP_ALARM_PART0,DISP_ALARM_PART0_HPACK_OOP):
             /*
             H-Pack脱落
             */
             m_astrPartAlarm[iIdx] = tr("H Pack Not Detected");
             aAsIndex[DISP_ALARM_PART0][aAsIds[iIdx].iId - MAKEID(DISP_ALARM_PART0,DISP_ALARM_PART0_254UV_OOP)] = iIdx; 
             break;
         case MAKEID(DISP_ALARM_PART0,DISP_ALARM_PART0_UPACK_OOP):
             /*
             U-Pack脱落
             */
             m_astrPartAlarm[iIdx] = tr("U Pack Not Detected");
             aAsIndex[DISP_ALARM_PART0][aAsIds[iIdx].iId - MAKEID(DISP_ALARM_PART0,DISP_ALARM_PART0_254UV_OOP)] = iIdx; 
             break;

             /* Part Two */
         case MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_SOURCE_WATER_PRESSURE):
             /*
             自来水压力低
             */
             m_astrPartAlarm[iIdx] = tr("Lower Tap Pres.");
             aAsIndex[DISP_ALARM_PART1][aAsIds[iIdx].iId - MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_SOURCE_WATER_PRESSURE)] = iIdx; 
             break;
         case MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_HIGER_SOURCE_WATER_CONDUCTIVITY):
             /*
             自来水电导率>设定值
             */
             m_astrPartAlarm[iIdx] = tr("Tap Cond. > ") + QString::number(gGlobalParam.MMParam.SP[MACHINE_PARAM_SP13]) + tr("us/cm");
             aAsIndex[DISP_ALARM_PART1][aAsIds[iIdx].iId - MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_SOURCE_WATER_PRESSURE)] = iIdx; 
             break;
         case MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_HIGER_RO_PRODUCT_CONDUCTIVITY):
             /*
             RO产水>设定值
             */
                 
             if (MACHINE_PURIST == gGlobalParam.iMachineType)
             {
                 m_astrPartAlarm[iIdx] = tr("UP Feed Cond. > ") + QString::number(gGlobalParam.MMParam.SP[MACHINE_PARAM_SP3]) + tr("us/cm");
             }
             else
             {
                 m_astrPartAlarm[iIdx] = tr("RO Product Cond. > ") + QString::number(gGlobalParam.MMParam.SP[MACHINE_PARAM_SP3]) + tr("us/cm");
             }
                 
             aAsIndex[DISP_ALARM_PART1][aAsIds[iIdx].iId - MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_SOURCE_WATER_PRESSURE)] = iIdx; 
             break;
         case MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_RO_RESIDUE_RATIO):
             
             /*
             RO截留率<设定值
             */
             m_astrPartAlarm[iIdx] = tr("RO Rejection < ") + QString::number(gGlobalParam.MMParam.SP[MACHINE_PARAM_SP2]) + tr("us/cm");
             aAsIndex[DISP_ALARM_PART1][aAsIds[iIdx].iId - MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_SOURCE_WATER_PRESSURE)] = iIdx; 
             break;
         case MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_EDI_PRODUCT_RESISTENCE):
             /*
             EDI产水<设定值
             */
             m_astrPartAlarm[iIdx] = tr("EDI Product Resis. < ") + QString::number(gGlobalParam.MMParam.SP[MACHINE_PARAM_SP4]) + tr("omg");
             aAsIndex[DISP_ALARM_PART1][aAsIds[iIdx].iId - MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_SOURCE_WATER_PRESSURE)] = iIdx; 
             break;
         case MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_UP_PRODUCT_RESISTENCE):
             /*
             UP取水<设定值
             */
             m_astrPartAlarm[iIdx] = tr("UP Product Resis. < ") + QString::number(gGlobalParam.MMParam.SP[MACHINE_PARAM_SP7]) + tr("omg");
             aAsIndex[DISP_ALARM_PART1][aAsIds[iIdx].iId - MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_SOURCE_WATER_PRESSURE)] = iIdx; 
             break;
         case MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_TUBE_RESISTENCE):
             /*
             管路水质<设定值
             */
             m_astrPartAlarm[iIdx] = tr("Loop Resis. < ") + QString::number(gGlobalParam.MMParam.SP[MACHINE_PARAM_SP17]) + tr("omg");
             aAsIndex[DISP_ALARM_PART1][aAsIds[iIdx].iId - MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_SOURCE_WATER_PRESSURE)] = iIdx; 
             break;
         case MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_PWTANKE_WATER_LEVEL):
             /*
             纯水箱液位<设定值
             */
             m_astrPartAlarm[iIdx] = tr("Pure Tank Level < ") + QString::number(gGlobalParam.MMParam.SP[MACHINE_PARAM_SP6]) + tr("%");
             aAsIndex[DISP_ALARM_PART1][aAsIds[iIdx].iId - MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_SOURCE_WATER_PRESSURE)] = iIdx; 
             break;
         case MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_SWTANKE_WATER_LEVEL):
             /*
             源水箱液位<设定值 
             */
             m_astrPartAlarm[iIdx] = tr("Feed Tank Level < ") + QString::number(gGlobalParam.MMParam.SP[MACHINE_PARAM_SP9]) + tr("%");
             aAsIndex[DISP_ALARM_PART1][aAsIds[iIdx].iId - MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_SOURCE_WATER_PRESSURE)] = iIdx; 
             break;
         case MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_RO_PRODUCT_FLOWING_VELOCITY):
             /*
             RO产水流速<设定值
             */
             m_astrPartAlarm[iIdx] = tr("RO Prod. Rate < ") + QString::number(gGlobalParam.MMParam.SP[MACHINE_PARAM_SP15]) + tr("L/h");
             aAsIndex[DISP_ALARM_PART1][aAsIds[iIdx].iId - MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_SOURCE_WATER_PRESSURE)] = iIdx; 
             break;
         case MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_RO_WASTE_FLOWING_VELOCITY):
             /*
             RO弃水流速<设定值
             */
             m_astrPartAlarm[iIdx] = tr("RO Rej. Rate < ") + QString::number(gGlobalParam.MMParam.SP[MACHINE_PARAM_SP16]) + tr("L/h");
             aAsIndex[DISP_ALARM_PART1][aAsIds[iIdx].iId - MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_SOURCE_WATER_PRESSURE)] = iIdx; 
             break;
         case MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_PWTANK_RESISTENCE):
             /*
             水箱水质 <设定值
             */
             m_astrPartAlarm[iIdx] = tr("Tank Resis. < ") + QString::number(gGlobalParam.MMParam.SP[MACHINE_PARAM_SP11]) + tr("omg");
             aAsIndex[DISP_ALARM_PART1][aAsIds[iIdx].iId - MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_SOURCE_WATER_PRESSURE)] = iIdx; 
             break;
             /*
         case MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_PW_PRODUCT_RESISTENCE):
             //纯水取水<设定值
             m_astrPartAlarm[iIdx] = tr("Feed Product Resis.") + QString::number(gGlobalParam.MMParam.SP[MACHINE_PARAM_SP12]) + tr("omg");
             aAsIndex[DISP_ALARM_PART1][aAsIds[iIdx].iId - MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_SOURCE_WATER_PRESSURE)] = iIdx; 
             break;
             */
         case MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_CIR_WATER_CONDUCTIVITY):
             /*
             循环水质<设定值 
             */
             m_astrPartAlarm[iIdx] = tr("Recir. Cond. < ") + QString::number(gGlobalParam.MMParam.SP[MACHINE_PARAM_SP31]) + tr("omg");
             aAsIndex[DISP_ALARM_PART1][aAsIds[iIdx].iId - MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_SOURCE_WATER_PRESSURE)] = iIdx; 
             break;
         case MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_HP_PRODUCT_WATER_CONDUCTIVITY):
             /*
             HP产水<设定值 
             */
             m_astrPartAlarm[iIdx] = tr("HP Product Cond. < ") + QString::number(gGlobalParam.MMParam.SP[MACHINE_PARAM_SP32]) + tr("omg");
             aAsIndex[DISP_ALARM_PART1][aAsIds[iIdx].iId - MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_SOURCE_WATER_PRESSURE)] = iIdx; 
             break;
         case MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_HIGHER_SOURCE_WATER_TEMPERATURE):
             /*
             进水温度>45℃
             */
             m_astrPartAlarm[iIdx] = tr("Feed Temp. > ") + QString::number(gGlobalParam.MMParam.SP[MACHINE_PARAM_SP18]) + tr("Celsius");
             aAsIndex[DISP_ALARM_PART1][aAsIds[iIdx].iId - MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_SOURCE_WATER_PRESSURE)] = iIdx; 
             break;
         case MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_SOURCE_WATER_TEMPERATURE):
            /*
            进水温度<5℃
            */
            m_astrPartAlarm[iIdx] = tr("Feed Temp. < ") + QString::number(gGlobalParam.MMParam.SP[MACHINE_PARAM_SP19]) + tr("Celsius");
            aAsIndex[DISP_ALARM_PART1][aAsIds[iIdx].iId - MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_SOURCE_WATER_PRESSURE)] = iIdx; 
         case MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_HIGHER_RO_PRODUCT_TEMPERATURE):
              /*
              RO产水温度>45℃
              */
              if (MACHINE_PURIST == gGlobalParam.iMachineType)
              {
                  m_astrPartAlarm[iIdx] = tr("UP Feed Temp. > ") + QString::number(gGlobalParam.MMParam.SP[MACHINE_PARAM_SP20]) + tr("Celsius");
              }
              else
              {
                  m_astrPartAlarm[iIdx] = tr("RO Temp. > ") + QString::number(gGlobalParam.MMParam.SP[MACHINE_PARAM_SP20]) + tr("Celsius");
              }
              
              aAsIndex[DISP_ALARM_PART1][aAsIds[iIdx].iId - MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_SOURCE_WATER_PRESSURE)] = iIdx; 
             break;
         case MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_RO_PRODUCT_TEMPERATURE):
             /*
             RO产水温度<5℃
             */
             if (MACHINE_PURIST == gGlobalParam.iMachineType)
             {
                 m_astrPartAlarm[iIdx] = tr("UP Feed Temp. < ") + QString::number(gGlobalParam.MMParam.SP[MACHINE_PARAM_SP21]) + tr("Celsius");
             }
             else
             {
                 m_astrPartAlarm[iIdx] = tr("RO Temp. < ") + QString::number(gGlobalParam.MMParam.SP[MACHINE_PARAM_SP21]) + tr("Celsius");
             }
             
             aAsIndex[DISP_ALARM_PART1][aAsIds[iIdx].iId - MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_SOURCE_WATER_PRESSURE)] = iIdx; 
             break;
         case MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_HIGHER_EDI_PRODUCT_TEMPERATURE):
             /*
             EDI产水温度>45℃
             */
             m_astrPartAlarm[iIdx] = tr("EDI Temp. > ") + QString::number(gGlobalParam.MMParam.SP[MACHINE_PARAM_SP22]) + tr("Celsius");
             aAsIndex[DISP_ALARM_PART1][aAsIds[iIdx].iId - MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_SOURCE_WATER_PRESSURE)] = iIdx; 
             break;
         case MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_EDI_PRODUCT_TEMPERATURE):
             /*
             EDI产水温度<5℃
             */
             m_astrPartAlarm[iIdx] = tr("EDI Temp. < ") + QString::number(gGlobalParam.MMParam.SP[MACHINE_PARAM_SP23]) + tr("Celsius");
             aAsIndex[DISP_ALARM_PART1][aAsIds[iIdx].iId - MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_SOURCE_WATER_PRESSURE)] = iIdx; 
             break;
         case MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_HIGHER_UP_PRODUCT_TEMPERATURE):
             /*
             UP产水温度>45℃
             */
             m_astrPartAlarm[iIdx] = tr("UP Temp. > ") + QString::number(gGlobalParam.MMParam.SP[MACHINE_PARAM_SP24]) + tr("Celsius");
             aAsIndex[DISP_ALARM_PART1][aAsIds[iIdx].iId - MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_SOURCE_WATER_PRESSURE)] = iIdx; 
             break;
         case MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_UP_PRODUCT_TEMPERATURE):
             /*
             UP产水温度<5℃
             */
             m_astrPartAlarm[iIdx] = tr("UP Temp. < ") + QString::number(gGlobalParam.MMParam.SP[MACHINE_PARAM_SP25]) + tr("Celsius");
             aAsIndex[DISP_ALARM_PART1][aAsIds[iIdx].iId - MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_SOURCE_WATER_PRESSURE)] = iIdx; 
             break;
         case MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_HIGHER_TUBE_TEMPERATURE):
             /*
             管路温度>45℃
             */
             m_astrPartAlarm[iIdx] = tr("Loop Temp. > ") + QString::number(gGlobalParam.MMParam.SP[MACHINE_PARAM_SP26]) + tr("Celsius");
             aAsIndex[DISP_ALARM_PART1][aAsIds[iIdx].iId - MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_SOURCE_WATER_PRESSURE)] = iIdx; 
             break;
         case MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_TUBE_TEMPERATURE):
             /*
             管路温度<5℃
             */
             m_astrPartAlarm[iIdx] = tr("Loop Temp. < ") + QString::number(gGlobalParam.MMParam.SP[MACHINE_PARAM_SP27]) + tr("Celsius");
             aAsIndex[DISP_ALARM_PART1][aAsIds[iIdx].iId - MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_SOURCE_WATER_PRESSURE)] = iIdx; 
             break;
         case MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_HIGHER_TOC_SENSOR_TEMPERATURE):
             /*
             TOC温度>45℃
             */
             m_astrPartAlarm[iIdx] = tr("TOC Temp. > ") + QString::number(gGlobalParam.MMParam.SP[MACHINE_PARAM_SP28]) + tr("Celsius");
             aAsIndex[DISP_ALARM_PART1][aAsIds[iIdx].iId - MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_SOURCE_WATER_PRESSURE)] = iIdx; 
             break;
         case MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_TOC_SENSOR_TEMPERATURE):
             /*
             TOC温度<5℃
             */
             m_astrPartAlarm[iIdx] = tr("TOC Temp. < ") + QString::number(gGlobalParam.MMParam.SP[MACHINE_PARAM_SP29]) + tr("Celsius");
             aAsIndex[DISP_ALARM_PART1][aAsIds[iIdx].iId - MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_SOURCE_WATER_PRESSURE)] = iIdx; 
             break;
         case MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_TOC_SOURCE_WATER_RESISTENCE):            
             /*
             TOC进水水质<15MΩ。cm
             */
             m_astrPartAlarm[iIdx] = tr("TOC Feed Resis. < ") + QString::number(gGlobalParam.MMParam.SP[MACHINE_PARAM_SP30]) + tr("omg");
             aAsIndex[DISP_ALARM_PART1][aAsIds[iIdx].iId - MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_SOURCE_WATER_PRESSURE)] = iIdx; 
             break;
         }
      }
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


void AlarmDisplayPage:: update()
{
    csUpdate();
    asUpdate();

}

void AlarmDisplayPage::switchLanguage()
{
    buildTranslation();

    buildTitles();

    selectTitle(titleIndex());
}

void AlarmDisplayPage::createList()
{
    int index;

     QColor colors[] = {QColor(200,200,188),QColor(228, 231, 240)};
    
     m_listWidget[0] = new QListWidget(m_widget);
     m_listWidget[1] = new QListWidget(m_widget);

     m_lbNotName = new QLabel(m_widget);
     m_lbNotName->setGeometry(QRect(10 , 55 , 380 , 45));
     m_lbNotName->setAlignment(Qt::AlignCenter);

     m_listWidget[0]->setStyleSheet("background-color:transparent");
     m_listWidget[0]->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
     m_listWidget[0]->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
     m_listWidget[0]->setFrameShape(QListWidget::NoFrame);
     m_listWidget[0]->setGeometry(QRect(10,100 ,375,490));

     m_lbAlarmName = new QLabel(m_widget);
     m_lbAlarmName->setGeometry(QRect(410 , 55 , 380 , 45));
     m_lbAlarmName->setAlignment(Qt::AlignCenter);
     
     m_listWidget[1]->setStyleSheet("background-color:transparent");
     m_listWidget[1]->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
     m_listWidget[1]->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
     m_listWidget[1]->setFrameShape(QListWidget::NoFrame);
     m_listWidget[1]->setGeometry(QRect(410,100 ,380,490));

     for(index = 0 ; index < m_aiRealNum[ALARM_TYPE_CS] ; index++ )
     {
         m_pCslistWidgetItem[index] = new QListWidgetItem;
         m_pCslistWidgetItem[index]->setSizeHint(QSize(390 , ADCS_LIST_WIDGET_HEIGHT));

         m_pCslistItem[index] = new AdcsListItem(0 ,this);

         m_pCslistWidgetItem[index]->setBackground(colors[index % 2]);

         m_listWidget[0]->insertItem(index,m_pCslistWidgetItem[index]);
         m_listWidget[0]->setItemWidget(m_pCslistWidgetItem[index] , m_pCslistItem[index]);
     }
     
     for(index = 0 ; index < m_aiRealNum[ALARM_TYPE_AS] ; index++ )
     {
         m_pAslistWidgetIem[index] = new QListWidgetItem;
         m_pAslistWidgetIem[index]->setSizeHint(QSize(390 , ADAS_LIST_WIDGET_HEIGHT));

         m_pAslistItem[index] = new AdasListItem(0 ,this);

         m_pAslistWidgetIem[index]->setBackground(colors[index % 2]);

         m_listWidget[1]->insertItem(index,m_pAslistWidgetIem[index]);
         m_listWidget[1]->setItemWidget(m_pAslistWidgetIem[index] , m_pAslistItem[index]);
     }
     
}

void AlarmDisplayPage::csUpdate()
{
    int     tmp;
    int     iIdx = 0;
    int     iLoop;
    QString strTmp;

    QString astrNames[] = {
        tr(" "),
        tr(" "),
    };

    /*Preporcess column */
    QString strWarnMsg = tr("Please replace it in time so as not to affect the performance of the system.");
    QString strROMsg = tr("Perform RO Membrane Cl2 Cleaning, Install a cleaning pack and press Clean.");
   
    for (iLoop = 0; iLoop < m_aiRealNum[ALARM_TYPE_CS]; iLoop++)
    {
        switch(aCsIds[iLoop].iId)
        {
        case DISP_PRE_PACK: 
            if (gCMUsage.ulUsageState & (1 << DISP_PRE_PACKLIFEDAY) 
                || gCMUsage.ulUsageState & (1 << DISP_PRE_PACKLIFEL))
            {
                /* for preprocess column */
                tmp = gCMUsage.info.aulCms[DISP_PRE_PACKLIFEL] ;
                if(tmp < 0)
                {
                    tmp = 0;
                }
                strTmp = astrNames[0] + "  " + QString::number(tmp) + "L";
                m_pCslistItem[iIdx]->setValue(strTmp);
            
                strTmp = tr("Installation Date ") + decodeTime(gCMUsage.info.aulCms[DISP_PRE_PACKLIFEDAY]);
                m_pCslistItem[iIdx]->setInstDate(strTmp);

                /*
                tmp = gGlobalParam.CMParam.aulCms[DISP_PRE_PACKLIFEDAY] - 
                    (DispGetCurSecond() - gCMUsage.info.aulCms[DISP_PRE_PACKLIFEDAY])/ DISP_DAYININSECOND;
                   */
                tmp = ((DispGetCurSecond() - gCMUsage.info.aulCms[DISP_PRE_PACKLIFEDAY])/DISP_DAYININSECOND) -
                      gGlobalParam.CMParam.aulCms[DISP_PRE_PACKLIFEDAY];


                if(tmp < 0)
                {
                    tmp = 0;
                }
//                strTmp = tr("Replace in ") + decodeDays(tmp) + tr(" ") + tr("days");
                strTmp = tr("It is ") + decodeDays(tmp) + tr(" ") + tr("days overdue. ") + strWarnMsg;
                m_pCslistItem[iIdx]->setName(tr("Prefilter"));
                m_pCslistItem[iIdx]->setChangeDate(strTmp);

                strTmp = tr("Cat No.:") + gGlobalParam.cmSn.aCn[DISP_PRE_PACK];
                m_pCslistItem[iIdx]->setCatNo(strTmp);
                
                strTmp = tr("Lot No.:") + gGlobalParam.cmSn.aLn[DISP_PRE_PACK];
                m_pCslistItem[iIdx]->setLotNo(strTmp);
                
                m_pCslistItem[iIdx]->updateState(1);
                m_pCslistItem[iIdx]->setId(iIdx);
                iIdx++;
            }
            break;
        case DISP_T_PACK:
            if (gCMUsage.ulUsageState & (1 << DISP_T_PACKLIFEDAY)
                || gCMUsage.ulUsageState & (1 << DISP_T_PACKLIFEL))
            {
                /* for preprocess column */
                tmp = gCMUsage.info.aulCms[DISP_T_PACKLIFEL] ;
                if(tmp < 0)
                {
                    tmp = 0;
                }
                strTmp = astrNames[0] + "  " + QString::number(tmp) + "L";
                m_pCslistItem[iIdx]->setValue(strTmp);

                strTmp = tr("Installation Date ") + decodeTime(gCMUsage.info.aulCms[DISP_T_PACKLIFEDAY]);
                m_pCslistItem[iIdx]->setInstDate(strTmp);

                /*
                tmp = gGlobalParam.CMParam.aulCms[DISP_T_PACKLIFEDAY] -
                    (DispGetCurSecond() - gCMUsage.info.aulCms[DISP_T_PACKLIFEDAY])/ DISP_DAYININSECOND;
                    */

                tmp = ((DispGetCurSecond() - gCMUsage.info.aulCms[DISP_T_PACKLIFEDAY])/ DISP_DAYININSECOND) -
                        gGlobalParam.CMParam.aulCms[DISP_T_PACKLIFEDAY];

                if(tmp < 0)
                {
                    tmp = 0;
                }
//                strTmp = tr("Replace in ") + decodeDays(tmp) + tr(" ") + tr("days");
                strTmp = tr("It is ") + decodeDays(tmp) + tr(" ") + tr("days overdue. ") + strWarnMsg;
                m_pCslistItem[iIdx]->setName(tr("T Pack"));
                m_pCslistItem[iIdx]->setChangeDate(strTmp);

                strTmp = tr("Cat No.:") + gGlobalParam.cmSn.aCn[DISP_T_PACK];
                m_pCslistItem[iIdx]->setCatNo(strTmp);

                strTmp = tr("Lot No.:") + gGlobalParam.cmSn.aLn[DISP_T_PACK];
                m_pCslistItem[iIdx]->setLotNo(strTmp);

                m_pCslistItem[iIdx]->updateState(1);
                m_pCslistItem[iIdx]->setId(iIdx);
                iIdx++;
            }
            break;

        case DISP_P_PACK:
            if (gCMUsage.ulUsageState & (1 << DISP_P_PACKLIFEDAY) 
                || gCMUsage.ulUsageState & (1 << DISP_P_PACKLIFEL))
            {
                /* for DISP_P_PACK column */
                tmp = gCMUsage.info.aulCms[DISP_P_PACKLIFEL] ;
                if(tmp < 0)
                {
                    tmp = 0;
                }
                strTmp = astrNames[0] + "  " + QString::number(tmp) + "L";
                m_pCslistItem[iIdx]->setValue(strTmp);
                
                strTmp = tr("Installation Date ") + decodeTime(gCMUsage.info.aulCms[DISP_P_PACKLIFEDAY]);
                m_pCslistItem[iIdx]->setInstDate(strTmp);
                /*
                tmp = gGlobalParam.CMParam.aulCms[DISP_P_PACKLIFEDAY] - 
                    (DispGetCurSecond() - gCMUsage.info.aulCms[DISP_P_PACKLIFEDAY])/ DISP_DAYININSECOND;
                    */
                tmp = ((DispGetCurSecond() - gCMUsage.info.aulCms[DISP_P_PACKLIFEDAY])/ DISP_DAYININSECOND) -
                        gGlobalParam.CMParam.aulCms[DISP_P_PACKLIFEDAY];

                if(tmp < 0)
                {
                    tmp = 0;
                }
//                strTmp = tr("Replace in ") + decodeDays(tmp) + tr(" ") + tr("days");
                strTmp = tr("It is ") + decodeDays(tmp) + tr(" ") + tr("days overdue. ") + strWarnMsg;
                m_pCslistItem[iIdx]->setChangeDate(strTmp);
                
                strTmp = tr("Cat No.:") + gGlobalParam.cmSn.aCn[DISP_P_PACK];
                m_pCslistItem[iIdx]->setCatNo(strTmp);
                
                strTmp = tr("Lot No.:") + gGlobalParam.cmSn.aLn[DISP_P_PACK];
                m_pCslistItem[iIdx]->setLotNo(strTmp);
                
                m_pCslistItem[iIdx]->updateState(1);
                m_pCslistItem[iIdx]->setId(iIdx);
                m_pCslistItem[iIdx]->setName(tr("P PACK"));
                iIdx++;
            }
            break; 
        case DISP_U_PACK:
            if (gCMUsage.ulUsageState & (1 << DISP_U_PACKLIFEDAY) 
                || gCMUsage.ulUsageState & (1 << DISP_U_PACKLIFEL))
            {
                /* for DISP_U_PACK column */
                tmp = gCMUsage.info.aulCms[DISP_U_PACKLIFEL] ;
                if(tmp < 0)
                {
                    tmp = 0;
                }
                strTmp = astrNames[0] + "  " + QString::number(tmp) + "L";
                m_pCslistItem[iIdx]->setValue(strTmp);
                
                strTmp = tr("Installation Date ") + decodeTime(gCMUsage.info.aulCms[DISP_U_PACKLIFEDAY]);
                m_pCslistItem[iIdx]->setInstDate(strTmp);
                
//                tmp = gGlobalParam.CMParam.aulCms[DISP_U_PACKLIFEDAY] -
//                    (DispGetCurSecond() - gCMUsage.info.aulCms[DISP_U_PACKLIFEDAY])/ DISP_DAYININSECOND;

                tmp = ((DispGetCurSecond() - gCMUsage.info.aulCms[DISP_U_PACKLIFEDAY])/ DISP_DAYININSECOND) -
                        gGlobalParam.CMParam.aulCms[DISP_U_PACKLIFEDAY];
                if(tmp < 0)
                {
                    tmp = 0;
                }
//                strTmp = tr("Replace in ") + decodeDays(tmp) + tr(" ") + tr("days");
                strTmp = tr("It is ") + decodeDays(tmp) + tr(" ") + tr("days overdue. ") + strWarnMsg;
                m_pCslistItem[iIdx]->setChangeDate(strTmp);
                
                strTmp = tr("Cat No.:") + gGlobalParam.cmSn.aCn[DISP_U_PACK];
                m_pCslistItem[iIdx]->setCatNo(strTmp);
                
                strTmp = tr("Lot No.:") + gGlobalParam.cmSn.aLn[DISP_U_PACK];
                m_pCslistItem[iIdx]->setLotNo(strTmp);
                m_pCslistItem[iIdx]->updateState(1);
                m_pCslistItem[iIdx]->setId(iIdx);
                m_pCslistItem[iIdx]->setName(tr("U PACK"));
                iIdx++;

            }
            break;     
        case DISP_AT_PACK:
            if (gCMUsage.ulUsageState & (1 << DISP_AT_PACKLIFEDAY) 
                || gCMUsage.ulUsageState & (1 << DISP_AT_PACKLIFEL))
            {
                /* for DISP_AT_PACK column */
                tmp = gCMUsage.info.aulCms[DISP_AT_PACKLIFEL] ;
                if(tmp < 0)
                {
                    tmp = 0;
                }
                strTmp = astrNames[0] + "  " + QString::number(tmp) + "L";
                m_pCslistItem[iIdx]->setValue(strTmp);
                
                strTmp = tr("Installation Date ") + decodeTime(gCMUsage.info.aulCms[DISP_AT_PACKLIFEDAY]);
                m_pCslistItem[iIdx]->setInstDate(strTmp);
                
//                tmp = gGlobalParam.CMParam.aulCms[DISP_AT_PACKLIFEDAY] -
//                    (DispGetCurSecond() - gCMUsage.info.aulCms[DISP_AT_PACKLIFEDAY])/ DISP_DAYININSECOND;
                tmp = ((DispGetCurSecond() - gCMUsage.info.aulCms[DISP_AT_PACKLIFEDAY])/ DISP_DAYININSECOND) -
                        gGlobalParam.CMParam.aulCms[DISP_AT_PACKLIFEDAY];

                if(tmp < 0)
                {
                    tmp = 0;
                }
//                strTmp = tr("Replace in ") + decodeDays(tmp) + tr(" ") + tr("days");
                strTmp = tr("It is ") + decodeDays(tmp) + tr(" ") + tr("days overdue. ") + strWarnMsg;
                m_pCslistItem[iIdx]->setChangeDate(strTmp);
                
                strTmp = tr("Cat No.:") + gGlobalParam.cmSn.aCn[DISP_AT_PACK];
                m_pCslistItem[iIdx]->setCatNo(strTmp);
                
                strTmp = tr("Lot No.:") + gGlobalParam.cmSn.aLn[DISP_AT_PACK];
                m_pCslistItem[iIdx]->setLotNo(strTmp);
                m_pCslistItem[iIdx]->updateState(1);
                m_pCslistItem[iIdx]->setId(iIdx);
                m_pCslistItem[iIdx]->setName(tr("AT PACK"));
                iIdx++;
                    
            }
            break;  
        case DISP_H_PACK:
            if (gCMUsage.ulUsageState & (1 << DISP_H_PACKLIFEDAY) 
                || gCMUsage.ulUsageState & (1 << DISP_H_PACKLIFEL))
            {
                /* for DISP_AT_PACK column */
                tmp = gCMUsage.info.aulCms[DISP_H_PACKLIFEL] ;
                if(tmp < 0)
                {
                    tmp = 0;
                }
                strTmp = astrNames[0] + "  " + QString::number(tmp) + "L";
                m_pCslistItem[iIdx]->setValue(strTmp);
                
                strTmp = tr("Installation Date ") + decodeTime(gCMUsage.info.aulCms[DISP_H_PACKLIFEDAY]);
                m_pCslistItem[iIdx]->setInstDate(strTmp);
                
//                tmp = gGlobalParam.CMParam.aulCms[DISP_H_PACKLIFEDAY] -
//                    (DispGetCurSecond() - gCMUsage.info.aulCms[DISP_H_PACKLIFEDAY])/ DISP_DAYININSECOND;

                tmp = ((DispGetCurSecond() - gCMUsage.info.aulCms[DISP_H_PACKLIFEDAY])/ DISP_DAYININSECOND) -
                        gGlobalParam.CMParam.aulCms[DISP_H_PACKLIFEDAY];

                if(tmp < 0)
                {
                    tmp = 0;
                }
//                strTmp = tr("Replace in ") + decodeDays(tmp) + tr(" ") + tr("days");
                strTmp = tr("It is ") + decodeDays(tmp) + tr(" ") + tr("days overdue. ") + strWarnMsg;
                m_pCslistItem[iIdx]->setChangeDate(strTmp);
                
                strTmp = tr("Cat No.:") + gGlobalParam.cmSn.aCn[DISP_H_PACK];
                m_pCslistItem[iIdx]->setCatNo(strTmp);
                
                strTmp = tr("Lot No.:") + gGlobalParam.cmSn.aLn[DISP_H_PACK];
                m_pCslistItem[iIdx]->setLotNo(strTmp);
                
                m_pCslistItem[iIdx]->updateState(1);
                m_pCslistItem[iIdx]->setId(iIdx);
                m_pCslistItem[iIdx]->setName(tr("U PACK"));
                iIdx++;

            }
            break;              
        case DISP_N1_UV:
            if (gCMUsage.ulUsageState & (1 << DISP_N1_UVLIFEDAY) 
                || gCMUsage.ulUsageState & (1 << DISP_N1_UVLIFEHOUR))
            {
                /* for DISP_N1_UV column */
                tmp = gCMUsage.info.aulCms[DISP_N1_UVLIFEHOUR] ;
                if(tmp < 0)
                {
                    tmp = 0;
                }
                strTmp = astrNames[1] + "  " + QString::number(tmp) + "H";
                m_pCslistItem[iIdx]->setValue(strTmp);
                
                strTmp = tr("Installation Date ") + decodeTime(gCMUsage.info.aulCms[DISP_N1_UVLIFEDAY]);
                m_pCslistItem[iIdx]->setInstDate(strTmp);
                
//                tmp = gGlobalParam.CMParam.aulCms[DISP_N1_UVLIFEDAY] -
//                    (DispGetCurSecond() - gCMUsage.info.aulCms[DISP_N1_UVLIFEDAY])/ DISP_DAYININSECOND;
                tmp = ((DispGetCurSecond() - gCMUsage.info.aulCms[DISP_N1_UVLIFEDAY])/ DISP_DAYININSECOND) -
                        gGlobalParam.CMParam.aulCms[DISP_N1_UVLIFEDAY];

                if(tmp < 0)
                {
                    tmp = 0;
                }
//                strTmp = tr("Replace in ") + decodeDays(tmp) + tr(" ") + tr("days");
                strTmp = tr("It is ") + decodeDays(tmp) + tr(" ") + tr("days overdue. ") + strWarnMsg;
                m_pCslistItem[iIdx]->setChangeDate(strTmp);

                strTmp = tr("Cat No.:") + gGlobalParam.cmSn.aCn[DISP_N1_UV];
                m_pCslistItem[iIdx]->setCatNo(strTmp);
                
                strTmp = tr("Lot No.:") + gGlobalParam.cmSn.aLn[DISP_N1_UV];
                m_pCslistItem[iIdx]->setLotNo(strTmp);
                
                m_pCslistItem[iIdx]->updateState(1);
                m_pCslistItem[iIdx]->setId(iIdx);
                m_pCslistItem[iIdx]->setName(tr("254 UV"));
                iIdx++;

            }
            break;  
        case DISP_N2_UV:
            if (gCMUsage.ulUsageState & (1 << DISP_N2_UVLIFEDAY) 
                || gCMUsage.ulUsageState & (1 << DISP_N2_UVLIFEHOUR))
            {
                /* for DISP_N2_UV column */
                tmp = gCMUsage.info.aulCms[DISP_N2_UVLIFEHOUR] ;
                if(tmp < 0)
                {
                    tmp = 0;
                }
                strTmp = astrNames[1] + "  " + QString::number(tmp) + "H";
                m_pCslistItem[iIdx]->setValue(strTmp);
                
                strTmp = tr("Installation Date ") + decodeTime(gCMUsage.info.aulCms[DISP_N2_UVLIFEDAY]);
                m_pCslistItem[iIdx]->setInstDate(strTmp);
                
//                tmp = gGlobalParam.CMParam.aulCms[DISP_N2_UVLIFEDAY] -
//                    (DispGetCurSecond() - gCMUsage.info.aulCms[DISP_N2_UVLIFEDAY])/ DISP_DAYININSECOND;
                tmp = ((DispGetCurSecond() - gCMUsage.info.aulCms[DISP_N2_UVLIFEDAY])/ DISP_DAYININSECOND) -
                        gGlobalParam.CMParam.aulCms[DISP_N2_UVLIFEDAY];

                if(tmp < 0)
                {
                    tmp = 0;
                }
//                strTmp = tr("Replace in ") + decodeDays(tmp) + tr(" ") + tr("days");
                strTmp = tr("It is ") + decodeDays(tmp) + tr(" ") + tr("days overdue. ") + strWarnMsg;
                m_pCslistItem[iIdx]->setChangeDate(strTmp);
                
                strTmp = tr("Cat No.:") + gGlobalParam.cmSn.aCn[DISP_N2_UV];
                m_pCslistItem[iIdx]->setCatNo(strTmp);
                
                strTmp = tr("Lot No.:") + gGlobalParam.cmSn.aLn[DISP_N2_UV];
                m_pCslistItem[iIdx]->setLotNo(strTmp);

                m_pCslistItem[iIdx]->updateState(1);
                m_pCslistItem[iIdx]->setId(iIdx);
                m_pCslistItem[iIdx]->setName(tr("185 UV"));
                iIdx++;
            }
            break;  
        case DISP_N3_UV:
            if (gCMUsage.ulUsageState & (1 << DISP_N3_UVLIFEDAY) 
                || gCMUsage.ulUsageState & (1 << DISP_N3_UVLIFEHOUR))
            {
            
                /* for DISP_N3_UV column */
                tmp = gCMUsage.info.aulCms[DISP_N3_UVLIFEHOUR] ;
                if(tmp < 0)
                {
                    tmp = 0;
                }
                strTmp = astrNames[1] + "  " + QString::number(tmp) + "H";
                m_pCslistItem[iIdx]->setValue(strTmp);
                
                strTmp = tr("Installation Date ") + decodeTime(gCMUsage.info.aulCms[DISP_N3_UVLIFEDAY]);
                m_pCslistItem[iIdx]->setInstDate(strTmp);
                
//                tmp = gGlobalParam.CMParam.aulCms[DISP_N3_UVLIFEDAY] -
//                    (DispGetCurSecond() - gCMUsage.info.aulCms[DISP_N3_UVLIFEDAY])/ DISP_DAYININSECOND;
                tmp = ((DispGetCurSecond() - gCMUsage.info.aulCms[DISP_N3_UVLIFEDAY])/ DISP_DAYININSECOND) -
                        gGlobalParam.CMParam.aulCms[DISP_N3_UVLIFEDAY];
                if(tmp < 0)
                {
                    tmp = 0;
                }
//                strTmp = tr("Replace in ") + decodeDays(tmp) + tr(" ") + tr("days");
                strTmp = tr("It is ") + decodeDays(tmp) + tr(" ") + tr("days overdue. ") + strWarnMsg;
                m_pCslistItem[iIdx]->setChangeDate(strTmp);

                strTmp = tr("Cat No.:") + gGlobalParam.cmSn.aCn[DISP_N3_UV];
                m_pCslistItem[iIdx]->setCatNo(strTmp);
                
                strTmp = tr("Lot No.:") + gGlobalParam.cmSn.aLn[DISP_N3_UV];
                m_pCslistItem[iIdx]->setLotNo(strTmp);
                
                m_pCslistItem[iIdx]->updateState(1);
                m_pCslistItem[iIdx]->setId(iIdx);
                m_pCslistItem[iIdx]->setName(tr("TANK UV"));
                iIdx++;
            }
            break;  
        case DISP_N4_UV:
            if (gCMUsage.ulUsageState & (1 << DISP_N4_UVLIFEDAY) 
                || gCMUsage.ulUsageState & (1 << DISP_N4_UVLIFEHOUR))
            {
                /* for DISP_N4_UV column */
                tmp = gCMUsage.info.aulCms[DISP_N4_UVLIFEHOUR] ;
                if(tmp < 0)
                {
                    tmp = 0;
                }
                strTmp = astrNames[1] + "  " + QString::number(tmp) + "H";
                m_pCslistItem[iIdx]->setValue(strTmp);
                
                strTmp = tr("Installation Date ") + decodeTime(gCMUsage.info.aulCms[DISP_N4_UVLIFEDAY]);
                m_pCslistItem[iIdx]->setInstDate(strTmp);
                
//                tmp = gGlobalParam.CMParam.aulCms[DISP_N4_UVLIFEDAY] -
//                    (DispGetCurSecond() - gCMUsage.info.aulCms[DISP_N4_UVLIFEDAY])/ DISP_DAYININSECOND;
                tmp = ((DispGetCurSecond() - gCMUsage.info.aulCms[DISP_N4_UVLIFEDAY])/ DISP_DAYININSECOND) -
                        gGlobalParam.CMParam.aulCms[DISP_N4_UVLIFEDAY];
                if(tmp < 0)
                {
                    tmp = 0;
                }
//                strTmp = tr("Replace in ") + decodeDays(tmp) + tr(" ") + tr("days");
                strTmp = tr("It is ") + decodeDays(tmp) + tr(" ") + tr("days overdue. ") + strWarnMsg;
                m_pCslistItem[iIdx]->setChangeDate(strTmp);

                strTmp = tr("Cat No.:") + gGlobalParam.cmSn.aCn[DISP_N4_UV];
                m_pCslistItem[iIdx]->setCatNo(strTmp);
                
                strTmp = tr("Lot No.:") + gGlobalParam.cmSn.aLn[DISP_N4_UV];
                m_pCslistItem[iIdx]->setLotNo(strTmp);
                
                m_pCslistItem[iIdx]->updateState(1);
                m_pCslistItem[iIdx]->setId(iIdx);
                m_pCslistItem[iIdx]->setName(tr("TUBE UV"));
                iIdx++;

            }
            break;  
        case DISP_N5_UV:
/*2018/05/21 TOC UV*/
#if 0
            if (gCMUsage.ulUsageState & (1 << DISP_N5_UVLIFEDAY) 
                || gCMUsage.ulUsageState & (1 << DISP_N5_UVLIFEHOUR))
            {
                /* for DISP_N4_UV column */
                tmp = gCMUsage.info.aulCms[DISP_N5_UVLIFEHOUR] ;
                strTmp = astrNames[1] + "  " + QString::number(tmp) + "H";
                m_pCslistItem[iIdx]->setValue(strTmp);
                
                strTmp = tr("Installation Date ") + decodeTime(gCMUsage.info.aulCms[DISP_N5_UVLIFEDAY]);
                m_pCslistItem[iIdx]->setInstDate(strTmp);
                
//                tmp = gGlobalParam.CMParam.aulCms[DISP_N5_UVLIFEDAY] -
//                    (DispGetCurSecond() - gCMUsage.info.aulCms[DISP_N5_UVLIFEDAY])/ DISP_DAYININSECOND;
                tmp = ((DispGetCurSecond() - gCMUsage.info.aulCms[DISP_N5_UVLIFEDAY])/ DISP_DAYININSECOND) -
                        gGlobalParam.CMParam.aulCms[DISP_N5_UVLIFEDAY];
//                strTmp = tr("Replace in ") + decodeDays(tmp) + tr(" ") + tr("days");
                strTmp = tr("It is ") + decodeDays(tmp) + tr(" ") + tr("days overdue. ") + strWarnMsg;
                m_pCslistItem[iIdx]->setChangeDate(strTmp);

                strTmp = tr("Cat No.:") + gGlobalParam.cmSn.aCn[DISP_N5_UV];
                m_pCslistItem[iIdx]->setCatNo(strTmp);
                
                strTmp = tr("Lot No.:") + gGlobalParam.cmSn.aLn[DISP_N5_UV];
                m_pCslistItem[iIdx]->setLotNo(strTmp);
                
                m_pCslistItem[iIdx]->updateState(1);
                m_pCslistItem[iIdx]->setId(iIdx);
                m_pCslistItem[iIdx]->setName(tr("TOC UV"));
                iIdx++;

            }
#endif
            break;              
        case DISP_W_FILTER:
            if ((gCMUsage.ulUsageState & (1 << DISP_W_FILTERLIFE) 
                || gCMUsage.ulUsageState & (1 << DISP_W_FILTERLIFE))
                && (gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_HaveB2)))
            {
                /* for DISP_W_FILTER column */
                strTmp = tr("Installation Date ") + decodeTime(gCMUsage.info.aulCms[DISP_W_FILTERLIFE]);
                m_pCslistItem[iIdx]->setInstDate(strTmp);
                
//                tmp = gGlobalParam.CMParam.aulCms[DISP_W_FILTERLIFE] -
//                    (DispGetCurSecond() - gCMUsage.info.aulCms[DISP_W_FILTERLIFE])/ DISP_DAYININSECOND;

                tmp = ((DispGetCurSecond() - gCMUsage.info.aulCms[DISP_W_FILTERLIFE])/ DISP_DAYININSECOND) -
                        gGlobalParam.CMParam.aulCms[DISP_W_FILTERLIFE];
                if(tmp < 0)
                {
                    tmp = 0;
                }
//                strTmp = tr("Replace in ") + decodeDays(tmp) + tr(" ") + tr("days");
                strTmp = tr("It is ") + decodeDays(tmp) + tr(" ") + tr("days overdue. ") + strWarnMsg;
                m_pCslistItem[iIdx]->setChangeDate(strTmp);

                strTmp = tr("Cat No.:") + gGlobalParam.cmSn.aCn[DISP_W_FILTER];
                
                m_pCslistItem[iIdx]->setCatNo(strTmp);
                
                strTmp = tr("Lot No.:") + gGlobalParam.cmSn.aLn[DISP_W_FILTER];
                m_pCslistItem[iIdx]->setLotNo(strTmp);
                
                m_pCslistItem[iIdx]->updateState(1);
                m_pCslistItem[iIdx]->setId(iIdx);
                m_pCslistItem[iIdx]->setName(tr("Tank Vent Filter"));
                iIdx++;
            }
            break;  
        case DISP_T_B_FILTER:
            if (gCMUsage.ulUsageState & (1 << DISP_T_B_FILTERLIFE) 
                || gCMUsage.ulUsageState & (1 << DISP_T_B_FILTERLIFE))
            {
                /* for DISP_T_B_FILTER column */
                strTmp = tr("Installation Date ") + decodeTime(gCMUsage.info.aulCms[DISP_T_B_FILTERLIFE]);
                m_pCslistItem[iIdx]->setInstDate(strTmp);
                
//                tmp = gGlobalParam.CMParam.aulCms[DISP_T_B_FILTERLIFE] -
//                    (DispGetCurSecond() - gCMUsage.info.aulCms[DISP_T_B_FILTERLIFE])/ DISP_DAYININSECOND;

                tmp = ((DispGetCurSecond() - gCMUsage.info.aulCms[DISP_T_B_FILTERLIFE])/ DISP_DAYININSECOND) -
                        gGlobalParam.CMParam.aulCms[DISP_T_B_FILTERLIFE];
                if(tmp < 0)
                {
                    tmp = 0;
                }
//                strTmp = tr("Replace in ") + decodeDays(tmp) + tr(" ") + tr("days");
                strTmp = tr("It is ") + decodeDays(tmp) + tr(" ") + tr("days overdue. ") + strWarnMsg;
                m_pCslistItem[iIdx]->setChangeDate(strTmp);

                strTmp = tr("Cat No.:") + gGlobalParam.cmSn.aCn[DISP_T_B_FILTER];
                m_pCslistItem[iIdx]->setCatNo(strTmp);
                
                strTmp = tr("Lot No.:") + gGlobalParam.cmSn.aLn[DISP_T_B_FILTER];
                
                m_pCslistItem[iIdx]->setLotNo(strTmp);
                m_pCslistItem[iIdx]->updateState(1);
                m_pCslistItem[iIdx]->setId(iIdx);
                m_pCslistItem[iIdx]->setName(tr("Final Fliter B"));
                iIdx++;
            }
            break;  
        case DISP_T_A_FILTER:
            /* for DISP_T_A_FILTER column */
            if (gCMUsage.ulUsageState & (1 << DISP_T_A_FILTERLIFE) 
                || gCMUsage.ulUsageState & (1 << DISP_T_A_FILTERLIFE))
            {
                strTmp = tr("Installation Date ") + decodeTime(gCMUsage.info.aulCms[DISP_T_A_FILTER]);
                m_pCslistItem[iIdx]->setInstDate(strTmp);
                
//                tmp = gGlobalParam.CMParam.aulCms[DISP_T_A_FILTERLIFE] -
//                    (DispGetCurSecond() - gCMUsage.info.aulCms[DISP_T_A_FILTER])/ DISP_DAYININSECOND;
                tmp = ((DispGetCurSecond() - gCMUsage.info.aulCms[DISP_T_A_FILTERLIFE])/ DISP_DAYININSECOND) -
                        gGlobalParam.CMParam.aulCms[DISP_T_A_FILTERLIFE];
                if(tmp < 0)
                {
                    tmp = 0;
                }
//                strTmp = tr("Replace in ") + decodeDays(tmp) + tr(" ") + tr("days");
                strTmp = tr("It is ") + decodeDays(tmp) + tr(" ") + tr("days overdue. ") + strWarnMsg;
                m_pCslistItem[iIdx]->setChangeDate(strTmp);

                strTmp = tr("Cat No.:") + gGlobalParam.cmSn.aCn[DISP_T_A_FILTER];
                m_pCslistItem[iIdx]->setCatNo(strTmp);
                
                strTmp = tr("Lot No.:") + gGlobalParam.cmSn.aLn[DISP_T_A_FILTER];
                m_pCslistItem[iIdx]->setLotNo(strTmp);
                
                m_pCslistItem[iIdx]->updateState(1);
                m_pCslistItem[iIdx]->setId(iIdx);
                m_pCslistItem[iIdx]->setName(tr("Final Fliter A"));
                iIdx++;
                
            }
            break;  
            
        case DISP_TUBE_FILTER:
            if (gCMUsage.ulUsageState & (1 << DISP_TUBE_FILTERLIFE) 
                || gCMUsage.ulUsageState & (1 << DISP_TUBE_FILTERLIFE))
            {
                
                /* for DISP_T_A_FILTER column */
                strTmp = tr("Installation Date ") + decodeTime(gCMUsage.info.aulCms[DISP_TUBE_FILTERLIFE]);
                m_pCslistItem[iIdx]->setInstDate(strTmp);
                
//                tmp = gGlobalParam.CMParam.aulCms[DISP_TUBE_FILTERLIFE] -
//                    (DispGetCurSecond() - gCMUsage.info.aulCms[DISP_TUBE_FILTERLIFE])/ DISP_DAYININSECOND;

                tmp = ((DispGetCurSecond() - gCMUsage.info.aulCms[DISP_TUBE_FILTERLIFE])/ DISP_DAYININSECOND) -
                        gGlobalParam.CMParam.aulCms[DISP_TUBE_FILTERLIFE];

                if(tmp < 0)
                {
                    tmp = 0;
                }
//                strTmp = tr("Replace in ") + decodeDays(tmp) + tr(" ") + tr("days");
                strTmp = tr("It is ") + decodeDays(tmp) + tr(" ") + tr("days overdue. ") + strWarnMsg;
                m_pCslistItem[iIdx]->setChangeDate(strTmp);

                strTmp = tr("Cat No.:") + gGlobalParam.cmSn.aCn[DISP_TUBE_FILTERLIFE];
                m_pCslistItem[iIdx]->setCatNo(strTmp);
                
                strTmp = tr("Lot No.:") + gGlobalParam.cmSn.aLn[DISP_TUBE_FILTERLIFE];
                m_pCslistItem[iIdx]->setLotNo(strTmp);
                
                m_pCslistItem[iIdx]->updateState(1);
                m_pCslistItem[iIdx]->setId(iIdx);
                m_pCslistItem[iIdx]->setName(tr("Loop Filter"));
                iIdx++;

            }
            break;  
            
        case DISP_TUBE_DI:
            if (gCMUsage.ulUsageState & (1 << DISP_TUBE_DI_LIFE) 
                || gCMUsage.ulUsageState & (1 << DISP_TUBE_DI_LIFE))
            {
            
                /* for DISP_TUBE_DI column */
                strTmp = tr("Installation Date ") + decodeTime(gCMUsage.info.aulCms[DISP_TUBE_DI_LIFE]);
                m_pCslistItem[iIdx]->setInstDate(strTmp);
                
//                tmp = gGlobalParam.CMParam.aulCms[DISP_TUBE_DI_LIFE] -
//                    (DispGetCurSecond() - gCMUsage.info.aulCms[DISP_TUBE_DI_LIFE])/ DISP_DAYININSECOND;

                tmp = ((DispGetCurSecond() - gCMUsage.info.aulCms[DISP_TUBE_DI_LIFE])/ DISP_DAYININSECOND) -
                        gGlobalParam.CMParam.aulCms[DISP_TUBE_DI_LIFE];

                if(tmp < 0)
                {
                    tmp = 0;
                }
//                strTmp = tr("Replace in ") + decodeDays(tmp) + tr(" ") + tr("days");
                strTmp = tr("It is ") + decodeDays(tmp) + tr(" ") + tr("days overdue. ") + strWarnMsg;
                m_pCslistItem[iIdx]->setChangeDate(strTmp);

                strTmp = tr("Cat No.:") + gGlobalParam.cmSn.aCn[DISP_TUBE_DI_LIFE];
                m_pCslistItem[iIdx]->setCatNo(strTmp);
                
                strTmp = tr("Lot No.:") + gGlobalParam.cmSn.aLn[DISP_TUBE_DI_LIFE];
                m_pCslistItem[iIdx]->setLotNo(strTmp);
                
                m_pCslistItem[iIdx]->updateState(1);
                m_pCslistItem[iIdx]->setId(iIdx);
                m_pCslistItem[iIdx]->setName(tr("Loop DI"));
                iIdx++;
            }
            break;  
        case DISP_ROC12:
            if (gCMUsage.ulUsageState & (1 << DISP_ROC12LIFEDAY) 
                || gCMUsage.ulUsageState & (1 << DISP_ROC12LIFEDAY))
            {
            
                /* for DISP_TUBE_DI column */
                strTmp = tr("Last Maintenance ") + decodeTime(gCMUsage.info.aulCms[DISP_ROC12LIFEDAY]);
                m_pCslistItem[iIdx]->setInstDate(strTmp);
                
//                tmp = gGlobalParam.CMParam.aulCms[DISP_ROC12LIFEDAY] -
//                    (DispGetCurSecond() - gCMUsage.info.aulCms[DISP_ROC12LIFEDAY])/ DISP_DAYININSECOND;

                tmp = ((DispGetCurSecond() - gCMUsage.info.aulCms[DISP_ROC12LIFEDAY])/ DISP_DAYININSECOND) -
                        gGlobalParam.CMParam.aulCms[DISP_ROC12LIFEDAY];


                if(tmp < 0)
                {
                    tmp = 0;
                }
//                strTmp = tr("Replace in ") + decodeDays(tmp) + tr(" ") + tr("days");\
                strTmp = tr("It is ") + decodeDays(tmp) + tr(" ") + tr("days overdue. ") + strROMsg;
                m_pCslistItem[iIdx]->setChangeDate(strTmp);

                /*
                strTmp = tr("Cat No.:") + gGlobalParam.cmSn.aCn[DISP_ROC12LIFEDAY];
                m_pCslistItem[iIdx]->setCatNo(strTmp);
                
                strTmp = tr("Lot No.:") + gGlobalParam.cmSn.aLn[DISP_ROC12LIFEDAY];
                m_pCslistItem[iIdx]->setLotNo(strTmp);
                */

                m_pCslistItem[iIdx]->updateState(1);
                m_pCslistItem[iIdx]->setId(iIdx);
                m_pCslistItem[iIdx]->setName(tr("RO Chlorine"));
                iIdx++;
            }
            break;   
        }
    }

    for (iLoop = 0; iLoop < iIdx; iLoop++)
    {
        m_listWidget[0]->setItemHidden(m_pCslistWidgetItem[iLoop],false);
    }

    for (iLoop = iIdx; iLoop < m_aiRealNum[ALARM_TYPE_CS]; iLoop++)
    {
        m_listWidget[0]->setItemHidden(m_pCslistWidgetItem[iLoop],true);
    }

}


void AlarmDisplayPage::asUpdate()
{
     int iLoop = 0;
     int iIdx  = 0;
    
     for (iLoop = 0; iLoop < m_aiRealNum[ALARM_TYPE_AS]; iLoop++)
     {
         switch(aAsIds[iLoop].iId)
         {
         case MAKEID(DISP_ALARM_PART0,DISP_ALARM_PART0_254UV_OOP):
         case MAKEID(DISP_ALARM_PART0,DISP_ALARM_PART0_185UV_OOP):
         case MAKEID(DISP_ALARM_PART0,DISP_ALARM_PART0_TANKUV_OOP):
         case MAKEID(DISP_ALARM_PART0,DISP_ALARM_PART0_TUBEUV_OOP):
         case MAKEID(DISP_ALARM_PART0,DISP_ALARM_PART0_PPACK_OOP):
         case MAKEID(DISP_ALARM_PART0,DISP_ALARM_PART0_ATPACK_OOP):
         case MAKEID(DISP_ALARM_PART0,DISP_ALARM_PART0_HPACK_OOP):
         case MAKEID(DISP_ALARM_PART0,DISP_ALARM_PART0_UPACK_OOP):
             if (m_wndMain->getAlarmInfo(DISP_ALARM_PART0,aAsIds[iLoop].iId - MAKEID(DISP_ALARM_PART0,DISP_ALARM_PART0_254UV_OOP)))
             {
                 if (0XFF != aAsIndex[DISP_ALARM_PART0][aAsIds[iLoop].iId - MAKEID(DISP_ALARM_PART0,DISP_ALARM_PART0_254UV_OOP)])
                 {
                     m_pAslistItem[iIdx]->setName(m_astrPartAlarm[aAsIndex[DISP_ALARM_PART0][aAsIds[iLoop].iId - MAKEID(DISP_ALARM_PART0,DISP_ALARM_PART0_254UV_OOP)]]);
                     m_pAslistItem[iIdx]->setId(aAsIds[iLoop].iId);
                     iIdx++;
                 }
             }
            break;
         default:
             if (m_wndMain->getAlarmInfo(DISP_ALARM_PART1,aAsIds[iLoop].iId - MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_SOURCE_WATER_PRESSURE)))
             {
                 if (0XFF != aAsIndex[DISP_ALARM_PART1][aAsIds[iLoop].iId - MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_SOURCE_WATER_PRESSURE)])
                 {
                     m_pAslistItem[iIdx]->setName(m_astrPartAlarm[aAsIndex[DISP_ALARM_PART1][aAsIds[iLoop].iId - MAKEID(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_SOURCE_WATER_PRESSURE)]]);
                     m_pAslistItem[iIdx]->setId(aAsIds[iLoop].iId);
                     iIdx++;
                 }
             }
             break;
         }

     }
     
     for (iLoop = 0; iLoop < iIdx; iLoop++)
     {
         m_listWidget[1]->setItemHidden(m_pAslistWidgetIem[iLoop],false);
     }
     
     for (iLoop = iIdx; iLoop < m_aiRealNum[ALARM_TYPE_AS]; iLoop++)
     {
         m_listWidget[1]->setItemHidden(m_pAslistWidgetIem[iLoop],true);
     }

}

void AlarmDisplayPage::setBackColor()
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

void AlarmDisplayPage::initUi()
{
    setBackColor();

    createList();
}

void AlarmDisplayPage::on_btn_clicked(int index)
{

}

