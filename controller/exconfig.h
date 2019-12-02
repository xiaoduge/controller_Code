#ifndef EXCONFIG_H
#define EXCONFIG_H
#include "dloginstate.h"
#include "exdisplay.h"

enum MachineFlow
{
    MachineFlow_5 = 5,
    MachineFlow_10 = 10,
    MachineFlow_12 = 12,
    MachineFlow_15 = 15,
    MachineFlow_24 = 24,
    MachineFlow_30 = 30,
    MachineFlow_32 = 32,
    MachineFlow_50 = 50,
    MachineFlow_60 = 60,
    MachineFlow_125 = 125,
    MachineFlow_150 = 150,
    MachineFlow_250 = 250,
    MachineFlow_300 = 300,
    MachineFlow_500 = 500,
    MachineFlow_600 = 600
};

struct ProductInfo
{
    int iCompany;
    QString strCatalogNo;
    QString strSerialNo;
    QString strProductDate;
    QString strInstallDate;
    QString strVersion;
};

struct AdditionalCfgParam
{
    int   iScreenSleepTime;
};

struct MachineInfo
{
    unsigned int iMachineFlow; //机型流量
};

struct AdditionalGlobalCfg
{
    int initMachine;
    ProductInfo productInfo;
    AdditionalCfgParam additionalParam;
    MachineInfo machineInfo;
    int lastRunState;  //0:standby 1:run
};

extern DLoginState gUserLoginState; //ex
extern AdditionalGlobalCfg gAdditionalCfgParam;

extern unsigned int gScreenSleepTimer;
extern bool gScreenSleeping;
extern unsigned int gAutoLogoutTimer;

#endif // EXTRADISPLAY_H
