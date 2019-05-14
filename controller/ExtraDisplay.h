#ifndef EXTRADISPLAY_H
#define EXTRADISPLAY_H
#include "ex_loginstate.h"
#include "Ex_Display_c.h"

typedef struct
{
    QString Ex_CatalogNo;
    QString Ex_SerialNo;
    QString Ex_ProDate;
    QString Ex_InsDate;
    QString Ex_SofeVer;
}Ex_SYSTEM_MSG;


typedef struct
{
    int   iScreenSleepTime;
    float flowRate; //
}Ex_COFIG_PARAM;

typedef struct
{
    unsigned int iMachineFlow; //机型流量
}Ex_MACHINE_MSG; //Machine


typedef struct
{
    int Ex_Default;
    Ex_SYSTEM_MSG Ex_System_Msg;
    Ex_COFIG_PARAM Ex_Config_Param;
    Ex_MACHINE_MSG Ex_Machine_Msg;
}Ex_DISP_GLOBAL_PARAM_STRU;

extern Ex_LoginState global_LoginState; //dcj_ex;
extern Ex_LoginState user_LoginState; //ex
extern Ex_DISP_GLOBAL_PARAM_STRU ex_gGlobalParam;

extern unsigned int g_screenSleep;
extern bool g_isScreenSleep;

#endif // EXTRADISPLAY_H
