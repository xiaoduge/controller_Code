#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <dirent.h>
#include <libgen.h>
#include <signal.h>
#include <fcntl.h>
#include <time.h>
#include <errno.h>
#include <sys/time.h>


#include "SatIpc.h"
#include "msgdef.h"
#include "msg.h"
#include "list.h"
#include "timer.h"
#include "zb.h"
#include "Interface.h"
#include "memory.h"
#include "SerialItf4zb.h"
#include "sapp.h"
#include "rpc.h"
#include "Errorcode.h"

#include "messageitf.h"
#include "cminterface.h"

#include "inneripc.h"

#include "helper.h"
#include "zbcmitf.h"
#include "vtask.h"
#include "Display.h"


#ifdef __cplusplus
extern "C"
{
#endif

#define TIMER_SECOND 0
#define TIMER_MINUTE 1

#define TIMER_STATE_LENGTH (5)

#define BUS_ITF_MAX_AMBLES   30

#define SERIAL_FILE "/dev/ttyO1"

#define ZB_DEBUG
#ifdef ZB_DEBUG
#define ZB_DP(fmt, args...) do {printf(fmt,##args);printf("\r\n");}while(0);
#define ZB_DP1(fmt, args...) do {printf(fmt,##args);}while(0);
#else
#define ZB_DP(fmt, args...) 
#define ZB_DP1(fmt, args...) do {printf(fmt,##args);}while(0);
#endif

extern TASK_HANDLE TASK_HANDLE_MAIN ;

enum IAP_PROC
{
   IAP_PROC_DOWNLOAD,
   IAP_PROC_FORCEBOOT,
   IAP_PROC_BUTT
};

enum IAP_STATE
{
   IAP_STATE_BOOT,
   IAP_STATE_HS,
   IAP_STATE_FS,
   IAP_STATE_DATA,
   IAP_STATE_BUTT
};

typedef struct
{
    int m_bDownloadVerify;
    
    int m_nCurrentPacketSize;
    
    int m_nInDownLoading;
    
    int m_nInVerifying;
    
    int m_nHandleShakeNum;
    
    int m_bEnableRpt;
    
    int m_cmdState;
    int m_cmdTimes;
    
    int m_nThroughputMode;
    
    int m_nFileSize;

    int m_nState; // IAP_STATE 
     
    int m_nProc;  // IAP_PROC
    
    int m_nDownLoadPercent;
 
}IAP_STRU;

#define MAX_CMD_BUF_NUM (32)

typedef void (*CMD_END_CALLBACK)(void *para,int rslt,char *rsp);

typedef void (*CMD_BGN_CALLBACK)(void *para); 


typedef struct
{
    list_t          list;
    int             idx;
    void            *para;
    CMD_END_CALLBACK end_cb;
    CMD_BGN_CALLBACK bgn_cb;
    char            *cmdId;
    int              stateTimer;
    char             cmdBuf[128];
}CMD_BUF_STRU;


unsigned char OutputBuf[256];
unsigned char *Sendbuf = &OutputBuf[BUS_ITF_MAX_AMBLES + 1];

unsigned char gFileMem[MAX_FILE_SIZE];

static CMD_BUF_STRU saCmdBuf[MAX_CMD_BUF_NUM];
static unsigned int sulCmdBufMask;

TASK_HANDLE TASK_HANDLE_SERIALITF4ZB;
extern TASK_HANDLE TASK_HANDLE_ZB;
static SYS_TIMEO *gzbTo;

INNER_IPC_STRU gZbIpc;

IAP_STRU Iap;
ZB_COO_STRU gCoo;

static int sZbSecond;

typedef struct {
    char *attr;
    char *rsp;
}ZB_CONFIG_PARA_STRU;

typedef struct {
    char *rsp;
}ZB_NETWORK_CONFIG_PARA_STRU;


int zbSndMsg2SerialItf(char *data,int length);
void zb_proc_coo_panid(void);
void zb_set_coo_cfg_mode(int iMode);
void zb_reset_coo(void);
void ProtolProc(char *Revbuf);

void main_printhex(unsigned char *buf,int len)
{
    int iLoop ;    
    for (iLoop = 0; iLoop < len;iLoop++)  
    {      
        ZB_DP1("%02x",buf[iLoop]); 
    }   
    ZB_DP1("\r\n"); 
}

void InitCmdBuf(int iIndex)
{
    INIT_LIST_HEAD(&saCmdBuf[iIndex].list);
    
    saCmdBuf[iIndex].idx = iIndex;

    saCmdBuf[iIndex].para   = NULL;
    saCmdBuf[iIndex].end_cb = NULL;
    saCmdBuf[iIndex].bgn_cb = NULL;
    saCmdBuf[iIndex].cmdId  = NULL;

    saCmdBuf[iIndex].stateTimer = 2;// default 2m
}


int AllocCmdBuf(void)
{
    int iLoop;
    for (iLoop = 0; iLoop < MAX_CMD_BUF_NUM; iLoop++)
    {
        if ((1 << iLoop) & sulCmdBufMask)
        {
            continue;
        }

        sulCmdBufMask |= (1 << iLoop);

        InitCmdBuf(iLoop);

        return iLoop;
    }

    return MAX_CMD_BUF_NUM;
}

void FreeCmdBuf(int iIndex)
{
    if (iIndex < MAX_CMD_BUF_NUM)
    {
        sulCmdBufMask &= ~(1 << iIndex);
    }
}

void CalcCheckSum(UINT8 *pData,UINT8 ucLen)
{
    UINT8 iLoop;
    UINT8 iCheckSum = 0;
    for (iLoop = 0; iLoop < ucLen; iLoop++)
    {
        iCheckSum ^= pData[iLoop];
    }

    pData[ucLen] = iCheckSum;
}

int WriteOutputReport(int MsgLen,int preambles)
{
    //Send a report to the device.

    int Result = 0;

    if (BUS_ITF_MAX_AMBLES < preambles)
    {
        return 1;
    }

    {
        uint8 *buf   = Sendbuf - preambles - 1;
        uint8 *pData = buf;
        memset(buf,0xff,preambles);
        pData   += preambles;
        pData[0] = RPC_UART_SOF;
        pData++;
        CalcCheckSum(pData,MsgLen);
        Result = zbSndMsg2SerialItf((char *)buf,preambles + MsgLen + RPC_UART_FRAME_OVHD);
    }

    return Result;
}

void SetCurDeviceTimer(uint32 cmdstate,uint32 time) 
{
    Iap.m_cmdState = cmdstate;
    Iap.m_cmdTimes = time ;
    return ;
}


void MakeHandShake()
{
    Sendbuf[RPC_POS_LEN] = 0; // no data
    Sendbuf[RPC_POS_CMD0] = RPC_SYS_BOOT;
    Sendbuf[RPC_POS_CMD1] = SBL_HANDSHAKE_CMD;

    {
       int cmdState = BUILD_UINT16(Sendbuf[RPC_POS_CMD1],Sendbuf[RPC_POS_CMD0]); 
       int cmdTimes = 1;
       
       SetCurDeviceTimer(cmdState,cmdTimes);    
    }
}

void MakeFileSizeCmd()
{

    ZB_DP("%s",__FUNCTION__);;

    Sendbuf[RPC_POS_LEN] = 2; // no data
    Sendbuf[RPC_POS_CMD0] = RPC_SYS_BOOT;
    Sendbuf[RPC_POS_CMD1] = SBL_FILE_SIZE_CMD;

    Sendbuf[SBL_REQ_ADDR_LSB] = (Iap.m_nFileSize >> 2) & 0xff;
    Sendbuf[SBL_REQ_ADDR_MSB] = (Iap.m_nFileSize >> 10) & 0xff;

    {
       int cmdState = BUILD_UINT16(Sendbuf[RPC_POS_CMD1],Sendbuf[RPC_POS_CMD0]); 
       int cmdTimes = TIMER_STATE_LENGTH;
       
       SetCurDeviceTimer(cmdState,cmdTimes);    
    }
}


void MakeWriteCmd()
{
    Sendbuf[RPC_POS_LEN]  = SBL_RW_BUF_LEN + 2; 
    Sendbuf[RPC_POS_CMD0] = RPC_SYS_BOOT;
    Sendbuf[RPC_POS_CMD1] = SBL_WRITE_CMD;
    
    Sendbuf[SBL_REQ_ADDR_LSB] = (Iap.m_nCurrentPacketSize/4) & 0xff;
    Sendbuf[SBL_REQ_ADDR_MSB] = ((Iap.m_nCurrentPacketSize/4)>>8) & 0xff;
    
    memcpy(&Sendbuf[SBL_REQ_DAT0],&gFileMem[Iap.m_nCurrentPacketSize],SBL_RW_BUF_LEN);
    
   {
       int cmdState = BUILD_UINT16(Sendbuf[RPC_POS_CMD1],Sendbuf[RPC_POS_CMD0]); 
       int cmdTimes = TIMER_STATE_LENGTH;
       
       SetCurDeviceTimer(cmdState,cmdTimes);    
   }    
    
}
void MakeReadCmd()
{

    Sendbuf[RPC_POS_LEN] =  2; // for addr
    Sendbuf[RPC_POS_CMD0] = RPC_SYS_BOOT;
    Sendbuf[RPC_POS_CMD1] = SBL_READ_CMD;
    
    Sendbuf[SBL_REQ_ADDR_LSB] = (Iap.m_nCurrentPacketSize/4) & 0xff;
    Sendbuf[SBL_REQ_ADDR_MSB] = ((Iap.m_nCurrentPacketSize/4)>>8) & 0xff;
    
    {
       int cmdState = BUILD_UINT16(Sendbuf[RPC_POS_CMD1],Sendbuf[RPC_POS_CMD0]); 
       int cmdTimes = TIMER_STATE_LENGTH;
       
       SetCurDeviceTimer(cmdState,cmdTimes);    
    }
    
}

void MakeEnableCmd()
{
    Sendbuf[RPC_POS_LEN] = 0; // no data
    Sendbuf[RPC_POS_CMD0] = RPC_SYS_BOOT;
    Sendbuf[RPC_POS_CMD1] = SBL_ENABLE_CMD;

    {
       int cmdState = BUILD_UINT16(Sendbuf[RPC_POS_CMD1],Sendbuf[RPC_POS_CMD0]); 
       int cmdTimes = TIMER_STATE_LENGTH;
       
       SetCurDeviceTimer(cmdState,cmdTimes);    
    }

}


void MakeDataCmd(unsigned char *pData,int iLen)
{
    Sendbuf[RPC_POS_LEN] = iLen; // no data
    Sendbuf[RPC_POS_CMD0] = RPC_SYS_APP;
    Sendbuf[RPC_POS_CMD1] = SAPP_CMD_DATA;

    memcpy(&Sendbuf[RPC_POS_DAT0],pData,iLen);

    
    WriteOutputReport(Sendbuf[RPC_POS_LEN] + RPC_FRAME_HDR_SZ,0);

}



void MakeAirDataCmd(unsigned char *pData,int iLen)
{
    Sendbuf[RPC_POS_LEN] = iLen; // no data
    Sendbuf[RPC_POS_CMD0] = RPC_SYS_APP;
    Sendbuf[RPC_POS_CMD1] = SAPP_CMD_AIR_DATA;

    memcpy(&Sendbuf[RPC_POS_DAT0],pData,iLen);

    //main_printhex(pData,iLen);                       
    
    WriteOutputReport(Sendbuf[RPC_POS_LEN] + RPC_FRAME_HDR_SZ,0);

}


void MakeResetCmd(void)
{
    Sendbuf[RPC_POS_LEN] = 0; // no data
    Sendbuf[RPC_POS_CMD0] = RPC_SYS_APP;
    Sendbuf[RPC_POS_CMD1] = SAPP_CMD_RESET;
    
    WriteOutputReport(Sendbuf[RPC_POS_LEN] + RPC_FRAME_HDR_SZ,0);

}

void MakeReInitCmd(void)
{
    Sendbuf[RPC_POS_LEN] = 0; // no data
    Sendbuf[RPC_POS_CMD0] = RPC_SYS_APP;
    Sendbuf[RPC_POS_CMD1] = SAPP_CMD_SYS_INIT;
    
    WriteOutputReport(Sendbuf[RPC_POS_LEN] + RPC_FRAME_HDR_SZ,0);

}

void MakeQueryId()
{
    Sendbuf[RPC_POS_LEN] = 0; // no data
    Sendbuf[RPC_POS_CMD0] = RPC_SYS_BOOT;
    Sendbuf[RPC_POS_CMD1] = SBL_QUERY_ID_CMD;

    {
       int cmdState = BUILD_UINT16(Sendbuf[RPC_POS_CMD1],Sendbuf[RPC_POS_CMD0]); 
       int cmdTimes = TIMER_STATE_LENGTH;
       
       SetCurDeviceTimer(cmdState,cmdTimes);    
    }

}



void MakeQueryVersion()
{

    Sendbuf[RPC_POS_LEN] = 0; // no data
    Sendbuf[RPC_POS_CMD0] = RPC_SYS_BOOT;
    Sendbuf[RPC_POS_CMD1] = SBL_QUERY_VERSION_CMD;

    {
       int cmdState = BUILD_UINT16(Sendbuf[RPC_POS_CMD1],Sendbuf[RPC_POS_CMD0]); 
       int cmdTimes = TIMER_STATE_LENGTH;
       
       SetCurDeviceTimer(cmdState,cmdTimes);    
    }

}

void MakeForcebootCmd()
{
    Sendbuf[RPC_POS_LEN] = 0; // no data
    Sendbuf[RPC_POS_CMD0] = RPC_SYS_BOOT;
    Sendbuf[RPC_POS_CMD1] = SBL_TGT_BOOTLOAD;

    {
        int cmdState = BUILD_UINT16(Sendbuf[RPC_POS_CMD1],Sendbuf[RPC_POS_CMD0]); 
        int cmdTimes = TIMER_STATE_LENGTH;
    
        SetCurDeviceTimer(cmdState,cmdTimes);   
    }

}


void MakeAtCmd(char *atCmd)
{
    int data_len = strlen(atCmd);

    Sendbuf[RPC_POS_LEN]  = data_len + 1; // late calc data size,append '\n'
    Sendbuf[RPC_POS_CMD0] = RPC_SYS_APP;
    Sendbuf[RPC_POS_CMD1] = SAPP_CMD_AT;

    strcpy((char *)&Sendbuf[RPC_POS_DAT0],atCmd);
    
    Sendbuf[RPC_POS_DAT0 + data_len] = '\n';
    
    Iap.m_cmdState = BUILD_UINT16(Sendbuf[RPC_POS_CMD1],Sendbuf[RPC_POS_CMD0]); 
    Iap.m_cmdTimes = TIMER_STATE_LENGTH;            
}



void MakeAirAtCmd(char *atCmd)
{
    int data_len = strlen(atCmd);

    Sendbuf[RPC_POS_LEN]  = data_len + 1; // late calc data size,append '\n'
    Sendbuf[RPC_POS_CMD0] = RPC_SYS_APP;
    Sendbuf[RPC_POS_CMD1] = SAPP_CMD_AIR_AT;

    strcpy((char *)&Sendbuf[RPC_POS_DAT0],atCmd);
    
    Sendbuf[RPC_POS_DAT0 + data_len] = '\n';
    
    Iap.m_cmdState = BUILD_UINT16(Sendbuf[RPC_POS_CMD1],Sendbuf[RPC_POS_CMD0]); 
    Iap.m_cmdTimes = TIMER_STATE_LENGTH;            
}


int PrepareForceBoot() 
{
    int offset = 10; // preambles to wake up device
    
    MakeForcebootCmd();
    
    return WriteOutputReport(Sendbuf[RPC_POS_LEN] + RPC_FRAME_HDR_SZ,offset);

}

void zb_ScheduleCmd(void)
{
    int iLoop;

    int bFind = FALSE;

    if (gCoo.bCmdFlag)
    {
        return;
    }

    for (iLoop = 0; iLoop < CMD_LIST_NUM; iLoop++)
    {
        if (list_empty(&gCoo.lstCmd[iLoop]))
        {
            continue ;
        }
        bFind = TRUE;

        break;
    
    }

    if (bFind)
    {
        CMD_BUF_STRU *pCmd = (CMD_BUF_STRU *)list_entry(gCoo.lstCmd[iLoop].next,CMD_BUF_STRU,list);
        
        (pCmd->bgn_cb)(pCmd->cmdBuf);
        
        WriteOutputReport(Sendbuf[RPC_POS_LEN] + RPC_FRAME_HDR_SZ,10);

        gCoo.bCmdFlag   = TRUE;
        gCoo.lstCmdTick = sZbSecond;
        gCoo.lstQueue   = iLoop;

        gCoo.stateTimer = pCmd->stateTimer;
 
        gCoo.szCmdId    = pCmd->cmdId;
        gCoo.cmdData    = pCmd->para;
    }
}


void zb_AddCmd(CMD_BUF_STRU *cmd,int queue)
{
    list_add_tail(&cmd->list,&gCoo.lstCmd[queue]);

    zb_ScheduleCmd();

}

void zb_RmvCmdHeader(void)
{

    if (gCoo.lstQueue >=  CMD_LIST_NUM)
    {
        return;
    }
    if (list_empty(&gCoo.lstCmd[gCoo.lstQueue]))
    {
        return ;
    }

    {
        CMD_BUF_STRU *pCmd = (CMD_BUF_STRU *)list_entry(gCoo.lstCmd[gCoo.lstQueue].next,CMD_BUF_STRU,list);

        if (pCmd->end_cb) (pCmd->end_cb)(pCmd->para,gCoo.atResultCode,gCoo.dataBuf);
        
        list_del_init(&pCmd->list);

        FreeCmdBuf(pCmd->idx);

        gCoo.bCmdFlag = FALSE;
        gCoo.lstQueue = CMD_LIST_NUM;

        zb_ScheduleCmd();
    }
    
}


void zb_ResetCmdQueue()
{
    int iLoop;

    for (iLoop = 0; iLoop < CMD_LIST_NUM; iLoop++)
    {
    
        list_t *pos,*next;
        list_for_each_safe(pos,next,&gCoo.lstCmd[iLoop])
        {
            CMD_BUF_STRU *pCmd = (CMD_BUF_STRU *)list_entry(gCoo.lstCmd[iLoop].next,CMD_BUF_STRU,list);
        
            list_del_init(&pCmd->list);
            
            FreeCmdBuf(pCmd->idx);
        }

    }
}

void zb_ResetCmd()
{

    if (sZbSecond - gCoo.lstCmdTick >= gCoo.stateTimer)
    {
        gCoo.atResultCode = -1; // timeout
        zb_RmvCmdHeader();
    }
}


void zbSndUpdResult(int iResult,int iPercent)
{
    DispUpdNotify(0,iResult,iPercent);
}

/****************************************************************************
 * @fn          sAddrExtCpy
 *
 * @brief       Copy an extended address.
 *
 * input parameters
 *
 * @param       pSrc         - Pointer to address to copy.
 *
 * output parameters
 *
 * @param       pDest        - Pointer to address of copy.
 *
 * @return      pDest + SADDR_EXT_LEN.
 */
void *sAddrExtCpy(uint8 * pDest, const uint8 * pSrc)
{
  return memcpy(pDest, pSrc, SADDR_EXT_LEN);
}


/****************************************************************************
 * @fn          sAddrCpy
 *
 * @brief       Copy a device address.
 *
 * input parameters
 *
 * @param       pSrc         - Pointer to address to copy.
 *
 * output parameters
 *
 * @param       pDest        - Pointer to address of copy.
 *
 * @return      None.
 */
void sAddrCpy(sAddr_t *pDest, const sAddr_t *pSrc)
{
  pDest->addrMode = pSrc->addrMode;

  if (pDest->addrMode == SADDR_MODE_EXT)
  {
    sAddrExtCpy(pDest->addr.extAddr, pSrc->addr.extAddr);
  }
  else
  {
    pDest->addr.shortAddr = pSrc->addr.shortAddr;
  }
}

/****************************************************************************
 * @fn          sAddrExtCmp
 *
 * @brief       Compare two extended addresses.
 *
 * input parameters
 *
 * @param       pAddr1        - Pointer to first address.
 * @param       pAddr2        - Pointer to second address.
 *
 * output parameters
 *
 * @return      TRUE if addresses are equal, FALSE otherwise
 */
int sAddrExtCmp(const uint8 * pAddr1, const uint8 * pAddr2)
{
  uint8 i;

  for (i = SADDR_EXT_LEN; i != 0; i--)
  {
    if (*pAddr1++ != *pAddr2++)
    {
      return FALSE;
    }
  }
  return TRUE;
}



/*********************************************************************
 * @fn      ZDAppForAutoConfig()
 *
 * @brief   Start auto config mode for shzn 
 *
 * @param   none
 *
 * @return  none
 */
uint16 zb_CalcPanId(unsigned int * pAddr )
{
    uint16 usId;

    usId = pAddr[0] + pAddr[1]+ pAddr[2]+ pAddr[3];
    usId += pAddr[4] + pAddr[5]+ pAddr[6]+ pAddr[7];

    usId ^= ((pAddr[0] << 8) | pAddr[1]);

    return usId;

}

/*********************************************************************
 * @fn      ZDAppForAutoConfig()
 *
 * @brief   Start auto config mode for shzn 
 *
 * @param   none
 *
 * @return  none
 */
uint16 zb_CalcRfId(unsigned int * pAddr )
{
    uint16 usId;

    usId = pAddr[0] + pAddr[1]+ pAddr[2]+ pAddr[3];
    usId += pAddr[4] + pAddr[5]+ pAddr[6]+ pAddr[7];

    usId ^= ((pAddr[0] << 8) | pAddr[1]);

    return usId;

}


unsigned char *zbStr2Addr( char * strAddr )
{
  int       i;
  static unsigned char hex[Z_EXTADDR_LEN];
  char   *pStr = strAddr;

  for ( i = 0; i < Z_EXTADDR_LEN; i++ )
  {
     hex[i] = helper_hex2bin(strAddr[2*i + 0],strAddr[2*i + 1]);
  }

  *pStr = 0;

  return hex;
}


void zb_Ieee2key(void)
{
    unsigned int auiValue[8];
    
    sscanf(gCoo.szIeeeAdr, "%02x%02x%02x%02x%02x%02x%02x%02x",&auiValue[0],&auiValue[1],&auiValue[2],&auiValue[3],&auiValue[4],&auiValue[5],&auiValue[6],&auiValue[7]);


    gCoo.ulKey  = (auiValue[0] << 24)|(auiValue[1] << 16)|(auiValue[2] << 8)|(auiValue[3] << 0);
    gCoo.ulKey ^= (auiValue[4] << 24)|(auiValue[5] << 16)|(auiValue[6] << 8)|(auiValue[7] << 0);
    gCoo.iCalcedPanId = zb_CalcPanId(auiValue);

    gCoo.iCalcedRfId  = (gCoo.iCalcedPanId % (RF_MAX_CHL_NUMBER - 1)) + 1;

    ZB_DP("iCalcedPanId = %d,iCalcedRfId = %d\r\n",gCoo.iCalcedPanId,gCoo.iCalcedRfId);
}


void zb_addNodes(Node_t *pNode)
{
    int iLoop;
    for (iLoop = 0; iLoop < gCoo.iNodes; iLoop++)
    {
        if (sAddrExtCmp(pNode->extAddr,gCoo.aNode[iLoop].extAddr))
        {
            break;
        }
    }

    if (iLoop < gCoo.iNodes)
    {
        gCoo.aNode[iLoop].nwkAddr      = pNode->nwkAddr;
        gCoo.aNode[iLoop].shortAddr    = pNode->shortAddr;
        gCoo.aNode[iLoop].capabilities = pNode->capabilities;
    }
    else if (gCoo.iNodes < Z_MAX_NODES)
    {
        gCoo.aNode[gCoo.iNodes].nwkAddr      = pNode->nwkAddr;
        gCoo.aNode[gCoo.iNodes].shortAddr    = pNode->shortAddr;
        gCoo.aNode[gCoo.iNodes].capabilities = pNode->capabilities;
        sAddrExtCpy(gCoo.aNode[gCoo.iNodes].extAddr,pNode->extAddr);
        gCoo.iNodes++;
    }
    else
    {
        return;
    }

    main_printhex(pNode->extAddr,Z_EXTADDR_LEN);

    ZB_DP("add node %d:%d\r\n",pNode->shortAddr,pNode->nwkAddr);
}

int zb_search_Nodes_by_short_addr(int iShortAddr)
{
    int iLoop;
    for (iLoop = 0; iLoop < gCoo.iNodes; iLoop++)
    {
        if (gCoo.aNode[iLoop].shortAddr == iShortAddr)
        {
            return iLoop;
        }
    }

    return Z_MAX_NODES;
}

int zb_search_Nodes_by_long_addr(ZLongAddr_t aLongAddr)
{
    int iLoop;
    for (iLoop = 0; iLoop < gCoo.iNodes; iLoop++)
    {
        if (sAddrExtCmp(gCoo.aNode[iLoop].extAddr,aLongAddr))
        {
            return iLoop;
        }
    }

    return Z_MAX_NODES;
}


void zb_Remove_Nodes_by_long_addr(ZLongAddr_t aLongAddr)
{
    int iLoop;
    int iNodeIdex = Z_MAX_NODES;
    for (iLoop = 0; iLoop < gCoo.iNodes; iLoop++)
    {
        if (sAddrExtCmp(gCoo.aNode[iLoop].extAddr,aLongAddr))
        {
            iNodeIdex = iLoop;
            break;
        }
    }

    if (iNodeIdex != Z_MAX_NODES)
    {

        for (iLoop = iNodeIdex; iLoop < gCoo.iNodes - 1; iLoop++)
        {
            memcpy(&gCoo.aNode[iLoop],&gCoo.aNode[iLoop + 1],sizeof(Node_t));

        }

        gCoo.iNodes--;
    }

}


int zb_Remove_Nodes_by_short_addr(int iShortAddr)
{
    int iLoop;
    int iNodeIdex = Z_MAX_NODES;
    
    for (iLoop = 0; iLoop < gCoo.iNodes; iLoop++)
    {
        if (gCoo.aNode[iLoop].shortAddr == iShortAddr)
        {
            iNodeIdex = iLoop;
            break;
        }
    }

    if (iNodeIdex != Z_MAX_NODES)
    {

        for (iLoop = iNodeIdex; iLoop < gCoo.iNodes - 1; iLoop++)
        {
            memcpy(&gCoo.aNode[iLoop],&gCoo.aNode[iLoop + 1],sizeof(Node_t));

        }

        gCoo.iNodes--;
    }

    return iNodeIdex;
}


void zb_node_RptMsg(Node_t *pNode)
{
    // send message to main
    int ret;

    TASK_HANDLE task = TASK_HANDLE_MAIN;

    ZBITF_MAIN_MSG_STRU *pCanItfMsg = NULL;

    
	pCanItfMsg = (ZBITF_MAIN_MSG_STRU *)SatAllocMsg(ZB_MAIN_MSG_SIZE + sizeof(Node_t));//malloc(sizeof(MAIN_CANITF_MSG_STRU) + CanRcvBuff[ucRcvBufIndex].len);
	if (NULL == pCanItfMsg)
	{
		return ;
	}
    pCanItfMsg->msgHead.event  = ZBMAIN_NODE_REPORT_MSG;
    pCanItfMsg->usShortAdr     = pNode->shortAddr;
    pCanItfMsg->usMsgLen       = sizeof(Node_t);


    memcpy(pCanItfMsg->aucData,pNode,pCanItfMsg->usMsgLen);

    ret = VOS_SndMsg(task,(SAT_MSG_HEAD *)pCanItfMsg);

    if (0 != ret)
    {
    }
}

int zbSndMsg2SerialItf(char *data,int length)
{
    ZB_SERIALITF_STRU *pCanCmd = (ZB_SERIALITF_STRU *)SatAllocMsg(ZB_SERIALITF_MSG_SIZE + length);
    unsigned char *pData;
    int ret;
    
    if (!pCanCmd)
    {
        return ERROR_CODE_MEM_ALLOC_FAIL;
    }

    pCanCmd->msgHead.event = ZB_SERIALITF_MSG;

    pData = (unsigned char *)pCanCmd->data;
    
    memcpy(&pData[0],data,length);

    ret = VOS_SndMsg(TASK_HANDLE_SERIALITF4ZB,(SAT_MSG_HEAD *)pCanCmd);

    if (0 != ret)
    {
    }

    return ret;
}


int zbPrepareBin(DATAPROC_ZB_UPD_STRU *zbUpd)
{
    Iap.m_nFileSize          = zbUpd->iLength;
    Iap.m_nCurrentPacketSize = 0;

    ZB_DP("read coo.bin succ");
    
    return 0;
    
}


void zbStartDownload(SAT_MSG_HEAD *pMsg)
{
    DATAPROC_ZB_UPD_STRU *zbUpd = (DATAPROC_ZB_UPD_STRU *)pMsg;

    if (IAP_PROC_BUTT != Iap.m_nProc)
    {
        return;
    }
    
    if (0 == zbPrepareBin(zbUpd))
    {
        ZB_DP("zbStartDownload");
        
        if (!PrepareForceBoot())
        {
            Iap.m_nProc = IAP_PROC_DOWNLOAD;

            zbSndUpdResult(0,0);

            return ;
        }
    }
    
    zbSndUpdResult(1,0);

}

void zbHandShake(void)
{
    ZB_DP("%s",__FUNCTION__);

    MakeHandShake();
    
    WriteOutputReport(Sendbuf[RPC_POS_LEN] + RPC_FRAME_HDR_SZ,10);

}

void zbFileSize(void)
{
    ZB_DP("%s",__FUNCTION__);

    MakeFileSizeCmd();
    
    WriteOutputReport(Sendbuf[RPC_POS_LEN] + RPC_FRAME_HDR_SZ,0);

}

void zbProcSerialItfMsg(SAT_MSG_HEAD *pMsg)
{
    ZB_SERIALITF_STRU *pCanCmd = (ZB_SERIALITF_STRU *)pMsg;

    ProtolProc(pCanCmd->data);
}

void zbProcMainMsg(SAT_MSG_HEAD *pMsg)
{

    MAIN_ZBITF_MSG_STRU *pCanCmd = (MAIN_ZBITF_MSG_STRU *)pMsg;


    memcpy(Sendbuf,pCanCmd->aucData,pCanCmd->usMsgLen);

    WriteOutputReport(pCanCmd->usMsgLen,0);

}



void zbSecondTask()
{
    sZbSecond++;

    if (Iap.m_cmdTimes > 0 )
    {
        Iap.m_cmdTimes--;

        if (0 == Iap.m_cmdTimes)
        {
            int cmd0 = HI_UINT16(Iap.m_cmdState);
            int cmd1 = LO_UINT16(Iap.m_cmdState);
            switch(cmd0)
            {
            case RPC_SYS_BOOT:
                switch(cmd1)
                {
                case SBL_HANDSHAKE_CMD:
                    Iap.m_nHandleShakeNum--;
                    if (Iap.m_nHandleShakeNum >= 0)
                    {
                        MakeHandShake();
                        WriteOutputReport(Sendbuf[RPC_POS_LEN] + RPC_FRAME_HDR_SZ,0);
                        return;
                    }
                    break;
                case SBL_FILE_SIZE_CMD:
                    ZB_DP("SBL_FILE_SIZE_CMD timeout");
                    break;
                default:
                    break;
                }
                break;
            }

            if (IAP_PROC_DOWNLOAD == Iap.m_nProc)
            {
                zbSndUpdResult(0X1,0);
            }
            gCoo.iAtCmdErrors++;

            Iap.m_nProc = IAP_PROC_BUTT;

        }
    }

    zb_ResetCmd();   
    
}


void zbProcTimerMsg(SAT_MSG_HEAD *pMsg)
{
    TIMER_MSG_STRU *pTimerMsg = (TIMER_MSG_STRU *)(pMsg);

    switch(pTimerMsg->id)
    {
    case TIMER_SECOND:
        zbSecondTask();
        break;
    default:
        break;
    }
}


void zb_second_handler(void *arg)
{
    int id = (int )arg;
    int ret;
    TIMER_MSG_STRU *tm = (TIMER_MSG_STRU *)SatAllocMsg(TIMER_MSG_LEN);//malloc(sizeof(TIMER_MSG_STRU));
    if (tm)
    {
        tm->id = id;
        tm->msgHead.event = TIMER_MSG;
        //tm->msgHead.len = TIMER_MSG_MSG_SIZE;
        ret = VOS_SndMsg(TASK_HANDLE_ZB,(SAT_MSG_HEAD *)tm);
        if (0 != ret)
        {
        }
    }
}

void zbCmdBgn4At(void *para)
{
    char *atCmd = (char *)para;
    MakeAtCmd(atCmd);
}

void zbCmdBgn4AirAt(void *para)
{
    char *atCmd = (char *)para;
    MakeAirAtCmd(atCmd);
}

void zbCmdBgn4Reset(void *para)
{
    (void)para;
    MakeResetCmd();
}

void zbCmdBgn4ReInit(void *para)
{
    (void)para;
    MakeReInitCmd();
}


void zb_set_coo_panid(int iPanId)
{
    // send an at command to get coo panid
    int iIdx = AllocCmdBuf();

    if (iIdx < MAX_CMD_BUF_NUM)
    {
        CMD_BUF_STRU *pCmd = &saCmdBuf[iIdx];

        if (pCmd)
        {
            sprintf(pCmd->cmdBuf,"AT+CONFIG=%s=%d\n",AT_CONFIG_PANID,iPanId);

            pCmd->cmdId = AT_CONFIG_PANID;
            
            pCmd->para = (void  *)iPanId;

            pCmd->bgn_cb = zbCmdBgn4At;

            zb_AddCmd(pCmd,CMD_LIST_NP);

        }
   }
}



void zb_set_coo_cfg_mode(int iMode)
{
    // send an at command to get coo panid
    int iIdx = AllocCmdBuf();

    if (iIdx < MAX_CMD_BUF_NUM)
    {
        CMD_BUF_STRU *pCmd = &saCmdBuf[iIdx];

        if (pCmd)
        {
            sprintf(pCmd->cmdBuf,"AT+CONFIG=%s=%d\n",AT_CONFIG_APPCFG,iMode);

            pCmd->cmdId = AT_CONFIG_APPCFG;
            
            pCmd->para = (void  *)iMode;

            pCmd->bgn_cb = zbCmdBgn4At;

            zb_AddCmd(pCmd,CMD_LIST_NP);

            ZB_DP("set cfg mode : %d",iMode);;
        }
   }
}


void zb_get_coo_panid(void)
{
    // send an at command to get coo panid
    int iIdx = AllocCmdBuf();
    
    if (iIdx < MAX_CMD_BUF_NUM)
    {
        CMD_BUF_STRU *pCmd = &saCmdBuf[iIdx];

        sprintf(pCmd->cmdBuf,"AT+CONFIG=%s=?\n",AT_CONFIG_PANID);

        pCmd->cmdId = AT_CONFIG_PANID;

        pCmd->bgn_cb = zbCmdBgn4At;

        zb_AddCmd(pCmd,CMD_LIST_NP);

    }
}

void zb_get_coo_ieee(void)
{
    // send an at command to get coo panid
    int iIdx = AllocCmdBuf();
    
    if (iIdx < MAX_CMD_BUF_NUM)
    {
        CMD_BUF_STRU *pCmd = &saCmdBuf[iIdx];

        sprintf(pCmd->cmdBuf,"AT+QUERY=%s=?\n",AT_QUERY_IEEE);

        pCmd->cmdId = AT_QUERY_IEEE;

        pCmd->bgn_cb = zbCmdBgn4At;

        zb_AddCmd(pCmd,CMD_LIST_NP);
    }    
}


void zb_get_device_ieee(int iShortAddr)
{
    // send an at command to get coo panid
    int iIdx = AllocCmdBuf();
    
    if (iIdx < MAX_CMD_BUF_NUM)
    {
        CMD_BUF_STRU *pCmd = &saCmdBuf[iIdx];

        //sprintf(pCmd->cmdBuf,"AT+QUERY=%s%d=?\n",AT_QUERY_SUBDEV_IEEE,iShortAddr);
        sprintf(pCmd->cmdBuf,"AT+QUERY=%s%d=?\n",AT_QUERY_SUBDEV_SHORT,iShortAddr);
        
        pCmd->cmdId = AT_QUERY_SUBDEV_SHORT;

        pCmd->bgn_cb = zbCmdBgn4At;

        zb_AddCmd(pCmd,CMD_LIST_NP);
    }    
}


void zb_get_coo_cfg_mode(void)
{
    // send an at command to get coo panid
    int iIdx = AllocCmdBuf();
    
    if (iIdx < MAX_CMD_BUF_NUM)
    {
        CMD_BUF_STRU *pCmd = &saCmdBuf[iIdx];

        sprintf(pCmd->cmdBuf,"AT+CONFIG=%s=?\n",AT_CONFIG_APPCFG);

        pCmd->cmdId = AT_CONFIG_APPCFG;

        pCmd->bgn_cb = zbCmdBgn4At;

        zb_AddCmd(pCmd,CMD_LIST_NP);
    }    
}

void zb_coo_list(void)
{
    // send an at command to get coo panid
    int iIdx = AllocCmdBuf();
    
    if (iIdx < MAX_CMD_BUF_NUM)
    {
        CMD_BUF_STRU *pCmd = &saCmdBuf[iIdx];

        sprintf(pCmd->cmdBuf,"AT+QUERY=LIST=?");

        pCmd->cmdId = AT_QUERY_LIST;

        pCmd->bgn_cb = zbCmdBgn4At;

        zb_AddCmd(pCmd,CMD_LIST_NP);

    }
}


void zb_reset_coo(void)
{
    // send an at command to get coo panid
    int iIdx = AllocCmdBuf();
    
    if (iIdx < MAX_CMD_BUF_NUM)
    {
        CMD_BUF_STRU *pCmd = &saCmdBuf[iIdx];

        pCmd->cmdId = NULL;

        pCmd->bgn_cb = zbCmdBgn4Reset;

        zb_AddCmd(pCmd,CMD_LIST_NP);
    }    
}

void zb_reInit_coo(void)
{
    // send an at command to get coo panid
    int iIdx = AllocCmdBuf();
    
    if (iIdx < MAX_CMD_BUF_NUM)
    {
        CMD_BUF_STRU *pCmd = &saCmdBuf[iIdx];

        pCmd->cmdId = NULL;

        pCmd->bgn_cb = zbCmdBgn4ReInit;

        zb_AddCmd(pCmd,CMD_LIST_NP);
    }    
}


static void zbRcvFrame(unsigned char *pData, int iLen)
{
    // send message to main
    int ret;

    TASK_HANDLE task = TASK_HANDLE_MAIN;

    ZBITF_MAIN_MSG_STRU *pCanItfMsg = NULL;

    
	pCanItfMsg = (ZBITF_MAIN_MSG_STRU *)SatAllocMsg(ZB_MAIN_MSG_SIZE + iLen);//malloc(sizeof(MAIN_CANITF_MSG_STRU) + CanRcvBuff[ucRcvBufIndex].len);
	if (NULL == pCanItfMsg)
	{
		return ;
	}
    pCanItfMsg->msgHead.event  = ZBITF_MAIN_MSG;
    pCanItfMsg->usShortAdr     = BUILD_UINT16(pData[0],pData[1]);
    pCanItfMsg->usMsgLen       = iLen - 2;

    if (zb_search_Nodes_by_short_addr(pCanItfMsg->usShortAdr) >= Z_MAX_NODES)
    {
        /* issue */
        zb_get_device_ieee(pCanItfMsg->usShortAdr) ;

        ZB_DP("unknow shortaddr : %d",pCanItfMsg->usShortAdr);
    }
    

    pData += 2;

    iLen -= 2 ;

    memcpy(pCanItfMsg->aucData,pData,iLen);

    ret = VOS_SndMsg(task,(SAT_MSG_HEAD *)pCanItfMsg);

    if (0 != ret)
    {
    }
}

static void zbMainSndMtnMsg(unsigned short usSubCode,unsigned char *pData, int iLen)
{
    // send message to main
    int ret;

    TASK_HANDLE task = TASK_HANDLE_MAIN;

    ZBITF_MAIN_MTN_MSG_STRU *pMsg = NULL;

	pMsg = (ZBITF_MAIN_MTN_MSG_STRU *)SatAllocMsg(ZBITF_MAIN_MTN_MSG_SIZE + iLen);//malloc(sizeof(MAIN_CANITF_MSG_STRU) + CanRcvBuff[ucRcvBufIndex].len);
	if (NULL == pMsg)
	{
		return ;
	}
    pMsg->msgHead.event  = ZBITF_MAIN_MTN_MSG;
    pMsg->usSubCode     = usSubCode;

    memcpy(pMsg->aucData,pData,iLen);

    ret = VOS_SndMsg(task,(SAT_MSG_HEAD *)pMsg);

    if (0 != ret)
    {
    }
}


void ProtolProc(char *Revbuf)
{

    // int nMsgLen = Revbuf[0] + RPC_FRAME_HDR_SZ; // 
    // ZB_DP("ProtolProc");

    switch ((Revbuf[RPC_POS_CMD0] & RPC_SUBSYSTEM_MASK))
    {
    case RPC_SYS_APP:
        if (Revbuf[RPC_POS_CMD1] & SAPP_RSP_MASK)
        {
        
            Iap.m_cmdTimes = 0;
            
            // handle response
            switch(Revbuf[RPC_POS_CMD1] & (~SAPP_RSP_MASK))
            {
            case SAPP_CMD_SYS_INIT:
                if (Revbuf[RPC_POS_DAT0])
                {
                    ZB_DP("init failed!");
                }
                else
                {
                    ZB_DP("init succeed!");
                }
                break;
            case SAPP_CMD_RESET:
                {
                    ZBITF_MAIN_MTN_RESET_STRU reset;
                    if (Revbuf[RPC_POS_DAT0])
                    {
                        ZB_DP("RESET failed!");
                    }
                    else
                    {
                        ZB_DP("RESET succeed!");
    
                        /* Notify Main Reset succeed */
                        gCoo.bit1Inited = TRUE;
                    }

                    //sleep(5);

                    //reset.state = Revbuf[RPC_POS_DAT0];
    
                    //zbMainSndMtnMsg(ZBITF_MAIN_MTN_CODE_RESET,(unsigned char *)&reset,sizeof(reset));
                }
                break;
            case SAPP_CMD_AT:
                if (0 == Revbuf[SBL_RSP_STATUS])
                {
                    int len = Revbuf[RPC_POS_LEN] - 1; 
                    char *atrsp = (char *)&Revbuf[SBL_RSP_STATUS+1];
            
                    if (len > 0 ) // if have additional data
                    {
                        char *parse;
                        
                        atrsp[len] = 0;

                        ZB_DP("at Rsp : %s",atrsp);;

                        // split 
                        parse = strchr(atrsp,':');

                        if (parse) 
                        {
                            *parse++ = 0;

                            if (0 == strcmp(atrsp,AT_CONFIG_PANID))
                            {
                                int iPanId = atoi(parse);

                                gCoo.iPanId = iPanId;

                            }
                            else if (0 == strcmp(atrsp,AT_QUERY_IEEE))
                            {
                                snprintf(gCoo.szIeeeAdr,16,"%s",parse);

                                zb_Ieee2key();
                            }
                            else if (0 == strcmp(atrsp,AT_CONFIG_APPCFG))
                            {
                                gCoo.iAppCfg = atoi(parse);

                                if (gCoo.iAppCfg != gCoo.iNetCfgMode)
                                {
                                    /* move to */
                                    zb_set_coo_cfg_mode(gCoo.iNetCfgMode);

                                    /* ensure an reset after cfg mode changed */
                                    gCoo.bit1Inited = 0;
                                }
                                else
                                {
                                   if (!gCoo.bit1Inited)
                                   {
                                       gCoo.bit1Inited = TRUE;
                                       zb_reset_coo();
                                   }
                                }
                                
                            }
                            else if (0 == strncmp(atrsp,AT_QUERY_SUBDEV_IEEE,6))
                            {
                                int iNwAddr = atoi(atrsp + 6);
                                {
                                    Node_t node;
            
                                    node.shortAddr = iNwAddr;
                                    node.nwkAddr   = iNwAddr;
            
                                    sAddrExtCpy(node.extAddr,zbStr2Addr(parse));
            
                                    node.capabilities = 0;
                                 
                                    zb_addNodes(&node);
            
                                    /* report to user */
                                    zb_node_RptMsg(&node);
                                    
                                }                                
                            }
                            else if (0 == strcmp(atrsp,AT_QUERY_LIST))
                            {
                                 /* do something here */
                            }

                            sprintf(gCoo.dataBuf,"%s",parse);
                        }
                        
                    }
                    else
                    {

                        if (0 == strcmp(gCoo.szCmdId,AT_CONFIG_APPCFG))
                        {

                            gCoo.iAppCfg = (int)gCoo.cmdData;
                            
                            zb_reset_coo();
                        }

                        sprintf(gCoo.dataBuf,"%s","OK");
                    }
                
                }   
                else
                {                                
                    ZB_DP("at cmd failed %s!",gCoo.szCmdId);
                                        
                    sprintf(gCoo.dataBuf,"%s","ERROR");
                }

                gCoo.atResultCode = Revbuf[SBL_RSP_STATUS];

                gCoo.iAtCmdErrors = 0;

                zb_RmvCmdHeader();

                break;
            case SAPP_CMD_AIR_AT:
                {
                    ZB_DP("air at cmd accepted!");
                }
                break;
            case SAPP_CMD_AIR_RSP:
                {
                    /* decode addr type */
                    unsigned char *pData = (unsigned char *)&Revbuf[RPC_POS_DAT0];
                    int len              = Revbuf[RPC_POS_LEN]; 
                    int adrMode          = *pData;
                    int iMatch           = FALSE;
                    
                    ZB_DP("air at cmd rsp : adrMode = %d : %d\r\n",adrMode,len);

                    main_printhex(pData,len);

                    pData++; /* skip address mode */

                    switch(adrMode)
                    {
                    case afAddr16Bit:
                        {
                            int address = (pData[0] << 8)|(pData[1] << 0);
                            pData += 2;

                            if (zb_search_Nodes_by_short_addr(address) < Z_MAX_NODES)
                            {
                                iMatch = TRUE;
                            }

                            ZB_DP("air at cmd rsp : address = %d \r\n",address);

                            gCoo.atResultCode = *pData;
                            
                        }
                        break;
                    case afAddr64Bit:
                        {
                            ZLongAddr_t laddr;

                            memcpy(laddr,pData,Z_EXTADDR_LEN);
                            pData += Z_EXTADDR_LEN;

                            if (zb_search_Nodes_by_long_addr(laddr) < Z_MAX_NODES)
                            {
                                iMatch = TRUE;
                            }

                            gCoo.atResultCode = *pData;
                            
                            main_printhex(laddr,Z_EXTADDR_LEN);
                        }
                        break;
                    }

                    if (iMatch) gCoo.iCfgNodeRspNum++;
   
                    if (gCoo.iCfgNodeRspNum >= gCoo.iNodes)
                    {
                        zb_RmvCmdHeader();
                    }

                    ZB_DP("air at cmd rsp!");
                }                
                break;
            case SAPP_CMD_CFG:
                {
                    if (0 == Revbuf[SBL_RSP_STATUS])
                    {
                        ZB_DP("wl query success!");

                    }
                }
                break;
            case SAPP_CMD_DATA:
                if (0 == Revbuf[SBL_RSP_STATUS])
                {
                    ZB_DP("data send succeed!");;
                }
                else
                {
                    ZB_DP("data send failed!");;
                }
                break;
            default:
                break;
            }
        }
        else
        {
            // handle requeset
            switch(Revbuf[RPC_POS_CMD1] )
            {
            case SAPP_CMD_DATA:
                {
                    ZB_DP("data received succeed!");

                    /* frame handle */
                    zbRcvFrame((unsigned char *)&Revbuf[RPC_POS_DAT0],Revbuf[RPC_POS_LEN]);
                
                }
                break;
            case SAPP_CMD_USER:
                {
                    ZB_DP("user received succeed!");

                }
                break;
            case SAPP_CMD_DBG_INFO:
                {
                    // print string to 
                    int len = Revbuf[RPC_POS_LEN]; 
                    char *atrsp = (char *)&Revbuf[RPC_POS_DAT0];
            
                    if (len > 0 )
                    {
                        atrsp[len] = 0;
            
                        ZB_DP("%s",atrsp);
                    }
                    
                }
                break;
            default:
                break;
            }
        }
        break;
    case RPC_SYS_BOOT:
        if (Revbuf[RPC_POS_CMD1] & SAPP_RSP_MASK)
        {
        
            Iap.m_cmdTimes = 0;
            // handle response
            switch(Revbuf[RPC_POS_CMD1] & (~SAPP_RSP_MASK))
            {
            case SBL_QUERY_ID_CMD:
                if (0 == Revbuf[RPC_POS_DAT0] )
                {

                    ZB_DP("query id succeed!");;
                }
                else
                {
                    ZB_DP("query id failed!");;
                }
                
                break;
            case SBL_QUERY_VERSION_CMD:
                if (0 == Revbuf[RPC_POS_DAT0] )
                {
                    int len = Revbuf[SBL_RSP_STATUS+1] ; 
                    char *atrsp = (char *)&Revbuf[SBL_RSP_STATUS + 2];
            
                    if (len > 0 )
                    {
                        atrsp[len] = 0;
            
            
                        ZB_DP("%s",atrsp);
                    }

                    ZB_DP("query version succeed!");;
                }
                else
                {
                    ZB_DP("query version failed!");;
                }

                break;
            case SBL_SET_ADDR_CMD:
                if (Revbuf[RPC_POS_DAT0])
                {
                    ZB_DP("set addr failed!");;
                }
                else
                {
                    ZB_DP("set addr succeed!");

                }               
                break;
            case SBL_TGT_BOOTLOAD:
                {
                    
                    if (Revbuf[RPC_POS_DAT0])
                    {
                        ZB_DP("force boot failed!");

                    
                         if (IAP_PROC_DOWNLOAD == Iap.m_nProc)
                         {
                             zbSndUpdResult(0X1,0);
                             Iap.m_nProc = IAP_PROC_BUTT;
                         }
                        
                    }
                    else
                    {
                        ZB_DP("force boot succeed!");

                        // delay a moment for target to reboot
                        sleep(1);

                        switch (Iap.m_nProc)
                        {
                        case IAP_PROC_FORCEBOOT:
                            Iap.m_nProc = IAP_PROC_BUTT;
                            break;
                        case IAP_PROC_DOWNLOAD:
                            Iap.m_nInDownLoading   = 1;
                            Iap.m_nHandleShakeNum  = 10;
                            Iap.m_nDownLoadPercent = 0;
                            MakeHandShake();
                            WriteOutputReport(Sendbuf[RPC_POS_LEN] + RPC_FRAME_HDR_SZ,0);
                            break;
                        }
                    }   
                }
                break;
            case SBL_HANDSHAKE_CMD:
                if (0 == Revbuf[RPC_POS_DAT0])
                {
                    ZB_DP("hand shake succeed!");;
                
                    Iap.m_nHandleShakeNum = 0; // no any more hand shake needed
                    
                    if (Iap.m_nInDownLoading)
                    {
                       MakeFileSizeCmd() ;

                       WriteOutputReport(Sendbuf[RPC_POS_LEN] + RPC_FRAME_HDR_SZ,0);

                    }
                    else if (Iap.m_nInVerifying)
                    {
                        Iap.m_nCurrentPacketSize = 0;
                        MakeReadCmd();    

                        WriteOutputReport(Sendbuf[RPC_POS_LEN] + RPC_FRAME_HDR_SZ,0);
                        Iap.m_nCurrentPacketSize += SBL_RW_BUF_LEN;                             
                    }
                    
                }
                else
                {
                    ZB_DP("hand shake failed!");;

                    if (Iap.m_nInDownLoading)
                    {
                        Iap.m_nInDownLoading = 0;
                        
                        zbSndUpdResult(0X1,0);
                    }

                    if (Iap.m_nInVerifying)
                    {
                        Iap.m_nInVerifying = 0;
                    }
                    
                    
                    Iap.m_nProc = IAP_PROC_BUTT;

                }
                break;
            case SBL_FILE_SIZE_CMD:
                if (1)//(0 == Revbuf[RPC_POS_DAT0])
                {
                    if (Iap.m_nInDownLoading)
                    {
                        
                        Iap.m_nCurrentPacketSize = 0;
                        {
                            MakeWriteCmd();
                        
                            WriteOutputReport(Sendbuf[RPC_POS_LEN] + RPC_FRAME_HDR_SZ,0);

                            Iap.m_nCurrentPacketSize += SBL_RW_BUF_LEN;
                            
                        }
                    }
                
                    ZB_DP("file size succeed!");;
                }
                else
                {
                    ZB_DP("file size failed!");
                    
                    Iap.m_nProc = IAP_PROC_BUTT;
                }               
                break;  
            case SBL_WRITE_CMD:
                if (0 == Revbuf[RPC_POS_DAT0])
                {
                    if (Iap.m_nInDownLoading)
                    {
                        if (Iap.m_nCurrentPacketSize < Iap.m_nFileSize)
                        {
                            MakeWriteCmd();
                        
                            WriteOutputReport(Sendbuf[RPC_POS_LEN] + RPC_FRAME_HDR_SZ ,0);

                            Iap.m_nCurrentPacketSize += SBL_RW_BUF_LEN;

                            if (Iap.m_nDownLoadPercent != (Iap.m_nCurrentPacketSize*100)/Iap.m_nFileSize)
                            {
                                Iap.m_nDownLoadPercent = (Iap.m_nCurrentPacketSize*100)/Iap.m_nFileSize;
                                zbSndUpdResult(0,Iap.m_nDownLoadPercent);
                            }
                        }
                        else
                        {
                             if (Iap.m_bDownloadVerify)
                             {
                                Iap.m_nCurrentPacketSize = 0;
                                MakeReadCmd();    
                                WriteOutputReport(Sendbuf[RPC_POS_LEN] + RPC_FRAME_HDR_SZ,0);
                                Iap.m_nCurrentPacketSize += SBL_RW_BUF_LEN;                             
                            }
                            else
                            {
                                MakeEnableCmd();
                                
                                WriteOutputReport(Sendbuf[RPC_POS_LEN] + RPC_FRAME_HDR_SZ,0);
                                
                            }
                        }
                    }
                }
                else
                {           
                    if (Iap.m_nInDownLoading)
                    {
                        Iap.m_nInDownLoading = 0;
                        zbSndUpdResult(0X1,0);
                    }               

                    Iap.m_nProc = IAP_PROC_BUTT;
                }
                break;
            case SBL_READ_CMD:
                if (0 == Revbuf[RPC_POS_DAT0])
                {
                    // check data
                    int bFail = 0;
                    if (Iap.m_nCurrentPacketSize == SBL_RW_BUF_LEN)
                    {
                        if (memcmp(&Revbuf[SBL_RSP_DAT0+4],&gFileMem[Iap.m_nCurrentPacketSize-SBL_RW_BUF_LEN+4],SBL_RW_BUF_LEN-4))
                        {
                            ZB_DP("verify fail!");
                            bFail = 1;
                        
                        }   
                    }
                    else if (memcmp(&Revbuf[SBL_RSP_DAT0],&gFileMem[Iap.m_nCurrentPacketSize-SBL_RW_BUF_LEN],SBL_RW_BUF_LEN))
                    {
                        ZB_DP("verify fail!");
                        
                        bFail = 1;
                    }   

                    if (!bFail)
                    {
                        if (Iap.m_nCurrentPacketSize < Iap.m_nFileSize)
                        {
                            MakeReadCmd();                               
                            WriteOutputReport(Sendbuf[RPC_POS_LEN] + RPC_FRAME_HDR_SZ,0);
                            Iap.m_nCurrentPacketSize += SBL_RW_BUF_LEN;                             
                        }
                        else
                        {   
                            if (Iap.m_nInDownLoading)
                            {
                                MakeEnableCmd();
                                
                                WriteOutputReport(Sendbuf[RPC_POS_LEN] + RPC_FRAME_HDR_SZ,0);
                                
                            }
                            else
                            {
                                if (Iap.m_nInVerifying)
                                {
                                    Iap.m_nInVerifying = 0;

                                    Iap.m_nProc = IAP_PROC_BUTT;
                                }
                            }
                        }
                    }
                    else
                    {
                        Iap.m_nInVerifying = 0;

                        if (Iap.m_nInDownLoading)
                        {
                            Iap.m_nInDownLoading = 0;
                            zbSndUpdResult(0X1,0);
                        }
                    
                        ZB_DP("verify failed");

                        Iap.m_nProc = IAP_PROC_BUTT;
                    
                    }
                }
                else
                {
                    if (Iap.m_nInDownLoading)
                    {
                        ZB_DP("download failed");
                        Iap.m_nInDownLoading = 0;

                        zbSndUpdResult(0X1,0);
                        
                    }
                    else if (Iap.m_nInVerifying)
                    {
                        ZB_DP("verifing failed");
                        Iap.m_nInVerifying = 0;
                    }   

                    Iap.m_nProc = IAP_PROC_BUTT;
                }
                break;
            case SBL_ENABLE_CMD:
                if (0 == Revbuf[RPC_POS_DAT0])
                {
                    if (Iap.m_nInDownLoading)
                    {
                        Iap.m_nInDownLoading = 0;
                        Iap.m_nCurrentPacketSize = 0;
                        ZB_DP("download succeed");

                        // notify client ,if any
                        zbSndUpdResult(0,100);

                        Iap.m_nProc = IAP_PROC_BUTT;

                        sleep(1);

                        /* Reinit */
                        zb_reInit_coo();

                        // get coo panid 
                        zb_get_coo_panid();
                        
                        zb_get_coo_ieee();
                        
                        zb_get_coo_cfg_mode();

                    }
                    ZB_DP("enable succeed!");;
                    
                }
                else
                {
                    if (Iap.m_nInDownLoading)
                    {
                       Iap.m_nInDownLoading = 0;
                       ZB_DP("download failed");

                       Iap.m_nProc = IAP_PROC_BUTT;

                        // notify client ,if any
                        zbSndUpdResult(0X1,0);

                    }
                    ZB_DP("enable failed!");;
                }
                break;
            default:
                break;
            }
            
        }
        else
        {
            // handle requeset
            switch(Revbuf[RPC_POS_CMD1])
            {
            case SBL_SET_BOOTP_CMD:

                break;
            case SBL_WRITE_CMD:
                if (Iap.m_nThroughputMode)
                {
                    // send test packet
                    Iap.m_nCurrentPacketSize = 0;

                    gFileMem[0]++;
                    
                    MakeWriteCmd();
                    
                    WriteOutputReport(Sendbuf[RPC_POS_LEN] + RPC_FRAME_HDR_SZ,0);
                }
                break;
            default:
                break;
            }
        }
        
        break;
    case RPC_SYS_SAPI:
        {
            if (0 == (Revbuf[RPC_POS_CMD1] & SAPP_RSP_MASK))
            {
            
                switch(Revbuf[RPC_POS_CMD1] )
                {
                case SAPI_CMD_DATA:
                    /* construct user list */
                    {
                        Node_t node;

                        uint8 *pucData = (uint8 *)&Revbuf[RPC_POS_DAT0];
                        
                        node.shortAddr = BUILD_UINT16(pucData[0],pucData[1]);
                        pucData += 2;
                        node.nwkAddr = BUILD_UINT16(pucData[0],pucData[1]);
                        pucData += 2;

                        sAddrExtCpy(node.extAddr,pucData);
                        pucData += Z_EXTADDR_LEN;

                        node.capabilities = pucData[0];
                     
                        zb_addNodes(&node);

                        /* report to user */
                        zb_node_RptMsg(&node);
                        
                    }
                    break;
                case SAPI_CMD_DEV_STATE:
                    {
                        uint8 *pucData = (uint8 *)&Revbuf[RPC_POS_DAT0];

                        if (DEV_ZB_COORD == pucData[0])
                        {
                            /* Do something here */
                            
                            ZB_DP("COO Ready");
                        }
                    }
                    break;
                }                
            }            
        }
        break;
    default:
        break;
    }

}


void zbInitInnerIpc(void)
{
    sp_thread_mutex_init(&gZbIpc.mutex,NULL);
    sp_thread_cond_init( &gZbIpc.cond, NULL );
}


void zbDeinitInnerIpc(void)
{
    sp_thread_mutex_destroy(&gZbIpc.mutex);
    sp_thread_cond_destroy( &gZbIpc.cond );
}


void zbInit(void)
{
   int ret;

    sulCmdBufMask = 0;

    memset(&Iap,0,sizeof(Iap));

    Iap.m_nProc  = IAP_PROC_BUTT;
    Iap.m_nState = IAP_STATE_BUTT;

    memset(&gCoo,0,sizeof(gCoo));

#if (TESTER == 0)
    gCoo.iNetCfgMode = 0;
#else
    gCoo.iNetCfgMode = 1;
#endif

    /* current forced common mode */
    gCoo.iNetCfgMode = 1;

    ZB_DP("cfg mode : %d",gCoo.iNetCfgMode);;

    memset(saCmdBuf,0,sizeof(saCmdBuf));

    for (ret = 0; ret < CMD_LIST_NUM; ret++)
    {
        INIT_LIST_HEAD(&gCoo.lstCmd[ret]);
    }

    zbInitInnerIpc();


    ret = zbSerialInit(&gzbSerialItfStru.fd,SERIAL_FILE);

    if (ret)
    {
        ZB_DP("init SERIAL_FILE failed!");
    }

    TASK_HANDLE_SERIALITF4ZB = Task_CreateMessageTask(zbSerialItfMsgProc,NULL,NULL,&gzbSerialItfStru);

    Task_DispatchWorkTask(zbSerialItfRcvProc,&gzbSerialItfStru);

    gzbSerialItfStru.thd4tx = 1;
    gzbSerialItfStru.thd4rx = 1;

    // start period check timer
    gzbTo = timer_add(1000,TIMER_PERIOD,zb_second_handler,(void *)TIMER_SECOND);

    VOS_SndMsg2(TASK_HANDLE_SERIALITF4ZB,INIT_ALL_THREAD_EVENT,0,NULL);

    zb_get_coo_ieee();

    // ensure zed to report
    zb_reset_coo();

    // get coo panid 
    zb_get_coo_panid();

    zb_get_coo_cfg_mode();

}

void zbDeInit(void)
{

}



void zbDataProcMsgProc(SAT_MSG_HEAD *pMsg)
{
    DATAPROC_ZB_STRU *pZbMsg = (DATAPROC_ZB_STRU *)pMsg;

     int iIdx = AllocCmdBuf();

    
     if (iIdx < MAX_CMD_BUF_NUM)
     {
         CMD_BUF_STRU *pCmd = &saCmdBuf[iIdx];
    
         if (pCmd)
         {
             sprintf(pCmd->cmdBuf,pZbMsg->data);
    
             pCmd->cmdId = AT_CONFIG_PANID;
             
             pCmd->para = pZbMsg->cbPara;

             pCmd->end_cb   = pZbMsg->cbFunc;;

             pCmd->bgn_cb = zbCmdBgn4At;
    
             zb_AddCmd(pCmd,CMD_LIST_NP);
         }
    }

}



CMD_BUF_STRU * zb_zed_panid_cfg(int iPanId)
{
    CMD_BUF_STRU *pLstCmd = NULL;

    // send an at command to get coo panid
    int iIdx = AllocCmdBuf();
    
    if (iIdx < MAX_CMD_BUF_NUM)
    {
        CMD_BUF_STRU *pCmd = &saCmdBuf[iIdx];

        sprintf(pCmd->cmdBuf,"AT+CONFIG=%s=%d\n",AT_CONFIG_PANID,iPanId);
        
        pCmd->cmdId = AT_CONFIG_PANID;

        pCmd->bgn_cb = zbCmdBgn4AirAt;

        zb_AddCmd(pCmd,CMD_LIST_HP);

        pLstCmd = pCmd;
    }   

    if (pLstCmd)
    {
        gCoo.bit1NetCfgFlag = TRUE;
        gCoo.iCfgNodeRspNum = 0;
    }
    
    return pLstCmd;

}

void zbCmdNetCfgMsg(SAT_MSG_HEAD *pMsg)
{
    DATAPROC_ZB_CFG_STRU * msg = (DATAPROC_ZB_CFG_STRU * )pMsg;


    ZB_DP("CMD_NETWORK_CFG_MSG : %d\r\n",msg->iType);

    switch (msg->iType)
    {
    case 0:
        /* reset to default config */
        zb_zed_panid_cfg(ZB_CFG_PANID);
        break;
    default:
        break;
    }
}

void zbDataProcNetworkCfg(SAT_MSG_HEAD *pMsg)
{
    DATAPROC_NETWORK_CFG_STRU *pNetworkCfgMsg = (DATAPROC_NETWORK_CFG_STRU *)pMsg;

    CMD_BUF_STRU *pLstCmd = zb_zed_panid_cfg(gCoo.iCalcedPanId);


    if (pLstCmd)
    {
        pLstCmd->para   = pNetworkCfgMsg->cbPara;
        pLstCmd->end_cb = pNetworkCfgMsg->cbFunc;
    }
    else
    {
        ((CMD_END_CALLBACK)pNetworkCfgMsg->cbFunc)(pNetworkCfgMsg->cbPara,0XE0,NULL);
    }
}

void zbMsgProc(void *para,SAT_MSG_HEAD *pMsg)
{
    switch(pMsg->event)
    {
    case INIT_ALL_THREAD_EVENT:
        zbInit();
        break;
    case SERIALITF_ZB_MSG:
        zbProcSerialItfMsg(pMsg);
        break;
    case MAIN_ZBITF_MSG:
        zbProcMainMsg(pMsg);
        break;
    //case SERIALITF_ZB_RF_MSG:
    //    zbProcSerialItfRfMsg(pMsg,nMsgLen);
    //    break;
    case CMD_ZB_DOWNLAOD_MSG:
        zbStartDownload(pMsg);
        break;
    case CMD_ZB_HANDSHAKE_MSG:
        zbHandShake();
        break;
    case CMD_ZB_FILESIZE_MSG:
        zbFileSize();
        break;
    case CMD_NETWORK_CFG_MSG:
        zbCmdNetCfgMsg(pMsg);
        break;
    case TIMER_MSG:
        zbProcTimerMsg(pMsg);
        break;
    case QUIT_ALL_THREAD_EVENT:
        zbDeInit();
        break;
    case DATAPRCO_ZB_MSG:
        zbDataProcMsgProc(pMsg);
        break;
    case DATAPRCO_CFG_NETWORK_ZB:
        zbDataProcNetworkCfg(pMsg);
        break;
    default:
        break;
   }
}


void zbConfigCallback(void *para,int result,char *rsp)
{
    ZB_CONFIG_PARA_STRU *zbCfgPara = (ZB_CONFIG_PARA_STRU *)para;

    sp_thread_mutex_lock  ( &gZbIpc.mutex );
    sp_thread_cond_signal ( &gZbIpc.cond  );
    sp_thread_mutex_unlock( &gZbIpc.mutex );

    if (0 == result)
    {
        sprintf(zbCfgPara->rsp,"%s=%s",zbCfgPara->attr,rsp);
    }
    else
    {
       zbCfgPara->rsp[0] = 0;
    }
}

void zbNetworkConfigCallback(void *para,int result,char *rsp)
{
    ZB_NETWORK_CONFIG_PARA_STRU *zbCfgPara = (ZB_NETWORK_CONFIG_PARA_STRU *)para;

    (void)rsp;

    sp_thread_mutex_lock  ( &gZbIpc.mutex );
    sp_thread_cond_signal ( &gZbIpc.cond  );
    sp_thread_mutex_unlock( &gZbIpc.mutex );

    zbCfgPara->rsp[0] = result;
}

int zbGetRfChlId(void)
{
   return gCoo.iCalcedRfId;
}

int zbGetNodes(void)
{
    //return gCoo.iNodes;
    return 1;
}

void zbReset(void)
{
    gCoo.iNodes = 0;
    MakeResetCmd();
}

/* call by any thread different from zbMsgPro's */
int zbConfiguration(char *pCmd, char *pRsp, int iRspLen)
{
    char atcmd[256];
    DATAPROC_ZB_STRU *pMsg;
    ZB_CONFIG_PARA_STRU Para;
    int   ret;
    char *pAtt    = NULL;
    char *pValue  = NULL;

    struct timeval now;
    struct timespec outtime;

    (void)iRspLen;

    if (!Att_value_parse(pCmd,'=',&pAtt,&pValue))
    {
        /* got no response */
        pRsp[0] = '\0';
        return -1;
    }

    sprintf(atcmd,"AT+CONFIG=%s=%s",pAtt,pValue);


    pMsg = (DATAPROC_ZB_STRU *)SatAllocMsg(DATAPROC_ZB_MSG_SIZE + strlen(atcmd) + 1);
    
    if (!pMsg)
    {
        /* got no response */
        pRsp[0] = '\0';
        return ERROR_CODE_MEM_ALLOC_FAIL;
    }

    pMsg->msgHead.event = DATAPRCO_ZB_MSG;

    Para.attr = pAtt;
    Para.rsp  = pRsp;

    pMsg->cbPara = &Para;
    pMsg->cbFunc = zbConfigCallback;
    
    strcpy (pMsg->data,atcmd);

    sp_thread_mutex_lock( &gZbIpc.mutex );
    
    ret = VOS_SndMsg(TASK_HANDLE_ZB,(SAT_MSG_HEAD *)pMsg);

    if (0 != ret)
    {

        sp_thread_mutex_unlock( &gZbIpc.mutex );

        /* got no response */
        pRsp[0] = '\0';
        
        return -1;
    }

    gettimeofday(&now, NULL);
    outtime.tv_sec  = now.tv_sec + 5;
    outtime.tv_nsec = now.tv_usec * 1000;

    ret = sp_thread_cond_timedwait( &gZbIpc.cond, &gZbIpc.mutex ,&outtime);//ylf: thread sleep here

    if(ETIMEDOUT == ret)
    {
        sp_thread_mutex_unlock( &gZbIpc.mutex );

        /* got no response */
        pRsp[0] = '\0';

        return -1;
    }

    sp_thread_mutex_unlock( &gZbIpc.mutex );

    return 0;
}

/* call by any thread different from zbMsgPro's */
int zbNetworkCfg(char *pRsp)
{
    DATAPROC_NETWORK_CFG_STRU *pMsg;
    ZB_NETWORK_CONFIG_PARA_STRU Para;
    int   ret;

    struct timeval now;
    struct timespec outtime;

    pMsg = (DATAPROC_NETWORK_CFG_STRU *)SatAllocMsg(DATAPROC_NETWORK_CFG_MSG_SIZE + 1 );
    
    if (!pMsg)
    {
        return ERROR_CODE_MEM_ALLOC_FAIL;
    }

    pMsg->msgHead.event = DATAPRCO_CFG_NETWORK_ZB;

    Para.rsp = pRsp;

    pMsg->cbPara = &Para;
    pMsg->cbFunc = zbNetworkConfigCallback;
    
    sp_thread_mutex_lock( &gZbIpc.mutex );
    
    ret = VOS_SndMsg(TASK_HANDLE_ZB,(SAT_MSG_HEAD *)pMsg);

    if (0 != ret)
    {

        sp_thread_mutex_unlock( &gZbIpc.mutex );

        /* failed */
        pRsp[0] = 0XE0;        
        return -1;
    }

    gettimeofday(&now, NULL);
    outtime.tv_sec  = now.tv_sec + 5;
    outtime.tv_nsec = now.tv_usec * 1000;

    ret = sp_thread_cond_timedwait( &gZbIpc.cond, &gZbIpc.mutex ,&outtime);//ylf: thread sleep here

    if(ETIMEDOUT == ret)
    {
        sp_thread_mutex_unlock( &gZbIpc.mutex );

        /* failed */
        pRsp[0] = 0XE1;  
        
        return -1;
    }

    sp_thread_mutex_unlock( &gZbIpc.mutex );

    return 0;
}

#ifdef __cplusplus
}
#endif

