/****************************************************************************
**
** @Author    dcj
** @DateTime  2019-12-03
** @version   v0.0.1
** @brief     Provide some additional configuration information
**
** 当前文件中的定义、声明只能用于纯C++中
****************************************************************************/

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

/**
 * @ iCompany       : rephile:0; VWR:0
 * @ strCatalogNo   : 设备货号   
 * @ strSerialNo    : 设备序列号
 * @ strProductDate : 设备生产日期   
 * @ strInstallDate : 设备安装日期    
 * @ strVersion     : 程序版本号  
 */
struct ProductInfo
{
    int iCompany;
    QString strCatalogNo;
    QString strSerialNo;
    QString strProductDate;
    QString strInstallDate;
    QString strVersion;
};

/**
 * @ iScreenSleepTime : 设置的时间内无操作设备屏保
 */
struct AdditionalCfgParam
{
    int   iScreenSleepTime;
};

/**
 * @ iMachineFlow : 设备的额定制水量
 */
struct MachineInfo
{
    unsigned int iMachineFlow; //机型流量
};


/**
 * @ initMachine     :  0:开机后设备进入初始化配置界面; 1:开机后设备进入主运行界面
 * @ productInfo     :  产品信息
 * @ additionalParam :  额外补充的一些配置信息
 * @ machineInfo     :  设备参数
 * @ lastRunState    :  设备上次运行状态(运行/待机)
 */
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
