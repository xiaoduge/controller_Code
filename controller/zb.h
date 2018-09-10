#ifndef _ZB_H_
#define _ZB_H_

#include "zbcomm.h"
#include "list.h"
#include "cminterface.h"



#ifdef __cplusplus
extern "C" {
#endif

#define AT_CONFIG_PANID    "PANID"
#define AT_CONFIG_POLLRATE "POLLRATE"
#define AT_CONFIG_DEVICEID "DEVICEID"
#define AT_QUERY_IEEE      "IEEE"
#define AT_CONFIG_APPCFG   "APPCFG"
#define AT_QUERY_SUBDEV_IEEE "NWADDR"
#define AT_QUERY_LIST        "LIST"
#define AT_QUERY_SUBDEV_SHORT "SHORTADDR"


#define Z_MAX_NODES (32)

#define RF_COMMUNICATION_ID (0xcdcd)

#define RF_MAX_CHL_NUMBER (16)

#define RF_CONFIGURATION_CHL (0)

#define ZB_CFG_PANID (65521)


enum
{
    CMD_LIST_HP = 0,
    CMD_LIST_NP,
    CMD_LIST_NUM,
}CMD_LIST_ENUM;


typedef struct
{
  uint16      shortAddr;
  uint16      nwkAddr;
  uint8       extAddr[Z_EXTADDR_LEN];
  uint8       capabilities;
} Node_t;


typedef struct 
{
    int bit1Inited    : 1;
    int bit1NetCfgFlag: 1;
    
    int iPanId;

    int iAppCfg;

    int iNetCfgMode;
    
    unsigned int ulKey;
    
    int iCalcedPanId;

    int iCalcedRfId;

    char szIeeeAdr[17];

    // for at cmd control
    char *szCmdId;
    void *cmdData;

    // for data proc
    void *cbPara;
    char dataBuf[256];
    int  databufLen;
    int  atResultCode;

    /* for buffering cmd */
    list_t lstCmd[CMD_LIST_NUM];  /* priority queue */
    int    lstCmdTick;
    int    bCmdFlag;
    int    lstQueue;
    int    stateTimer;

    int    iNodes;
    Node_t aNode[Z_MAX_NODES];

    /*2018/02/23 add for Node Check */
    

    /* for Node cfg ,temperorary data */
    int    iCfgNodeRspNum;

    int    iAtCmdErrors;
    
    int    iZbUpdated;
    
}ZB_COO_STRU;

extern ZB_COO_STRU gCoo;

void zbMsgProc(void *para,SAT_MSG_HEAD *pMsg);

int zbGetRfChlId(void);
unsigned char helper_hex2bin(unsigned char ucData1,unsigned char ucData2);
int zb_search_Nodes_by_short_addr(int iShortAddr);
int zb_search_Nodes_by_long_addr(ZLongAddr_t aLongAddr);
void zb_get_device_ieee(int iShortAddr);
int zbGetNodes(void);
void zbReset(void);
void main_printhex(unsigned char *buf,int len);

#ifdef __cplusplus
}
#endif

#endif
