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

extern Ex_DISP_PARAM_CALI_STRU ex_global_Cali;

#ifdef __cplusplus
}
#endif

#endif // EX_DISPLAY_C_H
