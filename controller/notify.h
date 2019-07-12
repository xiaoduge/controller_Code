#ifndef _NOTIFY_H_
#define _NOTIFY_H_

#include "datatype.h"
#include "cminterface.h"

#ifdef __cplusplus
 extern "C"
 {
#endif

#pragma   pack(1)

typedef enum
{
   DISP_NOT_PM = 0,   /* Pressure Meter for B1&B2&B3 */
   DISP_NOT_ECO,
   DISP_NOT_FM,
   DISP_NOT_GPUMP,
   DISP_NOT_RPUMP,
   DISP_NOT_EDI,
   DISP_NOT_RECT,
   DISP_NOT_STATE,    /* work state */
   DISP_NOT_ALARM,   
   DISP_NOT_ASC_INFO, /* Show ascii info */
   DISP_NOT_GET_PARAM,
   DISP_NOT_UPD_PARAM,
   DISP_NOT_NV_STAT, /* Statics info for ultra violet bulb */
   DISP_NOT_TW_STAT, /* Statics info for Taking water */
   DISP_NOT_PWDURATION_STAT, /* Statics info for Water Produce */
   DISP_NOT_PWVOLUME_STAT,  /*  Water Produce related measurement */
   DISP_NOT_FM_STAT,        /* Flow meter related measurement */
   DISP_NOT_WH_STAT,        /* WASH STATE report */

   DISP_NOT_RF_STATE,       /* RFID Reader state indication */
   DISP_NOT_HANDLER_STATE,     /* Handler State Notify */
   DISP_NOT_EXEBRD_STATE,      /* executing board State Notify */
   DISP_NOT_FMBRD_STATE,       /* flow measurement board State Notify */

   DISP_NOT_TUBEUV_STATE,      /* Pump Usage State */

   DISP_NOT_SWITCH_STATE,
   
   DISP_NOT_RPUMP_STATE,

   DISP_NOT_SW_PRESSURE,

   DISP_NOT_SPEED,
   
   DISP_NOT_TOC,

   DISP_NOT_UPD,

   DISP_NOT_QTW_VOLUME,

   DISP_NOT_DEC_PRESSURE,

   DISP_NOT_REALTIME_QTW_VOLUME, //2019.7.9 dcj add

   DISP_NOT_NUM,

}NOT_CODE_ENUM;

typedef struct
{
    unsigned char ucCode; /* refer NOT_CODE_ENUM */
    
}NOT_HEADER_STRU;

/* FOR ECO METER */
typedef struct
{
    unsigned char ucId;   /* 0 ~ 2, 0XFF serve as terminator */
    float          fValue;
    unsigned short usValue;
}NOT_ECO_ITEM_STRU;


/* FOR PRESSURE METER */
typedef struct
{
    unsigned char ucId;   /* 0 ~ 2, 0XFF serve as terminator */
    unsigned int  ulValue;
}NOT_PM_ITEM_STRU;

/* FOR FLOW METER */
typedef NOT_PM_ITEM_STRU        NOT_FM_ITEM_STRU;


typedef struct
{
    unsigned char ucPart; /* alarm part : refer DISP_ALARM_PART_ENUM */
    unsigned char ucId;   /* alarm type : refer DISP_ALARM_PART_ENUM */
    unsigned char ucFlag; /* 0: restore, 1: fired */
}NOT_ALARM_ITEM_STRU;

typedef struct
{
    unsigned char ucId;   /* alarm type : refer NOT_STATE_ENUM */
}NOT_STATE_ITEM_STRU;


typedef struct 
{
    unsigned char ucType; /* UP or Hyper */
    unsigned char ucId;   /* Handler Index */  
    unsigned int  ulValue; /* Flow */
    unsigned int  ulBgnTime; /* time in second */
    unsigned int  ulEndTime; /* time in second */
}NOT_NVS_ITEM_STRU;

typedef struct 
{
    unsigned char ucType; /* see display.h */
    unsigned char ucState; /* sate 0: stop ,other: start */  
}NOT_WH_ITEM_STRU;


typedef struct 
{
    unsigned char ucId;      /* ID */
    unsigned int  ulBgnTime; /* time in second */
}NOT_WP_ITEM_STRU;

/* Flow meter Related statistics */
typedef struct 
{
    unsigned char ucId;      /* id  */
    unsigned int  ulValue;   /* quantity */
}NOT_FMS_ITEM_STRU;  

/* FOR RFID Reader */
typedef struct
{
    unsigned char ucId;   /* 0 ~ 4, 0XFF serve as terminator */
    unsigned char ucState;
}NOT_RFID_ITEM_STRU;

typedef struct
{
    float          fB;
    float          fP;
}NOT_TOC_ITEM_STRU;

typedef struct
{
    int         iType;
    int         iResult;
    float       iPercent;
}NOT_UPD_ITEM_STRU;

typedef struct 
{
    unsigned char ucType; /* UP or Hyper */
    unsigned char ucId;   /* Handler Index */  
    unsigned int  ulValue; /* Qtw volumn */
}NOT_QTW_VOLUME_ITEM_STRU;

typedef struct
{
    unsigned int  ulValue;    //2019.7.9 dcj add
}NOT_REALTIME_QTW_VOLUME_ITEM_STRU;


typedef NOT_RFID_ITEM_STRU NOT_HANDLER_ITEM_STRU;

typedef NOT_RFID_ITEM_STRU NOT_EXEBRD_ITEM_STRU;

typedef NOT_RFID_ITEM_STRU NOT_FMBRD_ITEM_STRU;

typedef NOT_RFID_ITEM_STRU NOT_RPUMP_STATE_ITEM_STRU;

typedef NOT_STATE_ITEM_STRU NOT_ASC_INFO_ITEM_STRU;

/* FOR GETTING PARAM */
typedef NOT_STATE_ITEM_STRU NOT_GPM_ITEM_STRU;


typedef NOT_NVS_ITEM_STRU NOT_FS_ITEM_STRU;

typedef NOT_PM_ITEM_STRU NOT_RECT_ITEM_STRU;

typedef NOT_PM_ITEM_STRU NOT_GPUMP_ITEM_STRU;

typedef NOT_PM_ITEM_STRU NOT_RPUMP_ITEM_STRU;

typedef NOT_PM_ITEM_STRU NOT_EDI_ITEM_STRU;

typedef struct
{
    NOT_HEADER_STRU Hdr;
    unsigned char   aucData[1];
}NOT_INFO_STRU;
typedef struct 
{
    int  iType;
    int  iSpeed; 
}NOT_SPEED_ITEM_STRU;
typedef struct 
{
    int  iType;
    int  iAction; 
}NOT_DECPRE_ITEM_STRU;

typedef enum
{
    NOT_STATE_INIT = 0,
    NOT_STATE_WASH,    
    NOT_STATE_RUN,    
    NOT_STATE_LPP,     /* Low pressure protection state */
    NOT_STATE_QTW,     /* Quantity taking water  */
    NOT_STATE_CIR,     /* Circulation  */
    NOT_STATE_DEC,     /* Decompress  */
    NOT_STATE_OTHER,   /* Other state */
}NOT_STATE_ENUM;

typedef enum
{
    NOT_PARAM_MACHINE_TYPE = 0,  /* Machine Type */
    NOT_PARAM_MACHINE_PARAM,
    NOT_PARAM_SUBMODULE_PARAM,
    NOT_PARAM_ALARM_PARAM,
    NOT_PARAM_TIME_PARAM,
    NOT_PARAM_CM_PARAM,
    NOT_PARAM_FM_PARAM,
    NOT_PARAM_PM_PARAM,
    NOT_PARAM_MISC_PARAM,
    NOT_PARAM_ALL_PARAM = 0X7F,

}NOT_PARAM_ENUM;

typedef enum
{
    NOT_RUNING_STATE_NONE = 0,  /* Machine Type */
    NOT_RUNING_STATE_FLUSH,
    NOT_RUNING_STATE_CLEAN,
    NOT_RUNING_STATE_NUM,
}NOT_RUNING_STATE_ENUM;


#pragma pack()

#ifdef __cplusplus
}
#endif

#endif

