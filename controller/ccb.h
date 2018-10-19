#ifndef _CCB_H_
#define _CCB_H_

#include "SatIpc.h"
#include "Errorcode.h"
#include "msgdef.h"
#include "msg.h"
#include "list.h"
#include "timer.h"
#include "rpc.h"
#include "sapp.h"
#include "datatype.h"
#include "inneripc.h"

#include "Display.h"
#include "cminterface.h"
#include "list.h"
#include "vtask.h"

#ifdef __cplusplus
 extern "C"
 {
#endif


#define TIMER_SECOND 0
#define TIMER_MINUTE 1

#define MAX_HANDLER_NUM (APP_HAND_SET_MAX_NUMBER)

#define MAX_RFREADER_NUM (APP_RF_READER_MAX_NUMBER)

#define MAX_RFREADR_CONTENT_SIZE (256)

#define MAX_MODBUS_BUFFER_SIZE   (128)

typedef void (*modbus_call_back)(int,void *);

typedef void (*delay_call_back)(void *);


typedef void (*work_proc)(void *);

typedef union                                        
{
   float          f;
   unsigned char  auc[4];
   int            i;
   unsigned int   ul;
}un_data_type;

typedef struct
{
    list_t    list;
    work_proc Work;
    void      *Para;
}WORK_STRU;

typedef struct
{
    unsigned int ulEcoValidFlags;
    unsigned int ulPmValidFlags;
    unsigned int ulRectValidFlags;

    DISP_OBJ_STRU aValveObjs[APP_EXE_VALVE_NUM];
    DISP_OBJ_STRU aPMObjs[APP_EXE_PRESSURE_METER]; /* Pressure Meter */
    DISP_OBJ_STRU aGPumpObjs[APP_EXE_G_PUMP_NUM];
    DISP_OBJ_STRU aRPumpObjs[APP_EXE_R_PUMP_NUM];
    DISP_OBJ_STRU aRectObjs[APP_EXE_RECT_NUM];
    DISP_OBJ_STRU aEDIObjs[APP_EXE_EDI_NUM];
    DISP_OBJ_STRU aEcoObjs[APP_EXE_ECO_NUM];

    unsigned char ucDinState;

    unsigned short ausHoldRegister[APP_EXE_MAX_HOLD_REGISTERS];

    APP_PACKET_RPT_TOC_STRU tocInfo;  

    int                  iTrxMap; /* bit0 for CAN, bit1 for zigbee  */
    unsigned short       usShortAddr;
    
}EXE_BOARD_STRU;


typedef struct
{
    unsigned int   ulSec;
    int            iVChoice;
    DISP_OBJ_VALUE lstValue;
    DISP_OBJ_VALUE curValue;
}FM_HISTORY_STRU;

typedef struct
{
    unsigned int ulFmValidFlags;
    
    DISP_OBJ_STRU aFmObjs[APP_FM_FLOW_METER_NUM];

    /* for calc fm value */
    FM_HISTORY_STRU aHist[APP_FM_FLOW_METER_NUM];
    unsigned int    aulHistTotal[APP_FM_FLOW_METER_NUM];

    unsigned int    aulPwStart[APP_FM_FLOW_METER_NUM];
    unsigned int    aulPwEnd[APP_FM_FLOW_METER_NUM];
}FLOW_METER_STRU;

typedef struct
{
    unsigned int ulBgnFm;   //  in pulse
    unsigned int ulCurFm;   //  GEt cur volume = f(ulCurFm - ulBgnFm)
    
    unsigned int ulTotalFm; // in ml

    unsigned int ulBgnTm;
    unsigned int ulEndTm;
    
}QTW_MEAS_STRU;


typedef struct
{
    unsigned int  bit1Qtw        : 1;
    unsigned int  bit1PendingQtw : 1;
    unsigned int  bit1Active     : 1;
    
    int                  iDevType;      /* APP_DEV_HS_SUB_TYPE_ENUM */

    /* for temperory info */
    APP_PACKET_COMM_STRU CommHdr;
    int                  ulCanId;
    int                  iCanIdx;

    int                  iCurTrxIndex;

    QTW_MEAS_STRU        QtwMeas;

    /* for speed adjustment */
    int                  iAction; 
    int                  iSpeed; 

    int                  iTrxMap; /* bit0 for CAN, bit1 for zigbee  */
    unsigned short       usShortAddr;
}HANDLER_STRU;

typedef struct
{
   int bit1RfDetected  : 1;
   int bit1RfContValid : 1;
   int bit1Active      : 1;

   unsigned char ucBlkNum;
   unsigned char ucBlkSize;

   unsigned char aucUid[8];
   unsigned char aucRfCont[MAX_RFREADR_CONTENT_SIZE];

   INNER_IPC_STRU   Ipc;
   int              IpcRequest;
    
}RFREADER_STRU;


//typedef void (*work_proc)(void *);

typedef void (*work_cancel)(void *);


#define WORK_FLAG_CANCEL (1<<1)
#define WORK_FLAG_ACTIVE (1<<0)

typedef struct
{
    list_t      list;
    work_proc   proc;
    work_cancel cancel;
    int         flag;
    int         id;
    void        *para;
    void        *extra;
}WORK_ITEM_STRU;

typedef struct
{
    unsigned int ulMask;
    unsigned int ulValue;
}WORK_SETUP_REPORT_STRU;

typedef WORK_SETUP_REPORT_STRU WORK_SETUP_SWITCH_STRU;



typedef enum
{
    WORK_LIST_URGENT = 0,
    WORK_LIST_HP,         /* High Priority */
    WORK_LIST_LP ,        /* Low  Priority */
    WORK_LIST_NUM,
}WORK_LIST_ENUM;

typedef struct
{
    int              iMainWorkState; // refer DISP_WORK_STATE_ENUM
    int              iSubWorkState;  // refer DISP_WORK_SUB_STATE_ENUM
    int              iMainWorkState4Pw;
    int              iSubWorkState4Pw;

}WORK_STATE_STRU;

#define CCB_WORK_STATE_STACK_DEPTH (10)

typedef enum
{
    WORK_THREAD_STATE_BLK_MODBUS  = 0X1, 
    WORK_THREAD_STATE_BLK_TICKING = 0X2, 
}WORK_THREAD_STATE_ENUM;

#define MAX_HB_CHECK_COUNT 3
#define MAX_HB_CHECK_ITEMS 32

typedef struct
{
   unsigned int     bit1InitRuned       : 1;
   unsigned int     bit1AlarmRej        : 1;  /* ro jieliulv */
   unsigned int     bit1AlarmRoPw       : 1;  /* ro chanshui diandaolv */
   unsigned int     bit1AlarmEDI        : 1;  /* edi alarm */
   unsigned int     bit1AlarmUP         : 1;
   unsigned int     bit1AlarmTOC        : 1;
   unsigned int     bit1AlarmTapInPress : 1;
   unsigned int     bit1NeedFillTank    : 1;  
   unsigned int     bit1B2Full          : 1;  /* tank full */
   unsigned int     bit1FillingTank     : 1;  
   unsigned int     bit1N3Active        : 1;  /* ultralight N3 period handle flag */
   unsigned int     bit1ProduceWater    : 1;  /* RUN state & producing water */
   unsigned int     bit1B2Empty         : 1;  /* tank empty */
   unsigned int     bit1I54Cir          : 1;  
   unsigned int     bit1I44Nw           : 1;  
   unsigned int     bit1LeakKey4Reset   : 1;  
   unsigned int     bit1EngineerMode    : 1;
   unsigned int     bit1NeedTubeCir     : 1;
   unsigned int     bit1TubeCirOngoing  : 1;
   unsigned int     bit1TocOngoing      : 1;
   unsigned int     bit1SysMonitorStateRpt  : 1;
   unsigned int     bit1PeriodFlushing  : 1;

   /* 2018/01/05 add for B1 check, Life cycle starts earlier than bit1ProduceWater */
   unsigned int     bit1B1Check4RuningState  : 1;
   unsigned int     bit1B1UnderPressureDetected  : 1;
   unsigned int     bit3RuningState              : 3;    

   unsigned int     bit1AlarmROPLV        : 1;  /* ROPV alarm */
   unsigned int     bit1AlarmROPHV        : 1;  /* ROPV alarm */
   unsigned int     bit1AlarmRODV         : 1;   /* RODV alarm */
   unsigned int     bit1CirSpeedAdjust    : 1;   /* RODV alarm */


   unsigned int     ulMachineType;         /* refer MACHINE_TYPE_ENUM */
   unsigned int     ulRegisterMask;
   unsigned int     ulActiveMask;
   unsigned int     ulActiveShadowMask;
   unsigned int     ulActiveMask4HbCheck;

   unsigned char    aucHbCounts[MAX_HB_CHECK_ITEMS];
   
   EXE_BOARD_STRU   ExeBrd;

   FLOW_METER_STRU  FlowMeter;
   
   HANDLER_STRU     aHandler[MAX_HANDLER_NUM];

   RFREADER_STRU    aRfReader[MAX_RFREADER_NUM];
   
   int              iCurTwIdx;

   int              iHbtCnt;

   INNER_IPC_STRU   Ipc;
   modbus_call_back ModbusCb;

   /* ipc for rfreader */
   
   INNER_IPC_STRU   Ipc4Rfid;
   int              iRfIdRequest;

   WORK_STATE_STRU  curWorkState;

   WORK_STATE_STRU  aWorkStateStack[CCB_WORK_STATE_STACK_DEPTH];
   int              iWorkStateStackDepth;

   list_t            WorkList[WORK_LIST_NUM];
   int               iBusyWork;
   sp_thread_mutex_t WorkMutex;

   sp_thread_mutex_t ModbusMutex;
   
   sp_thread_mutex_t C12Mutex;

   sp_sem_t         Sem4Delay[WORK_LIST_NUM + 1];    /* Used in work Delay */
   SYS_TIMEO        to4Delay[WORK_LIST_NUM +1];

   /* All Following Parameters should be consistent with machine type */
   DISP_ALARM_SETTING_STRU      AlarmSettting;
   DISP_SUB_MODULE_SETTING_STRU SubModSetting;
   DISP_TIME_PARAM_STRU         TMParam;
   DISP_MACHINE_PARAM_STRU      MMParam; 
   DISP_CONSUME_MATERIAL_STRU   CMParam; 
   DISP_FM_SETTING_STRU         FMParam; 
   DISP_PM_SETTING_STRU         PMParam; 
   DISP_MISC_SETTING_STRU       MiscParam; 

   unsigned int    ulHyperTwMask;         /* switch Mask for taking hyper water */
   unsigned int    ulNormalTwMask;        /* switch Mask for taking normal water */
   unsigned int    ulCirMask;             /* switch Mask for ciculation water */

   unsigned int    ulRunMask;            /* switch Mask for producing water */

   unsigned int    ulPMMask;             /* PM facilities for specific machine type */
   


   unsigned int     ulAlarmEDITick ;
   unsigned int     ulAlarmRejTick ;
   unsigned int     ulAlarmRoPwTick ;
   unsigned int     ulAlarmUPTick ;

   unsigned int     ulB2FullTick;
   unsigned int     ulLPPTick;

   /* 2018/01/05 add accroding to ZHANG chunhe*/
   unsigned int     ulB1UnderPressureTick;
   
   unsigned int     ulTubeCirTick;
   unsigned int     ulProduceWaterBgnTime;

   unsigned int     ulCirTick;
   unsigned int     ulLstCirTick;
            int     iCirType;           /* refer APP_CIR_TYPE_ENUM */
            int     iTocStage;          /* refer TOC_STAGE_ENUM */
            int     iTocStageTimer;

            int     iTubeCirTime;       /* in minutes */
            
            int     iInitRunTimer;

   unsigned int     ulN3Duration;
   unsigned int     ulN3PeriodTimer;

   unsigned int     ulFiredAlarmFlags;

   unsigned int     ulKeyWorkStates;

   WORK_SETUP_REPORT_STRU WorkRptSetParam4Exe;
   WORK_SETUP_REPORT_STRU WorkRptSetParam4Fm;
   WORK_SETUP_SWITCH_STRU WorkSwitchSetParam4Exe;

   QTW_MEAS_STRU          QtwMeas;

   unsigned  char   aucModbusBuffer[MAX_MODBUS_BUFFER_SIZE];

   unsigned int     ulWorkThdState; /* 0: free , other blocked */
   unsigned int     ulWorkThdAbort;

   /* 2017/10/24 add for wash */
   int              iWashType;  

   /* for CAN Healthy check */
   unsigned int     ulCanCheck;

   /* 2018/01/11 add begin:  for REJ check */
   unsigned int     ulRejCheckCount;
   /* 2018/01/11 add end */

   /* 2018/01/15 add begin:  for ROP velocity check */
   unsigned int     ulRopVelocity;
   unsigned int     ulLstRopFlow;
   unsigned int     ulLstRopTick;
   int              iRopVCheckLowEventCount;
   int              iRopVCheckLowRestoreCount;

#if 0   
   int              iRopVCheckHighEventCount;
   int              iRopVCheckHighRestoreCount;
#endif   
   unsigned int     ulRodVelocity;
   unsigned int     ulLstRodFlow;
   unsigned int     ulLstRodTick;
   int              iRoDVCheckEventCount;
   int              iRoDVCheckRestoreCount;
   
   /* 2018/01/15 add end */

   /* 2018/01/24 ADD begin: for adapter */
   unsigned int     ulAdapterAgingCount;
   /* 2018/01/24 ADD end : for adapter */
    

   int              aiSpeed2ScaleMap[PUMP_SPEED_NUM];

   /* 2018/02/22 add for zigbee */
   int              aulActMask4Trx[APP_TRX_NUM];
   
}CCB;

extern TASK_HANDLE TASK_HANDLE_MAIN ;
extern TASK_HANDLE TASK_HANDLE_CANITF;
extern TASK_HANDLE TASK_HANDLE_MOCAN ;


extern CCB gCcb;
extern unsigned int gulSecond;

#define MOTOR_MAGIC 'L'
#define FEED_DOG 	_IOW(MOTOR_MAGIC, 0,int)

#ifdef __cplusplus
}
#endif

#endif

