#include <math.h>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "notify.h"
#include "CfgDialog.h"
#include "DbDialog.h"
#include "usermanage.h"

#include "opt.h"
#include "LoginDlg.h"

#include <QLabel>
#include <QString>
#include <QBrush>
#include <QRadialGradient>
#include <QStringListModel>
#include <QPushButton>
#include <QComboBox>
#include <QLineEdit>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QTime>
#include <QSqlError>
#include <QSqlDriver>
#include <QSqlRecord>
#include <QTextCodec>

#include <QPainter>

#include <QMouseEvent>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <dirent.h>
#include <libgen.h>
#include <signal.h>
#include <fcntl.h>
#include <time.h>
#include <errno.h>
#include <sys/time.h>
#include <termios.h>
#include <arpa/inet.h>

#include <QDebug>

#include "LoginDlg.h"
#include "rpc.h"
#include "sapp.h"
#include "Interface.h"
#include "MyParams.h"
#include "LockupDlg.h"
#include "memory.h"
#include "navigatorbar.h"
#include "mainpage.h"
#include "menupage.h"
#include "setpage.h"
#include "servicepage.h"
#include "SterilizePage.h"
#include <QMovie>
#include <typeinfo>
#include <QTcpSocket>
#include <QFileSystemWatcher>
#include <QDir>

#include "setdevicepage.h"
#include "systestpage.h"
#include "buzzer_ctl.h"
#include "waterqualitypage.h"
#include "systemmonitorpage.h"
#include "ConsumableinsPage.h"
#include "alarmdisplaypage.h"
#include "cminterface.h"
#include "ToastDlg.h"

#include "ExtraDisplay.h" //dcj_ex
#include "ex_loginstate.h"
#include "ex_init_languagepage.h"
#include "ex_init_networkpage.h"
#include "ex_init_handlecfgpage.h"
#include "ex_init_timepage.h"
#include "ex_init_tankcfgpage.h"
#include "ex_init_syscfgpage.h"
#include "ex_screensleeppage.h"
#include "ex_factorytestpage.h"
#include "ex_checkconsumaleinstall.h"
#include "ex_consumableinstalldialog.h"
#include "ex_flowchartpage.h"
#include "DNetworkConfig.h"
#include "dhttpworker.h"
#include "dwificonfigdialog.h"
#include "dwificonfigwidget.h"
#include "ex_hintdialog.h"

//#include "ex_screensleepthread.h"
/***********************************************
B3: source water tank
B2: pure water tank
B1: tube water pressure

ROˮ:  I2
ROˮ I1
EDIˮI3
TOC缫/ˮˮ(ˮѭ)I4 
UPˮ I5

S4: ROˮ
S3: ROˮ/EDIˮ
S2RO ˮ
S1ˮU pack



·DI: 
·UV:

ע⣺E1C1ͬʱرʱE1C1ʱ5s

C4ѭ (·UV)

ROˮ : S3 
ROˮ : S4 
ROˮ : S2 
ˮ S3+S4
EDIˮ: S3*0.8
EDIˮ: S3*0.2

ͳ:

Ԥ: S3+S4

P-PACK = S3+S4

U-PACK = UPˮ(S1)

AT-PACK= S3

H-PACK=  S1

************************************************/

/************************************************
E1  ROˮŷ    
E2  ROˮŷ    
E3  ROϸŷŷ  
E4  ˮˮŷ  
E5  UPѭŷ    
E6  HPѭŷ    
E7/E8   ˮŷ(UP/HPã  
E9  TOCϴŷ   
E10 ԭˮŷ  
        
C1  ROѹ    
C2  UPѭ    
C3  ԭˮѹ  
C4  ·ѭ  
        
B1  ѹ  
B2  ˮҺλ    
B3  ԭˮҺλ    
        
K1/K2   ˮأUP/HPãҪ㴥ͳֻģʽ 
K3  Ԥϴź  
K4  ˮź/©ˮź 
K5  ˮѹź  
        
N1  254nm 
N2  185nm 
N3  ˮ  
N4  ·  
        
T   EDIģ 
        
S1  ˮˮ    
S2  ROˮ  
S3  ROˮ  
S4  ROˮ  
        
I1  ROˮ絼    
I2  ROˮ絼    
I3  EDIˮ   
I4  ѭˮ    
I5  UPˮ    


*************************************************/

#define BUZZER_FILE       "/dev/buzzer_ctl"

/*
Version: 0.1.2.181119.release
0       :  Major version number
1       :  Minor version number
2       :  Revision number
181119  :  Date version number
release :  version phase
*/
QString strSoftwareVersion = QString("0.1.8.190618_debug");

MainWindow *gpMainWnd;


QPixmap    *gpGlobalPixmaps[GLOBAL_BMP_NUM];

QString gGlobalPixelmapName[GLOBAL_BMP_NUM] = 
{
    ":/pic/home.png",
    ":/pic/run.png",
    ":/pic/stop.png",
    ":/pic/btn_power_front.png",
    ":/pic/btn_power_back.png",
    ":/pic/bar_seperator.png",
    ":/pic/navigation_bar.png",
    ":/pic/time_bar.png",
    ":/pic/btn_general_active.png",
    ":/pic/btn_general_normal.png",
    ":/pic/current_page.png",
    ":/pic/unselected_page.png",
    ":/pic/back.png",
    ":/pic/Channel_Press.png",
    ":/pic/Channel_Release.png",
    ":/pic/page_navi_normal.png",
    ":/pic/page_navi_active.png",
    ":/pic/Cancle.png",
    ":/pic/Cancle_p.png",
    ":/pic/ok.png",
    ":/pic/ok_p.png",
    ":/pic/Send_Nor.png",
    ":/pic/Send_Pres.png",
    ":/pic/btn_general_30_active.png",
    ":/pic/btn_general_30_normal.png",
    ":/pic/cs_normal_state.png",
    ":/pic/cs_notification_state.png",
    ":/pic/shopping_cart.png",
    ":/pic/Remind.png",
    ":/pic/Warn.png",
    ":/pic/Monitor_Device_ON.png", //ex
    ":/pic/Monitor_Device_OFF.png", //ex
};


DISP_GLOBAL_PARAM_STRU gGlobalParam ;

Ex_DISP_PARAM_CALI_STRU ex_global_Cali; //dcj_ex;
Ex_LoginState global_LoginState; //dcj_ex;
Ex_LoginState user_LoginState; //ex
Ex_DISP_GLOBAL_PARAM_STRU ex_gGlobalParam; //dcj_ex;
EX_CCB  ex_gCcb; //dcj_ex
unsigned int g_screenSleep;
bool g_isScreenSleep;
unsigned int ex_gulSecond = 0;
unsigned short ex_isPackNew;
QMutex ex_gMutex;

DISP_CM_USAGE_STRU     gCMUsage ;

//#define PCBTEST

MACHINE_TYPE_STRU gaMachineType[MACHINE_NUM] =
{
#ifdef PCBTEST
    {MACH_NAME_LGenie,   MACHINE_MODEL_MID ,MACHINE_FUNCTION_ALL,DEFAULT_MODULES_L_Genie ,7055 ,{1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, 1,1,1,1, 1,1,1, 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,1},},
#else
    {MACH_NAME_LGenie,   MACHINE_MODEL_MID ,MACHINE_FUNCTION_ALL,DEFAULT_MODULES_L_Genie ,450 ,{1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, 1,1,1,1, 1,1,1, 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,1},},
#endif
    {MACH_NAME_LUP,      MACHINE_MODEL_MID ,MACHINE_FUNCTION_ALL,DEFAULT_MODULES_L_UP    ,450 ,{1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, 1,1,1,1, 1,1,1, 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,1},},
    {MACH_NAME_LEDI_LOOP,MACHINE_MODEL_MID ,MACHINE_FUNCTION_EDI,DEFAULT_MODULES_EDI_LOOP,450 ,{1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, 1,1,1,1, 1,1,1, 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,1},},
    {MACH_NAME_LRO_LOOP, MACHINE_MODEL_MID ,MACHINE_FUNCTION_EDI,DEFAULT_MODULES_RO_LOOP ,450 ,{1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, 1,1,1,1, 1,1,1, 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,1},},
    {MACH_NAME_Genie,    MACHINE_MODEL_DESK,MACHINE_FUNCTION_ALL,DEFAULT_MODULES_Genie   ,7055 ,{1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, 1,1,1,1, 1,1,1, 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,1},},
    {MACH_NAME_UP,       MACHINE_MODEL_DESK,MACHINE_FUNCTION_ALL,DEFAULT_MODULES_UP      ,7055 ,{1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, 1,1,1,1, 1,1,1, 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,1},},
    {MACH_NAME_EDI,      MACHINE_MODEL_DESK,MACHINE_FUNCTION_EDI,DEFAULT_MODULES_EDI     ,7055 ,{1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, 1,1,1,1, 1,1,1, 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,1},},
    {MACH_NAME_RO,       MACHINE_MODEL_DESK,MACHINE_FUNCTION_EDI,DEFAULT_MODULES_RO      ,7055 ,{1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, 1,1,1,1, 1,1,1, 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,1},},
    {MACH_NAME_PURIST,   MACHINE_MODEL_DESK,MACHINE_FUNCTION_UP ,DEFAULT_MODULES_PURIST  ,7055,{1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, 1,1,1,1, 1,1,1, 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,1},},
    {MACH_NAME_Adapt,    MACHINE_MODEL_DESK,MACHINE_FUNCTION_ALL,DEFAULT_MODULES_ADAPT   ,7055 ,{1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, 1,1,1,1, 1,1,1, 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,1},},
};

QString gastrTmCfgName[] = 
{
   "RoWashT1","RoWashT2","RoWashT3",
   "PhWashT1","PhWashT2","PhWashT3","PhWashT4","PhWashT5",    
   "InitRunT1",
   "NormRunT1","NormRunT2","NormRunT3","NormRunT4","NormRunT5",
   "N3Period", "N3Duration",
   "TOCT1","TOCT2","RoWashT3",
   "IdleCirPeriod", "InterCirDuration","LPP",
};

QString gastrSmCfgName[] = 
{
    "Flag",
};

QString gastrAlarmCfgName[] = 
{
    "Flag0",
    "Flag1",
};

QString gastrCMCfgName[] = 
{
    "P_PACKLIFEDAY","P_PACKLIFEL",
    "U_PACKLIFEDAY","U_PACKLIFEL",
    "H_PACKLIFEDAY","H_PACKLIFEL",
    "AT_PACKLIFEDAY","AT_PACKLIFEL",
    "N1_UVLIFEDAY","N1_UVLIFEHOUR",
    "N2_UVLIFEDAY","N2_UVLIFEHOUR","N3_UVLIFEDAY",
    "N3_UVLIFEHOUR","T_FILTERLIFE","P_FILTERLIFE",
    "W_FILTERLIFE","ROC12LIFEDAY",
};

QString gastrCMActionName[] = 
{
    "Install Prefilter",
    "Install AC Pack",
    "Install T Pack",
    "Install P Pack",
    "Install U Pack",
    "Install AT Pack",
    "Install H Pack",
    "Install 254 UV",
    "Install 185 UV",
    "Install Tank UV",
    "Install Loop UV",
    "Install TOC UV",
    "Install Tank Vent Filter",
    "Install Final Filter B",
    "Install Final Filter A",
    "Install Loop Filter",
    "Install Tube DI",
    "Reset ROC12",
};

QString gastrMachineryActionName[] = 
{
    "Install Feed Pump",
    "Install Dist. Pump",
    "Install Recir. Pump",
    "Install RO Membrane",
    "Install RO Pump",
    "Install EDI", 
};

QString gastrLoginOperateActionName[] = 
{
    "Consumables Life",
    "Alarm Config",
    "Test",
    "Calibration",
    "Alarm S.P",
    "System Config",
    "LCD",
    "Connectivity",
    "Dist. Control",
    "Time & Date",
    "Language",
    "Audio",
    "Units",
    "Connecting Device",
    "RFID Config",
    "Consumable intall Permission"
};



char tmpbuf[1024];

extern QString INSERT_sql_Water;
extern QString select_sql_Water;
extern QString update_sql_Water;

extern QString INSERT_sql_Alarm ;
extern QString INSERT_sql_GetW  ;
extern QString INSERT_sql_PW    ;
extern QString INSERT_sql_Log   ;

extern QString select_sql_Handler ;
extern QString delete_sql_Handler ;
extern QString INSERT_sql_Handler ;

extern QString select_sql_Rfid ;
extern QString delete_sql_Rfid ;
extern QString INSERT_sql_Rfid ;

extern QString select_sql_Consumable;
extern QString insert_sql_Consumable;
extern QString update_sql_Consumable;

char *State_info[] =
{
    "INIT","ROWASH",
    "PHWASH","RUN",
    "LPP","QTW",
    "CIR","DEC",
};

/* total alarm here */
QString gastrAlarmName[] =
{
    "Check 254 UV Lamp",
    "Check 185 UV Lamp",
    "Check Tank UV Lamp",
    "Check Loop UV Lamp",
    "Prefiltration Pack Not Detected",
    "AC-Pack Not Detected",
    "P-Pack Not Detected",
    "AT-Pack Not Detected",
    "H-Pack Not Detected",
    "U-Pack Not Detected",
    "Low Feed Water Pressure",
    "Feed Water Conductivity>SP",
    "RO Product Conductivity>SP",
    "RO Rejection Rate<SP",
    "EDI Product Resistivity<SP",
    "UP Product Resistivity<SP",
    "Loop Water Resistivity<SP",
    "Pure Tank Level<SP",
    "Feed Tank Level<SP",
    "RO Product Rate<SP",
    "RO Drain Rate<SP",
    "Recirculating Water Resistivity<SP",
    "HP Water Resistivity<SP",
    "Tank Water Resistivity<SP",
    "High Feed Water Temperature",
    "Low Feed Water Temperature",
    "High RO Product Temperature",
    "Low RO Product Temperature",
    "High EDI Product Temperature",
    "Low EDI Product Temperature",
    "High UP Product Temperature",
    "Low UP Product Temperature",
    "High Loop Water Temperature",
    "Low Loop Water Temperature",
    "High TOC Sensor Temperature",
    "Low TOC Sensor Temperature",
    "TOC Feed Water Resistivity<SP",
    "Leakage or Tank Overflow",
    "High Work Pressure"
};

void MainRetriveLastRunState(int iMachineType)
{
    QString strCfgName = gaMachineType[iMachineType].strName;
    strCfgName += ".ini";
    QSettings *config = new QSettings(strCfgName, QSettings::IniFormat);
    QString strV = "/LastRunState/";
    ex_gGlobalParam.lastRunState = config->value(strV, 0).toInt();
    qDebug() << strV << ":" << ex_gGlobalParam.lastRunState;

    if (config)
    {
        delete config;
        config = NULL;
    }
}

void MainRetriveDefaultState(int iMachineType)//
{
    QString strCfgName = gaMachineType[iMachineType].strName;
    strCfgName += ".ini";
    QSettings *config = new QSettings(strCfgName, QSettings::IniFormat);
    QString strV = "/DefaultState/";
    ex_gGlobalParam.Ex_Default = config->value(strV, 0).toInt();
    qDebug() << strV << ":" << ex_gGlobalParam.Ex_Default;

    if (config)
    {
        delete config;
        config = NULL;
    }
}

void MainRetriveExMachineMsg(int iMachineType) //ex_dcj
{
    QString strCfgName = gaMachineType[iMachineType].strName;
    strCfgName += ".ini";
    QSettings *config = new QSettings(strCfgName, QSettings::IniFormat);
    QString strV;

    strV = "/ExMachineMsg/MachineFlow/";
    ex_gGlobalParam.Ex_Machine_Msg.iMachineFlow = config->value(strV, 5).toInt();

    if (config)
    {
        delete config;
        config = NULL;
    }
}

void MainRetriveProductMsg(int iMachineType) //ex_dcj
{
    QString strCfgName = gaMachineType[iMachineType].strName;
    strCfgName += ".ini";
    QSettings *config = new QSettings(strCfgName, QSettings::IniFormat);
    QString strV;

    strV = "/ProductMsg/CatalogNo/";
    ex_gGlobalParam.Ex_System_Msg.Ex_CatalogNo = config->value(strV, "unknow").toString();

    strV = "/ProductMsg/SerialNo/";
    ex_gGlobalParam.Ex_System_Msg.Ex_SerialNo = config->value(strV, "unknow").toString();

    strV = "/ProductMsg/ProductionDate/";
    ex_gGlobalParam.Ex_System_Msg.Ex_ProDate = config->value(strV, "unknow").toString();

    strV = "/ProductMsg/SoftwareVersion/";
    ex_gGlobalParam.Ex_System_Msg.Ex_SofeVer = strSoftwareVersion;


    if (config)
    {
        delete config;
        config = NULL;
    }
}
void MainRetriveInstallMsg(int iMachineType) //ex_dcj
{
    QString strCfgName = gaMachineType[iMachineType].strName;
    strCfgName += ".ini";
    QSettings *config = new QSettings(strCfgName, QSettings::IniFormat);

    QString strV = "/InstallMsg/InstallDate/";
    ex_gGlobalParam.Ex_System_Msg.Ex_InsDate = config->value(strV, "unknow").toString();

    if (config)
    {
        delete config;
        config = NULL;
    }
}

void MainRetriveExConfigParam(int iMachineType)
{
    QString strCfgName = gaMachineType[iMachineType].strName;
    strCfgName += ".ini";
    QSettings *config = new QSettings(strCfgName, QSettings::IniFormat);

    QString strV;
    strV = "/ExConfigParam/ScreenSleepTime";
    ex_gGlobalParam.Ex_Config_Param.iScreenSleepTime = config->value(strV, 10).toInt();

    strV = "/ExConfigParam/DispenseFlowRate";
    ex_gGlobalParam.Ex_Config_Param.flowRate = config->value(strV, 2.0).toFloat();

    if (config)
    {
        delete config;
        config = NULL;
    }
}

//2019.1.21 add
void MainRetriveExConsumableMsg(int iMachineType, DISP_CONSUME_MATERIAL_SN_STRU &cParam, DISP_MACHINERY_SN_STRU  &mParam)
{
    Q_UNUSED(iMachineType);
    QString strUnknow = QString("unknow");
    int iLoop;
    for (iLoop = 0; iLoop < DISP_CM_NAME_NUM; iLoop++)
    {
        memset(cParam.aCn[iLoop],0,sizeof(CATNO));
        strncpy(cParam.aCn[iLoop],strUnknow.toAscii(),APP_CAT_LENGTH);
        memset(cParam.aLn[iLoop],0,sizeof(LOTNO));
        strncpy(cParam.aLn[iLoop],strUnknow.toAscii(),APP_LOT_LENGTH);
    }
    for (iLoop = 0; iLoop < DISP_MACHINERY_NAME_NUM; iLoop++)
    {
        memset(mParam.aCn[iLoop],0,sizeof(CATNO));
        strncpy(mParam.aCn[iLoop],strUnknow.toAscii(),APP_CAT_LENGTH);
        memset(mParam.aLn[iLoop],0,sizeof(LOTNO));
        strncpy(mParam.aLn[iLoop],strUnknow.toAscii(),APP_LOT_LENGTH);
    }
    qDebug() << "MainRetriveExConsumableMsg() running";

    QSqlQuery query;
    query.exec("select * from Consumable");
    while(query.next())
    {
        qDebug() << QString("MainRetriveExConsumableMsg: %1").arg(query.value(4).toInt());
        if(query.value(4).toInt() == 0)
        {
            qDebug() << QString("0 RetriveConsumable: %1").arg(query.value(1).toInt());
            memset(cParam.aCn[query.value(1).toInt()], 0, sizeof(CATNO));
            strncpy(cParam.aCn[query.value(1).toInt()], query.value(2).toString().toAscii(), APP_CAT_LENGTH);
            memset(cParam.aLn[query.value(1).toInt()], 0, sizeof(LOTNO));
            strncpy(cParam.aLn[query.value(1).toInt()], query.value(3).toString().toAscii(), APP_LOT_LENGTH);
        }
        else
        {
            qDebug() << QString("1 RetriveConsumable: %1").arg(query.value(1).toInt());
            memset(mParam.aCn[query.value(1).toInt()], 0, sizeof(CATNO));
            strncpy(mParam.aCn[query.value(1).toInt()], query.value(2).toString().toAscii(), APP_CAT_LENGTH);
            memset(mParam.aLn[query.value(1).toInt()], 0, sizeof(LOTNO));
            strncpy(mParam.aLn[query.value(1).toInt()], query.value(3).toString().toAscii(), APP_LOT_LENGTH);
        }
    }
}

void MainRetriveMachineType(int &iMachineType)
{
    /* retrive parameter from configuration */
    QSettings *cfgGlobal = new QSettings(GLOBAL_CFG_INI, QSettings::IniFormat);

    if (cfgGlobal)
    {
        iMachineType = cfgGlobal->value("/global/machtype", MACHINE_Genie).toInt(); //MACHINE_L_Genie
        /* More come here late implement*/
        delete cfgGlobal;
    }
}

void MainRetriveMachineParam(int iMachineType,DISP_MACHINE_PARAM_STRU  &Param)
{
    /* retrive parameter from configuration */
    int iLoop;

    float workPressure = 8.0;
    float defaultRejLimit = 92.0;
    float defaultROLimit = 50.0;
    switch(iMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_L_EDI_LOOP:
    case MACHINE_L_RO_LOOP:
        workPressure = 12.0;
        break;
    case MACHINE_ADAPT:
        defaultRejLimit = 95.0;
        defaultROLimit = 40.0;
        break;
    default:
        break;
    }

    float defautlValue[] = {MM_DEFALUT_SP1, defaultRejLimit, defaultROLimit, MM_DEFALUT_SP4,MM_DEFALUT_SP5,
                            MM_DEFALUT_SP6,MM_DEFALUT_SP7,MM_DEFALUT_SP8,MM_DEFALUT_SP9,MM_DEFALUT_SP10,
                            MM_DEFALUT_SP11,MM_DEFALUT_SP12,MM_DEFALUT_SP13,MM_DEFALUT_SP14,MM_DEFALUT_SP15,
                            MM_DEFALUT_SP16,MM_DEFALUT_SP17,MM_DEFALUT_SP18,MM_DEFALUT_SP19,MM_DEFALUT_SP20,
                            MM_DEFALUT_SP21,MM_DEFALUT_SP22,MM_DEFALUT_SP23,MM_DEFALUT_SP24,MM_DEFALUT_SP25,
                            MM_DEFALUT_SP26,MM_DEFALUT_SP27,MM_DEFALUT_SP28,MM_DEFALUT_SP29,MM_DEFALUT_SP30,
                            MM_DEFALUT_SP31,MM_DEFALUT_SP32, workPressure};

    QString strCfgName = gaMachineType[iMachineType].strName;

    strCfgName += ".ini";
    
    QSettings *config = new QSettings(strCfgName, QSettings::IniFormat);

    for(iLoop = 0; iLoop < MACHINE_PARAM_SP_NUM ; iLoop++)
    {
        QString strV = "/SP/";

        strV += QString::number(iLoop);

        //QString Result = config->value(strV).toString();

        Param.SP[iLoop] = config->value(strV,defautlValue[iLoop]).toFloat();

        qDebug() << strV << ":" << Param.SP[iLoop];

    }

    if (config)
    {
        delete config;
        config = NULL;
    }


}


void MainRetriveTMParam(int iMachineType,DISP_TIME_PARAM_STRU  &Param)
{
    /* retrive parameter from configuration */
    int iLoop;

    QString strCfgName = gaMachineType[iMachineType].strName;

    strCfgName += ".ini";

    QSettings *config = new QSettings(strCfgName, QSettings::IniFormat);

    for(iLoop = 0; iLoop < TIME_PARAM_NUM ; iLoop++)
    {
        QString strV = "/TM/";

        strV += gastrTmCfgName[iLoop];

        int iValue = config->value(strV,5000).toInt();
        Param.aulTime[iLoop] = iValue;

        qDebug() << strV << ":" <<iValue;
    }    
    if (config)
    {
        delete config;
        config = NULL;
    }
}

void MainRetriveAlarmSetting(int iMachineType,DISP_ALARM_SETTING_STRU  &Param)
{
    /* retrive parameter from configuration */
    int iLoop;

    unsigned int defautlValue[] = {DISP_ALARM_DEFAULT_PART0,DISP_ALARM_DEFAULT_PART1};

    QString strCfgName = gaMachineType[iMachineType].strName;

    strCfgName += ".ini";
    
    QSettings *config = new QSettings(strCfgName, QSettings::IniFormat);

    for(iLoop = 0; iLoop < 2 ; iLoop++)
    {
        QString strV = "/ALARM/";

        strV += gastrAlarmCfgName[iLoop];

        int iValue = config->value(strV,defautlValue[iLoop]).toInt();
        Param.aulFlag[iLoop] = iValue;

        qDebug() << strV << ":" <<iValue;
    } 

        
    if (config)
    {
        delete config;
        config = NULL;
    }
}


void MainRetriveSubModuleSetting(int iMachineType,DISP_SUB_MODULE_SETTING_STRU  &Param)
{
    /* retrive parameter from configuration */
    //int iLoop;

    QString strCfgName = gaMachineType[iMachineType].strName;

    strCfgName += ".ini";
    
    QSettings *config = new QSettings(strCfgName, QSettings::IniFormat);

    {
        QString strV = "/SM/" + gastrSmCfgName[0];

        int iValue ;
        //float fValue;

        iValue = config->value(strV,gaMachineType[iMachineType].iDefaultModule).toInt();

        Param.ulFlags  = iValue;

        qDebug() << strV << ":" << iValue;
        

    }

    switch(iMachineType)
    {
    case MACHINE_Genie:
    case MACHINE_EDI:
    case MACHINE_UP:
    case MACHINE_RO:
    case MACHINE_PURIST:
        Param.ulFlags &= ~(1 << DISP_SM_HaveB3); //
        break;
    case MACHINE_ADAPT:
        Param.ulFlags &= ~(1 << DISP_SM_HaveB3); //
        Param.ulFlags &= ~(1 << DISP_SM_HaveB2);
        break;
    }

    if (config)
    {
        delete config;
        config = NULL;
    }
}

void MainRetriveCMParam(int iMachineType,DISP_CONSUME_MATERIAL_STRU  &Param)
{
    /* retrive parameter from configuration */
    int iLoop;

    QString strCfgName = gaMachineType[iMachineType].strName;

    strCfgName += ".ini";
    
    QSettings *config = new QSettings(strCfgName, QSettings::IniFormat);

    for (iLoop = 0; iLoop < DISP_CM_NUM; iLoop++)
    {
        unsigned int ulValue ;
        QString strV = "/CM/" + QString::number(iLoop);
        
        ulValue = config->value(strV,INVALID_CONFIG_VALUE).toInt();
        Param.aulCms[iLoop] = ulValue;
    }    


    if (INVALID_CONFIG_VALUE == Param.aulCms[DISP_PRE_PACKLIFEDAY])
    {
        Param.aulCms[DISP_PRE_PACKLIFEDAY] = 90; 
    }
    if (INVALID_CONFIG_VALUE == Param.aulCms[DISP_PRE_PACKLIFEL])
    {
        Param.aulCms[DISP_PRE_PACKLIFEL] = 10000; // 
    }
    //2018.10.22 ADD AC PACK
    if (INVALID_CONFIG_VALUE == Param.aulCms[DISP_AC_PACKLIFEDAY])
    {
        Param.aulCms[DISP_AC_PACKLIFEDAY] = 180;
    }
    if (INVALID_CONFIG_VALUE == Param.aulCms[DISP_AC_PACKLIFEL])
    {
        Param.aulCms[DISP_AC_PACKLIFEL] = 30000; //
    }
    //2018.10.12 add T-Pack
    if (INVALID_CONFIG_VALUE == Param.aulCms[DISP_T_PACKLIFEDAY])
    {
        Param.aulCms[DISP_T_PACKLIFEDAY] = 360;
    }
    if (INVALID_CONFIG_VALUE == Param.aulCms[DISP_T_PACKLIFEL])
    {
        Param.aulCms[DISP_T_PACKLIFEL] = 10000; //
    }

    if (INVALID_CONFIG_VALUE == Param.aulCms[DISP_P_PACKLIFEDAY])
    {
        Param.aulCms[DISP_P_PACKLIFEDAY] = 180;
    }
    if (INVALID_CONFIG_VALUE == Param.aulCms[DISP_P_PACKLIFEL])
    {
        Param.aulCms[DISP_P_PACKLIFEL] = 30000; //
    }

    if (INVALID_CONFIG_VALUE == Param.aulCms[DISP_U_PACKLIFEDAY])
    {
        Param.aulCms[DISP_U_PACKLIFEDAY] = 360; 
    }
    
    if (INVALID_CONFIG_VALUE == Param.aulCms[DISP_U_PACKLIFEL])
    {
        if(iMachineType == MACHINE_ADAPT)
        {
            Param.aulCms[DISP_U_PACKLIFEL] = 1000; //
        }
        else
        {
            Param.aulCms[DISP_U_PACKLIFEL] = 3000; //
        }
    }

    if (INVALID_CONFIG_VALUE == Param.aulCms[DISP_AT_PACKLIFEDAY])
    {
        Param.aulCms[DISP_AT_PACKLIFEDAY] = 360; 
    }
    
    if (INVALID_CONFIG_VALUE == Param.aulCms[DISP_AT_PACKLIFEL])
    {
        Param.aulCms[DISP_AT_PACKLIFEL] = 6000; // 
    }

    if (INVALID_CONFIG_VALUE == Param.aulCms[DISP_H_PACKLIFEDAY])
    {
        Param.aulCms[DISP_H_PACKLIFEDAY] = 360; 
    }
    
    if (INVALID_CONFIG_VALUE == Param.aulCms[DISP_H_PACKLIFEL])
    {
        Param.aulCms[DISP_H_PACKLIFEL] = 3000; //
    }


    if (INVALID_CONFIG_VALUE == Param.aulCms[DISP_N1_UVLIFEDAY])
    {
        Param.aulCms[DISP_N1_UVLIFEDAY] = 720; // 
    }
    if (INVALID_CONFIG_VALUE == Param.aulCms[DISP_N1_UVLIFEHOUR])
    {
        Param.aulCms[DISP_N1_UVLIFEHOUR] = 1500; //
    }

    if (INVALID_CONFIG_VALUE == Param.aulCms[DISP_N2_UVLIFEDAY])
    {
        Param.aulCms[DISP_N2_UVLIFEDAY] = 720; // 
    }
    if (INVALID_CONFIG_VALUE == Param.aulCms[DISP_N2_UVLIFEHOUR])
    {
        Param.aulCms[DISP_N2_UVLIFEHOUR] = 500; //
    }

    if (INVALID_CONFIG_VALUE == Param.aulCms[DISP_N3_UVLIFEDAY])
    {
        Param.aulCms[DISP_N3_UVLIFEDAY] = 720; // 
    }
    if (INVALID_CONFIG_VALUE == Param.aulCms[DISP_N3_UVLIFEHOUR])
    {
        Param.aulCms[DISP_N3_UVLIFEHOUR] = 1500; //
    }

    if (INVALID_CONFIG_VALUE == Param.aulCms[DISP_N4_UVLIFEDAY])
    {
        Param.aulCms[DISP_N4_UVLIFEDAY] = 720; // 
    }
    if (INVALID_CONFIG_VALUE == Param.aulCms[DISP_N4_UVLIFEHOUR])
    {
        Param.aulCms[DISP_N4_UVLIFEHOUR] = 8000; // 
    }

    if (INVALID_CONFIG_VALUE == Param.aulCms[DISP_N5_UVLIFEDAY])
    {
        Param.aulCms[DISP_N5_UVLIFEDAY] = 720; // 
    }
    if (INVALID_CONFIG_VALUE == Param.aulCms[DISP_N5_UVLIFEHOUR])
    {
        Param.aulCms[DISP_N5_UVLIFEHOUR] = 8000; // 
    }
    if (INVALID_CONFIG_VALUE == Param.aulCms[DISP_W_FILTERLIFE])
    {
        if(iMachineType == MACHINE_PURIST)
        {
            Param.aulCms[DISP_W_FILTERLIFE] = 0; //
        }
        else
        {
            Param.aulCms[DISP_W_FILTERLIFE] = 360; //
        }
    }

    if (INVALID_CONFIG_VALUE == Param.aulCms[DISP_T_B_FILTERLIFE])
    {
        Param.aulCms[DISP_T_B_FILTERLIFE] = 360; //
    }
    if (INVALID_CONFIG_VALUE == Param.aulCms[DISP_T_A_FILTERLIFE])
    {
        Param.aulCms[DISP_T_A_FILTERLIFE] = 360; //
    }    
    if (INVALID_CONFIG_VALUE == Param.aulCms[DISP_TUBE_FILTERLIFE])
    {
        Param.aulCms[DISP_TUBE_FILTERLIFE] = 180; // 
    }
    if (INVALID_CONFIG_VALUE == Param.aulCms[DISP_TUBE_DI_LIFE])
    {
        Param.aulCms[DISP_TUBE_DI_LIFE] = 180; // 
    }
    if (INVALID_CONFIG_VALUE == Param.aulCms[DISP_ROC12LIFEDAY])
    {
        Param.aulCms[DISP_ROC12LIFEDAY] = 360; //
    }
    
    if (config)
    {
        delete config;
        config = NULL;
    }
}

void MainRetriveFmParam(int iMachineType,DISP_FM_SETTING_STRU  &Param)
{
    /* retrive parameter from configuration */
    int iLoop;

    QString strCfgName = gaMachineType[iMachineType].strName;

    strCfgName += ".ini";

    QSettings *config = new QSettings(strCfgName, QSettings::IniFormat);

    for(iLoop = 0; iLoop < DISP_FM_NUM ; iLoop++)
    {
        QString strV = "/FM/";

        strV += QString::number(iLoop);

        int iValue = config->value(strV,gaMachineType[iMachineType].iDefaultFmPulse).toInt();
        
        Param.aulCfg[iLoop] = iValue;

        qDebug() << strV << ":" << Param.aulCfg[iLoop];
        
    }    
    
    if (config)
    {
        delete config;
        config = NULL;
    }
}


void MainRetrivePmParam(int iMachineType,DISP_PM_SETTING_STRU  &Param)
{
    /* retrive parameter from configuration */
    int iLoop;

    QString strCfgName = gaMachineType[iMachineType].strName;

    strCfgName += ".ini";

    QSettings *config = new QSettings(strCfgName, QSettings::IniFormat);

    for(iLoop = 0; iLoop < DISP_PM_NUM ; iLoop++)
    {
        QString strV = "/PM/";
        float fValue;
        int   iValue;

        strV += QString::number(iLoop);

        switch(iLoop)
        {
        case DISP_PM_PM1:
            iValue = config->value(strV + "/TYPE",0).toInt(); 
    
            Param.aiBuckType[iLoop] = iValue;
    
            fValue = config->value(strV + "/CAP",1.6).toFloat(); 
            
            Param.afCap[iLoop] = fValue;
    
            fValue = config->value(strV + "/RANGE",1.6).toFloat(); 
            
            Param.afDepth[iLoop] = fValue;                  
            break;
        default:
            iValue = config->value(strV + "/TYPE",0).toInt(); //  type for bucket
    
            Param.aiBuckType[iLoop] = iValue;
    
            fValue = config->value(strV + "/CAP",30).toFloat(); //  LITRE for bucket
            
            Param.afCap[iLoop] = fValue;
    
            fValue = config->value(strV + "/RANGE",0.3).toFloat(); // max 100mm  default:0.3
            
            Param.afDepth[iLoop] = fValue;            
            break;
        }

        qDebug() << strV << ":" << Param.aiBuckType[iLoop]  << Param.afDepth[iLoop] << Param.afCap[iLoop];        
        
    }    
    
    if (config)
    {
        delete config;
        config = NULL;
    }
}



void MainRetriveCalParam(int iMachineType,DISP_CAL_SETTING_STRU  &Param)
{
    /* retrive parameter from configuration */
    int iLoop;

    QString strCfgName = gaMachineType[iMachineType].strName;

    strCfgName += ".ini";

    QSettings *config = new QSettings(strCfgName, QSettings::IniFormat);

    for(iLoop = 0; iLoop < DISP_CAL_NUM ; iLoop++)
    {
        QString strV = "/CAL/";

        strV += QString::number(iLoop);

        int iValue = config->value(strV,5).toFloat(); // max 200mm
        
        Param.afCfg[iLoop] = iValue;
    }    
    
    if (config)
    {
        delete config;
        config = NULL;
    }
}

void MainRetriveCalibrateParam(int iMachineType,DISP_PARAM_CALI_STRU  &Param)
{
    /* retrive parameter from configuration */
    int iLoop;

    QString strCfgName = gaMachineType[iMachineType].strName;

    strCfgName += ".ini";

    QSettings *config = new QSettings(strCfgName, QSettings::IniFormat);

    for(iLoop = 0; iLoop < DISP_PC_COFF_NUM ; iLoop++)
    {
        QString strV = "/PCCOFF/";
        float fValue ;

        strV += QString::number(iLoop);

        QString strV1 = "/K" + strV;
        fValue = config->value(strV1,1).toFloat(); // max 200mm
        Param.pc[iLoop].fk = fValue;

        QString strV2 = "/C" + strV;
        fValue = config->value(strV2,1).toFloat(); // max 200mm
        Param.pc[iLoop].fc = fValue;

        QString strV3 = "/V" + strV;
        fValue = config->value(strV3,1).toFloat(); // max 200mm
        Param.pc[iLoop].fv = fValue;
    }

    //
    switch(iMachineType)
    {
    case MACHINE_L_Genie:
    {
        for(iLoop = 0; iLoop < DISP_PC_COFF_NUM ; iLoop++)
        {
            ex_global_Cali.pc[iLoop].fk = Param.pc[iLoop].fk;
        }
        break;
    }
    case MACHINE_L_UP:
    {
        ex_global_Cali.pc[DISP_PC_COFF_SOURCE_WATER_CONDUCT].fk = Param.pc[0].fk;
        ex_global_Cali.pc[DISP_PC_COFF_SOURCE_WATER_TEMP].fk = Param.pc[1].fk;
        ex_global_Cali.pc[DISP_PC_COFF_RO_WATER_CONDUCT].fk = Param.pc[2].fk;
        ex_global_Cali.pc[DISP_PC_COFF_RO_WATER_TEMP].fk = Param.pc[3].fk;
        ex_global_Cali.pc[DISP_PC_COFF_UP_WATER_CONDUCT].fk = Param.pc[4].fk;
        ex_global_Cali.pc[DISP_PC_COFF_UP_WATER_TEMP].fk = Param.pc[5].fk;
        ex_global_Cali.pc[DISP_PC_COFF_TOC_WATER_CONDUCT].fk = Param.pc[6].fk;
        ex_global_Cali.pc[DISP_PC_COFF_TOC_WATER_TEMP].fk = Param.pc[7].fk;
        ex_global_Cali.pc[DISP_PC_COFF_S1].fk = Param.pc[8].fk;
        ex_global_Cali.pc[DISP_PC_COFF_S1].fc = 0;
        ex_global_Cali.pc[DISP_PC_COFF_S2 ].fk = Param.pc[9].fk;
        ex_global_Cali.pc[DISP_PC_COFF_S2].fc = 0;
        ex_global_Cali.pc[DISP_PC_COFF_S3].fk = Param.pc[10].fk;
        ex_global_Cali.pc[DISP_PC_COFF_S3].fc = 0;
        ex_global_Cali.pc[DISP_PC_COFF_S4].fk = Param.pc[11].fk;
        ex_global_Cali.pc[DISP_PC_COFF_S4].fc = 0;
        ex_global_Cali.pc[DISP_PC_COFF_PW_TANK_LEVEL].fk = Param.pc[12].fk;
        ex_global_Cali.pc[DISP_PC_COFF_SW_TANK_LEVEL].fk = Param.pc[13].fk;
        ex_global_Cali.pc[DISP_PC_COFF_SYS_PRESSURE].fk = Param.pc[14].fk;
        break;
    }
    case MACHINE_L_EDI_LOOP:
    {
        ex_global_Cali.pc[DISP_PC_COFF_SOURCE_WATER_CONDUCT].fk = Param.pc[0].fk;
        ex_global_Cali.pc[DISP_PC_COFF_SOURCE_WATER_TEMP].fk = Param.pc[1].fk;
        ex_global_Cali.pc[DISP_PC_COFF_RO_WATER_CONDUCT].fk = Param.pc[2].fk;
        ex_global_Cali.pc[DISP_PC_COFF_RO_WATER_TEMP].fk = Param.pc[3].fk;
        ex_global_Cali.pc[DISP_PC_COFF_EDI_WATER_CONDUCT].fk = Param.pc[4].fk;
        ex_global_Cali.pc[DISP_PC_COFF_EDI_WATER_TEMP].fk = Param.pc[5].fk;
        ex_global_Cali.pc[DISP_PC_COFF_TOC_WATER_CONDUCT].fk = Param.pc[6].fk;
        ex_global_Cali.pc[DISP_PC_COFF_TOC_WATER_TEMP].fk = Param.pc[7].fk;
        ex_global_Cali.pc[DISP_PC_COFF_S1].fk = Param.pc[8].fk;
        ex_global_Cali.pc[DISP_PC_COFF_S1].fc = 0;
        ex_global_Cali.pc[DISP_PC_COFF_S2 ].fk = Param.pc[9].fk;
        ex_global_Cali.pc[DISP_PC_COFF_S2 ].fc = 0;
        ex_global_Cali.pc[DISP_PC_COFF_S3].fk = Param.pc[10].fk;
        ex_global_Cali.pc[DISP_PC_COFF_S3].fc = 0;
        ex_global_Cali.pc[DISP_PC_COFF_S4].fk = Param.pc[11].fk;
        ex_global_Cali.pc[DISP_PC_COFF_S4].fc = 0;
        ex_global_Cali.pc[DISP_PC_COFF_PW_TANK_LEVEL].fk = Param.pc[12].fk;
        ex_global_Cali.pc[DISP_PC_COFF_SW_TANK_LEVEL].fk = Param.pc[13].fk;
        ex_global_Cali.pc[DISP_PC_COFF_SYS_PRESSURE].fk = Param.pc[14].fk;
        break;
    }
    case MACHINE_L_RO_LOOP:
    {
        ex_global_Cali.pc[DISP_PC_COFF_SOURCE_WATER_CONDUCT].fk = Param.pc[0].fk;
        ex_global_Cali.pc[DISP_PC_COFF_SOURCE_WATER_TEMP].fk = Param.pc[1].fk;
        ex_global_Cali.pc[DISP_PC_COFF_RO_WATER_CONDUCT].fk = Param.pc[2].fk;
        ex_global_Cali.pc[DISP_PC_COFF_RO_WATER_TEMP].fk = Param.pc[3].fk;
        ex_global_Cali.pc[DISP_PC_COFF_S1].fk = Param.pc[4].fk;
        ex_global_Cali.pc[DISP_PC_COFF_S2 ].fk = Param.pc[5].fk;
        ex_global_Cali.pc[DISP_PC_COFF_S3].fk = Param.pc[6].fk;
        ex_global_Cali.pc[DISP_PC_COFF_S4].fk = Param.pc[7].fk;
        ex_global_Cali.pc[DISP_PC_COFF_S1].fc = 0;
        ex_global_Cali.pc[DISP_PC_COFF_S2 ].fc = 0;
        ex_global_Cali.pc[DISP_PC_COFF_S3].fc = 0;
        ex_global_Cali.pc[DISP_PC_COFF_S4].fc = 0;
        ex_global_Cali.pc[DISP_PC_COFF_PW_TANK_LEVEL].fk = Param.pc[8].fk;
        ex_global_Cali.pc[DISP_PC_COFF_SW_TANK_LEVEL].fk = Param.pc[9].fk;
        ex_global_Cali.pc[DISP_PC_COFF_SYS_PRESSURE].fk = Param.pc[10].fk;
        break;
    }
    case MACHINE_Genie:
    {
        ex_global_Cali.pc[DISP_PC_COFF_SOURCE_WATER_CONDUCT].fk = Param.pc[0].fk;
        ex_global_Cali.pc[DISP_PC_COFF_SOURCE_WATER_TEMP].fk = Param.pc[1].fk;
        ex_global_Cali.pc[DISP_PC_COFF_RO_WATER_CONDUCT].fk = Param.pc[2].fk;
        ex_global_Cali.pc[DISP_PC_COFF_RO_WATER_TEMP].fk = Param.pc[3].fk;
        ex_global_Cali.pc[DISP_PC_COFF_EDI_WATER_CONDUCT].fk = Param.pc[4].fk;
        ex_global_Cali.pc[DISP_PC_COFF_EDI_WATER_TEMP].fk = Param.pc[5].fk;
        ex_global_Cali.pc[DISP_PC_COFF_UP_WATER_CONDUCT].fk = Param.pc[6].fk;
        ex_global_Cali.pc[DISP_PC_COFF_UP_WATER_TEMP].fk = Param.pc[7].fk;
        ex_global_Cali.pc[DISP_PC_COFF_TOC_WATER_CONDUCT].fk = Param.pc[8].fk;
        ex_global_Cali.pc[DISP_PC_COFF_TOC_WATER_TEMP].fk = Param.pc[9].fk;
        ex_global_Cali.pc[DISP_PC_COFF_S1].fk = Param.pc[10].fk;
        ex_global_Cali.pc[DISP_PC_COFF_S1].fc = 0;
        ex_global_Cali.pc[DISP_PC_COFF_PW_TANK_LEVEL].fk = Param.pc[11].fk;
        ex_global_Cali.pc[DISP_PC_COFF_SYS_PRESSURE].fk = Param.pc[12].fk;
        break;
    }
    case MACHINE_UP:
    {
        ex_global_Cali.pc[DISP_PC_COFF_SOURCE_WATER_CONDUCT].fk = Param.pc[0].fk;
        ex_global_Cali.pc[DISP_PC_COFF_SOURCE_WATER_TEMP].fk = Param.pc[1].fk;
        ex_global_Cali.pc[DISP_PC_COFF_RO_WATER_CONDUCT].fk = Param.pc[2].fk;
        ex_global_Cali.pc[DISP_PC_COFF_RO_WATER_TEMP].fk = Param.pc[3].fk;
        ex_global_Cali.pc[DISP_PC_COFF_EDI_WATER_CONDUCT].fk = Param.pc[4].fk;
        ex_global_Cali.pc[DISP_PC_COFF_EDI_WATER_TEMP].fk = Param.pc[5].fk;
        ex_global_Cali.pc[DISP_PC_COFF_UP_WATER_CONDUCT].fk = Param.pc[6].fk;
        ex_global_Cali.pc[DISP_PC_COFF_UP_WATER_TEMP].fk = Param.pc[7].fk;
        ex_global_Cali.pc[DISP_PC_COFF_TOC_WATER_CONDUCT].fk = Param.pc[8].fk;
        ex_global_Cali.pc[DISP_PC_COFF_TOC_WATER_TEMP].fk = Param.pc[9].fk;
        ex_global_Cali.pc[DISP_PC_COFF_S1].fk = Param.pc[10].fk;
        ex_global_Cali.pc[DISP_PC_COFF_S1].fc = 0;
        ex_global_Cali.pc[DISP_PC_COFF_PW_TANK_LEVEL].fk = Param.pc[11].fk;
        ex_global_Cali.pc[DISP_PC_COFF_SYS_PRESSURE].fk = Param.pc[12].fk;
        break;
    }
    case MACHINE_EDI:
    {
        ex_global_Cali.pc[DISP_PC_COFF_SOURCE_WATER_CONDUCT].fk = Param.pc[0].fk;
        ex_global_Cali.pc[DISP_PC_COFF_SOURCE_WATER_TEMP].fk = Param.pc[1].fk;
        ex_global_Cali.pc[DISP_PC_COFF_RO_WATER_CONDUCT].fk = Param.pc[2].fk;
        ex_global_Cali.pc[DISP_PC_COFF_RO_WATER_TEMP].fk = Param.pc[3].fk;
        ex_global_Cali.pc[DISP_PC_COFF_EDI_WATER_CONDUCT].fk = Param.pc[4].fk;
        ex_global_Cali.pc[DISP_PC_COFF_EDI_WATER_TEMP].fk = Param.pc[5].fk;
        ex_global_Cali.pc[DISP_PC_COFF_TOC_WATER_CONDUCT].fk = Param.pc[6].fk;
        ex_global_Cali.pc[DISP_PC_COFF_TOC_WATER_TEMP].fk = Param.pc[7].fk;
        ex_global_Cali.pc[DISP_PC_COFF_S1].fk = Param.pc[8].fk;
        ex_global_Cali.pc[DISP_PC_COFF_S1].fc = 0;
        ex_global_Cali.pc[DISP_PC_COFF_PW_TANK_LEVEL].fk = Param.pc[9].fk;
        ex_global_Cali.pc[DISP_PC_COFF_SYS_PRESSURE].fk = Param.pc[10].fk;
        break;
    }
    case MACHINE_RO:
    {
        ex_global_Cali.pc[DISP_PC_COFF_SOURCE_WATER_CONDUCT].fk = Param.pc[0].fk;
        ex_global_Cali.pc[DISP_PC_COFF_SOURCE_WATER_TEMP].fk = Param.pc[1].fk;
        ex_global_Cali.pc[DISP_PC_COFF_RO_WATER_CONDUCT].fk = Param.pc[2].fk;
        ex_global_Cali.pc[DISP_PC_COFF_RO_WATER_TEMP].fk = Param.pc[3].fk;
        ex_global_Cali.pc[DISP_PC_COFF_EDI_WATER_CONDUCT].fk = Param.pc[4].fk;
        ex_global_Cali.pc[DISP_PC_COFF_EDI_WATER_TEMP].fk = Param.pc[5].fk;
        ex_global_Cali.pc[DISP_PC_COFF_S1].fk = Param.pc[6].fk;
        ex_global_Cali.pc[DISP_PC_COFF_S1].fc = 0;
        ex_global_Cali.pc[DISP_PC_COFF_PW_TANK_LEVEL].fk = Param.pc[7].fk;
        ex_global_Cali.pc[DISP_PC_COFF_SYS_PRESSURE].fk = Param.pc[8].fk;
        break;
    }
    case MACHINE_PURIST:
    {
        ex_global_Cali.pc[DISP_PC_COFF_RO_WATER_CONDUCT].fk = Param.pc[0].fk;
        ex_global_Cali.pc[DISP_PC_COFF_RO_WATER_TEMP].fk = Param.pc[1].fk;
        ex_global_Cali.pc[DISP_PC_COFF_UP_WATER_CONDUCT].fk = Param.pc[2].fk;
        ex_global_Cali.pc[DISP_PC_COFF_UP_WATER_TEMP].fk = Param.pc[3].fk;
        ex_global_Cali.pc[DISP_PC_COFF_TOC_WATER_CONDUCT].fk = Param.pc[4].fk;
        ex_global_Cali.pc[DISP_PC_COFF_TOC_WATER_TEMP].fk = Param.pc[5].fk;
        ex_global_Cali.pc[DISP_PC_COFF_S1].fk = Param.pc[6].fk;
        ex_global_Cali.pc[DISP_PC_COFF_S1].fc = 0;
        ex_global_Cali.pc[DISP_PC_COFF_PW_TANK_LEVEL].fk = Param.pc[7].fk;
        ex_global_Cali.pc[DISP_PC_COFF_SYS_PRESSURE].fk = Param.pc[8].fk;
        break;
    }
    case MACHINE_ADAPT:
    {
        ex_global_Cali.pc[DISP_PC_COFF_SOURCE_WATER_CONDUCT].fk = Param.pc[0].fk;
        ex_global_Cali.pc[DISP_PC_COFF_SOURCE_WATER_TEMP].fk = Param.pc[1].fk;
        ex_global_Cali.pc[DISP_PC_COFF_RO_WATER_CONDUCT].fk = Param.pc[2].fk;
        ex_global_Cali.pc[DISP_PC_COFF_RO_WATER_TEMP].fk = Param.pc[3].fk;
        ex_global_Cali.pc[DISP_PC_COFF_UP_WATER_CONDUCT].fk = Param.pc[4].fk;
        ex_global_Cali.pc[DISP_PC_COFF_UP_WATER_TEMP].fk = Param.pc[5].fk;
        ex_global_Cali.pc[DISP_PC_COFF_TOC_WATER_CONDUCT].fk = Param.pc[6].fk;
        ex_global_Cali.pc[DISP_PC_COFF_TOC_WATER_TEMP].fk = Param.pc[7].fk;
        ex_global_Cali.pc[DISP_PC_COFF_S1].fk = Param.pc[8].fk;
        ex_global_Cali.pc[DISP_PC_COFF_S1].fc = 0;
        ex_global_Cali.pc[DISP_PC_COFF_SYS_PRESSURE].fk = Param.pc[9].fk;
        break;
    }
    default:
        break;
    }
    
    if (config)
    {
        delete config;
        config = NULL;
    }
}

void MainRetriveMiscParam(int iMachineType,DISP_MISC_SETTING_STRU  &Param)
{
    /* retrive parameter from configuration */
    // int iLoop;

    QString strCfgName = gaMachineType[iMachineType].strName;

    strCfgName += ".ini";

    QSettings *config = new QSettings(strCfgName, QSettings::IniFormat);

    {
        QString strV ;
        int iValue;

        strV = "/MISC/FLAG";
        iValue = config->value(strV,0).toInt();
        Param.ulAllocMask = iValue;

        strV = "/MISC/ICP";
        iValue = config->value(strV,60).toInt(); /* UNIT minute */
        Param.iTubeCirCycle = iValue;        

        strV = "/MISC/DT";
        iValue = config->value(strV,1).toInt(); /* UNIT minute */
        Param.iTubeCirDuration = iValue;        

        strV = "/MISC/ST";
        iValue = config->value(strV,0).toInt();
        Param.aiTubeCirTimeInfo[0] = iValue;        

        strV = "/MISC/ET";
        iValue = config->value(strV,0).toInt();
        Param.aiTubeCirTimeInfo[1] = iValue;        

        strV = "/MISC/RP0";
        iValue = config->value(strV,0).toInt();
        Param.RPumpSetting[0] = iValue;   
        
        strV = "/MISC/RP1";
        iValue = config->value(strV,0).toInt();
        Param.RPumpSetting[1] = iValue;        

        strV = "/MISC/LAN";
        iValue = config->value(strV,0).toInt();
        Param.iLan = iValue;        
        qDebug() << strV << ":" << iValue;        

        strV = "/MISC/BRIGHTNESS";
        iValue = config->value(strV,100).toInt();
        Param.iBrightness = iValue;        

        strV = "/MISC/ENERGYSAVE";
        iValue = config->value(strV,1).toInt();
        Param.iEnerySave = iValue;
    
        strV = "/MISC/CONDUCTIVITYUNIT";
        iValue = config->value(strV,0).toInt();
        Param.iUint4Conductivity = iValue;        
        qDebug() << strV << ":" << iValue;        

        strV = "/MISC/TEMPUNIT";
        iValue = config->value(strV,0).toInt();
        Param.iUint4Temperature = iValue;      
        qDebug() << strV << ":" << iValue;        
        
        strV = "/MISC/PRESSUREUNIT";
        iValue = config->value(strV,0).toInt();
        Param.iUint4Pressure = iValue;        
        qDebug() << strV << ":" << iValue;        

        strV = "/MISC/LIQUIDUNIT";
        iValue = config->value(strV,0).toInt();
        Param.iUint4LiquidLevel = iValue;        
        qDebug() << strV << ":" << iValue;        

        strV = "/MISC/FLOWVELOCITYUNIT";
        iValue = config->value(strV,0).toInt();
        Param.iUint4FlowVelocity = iValue;        
        qDebug() << strV << ":" << iValue;        

        strV = "/MISC/SOUNDMASK";
        iValue = config->value(strV,0).toInt();
        Param.iSoundMask = iValue;  
        
        strV = "/MISC/NETWORKMASK";
        iValue = config->value(strV,1).toInt(); //0629 1->3
        Param.iNetworkMask = iValue;        

        strV = "/MISC/MISCFLAG";
        iValue = config->value(strV,1).toInt();
        Param.ulMisFlags = iValue;        

        
        strV = "/MISC/TANKUVONTIME";
        iValue = config->value(strV,5).toInt();
        Param.iTankUvOnTime = iValue;        
        
        strV = "/MISC/AUTOLOGOUTTIME";
        iValue = config->value(strV,30).toInt();
        Param.iAutoLogoutTime = iValue;  
        
        strV = "/MISC/POWERONFLUSHTIME";
        iValue = config->value(strV,5).toInt(); //5
        Param.iPowerOnFlushTime = iValue;  

    }    
    
    if (config)
    {
        delete config;
        config = NULL;
    }
}


void MainRetriveCMInfo(int iMachineType,DISP_CONSUME_MATERIAL_STRU  &Param)
{
    /* retrive parameter from configuration */
    int iLoop;

    QString strCfgName = gaMachineType[iMachineType].strName;

    strCfgName += "_info.ini";
    
    QSettings *config = new QSettings(strCfgName, QSettings::IniFormat);

    for (iLoop = 0; iLoop < DISP_CM_NUM; iLoop++)
    {
        unsigned int ulValue ;
        QString strV = "/CM/" + QString::number(iLoop);
        
        ulValue = config->value(strV,INVALID_CONFIG_VALUE).toInt();
        Param.aulCms[iLoop] = ulValue;
    }    

    if (INVALID_CONFIG_VALUE == Param.aulCms[DISP_PRE_PACKLIFEDAY])
    {
        Param.aulCms[DISP_PRE_PACKLIFEDAY] = DispGetCurSecond(); // NEW pack
    }
    
    if (INVALID_CONFIG_VALUE == Param.aulCms[DISP_PRE_PACKLIFEL])
    {
        Param.aulCms[DISP_PRE_PACKLIFEL] = 0; // NEW pack
    }
    //2018.10.22 AC PACK
    if (INVALID_CONFIG_VALUE == Param.aulCms[DISP_AC_PACKLIFEDAY])
    {
        Param.aulCms[DISP_AC_PACKLIFEDAY] = DispGetCurSecond(); // NEW pack
    }

    if (INVALID_CONFIG_VALUE == Param.aulCms[DISP_AC_PACKLIFEL])
    {
        Param.aulCms[DISP_AC_PACKLIFEL] = 0; // NEW pack
    }

    //2018.10.12 T-Pack
    if (INVALID_CONFIG_VALUE == Param.aulCms[DISP_T_PACKLIFEDAY])
    {
        Param.aulCms[DISP_T_PACKLIFEDAY] = DispGetCurSecond(); // NEW pack
    }

    if (INVALID_CONFIG_VALUE == Param.aulCms[DISP_T_PACKLIFEL])
    {
        Param.aulCms[DISP_T_PACKLIFEL] = 0; // NEW pack
    }

    if (INVALID_CONFIG_VALUE == Param.aulCms[DISP_P_PACKLIFEDAY])
    {
        Param.aulCms[DISP_P_PACKLIFEDAY] = DispGetCurSecond(); // NEW pack
    }
    
    if (INVALID_CONFIG_VALUE == Param.aulCms[DISP_P_PACKLIFEL])
    {
        Param.aulCms[DISP_P_PACKLIFEL] = 0; // NEW pack
    }

    if (INVALID_CONFIG_VALUE == Param.aulCms[DISP_U_PACKLIFEDAY])
    {
        Param.aulCms[DISP_U_PACKLIFEDAY] = DispGetCurSecond(); // NEW pack
    }

    if (INVALID_CONFIG_VALUE == Param.aulCms[DISP_U_PACKLIFEL])
    {
        Param.aulCms[DISP_U_PACKLIFEL] = 0; // NEW pack
    }
    
    if (INVALID_CONFIG_VALUE == Param.aulCms[DISP_AT_PACKLIFEDAY])
    {
        Param.aulCms[DISP_AT_PACKLIFEDAY] = DispGetCurSecond(); // NEW pack
    }
    
    if (INVALID_CONFIG_VALUE == Param.aulCms[DISP_AT_PACKLIFEL])
    {
        Param.aulCms[DISP_AT_PACKLIFEL] = 0; // NEW pack
    }

    if (INVALID_CONFIG_VALUE == Param.aulCms[DISP_H_PACKLIFEDAY])
    {
        Param.aulCms[DISP_H_PACKLIFEDAY] = DispGetCurSecond(); // NEW pack
    }
    
    if (INVALID_CONFIG_VALUE == Param.aulCms[DISP_H_PACKLIFEL])
    {
        Param.aulCms[DISP_H_PACKLIFEL] = 0; // NEW pack
    }

    if (INVALID_CONFIG_VALUE == Param.aulCms[DISP_N1_UVLIFEDAY])
    {
        Param.aulCms[DISP_N1_UVLIFEDAY] = DispGetCurSecond(); // NEW uv
    }

    if (INVALID_CONFIG_VALUE == Param.aulCms[DISP_N1_UVLIFEHOUR])
    {
        Param.aulCms[DISP_N1_UVLIFEHOUR] = 0; // NEW uv
    }    

    if (INVALID_CONFIG_VALUE == Param.aulCms[DISP_N2_UVLIFEDAY])
    {
        Param.aulCms[DISP_N2_UVLIFEDAY] = DispGetCurSecond(); // NEW uv
    }

    if (INVALID_CONFIG_VALUE == Param.aulCms[DISP_N2_UVLIFEHOUR])
    {
        Param.aulCms[DISP_N2_UVLIFEHOUR] = 0; // NEW uv
    }    

    if (INVALID_CONFIG_VALUE == Param.aulCms[DISP_N3_UVLIFEDAY])
    {
        Param.aulCms[DISP_N3_UVLIFEDAY] = DispGetCurSecond(); // NEW uv
    }

    if (INVALID_CONFIG_VALUE == Param.aulCms[DISP_N3_UVLIFEHOUR])
    {
        Param.aulCms[DISP_N3_UVLIFEHOUR] = 0; // NEW uv
    }    

    if (INVALID_CONFIG_VALUE == Param.aulCms[DISP_N4_UVLIFEDAY])
    {
        Param.aulCms[DISP_N4_UVLIFEDAY] = DispGetCurSecond(); // NEW uv
    }

    if (INVALID_CONFIG_VALUE == Param.aulCms[DISP_N4_UVLIFEHOUR])
    {
        Param.aulCms[DISP_N4_UVLIFEHOUR] = 0; // NEW uv
    }    
    
    if (INVALID_CONFIG_VALUE == Param.aulCms[DISP_N5_UVLIFEDAY])
    {
        Param.aulCms[DISP_N5_UVLIFEDAY] = DispGetCurSecond(); // NEW uv
    }

    if (INVALID_CONFIG_VALUE == Param.aulCms[DISP_N5_UVLIFEHOUR])
    {
        Param.aulCms[DISP_N5_UVLIFEHOUR] = 0; // NEW uv
    }    

    if (INVALID_CONFIG_VALUE == Param.aulCms[DISP_T_B_FILTERLIFE])
    {
//        Param.aulCms[DISP_T_B_FILTERLIFE] = 0; // NEW pack
        Param.aulCms[DISP_T_B_FILTERLIFE] = DispGetCurSecond(); //
    }
    
    if (INVALID_CONFIG_VALUE == Param.aulCms[DISP_W_FILTERLIFE])
    {
//        Param.aulCms[DISP_W_FILTERLIFE] = 0; // NEW pack
        Param.aulCms[DISP_W_FILTERLIFE] = DispGetCurSecond(); //
    }
    
    if (INVALID_CONFIG_VALUE == Param.aulCms[DISP_T_A_FILTERLIFE])
    {
//        Param.aulCms[DISP_T_A_FILTERLIFE] = 0; // NEW pack
        Param.aulCms[DISP_T_A_FILTERLIFE] = DispGetCurSecond(); //
    }

    if (INVALID_CONFIG_VALUE == Param.aulCms[DISP_TUBE_FILTERLIFE])
    {
//        Param.aulCms[DISP_TUBE_FILTERLIFE] = 0; // NEW pack
        Param.aulCms[DISP_TUBE_FILTERLIFE] = DispGetCurSecond(); //
    }
    
    if (INVALID_CONFIG_VALUE == Param.aulCms[DISP_TUBE_DI_LIFE])
    {
//        Param.aulCms[DISP_TUBE_DI_LIFE] = 0; // NEW pack
        Param.aulCms[DISP_TUBE_DI_LIFE] = DispGetCurSecond(); //
    }

    if (INVALID_CONFIG_VALUE == Param.aulCms[DISP_ROC12LIFEDAY])
    {
        Param.aulCms[DISP_ROC12LIFEDAY] = DispGetCurSecond(); // NEW uv
    }

    
    for (iLoop = 0; iLoop < DISP_CM_NUM; iLoop++)
    {
         qDebug()<<"info"<<Param.aulCms[iLoop];
    }
    
    if (config)
    {
        delete config;
        config = NULL;
    }
}



void MainRetriveCleanParam(int iMachineType,DISP_CLEAN_SETTING_STRU  &Param)
{
    /* retrive parameter from configuration */
    int iLoop;

    QString strCfgName = gaMachineType[iMachineType].strName;

    strCfgName += ".ini";

    QSettings *config = new QSettings(strCfgName, QSettings::IniFormat);

    for (iLoop = 0; iLoop < DISP_CLEAN_NUM; iLoop++)
    {
        int iValue;
        QString strV = "/CLEAN/";
        strV += QString::number(iLoop);
        int defaultValue = QDateTime::currentDateTime().toTime_t();
        iValue = config->value(strV + "/LASTIME",defaultValue).toInt();
        Param.aCleans[iLoop].lstTime = iValue;

        defaultValue = QDateTime::currentDateTime().addDays(gGlobalParam.CMParam.aulCms[DISP_ROC12LIFEDAY]).toTime_t();
        iValue = config->value(strV + "/PERIOD",defaultValue).toInt();
        Param.aCleans[iLoop].period = iValue;
    }    
    
    if (config)
    {
        delete config;
        config = NULL;
    }
}

void MainRetriveCMSn(int iMachineType,DISP_CONSUME_MATERIAL_SN_STRU  &Param)
{
    /* retrive parameter from configuration */
    int iLoop;

    QString strCfgName = gaMachineType[iMachineType].strName;

    strCfgName += ".ini";

    QSettings *config = new QSettings(strCfgName, QSettings::IniFormat);

    for (iLoop = 0; iLoop < DISP_CM_NAME_NUM; iLoop++)
    {
        QString strValue;
        QString strV = "/CMSN/";
        strV += QString::number(iLoop);

        strValue = config->value(strV + "/CAT","unknow").toString();
        memset(Param.aCn[iLoop],0,sizeof(CATNO));
        strncpy(Param.aCn[iLoop],strValue.toAscii(),APP_CAT_LENGTH);

        strValue = config->value(strV + "/LOT","unknow").toString();
        memset(Param.aLn[iLoop],0,sizeof(LOTNO));
        strncpy(Param.aLn[iLoop],strValue.toAscii(),APP_LOT_LENGTH);
    }    
    
    if (config)
    {
        delete config;
        config = NULL;
    }
}

void MainRetriveMacSn(int iMachineType,DISP_MACHINERY_SN_STRU  &Param)
{
    /* retrive parameter from configuration */
    int iLoop;

    QString strCfgName = gaMachineType[iMachineType].strName;

    strCfgName += ".ini";

    QSettings *config = new QSettings(strCfgName, QSettings::IniFormat);

    for (iLoop = 0; iLoop < DISP_MACHINERY_NAME_NUM; iLoop++)
    {
        QString strValue;
        QString strV = "/MACSN/";
        strV += QString::number(iLoop);

        strValue = config->value(strV + "/CAT","unknow").toString();
        memset(Param.aCn[iLoop],0,sizeof(CATNO));
        strncpy(Param.aCn[iLoop],strValue.toAscii(),APP_CAT_LENGTH);

        strValue = config->value(strV + "/LOT","unknow").toString();
        memset(Param.aLn[iLoop],0,sizeof(LOTNO));
        strncpy(Param.aLn[iLoop],strValue.toAscii(),APP_LOT_LENGTH);

    }    
    
    if (config)
    {
        delete config;
        config = NULL;
    }
}

void MainSaveLastRunState(int iMachineType)
{
    QString strCfgName = gaMachineType[iMachineType].strName;
    strCfgName += ".ini";

    QSettings *config = new QSettings(strCfgName, QSettings::IniFormat);
    QString strV = "/LastRunState/";
    QString strTmp = QString::number(ex_gGlobalParam.lastRunState);
    config->setValue(strV, strTmp);

    if (config)
    {
        delete config;
        config = NULL;
    }
    sync();
}

void MainSaveDefaultState(int iMachineType) //ex_dcj
{
    QString strCfgName = gaMachineType[iMachineType].strName;
    strCfgName += ".ini";

    QSettings *config = new QSettings(strCfgName, QSettings::IniFormat);
    QString strV = "/DefaultState/";
    QString strTmp = QString::number(ex_gGlobalParam.Ex_Default);
    config->setValue(strV, strTmp);

    if (config)
    {
        delete config;
        config = NULL;
    }
    sync();
}

//
void MainSaveExMachineMsg(int iMachineType)
{
    QString strCfgName = gaMachineType[iMachineType].strName;
    strCfgName += ".ini";

    QSettings *config = new QSettings(strCfgName, QSettings::IniFormat);
    QString strV;

    strV = "/ExMachineMsg/MachineFlow/";
    config->setValue(strV, ex_gGlobalParam.Ex_Machine_Msg.iMachineFlow);

    if (config)
    {
        delete config;
        config = NULL;
    }
    sync();
}

void MainSaveProductMsg(int iMachineType) //ex_dcj
{
    QString strCfgName = gaMachineType[iMachineType].strName;
    strCfgName += ".ini";

    QSettings *config = new QSettings(strCfgName, QSettings::IniFormat);
    QString strV;

    strV = "/ProductMsg/CatalogNo/";
    config->setValue(strV, ex_gGlobalParam.Ex_System_Msg.Ex_CatalogNo);

    strV = "/ProductMsg/SerialNo/";
    config->setValue(strV, ex_gGlobalParam.Ex_System_Msg.Ex_SerialNo);

    strV = "/ProductMsg/ProductionDate/";
    config->setValue(strV, ex_gGlobalParam.Ex_System_Msg.Ex_ProDate);

    strV = "/ProductMsg/SoftwareVersion/";
    config->setValue(strV, ex_gGlobalParam.Ex_System_Msg.Ex_SofeVer);

    if (config)
    {
        delete config;
        config = NULL;
    }
    sync();
}

void MainSaveInstallMsg(int iMachineType) //ex_dcj
{
    QString strCfgName = gaMachineType[iMachineType].strName;
    strCfgName += ".ini";
    QSettings *config = new QSettings(strCfgName, QSettings::IniFormat);
    QString strV;

    strV = "/InstallMsg/InstallDate/";
    config->setValue(strV, ex_gGlobalParam.Ex_System_Msg.Ex_InsDate);

    if (config)
    {
        delete config;
        config = NULL;
    }
    sync();
}

void MainSaveExConfigParam(int iMachineType)
{
    QString strCfgName = gaMachineType[iMachineType].strName;
    strCfgName += ".ini";
    QSettings *config = new QSettings(strCfgName, QSettings::IniFormat);

    QString strV;
    strV = "/ExConfigParam/ScreenSleepTime";
    config->setValue(strV, ex_gGlobalParam.Ex_Config_Param.iScreenSleepTime);

    strV = "/ExConfigParam/DispenseFlowRate";
    config->setValue(strV, ex_gGlobalParam.Ex_Config_Param.flowRate);

    if (config)
    {
        delete config;
        config = NULL;
    }
    sync();
}

void MainSaveExConsumableMsg(int iMachineType,CATNO cn,LOTNO ln,int iIndex, int category)
{
    Q_UNUSED(iMachineType);
    QSqlQuery query;
    bool ret;

    query.prepare(select_sql_Consumable);
    query.addBindValue(iIndex);
    query.addBindValue(category);
    ret = query.exec();
    qDebug() << QString("MainSaveExConsumableMsg consumable compared with Sql: %1").arg(ret);
    if(query.next())
    {
            QString lotno = query.value(0).toString();
            if(ln == lotno)
            {
                return; // do nothing
            }
            else
            {
                QString strCurDate = QDate::currentDate().toString("yyyy-MM-dd");
                QSqlQuery queryUpdate;
                queryUpdate.prepare(update_sql_Consumable);
                queryUpdate.addBindValue(cn);
                queryUpdate.addBindValue(ln);
                queryUpdate.addBindValue(strCurDate);
                queryUpdate.addBindValue(iIndex);
                queryUpdate.addBindValue(category);
                bool ret = queryUpdate.exec();
                qDebug() << QString("consumable update Sql: %1").arg(ret);
            }
    }
    else
    {
        QString strCurDate = QDate::currentDate().toString("yyyy-MM-dd");
        QSqlQuery queryInsert;
        queryInsert.prepare(insert_sql_Consumable);
        queryInsert.bindValue(":iPackType", iIndex);
        queryInsert.bindValue(":CatNo", cn);
        queryInsert.bindValue(":LotNo", ln);
        queryInsert.bindValue(":category", category);
        queryInsert.bindValue(":time", strCurDate);
        bool ret = queryInsert.exec();
        qDebug() << QString("consumable insert Sql: %1").arg(ret);
    }
    sync();
}


void MainSaveMachineType(int &iMachineType)
{
    /* retrive parameter from configuration */
    QString strValue;

    if (iMachineType != gGlobalParam.iMachineType)
    {
        QSettings *cfgGlobal = new QSettings(GLOBAL_CFG_INI, QSettings::IniFormat);
    
        if (cfgGlobal)
        {
            strValue = QString::number(iMachineType);
            
            cfgGlobal->setValue("/global/machtype",strValue);
    
            /* More come here late implement*/
            delete cfgGlobal;
        }
        
        sync();
    }
}

void MainSaveMachineParam(int iMachineType,DISP_MACHINE_PARAM_STRU  &Param)
{
    /* retrive parameter from configuration */
    int iLoop;

    DISP_MACHINE_PARAM_STRU  tmpParam;

    QString strCfgName = gaMachineType[iMachineType].strName;

    strCfgName += ".ini";

    MainRetriveMachineParam(iMachineType,tmpParam);    
    
    QSettings *config = new QSettings(strCfgName, QSettings::IniFormat);

    for(iLoop = 0; iLoop < MACHINE_PARAM_SP_NUM ; iLoop++)
    {

        if (tmpParam.SP[iLoop] != Param.SP[iLoop])
        {
            QString strV = "/SP/";
            QString strTmp;
            
            strV += QString::number(iLoop);
            strTmp = QString::number(Param.SP[iLoop]);
            
            config->setValue(strV,strTmp);
        }
    }

    if (config)
    {
        delete config;
        config = NULL;
    }

    sync();

}


void MainSaveTMParam(int iMachineType,DISP_TIME_PARAM_STRU  &Param)
{
    /* retrive parameter from configuration */
    int iLoop;

    DISP_TIME_PARAM_STRU  tmpParam;

    QString strCfgName = gaMachineType[iMachineType].strName;

    strCfgName += ".ini";

    MainRetriveTMParam(iMachineType,tmpParam);    

    QSettings *config = new QSettings(strCfgName, QSettings::IniFormat);

    for(iLoop = 0; iLoop < TIME_PARAM_NUM ; iLoop++)
    {
        QString strV = "/TM/";
        QString strTmp;

        if (tmpParam.aulTime[iLoop] != Param.aulTime[iLoop])
        {
            strV += gastrTmCfgName[iLoop];
            
            strTmp = QString::number(Param.aulTime[iLoop]);
            
            config->setValue(strV,strTmp);
        }
        
        qDebug() << strV << ":" << Param.aulTime[iLoop];

    }    
    if (config)
    {
        delete config;
        config = NULL;
    }
}

void MainSaveAlarmSetting(int iMachineType,DISP_ALARM_SETTING_STRU  &Param)
{
    /* retrive parameter from configuration */
    int iLoop;

    DISP_ALARM_SETTING_STRU  tmpParam;

    QString strCfgName = gaMachineType[iMachineType].strName;

    strCfgName += ".ini";

    MainRetriveAlarmSetting(iMachineType,tmpParam);    
    
    QSettings *config = new QSettings(strCfgName, QSettings::IniFormat);

    for(iLoop = 0; iLoop < 2 ; iLoop++)
    {
        int iValue ;
        QString strV = "/ALARM/";
        QString strTmp;
        strV += gastrAlarmCfgName[iLoop];

        if (Param.aulFlag[iLoop] != tmpParam.aulFlag[iLoop])
        {
            iValue =  Param.aulFlag[iLoop];
            
            strTmp = QString::number(iValue);
            
            config->setValue(strV,strTmp);
        }
    }
        
    if (config)
    {
        delete config;
        config = NULL;
    }
    
    sync();
}


void MainSaveSubModuleSetting(int iMachineType,DISP_SUB_MODULE_SETTING_STRU  &Param)
{
    /* retrive parameter from configuration */
    DISP_SUB_MODULE_SETTING_STRU  tmpParam;

    QString strCfgName = gaMachineType[iMachineType].strName;

    strCfgName += ".ini";

    MainRetriveSubModuleSetting(iMachineType,tmpParam);    
    
    QSettings *config = new QSettings(strCfgName, QSettings::IniFormat);

    {
        QString strV = "/SM/" + gastrSmCfgName[0];

        int iValue ;
        QString strTmp;

        if (Param.ulFlags != tmpParam.ulFlags)
        {
            iValue = Param.ulFlags;

            strTmp = QString::number(iValue);
            
            config->setValue(strV,strTmp);
            
        }


    }    
    if (config)
    {
        delete config;
        config = NULL;
    }
    
    sync();
}

void MainSaveCMParam(int iMachineType,DISP_CONSUME_MATERIAL_STRU  &Param)
{
    /* retrive parameter from configuration */
    int iLoop;
    DISP_CONSUME_MATERIAL_STRU  tmpParam;

    QString strCfgName = gaMachineType[iMachineType].strName;

    strCfgName += ".ini";

    MainRetriveCMParam(iMachineType,tmpParam);    
    
    QSettings *config = new QSettings(strCfgName, QSettings::IniFormat);

    for (iLoop = 0; iLoop < DISP_CM_NUM; iLoop++)
    {
        QString strV;
        QString strTmp;

        if (Param.aulCms[iLoop] != tmpParam.aulCms[iLoop])
        {
            strV = "/CM/" + QString::number(iLoop);
            
            strTmp = QString::number(Param.aulCms[iLoop]);
            
            config->setValue(strV,strTmp);
            
        }
    } 
    
    if (config)
    {
        delete config;
        config = NULL;
    }
    
    sync();
}



void MainSaveFMParam(int iMachineType,DISP_FM_SETTING_STRU  &Param)
{
    /* retrive parameter from configuration */
    int iLoop;

    DISP_FM_SETTING_STRU  tmpParam;

    QString strCfgName = gaMachineType[iMachineType].strName;

    strCfgName += ".ini";

    MainRetriveFmParam(iMachineType,tmpParam);    

    QSettings *config = new QSettings(strCfgName, QSettings::IniFormat);

    for(iLoop = 0; iLoop < DISP_FM_NUM ; iLoop++)
    {
        QString strV = "/FM/";
        QString strTmp;

        if (Param.aulCfg[iLoop] != tmpParam.aulCfg[iLoop])
        {
            strV  += QString::number(iLoop);
            
            strTmp = QString::number(Param.aulCfg[iLoop]);
            
            config->setValue(strV,strTmp);

            qDebug() << "Save FM: " << strTmp;
        }

    }    
    if (config)
    {
        delete config;
        config = NULL;
    }
    
    sync();
}

void MainSavePMParam(int iMachineType,DISP_PM_SETTING_STRU  &Param)
{
    /* retrive parameter from configuration */
    int iLoop;

    DISP_PM_SETTING_STRU  tmpParam;

    QString strCfgName = gaMachineType[iMachineType].strName;

    strCfgName += ".ini";

    MainRetrivePmParam(iMachineType,tmpParam);    

    QSettings *config = new QSettings(strCfgName, QSettings::IniFormat);

    for(iLoop = 0; iLoop < DISP_PM_NUM ; iLoop++)
    {
        QString strV = "/PM/";
        QString strTmp;

        strV  += QString::number(iLoop);

        if (Param.aiBuckType[iLoop] != tmpParam.aiBuckType[iLoop])
        {
            strTmp = QString::number(Param.aiBuckType[iLoop]);

            config->setValue(strV + "/TYPE",strTmp);

        }
        if (Param.afCap[iLoop] != tmpParam.afCap[iLoop])
        {

            strTmp = QString::number(Param.afCap[iLoop],'f',2);
            
            config->setValue(strV + "/CAP",strTmp);

        }
        if (Param.afDepth[iLoop] != tmpParam.afDepth[iLoop])
        {
            strTmp = QString::number(Param.afDepth[iLoop],'f',2);
            
            config->setValue(strV + "/RANGE",strTmp);
        }
        

    }    
    if (config)
    {
        delete config;
        config = NULL;
    }
    
    sync();
}


void MainSaveCalParam(int iMachineType,DISP_CAL_SETTING_STRU  &Param)
{
    /* retrive parameter from configuration */
    int iLoop;

    DISP_CAL_SETTING_STRU  tmpParam;

    QString strCfgName = gaMachineType[iMachineType].strName;

    strCfgName += ".ini";

    MainRetriveCalParam(iMachineType,tmpParam);    

    QSettings *config = new QSettings(strCfgName, QSettings::IniFormat);

    for(iLoop = 0; iLoop < DISP_PM_NUM ; iLoop++)
    {
        QString strV = "/CAL/";
        QString strTmp;

        if (Param.afCfg[iLoop] != tmpParam.afCfg[iLoop])
        {
            strV  += QString::number(iLoop);
            
            strTmp = QString::number(Param.afCfg[iLoop],'f', 3);
            
            config->setValue(strV,strTmp);
        }

    }    
    if (config)
    {
        delete config;
        config = NULL;
    }
    
    sync();
}

void MainSaveCalibrateParam(int iMachineType,DISP_PARAM_CALI_STRU  &Param)
{
    /* retrive parameter from configuration */
    int iLoop;

    DISP_PARAM_CALI_STRU  tmpParam;

    QString strCfgName = gaMachineType[iMachineType].strName;

    strCfgName += ".ini";

    MainRetriveCalibrateParam(iMachineType,tmpParam);    

    QSettings *config = new QSettings(strCfgName, QSettings::IniFormat);

    for(iLoop = 0; iLoop < DISP_PC_COFF_NUM ; iLoop++) //DISP_PM_NUM
    {
        QString strV = "/PCCOFF/";
        QString strTmp;

        strV  += QString::number(iLoop);

        if (Param.pc[iLoop].fk != tmpParam.pc[iLoop].fk)
        {
            strTmp = QString::number(Param.pc[iLoop].fk,'f', 3);
            QString strV1 = "/K" + strV;
            config->setValue(strV1,strTmp);
        }
        
        if (Param.pc[iLoop].fc != tmpParam.pc[iLoop].fc)
        {
            strTmp = QString::number(Param.pc[iLoop].fc,'f', 3);
            QString strV2 = "/C" + strV;
            config->setValue(strV2,strTmp);
        }
        
        if (Param.pc[iLoop].fv != tmpParam.pc[iLoop].fv)
        {
            strTmp = QString::number(Param.pc[iLoop].fv,'f', 3);
            QString strV3 = "/V" + strV;
            config->setValue(strV3,strTmp);
        }
    }    

    //2019.1.3
    DISP_FM_SETTING_STRU fmParam;

    float default_L_Machine = 450.0;
    float default_Machine = 7055.0;

    switch(iMachineType)
    {
    case MACHINE_L_Genie:
        for(int i = 0;i < DISP_PC_COFF_NUM ; i++)
        {
            fmParam.aulCfg[DISP_FM_FM1] = default_L_Machine / Param.pc[DISP_PC_COFF_S1].fk;
            fmParam.aulCfg[DISP_FM_FM2]= default_L_Machine / Param.pc[DISP_PC_COFF_S2].fk;
            fmParam.aulCfg[DISP_FM_FM3] = default_L_Machine / Param.pc[DISP_PC_COFF_S3].fk;
            fmParam.aulCfg[DISP_FM_FM4] = default_L_Machine / Param.pc[DISP_PC_COFF_S4].fk;
        }
        break;
    case MACHINE_L_UP:
    {
        fmParam.aulCfg[DISP_FM_FM1] = default_L_Machine / Param.pc[8].fk;
        fmParam.aulCfg[DISP_FM_FM2] = default_L_Machine / Param.pc[9].fk;
        fmParam.aulCfg[DISP_FM_FM3] = default_L_Machine / Param.pc[10].fk;
        fmParam.aulCfg[DISP_FM_FM4] = default_L_Machine / Param.pc[11].fk;
        break;
    }
    case MACHINE_L_EDI_LOOP:
    {
        fmParam.aulCfg[DISP_FM_FM1] = default_L_Machine / Param.pc[8].fk;
        fmParam.aulCfg[DISP_FM_FM2] = default_L_Machine / Param.pc[9].fk;
        fmParam.aulCfg[DISP_FM_FM3] = default_L_Machine / Param.pc[10].fk;
        fmParam.aulCfg[DISP_FM_FM4] = default_L_Machine / Param.pc[11].fk;
        break;
    }
    case MACHINE_L_RO_LOOP:
    {
        fmParam.aulCfg[DISP_FM_FM1] = default_L_Machine / Param.pc[4].fk;
        fmParam.aulCfg[DISP_FM_FM2] = default_L_Machine / Param.pc[5].fk;
        fmParam.aulCfg[DISP_FM_FM3] = default_L_Machine / Param.pc[6].fk;
        fmParam.aulCfg[DISP_FM_FM4] = default_L_Machine / Param.pc[7].fk;
        break;
    }
    case MACHINE_Genie:
        fmParam.aulCfg[DISP_FM_FM1]  = default_Machine / Param.pc[10].fk;
        break;
    case MACHINE_UP:
        fmParam.aulCfg[DISP_FM_FM1]  = default_Machine / Param.pc[10].fk;
        break;
    case MACHINE_EDI:
        fmParam.aulCfg[DISP_FM_FM1]  = default_Machine / Param.pc[8].fk;
        break;
    case MACHINE_RO:
        fmParam.aulCfg[DISP_FM_FM1]  = default_Machine / Param.pc[6].fk;
        break;
    case MACHINE_PURIST:
        fmParam.aulCfg[DISP_FM_FM1]  = default_Machine / Param.pc[6].fk;
        break;
    case MACHINE_ADAPT:
        fmParam.aulCfg[DISP_FM_FM1]  = default_Machine / Param.pc[8].fk;
        break;
    default:
        break;
    }
    MainSaveFMParam(iMachineType, fmParam);

    //end
    if (config)
    {
        delete config;
        config = NULL;
    }
    
    sync();
}

void MainSaveMiscParam(int iMachineType,DISP_MISC_SETTING_STRU  &Param)
{
    /* retrive parameter from configuration */
    // int iLoop;

    DISP_MISC_SETTING_STRU  tmpParam;

    QString strCfgName = gaMachineType[iMachineType].strName;

    strCfgName += ".ini";

    MainRetriveMiscParam(iMachineType,tmpParam);    

    QSettings *config = new QSettings(strCfgName, QSettings::IniFormat);

    {
        QString strV = "/MISC/FLAG";
        QString strTmp;

        if (Param.ulAllocMask != tmpParam.ulAllocMask)
        {
            strTmp = QString::number(Param.ulAllocMask);
            config->setValue(strV,strTmp);
        }

        strV = "/MISC/ICP";
        
        if (Param.iTubeCirCycle != tmpParam.iTubeCirCycle)
        {
            strTmp = QString::number(Param.iTubeCirCycle);
            config->setValue(strV,strTmp);
        }
        
        strV = "/MISC/DT";
        
        if (Param.iTubeCirDuration != tmpParam.iTubeCirDuration)
        {
            strTmp = QString::number(Param.iTubeCirDuration);
            config->setValue(strV,strTmp);
        }

        strV = "/MISC/ST";
        
        if (Param.aiTubeCirTimeInfo[0] != tmpParam.aiTubeCirTimeInfo[0])
        {
            strTmp = QString::number(Param.aiTubeCirTimeInfo[0]);
            config->setValue(strV,strTmp);
        }
        
        strV = "/MISC/ET";
        
        if (Param.aiTubeCirTimeInfo[1] != tmpParam.aiTubeCirTimeInfo[1])
        {
            strTmp = QString::number(Param.aiTubeCirTimeInfo[1]);
            config->setValue(strV,strTmp);
        }
    
        strV = "/MISC/RP0";
        
        if (Param.RPumpSetting[0] != tmpParam.RPumpSetting[0])
        {
            strTmp = QString::number(Param.RPumpSetting[0]);
            config->setValue(strV,strTmp);
        }

        strV = "/MISC/RP1";
        
        if (Param.RPumpSetting[1] != tmpParam.RPumpSetting[1])
        {
            strTmp = QString::number(Param.RPumpSetting[1]);
            config->setValue(strV,strTmp);
        }
        
        strV = "/MISC/LAN";
        
        if (Param.iLan != tmpParam.iLan)
        {
            strTmp = QString::number(Param.iLan);
            config->setValue(strV,strTmp);
        }

        strV = "/MISC/BRIGHTNESS";
        
        if (Param.iBrightness != tmpParam.iBrightness)
        {
            strTmp = QString::number(Param.iBrightness);
            config->setValue(strV,strTmp);
        }
        
        strV = "/MISC/ENERGYSAVE";
        
        if (Param.iEnerySave != tmpParam.iEnerySave)
        {
            strTmp = QString::number(Param.iEnerySave);
            config->setValue(strV,strTmp);
        }
        
        strV = "/MISC/CONDUCTIVITYUNIT";
        if (Param.iUint4Conductivity != tmpParam.iUint4Conductivity)
        {
            strTmp = QString::number(Param.iUint4Conductivity);
            config->setValue(strV,strTmp);
        }       

        strV = "/MISC/TEMPUNIT";
        if (Param.iUint4Temperature != tmpParam.iUint4Temperature)
        {
            strTmp = QString::number(Param.iUint4Temperature);
            config->setValue(strV,strTmp);
        }       
        
        strV = "/MISC/PRESSUREUNIT";
        if (Param.iUint4Pressure != tmpParam.iUint4Pressure)
        {
            strTmp = QString::number(Param.iUint4Pressure);
            config->setValue(strV,strTmp);
        }       

        strV = "/MISC/LIQUIDUNIT";
        if (Param.iUint4LiquidLevel != tmpParam.iUint4LiquidLevel)
        {
            strTmp = QString::number(Param.iUint4LiquidLevel);
            config->setValue(strV,strTmp);
        }       

        strV = "/MISC/FLOWVELOCITYUNIT";
        if (Param.iUint4FlowVelocity != tmpParam.iUint4FlowVelocity)
        {
            strTmp = QString::number(Param.iUint4FlowVelocity);
            config->setValue(strV,strTmp);
        }  
        
        strV = "/MISC/SOUNDMASK";
        if (Param.iSoundMask != tmpParam.iSoundMask)
        {
            strTmp = QString::number(Param.iSoundMask);
            config->setValue(strV,strTmp);
        }  
    
        strV = "/MISC/NETWORKMASK";
        if (Param.iNetworkMask != tmpParam.iNetworkMask)
        {
            strTmp = QString::number(Param.iNetworkMask);
            config->setValue(strV,strTmp);
        }  

        strV = "/MISC/MISCFLAG";
        if (Param.ulMisFlags != tmpParam.ulMisFlags)
        {
            strTmp = QString::number(Param.ulMisFlags);
            config->setValue(strV,strTmp);
        }  
        
        strV = "/MISC/TANKUVONTIME";
        if (Param.iTankUvOnTime != tmpParam.iTankUvOnTime)
        {
            strTmp = QString::number(Param.iTankUvOnTime);
            config->setValue(strV,strTmp);
        }         
        strV = "/MISC/AUTOLOGOUTTIME";
        if (Param.iAutoLogoutTime != tmpParam.iAutoLogoutTime)
        {
            strTmp = QString::number(Param.iAutoLogoutTime);
            config->setValue(strV,strTmp);
        }         
        strV = "/MISC/POWERONFLUSHTIME";
        if (Param.iPowerOnFlushTime != tmpParam.iPowerOnFlushTime)
        {
            strTmp = QString::number(Param.iPowerOnFlushTime);
            config->setValue(strV,strTmp);
        }         

    }    
    if (config)
    {
        delete config;
        config = NULL;
    }
    
    sync();
}

void MainSaveCleanParam(int iMachineType,DISP_CLEAN_SETTING_STRU  &Param)
{
    /* retrive parameter from configuration */
    int iLoop;

    DISP_CLEAN_SETTING_STRU  tmpParam;

    QString strCfgName = gaMachineType[iMachineType].strName;

    strCfgName += ".ini";

    MainRetriveCleanParam(iMachineType,tmpParam);    

    QSettings *config = new QSettings(strCfgName, QSettings::IniFormat);

    for (iLoop = 0; iLoop < DISP_CLEAN_NUM; iLoop++)
    {
        QString strV = "/CLEAN/";
        QString strTmp;
        
        strV += QString::number(iLoop);

        if (Param.aCleans[iLoop].lstTime != tmpParam.aCleans[iLoop].lstTime)
        {
           strTmp = QString::number(Param.aCleans[iLoop].lstTime );
           
           config->setValue(strV + "/LASTIME",0);
        }

        if (Param.aCleans[iLoop].period != tmpParam.aCleans[iLoop].period)
        {
           strTmp = QString::number(Param.aCleans[iLoop].period );
           
           config->setValue(strV + "/PERIOD",0);
        }

    }    
    
    if (config)
    {
        delete config;
        config = NULL;
    }
    
    sync();
}

void MainSaveCMSnParam(int iMachineType,DISP_CONSUME_MATERIAL_SN_STRU  &Param)
{
    /* retrive parameter from configuration */
    int iLoop;

    DISP_CONSUME_MATERIAL_SN_STRU  tmpParam;

    QString strCfgName = gaMachineType[iMachineType].strName;

    strCfgName += ".ini";

    MainRetriveCMSn(iMachineType,tmpParam);    

    QSettings *config = new QSettings(strCfgName, QSettings::IniFormat);

    for (iLoop = 0; iLoop < DISP_CM_NAME_NUM; iLoop++)
    {
        QString strV = "/CMSN/";
        QString strTmp;
        
        strV += QString::number(iLoop);

        if (strncmp(Param.aCn[iLoop],tmpParam.aCn[iLoop],APP_CAT_LENGTH))
        {
           config->setValue(strV + "/CAT",Param.aCn[iLoop]);
        }
        if (strncmp(Param.aLn[iLoop],tmpParam.aLn[iLoop],APP_LOT_LENGTH))
        {
           config->setValue(strV + "/LOT",Param.aLn[iLoop]);
        }
    }    
    
    if (config)
    {
        delete config;
        config = NULL;
    }
    
    sync();
}

void MainSaveMacSnParam(int iMachineType,DISP_MACHINERY_SN_STRU  &Param)
{
    /* retrive parameter from configuration */
    int iLoop;

    DISP_MACHINERY_SN_STRU  tmpParam;

    QString strCfgName = gaMachineType[iMachineType].strName;

    strCfgName += ".ini";

    MainRetriveMacSn(iMachineType,tmpParam);    

    QSettings *config = new QSettings(strCfgName, QSettings::IniFormat);

    for (iLoop = 0; iLoop < DISP_MACHINERY_NAME_NUM; iLoop++)
    {
        QString strV = "/MACSN/";
        QString strTmp;
        
        strV += QString::number(iLoop);

        if (strncmp(Param.aCn[iLoop],tmpParam.aCn[iLoop],APP_CAT_LENGTH))
        {
           config->setValue(strV + "/CAT",Param.aCn[iLoop]);
        }
        if (strncmp(Param.aLn[iLoop],tmpParam.aLn[iLoop],APP_CAT_LENGTH))
        {
           config->setValue(strV + "/LOT",Param.aLn[iLoop]);
        }
    }    
    
    if (config)
    {
        delete config;
        config = NULL;
    }
    
    sync();
}

void MainSaveCMSnItem(int iMachineType,CATNO cn,LOTNO ln,int iIndex)
{
    /* retrive parameter from configuration */

    QString strCfgName = gaMachineType[iMachineType].strName;

    strCfgName += ".ini";


    QSettings *config = new QSettings(strCfgName, QSettings::IniFormat);

    if (iIndex < DISP_CM_NAME_NUM)
    {
        QString strV = "/CMSN/";
        QString strTmp;
        
        strV += QString::number(iIndex);

        config->setValue(strV + "/CAT",cn);
        
        config->setValue(strV + "/LOT",ln);
    }    
    
    if (config)
    {
        delete config;
        config = NULL;
    }
    
    sync();
}

void MainSaveMacSnItem(int iMachineType,CATNO cn,LOTNO ln,int iIndex)
{
    /* retrive parameter from configuration */

    QString strCfgName = gaMachineType[iMachineType].strName;

    strCfgName += ".ini";


    QSettings *config = new QSettings(strCfgName, QSettings::IniFormat);

    if (iIndex < DISP_MACHINERY_NAME_NUM)
    {
        QString strV = "/MACSN/";
        QString strTmp;
        
        strV += QString::number(iIndex);

        config->setValue(strV + "/CAT",cn);
        
        config->setValue(strV + "/LOT",ln);
    }    
    
    if (config)
    {
        delete config;
        config = NULL;
    }
    
    sync();
}


void MainRetriveGlobalParam(void)
{

    gGlobalParam.iMachineType   = MACHINE_L_Genie;

    gGlobalParam.TMParam.aulTime[TIME_PARAM_RoWashT1] = DEFAULT_RoWashT1;
    gGlobalParam.TMParam.aulTime[TIME_PARAM_RoWashT2] = DEFAULT_RoWashT2;
    gGlobalParam.TMParam.aulTime[TIME_PARAM_RoWashT3] = DEFAULT_RoWashT3;
    
    gGlobalParam.TMParam.aulTime[TIME_PARAM_PhWashT1] = DEFAULT_PhWashT1;
    gGlobalParam.TMParam.aulTime[TIME_PARAM_PhWashT2] = DEFAULT_PhWashT2;
    gGlobalParam.TMParam.aulTime[TIME_PARAM_PhWashT3] = DEFAULT_PhWashT3;
    gGlobalParam.TMParam.aulTime[TIME_PARAM_PhWashT4] = DEFAULT_PhWashT4;
    gGlobalParam.TMParam.aulTime[TIME_PARAM_PhWashT5] = DEFAULT_PhWashT5;
    
    gGlobalParam.TMParam.aulTime[TIME_PARAM_InitRunT1] = DEFAULT_InitRunT1;
    
    gGlobalParam.TMParam.aulTime[TIME_PARAM_NormRunT1] = DEFAULT_NormRunT1;
    gGlobalParam.TMParam.aulTime[TIME_PARAM_NormRunT2] = DEFAULT_NormRunT2;
    gGlobalParam.TMParam.aulTime[TIME_PARAM_NormRunT3] = DEFAULT_NormRunT3;
    gGlobalParam.TMParam.aulTime[TIME_PARAM_NormRunT4] = DEFAULT_NormRunT4;
    gGlobalParam.TMParam.aulTime[TIME_PARAM_NormRunT5] = DEFAULT_NormRunT5;
    
    gGlobalParam.TMParam.aulTime[TIME_PARAM_N3Period]  = DEFAULT_N3Period;
    gGlobalParam.TMParam.aulTime[TIME_PARAM_N3Duration]= DEFAULT_N3Duration;
    
    gGlobalParam.TMParam.aulTime[TIME_PARAM_TOCT1]     = DEFAULT_TOCT1;
    gGlobalParam.TMParam.aulTime[TIME_PARAM_TOCT2]     = DEFAULT_TOCT2;
    gGlobalParam.TMParam.aulTime[TIME_PARAM_TOCT3]     = DEFAULT_TOCT3;
    
    gGlobalParam.TMParam.aulTime[TIME_PARAM_IdleCirPeriod] = DEFAULT_IDLECIRTIME;
    gGlobalParam.TMParam.aulTime[TIME_PARAM_InterCirDuration] = DEFAULT_INTERCIRTIME;    

    gGlobalParam.TMParam.aulTime[TIME_PARAM_LPP] = DEFAULT_LPP;

    MainRetriveMachineType(gGlobalParam.iMachineType);

    MainRetriveLastRunState(gGlobalParam.iMachineType); //20190521
    MainRetriveDefaultState(gGlobalParam.iMachineType); //ex_dcj
    MainRetriveExMachineMsg(gGlobalParam.iMachineType);
    MainRetriveProductMsg(gGlobalParam.iMachineType); //ex_dcj
    MainRetriveInstallMsg(gGlobalParam.iMachineType); //ex_dcj
    MainRetriveExConfigParam(gGlobalParam.iMachineType);//ex_dcj

    MainRetriveMachineParam(gGlobalParam.iMachineType,gGlobalParam.MMParam);
#ifdef USER_TIMER_CONFIG    
    MainRetriveTMParam(gGlobalParam.iMachineType,gGlobalParam.TMParam);
#endif
    MainRetriveAlarmSetting(gGlobalParam.iMachineType,gGlobalParam.AlarmSettting);
    MainRetriveSubModuleSetting(gGlobalParam.iMachineType,gGlobalParam.SubModSetting);
    MainRetriveCMParam(gGlobalParam.iMachineType,gGlobalParam.CMParam);
    MainRetriveFmParam(gGlobalParam.iMachineType,gGlobalParam.FmParam);
    MainRetrivePmParam(gGlobalParam.iMachineType,gGlobalParam.PmParam);
    MainRetriveMiscParam(gGlobalParam.iMachineType,gGlobalParam.MiscParam);
    MainRetriveCleanParam(gGlobalParam.iMachineType,gGlobalParam.CleanParam);
//    MainRetriveCMSn(gGlobalParam.iMachineType,gGlobalParam.cmSn);
//    MainRetriveMacSn(gGlobalParam.iMachineType,gGlobalParam.macSn);
    MainRetriveCalibrateParam(gGlobalParam.iMachineType,gGlobalParam.Caliparam);

#ifdef SYSTEM_TEST
    //gGlobalParam.MMParam.SP[MACHINE_PARAM_SP1] = -100000;
    //gGlobalParam.MMParam.SP[MACHINE_PARAM_SP6] = -100000;
#endif    

    /* Init */
    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
        break;
    case MACHINE_Genie:
    case MACHINE_EDI:
    case MACHINE_UP:
    case MACHINE_RO:
    case MACHINE_PURIST:
    case MACHINE_ADAPT:
        gGlobalParam.SubModSetting.ulFlags &= ~(1 << DISP_SM_HaveB3);
        break;
    }
    
}

void MainUpdateGlobalParam(void)
{
    MainRetriveGlobalParam();
    CcbAsyncSetParams(NOT_PARAM_MACHINE_TYPE,&gGlobalParam);
}

void MainUpdateSpecificParam(int iParamType)
{
    switch(iParamType)
    {
    case NOT_PARAM_MACHINE_PARAM:
        MainRetriveMachineParam(gGlobalParam.iMachineType,gGlobalParam.MMParam);
        CcbAsyncSetParams(iParamType,&gGlobalParam.MMParam);
        break;
    case NOT_PARAM_SUBMODULE_PARAM:
        MainRetriveSubModuleSetting(gGlobalParam.iMachineType,gGlobalParam.SubModSetting);
        CcbAsyncSetParams(iParamType,&gGlobalParam.SubModSetting);
        break;
    case NOT_PARAM_ALARM_PARAM:
        MainRetriveAlarmSetting(gGlobalParam.iMachineType,gGlobalParam.AlarmSettting);
        CcbAsyncSetParams(iParamType,&gGlobalParam.AlarmSettting);
        break;
    case NOT_PARAM_TIME_PARAM:
#ifdef USER_TIMER_CONFIG    
        MainRetriveTMParam(gGlobalParam.iMachineType,gGlobalParam.TMParam);
#endif
        CcbAsyncSetParams(iParamType,&gGlobalParam.TMParam);
        break;
    case NOT_PARAM_CM_PARAM:
        MainRetriveCMParam(gGlobalParam.iMachineType,gGlobalParam.CMParam);
        CcbAsyncSetParams(iParamType,&gGlobalParam.CMParam);
        break;
    case NOT_PARAM_FM_PARAM:
        MainRetriveFmParam(gGlobalParam.iMachineType,gGlobalParam.FmParam);
        CcbAsyncSetParams(iParamType,&gGlobalParam.FmParam);
        break;
    case NOT_PARAM_PM_PARAM:
        MainRetrivePmParam(gGlobalParam.iMachineType,gGlobalParam.PmParam);
        CcbAsyncSetParams(iParamType,&gGlobalParam.PmParam);
        break;
    case NOT_PARAM_MISC_PARAM:
        MainRetriveMiscParam(gGlobalParam.iMachineType,gGlobalParam.MiscParam);
        CcbAsyncSetParams(iParamType,&gGlobalParam.MiscParam);
        break;
    default:
        return;
    }
}

unsigned int DispGetCurSecond(void)
{
    QDateTime sysDateTime;
    sysDateTime = QDateTime::currentDateTime();

    return (sysDateTime.toTime_t());

}

int DispGetDay(void)
{
    QDateTime sysDateTime;
    sysDateTime = QDateTime::currentDateTime();

    return (sysDateTime.date().day());

}


void CheckConsumptiveMaterialState(void)
{
    uint32_t ulCurTime = DispGetCurSecond();
    uint32_t ulTemp;
    int iMask;

    gCMUsage.ulUsageState = 0; 

    iMask = gpMainWnd->getMachineNotifyMask(gGlobalParam.iMachineType,0);

    if ((ulCurTime > gCMUsage.info.aulCms[DISP_PRE_PACKLIFEDAY])
            && (iMask & (1 << DISP_PRE_PACK)))
    {
        ulTemp = (ulCurTime - gCMUsage.info.aulCms[DISP_PRE_PACKLIFEDAY])/DISP_DAYININSECOND;


        if (ulTemp >= gGlobalParam.CMParam.aulCms[DISP_PRE_PACKLIFEDAY])
        {
            gCMUsage.ulUsageState |= (1 << DISP_PRE_PACKLIFEDAY);
        }
        
        if (gCMUsage.info.aulCms[DISP_PRE_PACKLIFEL] >= gGlobalParam.CMParam.aulCms[DISP_PRE_PACKLIFEL])
        {
            gCMUsage.ulUsageState |= (1 << DISP_PRE_PACKLIFEL);
        }

    }    
    //2018.10.22 AC PACK
    if ((ulCurTime > gCMUsage.info.aulCms[DISP_AC_PACKLIFEDAY])
            && (iMask & (1 << DISP_AC_PACK)))
    {
        ulTemp = (ulCurTime - gCMUsage.info.aulCms[DISP_AC_PACKLIFEDAY])/DISP_DAYININSECOND;


        if (ulTemp >= gGlobalParam.CMParam.aulCms[DISP_AC_PACKLIFEDAY])
        {
            gCMUsage.ulUsageState |= (1 << DISP_AC_PACKLIFEDAY);
        }

        if (gCMUsage.info.aulCms[DISP_AC_PACKLIFEL] >= gGlobalParam.CMParam.aulCms[DISP_AC_PACKLIFEL])
        {
            gCMUsage.ulUsageState |= (1 << DISP_AC_PACKLIFEL);
        }

    }

    //2018.10.12 T_Pack
    if ((ulCurTime > gCMUsage.info.aulCms[DISP_T_PACKLIFEDAY])
            && (iMask & (1 << DISP_T_PACK)))
    {
        ulTemp = (ulCurTime - gCMUsage.info.aulCms[DISP_T_PACKLIFEDAY])/DISP_DAYININSECOND;


        if (ulTemp >= gGlobalParam.CMParam.aulCms[DISP_T_PACKLIFEDAY])
        {
            gCMUsage.ulUsageState |= (1 << DISP_T_PACKLIFEDAY);
        }

        if (gCMUsage.info.aulCms[DISP_T_PACKLIFEL] >= gGlobalParam.CMParam.aulCms[DISP_T_PACKLIFEL])
        {
            gCMUsage.ulUsageState |= (1 << DISP_T_PACKLIFEL);
        }

    }
    //

    // check pack
    if ((ulCurTime > gCMUsage.info.aulCms[DISP_P_PACKLIFEDAY])
            && (iMask & (1 << DISP_P_PACK)))
    {
        ulTemp = (ulCurTime - gCMUsage.info.aulCms[DISP_P_PACKLIFEDAY])/DISP_DAYININSECOND;


        if (ulTemp >= gGlobalParam.CMParam.aulCms[DISP_P_PACKLIFEDAY])
        {
            gCMUsage.ulUsageState |= (1 << DISP_P_PACKLIFEDAY);
        }
        
        if (gCMUsage.info.aulCms[DISP_P_PACKLIFEL] >= gGlobalParam.CMParam.aulCms[DISP_P_PACKLIFEL])
        {
            gCMUsage.ulUsageState |= (1 << DISP_P_PACKLIFEL);
        }

    }

    if ((ulCurTime > gCMUsage.info.aulCms[DISP_U_PACKLIFEDAY])
            && (iMask & (1 << DISP_U_PACK)))
    {
        ulTemp = (ulCurTime - gCMUsage.info.aulCms[DISP_U_PACKLIFEDAY])/DISP_DAYININSECOND;


        if (ulTemp >= gGlobalParam.CMParam.aulCms[DISP_U_PACKLIFEDAY])
        {
            gCMUsage.ulUsageState |= (1 << DISP_U_PACKLIFEDAY);
        }
        
        if (gCMUsage.info.aulCms[DISP_U_PACKLIFEL] >= gGlobalParam.CMParam.aulCms[DISP_U_PACKLIFEL])
        {
            gCMUsage.ulUsageState |= (1 << DISP_U_PACKLIFEL);
        }

    }    

    // check pack
    if ((ulCurTime > gCMUsage.info.aulCms[DISP_H_PACKLIFEDAY])
            && (iMask & (1 << DISP_H_PACK)))
    {
        ulTemp = (ulCurTime - gCMUsage.info.aulCms[DISP_H_PACKLIFEDAY])/DISP_DAYININSECOND;


        if (ulTemp >= gGlobalParam.CMParam.aulCms[DISP_H_PACKLIFEDAY])
        {
            gCMUsage.ulUsageState |= (1 << DISP_H_PACKLIFEDAY);
        }
        
        if (gCMUsage.info.aulCms[DISP_H_PACKLIFEL] >= gGlobalParam.CMParam.aulCms[DISP_H_PACKLIFEL])
        {
            gCMUsage.ulUsageState |= (1 << DISP_H_PACKLIFEL);
        }

    }

    // check uv
    if ((ulCurTime > gCMUsage.info.aulCms[DISP_N1_UVLIFEDAY])
            && (iMask & (1 << DISP_N1_UV)))
    {
        ulTemp = (ulCurTime - gCMUsage.info.aulCms[DISP_N1_UVLIFEDAY])/DISP_DAYININSECOND;

        if (ulTemp >= gGlobalParam.CMParam.aulCms[DISP_N1_UVLIFEDAY])
        {
            gCMUsage.ulUsageState |= (1 << DISP_N1_UVLIFEDAY);
        }
        
        if (gCMUsage.info.aulCms[DISP_N1_UVLIFEHOUR] >= gGlobalParam.CMParam.aulCms[DISP_N1_UVLIFEHOUR])
        {
            gCMUsage.ulUsageState |= (1 << DISP_N1_UVLIFEHOUR);
        }

    }        
    if ((ulCurTime > gCMUsage.info.aulCms[DISP_N2_UVLIFEDAY])
            && (iMask & (1 << DISP_N2_UV)))
    {
        ulTemp = (ulCurTime - gCMUsage.info.aulCms[DISP_N2_UVLIFEDAY])/DISP_DAYININSECOND;

        if (ulTemp >= gGlobalParam.CMParam.aulCms[DISP_N2_UVLIFEDAY])
        {
            gCMUsage.ulUsageState |= (1 << DISP_N2_UVLIFEDAY);
        }
        
        if (gCMUsage.info.aulCms[DISP_N2_UVLIFEHOUR] >= gGlobalParam.CMParam.aulCms[DISP_N2_UVLIFEHOUR])
        {
            gCMUsage.ulUsageState |= (1 << DISP_N2_UVLIFEHOUR);
        }

    }        

    if ((ulCurTime > gCMUsage.info.aulCms[DISP_N3_UVLIFEDAY])
            && (iMask & (1 << DISP_N3_UV)))
    {
        ulTemp = (ulCurTime - gCMUsage.info.aulCms[DISP_N3_UVLIFEDAY])/DISP_DAYININSECOND;

        if (ulTemp >= gGlobalParam.CMParam.aulCms[DISP_N3_UVLIFEDAY])
        {
            gCMUsage.ulUsageState |= (1 << DISP_N3_UVLIFEDAY);
        }
        
        if (gCMUsage.info.aulCms[DISP_N3_UVLIFEHOUR] >= gGlobalParam.CMParam.aulCms[DISP_N3_UVLIFEHOUR])
        {
            gCMUsage.ulUsageState |= (1 << DISP_N3_UVLIFEHOUR);
        }

    }        

    if ((ulCurTime > gCMUsage.info.aulCms[DISP_N4_UVLIFEDAY])
            && (iMask & (1 << DISP_N4_UV)))
    {
        ulTemp = (ulCurTime - gCMUsage.info.aulCms[DISP_N4_UVLIFEDAY])/DISP_DAYININSECOND;

        if (ulTemp >= gGlobalParam.CMParam.aulCms[DISP_N4_UVLIFEDAY])
        {
            gCMUsage.ulUsageState |= (1 << DISP_N4_UVLIFEDAY);
        }
        
        if (gCMUsage.info.aulCms[DISP_N4_UVLIFEHOUR] >= gGlobalParam.CMParam.aulCms[DISP_N4_UVLIFEHOUR])
        {
            gCMUsage.ulUsageState |= (1 << DISP_N4_UVLIFEHOUR);
        }

    }        
    
    if ((ulCurTime > gCMUsage.info.aulCms[DISP_N5_UVLIFEDAY])
            && (iMask & (1 << DISP_N5_UV)))
    {
        ulTemp = (ulCurTime - gCMUsage.info.aulCms[DISP_N5_UVLIFEDAY])/DISP_DAYININSECOND;

        if (ulTemp >= gGlobalParam.CMParam.aulCms[DISP_N5_UVLIFEDAY])
        {
            gCMUsage.ulUsageState |= (1 << DISP_N5_UVLIFEDAY);
        }
        
        if (gCMUsage.info.aulCms[DISP_N5_UVLIFEHOUR] >= gGlobalParam.CMParam.aulCms[DISP_N5_UVLIFEHOUR])
        {
            gCMUsage.ulUsageState |= (1 << DISP_N5_UVLIFEHOUR);
        }

    }        

    if ((ulCurTime > gCMUsage.info.aulCms[DISP_W_FILTERLIFE])
            && (iMask & (1 << DISP_W_FILTER)))
    {
        ulTemp = (ulCurTime - gCMUsage.info.aulCms[DISP_W_FILTERLIFE])/DISP_DAYININSECOND;
    
        if (ulTemp >= gGlobalParam.CMParam.aulCms[DISP_W_FILTERLIFE])
        {
            gCMUsage.ulUsageState |= (1 << DISP_W_FILTERLIFE);
        }
    }

    if ((ulCurTime > gCMUsage.info.aulCms[DISP_T_B_FILTERLIFE])
            && (iMask & (1 << DISP_T_B_FILTER)))
    {
        ulTemp = (ulCurTime - gCMUsage.info.aulCms[DISP_T_B_FILTERLIFE])/DISP_DAYININSECOND;
    
        if (ulTemp >= gGlobalParam.CMParam.aulCms[DISP_T_B_FILTERLIFE])
        {
            gCMUsage.ulUsageState |= (1 << DISP_T_B_FILTERLIFE);
        }
    }

    if ((ulCurTime > gCMUsage.info.aulCms[DISP_T_A_FILTERLIFE])
            && (iMask & (1 << DISP_T_A_FILTER)))
    {
        ulTemp = (ulCurTime - gCMUsage.info.aulCms[DISP_T_A_FILTERLIFE])/DISP_DAYININSECOND;
    
        if (ulTemp >= gGlobalParam.CMParam.aulCms[DISP_T_A_FILTERLIFE])
        {
            gCMUsage.ulUsageState |= (1 << DISP_T_A_FILTERLIFE);
        }
    }    

    if ((ulCurTime > gCMUsage.info.aulCms[DISP_TUBE_FILTERLIFE])
            && (iMask & (1 << DISP_TUBE_FILTER)))
    {
        ulTemp = (ulCurTime - gCMUsage.info.aulCms[DISP_TUBE_FILTERLIFE])/DISP_DAYININSECOND;
    
        if (ulTemp >= gGlobalParam.CMParam.aulCms[DISP_TUBE_FILTERLIFE])
        {
            gCMUsage.ulUsageState |= (1 << DISP_TUBE_FILTERLIFE);
        }
    }  

    if ((ulCurTime > gCMUsage.info.aulCms[DISP_TUBE_DI_LIFE])
            && (iMask & (1 << DISP_TUBE_DI)))
    {
        ulTemp = (ulCurTime - gCMUsage.info.aulCms[DISP_TUBE_DI_LIFE])/DISP_DAYININSECOND;
    
        if (ulTemp >= gGlobalParam.CMParam.aulCms[DISP_TUBE_DI_LIFE])
        {
            gCMUsage.ulUsageState |= (1 << DISP_TUBE_DI_LIFE);
        }
    } 

    if (ulCurTime > gGlobalParam.CleanParam.aCleans[DISP_CLEAN_RO].period)
    {
        if(!(gGlobalParam.iMachineType == MACHINE_PURIST))
        {
            gCMUsage.ulUsageState |= (1 << DISP_ROC12LIFEDAY);
        }

    }
    else
    {
        gCMUsage.ulUsageState &= ~(1 << DISP_ROC12LIFEDAY);
    }
#ifdef D_HTTPWORK
    if((gGlobalParam.MiscParam.iNetworkMask & (1 << DISPLAY_NETWORK_WIFI))
        && ex_gGlobalParam.Ex_Default)
    {
        gpMainWnd->checkConsumableAlarm();
    }
#endif
}

void MainResetCmInfo(int iSel)
{
    switch(iSel)
    {
    case DISP_PRE_PACK:
        gCMUsage.info.aulCms[DISP_PRE_PACKLIFEDAY] = DispGetCurSecond();
//#ifndef RFIDTEST
        gCMUsage.info.aulCms[DISP_PRE_PACKLIFEL]   = 0;
//#endif
        gCMUsage.ulUsageState &= ~(1 << DISP_PRE_PACKLIFEDAY);
        gCMUsage.ulUsageState &= ~(1 << DISP_PRE_PACKLIFEL);
        gCMUsage.cmInfo.aulCumulatedData[DISP_PRE_PACKLIFEDAY] = 0;
        gCMUsage.cmInfo.aulCumulatedData[DISP_PRE_PACKLIFEL] = 0;
        break;
    case DISP_AC_PACK:
        gCMUsage.info.aulCms[DISP_AC_PACKLIFEDAY] = DispGetCurSecond();
#ifndef RFIDTEST
        gCMUsage.info.aulCms[DISP_AC_PACKLIFEL]   = 0;
#endif
        gCMUsage.ulUsageState &= ~(1 << DISP_AC_PACKLIFEDAY);
        gCMUsage.ulUsageState &= ~(1 << DISP_AC_PACKLIFEL);
        gCMUsage.cmInfo.aulCumulatedData[DISP_AC_PACKLIFEDAY] = 0;
        gCMUsage.cmInfo.aulCumulatedData[DISP_AC_PACKLIFEL] = 0;
        break;
    case DISP_T_PACK:
        gCMUsage.info.aulCms[DISP_T_PACKLIFEDAY] = DispGetCurSecond();
        gCMUsage.info.aulCms[DISP_T_PACKLIFEL]   = 0;
        gCMUsage.ulUsageState &= ~(1 << DISP_T_PACKLIFEDAY);
        gCMUsage.ulUsageState &= ~(1 << DISP_T_PACKLIFEL);
        gCMUsage.cmInfo.aulCumulatedData[DISP_T_PACKLIFEDAY] = 0;
        gCMUsage.cmInfo.aulCumulatedData[DISP_T_PACKLIFEL] = 0;
        break;
    case DISP_P_PACK:
        gCMUsage.info.aulCms[DISP_P_PACKLIFEDAY] = DispGetCurSecond();
#ifndef RFIDTEST
        gCMUsage.info.aulCms[DISP_P_PACKLIFEL]   = 0;
#endif
        gCMUsage.ulUsageState &= ~(1 << DISP_P_PACKLIFEDAY);
        gCMUsage.ulUsageState &= ~(1 << DISP_P_PACKLIFEL);
        gCMUsage.cmInfo.aulCumulatedData[DISP_P_PACKLIFEDAY] = 0;
        gCMUsage.cmInfo.aulCumulatedData[DISP_P_PACKLIFEL] = 0;
        ex_isPackNew = 1; //2019.3.4 add
        break;
    case DISP_U_PACK:
        gCMUsage.info.aulCms[DISP_U_PACKLIFEDAY] = DispGetCurSecond();
#ifndef RFIDTEST
        gCMUsage.info.aulCms[DISP_U_PACKLIFEL]   = 0;
#endif
        gCMUsage.ulUsageState &= ~(1 << DISP_U_PACKLIFEDAY);
        gCMUsage.ulUsageState &= ~(1 << DISP_U_PACKLIFEL);
        gCMUsage.cmInfo.aulCumulatedData[DISP_U_PACKLIFEDAY] = 0;
        gCMUsage.cmInfo.aulCumulatedData[DISP_U_PACKLIFEL] = 0;
        break;
    case DISP_AT_PACK:
        gCMUsage.info.aulCms[DISP_AT_PACKLIFEDAY] = DispGetCurSecond();
#ifndef RFIDTEST
        gCMUsage.info.aulCms[DISP_AT_PACKLIFEL]   = 0;
#endif
        gCMUsage.ulUsageState &= ~(1 << DISP_AT_PACKLIFEDAY);
        gCMUsage.ulUsageState &= ~(1 << DISP_AT_PACKLIFEL);
        gCMUsage.cmInfo.aulCumulatedData[DISP_AT_PACKLIFEDAY] = 0;
        gCMUsage.cmInfo.aulCumulatedData[DISP_AT_PACKLIFEL] = 0;
        break;
    case DISP_H_PACK:
        gCMUsage.info.aulCms[DISP_H_PACKLIFEDAY] = DispGetCurSecond();
#ifndef RFIDTEST
        gCMUsage.info.aulCms[DISP_H_PACKLIFEL]   = 0;
#endif
        gCMUsage.ulUsageState &= ~(1 << DISP_H_PACKLIFEDAY);
        gCMUsage.ulUsageState &= ~(1 << DISP_H_PACKLIFEL);
        gCMUsage.cmInfo.aulCumulatedData[DISP_H_PACKLIFEDAY] = 0;
        gCMUsage.cmInfo.aulCumulatedData[DISP_H_PACKLIFEL] = 0;
        break;
   case DISP_N1_UV:
        gCMUsage.info.aulCms[DISP_N1_UVLIFEDAY] = DispGetCurSecond();
        gCMUsage.info.aulCms[DISP_N1_UVLIFEHOUR]= 0;
        gCMUsage.ulUsageState &= ~(1 << DISP_N1_UVLIFEDAY);
        gCMUsage.ulUsageState &= ~(1 << DISP_N1_UVLIFEHOUR);
        gCMUsage.cmInfo.aulCumulatedData[DISP_N1_UVLIFEDAY] = 0;
        gCMUsage.cmInfo.aulCumulatedData[DISP_N1_UVLIFEHOUR] = 0;
        break;
    case DISP_N2_UV:
        gCMUsage.info.aulCms[DISP_N2_UVLIFEDAY] = DispGetCurSecond();
        gCMUsage.info.aulCms[DISP_N2_UVLIFEHOUR]= 0;
        gCMUsage.ulUsageState &= ~(1 << DISP_N2_UVLIFEDAY);
        gCMUsage.ulUsageState &= ~(1 << DISP_N2_UVLIFEHOUR);
        gCMUsage.cmInfo.aulCumulatedData[DISP_N2_UVLIFEDAY] = 0;
        gCMUsage.cmInfo.aulCumulatedData[DISP_N2_UVLIFEHOUR] = 0;
        break;
    case DISP_N3_UV:
        gCMUsage.info.aulCms[DISP_N3_UVLIFEDAY] = DispGetCurSecond();
        gCMUsage.info.aulCms[DISP_N3_UVLIFEHOUR]= 0;
        gCMUsage.ulUsageState &= ~(1 << DISP_N3_UVLIFEDAY);
        gCMUsage.ulUsageState &= ~(1 << DISP_N3_UVLIFEHOUR);
        gCMUsage.cmInfo.aulCumulatedData[DISP_N3_UVLIFEDAY] = 0;
        gCMUsage.cmInfo.aulCumulatedData[DISP_N3_UVLIFEHOUR] = 0;
        break;
    case DISP_N4_UV:
        gCMUsage.info.aulCms[DISP_N4_UVLIFEDAY] = DispGetCurSecond();
        gCMUsage.info.aulCms[DISP_N4_UVLIFEHOUR]= 0;
        gCMUsage.ulUsageState &= ~(1 << DISP_N4_UVLIFEDAY);
        gCMUsage.ulUsageState &= ~(1 << DISP_N4_UVLIFEHOUR);
        gCMUsage.cmInfo.aulCumulatedData[DISP_N4_UVLIFEDAY] = 0;
        gCMUsage.cmInfo.aulCumulatedData[DISP_N4_UVLIFEHOUR] = 0;
        break;
    case DISP_N5_UV:
        gCMUsage.info.aulCms[DISP_N5_UVLIFEDAY] = DispGetCurSecond();
        gCMUsage.info.aulCms[DISP_N5_UVLIFEHOUR]= 0;
        gCMUsage.ulUsageState &= ~(1 << DISP_N5_UVLIFEDAY);
        gCMUsage.ulUsageState &= ~(1 << DISP_N5_UVLIFEHOUR);
        gCMUsage.cmInfo.aulCumulatedData[DISP_N5_UVLIFEDAY] = 0;
        gCMUsage.cmInfo.aulCumulatedData[DISP_N5_UVLIFEHOUR] = 0;
        break;
    case DISP_W_FILTER:
        gCMUsage.info.aulCms[DISP_W_FILTERLIFE] = DispGetCurSecond();
        gCMUsage.ulUsageState &= ~(1 << DISP_W_FILTERLIFE);
        gCMUsage.cmInfo.aulCumulatedData[DISP_W_FILTERLIFE] = 0;
        break;
    case DISP_T_B_FILTER:
        gCMUsage.info.aulCms[DISP_T_B_FILTERLIFE] = DispGetCurSecond();
        gCMUsage.ulUsageState &= ~(1 << DISP_T_B_FILTERLIFE);
        gCMUsage.cmInfo.aulCumulatedData[DISP_T_B_FILTERLIFE] = 0;
        break;
    case DISP_T_A_FILTER:
        gCMUsage.info.aulCms[DISP_T_A_FILTERLIFE] = DispGetCurSecond();
        gCMUsage.ulUsageState &= ~(1 << DISP_T_A_FILTERLIFE);
        gCMUsage.cmInfo.aulCumulatedData[DISP_T_A_FILTERLIFE] = 0;
        break;
    case DISP_TUBE_FILTER:
        gCMUsage.info.aulCms[DISP_TUBE_FILTERLIFE] = DispGetCurSecond();
        gCMUsage.ulUsageState &= ~(1 << DISP_TUBE_FILTERLIFE);
        gCMUsage.cmInfo.aulCumulatedData[DISP_TUBE_FILTERLIFE] = 0;
        break;
    case DISP_TUBE_DI:
        gCMUsage.info.aulCms[DISP_TUBE_DI_LIFE] = DispGetCurSecond();
        gCMUsage.ulUsageState &= ~(1 << DISP_TUBE_DI_LIFE);
        gCMUsage.cmInfo.aulCumulatedData[DISP_TUBE_DI_LIFE] = 0;
        break;
        /*
    case DISP_ROC12:
        gCMUsage.info.aulCms[DISP_ROC12LIFEDAY] = DispGetCurSecond();
        gCMUsage.ulUsageState &= ~(1 << DISP_ROC12LIFEDAY);
        gCMUsage.cmInfo.aulCumulatedData[DISP_ROC12LIFEDAY] = 0;
        break;
        */
    }

    MainSaveCMInfo(gGlobalParam.iMachineType,gCMUsage.info);
    gpMainWnd->updateCMInfoWithRFID(0); //read to RFID
    //gCMUsage.bit1PendingInfoSave = TRUE;
}

void MainSaveCMInfo(int iMachineType,DISP_CONSUME_MATERIAL_STRU  &Param)
{
    /* retrive parameter from configuration */
    int iLoop;

    DISP_CONSUME_MATERIAL_STRU  tmpParam;

    QString strCfgName = gaMachineType[iMachineType].strName;

    strCfgName += "_info.ini";
    
    QSettings *config = new QSettings(strCfgName, QSettings::IniFormat);

    MainRetriveCMInfo(iMachineType,tmpParam);

    for (iLoop = 0; iLoop < DISP_CM_NUM; iLoop++)
    {
        if (Param.aulCms[iLoop] != tmpParam.aulCms[iLoop])
        {
            QString strV   = "/CM/" + QString::number(iLoop);
            
            QString strTmp = QString::number(Param.aulCms[iLoop]);
                
            config->setValue(strV,strTmp);
        }

    }    
    if (config)
    {
        delete config;
        config = NULL;
    }

    sync();
}

void SaveConsumptiveMaterialInfo(void)
{
   int chflag = FALSE;

   int iLoop;

   int aPackArray[] = {DISP_PRE_PACKLIFEL,DISP_AC_PACKLIFEL,DISP_T_PACKLIFEL,DISP_P_PACKLIFEL,DISP_H_PACKLIFEL,DISP_U_PACKLIFEL,DISP_AT_PACKLIFEL};
   int aNArray[]    = {DISP_N1_UVLIFEHOUR,DISP_N2_UVLIFEHOUR,DISP_N3_UVLIFEHOUR,DISP_N4_UVLIFEHOUR,DISP_N5_UVLIFEHOUR};

   for (iLoop = 0; iLoop < DISP_CM_NUM; iLoop++)
   {
       if (0 != gCMUsage.cmInfo.aulCumulatedData[iLoop])
       {
           chflag = TRUE;
           break;
       }
   }
    
   if (!chflag)
   {
       return; // nothing to be saved
   }

   
   chflag = FALSE;

   for (iLoop = 0; iLoop < (int)(sizeof(aPackArray)/sizeof(aPackArray[0])); iLoop++)
   {

       if (gCMUsage.cmInfo.aulCumulatedData[aPackArray[iLoop]] > 0)
       {
           uint32_t ulTmp = gCMUsage.cmInfo.aulCumulatedData[aPackArray[iLoop]]/1000; // convert to litre
    
           if (ulTmp > 0)
           {
               gCMUsage.cmInfo.aulCumulatedData[aPackArray[iLoop]] = gCMUsage.cmInfo.aulCumulatedData[aPackArray[iLoop]]%1000; // the remains
    
               if (0XFFFFFFFFUL - gCMUsage.info.aulCms[aPackArray[iLoop]] >= ulTmp)
               {
                   gCMUsage.info.aulCms[aPackArray[iLoop]] += ulTmp;
                    
                   chflag = TRUE;
               }
    
           }
       }
   }


   for (iLoop = 0; iLoop < (int)(sizeof(aNArray)/sizeof(aNArray[0])); iLoop++)
   {
       if (gCMUsage.cmInfo.aulCumulatedData[aNArray[iLoop]] > 0)
       {
           uint32_t ulTmp = gCMUsage.cmInfo.aulCumulatedData[aNArray[iLoop]] / UV_PFM_PEROID; // convert to 10minute
       
           if (ulTmp > 0)
           {
               gCMUsage.cmInfo.aulCumulatedData[aNArray[iLoop]] = gCMUsage.cmInfo.aulCumulatedData[aNArray[iLoop]] % UV_PFM_PEROID; // the remains
    
               if (0XFFFFFFFFUL - gCMUsage.info.aulCms[aNArray[iLoop]] >= ulTmp)
               {
                   gCMUsage.info.aulCms[aNArray[iLoop]] += ulTmp;
                    
                   chflag = TRUE;
               }
           }
       }
   }

   if (chflag)
   {
      gCMUsage.bit1PendingInfoSave = TRUE;
      MainSaveCMInfo(gGlobalParam.iMachineType,gCMUsage.info); //
      if(DISP_WORK_STATE_IDLE != DispGetWorkState4Pw())
      {
          gpMainWnd->updateCMInfoWithRFID(1);
      }
   }
   
}

QStringList MainWindow::m_strConsuamble[CAT_NUM];

QPixmap * MainWindow::getBitmap(int id)
{
    return gpGlobalPixmaps[id];
}

void MainWindow::on_btn_clicked(int index)
{
    printf("tmp = %d\r\n" , index);
}

void MainWindow::goAlarm()
{
    MenuPage *page = (MenuPage *)m_pSubPages[PAGE_MENU];

    page->goAlarm();
}

void MainWindow::switchLanguage()
{   
    int iLoop;

    buildTranslation();

    for (iLoop= 0; iLoop < m_pageList.count();iLoop++)
    {
        CPage *page = m_pageList[iLoop];

        page->switchLanguage();
    }   
}

void MainWindow::saveAlarmSP()
{   
    int iLoop;

    for (iLoop= 0; iLoop < m_pageList.count();iLoop++)
    {
        CPage *page = m_pageList[iLoop];

        page->buildTranslation();
    }   
}

void MainWindow::initUI()
{
    int /*row,*/index;

    QString astrPageName [PAGE_NUM] = {
#ifdef FLOWCHART
        "flowchart",
#endif
        "main",
        "menu",
        "service",
        "set",
    };

    QString initPageName[Ex_Init_Num] =
    {
        "InitLan",
        "InitTime",
        "InitTankSet",
        "InitSysConfig",
        "InitNetwork",
        "InitHandler",
    };

    this->resize(800, 600);

    this->setWindowFlags(Qt::FramelessWindowHint|Qt::Widget);
    this->setMouseTracking(true);

    mainWidget = new QWidget(this);
    mainWidget->setObjectName(QString::fromUtf8("mainWidget"));

    this->setCentralWidget(mainWidget);

    for (index = 0; index < GLOBAL_BMP_NUM; index++)
    {
//        gpGlobalPixmaps[index] = new QPixmap(gGlobalPixelmapName[index]);
        gpGlobalPixmaps[index] = new QPixmap(gGlobalPixelmapName[index], 0, Qt::ColorOnly);
    }

    m_pFonts[GLOBAL_FONT_12] = new QFont("" , 12 ,  QFont::Bold);
    m_pFonts[GLOBAL_FONT_14] = new QFont("" , 14 ,  QFont::Bold);
    m_pFonts[GLOBAL_FONT_24] = new QFont("" , 24 ,  QFont::Bold);
    m_pFonts[GLOBAL_FONT_30] = new QFont("" , 30 ,  QFont::Bold);
    m_pFonts[GLOBAL_FONT_40] = new QFont("" , 40 ,  QFont::Bold);
    m_pFonts[GLOBAL_FONT_60] = new QFont("" , 60 ,  QFont::Bold);
    //ex_dcj
    if(ex_gGlobalParam.Ex_Default == 0)
    {
        for(int i = 0; i < Ex_Init_Num; i++)
        {
            m_pExInitWidgets[i] = new CBaseWidget(mainWidget);
            m_pExInitWidgets[i]->setObjectName(initPageName[i]);
            m_pExInitWidgets[i]->hide();
            m_pExInitWidgets[i]->setGeometry(0, 0, 800, 600);
        }
        m_pExInitPages[Ex_Init_Lan] = new Ex_Init_Languagepage(0,(CBaseWidget *)m_pExInitWidgets[Ex_Init_Lan] , this);
        m_pExInitPages[Ex_Init_Time] = new Ex_Init_TimePage(0,(CBaseWidget *)m_pExInitWidgets[Ex_Init_Time] , this);
        m_pExInitPages[Ex_Init_Tankcfg] = new Ex_Init_Tankcfgpage(0,(CBaseWidget *)m_pExInitWidgets[Ex_Init_Tankcfg] , this);
        m_pExInitPages[Ex_Init_Syscfg] = new Ex_Init_Syscfgpage(0,(CBaseWidget *)m_pExInitWidgets[Ex_Init_Syscfg] , this);
        m_pExInitPages[Ex_Init_Network] = new Ex_Init_Networkpage(0,(CBaseWidget *)m_pExInitWidgets[Ex_Init_Network] , this);
        m_pExInitPages[Ex_Init_Handlercfg] = new Ex_Init_HandleCfgpage(0,(CBaseWidget *)m_pExInitWidgets[Ex_Init_Handlercfg] , this);
        m_curExInitPage = m_pExInitPages[Ex_Init_Lan];

        connect(m_pExInitPages[Ex_Init_Lan], SIGNAL(languageSwitchBtnClicked(int)), this, SLOT(on_Ex_Init_Lan(int))); //on_Ex_Init_Lan
        connect(m_pExInitPages[Ex_Init_Time], SIGNAL(timecfgSwitchBtnClicked(int)), this, SLOT(on_Ex_Init_Time(int))); //
        connect(m_pExInitPages[Ex_Init_Tankcfg], SIGNAL(tankcfgSwitchBtnClicked(int)), this, SLOT(on_Ex_Init_TankCfg(int))); //
        connect(m_pExInitPages[Ex_Init_Syscfg], SIGNAL(initcfgSwitchBtnClicked(int)), this, SLOT(on_Ex_Init_Syscfg(int))); //
        connect(m_pExInitPages[Ex_Init_Network], SIGNAL(networkSwitchBtnClicked(int)), this, SLOT(on_Ex_Init_Network(int))); //on_Ex_Init_Networ
        connect(m_pExInitPages[Ex_Init_Handlercfg], SIGNAL(exInitFinished()), this, SLOT(on_Ex_Init_Finished()));
        connect(m_pExInitPages[Ex_Init_Handlercfg], SIGNAL(handlercfgSwitchBtnClicked(int)), this, SLOT(on_Ex_Init_Handler(int)));
    }
    //ScreenPage
    m_pScreenSleepWidget = new CBaseWidget(mainWidget);
    m_pScreenSleepWidget->setObjectName("ScreenSleepPage");
    m_pScreenSleepWidget->hide();
    m_pScreenSleepWidget->setGeometry(0, 0, 800, 600);

    m_pScreenSleepPage = new Ex_ScreenSleepPage(0, m_pScreenSleepWidget, this);
    connect(m_pScreenSleepPage, SIGNAL(pageHide()), this, SLOT(on_Ex_ScreenPageHide()));
    connect(this, SIGNAL(SleepPageShow(bool)), m_pScreenSleepPage, SLOT(on_SleepPageShow(bool)));
    m_pPreviousPage = NULL;
    //end

    for (index = 0; index < PAGE_NUM; index++)
    {
        m_pSubWidget[index] = new CBaseWidget(mainWidget);
        m_pSubWidget[index]->setObjectName(astrPageName[index]);
        m_pSubWidget[index]->hide();
        m_pSubWidget[index]->setGeometry(0,0,800,600);
    }
#ifdef FLOWCHART
    m_pSubPages[PAGE_FLOWCHART]    = new Ex_FlowChartPage(0,(CBaseWidget *)m_pSubWidget[PAGE_FLOWCHART] , this);
    connect(this, SIGNAL(unitsChanged()), m_pSubPages[PAGE_FLOWCHART], SLOT(updateUnits()));
    connect(this, SIGNAL(updateFlowChartAlarm(const QString&,bool)),
            m_pSubPages[PAGE_FLOWCHART], SLOT(on_updateAlarmMsg(const QString&, bool)));
#endif
    m_pSubPages[PAGE_MAIN]    = new MainPage(0,(CBaseWidget *)m_pSubWidget[PAGE_MAIN] , this);

    m_pSubPages[PAGE_MENU]    = new MenuPage(0 , (CBaseWidget *)m_pSubWidget[PAGE_MENU],this );

    m_pSubPages[PAGE_SERVICE] = new ServicePage(0 , (CBaseWidget *)m_pSubWidget[PAGE_SERVICE] , this);

    m_pSubPages[PAGE_SET]     = new SetPage(0 , (CBaseWidget *)m_pSubWidget[PAGE_SET] , this);

    m_pCurPage   = m_pSubPages[PAGE_MAIN];
    m_curPageIdx = PAGE_MAIN;
}

MainWindow::MainWindow(QMainWindow *parent) :
    QMainWindow(parent)/*, ui(new Ui::MainWindow)*/
{
    int iLoop;

    m_bC1Regulator = false;
    m_isInitCMInfo = false; //2019.3.1 add
    ex_isPackNew = 0;

    //Set the factory default time for RFID tags
    m_consuambleInitDate = QString("2014-05-22");

    MainRetriveExConsumableMsg(gGlobalParam.iMachineType,gGlobalParam.cmSn,gGlobalParam.macSn);

    //ui->setupUi(this);
    gpMainWnd = this;

    {
        QFile qss(":/app/checkbox.qss");
        qss.open(QFile::ReadOnly);
        m_strQss4Chk = QLatin1String (qss.readAll());
        qss.close();    
    }

    {
        QFile qss(":/app/Table.qss");
        qss.open(QFile::ReadOnly);
        m_strQss4Tbl = QLatin1String (qss.readAll());
        qss.close();
    }

    {
        
        QFile qss(":/app/combox.qss");
        qss.open(QFile::ReadOnly);
        m_strQss4Cmb = QLatin1String (qss.readAll());
        qss.close();
    }
    
    m_iTubeCirFlags = -1;
    m_fd4buzzer     = -1;
    m_iNotState     = 0;
    m_iLevel        = 0;
    m_iRfidDelayedMask      = 0;
    m_iRfidBufferActiveMask = 0;
    m_pCurPage              = NULL;
    m_curExInitPage         = NULL;

    m_strConsuamble[ACPACK_CATNO] << "RR700AC01" << "171-1254";
    m_strConsuamble[TPACK_CATNO] << "RR700T101" << "171-1259";
    m_strConsuamble[PPACK_CATNO] << "RR700CP01" << "RR700CP02" << "171-1255" << "171-1256";
    m_strConsuamble[UPACK_CATNO] << "RR700Q101" << "RR700Q201" << "RR700Q301"
                                 << "RR700Q501" << "RR700Q601" << "RR700Q701"
                                 << "171-1258" << "171-1260" << "171-1261";

    m_strConsuamble[HPACK_CATNO] << "RR700H101" << "171-1257";
    m_strConsuamble[CLEANPACK_CATNO] << "RR700CL01" << "171-1281";

    m_strConsuamble[UV254_CATNO] << "RAUV135A7" << "171-1282";
    m_strConsuamble[UV185_CATNO] << "RAUV357B7" << "171-1283";
    m_strConsuamble[UVTANK_CATNO] << "RAUV357A7" << "171-1270";

    m_strConsuamble[TANKVENTFILTER_CATNO] << "RATANKVN7" << "171-1267";
    m_strConsuamble[FINALFILTER_CATNO] << "RAFFC7250" << "RAFFB7201"
                                       << "171-1262" << "171-1263";

    m_strConsuamble[UPPUMP_CATNO] << "RASP743" << "171-1280";
    m_strConsuamble[ROPACK_CATNO] << "RR70R0501" << "RR70R1001" << "RR70R1501"
                                  << "171-1276" << "171-1277";
    m_strConsuamble[ROPUMP_CATNO] << "RASP742" << "171-1279";

    m_strConsuamble[EDI_CATNO] << "W3T101572" << "W3T101573" << "W3T262701"
                               << "171-1288" << "171-1289" << "171-1290";
    gCMUsage.ulUsageState = 0;

    memset(m_iAlarmRcdMask ,0,sizeof(m_iAlarmRcdMask));

    MainRetriveCMInfo(gGlobalParam.iMachineType,gCMUsage.info);

    Write_sys_int(PWMLCD_FILE,gGlobalParam.MiscParam.iBrightness);

    m_bLoged = true;

    iFirst = 1;
    m_bLockupDlg = false;

    
    m_iExeActiveMask = 0;
    m_iFmActiveMask  = 0;
    m_eWorkMode      = APP_WORK_MODE_NORMAL; /* refer APP_WORK_MODE_NUM */

    for(iLoop = 0; iLoop < APP_EXE_ECO_NUM; iLoop++)
    {
        switch(iLoop)
        {
        case 0:
        case 1:
            m_EcowOfDay[iLoop].iQuality = 2000;
            break;
        case 2:
        case 3:
        case 4:
            m_EcowOfDay[iLoop].iQuality = 0;
            break;
        default:
            break;
        }

    }

    for (iLoop = 0; iLoop < APP_EXE_INPUT_REG_PUMP_NUM; iLoop++)
    {
//        m_aiRPumpVoltageLevel[iLoop] = 0; //ex_dcj
        m_aiRPumpVoltageLevel[iLoop] = PUMP_SPEED_10;
    }
    
    for (iLoop = 0; iLoop < APP_DEV_HS_SUB_NUM; iLoop++)
    {
        m_afWQuantity[iLoop] = 0;
    }

    for (iLoop = 0; iLoop < APP_FM_FLOW_METER_NUM; iLoop++)
    {
        m_ulFlowMeter[iLoop]       = 0;
        m_ulLstFlowMeter[iLoop]    = 0;
        m_iLstFlowMeterTick[iLoop] = 0;
        m_ulFlowRptTick[iLoop]     = 0;
    }

    m_EcoInfo[APP_EXE_I1_NO].fQuality    = 2000;
    m_EcoInfo[APP_EXE_I1_NO].fTemperature = 25;

    m_EcoInfo[APP_EXE_I2_NO].fQuality    = 2000;
    m_EcoInfo[APP_EXE_I2_NO].fTemperature = 25;

    m_EcoInfo[APP_EXE_I3_NO].fQuality    = 1.0;
    m_EcoInfo[APP_EXE_I3_NO].fTemperature = 25;

    m_EcoInfo[APP_EXE_I4_NO].fQuality    = 1.0;
    m_EcoInfo[APP_EXE_I4_NO].fTemperature = 25;

    m_EcoInfo[APP_EXE_I5_NO].fQuality    = 18.2;
    m_EcoInfo[APP_EXE_I5_NO].fTemperature = 25;

    memset(m_aHandler,0,sizeof(m_aHandler));

    memset(m_aRfid,0,sizeof(m_aRfid));
    
    /* ui init, place buildTranslation before initUI */
    buildTranslation();

    initHandler();

    initRfid();

    initRfIdLayout(&m_RfDataItems);   
    
    for (iLoop = 0; iLoop < APP_RFID_SUB_TYPE_NUM; iLoop++)
    {
        m_aRfidInfo[iLoop].setLayOut(&m_RfDataItems);
    }

    for (iLoop = 0; iLoop < MACHINE_NUM; iLoop++)
    {
        switch(iLoop)
        {
        case MACHINE_L_Genie:

            /*alarm masks */
            m_aMas[iLoop].aulMask[DISP_ALARM_PART0]  = DISP_ALARM_DEFAULT_PART0 ;
            m_aMas[iLoop].aulMask[DISP_ALARM_PART0] &= (~((1 << DISP_ALARM_PART0_HPACK_OOP)
                                                          |(1 << DISP_ALARM_PART0_PREPACK_OOP)
                                                          |(1 << DISP_ALARM_PART0_TUBEUV_OOP)));

            m_aMas[iLoop].aulMask[DISP_ALARM_PART1]  = DISP_ALARM_DEFAULT_PART1 ;
#ifdef PCBTEST
            m_bC1Regulator = true;
#endif
            break;
        case MACHINE_L_UP:
            
            /*alarm masks */
            m_aMas[iLoop].aulMask[DISP_ALARM_PART0]  = DISP_ALARM_DEFAULT_PART0 ;
            m_aMas[iLoop].aulMask[DISP_ALARM_PART0] &= (~((1 << DISP_ALARM_PART0_ATPACK_OOP)
                                                          |(1 << DISP_ALARM_PART0_PREPACK_OOP)
                                                          |(1 << DISP_ALARM_PART0_TUBEUV_OOP)));

            m_aMas[iLoop].aulMask[DISP_ALARM_PART1]  = DISP_ALARM_DEFAULT_PART1;            
            m_aMas[iLoop].aulMask[DISP_ALARM_PART1] &= (~((1 << DISP_ALARM_PART1_LOWER_EDI_PRODUCT_RESISTENCE)
                                                          |(1 << DISP_ALARM_PART1_HIGHER_EDI_PRODUCT_TEMPERATURE)
                                                          |(1 << DISP_ALARM_PART1_LOWER_EDI_PRODUCT_TEMPERATURE)));
            
            break;
        case MACHINE_L_EDI_LOOP:

            /*alarm masks */
            m_aMas[iLoop].aulMask[DISP_ALARM_PART0]  = DISP_ALARM_DEFAULT_PART0 ;
            m_aMas[iLoop].aulMask[DISP_ALARM_PART0] &= (~((1 << DISP_ALARM_PART0_UPACK_OOP)
                                                          |(1 << DISP_ALARM_PART0_TUBEUV_OOP)
                                                          |(1 << DISP_ALARM_PART0_PREPACK_OOP)
                                                          |(1 << DISP_ALARM_PART0_185UV_OOP)));

            m_aMas[iLoop].aulMask[DISP_ALARM_PART1]  = DISP_ALARM_DEFAULT_PART1;
            m_aMas[iLoop].aulMask[DISP_ALARM_PART1] &= ( ~((1 << DISP_ALARM_PART1_LOWER_UP_PRODUCT_RESISTENCE)
                                                           |(1 << DISP_ALARM_PART1_HIGHER_UP_PRODUCT_TEMPERATURE)
                                                           |(1 << DISP_ALARM_PART1_HIGHER_TOC_SENSOR_TEMPERATURE)
                                                           |(1 << DISP_ALARM_PART1_LOWER_TOC_SENSOR_TEMPERATURE)
                                                           |(1 << DISP_ALARM_PART1_LOWER_TOC_SOURCE_WATER_RESISTENCE)
                                                           |(1 << DISP_ALARM_PART1_LOWER_UP_PRODUCT_TEMPERATURE)));
            break;
        case MACHINE_L_RO_LOOP:
            
            /*alarm masks */
            m_aMas[iLoop].aulMask[DISP_ALARM_PART0]  = DISP_ALARM_DEFAULT_PART0;
            m_aMas[iLoop].aulMask[DISP_ALARM_PART0] &= ( ~((1 << DISP_ALARM_PART0_ATPACK_OOP)
                                                           |(1 << DISP_ALARM_PART0_UPACK_OOP)
                                                           |(1 << DISP_ALARM_PART0_TUBEUV_OOP)
                                                           |(1 << DISP_ALARM_PART0_PREPACK_OOP)
                                                           |(1 << DISP_ALARM_PART0_185UV_OOP)));

            m_aMas[iLoop].aulMask[DISP_ALARM_PART1]  = DISP_ALARM_DEFAULT_PART1;
            m_aMas[iLoop].aulMask[DISP_ALARM_PART1] &= (~((1 << DISP_ALARM_PART1_LOWER_EDI_PRODUCT_RESISTENCE)
                                                         |(1 << DISP_ALARM_PART1_LOWER_UP_PRODUCT_RESISTENCE)
                                                         |(1 << DISP_ALARM_PART1_LOWER_UP_PRODUCT_TEMPERATURE)
                                                         |(1 << DISP_ALARM_PART1_HIGHER_UP_PRODUCT_TEMPERATURE)
                                                         |(1 << DISP_ALARM_PART1_HIGHER_EDI_PRODUCT_TEMPERATURE)
                                                         |(1 << DISP_ALARM_PART1_HIGHER_TOC_SENSOR_TEMPERATURE)
                                                         |(1 << DISP_ALARM_PART1_LOWER_TOC_SENSOR_TEMPERATURE)
                                                         |(1 << DISP_ALARM_PART1_LOWER_TOC_SOURCE_WATER_RESISTENCE)
                                                         |(1 << DISP_ALARM_PART1_LOWER_EDI_PRODUCT_TEMPERATURE)));
            
            break;
        case MACHINE_Genie:

            /*alarm masks */
            m_aMas[iLoop].aulMask[DISP_ALARM_PART0]  = DISP_ALARM_DEFAULT_PART0;
            m_aMas[iLoop].aulMask[DISP_ALARM_PART0] &= (~((1 << DISP_ALARM_PART0_ATPACK_OOP)
                                                          |(1 << DISP_ALARM_PART0_HPACK_OOP)
                                                          |(1 << DISP_ALARM_PART0_PREPACK_OOP)
                                                          |(1 << DISP_ALARM_PART0_TUBEUV_OOP)));

            m_aMas[iLoop].aulMask[DISP_ALARM_PART1]  = DISP_ALARM_DEFAULT_PART1;
            m_aMas[iLoop].aulMask[DISP_ALARM_PART1] &= (~((1 << DISP_ALARM_PART1_LOWER_TUBE_RESISTENCE)
                                                         |(1 << DISP_ALARM_PART1_LOWER_SWTANKE_WATER_LEVEL)
                                                         |(1 << DISP_ALARM_PART1_LOWER_RO_PRODUCT_FLOWING_VELOCITY)
                                                         |(1 << DISP_ALARM_PART1_LOWER_RO_WASTE_FLOWING_VELOCITY)
                                                         |(1 << DISP_ALARM_PART1_HIGHER_TUBE_TEMPERATURE)
                                                         |(1 << DISP_ALARM_PART1_LOWER_TUBE_TEMPERATURE)));
            m_bC1Regulator = true;
            
            break;
        case MACHINE_UP:

            /*alarm masks */
            m_aMas[iLoop].aulMask[DISP_ALARM_PART0]  = DISP_ALARM_DEFAULT_PART0 ;
            m_aMas[iLoop].aulMask[DISP_ALARM_PART0] &= (~((1 << DISP_ALARM_PART0_ATPACK_OOP)
                                                         |(1 << DISP_ALARM_PART0_PREPACK_OOP)
                                                         |(1 << DISP_ALARM_PART0_TUBEUV_OOP)));

            m_aMas[iLoop].aulMask[DISP_ALARM_PART1]  = DISP_ALARM_DEFAULT_PART1;
            m_aMas[iLoop].aulMask[DISP_ALARM_PART1] &= (~((1 << DISP_ALARM_PART1_LOWER_EDI_PRODUCT_RESISTENCE)
                                                         |(1 << DISP_ALARM_PART1_LOWER_TUBE_RESISTENCE)
                                                         |(1 << DISP_ALARM_PART1_LOWER_SWTANKE_WATER_LEVEL)
                                                         |(1 << DISP_ALARM_PART1_LOWER_RO_PRODUCT_FLOWING_VELOCITY)
                                                         |(1 << DISP_ALARM_PART1_LOWER_RO_WASTE_FLOWING_VELOCITY)
                                                         |(1 << DISP_ALARM_PART1_HIGHER_EDI_PRODUCT_TEMPERATURE)
                                                         |(1 << DISP_ALARM_PART1_LOWER_EDI_PRODUCT_TEMPERATURE)
                                                         |(1 << DISP_ALARM_PART1_HIGHER_TUBE_TEMPERATURE)
                                                         |(1 << DISP_ALARM_PART1_LOWER_TUBE_TEMPERATURE)));
            
            m_bC1Regulator = true;
            break;
        case MACHINE_EDI:

            /*alarm masks */
            m_aMas[iLoop].aulMask[DISP_ALARM_PART0]  = DISP_ALARM_DEFAULT_PART0 ;
            m_aMas[iLoop].aulMask[DISP_ALARM_PART0] &= (~((1 << DISP_ALARM_PART0_185UV_OOP)
                                                         |(1 << DISP_ALARM_PART0_ATPACK_OOP)
                                                         |(1 << DISP_ALARM_PART0_PREPACK_OOP)
                                                         |(1 << DISP_ALARM_PART0_HPACK_OOP) //2018.11.19
                                                         |(1 << DISP_ALARM_PART0_TUBEUV_OOP)
                                                         |(1 << DISP_ALARM_PART0_UPACK_OOP)));

            m_aMas[iLoop].aulMask[DISP_ALARM_PART1]  = DISP_ALARM_DEFAULT_PART1;
            m_aMas[iLoop].aulMask[DISP_ALARM_PART1] &= (~((1 << DISP_ALARM_PART1_LOWER_UP_PRODUCT_RESISTENCE)
                                                          |(1 << DISP_ALARM_PART1_LOWER_TUBE_RESISTENCE)
                                                          |(1 << DISP_ALARM_PART1_LOWER_SWTANKE_WATER_LEVEL)
                                                          |(1 << DISP_ALARM_PART1_LOWER_RO_PRODUCT_FLOWING_VELOCITY)
                                                          |(1 << DISP_ALARM_PART1_LOWER_RO_WASTE_FLOWING_VELOCITY)
                                                          |(1 << DISP_ALARM_PART1_HIGHER_UP_PRODUCT_TEMPERATURE)
                                                          |(1 << DISP_ALARM_PART1_LOWER_UP_PRODUCT_TEMPERATURE)
                                                          |(1 << DISP_ALARM_PART1_HIGHER_TUBE_TEMPERATURE)
                                                          |(1 << DISP_ALARM_PART1_HIGHER_TOC_SENSOR_TEMPERATURE)
                                                          |(1 << DISP_ALARM_PART1_LOWER_TOC_SENSOR_TEMPERATURE)
                                                          |(1 << DISP_ALARM_PART1_LOWER_TOC_SOURCE_WATER_RESISTENCE)
                                                          |(1 << DISP_ALARM_PART1_LOWER_TUBE_TEMPERATURE)));
            
            m_bC1Regulator = true;
            break;
        case MACHINE_RO:

            /*alarm masks */
            m_aMas[iLoop].aulMask[DISP_ALARM_PART0]  = DISP_ALARM_DEFAULT_PART0 ;
            m_aMas[iLoop].aulMask[DISP_ALARM_PART0] &= (~((1 << DISP_ALARM_PART0_185UV_OOP)
                                                         |(1 << DISP_ALARM_PART0_ATPACK_OOP)
                                                         |(1 << DISP_ALARM_PART0_HPACK_OOP) //2018.11.12
                                                         |(1 << DISP_ALARM_PART0_TUBEUV_OOP)
                                                         |(1 << DISP_ALARM_PART0_PREPACK_OOP)
                                                         |(1 << DISP_ALARM_PART0_UPACK_OOP)));

            m_aMas[iLoop].aulMask[DISP_ALARM_PART1]  = DISP_ALARM_DEFAULT_PART1;
            m_aMas[iLoop].aulMask[DISP_ALARM_PART1] &= (~((1 << DISP_ALARM_PART1_LOWER_EDI_PRODUCT_RESISTENCE)
                                                         |(1 << DISP_ALARM_PART1_LOWER_UP_PRODUCT_RESISTENCE)
                                                         |(1 << DISP_ALARM_PART1_LOWER_TUBE_RESISTENCE)
                                                         |(1 << DISP_ALARM_PART1_LOWER_SWTANKE_WATER_LEVEL)
                                                         |(1 << DISP_ALARM_PART1_LOWER_RO_PRODUCT_FLOWING_VELOCITY)
                                                         |(1 << DISP_ALARM_PART1_LOWER_RO_WASTE_FLOWING_VELOCITY)
                                                         |(1 << DISP_ALARM_PART1_HIGHER_EDI_PRODUCT_TEMPERATURE)
                                                         |(1 << DISP_ALARM_PART1_LOWER_EDI_PRODUCT_TEMPERATURE)
                                                         |(1 << DISP_ALARM_PART1_HIGHER_UP_PRODUCT_TEMPERATURE)
                                                         |(1 << DISP_ALARM_PART1_LOWER_UP_PRODUCT_TEMPERATURE)
                                                         |(1 << DISP_ALARM_PART1_HIGHER_TUBE_TEMPERATURE)
                                                         |(1 << DISP_ALARM_PART1_HIGHER_TOC_SENSOR_TEMPERATURE)
                                                         |(1 << DISP_ALARM_PART1_LOWER_TOC_SENSOR_TEMPERATURE)
                                                         |(1 << DISP_ALARM_PART1_LOWER_TOC_SOURCE_WATER_RESISTENCE)
                                                         |(1 << DISP_ALARM_PART1_LOWER_TUBE_TEMPERATURE)));
            m_bC1Regulator = true;
            break;
        case MACHINE_PURIST:

            /*alarm masks */
            m_aMas[iLoop].aulMask[DISP_ALARM_PART0]  = DISP_ALARM_DEFAULT_PART0 ;
            m_aMas[iLoop].aulMask[DISP_ALARM_PART0] &= (~((1 << DISP_ALARM_PART0_254UV_OOP)
                                                          |(1 << DISP_ALARM_PART0_TANKUV_OOP)
                                                          |(1 << DISP_ALARM_PART0_TUBEUV_OOP)
                                                          |(1 << DISP_ALARM_PART0_PPACK_OOP)
                                                          |(1 << DISP_ALARM_PART0_ACPACK_OOP)
                                                          |(1 << DISP_ALARM_PART0_PREPACK_OOP)
                                                          |(1 << DISP_ALARM_PART0_ATPACK_OOP)));

            m_aMas[iLoop].aulMask[DISP_ALARM_PART1]  = DISP_ALARM_DEFAULT_PART1;
            m_aMas[iLoop].aulMask[DISP_ALARM_PART1] &= (~((1 << DISP_ALARM_PART1_LOWER_SOURCE_WATER_PRESSURE)
                                                         |(1 << DISP_ALARM_PART1_HIGER_SOURCE_WATER_CONDUCTIVITY)
                                                         |(1 << DISP_ALARM_PART1_LOWER_RO_RESIDUE_RATIO)
                                                         |(1 << DISP_ALARM_PART1_LOWER_EDI_PRODUCT_RESISTENCE)
                                                         |(1 << DISP_ALARM_PART1_LOWER_TUBE_RESISTENCE)
                                                         |(1 << DISP_ALARM_PART1_LOWER_SWTANKE_WATER_LEVEL)
                                                         |(1 << DISP_ALARM_PART1_LOWER_RO_PRODUCT_FLOWING_VELOCITY)
                                                         |(1 << DISP_ALARM_PART1_LOWER_RO_WASTE_FLOWING_VELOCITY)
                                                         |(1 << DISP_ALARM_PART1_LOWER_CIR_WATER_CONDUCTIVITY)
                                                         |(1 << DISP_ALARM_PART1_LOWER_HP_PRODUCT_WATER_CONDUCTIVITY)
                                                         |(1 << DISP_ALARM_PART1_HIGHER_EDI_PRODUCT_TEMPERATURE)
                                                         |(1 << DISP_ALARM_PART1_LOWER_EDI_PRODUCT_TEMPERATURE)
                                                         |(1 << DISP_ALARM_PART1_HIGHER_TUBE_TEMPERATURE)
                                                         |(1 << DISP_ALARM_PART1_LOWER_TUBE_TEMPERATURE)
                                                         |(1 << DISP_ALARM_PART1_HIGHER_SOURCE_WATER_TEMPERATURE)
                                                         |(1 << DISP_ALARM_PART1_LOWER_SOURCE_WATER_TEMPERATURE)
                                                         |(1 << DISP_ALARM_PART1_HIGH_WORK_PRESSURE)));
            
            m_bC1Regulator = true;
            break;
        case MACHINE_ADAPT:

            /*alarm masks */
            m_aMas[iLoop].aulMask[DISP_ALARM_PART0]  = DISP_ALARM_DEFAULT_PART0 ;
            m_aMas[iLoop].aulMask[DISP_ALARM_PART0] &= (~((1 << DISP_ALARM_PART0_254UV_OOP)
                                                         |(1 << DISP_ALARM_PART0_TANKUV_OOP)
                                                         |(1 << DISP_ALARM_PART0_TUBEUV_OOP)
                                                         |(1 << DISP_ALARM_PART0_PREPACK_OOP)
                                                         |(1 << DISP_ALARM_PART0_ATPACK_OOP)));

            m_aMas[iLoop].aulMask[DISP_ALARM_PART1]  = DISP_ALARM_DEFAULT_PART1;
            m_aMas[iLoop].aulMask[DISP_ALARM_PART1] &= (~((1 << DISP_ALARM_PART1_LOWER_EDI_PRODUCT_RESISTENCE)
                                                         |(1 << DISP_ALARM_PART1_LOWER_TUBE_RESISTENCE)
                                                         |(1 << DISP_ALARM_PART1_LOWER_PWTANKE_WATER_LEVEL)
                                                         |(1 << DISP_ALARM_PART1_LOWER_PWTANKE_WATER_LEVEL)
                                                         |(1 << DISP_ALARM_PART1_LOWER_RO_PRODUCT_FLOWING_VELOCITY)
                                                         |(1 << DISP_ALARM_PART1_LOWER_RO_WASTE_FLOWING_VELOCITY)
                                                         |(1 << DISP_ALARM_PART1_LOWER_CIR_WATER_CONDUCTIVITY)
                                                         |(1 << DISP_ALARM_PART1_LOWER_HP_PRODUCT_WATER_CONDUCTIVITY)
                                                         |(1 << DISP_ALARM_PART1_HIGHER_EDI_PRODUCT_TEMPERATURE)
                                                         |(1 << DISP_ALARM_PART1_LOWER_EDI_PRODUCT_TEMPERATURE)
                                                         |(1 << DISP_ALARM_PART1_HIGHER_TUBE_TEMPERATURE)
                                                         |(1 << DISP_ALARM_PART1_LOWER_TUBE_TEMPERATURE)
                                                         |(1 << DISP_ALARM_PART1_HIGHER_TOC_SENSOR_TEMPERATURE)
                                                         |(1 << DISP_ALARM_PART1_LOWER_TOC_SENSOR_TEMPERATURE)
                                                         |(1 << DISP_ALARM_PART1_LOWER_TOC_SOURCE_WATER_RESISTENCE)));
            
            m_bC1Regulator = true;
            break;
        } 
    }

    
    for (iLoop = 0; iLoop < MACHINE_NUM; iLoop++)
    {
        switch(iLoop)
        {
        case MACHINE_L_Genie:

            /*notify masks */
            m_cMas[iLoop].aulMask[0]  = DISP_NOTIFY_DEFAULT ;
            
            m_cMas[iLoop].aulMask[0] &= (~((1 << DISP_H_PACK)
                                         |(1 << DISP_PRE_PACK)));
            
            break;
        case MACHINE_L_UP:
            
            /*notify masks */
            m_cMas[iLoop].aulMask[0]  = DISP_NOTIFY_DEFAULT;
            
            m_cMas[iLoop].aulMask[0] &= ~(1 << DISP_AT_PACK);
            break;
        case MACHINE_L_EDI_LOOP:

            /*notify masks */
            m_cMas[iLoop].aulMask[0]  = DISP_NOTIFY_DEFAULT;

            m_cMas[iLoop].aulMask[0] &= (~((1 << DISP_U_PACK)
                                         |(1 << DISP_N2_UV)));
            break;
        case MACHINE_L_RO_LOOP:
            
            /*notify masks */
            m_cMas[iLoop].aulMask[0]  = DISP_NOTIFY_DEFAULT;

            m_cMas[iLoop].aulMask[0] &= ~(1 << DISP_H_PACK);
            break;

        case MACHINE_Genie:

            /*notify masks */
            m_cMas[iLoop].aulMask[0]  = DISP_NOTIFY_DEFAULT;

            m_cMas[iLoop].aulMask[0] &= (~((1 << DISP_AT_PACK)
                                         |(1 << DISP_H_PACK)
                                         |(1 << DISP_N4_UV)
                                         |(1 << DISP_N5_UV)
                                         |(1 << DISP_TUBE_FILTER)
                                         |(1 << DISP_TUBE_DI)));
            break;

        case MACHINE_UP:
            /*notify masks */
            m_cMas[iLoop].aulMask[0]  = DISP_NOTIFY_DEFAULT;

            m_cMas[iLoop].aulMask[0] &= (~((1 << DISP_AT_PACK)
                                         |(1 << DISP_N1_UV)
                                         |(1 << DISP_N4_UV)
                                         |(1 << DISP_N5_UV)
                                         |(1 << DISP_TUBE_FILTER)
                                         |(1 << DISP_TUBE_DI)));
            break;
        case MACHINE_EDI:

            /*notify masks */
            m_cMas[iLoop].aulMask[0]  = DISP_NOTIFY_DEFAULT;

            m_cMas[iLoop].aulMask[0] &= (~((1 << DISP_AT_PACK)
                                         |(1 << DISP_U_PACK)
                                         |(1 << DISP_H_PACK)
                                         |(1 << DISP_N2_UV)
                                         |(1 << DISP_N4_UV)
                                         |(1 << DISP_N5_UV)
                                         |(1 << DISP_T_B_FILTER) //2018.11.19
                                         |(1 << DISP_TUBE_FILTER)
                                         |(1 << DISP_TUBE_DI)));
            break;

        case MACHINE_RO:

            /*notify masks */
            m_cMas[iLoop].aulMask[0]  = DISP_NOTIFY_DEFAULT;
           //2018
            m_cMas[iLoop].aulMask[0] &= (~((1 << DISP_AT_PACK)
                                         |(1 << DISP_U_PACK)
                                         |(1 << DISP_H_PACK)
                                         |(1 << DISP_N1_UV)
                                         |(1 << DISP_N2_UV)
                                         |(1 << DISP_N4_UV)
                                         |(1 << DISP_N5_UV)
                                         |(1 << DISP_T_B_FILTER) //2018.11.12
                                         |(1 << DISP_TUBE_FILTER)
                                         |(1 << DISP_TUBE_DI)));
            break;

        case MACHINE_PURIST:

            /*notify masks */
            m_cMas[iLoop].aulMask[0]  = DISP_NOTIFY_DEFAULT;

            m_cMas[iLoop].aulMask[0] &= (~((1 << DISP_PRE_PACK)
                                         |(1 << DISP_AC_PACK)
                                         |(1 << DISP_T_PACK)
                                         |(1 << DISP_P_PACK)
                                         |(1 << DISP_AT_PACK)
                                         |(1 << DISP_T_B_FILTER)
                                         |(1 << DISP_N1_UV)
                                         |(1 << DISP_N3_UV)
                                         |(1 << DISP_N4_UV)
                                         |(1 << DISP_N5_UV)
                                         |(1 << DISP_TUBE_FILTER)
                                         |(1 << DISP_TUBE_DI)));
            
            break;

        case MACHINE_ADAPT:
            /*notify masks */
            m_cMas[iLoop].aulMask[0]  = DISP_NOTIFY_DEFAULT;

            m_cMas[iLoop].aulMask[0] &= (~((1 << DISP_AT_PACK)
                                           |(1 << DISP_AC_PACK)
                                           |(1 << DISP_H_PACK)
                                           |(1 << DISP_T_PACK)
                                           |(1 << DISP_N1_UV)
                                           |(1 << DISP_N3_UV)
                                           |(1 << DISP_N4_UV)
                                           |(1 << DISP_N5_UV)
                                           |(1 << DISP_TUBE_FILTER)
                                           |(1 << DISP_TUBE_DI)));
            break;
        } 

        if(!(gGlobalParam.MiscParam.ulMisFlags & (1 << DISP_SM_HP_Water_Cir)))
        {
            m_cMas[iLoop].aulMask[0] &= ~(1 <<  DISP_T_PACK);
        }

      // if (gGlobalParam.SubModSetting.ulFlags & DISP_SM_Pre_Filter) //DISP_SM_PreFilterColumn
       if(!(gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_Pre_Filter)))
       {
           m_cMas[iLoop].aulMask[0] &= ~(1 << DISP_PRE_PACK);
       }

      // if (gGlobalParam.SubModSetting.ulFlags & DISP_SM_TubeUV)
       if (!(gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_TubeUV)))
       {
           m_cMas[iLoop].aulMask[0] &= ~(1 << DISP_N4_UV);        
       }

       //if (gGlobalParam.SubModSetting.ulFlags & DISP_SM_HaveTubeFilter)
       if (!(gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_HaveTubeFilter)))
       {
           m_cMas[iLoop].aulMask[0] &= ~(1 << DISP_TUBE_FILTER);                 
       }
    
       //if (gGlobalParam.SubModSetting.ulFlags & DISP_SM_TubeDI)
       if (!(gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_TubeDI)))
       {
           m_cMas[iLoop].aulMask[0] &= ~(1 << DISP_TUBE_DI);
       }
    }

    checkCMParam(); //2019.6.17 add

    initUI();  
    
    /* other init */
    m_iLstDay = DispGetDay();

    mQFALARM.setPointSize(18);
    mQPALARM.setColor(QPalette::WindowText,Qt::red);


    m_timeTimer = new QTimer(this);
    connect(m_timeTimer, SIGNAL(timeout()), this, SLOT(on_timerEvent()));
    m_timeTimer->start(1000 * 30); // peroid of half minute

    m_timerPeriodEvent = new QTimer(this);
    connect(m_timerPeriodEvent, SIGNAL(timeout()), this, SLOT(on_timerPeriodEvent()),Qt::QueuedConnection);
    m_timerPeriodEvent->start(PERIOD_EVENT_LENGTH); // peroid of one second
    m_periodEvents = 0;

    m_timeSecondTimer = new QTimer(this);
    connect(m_timeSecondTimer, SIGNAL(timeout()), this, SLOT(on_timerSecondEvent()),Qt::QueuedConnection);
    m_timeSecondTimer->start(SECOND_EVENT_LENGTH); // peroid of one second

    connect(this, SIGNAL(dispIndication(unsigned char *,int)),
     this, SLOT(on_dispIndication(unsigned char *,int))/*,Qt::DirectConnection */);

    connect(this, SIGNAL(iapIndication(IAP_NOTIFY_STRU *)),
     this, SLOT(on_IapIndication(IAP_NOTIFY_STRU *)));

    connect(this, SIGNAL(autoLogin()),
     this, SLOT(on_AutoLogin()),Qt::QueuedConnection);

    m_timerBuzzer = new QTimer(this);
    connect(m_timerBuzzer, SIGNAL(timeout()), this, SLOT(on_timerBuzzerEvent()),Qt::QueuedConnection);

    m_fd4buzzer = ::open(BUZZER_FILE, O_RDWR);

    if (m_fd4buzzer < 0)
    {
        qDebug() << " open " << BUZZER_FILE << "failed" << endl;
    }

    m_pOptDlg    = new COptDlg(this);
    //m_pDeviceDlg = new DeviceDlg(this);

    CheckConsumptiveMaterialState();

    updateTubeCirCfg();

    /* 2017/12/26 add begin : for rfid and machine type map */
    memset(&MacRfidMap,0,sizeof(MacRfidMap));
    
    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
        
        MacRfidMap.ulMask4Normlwork |= (1 << APP_RFID_SUB_TYPE_PPACK_CLEANPACK);
        MacRfidMap.aiDeviceType4Normal[APP_RFID_SUB_TYPE_PPACK_CLEANPACK] = DISP_P_PACK;
        
        MacRfidMap.ulMask4Normlwork |= (1 << APP_RFID_SUB_TYPE_HPACK_ATPACK);
        MacRfidMap.aiDeviceType4Normal[APP_RFID_SUB_TYPE_HPACK_ATPACK] = DISP_AT_PACK;

        MacRfidMap.ulMask4Normlwork |= (1 << APP_RFID_SUB_TYPE_UPACK_HPACK);
        MacRfidMap.aiDeviceType4Normal[APP_RFID_SUB_TYPE_UPACK_HPACK] = DISP_U_PACK;
        
//        if (gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_Pre_Filter))
//        {
//            MacRfidMap.ulMask4Normlwork |= (1 << APP_RFID_SUB_TYPE_PREPACK);
//            MacRfidMap.aiDeviceType4Normal[APP_RFID_SUB_TYPE_PREPACK] = DISP_PRE_PACK;
//        }

        MacRfidMap.ulMask4Normlwork |= (1 << APP_RFID_SUB_TYPE_ROPACK_OTHERS);
        MacRfidMap.aiDeviceType4Normal[APP_RFID_SUB_TYPE_ROPACK_OTHERS] = DISP_AC_PACK;

        /*rfid for cleaning stage */
        MacRfidMap.ulMask4Cleaning |= (1 << APP_RFID_SUB_TYPE_PPACK_CLEANPACK);
        MacRfidMap.aiDeviceType4Cleaning[APP_RFID_SUB_TYPE_PPACK_CLEANPACK] = DISP_P_PACK | (1 << 16);
        
        MacRfidMap.ulMask4Cleaning |= (1 << APP_RFID_SUB_TYPE_HPACK_ATPACK);
        MacRfidMap.aiDeviceType4Cleaning[APP_RFID_SUB_TYPE_HPACK_ATPACK] = DISP_AT_PACK;

        MacRfidMap.ulMask4Cleaning |= (1 << APP_RFID_SUB_TYPE_UPACK_HPACK);
        MacRfidMap.aiDeviceType4Cleaning[APP_RFID_SUB_TYPE_UPACK_HPACK] = DISP_U_PACK;
        
//        if (gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_Pre_Filter))
//        {
//            MacRfidMap.ulMask4Normlwork |= (1 << APP_RFID_SUB_TYPE_PREPACK);
//            MacRfidMap.aiDeviceType4Normal[APP_RFID_SUB_TYPE_PREPACK] = DISP_PRE_PACK;
//        }

        MacRfidMap.ulMask4Normlwork |= (1 << APP_RFID_SUB_TYPE_ROPACK_OTHERS);
        MacRfidMap.aiDeviceType4Normal[APP_RFID_SUB_TYPE_ROPACK_OTHERS] = DISP_AC_PACK;
        break;
    case MACHINE_L_UP:
        
        MacRfidMap.ulMask4Normlwork |= (1 << APP_RFID_SUB_TYPE_PPACK_CLEANPACK);
        MacRfidMap.aiDeviceType4Normal[APP_RFID_SUB_TYPE_PPACK_CLEANPACK] = DISP_P_PACK;
        
        MacRfidMap.ulMask4Normlwork |= (1 << APP_RFID_SUB_TYPE_HPACK_ATPACK);
        MacRfidMap.aiDeviceType4Normal[APP_RFID_SUB_TYPE_HPACK_ATPACK] = DISP_H_PACK;

        MacRfidMap.ulMask4Normlwork |= (1 << APP_RFID_SUB_TYPE_UPACK_HPACK);
        MacRfidMap.aiDeviceType4Normal[APP_RFID_SUB_TYPE_UPACK_HPACK] = DISP_U_PACK;
        
//        if (gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_Pre_Filter))
//        {
//            MacRfidMap.ulMask4Normlwork |= (1 << APP_RFID_SUB_TYPE_PREPACK);
//            MacRfidMap.aiDeviceType4Normal[APP_RFID_SUB_TYPE_PREPACK] = DISP_PRE_PACK;
//        }

        MacRfidMap.ulMask4Normlwork |= (1 << APP_RFID_SUB_TYPE_ROPACK_OTHERS);
        MacRfidMap.aiDeviceType4Normal[APP_RFID_SUB_TYPE_ROPACK_OTHERS] = DISP_AC_PACK;

        /*rfid for cleaning stage */
        MacRfidMap.ulMask4Cleaning |= (1 << APP_RFID_SUB_TYPE_PPACK_CLEANPACK);
        MacRfidMap.aiDeviceType4Cleaning[APP_RFID_SUB_TYPE_PPACK_CLEANPACK] = DISP_P_PACK | (1 << 16);
        
        MacRfidMap.ulMask4Cleaning |= (1 << APP_RFID_SUB_TYPE_HPACK_ATPACK);
        MacRfidMap.aiDeviceType4Cleaning[APP_RFID_SUB_TYPE_HPACK_ATPACK] = DISP_H_PACK;

        MacRfidMap.ulMask4Cleaning |= (1 << APP_RFID_SUB_TYPE_UPACK_HPACK);
        MacRfidMap.aiDeviceType4Cleaning[APP_RFID_SUB_TYPE_UPACK_HPACK] = DISP_U_PACK;
        
//        if (gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_Pre_Filter))
//        {
//            MacRfidMap.ulMask4Normlwork |= (1 << APP_RFID_SUB_TYPE_PREPACK);
//            MacRfidMap.aiDeviceType4Normal[APP_RFID_SUB_TYPE_PREPACK] = DISP_PRE_PACK;
//        }

        MacRfidMap.ulMask4Normlwork |= (1 << APP_RFID_SUB_TYPE_ROPACK_OTHERS);
        MacRfidMap.aiDeviceType4Normal[APP_RFID_SUB_TYPE_ROPACK_OTHERS] = DISP_AC_PACK;
        break;
    case MACHINE_L_EDI_LOOP:
        
        MacRfidMap.ulMask4Normlwork |= (1 << APP_RFID_SUB_TYPE_PPACK_CLEANPACK);
        MacRfidMap.aiDeviceType4Normal[APP_RFID_SUB_TYPE_PPACK_CLEANPACK] = DISP_P_PACK;
        
        MacRfidMap.ulMask4Normlwork |= (1 << APP_RFID_SUB_TYPE_HPACK_ATPACK);
        MacRfidMap.aiDeviceType4Normal[APP_RFID_SUB_TYPE_HPACK_ATPACK] = DISP_AT_PACK;

        MacRfidMap.ulMask4Normlwork |= (1 << APP_RFID_SUB_TYPE_UPACK_HPACK);
        MacRfidMap.aiDeviceType4Normal[APP_RFID_SUB_TYPE_UPACK_HPACK] = DISP_H_PACK;
        
//        if (gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_Pre_Filter))
//        {
//            MacRfidMap.ulMask4Normlwork |= (1 << APP_RFID_SUB_TYPE_PREPACK);
//            MacRfidMap.aiDeviceType4Normal[APP_RFID_SUB_TYPE_PREPACK] = DISP_PRE_PACK;
//        }

        MacRfidMap.ulMask4Normlwork |= (1 << APP_RFID_SUB_TYPE_ROPACK_OTHERS);
        MacRfidMap.aiDeviceType4Normal[APP_RFID_SUB_TYPE_ROPACK_OTHERS] = DISP_AC_PACK;

        /*rfid for cleaning stage */
        MacRfidMap.ulMask4Cleaning |= (1 << APP_RFID_SUB_TYPE_PPACK_CLEANPACK);
        MacRfidMap.aiDeviceType4Cleaning[APP_RFID_SUB_TYPE_PPACK_CLEANPACK] = DISP_P_PACK | (1 << 16);
        
        MacRfidMap.ulMask4Cleaning |= (1 << APP_RFID_SUB_TYPE_HPACK_ATPACK);
        MacRfidMap.aiDeviceType4Cleaning[APP_RFID_SUB_TYPE_HPACK_ATPACK] = DISP_AT_PACK;

        MacRfidMap.ulMask4Cleaning |= (1 << APP_RFID_SUB_TYPE_UPACK_HPACK);
        MacRfidMap.aiDeviceType4Cleaning[APP_RFID_SUB_TYPE_UPACK_HPACK] = DISP_H_PACK;
        
//        if (gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_Pre_Filter))
//        {
//            MacRfidMap.ulMask4Normlwork |= (1 << APP_RFID_SUB_TYPE_PREPACK);
//            MacRfidMap.aiDeviceType4Normal[APP_RFID_SUB_TYPE_PREPACK] = DISP_PRE_PACK;
//        }

        MacRfidMap.ulMask4Normlwork |= (1 << APP_RFID_SUB_TYPE_ROPACK_OTHERS);
        MacRfidMap.aiDeviceType4Normal[APP_RFID_SUB_TYPE_ROPACK_OTHERS] = DISP_AC_PACK;
        break;
    case MACHINE_L_RO_LOOP:
        
        MacRfidMap.ulMask4Normlwork |= (1 << APP_RFID_SUB_TYPE_PPACK_CLEANPACK);
        MacRfidMap.aiDeviceType4Normal[APP_RFID_SUB_TYPE_PPACK_CLEANPACK] = DISP_P_PACK;
        
        MacRfidMap.ulMask4Normlwork |= (1 << APP_RFID_SUB_TYPE_HPACK_ATPACK);
        MacRfidMap.aiDeviceType4Normal[APP_RFID_SUB_TYPE_HPACK_ATPACK] = DISP_H_PACK;

        MacRfidMap.ulMask4Normlwork |= (1 << APP_RFID_SUB_TYPE_UPACK_HPACK);
        MacRfidMap.aiDeviceType4Normal[APP_RFID_SUB_TYPE_UPACK_HPACK] = DISP_U_PACK;
        
//        if (gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_Pre_Filter))
//        {
//            MacRfidMap.ulMask4Normlwork |= (1 << APP_RFID_SUB_TYPE_PREPACK);
//            MacRfidMap.aiDeviceType4Normal[APP_RFID_SUB_TYPE_PREPACK] = DISP_PRE_PACK;
//        }

        MacRfidMap.ulMask4Normlwork |= (1 << APP_RFID_SUB_TYPE_ROPACK_OTHERS);
        MacRfidMap.aiDeviceType4Normal[APP_RFID_SUB_TYPE_ROPACK_OTHERS] = DISP_AC_PACK;

        /*rfid for cleaning stage */
        MacRfidMap.ulMask4Cleaning |= (1 << APP_RFID_SUB_TYPE_PPACK_CLEANPACK);
        MacRfidMap.aiDeviceType4Cleaning[APP_RFID_SUB_TYPE_PPACK_CLEANPACK] = DISP_P_PACK | (1 << 16);
        
        MacRfidMap.ulMask4Cleaning |= (1 << APP_RFID_SUB_TYPE_HPACK_ATPACK);
        MacRfidMap.aiDeviceType4Cleaning[APP_RFID_SUB_TYPE_HPACK_ATPACK] = DISP_H_PACK;

        MacRfidMap.ulMask4Cleaning |= (1 << APP_RFID_SUB_TYPE_UPACK_HPACK);
        MacRfidMap.aiDeviceType4Cleaning[APP_RFID_SUB_TYPE_UPACK_HPACK] = DISP_U_PACK;
        
//        if (gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_Pre_Filter))
//        {
//            MacRfidMap.ulMask4Normlwork |= (1 << APP_RFID_SUB_TYPE_PREPACK);
//            MacRfidMap.aiDeviceType4Normal[APP_RFID_SUB_TYPE_PREPACK] = DISP_PRE_PACK;
//        }

        MacRfidMap.ulMask4Normlwork |= (1 << APP_RFID_SUB_TYPE_ROPACK_OTHERS);
        MacRfidMap.aiDeviceType4Normal[APP_RFID_SUB_TYPE_ROPACK_OTHERS] = DISP_AC_PACK;
        break;
    case MACHINE_Genie:
        MacRfidMap.ulMask4Normlwork |= (1 << APP_RFID_SUB_TYPE_PPACK_CLEANPACK);
        MacRfidMap.aiDeviceType4Normal[APP_RFID_SUB_TYPE_PPACK_CLEANPACK] = DISP_P_PACK;
#if 0
        MacRfidMap.ulMask4Normlwork |= (1 << APP_RFID_SUB_TYPE_HPACK_ATPACK);
        MacRfidMap.aiDeviceType4Normal[APP_RFID_SUB_TYPE_HPACK_ATPACK] = DISP_H_PACK;
#endif

        MacRfidMap.ulMask4Normlwork |= (1 << APP_RFID_SUB_TYPE_UPACK_HPACK);
        MacRfidMap.aiDeviceType4Normal[APP_RFID_SUB_TYPE_UPACK_HPACK] = DISP_U_PACK;

        MacRfidMap.ulMask4Normlwork |= (1 << APP_RFID_SUB_TYPE_ROPACK_OTHERS);
        MacRfidMap.aiDeviceType4Normal[APP_RFID_SUB_TYPE_ROPACK_OTHERS] = DISP_AC_PACK;

//        if (gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_Pre_Filter))
//        {
//            MacRfidMap.ulMask4Normlwork |= (1 << APP_RFID_SUB_TYPE_PREPACK);
//            MacRfidMap.aiDeviceType4Normal[APP_RFID_SUB_TYPE_PREPACK] = DISP_PRE_PACK;
//        }

        /*rfid for cleaning stage */
        MacRfidMap.ulMask4Cleaning |= (1 << APP_RFID_SUB_TYPE_PPACK_CLEANPACK);
        MacRfidMap.aiDeviceType4Cleaning[APP_RFID_SUB_TYPE_PPACK_CLEANPACK] = DISP_P_PACK | (1 << 16);
        
//        MacRfidMap.ulMask4Cleaning |= (1 << APP_RFID_SUB_TYPE_HPACK_ATPACK);
//        MacRfidMap.aiDeviceType4Cleaning[APP_RFID_SUB_TYPE_HPACK_ATPACK] = DISP_H_PACK;

        MacRfidMap.ulMask4Cleaning |= (1 << APP_RFID_SUB_TYPE_UPACK_HPACK);
        MacRfidMap.aiDeviceType4Cleaning[APP_RFID_SUB_TYPE_UPACK_HPACK] = DISP_U_PACK;
        
//        if (gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_Pre_Filter))
//        {
//            MacRfidMap.ulMask4Normlwork |= (1 << APP_RFID_SUB_TYPE_PREPACK);
//            MacRfidMap.aiDeviceType4Normal[APP_RFID_SUB_TYPE_PREPACK] = DISP_PRE_PACK;
//        }

        MacRfidMap.ulMask4Normlwork |= (1 << APP_RFID_SUB_TYPE_ROPACK_OTHERS);
        MacRfidMap.aiDeviceType4Normal[APP_RFID_SUB_TYPE_ROPACK_OTHERS] = DISP_AC_PACK;
        break;
    case MACHINE_UP:
        MacRfidMap.ulMask4Normlwork |= (1 << APP_RFID_SUB_TYPE_PPACK_CLEANPACK);
        MacRfidMap.aiDeviceType4Normal[APP_RFID_SUB_TYPE_PPACK_CLEANPACK] = DISP_P_PACK;
        
        MacRfidMap.ulMask4Normlwork |= (1 << APP_RFID_SUB_TYPE_HPACK_ATPACK);
        MacRfidMap.aiDeviceType4Normal[APP_RFID_SUB_TYPE_HPACK_ATPACK] = DISP_H_PACK;

        MacRfidMap.ulMask4Normlwork |= (1 << APP_RFID_SUB_TYPE_UPACK_HPACK);
        MacRfidMap.aiDeviceType4Normal[APP_RFID_SUB_TYPE_UPACK_HPACK] = DISP_U_PACK;
        
//        if (gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_Pre_Filter))
//        {
//            MacRfidMap.ulMask4Normlwork |= (1 << APP_RFID_SUB_TYPE_PREPACK);
//            MacRfidMap.aiDeviceType4Normal[APP_RFID_SUB_TYPE_PREPACK] = DISP_PRE_PACK;
//        }

        MacRfidMap.ulMask4Normlwork |= (1 << APP_RFID_SUB_TYPE_ROPACK_OTHERS);
        MacRfidMap.aiDeviceType4Normal[APP_RFID_SUB_TYPE_ROPACK_OTHERS] = DISP_AC_PACK;

        /*rfid for cleaning stage */
        MacRfidMap.ulMask4Cleaning |= (1 << APP_RFID_SUB_TYPE_PPACK_CLEANPACK);
        MacRfidMap.aiDeviceType4Cleaning[APP_RFID_SUB_TYPE_PPACK_CLEANPACK] = DISP_P_PACK | (1 << 16);
        
        MacRfidMap.ulMask4Cleaning |= (1 << APP_RFID_SUB_TYPE_HPACK_ATPACK);
        MacRfidMap.aiDeviceType4Cleaning[APP_RFID_SUB_TYPE_HPACK_ATPACK] = DISP_H_PACK;

        MacRfidMap.ulMask4Cleaning |= (1 << APP_RFID_SUB_TYPE_UPACK_HPACK);
        MacRfidMap.aiDeviceType4Cleaning[APP_RFID_SUB_TYPE_UPACK_HPACK] = DISP_U_PACK;
        
//        if (gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_Pre_Filter))
//        {
//            MacRfidMap.ulMask4Normlwork |= (1 << APP_RFID_SUB_TYPE_PREPACK);
//            MacRfidMap.aiDeviceType4Normal[APP_RFID_SUB_TYPE_PREPACK] = DISP_PRE_PACK;
//        }

        MacRfidMap.ulMask4Normlwork |= (1 << APP_RFID_SUB_TYPE_ROPACK_OTHERS);
        MacRfidMap.aiDeviceType4Normal[APP_RFID_SUB_TYPE_ROPACK_OTHERS] = DISP_AC_PACK;
        break;
    case MACHINE_EDI:
        MacRfidMap.ulMask4Normlwork |= (1 << APP_RFID_SUB_TYPE_PPACK_CLEANPACK);
        MacRfidMap.aiDeviceType4Normal[APP_RFID_SUB_TYPE_PPACK_CLEANPACK] = DISP_P_PACK;
        /*
        MacRfidMap.ulMask4Normlwork |= (1 << APP_RFID_SUB_TYPE_HPACK_ATPACK);
        MacRfidMap.aiDeviceType4Normal[APP_RFID_SUB_TYPE_HPACK_ATPACK] = DISP_H_PACK;
        */
//        if (gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_Pre_Filter))
//        {
//            MacRfidMap.ulMask4Normlwork |= (1 << APP_RFID_SUB_TYPE_PREPACK);
//            MacRfidMap.aiDeviceType4Normal[APP_RFID_SUB_TYPE_PREPACK] = DISP_PRE_PACK;
//        }

        MacRfidMap.ulMask4Normlwork |= (1 << APP_RFID_SUB_TYPE_ROPACK_OTHERS);
        MacRfidMap.aiDeviceType4Normal[APP_RFID_SUB_TYPE_ROPACK_OTHERS] = DISP_AC_PACK;

        /*rfid for cleaning stage */
        MacRfidMap.ulMask4Cleaning |= (1 << APP_RFID_SUB_TYPE_PPACK_CLEANPACK);
        MacRfidMap.aiDeviceType4Cleaning[APP_RFID_SUB_TYPE_PPACK_CLEANPACK] = DISP_P_PACK | (1 << 16);
        /*
        MacRfidMap.ulMask4Cleaning |= (1 << APP_RFID_SUB_TYPE_HPACK_ATPACK);
        MacRfidMap.aiDeviceType4Cleaning[APP_RFID_SUB_TYPE_HPACK_ATPACK] = DISP_H_PACK;
        */
//        if (gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_Pre_Filter))
//        {
//            MacRfidMap.ulMask4Normlwork |= (1 << APP_RFID_SUB_TYPE_PREPACK);
//            MacRfidMap.aiDeviceType4Normal[APP_RFID_SUB_TYPE_PREPACK] = DISP_PRE_PACK;
//        }

        MacRfidMap.ulMask4Normlwork |= (1 << APP_RFID_SUB_TYPE_ROPACK_OTHERS);
        MacRfidMap.aiDeviceType4Normal[APP_RFID_SUB_TYPE_ROPACK_OTHERS] = DISP_AC_PACK;
        break;
    case MACHINE_RO:
        MacRfidMap.ulMask4Normlwork |= (1 << APP_RFID_SUB_TYPE_PPACK_CLEANPACK);
        MacRfidMap.aiDeviceType4Normal[APP_RFID_SUB_TYPE_PPACK_CLEANPACK] = DISP_P_PACK;
      /*
        MacRfidMap.ulMask4Normlwork |= (1 << APP_RFID_SUB_TYPE_HPACK_ATPACK);
        MacRfidMap.aiDeviceType4Normal[APP_RFID_SUB_TYPE_HPACK_ATPACK] = DISP_H_PACK;
        */
//        if (gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_Pre_Filter))
//        {
//            MacRfidMap.ulMask4Normlwork |= (1 << APP_RFID_SUB_TYPE_PREPACK);
//            MacRfidMap.aiDeviceType4Normal[APP_RFID_SUB_TYPE_PREPACK] = DISP_PRE_PACK;
//        }

        MacRfidMap.ulMask4Normlwork |= (1 << APP_RFID_SUB_TYPE_ROPACK_OTHERS);
        MacRfidMap.aiDeviceType4Normal[APP_RFID_SUB_TYPE_ROPACK_OTHERS] = DISP_AC_PACK;

        /*rfid for cleaning stage */
        MacRfidMap.ulMask4Cleaning |= (1 << APP_RFID_SUB_TYPE_PPACK_CLEANPACK);
        MacRfidMap.aiDeviceType4Cleaning[APP_RFID_SUB_TYPE_PPACK_CLEANPACK] = DISP_P_PACK | (1 << 16);
        /*
        MacRfidMap.ulMask4Cleaning |= (1 << APP_RFID_SUB_TYPE_HPACK_ATPACK);
        MacRfidMap.aiDeviceType4Cleaning[APP_RFID_SUB_TYPE_HPACK_ATPACK] = DISP_H_PACK;
        */
//        if (gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_Pre_Filter))
//        {
//            MacRfidMap.ulMask4Normlwork |= (1 << APP_RFID_SUB_TYPE_PREPACK);
//            MacRfidMap.aiDeviceType4Normal[APP_RFID_SUB_TYPE_PREPACK] = DISP_PRE_PACK;
//        }

        MacRfidMap.ulMask4Normlwork |= (1 << APP_RFID_SUB_TYPE_ROPACK_OTHERS);
        MacRfidMap.aiDeviceType4Normal[APP_RFID_SUB_TYPE_ROPACK_OTHERS] = DISP_AC_PACK;
        break;
    case MACHINE_PURIST:
        MacRfidMap.ulMask4Normlwork |= (1 << APP_RFID_SUB_TYPE_UPACK_HPACK);
        MacRfidMap.aiDeviceType4Normal[APP_RFID_SUB_TYPE_UPACK_HPACK] = DISP_U_PACK;
        
        MacRfidMap.ulMask4Normlwork |= (1 << APP_RFID_SUB_TYPE_HPACK_ATPACK);
        MacRfidMap.aiDeviceType4Normal[APP_RFID_SUB_TYPE_HPACK_ATPACK] = DISP_H_PACK;

        /*rfid for cleaning stage */
        MacRfidMap.ulMask4Cleaning = 0;
        
        break;
    case MACHINE_ADAPT:
        MacRfidMap.ulMask4Normlwork |= (1 << APP_RFID_SUB_TYPE_PPACK_CLEANPACK);
        MacRfidMap.aiDeviceType4Normal[APP_RFID_SUB_TYPE_PPACK_CLEANPACK] = DISP_P_PACK;

        MacRfidMap.ulMask4Normlwork |= (1 << APP_RFID_SUB_TYPE_UPACK_HPACK);
        MacRfidMap.aiDeviceType4Normal[APP_RFID_SUB_TYPE_UPACK_HPACK] = DISP_U_PACK;
        
//        if (gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_Pre_Filter))
//        {
//            MacRfidMap.ulMask4Normlwork |= (1 << APP_RFID_SUB_TYPE_PREPACK);
//            MacRfidMap.aiDeviceType4Normal[APP_RFID_SUB_TYPE_PREPACK] = DISP_PRE_PACK;
//        }


        /* rfid for cleaning stage */
        MacRfidMap.ulMask4Cleaning |= (1 << APP_RFID_SUB_TYPE_PPACK_CLEANPACK);
        MacRfidMap.aiDeviceType4Cleaning[APP_RFID_SUB_TYPE_PPACK_CLEANPACK] = DISP_P_PACK | (1 << 16);
        

        MacRfidMap.ulMask4Cleaning |= (1 << APP_RFID_SUB_TYPE_UPACK_HPACK);
        MacRfidMap.aiDeviceType4Cleaning[APP_RFID_SUB_TYPE_UPACK_HPACK] = DISP_U_PACK;
        
//        if (gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_Pre_Filter))
//        {
//            MacRfidMap.ulMask4Normlwork |= (1 << APP_RFID_SUB_TYPE_PREPACK);
//            MacRfidMap.aiDeviceType4Normal[APP_RFID_SUB_TYPE_PREPACK] = DISP_PRE_PACK;
//        }
        
        break;
    } 

    m_iRfidBufferActiveMask = MacRfidMap.ulMask4Normlwork;

    for (iLoop = 0; iLoop < APP_RFID_SUB_TYPE_NUM; iLoop++)
    {
       if (m_iRfidBufferActiveMask & (1 << iLoop))
       {
           addRfid2DelayList(iLoop);
       }
    }

    //ex
    for(iLoop = 0; iLoop < APP_RFID_SUB_TYPE_NUM; iLoop++)
    {
        m_checkConsumaleInstall[iLoop] = new Ex_CheckConsumaleInstall(iLoop, this);
        m_consumaleInstallDialog[iLoop] = new Ex_ConsumableInstallDialog(iLoop, this);
        m_consumaleInstallDialog[iLoop]->hide();
        m_consumaleInstallDialog[iLoop]->setGeometry(200, 175, 350, 250);

        connect(m_checkConsumaleInstall[iLoop], SIGNAL(consumableMsg(int,QString,QString)),
            m_consumaleInstallDialog[iLoop], SLOT(setConsumableName(int,QString,QString)));

        connect(m_consumaleInstallDialog[iLoop], SIGNAL(installConusumable()),
            m_checkConsumaleInstall[iLoop], SLOT(updateConsumaleMsg()));

        connect(m_consumaleInstallDialog[iLoop], SIGNAL(setCheckStatus(bool)),
            m_checkConsumaleInstall[iLoop], SLOT(setBusystatus(bool)));

        connect(m_consumaleInstallDialog[iLoop], SIGNAL(consumableTypeChanged(int)),
                m_checkConsumaleInstall[iLoop], SLOT(updateConsumableType(int)));
    }


    for(iLoop = 0; iLoop < EX_RECT_NUM; iLoop++)
    {
        ex_gCcb.Ex_Rect_State.EX_N_NO[iLoop] = 0;
    }
    ex_gCcb.Ex_Alarm_Bit.bit1AlarmN1 = 0;
    ex_gCcb.Ex_Alarm_Bit.bit1AlarmN2 = 0;
    ex_gCcb.Ex_Alarm_Bit.bit1AlarmN3 = 0;

    ex_gCcb.Ex_Alarm_Bit.bit1AlarmDelayN1 = 0;
    ex_gCcb.Ex_Alarm_Bit.bit1AlarmDelayN2 = 0;
    ex_gCcb.Ex_Alarm_Bit.bit1AlarmDelayN3 = 0;

    ex_gCcb.EX_Check_State.bit1CheckDecPressure = 0;
    //end

    CcbInit();

    if(ex_gGlobalParam.Ex_Default == 0)
    {
        m_startCheckConsumale = false;
        m_pExInitPages[Ex_Init_Lan]->show(true);    
    }
    else
    {
        m_startCheckConsumale = true;
#ifdef VWR
        mainDisplay();
#else
        Splash();
#endif
    }
    initScreenSleep(); //ex
    initMachineFlow();  //ex 2018.11.19

    QTimer::singleShot(3000, this, SLOT(retriveCMInfoWithRFID()));

#ifdef D_HTTPWORK
    for(int i = 0; i < HTTP_NOTIFY_NUM; i++)
    {
        m_conAlarmMark[i] = false;
    }
    initHttpWorker();
#endif
}

void MainWindow::on_timerBuzzerEvent()
{
    buzzerHandle();
}

void MainWindow::setRelay(int iIdx,int iEnable)
{
    if (m_fd4buzzer != -1)
    {
        char aibuf[2];
        int iRet;

        aibuf[0] = iIdx;
        aibuf[1] = iEnable;

        iRet = ::write(m_fd4buzzer,aibuf,sizeof(aibuf));

        if (iRet != sizeof(aibuf))
        {
            qDebug() << " setRelay fail:" << iRet ;     
        }

    }
}

void MainWindow::startBuzzer()
{
    Alarm.m_bActive = true;
    Alarm.m_bDuty   = false;
    buzzerHandle();
}

void MainWindow::stopBuzzer()
{
    Alarm.m_bActive= false;
    buzzerHandle();
}

void MainWindow::prepareAlarmNormal()
{
    Alarm.m_iAlarmPeriod = 1000; // ms
    Alarm.m_iAlarmDuty   = 50; // ms
    Alarm.m_iAlarmNum    = 5; // ms
    startBuzzer();
}

void MainWindow::prepareAlarmAlarm()
{
    Alarm.m_iAlarmPeriod = 1000; // ms
    Alarm.m_iAlarmDuty   = 100; // ms
    Alarm.m_iAlarmNum    = 5; // ms
    startBuzzer();
}

void MainWindow::prepareAlarmFault()
{
    Alarm.m_iAlarmPeriod = 1000; // ms
    Alarm.m_iAlarmDuty   = 100; // ms
    Alarm.m_iAlarmNum    = 0X7FFFFFFF; // ms
    startBuzzer();
}

void MainWindow::stopAlarm()
{
    Alarm.m_bActive = false;
    Alarm.m_bDuty   = false;
    setRelay(BUZZER_CTL_BUZZ,0);
    m_timerBuzzer->stop();
}

void MainWindow::prepareKeyStroke()
{
    if (gGlobalParam.MiscParam.iSoundMask & (1 << DISPLAY_SOUND_KEY_NOTIFY))
    {
        Alarm.m_iAlarmPeriod = 100; // ms
        Alarm.m_iAlarmDuty   = 50; // ms
        Alarm.m_iAlarmNum    = 1; // ms
        startBuzzer();
    }
}

void MainWindow::buzzerHandle()
{
    int iTimerLen  ;
    
    if (!Alarm.m_bActive) 
    {
        setRelay(BUZZER_CTL_BUZZ,0);
        m_timerBuzzer->stop();
        return ;
    }
    
    if (!Alarm.m_iAlarmPeriod)
    {
        setRelay(BUZZER_CTL_BUZZ,0);
        m_timerBuzzer->stop();
        return ;
    }

    if (Alarm.m_iAlarmNum <= 0)
    {
        setRelay(BUZZER_CTL_BUZZ,0);
        m_timerBuzzer->stop();
        return ;
    }  

    m_timerBuzzer->stop(); 
    
    do {
    
        if (!Alarm.m_bDuty)
        {
            Alarm.m_iAlarmNum--;
        }
    
        Alarm.m_bDuty = !Alarm.m_bDuty;
    
        /* get cycle */
    
        if (Alarm.m_bDuty)
        {
            iTimerLen = (Alarm.m_iAlarmDuty * Alarm.m_iAlarmPeriod) / 100 ;
        }
        else
        {
            iTimerLen = ( (100 - Alarm.m_iAlarmDuty) * Alarm.m_iAlarmPeriod) / 100 ;
        }
        
    }while(iTimerLen == 0);
    
    if (Alarm.m_bDuty)
    {
        setRelay(BUZZER_CTL_BUZZ,1);
    }
    else
    {
        setRelay(BUZZER_CTL_BUZZ,0);
    }
    
    m_timerBuzzer->start(iTimerLen); 

}

void MainWindow::mainDisplay()
{
#ifndef VWR
    m_pMovieGif->stop();
    m_pLabelGif->hide();

    delete m_pLabelGif;
    delete m_pMovieGif;
#endif
    m_bSplash = false;

    m_pSubPages[PAGE_MAIN]->show(true);

    if(ex_gGlobalParam.Ex_Default)
    {
        QTimer::singleShot(1000, this, SLOT(retriveLastRunState()));
    }
}

void MainWindow::updEcoInfo(int index)
{

    if (NULL != m_pSubPages[PAGE_MAIN])
    {
        MainPage *page = (MainPage *)m_pSubPages[PAGE_MAIN];
        page->updEcoInfo(index,&m_EcoInfo[index]);
    }
#ifdef FLOWCHART
    if (NULL != m_pSubPages[PAGE_FLOWCHART])
    {
        Ex_FlowChartPage *page = (Ex_FlowChartPage *)m_pSubPages[PAGE_FLOWCHART];
        page->updEcoInfo(index, &m_EcoInfo[index]);
    }
#endif
    if (NULL != m_pSubPages[PAGE_MENU])
    {
        MenuPage *page = (MenuPage *)m_pSubPages[PAGE_MENU];

        WaterQualityPage *subpage = (WaterQualityPage *)page->getSubPage(MENU_BTN_WATER_QUALITY_PARAMETER);

        subpage->updEcoInfo(index,&m_EcoInfo[index]);
    }

    if (APP_EXE_I1_NO == index)
    {
        if (DispGetInitRunFlag())
        {
            m_fSourceWaterConductivity = m_EcoInfo[index].fQuality;
        }
    }

#ifdef D_HTTPWORK
    m_networkData.waterQuality[index].fG25x = m_EcoInfo[index].fQuality;
    m_networkData.waterQuality[index].tx = m_EcoInfo[index].fTemperature;
    if(index == APP_EXE_I2_NO)
    {
         DispGetREJ(&m_networkData.fResidue);
    }
#endif
}

void MainWindow::updTank()
{
    /* calc */
    float liter = (m_fPressure[APP_EXE_PM2_NO]/100)*gGlobalParam.PmParam.afCap[APP_EXE_PM2_NO];
    int   level = (int)((liter*100) / gGlobalParam.PmParam.afCap[APP_EXE_PM2_NO]);
#ifdef FLOWCHART
    if (NULL != m_pSubPages[PAGE_FLOWCHART])
    {
        Ex_FlowChartPage *page = (Ex_FlowChartPage *)m_pSubPages[PAGE_FLOWCHART];
        page->updTank(level, liter);
    }
#endif
    if (NULL != m_pSubPages[PAGE_MAIN])
    {
        MainPage *page = (MainPage *)m_pSubPages[PAGE_MAIN];
        page->updTank(level,liter);
    }

    if (NULL != m_pSubPages[PAGE_MENU])
    {
        MenuPage *page = (MenuPage *)m_pSubPages[PAGE_MENU];

        WaterQualityPage *subpage =(WaterQualityPage *)page->getSubPage(MENU_BTN_WATER_QUALITY_PARAMETER);
        
        subpage->updTank(level,liter);
    }   
    //ex
    if (NULL != m_pSubPages[PAGE_MENU])
    {
        MenuPage *page = (MenuPage *)m_pSubPages[PAGE_MENU];

        SystemMonitorPage *subpage =(SystemMonitorPage *)page->getSubPage(MENU_BTN_SYSTEM_MONITOR);

        subpage->updateTankLevel(level);
    }

    if (NULL != m_pSubPages[PAGE_SET])
    {
        SetPage *page = (SetPage *)m_pSubPages[PAGE_SET];

        Ex_FactoryTestPage *subpage =(Ex_FactoryTestPage *)page->getSubPage(SET_BTN_SYSTEM_FACTORYTEST);

        subpage->updTank(level,liter);
    }
    //end
    if (abs(m_iLevel - level) >= 1)
    {
        m_iLevel = level;

        DispSndHoPpbAndTankLevel(APP_PROTOL_CANID_BROADCAST,APP_PACKET_HO_QL_TYPE_LEVEL,m_iLevel,0);
    }
}

void MainWindow::updSoureTank()
{
    /* calc */
    float liter = (m_fPressure[APP_EXE_PM3_NO]/100)*gGlobalParam.PmParam.afCap[APP_EXE_PM3_NO];
    int   level = (int)((liter*100) / gGlobalParam.PmParam.afCap[APP_EXE_PM3_NO]);

    if (NULL != m_pSubPages[PAGE_SET])
    {
        SetPage *page = (SetPage *)m_pSubPages[PAGE_SET];

        Ex_FactoryTestPage *subpage =(Ex_FactoryTestPage *)page->getSubPage(SET_BTN_SYSTEM_FACTORYTEST);

        subpage->updSoureTank(level,liter);
    }
}

void MainWindow::updQtwState(int iType,bool bStart)
{
    MainPage *pPage = (MainPage *)m_pSubPages[PAGE_MAIN];

    if (pPage)
    {
       pPage->updQtwState(iType,bStart);
    }

}

void MainWindow::updPressure(int iIdx)
{

    switch(iIdx)
    {
    case APP_EXE_PM1_NO:
    {
        if (NULL != m_pSubPages[PAGE_MENU])
        {
            MenuPage *page = (MenuPage *)m_pSubPages[PAGE_MENU];
        
            WaterQualityPage *subpage = (WaterQualityPage *)page->getSubPage(MENU_BTN_WATER_QUALITY_PARAMETER);

            subpage->updPressure(iIdx, m_fPressure[iIdx]);

        }
        if (NULL != m_pSubPages[PAGE_SET])
        {
            SetPage *page = (SetPage *)m_pSubPages[PAGE_SET];

            Ex_FactoryTestPage *subpage =(Ex_FactoryTestPage *)page->getSubPage(SET_BTN_SYSTEM_FACTORYTEST);

            subpage->updatePressure(iIdx, m_fPressure[iIdx]);
        }
#ifdef FLOWCHART
        if (NULL != m_pSubPages[PAGE_FLOWCHART])
        {
            Ex_FlowChartPage *page = (Ex_FlowChartPage *)m_pSubPages[PAGE_FLOWCHART];
            page->updPressure(iIdx, m_fPressure[iIdx]);
        }
#endif
        break;
     }
    case APP_EXE_PM2_NO:
        updTank();
        break;

    case APP_EXE_PM3_NO:
        updSoureTank();
        break;
    }

}

void MainWindow::updFlowInfo(int iIdx)
{
    if (NULL != m_pSubPages[PAGE_MENU])
    {
        MenuPage *page = (MenuPage *)m_pSubPages[PAGE_MENU];
    
        WaterQualityPage *subpage =(WaterQualityPage *) page->getSubPage(MENU_BTN_WATER_QUALITY_PARAMETER);
    
        {
            int iTmDelta = m_periodEvents - m_iLstFlowMeterTick[iIdx];
            int iFmDelta;
    
            if ((iTmDelta >= (FM_CALC_PERIOD/PERIOD_EVENT_LENGTH))
                && (m_ulLstFlowMeter[iIdx] != 0))
            {
                iFmDelta = m_ulFlowMeter[iIdx] - m_ulLstFlowMeter[iIdx];
            
                subpage->updFlowInfo(iIdx,(iFmDelta * TOMLPERMIN / iTmDelta));
            }
        }       
        
    }
    if (NULL != m_pSubPages[PAGE_SET])
    {
        SetPage *page = (SetPage *)m_pSubPages[PAGE_SET];

        Ex_FactoryTestPage *subpage =(Ex_FactoryTestPage *) page->getSubPage(SET_BTN_SYSTEM_FACTORYTEST);

        {
            int iTmDelta = m_periodEvents - m_iLstFlowMeterTick[iIdx];
            int iFmDelta;

            if ((iTmDelta >= (FM_CALC_PERIOD/PERIOD_EVENT_LENGTH))
                && (m_ulLstFlowMeter[iIdx] != 0))
            {
                iFmDelta = m_ulFlowMeter[iIdx] - m_ulLstFlowMeter[iIdx];

                subpage->updateFlow(iIdx,(iFmDelta * TOMLPERMIN / iTmDelta));
            }
        }

    }
#ifdef FLOWCHART
    if (NULL != m_pSubPages[PAGE_FLOWCHART])
    {
        Ex_FlowChartPage *page = (Ex_FlowChartPage *)m_pSubPages[PAGE_FLOWCHART];
        {
            int iTmDelta = m_periodEvents - m_iLstFlowMeterTick[iIdx];
            int iFmDelta;

            if ((iTmDelta >= (FM_CALC_PERIOD/PERIOD_EVENT_LENGTH))
                && (m_ulLstFlowMeter[iIdx] != 0))
            {
                iFmDelta = m_ulFlowMeter[iIdx] - m_ulLstFlowMeter[iIdx];

                page->updFlowInfo(iIdx,(iFmDelta * TOMLPERMIN / iTmDelta));
            }
        }

    }
#endif
    if ( (0 == m_ulLstFlowMeter[iIdx]) 
        || (m_periodEvents - m_iLstFlowMeterTick[iIdx] >= ((FM_UPDATE_PERIOD)/PERIOD_EVENT_LENGTH)))
    {
        m_iLstFlowMeterTick[iIdx] = m_periodEvents;
        m_ulLstFlowMeter[iIdx]    = m_ulFlowMeter[iIdx];
    }
}


void MainWindow::initHandler()
{
    QSqlQuery query;

    m_iHandlerMask       = 0;
    m_iHandlerActiveMask = 0;

    query.exec(select_sql_Handler);

    while (query.next())
    {
        int addr = query.value(HANDLER_TBL_FILED_ADDRESS).toInt();

        if (addr >= APP_HAND_SET_BEGIN_ADDRESS && addr < APP_HAND_SET_END_ADDRESS)
        {
            int iIdx = (addr - APP_HAND_SET_BEGIN_ADDRESS);
            
            m_iHandlerMask |= 1 << iIdx;

            m_aHandler[iIdx].address = addr;
            m_aHandler[iIdx].type    = query.value(HANDLER_TBL_FILED_TYPE).toInt();
            m_aHandler[iIdx].def     = query.value(HANDLER_TBL_FILED_DEF).toInt();

            QString str = query.value(HANDLER_TBL_FILED_NAME).toString();
            QString adr = query.value(HANDLER_TBL_FILED_ADDRESS).toString();
            
            strncpy(m_aHandler[iIdx].name,str.toAscii(),APP_SN_LENGTH);

            qDebug() << iIdx << m_aHandler[iIdx].name << addr << m_aHandler[iIdx].type << m_aHandler[iIdx].def;
        }
    }

}

void MainWindow::initRfIdLayout(RF_DATA_LAYOUT_ITEMS *pLayout)
{
    int iLoop;

    /* for RF id */
    pLayout->aItem[RF_DATA_LAYOUT_HEAD_SIZE].size = 4;
    pLayout->aItem[RF_DATA_LAYOUT_HEAD_SIZE].write = TRUE;  
    
    pLayout->aItem[RF_DATA_LAYOUT_PROPERTY_SIZE].size = 4;
    pLayout->aItem[RF_DATA_LAYOUT_PROPERTY_SIZE].write = TRUE;
    
    pLayout->aItem[RF_DATA_LAYOUT_DEVICE].size = 4;
    pLayout->aItem[RF_DATA_LAYOUT_DEVICE].write = TRUE;
    
    pLayout->aItem[RF_DATA_LAYOUT_TYPE].size = 4;
    pLayout->aItem[RF_DATA_LAYOUT_TYPE].write = TRUE;
    
    pLayout->aItem[RF_DATA_LAYOUT_TRADE_MARK].size = 24;
    pLayout->aItem[RF_DATA_LAYOUT_TRADE_MARK].write = TRUE;
    
    pLayout->aItem[RF_DATA_LAYOUT_TRADE_NAME].size = 24;
    pLayout->aItem[RF_DATA_LAYOUT_TRADE_NAME].write = TRUE;
    
    pLayout->aItem[RF_DATA_LAYOUT_CATALOGUE_NUM].size = 24;
    pLayout->aItem[RF_DATA_LAYOUT_CATALOGUE_NUM].write = TRUE;
    
    pLayout->aItem[RF_DATA_LAYOUT_UNKNOW].size = 24;
    pLayout->aItem[RF_DATA_LAYOUT_UNKNOW].write = TRUE;
    
    pLayout->aItem[RF_DATA_LAYOUT_LOT_NUMBER].size = 24;
    pLayout->aItem[RF_DATA_LAYOUT_LOT_NUMBER].write = TRUE;
    
    pLayout->aItem[RF_DATA_LAYOUT_UNKNOW_DATE].size = 8;
    pLayout->aItem[RF_DATA_LAYOUT_UNKNOW_DATE].write = FALSE;
    
    pLayout->aItem[RF_DATA_LAYOUT_INSTALL_DATE].size = 8;
    pLayout->aItem[RF_DATA_LAYOUT_INSTALL_DATE].write = TRUE;
    
    pLayout->aItem[RF_DATA_LAYOUT_UNKNOW_DATA].size = 24;
    pLayout->aItem[RF_DATA_LAYOUT_UNKNOW_DATA].write = FALSE;
    
    // Then for offset
    pLayout->aItem[RF_DATA_LAYOUT_HEAD_SIZE].offset = 0;
    
    for (iLoop = RF_DATA_LAYOUT_PROPERTY_SIZE; iLoop < RF_DATA_LAYOUT_NUM; iLoop++)
    {
        pLayout->aItem[iLoop].offset = pLayout->aItem[iLoop-1].offset + pLayout->aItem[iLoop-1].size;
    }     

}

void MainWindow::initRfid()
{
    QSqlQuery query;

    m_iRfidMask       = 0;
    m_iRfidActiveMask = 0;

    query.exec(select_sql_Rfid);

    while (query.next())
    {
        int addr = query.value(RFID_TBL_FILED_ADDRESS).toInt();

        if (addr >= APP_RF_READER_BEGIN_ADDRESS && addr < APP_RF_READER_END_ADDRESS)
        {
            int iIdx = (addr - APP_RF_READER_BEGIN_ADDRESS);
            
            m_iRfidMask |= 1 << iIdx;

            m_aRfid[iIdx].address = addr;
            m_aRfid[iIdx].type    = query.value(RFID_TBL_FILED_TYPE).toInt();

            QString str = query.value(RFID_TBL_FILED_NAME).toString();
            strncpy(m_aRfid[iIdx].name,str.toAscii(),APP_SN_LENGTH);

            qDebug() << iIdx << m_aRfid[iIdx].name;
            
        }
    }

}

void MainWindow::saveHandler()
{
    int iLoop;
    
    QSqlQuery query;

    bool bDbResult;

    query.exec(delete_sql_Handler);

    for(iLoop = 0 ; iLoop < APP_HAND_SET_MAX_NUMBER ; iLoop++)
    {
        if (m_iHandlerMask & (1 << iLoop))
        {
            query.prepare(INSERT_sql_Handler);
            QString strName = m_aHandler[iLoop].name;
            query.bindValue(":name",   strName);
            query.bindValue(":address",m_aHandler[iLoop].address);
            query.bindValue(":type",   m_aHandler[iLoop].type);
            query.bindValue(":def",    m_aHandler[iLoop].def);
            bDbResult = query.exec();

            qDebug() << m_aHandler[iLoop].name << bDbResult;
        }
    
    }

}
void MainWindow::deleteHandler(SN name)
{   
    int iLoop;

    QSqlQuery query;
    
    query.exec(QString("delete from Handler where name=%1").arg(name));

}

void MainWindow::saveRfid()
{
    int iLoop;
    
    QSqlQuery query;

    query.exec(delete_sql_Rfid);

    for(iLoop = 0 ; iLoop < APP_RF_READER_MAX_NUMBER ; iLoop++)
    {
        if (m_iRfidMask & (1 << iLoop))
        {
            query.prepare(INSERT_sql_Rfid);
            query.bindValue(":name",   m_aRfid[iLoop].name);
            query.bindValue(":address",m_aRfid[iLoop].address);
            query.bindValue(":type",   m_aRfid[iLoop].type);
            query.exec();
        }
    
    }

}

void MainWindow::updHandler(int iMask,DB_HANDLER_STRU *hdls)
{
     int iLoop;

     m_iHandlerMask = iMask;

     qDebug() << "updHandler" << m_iHandlerMask;
     
     for(iLoop = 0 ; iLoop < APP_HAND_SET_MAX_NUMBER ; iLoop++)
     {
         if (m_iHandlerMask & (1 << iLoop))
         {
             memset(&m_aHandler[iLoop],0,sizeof(DB_HANDLER_STRU));
             
             strncpy(m_aHandler[iLoop].name , hdls[iLoop].name,APP_SN_LENGTH);
             m_aHandler[iLoop].address = hdls[iLoop].address;
             m_aHandler[iLoop].type    = hdls[iLoop].type;
             m_aHandler[iLoop].def     = hdls[iLoop].def;
         }
     
     }
     saveHandler();

}

void MainWindow::delHandler(SN name)
{
     deleteHandler(name);
}


void MainWindow::updRfReader(int iMask,DB_RFID_STRU *hdls)
{
     int iLoop;

     m_iRfidMask = iMask;
     
     for(iLoop = 0 ; iLoop < APP_RF_READER_MAX_NUMBER ; iLoop++)
     {
         if (m_iRfidMask & (1 << iLoop))
         {
             memset(&m_aRfid[iLoop],0,sizeof(DB_RFID_STRU));
         
             strncpy(m_aRfid[iLoop].name , hdls[iLoop].name,APP_SN_LENGTH);
             m_aRfid[iLoop].address = hdls[iLoop].address;
             m_aRfid[iLoop].type    = hdls[iLoop].type;
         }
     
     }
     saveRfid();

}


void MainWindow::on_Gif_State_Change()
{
    if (m_pMovieGif->currentFrameNumber() == (m_pMovieGif->frameCount() - 1))
    {
        qDebug("GIf frams = %d \r\n",m_pMovieGif->currentFrameNumber());
        mainDisplay();
    }
}
    
    
void MainWindow::Splash()
{
    m_pLabelGif = new QLabel(mainWidget);
    m_pLabelGif->setGeometry(QRect(0, 0, 800, 600));

    m_pMovieGif = new QMovie(":/pic/LOGO.gif");

    m_pLabelGif->setMovie(m_pMovieGif);
    
    m_pMovieGif->start();

    m_pLabelGif->show();

    m_bSplash = true;

    if (m_pMovieGif->frameCount() < 0)
    {
        mainDisplay();
    }
    else
    {
       connect(m_pMovieGif,SIGNAL(frameChanged(int)),this,SLOT(on_Gif_State_Change()));
    }
}

MainWindow::~MainWindow()
{
#ifdef D_HTTPWORK
    m_workerThread.quit();
    m_workerThread.wait();
#endif

}

void MainWindow::mousePressEvent(QMouseEvent *e)
{   
    (void)e;
    if (!m_bLoged)
    {
        emit autoLogin();

        qDebug("autoLogin \r\n");
    }

}

void MainWindow::clearIdleSecond()
{
}

int MainWindow:: getAlarmState()
{
    int iType = 0;
    if ((m_iAlarmRcdMask[0][DISP_ALARM_PART0] & getMachineAlarmMask(gGlobalParam.iMachineType,DISP_ALARM_PART0))
        | (m_iAlarmRcdMask[0][DISP_ALARM_PART1] & getMachineAlarmMask(gGlobalParam.iMachineType,DISP_ALARM_PART1)))
    {
        iType |= MAINPAGE_NOTIFY_STATE_ALARM;

        qDebug() << "alarm" << m_iAlarmRcdMask[0][DISP_ALARM_PART0] << m_iAlarmRcdMask[0][DISP_ALARM_PART1];
    }
    if (gCMUsage.ulUsageState & getMachineNotifyMask(gGlobalParam.iMachineType,0))
    {
        qDebug() << "Yellow notify:" << gCMUsage.ulUsageState;
    
        iType |= MAINPAGE_NOTIFY_STATE_NOT;
    }

    return iType;

}


void MainWindow::on_timerEvent()
{
    unsigned int  savedUsageState = gCMUsage.ulUsageState;
    
    SaveConsumptiveMaterialInfo();

    CheckConsumptiveMaterialState();

    //More than 1 hour, start cir;
    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_Genie:
    case MACHINE_UP:
    case MACHINE_PURIST:
    case MACHINE_ADAPT:
        autoCirPreHour(); //More than 1 hour, start cir;
        break;
    case MACHINE_L_EDI_LOOP:
    case MACHINE_L_RO_LOOP:
    case MACHINE_EDI:
    case MACHINE_RO:
        break;
    default:
        break;
    }
    //end

  //  autoCirPreHour(); //More than 1 hour, start cir;

    if (m_iLstDay != DispGetDay())
    {
        m_iLstDay = DispGetDay();

        //memset(m_iAlarmRcdMask ,0,sizeof(m_iAlarmRcdMask));

        if (gCMUsage.bit1PendingInfoSave)
        {
            gCMUsage.bit1PendingInfoSave = FALSE;
            MainSaveCMInfo(gGlobalParam.iMachineType,gCMUsage.info);
            if(DISP_WORK_STATE_IDLE != DispGetWorkState4Pw())
            {
                updateCMInfoWithRFID(1);
            }
        }
        
    }

    /* tube circulation check */
    if (gGlobalParam.MiscParam.ulAllocMask & (1 << DISPLAY_ALLOC_FLAG_SWITCHON))
    {

       /* get hour */
       QDateTime dt = QDateTime::currentDateTime();
       int wd       = dt.date().dayOfWeek();
       int hour = dt.time().hour();
       int min  = dt.time().minute();

       if ( (1 << wd) & gGlobalParam.MiscParam.ulAllocMask)
       {

           /* check current minute */
           if (m_iTubeCirFlags != min)
           {
               int tgtMin = hour*60 + min;

               if (tgtMin >= m_iStartMinute && tgtMin < m_iEndMinute)
               {
                   if ( 0 == ((tgtMin - m_iStartMinute) %  m_iTubeCirCycle))
                   {

                        DISP_CMD_TC_STRU tc;

                        tc.iStart    = 1;
                        tc.iDuration = gGlobalParam.MiscParam.iTubeCirDuration;
                        
                        /* lets go */
                        if (DISP_INVALID_HANDLE != DispCmdEntry(DISP_CMD_TUBE_CIR,(unsigned char *)&tc,sizeof(DISP_CMD_TC_STRU)))
                        {
                            m_iTubeCirFlags = min;
                        }
                   }
               }
           }
       }
       
    }
    
    if (savedUsageState != gCMUsage.ulUsageState)
    {
        int iType = getAlarmState();

        if (typeid(*m_pCurPage) == typeid(MainPage))  
        {
            MainPage *page = (MainPage *)m_pCurPage;
        
            page->updAlarmState(iType);
        }
        
        if (m_iNotState != iType)
        {
            m_iNotState = iType;
            
            DispSndHoAlarm(APP_PROTOL_CANID_BROADCAST,iType);
        }
    }

    //2019.3.1
    if(!m_isInitCMInfo)
    {
        updateCMInfoWithRFID(0);
    }
}

void MainWindow::on_timerPeriodEvent()
{
    m_periodEvents++;

}

void MainWindow::on_timerSecondEvent()
{
    updateRectState(); //ex
    updateRectAlarmState();//
    updatePackFlow(); //

    if (m_iRfidDelayedMask)
    {
        int iLoop;
        for (iLoop = 0; iLoop < APP_RF_READER_MAX_NUMBER; iLoop++)
        {
            if (m_iRfidDelayedMask & (1 << iLoop))
            {
                m_aiRfidDelayedCnt[iLoop]--;

                if (0 == m_aiRfidDelayedCnt[iLoop])
                {
                    m_iRfidBufferActiveMask &= ~(1 << iLoop);
                    m_iRfidDelayedMask      &= ~(1 << iLoop);

                    if (APP_WORK_MODE_NORMAL == m_eWorkMode)
                    {
                        /*raise alarm */
                        switch(MacRfidMap.aiDeviceType4Normal[iLoop])
                        {
                        case DISP_P_PACK:
                            alarmCommProc(true,DISP_ALARM_PART0,DISP_ALARM_PART0_PPACK_OOP);
                            break;
                        case DISP_PRE_PACK:
                            alarmCommProc(true,DISP_ALARM_PART0,DISP_ALARM_PART0_PREPACK_OOP);
                            break;
                        case DISP_AC_PACK:
                            alarmCommProc(true,DISP_ALARM_PART0,DISP_ALARM_PART0_ACPACK_OOP);
                            break;
                        case DISP_U_PACK:
                            alarmCommProc(true,DISP_ALARM_PART0,DISP_ALARM_PART0_UPACK_OOP);
                            break;
                        case DISP_AT_PACK:
                            alarmCommProc(true,DISP_ALARM_PART0,DISP_ALARM_PART0_ATPACK_OOP);
                            break;
                        case DISP_H_PACK:
                            alarmCommProc(true,DISP_ALARM_PART0,DISP_ALARM_PART0_HPACK_OOP);
                            break;
                            
                        }
                    }
                }
            }
        }
    }
    
    /* faked flow report */
    if (!m_bSplash)
    {
        int iDelta;
        iDelta = m_periodEvents - m_ulFlowRptTick[APP_FM_FM2_NO];

        if ((iDelta >= FM_CALC_PERIOD/PERIOD_EVENT_LENGTH)
            && (iDelta <= FM_SIMULATION_PERIOD/PERIOD_EVENT_LENGTH))
        {
           /* simulating report */
           updFlowInfo(APP_FM_FM2_NO);
        }

        iDelta = m_periodEvents - m_ulFlowRptTick[APP_FM_FM3_NO];
        
        if ((iDelta >= FM_CALC_PERIOD/PERIOD_EVENT_LENGTH)
            && (iDelta <= FM_SIMULATION_PERIOD/PERIOD_EVENT_LENGTH))
        {
           /* simulating report */
           updFlowInfo(APP_FM_FM3_NO);
        }  

        /* 2018/04/04 simulate */
        iDelta = m_periodEvents - m_ulFlowRptTick[APP_FM_FM4_NO];
        
        if ((iDelta >= FM_CALC_PERIOD/PERIOD_EVENT_LENGTH)
            && (iDelta <= FM_SIMULATION_PERIOD/PERIOD_EVENT_LENGTH))
        {
           /* simulating report */
           updFlowInfo(APP_FM_FM4_NO);
        }  
        
    }
}

void MainWindow::on_timerScreenSleepEvent()
{
    QMutexLocker locker(&ex_gMutex);
    g_screenSleep++;

    if(g_screenSleep == ex_gGlobalParam.Ex_Config_Param.iScreenSleepTime * 6)
    {
        on_ScreenSleep(true);
    }
}


void Hex2String(QString &str,uint8 *pHexIn,int HexInLen)
{
    int iLoop;
    int x1,x2;
    
    memset(&tmpbuf[0],0,sizeof(tmpbuf));
    

    for (iLoop = 0; iLoop < HexInLen; iLoop++)
    {
        x1 = ((pHexIn[iLoop] >> 4) & 0xf);
        x2 = ((pHexIn[iLoop] ) & 0xf);
        tmpbuf[2*iLoop+0] =  x1 >= 10 ? x1-10 +'A': x1 + '0';
        tmpbuf[2*iLoop+1] =  x2 >= 10 ? x2-10 +'A': x2 + '0';
    }
    str = tmpbuf;

}

void MainWindow::AfDataMsg(IAP_NOTIFY_STRU *pIapNotify)
{
    APP_PACKET_COMM_STRU *pmg = (APP_PACKET_COMM_STRU *)&pIapNotify->data[1 + RPC_POS_DAT0]; 
    switch((pmg->ucMsgType & 0x7f))
    {
    case APP_PACKET_HAND_OPERATION:
        if ((pmg->ucMsgType & 0x80))
        {
            APP_PACKET_HO_STRU *pmg = (APP_PACKET_HO_STRU *)&pIapNotify->data[1 + RPC_POS_DAT0]; 
        
            switch(pmg->ucOpsType)
            {
            case APP_PACKET_HO_ADR_SET:
                {
                    /*
                    if (ex_gGlobalParam.Ex_Default == 0)
                    {
                        Ex_Init_HandleCfgpage* ex_Page = getExInitPage();
                        if(ex_Page) ex_Page->cfgHandlerRsp();
                    }
                    else
                    {
                        SetDevicePage *page = getDeviceDlg();
                        if (page) page->cfgHandlerRsp();
                    }*/
                }                
                break;
            case APP_PACKET_HO_ADR_QRY:
                {
                    /* get sn & address */
                    int offset = 0;
                    int iAddress;

                    int iDevType;

                    iDevType = pmg->aucData[offset];
                    QString strType = (APP_DEV_HS_SUB_HYPER == iDevType ) ? "UP" : "EDI";
                    offset += 1;

                    QString strSn = QString::fromAscii((char *)&pmg->aucData[offset],APP_SN_LENGTH);
                    offset += APP_SN_LENGTH;

                    iAddress = (pmg->aucData[offset] << 8) | (pmg->aucData[offset+1] << 0);
                    QString strAddress = QString::number(iAddress);
                    offset += 2;

                    //m_pDeviceDlg->addHandler(strSn,strAddress,strType); 
//                    {
//                        SetDevicePage *page = getDeviceDlg();
//                        if (page) page->addHandler(1,strSn,strAddress);
//                    }

                    if (ex_gGlobalParam.Ex_Default == 0)
                    {
                        Ex_Init_HandleCfgpage* ex_Page = getExInitPage();
                        if(ex_Page)
                            ex_Page->addHandler(1,strSn,strAddress);
                    }
                    else
                    {
                        SetDevicePage *page = getDeviceDlg();
                        if (page) page->addHandler(1,strSn,strAddress);
                    }
                }
                break;
            }        
        }
        break;
    case APP_PACKET_RF_OPERATION:
        if ((pmg->ucMsgType & 0x80))
        {
            APP_PACKET_RF_STRU *pmg = (APP_PACKET_RF_STRU *)&pIapNotify->data[1 + RPC_POS_DAT0]; 
        
            switch(pmg->ucOpsType)
            {
            case APP_PACKET_RF_ADR_SET:
                break;
            case APP_PACKET_RF_ADR_QRY:
                {
                    /* get sn & address */
                    int offset = 0;
                    int iAddress;

                    QString strSn = QString::fromAscii((char *)&pmg->aucData[offset],APP_SN_LENGTH);
                    offset += APP_SN_LENGTH;

                    iAddress = (pmg->aucData[offset] << 8) | (pmg->aucData[offset+1] << 0);
                    QString strAddress = QString::number(iAddress);

                    //m_pDeviceDlg->addRfReader(strSn,strAddress);
                    {
                        SetDevicePage *page = getDeviceDlg();
                        if (page) page->addRfReader(strSn,strAddress); 
                    }

                    qDebug() << "sn: " << strSn << "adr: " << strAddress;
                    
                }
                break;
            case APP_PACKET_RF_SEARCH:
            case APP_PACKET_RF_READ:
            case APP_PACKET_RF_WRITE:
                m_pOptDlg->EngRfMsg(pIapNotify);
                /* check */
                break;
            }        
        }        
        break;
    default: // yet to be implemented
        break;
    }

}


void MainWindow::zigbeeDataMsg(IAP_NOTIFY_STRU *pIapNotify)
{
    APP_PACKET_COMM_STRU *pmg = (APP_PACKET_COMM_STRU *)&pIapNotify->data[APP_ZIGBEE_SUB_PROTOL_LENGTH]; 
    switch((pmg->ucMsgType & 0x7f))
    {
    case APP_PACKET_HAND_OPERATION:
        if ((pmg->ucMsgType & 0x80))
        {
            APP_PACKET_HO_STRU *pmg = (APP_PACKET_HO_STRU *)&pIapNotify->data[APP_ZIGBEE_SUB_PROTOL_LENGTH]; 
        
            switch(pmg->ucOpsType)
            {
            case APP_PACKET_HO_ADR_SET:
                {
                    /*
                    if (ex_gGlobalParam.Ex_Default == 0)
                    {
                        Ex_Init_HandleCfgpage* ex_Page = getExInitPage();
                        if(ex_Page) ex_Page->cfgHandlerRsp();
                    }
                    else
                    {
                        SetDevicePage *page = getDeviceDlg();
                        if (page) page->cfgHandlerRsp();
                    }
                    */
                }                
                break;
            case APP_PACKET_HO_ADR_QRY:
                {
                    /* get sn & address */
                    int offset = 0;
                    int iAddress;

                    int iDevType;

                    iDevType = pmg->aucData[offset];
                    QString strType = (APP_DEV_HS_SUB_HYPER == iDevType ) ? "UP" : "EDI";
                    offset += 1;

                    QString strSn = QString::fromAscii((char *)&pmg->aucData[offset],APP_SN_LENGTH);
                    offset += APP_SN_LENGTH;

                    iAddress = (pmg->aucData[offset] << 8) | (pmg->aucData[offset+1] << 0);
                    QString strAddress = QString::number(iAddress);
                    offset += 2;

                    //m_pDeviceDlg->addHandler(strSn,strAddress,strType); 
//                    {
//                        SetDevicePage *page = getDeviceDlg();
//                        if (page) page->addHandler(1,strSn,strAddress);
//                    }
                    if (ex_gGlobalParam.Ex_Default == 0)
                    {
                        Ex_Init_HandleCfgpage* ex_Page = getExInitPage();
                        if(ex_Page)
                            ex_Page->addHandler(1,strSn,strAddress);
                    }
                    else
                    {
                        SetDevicePage *page = getDeviceDlg();
                        if (page) page->addHandler(1,strSn,strAddress);
                    }
                }
                break;
            }        
        }
        break;
    default: // yet to be implemented
        break;
    }

}


void MainWindow::on_IapIndication(IAP_NOTIFY_STRU *pIapNotify)
{
    qDebug("in iap \r\n");

    if (APP_TRX_CAN == pIapNotify->iTrxIndex)
    {
        switch ((pIapNotify->data[1 + RPC_POS_CMD0] & RPC_SUBSYSTEM_MASK))
        {
        case RPC_SYS_BOOT:
            if (pIapNotify->data[1 + RPC_POS_CMD1] & SAPP_RSP_MASK)
            {
            
                // handle response
                switch(pIapNotify->data[1 + RPC_POS_CMD1] & (~SAPP_RSP_MASK))
                {
                case SBL_QUERY_ID_CMD:
                    if (0 == pIapNotify->data[1 + RPC_POS_DAT0] )
                    {
                        uint8 deviceid[20];
                        uint8 deviceidLen;

                        deviceidLen = pIapNotify->data[1 + RPC_POS_DAT0 + 1];
                        memcpy(deviceid,&pIapNotify->data[1 + RPC_POS_DAT0 + 1 + 1],deviceidLen);
    
                        {
                            char szdevtype[8+1];
                            uint8 *pdata     = (uint8 *)&pIapNotify->data[1 + RPC_POS_DAT0 + 1 + 1 + deviceidLen];
                            uint16 usaddress = (pdata[0] << 8)|(pdata[1]);
                            pdata += 2;
                            memcpy (szdevtype,pdata,8);
                            szdevtype[8] = 0;
    
                            QString strAddress = QString::number(usaddress);
                            QString strDevType = szdevtype;
                            QString strElecId;
    
                            Hex2String(strElecId,deviceid,deviceidLen);
    
                            {
                                SetDevicePage *page = getDeviceDlg();
                                if (page) page->addDevice(strElecId,strAddress,strDevType);
                            }
                        }
    
                    }
                    else
                    {
                    }
    
                    break;
                case SBL_QUERY_VERSION_CMD:
                    if (0 == pIapNotify->data[1 + RPC_POS_DAT0] )
                    {
                        int len = pIapNotify->data[1 + SBL_RSP_STATUS+1] ; 
                        char *atrsp = (char *)&pIapNotify->data[1 + SBL_RSP_STATUS + 2];
                
                        if (len > 0 )
                        {
                            atrsp[len] = 0;
    
                            QString strInfo = atrsp;
    
                            QString strAdr = QString::number(CAN_SRC_ADDRESS(pIapNotify->ulCanId));
    
                            //m_pDeviceDlg->deviceVersion(strAdr,strInfo);            
                            {
                                SetDevicePage *page = getDeviceDlg();
                                if (page) page->deviceVersion(strAdr,strInfo);
                            }
                         }
                    }
                    break;
                case SBL_SET_ADDR_CMD:
                    if (pIapNotify->data[1 + RPC_POS_DAT0])
                    {
                    }
                    else
                    {
                    }               
                    break;
                case SBL_HANDSHAKE_CMD:
                    if (0 == pIapNotify->data[1 + RPC_POS_DAT0])
                    {
    
                    }
                    else
                    {
    
                    }
                    break;
                default:
                    break;
                
                }
            }
            break;
        case RPC_SYS_AF: /* for handler management */
            {
                switch(pIapNotify->data[1 + RPC_POS_CMD1] & (~SAPP_RSP_MASK))
                {
                case SAPP_CMD_DATA:
                    AfDataMsg(pIapNotify);
                    break;
                default:
                    break;
                }
            }
            break;
        default:
            break;
        }
    }
    else
    {
        zigbeeDataMsg(pIapNotify);
    }
    
    mem_free(pIapNotify);
}

bool alarmHaveAssociatedModule(int iAlarmPart,int iAlarmId)
{
   bool bDevice = true; /* default to have associated device ,maybe imaginary */
   
   switch(iAlarmPart)
   {
   case DISP_ALARM_PART0:
   {
        switch(iAlarmId)
        {
        case DISP_ALARM_PART0_TUBEUV_OOP:
            if (!(gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_TubeUV )))
            {
                bDevice = false ;
            }
            break;
        case DISP_ALARM_PART0_PREPACK_OOP:
            if (!(gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_Pre_Filter))) //DISP_SM_PreFilterColumn
            {
                bDevice = false ;
            }
            if (gGlobalParam.MiscParam.ulMisFlags & (1 << DISP_SM_RFID_Authorization))
            {
                bDevice = false ;
            }
            break;
        case DISP_ALARM_PART0_ACPACK_OOP:
        case DISP_ALARM_PART0_PPACK_OOP:
        case DISP_ALARM_PART0_ATPACK_OOP:
        case DISP_ALARM_PART0_HPACK_OOP:
        case DISP_ALARM_PART0_UPACK_OOP:  
            if (gGlobalParam.MiscParam.ulMisFlags & (1 << DISP_SM_RFID_Authorization))
            {
                bDevice = false ;
            }
            break;
        default:
             break;
        }  
//        if (gGlobalParam.MiscParam.ulMisFlags & (1 << DISP_SM_RFID_Authorization))
//        {
//            bDevice = false ;
//        }
       break;
   }
   case DISP_ALARM_PART1:
       switch(iAlarmId)
       {
       case DISP_ALARM_PART1_LOWER_SOURCE_WATER_PRESSURE:
           break;
       case DISP_ALARM_PART1_HIGHER_TOC_SENSOR_TEMPERATURE:
       case DISP_ALARM_PART1_LOWER_TOC_SENSOR_TEMPERATURE:
       case DISP_ALARM_PART1_LOWER_TOC_SOURCE_WATER_RESISTENCE:
            if (!(gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_HaveTOC )))
            {
                bDevice = false ;
            }
           break;
       default:
           break;
       }
       break;
   }

   return bDevice;
}

void MainWindow::alarmCommProc(bool bAlarm,int iAlarmPart,int iAlarmId)
{
    QSqlQuery query;
    
    bool bDbResult = false;

    int iAlarmMask = (1 << iAlarmId );

    bool bChanged = false;

    if (!(gGlobalParam.AlarmSettting.aulFlag[iAlarmPart] & iAlarmMask))
    {
        return;
    }


    if (bAlarm)
    {

        if (!(m_aMas[gGlobalParam.iMachineType].aulMask[iAlarmPart] & iAlarmMask))
        {
            return;
        }    
        if (!alarmHaveAssociatedModule(iAlarmPart,iAlarmId))
        {
            return;
        }
        if (!(m_iAlarmRcdMask[0][iAlarmPart] & iAlarmMask))
        {
            QString strdataTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
        
            query.prepare(INSERT_sql_Alarm);
            query.bindValue(":type", gastrAlarmName[(iAlarmPart * DISP_ALARM_PART0_NUM) + iAlarmId]);
            query.bindValue(":status", 1);
            query.bindValue(":time", strdataTime);
    
            bDbResult = query.exec();
            m_iAlarmRcdMask[0][iAlarmPart] |=  iAlarmMask; // prevent extra alarm fire message
            m_iAlarmRcdMask[1][iAlarmPart] &= ~iAlarmMask; // prepare to receive alarm restore message

            bChanged = true;
            updateFlowChartAlarm(gastrAlarmName[(iAlarmPart * DISP_ALARM_PART0_NUM) + iAlarmId], true);

#ifdef D_HTTPWORK
            if((gGlobalParam.MiscParam.iNetworkMask & (1 << DISPLAY_NETWORK_WIFI))
                && ex_gGlobalParam.Ex_Default)
            {
                QString strHttpAlarm = QString("{\"type\":0,\"code\":%1,\"status\":1,\"time\":\"%2\"}")
                                            .arg((iAlarmPart * DISP_ALARM_PART0_NUM) + iAlarmId)
                                            .arg(strdataTime);
                emitHttpAlarm(strHttpAlarm);
            }
#endif
        }
    }
    else
    {
        if ((!(m_iAlarmRcdMask[1][iAlarmPart] & iAlarmMask ))
              && (m_iAlarmRcdMask[0][iAlarmPart] & iAlarmMask))
        {
            QString strdataTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
        
            query.prepare(INSERT_sql_Alarm);
            query.bindValue(":type", gastrAlarmName[(iAlarmPart * DISP_ALARM_PART0_NUM) + iAlarmId]);
            query.bindValue(":status", 0);
            query.bindValue(":time", strdataTime);
            bDbResult = query.exec();
            m_iAlarmRcdMask[1][iAlarmPart] |= iAlarmMask;     // prevent extra alarm restore message
            m_iAlarmRcdMask[0][iAlarmPart] &= ~iAlarmMask;  // prepare to receive alarm fire message
            
            bChanged = true;
            updateFlowChartAlarm(gastrAlarmName[(iAlarmPart * DISP_ALARM_PART0_NUM) + iAlarmId], false);

#ifdef D_HTTPWORK
            if((gGlobalParam.MiscParam.iNetworkMask & (1 << DISPLAY_NETWORK_WIFI))
                && ex_gGlobalParam.Ex_Default)
            {
                QString strHttpAlarm = QString("{\"type\":0,\"code\":%1,\"status\":0,\"time\":\"%2\"}")
                                            .arg((iAlarmPart * DISP_ALARM_PART0_NUM) + iAlarmId)
                                            .arg(strdataTime);
                emitHttpAlarm(strHttpAlarm);
            }
#endif
        }
    }    

    if (bChanged )
    {
        int iType = getAlarmState();
        
        if (m_pCurPage && (typeid(*m_pCurPage) == typeid(MainPage)))
        {
             MainPage *page = (MainPage *)m_pCurPage;
             
             page->updAlarmState(iType);
        }

        if (m_pCurPage && (typeid(*m_pCurPage) == typeid(AlarmDisplayPage)))
        {
             AlarmDisplayPage *page = (AlarmDisplayPage *)m_pCurPage;
             
             page->update();
        }

        if (m_iNotState != iType)
        {
            m_iNotState = iType;
            
            DispSndHoAlarm(APP_PROTOL_CANID_BROADCAST,iType);
        }
    }
    

}

int  MainWindow::readRfid(int iRfId)
{
    int iRet = -1;
    
    /* read card info */
    iRet =  CcbReadRfid(iRfId,2000,m_RfDataItems.aItem[RF_DATA_LAYOUT_CATALOGUE_NUM].offset,m_RfDataItems.aItem[RF_DATA_LAYOUT_CATALOGUE_NUM].size);
    if (0 == iRet)
    {
        iRet =  CcbReadRfid(iRfId,2000,m_RfDataItems.aItem[RF_DATA_LAYOUT_LOT_NUMBER].offset,m_RfDataItems.aItem[RF_DATA_LAYOUT_LOT_NUMBER].size);
    
        if (0 == iRet)
        {
#ifdef RFIDTEST
            iRet =  CcbReadRfid(iRfId,2000,m_RfDataItems.aItem[RF_DATA_LAYOUT_INSTALL_DATE].offset,m_RfDataItems.aItem[RF_DATA_LAYOUT_LOT_NUMBER].size);
            if(0 != iRet)
            {
                return -1;
            }
            iRet =  CcbReadRfid(iRfId,2000,m_RfDataItems.aItem[RF_DATA_LAYOUT_UNKNOW_DATA].offset,m_RfDataItems.aItem[RF_DATA_LAYOUT_LOT_NUMBER].size);
            if(0 != iRet)
            {
                return -1;
            }
#endif
            iRet = CcbGetRfidCont(iRfId,m_RfDataItems.aItem[RF_DATA_LAYOUT_CATALOGUE_NUM].offset,m_RfDataItems.aItem[RF_DATA_LAYOUT_CATALOGUE_NUM].size,m_aRfidInfo[iRfId].aucContent + m_RfDataItems.aItem[RF_DATA_LAYOUT_CATALOGUE_NUM].offset);
            if (0 == iRet)
            {
               return -1;
            }
            
            iRet = CcbGetRfidCont(iRfId,m_RfDataItems.aItem[RF_DATA_LAYOUT_LOT_NUMBER].offset,m_RfDataItems.aItem[RF_DATA_LAYOUT_LOT_NUMBER].size,m_aRfidInfo[iRfId].aucContent + m_RfDataItems.aItem[RF_DATA_LAYOUT_LOT_NUMBER].offset);
            if (0 == iRet)
            {
                return -1;
            }

#ifdef RFIDTEST
            iRet = CcbGetRfidCont(iRfId,
                                  m_RfDataItems.aItem[RF_DATA_LAYOUT_INSTALL_DATE].offset,
                                  m_RfDataItems.aItem[RF_DATA_LAYOUT_INSTALL_DATE].size,
                                  m_aRfidInfo[iRfId].aucContent + m_RfDataItems.aItem[RF_DATA_LAYOUT_INSTALL_DATE].offset);
            if (0 == iRet)
            {
                return -1;
            }
            iRet = CcbGetRfidCont(iRfId,
                                  m_RfDataItems.aItem[RF_DATA_LAYOUT_UNKNOW_DATA].offset,
                                  m_RfDataItems.aItem[RF_DATA_LAYOUT_UNKNOW_DATA].size,
                                  m_aRfidInfo[iRfId].aucContent + m_RfDataItems.aItem[RF_DATA_LAYOUT_UNKNOW_DATA].offset);
            if (0 == iRet)
            {
                return -1;
            }
#endif
            
            m_aRfidInfo[iRfId].ucValid = 1;

            m_aRfidInfo[iRfId].parse();

            iRet = 0; // succeed!!
        }
    }

    return iRet;
}

#ifdef RFIDTEST
int MainWindow::writeRfid(int iRfId, int dataLayout, QString strData)
{
    //make data
    unsigned char pData[m_RfDataItems.aItem[dataLayout].size];
    char tmpbuf[256];
    memset(tmpbuf, 0, sizeof(tmpbuf));
    strcpy(tmpbuf, strData.toAscii());
    switch(dataLayout)
    {
    case RF_DATA_LAYOUT_CATALOGUE_NUM:
        for (int iLoop = 0; iLoop < m_RfDataItems.aItem[RF_DATA_LAYOUT_CATALOGUE_NUM].size; iLoop += 4)
        {
            pData[iLoop]   = tmpbuf[iLoop + 3];
            pData[iLoop+1] = tmpbuf[iLoop + 2];
            pData[iLoop+2] = tmpbuf[iLoop + 1];
            pData[iLoop+3] = tmpbuf[iLoop + 0];
        }
        break;
    case RF_DATA_LAYOUT_LOT_NUMBER:
        for (int iLoop = 0; iLoop < m_RfDataItems.aItem[RF_DATA_LAYOUT_LOT_NUMBER].size; iLoop += 4)
        {
            pData[iLoop]   = tmpbuf[iLoop + 3];
            pData[iLoop+1] = tmpbuf[iLoop + 2];
            pData[iLoop+2] = tmpbuf[iLoop + 1];
            pData[iLoop+3] = tmpbuf[iLoop + 0];
        }
        break;
    case RF_DATA_LAYOUT_INSTALL_DATE:
    {
//        QDate curDate = QDate::currentDate();
        QDate curDate = QDate::fromString(strData, "yyyy-MM-dd");
        pData[0] = curDate.day();
        pData[1] = curDate.month();
        pData[2] = (curDate.year() >> 8) & 0xff;
        pData[3] = (curDate.year() >> 0) & 0xff;
        break;
    }
    case RF_DATA_LAYOUT_UNKNOW_DATA:
    {
        int num, num1, num2, num3;
        num = strData.toInt();
        if(num > 999999)
        {
            num = 999999;
        }

        num1 = num%100;
        num2 = num%10000/100;
        num3 = num/10000;
        pData[0] = num1;
        pData[1] = num2;
        pData[2] = num3;
        break;
    }
    default:
        return -1;
    }
    //end make data

    int iRet = CcbWriteRfid(iRfId, 2000, m_RfDataItems.aItem[dataLayout].offset,
                 m_RfDataItems.aItem[dataLayout].size,
                 pData);
    return iRet;
}
#endif

void MainWindow::saveFmData(int id,unsigned int ulValue)
{
    m_ulFlowMeter[id]   = ulValue;
    m_ulFlowRptTick[id] = m_periodEvents;
}

#ifdef D_HTTPWORK
void MainWindow::showWifiConfigDlg(const QString& name)
{
    m_pWifiConfigDlg->setSSIDName(name);
    m_pWifiConfigDlg->show();
}

void MainWindow::emitHttpAlarm(const QString &strAlarm)
{
    emit sendHttpAlarm(strAlarm);
}

void MainWindow::checkConsumableAlarm()
{
    QString strCurTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    QString strJson = QString("{\"type\":1,\"code\":%1,\"status\":%2,\"time\":\"%3\"}");

    if (gCMUsage.ulUsageState & (1 << DISP_PRE_PACKLIFEDAY)
        || gCMUsage.ulUsageState & (1 << DISP_PRE_PACKLIFEL))
    {
        if(!m_conAlarmMark[DISP_PRE_NOTIFY])
        {
            QString strAlarm = strJson.arg(DISP_PRE_NOTIFY).arg(1).arg(strCurTime);
            emitHttpAlarm(strAlarm);
            m_conAlarmMark[DISP_PRE_NOTIFY] = true;
        }
    }
    else
    {
        if(m_conAlarmMark[DISP_PRE_NOTIFY])
        {
            QString strAlarm = strJson.arg(DISP_PRE_NOTIFY).arg(0).arg(strCurTime);
            emitHttpAlarm(strAlarm);
            m_conAlarmMark[DISP_PRE_NOTIFY] = false;
        }
    }

    if (gCMUsage.ulUsageState & (1 << DISP_AC_PACKLIFEDAY)
        || gCMUsage.ulUsageState & (1 << DISP_AC_PACKLIFEL))
    {
        if(!m_conAlarmMark[DISP_AC_NOTIFY])
        {
            QString strAlarm = strJson.arg(DISP_AC_NOTIFY).arg(1).arg(strCurTime);
            emitHttpAlarm(strAlarm);
            m_conAlarmMark[DISP_AC_NOTIFY] = true;
        }
    }
    else
    {
        if(m_conAlarmMark[DISP_AC_NOTIFY])
        {
            QString strAlarm = strJson.arg(DISP_AC_NOTIFY).arg(0).arg(strCurTime);
            emitHttpAlarm(strAlarm);
            m_conAlarmMark[DISP_AC_NOTIFY] = false;
        }
    }

    if (gCMUsage.ulUsageState & (1 << DISP_T_PACKLIFEDAY)
        || gCMUsage.ulUsageState & (1 << DISP_T_PACKLIFEL))
    {
        if(!m_conAlarmMark[DISP_T_NOTIFY])
        {
            QString strAlarm = strJson.arg(DISP_T_NOTIFY).arg(1).arg(strCurTime);
            emitHttpAlarm(strAlarm);
            m_conAlarmMark[DISP_T_NOTIFY] = true;
        }
    }
    else
    {
        if(m_conAlarmMark[DISP_T_NOTIFY])
        {
            QString strAlarm = strJson.arg(DISP_T_NOTIFY).arg(0).arg(strCurTime);
            emitHttpAlarm(strAlarm);
            m_conAlarmMark[DISP_T_NOTIFY] = false;
        }
    }


    if (gCMUsage.ulUsageState & (1 << DISP_P_PACKLIFEDAY)
        || gCMUsage.ulUsageState & (1 << DISP_P_PACKLIFEL))
    {
        if(!m_conAlarmMark[DISP_P_NOTIFY])
        {
            QString strAlarm = strJson.arg(DISP_P_NOTIFY).arg(1).arg(strCurTime);
            emitHttpAlarm(strAlarm);
            m_conAlarmMark[DISP_P_NOTIFY] = true;
        }
    }
    else
    {
        if(m_conAlarmMark[DISP_P_NOTIFY])
        {
            QString strAlarm = strJson.arg(DISP_P_NOTIFY).arg(0).arg(strCurTime);
            emitHttpAlarm(strAlarm);
            m_conAlarmMark[DISP_P_NOTIFY] = false;
        }
    }

    if (gCMUsage.ulUsageState & (1 << DISP_U_PACKLIFEDAY)
        || gCMUsage.ulUsageState & (1 << DISP_U_PACKLIFEL))
    {
        if(!m_conAlarmMark[DISP_U_NOTIFY])
        {
            QString strAlarm = strJson.arg(DISP_U_NOTIFY).arg(1).arg(strCurTime);
            emitHttpAlarm(strAlarm);
            m_conAlarmMark[DISP_U_NOTIFY] = true;
        }
    }
    else
    {
        if(m_conAlarmMark[DISP_U_NOTIFY])
        {
            QString strAlarm = strJson.arg(DISP_U_NOTIFY).arg(0).arg(strCurTime);
            emitHttpAlarm(strAlarm);
            m_conAlarmMark[DISP_U_NOTIFY] = false;
        }
    }

    if (gCMUsage.ulUsageState & (1 << DISP_AT_PACKLIFEDAY)
        || gCMUsage.ulUsageState & (1 << DISP_AT_PACKLIFEL))
    {
        if(!m_conAlarmMark[DISP_AT_NOTIFY])
        {
            QString strAlarm = strJson.arg(DISP_AT_NOTIFY).arg(1).arg(strCurTime);
            emitHttpAlarm(strAlarm);
            m_conAlarmMark[DISP_AT_NOTIFY] = true;
        }
    }
    else
    {
        if(m_conAlarmMark[DISP_AT_NOTIFY])
        {
            QString strAlarm = strJson.arg(DISP_AT_NOTIFY).arg(0).arg(strCurTime);
            emitHttpAlarm(strAlarm);
            m_conAlarmMark[DISP_AT_NOTIFY] = false;
        }
    }

    if (gCMUsage.ulUsageState & (1 << DISP_H_PACKLIFEDAY)
        || gCMUsage.ulUsageState & (1 << DISP_H_PACKLIFEL))
    {
        if(!m_conAlarmMark[DISP_H_NOTIFY])
        {
            QString strAlarm = strJson.arg(DISP_H_NOTIFY).arg(1).arg(strCurTime);
            emitHttpAlarm(strAlarm);
            m_conAlarmMark[DISP_H_NOTIFY] = true;
        }
    }
    else
    {
        if(m_conAlarmMark[DISP_H_NOTIFY])
        {
            QString strAlarm = strJson.arg(DISP_H_NOTIFY).arg(0).arg(strCurTime);
            emitHttpAlarm(strAlarm);
            m_conAlarmMark[DISP_H_NOTIFY] = false;
        }
    }

    if (gCMUsage.ulUsageState & (1 << DISP_N1_UVLIFEDAY)
        || gCMUsage.ulUsageState & (1 << DISP_N1_UVLIFEHOUR))
    {
        if(!m_conAlarmMark[DISP_N1_NOTIFY])
        {
            QString strAlarm = strJson.arg(DISP_N1_NOTIFY).arg(1).arg(strCurTime);
            emitHttpAlarm(strAlarm);
            m_conAlarmMark[DISP_N1_NOTIFY] = true;
        }
    }
    else
    {
        if(m_conAlarmMark[DISP_N1_NOTIFY])
        {
            QString strAlarm = strJson.arg(DISP_N1_NOTIFY).arg(0).arg(strCurTime);
            emitHttpAlarm(strAlarm);
            m_conAlarmMark[DISP_N1_NOTIFY] = false;
        }
    }

    if (gCMUsage.ulUsageState & (1 << DISP_N2_UVLIFEDAY)
        || gCMUsage.ulUsageState & (1 << DISP_N2_UVLIFEHOUR))
    {
        if(!m_conAlarmMark[DISP_N2_NOTIFY])
        {
            QString strAlarm = strJson.arg(DISP_N2_NOTIFY).arg(1).arg(strCurTime);
            emitHttpAlarm(strAlarm);
            m_conAlarmMark[DISP_N2_NOTIFY] = true;
        }
    }
    else
    {
        if(m_conAlarmMark[DISP_N2_NOTIFY])
        {
            QString strAlarm = strJson.arg(DISP_N2_NOTIFY).arg(0).arg(strCurTime);
            emitHttpAlarm(strAlarm);
            m_conAlarmMark[DISP_N2_NOTIFY] = false;
        }
    }

    if (gCMUsage.ulUsageState & (1 << DISP_N3_UVLIFEDAY)
        || gCMUsage.ulUsageState & (1 << DISP_N3_UVLIFEHOUR))
    {
        if(!m_conAlarmMark[DISP_N3_NOTIFY])
        {
            QString strAlarm = strJson.arg(DISP_N3_NOTIFY).arg(1).arg(strCurTime);
            emitHttpAlarm(strAlarm);
            m_conAlarmMark[DISP_N3_NOTIFY] = true;
        }
    }
    else
    {
        if(m_conAlarmMark[DISP_N3_NOTIFY])
        {
            QString strAlarm = strJson.arg(DISP_N3_NOTIFY).arg(0).arg(strCurTime);
            emitHttpAlarm(strAlarm);
            m_conAlarmMark[DISP_N3_NOTIFY] = false;
        }
    }

    if (gCMUsage.ulUsageState & (1 << DISP_N4_UVLIFEDAY)
        || gCMUsage.ulUsageState & (1 << DISP_N4_UVLIFEHOUR))
    {
        if(!m_conAlarmMark[DISP_N4_NOTIFY])
        {
            QString strAlarm = strJson.arg(DISP_N4_NOTIFY).arg(1).arg(strCurTime);
            emitHttpAlarm(strAlarm);
            m_conAlarmMark[DISP_N4_NOTIFY] = true;
        }
    }
    else
    {
        if(m_conAlarmMark[DISP_N4_NOTIFY])
        {
            QString strAlarm = strJson.arg(DISP_N4_NOTIFY).arg(0).arg(strCurTime);
            emitHttpAlarm(strAlarm);
            m_conAlarmMark[DISP_N4_NOTIFY] = false;
        }
    }

    if (gCMUsage.ulUsageState & (1 << DISP_N5_UVLIFEDAY)
        || gCMUsage.ulUsageState & (1 << DISP_N5_UVLIFEHOUR))
    {
        if(!m_conAlarmMark[DISP_N5_NOTIFY])
        {
            QString strAlarm = strJson.arg(DISP_N5_NOTIFY).arg(1).arg(strCurTime);
            emitHttpAlarm(strAlarm);
            m_conAlarmMark[DISP_N5_NOTIFY] = true;
        }
    }
    else
    {
        if(m_conAlarmMark[DISP_N5_NOTIFY])
        {
            QString strAlarm = strJson.arg(DISP_N5_NOTIFY).arg(0).arg(strCurTime);
            emitHttpAlarm(strAlarm);
            m_conAlarmMark[DISP_N5_NOTIFY] = false;
        }
    }


    if ((gCMUsage.ulUsageState & (1 << DISP_W_FILTERLIFE)
        || gCMUsage.ulUsageState & (1 << DISP_W_FILTERLIFE))
        && (gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_HaveB2)))
    {
        if(!m_conAlarmMark[DISP_W_NOTIFY])
        {
            QString strAlarm = strJson.arg(DISP_W_NOTIFY).arg(1).arg(strCurTime);
            emitHttpAlarm(strAlarm);
            m_conAlarmMark[DISP_W_NOTIFY] = true;
        }
    }
    else
    {
        if(m_conAlarmMark[DISP_W_NOTIFY])
        {
            QString strAlarm = strJson.arg(DISP_W_NOTIFY).arg(0).arg(strCurTime);
            emitHttpAlarm(strAlarm);
            m_conAlarmMark[DISP_W_NOTIFY] = false;
        }
    }

    if (gCMUsage.ulUsageState & (1 << DISP_T_B_FILTERLIFE)
        || gCMUsage.ulUsageState & (1 << DISP_T_B_FILTERLIFE))
    {
        if(!m_conAlarmMark[DISP_T_B_NOTIFY])
        {
            QString strAlarm = strJson.arg(DISP_T_B_NOTIFY).arg(1).arg(strCurTime);
            emitHttpAlarm(strAlarm);
            m_conAlarmMark[DISP_T_B_NOTIFY] = true;
        }
    }
    else
    {
        if(m_conAlarmMark[DISP_T_B_NOTIFY])
        {
            QString strAlarm = strJson.arg(DISP_T_B_NOTIFY).arg(0).arg(strCurTime);
            emitHttpAlarm(strAlarm);
            m_conAlarmMark[DISP_T_B_NOTIFY] = false;
        }
    }


    if (gCMUsage.ulUsageState & (1 << DISP_T_A_FILTERLIFE)
        || gCMUsage.ulUsageState & (1 << DISP_T_A_FILTERLIFE))
    {
        if(!m_conAlarmMark[DISP_T_A_NOTIFY])
        {
            QString strAlarm = strJson.arg(DISP_T_A_NOTIFY).arg(1).arg(strCurTime);
            emitHttpAlarm(strAlarm);
            m_conAlarmMark[DISP_T_A_NOTIFY] = true;
        }
    }
    else
    {
        if(m_conAlarmMark[DISP_T_A_NOTIFY])
        {
            QString strAlarm = strJson.arg(DISP_T_A_NOTIFY).arg(0).arg(strCurTime);
            emitHttpAlarm(strAlarm);
            m_conAlarmMark[DISP_T_A_NOTIFY] = false;
        }
    }

    if (gCMUsage.ulUsageState & (1 << DISP_TUBE_FILTERLIFE)
        || gCMUsage.ulUsageState & (1 << DISP_TUBE_FILTERLIFE))
    {
        if(!m_conAlarmMark[DISP_TUBE_NOTIFY])
        {
            QString strAlarm = strJson.arg(DISP_TUBE_NOTIFY).arg(1).arg(strCurTime);
            emitHttpAlarm(strAlarm);
            m_conAlarmMark[DISP_TUBE_NOTIFY] = true;
        }
    }
    else
    {
        if(m_conAlarmMark[DISP_TUBE_NOTIFY])
        {
            QString strAlarm = strJson.arg(DISP_TUBE_NOTIFY).arg(0).arg(strCurTime);
            emitHttpAlarm(strAlarm);
            m_conAlarmMark[DISP_TUBE_NOTIFY] = false;
        }
    }


    if (gCMUsage.ulUsageState & (1 << DISP_TUBE_DI_LIFE)
        || gCMUsage.ulUsageState & (1 << DISP_TUBE_DI_LIFE))
    {
        if(!m_conAlarmMark[DISP_TUBE_DI_NOTIFY])
        {
            QString strAlarm = strJson.arg(DISP_TUBE_DI_NOTIFY).arg(1).arg(strCurTime);
            emitHttpAlarm(strAlarm);
            m_conAlarmMark[DISP_TUBE_DI_NOTIFY] = true;
        }
    }
    else
    {
        if(m_conAlarmMark[DISP_TUBE_DI_NOTIFY])
        {
            QString strAlarm = strJson.arg(DISP_TUBE_DI_NOTIFY).arg(0).arg(strCurTime);
            emitHttpAlarm(strAlarm);
            m_conAlarmMark[DISP_TUBE_DI_NOTIFY] = false;
        }
    }

    if (gCMUsage.ulUsageState & (1 << DISP_ROC12LIFEDAY)
        || gCMUsage.ulUsageState & (1 << DISP_ROC12LIFEDAY))
    {
        if(!m_conAlarmMark[DISP_ROC12_NOTIFY])
        {
            QString strAlarm = strJson.arg(DISP_ROC12_NOTIFY).arg(1).arg(strCurTime);
            emitHttpAlarm(strAlarm);
            m_conAlarmMark[DISP_ROC12_NOTIFY] = true;
        }
    }
    else
    {
        if(m_conAlarmMark[DISP_ROC12_NOTIFY])
        {
            QString strAlarm = strJson.arg(DISP_ROC12_NOTIFY).arg(0).arg(strCurTime);
            emitHttpAlarm(strAlarm);
            m_conAlarmMark[DISP_ROC12_NOTIFY] = false;
        }
    }
}

void MainWindow::on_timerNetworkEvent()
{
    QString strCurTime = QTime::currentTime().toString("hh:mm:ss:zzz");
    QString msg = QString("Genie client Message: hello Mqtt %1").arg(strCurTime);
    publishMqttMessage(msg.toUtf8());

    if(ex_gGlobalParam.Ex_Default == 0)
    {
        return;
    }
    if(gGlobalParam.MiscParam.iNetworkMask & (1 << DISPLAY_NETWORK_WIFI))
    {
        //test
        emit sendHttpHeartData(m_networkData);
        emit httpHeartPost();
    }
}

bool MainWindow::isDirExist(const QString &fullPath)
{
    QDir dir(fullPath);
    if(dir.exists())
    {
        return true;
    }
    else
    {
        bool ok = dir.mkdir(fullPath);//只创建一级子目录，即必须保证上级目录存在
        return ok;
    }
}

void MainWindow::initHttpWorker()
{
    DHttpWorker *worker = new DHttpWorker;
    worker->moveToThread(&m_workerThread);

    connect(&m_workerThread, SIGNAL(finished()), worker, SLOT(deleteLater()));
    connect(&m_workerThread, SIGNAL(started()), worker, SLOT(on_initHttp()));

    connect(this, SIGNAL(sendHttpRunMsg(const QString&, int)), worker, SLOT(on_updateRunMsgList(const QString&, int)));
    connect(this, SIGNAL(sendHttpAlarm(const QString&)), worker, SLOT(on_updateAlarmList(const QString&)));
    connect(this, SIGNAL(sendHttpHeartData(const NetworkData&)), worker, SLOT(on_updateHeartList(const NetworkData&)));

    connect(this, SIGNAL(httpHeartPost()), worker, SLOT(on_heartHttpPost()));
    connect(worker, SIGNAL(feedback(const QByteArray&)), this, SLOT(on_updateText(const QByteArray&)));

    m_workerThread.start();

    m_networkTimer = new QTimer(this);
    connect(m_networkTimer, SIGNAL(timeout()), this, SLOT(on_timerNetworkEvent()),Qt::QueuedConnection);
    m_networkTimer->start(1000*10); // peroid of one second

    m_pWifiConfigDlg = new DWifiConfigDialog(this);
    m_pWifiConfigDlg->hide();
    m_pWifiConfigDlg->move(250, 210);

    //test
    m_networkData.waterQuality[0].fG25x = 2000;
    m_networkData.waterQuality[0].tx = 25.0;

    m_networkData.waterQuality[1].fG25x = 50;
    m_networkData.waterQuality[1].tx = 25.1;

    m_networkData.waterQuality[2].fG25x = 16.0;
    m_networkData.waterQuality[2].tx = 25.2;

    m_networkData.waterQuality[3].fG25x = 18.0;
    m_networkData.waterQuality[3].tx = 25.3;

    m_networkData.waterQuality[4].fG25x = 18.2;
    m_networkData.waterQuality[4].tx = 25.4;

    m_networkData.fResidue = 98;
    m_networkData.fToc = 3;

    m_networkData.runStatus = 0;

    initMqtt();
}

void MainWindow::initMqtt()
{ 
    mqttProcessRun = false;
    mqttNum = 1;
    if(QFile::exists("/opt/shzn/MqttClient"))
    {
        m_mqttProcess.start("/opt/shzn/MqttClient");
        connect(&m_mqttProcess, SIGNAL(started()), this, SLOT(on_mqttProcess_started()));
        connect(&m_mqttProcess, SIGNAL(finished(int,QProcess::ExitStatus)),
                this, SLOT(on_mqttProcess_finished(int, QProcess::ExitStatus)));
        qDebug() << "start MqttClient";
    }
    else
    {
        qDebug() << "can not find MqttClient";
    }
}

void MainWindow::on_mqttWatcher_fileChanged(const QString &fileName)
{
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Controller: " <<"Open subscribe file failed";
        return;
    }

    QByteArray array = file.readAll();
    if(0 == array.size())
    {
        return;
    }
    qDebug() << QString("Controller Mqtt %1: ").arg(mqttNum) << array;
    mqttNum++;

}

void MainWindow::on_mqttProcess_started()
{
    qDebug() << "The MQTT process started successfully";
    //启动订阅文件检测
    if(isDirExist("/opt/mqttTempFile") && !mqttProcessRun)
    {
        QStringList fileList;
        fileList << "/opt/mqttTempFile/subscribeFile.txt";
        m_pFileWatcher = new QFileSystemWatcher(fileList, this);
        connect(m_pFileWatcher, SIGNAL(fileChanged (const QString &)),
                this, SLOT(on_mqttWatcher_fileChanged(const QString &)));

        mqttProcessRun = true;
    }
}

void MainWindow::on_mqttProcess_finished(int exitCode, QProcess::ExitStatus exitStatus)
{
    qDebug() << QString("mqtt process finished code: %1, exitStatus: %2").arg(exitCode)
                .arg(exitStatus);

    m_mqttProcess.start("/opt/shzn/MqttClient");
}

//向发布文件中写入需要发布的新内容
void MainWindow::publishMqttMessage(const QByteArray &msg)
{
    if(!mqttProcessRun)
    {
        return;
    }
    if(isDirExist("/opt/mqttTempFile"))
    {
        QFile file("/opt/mqttTempFile/publishFile.txt");
        if(!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
        {
            qDebug() << "Controller: " << "Open publish file failed";
            return;
        }

        QTextStream out(&file);
        out << msg;
        file.close();
    }
}

void MainWindow::on_updateText(const QByteArray& array)
{
    if (NULL != m_pSubPages[PAGE_SET])
    {
        SetPage *page = (SetPage *)m_pSubPages[PAGE_SET];
        Ex_FactoryTestPage *subpage = (Ex_FactoryTestPage *)page->getSubPage(SET_BTN_SYSTEM_FACTORYTEST);
        if(subpage->isVisible())
        {
            subpage->updateWifiTestMsg(array);
        }
    }
}
#endif

void MainWindow::initScreenSleep()
{
//    m_screenSleepThread = new Ex_ScreenSleepThread(this);
//    m_screenSleepThread->start();
//    connect(m_screenSleepThread, SIGNAL(screenSleep(bool)), this, SLOT(on_ScreenSleep(bool)));
    m_screenSleepTimer = new QTimer(this);
    connect(m_screenSleepTimer, SIGNAL(timeout()), this, SLOT(on_timerScreenSleepEvent()));
    m_screenSleepTimer->start(1000*10);
}

void MainWindow::autoCirPreHour()
{
    if(ex_gulSecond - ex_gCcb.Ex_Auto_Cir_Tick.ulUPAutoCirTick > 60*60) //More than 1 hour, start cir;
    {
        if(!(DispGetUpQtwFlag() || DispGetUpCirFlag() || DispGetEdiQtwFlag()
             || DispGetTankCirFlag() || DispGetTocCirFlag()))
        {
            this->startCir(CIR_TYPE_UP);
        }
    }

}

void MainWindow::on_ScreenSleep(bool sleep)
{
    if(sleep && (g_screenSleep != 0) && (!g_isScreenSleep) && gGlobalParam.MiscParam.iEnerySave)
    {

        g_isScreenSleep = true;
//        int value = 10;
//        Write_sys_int(PWMLCD_FILE, value);

        m_pPreviousPage = m_pCurPage;
        m_pCurPage->show(false);

        m_pScreenSleepPage->show(true);
        emit SleepPageShow(true);
    }

}

void MainWindow::on_dispIndication(unsigned char *pucData,int iLength)
{
    NOT_INFO_STRU *pNotify = (NOT_INFO_STRU *)pucData;
    int WLoop;
    QSqlQuery query;
    QSqlQuery ex_query;
    bool bDbResult = false;
    //static int iSeq1 = 0;
    //static int iSeq2 = 0;

    MainPage *pMainPage = (MainPage *)m_pSubPages[PAGE_MAIN];

    //qDebug("in Ind %d %d\r\n" ,++iSeq1,iSeq2);
    
    (void)iLength;
    switch(pNotify->Hdr.ucCode)
    {
    case DISP_NOT_GET_PARAM:
        {
            NOT_GPM_ITEM_STRU *pItem = (NOT_GPM_ITEM_STRU *)pNotify->aucData;
            
            qDebug("on_dispIndication DISP_NOT_GET_PARAM \r\n");

            while(pItem->ucId != 0XFF)
            {
               switch(pItem->ucId)
               {
               default:
               case NOT_PARAM_ALL_PARAM:
               case NOT_PARAM_MACHINE_PARAM:
                    CcbSyncSetMachineType(gGlobalParam.iMachineType);
                    CcbSyncSetMachineParams(&gGlobalParam.MMParam);
                    CcbSyncSetModuleParams(&gGlobalParam.SubModSetting);
                    CcbSyncSetAlarmParams(&gGlobalParam.AlarmSettting);
                    CcbSyncSetTimeParams(&gGlobalParam.TMParam);
                    CcbSyncSetConsumeMaterialParams(&gGlobalParam.CMParam);
                    CcbSyncSetFmParams(&gGlobalParam.FmParam);
                    CcbSyncSetPmParams(&gGlobalParam.PmParam);
                    CcbSyncSetMiscParams(&gGlobalParam.MiscParam);
                    CcbInitMachineType();
                    break;
               case NOT_PARAM_SUBMODULE_PARAM:
                    CcbSyncSetModuleParams(&gGlobalParam.SubModSetting);
                    break;
               case NOT_PARAM_ALARM_PARAM:
                    CcbSyncSetAlarmParams(&gGlobalParam.AlarmSettting);
                    break;
               case NOT_PARAM_TIME_PARAM:
                    CcbSyncSetTimeParams(&gGlobalParam.TMParam);
                    break;
               case NOT_PARAM_CM_PARAM:
                    CcbSyncSetConsumeMaterialParams(&gGlobalParam.CMParam);
                    break;
               case NOT_PARAM_FM_PARAM:
                    CcbSyncSetFmParams(&gGlobalParam.FmParam);
                    break;
                case NOT_PARAM_PM_PARAM:
                     CcbSyncSetPmParams(&gGlobalParam.PmParam);
                     break;
               case NOT_PARAM_MISC_PARAM:
                    CcbSyncSetMiscParams(&gGlobalParam.MiscParam);
                    break;
               }
               pItem++;
            }            
        }
        break;
    case DISP_NOT_ASC_INFO:
        {
            NOT_ASC_INFO_ITEM_STRU *pInfo = (NOT_ASC_INFO_ITEM_STRU *)pNotify->aucData;
            
            qDebug("on_dispIndication:DISP_NOT_ASC_INFO %s\r\n",DispGetAscInfo(pInfo->ucId));
        }
        break;
    case DISP_NOT_ECO:
        {
            QString strdataTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");//  hh:mm:ss

            qDebug("DISP_NOT_ECO\r\n");
            
            NOT_ECO_ITEM_STRU *pItem = (NOT_ECO_ITEM_STRU *)pNotify->aucData;
            while(pItem->ucId != 0XFF)
            {
                if (pItem->ucId < APP_EXE_ECO_NUM)
                {
                    m_EcoInfo[pItem->ucId].fQuality     = pItem->fValue;
                    m_EcoInfo[pItem->ucId].fTemperature = (pItem->usValue*1.0)/10;

                    //ex
                    switch(pItem->ucId)
                    {
                    case APP_EXE_I1_NO:
                    case APP_EXE_I2_NO:
                        if(m_EcowOfDay[pItem->ucId].iQuality > pItem->fValue)
                        {
                            m_EcowOfDay[pItem->ucId].iECOid    = pItem->ucId;
                            m_EcowOfDay[pItem->ucId].iQuality  = pItem->fValue;
                            m_EcowOfDay[pItem->ucId].iTemp     = pItem->usValue;
                            m_EcowOfDay[pItem->ucId].time      = strdataTime;
                        }
                        break;

                    case APP_EXE_I3_NO:
                    case APP_EXE_I4_NO:
                    case APP_EXE_I5_NO:
                        if(m_EcowOfDay[pItem->ucId].iQuality < pItem->fValue)
                        {
                            m_EcowOfDay[pItem->ucId].iECOid    = pItem->ucId;
                            m_EcowOfDay[pItem->ucId].iQuality  = pItem->fValue;
                            m_EcowOfDay[pItem->ucId].iTemp     = pItem->usValue;
                            m_EcowOfDay[pItem->ucId].time      = strdataTime;
                        }
                        break;
                    }
                    //end

                    m_EcowCurr[pItem->ucId].iECOid    = pItem->ucId;
                    m_EcowCurr[pItem->ucId].iQuality  = pItem->fValue;
                    m_EcowCurr[pItem->ucId].iTemp     = pItem->usValue;

                    /* update for eng windwos */
                    m_pOptDlg->EngNotify(pNotify->Hdr.ucCode,pItem);
                    m_pOptDlg->CfgNotify(pNotify->Hdr.ucCode,pItem);

                    updEcoInfo(pItem->ucId);

                    switch(pItem->ucId)
                    {
                    case APP_EXE_I1_NO:
                        if (m_EcoInfo[pItem->ucId].fQuality > gGlobalParam.MMParam.SP[MACHINE_PARAM_SP13])
                        {
                           alarmCommProc(true,DISP_ALARM_PART1,DISP_ALARM_PART1_HIGER_SOURCE_WATER_CONDUCTIVITY);
                        }
                        else if (m_iAlarmRcdMask[0][DISP_ALARM_PART1] & DISP_MAKE_ALARM(DISP_ALARM_PART1_HIGER_SOURCE_WATER_CONDUCTIVITY))
                        {
                            alarmCommProc(false,DISP_ALARM_PART1,DISP_ALARM_PART1_HIGER_SOURCE_WATER_CONDUCTIVITY);
                        }

                        if (m_EcoInfo[pItem->ucId].fTemperature > gGlobalParam.MMParam.SP[MACHINE_PARAM_SP18])
                        {
                           alarmCommProc(true,DISP_ALARM_PART1,DISP_ALARM_PART1_HIGHER_SOURCE_WATER_TEMPERATURE);
                           alarmCommProc(false,DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_SOURCE_WATER_TEMPERATURE);
                        }
                        else if (m_EcoInfo[pItem->ucId].fTemperature < gGlobalParam.MMParam.SP[MACHINE_PARAM_SP19])
                        {
                            alarmCommProc(true,DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_SOURCE_WATER_TEMPERATURE);
                            alarmCommProc(false,DISP_ALARM_PART1,DISP_ALARM_PART1_HIGHER_SOURCE_WATER_TEMPERATURE);
                        }
                        else if (m_iAlarmRcdMask[0][DISP_ALARM_PART1] & (DISP_ALARM_PART1_SOURCE_WATER_TEMP_ALARM))
                        {
                            alarmCommProc(false,DISP_ALARM_PART1,DISP_ALARM_PART1_HIGHER_SOURCE_WATER_TEMPERATURE);
                            alarmCommProc(false,DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_SOURCE_WATER_TEMPERATURE);
                        }
                       if ( m_bC1Regulator ) DispC1Regulator();  //test c1
                        break;
                    case APP_EXE_I2_NO:
                        if (DispGetPwFlag())
                        {
                            if (m_EcoInfo[pItem->ucId].fTemperature > gGlobalParam.MMParam.SP[MACHINE_PARAM_SP20])
                            {
                               alarmCommProc(true,DISP_ALARM_PART1,DISP_ALARM_PART1_HIGHER_RO_PRODUCT_TEMPERATURE);
                               alarmCommProc(false,DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_RO_PRODUCT_TEMPERATURE);
                            }
                            else if (m_EcoInfo[pItem->ucId].fTemperature < gGlobalParam.MMParam.SP[MACHINE_PARAM_SP21])
                            {
                                alarmCommProc(true,DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_RO_PRODUCT_TEMPERATURE);
                                alarmCommProc(false,DISP_ALARM_PART1,DISP_ALARM_PART1_HIGHER_RO_PRODUCT_TEMPERATURE);
                            }
                            else if (m_iAlarmRcdMask[0][DISP_ALARM_PART1] & (DISP_ALARM_PART1_RO_TEMP_ALARM))
                            {
                                alarmCommProc(false,DISP_ALARM_PART1,DISP_ALARM_PART1_HIGHER_RO_PRODUCT_TEMPERATURE);
                                alarmCommProc(false,DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_RO_PRODUCT_TEMPERATURE);
                            }
                        }
                        break;
                    case APP_EXE_I3_NO:
                        if (DispGetPwFlag())
                        {
                            if (m_EcoInfo[pItem->ucId].fTemperature > gGlobalParam.MMParam.SP[MACHINE_PARAM_SP22])
                            {
                               alarmCommProc(true,DISP_ALARM_PART1,DISP_ALARM_PART1_HIGHER_EDI_PRODUCT_TEMPERATURE);
                               alarmCommProc(false,DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_EDI_PRODUCT_TEMPERATURE);
                            }
                            else if (m_EcoInfo[pItem->ucId].fTemperature < gGlobalParam.MMParam.SP[MACHINE_PARAM_SP23])
                            {
                                alarmCommProc(true,DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_EDI_PRODUCT_TEMPERATURE);
                                alarmCommProc(false,DISP_ALARM_PART1,DISP_ALARM_PART1_HIGHER_EDI_PRODUCT_TEMPERATURE);
                            }
                            else if (m_iAlarmRcdMask[0][DISP_ALARM_PART1] & (DISP_ALARM_PART1_EDI_TEMP_ALARM))
                            {
                                alarmCommProc(false,DISP_ALARM_PART1,DISP_ALARM_PART1_HIGHER_EDI_PRODUCT_TEMPERATURE);
                                alarmCommProc(false,DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_EDI_PRODUCT_TEMPERATURE);
                            }
                        }
                        if((gGlobalParam.iMachineType == MACHINE_UP) || (gGlobalParam.iMachineType == MACHINE_RO))
                        {
                            if(gGlobalParam.MiscParam.ulMisFlags & (1 << DISP_SM_HP_Water_Cir))
                            {
                                if (m_EcoInfo[pItem->ucId].fQuality < gGlobalParam.MMParam.SP[MACHINE_PARAM_SP32])
                                {
                                   alarmCommProc(true,DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_HP_PRODUCT_WATER_CONDUCTIVITY);
                                }
                                else if (m_iAlarmRcdMask[0][DISP_ALARM_PART1] & DISP_MAKE_ALARM(DISP_ALARM_PART1_LOWER_HP_PRODUCT_WATER_CONDUCTIVITY))
                                {
                                    alarmCommProc(false,DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_HP_PRODUCT_WATER_CONDUCTIVITY);
                                }

                                //??????????????T Pack
                                if (DispGetTankCirFlag())
                                {
                                    if (m_EcoInfo[pItem->ucId].fQuality < gGlobalParam.MMParam.SP[MACHINE_PARAM_SP31])
                                    {
                                       //alarmCommProc(true,DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_CIR_WATER_CONDUCTIVITY);
                                        gCMUsage.ulUsageState |= (1 << DISP_T_PACKLIFEL);
                                    }
                                    else if (m_iAlarmRcdMask[0][DISP_ALARM_PART1] & DISP_MAKE_ALARM(DISP_ALARM_PART1_LOWER_CIR_WATER_CONDUCTIVITY))
                                    {
                                        //alarmCommProc(false,DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_CIR_WATER_CONDUCTIVITY);
                                        gCMUsage.ulUsageState &= ~(1 << DISP_T_PACKLIFEL);
                                    }
                                }
                            }
                        }
                        break;    
                    case APP_EXE_I4_NO:
                        if (DispGetTubeCirFlag())
                        {
                            if (m_EcoInfo[pItem->ucId].fTemperature > gGlobalParam.MMParam.SP[MACHINE_PARAM_SP26])
                            {
                               alarmCommProc(true,DISP_ALARM_PART1,DISP_ALARM_PART1_HIGHER_TUBE_TEMPERATURE);
                               alarmCommProc(false,DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_TUBE_TEMPERATURE);
                            }
                            else if (m_EcoInfo[pItem->ucId].fTemperature < gGlobalParam.MMParam.SP[MACHINE_PARAM_SP27])
                            {
                                alarmCommProc(true,DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_TUBE_TEMPERATURE);
                                alarmCommProc(false,DISP_ALARM_PART1,DISP_ALARM_PART1_HIGHER_TUBE_TEMPERATURE);
                            }
                            else if (m_iAlarmRcdMask[0][DISP_ALARM_PART1] & (DISP_ALARM_PART1_TUBE_TEMP_ALARM))
                            {
                                alarmCommProc(false,DISP_ALARM_PART1,DISP_ALARM_PART1_HIGHER_TUBE_TEMPERATURE);
                                alarmCommProc(false,DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_TUBE_TEMPERATURE);
                            }
                        }
                        else if (DispGetTocCirFlag())
                        {
                            if (m_EcoInfo[pItem->ucId].fTemperature > gGlobalParam.MMParam.SP[MACHINE_PARAM_SP28])
                            {
                               alarmCommProc(true,DISP_ALARM_PART1,DISP_ALARM_PART1_HIGHER_TOC_SENSOR_TEMPERATURE);
                               alarmCommProc(false,DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_TOC_SENSOR_TEMPERATURE);
                            }
                            else if (m_EcoInfo[pItem->ucId].fTemperature < gGlobalParam.MMParam.SP[MACHINE_PARAM_SP29])
                            {
                                alarmCommProc(true,DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_TOC_SENSOR_TEMPERATURE);
                                alarmCommProc(false,DISP_ALARM_PART1,DISP_ALARM_PART1_HIGHER_TOC_SENSOR_TEMPERATURE);
                            }
                            else if (m_iAlarmRcdMask[0][DISP_ALARM_PART1] & (DISP_ALARM_PART1_TOC_TEMP_ALARM))
                            {
                                alarmCommProc(false,DISP_ALARM_PART1,DISP_ALARM_PART1_HIGHER_TOC_SENSOR_TEMPERATURE);
                                alarmCommProc(false,DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_TOC_SENSOR_TEMPERATURE);
                            }
                        } 
                        else if (DispGetEdiQtwFlag()
                                 && ((gGlobalParam.iMachineType != MACHINE_UP) &&(gGlobalParam.iMachineType != MACHINE_RO)))
                        {
                            if (m_EcoInfo[pItem->ucId].fQuality < gGlobalParam.MMParam.SP[MACHINE_PARAM_SP32])
                            {
                               alarmCommProc(true,DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_HP_PRODUCT_WATER_CONDUCTIVITY);
                            }
                            else if (m_iAlarmRcdMask[0][DISP_ALARM_PART1] & DISP_MAKE_ALARM(DISP_ALARM_PART1_LOWER_HP_PRODUCT_WATER_CONDUCTIVITY))
                            {
                                alarmCommProc(false,DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_HP_PRODUCT_WATER_CONDUCTIVITY);
                            }
                        }
                        else if (DispGetTankCirFlag()
                                 && ((gGlobalParam.iMachineType != MACHINE_UP) &&(gGlobalParam.iMachineType != MACHINE_RO)))
                        {
                            //??????????????T Pack
                            if (m_EcoInfo[pItem->ucId].fQuality < gGlobalParam.MMParam.SP[MACHINE_PARAM_SP31])
                            {
                               //alarmCommProc(true,DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_CIR_WATER_CONDUCTIVITY);
                                gCMUsage.ulUsageState |= (1 << DISP_T_PACKLIFEL);
                            }
                            else if (m_iAlarmRcdMask[0][DISP_ALARM_PART1] & DISP_MAKE_ALARM(DISP_ALARM_PART1_LOWER_CIR_WATER_CONDUCTIVITY))
                            {
                                //alarmCommProc(false,DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_CIR_WATER_CONDUCTIVITY);
                                gCMUsage.ulUsageState &= ~(1 << DISP_T_PACKLIFEL);
                            }
                        }
                        break;                           
                    case APP_EXE_I5_NO:
                        if (DispGetUpQtwFlag())
                        {
                            if (m_EcoInfo[pItem->ucId].fTemperature > gGlobalParam.MMParam.SP[MACHINE_PARAM_SP24])
                            {
                               alarmCommProc(true,DISP_ALARM_PART1,DISP_ALARM_PART1_HIGHER_UP_PRODUCT_TEMPERATURE);
                               alarmCommProc(false,DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_UP_PRODUCT_TEMPERATURE);
                            }
                            else if (m_EcoInfo[pItem->ucId].fTemperature < gGlobalParam.MMParam.SP[MACHINE_PARAM_SP25])
                            {
                                alarmCommProc(true,DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_UP_PRODUCT_TEMPERATURE);
                                alarmCommProc(false,DISP_ALARM_PART1,DISP_ALARM_PART1_HIGHER_UP_PRODUCT_TEMPERATURE);
                            }
                            else if (m_iAlarmRcdMask[0][DISP_ALARM_PART1] & (DISP_ALARM_PART1_UP_TEMP_ALARM))
                            {
                                alarmCommProc(false,DISP_ALARM_PART1,DISP_ALARM_PART1_HIGHER_UP_PRODUCT_TEMPERATURE);
                                alarmCommProc(false,DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_UP_PRODUCT_TEMPERATURE);
                            }
                        }
                        break;                            
                    }

                    qDebug() << "I" <<pItem->ucId<< pItem->fValue <<pItem->usValue ;
                    
                }
                
                pItem++;
            }
#if 0   //2018-10-19
            //ex
            QString strCurDate = QDate::currentDate().toString("yyyy-MM-dd") + QString("%"); //"yyyy-MM-dd hh:mm:ss"
            for(WLoop = 0; WLoop < APP_EXE_ECO_NUM; WLoop++)
            {
                QString selectCurMsg = QString("SELECT id, ecoid, quality, time FROM Water where ecoid = %1 and time like '%2'")
                                                  .arg(WLoop).arg(strCurDate);
                bool ret = query.exec(selectCurMsg);
                if(!ret)
                {
                    break;
                }
                if(query.next())
                {
                    //update
                    int hEcoid = query.value(1).toInt();
                    double hQuality = query.value(2).toDouble();
                    QString updateCm = QString("update Water SET quality = ?, time = ? where ecoid = ? and time like '%1'").arg(strCurDate);
                    ex_query.prepare(updateCm);
                    switch(hEcoid)
                    {
                    case 0:
                    case 1:
                    {
                        if(hQuality > m_EcowOfDay[hEcoid].iQuality)
                        {
                            ex_query.addBindValue(m_EcowOfDay[hEcoid].iQuality);
                            ex_query.addBindValue(m_EcowOfDay[hEcoid].time);
                            ex_query.addBindValue(hEcoid);
                            ex_query.exec();
                        }
                        break;
                    }
                    case 2:
                    case 3:
                    case 4:
                    {
                        if(hQuality < m_EcowOfDay[hEcoid].iQuality)
                        {
                            ex_query.addBindValue(m_EcowOfDay[hEcoid].iQuality);
                            ex_query.addBindValue(m_EcowOfDay[hEcoid].time);
                            ex_query.addBindValue(hEcoid);
                            ex_query.exec();
                        }
                        break;
                    }
                    }
                }
                else
                {
                    //insert new
                    query.prepare(INSERT_sql_Water);
                    query.bindValue(":ecoid", m_EcowOfDay[WLoop].iECOid);
                    query.bindValue(":quality",m_EcowOfDay[WLoop].iQuality);
                    query.bindValue(":time", m_EcowOfDay[WLoop].time);
                    query.exec();

                }
            }
#endif
            //end
        }
        break;
    case DISP_NOT_PM:
        {
            NOT_PM_ITEM_STRU *pItem = (NOT_PM_ITEM_STRU *)pNotify->aucData;
            while(pItem->ucId != 0XFF)
            {
                if (pItem->ucId < APP_EXE_PRESSURE_METER)
                {

                    switch (pItem->ucId)
                    {
                    case APP_EXE_PM1_NO:
                        m_fPressure[pItem->ucId] = CcbConvert2Pm1Data(pItem->ulValue);
                        break;
                    case APP_EXE_PM2_NO:
                        {
                            m_fPressure[pItem->ucId] = CcbConvert2Pm2SP(pItem->ulValue);
                            
                            if (gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_HaveB2))
                            {
                                if (m_fPressure[pItem->ucId] < gGlobalParam.MMParam.SP[MACHINE_PARAM_SP6])
                                {
                                   alarmCommProc(true,DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_PWTANKE_WATER_LEVEL);
                                }
                                else if (m_iAlarmRcdMask[0][DISP_ALARM_PART1] & (DISP_ALARM_PART1_LOWER_PWTANKE_WATER_LEVEL_ALARM))
                                {
                                    alarmCommProc(false,DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_PWTANKE_WATER_LEVEL);
                                }
                            }
                        }
                        break;
                    case APP_EXE_PM3_NO:
                        {
                            m_fPressure[pItem->ucId] = CcbConvert2Pm3SP(pItem->ulValue);

                            if (gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_HaveB3))
                            {
                                if (m_fPressure[pItem->ucId] < gGlobalParam.MMParam.SP[MACHINE_PARAM_SP9])
                                {
                                   alarmCommProc(true,DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_SWTANKE_WATER_LEVEL);
                                }
                                else if (m_iAlarmRcdMask[0][DISP_ALARM_PART1] & (DISP_ALARM_PART1_LOWER_SWTANKE_WATER_LEVEL_ALARM))
                                {
                                    alarmCommProc(false,DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_SWTANKE_WATER_LEVEL);
                                }
                            }
                        }
                        break;
                    }
                    
                    qDebug() << "PM" << pItem->ucId << " " << pItem->ulValue;

                    /* update for eng windwos */
                    m_pOptDlg->EngNotify(pNotify->Hdr.ucCode,pItem);
                    
                    m_pOptDlg->CfgNotify(pNotify->Hdr.ucCode,pItem);
                    
                    if (!m_bSplash) updPressure(pItem->ucId);
                    
                }
                pItem++;
            }
        }
        break;
    case DISP_NOT_SW_PRESSURE:
        {
            NOT_PM_ITEM_STRU *pItem = (NOT_PM_ITEM_STRU *)pNotify->aucData;
            
            m_fPressure[APP_EXE_PM1_NO] = CcbConvert2Pm1Data(pItem->ulValue);
            
            m_fSourceWaterPressure = m_fPressure[APP_EXE_PM1_NO];

            if (NULL != m_pSubPages[PAGE_MENU])
            {
                MenuPage *page = (MenuPage *)m_pSubPages[PAGE_MENU];
            
                WaterQualityPage *subpage = (WaterQualityPage *)page->getSubPage(MENU_BTN_WATER_QUALITY_PARAMETER);
                
                subpage->updSwPressure(m_fPressure[APP_EXE_PM1_NO]);
            }
            
        }
        break;
    case DISP_NOT_FM:
        {
            NOT_FM_ITEM_STRU *pItem = (NOT_FM_ITEM_STRU *)pNotify->aucData;
            while(pItem->ucId != 0XFF)
            {
                 if (pItem->ucId < APP_FM_FLOW_METER_NUM)
                 {
                     unsigned int ulValue;
                     switch(pItem->ucId)
                     {
                     case APP_FM_FM1_NO:
                        ulValue = CcbConvert2Fm1Data(pItem->ulValue);
                        break;
                     case APP_FM_FM2_NO:
                        ulValue = CcbConvert2Fm2Data(pItem->ulValue);
                        break;
                     case APP_FM_FM3_NO:
                        ulValue = CcbConvert2Fm3Data(pItem->ulValue);
                        break;
                     case APP_FM_FM4_NO:
                        ulValue = CcbConvert2Fm4Data(pItem->ulValue);
                        break;
                     }

                     /* for FM statistic */
                     //m_ulFlowMeter[pItem->ucId] = ulValue;
                     saveFmData(pItem->ucId,ulValue);

                     updFlowInfo(pItem->ucId);
                     
                     /* update for eng windwos */
                     m_pOptDlg->EngNotify(pNotify->Hdr.ucCode,pItem);

                     m_pOptDlg->CfgNotify(pNotify->Hdr.ucCode,pItem);

                     qDebug() << "FM" <<pItem->ucId<< " " << ulValue;
                     
                 }
                 pItem++;
            }
        }
        break;
    case DISP_NOT_RECT:
        {
            NOT_RECT_ITEM_STRU *pItem = (NOT_RECT_ITEM_STRU *)pNotify->aucData;
            qDebug("DISP_NOT_RECT\r\n");
            
            while(pItem->ucId != 0XFF)
            {
                if (pItem->ucId < APP_EXE_RECT_NUM)
                {
       
                    //int iAlarmId = (DISP_ALARM_N1 + pItem->ucId);
                   // bool bAlarm  = (pItem->ulValue <= gGlobalParam.AlarmSettting.fAlarms[iAlarmId]);
                
                    /* Judge state */                     
                    m_pOptDlg->CfgNotify(pNotify->Hdr.ucCode,pItem);
                    
                    m_fRectifier[pItem->ucId] = CcbConvert2RectAndEdiData(pItem->ulValue);
           
                    //alarmCommProc(bAlarm,iAlarmId);

                    if (typeid(*m_pCurPage) == typeid(SysTestPage))
                    {
                        SysTestPage *page = (SysTestPage *)m_pCurPage;
                        page->updateInfo(pNotify->Hdr.ucCode,pItem->ucId,m_fRectifier[pItem->ucId]);
                    }
                    else if (typeid(*m_pCurPage) == typeid(SystemMonitorPage))
                    {
                        SystemMonitorPage *page = (SystemMonitorPage *)m_pCurPage;
                        page->updateRectInfo(pItem->ucId);
                    }
#ifdef FLOWCHART
                    else if (typeid(*m_pCurPage) == typeid(Ex_FlowChartPage))
                    {
                        Ex_FlowChartPage *page = (Ex_FlowChartPage *)m_pCurPage;
                        page->updateRectInfo(pItem->ucId);
                    }
#endif
                }
                pItem++;
            }
        }
        break;
    case DISP_NOT_GPUMP:
        {
            NOT_RECT_ITEM_STRU *pItem = (NOT_RECT_ITEM_STRU *)pNotify->aucData;
            qDebug("DISP_NOT_GPUMP\r\n");
            
            while(pItem->ucId != 0XFF)
            {
                 if (pItem->ucId < APP_EXE_G_PUMP_NUM)
                 {
                     /* Judge state */                     
                     m_pOptDlg->CfgNotify(pNotify->Hdr.ucCode,pItem);
    
                     //m_pEditGPump[pItem->ucId]->setText(QString::number(CcbConvert2GPumpData(pItem->ulValue)));

                     m_fPumpV[pItem->ucId] = CcbConvert2GPumpData(pItem->ulValue);

                    if (typeid(*m_pCurPage) == typeid(SysTestPage))
                    {
                        SysTestPage *page = (SysTestPage *)m_pCurPage;
                    
                        page->updateInfo(pNotify->Hdr.ucCode, pItem->ucId,m_fPumpV[pItem->ucId]);
                    }
                    else if (typeid(*m_pCurPage) == typeid(SystemMonitorPage))
                    {
                        SystemMonitorPage *page = (SystemMonitorPage *)m_pCurPage;
                        page->updateGpumpInfo(pItem->ucId);
                    }
#ifdef FLOWCHART
                    else if (typeid(*m_pCurPage) == typeid(Ex_FlowChartPage))
                    {
                        Ex_FlowChartPage *page = (Ex_FlowChartPage *)m_pCurPage;
                        page->updateGpumpInfo(pItem->ucId);
                    }
#endif
                 }
                pItem++;
            }
        }
        break;
    case DISP_NOT_RPUMP:
        {
            NOT_RECT_ITEM_STRU *pItem = (NOT_RECT_ITEM_STRU *)pNotify->aucData;
            
            while(pItem->ucId != 0XFF)
            {
                 if (pItem->ucId < APP_EXE_R_PUMP_NUM)
                 {
                     /* Judge state */                     
                     m_pOptDlg->CfgNotify(pNotify->Hdr.ucCode,pItem);

                     m_fRPumpV[pItem->ucId] = CcbGetRPumpVData(pItem->ucId);

                     m_fRPumpI[pItem->ucId] = CcbConvert2RPumpIData(pItem->ulValue);

                     if (typeid(*m_pCurPage) == typeid(SysTestPage))
                     {
                         SysTestPage *page = (SysTestPage *)m_pCurPage;
                         
                         page->updateInfo(pNotify->Hdr.ucCode,(1<<8) | pItem->ucId,m_fRPumpV[pItem->ucId]);

                         page->updateInfo(pNotify->Hdr.ucCode, pItem->ucId,m_fRPumpI[pItem->ucId]);
                     }
                     else if (typeid(*m_pCurPage) == typeid(SystemMonitorPage))
                     {
                         SystemMonitorPage *page = (SystemMonitorPage *)m_pCurPage;
                         
                         page->updateRpumpInfo(pItem->ucId);
                     }  
#ifdef FLOWCHART
                     else if (typeid(*m_pCurPage) == typeid(Ex_FlowChartPage))
                     {
                         Ex_FlowChartPage *page = (Ex_FlowChartPage *)m_pCurPage;

                         page->updateRpumpInfo(pItem->ucId);
                     }
#endif
                     qDebug("DISP_NOT_RPUMP %d\r\n",pItem->ucId);
                 }
                pItem++;
            }
        }
        break;
    case DISP_NOT_EDI:
        {
            NOT_RECT_ITEM_STRU *pItem = (NOT_RECT_ITEM_STRU *)pNotify->aucData;
            qDebug("DISP_NOT_EDI\r\n");
            
            while(pItem->ucId != 0XFF)
            {
                 if (pItem->ucId < APP_EXE_EDI_NUM)
                 {
                     //int iAlarmId    = (DISP_ALARM_EDI + pItem->ucId);
                     //bool bAlarm = (pItem->ulValue <= gGlobalParam.AlarmSettting.fAlarms[iAlarmId]);
                 
                     m_pOptDlg->CfgNotify(pNotify->Hdr.ucCode,pItem);
                     
                     m_fEDI[pItem->ucId] = CcbConvert2RectAndEdiData(pItem->ulValue);

                     if (typeid(*m_pCurPage) == typeid(SysTestPage))
                     {
                         SysTestPage *page = (SysTestPage *)m_pCurPage;
                         
                         page->updateInfo(pNotify->Hdr.ucCode,pItem->ucId,m_fEDI[pItem->ucId]);
                     }
                     else if (typeid(*m_pCurPage) == typeid(SystemMonitorPage))
                     {
                         SystemMonitorPage *page = (SystemMonitorPage *)m_pCurPage;
                         page->updateEdiInfo(pItem->ucId);
                     }  
#ifdef FLOWCHART
                     else if (typeid(*m_pCurPage) == typeid(Ex_FlowChartPage))
                     {
                         Ex_FlowChartPage *page = (Ex_FlowChartPage *)m_pCurPage;
                         page->updateEdiInfo(pItem->ucId);
                     }
#endif
                     //alarmCommProc(bAlarm,iAlarmId);

                 }
                 pItem++;
            } 
        }
        break;
    case DISP_NOT_NV_STAT:
        {
            NOT_NVS_ITEM_STRU *pItem = (NOT_NVS_ITEM_STRU *)pNotify->aucData;
            while(pItem->ucId != 0XFF)
            {
                 if (pItem->ucId < APP_EXE_RECT_NUM)
                 {
                     switch(pItem->ucId)
                     {
                     case 0:
                        gCMUsage.cmInfo.aulCumulatedData[DISP_N1_UVLIFEHOUR] += pItem->ulValue;                       
                        break;
                     case 1:
                        gCMUsage.cmInfo.aulCumulatedData[DISP_N2_UVLIFEHOUR] += pItem->ulValue;                       
                        break;
                     case 2:
                        gCMUsage.cmInfo.aulCumulatedData[DISP_N3_UVLIFEHOUR] += pItem->ulValue;                       
                        break;
                     }
                 }
                pItem++;
            }
            
        }
        break;
    case DISP_NOT_TUBEUV_STATE:
        {
            NOT_NVS_ITEM_STRU *pItem = (NOT_NVS_ITEM_STRU *)pNotify->aucData;
            while(pItem->ucId != 0XFF)
            {
                 switch(pItem->ucId)
                 {
                 case 0:
                    gCMUsage.cmInfo.aulCumulatedData[DISP_N4_UVLIFEHOUR] += pItem->ulValue;                       
                    break;
                 default:
                    break;
                 }
                 pItem++;
            }
            
        }        
        break;
    case DISP_NOT_FM_STAT:
        {
            NOT_FM_ITEM_STRU *pItem = (NOT_FM_ITEM_STRU *)pNotify->aucData;
            unsigned int ulQuantity;
            while(pItem->ucId != 0XFF)
            {

                 switch(pItem->ucId)
                 {
                 case APP_FM_FM1_NO:
                     ulQuantity = CcbConvert2Fm1Data(pItem->ulValue);
                     break;
                 case APP_FM_FM2_NO:
                     break;
                 case APP_FM_FM3_NO:
                 {
                     ulQuantity = CcbConvert2Fm3Data(pItem->ulValue);

                     switch(gGlobalParam.iMachineType)
                     {
                     case MACHINE_L_Genie:
                     case MACHINE_L_UP:
                     case MACHINE_L_EDI_LOOP:
                     case MACHINE_L_RO_LOOP:
                     {
                         gCMUsage.cmInfo.aulCumulatedData[DISP_P_PACKLIFEL] += ulQuantity;
                         gCMUsage.cmInfo.aulCumulatedData[DISP_AC_PACKLIFEL] += ulQuantity;
                         gCMUsage.cmInfo.aulCumulatedData[DISP_AT_PACKLIFEL] += ulQuantity;
//                         gCMUsage.cmInfo.aulCumulatedData[DISP_PRE_PACKLIFEL] += ulQuantity;
                     }
                         break;
                     default:
                         break;
                     }
                 }
                     break;
                 case APP_FM_FM4_NO:
                 {
                     ulQuantity = CcbConvert2Fm4Data(pItem->ulValue);

                     switch(gGlobalParam.iMachineType)
                     {
                     case MACHINE_L_Genie:
                     case MACHINE_L_UP:
                     case MACHINE_L_EDI_LOOP:
                     case MACHINE_L_RO_LOOP:
                     {
                         gCMUsage.cmInfo.aulCumulatedData[DISP_P_PACKLIFEL] += ulQuantity;
                         gCMUsage.cmInfo.aulCumulatedData[DISP_AC_PACKLIFEL] += ulQuantity;
//                         gCMUsage.cmInfo.aulCumulatedData[DISP_PRE_PACKLIFEL] += ulQuantity;
                     }
                         break;
                     default:
                         break;
                     }
                 }
                     break;
                 default:
                     break;
                 }
                 pItem++;
            }
            
        }        
        break;
    case DISP_NOT_TW_STAT:
        {
            NOT_NVS_ITEM_STRU *pItem = (NOT_NVS_ITEM_STRU *)pNotify->aucData;
            while(pItem->ucId != 0XFF)
            {
                time_t timer      = pItem->ulBgnTime;
                struct tm *tblock = localtime(&timer);
                QString strTime;

                strTime.sprintf("%04d-%02d-%02d %02d:%02d:%02d",tblock->tm_year + 1900,tblock->tm_mon + 1,tblock->tm_mday,tblock->tm_hour,tblock->tm_min,tblock->tm_sec);

                unsigned int   ulQuantity = CcbConvert2Fm1Data(pItem->ulValue);  
                float          fQuantity  = ulQuantity/ 1000.0;
            
                 switch(pItem->ucType)
                 {
                 case APP_DEV_HS_SUB_REGULAR:
                    {
                         float tmpI4 = (m_EcowCurr[APP_EXE_I4_NO].iTemp*1.0)/10;
                         query.prepare(INSERT_sql_GetW);
                         query.bindValue(":name", "HP");
                         query.bindValue(":quantity",fQuantity);
                         query.bindValue(":quality",m_EcowCurr[APP_EXE_I4_NO].iQuality);
                         query.bindValue(":tmp",tmpI4);
                         query.bindValue(":time", strTime);
                         bDbResult = query.exec();

                         updQtwState(APP_DEV_HS_SUB_REGULAR,false);
                     }                    
                    break;
                 case APP_DEV_HS_SUB_HYPER:
                    {
                         float tmpI5 = (m_EcowCurr[APP_EXE_I5_NO].iTemp*1.0)/10;
                         gCMUsage.cmInfo.aulCumulatedData[DISP_U_PACKLIFEL] += ulQuantity;
                         gCMUsage.cmInfo.aulCumulatedData[DISP_H_PACKLIFEL] += ulQuantity;
                         query.prepare(INSERT_sql_GetW);
                         query.bindValue(":name", "UP");
                         query.bindValue(":quantity",fQuantity);
                         query.bindValue(":quality" ,m_EcowCurr[APP_EXE_I5_NO].iQuality);
                         query.bindValue(":tmp"     ,tmpI5);
                         query.bindValue(":time"    ,strTime);
                         bDbResult = query.exec();

                         updQtwState(APP_DEV_HS_SUB_HYPER,false);
                    }                    
                    break;
                 }


                 pItem++;
                 qDebug() << "TW: " << fQuantity << "TIME: "<< strTime << bDbResult << endl;
            }
            
        }
        break;
    case DISP_NOT_PWDURATION_STAT:
        {
            NOT_WP_ITEM_STRU *pItem = (NOT_WP_ITEM_STRU *)pNotify->aucData;
            {
                time_t timer = pItem->ulBgnTime;
                time_t etime = time(NULL);
                struct tm *tblock = localtime(&timer);
                unsigned int ulDelSec = etime - timer;
                QString strTime;

                strTime.sprintf("%04d-%02d-%02d %02d:%02d:%02d",tblock->tm_year + 1900,tblock->tm_mon + 1,tblock->tm_mday,tblock->tm_hour,tblock->tm_min,tblock->tm_sec);
            
                {
                    double res = 0;

                    if (m_EcowCurr[APP_EXE_I1_NO].iQuality != 0)
                    {
                        res = (m_EcowCurr[APP_EXE_I1_NO].iQuality - m_EcowCurr[APP_EXE_I2_NO].iQuality)/m_EcowCurr[APP_EXE_I1_NO].iQuality ;
                    }
                    query.prepare(INSERT_sql_PW);
                    query.bindValue(":duration", ulDelSec/60); // to minute
                    query.bindValue(":ecoroin",m_EcowCurr[APP_EXE_I1_NO].iQuality);
                    query.bindValue(":tmproin", (m_EcowCurr[APP_EXE_I1_NO].iTemp*1.0)/10);

                    query.bindValue(":ecorores",res);

                    query.bindValue(":ecoropw",m_EcowCurr[APP_EXE_I2_NO].iQuality);
                    query.bindValue(":tmpropw", (m_EcowCurr[APP_EXE_I2_NO].iTemp*1.0)/10);

                    query.bindValue(":ecoedi", m_EcowCurr[APP_EXE_I3_NO].iQuality);
                    query.bindValue(":tmpedi", (m_EcowCurr[APP_EXE_I3_NO].iTemp*1.0)/10);
                    query.bindValue(":time", strTime);
                    bDbResult = query.exec();

                    qDebug() <<"PW: " << ulDelSec << "TIME: "<< strTime << bDbResult << endl;
                    
                }

                 pItem++;
            }
            
        }
        break;   
    case DISP_NOT_PWVOLUME_STAT:
        {
            NOT_FMS_ITEM_STRU *pItem = (NOT_FMS_ITEM_STRU *)pNotify->aucData;
            
            while(pItem->ucId != 0XFF)
            {
                 //unsigned int ulQuantity;   
                 
                 if (pItem->ucId < APP_FM_FLOW_METER_NUM)
                 {
                     switch(pItem->ucId)
                     {
                     case APP_FM_FM3_NO:
                        {
                            //ulQuantity = CcbConvert2Fm3Data(pItem->ulValue);  
                        }
                        break;
                     case APP_FM_FM4_NO:
                        {
                            //ulQuantity = CcbConvert2Fm4Data(pItem->ulValue);  
                        }
                        break;
                     }
                 }
                pItem++;
            }
            
            qDebug() <<"DISP_NOT_PWVOLUME_STAT : " << endl;

//            updatePackFlow(); //ex 2018.10.26
            
            if (typeid(*m_pCurPage) == typeid(SystemMonitorPage))
            {
                SystemMonitorPage *page = (SystemMonitorPage *)m_pCurPage;
                page->updateWorkState();
            }
#ifdef FLOWCHART
            if (typeid(*m_pCurPage) == typeid(Ex_FlowChartPage))
            {
                Ex_FlowChartPage *page = (Ex_FlowChartPage *)m_pCurPage;
                page->updateWorkState();
            }
#endif
            if (typeid(*m_pCurPage) == typeid(MainPage))
            {
                pMainPage->updMainpageState();
            }
            
        }
        break;          
    case DISP_NOT_STATE:
        {
            NOT_STATE_ITEM_STRU *pItem = (NOT_STATE_ITEM_STRU *)pNotify->aucData;

            switch(pItem->ucId)
            {
            case NOT_STATE_INIT:
                {
                    qDebug("on_dispIndication:DISP_NOT_STATE DISP_NOT_STATE NOT_STATE_INIT \r\n");

#ifdef D_HTTPWORK
                    m_networkData.runStatus = 0;
#endif
                    //2019.6.3
                    ex_gGlobalParam.lastRunState = 0;
                    MainSaveLastRunState(gGlobalParam.iMachineType);

                    if (typeid(*m_pCurPage) == typeid(MainPage))
                    {
                         pMainPage->updateRunInfo(false);                         
                    }
                }
                break;
            case NOT_STATE_RUN:
                {
                    qDebug("on_dispIndication:DISP_NOT_STATE NOT_STATE_RUN \r\n");
#ifdef D_HTTPWORK
                    m_networkData.runStatus = 1;
#endif
                    //2019.6.3
                    ex_gGlobalParam.lastRunState = 1;
                    MainSaveLastRunState(gGlobalParam.iMachineType);

                    if (typeid(*m_pCurPage) == typeid(MainPage))
                    {
                        pMainPage->updateRunInfo(true);
                        
                    }
                    
                }
                break;
            case NOT_STATE_LPP:
                {
                    qDebug("on_dispIndication:DISP_NOT_STATE NOT_STATE_LPP \r\n");
                    if (typeid(*m_pCurPage) == typeid(MainPage))
                    {
                        //pMainPage->updateRunInfo(false);
                    }
                }
                break;
            case NOT_STATE_QTW:
                qDebug("on_dispIndication:DISP_NOT_STATE NOT_STATE_QTW \r\n");
                break;
            case NOT_STATE_CIR:
                qDebug("on_dispIndication:DISP_NOT_STATE NOT_STATE_CIR \r\n");
                break;
            case NOT_STATE_DEC:
                qDebug("on_dispIndication:DISP_NOT_STATE NOT_STATE_DEC \r\n");
                break;
            }

//            updatePackFlow(); //ex 2018.10.26
            
            if (typeid(*m_pCurPage) == typeid(SystemMonitorPage))
            {
                SystemMonitorPage *page = (SystemMonitorPage *)m_pCurPage;
                page->updateWorkState();
            }
#ifdef FLOWCHART
            if (typeid(*m_pCurPage) == typeid(Ex_FlowChartPage))
            {
                Ex_FlowChartPage *page = (Ex_FlowChartPage*)m_pCurPage;
                page->updateWorkState();
            }
#endif
            if (typeid(*m_pCurPage) == typeid(MainPage))
            {
                pMainPage->updMainpageState();
            }
            
        }
        break;
    case DISP_NOT_ALARM:
        {
            qDebug("on_dispIndication:DISP_NOT_ALARM \r\n");

            NOT_ALARM_ITEM_STRU *pInfo = (NOT_ALARM_ITEM_STRU *)pNotify->aucData;

            qDebug() << QString("Targer Alarm: %1; %2; %3 ")
                        .arg(!!pInfo->ucFlag).arg(pInfo->ucPart).arg(pInfo->ucId);
            
            switch(pInfo->ucPart)
            {
            case DISP_ALARM_PART0:
                if (pInfo->ucId < DISP_ALARM_PART0_NUM)
                {
//                    alarmCommProc(!!pInfo->ucFlag,pInfo->ucPart,pInfo->ucId);
                }
                break;
            case DISP_ALARM_PART1:
                if (pInfo->ucId < DISP_ALARM_PART1_NUM)
                {
                    alarmCommProc(!!pInfo->ucFlag,pInfo->ucPart,pInfo->ucId);
                }
                break;
            case 0XFF:
                {
                    switch(pInfo->ucId)
                    {
                    case DISP_ALARM_B_LEAK:
                        alarmCommProc(!!pInfo->ucFlag, DISP_ALARM_PART1, DISP_ALARM_PART1_LEAK_OR_TANKOVERFLOW);
                        /* show lockup dialog */
                        if (!m_bLockupDlg && pInfo->ucFlag)
                        {
                            LockupDlg dlg;
                            m_bLockupDlg = true;
                            dlg.exec() ;
                        }
                        break;                      
                    default :
                        break;
                    }       
                }
                break;
            }
        }
        break;
    case DISP_NOT_WH_STAT:
        {
            NOT_WH_ITEM_STRU *pItem = (NOT_WH_ITEM_STRU *)pNotify->aucData;
            {
                
                if (typeid(*m_pCurPage) == typeid(SterilizePage))
                {
                    SterilizePage *page = (SterilizePage *)m_pCurPage;
                    page->updateRunInfo(pItem->ucType,!!pItem->ucState);
                }
            }            
        }
        break;
    case DISP_NOT_RF_STATE:
        {
            /* do something here */
            NOT_RFID_ITEM_STRU *pItem = (NOT_RFID_ITEM_STRU *)pNotify->aucData;

            if (pItem->ucId < APP_RFID_SUB_TYPE_NUM)
            {
                if (pItem->ucState)
                {
                    int iRet ;

                    m_iRfidActiveMask       |= (1 << pItem->ucId);
                    m_iRfidBufferActiveMask |= (1 << pItem->ucId);
                    rmvRfidFromDelayList(pItem->ucId);

                    iRet = readRfid(pItem->ucId);

                    qDebug() << "readRfid " << pItem->ucId << iRet; 

                }
                else
                {
                    if (m_iRfidBufferActiveMask & (1 << pItem->ucId))
                    {
                        /* delay a moment */
                        addRfid2DelayList(pItem->ucId);
                    }
                    //Set to idle when abnormal 2019.06.19
                    m_checkConsumaleInstall[pItem->ucId]->setBusystatus(false);

                    m_iRfidActiveMask &= ~(1 << pItem->ucId);
                    
                    m_aRfidInfo[pItem->ucId].ucValid = 0;
                }
            }

            switch(m_eWorkMode)
            {
            case APP_WORK_MODE_NORMAL:
                if(DISP_WORK_STATE_IDLE == DispGetWorkState4Pw()
                   && m_startCheckConsumale)
                {
                    checkConsumableInstall(pItem->ucId); //2019.1.21
                }
                break;
            case APP_WORK_MODE_CLEAN:
                switch(pItem->ucId)
                {
                case APP_RFID_SUB_TYPE_PPACK_CLEANPACK:
                    if (m_pCurPage && typeid(*m_pCurPage) == typeid(SterilizePage))
                    {
                        SterilizePage *page = (SterilizePage *)m_pCurPage;
                        page->updateRfidInfo(pItem->ucId);
                    }
                    break;
                }                
                break;
            case APP_WORK_MODE_INSTALL:
                if (m_pCurPage && typeid(*m_pCurPage) == typeid(ConsumableInsPage))
                {
                    ConsumableInsPage *page = (ConsumableInsPage *)m_pCurPage;
                    page->updateRfidInfo(pItem->ucId);
                }
                break;
                
            }
            if (!m_bSplash && m_pCurPage && (typeid(*m_pCurPage) == typeid(SystemMonitorPage)))
            {
                SystemMonitorPage *page = (SystemMonitorPage *)m_pCurPage;
                page->updateDeviceState();
            }

        }
        break;
    case DISP_NOT_HANDLER_STATE:
        {
            /* do something here */
            
            NOT_HANDLER_ITEM_STRU *pItem = (NOT_HANDLER_ITEM_STRU *)pNotify->aucData;

            if (pItem->ucState)
            {
                m_iHandlerActiveMask |= 1 << pItem->ucId;
            }
            else
            {
                m_iHandlerActiveMask &= ~(1 << pItem->ucId);
            }
            qDebug() << "DISP_NOT_HANDLER_STATE " << pItem->ucId << pItem->ucState; 
            
            if (!m_bSplash && m_pCurPage && (typeid(*m_pCurPage) == typeid(SystemMonitorPage)))
            {
                SystemMonitorPage *page = (SystemMonitorPage *)m_pCurPage;
                page->updateDeviceState();
            }
        }
        break;
    case DISP_NOT_SPEED:
        {
            NOT_SPEED_ITEM_STRU *pItem = (NOT_SPEED_ITEM_STRU *)pNotify->aucData;

            if (typeid(*m_pCurPage) == typeid(MainPage))
            {
                pMainPage->updSpeed(pItem->iType,pItem->iSpeed);
            }
        }
        break;
    case DISP_NOT_DEC_PRESSURE:
        {
            NOT_DECPRE_ITEM_STRU *pItem = (NOT_DECPRE_ITEM_STRU *)pNotify->aucData;
            if (typeid(*m_pCurPage) == typeid(MainPage))
            {
                pMainPage->updDecPressureState(pItem->iType,pItem->iAction);          
                if(pItem->iAction)
                {
                    //ex
                    ex_gCcb.Ex_Delay_Tick.ulDecPressure = ex_gulSecond;
                    ex_gCcb.EX_Check_State.bit1CheckDecPressure = 1;
                    //end

                    DispSetSubWorkState4Pw(DISP_WORK_SUB_IDLE_DEPRESSURE);
                }
                else
                {
                    //ex
                    ex_gCcb.EX_Check_State.bit1CheckDecPressure = 0;
                    //end

                    DispSetSubWorkState4Pw(DISP_WORK_SUB_IDLE);
                }
                
                pMainPage->updMainpageState();
            }
        }
        break;
    case DISP_NOT_EXEBRD_STATE:
        {
            /* do something here */
            
            NOT_EXEBRD_ITEM_STRU *pItem = (NOT_EXEBRD_ITEM_STRU *)pNotify->aucData;

            if (pItem->ucState)
            {
                m_iExeActiveMask |= 1 << pItem->ucId;
            }
            else
            {
                m_iExeActiveMask &= ~(1 << pItem->ucId);
            }
            
            if (!m_bSplash && m_pCurPage && (typeid(*m_pCurPage) == typeid(SystemMonitorPage)))
            {
                SystemMonitorPage *page = (SystemMonitorPage *)m_pCurPage;
                page->updateDeviceState();
            }
        }        
        break;
    case DISP_NOT_FMBRD_STATE:
        {
            /* do something here */
            
            NOT_FMBRD_ITEM_STRU *pItem = (NOT_FMBRD_ITEM_STRU *)pNotify->aucData;

            if (pItem->ucState)
            {
                m_iFmActiveMask |= 1 << pItem->ucId;
            }
            else
            {
                m_iFmActiveMask &= ~(1 << pItem->ucId);
            }
            
            if (!m_bSplash && m_pCurPage && (typeid(*m_pCurPage) == typeid(SystemMonitorPage)))
            {
                SystemMonitorPage *page = (SystemMonitorPage *)m_pCurPage;
                page->updateDeviceState();
            }
        }        
        break;
    case DISP_NOT_SWITCH_STATE:
        {
            qDebug("on_dispIndication:DISP_NOT_SWITCH_STATE \r\n");
            //updateRectState(); //ex
            
            if (!m_bSplash && (typeid(*m_pCurPage) == typeid(SystemMonitorPage)))
            {
                SystemMonitorPage *page = (SystemMonitorPage *)m_pCurPage;
                page->updateSwitchInfo();
            }
#ifdef FLOWCHART
            if (!m_bSplash && (typeid(*m_pCurPage) == typeid(Ex_FlowChartPage)))
            {
                Ex_FlowChartPage *page = (Ex_FlowChartPage *)m_pCurPage;
                page->updateSwitchInfo();
            }
#endif
        }
        break;
    case DISP_NOT_RPUMP_STATE:
        {
            
            qDebug("DISP_NOT_RPUMP\r\n");
            
            if (!m_bSplash && (typeid(*m_pCurPage) == typeid(SystemMonitorPage)))
            {
                SystemMonitorPage *page = (SystemMonitorPage *)m_pCurPage;

                NOT_RPUMP_STATE_ITEM_STRU *pItem = (NOT_RPUMP_STATE_ITEM_STRU *)pNotify->aucData;
                
                page->updateRpumpInfo(pItem->ucId);
            }
#ifdef FLOWCHART
            if (!m_bSplash && (typeid(*m_pCurPage) == typeid(Ex_FlowChartPage)))
            {
                Ex_FlowChartPage *page = (Ex_FlowChartPage *)m_pCurPage;

                NOT_RPUMP_STATE_ITEM_STRU *pItem = (NOT_RPUMP_STATE_ITEM_STRU *)pNotify->aucData;

                page->updateRpumpInfo(pItem->ucId);
            }
#endif
            
        }
        break;
    case DISP_NOT_TOC:
        {
            NOT_TOC_ITEM_STRU *pItem = (NOT_TOC_ITEM_STRU *)pNotify->aucData;

            float fToc;
#if 0
            if(pItem->fP > 4)
            {
                fToc = 2;
            }
            else
            {
                fToc = 6.395*pow(pItem->fP, 4) - 91.15*pow(pItem->fP, 3) + 479*pow(pItem->fP, 2) - 1102*(pItem->fP) + 940;
            }
//            float fToc = 6.395*pow(pItem->fP, 4) - 91.15*pow(pItem->fP, 3) + 479*pow(pItem->fP, 2) - 1102*(pItem->fP) + 940;
#endif
            if(pItem->fP < 2.5)
            {
                fToc = -315.5*pow(pItem->fP, 3) + 2041*pow(pItem->fP, 2) - 4404*(pItem->fP) + 3195;
            }
            else
            {
                fToc = -29.14 * log(pItem->fP) + 42;
            }

            if(fToc < 2)
            {
                fToc = 2;
            }
            if(fToc > 200)
            {
                fToc = 200;
            }

            pMainPage->updToc(fToc);

#ifdef D_HTTPWORK
            m_networkData.fToc = fToc;
#endif

            //2018.11.13
            if (NULL != m_pSubPages[PAGE_MENU])
            {
                MenuPage *page = (MenuPage *)m_pSubPages[PAGE_MENU];

                WaterQualityPage *subpage = (WaterQualityPage *)page->getSubPage(MENU_BTN_WATER_QUALITY_PARAMETER);

                subpage->updTOC(fToc);
            }
            //end
#ifdef FLOWCHART
            if (NULL != m_pSubPages[PAGE_FLOWCHART])
            {
                Ex_FlowChartPage *page = (Ex_FlowChartPage *)m_pSubPages[PAGE_FLOWCHART];
                page->updTOC(fToc);
            }
#endif
            DispSndHoPpbAndTankLevel(APP_PROTOL_CANID_BROADCAST,APP_PACKET_HO_QL_TYPE_PPB,0,fToc);
        }        
        break;
    case DISP_NOT_UPD:
        {
            NOT_UPD_ITEM_STRU *pItem = (NOT_UPD_ITEM_STRU *)pNotify->aucData;
            
            switch(pItem->iType)
            {
            case 0:
                {
                    SetDevicePage *page = getDeviceDlg();
                    if (page) page->zigbeeUpdResult(pItem->iResult,pItem->iPercent);
                        
                }
                break;
            }
        }        
        break;
    case DISP_NOT_QTW_VOLUME:
        {           
            NOT_QTW_VOLUME_ITEM_STRU *pItem = (NOT_QTW_VOLUME_ITEM_STRU *)pNotify->aucData;
            
            DB_HANDLER_STRU *hdl = getDefaultHandler(pItem->ucType);

            if (hdl)
            {
                qDebug("DISP_NOT_QTW_VOLUME %d&%d&%d\r\n",pItem->ulValue,hdl->address,pItem->ucId);
            
                if (CcbGetHandlerId2Index(hdl->address) == pItem->ucId)
                {
                    setWaterQuantity(pItem->ucType,pItem->ulValue / APP_QTW_UNIT);
                }
            }
        }
        break;
    }
    mem_free(pucData);
    
    //qDebug("ou Ind %d %d\r\n" ,iSeq1,++iSeq2);
}

void MainWindow :: addRfid2DelayList(int iRfId)
{
    m_iRfidDelayedMask |= (1 << iRfId);
    m_aiRfidDelayedCnt[iRfId] = 15;
}

void MainWindow :: rmvRfidFromDelayList(int iRfId)
{
    //printf("rmvRfidFromDelayList %x %x %x",m_iRfidDelayedMask,(1 << iRfId),m_iAlarmRcdMask[0][DISP_ALARM_PART0]);
    if (APP_WORK_MODE_NORMAL == m_eWorkMode)
    {
        /*raise alarm */
        switch(MacRfidMap.aiDeviceType4Normal[iRfId])
        {
        case DISP_P_PACK:
            if (m_iAlarmRcdMask[0][DISP_ALARM_PART0] & DISP_MAKE_ALARM(DISP_ALARM_PART0_PPACK_OOP))
            {
                alarmCommProc(false,DISP_ALARM_PART0,DISP_ALARM_PART0_PPACK_OOP);
                checkConsumableInstall(iRfId);
            }
            break;
        case DISP_PRE_PACK:
            if (m_iAlarmRcdMask[0][DISP_ALARM_PART0] & DISP_MAKE_ALARM(DISP_ALARM_PART0_PREPACK_OOP))
            {
                alarmCommProc(false,DISP_ALARM_PART0,DISP_ALARM_PART0_PREPACK_OOP);
                checkConsumableInstall(iRfId);
            }
            break;
        case DISP_AC_PACK:
            if (m_iAlarmRcdMask[0][DISP_ALARM_PART0] & DISP_MAKE_ALARM(DISP_ALARM_PART0_ACPACK_OOP))
            {
                alarmCommProc(false,DISP_ALARM_PART0,DISP_ALARM_PART0_ACPACK_OOP);
                checkConsumableInstall(iRfId);
            }
            break;
        case DISP_U_PACK:
            if (m_iAlarmRcdMask[0][DISP_ALARM_PART0] & DISP_MAKE_ALARM(DISP_ALARM_PART0_UPACK_OOP))
            {
                alarmCommProc(false,DISP_ALARM_PART0,DISP_ALARM_PART0_UPACK_OOP);
                checkConsumableInstall(iRfId);
            }
            break;
        case DISP_AT_PACK:
            if (m_iAlarmRcdMask[0][DISP_ALARM_PART0] & DISP_MAKE_ALARM(DISP_ALARM_PART0_ATPACK_OOP))
            {
                alarmCommProc(false,DISP_ALARM_PART0,DISP_ALARM_PART0_ATPACK_OOP);
                checkConsumableInstall(iRfId);
            }
            break;
        case DISP_H_PACK:
            if (m_iAlarmRcdMask[0][DISP_ALARM_PART0] & DISP_MAKE_ALARM(DISP_ALARM_PART0_HPACK_OOP))
            {
                alarmCommProc(false,DISP_ALARM_PART0,DISP_ALARM_PART0_HPACK_OOP);
                checkConsumableInstall(iRfId);
            }
            break;
            
        }
    }
    m_iRfidDelayedMask &= ~(1 << iRfId);
}

void MainWindow::on_AutoLogin(void)
{
    static bool bEnter = false;

    if (bEnter ) return ;

    qDebug("on_AutoLogin \r\n");

    bEnter = true;

    {
        LoginDlg Login;
        Login.setGeometry(200,100,300,200);
        Login.show();
        
        if (Login.exec() == QDialog::Accepted) 
        {
            m_bLoged = true;
        }
        
        qDebug("close LoginDlg \r\n");
        
    }
    
    bEnter = false;
}


void MainWindow::emitDispIndication(unsigned char *pucData,int iLength)
{
   emit dispIndication(pucData,iLength);
}

void DispIndicationEntry(unsigned char *pucData,int iLength)
{
   // do something here
   unsigned char *pdi = (unsigned char *)mem_malloc(iLength);

   if (pdi)
   {
       memcpy(pdi,pucData,iLength);
       if (gpMainWnd) gpMainWnd->emitDispIndication(pdi,iLength);
   }
}

void DispGetHandlerConfig(int addr)
{
   if (gpMainWnd)
   {
       DB_HANDLER_STRU *  hdl = gpMainWnd->getHandler(addr);
       if (hdl)
       {
           DispSetHanlerConfig(hdl);
       }
   }
}


void MainWindow::emitIapIndication(IAP_NOTIFY_STRU *pIapNotify)
{
    emit iapIndication(pIapNotify);
}

void DispIapIndEntry(IAP_NOTIFY_STRU *pIapNotify)
{
   // do something here
   IAP_NOTIFY_STRU *pdi = (IAP_NOTIFY_STRU *)mem_malloc(IAP_NOTIFY_SIZE + pIapNotify->iMsgLen);

   if (pdi)
   {
       memcpy(pdi,pIapNotify,IAP_NOTIFY_SIZE + pIapNotify->iMsgLen);
       
       gpMainWnd->emitIapIndication(pdi);
   }
   
}

DB_HANDLER_STRU * getHandler(int addr)
{
    return gpMainWnd->getHandler(addr);
}

DB_RFID_STRU * getRfid(int addr)
{
    return gpMainWnd->getRfid(addr);
}

int getAlarmState(void)
{
    return gpMainWnd->getAlarmState();
}

int getTankLevel(void)
{
    return gpMainWnd->getTankLevel();
}

int getEventCount(void)
{
    return gpMainWnd->getEventCount();
}

ECO_INFO_STRU * MainWindow:: getEco(int index)
{
    return  &m_EcoInfo[index];
}

void MainWindow::on_Opt_clicked()
{
    if (m_bLoged)
    {
        m_pOptDlg->show();
    }
    else
    {
        emit autoLogin();
    }
}

void MainWindow::on_Stop_clicked()
{
    DispCmdEntry(DISP_CMD_HALT,NULL,0);

}

void MainWindow::on_pbDevice_clicked()
{
    if (m_bLoged)
    {
        //m_pDeviceDlg->show();
    }
    else
    {
        emit autoLogin();
    }
}

void MainWindow::showDeviceDlg(bool bShow)
{
    if (bShow)
    {
        //m_pDeviceDlg->show();
    }
    else
    {
        //m_pDeviceDlg->hide();
    }
}

void MainWindow::showOptDlg(bool bShow)
{
    if (bShow)
    {
        m_pOptDlg->show();
    }
    else
    {
        m_pOptDlg->hide();
    }
}


SetDevicePage *MainWindow::getDeviceDlg()
{
    if (typeid(*m_pCurPage) == typeid(SetDevicePage))
    {
        SetDevicePage *page = (SetDevicePage *)m_pCurPage;
        return page;
    }

    return NULL;

}

Ex_Init_HandleCfgpage *MainWindow::getExInitPage()
{
    if (typeid(*m_curExInitPage) == typeid(Ex_Init_HandleCfgpage))
    {
        Ex_Init_HandleCfgpage *page = (Ex_Init_HandleCfgpage *)m_curExInitPage;
        return page;
    }

    return NULL;
}

void MainWindow::on_Exit_clicked()
{
    m_bLoged = false;

}
void MainWindow::on_pbRun_clicked()
{
    if (m_bLoged)
    {
        DispCmdEntry(DISP_CMD_RUN,NULL,0);
    }
    else
    {
        emit autoLogin();
    }

}

void MainWindow::run(bool bRun)
{
    /* check machine state */
    int iRet;
    
    MainPage *pMainPage = (MainPage *)m_pSubPages[PAGE_MAIN];
    
    if (bRun)
    {
        if (!getActiveExeBrds())
        {
            if (typeid(*m_pCurPage) == typeid(MainPage))
            {
                pMainPage->updateRunInfo(false);
            }
            return;
        }

        if (!getActiveFmBrds())
        {
            if (typeid(*m_pCurPage) == typeid(MainPage))
            {
                pMainPage->updateRunInfo(false);
            }
            return;
        }
        
        if (!(gGlobalParam.MiscParam.ulMisFlags & (1 << DISP_SM_RFID_Authorization)))
        {
            iRet = getActiveRfidBrds();

            if (iRet <= 0)
            {
               if (typeid(*m_pCurPage) == typeid(MainPage))
               {
                   pMainPage->updateRunInfo(false);
               }

               switch(-iRet)
               {
               case DISP_PRE_PACK:
                    ToastDlg::makeToast(tr("No Pre-PACK detected!"));
                    break;
               case DISP_AC_PACK:
                    ToastDlg::makeToast(tr("No AC-PACK detected!"));
                    break;
               case DISP_P_PACK:
                    ToastDlg::makeToast(tr("No P-PACK detected!"));
                    break;
               case DISP_U_PACK:
                    ToastDlg::makeToast(tr("No U-PACK detected!"));
                    break;
               case DISP_AT_PACK:
                    ToastDlg::makeToast(tr("No AT-PACK detected!"));
                    break;
               case DISP_H_PACK:
                    ToastDlg::makeToast(tr("No H-PACK detected!"));
                    break;
#if 0
               case (DISP_PRE_PACK | 0xFF00):
                    ToastDlg::makeToast(tr("False Pre-PACK detected!"));
                    break;
               case (DISP_AC_PACK | 0xFF00):
                    ToastDlg::makeToast(tr("False AC-PACK detected!"));
                    break;
               case (DISP_P_PACK | 0xFF00):
                    ToastDlg::makeToast(tr("False P-PACK detected!"));
                    break;
               case (DISP_U_PACK | 0xFF00):
                    ToastDlg::makeToast(tr("False U-PACK detected!"));
                    break;
               case (DISP_AT_PACK | 0xFF00):
                    ToastDlg::makeToast(tr("False AT-PACK detected!"));
                    break;
               case (DISP_H_PACK | 0xFF00):
                    ToastDlg::makeToast(tr("False H-PACK detected!"));
                    break;
#endif
               }
               return;
            }
        }

        //Check whether the Pack is installed correctly, whether or not the RFID is turned on.
        {
            iRet = getActiveRfidBrds();

            if (iRet <= 0)
            {
               if (typeid(*m_pCurPage) == typeid(MainPage))
               {
                   pMainPage->updateRunInfo(false);
               }
               bool isError = false;
               QMessageBox msgBox;

               switch(-iRet)
               {
               case (DISP_PRE_PACK | 0xFF00):
                    msgBox.setText(tr("False Pre-PACK detected!"));
                    isError = true;
                    break;
               case (DISP_AC_PACK | 0xFF00):
                    msgBox.setText(tr("False AC-PACK detected!"));
                    isError = true;
                    break;
               case (DISP_P_PACK | 0xFF00):
                    msgBox.setText(tr("False P-PACK detected!"));
                    isError = true;
                    break;
               case (DISP_U_PACK | 0xFF00):
                    msgBox.setText(tr("False U-PACK detected!"));
                    isError = true;
                    break;
               case (DISP_AT_PACK | 0xFF00):
                    msgBox.setText(tr("False AT-PACK detected!"));
                    isError = true;
                    break;
               case (DISP_H_PACK | 0xFF00):
                    msgBox.setText(tr("False H-PACK detected!"));
                    isError = true;
                    break;
               }

               if(isError)
               {
                    msgBox.setInformativeText("Do you want to continue?");
                    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
                    msgBox.setDefaultButton(QMessageBox::No);
                    int ret = msgBox.exec();
                    if(QMessageBox::No == ret)
                    {
                        ex_gGlobalParam.lastRunState = 0;
                        MainSaveLastRunState(gGlobalParam.iMachineType);

                        return;
                    }
               }
            }
        }
        
        if (DISP_WORK_STATE_IDLE == DispGetWorkState()
            || DISP_WORK_STATE_LPP == DispGetWorkState())
        {
            DispCmdEntry(DISP_CMD_RUN,NULL,0);

            ex_gGlobalParam.lastRunState = 1;
            MainSaveLastRunState(gGlobalParam.iMachineType);

            return;
        }

        if (typeid(*m_pCurPage) == typeid(MainPage))
        {
            pMainPage->updateRunInfo(false);
        }
        
        return ;
       
    }
    
    if (DISP_WORK_STATE_IDLE != DispGetWorkState())
    {
        DispCmdEntry(DISP_CMD_HALT,NULL,0);

        ex_gGlobalParam.lastRunState = 0;
        MainSaveLastRunState(gGlobalParam.iMachineType);
    
        return ;
    }

}

void MainWindow::home()
{
    int iIdx;
#ifdef FLOWCHART
    m_curPageIdx = 1;
#else
    m_curPageIdx = 0;
#endif


    for (iIdx = 0; iIdx < m_pageList.count();iIdx++)
    {
        CPage *page = m_pageList[iIdx];

        if (page != m_pSubPages[m_curPageIdx])
        {
            page->show(false);
        }
    }   
    
    m_pSubPages[m_curPageIdx]->show(true);
}

void MainWindow::naviPage(int iCurPage,int iDir)
{
#ifdef FLOWCHART
    if (iCurPage < 4 && !iDir )
    {
        m_pSubPages[iCurPage]->show(false);
        m_pSubPages[iCurPage + 1]->show(true);
        m_curPageIdx += 1;
    }
    else if (iCurPage > 0 && iDir )
    {
        m_pSubPages[iCurPage]->show(false);
        m_pSubPages[iCurPage - 1]->show(true);
        m_curPageIdx -= 1;
    }
#else
    if (iCurPage < 3 && !iDir )
    {
        m_pSubPages[iCurPage]->show(false);
        m_pSubPages[iCurPage + 1]->show(true);
        m_curPageIdx += 1;
    }
    else if (iCurPage > 0 && iDir )
    {
        m_pSubPages[iCurPage]->show(false);
        m_pSubPages[iCurPage - 1]->show(true);
        m_curPageIdx -= 1;
    }
#endif
}

void MainWindow::saveLoginfo(QString strUserName)
{
    
    m_strUserName = strUserName;
    //m_strPassword = strPassword;
    
    qDebug() << "saveLoginfo  " << m_strUserName;
}

QString MainWindow::getLoginfo()
{
    return m_strUserName;
}

void MainWindow::MainWriteLoginOperationInfo2Db(int iActId)
{
   /* write to log */
   QSqlQuery query;

   bool bResult;

   time_t timer = time(NULL);
   struct tm *tblock = localtime(&timer);
   QString strTime  ;

   
   strTime.sprintf("%04d-%02d-%02d %02d:%02d:%02d",tblock->tm_year + 1900,tblock->tm_mon + 1,tblock->tm_mday,tblock->tm_hour,tblock->tm_min,tblock->tm_sec);
   query.prepare(INSERT_sql_Log);
   query.bindValue(":name"  ,m_strUserName);
   query.bindValue(":action",gastrLoginOperateActionName[iActId]);
   query.bindValue(":info"  ,"none");
   query.bindValue(":time"  ,strTime);
   bResult = query.exec();

   qDebug() << "MainWriteLoginOperationInfo2Db  " << m_strUserName <<gastrLoginOperateActionName[iActId] << bResult;
}

void MainWindow::MainWriteCMInstallInfo2Db(int iActId,int iItemIdx,CATNO cn,LOTNO ln)
{
   /* write to log */
   QSqlQuery query;

   QString strInfo = cn ;

   time_t timer = time(NULL);
   struct tm *tblock = localtime(&timer);
   QString strTime   ;
   
   (void)iItemIdx;

   strInfo += "-";
   strInfo += ln;

   strTime.sprintf("%04d-%02d-%02d %02d:%02d:%02d",tblock->tm_year + 1900,tblock->tm_mon + 1,tblock->tm_mday,tblock->tm_hour,tblock->tm_min,tblock->tm_sec);
   
   query.prepare(INSERT_sql_Log);
   query.bindValue(":name"  ,m_strUserName);
   query.bindValue(":action",gastrCMActionName[iActId]);
   query.bindValue(":info"  ,strInfo);
   query.bindValue(":time"  ,strTime);
   query.exec();
}


void MainWindow::MainWriteMacInstallInfo2Db(int iActId,int iItemIdx,CATNO cn,LOTNO ln)
{
   /* write to log */
   QSqlQuery query;

   time_t timer = time(NULL);
   struct tm *tblock = localtime(&timer);
   QString strTime   ;
   QString strInfo   = cn ;

   (void)iItemIdx;

   strTime.sprintf("%04d-%02d-%02d %02d:%02d:%02d",tblock->tm_year + 1900,tblock->tm_mon + 1,tblock->tm_mday,tblock->tm_hour,tblock->tm_min,tblock->tm_sec);

   strInfo += "-";
   strInfo += ln;
   
   query.prepare(INSERT_sql_Log);
   query.bindValue(":name"  ,m_strUserName);
   query.bindValue(":action",gastrMachineryActionName[iActId]);
   query.bindValue(":info"  ,strInfo);
   query.bindValue(":time"  ,strTime);
   query.exec();
}

void MainWindow::setWaterQuantity(int iType,float fValue)
{
    if (iType < 0 || iType >= APP_DEV_HS_SUB_NUM) return ;

    m_afWQuantity[iType] = fValue;
}


void MainWindow::changeWaterQuantity(int iType,bool bAdd,float fValue)
{
    if (iType < 0 || iType >= APP_DEV_HS_SUB_NUM) return ;

    if (bAdd) 
    {
        m_afWQuantity[iType] += fValue;
    }
    else
    {
        m_afWQuantity[iType] = m_afWQuantity[iType] >= fValue ? m_afWQuantity[iType] - fValue : 0;
    }
}

void MainWindow::startCir(int iCirType)
{
    unsigned char buf[32];

    
    DISP_CMD_CIR_STRU *pCmd = (DISP_CMD_CIR_STRU *)buf;

    pCmd->iType = iCirType;
    
    DispCmdEntry(DISP_CMD_CIR,buf,sizeof(DISP_CMD_CIR_STRU));
}

void MainWindow::startQtw(int iType,bool bQtw)
{
    DB_HANDLER_STRU *hdl;

    unsigned char buf[32];
    
    DISP_CMD_TW_STRU *pCmd = (DISP_CMD_TW_STRU *)buf;

    pCmd->iType = iType;

    pCmd->iVolume = bQtw ? (int)(m_afWQuantity[iType]*APP_QTW_UNIT) : INVALID_FM_VALUE; // 0.1L ->ml

    hdl = getDefaultHandler(iType);

    if (hdl)
    {
        DISPHANDLE cmdRslt;
        
        pCmd->iDevId  = hdl->address;
        
        cmdRslt = DispCmdEntry(DISP_CMD_TW,buf,sizeof(DISP_CMD_TW_STRU));

        if (!cmdRslt)
        {
            qDebug() << "DISP_CMD_TW Failed " << iType;
        }
    }
    else
    {
        qDebug() << "No default handset: " << iType ;
    }
        
}

DISPHANDLE MainWindow::startClean(int iType,bool bStart)
{
    unsigned char buf[32];

    DISP_CMD_WASH_STRU *pCmd = (DISP_CMD_WASH_STRU *)buf;

    (void)bStart;

    pCmd->iType  = iType;
    pCmd->iState = bStart ? TRUE : FALSE;

    return DispCmdEntry(DISP_CMD_WASH,buf,sizeof(DISP_CMD_WASH_STRU));
}


void MainWindow::changeRPumpValue(int iIdx,int iValue)
{

    if (iValue < PUMP_SPEED_NUM)
    {
       m_aiRPumpVoltageLevel[iIdx] = iValue;
    }
}

void MainWindow::updateTubeCirCfg()
{
    m_iStartMinute  = ((gGlobalParam.MiscParam.aiTubeCirTimeInfo[0] >> 8) & 0XFF)*60 + ((gGlobalParam.MiscParam.aiTubeCirTimeInfo[0] >> 0) & 0XFF);
    m_iEndMinute    = ((gGlobalParam.MiscParam.aiTubeCirTimeInfo[1] >> 8) & 0XFF)*60 + ((gGlobalParam.MiscParam.aiTubeCirTimeInfo[1] >> 0) & 0XFF);
    m_iTubeCirCycle = gGlobalParam.MiscParam.iTubeCirCycle;

    qDebug() << "updateTubeCirCfg " << m_iStartMinute << m_iEndMinute << m_iTubeCirCycle << gGlobalParam.MiscParam.ulAllocMask;
}

DB_HANDLER_STRU * MainWindow::getHandler(SN sn)
{
    int iLoop;
    
    for(iLoop = 0 ; iLoop < APP_HAND_SET_MAX_NUMBER ; iLoop++)
    {
        if (m_iHandlerMask & (1 << iLoop))
        {
           if (0 == strncmp(sn,m_aHandler[iLoop].name ,APP_SN_LENGTH))
           {
               return &m_aHandler[iLoop];
           }
        }
    
    }

    return NULL;
}

DB_HANDLER_STRU * MainWindow::getHandler(void)
{
    return m_aHandler;
}

int MainWindow::getHandlerMask() 
{
    return m_iHandlerMask;
}

DB_RFID_STRU * MainWindow::getRfid(SN sn)
{
    int iLoop;
    

    for(iLoop = 0 ; iLoop < APP_HAND_SET_MAX_NUMBER ; iLoop++)
    {
        if (m_iRfidMask & (1 << iLoop))
        {
           if (0 == strncmp(sn,m_aRfid[iLoop].name ,APP_SN_LENGTH))
           {
               return &m_aRfid[iLoop];
           }
        }
    
    }

    return NULL;

}

DB_HANDLER_STRU * MainWindow::getHandler(int addr)
{
    int iLoop;
    

    for(iLoop = 0 ; iLoop < APP_HAND_SET_MAX_NUMBER ; iLoop++)
    {
        if (m_iHandlerMask & (1 << iLoop))
        {
           if (addr == m_aHandler[iLoop].address)
           {
               return &m_aHandler[iLoop];
           }
        }
    
    }

    return NULL;

}

/********************************************************
  iType: refer APP_CIR_TYPE_ENUM
********************************************************/
DB_HANDLER_STRU * MainWindow::getDefaultHandler(int iType)
{
    int iLoop;
    

    for(iLoop = 0 ; iLoop < APP_HAND_SET_MAX_NUMBER ; iLoop++)
    {
        if (m_iHandlerMask & (1 << iLoop))
        {
           if (iType == m_aHandler[iLoop].type
               && m_aHandler[iLoop].def)
           {
               return &m_aHandler[iLoop];
           }
        }
    
    }

    return NULL;

}


DB_RFID_STRU * MainWindow::getRfid(int addr)
{
    int iLoop;
    

    for(iLoop = 0 ; iLoop < APP_HAND_SET_MAX_NUMBER ; iLoop++)
    {
        if (m_iRfidMask & (1 << iLoop))
        {
            if (addr == m_aRfid[iLoop].address)
           {
               return &m_aRfid[iLoop];
           }
        }
    
    }

    return NULL;

}

void MainWindow::buildTranslation()
{
    m_astrDbName[0] = tr("Water");
    m_astrDbName[1] = tr("Alarm");
    m_astrDbName[2] = tr("User");
    m_astrDbName[3] = tr("GetW");
    m_astrDbName[4] = tr("PWater");
    m_astrDbName[5] = tr("Log");

}

void MainWindow::on_Ex_Init_Lan(int index)
{
    switch(index)
    {
    case 0:
        break;
    case 1:
    {
        m_pExInitPages[Ex_Init_Lan]->show(false);
        m_pExInitPages[Ex_Init_Time]->show(true);
        m_curExInitPage = m_pExInitPages[Ex_Init_Time];
        break;
    }
    default:
        break;
    }
}
void MainWindow::on_Ex_Init_Network(int index)
{
    switch(index)
    {
    case 0:
    {
        if(gGlobalParam.iMachineType == MACHINE_ADAPT)
        {
            m_pExInitPages[Ex_Init_Network]->show(false);
            m_pExInitPages[Ex_Init_Time]->show(true);
            m_curExInitPage = m_pExInitPages[Ex_Init_Time];
        }
        else
        {
            if(gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_HaveB2))
            {
                m_pExInitPages[Ex_Init_Network]->show(false);
                m_pExInitPages[Ex_Init_Syscfg]->show(true);
                m_curExInitPage = m_pExInitPages[Ex_Init_Syscfg];
            }
            else
            {
                m_pExInitPages[Ex_Init_Network]->show(false);
                m_pExInitPages[Ex_Init_Tankcfg]->show(true);
                m_curExInitPage = m_pExInitPages[Ex_Init_Tankcfg];
            }

        }
        break;
    }
    case 1:
    {
        m_pExInitPages[Ex_Init_Network]->show(false);
        m_pExInitPages[Ex_Init_Handlercfg]->show(true);
        m_curExInitPage = m_pExInitPages[Ex_Init_Handlercfg];
        break;
    }
    default:
        break;
    }
}

void MainWindow::on_Ex_Init_Time(int index)
{
    switch(index)
    {
    case 0:
    {
        m_pExInitPages[Ex_Init_Time]->show(false);
        m_pExInitPages[Ex_Init_Lan]->show(true);
        m_curExInitPage = m_pExInitPages[Ex_Init_Lan];
        break;
    }
    case 1:
    {
        if(gGlobalParam.iMachineType == MACHINE_ADAPT)
        {
            m_pExInitPages[Ex_Init_Time]->show(false);
            m_pExInitPages[Ex_Init_Network]->show(true);
            m_curExInitPage = m_pExInitPages[Ex_Init_Network];
        }
        else
        {
            m_pExInitPages[Ex_Init_Time]->show(false);
            m_pExInitPages[Ex_Init_Tankcfg]->show(true);
            m_curExInitPage = m_pExInitPages[Ex_Init_Tankcfg];
        }
        break;
    }
    default:
        break;
    }
}

void MainWindow::on_Ex_Init_TankCfg(int index)
{
    switch(index)
    {
    case 0:
    {
        m_pExInitPages[Ex_Init_Tankcfg]->show(false);
        m_pExInitPages[Ex_Init_Time]->show(true);
        m_curExInitPage = m_pExInitPages[Ex_Init_Time];
        break;
    }
    case 1:
    {
        if(gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_HaveB2))
        {
            m_pExInitPages[Ex_Init_Tankcfg]->show(false);
            m_pExInitPages[Ex_Init_Syscfg]->show(true);
            m_curExInitPage = m_pExInitPages[Ex_Init_Syscfg];
        }
        else
        {
            m_pExInitPages[Ex_Init_Tankcfg]->show(false);
            m_pExInitPages[Ex_Init_Network]->show(true);
            m_curExInitPage = m_pExInitPages[Ex_Init_Network];
        }

        break;
    }
    default:
        break;
    }
}

void MainWindow::on_Ex_Init_Syscfg(int index)
{
    switch(index)
    {
    case 0:
    {
        m_pExInitPages[Ex_Init_Syscfg]->show(false);
        m_pExInitPages[Ex_Init_Tankcfg]->show(true);
        m_curExInitPage = m_pExInitPages[Ex_Init_Tankcfg];
        break;
    }
    case 1:
    {
        m_pExInitPages[Ex_Init_Syscfg]->show(false);
        m_pExInitPages[Ex_Init_Network]->show(true);
        m_curExInitPage = m_pExInitPages[Ex_Init_Network];
        break;
    }
    default:
        break;
    }
}

void MainWindow::on_Ex_Init_Finished()
{
    m_curExInitPage = NULL;

#ifdef VWR
        mainDisplay();
#else
        Splash();
#endif
}

void MainWindow::on_Ex_Init_Handler(int index)
{
    switch(index)
    {
    case 0:
    {
        m_pExInitPages[Ex_Init_Handlercfg]->show(false);
        m_pExInitPages[Ex_Init_Network]->show(true);
        m_curExInitPage = m_pExInitPages[Ex_Init_Network];
        break;
    }
    case 1:
        break;
    default:
        break;
    }
}

void MainWindow::on_Ex_ScreenPageHide()
{
    if(typeid(*m_pCurPage) == typeid(Ex_ScreenSleepPage))
    {
        m_pCurPage->show(false);
        emit SleepPageShow(false);
        m_pPreviousPage->show(true);
    }

}

int MainWindow::getActiveHandlerBrds() 
{
    if ((m_iHandlerMask & m_iHandlerActiveMask) == m_iHandlerMask)
    {
        return 1;
    }

    if ((m_iHandlerMask & m_iHandlerActiveMask))
    {
       return 2;
    }
    return 0;
}

int MainWindow::getActiveRfidBrds() 
{
    int iLoop;
    
    //if ((m_iRfidActiveMask & MacRfidMap.ulMask4Normlwork) != MacRfidMap.ulMask4Normlwork)
    //{
    //    return 0;
    //}
#if 1
    /* check Packs */
    for (iLoop = 0; iLoop < APP_RFID_SUB_TYPE_NUM; iLoop++)
    {
        if ((m_iRfidActiveMask & MacRfidMap.ulMask4Normlwork) & (1 << iLoop))
        {
           if (MacRfidMap.aiDeviceType4Normal[iLoop] != m_aRfidInfo[iLoop].getPackType())
           {
               return -(MacRfidMap.aiDeviceType4Normal[iLoop] | 0XFF00);
           }
        }
        else if (MacRfidMap.ulMask4Normlwork &  (1 << iLoop))
        {
            return -(MacRfidMap.aiDeviceType4Normal[iLoop]);
        }
    }
#endif
    return 1;
}

int MainWindow:: getActiveRfidBrds4Cleaning()
{
    int iLoop;
    
    //if ((m_iRfidActiveMask & MacRfidMap.ulMask4Cleaning) != MacRfidMap.ulMask4Cleaning)
    //{
    //    return 0;
    //}

    /* check Packs */
    for (iLoop = 0; iLoop < APP_RFID_SUB_TYPE_NUM; iLoop++)
    {
        if ((m_iRfidActiveMask & MacRfidMap.ulMask4Cleaning) & (1 << iLoop))
        {
           if (MacRfidMap.aiDeviceType4Cleaning[iLoop] != m_aRfidInfo[iLoop].getPackType())
           {
               return -MacRfidMap.aiDeviceType4Cleaning[iLoop];
           }
        }
        else if (MacRfidMap.ulMask4Cleaning &  (1 << iLoop))
        {
            return -MacRfidMap.aiDeviceType4Cleaning[iLoop];
        }
    }

    return 1;
}
int MainWindow::getMachineAlarmMask(int iMachineType,int iPart) 
{
    return m_aMas[iMachineType].aulMask[iPart];
}

int MainWindow::getMachineNotifyMask(int iMachineType,int iPart) 
{
    return m_cMas[iMachineType].aulMask[iPart];
}

//dcj add 2019.6.17
void MainWindow::checkCMParam()
{
    if ((0 == gGlobalParam.CMParam.aulCms[DISP_PRE_PACKLIFEDAY])
        ||(0 == gGlobalParam.CMParam.aulCms[DISP_PRE_PACKLIFEL]))
    {
        m_cMas[gGlobalParam.iMachineType].aulMask[0] &= ~(1 <<  DISP_PRE_PACK);
    }

    if ((0 == gGlobalParam.CMParam.aulCms[DISP_AC_PACKLIFEDAY])
        ||(0 == gGlobalParam.CMParam.aulCms[DISP_AC_PACKLIFEL]))
    {
        m_cMas[gGlobalParam.iMachineType].aulMask[0] &= ~(1 <<  DISP_AC_PACK);
    }

    if ((0 == gGlobalParam.CMParam.aulCms[DISP_T_PACKLIFEDAY])
        ||(0 == gGlobalParam.CMParam.aulCms[DISP_T_PACKLIFEL]))
    {
        m_cMas[gGlobalParam.iMachineType].aulMask[0] &= ~(1 <<  DISP_T_PACK);
    }

    if ((0 == gGlobalParam.CMParam.aulCms[DISP_P_PACKLIFEDAY])
        ||(0 == gGlobalParam.CMParam.aulCms[DISP_P_PACKLIFEL]))
    {
        m_cMas[gGlobalParam.iMachineType].aulMask[0] &= ~(1 <<  DISP_P_PACK);
    }

    if ((0 == gGlobalParam.CMParam.aulCms[DISP_U_PACKLIFEDAY])
        ||(0 == gGlobalParam.CMParam.aulCms[DISP_U_PACKLIFEL]))
    {
        m_cMas[gGlobalParam.iMachineType].aulMask[0] &= ~(1 <<  DISP_U_PACK);
    }

    if ((0 == gGlobalParam.CMParam.aulCms[DISP_AT_PACKLIFEDAY])
        ||(0 == gGlobalParam.CMParam.aulCms[DISP_AT_PACKLIFEL]))
    {
        m_cMas[gGlobalParam.iMachineType].aulMask[0] &= ~(1 <<  DISP_AT_PACK);
    }

    if ((0 == gGlobalParam.CMParam.aulCms[DISP_H_PACKLIFEDAY])
        ||(0 == gGlobalParam.CMParam.aulCms[DISP_H_PACKLIFEL]))
    {
        m_cMas[gGlobalParam.iMachineType].aulMask[0] &= ~(1 <<  DISP_H_PACK);
    }

    if ((0 == gGlobalParam.CMParam.aulCms[DISP_N1_UVLIFEDAY])
        ||(0 == gGlobalParam.CMParam.aulCms[DISP_N1_UVLIFEHOUR]))
    {
        m_cMas[gGlobalParam.iMachineType].aulMask[0] &= ~(1 <<  DISP_N1_UV);
    }

    if ((0 == gGlobalParam.CMParam.aulCms[DISP_N2_UVLIFEDAY])
        ||(0 == gGlobalParam.CMParam.aulCms[DISP_N2_UVLIFEHOUR]))
    {
        m_cMas[gGlobalParam.iMachineType].aulMask[0] &= ~(1 <<  DISP_N2_UV);
    }

    if ((0 == gGlobalParam.CMParam.aulCms[DISP_N3_UVLIFEDAY])
        ||(0 == gGlobalParam.CMParam.aulCms[DISP_N3_UVLIFEHOUR]))
    {
        m_cMas[gGlobalParam.iMachineType].aulMask[0] &= ~(1 <<  DISP_N3_UV);
    }

    if ((0 == gGlobalParam.CMParam.aulCms[DISP_N4_UVLIFEDAY])
        ||(0 == gGlobalParam.CMParam.aulCms[DISP_N4_UVLIFEHOUR]))
    {
        m_cMas[gGlobalParam.iMachineType].aulMask[0] &= ~(1 <<  DISP_N4_UV);
    }

    if ((0 == gGlobalParam.CMParam.aulCms[DISP_N5_UVLIFEDAY])
        ||(0 == gGlobalParam.CMParam.aulCms[DISP_N5_UVLIFEHOUR]))
    {
        m_cMas[gGlobalParam.iMachineType].aulMask[0] &= ~(1 <<  DISP_N5_UV);
    }

    if (0 == gGlobalParam.CMParam.aulCms[DISP_W_FILTERLIFE])
    {
        m_cMas[gGlobalParam.iMachineType].aulMask[0] &= ~(1 <<  DISP_W_FILTER);
    }

    if (0 == gGlobalParam.CMParam.aulCms[DISP_T_B_FILTERLIFE])
    {
        m_cMas[gGlobalParam.iMachineType].aulMask[0] &= ~(1 << DISP_T_B_FILTER);
    }

    if (0 == gGlobalParam.CMParam.aulCms[DISP_T_A_FILTERLIFE])
    {
        m_cMas[gGlobalParam.iMachineType].aulMask[0] &= ~(1 << DISP_T_A_FILTER);
    }

    if (0 == gGlobalParam.CMParam.aulCms[DISP_TUBE_FILTERLIFE])
    {
        m_cMas[gGlobalParam.iMachineType].aulMask[0] &= ~(1 << DISP_TUBE_FILTER);
    }

    if (0 == gGlobalParam.CMParam.aulCms[DISP_TUBE_DI_LIFE])
    {
        m_cMas[gGlobalParam.iMachineType].aulMask[0] &= ~(1 << DISP_TUBE_DI);
    }

    if (0 == gGlobalParam.CMParam.aulCms[DISP_ROC12LIFEDAY])
    {
        m_cMas[gGlobalParam.iMachineType].aulMask[0] &= ~(1 << DISP_ROC12);
    }
}

void MainWindow::ClearToc()
{
    if (!(gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_HaveTOC)))
    {
        alarmCommProc(false,DISP_ALARM_PART1,DISP_ALARM_PART1_HIGHER_TOC_SENSOR_TEMPERATURE);
        alarmCommProc(false,DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_TOC_SENSOR_TEMPERATURE);
        alarmCommProc(false,DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_TOC_SOURCE_WATER_RESISTENCE);
    }
}

void MainWindow::updateRectState()
{
    bool temp;
    unsigned int nRectSwitchMask;
    nRectSwitchMask = DispGetSwitchState(APP_EXE_SWITCHS_MASK);
    //N1
    temp = nRectSwitchMask & (1 << APP_EXE_N1_NO) ? true : false;
    if(temp && (ex_gCcb.Ex_Rect_State.EX_N_NO[EX_RECT_N1] == 0))
    {
        ex_gCcb.Ex_Rect_State.EX_N_NO[EX_RECT_N1] = 1;
        ex_gCcb.Ex_Alarm_Tick.ulAlarmNRectTick[EX_RECT_N1] = ex_gulSecond;
    }
    if(!temp)
    {
        ex_gCcb.Ex_Rect_State.EX_N_NO[EX_RECT_N1] = 0;
    }
    //N2
    temp = nRectSwitchMask & (1 << APP_EXE_N2_NO) ? true : false;
    if(temp && (ex_gCcb.Ex_Rect_State.EX_N_NO[EX_RECT_N2] == 0))
    {
        ex_gCcb.Ex_Rect_State.EX_N_NO[EX_RECT_N2] = 1;
        ex_gCcb.Ex_Alarm_Tick.ulAlarmNRectTick[EX_RECT_N2] = ex_gulSecond;
    }
    if(!temp)
    {
        ex_gCcb.Ex_Rect_State.EX_N_NO[EX_RECT_N2] = 0;
    }
    //N3
    temp = nRectSwitchMask & (1 << APP_EXE_N3_NO) ? true : false;
    if(temp && (ex_gCcb.Ex_Rect_State.EX_N_NO[EX_RECT_N3] == 0))
    {
        ex_gCcb.Ex_Rect_State.EX_N_NO[EX_RECT_N3] = 1;
        ex_gCcb.Ex_Alarm_Tick.ulAlarmNRectTick[EX_RECT_N3] = ex_gulSecond;
    }
    if(!temp)
    {
        ex_gCcb.Ex_Rect_State.EX_N_NO[EX_RECT_N3] = 0;
    }

}

void MainWindow::updateRectAlarmState()
{
    int iTmpData;
    //N1
    if(ex_gCcb.Ex_Rect_State.EX_N_NO[EX_RECT_N1] == 1)
    {
        DispGetOtherCurrent(APP_EXE_N1_NO, &iTmpData);
        if((ex_gulSecond - ex_gCcb.Ex_Alarm_Tick.ulAlarmNRectTick[EX_RECT_N1]) > 15)
        {
            if(iTmpData < 50)
            {
                //Alaram
                if(ex_gCcb.Ex_Alarm_Bit.bit1AlarmN1 == 0)
                {
                    //2018.10.23
                    if(ex_gCcb.Ex_Alarm_Bit.bit1AlarmDelayN1 == 0)
                    {
                        ex_gCcb.Ex_Alarm_Tick.ulAlarmNRectDelay[EX_RECT_N1] = ex_gulSecond;
                        ex_gCcb.Ex_Alarm_Bit.bit1AlarmDelayN1 = 1;
                    }
                    else
                    {
                        if((ex_gulSecond - ex_gCcb.Ex_Alarm_Tick.ulAlarmNRectDelay[EX_RECT_N1]) > 5)
                        {
                            ex_gCcb.Ex_Alarm_Bit.bit1AlarmN1 = 1;
                            alarmCommProc(true,DISP_ALARM_PART0,DISP_ALARM_PART0_254UV_OOP);
                        }
                    }
                    //
                }
            }
            else
            {
                if(ex_gCcb.Ex_Alarm_Bit.bit1AlarmN1 == 1)
                {
                    ex_gCcb.Ex_Alarm_Bit.bit1AlarmDelayN1 = 0;
                    ex_gCcb.Ex_Alarm_Bit.bit1AlarmN1 = 0;
                    alarmCommProc(false,DISP_ALARM_PART0,DISP_ALARM_PART0_254UV_OOP);
                }
            }
        }
    }
    //N2
    if(ex_gCcb.Ex_Rect_State.EX_N_NO[EX_RECT_N2] == 1)
    {
        DispGetOtherCurrent(APP_EXE_N2_NO, &iTmpData);
        if((ex_gulSecond - ex_gCcb.Ex_Alarm_Tick.ulAlarmNRectTick[EX_RECT_N2]) > 15)
        {
            if(iTmpData < 50)
            {
                //Alaram
                if(ex_gCcb.Ex_Alarm_Bit.bit1AlarmN2 == 0)
                {
                    //2018.10.23
                    if(ex_gCcb.Ex_Alarm_Bit.bit1AlarmDelayN2 == 0)
                    {
                        ex_gCcb.Ex_Alarm_Tick.ulAlarmNRectDelay[EX_RECT_N2] = ex_gulSecond;
                        ex_gCcb.Ex_Alarm_Bit.bit1AlarmDelayN2 = 1;
                    }
                    else
                    {
                        if((ex_gulSecond - ex_gCcb.Ex_Alarm_Tick.ulAlarmNRectDelay[EX_RECT_N2]) > 5)
                        {
                            ex_gCcb.Ex_Alarm_Bit.bit1AlarmN2 = 1;
                            alarmCommProc(true, DISP_ALARM_PART0, DISP_ALARM_PART0_185UV_OOP);
                        }
                    }
                    //
                }
            }
            else
            {
                if(ex_gCcb.Ex_Alarm_Bit.bit1AlarmN2 == 1)
                {
                    ex_gCcb.Ex_Alarm_Bit.bit1AlarmDelayN2 = 0;
                    ex_gCcb.Ex_Alarm_Bit.bit1AlarmN2 = 0;
                    alarmCommProc(false, DISP_ALARM_PART0, DISP_ALARM_PART0_185UV_OOP);
                }
            }
        }
    }

    //N3
    if(ex_gCcb.Ex_Rect_State.EX_N_NO[EX_RECT_N3] == 1)
    {
        if(!gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_TankUV))
        {
            if(ex_gCcb.Ex_Alarm_Bit.bit1AlarmN3 == 1)
            {
                ex_gCcb.Ex_Alarm_Bit.bit1AlarmDelayN3 = 0;
                ex_gCcb.Ex_Alarm_Bit.bit1AlarmN3 = 0;
                alarmCommProc(false,DISP_ALARM_PART0,DISP_ALARM_PART0_TANKUV_OOP);
            }
            else
            {
                return;
            }
        }

        DispGetOtherCurrent(APP_EXE_N3_NO, &iTmpData);
        if((ex_gulSecond - ex_gCcb.Ex_Alarm_Tick.ulAlarmNRectTick[EX_RECT_N3]) > 15)
        {
            if(iTmpData < 50)
            {
                //Alaram
                if(ex_gCcb.Ex_Alarm_Bit.bit1AlarmN3 == 0)
                {
                    //2018.10.23
                    if(ex_gCcb.Ex_Alarm_Bit.bit1AlarmDelayN3 == 0)
                    {
                        ex_gCcb.Ex_Alarm_Tick.ulAlarmNRectDelay[EX_RECT_N3] = ex_gulSecond;
                        ex_gCcb.Ex_Alarm_Bit.bit1AlarmDelayN3 = 1;
                    }
                    else
                    {
                        if((ex_gulSecond - ex_gCcb.Ex_Alarm_Tick.ulAlarmNRectDelay[EX_RECT_N3]) > 5)
                        {
                            ex_gCcb.Ex_Alarm_Bit.bit1AlarmN3 = 1;
                            alarmCommProc(true,DISP_ALARM_PART0,DISP_ALARM_PART0_TANKUV_OOP);
                        }
                    }
                    //
                }

            }
            else
            {
                if(ex_gCcb.Ex_Alarm_Bit.bit1AlarmN3 == 1)
                {
                    ex_gCcb.Ex_Alarm_Bit.bit1AlarmDelayN3 = 0;
                    ex_gCcb.Ex_Alarm_Bit.bit1AlarmN3 = 0;
                    alarmCommProc(false,DISP_ALARM_PART0,DISP_ALARM_PART0_TANKUV_OOP);
                }
            }
        }
    }
}

void MainWindow::initMachineFlow()
{
    int rate = 0;
    unsigned int machineFlow = ex_gGlobalParam.Ex_Machine_Msg.iMachineFlow;

    rate = 120;
    m_flushMachineFlow = ((rate * 1000.0)/3600.0) + 0.5;

    rate = 39;
    switch(machineFlow)
    {
    case 5:
        rate += 8;
        break;
    case 10:
        rate += 16;
        break;
    case 12:
        rate += 12;
        break;
    case 15:
        rate += 24;
        break;
    case 24:
        rate += 24;
        break;
    case 32:
        rate += 32;
        break;
    default:
        break;

    }
    m_productMachineFlow = ((rate * 1000.0)/3600.0) + 0.5;

}

void MainWindow::calcFlow(int state)
{
    switch(state)
    {
    case 0:
        gCMUsage.cmInfo.aulCumulatedData[DISP_P_PACKLIFEL] += m_flushMachineFlow;
        gCMUsage.cmInfo.aulCumulatedData[DISP_AC_PACKLIFEL] += m_flushMachineFlow;
        gCMUsage.cmInfo.aulCumulatedData[DISP_PRE_PACKLIFEL] += m_flushMachineFlow;
        break;
    case 1:
        gCMUsage.cmInfo.aulCumulatedData[DISP_P_PACKLIFEL] += m_productMachineFlow;
        gCMUsage.cmInfo.aulCumulatedData[DISP_AC_PACKLIFEL] += m_productMachineFlow;
        gCMUsage.cmInfo.aulCumulatedData[DISP_PRE_PACKLIFEL] += m_productMachineFlow;
        break;
    default:
        break;
    }

}


void MainWindow::updatePackFlow()
{
    unsigned int nSwitchMask = DispGetSwitchState(APP_EXE_SWITCHS_MASK);

    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_Genie:
    case MACHINE_UP:
    case MACHINE_EDI:
    case MACHINE_RO:
    case MACHINE_ADAPT:
    {
        if((nSwitchMask & ( 1 << APP_EXE_E1_NO))
            && (nSwitchMask & ( 1 << APP_EXE_E2_NO)))
        {
            calcFlow(0);
        }
        else if((nSwitchMask & ( 1 << APP_EXE_E1_NO))
                && (nSwitchMask & ( 1 << APP_EXE_C1_NO)))
        {
            calcFlow(1);
        }

        break;
    }
    default:
        break;
    }
}

void MainWindow::checkConsumableInstall(int iRfId)
{
    if(!m_startCheckConsumale)
    {
        return;
    }
    if(m_checkConsumaleInstall[iRfId]->ischeckBusy())
    {
        return;
    }

    if(!m_checkConsumaleInstall[iRfId]->check(iRfId))
    {
        return;
    }

    if(!m_checkConsumaleInstall[iRfId]->isCorrectRfId())
    {
        return;
    }

    if(m_checkConsumaleInstall[iRfId]->comparedWithSql())
    {
        m_consumaleInstallDialog[iRfId]->show();
    }
}

QStringList MainWindow::consumableCatNo(CONSUMABLE_CATNO iType)
{
    return m_strConsuamble[iType];
}

void MainWindow::emitUnitsChanged()
{
    emit unitsChanged();
}

void MainWindow::retriveCMInfoWithRFID()
{
    updateCMInfoWithRFID(0);
    MainSaveCMInfo(gGlobalParam.iMachineType,gCMUsage.info);
}

void MainWindow::retriveLastRunState()
{
    if(ex_gGlobalParam.lastRunState)
    {
        this->run(true);
    }
}

#ifdef RFIDTEST
void MainWindow::updateCMInfoWithRFID(int operate)
{
    int iRfId;
    int packType;

    if (gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_Pre_Filter)) //DISP_SM_PreFilterColumn
    {
        packType = DISP_PRE_PACK;
        iRfId = APP_RFID_SUB_TYPE_PREPACK;
        if(0 == operate)
        {
            readCMInfoFromRFID(iRfId, packType);
        }
        else
        {
            writeCMInfoToRFID(iRfId, packType);
        }
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
    {
        packType   = DISP_AC_PACK;
        iRfId = APP_RFID_SUB_TYPE_ROPACK_OTHERS;
        if(0 == operate)
        {
            readCMInfoFromRFID(iRfId, packType);
        }
        else
        {
            writeCMInfoToRFID(iRfId, packType);
        }
        break;
    }
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
    case MACHINE_ADAPT:
    {
        packType = DISP_P_PACK;
        iRfId = APP_RFID_SUB_TYPE_PPACK_CLEANPACK;
        if(0 == operate)
        {
            readCMInfoFromRFID(iRfId, packType);
        }
        else
        {
            writeCMInfoToRFID(iRfId, packType);
        }
        break;
    }
    case MACHINE_PURIST:
        break;
    }

    switch(gGlobalParam.iMachineType)
    {
     case MACHINE_L_Genie:
     case MACHINE_L_EDI_LOOP:
     {
         packType = DISP_AT_PACK;
         iRfId = APP_RFID_SUB_TYPE_HPACK_ATPACK;
         if(0 == operate)
         {
             readCMInfoFromRFID(iRfId, packType);
         }
         else
         {
             writeCMInfoToRFID(iRfId, packType);
         }
         break;
     }
    }

     switch(gGlobalParam.iMachineType)
     {
     case MACHINE_L_UP:
     case MACHINE_L_RO_LOOP:
     case MACHINE_UP:
     case MACHINE_PURIST:
     case MACHINE_ADAPT:
     {
         packType = DISP_H_PACK;
         iRfId = APP_RFID_SUB_TYPE_HPACK_ATPACK;
         if(0 == operate)
         {
             readCMInfoFromRFID(iRfId, packType);
         }
         else
         {
             writeCMInfoToRFID(iRfId, packType);
         }
         break;
     }
     case MACHINE_L_EDI_LOOP:
     {
        packType = DISP_H_PACK;
        iRfId = APP_RFID_SUB_TYPE_UPACK_HPACK;
        if(0 == operate)
        {
            readCMInfoFromRFID(iRfId, packType);
        }
        else
        {
            writeCMInfoToRFID(iRfId, packType);
        }
        break;
     }
     }

     switch(gGlobalParam.iMachineType)
     {
     case MACHINE_L_Genie:
     case MACHINE_L_UP:
     case MACHINE_Genie:
     case MACHINE_UP:
     case MACHINE_PURIST:
     case MACHINE_ADAPT:
     {
         packType = DISP_U_PACK;
         iRfId = APP_RFID_SUB_TYPE_UPACK_HPACK;
         if(0 == operate)
         {
             readCMInfoFromRFID(iRfId, packType);
         }
         else
         {
             writeCMInfoToRFID(iRfId, packType);
         }
         break;
     }
     }
}

void MainWindow::readCMInfoFromRFID(int iRfId, int type)
{
    int iRet;

    CATNO cn;
    LOTNO ln;
    QDate installDate;
    int volUsed;

    memset(cn, 0, sizeof(CATNO));
    memset(ln, 0, sizeof(LOTNO));
#if 0
    if (this->getRfidState(iRfId))
    {
        if(m_aRfidInfo[iRfId].getPackType() == type)
        {
            this->getRfidInstallDate(iRfId, &installDate);
            this->getRfidVolofUse(iRfId, volUsed);
        }
        else
        {
            return;
        }

    }

    else
#endif
    {
        iRet = this->readRfid(iRfId);
        if (iRet)
        {
            return;
        }

        if(m_aRfidInfo[iRfId].getPackType() == type)
        {
            this->getRfidInstallDate(iRfId, &installDate);
            this->getRfidVolofUse(iRfId, volUsed);
        }
        else
        {
            return;
        }
    }

    QDateTime installTime(installDate);
    switch(type)
    {
    case DISP_PRE_PACK:
        gCMUsage.info.aulCms[DISP_PRE_PACKLIFEDAY] = installTime.toTime_t();
        gCMUsage.info.aulCms[DISP_PRE_PACKLIFEL]   = volUsed;
        break;
    case DISP_AC_PACK:
        gCMUsage.info.aulCms[DISP_AC_PACKLIFEDAY] = installTime.toTime_t();
        gCMUsage.info.aulCms[DISP_AC_PACKLIFEL]   = volUsed;
        break;
    case DISP_T_PACK:
        gCMUsage.info.aulCms[DISP_T_PACKLIFEDAY] = installTime.toTime_t();
        gCMUsage.info.aulCms[DISP_T_PACKLIFEL]   = volUsed;
        break;
    case DISP_P_PACK:
        gCMUsage.info.aulCms[DISP_P_PACKLIFEDAY] = installTime.toTime_t();
        gCMUsage.info.aulCms[DISP_P_PACKLIFEL]   = volUsed;
        break;
    case DISP_U_PACK:
        gCMUsage.info.aulCms[DISP_U_PACKLIFEDAY] = installTime.toTime_t();
        gCMUsage.info.aulCms[DISP_U_PACKLIFEL]   = volUsed;
        break;
    case DISP_AT_PACK:
        gCMUsage.info.aulCms[DISP_AT_PACKLIFEDAY] = installTime.toTime_t();
        gCMUsage.info.aulCms[DISP_AT_PACKLIFEL]   = volUsed;
        break;
    case DISP_H_PACK:
        gCMUsage.info.aulCms[DISP_H_PACKLIFEDAY] = installTime.toTime_t();
        gCMUsage.info.aulCms[DISP_H_PACKLIFEL]   = volUsed;
        break;
    }
    if(!m_isInitCMInfo)
    {
        m_isInitCMInfo = true;
    }
}

void MainWindow::writeCMInfoToRFID(int iRfId, int type)
{
    qDebug() << QString("writeCMInfoToRFIF(%1, %2)").arg(iRfId).arg(type);
    if(!m_isInitCMInfo)
    {
        return;
    }

    int iRet;
    QString volData;
    switch(type)
    {
    case DISP_PRE_PACK:
        volData = QString("%1").arg(gCMUsage.info.aulCms[DISP_PRE_PACKLIFEL]);
        break;
    case DISP_AC_PACK:
        volData = QString("%1").arg(gCMUsage.info.aulCms[DISP_AC_PACKLIFEL]);
        break;
    case DISP_P_PACK:
        volData = QString("%1").arg(gCMUsage.info.aulCms[DISP_P_PACKLIFEL]);
        break;
    case DISP_U_PACK:
        volData = QString("%1").arg(gCMUsage.info.aulCms[DISP_U_PACKLIFEL]);
        break;
    case DISP_AT_PACK:
        volData = QString("%1").arg(gCMUsage.info.aulCms[DISP_AT_PACKLIFEL]);
        break;
    case DISP_H_PACK:
        volData = QString("%1").arg(gCMUsage.info.aulCms[DISP_H_PACKLIFEL]);
        break;
    }

    iRet = this->writeRfid(iRfId, RF_DATA_LAYOUT_UNKNOW_DATA, volData);
    if(iRet != 0)
    {
        qDebug() << "write pack info error";
    }
}

void MainWindow::updateExConsumableMsg(int iMachineType, CATNO cn, LOTNO ln, int iIndex,
                                       int category, QDate &date, int iRfid)
{
    Q_UNUSED(iMachineType);
    QSqlQuery query;
    bool ret;

    query.prepare(select_sql_Consumable);
    query.addBindValue(iIndex);
    query.addBindValue(category);
    ret = query.exec();
    if(query.next())
    {
            QString lotno = query.value(0).toString();
            if(ln == lotno)
            {
                resetExConsumableMsg(date, iRfid, iIndex);
                return; // do nothing
            }
            else
            {
                QString installDate = resetExConsumableMsg(date, iRfid, iIndex).toString("yyyy-MM-dd");
                QSqlQuery queryUpdate;
                queryUpdate.prepare(update_sql_Consumable);
                queryUpdate.addBindValue(cn);
                queryUpdate.addBindValue(ln);
                queryUpdate.addBindValue(installDate);
                queryUpdate.addBindValue(iIndex);
                queryUpdate.addBindValue(category);
                bool ret = queryUpdate.exec();
                qDebug() << QString("consumable update Sql: %1").arg(ret);
            }
    }
    else
    {
        QString installDate = resetExConsumableMsg(date, iRfid, iIndex).toString("yyyy-MM-dd");
        QSqlQuery queryInsert;
        queryInsert.prepare(insert_sql_Consumable);
        queryInsert.bindValue(":iPackType", iIndex);
        queryInsert.bindValue(":CatNo", cn);
        queryInsert.bindValue(":LotNo", ln);
        queryInsert.bindValue(":category", category);
        queryInsert.bindValue(":time", installDate);
        bool ret = queryInsert.exec();
        qDebug() << QString("consumable insert Sql: %1").arg(ret);
    }
}

const QDate MainWindow::resetExConsumableMsg(QDate &date, int iRfid, int iType)
{
    switch(iType)
    {
    case DISP_AC_PACK:
    case DISP_U_PACK:
    case DISP_P_PACK:
    case DISP_H_PACK:
    {
        if(date.toString("yyyy-MM-dd") == m_consuambleInitDate)
        {
            QDate curDate = QDate::currentDate();
            QString strDate = curDate.toString("yyyy-MM-dd");
            int iRet = gpMainWnd->writeRfid(iRfid, RF_DATA_LAYOUT_INSTALL_DATE, strDate);
            if(iRet != 0)
            {
                QMessageBox::warning(NULL, tr("Warning"), tr("write install date error"), QMessageBox::Ok);
            }

            iRet = gpMainWnd->writeRfid(iRfid, RF_DATA_LAYOUT_UNKNOW_DATA, "0");
            if(iRet != 0)
            {
                QMessageBox::warning(NULL, tr("Warning"), tr("write vol data error"), QMessageBox::Ok);
            }
            return curDate;
        }
        return date;
    }
    default:
        return date;

    }

}

const QString &MainWindow::consumableInitDate() const
{
    return m_consuambleInitDate;
}

#endif


