#ifndef MYPARAMS_H
#define MYPARAMS_H

#include "Display.h"
#include <QMap>

#ifdef __cplusplus
extern "C"
{
#endif

#define GLOBAL_CFG_INI           "global.ini"

#define MACH_NAME_LGenie         "Super-Genie G"
#define MACH_NAME_LUP            "Super-Genie U"
#define MACH_NAME_LEDI_LOOP      "Super-Genie E"
#define MACH_NAME_LRO_LOOP       "Super-Genie R"
#define MACH_NAME_Genie          "Genie G"
#define MACH_NAME_UP             "Genie U"
#define MACH_NAME_EDI            "Genie E"
#define MACH_NAME_RO             "Genie R"
#define MACH_NAME_PURIST         "Genie PURIST"
#define MACH_NAME_Adapt          "Genie A"

typedef enum
{
    MACHINE_MODEL_DESK = 0,
    MACHINE_MODEL_MID  = 1
}MACHINE_MODEL_ENUM;


#define MACHINE_PARAM_TM_NUM (TIME_PARAM_NUM)
#define MACHINE_PARAM_SUBMODULE_NUM (DISP_SM_NUM)

#define LESUM (MACHINE_PARAM_SP_NUM + MACHINE_PARAM_TM_NUM + DISP_FM_NUM + DISP_PM_NUM + DISP_CAL_NUM + DISP_ALARM_PARAMS)

typedef enum
{
    ROP = 0,
    PHP ,
    DelP ,
    AddUserP
}NOT_USER_PER_ENUM;

#define AdminP (1 << ROP | 1 << PHP | 1 << DelP | 1 << AddUserP)

typedef enum
{
    MACHINE_FUNCTION_UP  = 0x1,
    MACHINE_FUNCTION_EDI = 0x2
}MACHINE_FUNCTION_ENUM;

#define MACHINE_FUNCTION_ALL (MACHINE_FUNCTION_UP|MACHINE_FUNCTION_EDI)

typedef struct
{
    char *strName;
    int iType;
    int iFunctions;
    int iDefaultModule;
    int iDefaultFmPulse;
    // int aParamId[LESUM];
}MACHINE_TYPE_STRU;

extern MACHINE_TYPE_STRU gaMachineType[MACHINE_NUM];

//ex_dcj
void MainRetriveLastRunState(int iMachineType);
void MainRetriveDefaultState(int iMachineType);
void MainRetriveMachineInfo(int iMachineType);
void MainRetriveProductMsg(int iMachineType);
void MainRetriveInstallMsg(int iMachineType);
void MainRetriveExConfigParam(int iMachineType);
void MainRetriveExConsumableMsg(int iMachineType, DISP_CONSUME_MATERIAL_SN_STRU &cParam, DISP_MACHINERY_SN_STRU  &mParam);
//end

void MainRetriveMachineParam(int iMachineType,DISP_MACHINE_PARAM_STRU  &Param);
void MainRetriveTMParam(int iMachineType,DISP_TIME_PARAM_STRU  &Param);
void MainRetriveAlarmSetting(int iMachineType,DISP_ALARM_SETTING_STRU  &Param);
void MainRetriveSubModuleSetting(int iMachineType,DISP_SUB_MODULE_SETTING_STRU  &Param);
void MainRetriveCMParam(int iMachineType,DISP_CONSUME_MATERIAL_STRU  &Param);
void MainSaveMachineType(int &iMachineType);
void MainSaveMachineParam(int iMachineType,DISP_MACHINE_PARAM_STRU  &Param);
void MainSaveTMParam(int iMachineType,DISP_TIME_PARAM_STRU  &Param);
void MainSaveAlarmSetting(int iMachineType,DISP_ALARM_SETTING_STRU  &Param);
void MainSaveSubModuleSetting(int iMachineType,DISP_SUB_MODULE_SETTING_STRU  &Param);
void MainSaveCMParam(int iMachineType,DISP_CONSUME_MATERIAL_STRU  &Param);
void MainRetriveGlobalParam(void);
void MainResetCmInfo(int iSel);
void MainSaveCMInfo(int iMachineType,DISP_CONSUME_MATERIAL_STRU  &Param);
void MainSaveFMParam(int iMachineType,DISP_FM_SETTING_STRU  &Param);
void MainRetriveFmParam(int iMachineType,DISP_FM_SETTING_STRU  &Param);

//ex_dcj
void MainSaveLastRunState(int iMachineType);
void MainSaveDefaultState(int iMachineType);
void MainSaveExMachineMsg(int iMachineType);
void MainSaveProductMsg(int iMachineType);
void MainSaveInstallMsg(int iMachineType);
void MainSaveExConfigParam(int iMachineType);
void MainSaveExConsumableMsg(int iMachineType,CATNO cn,LOTNO ln,int iIndex, int category);
void MainSaveCalibrateParam(int iMachineType, QMap<int, DISP_PARAM_CALI_ITEM_STRU> &map);
//end

void MainSavePMParam(int iMachineType,DISP_PM_SETTING_STRU  &Param);
void MainRetrivePmParam(int iMachineType,DISP_PM_SETTING_STRU  &Param);
void MainUpdateSpecificParam(int iParamType);
void MainUpdateGlobalParam(void);
void MainSaveMiscParam(int iMachineType,DISP_MISC_SETTING_STRU  &Param);
void MainRetriveMiscParam(int iMachineType,DISP_MISC_SETTING_STRU  &Param);
void MainRetriveCleanParam(int iMachineType,DISP_CLEAN_SETTING_STRU  &Param);
void MainSaveMacSnItem(int iMachineType,CATNO cn,LOTNO ln,int iIndex);
void MainSaveCMSnItem(int iMachineType,CATNO cn,LOTNO ln,int iIndex);
void MainSaveMacSnParam(int iMachineType,DISP_MACHINERY_SN_STRU  &Param);
void MainSaveCMSnParam(int iMachineType,DISP_CONSUME_MATERIAL_SN_STRU  &Param);
void MainSavePMParam(int iMachineType,DISP_PM_SETTING_STRU  &Param);
void MainRetrivePmParam(int iMachineType,DISP_PM_SETTING_STRU  &Param);
void MainRetriveCalParam(int iMachineType,DISP_CAL_SETTING_STRU  &Param);
void MainSaveCalParam(int iMachineType,DISP_CAL_SETTING_STRU  &Param);
void MainSaveCleanParam(int iMachineType,DISP_CLEAN_SETTING_STRU  &Param);

// for printer
int printerDispenseWater(QString &strMachineType, QString &strSN,QString &strMfd, QString& strTime,QString &strDate,float res,float temp,int toc,float vol,QString& type,QString &user);
int printerFillingTank(QString &strMachineType, QString &strSN,QString &strMfd, QString& strTime,QString &strDate,float prod,float temp,int rej,int feed,int perm,int dur);
int printerAlert(bool bAlert,QString &strMachineType, QString &strSN,QString &strMfd, QString& strTime,QString &strDate,QString& strInfo);
int printerAlarm(bool bAlarm,QString &strMachineType, QString &strSN,QString &strMfd, QString& strTime,QString &strDate,QString& strInfo);
int printerService(QString &strMachineType, QString &strSN,QString &strMfd, QString& strTime,QString &strDate,QString &strInfo);

#ifdef __cplusplus
}
#endif

#endif // MYPARAMS_H
