#ifndef _MSG_APP_ITF_H____
#define _MSG_APP_ITF_H____

#include "msg.h"


#ifdef __cplusplus
extern "C"
{
#endif

typedef struct
{
    SAT_MSG_HEAD msgHead;
    int   id;
}TIMER_MSG_STRU;


typedef struct
{
    SAT_MSG_HEAD msgHead;
    int iCanChl;
    unsigned int ulCanId;
    int iMsgLen;
    unsigned char aucData[1];
}MAIN_CANITF_MSG_STRU;

/* 2018/02/21 add */
typedef struct
{
    SAT_MSG_HEAD msgHead;
    unsigned short usShortAdr;
    unsigned short usMsgLen;
    unsigned char  aucData[1];
}ZBITF_MAIN_MSG_STRU;

typedef struct
{
    SAT_MSG_HEAD msgHead;
    unsigned short usMsgLen;
    unsigned char  aucData[1];
}MAIN_ZBITF_MSG_STRU;


typedef struct
{
    SAT_MSG_HEAD msgHead;
    int iSubMsg;        /*  */
    int iMsgLen;        
    unsigned char aucData[1];
}MAIN_WORK_MSG_STRU;

typedef struct
{
    SAT_MSG_HEAD msgHead;
    int iSubMsgType; 
    int iMsgLen;        
    unsigned char aucData[1];
}MAIN_PARAM_MSG_STRU;

typedef struct
{
    SAT_MSG_HEAD msgHead;
    char data[1]; 
}ZB_SERIALITF_STRU;

typedef enum
{
    DATAPROC_ZB_ACTION_GET_PANID,
    DATAPROC_ZB_ACTION_SET_PANID,
        
}DATAPROC_ZB_ACTION_TYPE_ENUM;

typedef struct
{
    SAT_MSG_HEAD msgHead;
    void *cbPara;
    void *cbFunc;
    char data[1];
}DATAPROC_ZB_STRU;

typedef struct
{
    SAT_MSG_HEAD msgHead;
    int   iType;
}DATAPROC_ZB_CFG_STRU;

typedef struct
{
    SAT_MSG_HEAD msgHead;
    int   iLength;
    void  *mem;
}DATAPROC_ZB_UPD_STRU;


/* 2018/04/12 add */

typedef enum
{
    ZBITF_MAIN_MTN_CODE_RESET = 0,
    ZBITF_MAIN_MTN_CODE_NUM,
}ZBITF_MAIN_MTN_CODE_ENUM;

typedef struct
{
    unsigned char  state;
}ZBITF_MAIN_MTN_RESET_STRU;


typedef struct
{
    SAT_MSG_HEAD msgHead;
    unsigned short usSubCode;
    unsigned char  aucData[1];
}ZBITF_MAIN_MTN_MSG_STRU;


typedef DATAPROC_ZB_STRU DATAPROC_NETWORK_CFG_STRU;

#define ZB_SERIALITF_MSG_SIZE (sizeof(ZB_SERIALITF_STRU) - sizeof(SAT_MSG_HEAD) - 1)

#define MAIN_CANITF_MSG_SIZE (sizeof(MAIN_CANITF_MSG_STRU) - sizeof(SAT_MSG_HEAD) - 1)

#define ZB_MAIN_MSG_SIZE (sizeof(ZBITF_MAIN_MSG_STRU) - sizeof(SAT_MSG_HEAD) - 1)

#define MAIN_ZB_MSG_SIZE (sizeof(MAIN_ZBITF_MSG_STRU) - sizeof(SAT_MSG_HEAD) - 1)

#define TIMER_MSG_MSG_SIZE   (sizeof(TIMER_MSG_STRU) - sizeof(SAT_MSG_HEAD))

#define TIMER_MSG_LEN        (sizeof(TIMER_MSG_STRU) - sizeof(SAT_MSG_HEAD))

#define MAIN_WORK_MSG_SIZE   (sizeof(MAIN_WORK_MSG_STRU) - sizeof(SAT_MSG_HEAD) - 1)

#define MAIN_PARAM_MSG_SIZE   (sizeof(MAIN_PARAM_MSG_STRU) - sizeof(SAT_MSG_HEAD) - 1)

#define DATAPROC_ZB_MSG_SIZE  (sizeof(DATAPROC_ZB_STRU) - sizeof(SAT_MSG_HEAD))

#define DATAPROC_ZB_UPD_MSG_SIZE  (sizeof(DATAPROC_ZB_UPD_STRU) - sizeof(SAT_MSG_HEAD))

#define DATAPROC_NETWORK_CFG_MSG_SIZE  (sizeof(DATAPROC_NETWORK_CFG_STRU) - sizeof(SAT_MSG_HEAD) - 1)

#define ZBITF_MAIN_MTN_MSG_SIZE  (sizeof(ZBITF_MAIN_MTN_MSG_STRU) - sizeof(SAT_MSG_HEAD) - 1)

#ifdef __cplusplus
}
#endif

#endif
