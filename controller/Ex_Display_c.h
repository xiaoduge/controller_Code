#ifndef EX_DISPLAY_C_H
#define EX_DISPLAY_C_H
#include "Display.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct
{
   float fk;
}Ex_DISP_PARAM_CALI_ITEM_STRU;

typedef struct
{
    Ex_DISP_PARAM_CALI_ITEM_STRU pc[DISP_PC_COFF_NUM];

}Ex_DISP_PARAM_CALI_STRU;

enum EX_RECT_N
{
    EX_RECT_N1 = 0,
    EX_RECT_N2,
    EX_RECT_N3,
//    EX_RECT_LOOPN,
    EX_RECT_NUM
};

typedef struct
{
    int EX_N_NO[EX_RECT_NUM];
}EX_RECT_STATE;

typedef struct
{
    unsigned int ulAlarmNRectTick[EX_RECT_NUM];
}EX_ALARM_TICK;

typedef struct
{
    unsigned int     bit1AlarmN1       : 1;
    unsigned int     bit1AlarmN2       : 1;
    unsigned int     bit1AlarmN3       : 1;

}EX_ALARM_RECT;

typedef struct
{
    EX_RECT_STATE Ex_Rect_State;
    EX_ALARM_TICK Ex_Alarm_Tick;
    EX_ALARM_RECT Ex_Alarm_Bit;
}EX_CCB;

extern Ex_DISP_PARAM_CALI_STRU ex_global_Cali;
extern EX_CCB  ex_gCcb;

extern unsigned int ex_gulSecond;

int Ex_FactoryTest(int select);


#ifdef __cplusplus
}
#endif

#endif // EX_DISPLAY_C_H
