#include <math.h>
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
#include <errno.h>
#include <time.h>
#include <sys/time.h>
#include "ccb.h"
#include "canItf.h"
#include "msg.h"
#include "messageitf.h"
#include "memory.h"
#include "cminterface.h"
#include "Interface.h"
#include "DefaultParams.h"
#include "log.h"

#include "notify.h"
#include "zb.h"
#include "exdisplay.h"


#ifdef __cplusplus
extern "C"
{
#endif

typedef void (* RUN_COMM_CALL_BACK )(int id);


TIMER_PARAM_STRU gTps;

CCB gCcb;

TASK_HANDLE TASK_HANDLE_MAIN ;
TASK_HANDLE TASK_HANDLE_CANITF;
TASK_HANDLE TASK_HANDLE_MOCAN ;
TASK_HANDLE TASK_HANDLE_ZB;

unsigned int gulSecond = 0;

unsigned char gaucNotifyBuffer[1024];

unsigned char gaucIapBuffer[1024];


#define HEART_BEAT_PERIOD (15)

#define HEART_BEAT_MARGIN (5)

#define GET_B_MASK(no) (1<<(no+APP_EXE_ECO_NUM))

#define MAKE_B_MASK(mask) (mask <<(APP_EXE_ECO_NUM))

#define GET_RECT_MASK(no) (1<<(no+APP_EXE_ECO_NUM + APP_EXE_PRESSURE_METER))

#define MAKE_RECT_MASK(mask) (mask <<(APP_EXE_ECO_NUM + APP_EXE_PRESSURE_METER))

#define is_B3_FULL(ulValue) ((ulValue / gCcb.PMParam.afDepth[DISP_PM_PM3] * 100) >= B3_FULL)

//#define is_B2_FULL(ulValue) ((ulValue / gCcb.PMParam.afDepth[DISP_PM_PM2] * 100) >= B2_FULL)

#define is_SWPUMP_FRONT(pCcb) ((pCcb)->MiscParam.ulMisFlags & (1 << DISP_SM_SW_PUMP))

#define haveB1(pCcb)((pCcb)->SubModSetting.ulFlags & (1 << DISP_SM_HaveB1))
#define haveB3(pCcb)((pCcb)->SubModSetting.ulFlags & (1 << DISP_SM_HaveB3))
#define haveTOC(pCcb)((pCcb)->SubModSetting.ulFlags & (1 << DISP_SM_HaveTOC))
#define haveB2(pCcb)((pCcb)->SubModSetting.ulFlags & (1 << DISP_SM_HaveB2))

#define haveHPCir(pCcb)((pCcb)->MiscParam.ulMisFlags & (1 << DISP_SM_HP_Water_Cir))
#define haveZigbee(pCcb)((pCcb)->MiscParam.iNetworkMask & (1 << DISPLAY_NETWORK_ZIGBEE))

//ex
int Check_TOC_Alarm = 0;
int AlarmHighWorkPress = 0;
const float cor_H_PurtTank = 0.08;
int isAdaptQtwFlush = 0;

//end

int is_B2_FULL(ulValue)
{
    if(DISP_WATER_BARREL_TYPE_UDF == gCcb.PMParam.aiBuckType[DISP_PM_PM2])
    {
        return ((ulValue / gCcb.PMParam.afDepth[DISP_PM_PM2] * 100) >= B2_FULL);
    }
    else
    {
        float correction = cor_H_PurtTank;
        if(ulValue > correction)
        {
            ulValue = ulValue - correction;
        }
        else
        {
            ulValue = 0;
        }
        return ((ulValue / (gCcb.PMParam.afDepth[DISP_PM_PM2] - correction) * 100) >= B2_FULL);
    }

}

typedef enum
{
    WORK_MSG_TYPE_START_QTW = 0,
    WORK_MSG_TYPE_STOP_QTW,
    WORK_MSG_TYPE_INIT_RUN,
    WORK_MSG_TYPE_WASH,
    WORK_MSG_TYPE_RUN,
    
    WORK_MSG_TYPE_SFW, /* Start fill water */
    WORK_MSG_TYPE_EFW, /* End fill water */

    WORK_MSG_TYPE_SCIR, /* Start circulation     */
    WORK_MSG_TYPE_ECIR, /* End circulation water */

    WORK_MSG_TYPE_SN3, /* Start N3     */
    WORK_MSG_TYPE_EN3, /* End   N3 */

    WORK_MSG_TYPE_SKH,  /* Start Key Handle */
    WORK_MSG_TYPE_EKH,  /* End Key Handle */

    WORK_MSG_TYPE_EPW,  /* End  */

    WORK_MSG_TYPE_ESR,  /* End of Speed Regulation */

    WORK_MSG_TYPE_IDLE,  /* Return to Idle */
    

    WORK_MSG_TYPE_STATE = 0x80,  /* State Changed */

    WORK_MSG_TYPE_STATE4PW
    
}WORK_MSG_TYPE_ENUM;

char *notify_info[] = 
{
    "start taking water succ","start taking water fail",
    "stop taking water succ","",
    "init run succ","init run fail",
    "wash succ","wash fail",
    "run succ","run fail",
    "start filling tank","fill tank fail",
    "stop filling tank ","",
    "start circulation","circulation fail",
    "stop circulation ","",
    "start n3","n3 fail",
    "stop n3 ","",
    "start key handle succ","start key handle fail",
    "stop key handle ","",
    "return to idle ","",
};

void WorkCommEntry(void *para);

int CcbUpdateSwitch(int id,unsigned int ulAddress, unsigned int ulMask,unsigned int ulSwitchs);

int CcbModbusWorkEntry(int iChl,unsigned int ulCanId,unsigned char *pucModbus,int iPayLoad,int iTime,modbus_call_back cb);

int CcbModbusWorkEntryWrapper(int id,int iChl,unsigned int ulCanId,unsigned char *pucModbus,int iPayLoad,int iTime,modbus_call_back cb);

int CcbWorkDelayEntry(int id,unsigned int ulTime,sys_timeout_handler cb);

void work_idle(void *para);
void CcbAbortWork(WORK_ITEM_STRU *pWorkItem);

void work_run_comm_proc(WORK_ITEM_STRU *pWorkItem,CCB *pCcb,RUN_COMM_CALL_BACK cbf,RUN_COMM_CALL_BACK cbs);
void CcbCancelAllWork(void);
void CcbRmvWork(work_proc proc);

void work_idle_succ(void);
void CanCcbSndHOState(int iIndex , int iState);
void CcbStopQtw(void);
unsigned int CcbGetSwitchObjState(unsigned int ulMask);
void CcbUpdateRPumpObjState(int iChl,int iState);
void CcbRPumpStateNotify(int ichl,int state);
void CcbSwithStateNotify(void);
void CcbExeBrdNotify(void);
void CcbFmBrdNotify(void);
void CcbHandlerNotify(int ucIndex);
DISPHANDLE CcbCancelWork(DISPHANDLE handle);
void CcbNotState(int state);
void CcbNotSWPressure(void);
void CcbNotAlarmFire(int iPart,int iId,int bFired);
void CcbNotSpeed(int iType,int iSpeed);
int CcbC2Regulator(int id,float fv,int on);

DISPHANDLE CcbInnerWorkInitRun(void);

void CanCcbSndQtwRspMsg(int iIndex,int iCode);

int MakeThdState(int id,int flag)
{
    return flag << (id*2);
}

int GetThdState(int id,int flag)
{
    return (flag >> (id*2)) & ((1 << 2)-1);
}

inline void CcbSetActiveMask(int iSrcAddr)
{
    gCcb.ulActiveMask   |= (1 << iSrcAddr);
    if (iSrcAddr < MAX_HB_CHECK_ITEMS) gCcb.aucHbCounts[iSrcAddr] = MAX_HB_CHECK_COUNT;    
}

void CcbDefaultModbusCallBack(int code,void *param)
{
    MODBUS_PACKET_COMM_STRU *pmg = (MODBUS_PACKET_COMM_STRU *)param;

    int iContLen = 0;

    if (ERROR_CODE_SUCC == code)
    {
        if (pmg)
        {
            switch(pmg->ucFuncode)
            {
            case MODBUS_FUNC_CODE_Read_Coil_Status:
            case MODBUS_FUNC_CODE_Read_Input_Status:
            case MODBUS_FUNC_CODE_Read_Holding_Registers:
            case MODBUS_FUNC_CODE_Read_Input_Registers:
                iContLen = pmg->aucData[0] + 2;
                break;
            case MODBUS_FUNC_CODE_Force_Single_Coil:
            case MODBUS_FUNC_CODE_Preset_Single_Register:
            case MODBUS_FUNC_CODE_Force_Multiple_Coils:
            case MODBUS_FUNC_CODE_Preset_Multiple_Registers:
                iContLen = 5;
                break;
            case MODBUS_FUNC_CODE_Mask_Write_0X_Register:
            case MODBUS_FUNC_CODE_Mask_Write_4X_Register:
                iContLen = 7;
                break;
            }

            if (0 == iContLen)
            {
                return;
            }

            if (iContLen <= MAX_MODBUS_BUFFER_SIZE)
            {
                memcpy(gCcb.aucModbusBuffer,(unsigned char *)pmg,iContLen);
            }
        }
    }
}

int DispGetUpCirFlag(void)
{
    if (DISP_WORK_STATE_RUN == gCcb.curWorkState.iMainWorkState4Pw
        && DISP_WORK_SUB_RUN_CIR == gCcb.curWorkState.iSubWorkState4Pw)
    {
       return (CIR_TYPE_UP == gCcb.iCirType);
    }

    return 0;
}

int DispGetTankCirFlag(void)
{
    if (DISP_WORK_STATE_RUN == gCcb.curWorkState.iMainWorkState4Pw
        && DISP_WORK_SUB_RUN_CIR == gCcb.curWorkState.iSubWorkState4Pw)
    {
       return (CIR_TYPE_HP == gCcb.iCirType);
    }

    return 0;

}

int DispGetTocCirFlag(void)
{
    if (DISP_WORK_STATE_RUN == gCcb.curWorkState.iMainWorkState4Pw
        && DISP_WORK_SUB_RUN_CIR == gCcb.curWorkState.iSubWorkState4Pw)
    {
        return gCcb.bit1TocOngoing;
    }

    return 0;
}

int DispGetUpQtwFlag(void)
{
    int iLoop;

    for (iLoop = 0; iLoop < MAX_HANDLER_NUM; iLoop++)
    {
        if (gCcb.aHandler[iLoop].bit1Qtw 
            && APP_DEV_HS_SUB_HYPER == gCcb.aHandler[iLoop].iDevType) 
        {
            return 1;
        }
    }

    return 0;
}

int DispGetEdiQtwFlag(void)
{
    int iLoop;

    for (iLoop = 0; iLoop < MAX_HANDLER_NUM; iLoop++)
    {
        if (gCcb.aHandler[iLoop].bit1Qtw 
            && APP_DEV_HS_SUB_REGULAR == gCcb.aHandler[iLoop].iDevType) 
        {
            return 1;
        }
    }

    return 0;

}


int DispGetREJ(float *pfValue)
{
    if ((DISP_WORK_STATE_RUN == gCcb.curWorkState.iMainWorkState)
       ||(gCcb.ulMachineType == MACHINE_ADAPT && CcbGetTwPendingFlag()))
    {
       *pfValue = CcbCalcREJ();
       
       return (1);
    }

    return 0;

}

int DispGetSwitchState(int ulMask)
{
   return CcbGetSwitchObjState(ulMask);
}

int DispGetSingleSwitchState(int iChlNo)
{
   return !!(gCcb.ExeBrd.aValveObjs[iChlNo-APP_EXE_E1_NO].iActive & DISP_ACT_TYPE_SWITCH) ;
}

void DispGetOtherCurrent(int iChlNo,int *pmA)
{
   switch(iChlNo)
   {
   case APP_EXE_N1_NO ... APP_EXE_N3_NO:
       *pmA = CcbConvert2RectAndEdiData(gCcb.ExeBrd.aRectObjs[iChlNo-APP_EXE_N1_NO].Value.ulV);
       break;
   case APP_EXE_T1_NO:
       *pmA = CcbConvert2RectAndEdiData(gCcb.ExeBrd.aEDIObjs[iChlNo-APP_EXE_T1_NO].Value.ulV);
       break;
   case APP_EXE_C3_NO ... APP_EXE_C4_NO:
       *pmA = CcbConvert2GPumpData(gCcb.ExeBrd.aGPumpObjs[iChlNo-APP_EXE_T1_NO].Value.ulV);
       break;
   default:
       return ;
   }
}


void DispGetRPumpRti(int iChlNo,int *pmV,int *pmA)
{
   if (iChlNo >= APP_EXE_C1_NO && iChlNo <= APP_EXE_C2_NO )
   {
       unsigned int ulTemp = gCcb.ExeBrd.aRPumpObjs[iChlNo-APP_EXE_C1_NO].Value.ulV;
       
       *pmA = CcbConvert2RPumpIData(ulTemp);

       *pmV = DispConvertRPumpSpeed2Voltage(gCcb.ExeBrd.ausHoldRegister[APP_EXE_HOLD_REG_RPUMP_OFFSET + iChlNo - APP_EXE_C1_NO]);
   
   }
}


int CcbGetTwFlag(void)
{
    int iLoop;

    int iFlags = 0;

    for (iLoop = 0; iLoop < MAX_HANDLER_NUM; iLoop++)
    {
        if (gCcb.aHandler[iLoop].bit1Qtw) iFlags |= 1 << iLoop;
    }

    return iFlags;
}


int CcbGetTwPendingFlag(void)
{
    int iLoop;

    int iFlags = 0;

    for (iLoop = 0; iLoop < MAX_HANDLER_NUM; iLoop++)
    {
        if (gCcb.aHandler[iLoop].bit1PendingQtw) iFlags |= 1 << iLoop;
    }

    return iFlags;
    
}



int DispGetInitRunFlag(void)
{
    if (DISP_WORK_STATE_PREPARE == gCcb.curWorkState.iMainWorkState
        && DISP_WORK_SUB_IDLE == gCcb.curWorkState.iSubWorkState)
    {
       return 1;
    }

    return 0;

}

int DispGetTubeCirFlag(void)
{

   return !!gCcb.bit1TubeCirOngoing;

}


int DispGetPwFlag(void)
{
   return !!gCcb.bit1ProduceWater;
}

int DispGetFwFlag(void)
{
   return !!gCcb.bit1FillingTank;
}

int DispGetTankFullLoopFlag(void)
{
   return !!gCcb.bit1PeriodFlushing;
}

int DispGetRunningStateFlag(void)
{
   return gCcb.bit3RuningState;
}

int CcbGetTwFlagByDevType(int type)
{
    int iLoop;
    //int iFlags = 0;

    for (iLoop = 0; iLoop < MAX_HANDLER_NUM; iLoop++)
    {
        if (gCcb.aHandler[iLoop].bit1Qtw
            && type == gCcb.aHandler[iLoop].iDevType
            && gCcb.aHandler[iLoop].bit1Active) 
        {
            return iLoop; 
        }
    }
    return MAX_HANDLER_NUM;
}

int CcbGetHandlerStatus(int iDevId)
{
    int iIdx = iDevId - APP_HAND_SET_BEGIN_ADDRESS;

    if (iIdx >= 0 && iIdx < APP_HAND_SET_MAX_NUMBER)
    {
        return gCcb.aHandler[iIdx].bit1Active;
    }

    return 0;
    
}

int CcbGetHandlerType(int iDevId)
{
    int iIdx = iDevId - APP_HAND_SET_BEGIN_ADDRESS;

    if (iIdx >= 0 && iIdx < APP_HAND_SET_MAX_NUMBER)
    {
        return gCcb.aHandler[iIdx].iDevType;
    }

    return 0;
    
}


int CcbGetHandlerTwFlag(int iDevId)
{
    int iIdx = iDevId - APP_HAND_SET_BEGIN_ADDRESS;

    if (iIdx >= 0 && iIdx < APP_HAND_SET_MAX_NUMBER)
    {
        return gCcb.aHandler[iIdx].bit1Qtw;
    }

    return 0;
    
}


int CcbGetHandlerId2Index(int iDevId)
{
    int iIdx = iDevId - APP_HAND_SET_BEGIN_ADDRESS;

    if (iIdx >= 0 && iIdx < APP_HAND_SET_MAX_NUMBER)
    {
        return iIdx;
    }

    return APP_HAND_SET_MAX_NUMBER;
    
}

void CanCcbSaveQtw2(int iTrxIndex, int iDevId,int iVolume)
{

    int iIndex   = (iDevId - APP_HAND_SET_BEGIN_ADDRESS);
    int ulCanId;

    CAN_BUILD_ADDRESS_IDENTIFIER(ulCanId,iDevId,APP_DEV_TYPE_MAIN_CTRL);

    gCcb.aHandler[iIndex].CommHdr.ucDevType = APP_DEV_TYPE_HAND_SET;
    gCcb.aHandler[iIndex].CommHdr.ucTransId = 0XF0;
    gCcb.aHandler[iIndex].CommHdr.ucMsgType = APP_PACKET_HAND_OPERATION;

    gCcb.aHandler[iIndex].ulCanId = ulCanId;
    gCcb.aHandler[iIndex].iCanIdx = 0;
    if(INVALID_FM_VALUE != iVolume)
    {
        gCcb.QtwMeas.ulTotalFm        = iVolume - gCaliParam.pc[DISP_PC_COFF_S1].fc;
    }
    else
    {
        gCcb.QtwMeas.ulTotalFm        = iVolume;
    }

    gCcb.aHandler[iIndex].iCurTrxIndex = iTrxIndex;

    
    VOS_LOG(VOS_LOG_WARNING,"%s:%d",__FUNCTION__,gCcb.QtwMeas.ulTotalFm);

}

void CcbInitHandlerQtwMeas(int iIndex)
{
    QTW_MEAS_STRU         *pQtwMeas;

    /* DO NOT init ulTotalFm */
    pQtwMeas = &gCcb.aHandler[iIndex].QtwMeas;
    
    pQtwMeas->ulBgnFm   = INVALID_FM_VALUE;
    pQtwMeas->ulCurFm   = INVALID_FM_VALUE;

    pQtwMeas->ulBgnTm   = time(NULL);
    pQtwMeas->ulEndTm   = pQtwMeas->ulBgnTm;

    pQtwMeas = &gCcb.QtwMeas;

    pQtwMeas->ulBgnFm   = INVALID_FM_VALUE;
    pQtwMeas->ulCurFm   = INVALID_FM_VALUE;
    
    pQtwMeas->ulBgnTm   = time(NULL);
    pQtwMeas->ulEndTm   = pQtwMeas->ulBgnTm;
        
}

void CcbFiniHandlerQtwMeas(int iIndex)
{
    QTW_MEAS_STRU         *pQtwMeas;

    pQtwMeas = &gCcb.aHandler[iIndex].QtwMeas;

    pQtwMeas->ulEndTm   = time(NULL);

    pQtwMeas = &gCcb.QtwMeas;

    pQtwMeas->ulEndTm   = pQtwMeas->ulEndTm;
}


void MainSndWorkMsg(int iSubMsg,unsigned char *data, int iPayLoadLen)
{
    // send message to main
    int ret;

    MAIN_WORK_MSG_STRU *pWorkMsg = NULL;
    
    pWorkMsg = (MAIN_WORK_MSG_STRU *)SatAllocMsg(MAIN_WORK_MSG_SIZE + iPayLoadLen);//malloc(sizeof(MAIN_CANITF_MSG_STRU) + CanRcvBuff[ucRcvBufIndex].len);
    if (NULL == pWorkMsg)
    {
        return ;
    }
    
    pWorkMsg->msgHead.event  = MAIN_WORK_MSG;
    pWorkMsg->iMsgLen        = iPayLoadLen;
    pWorkMsg->iSubMsg        = iSubMsg;
 
    memcpy(pWorkMsg->aucData,data,iPayLoadLen);

    ret = VOS_SndMsg(TASK_HANDLE_MAIN,(SAT_MSG_HEAD *)pWorkMsg);

    if (0 != ret)
    {
    }
}



int CcbSndCanCmd(int iChl,unsigned int ulCanId,unsigned char ucCmd0,unsigned char ucCmd1,unsigned char *data, int iPayLoadLen)
{
    int iRet;
    int iCanMsgLen = iPayLoadLen + RPC_FRAME_HDR_SZ + RPC_UART_FRAME_OVHD;
    unsigned char        *sbBuf ;
    MAIN_CANITF_MSG_STRU *pMsg = (MAIN_CANITF_MSG_STRU *)SatAllocMsg(MAIN_CANITF_MSG_SIZE + iCanMsgLen);//malloc(sizeof(TIMER_MSG_STRU));
    if (pMsg)
    {
        unsigned char ucFcs = 0;
        int           iLen;
        int           iIdx;

        pMsg->msgHead.event = MAIN_CANITF_MSG;
        pMsg->ulCanId       = ulCanId;
        pMsg->iCanChl       = iChl;
        pMsg->iMsgLen       = iCanMsgLen;
        pMsg->aucData[0]    = RPC_UART_SOF;
        
        sbBuf = pMsg->aucData + 1;
        sbBuf[RPC_POS_LEN]  = iPayLoadLen; // iLen for data area (NOT INCLUDE CMD0&CMD1&LEN itself)
        sbBuf[RPC_POS_CMD0] = ucCmd0;
        sbBuf[RPC_POS_CMD1] = ucCmd1;//SAPP_CMD_DATA;
        memcpy(&sbBuf[RPC_POS_DAT0],data,iPayLoadLen);
   
        iLen = sbBuf[RPC_POS_LEN] + RPC_FRAME_HDR_SZ;
        for ( iIdx = RPC_POS_LEN; iIdx < iLen; iIdx++)
        {
          ucFcs ^= sbBuf[iIdx];
        }
        sbBuf[iLen] = ucFcs;
       
        iRet = VOS_SndMsg(TASK_HANDLE_CANITF,(SAT_MSG_HEAD *)pMsg);
        if (0 != iRet)
        {
            return -1;
        }

        return 0;
    }

    return -1;

}



int CcbSndAfCanCmd(int iChl,unsigned int ulCanId,unsigned char ucCmd,unsigned char *data, int iPayLoadLen)
{
    return CcbSndCanCmd(iChl,ulCanId,RPC_SYS_AF,ucCmd,data,iPayLoadLen);
}

int CcbSndIapCanCmd(int iChl,unsigned int ulCanId,unsigned char ucCmd,unsigned char *data, int iPayLoadLen)
{
    return CcbSndCanCmd(iChl,ulCanId,RPC_SYS_BOOT,ucCmd,data,iPayLoadLen);
}

int CcbSndZigbeeCmd(sAddr_t *addr,unsigned char ucProtol,unsigned char *data, int iPayLoadLen)
{
    int iRet;
    int iCanMsgLen ;
    int iAdrLen = 0;
    unsigned char       *sbBuf ;
    MAIN_ZBITF_MSG_STRU *pMsg ;


    switch(addr->addrMode)
    {
    case Addr64Bit:
        iAdrLen = 9;
        break;
    case AddrBroadcast:
        iAdrLen = 1;
        break;
    default:
        iAdrLen = 3;
        break;
    }

    iCanMsgLen =  iPayLoadLen + iAdrLen + APP_ZIGBEE_SUB_PROTOL_LENGTH + RPC_FRAME_HDR_SZ; /* 1byte for protol */

    pMsg = (MAIN_ZBITF_MSG_STRU *)SatAllocMsg(MAIN_CANITF_MSG_SIZE + iCanMsgLen);//malloc(sizeof(TIMER_MSG_STRU));
    if (pMsg)
    {
        pMsg->msgHead.event = MAIN_ZBITF_MSG;
        pMsg->usMsgLen       = iCanMsgLen;
        
        sbBuf = pMsg->aucData ;
        sbBuf[RPC_POS_LEN]  = iPayLoadLen + iAdrLen + APP_ZIGBEE_SUB_PROTOL_LENGTH;
        sbBuf[RPC_POS_CMD0] = RPC_SYS_APP;
        sbBuf[RPC_POS_CMD1] = SAPP_CMD_DATA;//SAPP_CMD_DATA;

        sbBuf += RPC_FRAME_HDR_SZ;

         if (addr->addrMode == Addr64Bit)
         {
             sbBuf[0] = Addr64Bit;
             memcpy(&sbBuf[1],addr->addr.extAddr,Z_EXTADDR_LEN);
         }
         else if (AddrBroadcast == addr->addrMode)
         {
             sbBuf[0] = afAddrBroadcast;
         
         }
         else
         {
             sbBuf[0] = afAddr16Bit;
             sbBuf[1] = (addr->addr.shortAddr >> 8) & 0XFF;
             sbBuf[2] = (addr->addr.shortAddr >> 0) & 0XFF;
         }

        sbBuf += iAdrLen;  

        sbBuf[0] = ucProtol;

        sbBuf += APP_ZIGBEE_SUB_PROTOL_LENGTH;
         
        memcpy(&sbBuf[0],data,iPayLoadLen);
   
        iRet = VOS_SndMsg(TASK_HANDLE_ZB,(SAT_MSG_HEAD *)pMsg);
        if (0 != iRet)
        {

            return -1;
        }

        return 0;
    }

    return -1;

}

void CcbZigbeeModeDedicate(unsigned char *data, int iPayLoadLen)
{
    int iLoop  = 0;
    sAddr_t addr ;
    for (iLoop = 0; iLoop < MAX_HANDLER_NUM; iLoop++)
    {
        if (gCcb.aHandler[iLoop].iTrxMap & (1 << APP_TRX_ZIGBEE)
            &&  gCcb.aHandler[iLoop].bit1Active)
        {
              addr.addrMode            = afAddr16Bit;
              addr.addr.shortAddr      = gCcb.aHandler[iLoop].usShortAddr;
              if (CcbSndZigbeeCmd(&addr,SAPP_CMD_DATA,data,iPayLoadLen))
              {
                  VOS_LOG(VOS_LOG_DEBUG,"CcbSndZigbeeCmd failed %d\r\n",gCcb.aHandler[iLoop].usShortAddr);
              }
        }
    }
}


void CcbZigbeeModeSingle(int iIdx,unsigned char *data, int iPayLoadLen)
{
    sAddr_t addr ;
    if (gCcb.aHandler[iIdx].iTrxMap & (1 << APP_TRX_ZIGBEE)
        &&  gCcb.aHandler[iIdx].bit1Active)
    {
          addr.addrMode            = afAddr16Bit;
          addr.addr.shortAddr      = gCcb.aHandler[iIdx].usShortAddr;

          if (CcbSndZigbeeCmd(&addr,SAPP_CMD_DATA,data,iPayLoadLen))
          {
              VOS_LOG(VOS_LOG_DEBUG,"CcbSndZigbeeCmd failed %d\r\n",gCcb.aHandler[iIdx].usShortAddr);
          }
          
    }
}


void CcbZigbeeModeBroadcast(unsigned char *data, int iPayLoadLen)
{
    sAddr_t addr ;
    addr.addrMode            = AddrBroadcast;
    addr.addr.shortAddr      = 0xFFFF;
    CcbSndZigbeeCmd(&addr,SAPP_CMD_DATA,data,iPayLoadLen);
}


float CcbConvert2Pm3Data(unsigned int ulValue)
{
    float fValue = (ulValue * 3300);

    //VOS_LOG(VOS_LOG_DEBUG,"pm3 raw %d",ulValue); 

    fValue /= (4096*125);

    /* I to pm value */
    if (fValue <= 4) fValue = 4;

    fValue = (fValue - 4) / 16; /* to percent */
    
    return fValue * B3_LEVEL;
}

// Source Tank
float CcbConvert2Pm3SP(unsigned int ulValue)
{
    float fValue = CcbConvert2Pm3Data(ulValue);

    if (0 == gCcb.PMParam.afDepth[DISP_PM_PM3])
    {
        return 100.0;
    }

    //ex_dcj
    float tmp = (fValue / gCcb.PMParam.afDepth[DISP_PM_PM3]) * 100 ;
    tmp *= gCaliParam.pc[DISP_PC_COFF_SW_TANK_LEVEL].fk;
    return tmp;
   // return (fValue / gCcb.PMParam.afDepth[DISP_PM_PM3]) * 100 ;  /* convert to 0~100 percent */
}



float CcbConvert2Pm2Data(unsigned int ulValue)
{
    float fValue = (ulValue * 3300);

    fValue /= (4096*125);

    if (fValue <= 4) fValue = 4;

    fValue = (fValue - 4) / 16; 
    
    return fValue * B2_LEVEL;
}

//Pure Tank
float CcbConvert2Pm2SP(unsigned int ulValue)
{
    float fValue = CcbConvert2Pm2Data(ulValue);

    float correction_fValue = fValue;
    float correction = 0.0;

    if (0 == gCcb.PMParam.afDepth[DISP_PM_PM2])
    {
        return 100.0;
    }

    if(DISP_WATER_BARREL_TYPE_UDF != gCcb.PMParam.aiBuckType[DISP_PM_PM2])
    {
        correction = cor_H_PurtTank;
        if(fValue > correction)
        {
            correction_fValue = fValue - correction;
        }
        else
        {
            correction_fValue = 0;
        }
    }

    float tmp = (correction_fValue / (gCcb.PMParam.afDepth[DISP_PM_PM2] - correction)) * 100 ;
    tmp *= gCaliParam.pc[DISP_PC_COFF_PW_TANK_LEVEL].fk;
    return tmp;
}

//work pressure
float CcbConvert2Pm1Data(unsigned int ulValue)
{
    float fValue = (ulValue * 3300);

    fValue /= (4096*125);

    if (fValue <= 4) fValue = 4;

    fValue = (fValue - 4) / 16; 
    //ex_dcj
    fValue *= B1_PRESSURE;
    fValue *= gCaliParam.pc[DISP_PC_COFF_SYS_PRESSURE].fk;
    return fValue;
    
    //return fValue * B1_PRESSURE;
}

/**
 * 计算过水量
 * @param  ulValue [脉冲数]
 * @return         [过水量，单位ml]
 */
unsigned int CcbConvert2Fm1Data(unsigned int ulValue)
{
    /* x pulse per litre */
    if (!gCcb.FMParam.aulCfg[0])
    {
        VOS_LOG(VOS_LOG_WARNING,"gCcb.FMParam.aulCfg[0] = 0");    
    
        return 0;
    }
    unsigned int tmp = (ulValue * 1000)/ gCcb.FMParam.aulCfg[0]; //ml
    return tmp;
}

unsigned int CcbConvert2Fm2Data(unsigned int ulValue)
{
    /* x pulse per litre */
    if (!gCcb.FMParam.aulCfg[1])
    {
        VOS_LOG(VOS_LOG_WARNING,"gCcb.FMParam.aulCfg[1] = 0");    
    
        return 0;
    }
    unsigned int tmp = (ulValue * 1000)/ gCcb.FMParam.aulCfg[1];
    return tmp;
}

unsigned int CcbConvert2Fm3Data(unsigned int ulValue)
{
    /* x pulse per litre */
    if (!gCcb.FMParam.aulCfg[2])
    {
        VOS_LOG(VOS_LOG_WARNING,"gCcb.FMParam.aulCfg[2] = 0");    
    
        return 0;
    }
    unsigned int tmp = (ulValue * 1000)/ gCcb.FMParam.aulCfg[2];
    return tmp;
}

unsigned int CcbConvert2Fm4Data(unsigned int ulValue)
{   
    /* x pulse per litre */
    if (!gCcb.FMParam.aulCfg[3])
    {
        VOS_LOG(VOS_LOG_WARNING,"gCcb.FMParam.aulCfg[3] = 0");    
    
        return 0;
    }
    unsigned int tmp = (ulValue * 1000)/ gCcb.FMParam.aulCfg[3];
    return tmp;
}

unsigned int CcbConvert2RectAndEdiData(unsigned int ulValue)
{
    return ((ulValue * 100)/ 198) ; // ->ma
}

unsigned int CcbConvert2GPumpData(unsigned int ulValue)
{
    return (((ulValue  & 0XFFFF))*100)/99 ; // ->ma
}

unsigned int CcbGetRPumpVData(int iChl)
{
    return DispConvertRPumpSpeed2Voltage( gCcb.ExeBrd.ausHoldRegister[APP_EXE_HOLD_REG_RPUMP_OFFSET + iChl]);
}

unsigned int CcbConvert2RPumpIData(unsigned int ulValue)
{
    
    return (((ulValue  & 0XFFFF))*100)/99 ; // ->ma
}

float CcbGetSp1(void)
{
    return gCcb.MMParam.SP[MACHINE_PARAM_SP1];
}

float CcbGetSp2(void)
{
    return gCcb.MMParam.SP[MACHINE_PARAM_SP2];

}

float CcbGetSp3(void)
{
    return gCcb.MMParam.SP[MACHINE_PARAM_SP3];

}

float CcbGetSp4(void)
{
    return gCcb.MMParam.SP[MACHINE_PARAM_SP4];

}

float CcbGetSp5(void)
{
    return gCcb.MMParam.SP[MACHINE_PARAM_SP5];
}


float CcbGetSp6(void)
{
    return gCcb.MMParam.SP[MACHINE_PARAM_SP6];
}

float CcbGetSp7(void)
{
    return gCcb.MMParam.SP[MACHINE_PARAM_SP7];
}

float CcbGetSp8(void)
{
    return gCcb.MMParam.SP[MACHINE_PARAM_SP8];
}

float CcbGetSp9(void)
{
    return gCcb.MMParam.SP[MACHINE_PARAM_SP9];
}

float CcbGetSp10(void)
{
    return gCcb.MMParam.SP[MACHINE_PARAM_SP10];
}

float CcbGetSp11(void)
{
    return gCcb.MMParam.SP[MACHINE_PARAM_SP11];
}

float CcbGetSp12(void)
{
    return gCcb.MMParam.SP[MACHINE_PARAM_SP12];
}

float CcbGetSp13(void)
{
    return gCcb.MMParam.SP[MACHINE_PARAM_SP13];
}

float CcbGetSp14(void)
{
    return gCcb.MMParam.SP[MACHINE_PARAM_SP14];
}

float CcbGetSp15(void)
{
    return gCcb.MMParam.SP[MACHINE_PARAM_SP15];
}

float CcbGetSp16(void)
{
    return gCcb.MMParam.SP[MACHINE_PARAM_SP16];
}

float CcbGetSp17(void)
{
    return gCcb.MMParam.SP[MACHINE_PARAM_SP17];
}

float CcbGetSp30(void)
{
    return gCcb.MMParam.SP[MACHINE_PARAM_SP30];
}

float CcbGetSp31(void)
{
    return gCcb.MMParam.SP[MACHINE_PARAM_SP31];
}

float CcbGetSp32(void)
{
    return gCcb.MMParam.SP[MACHINE_PARAM_SP32];
}

float CcbGetSp33(void)
{
    return gCcb.MMParam.SP[MACHINE_PARAM_SP33];
}

float CcbGetB2Full(void)
{
    return 0.9;
}

void CanPrepare4Pm2Full(void)
{
    if (!gCcb.bit1B2Full)
    {
        gCcb.bit1B2Full = TRUE;
        gCcb.ulB2FullTick = gulSecond;
    }
}

float CcbCalcREJ(void)
{
    float fTmp;
    if (0 == gCcb.ExeBrd.aEcoObjs[APP_EXE_I1_NO].Value.eV.fWaterQ) return 0;

    fTmp = (gCcb.ExeBrd.aEcoObjs[APP_EXE_I1_NO].Value.eV.fWaterQ - gCcb.ExeBrd.aEcoObjs[APP_EXE_I2_NO].Value.eV.fWaterQ);

    return (fTmp/gCcb.ExeBrd.aEcoObjs[APP_EXE_I1_NO].Value.eV.fWaterQ)*100;
}



void CcbUpdateSwitchObjState(unsigned int ulMask,unsigned int ulValue)
{
    int iLoop;
    for (iLoop = 0; iLoop < APP_EXE_SWITCHS; iLoop++)
    {
        if (ulMask & (1 << iLoop))
        {
            switch(iLoop)
            {
            case APP_EXE_E1_NO ... APP_EXE_E10_NO:
                if (ulValue & (1 << iLoop))
                {
                    gCcb.ExeBrd.aValveObjs[iLoop-APP_EXE_E1_NO].iActive |= DISP_ACT_TYPE_SWITCH;
                }
                else
                {
                    gCcb.ExeBrd.aValveObjs[iLoop-APP_EXE_E1_NO].iActive &= ~DISP_ACT_TYPE_SWITCH;
                }
                break;
            case APP_EXE_C3_NO ... APP_EXE_C4_NO:
                if (ulValue & (1 << iLoop))
                {
                    if (!gCcb.ExeBrd.aGPumpObjs[iLoop-APP_EXE_C3_NO].iActive)
                    {
                         gCcb.ExeBrd.aGPumpObjs[iLoop-APP_EXE_C3_NO].ulSec = DispGetCurSecond();
                    }
                    
                    gCcb.ExeBrd.aGPumpObjs[iLoop-APP_EXE_C3_NO].iActive |= DISP_ACT_TYPE_SWITCH;
                }
                else
                {
                    if (gCcb.ExeBrd.aGPumpObjs[iLoop-APP_EXE_C3_NO].iActive)
                    {
                        unsigned int ulEndSec = DispGetCurSecond();
                        
                        if (ulEndSec > gCcb.ExeBrd.aGPumpObjs[iLoop-APP_EXE_C3_NO].ulSec)
                        {
                            gCcb.ExeBrd.aGPumpObjs[iLoop-APP_EXE_C3_NO].ulDurSec += ulEndSec - gCcb.ExeBrd.aGPumpObjs[iLoop-APP_EXE_C3_NO].ulSec;
                        }
                    }
                
                    gCcb.ExeBrd.aGPumpObjs[iLoop-APP_EXE_C3_NO].iActive &= ~DISP_ACT_TYPE_SWITCH;
                }
                break;
            case APP_EXE_C1_NO ... APP_EXE_C2_NO:
                if (ulValue & (1 << iLoop))
                {
                    CcbUpdateRPumpObjState(iLoop-APP_EXE_C1_NO,0XFF00);
                }
                else
                {
                    CcbUpdateRPumpObjState(iLoop-APP_EXE_C1_NO,0X0000);
                }
                break;
            case APP_EXE_N1_NO ... APP_EXE_N3_NO:
                if (ulValue & (1 << iLoop))
                {
                    if (!gCcb.ExeBrd.aRectObjs[iLoop-APP_EXE_N1_NO].iActive)
                    {
                        gCcb.ExeBrd.aRectObjs[iLoop-APP_EXE_N1_NO].ulSec = DispGetCurSecond();
                    }
                    gCcb.ExeBrd.aRectObjs[iLoop-APP_EXE_N1_NO].iActive |= DISP_ACT_TYPE_SWITCH;
                }
                else
                {
                    if (gCcb.ExeBrd.aRectObjs[iLoop-APP_EXE_N1_NO].iActive)
                    {
                        unsigned int ulEndSec = DispGetCurSecond();
                        
                        if (ulEndSec > gCcb.ExeBrd.aRectObjs[iLoop-APP_EXE_N1_NO].ulSec)
                        {
                            gCcb.ExeBrd.aRectObjs[iLoop-APP_EXE_N1_NO].ulDurSec += ulEndSec - gCcb.ExeBrd.aRectObjs[iLoop-APP_EXE_N1_NO].ulSec;
                        }
                    }
                    gCcb.ExeBrd.aRectObjs[iLoop-APP_EXE_N1_NO].iActive &= ~DISP_ACT_TYPE_SWITCH;
                }
                break;
            case APP_EXE_T1_NO:
                if (ulValue & (1 << iLoop))
                {
                    gCcb.ExeBrd.aEDIObjs[iLoop-APP_EXE_T1_NO].iActive |= DISP_ACT_TYPE_SWITCH;
                }
                else
                {
                    gCcb.ExeBrd.aEDIObjs[iLoop-APP_EXE_T1_NO].iActive &= ~DISP_ACT_TYPE_SWITCH;
                }
                break;
            }
        }
    }
    if (gCcb.bit1SysMonitorStateRpt) CcbSwithStateNotify();
}

unsigned int CcbGetSwitchObjState(unsigned int ulMask)
{
    int iLoop    ;
    unsigned int ulValue = 0;
    
    for (iLoop = 0; iLoop < APP_EXE_SWITCHS; iLoop++)
    {
        if (ulMask & (1 << iLoop))
        {
            switch(iLoop)
            {
            case APP_EXE_E1_NO ... APP_EXE_E10_NO:
                if (gCcb.ExeBrd.aValveObjs[iLoop-APP_EXE_E1_NO].iActive & DISP_ACT_TYPE_SWITCH)  ulValue |= (1 << iLoop);
                break;
            case APP_EXE_C3_NO ... APP_EXE_C4_NO:
                if (gCcb.ExeBrd.aGPumpObjs[iLoop-APP_EXE_C3_NO].iActive & DISP_ACT_TYPE_SWITCH)  ulValue |= (1 << iLoop);
                break;
            case APP_EXE_C1_NO ... APP_EXE_C2_NO:
                if (gCcb.ExeBrd.aRPumpObjs[iLoop-APP_EXE_C1_NO].iActive & DISP_ACT_TYPE_SWITCH)  ulValue |= (1 << iLoop);
                break;
            case APP_EXE_N1_NO ... APP_EXE_N3_NO:
                if (gCcb.ExeBrd.aRectObjs[iLoop-APP_EXE_N1_NO].iActive & DISP_ACT_TYPE_SWITCH)  ulValue |= (1 << iLoop);
                break;
            case APP_EXE_T1_NO:
                if (gCcb.ExeBrd.aEDIObjs[iLoop-APP_EXE_T1_NO].iActive & DISP_ACT_TYPE_SWITCH)  ulValue |= (1 << iLoop);
                break;
            }
        }
    }
    return ulValue;
}

void CcbUpdateRPumpObjState(int iChl,int iState)
{
    if ((iState >> 8) & (0XFF))
    {
        if (!gCcb.ExeBrd.aRPumpObjs[iChl].iActive)
        {
             gCcb.ExeBrd.aRPumpObjs[iChl].ulSec = DispGetCurSecond();
        }

        gCcb.ExeBrd.aRPumpObjs[iChl].iActive |= DISP_ACT_TYPE_SWITCH;

        if (gCcb.bit1SysMonitorStateRpt) CcbRPumpStateNotify(iChl,1);
        
    }
    else
    {
        if (gCcb.ExeBrd.aRPumpObjs[iChl].iActive)
        {
            unsigned int ulEndSec = DispGetCurSecond();
            
            if (ulEndSec > gCcb.ExeBrd.aRPumpObjs[iChl].ulSec)
            {
                gCcb.ExeBrd.aRPumpObjs[iChl].ulDurSec += 
                       ulEndSec - gCcb.ExeBrd.aRPumpObjs[iChl].ulSec;
            }
        }                
    
        gCcb.ExeBrd.aRPumpObjs[iChl].iActive &= ~DISP_ACT_TYPE_SWITCH;
        
        if (gCcb.bit1SysMonitorStateRpt) CcbRPumpStateNotify(iChl,0);
    }
}


void CcbUpdatePmObjState(unsigned int ulMask,unsigned int ulValue)
{
    int iLoop;
    for (iLoop = 0; iLoop < APP_EXE_PRESSURE_METER; iLoop++)
    {
        if (ulMask & GET_B_MASK(iLoop))
        {
            if (ulValue & GET_B_MASK(iLoop))
            {
                gCcb.ExeBrd.aPMObjs[iLoop].iActive |= DISP_ACT_TYPE_RPT;
            }
            else
            {
                gCcb.ExeBrd.aPMObjs[iLoop].iActive &= ~DISP_ACT_TYPE_RPT;
            }
        }
    }
}

unsigned int CcbGetPmObjState(unsigned int ulMask)
{
    int iLoop    ;
    unsigned int ulValue = 0;
    
    for (iLoop = 0; iLoop < APP_EXE_PRESSURE_METER; iLoop++)
    {
        if (ulMask & (1 << iLoop))
        {
            if (gCcb.ExeBrd.aPMObjs[iLoop].iActive & DISP_ACT_TYPE_RPT)  ulValue |= (1 << iLoop);
        }
    }
    return ulValue;
}

void CcbUpdateIObjState(unsigned int ulMask,unsigned int ulValue)
{
    int iLoop;
    for (iLoop = 0; iLoop < APP_EXE_ECO_NUM; iLoop++)
    {
        if (ulMask & (1 << iLoop))
        {
            if (ulValue & (1 << iLoop))
            {
                gCcb.ExeBrd.aEcoObjs[iLoop].iActive |= DISP_ACT_TYPE_RPT;
            }
            else
            {
                gCcb.ExeBrd.aEcoObjs[iLoop].iActive &= ~DISP_ACT_TYPE_RPT;
            }
        }
    }
}

unsigned int CcbGetIObjState(unsigned int ulMask)
{
    int iLoop    ;
    unsigned int ulValue = 0;
    
    for (iLoop = 0; iLoop < APP_EXE_ECO_NUM; iLoop++)
    {
        if (ulMask & (1 << iLoop))
        {
            if (gCcb.ExeBrd.aEcoObjs[iLoop].iActive & DISP_ACT_TYPE_RPT)  ulValue |= (1 << iLoop);
        }
    }
    return ulValue;
}


void CcbUpdateFmObjState(unsigned int ulMask,unsigned int ulValue)
{
    int iLoop;
    for (iLoop = 0; iLoop < APP_FM_FLOW_METER_NUM; iLoop++)
    {
        if (ulMask & (1 << iLoop))
        {
            if (ulValue & (1 << iLoop))
            {
                gCcb.FlowMeter.aFmObjs[iLoop].iActive |= DISP_ACT_TYPE_RPT;
            }
            else
            {
                gCcb.FlowMeter.aFmObjs[iLoop].iActive &= ~DISP_ACT_TYPE_RPT;
            }
        }
    }
}

unsigned int CcbGetFmObjState(unsigned int ulMask)
{
    int iLoop    ;
    unsigned int ulValue = 0;
    
    for (iLoop = 0; iLoop < APP_FM_FLOW_METER_NUM; iLoop++)
    {
        if (ulMask & (1 << iLoop))
        {
            if (gCcb.FlowMeter.aFmObjs[iLoop].iActive & DISP_ACT_TYPE_RPT)  ulValue |= (1 << iLoop);
        }
    }
    return ulValue;
}

int CcbWorkDelayEntryWrapper(int id,unsigned int ulTime,sys_timeout_handler cb)
{
    timer_reset(&gCcb.to4Delay[id],ulTime,TIMER_ONE_SHOT,cb,(void *)id);  
    
    sp_thread_sem_pend(&gCcb.Sem4Delay[id]);

    if (gCcb.ulWorkThdAbort & MakeThdState(id,WORK_THREAD_STATE_BLK_TICKING))
    {
        return -1;
    }

    return 0;
}


void CcbDelayCallBack(void *para)
{
    int id = (int)para;

    sp_thread_sem_post(&gCcb.Sem4Delay[id]);

}


int CcbWorkDelayEntry(int id,unsigned int ulTime,sys_timeout_handler cb)
{
    int iRet;
    
    sp_thread_mutex_lock( &gCcb.WorkMutex );
    gCcb.ulWorkThdState |= MakeThdState(id,WORK_THREAD_STATE_BLK_TICKING);
    sp_thread_mutex_unlock( &gCcb.WorkMutex );

    iRet = CcbWorkDelayEntryWrapper(id,ulTime,cb);    

    sp_thread_mutex_lock( &gCcb.WorkMutex );
    gCcb.ulWorkThdState &= ~ MakeThdState(id,WORK_THREAD_STATE_BLK_TICKING);
    gCcb.ulWorkThdAbort &= ~ MakeThdState(id,WORK_THREAD_STATE_BLK_TICKING);
    sp_thread_mutex_unlock( &gCcb.WorkMutex );

    return iRet; // success
}


unsigned int CcbGetModbus2BytesData(int offset)
{
    return BUILD_UINT16(gCcb.aucModbusBuffer[offset+1],gCcb.aucModbusBuffer[offset]);
}

unsigned int CcbGetModbus4BytesData(int offset)
{
    return BUILD_UINT32(gCcb.aucModbusBuffer[offset+3],gCcb.aucModbusBuffer[offset+2],gCcb.aucModbusBuffer[offset+1],gCcb.aucModbusBuffer[offset+0]);
}


int CcbSwitchUpdateModbusWrapper(int id,unsigned int ulAddress, unsigned int ulMask,unsigned int ulSwitchs)
{
    int iSkip = 1;
    
    unsigned int ulIdentifier;

    unsigned char aucModbusBuf[16];

    int iRet = -1;

    int iIdx = 0;

    MODBUS_PACKET_COMM_STRU *pModBus = (MODBUS_PACKET_COMM_STRU *)aucModbusBuf;

    if (MACHINE_ADAPT != gCcb.ulMachineType)
    {
        if ((((1 << APP_EXE_E1_NO)|(1 << APP_EXE_C1_NO) ) == (ulMask & ((1 << APP_EXE_E1_NO)|(1 << APP_EXE_C1_NO))))
            && (0 == (((1 << APP_EXE_E1_NO)|(1 << APP_EXE_C1_NO) ) & ulSwitchs) ))
        {
    
            unsigned int  ulState = CcbGetSwitchObjState( (1 << APP_EXE_E1_NO)|(1 << APP_EXE_C1_NO));  
    
            if (((1 << APP_EXE_E1_NO)|(1 << APP_EXE_C1_NO)) == ulState)
            {
                iSkip = 0;
            }
        }
    }

    if (!iSkip)
    {
        iIdx = 0;

        /* 0. Node Address (2018/03/04 add for zigbee) */
        pModBus->ucAddress = APP_PAKCET_ADDRESS_EXE; 
        
        pModBus->ucFuncode = MODBUS_FUNC_CODE_Mask_Write_0X_Register;

        ulMask &=  ~(1<<APP_EXE_E1_NO);

        
        /* 1.Reg Address */
        pModBus->aucData[iIdx++] = (ulAddress >> 8) & 0XFF; 
        pModBus->aucData[iIdx++] = (ulAddress >> 0) & 0XFF;
        
        /* 2.Mask Value */
        pModBus->aucData[iIdx++] = (ulMask >> 8) & 0XFF;
        pModBus->aucData[iIdx++] = (ulMask >> 0) & 0XFF;
        
        /* 3. Value */
        pModBus->aucData[iIdx++] = (ulSwitchs >> 8) & 0XFF;
        pModBus->aucData[iIdx++] = (ulSwitchs >> 0) & 0XFF;
        
        CAN_BUILD_ADDRESS_IDENTIFIER(ulIdentifier,APP_PAKCET_ADDRESS_MC,APP_PAKCET_ADDRESS_EXE);
        
        iRet = CcbModbusWorkEntryWrapper(id,0,ulIdentifier,aucModbusBuf,iIdx + MODBUS_PACKET_COMM_HDR_LEN,2000,CcbDefaultModbusCallBack); // don care modbus exe result
        
        if (iRet )
        {
            VOS_LOG(VOS_LOG_WARNING,"CcbSwitchUpdateModbusWrapper Fail %d",iRet);    
            /* notify ui (late implemnt) */
            return iRet;
        }

        iRet = CcbWorkDelayEntry(id,5000,CcbDelayCallBack);
        if (iRet )
        {
            VOS_LOG(VOS_LOG_WARNING,"CcbSwitchUpdateModbusWrapper Fail %d",iRet);    
            /* notify ui (late implemnt) */
            return iRet;
        }

        ulMask = 1 << APP_EXE_E1_NO;
        ulSwitchs &= 1 << APP_EXE_E1_NO;
        
        iIdx = 0;
        /* 0. Node Address (2018/03/04 add for zigbee) */
        pModBus->ucAddress = APP_PAKCET_ADDRESS_EXE; 
        pModBus->ucFuncode = MODBUS_FUNC_CODE_Mask_Write_0X_Register;
        
        /* 1.Reg Address */
        pModBus->aucData[iIdx++] = (ulAddress >> 8) & 0XFF; 
        pModBus->aucData[iIdx++] = (ulAddress >> 0) & 0XFF;
        
        /* 2.Mask Value */
        pModBus->aucData[iIdx++] = (ulMask >> 8) & 0XFF;
        pModBus->aucData[iIdx++] = (ulMask >> 0) & 0XFF;
        
        /* 3. Value */
        pModBus->aucData[iIdx++] = (ulSwitchs >> 8) & 0XFF;
        pModBus->aucData[iIdx++] = (ulSwitchs >> 0) & 0XFF;
        
        CAN_BUILD_ADDRESS_IDENTIFIER(ulIdentifier,APP_PAKCET_ADDRESS_MC,APP_PAKCET_ADDRESS_EXE);
        
        iRet = CcbModbusWorkEntryWrapper(id,0,ulIdentifier,aucModbusBuf,iIdx + MODBUS_PACKET_COMM_HDR_LEN,2000,CcbDefaultModbusCallBack); // don care modbus exe result
        
        if (iRet )
        {
            VOS_LOG(VOS_LOG_WARNING,"CcbSwitchUpdateModbusWrapper Fail %d",iRet);    
            /* notify ui (late implemnt) */
            return iRet;
        }
        
    }
    else
    {

        iIdx = 0;
        /* 0. Node Address (2018/03/04 add for zigbee) */
        pModBus->ucAddress = APP_PAKCET_ADDRESS_EXE; 
        pModBus->ucFuncode = MODBUS_FUNC_CODE_Mask_Write_0X_Register;
        
        /* 1.Reg Address */
        pModBus->aucData[iIdx++] = (ulAddress >> 8) & 0XFF; 
        pModBus->aucData[iIdx++] = (ulAddress >> 0) & 0XFF;
        
        /* 2.Mask Value */
        pModBus->aucData[iIdx++] = (ulMask >> 8) & 0XFF;
        pModBus->aucData[iIdx++] = (ulMask >> 0) & 0XFF;
        
        /* 3. Value */
        pModBus->aucData[iIdx++] = (ulSwitchs >> 8) & 0XFF;
        pModBus->aucData[iIdx++] = (ulSwitchs >> 0) & 0XFF;
        
        CAN_BUILD_ADDRESS_IDENTIFIER(ulIdentifier,APP_PAKCET_ADDRESS_MC,APP_PAKCET_ADDRESS_EXE);
        
        iRet = CcbModbusWorkEntryWrapper(id,0,ulIdentifier,aucModbusBuf,iIdx + MODBUS_PACKET_COMM_HDR_LEN,2000,CcbDefaultModbusCallBack); // don care modbus exe result
        
        if (iRet )
        {
            VOS_LOG(VOS_LOG_WARNING,"CcbSwitchUpdateModbusWrapper Fail %d",iRet);    
            /* notify ui (late implemnt) */
            return iRet;
        }
    }

    return iRet;
}

int CcbSwitchSetModbusWrapper(int id,unsigned int ulAddress, unsigned int ulNums,unsigned int ulSwitchs)
{
    unsigned int ulIdentifier;

    unsigned char aucModbusBuf[16];

    int iTmp;

    int iRet = -1;

    int iIdx = 0;

    int iSkip = 1;

    unsigned int ulMask = 0;
    unsigned int ulSwMask = 0;

    MODBUS_PACKET_COMM_STRU *pModBus = (MODBUS_PACKET_COMM_STRU *)aucModbusBuf;

    ulMask = ((1 << ulNums) - 1) << ulAddress;

    ulSwMask = ulSwitchs << ulAddress;

    if (MACHINE_ADAPT != gCcb.ulMachineType)
    {
        if ((((1 << APP_EXE_E1_NO)|(1 << APP_EXE_C1_NO) ) == (ulMask & ((1 << APP_EXE_E1_NO)|(1 << APP_EXE_C1_NO))))
            && (0 == (((1 << APP_EXE_E1_NO)|(1 << APP_EXE_C1_NO) ) & ulSwMask )))
        {
    
            unsigned int  ulState = CcbGetSwitchObjState( (1 << APP_EXE_E1_NO)|(1 << APP_EXE_C1_NO));  
    
            if (((1 << APP_EXE_E1_NO)|(1 << APP_EXE_C1_NO)) == ulState)
            {
                iSkip = 0;
            }
        }
    }
    if (!iSkip)
    {
        int iNewAdrOff = APP_EXE_C1_NO - ulAddress;
        
        iIdx = 0;
        /* 0. Node Address (2018/03/04 add for zigbee) */
        pModBus->ucAddress = APP_PAKCET_ADDRESS_EXE; 
        pModBus->ucFuncode = MODBUS_FUNC_CODE_Force_Multiple_Coils;
        
        /* 1.Start Address */
        pModBus->aucData[iIdx++] = (APP_EXE_C1_NO >> 8) & 0XFF; 
        pModBus->aucData[iIdx++] = (APP_EXE_C1_NO >> 0) & 0XFF;
        
        /* 2.Reg Numbers */
        iTmp = ulNums - iNewAdrOff;
        pModBus->aucData[iIdx++] = (iTmp >> 8) & 0XFF;
        pModBus->aucData[iIdx++] = (iTmp >> 0) & 0XFF;
        
        /* 3. data counts */
        pModBus->aucData[iIdx++] = 2;
        
        /* set  valves */
        pModBus->aucData[iIdx++] = (ulSwitchs >> iNewAdrOff) & 0XFF;
        pModBus->aucData[iIdx++] = (ulSwitchs >> (8 + iNewAdrOff)) & 0XFF;
        
        CAN_BUILD_ADDRESS_IDENTIFIER(ulIdentifier,APP_PAKCET_ADDRESS_MC,APP_PAKCET_ADDRESS_EXE);
        
        iRet = CcbModbusWorkEntryWrapper(id,0,ulIdentifier,aucModbusBuf,iIdx + MODBUS_PACKET_COMM_HDR_LEN,2000,CcbDefaultModbusCallBack); // don care modbus exe result
        
        if (iRet )
        {
            VOS_LOG(VOS_LOG_WARNING,"CcbModbusWorkEntryWrapper Fail %d",iRet);    
            /* notify ui (late implemnt) */
            return iRet;
        }

        iRet = CcbWorkDelayEntry(id,5000,CcbDelayCallBack);
        if (iRet )
        {
            VOS_LOG(VOS_LOG_WARNING,"CcbWorkDelayEntry Fail %d",iRet);    
            /* notify ui (late implemnt) */
            return iRet;
        }
        

        iIdx = 0;
        /* 0. Node Address (2018/03/04 add for zigbee) */
        pModBus->ucAddress = APP_PAKCET_ADDRESS_EXE; 
        pModBus->ucFuncode = MODBUS_FUNC_CODE_Force_Multiple_Coils;
        
        /* 1.Start Address */
        pModBus->aucData[iIdx++] = (ulAddress >> 8) & 0XFF; 
        pModBus->aucData[iIdx++] = (ulAddress >> 0) & 0XFF;
        
        /* 2.Reg Numbers */
        iTmp = iNewAdrOff;
        pModBus->aucData[iIdx++] = (iTmp >> 8) & 0XFF;
        pModBus->aucData[iIdx++] = (iTmp >> 0) & 0XFF;
        
        /* 3. data counts */
        pModBus->aucData[iIdx++] = 2;
        
        /* set  valves */
        pModBus->aucData[iIdx++] = (ulSwitchs >> 0) & 0XFF;
        pModBus->aucData[iIdx++] = (ulSwitchs >> 8) & 0XFF;
        
        CAN_BUILD_ADDRESS_IDENTIFIER(ulIdentifier,APP_PAKCET_ADDRESS_MC,APP_PAKCET_ADDRESS_EXE);
        
        iRet = CcbModbusWorkEntryWrapper(id,0,ulIdentifier,aucModbusBuf,iIdx + MODBUS_PACKET_COMM_HDR_LEN,2000,CcbDefaultModbusCallBack); // don care modbus exe result
        
        if (iRet )
        {
            VOS_LOG(VOS_LOG_WARNING,"CcbModbusWorkEntryWrapper Fail %d",iRet);    
            /* notify ui (late implemnt) */
            return iRet;
        }
         
    }
    else
    {

        iIdx = 0;
        /* 0. Node Address (2018/03/04 add for zigbee) */
        pModBus->ucAddress = APP_PAKCET_ADDRESS_EXE; 
        pModBus->ucFuncode = MODBUS_FUNC_CODE_Force_Multiple_Coils;
        
        /* 1.Start Address */
        pModBus->aucData[iIdx++] = (ulAddress >> 8) & 0XFF; 
        pModBus->aucData[iIdx++] = (ulAddress >> 0) & 0XFF;
        
        /* 2.Reg Numbers */
        iTmp = ulNums;
        pModBus->aucData[iIdx++] = (iTmp >> 8) & 0XFF;
        pModBus->aucData[iIdx++] = (iTmp >> 0) & 0XFF;
        
        /* 3. data counts */
        pModBus->aucData[iIdx++] = 2;
        
        /* set  valves */
        pModBus->aucData[iIdx++] = (ulSwitchs >> 0) & 0XFF;
        pModBus->aucData[iIdx++] = (ulSwitchs >> 8) & 0XFF;
        
        CAN_BUILD_ADDRESS_IDENTIFIER(ulIdentifier,APP_PAKCET_ADDRESS_MC,APP_PAKCET_ADDRESS_EXE);
        
        iRet = CcbModbusWorkEntryWrapper(id,0,ulIdentifier,aucModbusBuf,iIdx + MODBUS_PACKET_COMM_HDR_LEN,2000,CcbDefaultModbusCallBack); // don care modbus exe result
        
        if (iRet )
        {
            VOS_LOG(VOS_LOG_WARNING,"CcbModbusWorkEntryWrapper Fail %d",iRet);    
            /* notify ui (late implemnt) */
            return iRet;
        }
    }

    //CcbUpdateSwitchObjState(APP_EXE_SWITCHS_MASK,iTmp);

    return iRet;
}



int CcbInnerSetSwitch(int id,unsigned int ulAddress, unsigned int ulSwitchs)
{

    int iTmp;
    int iRet = -1;

    VOS_LOG(VOS_LOG_WARNING,"CcbInnerSetSwitch id=%x %x",id,ulSwitchs);    
    
    /* 2.Reg Numbers */
    iTmp = APP_EXE_VALVE_NUM + APP_EXE_G_PUMP_NUM + APP_EXE_R_PUMP_NUM + APP_EXE_EDI_NUM + APP_EXE_RECT_NUM;
    
    iRet = CcbSwitchSetModbusWrapper(id,ulAddress,iTmp,ulSwitchs); // don care modbus exe result
    if (iRet )
    {
        VOS_LOG(VOS_LOG_WARNING,"CcbModbusWorkEntry Fail %d",iRet);    
        /* notify ui (late implemnt) */
        return iRet;
    }

    CcbUpdateSwitchObjState(APP_EXE_SWITCHS_MASK,ulSwitchs);

    return iRet;
}


int CcbSetSwitch(int id,unsigned int ulAddress, unsigned int ulSwitchs)
{
    int iRet = -1;

    sp_thread_mutex_lock( &gCcb.C12Mutex );
    
    iRet = CcbInnerSetSwitch(id,ulAddress,ulSwitchs);

    sp_thread_mutex_unlock( &gCcb.C12Mutex );

    return iRet;
}


int CcbInnerUpdateSwitch(int id,unsigned int ulAddress, unsigned int ulMask,unsigned int ulSwitchs)
{
    int iRet = -1;

    VOS_LOG(VOS_LOG_WARNING,"updswitch %x %x",ulMask,ulSwitchs);    

    iRet = CcbSwitchUpdateModbusWrapper(id,ulAddress,ulMask,ulSwitchs); // don care modbus exe result
    
    if (iRet )
    {
        VOS_LOG(VOS_LOG_WARNING,"CcbSwitchUpdateModbusWrapper Fail %d",iRet);    
        /* notify ui (late implemnt) */
        return iRet;
    }

    CcbUpdateSwitchObjState(ulMask,ulSwitchs);

    return iRet;
}


int CcbUpdateSwitch(int id,unsigned int ulAddress, unsigned int ulMask,unsigned int ulSwitchs)
{
    int iRet = -1;

    sp_thread_mutex_lock( &gCcb.C12Mutex );
    
    iRet = CcbInnerUpdateSwitch(id,ulAddress,ulMask,ulSwitchs);

    sp_thread_mutex_unlock( &gCcb.C12Mutex );

    return iRet;
}

int CcbSetIAndBs(int id,unsigned int ulAddress, unsigned int ulSwitchs)
{
    unsigned int ulIdentifier;

    unsigned char aucModbusBuf[16];

    int iRet = -1;

    int iIdx = 0;

    MODBUS_PACKET_COMM_STRU *pModBus = (MODBUS_PACKET_COMM_STRU *)aucModbusBuf;

    iIdx = 0;
    /* 0. Node Address (2018/03/04 add for zigbee) */
    pModBus->ucAddress = APP_PAKCET_ADDRESS_EXE; 
    pModBus->ucFuncode = MODBUS_FUNC_CODE_Preset_Single_Register;
    
    /* 1.Start Address */
    pModBus->aucData[iIdx++] = (ulAddress >> 8) & 0XFF; 
    pModBus->aucData[iIdx++] = (ulAddress >> 0) & 0XFF;
    
    /* 2. I&B ID */
    pModBus->aucData[iIdx++] = (ulSwitchs >> 8) & 0XFF;
    pModBus->aucData[iIdx++] = (ulSwitchs >> 0) & 0XFF;
    
    CAN_BUILD_ADDRESS_IDENTIFIER(ulIdentifier,APP_PAKCET_ADDRESS_MC,APP_PAKCET_ADDRESS_EXE);

    
    iRet = CcbModbusWorkEntryWrapper(id,0,ulIdentifier,aucModbusBuf,iIdx + MODBUS_PACKET_COMM_HDR_LEN,2000,CcbDefaultModbusCallBack); // don care modbus exe result
    
    if (iRet )
    {
        VOS_LOG(VOS_LOG_WARNING,"CcbModbusWorkEntry Fail %d",iRet);    
        /* notify ui (late implemnt) */
        return iRet;
    }

    CcbUpdateIObjState(APP_EXE_IP_MASK,ulSwitchs);
    
    CcbUpdatePmObjState(APP_EXE_IP_MASK,ulSwitchs);

    return iRet;
}

int CcbUpdateIAndBs(int id,unsigned int ulAddress, unsigned int ulMask,unsigned int ulValue)
{
    unsigned int ulIdentifier;

    unsigned char aucModbusBuf[16];

    int iRet = -1;

    int iIdx = 0;

    MODBUS_PACKET_COMM_STRU *pModBus = (MODBUS_PACKET_COMM_STRU *)aucModbusBuf;

    iIdx = 0;
    /* 0. Node Address (2018/03/04 add for zigbee) */
    pModBus->ucAddress = APP_PAKCET_ADDRESS_EXE; 
    pModBus->ucFuncode = MODBUS_FUNC_CODE_Mask_Write_4X_Register;
    
    /* 1.Reg Address */
    pModBus->aucData[iIdx++] = (ulAddress >> 8) & 0XFF; 
    pModBus->aucData[iIdx++] = (ulAddress >> 0) & 0XFF;
    
    /* 2.Mask Value  */
    pModBus->aucData[iIdx++] = (ulMask >> 8) & 0XFF;
    pModBus->aucData[iIdx++] = (ulMask >> 0) & 0XFF;
    
    /* 3.Value       */
    pModBus->aucData[iIdx++] = (ulValue >> 8) & 0XFF;
    pModBus->aucData[iIdx++] = (ulValue >> 0) & 0XFF;
    
    CAN_BUILD_ADDRESS_IDENTIFIER(ulIdentifier,APP_PAKCET_ADDRESS_MC,APP_PAKCET_ADDRESS_EXE);
    
    iRet = CcbModbusWorkEntryWrapper(id,0,ulIdentifier,aucModbusBuf,iIdx + MODBUS_PACKET_COMM_HDR_LEN,2000,CcbDefaultModbusCallBack); // don care modbus exe result
    
    if (iRet )
    {
        VOS_LOG(VOS_LOG_WARNING,"CcbModbusWorkEntry Fail %d",iRet);    
        /* notify ui (late implemnt) */
        return iRet;
    }

    CcbUpdateIObjState(ulMask,ulValue);
    
    CcbUpdatePmObjState(ulMask,ulValue);

    return iRet;
}


int CcbSetFms(int id,unsigned int ulAddress, unsigned int ulSwitchs)
{
    unsigned int ulIdentifier;

    unsigned char aucModbusBuf[16];

    int iRet = -1;

    int iIdx = 0;

    MODBUS_PACKET_COMM_STRU *pModBus = (MODBUS_PACKET_COMM_STRU *)aucModbusBuf;

    iIdx = 0;
    /* 0. Node Address (2018/03/04 add for zigbee) */
    pModBus->ucAddress = APP_PAKCET_ADDRESS_FM; 
    pModBus->ucFuncode = MODBUS_FUNC_CODE_Preset_Single_Register;
    
    /* 1.Start Address */
    pModBus->aucData[iIdx++] = (ulAddress >> 8) & 0XFF; 
    pModBus->aucData[iIdx++] = (ulAddress >> 0) & 0XFF;
    
    
    /* 2. I&B ID */
    pModBus->aucData[iIdx++] = (ulSwitchs >> 8) & 0XFF;
    pModBus->aucData[iIdx++] = (ulSwitchs >> 0) & 0XFF;
    
    CAN_BUILD_ADDRESS_IDENTIFIER(ulIdentifier,APP_PAKCET_ADDRESS_MC,APP_PAKCET_ADDRESS_FM);

    
    iRet = CcbModbusWorkEntryWrapper(id,0,ulIdentifier,aucModbusBuf,iIdx + MODBUS_PACKET_COMM_HDR_LEN,2000,CcbDefaultModbusCallBack); // don care modbus exe result
    
    if (iRet )
    {
        VOS_LOG(VOS_LOG_WARNING,"CcbModbusWorkEntry Fail %d",iRet);    
        /* notify ui (late implemnt) */
        return iRet;
    }

    CcbUpdateFmObjState(APP_FM_FM_MASK,ulSwitchs);

    return iRet;
}


int CcbUpdateFms(int id,unsigned int ulAddress, unsigned int ulMask,unsigned int ulValue)
{
    unsigned int ulIdentifier;

    unsigned char aucModbusBuf[16];

    int iRet = -1;

    int iIdx = 0;

    MODBUS_PACKET_COMM_STRU *pModBus = (MODBUS_PACKET_COMM_STRU *)aucModbusBuf;

    iIdx = 0;
    /* 0. Node Address (2018/03/04 add for zigbee) */
    pModBus->ucAddress = APP_PAKCET_ADDRESS_FM; 
    pModBus->ucFuncode = MODBUS_FUNC_CODE_Mask_Write_4X_Register;
    
    /* 1.Reg Address */
    pModBus->aucData[iIdx++] = (ulAddress >> 8) & 0XFF; 
    pModBus->aucData[iIdx++] = (ulAddress >> 0) & 0XFF;
    
    /* 2.Mask Value */
    pModBus->aucData[iIdx++] = (ulMask >> 8) & 0XFF;
    pModBus->aucData[iIdx++] = (ulMask >> 0) & 0XFF;
    
    /* 3. Value */
    pModBus->aucData[iIdx++] = (ulValue >> 8) & 0XFF;
    pModBus->aucData[iIdx++] = (ulValue >> 0) & 0XFF;
    
    CAN_BUILD_ADDRESS_IDENTIFIER(ulIdentifier,APP_PAKCET_ADDRESS_MC,APP_PAKCET_ADDRESS_FM);
    
    iRet = CcbModbusWorkEntryWrapper(id,0,ulIdentifier,aucModbusBuf,iIdx + MODBUS_PACKET_COMM_HDR_LEN,2000,CcbDefaultModbusCallBack); // don care modbus exe result
    
    if (iRet )
    {
        VOS_LOG(VOS_LOG_WARNING,"CcbModbusWorkEntry Fail %d",iRet);    
        /* notify ui (late implemnt) */
        return iRet;
    }

    CcbUpdateFmObjState(ulMask,ulValue);

    return iRet;
}


int CcbSetExeHoldRegs(int id,unsigned int ulAddress, unsigned int ulValue)
{
    unsigned int ulIdentifier;

    unsigned char aucModbusBuf[16];

    int iRet = -1;

    int iIdx = 0;

    MODBUS_PACKET_COMM_STRU *pModBus = (MODBUS_PACKET_COMM_STRU *)aucModbusBuf;

    iIdx = 0;
    /* 0. Node Address (2018/03/04 add for zigbee) */
    pModBus->ucAddress       = APP_PAKCET_ADDRESS_EXE; 
    pModBus->ucFuncode       = MODBUS_FUNC_CODE_Preset_Multiple_Registers;
    
    /* 1.Reg Address */
    pModBus->aucData[iIdx++] = (ulAddress >> 8) & 0XFF; 
    pModBus->aucData[iIdx++] = (ulAddress >> 0) & 0XFF;

    /* 1.Reg NUM */
    pModBus->aucData[iIdx++] = (1 >> 8) & 0XFF; 
    pModBus->aucData[iIdx++] = (1 >> 0) & 0XFF;

    /* byte counts */
    pModBus->aucData[iIdx++] = 2;
    
    /* 2.Value   */
    pModBus->aucData[iIdx++] = (ulValue >> 8) & 0XFF;
    pModBus->aucData[iIdx++] = (ulValue >> 0) & 0XFF;
    
    CAN_BUILD_ADDRESS_IDENTIFIER(ulIdentifier,APP_PAKCET_ADDRESS_MC,APP_PAKCET_ADDRESS_EXE);
    
    iRet = CcbModbusWorkEntryWrapper(id,0,ulIdentifier,aucModbusBuf,iIdx + MODBUS_PACKET_COMM_HDR_LEN,2000,CcbDefaultModbusCallBack); // don care modbus exe result
    
    if (iRet )
    {
        VOS_LOG(VOS_LOG_WARNING,"CcbModbusWorkEntry Fail %d",iRet);    
        /* notify ui (late implemnt) */
        return iRet;
    }

    /* update exe */
    for (iIdx = 0; iIdx < 1; iIdx++)
    {
       gCcb.ExeBrd.ausHoldRegister[ulAddress] = ulValue;

       if (ulAddress >= APP_EXE_HOLD_REG_RPUMP_OFFSET)
       {
           CcbUpdateRPumpObjState(ulAddress - APP_EXE_HOLD_REG_RPUMP_OFFSET ,ulValue);
       }
       
    }
    return iRet;
}



int CcbGetExeHoldRegs(int id,unsigned int ulAddress,int Regs)
{
    unsigned int ulIdentifier;

    unsigned char aucModbusBuf[16];

    int iRet = -1;

    int iIdx = 0;

    MODBUS_PACKET_COMM_STRU *pModBus = (MODBUS_PACKET_COMM_STRU *)aucModbusBuf;

    iIdx = 0;
    /* 0. Node Address (2018/03/04 add for zigbee) */
    pModBus->ucAddress = APP_PAKCET_ADDRESS_EXE; 
    pModBus->ucFuncode       = MODBUS_FUNC_CODE_Read_Holding_Registers;
    
    /* 1.Reg Address */
    pModBus->aucData[iIdx++] = (ulAddress >> 8) & 0XFF; 
    pModBus->aucData[iIdx++] = (ulAddress >> 0) & 0XFF;

    /* 1.Reg NUM */
    pModBus->aucData[iIdx++] = (Regs >> 8) & 0XFF; 
    pModBus->aucData[iIdx++] = (Regs >> 0) & 0XFF;
    
    CAN_BUILD_ADDRESS_IDENTIFIER(ulIdentifier,APP_PAKCET_ADDRESS_MC,APP_PAKCET_ADDRESS_EXE);
    
    iRet = CcbModbusWorkEntryWrapper(id,0,ulIdentifier,aucModbusBuf,iIdx + MODBUS_PACKET_COMM_HDR_LEN,2000,CcbDefaultModbusCallBack); // don care modbus exe result
    
    if (iRet )
    {
        VOS_LOG(VOS_LOG_WARNING,"CcbModbusWorkEntry Fail %d",iRet);    
        /* notify ui (late implemnt) */
        return iRet;
    }

    /* decode modebus contents */
    {
        int iTmp = MODBUS_PACKET_COMM_HDR_LEN + 1; /* 1byet for code + 1byte for len */
        for (iIdx = 0; iIdx < Regs; iIdx++)
        {
            gCcb.ExeBrd.ausHoldRegister[ulAddress + iIdx] = CcbGetModbus2BytesData(iTmp);
            iTmp += 2;
        }
    }

    return iRet;
}


int CcbGetPmValue(int id,unsigned int ulAddress, int iPMs)
{
    int          iIdx = 0;
    int          iTmp;
    int          iRet;
    unsigned int ulIdentifier;
    unsigned char aucModbusBuf[32];
    

    MODBUS_PACKET_COMM_STRU *pModBus = (MODBUS_PACKET_COMM_STRU *)aucModbusBuf;

    /* get Bx from exe */
    iIdx = 0;
    /* 0. Node Address (2018/03/04 add for zigbee) */
    pModBus->ucAddress = APP_PAKCET_ADDRESS_EXE; 
    pModBus->ucFuncode = MODBUS_FUNC_CODE_Read_Input_Registers;
   
    /* 1.Start Address */
    pModBus->aucData[iIdx++] = (ulAddress >> 8) & 0XFF; 
    pModBus->aucData[iIdx++] = (ulAddress >> 0) & 0XFF;
    
    
    /* 2. Regs  */
    iTmp = iPMs;
    pModBus->aucData[iIdx++] = (iTmp >> 8) & 0XFF;
    pModBus->aucData[iIdx++] = (iTmp >> 0) & 0XFF;
    
    CAN_BUILD_ADDRESS_IDENTIFIER(ulIdentifier,APP_PAKCET_ADDRESS_MC,APP_PAKCET_ADDRESS_EXE);

    
    iRet = CcbModbusWorkEntryWrapper(id,0,ulIdentifier,aucModbusBuf,iIdx + MODBUS_PACKET_COMM_HDR_LEN,2000,CcbDefaultModbusCallBack); // don care modbus exe result
    
    if (iRet )
    {
        VOS_LOG(VOS_LOG_WARNING,"CcbModbusWorkEntry Fail %d",iRet);    
        /* notify ui (late implemnt) */
        return iRet;
    }

    iTmp = MODBUS_PACKET_COMM_HDR_LEN + 1; /* 1byet for code + 1byte for len */

    for (iIdx = 0; iIdx < iPMs; iIdx++)
    {
        if (APP_EXE_PM1_NO + ulAddress + iIdx < APP_EXE_PRESSURE_METER)
        {
            gCcb.ExeBrd.aPMObjs[APP_EXE_PM1_NO + ulAddress + iIdx].Value.ulV = CcbGetModbus2BytesData(iTmp);
        }
        iTmp += 2;
    }

    return 0;

}

int CcbGetIValue(int id,unsigned int ulAddress, int iIs)
{
    int          iIdx = 0;
    int          iTmp;
    int          iRet;
    unsigned int ulIdentifier;
    unsigned char aucModbusBuf[32];
    

    MODBUS_PACKET_COMM_STRU *pModBus = (MODBUS_PACKET_COMM_STRU *)aucModbusBuf;

    /* get Bx from exe */
    iIdx = 0;
    /* 0. Node Address (2018/03/04 add for zigbee) */
    pModBus->ucAddress = APP_PAKCET_ADDRESS_EXE; 
    pModBus->ucFuncode = MODBUS_FUNC_CODE_Read_Input_Registers;
   
    /* 1.Start Address */
    pModBus->aucData[iIdx++] = (ulAddress >> 8) & 0XFF; 
    pModBus->aucData[iIdx++] = (ulAddress >> 0) & 0XFF;
    
    
    /* 2. Regs  */
    iTmp = iIs;
    pModBus->aucData[iIdx++] = (iTmp >> 8) & 0XFF;
    pModBus->aucData[iIdx++] = (iTmp >> 0) & 0XFF;
    
    CAN_BUILD_ADDRESS_IDENTIFIER(ulIdentifier,APP_PAKCET_ADDRESS_MC,APP_PAKCET_ADDRESS_EXE);

    
    iRet = CcbModbusWorkEntryWrapper(id,0,ulIdentifier,aucModbusBuf,iIdx + MODBUS_PACKET_COMM_HDR_LEN,2000,CcbDefaultModbusCallBack); // don care modbus exe result
    
    if (iRet )
    {
        VOS_LOG(VOS_LOG_WARNING,"CcbModbusWorkEntry Fail %d",iRet);    
        /* notify ui (late implemnt) */
        return iRet;
    }

    iTmp = MODBUS_PACKET_COMM_HDR_LEN + 1; /* 1byet for code + 1byte for len */

    ulAddress -= APP_EXE_INPUT_REG_QMI_OFFSET;

    for (iIdx = 0; iIdx < iIs; iIdx++)
    {
        if (APP_EXE_I1_NO + ulAddress + iIdx < APP_EXE_ECO_NUM)
        {
            un_data_type data;

            ulIdentifier = CcbGetModbus2BytesData(iTmp);
            data.auc[3] = (ulIdentifier >> 8) & 0xff;
            data.auc[2] = (ulIdentifier >> 0) & 0xff;
            ulIdentifier = CcbGetModbus2BytesData(iTmp+2);
            data.auc[1] = (ulIdentifier >> 8) & 0xff;
            data.auc[0] = (ulIdentifier >> 0) & 0xff;
            gCcb.ExeBrd.aEcoObjs[APP_EXE_I1_NO + ulAddress + iIdx].Value.eV.fWaterQ = data.f;

            ulIdentifier = CcbGetModbus2BytesData(iTmp+2);
            gCcb.ExeBrd.aEcoObjs[APP_EXE_I1_NO + ulAddress + iIdx].Value.eV.usTemp = (unsigned short)ulIdentifier;
            
        }
        iTmp += 6;
    }

    return 0;

}

int CcbGetFmValue(int id,unsigned int ulAddress, int iFMs)
{
    int          iIdx = 0;
    int          iTmp;
    int          iRet;
    unsigned int ulIdentifier;
    unsigned char aucModbusBuf[32];
    

    MODBUS_PACKET_COMM_STRU *pModBus = (MODBUS_PACKET_COMM_STRU *)aucModbusBuf;

    /* get Bx from exe */
    iIdx = 0;
    /* 0. Node Address (2018/03/04 add for zigbee) */
    pModBus->ucAddress = APP_PAKCET_ADDRESS_FM; 
    pModBus->ucFuncode = MODBUS_FUNC_CODE_Read_Input_Registers;
   
    /* 1.Start Address */
    pModBus->aucData[iIdx++] = (ulAddress >> 8) & 0XFF; 
    pModBus->aucData[iIdx++] = (ulAddress >> 0) & 0XFF;
    
    
    /* 2. Regs  */
    iTmp = iFMs*2; 
    pModBus->aucData[iIdx++] = (iTmp >> 8) & 0XFF;
    pModBus->aucData[iIdx++] = (iTmp >> 0) & 0XFF;
    
    CAN_BUILD_ADDRESS_IDENTIFIER(ulIdentifier,APP_PAKCET_ADDRESS_MC,APP_PAKCET_ADDRESS_FM);

    
    iRet = CcbModbusWorkEntryWrapper(id,0,ulIdentifier,aucModbusBuf,iIdx + MODBUS_PACKET_COMM_HDR_LEN,2000,CcbDefaultModbusCallBack); // don care modbus exe result
    
    if (iRet )
    {
        VOS_LOG(VOS_LOG_WARNING,"CcbModbusWorkEntry Fail %d",iRet);    
        /* notify ui (late implemnt) */
        return iRet;
    }

    iTmp = MODBUS_PACKET_COMM_HDR_LEN + 1; 

    for (iIdx = 0; iIdx < iFMs; iIdx++)
    {
        if (APP_FM_FM1_NO + ulAddress + iIdx < APP_EXE_PRESSURE_METER)
        {
            gCcb.FlowMeter.aFmObjs[APP_FM_FM1_NO + ulAddress + iIdx].Value.ulV = CcbGetModbus4BytesData(iTmp);
        }
        iTmp += 4;
    }

    return 0;

}

int CcbGetDinState(int id)
{
    int          iIdx = 0;
    int          iTmp;
    int          iRet;
    unsigned int ulIdentifier;
    unsigned char aucModbusBuf[32];
    
    MODBUS_PACKET_COMM_STRU *pModBus = (MODBUS_PACKET_COMM_STRU *)aucModbusBuf;

    /* get Bx from exe */
    iIdx = 0;
    /* 0. Node Address (2018/03/04 add for zigbee) */
    pModBus->ucAddress = APP_PAKCET_ADDRESS_EXE; 
    pModBus->ucFuncode = MODBUS_FUNC_CODE_Read_Input_Registers;
   
    /* 1.Start Address */
    pModBus->aucData[iIdx++] = (APP_EXE_INPUT_REG_DIN_OFFSET >> 8) & 0XFF; 
    pModBus->aucData[iIdx++] = (APP_EXE_INPUT_REG_DIN_OFFSET >> 0) & 0XFF;
    
    
    /* 2.Regs  */
    iTmp = 1;
    pModBus->aucData[iIdx++] = (iTmp >> 8) & 0XFF;
    pModBus->aucData[iIdx++] = (iTmp >> 0) & 0XFF;
    
    CAN_BUILD_ADDRESS_IDENTIFIER(ulIdentifier,APP_PAKCET_ADDRESS_MC,APP_PAKCET_ADDRESS_EXE);

    iRet = CcbModbusWorkEntryWrapper(id,0,ulIdentifier,aucModbusBuf,iIdx + MODBUS_PACKET_COMM_HDR_LEN,2000,CcbDefaultModbusCallBack); // don care modbus exe result
    
    if (iRet )
    {
        VOS_LOG(VOS_LOG_WARNING,"CcbModbusWorkEntry Fail %d",iRet);    
        /* notify ui (late implemnt) */
        return iRet;
    }

    iTmp = MODBUS_PACKET_COMM_HDR_LEN + 1; 

    gCcb.ExeBrd.ucDinState = CcbGetModbus2BytesData(iTmp);

    gCcb.ExeBrd.ucDinState = (~(gCcb.ExeBrd.ucDinState)) & APP_EXE_DIN_MASK;

    return 0;
}


void CanCcbTransState(int iDstMainState,int iDstSubState)
{


    int aiCont[2] = {iDstMainState,iDstSubState};

    VOS_LOG(VOS_LOG_WARNING,"Main %d & Sub %d",iDstMainState,iDstSubState);    
    
    gCcb.curWorkState.iMainWorkState = iDstMainState;
    gCcb.curWorkState.iSubWorkState  = iDstSubState;

    /* do stuff here */
    switch(iDstMainState)
    {
    case DISP_WORK_STATE_RUN:
        switch(iDstSubState)
        {
        case DISP_WORK_SUB_IDLE:
            //gCcb.ulB2FullTick = gulSecond;
            break;
        }
        break;
    case DISP_WORK_STATE_LPP:
        gCcb.ulLPPTick = gulSecond;
        break;
    default:
        break;
    }

    /* Notify Ui */
    // if (((int)&gCcb) != tls_getkey(&pthread_tno_key))
    {
        MainSndWorkMsg(WORK_MSG_TYPE_STATE,(unsigned char *)aiCont,sizeof(aiCont));
    }
}


void CanCcbTransState4Pw(int iDstMainState,int iDstSubState)
{
    int aiCont[2] = {iDstMainState,iDstSubState};

    gCcb.curWorkState.iMainWorkState4Pw = iDstMainState;
    gCcb.curWorkState.iSubWorkState4Pw  = iDstSubState;

    MainSndWorkMsg(WORK_MSG_TYPE_STATE4PW,(unsigned char *)aiCont,sizeof(aiCont));
}


DISPHANDLE SearchWork(work_proc proc)
{
    int iLoop;
    sp_thread_mutex_lock(&gCcb.WorkMutex);
    for (iLoop = 0; iLoop < WORK_LIST_NUM; iLoop++)
    {
        list_t *pos, *n;
        list_for_each_safe(pos,n,&gCcb.WorkList[iLoop])
        {
            WORK_ITEM_STRU *pWorkItem = list_entry(pos,WORK_ITEM_STRU,list) ;

            if ((pWorkItem->proc == proc)
                && (!(pWorkItem->flag & WORK_FLAG_CANCEL)))
            {
                sp_thread_mutex_unlock(&gCcb.WorkMutex);
                
                return (DISPHANDLE)pWorkItem;
            }
        }
    }
    sp_thread_mutex_unlock(&gCcb.WorkMutex);

    return DISP_INVALID_HANDLE; 
}

/*
  Protect by Mutex
*/
void CcbDefaultCancelWork(void *param)
{
    WORK_ITEM_STRU *pWorkItem = (WORK_ITEM_STRU *)param;

    CcbAbortWork(pWorkItem);
}

WORK_ITEM_STRU *CcbAllocWorkItem(void)
{
   WORK_ITEM_STRU *pWorkItem = (WORK_ITEM_STRU *)malloc(sizeof(WORK_ITEM_STRU));

   if (!pWorkItem) return NULL;

   memset(pWorkItem,0,sizeof(WORK_ITEM_STRU));

   /* Do all Init stuff here */
   INIT_LIST_HEAD(&pWorkItem->list);

   
   pWorkItem->proc   = NULL;
   pWorkItem->cancel = CcbDefaultCancelWork;
   pWorkItem->flag   = 0;
   pWorkItem->id     = 0;
   pWorkItem->para   = NULL;
   pWorkItem->extra  = NULL;


   return pWorkItem;
}


void CcbScheduleWork(void)
{
     int iLoop;
     
     for (iLoop = 0; iLoop < WORK_LIST_NUM; iLoop++)
     {
         if (gCcb.iBusyWork & (1 << iLoop))
         {
             continue;
         }

         if (!list_empty(&gCcb.WorkList[iLoop]))
         {
             int iRet;
             WORK_ITEM_STRU *pWorkItem = list_entry(gCcb.WorkList[iLoop].next,WORK_ITEM_STRU,list) ;

             iRet = Task_DispatchWorkTask(WorkCommEntry,pWorkItem);
             if (iRet)
             {
                 /* dispatch failed */
                 VOS_LOG(VOS_LOG_WARNING,"dispatch_threadpool Fail %d",iRet);    
             }
         }
     }

     /* notify ui (late implement) */
}


void CcbAddWork(int iList,WORK_ITEM_STRU *pWorkItem)
{
    sp_thread_mutex_lock(&gCcb.WorkMutex);
    pWorkItem->id = iList;
    list_add_tail(&pWorkItem->list,&gCcb.WorkList[iList]);
    sp_thread_mutex_unlock(&gCcb.WorkMutex);
     
    CcbScheduleWork();
     
}

void WorkCommEntry(void *para)
{
    WORK_ITEM_STRU *pWorkItem = (WORK_ITEM_STRU *)para;

    /* prevent repeated excuting */
    sp_thread_mutex_lock(&gCcb.WorkMutex);
    if (gCcb.iBusyWork & (1 << pWorkItem->id))
    {
        sp_thread_mutex_unlock(&gCcb.WorkMutex);
        return ;
    }
    gCcb.iBusyWork |= 1 << pWorkItem->id;
    sp_thread_mutex_unlock(&gCcb.WorkMutex);

    pWorkItem->flag |= WORK_FLAG_ACTIVE;

    if (!(pWorkItem->flag & WORK_FLAG_CANCEL))
    {
        pWorkItem->proc(pWorkItem);
    }

    sp_thread_mutex_lock(&gCcb.WorkMutex);
    gCcb.iBusyWork &= ~(1 << pWorkItem->id);
    list_del_init(&pWorkItem->list);
    sp_thread_mutex_unlock(&gCcb.WorkMutex);

    free(pWorkItem);

    CcbScheduleWork();
}

/*********************************************************************
 * Function:        void work_stop_pw(void *para)
 *
 * PreCondition:    None
 *
 * Input:           para  type of WORK_ITEM_STRU 
 *
 * Output:          Stop producing water
 *
 * Side Effects:    None
 *
 * Overview:        Stop producing water
 *
 * Note:            None.
 ********************************************************************/
void work_stop_pw(void *para)
{
    int iTmp;

    int iRet;

    int aiCont[1] = {0};

    WORK_ITEM_STRU *pWorkItem = (WORK_ITEM_STRU *)para;

    CCB *pCcb = (CCB *)pWorkItem->para;
    
    iTmp = pCcb->ulRunMask;

    iRet = CcbUpdateSwitch(pWorkItem->id,0,iTmp,0);
    
    if (iRet )
    {
        VOS_LOG(VOS_LOG_WARNING,"CcbUpdateSwitch Fail %d",iRet);    
        /* notify ui (late implemnt) */
    }

    //2018.11.22
    switch(pCcb->ulMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_L_EDI_LOOP:
    case MACHINE_Genie:
    case MACHINE_EDI:
        iTmp  = (1 << APP_EXE_I1_NO)|(1<<APP_EXE_I2_NO)|(1<<APP_EXE_I3_NO);
        iRet = CcbUpdateIAndBs(pWorkItem->id, 0, iTmp, 0);
        if (iRet )
        {
            VOS_LOG(VOS_LOG_WARNING,"CcbUpdateIAndBs Fail %d",iRet);
            /* notify ui (late implemnt) */
        }
        break;
    case MACHINE_L_UP:
    case MACHINE_L_RO_LOOP:
    case MACHINE_UP:
    case MACHINE_RO:
        iTmp  = (1 << APP_EXE_I1_NO)|(1<<APP_EXE_I2_NO);
        iRet = CcbUpdateIAndBs(pWorkItem->id, 0, iTmp, 0);
        if (iRet )
        {
            VOS_LOG(VOS_LOG_WARNING,"CcbUpdateIAndBs Fail %d",iRet);
            /* notify ui (late implemnt) */
        }
        break;
    case MACHINE_PURIST:
        break;
    case MACHINE_ADAPT:
        break;
    }
    //end

    MainSndWorkMsg(WORK_MSG_TYPE_EPW,(unsigned char *)aiCont,sizeof(aiCont));

    pCcb->bit1ProduceWater        = FALSE; 
    pCcb->bit1B1Check4RuningState = FALSE;

    for (iTmp = 0; iTmp < APP_FM_FLOW_METER_NUM; iTmp++)
    {
       pCcb->FlowMeter.aulPwEnd[iTmp] = pCcb->FlowMeter.aHist[iTmp].curValue.ulV;
    }

}

DISPHANDLE CcbInnerWorkStopProduceWater(void)
{
    WORK_ITEM_STRU *pWorkItem = CcbAllocWorkItem();

    if (!pWorkItem)
    {
        return DISP_INVALID_HANDLE;
    }

    pWorkItem->proc = work_stop_pw;
    pWorkItem->para = &gCcb;

    CcbAddWork(WORK_LIST_LP,pWorkItem);

    return (DISPHANDLE)pWorkItem;

}

void work_fill_water_fail(int iWorkId)
{
    int aiCont[1] = {-1};
    
    if (CcbGetSwitchObjState((1 << APP_EXE_E10_NO)))
    {
        CcbUpdateSwitch(iWorkId,0,(1 << APP_EXE_E10_NO),0);
    }
    
    if (CcbGetPmObjState((1 << APP_EXE_PM3_NO)))
    {
        CcbUpdateIAndBs(iWorkId,0,GET_B_MASK(APP_EXE_PM3_NO),0);
    }
    
    MainSndWorkMsg(WORK_MSG_TYPE_SFW,(unsigned char *)aiCont,sizeof(aiCont));

}

void work_fill_water_succ(void)
{
    int aiCont[1] = {0};
    
    MainSndWorkMsg(WORK_MSG_TYPE_SFW,(unsigned char *)aiCont,sizeof(aiCont));

}


void work_start_fill_water(void *para)
{
    int iTmp;
    int iRet;

    WORK_ITEM_STRU *pWorkItem = (WORK_ITEM_STRU *)para;
    
    /* late implemnt */
    /* E10 ON*/
    /* set  valves */
    iTmp = (1 << APP_EXE_E10_NO);
    iRet = CcbUpdateSwitch(pWorkItem->id, 0,iTmp,iTmp);
    if (iRet )
    {
        VOS_LOG(VOS_LOG_WARNING,"CcbUpdateSwitch Fail %d",iRet);    
        work_fill_water_fail(pWorkItem->id);
        return ;
    }

    iTmp  = (GET_B_MASK(APP_EXE_PM3_NO));
    iRet = CcbUpdateIAndBs(pWorkItem->id,0,iTmp,iTmp);
    if (iRet )
    {
        VOS_LOG(VOS_LOG_WARNING,"CcbUpdateIAndBs Fail %d",iRet);    
        /* notify ui (late implemnt) */
        work_fill_water_fail(pWorkItem->id);
        return ;
    }     

    gCcb.bit1FillingTank = TRUE;

    work_fill_water_succ();
}


DISPHANDLE CcbInnerWorkStartFillWater(void)
{
    WORK_ITEM_STRU *pWorkItem = CcbAllocWorkItem();

    if (!pWorkItem)
    {
        return DISP_INVALID_HANDLE;
    }

    pWorkItem->proc = work_start_fill_water;
    pWorkItem->para = &gCcb;

    CcbAddWork(WORK_LIST_URGENT,pWorkItem);

    return (DISPHANDLE)pWorkItem;
}

void work_stop_water_succ(void)
{
    int aiCont[1] = {0};
    
    MainSndWorkMsg(WORK_MSG_TYPE_EFW,(unsigned char *)aiCont,sizeof(aiCont));

}

void work_stop_fill_water(void *para)
{
    int iTmp;
    int iRet;

    WORK_ITEM_STRU *pWorkItem = (WORK_ITEM_STRU *)para;
    
    /* late implemnt */
    /* E10 ON        */
    /* set  valves   */
    iTmp = (1 << APP_EXE_E10_NO);
    iRet = CcbUpdateSwitch(pWorkItem->id,0,iTmp,0);
    if (iRet )
    {
        VOS_LOG(VOS_LOG_WARNING,"CcbUpdateSwitch Fail %d",iRet);    
    }
    //2019.12.2 修改，停止取水时不需要关闭原水箱检测
#if 0
    iTmp  = (GET_B_MASK(APP_EXE_PM3_NO));
    iRet = CcbUpdateIAndBs(pWorkItem->id,0,iTmp,0);
    if (iRet )
    {
        VOS_LOG(VOS_LOG_WARNING,"CcbUpdateIAndBs Fail %d",iRet);    
        /* notify ui (late implemnt) */
    }     
#endif
    gCcb.bit1FillingTank = FALSE;
    gCcb.bit1NeedFillTank = FALSE;

    work_stop_water_succ();
    
}


DISPHANDLE CcbInnerWorkStopFillWater(void)
{
    WORK_ITEM_STRU *pWorkItem = CcbAllocWorkItem();

    if (!pWorkItem)
    {
        return DISP_INVALID_HANDLE;
    }

    pWorkItem->proc = work_stop_fill_water;
    pWorkItem->para = &gCcb;

    CcbAddWork(WORK_LIST_URGENT,pWorkItem);

    return (DISPHANDLE)pWorkItem;
}


void work_N3_fail(int iWorkId)
{
    int aiCont[1] = {-1};
    
    if (CcbGetSwitchObjState((1 << APP_EXE_N3_NO)))
    {
        CcbUpdateSwitch(iWorkId,0,(1 << APP_EXE_N3_NO),0);
    }
    
    MainSndWorkMsg(WORK_MSG_TYPE_SN3,(unsigned char *)aiCont,sizeof(aiCont));

}

void work_N3_succ(void)
{
    int aiCont[1] = {0};
    
    MainSndWorkMsg(WORK_MSG_TYPE_SN3,(unsigned char *)aiCont,sizeof(aiCont));

}


void work_start_N3(void *para)
{
    int iTmp;
    int iRet;
    WORK_ITEM_STRU *pWorkItem = (WORK_ITEM_STRU *)para;
    
    /* late implemnt */
    /* E10 ON*/
    /* set  valves */
    iTmp = (1 << APP_EXE_N3_NO);
    iRet = CcbUpdateSwitch(pWorkItem->id,0,iTmp,iTmp);
    if (iRet )
    {
        VOS_LOG(VOS_LOG_WARNING,"CcbUpdateSwitch Fail %d",iRet);  
        
        work_N3_fail(pWorkItem->id);
        return ;
    }
    work_N3_succ();
}


DISPHANDLE CcbInnerWorkStartN3(void)
{
    WORK_ITEM_STRU *pWorkItem = CcbAllocWorkItem();

    if (!pWorkItem)
    {
        return DISP_INVALID_HANDLE;
    }

    pWorkItem->proc = work_start_N3;
    pWorkItem->para = &gCcb;

    CcbAddWork(WORK_LIST_URGENT,pWorkItem);

    return (DISPHANDLE)pWorkItem;
}

void work_stop_N3_succ(void)
{
    int aiCont[1] = {0};
    
    MainSndWorkMsg(WORK_MSG_TYPE_EN3,(unsigned char *)aiCont,sizeof(aiCont));

}

void work_stop_N3(void *para)
{
    int iTmp;
    int iRet;

    WORK_ITEM_STRU *pWorkItem = (WORK_ITEM_STRU *)para;
    
    /* late implemnt */
    /* E10 ON        */
    /* set  valves   */
    iTmp = (1 << APP_EXE_N3_NO);
    iRet = CcbUpdateSwitch(pWorkItem->id,0,iTmp,0);
    if (iRet )
    {
        VOS_LOG(VOS_LOG_WARNING,"CcbSetSwitch Fail %d",iRet);    
    }

    work_stop_N3_succ();
    
}


DISPHANDLE CcbInnerWorkStopN3(void)
{
    WORK_ITEM_STRU *pWorkItem = CcbAllocWorkItem();

    if (!pWorkItem)
    {
        return DISP_INVALID_HANDLE;
    }

    pWorkItem->proc = work_stop_N3;
    pWorkItem->para = &gCcb;

    CcbAddWork(WORK_LIST_URGENT,pWorkItem);

    return (DISPHANDLE)pWorkItem;
}


void work_qtw_fail(CCB *pCcb,int code,int iIndex,int iWorkId)
{
    int aiCont[2] = {code,iIndex};

    unsigned int ulTmp;

    if (APP_DEV_HS_SUB_HYPER == pCcb->aHandler[iIndex].iDevType)
    {
        ulTmp = pCcb->ulHyperTwMask;
    }
    else
    {
        ulTmp = pCcb->ulNormalTwMask;
    }
    
    if (CcbGetSwitchObjState(APP_EXE_SWITCHS_MASK))
    {
        CcbUpdateSwitch(iWorkId,0,ulTmp,0);
    }
    
    if (CcbGetIObjState((1 << APP_EXE_I4_NO)|(1 << APP_EXE_I5_NO)))
    {
        CcbUpdateIAndBs(iWorkId,0,(1 << APP_EXE_I4_NO)|(1 << APP_EXE_I5_NO),0);
    }

    if (CcbGetFmObjState(APP_FM_FM1_NO))
    {
        CcbUpdateFms(iWorkId,0,APP_FM_FM1_NO,0);
    }

    CanCcbTransState4Pw(DISP_WORK_STATE_RUN,DISP_WORK_SUB_IDLE);

    MainSndWorkMsg(WORK_MSG_TYPE_START_QTW,(unsigned char *)aiCont,sizeof(aiCont));

}

void work_qtw_succ(int iIndex)
{
    int aiCont[2] = {0,iIndex};
    
    MainSndWorkMsg(WORK_MSG_TYPE_START_QTW,(unsigned char *)aiCont,sizeof(aiCont));
}

void work_stop_qtw_succ(int iIndex)
{
    int aiCont[2] = {0,iIndex};
    
    MainSndWorkMsg(WORK_MSG_TYPE_STOP_QTW,(unsigned char *)aiCont,sizeof(aiCont));

}

void work_start_qtw(void *para)
{
    WORK_ITEM_STRU *pWorkItem = (WORK_ITEM_STRU *)para;

    CCB *pCcb = (CCB *)pWorkItem->para;
    int iIndex = (int)pWorkItem->extra;
    unsigned int ulQtwSwMask = 0;
    int iTmp;
    int iRet;

    pCcb->bit1B2Empty = FALSE;
    pCcb->bit1AlarmUP = FALSE;

    CcbInitHandlerQtwMeas(iIndex);

    /* Processing */
    CanCcbTransState4Pw(DISP_WORK_STATE_RUN,DISP_WORK_SUB_RUN_QTWING);

    //2018.10.26 add
    if(pCcb->aHandler[iIndex].iDevType == APP_DEV_HS_SUB_HYPER)
    {
        ex_gCcb.Ex_Auto_Cir_Tick.ulUPAutoCirTick = ex_gulSecond; //UP Auto cir 60min
    }

    switch(pCcb->ulMachineType)
    {
    case MACHINE_ADAPT:
         {   
            int iLoop;

            CanCcbTransState(DISP_WORK_STATE_RUN,DISP_WORK_SUB_RUN_INIT);//2019.3.29 add

            /* 2018/01/05 add extra 10 seconds for flush according to ZHANG Chunhe */
            if ((gulSecond - pCcb->ulAdapterAgingCount > 60))
            {
                gCcb.aHandler[iIndex].bit1PendingQtw = 1;
                
                /* 2018/01/05 add E3 according to ZHANG */
                if(haveB3(&gCcb))
                {
                    iTmp = (1 << APP_EXE_E1_NO)|(1<<APP_EXE_E3_NO)|(1 << APP_EXE_C3_NO);
                }
                else
                {
                    iTmp = (1 << APP_EXE_E1_NO)|(1<<APP_EXE_E3_NO)|(1 << APP_EXE_E10_NO)|(1 << APP_EXE_C3_NO);
                }
                
                iRet = CcbUpdateSwitch(pWorkItem->id,0,pCcb->ulRunMask,iTmp);
                if (iRet )
                {
                    VOS_LOG(VOS_LOG_WARNING,"CcbUpdateSwitch Fail %d",iRet);
            
                    work_qtw_fail(pCcb,APP_PACKET_HO_ERROR_CODE_UNKNOW,iIndex,pWorkItem->id);        
                    
                    /* notify ui (late implemnt) */
                    return ;
                }
                
                VOS_LOG(VOS_LOG_WARNING,"E1,E3 ON"); 
                
        
                pCcb->bit3RuningState = NOT_RUNING_STATE_CLEAN;
                
                CcbNotState(NOT_STATE_OTHER);
                
                /* E1,E3 ON*/
                /* set  valves */
                if(haveB3(&gCcb))
                {
                    iTmp = (1 << APP_EXE_E1_NO)|(1<<APP_EXE_E3_NO)|(1 << APP_EXE_C3_NO);
                }
                else
                {
                    iTmp = (1 << APP_EXE_E1_NO)|(1<<APP_EXE_E3_NO)|(1<<APP_EXE_E10_NO)|(1<<APP_EXE_C3_NO);
                }

                iRet = CcbUpdateSwitch(pWorkItem->id,0,pCcb->ulRunMask,iTmp);
                if (iRet )
                {
                    VOS_LOG(VOS_LOG_WARNING,"CcbUpdateSwitch Fail %d",iRet);    
                    /* notify ui (late implemnt) */
                    work_qtw_fail(pCcb,APP_PACKET_HO_ERROR_CODE_UNKNOW,iIndex,pWorkItem->id);        
                    return ;
                }
                
                for (iLoop = 0; iLoop < 10; iLoop++) //10
                {
                    /* get B1 reports from exe */
                    iRet = CcbGetPmValue(pWorkItem->id,APP_EXE_PM1_NO,1);
                    if (iRet )
                    {
                        VOS_LOG(VOS_LOG_WARNING,"CcbGetPmValue Fail %d",iRet);    
                        /* notify ui (late implemnt) */
                        work_qtw_fail(pCcb,APP_PACKET_HO_ERROR_CODE_UNKNOW,iIndex,pWorkItem->id);        
                        return ;
                    }    
            
                    CcbNotSWPressure();
            
                    iRet = CcbWorkDelayEntry(pWorkItem->id,1000,CcbDelayCallBack);
                    if (iRet )
                    {
                        VOS_LOG(VOS_LOG_WARNING,"CcbWorkDelayEntry Fail %d",iRet);
                        
                        work_qtw_fail(pCcb,APP_PACKET_HO_ERROR_CODE_UNKNOW,iIndex,pWorkItem->id);        
                        
                        return;
                    }
            
                    
                }
                if (CcbConvert2Pm1Data(pCcb->ExeBrd.aPMObjs[APP_EXE_PM1_NO].Value.ulV) < CcbGetSp1())
                {
                    /* 1. ui promote */
                    iTmp = 0; 
                    iRet = CcbUpdateSwitch(pWorkItem->id,0,pCcb->ulRunMask,iTmp);
                    if (iRet )
                    {
                        VOS_LOG(VOS_LOG_WARNING,"CcbSetSwitch Fail %d",iRet);
            
                        work_qtw_fail(pCcb,APP_PACKET_HO_ERROR_CODE_UNKNOW,iIndex,pWorkItem->id);        
                        return;
                    }
            
                    /* fire alarm */
                    if (!gCcb.bit1AlarmTapInPress)
                    {
                        gCcb.bit1AlarmTapInPress   = TRUE;
                
                        gCcb.ulFiredAlarmFlags |= ALARM_TLP;
                
                        CcbNotAlarmFire(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_SOURCE_WATER_PRESSURE,TRUE);
                    }
        
                    VOS_LOG(VOS_LOG_WARNING," DISP_WORK_STATE_LPP %d",iRet);  
                
                    CanCcbTransState(DISP_WORK_STATE_LPP,DISP_WORK_SUB_IDLE);
                            
                    gCcb.aHandler[iIndex].bit1PendingQtw = 0;

                    return;
                }
        
                /* 2018/01/05 begin : add for B1 under pressure check according to ZHANG chunhe */
                gCcb.bit1B1Check4RuningState  = TRUE;  
                /* 2018/01/05 end : add for B1 under pressure check */
                
                 /* clear alarm */
                if (gCcb.bit1AlarmTapInPress)
                {
                    gCcb.bit1AlarmTapInPress   = FALSE;
               
                    gCcb.ulFiredAlarmFlags &= ~ALARM_TLP;
               
                    CcbNotAlarmFire(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_SOURCE_WATER_PRESSURE,FALSE);
                } 
                
                if (APP_DEV_HS_SUB_HYPER == pCcb->aHandler[iIndex].iDevType)
                {
                    VOS_LOG(VOS_LOG_WARNING,"E1,E3,E5,C1,C2 ON; I1b,I2,I4,I5,B1;"); 
                    /*E1,E3,E5,C1,C2 ON; I1b,I2,I4,I5,B1;*/
                    if(haveB3(&gCcb))
                    {
                        iTmp = (1 << APP_EXE_E1_NO)|(1 << APP_EXE_E3_NO)|(1 << APP_EXE_E5_NO)
                              |(1 << APP_EXE_C1_NO)|(1 << APP_EXE_C2_NO)|(1 << APP_EXE_C3_NO);

                    }
                    else
                    {
                        iTmp = (1 << APP_EXE_E1_NO)|(1 << APP_EXE_E3_NO)|(1 << APP_EXE_E5_NO)
                              |(1 << APP_EXE_C1_NO)|(1 << APP_EXE_C2_NO)|(1 << APP_EXE_E10_NO)|(1 << APP_EXE_C3_NO);
                    }

                    iRet = CcbUpdateSwitch(pWorkItem->id,0,pCcb->ulRunMask,iTmp);
                    if (iRet )
                    {
                        VOS_LOG(VOS_LOG_WARNING,"CcbModbusWorkEntry Fail %d",iRet);    
                        /* notify ui (late implemnt) */
                        work_qtw_fail(pCcb,APP_PACKET_HO_ERROR_CODE_UNKNOW,iIndex,pWorkItem->id);        
                        return ;
                    }

                    if ((pCcb->ulHyperTwMask & (1 << APP_EXE_C2_NO))
                       && pCcb->bit1CirSpeedAdjust)
                    {
                        CcbC2Regulator(pWorkItem->id,8,TRUE);
                    }

                    iTmp = (1 << APP_EXE_I1_NO)|(1 << APP_EXE_I2_NO)|(1 << APP_EXE_I4_NO)|(1 << APP_EXE_I5_NO)|(GET_B_MASK(APP_EXE_PM1_NO));
                    iRet = CcbUpdateIAndBs(pWorkItem->id,0,iTmp,iTmp);
                    if (iRet )
                    {
                        VOS_LOG(VOS_LOG_WARNING,"CcbSetIAndBs Fail %d",iRet);    
                        /* notify ui (late implemnt) */
                        work_qtw_fail(pCcb,APP_PACKET_HO_ERROR_CODE_UNKNOW,iIndex,pWorkItem->id);        
                        return ;
                    }   
                }
                else /* EDI */
                {
                    VOS_LOG(VOS_LOG_WARNING,"E1,E3,C1 ON; I1b,I2,B1;"); 
                    /*E1,E3,C1 ON; I1b,I2,B1*/
                    if(haveB3(&gCcb))
                    {
                        iTmp = (1 << APP_EXE_E1_NO)|(1<<APP_EXE_E3_NO)|(1<<APP_EXE_C1_NO)|(1<<APP_EXE_C3_NO);
                    }
                    else
                    {
                        iTmp = (1 << APP_EXE_E1_NO)|(1<<APP_EXE_E3_NO)|(1<<APP_EXE_C1_NO)|(1 << APP_EXE_E10_NO)|(1<<APP_EXE_C3_NO);
                    }

                    iRet = CcbUpdateSwitch(pWorkItem->id,0,pCcb->ulRunMask,iTmp);
                    if (iRet )
                    {
                        VOS_LOG(VOS_LOG_WARNING,"CcbModbusWorkEntry Fail %d",iRet);    
                        /* notify ui (late implemnt) */
                        work_qtw_fail(pCcb,APP_PACKET_HO_ERROR_CODE_UNKNOW,iIndex,pWorkItem->id);        
                        return ;
                    }
                    
                    iTmp = (1 << APP_EXE_I1_NO)|(1 << APP_EXE_I2_NO)|(GET_B_MASK(APP_EXE_PM1_NO));
                    iRet = CcbUpdateIAndBs(pWorkItem->id,0,iTmp,iTmp);
                    if (iRet )
                    {
                        VOS_LOG(VOS_LOG_WARNING,"CcbSetIAndBs Fail %d",iRet);    
                        /* notify ui (late implemnt) */
                        work_qtw_fail(pCcb,APP_PACKET_HO_ERROR_CODE_UNKNOW,iIndex,pWorkItem->id);        
                        return ;
                    }       
                }
                
                iRet = CcbWorkDelayEntry(pWorkItem->id,3000,CcbDelayCallBack);
                if (iRet )
                {
                    VOS_LOG(VOS_LOG_WARNING,"CcbWorkDelayEntry Fail %d",iRet);    
                    /* notify ui (late implemnt) */
                    work_qtw_fail(pCcb,APP_PACKET_HO_ERROR_CODE_UNKNOW,iIndex,pWorkItem->id);        
                    return ;
                }
            
                /* check data */
                {
                    int iValidCount = 0;
                    
                    float fRej ;
                    
                    /* check appromixly 5*60 seconds */
                    for (iLoop = 0; iLoop < (DEFAULT_REG_CHECK_DURATION/5) / DEFAULT_REJ_CHECK_PERIOD; iLoop++)
                    {
                        iRet = CcbWorkDelayEntry(pWorkItem->id,DEFAULT_REJ_CHECK_PERIOD*1000,CcbDelayCallBack);
                        if (iRet )
                        {
                            VOS_LOG(VOS_LOG_WARNING,"CcbWorkDelayEntry Fail %d",iRet);    
                            /* notify ui (late implemnt) */
                            work_qtw_fail(pCcb,APP_PACKET_HO_ERROR_CODE_UNKNOW,iIndex,pWorkItem->id);                
                            return ;
                        }  
                
                        fRej = CcbCalcREJ();
                
                        if ((fRej >= CcbGetSp2())
                            || (gCcb.ExeBrd.aEcoObjs[APP_EXE_I2_NO].Value.eV.fWaterQ < CcbGetSp3()))
                        {
                            iValidCount ++;
                
                            if (iValidCount >= DEFAULT_REJ_CHECK_NUMBER)
                            {
                                break;
                            }
                        }
                        else
                        {
                            iValidCount = 0;
                        }
                    }
                
//                    if (fRej < CcbGetSp2())
//                    {
//                        /*alarem */
//                        gCcb.bit1AlarmRej = TRUE;
//                        gCcb.ulAlarmRejTick = gulSecond;
//                    }
                
//                    if (gCcb.ExeBrd.aEcoObjs[APP_EXE_I2_NO].Value.eV.fWaterQ > CcbGetSp3())
//                    {
//                        gCcb.bit1AlarmRoPw   = TRUE;
//                        gCcb.ulAlarmRoPwTick = gulSecond;
//                    }
                    
                }

                pCcb->bit3RuningState = NOT_RUNING_STATE_NONE;
                CcbNotState(NOT_STATE_OTHER);

            }            
            //2019.7.30 add
            else
            {
                if (APP_DEV_HS_SUB_HYPER == pCcb->aHandler[iIndex].iDevType)
                {
                    iTmp = (1 << APP_EXE_I1_NO)|(1 << APP_EXE_I2_NO)|(1 << APP_EXE_I4_NO)|(1 << APP_EXE_I5_NO)|(GET_B_MASK(APP_EXE_PM1_NO));
                    iRet = CcbUpdateIAndBs(pWorkItem->id,0,iTmp,iTmp);
                    if (iRet )
                    {
                        VOS_LOG(VOS_LOG_WARNING,"CcbSetIAndBs Fail %d",iRet);
                        /* notify ui (late implemnt) */
                        work_qtw_fail(pCcb,APP_PACKET_HO_ERROR_CODE_UNKNOW,iIndex,pWorkItem->id);
                        return ;
                    }
                }
                else
                {
                    iTmp = (1 << APP_EXE_I1_NO)|(1 << APP_EXE_I2_NO)|(GET_B_MASK(APP_EXE_PM1_NO));
                    iRet = CcbUpdateIAndBs(pWorkItem->id,0,iTmp,iTmp);
                    if (iRet )
                    {
                        VOS_LOG(VOS_LOG_WARNING,"CcbSetIAndBs Fail %d",iRet);
                        /* notify ui (late implemnt) */
                        work_qtw_fail(pCcb,APP_PACKET_HO_ERROR_CODE_UNKNOW,iIndex,pWorkItem->id);
                        return ;
                    }
                }
            }

            /* produce water */
            if (APP_DEV_HS_SUB_HYPER == pCcb->aHandler[iIndex].iDevType)
            {
                VOS_LOG(VOS_LOG_WARNING,"E1,C1,N2 ON I1b,I2,I4,I5,B1"); 
                if(haveB3(&gCcb))
                {
                    iTmp  = (1 << APP_EXE_E1_NO)|(1 << APP_EXE_C3_NO);
                }
                else
                {
                    iTmp  = (1 << APP_EXE_E1_NO)|(1 << APP_EXE_E10_NO)|(1 << APP_EXE_C3_NO);
                }

                iTmp |= (1<<APP_EXE_C1_NO);
                iTmp |= (1 << APP_EXE_N2_NO); 
            }
            else
            {
                VOS_LOG(VOS_LOG_WARNING,"E1,C1,ON I1b,I2,I4,I5,B1"); 
                if(haveB3(&gCcb))
                {
                    iTmp  = (1 << APP_EXE_E1_NO)|(1 << APP_EXE_C3_NO);
                }
                else
                {
                    iTmp  = (1 << APP_EXE_E1_NO)|(1 << APP_EXE_E10_NO)|(1 << APP_EXE_C3_NO);
                }

                iTmp |= (1<<APP_EXE_C1_NO);
            }
            
            iRet = CcbUpdateSwitch(pWorkItem->id,0,pCcb->ulRunMask,iTmp);
            if (iRet )
            {
                VOS_LOG(VOS_LOG_WARNING,"CcbSetSwitch Fail %d",iRet);    
                /* notify ui (late implemnt) */
                work_qtw_fail(pCcb,APP_PACKET_HO_ERROR_CODE_UNKNOW,iIndex,pWorkItem->id);        
                return ;
            }
     
            iTmp  = (1<<APP_FM_FM1_NO);
            iRet = CcbUpdateFms(pWorkItem->id,0,iTmp,iTmp);
            if (iRet )
            {
                VOS_LOG(VOS_LOG_WARNING,"CcbUpdateFms Fail %d",iRet);    
                /* notify ui (late implemnt) */
                work_qtw_fail(pCcb,APP_PACKET_HO_ERROR_CODE_UNKNOW,iIndex,pWorkItem->id);        
                return ;
            }

            pCcb->iCurTwIdx = iIndex;
            
            work_qtw_succ(iIndex);        
        }        
        break;
    default:
        
        /* check B2 & get B2 reports from exe */
        if (haveB2(&gCcb))
        {
            iRet = CcbGetPmValue(pWorkItem->id,APP_EXE_PM2_NO,1);
            if (iRet )
            {
                VOS_LOG(VOS_LOG_WARNING,"CcbGetPmValue Fail %d",iRet);  
        
                work_qtw_fail(pCcb,APP_PACKET_HO_ERROR_CODE_TANK_EMPTY,iIndex,pWorkItem->id);        
                return ;
            }
        
            if (CcbConvert2Pm2SP(pCcb->ExeBrd.aPMObjs[APP_EXE_PM2_NO].Value.ulV) < CcbGetSp6())
            {
                pCcb->bit1B2Empty = TRUE;
            
                work_qtw_fail(pCcb,APP_PACKET_HO_ERROR_CODE_TANK_EMPTY,iIndex,pWorkItem->id);        
                return;
            }
        }
    
        /* set  switchs */
        if (APP_DEV_HS_SUB_HYPER == pCcb->aHandler[iIndex].iDevType)
        {
            iTmp = (1<<APP_EXE_E4_NO)|(1<<APP_EXE_E9_NO)|(1<<APP_EXE_C2_NO)|(1<<APP_EXE_N2_NO);
            ulQtwSwMask = pCcb->ulHyperTwMask;
        }
        else
        {
            iTmp = (1<<APP_EXE_E4_NO)|(1<<APP_EXE_E9_NO)|(1<<APP_EXE_C2_NO);
            ulQtwSwMask = pCcb->ulNormalTwMask;
        }
        
        iRet = CcbUpdateSwitch(pWorkItem->id,0,ulQtwSwMask,iTmp);
        if (iRet )
        {
            VOS_LOG(VOS_LOG_WARNING,"CcbUpdateSwitch Fail %d",iRet);    
            work_qtw_fail(pCcb,APP_PACKET_HO_ERROR_CODE_UNKNOW,iIndex,pWorkItem->id);
            return ;
        }

        if ((ulQtwSwMask & (1 << APP_EXE_C2_NO))
           && pCcb->bit1CirSpeedAdjust)
        {
            CcbC2Regulator(pWorkItem->id,24,TRUE);
        }

        iTmp = (1 << APP_EXE_I4_NO)|(1 << APP_EXE_I5_NO);

        switch(pCcb->ulMachineType)
        {
        case MACHINE_PURIST:
            iTmp |= (1 << APP_EXE_I2_NO);
            break;
        case MACHINE_UP:
        case MACHINE_RO:
            iTmp |= (1 << APP_EXE_I3_NO);
            break;
        default:

            break;
        }

        iRet = CcbUpdateIAndBs(pWorkItem->id,0,iTmp,iTmp);
        if (iRet )
        {
            VOS_LOG(VOS_LOG_WARNING,"CcbUpdateIAndBs Fail %d",iRet);
            
            /* notify ui (late implemnt) */
            work_qtw_fail(pCcb,APP_PACKET_HO_ERROR_CODE_UNKNOW,iIndex,pWorkItem->id);        
            return ;
        }
    
        iTmp = (1 << APP_FM_FM1_NO); /* start flow report */
        
        iRet = CcbUpdateFms(pWorkItem->id,0,iTmp,iTmp);
        if (iRet )
        {
            VOS_LOG(VOS_LOG_WARNING,"CcbUpdateFms Fail %d",iRet);    
            /* notify ui (late implemnt) */
            work_qtw_fail(pCcb,APP_PACKET_HO_ERROR_CODE_UNKNOW,iIndex,pWorkItem->id);        
            return ;
        }
    
        pCcb->iCurTwIdx = iIndex;
    
        work_qtw_succ(iIndex);        
        break;
    }
    
}



DISPHANDLE CcbInnerWorkStartQtw(int iIndex)
{
    //2019.10.15 add
    if(!check_Sub_Account())
    {
        return;
    }

    WORK_ITEM_STRU *pWorkItem = CcbAllocWorkItem();

    if (!pWorkItem)
    {
        return DISP_INVALID_HANDLE;
    }

    pWorkItem->proc  = work_start_qtw;
    pWorkItem->para  = &gCcb;
    pWorkItem->extra = (void *)iIndex;

    CcbAddWork(WORK_LIST_HP,pWorkItem);

    return (DISPHANDLE)pWorkItem;
}

void work_stop_qtw(void *para)
{
    WORK_ITEM_STRU *pWorkItem = (WORK_ITEM_STRU *)para;

    CCB *pCcb = (CCB *)pWorkItem->para;

    int iTmp;
    int iRet;
    int iLoop;

    int iIndex = (int)pWorkItem->extra;

    if (APP_DEV_HS_SUB_HYPER == pCcb->aHandler[iIndex].iDevType)
    {
        iTmp = pCcb->ulHyperTwMask;
    }
    else
    {
        iTmp = pCcb->ulNormalTwMask;
    }

    CcbFiniHandlerQtwMeas(iIndex);
    
    /* late implemnt */
    /* E10 ON        */
    /* set  valves   */
    iRet = CcbUpdateSwitch(pWorkItem->id,0,iTmp,0);
    if (iRet )
    {
        VOS_LOG(VOS_LOG_WARNING,"CcbSetSwitch Fail %d",iRet);    
    }

    iTmp  = (1 << APP_EXE_I4_NO) | (1 << APP_EXE_I5_NO);

    switch(pCcb->ulMachineType)
    {
    case MACHINE_PURIST:
    case MACHINE_ADAPT:
        iTmp |= (1 << APP_EXE_I2_NO);
        break;
    case MACHINE_UP:
    case MACHINE_RO:
        iTmp |= (1 << APP_EXE_I3_NO);
        break;
    default:
        break;
    }

    iRet = CcbUpdateIAndBs(pWorkItem->id,0,iTmp,0);
    if (iRet )
    {
        VOS_LOG(VOS_LOG_WARNING,"CcbSetIAndBs Fail %d",iRet);    
        /* notify ui (late implemnt) */
    }     

    iTmp  = (1 << APP_FM_FM1_NO);
    iRet = CcbUpdateFms(pWorkItem->id,0,iTmp,0);
    if (iRet )
    {
        VOS_LOG(VOS_LOG_WARNING,"CcbSetIAndBs Fail %d",iRet);    
        /* notify ui (late implemnt) */
    }

    //2019.2.19 add
    if(MACHINE_ADAPT == pCcb->ulMachineType)
    {
        pCcb->bit3RuningState = NOT_RUNING_STATE_NONE;
    }

    work_stop_qtw_succ(iIndex);

    pCcb->ulAdapterAgingCount = gulSecond;
}


DISPHANDLE CcbInnerWorkStopQtw(int iIndex)
{
    WORK_ITEM_STRU *pWorkItem = CcbAllocWorkItem();

    if (!pWorkItem)
    {
        return DISP_INVALID_HANDLE;
    }
    
    VOS_LOG(VOS_LOG_WARNING,"CcbInnerWorkStopQtw");    

    pWorkItem->proc = work_stop_qtw;
    pWorkItem->para = &gCcb;
    pWorkItem->extra = (void *)iIndex;

    CcbAddWork(WORK_LIST_HP,pWorkItem);

    return (DISPHANDLE)pWorkItem;
}

void work_cir_fail(int iWorkId)
{
    int aiCont[1] = {-1};
    
    if (CcbGetSwitchObjState(APP_EXE_INNER_SWITCHS))
    {
        CcbUpdateSwitch(iWorkId,0,gCcb.ulCirMask,0);
    }
    
    if (CcbGetIObjState((1 << APP_EXE_I4_NO)|(1 << APP_EXE_I5_NO)))
    {
        int iTmp = (1 << APP_EXE_I4_NO)|(1 << APP_EXE_I5_NO);
        CcbUpdateIAndBs(iWorkId,0,iTmp,0);
    }

    CanCcbTransState4Pw(DISP_WORK_STATE_RUN,DISP_WORK_SUB_IDLE);

    MainSndWorkMsg(WORK_MSG_TYPE_SCIR,(unsigned char *)aiCont,sizeof(aiCont));

}


void work_cir_succ(void)
{
    int aiCont[1] = {0};
    
    MainSndWorkMsg(WORK_MSG_TYPE_SCIR,(unsigned char *)aiCont,sizeof(aiCont));
}



void work_start_toc_cir(void *para)
{
    WORK_ITEM_STRU *pWorkItem = (WORK_ITEM_STRU *)para;

    CCB *pCcb = (CCB *)pWorkItem->para;

    int iTocStage = (int)pWorkItem->extra;

    int iTmp;
    int iRet;

    pCcb->iTocStage   = iTocStage;

    pCcb->iTocStageTimer = 0;

    VOS_LOG(VOS_LOG_DEBUG,"%s : %d",__FUNCTION__,iTocStage);  
    

    switch(iTocStage)
    {
    case APP_PACKET_EXE_TOC_STAGE_OXDIZATION:
        iTmp = (1 << APP_EXE_E5_NO) | (1 << APP_EXE_C2_NO)| (1 << APP_EXE_N2_NO);
       
        iRet = CcbUpdateSwitch(pWorkItem->id,0,pCcb->ulCirMask,iTmp);
        if (iRet )
        {
            VOS_LOG(VOS_LOG_WARNING,"CcbUpdateSwitch Fail %d",iRet);    
            work_cir_fail(pWorkItem->id);
            return ;
        }

        /* tell exe to calc toc */
                
        break;
    case APP_PACKET_EXE_TOC_STAGE_FLUSH2:
         iTmp = (1 << APP_EXE_E5_NO) | (1 << APP_EXE_E9_NO) | (1 << APP_EXE_C2_NO)| (1 << APP_EXE_N2_NO);
 
         iRet = CcbUpdateSwitch(pWorkItem->id,0,pCcb->ulCirMask,iTmp);
         if (iRet )
         {
             VOS_LOG(VOS_LOG_WARNING,"CcbUpdateSwitch Fail %d",iRet);    
             work_cir_fail(pWorkItem->id);
             return ;
         }
         
        break;
    }
    
    DispSetTocState(iTocStage);
}


void work_start_cir(void *para)
{
    WORK_ITEM_STRU *pWorkItem = (WORK_ITEM_STRU *)para;

    CCB *pCcb = (CCB *)pWorkItem->para;

    int iCirType = (int)pWorkItem->extra;

    int iTmp;
    int iRet;
    int iLoop; //ex

    pCcb->bit1I54Cir   = FALSE;
    pCcb->iCirType     = iCirType;
    pCcb->bit1B2Empty  = FALSE;
    pCcb->bit1AlarmTOC = FALSE;

    CanCcbTransState4Pw(DISP_WORK_STATE_RUN,DISP_WORK_SUB_RUN_CIRING);

    if (haveB2(pCcb))
    {
        if (CcbConvert2Pm2SP(pCcb->ExeBrd.aPMObjs[APP_EXE_PM2_NO].Value.ulV) < CcbGetSp6())
        {
            pCcb->bit1B2Empty = TRUE;
        
            work_cir_fail(pWorkItem->id);
    
            VOS_LOG(VOS_LOG_WARNING,"< CcbGetSp6 ");    
            
            return;
        }  
    }

    //2018.10.26 add
    if(iCirType == CIR_TYPE_UP)
    {
        ex_gCcb.Ex_Auto_Cir_Tick.ulUPAutoCirTick = ex_gulSecond; //UP Auto cir 60min
    }

    switch(iCirType)
    {
    case CIR_TYPE_UP:
         if (haveTOC(pCcb))
         {           
             iTmp = (1 << APP_EXE_E5_NO) | (1 << APP_EXE_E9_NO)| (1 << APP_EXE_C2_NO);
     
             iRet = CcbUpdateSwitch(pWorkItem->id,0,pCcb->ulCirMask ,iTmp);
             if (iRet )
             {
                 VOS_LOG(VOS_LOG_WARNING,"CcbUpdateSwitch Fail %d",iRet);    
                 work_cir_fail(pWorkItem->id);
                 return ;
             }

             if ((pCcb->ulCirMask & (1 << APP_EXE_C2_NO))
                && pCcb->bit1CirSpeedAdjust)
             {
                 CcbC2Regulator(pWorkItem->id,8,TRUE);
             }

             iTmp = (1 << APP_EXE_I4_NO)|(1 << APP_EXE_I5_NO);
             iRet = CcbUpdateIAndBs(pWorkItem->id,0,iTmp,iTmp);
             if (iRet )
             {
                 VOS_LOG(VOS_LOG_WARNING,"CcbUpdateIAndBs Fail %d",iRet);    
                 /* notify ui (late implemnt) */
                 work_cir_fail(pWorkItem->id);        
                 return ;
             }
             //Ex FM1 report
             iTmp = (1 << APP_FM_FM1_NO); /* start flow report */

             iRet = CcbUpdateFms(pWorkItem->id,0,iTmp,iTmp);
             if (iRet )
             {
                 VOS_LOG(VOS_LOG_WARNING,"CcbUpdateFms Fail %d",iRet);
                 /* notify ui (late implemnt) */
                 work_cir_fail(pWorkItem->id);
                 return ;
             }
             //end

             pCcb->iTocStage      = APP_PACKET_EXE_TOC_STAGE_FLUSH1;
             pCcb->iTocStageTimer = 0;
             pCcb->bit1TocOngoing = 1;
             /* toc3 left to main loop */
             
         }
         else
         {
//             iTmp = (1 << APP_EXE_E5_NO) | (1 << APP_EXE_E9_NO) | (1 << APP_EXE_C2_NO)| (1 << APP_EXE_N2_NO);
             iTmp = (1 << APP_EXE_E5_NO) | (1 << APP_EXE_E9_NO) | (1 << APP_EXE_C2_NO);
             iRet = CcbUpdateSwitch(pWorkItem->id,0,pCcb->ulCirMask,iTmp);
             if (iRet )
             {
                 VOS_LOG(VOS_LOG_WARNING,"CcbUpdateSwitch Fail %d",iRet);    
                 work_cir_fail(pWorkItem->id);
                 return ;
             }

             if ((pCcb->ulCirMask & (1 << APP_EXE_C2_NO))
                && pCcb->bit1CirSpeedAdjust)
             {
                 CcbC2Regulator(pWorkItem->id,8,TRUE);
             }
     
             iTmp = (1 << APP_EXE_I4_NO)|(1 << APP_EXE_I5_NO);
             iRet = CcbUpdateIAndBs(pWorkItem->id,0,iTmp,iTmp);
             if (iRet )
             {
                 VOS_LOG(VOS_LOG_WARNING,"CcbUpdateIAndBs Fail %d",iRet);    
                 /* notify ui (late implemnt) */
                 work_cir_fail(pWorkItem->id);        
                 return ;
             }

             //Ex FM1 report
             iTmp = (1 << APP_FM_FM1_NO); /* start flow report */

             iRet = CcbUpdateFms(pWorkItem->id,0,iTmp,iTmp);
             if (iRet )
             {
                 VOS_LOG(VOS_LOG_WARNING,"CcbUpdateFms Fail %d",iRet);
                 /* notify ui (late implemnt) */
                 work_cir_fail(pWorkItem->id);
                 return ;
             }
             //end
             //EX
             for (iLoop = 0; iLoop < 3; iLoop++) //延时启动N2
             {
                 iRet = CcbWorkDelayEntry(pWorkItem->id,1000,CcbDelayCallBack);
                 if (iRet )
                 {
                     VOS_LOG(VOS_LOG_WARNING,"CcbModbusWorkEntry Fail %d",iRet);
                     work_cir_fail(pWorkItem->id);
                     return;
                 }
             }
             iTmp = (1 << APP_EXE_E5_NO) | (1 << APP_EXE_E9_NO) | (1 << APP_EXE_C2_NO)| (1 << APP_EXE_N2_NO);
             iRet = CcbUpdateSwitch(pWorkItem->id,0,pCcb->ulCirMask,iTmp);
             
             pCcb->iTocStage      = APP_PACKET_EXE_TOC_STAGE_NUM;
         }
         pCcb->bit1AlarmUP = FALSE;
         break;
    case CIR_TYPE_HP:
        {
            /* check B2 & get B2 reports from exe */
            iRet = CcbGetPmValue(pWorkItem->id,APP_EXE_PM2_NO,1);
            if (iRet )
            {
                VOS_LOG(VOS_LOG_WARNING,"CcbGetPmValue Fail %d",iRet);  
            
                work_cir_fail(pWorkItem->id);
            
                return ;
            }
            if (haveB2(pCcb))
            {
            
                if (CcbConvert2Pm2SP(pCcb->ExeBrd.aPMObjs[APP_EXE_PM2_NO].Value.ulV) < CcbGetSp6())
                {
                    pCcb->bit1B2Empty = TRUE;
                
                    work_cir_fail(pWorkItem->id);
                    
                    return;
                }
            }
            
             iTmp = (1 << APP_EXE_E4_NO) | (1 << APP_EXE_E6_NO) |(1 << APP_EXE_E9_NO)| (1 << APP_EXE_C2_NO);
     
             iRet = CcbUpdateSwitch(pWorkItem->id,0,pCcb->ulCirMask ,iTmp);
             if (iRet )
             {
                 VOS_LOG(VOS_LOG_WARNING,"CcbUpdateSwitch Fail %d",iRet);    
                 work_cir_fail(pWorkItem->id);
                 return ;
             }

     
             if ((pCcb->ulCirMask & (1 << APP_EXE_C2_NO))
                && pCcb->bit1CirSpeedAdjust)
             {
                 CcbC2Regulator(pWorkItem->id,8,TRUE);
             }
             
             iTmp = (1 << APP_EXE_I4_NO);

             //ex UP Tank I3 Report
             if(MACHINE_UP == pCcb->ulMachineType)
             {
                 iTmp |= (1 << APP_EXE_I3_NO);
             }

             if(MACHINE_RO == pCcb->ulMachineType) // && haveHPCir(&gCcb))
             {
                 iTmp |= (1 << APP_EXE_I3_NO);
             }
             //endl

             iRet = CcbUpdateIAndBs(pWorkItem->id,0,iTmp,iTmp);
             if (iRet )
             {
                 VOS_LOG(VOS_LOG_WARNING,"CcbUpdateIAndBs Fail %d",iRet);    
                 /* notify ui (late implemnt) */
                 work_cir_fail(pWorkItem->id);        
                 return ;
             } 

            iTmp = (1 << APP_FM_FM1_NO); /* start flow report */
            
            iRet = CcbUpdateFms(pWorkItem->id,0,iTmp,iTmp);
            if (iRet )
            {
                VOS_LOG(VOS_LOG_WARNING,"CcbUpdateFms Fail %d",iRet);    
                /* notify ui (late implemnt) */
                work_cir_fail(pWorkItem->id);        
                return ;
            }
            
             
        }
        break;
    }

    work_cir_succ();
    
}

DISPHANDLE CcbInnerWorkStartCirToc(int iStage)
{
    WORK_ITEM_STRU *pWorkItem = CcbAllocWorkItem();

    if (!pWorkItem)
    {
        return DISP_INVALID_HANDLE;
    }

    VOS_LOG(VOS_LOG_WARNING,"%s : %d",__FUNCTION__,iStage);    

    pWorkItem->proc  = work_start_toc_cir;
    pWorkItem->para  = &gCcb;
    pWorkItem->extra  = (void *)iStage;

    CcbAddWork(WORK_LIST_HP,pWorkItem);

    return (DISPHANDLE)pWorkItem;
}


DISPHANDLE CcbInnerWorkStartCir(int iType)
{
    WORK_ITEM_STRU *pWorkItem = CcbAllocWorkItem();

    if (!pWorkItem)
    {
        return DISP_INVALID_HANDLE;
    }

    VOS_LOG(VOS_LOG_WARNING,"%s : %d",__FUNCTION__,iType);    

    pWorkItem->proc  = work_start_cir;
    pWorkItem->para  = &gCcb;
    pWorkItem->extra  = (void *)iType;

    CcbAddWork(WORK_LIST_HP,pWorkItem);

    return (DISPHANDLE)pWorkItem;
}

void work_stop_cir_succ(void)
{
    int aiCont[1] = {0};
    
    MainSndWorkMsg(WORK_MSG_TYPE_ECIR,(unsigned char *)aiCont,sizeof(aiCont));

}

void work_stop_cir(void *para)
{
    WORK_ITEM_STRU *pWorkItem = (WORK_ITEM_STRU *)para;

    CCB *pCcb = (CCB *)pWorkItem->para;

    int iTmp;
    int iRet;

    pCcb->bit1I44Nw = FALSE;

    pCcb->bit1TocOngoing = FALSE;

    /* set  valves   */
    iTmp = pCcb->ulCirMask;
    iRet = CcbUpdateSwitch(pWorkItem->id,0,iTmp,0);
    if (iRet )
    {
        VOS_LOG(VOS_LOG_WARNING,"CcbSetSwitch Fail %d",iRet);    
    }

    if ((pCcb->ulCirMask & (1 << APP_EXE_C2_NO))
       && pCcb->bit1CirSpeedAdjust)
    {
        CcbC2Regulator(pWorkItem->id,24,FALSE);
    }

    iTmp = (1 << APP_EXE_I4_NO)|(1 << APP_EXE_I5_NO);

    //ex UP Tank I3 Report
    if(MACHINE_UP == pCcb->ulMachineType)
    {
        iTmp |= (1 << APP_EXE_I3_NO);
    }
    if(MACHINE_RO == pCcb->ulMachineType)// && haveHPCir(&gCcb))
    {
        iTmp |= (1 << APP_EXE_I3_NO);
    }
    //endl

    iRet = CcbUpdateIAndBs(pWorkItem->id,0,iTmp,0);
    if (iRet )
    {
        VOS_LOG(VOS_LOG_WARNING,"CcbSetIAndBs Fail %d",iRet);    
        /* notify ui (late implemnt) */
    }     

    iTmp  = (APP_FM_FM1_NO << 1);
    iRet = CcbUpdateFms(pWorkItem->id,0,iTmp,0);
    if (iRet )
    {
        VOS_LOG(VOS_LOG_WARNING,"CcbUpdateFms Fail %d",iRet);    
        /* notify ui (late implemnt) */
    }     

    work_stop_cir_succ();
}


DISPHANDLE CcbInnerWorkStopCir(void)
{
    WORK_ITEM_STRU *pWorkItem = CcbAllocWorkItem();

    if (!pWorkItem)
    {
        return DISP_INVALID_HANDLE;
    }

    VOS_LOG(VOS_LOG_WARNING,"CcbInnerWorkStopCir");    

    pWorkItem->proc = work_stop_cir;
    pWorkItem->para = &gCcb;

    CcbAddWork(WORK_LIST_HP,pWorkItem);

    return (DISPHANDLE)pWorkItem;
}

void work_speed_regulation_end(int iIndex,int iResult)
{
    int aiCont[2] = {iResult,iIndex};
    
    MainSndWorkMsg(WORK_MSG_TYPE_ESR,(unsigned char *)aiCont,sizeof(aiCont));
}


void work_start_speed_regulation(void *para)
{
    WORK_ITEM_STRU *pWorkItem = (WORK_ITEM_STRU *)para;

    CCB *pCcb = (CCB *)pWorkItem->para;
    int iIndex = (int)pWorkItem->extra;

    int iTmp;
    int iActive;

    int iRet;

    int iSpeed = pCcb->aHandler[iIndex].iSpeed;

    float fv; //2019.1.7

    
    DispGetRPumpSwitchState(APP_EXE_C2_NO - APP_EXE_C1_NO,&iActive);

    //2019.1.7
    if(10 <= iSpeed)
    {
        fv = 24.0;
    }
    else if(iSpeed == 9)
    {
        fv = 18.0;
    }
    else if(iSpeed == 8)
    {
        fv = 15.0;
    }
    else
    {
        fv = iSpeed + 6.0;
    }
    iTmp = DispConvertVoltage2RPumpSpeed(fv);
    //end

   // iTmp = DispConvertVoltage2RPumpSpeed(((24.0 - 5.0)*iSpeed)/PUMP_SPEED_NUM + 5);
    
    if (iActive) iTmp = 0XFF00 | iTmp;

    iRet = CcbSetRPump(pWorkItem->id ,APP_EXE_C2_NO - APP_EXE_C1_NO,iTmp);

    work_speed_regulation_end(iIndex,iRet);
}


DISPHANDLE CcbInnerWorkStartSpeedRegulation(int iIndex)
{
    WORK_ITEM_STRU *pWorkItem = CcbAllocWorkItem();

    if (!pWorkItem)
    {
        return DISP_INVALID_HANDLE;
    }

    pWorkItem->proc  = work_start_speed_regulation;
    pWorkItem->para  = &gCcb;
    pWorkItem->extra = (void *)iIndex;

    CcbAddWork(WORK_LIST_HP,pWorkItem);

    return (DISPHANDLE)pWorkItem;
}


void work_normal_run_fail(int iWorkId)
{
    int aiCont[1] = {-1};

    int iTmp;

    /* close all switchs */
    if (CcbGetSwitchObjState(APP_EXE_SWITCHS_MASK))
    {
        CcbUpdateSwitch(iWorkId,0,gCcb.ulRunMask,0);
    }
    
    /*stop all report */
    if (CcbGetPmObjState((1 << APP_EXE_PM1_NO))
        || CcbGetIObjState((1 << APP_EXE_I1_NO)|(1 << APP_EXE_I2_NO)|(1 << APP_EXE_I3_NO)))
    {
        iTmp = GET_B_MASK(APP_EXE_PM1_NO) |(1 << APP_EXE_I1_NO)|(1 << APP_EXE_I2_NO)|(1 << APP_EXE_I3_NO) ;
    
        CcbUpdateIAndBs(iWorkId,0,iTmp,0);
    }

    iTmp = (1 << APP_FM_FM2_NO)|(1<<APP_FM_FM3_NO)|(1<<APP_FM_FM4_NO);

    if (CcbGetFmObjState(iTmp))
    {
        CcbUpdateFms(iWorkId,0,iTmp,0);
    }
    
    gCcb.bit1AlarmRej = FALSE;
    gCcb.bit1AlarmRoPw = FALSE;
    gCcb.bit1AlarmEDI = FALSE;
    gCcb.bit3RuningState = NOT_RUNING_STATE_NONE;

    MainSndWorkMsg(WORK_MSG_TYPE_RUN,(unsigned char *)aiCont,sizeof(aiCont));
    
    
}

void work_normal_run_succ(int id)
{
    int aiCont[1] = {0};

    (void )id;
    
    gCcb.bit3RuningState = NOT_RUNING_STATE_NONE;
    
    MainSndWorkMsg(WORK_MSG_TYPE_RUN,(unsigned char *)aiCont,sizeof(aiCont));
}


void work_normal_run_wrapper(void *para)
{
    WORK_ITEM_STRU *pWorkItem = (WORK_ITEM_STRU *)para;

    CCB *pCcb = (CCB *)pWorkItem->para;

    /* init */
    pCcb->bit1AlarmRej     = FALSE;
    pCcb->bit1ProduceWater = FALSE;
    pCcb->bit1AlarmRoPw    = FALSE;
    pCcb->bit1AlarmEDI     = FALSE;
    pCcb->bit1PeriodFlushing = FALSE;
    pCcb->bit1B1Check4RuningState = FALSE;
    pCcb->bit1B1UnderPressureDetected = FALSE;
    pCcb->ulRejCheckCount = 0;

    /* 2018/01/15 add begin*/
    pCcb->bit1AlarmROPHV    = FALSE;
    pCcb->bit1AlarmROPLV    = FALSE;
    pCcb->bit1AlarmRODV    = FALSE;
    pCcb->ulRopVelocity    = 0;
    pCcb->ulLstRopFlow     = INVALID_FM_VALUE;
    pCcb->ulLstRopTick     = 0;
    pCcb->iRopVCheckLowEventCount  = 0;
    pCcb->iRopVCheckLowRestoreCount  = 0;
    
    pCcb->ulRodVelocity    = 0;
    pCcb->ulLstRodFlow     = INVALID_FM_VALUE;
    pCcb->ulLstRodTick     = 0;
    pCcb->iRoDVCheckEventCount  = 0;
    pCcb->iRoDVCheckRestoreCount  = 0;
    /* 2018/01/15 add end*/

    work_run_comm_proc(pWorkItem,pCcb,work_normal_run_fail,work_normal_run_succ);

}


void work_normal_run(void *para)
{
    work_normal_run_wrapper(para);
}

DISPHANDLE CcbInnerWorkRun(void)
{
    WORK_ITEM_STRU *pWorkItem = CcbAllocWorkItem();

    if (!pWorkItem)
    {
        return DISP_INVALID_HANDLE;
    }

    pWorkItem->proc = work_normal_run;
    pWorkItem->para = &gCcb;

    CcbAddWork(WORK_LIST_LP,pWorkItem);

    return (DISPHANDLE)pWorkItem;

}



DISPHANDLE CcbInnerWorkIdle(void)
{
    WORK_ITEM_STRU *pWorkItem = CcbAllocWorkItem();

    if (!pWorkItem)
    {
        return DISP_INVALID_HANDLE;
    }

    pWorkItem->proc = work_idle;
    pWorkItem->para = &gCcb;

    CcbAddWork(WORK_LIST_HP,pWorkItem);

    return (DISPHANDLE)pWorkItem;
}

int CanCcbAfModbusMsg(MAIN_CANITF_MSG_STRU *pCanItfMsg)
{
    MODBUS_PACKET_COMM_STRU *pmg = (MODBUS_PACKET_COMM_STRU *)&pCanItfMsg->aucData[1 + RPC_POS_DAT0]; 

    if (gCcb.ModbusCb)
    {
        (gCcb.ModbusCb)(0,pmg);
        sp_thread_mutex_lock  ( &gCcb.Ipc.mutex );
        sp_thread_cond_signal ( &gCcb.Ipc.cond  );// ylf: all thread killed
        sp_thread_mutex_unlock( &gCcb.Ipc.mutex );
    }
    
    return 0;
}

void CanCcbAfRfIdMsg(int mask)
{

    if (gCcb.iRfIdRequest & mask)
    {
        sp_thread_mutex_lock  ( &gCcb.Ipc4Rfid.mutex );
        sp_thread_cond_signal ( &gCcb.Ipc4Rfid.cond  );// ylf: all thread killed
        sp_thread_mutex_unlock( &gCcb.Ipc4Rfid.mutex );
    }
    
}

void CcbReset(void)
{
    gCcb.bit1AlarmRej      = 0;
    gCcb.bit1AlarmRoPw     = 0;
    gCcb.bit1AlarmEDI      = 0;
    gCcb.bit1NeedFillTank  = 0;  
    gCcb.bit1B2Full        = 0;  /* tank full */
    gCcb.bit1FillingTank   = 0;  
    gCcb.bit1N3Active      = 0;  /* ultralight N3 period handle flag */
    gCcb.bit1ProduceWater  = 0;  /* RUN state & producing water */
    gCcb.bit1B1Check4RuningState = 0;
    gCcb.bit1B1UnderPressureDetected = 0;
    gCcb.bit3RuningState = 0;

    gCcb.iWorkStateStackDepth = 0;
    gCcb.bit1LeakKey4Reset    = 0;
    gCcb.bit1NeedTubeCir      = 0;
    gCcb.bit1TubeCirOngoing   = 0;
    gCcb.bit1PeriodFlushing   = 0;

}

void CcbCleanup(void)
{
   switch(gCcb.curWorkState.iMainWorkState)
   {
   case DISP_WORK_STATE_IDLE:
        CcbReset();
        break;
   case DISP_WORK_STATE_LPP:
        gCcb.bit3RuningState = 0;
        gCcb.bit1FillingTank = 0;
        gCcb.bit1AlarmRej      = 0;
        gCcb.bit1AlarmRoPw     = 0;
        gCcb.bit1AlarmEDI      = 0;
        break;
   }
}

void CanCcbExeReset(void)
{
    gCcb.ExeBrd.ulEcoValidFlags = 0;
    gCcb.ExeBrd.ulPmValidFlags = 0;
    gCcb.ExeBrd.ucDinState = 0;
}

void CanCcbFMReset()
{
    gCcb.FlowMeter.ulFmValidFlags = 0;
}


void CanCcbInnerReset(int iFlags)
{
    int iLoop;
    
    if ((iFlags & (1 << APP_DEV_TYPE_EXE_BOARD))
        || (iFlags & (1 << APP_DEV_TYPE_FLOW_METER)))
    {
        CcbCancelAllWork();
        
        /* reset to default state */
        CcbReset(); 

        if (gCcb.ulActiveMask & (1 << APP_DEV_TYPE_EXE_BOARD))
        {
            /* shutdown all switches */
            CcbSetSwitch(WORK_LIST_NUM,0,0);
            
            CcbSetIAndBs(WORK_LIST_NUM,0,0);  

            CanCcbExeReset();
            
        }
    
        if (gCcb.ulActiveMask & (1 << APP_DEV_TYPE_FLOW_METER))
        {
            /* shutdown all switches */
            CcbSetFms(WORK_LIST_NUM,0,0);

            CanCcbFMReset();
        }
                
        if (DISP_WORK_STATE_IDLE != gCcb.curWorkState.iMainWorkState)
        {
            work_idle_succ();
        }
        return ;
    }

    /* handle reset */
    iFlags >>= APP_HAND_SET_BEGIN_ADDRESS;

    for (iLoop = 0; iLoop < MAX_HANDLER_NUM; iLoop++)
    {
        if (iFlags & (1 << iLoop))
        {
            if (gCcb.aHandler[iLoop].bit1Qtw)
            {
                /* stop taking water */
                CcbInnerWorkStopQtw(iLoop);
            }
        }
    }
    
    
}

int CanCcbGetHoState(void)
{
    uint8_t ucDstState ;
    
    switch(gCcb.curWorkState.iMainWorkState4Pw)
    {
    default :
        ucDstState = APP_PACKET_HS_STATE_IDLE;
        break;
    case DISP_WORK_STATE_IDLE:        
        switch(gCcb.curWorkState.iSubWorkState4Pw)
        {
        default:
            ucDstState = APP_PACKET_HS_STATE_IDLE;
            break;
        case DISP_WORK_SUB_IDLE_DEPRESSURE:
            ucDstState = APP_PACKET_HS_STATE_DEC;
            break;
        }
        break;
    case DISP_WORK_STATE_RUN:
        switch(gCcb.curWorkState.iSubWorkState4Pw)
        {
        default:
            ucDstState = APP_PACKET_HS_STATE_RUN;
            break;
        case DISP_WORK_SUB_RUN_QTW:
        case DISP_WORK_SUB_RUN_QTWING:
            ucDstState = APP_PACKET_HS_STATE_QTW;
            break;
        case DISP_WORK_SUB_RUN_CIR:
        case DISP_WORK_SUB_RUN_CIRING:
            ucDstState = APP_PACKET_HS_STATE_CIR;
            break;
        case DISP_WORK_SUB_RUN_DEC:
            ucDstState = APP_PACKET_HS_STATE_DEC;
            break;
        }
        break;
    }

    return ucDstState;

}

int CanCcbAfDataOnLineNotiIndMsg(MAIN_CANITF_MSG_STRU *pCanItfMsg)
{

    APP_PACKET_ONLINE_NOTI_IND_STRU *pmg = (APP_PACKET_ONLINE_NOTI_IND_STRU *)&pCanItfMsg->aucData[1 + RPC_POS_DAT0];

    int iSrcAdr     = CAN_SRC_ADDRESS(pCanItfMsg->ulCanId);
    int iFlags      = 0;
    //int isHandler   = FALSE;
    int iHandlerIdx = 0;

    printf("OnLine Ind 0x%x\r\n",pCanItfMsg->ulCanId);
    
    switch(pmg->hdr.ucDevType)
    {
    case APP_DEV_TYPE_EXE_BOARD:
        if (iSrcAdr != pmg->hdr.ucDevType)
        {
            return -1;
        }
        iFlags = 1 << iSrcAdr;  

        {
            /* update current FM information */
            APP_PACKET_ONLINE_NOTI_IND_EXE_STRU *pFm = (APP_PACKET_ONLINE_NOTI_IND_EXE_STRU *)pmg->acInfo;
            
            for (iHandlerIdx = 0; iHandlerIdx < APP_EXE_MAX_HOLD_REGISTERS; iHandlerIdx++)
            {
                gCcb.ExeBrd.ausHoldRegister[iHandlerIdx] = pFm->ausHoldRegs[iHandlerIdx];
            }
        }    
        
        break;
    case APP_DEV_TYPE_FLOW_METER:
        if (iSrcAdr != pmg->hdr.ucDevType)
        {
            return -1;
        }
        iFlags = 1 << iSrcAdr;

        {
            /* update current FM information */
            APP_PACKET_ONLINE_NOTI_IND_FM_STRU *pFm = (APP_PACKET_ONLINE_NOTI_IND_FM_STRU *)pmg->acInfo;
            
            for (iHandlerIdx = 0; iHandlerIdx < APP_FM_FLOW_METER_NUM; iHandlerIdx++)
            {
                if (  (gCcb.FlowMeter.aHist[iHandlerIdx].curValue.ulV != 
                       gCcb.FlowMeter.aHist[iHandlerIdx].lstValue.ulV)
                    &&(gCcb.FlowMeter.aHist[iHandlerIdx].curValue.ulV != INVALID_FM_VALUE
                       && gCcb.FlowMeter.aHist[iHandlerIdx].lstValue.ulV != INVALID_FM_VALUE)
                    && (gCcb.FlowMeter.aHist[iHandlerIdx].curValue.ulV > 
                        gCcb.FlowMeter.aHist[iHandlerIdx].lstValue.ulV ))
                {
                    gCcb.FlowMeter.aulHistTotal[iHandlerIdx] += gCcb.FlowMeter.aHist[iHandlerIdx].curValue.ulV - gCcb.FlowMeter.aHist[iHandlerIdx].lstValue.ulV;
                }
                
                gCcb.FlowMeter.aHist[iHandlerIdx].curValue.ulV = pFm->aulFm[iHandlerIdx];
                gCcb.FlowMeter.aHist[iHandlerIdx].lstValue.ulV = INVALID_FM_VALUE;
            }
        }
        break;
    case APP_DEV_TYPE_HAND_SET:
        if (iSrcAdr < APP_HAND_SET_BEGIN_ADDRESS
            || iSrcAdr > APP_HAND_SET_END_ADDRESS )
        {
            return -1;
        }
        /* fill infomation */
        {
            int iIdx = iSrcAdr - APP_HAND_SET_BEGIN_ADDRESS;
            //gCcb.aHandler[iIdx].iDevType   = pmg->acInfo[0];
            gCcb.aHandler[iIdx].bit1Active = TRUE;

            DispGetHandlerConfig(iSrcAdr);

           // isHandler = TRUE;

           gCcb.aHandler[iIdx].iTrxMap |= (1 << APP_TRX_CAN);

            iHandlerIdx = iIdx;
        }

        iFlags = 1 << iSrcAdr;
        
        break;
    case APP_DEV_TYPE_RF_READER:
        if (iSrcAdr < APP_RF_READER_BEGIN_ADDRESS
            || iSrcAdr > APP_RF_READER_END_ADDRESS )
        {
            return -1;
        }
        /* fill infomation */
        {
            int iIdx = iSrcAdr - APP_RF_READER_BEGIN_ADDRESS;
            gCcb.aRfReader[iIdx].bit1Active = TRUE;

        }
        gCcb.aulActMask4Trx[APP_TRX_CAN] |= (1 << iSrcAdr);

        iFlags = 1 << iSrcAdr;
        break;
    default:
        printf("unknow dev type %x\r\n",pmg->hdr.ucDevType);
        return -1;
    }
    
    gCcb.ulRegisterMask |= (1 << iSrcAdr);

    CcbSetActiveMask(iSrcAdr);

    /* Send response message */
    {
        char buf[32];
        APP_PACKET_ONLINE_NOTI_CONF_STRU *pNotiCnf = (APP_PACKET_ONLINE_NOTI_CONF_STRU *)buf;

        unsigned int ulIdentifier ;

        pNotiCnf->hdr.ucLen         = 0;
        pNotiCnf->hdr.ucTransId     = pmg->hdr.ucTransId;
        pNotiCnf->hdr.ucDevType     = APP_DEV_TYPE_MAIN_CTRL;
        pNotiCnf->hdr.ucMsgType     = pmg->hdr.ucMsgType|0X80;

        if (APP_DEV_TYPE_HAND_SET == pmg->hdr.ucDevType)
        {
            APP_PACKET_ONLINE_NOTI_CONF_HANDLER_STRU *pInfo = (APP_PACKET_ONLINE_NOTI_CONF_HANDLER_STRU *)pNotiCnf->aucInfo;
            
            pInfo->usHeartBeatPeriod = HEART_BEAT_PERIOD*1000;
            pInfo->ucLan             = gCcb.MiscParam.iLan;
            pInfo->ucMode            = gCcb.aHandler[iHandlerIdx].iDevType;
            pInfo->ucState           = CanCcbGetHoState();
            pInfo->ucAlarmState      = getAlarmState();
            pInfo->ucLiquidLevel     = getTankLevel();
            pInfo->ucQtwSpeed        = DispConvertRPumpSpeed2Scale(gCcb.ExeBrd.ausHoldRegister[APP_EXE_HOLD_REG_RPUMP_OFFSET + APP_EXE_C2_NO - APP_EXE_C1_NO]);
            pInfo->ucHaveTOC         = !!haveTOC(&gCcb);
            
            switch(pInfo->ucState)
            {
            case APP_PACKET_HS_STATE_QTW:
                pInfo->ucAddData    = CcbGetTwFlag();
                break;
            case APP_PACKET_HS_STATE_CIR:
                pInfo->ucAddData    = gCcb.iCirType;
                break;
            default :
                pInfo->ucAddData    = 0;
                break;
            }

            pNotiCnf->hdr.ucLen     += sizeof(APP_PACKET_ONLINE_NOTI_CONF_HANDLER_STRU);

            CcbNotSpeed(pInfo->ucMode,pInfo->ucQtwSpeed);
        }
        else
        {
            APP_PACKET_ONLINE_NOTI_CONF_EXE_STRU *pInfo = (APP_PACKET_ONLINE_NOTI_CONF_EXE_STRU *)pNotiCnf->aucInfo;
            
            pInfo->usHeartBeatPeriod = HEART_BEAT_PERIOD*1000;
            pNotiCnf->hdr.ucLen     += sizeof(APP_PACKET_ONLINE_NOTI_CONF_EXE_STRU);
        }

        CAN_BUILD_ADDRESS_IDENTIFIER(ulIdentifier,APP_DEV_TYPE_MAIN_CTRL,iSrcAdr);

        CcbSndAfCanCmd(0,ulIdentifier,SAPP_CMD_DATA,(unsigned char *)buf,APP_PROTOL_HEADER_LEN + pNotiCnf->hdr.ucLen );
    }

    //if (isHandler)
    //{
    //    uint8_t ucDstState = CanCcbGetHoState();
    //    CanCcbSndHOState(iHandlerIdx,ucDstState);        
    //}

    /* leave to reset handler */
    CanCcbInnerReset(iFlags);

    switch(pmg->hdr.ucDevType)
    {
    case APP_DEV_TYPE_EXE_BOARD:
        {
            CcbExeBrdNotify();
        }
        break;
    case APP_DEV_TYPE_FLOW_METER:
        {
            CcbFmBrdNotify();
        }
        break;
    case APP_DEV_TYPE_HAND_SET:
        {
            CcbHandlerNotify(iHandlerIdx);
        }
        break;
    }

    return 0;
}


int CanCcbZigbeeOnLineNotiIndMsg(ZBITF_MAIN_MSG_STRU *pZigbeeItfMsg)
{

    APP_PACKET_ONLINE_NOTI_IND_STRU *pmg = (APP_PACKET_ONLINE_NOTI_IND_STRU *)&pZigbeeItfMsg->aucData[APP_ZIGBEE_SUB_PROTOL_LENGTH];

    int iFlags      = 0;
    //int isHandler   = FALSE;
    int iHandlerIdx = 0;

    printf("zb OnLine Ind 0x%x\r\n",pZigbeeItfMsg->usShortAdr);
    
    switch(pmg->hdr.ucDevType)
    {
    case APP_DEV_TYPE_HAND_SET:
        {
            /* fill infomation */
            APP_PACKET_ONLINE_NOTI_IND_HANDLE_STRU *pCont = (APP_PACKET_ONLINE_NOTI_IND_HANDLE_STRU *)pmg->acInfo;
        
            int iIdx = pCont->ucHandsetAddr - APP_HAND_SET_BEGIN_ADDRESS;

            if (pCont->ucHandsetAddr >= APP_HAND_SET_BEGIN_ADDRESS 
                 && pCont->ucHandsetAddr < APP_HAND_SET_BEGIN_ADDRESS + APP_HAND_SET_MAX_NUMBER)
            {
                gCcb.aHandler[iIdx].bit1Active = TRUE;

                gCcb.aHandler[iIdx].iTrxMap |= (1 << APP_TRX_ZIGBEE);

                gCcb.aHandler[iIdx].usShortAddr = pZigbeeItfMsg->usShortAdr;
        
                DispGetHandlerConfig(pCont->ucHandsetAddr);
        
                iHandlerIdx = iIdx;
        
                iFlags = 1 << pCont->ucHandsetAddr;

                gCcb.ulRegisterMask |= (1 << pCont->ucHandsetAddr);
                CcbSetActiveMask(pCont->ucHandsetAddr);

                gCcb.aulActMask4Trx[APP_TRX_ZIGBEE] |= (1 << pCont->ucHandsetAddr);
                
            }
            else
            {
                VOS_LOG(VOS_LOG_DEBUG,"unknow src address %x\r\n",pCont->ucHandsetAddr);    
                return -1;
            }
        }
        break;
    default:
        VOS_LOG(VOS_LOG_DEBUG,"unknow dev type %x\r\n",pmg->hdr.ucDevType);
        return -1;
    }
    

    /* Send response message */
    {
        char buf[32];
        APP_PACKET_ONLINE_NOTI_CONF_STRU *pNotiCnf = (APP_PACKET_ONLINE_NOTI_CONF_STRU *)buf;
        APP_PACKET_ONLINE_NOTI_CONF_HANDLER_STRU *pInfo = (APP_PACKET_ONLINE_NOTI_CONF_HANDLER_STRU *)pNotiCnf->aucInfo;

        sAddr_t addr ;

        pNotiCnf->hdr.ucLen         = 0;
        pNotiCnf->hdr.ucTransId     = pmg->hdr.ucTransId;
        pNotiCnf->hdr.ucDevType     = APP_DEV_TYPE_MAIN_CTRL;
        pNotiCnf->hdr.ucMsgType     = pmg->hdr.ucMsgType|0X80;

        pInfo->usHeartBeatPeriod    = HEART_BEAT_PERIOD*1000;
        pInfo->ucLan                = gCcb.MiscParam.iLan;
        pInfo->ucMode               = gCcb.aHandler[iHandlerIdx].iDevType;
        pInfo->ucState              = CanCcbGetHoState();
        pInfo->ucAlarmState         = getAlarmState();
        pInfo->ucLiquidLevel        = getTankLevel();
        pInfo->ucQtwSpeed           = DispConvertRPumpSpeed2Scale(gCcb.ExeBrd.ausHoldRegister[APP_EXE_HOLD_REG_RPUMP_OFFSET + APP_EXE_C2_NO - APP_EXE_C1_NO]);
        pInfo->ucHaveTOC            = !!haveTOC(&gCcb);
        
        switch(pInfo->ucState)
        {
        case APP_PACKET_HS_STATE_QTW:
            pInfo->ucAddData    = CcbGetTwFlag();
            break;
        case APP_PACKET_HS_STATE_CIR:
            pInfo->ucAddData    = gCcb.iCirType;
            break;
        default :
            pInfo->ucAddData    = 0;
            break;
        }

        pNotiCnf->hdr.ucLen        += sizeof(APP_PACKET_ONLINE_NOTI_CONF_HANDLER_STRU);

        CcbNotSpeed(pInfo->ucMode,pInfo->ucQtwSpeed);

        addr.addrMode            = afAddr16Bit;
        addr.addr.shortAddr      = pZigbeeItfMsg->usShortAdr;

        CcbSndZigbeeCmd(&addr,SAPP_CMD_DATA,(unsigned char *)buf,APP_PROTOL_HEADER_LEN + pNotiCnf->hdr.ucLen );
    }

    /* leave to reset handler */
    CanCcbInnerReset(iFlags);

    switch(pmg->hdr.ucDevType)
    {
    case APP_DEV_TYPE_HAND_SET:
        CcbHandlerNotify(iHandlerIdx);
        break;
    }

    return 0;
}

int CanCcbZigbeeSearchHandset(unsigned short usShortAddr)
{
    int iLoop;

    for (iLoop = 0; iLoop < MAX_HANDLER_NUM; iLoop++)
    {
        if (gCcb.aHandler[iLoop].iTrxMap & (1 << APP_TRX_ZIGBEE) ) 
        {
             if (gCcb.aHandler[iLoop].usShortAddr == usShortAddr)
             {
                 return iLoop;
             }
        }
    }

    return MAX_HANDLER_NUM;
}

int CanCcbZigbeeHeartBeatRspMsg(ZBITF_MAIN_MSG_STRU *pZigbeeItfMsg)
{
    int              iHandlerIdx     = 0;
    unsigned int     ulOldActiveMask = gCcb.ulActiveShadowMask;
    int              iSrcAdr         = 0;
    int              iDelta          = 0;

    APP_PACKET_ONLINE_NOTI_CONF_STRU *pmg = (APP_PACKET_ONLINE_NOTI_CONF_STRU *)&pZigbeeItfMsg->aucData[APP_ZIGBEE_SUB_PROTOL_LENGTH]; 

    //printf("Hbt response 0x%x\r\n",pCanItfMsg->ulCanId);
    
    switch(pmg->hdr.ucDevType)
    {
    case APP_DEV_TYPE_HAND_SET:

        iHandlerIdx = CanCcbZigbeeSearchHandset (pZigbeeItfMsg->usShortAdr);
        
        if (iHandlerIdx >= MAX_HANDLER_NUM)
        {
            VOS_LOG(VOS_LOG_DEBUG,"unknow hs %x\r\n",pZigbeeItfMsg->usShortAdr);

            CcbZigbeeResetHandset(pZigbeeItfMsg->usShortAdr);
            
            return -1;
        }

        iSrcAdr = iHandlerIdx + APP_HAND_SET_BEGIN_ADDRESS;
        
        break;
    default:
        VOS_LOG(VOS_LOG_DEBUG,"unknow dev type %x\r\n",pmg->hdr.ucDevType);
        return -1;
    }
    
    gCcb.aulActMask4Trx[APP_TRX_ZIGBEE] |= (1 << iSrcAdr);
    CcbSetActiveMask(iSrcAdr);

    iDelta = ulOldActiveMask ^ gCcb.ulActiveMask;

    if (iDelta && (iDelta & (1 << iSrcAdr)))
    {
        switch(pmg->hdr.ucDevType)
        {
        case APP_DEV_TYPE_HAND_SET:
            CcbHandlerNotify(iHandlerIdx);
            break;
        }
    }

    return 0;
}

int CanCcbAfDataHeartBeatRspMsg(MAIN_CANITF_MSG_STRU *pCanItfMsg)
{
    int              iHandlerIdx     = 0;
    unsigned int     ulOldActiveMask = gCcb.ulActiveShadowMask;

    APP_PACKET_ONLINE_NOTI_CONF_STRU *pmg = (APP_PACKET_ONLINE_NOTI_CONF_STRU *)&pCanItfMsg->aucData[1 + RPC_POS_DAT0]; 

    int iSrcAdr = CAN_SRC_ADDRESS(pCanItfMsg->ulCanId);

    int iDelta = 0;

    //printf("Hbt response 0x%x\r\n",pCanItfMsg->ulCanId);
    
    switch(pmg->hdr.ucDevType)
    {
    case APP_DEV_TYPE_EXE_BOARD:
    case APP_DEV_TYPE_FLOW_METER:
        if (iSrcAdr != pmg->hdr.ucDevType)
        {
            return -1;
        }
        break;
    case APP_DEV_TYPE_HAND_SET:
        if (iSrcAdr < APP_HAND_SET_BEGIN_ADDRESS)
        {
            return -1;
        }
        iHandlerIdx = iSrcAdr - APP_HAND_SET_BEGIN_ADDRESS;
        break;
    case APP_DEV_TYPE_RF_READER:
        if (iSrcAdr < APP_RF_READER_BEGIN_ADDRESS)
        {
            return -1;
        }
        
        iHandlerIdx = iSrcAdr - APP_RF_READER_BEGIN_ADDRESS;
        break;
    default:
        printf("unknow dev type %x\r\n",pmg->hdr.ucDevType);
        return -1;
    }
    
    gCcb.aulActMask4Trx[APP_TRX_ZIGBEE] |= (1 << iSrcAdr);
    CcbSetActiveMask(iSrcAdr);

    iDelta = ulOldActiveMask ^ gCcb.ulActiveMask;

    if (iDelta && (iDelta & (1 << iSrcAdr)))
    {
        switch(pmg->hdr.ucDevType)
        {
        case APP_DEV_TYPE_EXE_BOARD:
            CcbExeBrdNotify();
            break;
        case APP_DEV_TYPE_FLOW_METER:
            CcbFmBrdNotify();
            break;
        case APP_DEV_TYPE_HAND_SET:
            CcbHandlerNotify(iHandlerIdx);
            break;
        }
    }

    return 0;
}

void CanCcbPmMeasurePostProcess(int iPmId)
{
    switch(gCcb.curWorkState.iMainWorkState4Pw)
    {
    case DISP_WORK_STATE_RUN:
        {
            switch(gCcb.curWorkState.iSubWorkState4Pw)
            {
            case DISP_WORK_SUB_RUN_QTW:
                {
                   if (iPmId == APP_EXE_PM2_NO)
                   {
                       if (haveB2(&gCcb))
                       {
                           if (CcbConvert2Pm2SP(gCcb.ExeBrd.aPMObjs[APP_EXE_PM2_NO].Value.ulV) < CcbGetSp6())
                           {
                              gCcb.bit1B2Empty = TRUE;
    
                              /* stop taking water */
                              CcbStopQtw();
                           }
                           else
                           {
                              gCcb.bit1B2Empty = FALSE;
                           } 
                       }
                   }
                }
                break;
            default:
                break;
            }
            
            if (iPmId == APP_EXE_PM2_NO)
            {
                if (haveB2(&gCcb))
                {
                    float fValue = CcbConvert2Pm2SP(gCcb.ExeBrd.aPMObjs[APP_EXE_PM2_NO].Value.ulV);
                    if (fValue >= B2_FULL)
                    {
                        /* check if EXE already stopped producing water */
                        CanPrepare4Pm2Full();
                        
                        if (gCcb.bit1ProduceWater )
                        {
                           /* stop producing water */
                           if (!SearchWork(work_stop_pw))
                           {
                               CcbInnerWorkStopProduceWater();
                           }
                        }
                    }
                }
            }           

            if (iPmId == APP_EXE_PM3_NO)
            {
                if (gCcb.bit1FillingTank)
                {
                    float fValue = CcbConvert2Pm3SP(gCcb.ExeBrd.aPMObjs[APP_EXE_PM3_NO].Value.ulV);
                    //if (is_B3_FULL(fValue))
                    if(fValue >= B3_FULL)
                    {
                        if (!SearchWork(work_stop_fill_water))
                        {
                            CcbInnerWorkStopFillWater();
                        }                
                    }    
                }
                else
                {
                    float fValue = CcbConvert2Pm3SP(gCcb.ExeBrd.aPMObjs[APP_EXE_PM3_NO].Value.ulV);
                    if ( fValue < CcbGetSp8()
                        && haveB3(&gCcb))
                    {
                        if (!SearchWork(work_start_fill_water))
                        {
                            CcbInnerWorkStartFillWater();
                        }                
                    }                
                }  
                //2019.9.29 add
                if (haveB3(&gCcb))
                {
                    float fValue = CcbConvert2Pm3SP(gCcb.ExeBrd.aPMObjs[APP_EXE_PM3_NO].Value.ulV);
                    if(fValue < CcbGetSp9())
                    {
                        if (gCcb.bit1ProduceWater)
                        {
                            /* stop producing water */
                            if (!SearchWork(work_stop_pw))
                            {
                                CcbInnerWorkStopProduceWater();
                            }
                        }
                    }

                }
            }
        }
        break;
    case DISP_WORK_STATE_LPP:
        break;
    default:
        break;
    }

    switch(gCcb.curWorkState.iMainWorkState)
    {
    case DISP_WORK_STATE_RUN:
        {
            if (gCcb.bit1B1Check4RuningState)
            {
                if (iPmId == APP_EXE_PM1_NO)
                {
                    float fValue = CcbConvert2Pm1Data(gCcb.ExeBrd.aPMObjs[APP_EXE_PM1_NO].Value.ulV);
                    float fThd = 2; /* for desktop machine(unit Bar) */
    
                    switch(gCcb.ulMachineType)
                    {
                    case MACHINE_L_Genie :
                    case MACHINE_L_UP:
                    case MACHINE_L_EDI_LOOP:
                    case MACHINE_L_RO_LOOP:
                        fThd = 5.0 ; /* for Large Machine (unit Bar) */
                        break;
                    default:
                        break;
                    }
                    //2019.3.14
                    if(MACHINE_ADAPT == gCcb.ulMachineType)
                    {
                        if(CcbGetTwFlag() || CcbGetTwPendingFlag())
                        {
                            if (fValue >= fThd)
                            {
                                gCcb.bit1B1UnderPressureDetected = 0;
                            }
                            else
                            {
                                if (!gCcb.bit1B1UnderPressureDetected)
                                {
                                    gCcb.bit1B1UnderPressureDetected = TRUE;
                                    gCcb.ulB1UnderPressureTick = gulSecond;
                                }
                            }
                        }
                    }
                    else
                    {
                        if (fValue >= fThd)
                        {
                            gCcb.bit1B1UnderPressureDetected = 0;
                        }
                        else
                        {
                            if (!gCcb.bit1B1UnderPressureDetected)
                            {
                                gCcb.bit1B1UnderPressureDetected = TRUE;
                                gCcb.ulB1UnderPressureTick = gulSecond;
                            }
                        }
                    }

                    //high work pressure
                    if(fValue >= CcbGetSp33())
                    {
                        if(0 == AlarmHighWorkPress)
                        {
                            AlarmHighWorkPress = 1;
                            CcbNotAlarmFire(DISP_ALARM_PART1,DISP_ALARM_PART1_HIGH_WORK_PRESSURE,TRUE);
                        }
                    }
                    else
                    {
                        if(1 == AlarmHighWorkPress)
                        {
                            AlarmHighWorkPress = 0;
                            CcbNotAlarmFire(DISP_ALARM_PART1,DISP_ALARM_PART1_HIGH_WORK_PRESSURE,FALSE);
                        }
                    }
                    //end
                }       
            }
            
            if (iPmId == APP_EXE_PM2_NO)
            {
                float fValue = CcbConvert2Pm2SP(gCcb.ExeBrd.aPMObjs[APP_EXE_PM2_NO].Value.ulV);
                if (fValue >= B2_FULL)
                {
                    /* check if EXE already stopped producing water */
                    CanPrepare4Pm2Full();
                    
                    if ( gCcb.bit1ProduceWater )
                    {
                       /* stop produce water */
                       if (!SearchWork(work_stop_pw))
                       {
                           CcbInnerWorkStopProduceWater();
                       }
                    }
                }
            }           
    
            if (iPmId == APP_EXE_PM3_NO)
            {
                if (gCcb.bit1FillingTank)
                {
                    float fValue = CcbConvert2Pm3SP(gCcb.ExeBrd.aPMObjs[APP_EXE_PM3_NO].Value.ulV);
                    //if (is_B3_FULL(fValue))
                    if(fValue >= B3_FULL)
                    {
                        if (!SearchWork(work_stop_fill_water))
                        {
                            CcbInnerWorkStopFillWater();
                        }                
                    }    
                }
                else
                {
                    float fValue = CcbConvert2Pm3SP(gCcb.ExeBrd.aPMObjs[APP_EXE_PM3_NO].Value.ulV);
                    if ( fValue < CcbGetSp8()
                        && haveB3(&gCcb))
                    {
                        if (!SearchWork(work_start_fill_water))
                        {
                            CcbInnerWorkStartFillWater();
                        }                
                    }                
                }  
                //2019.9.29 add
                if (haveB3(&gCcb))
                {
                    float fValue = CcbConvert2Pm3SP(gCcb.ExeBrd.aPMObjs[APP_EXE_PM3_NO].Value.ulV);
                    if(fValue < CcbGetSp9())
                    {
                        if (gCcb.bit1ProduceWater)
                        {
                            /* stop producing water */
                            if (!SearchWork(work_stop_pw))
                            {
                                CcbInnerWorkStopProduceWater();
                            }
                        }
                    }
                }

            }
        }
        break;
    case DISP_WORK_STATE_LPP:
        break;
    default:
        break;
    }

}

void CcbNotAlarmFire(int iPart,int iId,int bFired)
{

    int iLength;
    
    NOT_INFO_STRU *pNotify = (NOT_INFO_STRU *)gaucNotifyBuffer;

    NOT_ALARM_ITEM_STRU *pItem = (NOT_ALARM_ITEM_STRU *)pNotify->aucData;

    printf("alarm %d:%d \r\n",iId,bFired);

    pNotify->Hdr.ucCode = DISP_NOT_ALARM;
    iLength             = sizeof(NOT_HEADER_STRU);

    pItem->ucFlag = bFired;
    pItem->ucPart = iPart;
    pItem->ucId   = iId;
    iLength      += sizeof(NOT_ALARM_ITEM_STRU);

    pItem++;
    pItem->ucId = 0XFF;
    iLength    += sizeof(NOT_ALARM_ITEM_STRU);

    DispIndicationEntry(gaucNotifyBuffer,iLength);

}

//2018.12.21 add
void Check_RO_EDI_Alarm(int iEcoId)
{
    float fRej;

    if(!DispGetPwFlag())
    {
        return;
    }

    switch(iEcoId)
    {
    case 1:
        if(gCcb.ulMachineType != MACHINE_PURIST)
        {
            fRej = CcbCalcREJ();
            if (fRej < CcbGetSp2())
            {
                if(!gCcb.bit1AlarmRej)
                {
                    gCcb.bit1AlarmRej = TRUE;
                    gCcb.ulAlarmRejTick = gulSecond;
                }
                else
                {
                    if (!(gCcb.ulFiredAlarmFlags  & ALARM_REJ))
                    {
                        if ((gulSecond - gCcb.ulAlarmRejTick) >= 60*5) //60*5
                        {
                            gCcb.ulFiredAlarmFlags |= ALARM_REJ;

                            CcbNotAlarmFire(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_RO_RESIDUE_RATIO, TRUE);
                        }
                    }
                }

            }
            else
            {
                if (gCcb.ulFiredAlarmFlags  & ALARM_REJ)
                {
                    gCcb.ulFiredAlarmFlags &=  ~ALARM_REJ;
                    CcbNotAlarmFire(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_RO_RESIDUE_RATIO, FALSE);
                }

                if (gCcb.bit1AlarmRej) gCcb.bit1AlarmRej = FALSE;
            }
        }
        //RO
        if (gCcb.ExeBrd.aEcoObjs[APP_EXE_I2_NO].Value.eV.fWaterQ > CcbGetSp3())
        {
            if(!gCcb.bit1AlarmRoPw)
            {
                gCcb.bit1AlarmRoPw   = TRUE;
                gCcb.ulAlarmRoPwTick = gulSecond;
            }
            else
            {
                if (!(gCcb.ulFiredAlarmFlags  & ALARM_ROPW))
                {
                    if ((gulSecond - gCcb.ulAlarmRoPwTick) >= 60*5)
                    {
                        gCcb.ulFiredAlarmFlags |= ALARM_ROPW;

                        CcbNotAlarmFire(DISP_ALARM_PART1,DISP_ALARM_PART1_HIGER_RO_PRODUCT_CONDUCTIVITY,TRUE);
                    }
                }
            }
        }
        else
        {
            if (gCcb.ulFiredAlarmFlags  & ALARM_ROPW)
            {
                gCcb.ulFiredAlarmFlags &=  ~ALARM_ROPW;

                CcbNotAlarmFire(DISP_ALARM_PART1,DISP_ALARM_PART1_HIGER_RO_PRODUCT_CONDUCTIVITY, FALSE);
            }

            if (gCcb.bit1AlarmRoPw) gCcb.bit1AlarmRoPw = FALSE;
        }
        break;
    case 2:
        if (gCcb.ExeBrd.aEcoObjs[APP_EXE_I3_NO].Value.eV.fWaterQ < CcbGetSp4())
        {
            if(!gCcb.bit1AlarmEDI)
            {
                gCcb.bit1AlarmEDI   = TRUE;
                gCcb.ulAlarmEDITick = gulSecond;
            }
            else
            {
                if (!(gCcb.ulFiredAlarmFlags  & ALARM_EDI))
                {
                    if ((gulSecond - gCcb.ulAlarmEDITick) >= 60*5)
                    {
                        gCcb.ulFiredAlarmFlags |= ALARM_EDI;
                        CcbNotAlarmFire(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_EDI_PRODUCT_RESISTENCE,TRUE);
                    }
                }
            }
        }
        else
        {
            if (gCcb.ulFiredAlarmFlags  & ALARM_EDI)
            {
                gCcb.ulFiredAlarmFlags &=  ~ALARM_EDI;

                CcbNotAlarmFire(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_EDI_PRODUCT_RESISTENCE,FALSE);
            }

            if (gCcb.bit1AlarmEDI) gCcb.bit1AlarmEDI = FALSE;
        }
        break;
    default:
        break;
    }
}

void CanCcbEcoMeasurePostProcess(int iEcoId)
{
    switch(gCcb.curWorkState.iMainWorkState4Pw)
    {
    case DISP_WORK_STATE_RUN:
        {
            switch(gCcb.curWorkState.iSubWorkState4Pw)
            {
            case DISP_WORK_SUB_RUN_CIR:
                {
                    switch(gCcb.iCirType)
                    {
                    case CIR_TYPE_UP:
                    {
                        if (iEcoId == APP_EXE_I5_NO)
                        {
                            float fValue = gCcb.ExeBrd.aEcoObjs[APP_EXE_I5_NO].Value.eV.fWaterQ;
                            if (fValue < CcbGetSp7())
                            {
                                if (!gCcb.bit1I54Cir )
                                {
                                    gCcb.bit1I54Cir = TRUE;

                                    /* Notify Handler , late implement */
                                }
                            }  
                            else
                            {
                                if (gCcb.bit1I54Cir) 
                                {
                                    gCcb.bit1I54Cir = FALSE;
                                    
                                    /* Notify Handler , late implement */
                                }
                            }

                            //2018.12.7 add
                            if (fValue < CcbGetSp7())
                            {
                                if (!gCcb.bit1AlarmUP)
                                {
                                    gCcb.bit1AlarmUP = TRUE;
                                    gCcb.ulAlarmUPTick = gulSecond;
                                }
                                else
                                {
                                    /* fire alarm */

                                    if (!(gCcb.ulFiredAlarmFlags  & ALARM_UP))
                                    {
                                        if (gulSecond - gCcb.ulAlarmUPTick >= 30)
                                        {
                                            gCcb.ulFiredAlarmFlags |= ALARM_UP;

                                            CcbNotAlarmFire(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_UP_PRODUCT_RESISTENCE,TRUE);
                                        }
                                    }
                                }

                            }
                            else
                            {
                                if (gCcb.ulFiredAlarmFlags & ALARM_UP)
                                {
                                    gCcb.ulFiredAlarmFlags &=  ~ALARM_UP;

                                    CcbNotAlarmFire(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_UP_PRODUCT_RESISTENCE,FALSE);
                                }

                                if (gCcb.bit1AlarmUP) gCcb.bit1AlarmUP = FALSE;
                            }
                            //
                        }  
                        else if (iEcoId == APP_EXE_I4_NO)
                        {
                            if (gCcb.bit1TocOngoing  && Check_TOC_Alarm)
                            {
                                float fValue = gCcb.ExeBrd.aEcoObjs[APP_EXE_I4_NO].Value.eV.fWaterQ;
                                if(fValue < CcbGetSp30())
                                {
                                    if(!(gCcb.ulFiredAlarmFlags & ALARM_TOC))
                                    {
                                        gCcb.bit1AlarmTOC   = TRUE;

                                        gCcb.ulFiredAlarmFlags |= ALARM_TOC;

                                        CcbNotAlarmFire(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_TOC_SOURCE_WATER_RESISTENCE,TRUE);
                                    }
                                }

                                else
                                {
                                    if ( gCcb.ulFiredAlarmFlags & ALARM_TOC)
                                    {
                                        gCcb.ulFiredAlarmFlags &= ~ALARM_TOC;

                                        CcbNotAlarmFire(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_TOC_SOURCE_WATER_RESISTENCE,FALSE);
                                    }
                                    if(gCcb.bit1AlarmTOC) gCcb.bit1AlarmTOC = FALSE;

                                }
                            }
                            
                        }
                        break;
                    }
                    case CIR_TYPE_HP:
                        {
                            if((gCcb.ulMachineType == MACHINE_UP)
                               || (gCcb.ulMachineType == MACHINE_RO))
                            {
                                if(iEcoId == APP_EXE_I3_NO)
                                {
                                    float fValue = gCcb.ExeBrd.aEcoObjs[APP_EXE_I3_NO].Value.eV.fWaterQ;
                                    if (fValue >= CcbGetSp11())
                                    {
                                        /* Stop Circulation */
                                        CcbInnerWorkStopCir();
                                    }
                                }
                            }
                            else
                            {
                                if (iEcoId == APP_EXE_I4_NO)
                                {
                                    float fValue = gCcb.ExeBrd.aEcoObjs[APP_EXE_I4_NO].Value.eV.fWaterQ;
                                    if (fValue >= CcbGetSp11())
                                    {
                                        /* Stop Circulation */
                                        CcbInnerWorkStopCir();
                                    }
                                }
                            }

                        }
                        break;
                    }
                }
                break;
            case DISP_WORK_SUB_RUN_QTW:
                switch(iEcoId)
                {
                case APP_EXE_I3_NO:
                    if((gCcb.ulMachineType == MACHINE_UP) || (gCcb.ulMachineType == MACHINE_RO))
                    {
                        float fValue = gCcb.ExeBrd.aEcoObjs[APP_EXE_I3_NO].Value.eV.fWaterQ;
                        gCcb.bit1I44Nw = fValue >= CcbGetSp10() ? TRUE : FALSE ;
                    }
                    break;
                case APP_EXE_I4_NO:
                    {
                        float fValue = gCcb.ExeBrd.aEcoObjs[APP_EXE_I4_NO].Value.eV.fWaterQ;

                        /* 2018/05/04 Modify from CcbGetSp11->CcbGetSp10*/
                        gCcb.bit1I44Nw = fValue >= CcbGetSp10() ? TRUE : FALSE ; 
                    }
                    break;
                case APP_EXE_I5_NO:
                    {
                        if (gCcb.iCurTwIdx < MAX_HANDLER_NUM)
                        {
                            if (APP_DEV_HS_SUB_HYPER == gCcb.aHandler[gCcb.iCurTwIdx].iDevType)
                            {
                                float fValue = gCcb.ExeBrd.aEcoObjs[APP_EXE_I5_NO].Value.eV.fWaterQ;
                                if (fValue < CcbGetSp7())
                                {
                                    if (!gCcb.bit1AlarmUP)
                                    {
                                        gCcb.bit1AlarmUP = TRUE;
                                        gCcb.ulAlarmUPTick = gulSecond;
                                    }
                                    else
                                    {
                                        /* fire alarm */
                                        
                                        if (!(gCcb.ulFiredAlarmFlags  & ALARM_UP))
                                        {
                                            if (gulSecond - gCcb.ulAlarmUPTick >= 30)
                                            {
                                                gCcb.ulFiredAlarmFlags |= ALARM_UP;
                                            
                                                CcbNotAlarmFire(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_UP_PRODUCT_RESISTENCE,TRUE);
                                            }
                                        }
                                    }
                                    
                                }
                                else 
                                {
                                    if (gCcb.ulFiredAlarmFlags & ALARM_UP)
                                    {
                                        gCcb.ulFiredAlarmFlags &=  ~ALARM_UP;

                                        CcbNotAlarmFire(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_UP_PRODUCT_RESISTENCE,FALSE);
                                    }

                                    if (gCcb.bit1AlarmUP) gCcb.bit1AlarmUP = FALSE;
                                }
                            }
                        }
                    }
                    break;
                }               
            default:
                break;
            }
        }
        break;
    case DISP_WORK_STATE_LPP:
        break;
    default:
        break;
    }
    Check_RO_EDI_Alarm(iEcoId);
}

void CanCcbRectNMeasurePostProcess(int rectID)
{
    int iTmpData;
    //N1
    if(ex_gCcb.Ex_Rect_State.EX_N_NO[EX_RECT_N1] == 1)
    {
        DispGetOtherCurrent(APP_EXE_N1_NO, &iTmpData);
        if((ex_gulSecond - ex_gCcb.Ex_Alarm_Tick.ulAlarmNRectTick[EX_RECT_N1]) > 15)
        {
            if(iTmpData < 100)
            {
                //Alaram
                if(!ex_gCcb.Ex_Alarm_Bit.bit1AlarmN1)
                {
                    //2018.10.23
                    if(!ex_gCcb.Ex_Alarm_Bit.bit1AlarmDelayN1)
                    {
                        ex_gCcb.Ex_Alarm_Tick.ulAlarmNRectDelay[EX_RECT_N1] = ex_gulSecond;
                        ex_gCcb.Ex_Alarm_Bit.bit1AlarmDelayN1 = 1;
                    }
                    else
                    {
                        if((ex_gulSecond - ex_gCcb.Ex_Alarm_Tick.ulAlarmNRectDelay[EX_RECT_N1]) > 5)
                        {
                            ex_gCcb.Ex_Alarm_Bit.bit1AlarmN1 = 1;
                            gCcb.ulFiredAlarmFlags |= ALARM_N1;
                            CcbNotAlarmFire(DISP_ALARM_PART0,DISP_ALARM_PART0_254UV_OOP,TRUE);
                        }
                    }
                    //
                }
            }
            else
            {
                if(!!ex_gCcb.Ex_Alarm_Bit.bit1AlarmN1)
                {
                    ex_gCcb.Ex_Alarm_Bit.bit1AlarmDelayN1 = 0;
                    ex_gCcb.Ex_Alarm_Bit.bit1AlarmN1 = 0;
                    gCcb.ulFiredAlarmFlags &= ~ALARM_N1;
                    CcbNotAlarmFire(DISP_ALARM_PART0,DISP_ALARM_PART0_254UV_OOP, FALSE);
                }
            }
        }
    }
    //N2
    if(ex_gCcb.Ex_Rect_State.EX_N_NO[EX_RECT_N2] == 1)
    {
        DispGetOtherCurrent(APP_EXE_N2_NO, &iTmpData);
        if((ex_gulSecond - ex_gCcb.Ex_Alarm_Tick.ulAlarmNRectTick[EX_RECT_N2]) > 15)
        {
            if(iTmpData < 100)
            {
                //Alaram
                if(!ex_gCcb.Ex_Alarm_Bit.bit1AlarmN2)
                {
                    //2018.10.23
                    if(!ex_gCcb.Ex_Alarm_Bit.bit1AlarmDelayN2)
                    {
                        ex_gCcb.Ex_Alarm_Tick.ulAlarmNRectDelay[EX_RECT_N2] = ex_gulSecond;
                        ex_gCcb.Ex_Alarm_Bit.bit1AlarmDelayN2 = 1;
                    }
                    else
                    {
                        if((ex_gulSecond - ex_gCcb.Ex_Alarm_Tick.ulAlarmNRectDelay[EX_RECT_N2]) > 5)
                        {
                            ex_gCcb.Ex_Alarm_Bit.bit1AlarmN2 = 1;
                            gCcb.ulFiredAlarmFlags |= ALARM_N2;
                            CcbNotAlarmFire(DISP_ALARM_PART0,DISP_ALARM_PART0_185UV_OOP,TRUE);
                        }
                    }
                    //
                }
            }
            else
            {
                if(!!ex_gCcb.Ex_Alarm_Bit.bit1AlarmN2)
                {
                    ex_gCcb.Ex_Alarm_Bit.bit1AlarmDelayN2 = 0;
                    ex_gCcb.Ex_Alarm_Bit.bit1AlarmN2 = 0;
                    gCcb.ulFiredAlarmFlags &= ~ALARM_N2;
                    CcbNotAlarmFire(DISP_ALARM_PART0,DISP_ALARM_PART0_185UV_OOP, FALSE);
                }
            }
        }
    }

    //N3
    if(ex_gCcb.Ex_Rect_State.EX_N_NO[EX_RECT_N3] == 1)
    {
        DispGetOtherCurrent(APP_EXE_N3_NO, &iTmpData);
        if((ex_gulSecond - ex_gCcb.Ex_Alarm_Tick.ulAlarmNRectTick[EX_RECT_N3]) > 15)
        {
            if(iTmpData < 100)
            {
                //Alaram
                if(!ex_gCcb.Ex_Alarm_Bit.bit1AlarmN3)
                {
                    //2018.10.23
                    if(!ex_gCcb.Ex_Alarm_Bit.bit1AlarmDelayN3)
                    {
                        ex_gCcb.Ex_Alarm_Tick.ulAlarmNRectDelay[EX_RECT_N3] = ex_gulSecond;
                        ex_gCcb.Ex_Alarm_Bit.bit1AlarmDelayN3 = 1;
                    }
                    else
                    {
                        if((ex_gulSecond - ex_gCcb.Ex_Alarm_Tick.ulAlarmNRectDelay[EX_RECT_N3]) > 5)
                        {
                            ex_gCcb.Ex_Alarm_Bit.bit1AlarmN3 = 1;
                            gCcb.ulFiredAlarmFlags |= ALARM_N3;
                            CcbNotAlarmFire(DISP_ALARM_PART0,DISP_ALARM_PART0_TANKUV_OOP,TRUE);
                        }
                    }
                    //
                }

            }
            else
            {
                if(!!ex_gCcb.Ex_Alarm_Bit.bit1AlarmN3)
                {
                    ex_gCcb.Ex_Alarm_Bit.bit1AlarmDelayN3 = 0;
                    ex_gCcb.Ex_Alarm_Bit.bit1AlarmN3 = 0;
                    gCcb.ulFiredAlarmFlags &= ~ALARM_N3;
                    CcbNotAlarmFire(DISP_ALARM_PART0,DISP_ALARM_PART0_TANKUV_OOP, FALSE);
                }
            }
        }
    }
}


void CcbEcoNotify(void)
{
    int iLoop ;

    int iLength;

    int iMask;
    
    NOT_INFO_STRU *pNotify = (NOT_INFO_STRU *)gaucNotifyBuffer;

    NOT_ECO_ITEM_STRU *pItem = (NOT_ECO_ITEM_STRU *)pNotify->aucData;

    pNotify->Hdr.ucCode = DISP_NOT_ECO;

    iLength = sizeof(NOT_HEADER_STRU);

    iMask = CcbGetIObjState(APP_EXE_I_MASK);

    for (iLoop = 0; iLoop < APP_EXE_ECO_NUM; iLoop++ )
    {
        if (iMask & (1 << iLoop))
        {
            pItem->ucId           = (unsigned char )iLoop;
            pItem->fValue         = gCcb.ExeBrd.aEcoObjs[iLoop].Value.eV.fWaterQ;
            pItem->usValue        = gCcb.ExeBrd.aEcoObjs[iLoop].Value.eV.usTemp;
            pItem++;   

            iLength += sizeof(NOT_ECO_ITEM_STRU);
        }
    }


    pItem->ucId = 0XFF;

    iLength += sizeof(NOT_ECO_ITEM_STRU);

    DispIndicationEntry(gaucNotifyBuffer,iLength);

    
//    VOS_LOG(VOS_LOG_DEBUG,"wq %x",iMask);                       
}

void CcbPmNotify(void)
{
    int iLoop ;

    int iLength;

    int iMask;
    
    NOT_INFO_STRU *pNotify = (NOT_INFO_STRU *)gaucNotifyBuffer;

    NOT_PM_ITEM_STRU *pItem = (NOT_PM_ITEM_STRU *)pNotify->aucData;

    pNotify->Hdr.ucCode = DISP_NOT_PM;

    iLength = sizeof(NOT_HEADER_STRU);

    iMask = CcbGetPmObjState(APP_EXE_PM_MASK);

    for (iLoop = 0; iLoop < APP_EXE_PRESSURE_METER; iLoop++ )
    {

        if (iMask & (1 << iLoop))
        {
            pItem->ucId          = (unsigned char )iLoop;
            pItem->ulValue       = gCcb.ExeBrd.aPMObjs[iLoop].Value.ulV;
            pItem++;   

            iLength += sizeof(NOT_PM_ITEM_STRU);
        }
    }

    pItem->ucId = 0XFF;

    iLength += sizeof(NOT_PM_ITEM_STRU);

    DispIndicationEntry(gaucNotifyBuffer,iLength);

    
}


void CcbRectNotify(void)
{
    int iLoop ;

    int iLength;

    int iMask;
    
    NOT_INFO_STRU *pNotify = (NOT_INFO_STRU *)gaucNotifyBuffer;

    NOT_PM_ITEM_STRU *pItem = (NOT_PM_ITEM_STRU *)pNotify->aucData;

    pNotify->Hdr.ucCode = DISP_NOT_RECT;

    iLength = sizeof(NOT_HEADER_STRU);


    iMask = CcbGetSwitchObjState((APP_EXE_RECT_MASK << APP_EXE_RECT_REPORT_POS));

    for (iLoop = 0; iLoop < APP_EXE_RECT_NUM; iLoop++ )
    {

        if ((iMask >> APP_EXE_RECT_REPORT_POS) & (1 << iLoop))
        {
            pItem->ucId          = (unsigned char )iLoop;
            pItem->ulValue       = gCcb.ExeBrd.aRectObjs[iLoop].Value.ulV;
            pItem++;   

            iLength += sizeof(NOT_RECT_ITEM_STRU);
        }
    }

    pItem->ucId = 0XFF;

    iLength += sizeof(NOT_RECT_ITEM_STRU);

    DispIndicationEntry(gaucNotifyBuffer,iLength);

    
}

void CcbGPumpNotify(void)
{
    int iLoop ;

    int iLength;

    int iMask;
    
    NOT_INFO_STRU       *pNotify = (NOT_INFO_STRU *)gaucNotifyBuffer;

    NOT_GPUMP_ITEM_STRU *pItem   = (NOT_GPUMP_ITEM_STRU *)pNotify->aucData;

    pNotify->Hdr.ucCode = DISP_NOT_GPUMP;

    iLength = sizeof(NOT_HEADER_STRU);


    iMask = CcbGetSwitchObjState((APP_EXE_GCMASK << APP_EXE_GPUMP_REPORT_POS));

    for (iLoop = 0; iLoop < APP_EXE_G_PUMP_NUM; iLoop++ )
    {

        if ((iMask >> APP_EXE_VALVE_NUM) & (1 << iLoop))
        {
            pItem->ucId          = (unsigned char )iLoop;
            pItem->ulValue       = gCcb.ExeBrd.aGPumpObjs[iLoop].Value.ulV;
            pItem++;   

            iLength += sizeof(NOT_GPUMP_ITEM_STRU);
        }
    }

    pItem->ucId = 0XFF;

    iLength += sizeof(NOT_GPUMP_ITEM_STRU);

    DispIndicationEntry(gaucNotifyBuffer,iLength);

    
}

void CcbRPumpNotify(void)
{
    int iLoop ;

    int iLength;

    int iMask;
    
    NOT_INFO_STRU       *pNotify = (NOT_INFO_STRU *)gaucNotifyBuffer;

    NOT_RPUMP_ITEM_STRU *pItem   = (NOT_RPUMP_ITEM_STRU *)pNotify->aucData;

    pNotify->Hdr.ucCode = DISP_NOT_RPUMP;

    iLength = sizeof(NOT_HEADER_STRU);


    iMask = CcbGetSwitchObjState((APP_EXE_RCMASK << APP_EXE_RPUMP_REPORT_POS));

    for (iLoop = 0; iLoop < APP_EXE_R_PUMP_NUM; iLoop++ )
    {

        if ((iMask >> APP_EXE_RPUMP_REPORT_POS) & (1 << iLoop))
        {
            pItem->ucId          = (unsigned char )iLoop;
            pItem->ulValue       = gCcb.ExeBrd.aRPumpObjs[iLoop].Value.ulV;
            pItem++;   

            iLength += sizeof(NOT_RPUMP_ITEM_STRU);
        }
    }

    pItem->ucId = 0XFF;

    iLength += sizeof(NOT_RPUMP_ITEM_STRU);

    DispIndicationEntry(gaucNotifyBuffer,iLength);

    
}


void CcbEDINotify(void)
{
    int iLoop ;

    int iLength;

    int iMask;
    
    NOT_INFO_STRU   *pNotify = (NOT_INFO_STRU *)gaucNotifyBuffer;

    NOT_EDI_ITEM_STRU *pItem = (NOT_EDI_ITEM_STRU *)pNotify->aucData;

    pNotify->Hdr.ucCode = DISP_NOT_EDI;

    iLength = sizeof(NOT_HEADER_STRU);


    iMask = CcbGetSwitchObjState((APP_EXE_TMASK << APP_EXE_EDI_REPORT_POS));

    for (iLoop = 0; iLoop < APP_EXE_EDI_NUM; iLoop++ )
    {

        if ((iMask >> APP_EXE_EDI_REPORT_POS) & (1 << iLoop))
        {
            pItem->ucId          = (unsigned char )iLoop;
            pItem->ulValue       = gCcb.ExeBrd.aEDIObjs[iLoop].Value.ulV;
            pItem++;   

            iLength += sizeof(NOT_EDI_ITEM_STRU);
        }
    }

    pItem->ucId = 0XFF;

    iLength += sizeof(NOT_EDI_ITEM_STRU);

    DispIndicationEntry(gaucNotifyBuffer,iLength);

    
}


void CcbFmNotify(void)
{
    int iLoop ;

    int iLength;

    int iMask;
    
    NOT_INFO_STRU  *pNotify = (NOT_INFO_STRU *)gaucNotifyBuffer;

    NOT_FM_ITEM_STRU *pItem = (NOT_FM_ITEM_STRU *)pNotify->aucData;

    pNotify->Hdr.ucCode = DISP_NOT_FM;

    iLength = sizeof(NOT_HEADER_STRU);

    iMask = CcbGetFmObjState(APP_FM_FM_MASK);

    for (iLoop = 0; iLoop < APP_FM_FLOW_METER_NUM; iLoop++ )
    {

        if (iMask & (1 << iLoop))
        {
            pItem->ucId          = (unsigned char )iLoop;
            pItem->ulValue       = gCcb.FlowMeter.aFmObjs[iLoop].Value.ulV;
            pItem++;   

            iLength += sizeof(NOT_FM_ITEM_STRU);
        }
    }

    pItem->ucId = 0XFF;

    iLength += sizeof(NOT_FM_ITEM_STRU);

    DispIndicationEntry(gaucNotifyBuffer,iLength);
}


void CcbNotState(int state)
{

    int iLength;
    
    NOT_INFO_STRU       *pNotify = (NOT_INFO_STRU *)gaucNotifyBuffer;

    NOT_STATE_ITEM_STRU *pItem   = (NOT_STATE_ITEM_STRU *)pNotify->aucData;

    pNotify->Hdr.ucCode = DISP_NOT_STATE;

    iLength = sizeof(NOT_HEADER_STRU);

    pItem->ucId   = state;
    iLength += sizeof(NOT_STATE_ITEM_STRU);

    pItem++;
    pItem->ucId = 0XFF;
    iLength += sizeof(NOT_STATE_ITEM_STRU);

    DispIndicationEntry(gaucNotifyBuffer,iLength);

    
}

void CcbNotSpeed(int iType,int iSpeed)
{

    int iLength;
    
    NOT_INFO_STRU       *pNotify = (NOT_INFO_STRU *)gaucNotifyBuffer;

    NOT_SPEED_ITEM_STRU *pItem   = (NOT_SPEED_ITEM_STRU *)pNotify->aucData;

    pNotify->Hdr.ucCode = DISP_NOT_SPEED;

    iLength = sizeof(NOT_HEADER_STRU);

    pItem->iType   = iType;
    pItem->iSpeed  = iSpeed;
    iLength += sizeof(NOT_SPEED_ITEM_STRU);

    DispIndicationEntry(gaucNotifyBuffer,iLength);

    
}

void CcbNotDecPressure(int iType,int iAction)
{

    int iLength;
    
    NOT_INFO_STRU       *pNotify = (NOT_INFO_STRU *)gaucNotifyBuffer;

    NOT_DECPRE_ITEM_STRU *pItem   = (NOT_DECPRE_ITEM_STRU *)pNotify->aucData;

    pNotify->Hdr.ucCode = DISP_NOT_DEC_PRESSURE;

    iLength = sizeof(NOT_HEADER_STRU);

    pItem->iType   = iType;
    pItem->iAction = iAction;
    
    iLength += sizeof(NOT_DECPRE_ITEM_STRU);

    DispIndicationEntry(gaucNotifyBuffer,iLength);
    
}


void CcbNotSWPressure(void)
{

    int iLength;
    
    NOT_INFO_STRU       *pNotify = (NOT_INFO_STRU *)gaucNotifyBuffer;

    NOT_PM_ITEM_STRU *pItem   = (NOT_PM_ITEM_STRU *)pNotify->aucData;

    pNotify->Hdr.ucCode = DISP_NOT_SW_PRESSURE;

    iLength = sizeof(NOT_HEADER_STRU);

    pItem->ucId      = APP_EXE_PM1_NO;
    pItem->ulValue   = gCcb.ExeBrd.aPMObjs[APP_EXE_PM1_NO].Value.ulV;;
    
    iLength += sizeof(NOT_PM_ITEM_STRU);

    pItem++;
    pItem->ucId = 0XFF;
    iLength += sizeof(NOT_PM_ITEM_STRU);

    DispIndicationEntry(gaucNotifyBuffer,iLength);

    
}



void CcbNotAscInfo(int iInfoIdx)
{

    int iLength;
    
    NOT_INFO_STRU *pNotify = (NOT_INFO_STRU *)gaucNotifyBuffer;

    NOT_ASC_INFO_ITEM_STRU *pItem = (NOT_ASC_INFO_ITEM_STRU *)pNotify->aucData;

    pNotify->Hdr.ucCode = DISP_NOT_ASC_INFO;

    iLength = sizeof(NOT_HEADER_STRU);

    pItem->ucId   = iInfoIdx;
    iLength += sizeof(NOT_ASC_INFO_ITEM_STRU);

    pItem++;
    pItem->ucId = 0XFF;
    iLength += sizeof(NOT_ASC_INFO_ITEM_STRU);

    DispIndicationEntry(gaucNotifyBuffer,iLength);

    
}


void CcbGetParamNotify(int iIndex)
{

    int iLength;
    
    NOT_INFO_STRU *pNotify = (NOT_INFO_STRU *)gaucNotifyBuffer;

    NOT_ASC_INFO_ITEM_STRU *pItem = (NOT_ASC_INFO_ITEM_STRU *)pNotify->aucData;

    pNotify->Hdr.ucCode = DISP_NOT_GET_PARAM;

    iLength = sizeof(NOT_HEADER_STRU);

    pItem->ucId   = iIndex;
    iLength += sizeof(NOT_ASC_INFO_ITEM_STRU);

    pItem++;
    pItem->ucId = 0XFF;
    iLength += sizeof(NOT_ASC_INFO_ITEM_STRU);

    DispIndicationEntry(gaucNotifyBuffer,iLength);
    
}

void CcbNvStaticsNotify(void)
{
    int iLoop ;

    int iLength;

    int iReportFlag = FALSE;
    
    NOT_INFO_STRU *pNotify = (NOT_INFO_STRU *)gaucNotifyBuffer;

    NOT_NVS_ITEM_STRU *pItem = (NOT_NVS_ITEM_STRU *)pNotify->aucData;

    pNotify->Hdr.ucCode = DISP_NOT_NV_STAT;

    iLength = sizeof(NOT_HEADER_STRU);

    for (iLoop = 0; iLoop < APP_EXE_RECT_NUM; iLoop++ )
    {

        if (gCcb.ExeBrd.aRectObjs[iLoop].ulDurSec)
        {
            iReportFlag = TRUE;
        
            pItem->ucId          = (unsigned char )iLoop;
            pItem->ulValue       = gCcb.ExeBrd.aRectObjs[iLoop].ulDurSec;
            gCcb.ExeBrd.aRectObjs[iLoop].ulDurSec = 0;
            gCcb.ExeBrd.aRectObjs[iLoop].ulSec    = DispGetCurSecond();
            pItem++;  
            
            iLength += sizeof(NOT_NVS_ITEM_STRU);
        }

    }

    pItem->ucId = 0XFF;

    iLength += sizeof(NOT_NVS_ITEM_STRU);

    if (iReportFlag) DispIndicationEntry(gaucNotifyBuffer,iLength);
}

/* current only for C4 */
void CcbPumpStaticsNotify(void)
{
    int iIdx = (APP_EXE_C4_NO - APP_EXE_C3_NO);

    if (gCcb.ExeBrd.aGPumpObjs[iIdx].ulDurSec)
    {
        int iLength;
        
        int iReportFlag = FALSE;
        
        NOT_INFO_STRU *pNotify = (NOT_INFO_STRU *)gaucNotifyBuffer;
        
        NOT_FMS_ITEM_STRU *pItem = (NOT_FMS_ITEM_STRU *)pNotify->aucData;
        
        pNotify->Hdr.ucCode = DISP_NOT_TUBEUV_STATE;
        
        iLength = sizeof(NOT_HEADER_STRU);
    
        iReportFlag = TRUE;
    
        pItem->ucId          = (unsigned char )iIdx;
        pItem->ulValue       = gCcb.ExeBrd.aGPumpObjs[iIdx].ulDurSec;
        gCcb.ExeBrd.aGPumpObjs[iIdx].ulDurSec = 0;
        gCcb.ExeBrd.aGPumpObjs[iIdx].ulSec    = DispGetCurSecond();
        pItem++;  
        
        iLength += sizeof(NOT_FMS_ITEM_STRU);

        pItem->ucId = 0XFF;
    
        iLength += sizeof(NOT_FMS_ITEM_STRU);
    
        if (iReportFlag) DispIndicationEntry(gaucNotifyBuffer,iLength);
        
    }

}



/* current only for C4 */
void CcbFmStaticsNotify(void)
{
    int iLoop ;

    int iLength;

    int iReportFlag = FALSE;
    
    NOT_INFO_STRU *pNotify = (NOT_INFO_STRU *)gaucNotifyBuffer;

    NOT_FM_ITEM_STRU *pItem = (NOT_FM_ITEM_STRU *)pNotify->aucData;

    pNotify->Hdr.ucCode = DISP_NOT_FM_STAT;

    iLength = sizeof(NOT_HEADER_STRU);

    for (iLoop = 0; iLoop < APP_FM_FLOW_METER_NUM; iLoop++ )
    {

        if (INVALID_FM_VALUE != gCcb.FlowMeter.aHist[iLoop].curValue.ulV
            && INVALID_FM_VALUE != gCcb.FlowMeter.aHist[iLoop].lstValue.ulV)
        {
            iReportFlag = TRUE;
        
            pItem->ucId          = (unsigned char )iLoop;
            pItem->ulValue       = gCcb.FlowMeter.aHist[iLoop].curValue.ulV 
                                   - gCcb.FlowMeter.aHist[iLoop].lstValue.ulV
                                   + gCcb.FlowMeter.aulHistTotal[iLoop];
            
            gCcb.FlowMeter.aHist[iLoop].curValue.ulV = INVALID_FM_VALUE;
            gCcb.FlowMeter.aHist[iLoop].lstValue.ulV = INVALID_FM_VALUE;
            gCcb.FlowMeter.aulHistTotal[iLoop] = 0;
            
            pItem++;  
            
            iLength += sizeof(NOT_FM_ITEM_STRU);
        }

    }

    pItem->ucId = 0XFF;

    iLength += sizeof(NOT_FM_ITEM_STRU);

    if (iReportFlag) DispIndicationEntry(gaucNotifyBuffer,iLength);

}


void CcbTwStaticsNotify(int iType,int iIdx,QTW_MEAS_STRU *pMeas)
{
    int iLength;

    NOT_INFO_STRU *pNotify = (NOT_INFO_STRU *)gaucNotifyBuffer;

    NOT_NVS_ITEM_STRU *pItem = (NOT_NVS_ITEM_STRU *)pNotify->aucData;

    pNotify->Hdr.ucCode = DISP_NOT_TW_STAT;

    iLength = sizeof(NOT_HEADER_STRU);

    
    pItem->ucType        = (unsigned char )iType;
    pItem->ucId          = (unsigned char )iIdx;
    pItem->ulValue       = pMeas->ulCurFm - pMeas->ulBgnFm;
    pItem->ulBgnTime     = pMeas->ulBgnTm;
    pItem->ulEndTime     = pMeas->ulEndTm;
    pItem++;  
    
    iLength += sizeof(NOT_NVS_ITEM_STRU);


    pItem->ucId = 0XFF;

    iLength += sizeof(NOT_NVS_ITEM_STRU);

    DispIndicationEntry(gaucNotifyBuffer,iLength);
}

void CcbRealTimeQTwVolumnNotify(unsigned int value)
{
    int iLength;

    NOT_INFO_STRU *pNotify = (NOT_INFO_STRU *)gaucNotifyBuffer;

    NOT_REALTIME_QTW_VOLUME_ITEM_STRU *pItem = (NOT_REALTIME_QTW_VOLUME_ITEM_STRU *)pNotify->aucData;

    pNotify->Hdr.ucCode = DISP_NOT_REALTIME_QTW_VOLUME;

    iLength = sizeof(NOT_HEADER_STRU);

    pItem->ulValue = value;

    iLength += sizeof(NOT_REALTIME_QTW_VOLUME_ITEM_STRU);

    DispIndicationEntry(gaucNotifyBuffer,iLength);

}

void CcbQTwVolumnNotify(int iType,int iIdx,QTW_MEAS_STRU *pMeas)
{
    if (INVALID_FM_VALUE != pMeas->ulTotalFm)
    {
        int iLength;
        
        NOT_INFO_STRU *pNotify = (NOT_INFO_STRU *)gaucNotifyBuffer;
        
        NOT_QTW_VOLUME_ITEM_STRU *pItem = (NOT_QTW_VOLUME_ITEM_STRU *)pNotify->aucData;
        
        pNotify->Hdr.ucCode = DISP_NOT_QTW_VOLUME;
        
        iLength = sizeof(NOT_HEADER_STRU);
        
        pItem->ucType        = (unsigned char )iType;
        pItem->ucId          = (unsigned char )iIdx;
        pItem->ulValue       = pMeas->ulTotalFm;
        pItem++;  
        
        iLength += sizeof(NOT_QTW_VOLUME_ITEM_STRU);
        
        pItem->ucId = 0XFF;
        
        iLength += sizeof(NOT_QTW_VOLUME_ITEM_STRU);
        
        DispIndicationEntry(gaucNotifyBuffer,iLength);
    }
}



void CcbWashStateNotify(int iType,int state)
{
    int iLength;

    NOT_INFO_STRU *pNotify = (NOT_INFO_STRU *)gaucNotifyBuffer;

    NOT_WH_ITEM_STRU *pItem = (NOT_WH_ITEM_STRU *)pNotify->aucData;

    pNotify->Hdr.ucCode = DISP_NOT_WH_STAT;

    iLength = sizeof(NOT_HEADER_STRU);

    
    pItem->ucType        = (unsigned char )iType;
    pItem->ucState       = (unsigned char )state;
    pItem++;  
    
    iLength += sizeof(NOT_WH_ITEM_STRU);

    DispIndicationEntry(gaucNotifyBuffer,iLength);
}




void CcbSwithStateNotify(void)
{
    int iLength;

    NOT_INFO_STRU *pNotify = (NOT_INFO_STRU *)gaucNotifyBuffer;

    pNotify->Hdr.ucCode = DISP_NOT_SWITCH_STATE;

    iLength = sizeof(NOT_HEADER_STRU);

    DispIndicationEntry(gaucNotifyBuffer,iLength);
}

void CcbRPumpStateNotify(int ichl,int state)
{
    int iLength;

    NOT_INFO_STRU *pNotify = (NOT_INFO_STRU *)gaucNotifyBuffer;

    NOT_RPUMP_STATE_ITEM_STRU *pItem = (NOT_RPUMP_STATE_ITEM_STRU *)pNotify->aucData;

    pNotify->Hdr.ucCode = DISP_NOT_RPUMP_STATE;

    iLength = sizeof(NOT_HEADER_STRU);

    pItem->ucId = ichl;

    pItem->ucState = state;

    DispIndicationEntry(gaucNotifyBuffer,iLength);
}


void CcbProduceWater(unsigned int ulBgnTm)
{
    int iLength;

    NOT_INFO_STRU *pNotify = (NOT_INFO_STRU *)gaucNotifyBuffer;

    NOT_WP_ITEM_STRU *pItem = (NOT_WP_ITEM_STRU *)pNotify->aucData;

    pNotify->Hdr.ucCode = DISP_NOT_PWDURATION_STAT;

    iLength = sizeof(NOT_HEADER_STRU);

    
    pItem->ucId          = 0;
    pItem->ulBgnTime     = ulBgnTm;
    pItem++;  
    
    iLength += sizeof(NOT_WP_ITEM_STRU);

    pItem->ucId = 0XFF;

    iLength += sizeof(NOT_WP_ITEM_STRU);

    DispIndicationEntry(gaucNotifyBuffer,iLength);
}

void CcbPwVolumeStatistics(void)
{
    int iLength;

    int iLoop;

    NOT_INFO_STRU *pNotify = (NOT_INFO_STRU *)gaucNotifyBuffer;

    NOT_FMS_ITEM_STRU *pItem = (NOT_FMS_ITEM_STRU *)pNotify->aucData;

    pNotify->Hdr.ucCode = DISP_NOT_PWVOLUME_STAT;

    iLength = sizeof(NOT_HEADER_STRU);


    for (iLoop = APP_FM_FM3_NO ; iLoop <= APP_FM_FM4_NO; iLoop++)
    {
        pItem->ucId = iLoop;

        if (gCcb.FlowMeter.aulPwStart[iLoop] != INVALID_FM_VALUE
            && gCcb.FlowMeter.aulPwEnd[iLoop]!= INVALID_FM_VALUE)
        {
             pItem->ulValue                   = gCcb.FlowMeter.aulPwEnd[iLoop] - gCcb.FlowMeter.aulPwStart[iLoop];
             gCcb.FlowMeter.aulPwStart[iLoop] = INVALID_FM_VALUE;
             gCcb.FlowMeter.aulPwEnd[iLoop]   = INVALID_FM_VALUE;
             iLength += sizeof(NOT_FMS_ITEM_STRU);
             pItem++;  
        }

    }
    pItem->ucId = 0XFF;

    iLength += sizeof(NOT_FMS_ITEM_STRU);

    DispIndicationEntry(gaucNotifyBuffer,iLength);
}



void CcbRfidNotify(int ucIndex)
{

    int iLength;

    NOT_INFO_STRU *pNotify = (NOT_INFO_STRU *)gaucNotifyBuffer;

    NOT_RFID_ITEM_STRU *pItem = (NOT_RFID_ITEM_STRU *)pNotify->aucData;

    pNotify->Hdr.ucCode = DISP_NOT_RF_STATE;

    iLength = sizeof(NOT_HEADER_STRU);

    pItem->ucId    = (unsigned char )ucIndex;
    pItem->ucState = (gCcb.aRfReader[ucIndex].bit1RfDetected && gCcb.aRfReader[ucIndex].bit1Active);
    
    pItem++;   

    iLength     += sizeof(NOT_RFID_ITEM_STRU);

    pItem->ucId = 0XFF;

    iLength += sizeof(NOT_RFID_ITEM_STRU);

    DispIndicationEntry(gaucNotifyBuffer,iLength);

    
}

void CcbHandlerNotify(int ucIndex)
{

    int iLength;

    NOT_INFO_STRU *pNotify = (NOT_INFO_STRU *)gaucNotifyBuffer;

    NOT_HANDLER_ITEM_STRU *pItem = (NOT_HANDLER_ITEM_STRU *)pNotify->aucData;

    pNotify->Hdr.ucCode = DISP_NOT_HANDLER_STATE;

    iLength = sizeof(NOT_HEADER_STRU);

    pItem->ucId    = (unsigned char )ucIndex;
    pItem->ucState = gCcb.aHandler[ucIndex].bit1Active;
    
    pItem++;   

    iLength    += sizeof(NOT_HANDLER_ITEM_STRU);

    pItem->ucId = 0XFF;

    iLength += sizeof(NOT_HANDLER_ITEM_STRU);

    DispIndicationEntry(gaucNotifyBuffer,iLength);

    
}

void CcbExeBrdNotify(void)
{

    int iLength;

    NOT_INFO_STRU *pNotify = (NOT_INFO_STRU *)gaucNotifyBuffer;

    NOT_EXEBRD_ITEM_STRU *pItem = (NOT_EXEBRD_ITEM_STRU *)pNotify->aucData;

    pNotify->Hdr.ucCode = DISP_NOT_EXEBRD_STATE;

    iLength = sizeof(NOT_HEADER_STRU);

    pItem->ucId    = 0;
    pItem->ucState = !!(gCcb.ulActiveMask & (1 << APP_PAKCET_ADDRESS_EXE));
    
    pItem++;   

    iLength     += sizeof(NOT_EXEBRD_ITEM_STRU);

    pItem->ucId = 0XFF;

    iLength += sizeof(NOT_EXEBRD_ITEM_STRU);

    DispIndicationEntry(gaucNotifyBuffer,iLength);

    
}

void CcbFmBrdNotify(void)
{

    int iLength;

    NOT_INFO_STRU *pNotify = (NOT_INFO_STRU *)gaucNotifyBuffer;

    NOT_FMBRD_ITEM_STRU *pItem = (NOT_FMBRD_ITEM_STRU *)pNotify->aucData;

    pNotify->Hdr.ucCode = DISP_NOT_FMBRD_STATE;

    iLength = sizeof(NOT_HEADER_STRU);

    pItem->ucId    = 0;
    pItem->ucState = !!(gCcb.ulActiveMask & (1 << APP_PAKCET_ADDRESS_FM));
    
    pItem++;   

    iLength     += sizeof(NOT_FMBRD_ITEM_STRU);

    pItem->ucId = 0XFF;

    iLength += sizeof(NOT_FMBRD_ITEM_STRU);

    DispIndicationEntry(gaucNotifyBuffer,iLength);

    
}

void CcbTocNotify(void)
{
    int iLength;

    NOT_INFO_STRU *pNotify = (NOT_INFO_STRU *)gaucNotifyBuffer;

    NOT_TOC_ITEM_STRU *pItem = (NOT_TOC_ITEM_STRU *)pNotify->aucData;

    VOS_LOG(VOS_LOG_DEBUG,"Enter %s",__FUNCTION__);                       

    pNotify->Hdr.ucCode = DISP_NOT_TOC;

    iLength = sizeof(NOT_HEADER_STRU);

    pItem->fB = gCcb.ExeBrd.tocInfo.fB;
    pItem->fP = gCcb.ExeBrd.tocInfo.fP;

    iLength += sizeof(NOT_TOC_ITEM_STRU);

    DispIndicationEntry(gaucNotifyBuffer,iLength);
   
}

void DispUpdNotify(int iType,int iResult,int iPercent)
{
    int iLength;

    NOT_INFO_STRU *pNotify = (NOT_INFO_STRU *)gaucNotifyBuffer;

    NOT_UPD_ITEM_STRU *pItem = (NOT_UPD_ITEM_STRU *)pNotify->aucData;

    VOS_LOG(VOS_LOG_DEBUG,"Enter %s: %d&%d&%d",__FUNCTION__, iType, iResult, iPercent);                       

    pNotify->Hdr.ucCode = DISP_NOT_UPD;

    iLength = sizeof(NOT_HEADER_STRU);

    pItem->iType    = iType;
    pItem->iResult  = iResult;
    pItem->iPercent = iPercent;

    iLength += sizeof(NOT_UPD_ITEM_STRU);

    DispIndicationEntry(gaucNotifyBuffer,iLength);
   
}


void CanCcbAfDataClientRpt4ExeDinAck(MAIN_CANITF_MSG_STRU *pCanItfMsg)
{
    /* send response */
    unsigned int ulCanId;

    int iPayLoad;
    
    APP_PACKET_CLIENT_RPT_IND_STRU *pmg = (APP_PACKET_CLIENT_RPT_IND_STRU *)&pCanItfMsg->aucData[1 + RPC_POS_DAT0];

    pmg->hdr.ucDevType = APP_DEV_TYPE_MAIN_CTRL;
    pmg->hdr.ucMsgType |= 0x80;

    CAN_BUILD_ADDRESS_IDENTIFIER(ulCanId,APP_PAKCET_ADDRESS_MC,APP_PAKCET_ADDRESS_EXE);

    iPayLoad = APP_POROTOL_PACKET_CLIENT_RPT_IND_TOTAL_LENGTH + sizeof(APP_PACKET_RPT_DIN_STRU);

    CcbSndAfCanCmd(pCanItfMsg->iCanChl,ulCanId,SAPP_CMD_DATA,(unsigned char *)pmg,iPayLoad);

}

void CcbPushState(void)
{     if (gCcb.iWorkStateStackDepth < CCB_WORK_STATE_STACK_DEPTH)
    {
        gCcb.aWorkStateStack[gCcb.iWorkStateStackDepth++] = gCcb.curWorkState;
    }
}

void CcbPopState(void)
{
    if (gCcb.iWorkStateStackDepth > 0)
    {
        gCcb.curWorkState = gCcb.aWorkStateStack[gCcb.iWorkStateStackDepth--];
    }

}

void CcbCancelKeyWork(void *param)
{
    WORK_ITEM_STRU *pWorkItem = (WORK_ITEM_STRU *)param;
    pWorkItem = pWorkItem;
    
}

void work_start_key_fail(int iKeyId)
{
    int aiCont[1] = {-1};

    gCcb.ulKeyWorkStates &= ~(1<<iKeyId); //  finish work
    
    MainSndWorkMsg(WORK_MSG_TYPE_SKH,(unsigned char *)aiCont,sizeof(aiCont));
}

void work_start_key_succ(void)
{
    int aiCont[1] = {0};
    
    MainSndWorkMsg(WORK_MSG_TYPE_SKH,(unsigned char *)aiCont,sizeof(aiCont));
}

//12.18
void work_start_key(void *para)
{
    WORK_ITEM_STRU *pWorkItem = (WORK_ITEM_STRU *)para;
    CCB            *pCcb      = (CCB *)pWorkItem->para;

    int             iTmp,iRet;

    switch((int)pWorkItem->extra)
    {
    case APP_EXE_DIN_RF_KEY:
        {
            /* E10 ON */
            if(is_SWPUMP_FRONT(&gCcb))
            {
                iTmp = (1 << APP_EXE_E10_NO)|(1 << APP_EXE_C3_NO);
            }
            else
            {
                iTmp = (1 << APP_EXE_E10_NO);
            }

            iRet = CcbUpdateSwitch(pWorkItem->id,0,iTmp,iTmp);
            if (iRet )
            {
                VOS_LOG(VOS_LOG_WARNING,"CcbUpdateSwitch Fail %d",iRet);
                work_start_key_fail((int)pWorkItem->extra);

                return ;
            }

            //CanCcbTransState(DISP_WORK_STATE_KP,DISP_WORK_SUB_IDLE);
        }
        break;
    case APP_EXE_DIN_LEAK_KEY:
        {
            /* stop all */
            iTmp = 0;
            iRet = CcbSetSwitch(pWorkItem->id,0,iTmp); // don care modbus exe result
            if (iRet )
            {
                VOS_LOG(VOS_LOG_WARNING,"CcbSetSwitch Fail %d",iRet);

                /* notify ui (late implemnt) */
                work_start_key_fail((int)pWorkItem->extra);

                return ;
            }

            pCcb->bit1LeakKey4Reset = TRUE;
            CcbNotAlarmFire(0XFF,DISP_ALARM_B_LEAK,TRUE);

            CanCcbTransState(DISP_WORK_STATE_KP,DISP_WORK_SUB_IDLE);
        }
        break;
    }

    /* push state */
    // CcbPushState();

    work_start_key_succ();

}

#if 0
void work_start_key(void *para)
{
    WORK_ITEM_STRU *pWorkItem = (WORK_ITEM_STRU *)para;
    CCB            *pCcb      = (CCB *)pWorkItem->para;

    int             iTmp,iRet;

    /* stop all */
    iTmp = 0;
    iRet = CcbSetSwitch(pWorkItem->id,0,iTmp); // don care modbus exe result
    if (iRet )
    {
        VOS_LOG(VOS_LOG_WARNING,"CcbSetSwitch Fail %d",iRet);    
    
        /* notify ui (late implemnt) */
        work_start_key_fail((int)pWorkItem->extra);  
        
        return ;
    }            

    switch((int)pWorkItem->extra)
    {
    case APP_EXE_DIN_RF_KEY:
        {
            /* E10 ON */
            iTmp = 1 << APP_EXE_E10_NO;
            iRet = CcbUpdateSwitch(pWorkItem->id,0,iTmp,iTmp); 
            if (iRet )
            {
                VOS_LOG(VOS_LOG_WARNING,"CcbUpdateSwitch Fail %d",iRet);    
            
                /* notify ui (late implemnt) */
                work_start_key_fail((int)pWorkItem->extra);  
                
                return ;
            }   
            
            CanCcbTransState(DISP_WORK_STATE_KP,DISP_WORK_SUB_IDLE);
        }
        break;
    case APP_EXE_DIN_LEAK_KEY: 
        {
            
            pCcb->bit1LeakKey4Reset = TRUE;
            CcbNotAlarmFire(0XFF,DISP_ALARM_B_LEAK,TRUE);
            
            CanCcbTransState(DISP_WORK_STATE_KP,DISP_WORK_SUB_IDLE);
        }
        break;
    }

    /* push state */
    // CcbPushState();

    work_start_key_succ();

}
#endif

void work_stop_key_succ(void)
{
    int aiCont[1] = {0};
    
    MainSndWorkMsg(WORK_MSG_TYPE_EKH,(unsigned char *)aiCont,sizeof(aiCont));
}

void work_stop_key(void *para)
{
    WORK_ITEM_STRU *pWorkItem = (WORK_ITEM_STRU *)para;
    CCB            *pCcb      = (CCB *)pWorkItem->para;
    int             iRet;
    int             iTmp;

    /* stop all */

    switch((int)pWorkItem->extra)
    {
    case APP_EXE_DIN_RF_KEY:
        {
            /* E10 OFF */
            if(is_SWPUMP_FRONT(&gCcb))
            {
                iTmp = (1 << APP_EXE_E10_NO)|(1 << APP_EXE_C3_NO);
            }
            else
            {
                iTmp = 1 << APP_EXE_E10_NO;
            }
            iRet = CcbUpdateSwitch(pWorkItem->id,0,iTmp,0);
            if (iRet )
            {
                VOS_LOG(VOS_LOG_WARNING,"CcbUpdateSwitch Fail %d",iRet);
            }


            if (DISP_WORK_STATE_IDLE == DispGetWorkState()
                || DISP_WORK_STATE_LPP == DispGetWorkState())
            {
                DispCmdEntry(DISP_CMD_RUN,NULL,0);
            }
        }
        break;
    case APP_EXE_DIN_LEAK_KEY:
        {
            CcbNotAlarmFire(0XFF,DISP_ALARM_B_LEAK,FALSE);
        }
        break;
    }

    iTmp = (int)pWorkItem->extra;

    pCcb->ulKeyWorkStates &= ~(1 << iTmp);

    work_stop_key_succ();

}

#if 0
void work_stop_key(void *para)
{
    WORK_ITEM_STRU *pWorkItem = (WORK_ITEM_STRU *)para;
    CCB            *pCcb      = (CCB *)pWorkItem->para;
    int             iRet;
    int             iTmp;

    /* stop all */

    switch((int)pWorkItem->extra)
    {
    case APP_EXE_DIN_RF_KEY:
        {
            /* E10 OFF */
            iTmp = 1 << APP_EXE_E10_NO;
            iRet = CcbUpdateSwitch(pWorkItem->id,0,iTmp,0); 
            if (iRet )
            {
                VOS_LOG(VOS_LOG_WARNING,"CcbUpdateSwitch Fail %d",iRet);    
            }            
        }
        break;
    case APP_EXE_DIN_LEAK_KEY:
        {
            CcbNotAlarmFire(0XFF,DISP_ALARM_B_LEAK,FALSE);
        }
        break;
    }

    iTmp = (int)pWorkItem->extra;

    pCcb->ulKeyWorkStates &= ~(1 << iTmp);

    work_stop_key_succ();

}
#endif

DISPHANDLE CcbInnerWorkStartKeyWok(int iKey)
{
    int iLoop;
    //2018.12.18 add
    if(iKey == APP_EXE_DIN_RF_KEY || iKey == APP_EXE_DIN_LEAK_KEY)
    {
        if (DISP_WORK_STATE_IDLE != DispGetWorkState())
        {
            DispCmdEntry(DISP_CMD_HALT,NULL,0);
        }

        for (iLoop = 0; iLoop < MAX_HANDLER_NUM; iLoop++)
        {
            if (gCcb.aHandler[iLoop].bit1Qtw)
            {
                gCcb.aHandler[iLoop].bit1Qtw = 0;
            }
        }
    } //end

    WORK_ITEM_STRU *pWorkItem = CcbAllocWorkItem();

    if (!pWorkItem)
    {
        return DISP_INVALID_HANDLE;
    }

    pWorkItem->proc = work_start_key;
    pWorkItem->para = &gCcb;
    pWorkItem->extra = (void *)iKey;
    pWorkItem->cancel = CcbCancelKeyWork;

    CcbAddWork(WORK_LIST_URGENT,pWorkItem);

    return (DISPHANDLE)pWorkItem;

}

/*
  Protect by Mutex
*/
void CcbAbortWork(WORK_ITEM_STRU *pWorkItem)
{

    int id = pWorkItem->id;

    if (!gCcb.ulWorkThdState)
    {
        return;
    }
    
    VOS_LOG(VOS_LOG_WARNING,"CcbAbortWork %d:%d",id,gCcb.ulWorkThdState);    

    if (gCcb.ulWorkThdState & MakeThdState(id, WORK_THREAD_STATE_BLK_MODBUS))
    {
        gCcb.ulWorkThdAbort |= MakeThdState(id,WORK_THREAD_STATE_BLK_MODBUS);
        sp_thread_mutex_lock  ( &gCcb.Ipc.mutex );
        sp_thread_cond_signal ( &gCcb.Ipc.cond  );// ylf: all thread killed
        sp_thread_mutex_unlock( &gCcb.Ipc.mutex );
    }

    if (gCcb.ulWorkThdState & MakeThdState(id, WORK_THREAD_STATE_BLK_TICKING))
    {
        gCcb.ulWorkThdAbort |= MakeThdState(id, WORK_THREAD_STATE_BLK_TICKING);
        sp_thread_sem_post(&gCcb.Sem4Delay[id]);
    }
    
}

DISPHANDLE CcbInnerWorkStopKeyWork(int iKey)
{
    WORK_ITEM_STRU *pWorkItem = CcbAllocWorkItem();

    if (!pWorkItem)
    {
        return DISP_INVALID_HANDLE;
    }

    pWorkItem->proc = work_stop_key;
    pWorkItem->para = &gCcb;
    pWorkItem->extra = (void *)iKey;
    pWorkItem->cancel = NULL;

    CcbAddWork(WORK_LIST_LP,pWorkItem);

    return (DISPHANDLE)pWorkItem;

}

void CanCcbDinProcess(void)
{
    int iLoop;

    VOS_LOG(VOS_LOG_WARNING,"%s %d",__FUNCTION__,gCcb.ExeBrd.ucDinState);    
     
    
    /* check state */
    for (iLoop = 0; iLoop < APP_EXE_DIN_NUM; iLoop++)
    {
        int iDinMask = (1<<iLoop);

        if (iDinMask & APP_EXE_DIN_FAIL_MASK)
        {
            if ((iDinMask & gCcb.ulKeyWorkStates) && (iDinMask & gCcb.ExeBrd.ucDinState))
            {
                // work ongoing
                return ;
            }
        
            if ((iDinMask & gCcb.ulKeyWorkStates) && (!(iDinMask & gCcb.ExeBrd.ucDinState)))
            {
                // stop working
                CcbInnerWorkStopKeyWork(iLoop);
                return ;
            }
        
            if ((!gCcb.ulKeyWorkStates) && (iDinMask & gCcb.ExeBrd.ucDinState))
            {
                /* append work */
                CcbInnerWorkStartKeyWok(iLoop);
        
                gCcb.ulKeyWorkStates |= (1<<iLoop); // prevent further action
                return ;
            }
        }
    }

}

void DispC1Regulator(void)
{
//    if((!gCcb.bit1B1Check4RuningState) && (DISP_ACT_TYPE_SWITCH & gCcb.ExeBrd.aRPumpObjs[0].iActive))
    if(!gCcb.bit1B1Check4RuningState)
    {
        CcbUpdateRPumpObjState(0, 0X0000);
        return;
    }
    sp_thread_mutex_lock( &gCcb.C12Mutex );
    
    if (DISP_ACT_TYPE_SWITCH & gCcb.ExeBrd.aRPumpObjs[0].iActive)
    {
        float ft = (0.1 * gCcb.ExeBrd.aEcoObjs[APP_EXE_I1_NO].Value.eV.usTemp);
        float fv = 0.012*ft*ft - 0.8738*ft + 33;  //0.012*水温2 - 0.8738*水温 + 33

        int speed = DispConvertVoltage2RPumpSpeed(fv);

        if (speed != (0X00FF & gCcb.ExeBrd.ausHoldRegister[APP_EXE_HOLD_REG_RPUMP_OFFSET]))
        {
            DispSetRPump(0,(0XFF00|speed));
        }
    }
    
    sp_thread_mutex_unlock( &gCcb.C12Mutex );
}

int CcbC2Regulator(int id,float fv,int on)
{
    int speed = DispConvertVoltage2RPumpSpeed(fv);
    int iRet = 0;

    if (speed != (0X00FF & gCcb.ExeBrd.ausHoldRegister[APP_EXE_HOLD_REG_RPUMP_OFFSET+1]))
    {
        if (on)
        {
           iRet = CcbSetRPump(id,1,(0XFF00|speed));
        }
        else
        {
            iRet = CcbSetRPump(id,1,(speed));
        }
    }

    if (iRet )
    {
        VOS_LOG(VOS_LOG_WARNING,"CcbC2Regulator Fail %d",iRet);    
        return iRet;
    }   

    return 0;
}

int CanCcbAfDataClientRpt4ExeBoard(MAIN_CANITF_MSG_STRU *pCanItfMsg)
{
    APP_PACKET_CLIENT_RPT_IND_STRU *pmg = (APP_PACKET_CLIENT_RPT_IND_STRU *)&pCanItfMsg->aucData[1 + RPC_POS_DAT0];

    switch(pmg->ucRptType)
    {
    case APP_PACKET_RPT_ECO:
        {
            int iPackets = (pmg->hdr.ucLen - 1) / sizeof (APP_PACKET_RPT_ECO_STRU);
            APP_PACKET_RPT_ECO_STRU *pEco = (APP_PACKET_RPT_ECO_STRU *)pmg->aucData;
            int iLoop;
            int iEco4ZigbeeMask = 0;
            
            for (iLoop = 0; iLoop < iPackets; iLoop++,pEco++ )
            {
                if (pEco->ucId < APP_EXE_ECO_NUM)
                {
                    gCcb.ExeBrd.aEcoObjs[pEco->ucId].Value.eV.fWaterQ = pEco->ev.fWaterQ;
                    gCcb.ExeBrd.aEcoObjs[pEco->ucId].Value.eV.usTemp = pEco->ev.usTemp;
                    gCcb.ExeBrd.ulEcoValidFlags |= 1 << pEco->ucId;

                    //ex_dcj
                    switch(pEco->ucId)
                    {
                    case 0:
                        gCcb.ExeBrd.aEcoObjs[pEco->ucId].Value.eV.fWaterQ *= gCaliParam.pc[DISP_PC_COFF_SOURCE_WATER_CONDUCT].fk;
                        gCcb.ExeBrd.aEcoObjs[pEco->ucId].Value.eV.usTemp *= gCaliParam.pc[DISP_PC_COFF_SOURCE_WATER_TEMP].fk;
                        break;
                    case 1:
                        gCcb.ExeBrd.aEcoObjs[pEco->ucId].Value.eV.fWaterQ *= gCaliParam.pc[DISP_PC_COFF_RO_WATER_CONDUCT].fk;
                        gCcb.ExeBrd.aEcoObjs[pEco->ucId].Value.eV.usTemp *= gCaliParam.pc[DISP_PC_COFF_RO_WATER_TEMP].fk;
                        break;
                    case 2:
                    {
                        gCcb.ExeBrd.aEcoObjs[pEco->ucId].Value.eV.fWaterQ *= gCaliParam.pc[DISP_PC_COFF_EDI_WATER_CONDUCT].fk;
                        gCcb.ExeBrd.aEcoObjs[pEco->ucId].Value.eV.usTemp *= gCaliParam.pc[DISP_PC_COFF_EDI_WATER_TEMP].fk;
                        if(gCcb.ExeBrd.aEcoObjs[pEco->ucId].Value.eV.fWaterQ > 16)
                            gCcb.ExeBrd.aEcoObjs[pEco->ucId].Value.eV.fWaterQ = 16;
                        break;
                    }
                    case 3:
                    {
                        gCcb.ExeBrd.aEcoObjs[pEco->ucId].Value.eV.fWaterQ *= gCaliParam.pc[DISP_PC_COFF_TOC_WATER_CONDUCT].fk;
                        gCcb.ExeBrd.aEcoObjs[pEco->ucId].Value.eV.usTemp *= gCaliParam.pc[DISP_PC_COFF_TOC_WATER_TEMP].fk;
                        if(gCcb.ExeBrd.aEcoObjs[pEco->ucId].Value.eV.fWaterQ > 18.2)
                            gCcb.ExeBrd.aEcoObjs[pEco->ucId].Value.eV.fWaterQ = 18.2;
                        break;
                    }
                    case 4:
                    {
                        gCcb.ExeBrd.aEcoObjs[pEco->ucId].Value.eV.fWaterQ *= gCaliParam.pc[DISP_PC_COFF_UP_WATER_CONDUCT].fk;
                        gCcb.ExeBrd.aEcoObjs[pEco->ucId].Value.eV.usTemp *= gCaliParam.pc[DISP_PC_COFF_UP_WATER_TEMP].fk;
                        if(gCcb.ExeBrd.aEcoObjs[pEco->ucId].Value.eV.fWaterQ > 18.2)
                            gCcb.ExeBrd.aEcoObjs[pEco->ucId].Value.eV.fWaterQ = 18.2;
                        break;
                    }
                    }

                    if(gCcb.ExeBrd.aEcoObjs[pEco->ucId].Value.eV.fWaterQ < 0)
                        gCcb.ExeBrd.aEcoObjs[pEco->ucId].Value.eV.fWaterQ = 0;

                    if(gCcb.ExeBrd.aEcoObjs[pEco->ucId].Value.eV.usTemp < 0)
                        gCcb.ExeBrd.aEcoObjs[pEco->ucId].Value.eV.usTemp = 0;
                    //end
                    CanCcbEcoMeasurePostProcess(pEco->ucId);

                    //2019.3.26 add
                    switch(gCcb.ulMachineType)
                    {
                    case MACHINE_L_Genie:
                    case MACHINE_L_EDI_LOOP:
                    case MACHINE_Genie:
                    case MACHINE_EDI:
                    {
                        if(!haveHPCir(&gCcb) && (APP_EXE_I3_NO == pEco->ucId))
                        {
                            iEco4ZigbeeMask |= 0x1 << APP_EXE_I4_NO;
                        }
                        break;
                    }
                    case MACHINE_L_UP:
                    case MACHINE_L_RO_LOOP:
                    case MACHINE_UP:
                    case MACHINE_RO:
                    {
                        if(!haveHPCir(&gCcb) && (APP_EXE_I2_NO == pEco->ucId))
                        {
                            iEco4ZigbeeMask |= 0x1 << APP_EXE_I4_NO;
                        }
                        break;
                    }
                    case MACHINE_ADAPT:
                    {
                        if(APP_EXE_I2_NO == pEco->ucId)
                        {
                            iEco4ZigbeeMask |= 0x1 << APP_EXE_I4_NO;
                        }
                        break;
                    }
                    }
                    //end

                    if (APP_EXE_I4_NO == pEco->ucId)
                    {
                        iEco4ZigbeeMask |= 0x1 << APP_EXE_I4_NO;
                    }
                    else if (APP_EXE_I5_NO == pEco->ucId)
                    {
                        iEco4ZigbeeMask |= 0x1 << APP_EXE_I5_NO;;
                    }
                    //VOS_LOG(VOS_LOG_DEBUG,"wq %d&%f&%u",pEco->ucId,pEco->ev.fWaterQ,pEco->ev.usTemp);                       
                }
            }

            if (iEco4ZigbeeMask)
            {
                DispSndHoEco(iEco4ZigbeeMask);
            }

            CcbEcoNotify();
        }
        break;
    case APP_PACKET_RPT_PM:
        {
            int iPackets = (pmg->hdr.ucLen - 1) / sizeof (APP_PACKET_RPT_PM_STRU);
            APP_PACKET_RPT_PM_STRU *pPm = (APP_PACKET_RPT_PM_STRU *)pmg->aucData;
            int iLoop;
            
            for (iLoop = 0; iLoop < iPackets; iLoop++,pPm++ )
            {
                if (pPm->ucId < APP_EXE_PRESSURE_METER)
                {
                    gCcb.ExeBrd.aPMObjs[pPm->ucId].Value.ulV = pPm->ulValue;
                    gCcb.ExeBrd.ulPmValidFlags |= 1 << (pPm->ucId);

                    CanCcbPmMeasurePostProcess(pPm->ucId);
                    
                }
            }
            
            CcbPmNotify();
        }
        break;
    case APP_PACKET_RPT_RECT:
        {
            int iPackets = (pmg->hdr.ucLen - 1) / sizeof (APP_PACKET_RPT_RECT_STRU);
            APP_PACKET_RPT_PM_STRU *pPm = (APP_PACKET_RPT_RECT_STRU *)pmg->aucData;
            int iLoop;
            
            for (iLoop = 0; iLoop < iPackets; iLoop++,pPm++ )
            {
                if (pPm->ucId < APP_EXE_PRESSURE_METER)
                {
                    gCcb.ExeBrd.aRectObjs[pPm->ucId].Value.ulV = pPm->ulValue;
                    gCcb.ExeBrd.ulRectValidFlags |= 1 << (pPm->ucId);
                }
            }

            CcbRectNotify();
            //CanCcbRectNMeasurePostProcess(pPm->ucId); //ex
        }
        break;    
    case APP_PACKET_RPT_GPUMP:
        {
            int iPackets = (pmg->hdr.ucLen - 1) / sizeof (APP_PACKET_RPT_GPUMP_STRU);
            APP_PACKET_RPT_GPUMP_STRU *pPm = (APP_PACKET_RPT_GPUMP_STRU *)pmg->aucData;
            int iLoop;
            
            for (iLoop = 0; iLoop < iPackets; iLoop++,pPm++ )
            {
                if (pPm->ucId < APP_EXE_G_PUMP_NUM)
                {
                    gCcb.ExeBrd.aGPumpObjs[pPm->ucId].Value.ulV = pPm->ulValue;

                }
            }
            
            CcbGPumpNotify();
        }
        break;             
    case APP_PACKET_RPT_RPUMP:
        {
            int iPackets = (pmg->hdr.ucLen - 1) / sizeof (APP_PACKET_RPT_RPUMP_STRU);
            APP_PACKET_RPT_RPUMP_STRU *pPm = (APP_PACKET_RPT_RPUMP_STRU *)pmg->aucData;
            int iLoop;
            
            for (iLoop = 0; iLoop < iPackets; iLoop++,pPm++ )
            {
                if (pPm->ucId < APP_EXE_R_PUMP_NUM)
                {
                    gCcb.ExeBrd.aRPumpObjs[pPm->ucId].Value.ulV = pPm->ulValue;

                }
            }
            
            CcbRPumpNotify();
        }
        break;       
    case APP_PACKET_RPT_EDI:
        {
            int iPackets = (pmg->hdr.ucLen - 1) / sizeof (APP_PACKET_RPT_EDI_STRU);
            APP_PACKET_RPT_EDI_STRU *pPm = (APP_PACKET_RPT_EDI_STRU *)pmg->aucData;
            int iLoop;
            
            for (iLoop = 0; iLoop < iPackets; iLoop++,pPm++ )
            {
                if (pPm->ucId < APP_EXE_EDI_NUM)
                {
                    gCcb.ExeBrd.aEDIObjs[pPm->ucId].Value.ulV = pPm->ulValue;

                }
            }
            
            CcbEDINotify();
        }
        break;               
    case APP_PACKET_DIN_STATUS:
        {
            APP_PACKET_RPT_DIN_STRU *pDin = (APP_PACKET_RPT_DIN_STRU *)pmg->aucData;
            
            gCcb.ExeBrd.ucDinState = (~pDin->ucState) & APP_EXE_DIN_MASK;

            CanCcbAfDataClientRpt4ExeDinAck(pCanItfMsg);

            // do something here
            CanCcbDinProcess();
        }        
        break;
    case APP_PACKET_MT_STATUS:
        break;
    case APP_PACKET_RPT_TOC:
        {
            APP_PACKET_RPT_TOC_STRU *pToc = (APP_PACKET_RPT_TOC_STRU *)pmg->aucData;

            gCcb.ExeBrd.tocInfo = *pToc;

            CcbTocNotify();
        }        
        break;
    }
    return 0;
}

int CanCcbAfDataClientRpt4RFReader(MAIN_CANITF_MSG_STRU *pCanItfMsg)
{
    APP_PACKET_CLIENT_RPT_IND_STRU *pmg = (APP_PACKET_CLIENT_RPT_IND_STRU *)&pCanItfMsg->aucData[1 + RPC_POS_DAT0];
    int     iSrcAdr  = CAN_SRC_ADDRESS(pCanItfMsg->ulCanId);
    unsigned char ucResult;

    uint8_t ucIndex  = (iSrcAdr - APP_RF_READER_BEGIN_ADDRESS);

    if (ucIndex >=  MAX_RFREADER_NUM)
    {
        ucResult = APP_PACKET_RF_NO_DEVICE;
        goto end;
    }
        
    switch(pmg->ucRptType)
    {
    case APP_PACKET_RPT_RF_STATE:
        {
            APP_PACKET_RPT_RF_STATE_STRU *pRfState = (APP_PACKET_RPT_RF_STATE_STRU *)pmg->aucData;

            RFREADER_STRU *pRfReader = &gCcb.aRfReader[ucIndex];

            pRfReader->bit1RfDetected = pRfState->ucState;

            if (pRfState->ucState)
            {
                APP_PACKET_RPT_RF_STATE_CONT_STRU *pCnt = (APP_PACKET_RPT_RF_STATE_CONT_STRU *)pRfState->aucData;
                pRfReader->ucBlkNum  = pCnt->ucBlkNum;
                pRfReader->ucBlkSize = pCnt->ucBlkSize;
                memcpy(pRfReader->aucUid,pCnt->aucUid,8);
                
                VOS_LOG(VOS_LOG_WARNING,"RF detected %d&%d",pCnt->ucBlkNum,pCnt->ucBlkSize);  
            }
            else
            {
                pRfReader->bit1RfContValid = FALSE;
                VOS_LOG(VOS_LOG_WARNING,"RF lost");  
            }
            CcbRfidNotify(ucIndex);
        }
        break;
    default:
        break;
    }

    return 0;

end : 
    return ucResult;

}

int CcbReadRfid(int iIndex,int iTime,int offset,int len)
{
    int iRet;

    struct timeval now;
    struct timespec outtime;
    
    sp_thread_mutex_lock( &gCcb.Ipc4Rfid.mutex );

    {
        char buf[64];
        IAP_CAN_CMD_STRU *pCmd = (IAP_CAN_CMD_STRU *)buf;
    
        APP_PACKET_RF_STRU *pHo = (APP_PACKET_RF_STRU *)pCmd->data;

        pCmd->iCanChl     = 0;
        pCmd->ucCmd       = SAPP_CMD_DATA;
        pCmd->iPayLoadLen = APP_PROTOL_HEADER_LEN;
        pCmd->ulCanId     = iIndex;
    
        pHo->hdr.ucLen     = 1 + sizeof(APP_PACKET_RF_READ_REQ_STRU);
        pHo->hdr.ucTransId = 0XF0;
        pHo->hdr.ucDevType = APP_DEV_TYPE_MAIN_CTRL;
        pHo->hdr.ucMsgType = APP_PACKET_RF_OPERATION;
    
        pHo->ucOpsType     = APP_PACKET_RF_READ;

        {
            APP_PACKET_RF_READ_REQ_STRU *pRd = (APP_PACKET_RF_READ_REQ_STRU *)pHo->aucData;

            pRd->ucLen = len;
            pRd->ucOff = offset;
            
        }   
        pCmd->iPayLoadLen += pHo->hdr.ucLen;   
        iRet = DispAfRfEntry(pCmd);
        
        if (0 != iRet)
        {
            sp_thread_mutex_unlock( &gCcb.Ipc4Rfid.mutex );
            
            return -1;
        }
    }

    gCcb.iRfIdRequest = 0X1;
    
    gettimeofday(&now, NULL);
    outtime.tv_sec  = now.tv_sec + iTime/1000;
    outtime.tv_nsec = (now.tv_usec + (iTime  % 1000)*1000)* 1000;

    iRet = sp_thread_cond_timedwait( &gCcb.Ipc4Rfid.cond, &gCcb.Ipc4Rfid.mutex ,&outtime);//ylf: thread sleep here
    if(ETIMEDOUT == iRet)
    {
        gCcb.iRfIdRequest &= ~0X1;
    
        sp_thread_mutex_unlock( &gCcb.Ipc4Rfid.mutex );
        return -1;
    }

    gCcb.iRfIdRequest &= ~0X1;

    sp_thread_mutex_unlock( &gCcb.Ipc4Rfid.mutex );

    return 0;
}

int CcbGetRfidCont(int iIndex,int offset,int len,unsigned char *pucData)
{
    if (iIndex >= MAX_RFREADER_NUM)
    {
        return 0;
    }
    if (gCcb.aRfReader[iIndex].bit1RfContValid)
    {
        memcpy(pucData,&gCcb.aRfReader[iIndex].aucRfCont[offset],len);

        return len;
    }

    return 0;
}


int CcbWriteRfid(int iIndex,int iTime,int offset,int len,unsigned char *pucData)
{
    int iRet;

    struct timeval now;
    struct timespec outtime;
    
    sp_thread_mutex_lock( &gCcb.Ipc4Rfid.mutex );

    {
        char buf[64];
        IAP_CAN_CMD_STRU *pCmd = (IAP_CAN_CMD_STRU *)buf;
        
        APP_PACKET_RF_STRU *pHo = (APP_PACKET_RF_STRU *)pCmd->data;
        
        pCmd->iCanChl     = 0;
        pCmd->ucCmd       = SAPP_CMD_DATA;
        pCmd->iPayLoadLen = APP_PROTOL_HEADER_LEN;
        pCmd->ulCanId     = iIndex ;
        
        pHo->hdr.ucLen     = 1 + 2;
        pHo->hdr.ucTransId = 0XF0;
        pHo->hdr.ucDevType = APP_DEV_TYPE_MAIN_CTRL;
        pHo->hdr.ucMsgType = APP_PACKET_RF_OPERATION;
        
        pHo->ucOpsType     = APP_PACKET_RF_WRITE;
        
        {
            APP_PACKET_RF_WRITE_REQ_STRU *pWr = (APP_PACKET_RF_WRITE_REQ_STRU *)pHo->aucData;
        
            pWr->ucLen = len;
            pWr->ucOff = offset;
            
            pHo->hdr.ucLen += pWr->ucLen;
            
            memcpy(pWr->aucData,pucData,len);
            
        }   
        pCmd->iPayLoadLen += pHo->hdr.ucLen;  
        
        iRet = DispAfRfEntry(pCmd);      

        if (0 != iRet)
        {
            sp_thread_mutex_unlock( &gCcb.Ipc4Rfid.mutex );
            
            return -1;
        }
    }

    gCcb.iRfIdRequest = 0X2;
    
    gettimeofday(&now, NULL);
    outtime.tv_sec  = now.tv_sec + iTime/1000;
    outtime.tv_nsec = (now.tv_usec + (iTime  % 1000)*1000)* 1000;

    iRet = sp_thread_cond_timedwait( &gCcb.Ipc4Rfid.cond, &gCcb.Ipc4Rfid.mutex ,&outtime);//ylf: thread sleep here
    if(ETIMEDOUT == iRet)
    {
        gCcb.iRfIdRequest &= ~0X2;
    
        sp_thread_mutex_unlock( &gCcb.Ipc4Rfid.mutex );
        return -1;
    }

    gCcb.iRfIdRequest &= ~0X2;

    sp_thread_mutex_unlock( &gCcb.Ipc4Rfid.mutex );

    return 0;
}


int CcbScanRfid(int iIndex,int iTime)
{
    int iRet;

    struct timeval now;
    struct timespec outtime;
    
    sp_thread_mutex_lock( &gCcb.Ipc4Rfid.mutex );

    {
        char buf[64];
        IAP_CAN_CMD_STRU *pCmd = (IAP_CAN_CMD_STRU *)buf;
    
        APP_PACKET_RF_STRU *pHo = (APP_PACKET_RF_STRU *)pCmd->data;

        pCmd->iCanChl     = 0;
        pCmd->ucCmd       = SAPP_CMD_DATA;
        pCmd->iPayLoadLen = APP_PROTOL_HEADER_LEN;
        pCmd->ulCanId     = iIndex ;
    
        pHo->hdr.ucLen     = 1 + 0;
        pHo->hdr.ucTransId = 0XF0;
        pHo->hdr.ucDevType = APP_DEV_TYPE_MAIN_CTRL;
        pHo->hdr.ucMsgType = APP_PACKET_RF_OPERATION;
        pHo->ucOpsType     = APP_PACKET_RF_SEARCH;

        pCmd->iPayLoadLen += pHo->hdr.ucLen;  
        
        iRet = DispAfRfEntry(pCmd);    

        if (0 != iRet)
        {
            sp_thread_mutex_unlock( &gCcb.Ipc4Rfid.mutex );
            
            return -1;
        }
    }

    gCcb.iRfIdRequest = 0X4;
    
    gettimeofday(&now, NULL);
    outtime.tv_sec  = now.tv_sec + iTime/1000;
    outtime.tv_nsec = (now.tv_usec + (iTime  % 1000)*1000)* 1000;

    iRet = sp_thread_cond_timedwait( &gCcb.Ipc4Rfid.cond, &gCcb.Ipc4Rfid.mutex ,&outtime);//ylf: thread sleep here
    if(ETIMEDOUT == iRet)
    {
        gCcb.iRfIdRequest &= ~0X4;
    
        sp_thread_mutex_unlock( &gCcb.Ipc4Rfid.mutex );
        return -1;
    }

    gCcb.iRfIdRequest &= ~0X4;

    sp_thread_mutex_unlock( &gCcb.Ipc4Rfid.mutex );

    return 0;
}

int CcbSetRPump(int id,int iChl,unsigned int ulValue)
{
   return CcbSetExeHoldRegs(id,APP_EXE_HOLD_REG_RPUMP_OFFSET + iChl,ulValue);
}

int DispSetRPump(int iChl,unsigned int ulValue)
{
    if (0 == (gCcb.ulActiveMask & (1 << APP_DEV_TYPE_EXE_BOARD)))
    {
        return -1;
    }

   return CcbSetRPump(WORK_LIST_NUM,iChl,ulValue);
}

void DispSetTocState(int iStage)
{        char buf[32];
    
    APP_PACKET_EXE_STRU *pExe = (APP_PACKET_EXE_STRU *)buf;

    APP_PACKET_EXE_TOC_REQ_STRU *pReq = (APP_PACKET_EXE_TOC_REQ_STRU *)pExe->aucData;

    unsigned int ulIdentifier ;

    VOS_LOG(VOS_LOG_DEBUG,"Enter %s:%d",__FUNCTION__,iStage);  

    pExe->hdr.ucLen         = 1;
    pExe->hdr.ucTransId     = APP_DEV_TYPE_MAIN_CTRL;
    pExe->hdr.ucDevType     = APP_DEV_TYPE_MAIN_CTRL;
    pExe->hdr.ucMsgType     = APP_PACKET_EXE_OPERATION;

    pExe->ucOpsType         = APP_PACKET_EXE_TOC;

    pReq->ucStage           = iStage;
    pExe->hdr.ucLen        += sizeof(APP_PACKET_EXE_TOC_REQ_STRU);

    CAN_BUILD_ADDRESS_IDENTIFIER(ulIdentifier,APP_DEV_TYPE_MAIN_CTRL,APP_DEV_TYPE_EXE_BOARD);

    CcbSndAfCanCmd(0,ulIdentifier,SAPP_CMD_DATA,(unsigned char *)buf,APP_PROTOL_HEADER_LEN + pExe->hdr.ucLen );
}


void CcbStopQtw(void)
{
    int iLoop;

    VOS_LOG(VOS_LOG_WARNING,"CcbStopQtw");    
    
    if (!SearchWork(work_stop_qtw))
    {
        for (iLoop = 0; iLoop < MAX_HANDLER_NUM; iLoop++)
        {
            if (gCcb.aHandler[iLoop].bit1Qtw)
            {
                CcbInnerWorkStopQtw(iLoop);
            }
        }
    }    

}

int CcbGetOtherKeyState(int iSelfKey)
{
    return (gCcb.ExeBrd.ucDinState & (~(1<<iSelfKey)));
}

int CanCcbAfDataClientRpt4FlowMeter(MAIN_CANITF_MSG_STRU *pCanItfMsg)
{
    APP_PACKET_CLIENT_RPT_IND_STRU *pmg = (APP_PACKET_CLIENT_RPT_IND_STRU *)&pCanItfMsg->aucData[1 + RPC_POS_DAT0];

    switch(pmg->ucRptType)
    {
    case APP_PACKET_RPT_FM:
         {
            int iPackets = (pmg->hdr.ucLen - 1) / sizeof (APP_PACKET_RPT_FM_STRU);
            APP_PACKET_RPT_FM_STRU *pFM = (APP_PACKET_RPT_FM_STRU *)pmg->aucData;
            int iLoop;
            int iTmp = 0;
            
            for (iLoop = 0; iLoop < iPackets; iLoop++,pFM++ )
            {
                if (pFM->ucId < APP_FM_FLOW_METER_NUM)
                {
                    gCcb.FlowMeter.aFmObjs[pFM->ucId].Value.ulV = pFM->ulValue;
                    gCcb.FlowMeter.ulFmValidFlags |= 1 << pFM->ucId;

                    if (INVALID_FM_VALUE == gCcb.FlowMeter.aHist[pFM->ucId].lstValue.ulV)
                    {
                        gCcb.FlowMeter.aHist[pFM->ucId].lstValue.ulV = gCcb.FlowMeter.aHist[pFM->ucId].curValue.ulV ;
                    }

                    gCcb.FlowMeter.aHist[pFM->ucId].curValue.ulV = pFM->ulValue;

                    iTmp |= 1 << iLoop;

                    if (INVALID_FM_VALUE == gCcb.FlowMeter.aulPwStart[pFM->ucId])
                    {
                        gCcb.FlowMeter.aulPwStart[pFM->ucId] = pFM->ulValue;
                    }
                    
                }
            }
            CcbFmNotify();

            if (iTmp & (1 << APP_FM_FM1_NO))
            {
                if (DISP_WORK_STATE_RUN == gCcb.curWorkState.iMainWorkState4Pw
                    && DISP_WORK_SUB_RUN_QTW == gCcb.curWorkState.iSubWorkState4Pw)
                {
                    QTW_MEAS_STRU *pQtw = &gCcb.QtwMeas;
                    
                    if (INVALID_FM_VALUE == pQtw->ulBgnFm )
                    {
                        pQtw->ulBgnFm = gCcb.FlowMeter.aFmObjs[APP_FM_FM1_NO].Value.ulV;
                    }

                    pQtw->ulCurFm = gCcb.FlowMeter.aFmObjs[APP_FM_FM1_NO].Value.ulV;

                    if (gCcb.iCurTwIdx < MAX_HANDLER_NUM)
                    {
                        pQtw = &gCcb.aHandler[gCcb.iCurTwIdx].QtwMeas; 
                        
                        if (INVALID_FM_VALUE == pQtw->ulBgnFm )
                        {
                            pQtw->ulBgnFm = gCcb.FlowMeter.aFmObjs[APP_FM_FM1_NO].Value.ulV;
                        }
                        
                        pQtw->ulCurFm = gCcb.FlowMeter.aFmObjs[APP_FM_FM1_NO].Value.ulV;
                    }


                    if (CcbConvert2Fm1Data(gCcb.QtwMeas.ulCurFm - gCcb.QtwMeas.ulBgnFm) >= gCcb.QtwMeas.ulTotalFm
                        && gCcb.QtwMeas.ulTotalFm != INVALID_FM_VALUE)
                    {
                        /* enough water has been taken */
                        CcbStopQtw();
                    }
                    CcbRealTimeQTwVolumnNotify(CcbConvert2Fm1Data(gCcb.QtwMeas.ulCurFm - gCcb.QtwMeas.ulBgnFm));
                }
            }

            if (gCcb.bit1ProduceWater)
            {
               /*Calc Flow */
               switch(gCcb.ulMachineType)
               {
               case MACHINE_L_Genie:
               case MACHINE_L_EDI_LOOP:
               
                   if (iTmp & (1 << APP_FM_FM3_NO))
                   {
                       if (INVALID_FM_VALUE == gCcb.ulLstRopFlow)
                       {
                           gCcb.ulLstRopFlow = gCcb.FlowMeter.aFmObjs[APP_FM_FM3_NO].Value.ulV;
                           gCcb.ulLstRopTick = getEventCount();
                       }

                       {
                           int iTmDelta = getEventCount() - gCcb.ulLstRopTick;
                           int iFmDelta;
                   
                           if ((iTmDelta >= (FM_CALC_PERIOD/PERIOD_EVENT_LENGTH))
                               && (gCcb.ulLstRopFlow != INVALID_FM_VALUE))
                           {
                               iFmDelta = gCcb.FlowMeter.aFmObjs[APP_FM_FM3_NO].Value.ulV - gCcb.ulLstRopFlow;

                               gCcb.ulRopVelocity = (CcbConvert2Fm3Data(iFmDelta) * TOMLPERHOUR / iTmDelta);

                               gCcb.ulLstRopFlow = gCcb.FlowMeter.aFmObjs[APP_FM_FM3_NO].Value.ulV;
                               gCcb.ulLstRopTick = getEventCount();

                               if (gCcb.ulRopVelocity < CcbGetSp15()*1000)
                               {
                                  gCcb.iRopVCheckLowEventCount++;

                                  if (gCcb.iRopVCheckLowEventCount * FM_CALC_PERIOD >= FM_ALARM_CHECK_PERIOD)
                                  {
                                      if (!gCcb.bit1AlarmROPLV) 
                                      {
                                          gCcb.bit1AlarmROPLV = TRUE;
          
                                          gCcb.ulFiredAlarmFlags |= ALARM_ROPLV;
                                          
                                          CcbNotAlarmFire(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_RO_PRODUCT_FLOWING_VELOCITY,TRUE);

                                          /* stop EDI */
                                      }
                                      
                                  }
                                  gCcb.iRopVCheckLowRestoreCount = 0;
                               }
                               else
                               {
                                  if (gCcb.bit1AlarmROPLV)
                                  {
                                      gCcb.iRopVCheckLowRestoreCount++;

                                      if (gCcb.iRopVCheckLowRestoreCount  * FM_CALC_PERIOD >= FM_ALARM_CHECK_PERIOD)
                                      {
                                          gCcb.bit1AlarmROPLV = FALSE;

                                          if (gCcb.ulFiredAlarmFlags  & ALARM_ROPLV)
                                          {
                                              gCcb.ulFiredAlarmFlags &= ~ALARM_ROPLV;
                                              
                                              CcbNotAlarmFire(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_RO_PRODUCT_FLOWING_VELOCITY,FALSE);

                                              /* start EDI */
                                          
                                          }
                                          
                                      }
                                  }
                                  gCcb.iRopVCheckLowEventCount = 0;
                               }

#if 0
                               if (gCcb.ulRopVelocity > CcbGetSp14()*1000)
                               {
                                  gCcb.iRopVCheckHighEventCount++;

                                  if (gCcb.iRopVCheckHighEventCount  * FM_CALC_PERIOD >= FM_ALARM_CHECK_PERIOD)
                                  {
                                      if (!gCcb.bit1AlarmROPHV) 
                                      {
                                          gCcb.bit1AlarmROPHV = TRUE;
          
                                          gCcb.ulFiredAlarmFlags |= ALARM_ROPHV;
                                          
                                          //CcbNotAlarmFire(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_RO_PRODUCT_FLOWING_VELOCITY,TRUE);

                                          /* stop EDI */
                                      }
                                      
                                  }
                                  gCcb.iRopVCheckHighRestoreCount = 0;
                               }
                               else
                               {
                                  if (gCcb.bit1AlarmROPLV)
                                  {
                                      gCcb.iRopVCheckHighRestoreCount++;

                                      if (gCcb.iRopVCheckHighRestoreCount  * FM_CALC_PERIOD >= FM_ALARM_CHECK_PERIOD)
                                      {
                                          gCcb.bit1AlarmROPHV = FALSE;

                                          if (gCcb.ulFiredAlarmFlags  & ALARM_ROPHV)
                                          {
                                              gCcb.ulFiredAlarmFlags &= ~ALARM_ROPHV;
                                              
                                              //CcbNotAlarmFire(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_RO_PRODUCT_FLOWING_VELOCITY,FALSE);

                                              /* start EDI */
                                          
                                          }
                                          
                                      }
                                  }
                                  gCcb.iRopVCheckHighEventCount = 0;
                               }

#endif                           
                           }
                       }                   
                   }

                   if (iTmp & (1 << APP_FM_FM4_NO))
                   {
                       if (INVALID_FM_VALUE == gCcb.ulLstRodFlow)
                       {
                           gCcb.ulLstRodFlow = gCcb.FlowMeter.aFmObjs[APP_FM_FM4_NO].Value.ulV;
                           gCcb.ulLstRodTick = getEventCount();
                       }

                       {
                           int iTmDelta = getEventCount() - gCcb.ulLstRodTick;
                           int iFmDelta;
                   
                           if ((iTmDelta >= (FM_CALC_PERIOD/PERIOD_EVENT_LENGTH))
                               && (gCcb.ulLstRodFlow != INVALID_FM_VALUE))
                           {
                               iFmDelta = gCcb.FlowMeter.aFmObjs[APP_FM_FM4_NO].Value.ulV - gCcb.ulLstRodFlow;

                               gCcb.ulRodVelocity = (CcbConvert2Fm4Data(iFmDelta) * TOMLPERHOUR / iTmDelta);

                               gCcb.ulLstRodFlow = gCcb.FlowMeter.aFmObjs[APP_FM_FM4_NO].Value.ulV;

                               gCcb.ulLstRodTick = getEventCount();

                               if (gCcb.ulRodVelocity < CcbGetSp16()*1000)
                               {
                                  gCcb.iRoDVCheckEventCount++;

                                  if (gCcb.iRoDVCheckEventCount * FM_CALC_PERIOD >= FM_ALARM_CHECK_PERIOD)
                                  {
                                      if (!gCcb.bit1AlarmRODV) 
                                      {
                                          gCcb.bit1AlarmRODV = TRUE;
                                          gCcb.ulFiredAlarmFlags |= ALARM_RODV;
                                          
                                          CcbNotAlarmFire(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_RO_WASTE_FLOWING_VELOCITY,TRUE);
                                      }
                                  }
                                  gCcb.iRoDVCheckRestoreCount = 0;
                               }
                               else
                               {
                                  if (gCcb.bit1AlarmRODV)
                                  {
                                      gCcb.iRoDVCheckRestoreCount++;

                                      if (gCcb.iRoDVCheckRestoreCount * FM_CALC_PERIOD >= FM_ALARM_CHECK_PERIOD)
                                      {
                                          gCcb.bit1AlarmRODV = FALSE;
                                          
                                          if (gCcb.ulFiredAlarmFlags  & ALARM_RODV)
                                          {
                                              gCcb.ulFiredAlarmFlags &= ~ALARM_RODV;
                                              
                                              CcbNotAlarmFire(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_RO_WASTE_FLOWING_VELOCITY,FALSE);
                                          }
                                          
                                      }
                                  }

                                  gCcb.iRoDVCheckEventCount = 0;
                               }
                               
                           }
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

    return 0;
}


int CanCcbAfDataClientRptMsg(MAIN_CANITF_MSG_STRU *pCanItfMsg)
{

    APP_PACKET_CLIENT_RPT_IND_STRU *pmg = (APP_PACKET_CLIENT_RPT_IND_STRU *)&pCanItfMsg->aucData[1 + RPC_POS_DAT0];

    int iSrcAdr = CAN_SRC_ADDRESS(pCanItfMsg->ulCanId);

    /* message validation  */
    switch(pmg->hdr.ucDevType)
    {
    case APP_DEV_TYPE_EXE_BOARD:
    case APP_DEV_TYPE_FLOW_METER:
        if (iSrcAdr != pmg->hdr.ucDevType
            && iSrcAdr != 0XFF)
        {
            return -1;
        }
        break;
    case APP_DEV_TYPE_RF_READER:
        if (iSrcAdr < APP_RF_READER_BEGIN_ADDRESS
            || iSrcAdr > APP_RF_READER_END_ADDRESS )
        {
            return -1;
        }
        break;        
    default:
        printf("unknow dev type %x\r\n",pmg->hdr.ucDevType);
        return -1;
    }

    switch(pmg->hdr.ucDevType)
    {
    case APP_DEV_TYPE_EXE_BOARD:
        return CanCcbAfDataClientRpt4ExeBoard(pCanItfMsg);
    case APP_DEV_TYPE_FLOW_METER:
        return CanCcbAfDataClientRpt4FlowMeter(pCanItfMsg);
    case APP_DEV_TYPE_RF_READER:
        return CanCcbAfDataClientRpt4RFReader(pCanItfMsg);
    default:
        break;
    }

    return 0;
}

int CanCcbAfDataHOCirReqMsg(MAIN_CANITF_MSG_STRU *pCanItfMsg)
{
    APP_PACKET_HO_STRU *pmg = (APP_PACKET_HO_STRU *)&pCanItfMsg->aucData[1 + RPC_POS_DAT0]; 

    APP_PACKET_HO_CIR_REQ_STRU *pCirReq = (APP_PACKET_HO_CIR_REQ_STRU *)pmg->aucData;

    uint8_t ucResult = APP_PACKET_HO_ERROR_CODE_UNSUPPORT;

    int     iSrcAdr  = CAN_SRC_ADDRESS(pCanItfMsg->ulCanId);

    uint8_t ucIndex  = (iSrcAdr - APP_HAND_SET_BEGIN_ADDRESS);

    VOS_LOG(VOS_LOG_WARNING,"Enter %s:%d:%d",__FUNCTION__,pCirReq->ucAction,ucIndex);    


    if (ucIndex >=  MAX_HANDLER_NUM)
    {
        ucResult = APP_PACKET_HO_ERROR_CODE_UNKNOW;
        goto end;
    }

    switch(pCirReq->ucAction)
    {
    case APP_PACKET_HO_ACTION_START:
        {
            if (DISP_WORK_STATE_RUN == gCcb.curWorkState.iMainWorkState4Pw
                && DISP_WORK_SUB_IDLE == gCcb.curWorkState.iSubWorkState4Pw)
            {
               if (!SearchWork(work_start_cir))
               {
                   CcbInnerWorkStartCir(CIR_TYPE_UP);
               }
               ucResult = APP_PACKET_HO_ERROR_CODE_SUCC;
            }
        }
        break;
    default:
        if (DISP_WORK_STATE_RUN == gCcb.curWorkState.iMainWorkState4Pw
            && DISP_WORK_SUB_RUN_CIR == gCcb.curWorkState.iSubWorkState4Pw)
        {
           if (!SearchWork(work_stop_cir))
           {
               CcbInnerWorkStopCir();
           }   
           ucResult = APP_PACKET_HO_ERROR_CODE_SUCC;
        }
        break;
    }

end:
    {
        /* send response */
        unsigned char buf[32];

        unsigned int ulCanId;

        int iPayLoad;

        APP_PACKET_HO_STRU *pHoRsp = (APP_PACKET_HO_STRU *)buf;
        APP_PACKET_HO_CIR_RSP_STRU *pCirRsp = (APP_PACKET_HO_CIR_RSP_STRU *)pHoRsp->aucData;

        pHoRsp->hdr = pmg->hdr;

        pHoRsp->hdr.ucLen     = sizeof(APP_PACKET_HO_CIR_RSP_STRU) + 1; // 1 for ops type
        pHoRsp->hdr.ucDevType = APP_DEV_TYPE_MAIN_CTRL;
        pHoRsp->hdr.ucMsgType |= 0x80;
        pHoRsp->ucOpsType     = pmg->ucOpsType;

        pCirRsp->ucAction     = pCirReq->ucAction;
        pCirRsp->ucResult     = ucResult;

        iPayLoad = APP_POROTOL_PACKET_HO_COMMON_LENGTH + sizeof(APP_PACKET_HO_CIR_RSP_STRU);

        CAN_BUILD_ADDRESS_IDENTIFIER(ulCanId,0x1,CAN_SRC_ADDRESS(pCanItfMsg->ulCanId));

        CcbSndAfCanCmd(pCanItfMsg->iCanChl,ulCanId,SAPP_CMD_DATA,buf,iPayLoad);

    }
    return 0;
}
int CanCcbAfDataHOCirRspMsg(MAIN_CANITF_MSG_STRU *pCanItfMsg)
{
    /* do nothing here */
    pCanItfMsg = pCanItfMsg;
    return 0;
}

void CanCcbSaveSpeedMsg(int iTrxIndex,void *pMsg,int iSpeed)
{
    //DrawSpeed(gCcb.aHandler[iIndex].iDevType,gCcb.aHandler[iIndex].iSpeed);
    switch(iTrxIndex)
    {
    case APP_TRX_CAN:
        {
            MAIN_CANITF_MSG_STRU *pCanItfMsg = (MAIN_CANITF_MSG_STRU *)pMsg;
            
            APP_PACKET_HO_STRU *pmg = (APP_PACKET_HO_STRU *)&pCanItfMsg->aucData[1 + RPC_POS_DAT0]; 
            
            APP_PACKET_HO_SPEED_REQ_STRU *pReq = (APP_PACKET_HO_SPEED_REQ_STRU *)pmg->aucData;
            
            int iSrcAdr  = CAN_SRC_ADDRESS(pCanItfMsg->ulCanId);
            int iIndex   = (iSrcAdr - APP_HAND_SET_BEGIN_ADDRESS);
            
            gCcb.aHandler[iIndex].CommHdr = pmg->hdr;
            gCcb.aHandler[iIndex].ulCanId = pCanItfMsg->ulCanId;
            gCcb.aHandler[iIndex].iCanIdx = pCanItfMsg->iCanChl;
            
            gCcb.aHandler[iIndex].iAction = pReq->ucAction;  
            gCcb.aHandler[iIndex].iSpeed = iSpeed;
            
            gCcb.aHandler[iIndex].iCurTrxIndex = iTrxIndex;

        }
        break;
    default:
        {
            ZBITF_MAIN_MSG_STRU *pZigbeeItfMsg = (ZBITF_MAIN_MSG_STRU *)pMsg;
            
            APP_PACKET_HO_STRU *pmg = (APP_PACKET_HO_STRU *)&pZigbeeItfMsg->aucData[APP_ZIGBEE_SUB_PROTOL_LENGTH]; 
            
            APP_PACKET_HO_SPEED_REQ_STRU *pReq = (APP_PACKET_HO_SPEED_REQ_STRU *)pmg->aucData;

            int iIndex = CanCcbZigbeeSearchHandset (pZigbeeItfMsg->usShortAdr);
            
            if (iIndex >= MAX_HANDLER_NUM)
            {
                return ;
            }

            gCcb.aHandler[iIndex].CommHdr = pmg->hdr;
            gCcb.aHandler[iIndex].iAction = pReq->ucAction;  
            gCcb.aHandler[iIndex].iSpeed = iSpeed;

            gCcb.aHandler[iIndex].iCurTrxIndex = iTrxIndex;
            
        }        
        break;
    }    
}

void CanCcbSndHoSpeedRspMsg(int iIndex,int iCode)
{
    /* send response */
    unsigned char buf[32];

    unsigned int ulCanId;
    int iPayLoad;

    APP_PACKET_HO_STRU *pHoRsp          = (APP_PACKET_HO_STRU *)buf;
    APP_PACKET_HO_SPEED_RSP_STRU  *pRsp = (APP_PACKET_HO_SPEED_RSP_STRU *)pHoRsp->aucData;

    pHoRsp->hdr           = gCcb.aHandler[iIndex].CommHdr;
    pHoRsp->hdr.ucLen     = sizeof(APP_PACKET_HO_QTW_RSP_STRU) + 1; // 1 for ops type
    pHoRsp->hdr.ucDevType = APP_DEV_TYPE_MAIN_CTRL;
    pHoRsp->hdr.ucMsgType |= 0x80;
    pHoRsp->ucOpsType     = APP_PACKET_HO_SPEED;

    pRsp->ucAction     = gCcb.aHandler[iIndex].iAction;
    pRsp->ucResult     = iCode;
    pRsp->ucSpeed      = gCcb.aHandler[iIndex].iSpeed;
    pRsp->ucType       = gCcb.aHandler[iIndex].iDevType;

    iPayLoad = APP_POROTOL_PACKET_HO_COMMON_LENGTH + sizeof(APP_PACKET_HO_SPEED_RSP_STRU);

    if (APP_TRX_CAN == gCcb.aHandler[iIndex].iCurTrxIndex)
    {
         CAN_BUILD_ADDRESS_IDENTIFIER(ulCanId,APP_DEV_TYPE_MAIN_CTRL,CAN_SRC_ADDRESS(gCcb.aHandler[iIndex].ulCanId));
         
         CcbSndAfCanCmd(gCcb.aHandler[iIndex].iCanIdx,ulCanId,SAPP_CMD_DATA,buf,iPayLoad);

         VOS_LOG(VOS_LOG_WARNING,"%s:%x",__FUNCTION__,ulCanId);
    }
    else
    {
        if (haveZigbee(&gCcb))
        {
            sAddr_t addr ;
            addr.addrMode            = afAddr16Bit;
            addr.addr.shortAddr      = gCcb.aHandler[iIndex].usShortAddr;
        
            CcbSndZigbeeCmd(&addr,SAPP_CMD_DATA,(unsigned char *)buf,APP_PROTOL_HEADER_LEN + pHoRsp->hdr.ucLen );
        }
    }


}

void DispSndHoSpeed(int iType,int iSpeed)
{
    /* send response */
    unsigned char buf[32];

    unsigned int ulCanId;
             int iPayLoad;

    APP_PACKET_HO_STRU *pHoRsp          = (APP_PACKET_HO_STRU *)buf;
    APP_PACKET_HO_SPEED_RSP_STRU  *pRsp = (APP_PACKET_HO_SPEED_RSP_STRU *)pHoRsp->aucData;

    pHoRsp->hdr.ucTransId = 0XFE;
    pHoRsp->hdr.ucLen     = sizeof(APP_PACKET_HO_SPEED_RSP_STRU) + 1; // 1 for ops type
    pHoRsp->hdr.ucDevType = APP_DEV_TYPE_MAIN_CTRL;
    pHoRsp->hdr.ucMsgType = APP_PACKET_HAND_OPERATION | 0x80;
    pHoRsp->ucOpsType     = APP_PACKET_HO_SPEED;

    pRsp->ucAction     = APP_PACKET_HO_ACTION_START;
    pRsp->ucResult     = ERROR_CODE_SUCC;
    pRsp->ucSpeed      = iSpeed;
    pRsp->ucType       = iType;

    iPayLoad = APP_POROTOL_PACKET_HO_COMMON_LENGTH + sizeof(APP_PACKET_HO_SPEED_RSP_STRU);

    CAN_BUILD_ADDRESS_IDENTIFIER(ulCanId,APP_DEV_TYPE_MAIN_CTRL,CAN_BROAD_CAST_ADDRESS);

    CcbSndAfCanCmd(0,ulCanId,SAPP_CMD_DATA,buf,iPayLoad);

    if (haveZigbee(&gCcb) && zbGetNodes())
    {
        CcbZigbeeModeDedicate(buf,iPayLoad);
    }

}

void DispSndHoQtwVolumn(int iType,float fValue)
{
    /* send response */
    unsigned char buf[32];

    unsigned int ulCanId;
             int iPayLoad;

    APP_PACKET_HO_STRU *pHoReq          = (APP_PACKET_HO_STRU *)buf;
    APP_PACKET_HO_QTW_REQ_STRU  *pReq   = (APP_PACKET_HO_QTW_REQ_STRU *)pHoReq->aucData;

    pHoReq->hdr.ucTransId = 0XFE;
    pHoReq->hdr.ucLen     = sizeof(APP_PACKET_HO_QTW_REQ_STRU) + 1; // 1 for ops type
    pHoReq->hdr.ucDevType = APP_DEV_TYPE_MAIN_CTRL;
    pHoReq->hdr.ucMsgType = APP_PACKET_HAND_OPERATION | 0x80;
    pHoReq->ucOpsType     = APP_PACKET_HO_QTW_VOLUMN;

    pReq->ucAction     = iType;
    pReq->ulVolumn     = fValue;

    iPayLoad = APP_POROTOL_PACKET_HO_COMMON_LENGTH + sizeof(APP_PACKET_HO_QTW_REQ_STRU);

    CAN_BUILD_ADDRESS_IDENTIFIER(ulCanId,APP_DEV_TYPE_MAIN_CTRL,CAN_BROAD_CAST_ADDRESS);

    CcbSndAfCanCmd(0,ulCanId,SAPP_CMD_DATA,buf,iPayLoad);

    if (haveZigbee(&gCcb) && zbGetNodes())
    {
        CcbZigbeeModeDedicate(buf,iPayLoad);
    }

}

void DispSndHoAlarm(int iIndex,int iCode)
{

    /* send response */
    unsigned char buf[32];

    unsigned int ulCanId;
    int          iPayLoad;

    APP_PACKET_HO_STRU *pHoState = (APP_PACKET_HO_STRU *)buf;

    APP_PACKET_HO_ALARM_STATE_NOT_STRU *pLoad = (APP_PACKET_HO_ALARM_STATE_NOT_STRU *)pHoState->aucData;

    pHoState->hdr.ucLen      = 1 + sizeof(APP_PACKET_HO_ALARM_STATE_NOT_STRU) ; // 1 for ops type
    pHoState->hdr.ucDevType  = APP_DEV_TYPE_MAIN_CTRL;
    pHoState->hdr.ucMsgType  = APP_PACKET_HAND_OPERATION;
    pHoState->ucOpsType      = APP_PACKET_HO_ALARM_STATE_NOT;

    pLoad->ucMask            = iCode;

    iPayLoad = APP_POROTOL_PACKET_HO_COMMON_LENGTH +  sizeof(APP_PACKET_HO_ALARM_STATE_NOT_STRU) ;

    if (iIndex == APP_PROTOL_CANID_BROADCAST)
    {
        /* broadcast */
        CAN_BUILD_ADDRESS_IDENTIFIER(ulCanId,APP_DEV_TYPE_MAIN_CTRL,CAN_BROAD_CAST_ADDRESS);
    }
    else
    {
        CAN_BUILD_ADDRESS_IDENTIFIER(ulCanId,APP_DEV_TYPE_MAIN_CTRL,(iIndex + APP_HAND_SET_BEGIN_ADDRESS));
    
    }
    CcbSndAfCanCmd(0,ulCanId,SAPP_CMD_DATA,buf,iPayLoad);

    if (haveZigbee(&gCcb) && zbGetNodes())
    {
        CcbZigbeeModeDedicate(buf,iPayLoad);
    }

}

void DispSndHoPpbAndTankLevel(int iIndex,int iMask,int iLevel,float fPpb)
{
    /* send response */
    unsigned char buf[32];

    unsigned int ulCanId;
    int          iPayLoad;

    APP_PACKET_HO_STRU *pHoState     = (APP_PACKET_HO_STRU *)buf;
    APP_PACKET_HO_QL_UPD_STRU *pLoad  = (APP_PACKET_HO_QL_UPD_STRU *)pHoState->aucData;

    pHoState->hdr.ucLen      = 1 + sizeof(APP_PACKET_HO_QL_UPD_STRU) ; // 1 for ops type
    pHoState->hdr.ucDevType  = APP_DEV_TYPE_MAIN_CTRL;
    pHoState->hdr.ucMsgType  = APP_PACKET_HAND_OPERATION;
    pHoState->ucOpsType      = APP_PACKET_HO_QL_UPDATE;

    pLoad->ucMask            = iMask;
    pLoad->ucLevel           = iLevel;
    pLoad->fWaterQppb        = fPpb;

    iPayLoad = APP_POROTOL_PACKET_HO_COMMON_LENGTH +  sizeof(APP_PACKET_HO_QL_UPD_STRU) ;

    if (iIndex == APP_PROTOL_CANID_BROADCAST)
    {
        /* broadcast */
        CAN_BUILD_ADDRESS_IDENTIFIER(ulCanId,APP_DEV_TYPE_MAIN_CTRL,CAN_BROAD_CAST_ADDRESS);
    }
    else
    {
        CAN_BUILD_ADDRESS_IDENTIFIER(ulCanId,APP_DEV_TYPE_MAIN_CTRL,(iIndex + APP_HAND_SET_BEGIN_ADDRESS));
    }
    CcbSndAfCanCmd(0,ulCanId,SAPP_CMD_DATA,buf,iPayLoad);

    if (haveZigbee(&gCcb) && zbGetNodes())
    {
        CcbZigbeeModeDedicate(buf,iPayLoad);
    }
}

void DispSndHoSystemTest(int iIndex,int iAction,int iOpreatType)
{
    /* send response */
    unsigned char buf[32];

    unsigned int ulCanId;
    int          iPayLoad;

    APP_PACKET_HO_STRU *pHoState     = (APP_PACKET_HO_STRU *)buf;
    APP_PACKET_HO_SYSTEMTEST_REQ_STRU *pLoad = (APP_PACKET_HO_SYSTEMTEST_REQ_STRU *)pHoState->aucData;

    pHoState->hdr.ucLen      = 1 + sizeof(APP_PACKET_HO_SYSTEMTEST_REQ_STRU) ; // 1 for ops type
    pHoState->hdr.ucDevType  = APP_DEV_TYPE_MAIN_CTRL;
    pHoState->hdr.ucMsgType  = APP_PACKET_HAND_OPERATION;
    pHoState->ucOpsType      = APP_PACKET_HO_SYSTEM_TEST;

    pLoad->ucAction          = iAction;
    pLoad->ucType            = iOpreatType;

    iPayLoad = APP_POROTOL_PACKET_HO_COMMON_LENGTH +  sizeof(APP_PACKET_HO_SYSTEMTEST_REQ_STRU) ;

    VOS_LOG(VOS_LOG_DEBUG,"DispSndHoSystemTest %d&%d\r\n",iAction,iOpreatType);

    if (iIndex == APP_PROTOL_CANID_BROADCAST)
    {
        /* broadcast */
        CAN_BUILD_ADDRESS_IDENTIFIER(ulCanId,APP_DEV_TYPE_MAIN_CTRL,CAN_BROAD_CAST_ADDRESS);
    }
    else
    {
        CAN_BUILD_ADDRESS_IDENTIFIER(ulCanId,APP_DEV_TYPE_MAIN_CTRL,(iIndex + APP_HAND_SET_BEGIN_ADDRESS));
    }

    if (APP_PACKET_HO_SYS_TEST_TYPE_DEPRESSURE == iOpreatType)
    {
        CcbNotDecPressure(gCcb.aHandler[iIndex].iDevType ,iAction);
    }
    
    CcbSndAfCanCmd(0,ulCanId,SAPP_CMD_DATA,buf,iPayLoad);

    if (haveZigbee(&gCcb) && zbGetNodes())
    {
       if (iIndex < MAX_HANDLER_NUM)
       {
          VOS_LOG(VOS_LOG_DEBUG,"CcbZigbeeModeSingle send succeed\r\n");
          CcbZigbeeModeSingle(iIndex,buf,iPayLoad);
       }
    }
}

void DispSndHoEco(int iMask)
{
    unsigned char buf[32];

    unsigned int ulCanId;
    int          iPayLoad;

    APP_PACKET_HO_STRU *pHoMsg = (APP_PACKET_HO_STRU *)buf;

    APP_PACKET_RPT_ECO_STRU *pEco = (APP_PACKET_RPT_ECO_STRU *)pHoMsg->aucData;

    pHoMsg->hdr.ucLen      = 1 ; // 1 for ops type
    pHoMsg->hdr.ucDevType  = APP_DEV_TYPE_MAIN_CTRL;
    pHoMsg->hdr.ucMsgType  = APP_PACKET_HAND_OPERATION;
    pHoMsg->ucOpsType      = APP_PACKET_HO_WQ_NOTIFY;

    if (iMask & (1 << APP_EXE_I4_NO))
    {
        pEco->ucId         = APP_EXE_I4_NO;
        //ex
        switch(gCcb.ulMachineType)
        {
        case MACHINE_L_Genie:
        case MACHINE_L_EDI_LOOP:
        case MACHINE_Genie:
        case MACHINE_EDI:
        {
            if(haveHPCir(&gCcb))
            {
                pEco->ev = gCcb.ExeBrd.aEcoObjs[APP_EXE_I4_NO].Value.eV;
            }
            else
            {
                pEco->ev = gCcb.ExeBrd.aEcoObjs[APP_EXE_I3_NO].Value.eV;
            }
            break;
        }
        case MACHINE_L_UP:
        case MACHINE_L_RO_LOOP:
        case MACHINE_UP:
        case MACHINE_RO:
        {
            if(haveHPCir(&gCcb))
            {
                pEco->ev = gCcb.ExeBrd.aEcoObjs[APP_EXE_I3_NO].Value.eV;
            }
            else
            {
                pEco->ucId         = APP_EXE_I2_NO;
                pEco->ev = gCcb.ExeBrd.aEcoObjs[APP_EXE_I2_NO].Value.eV;
            }
            break;
        }
        case MACHINE_ADAPT:
        {
            pEco->ucId         = APP_EXE_I2_NO;
            pEco->ev = gCcb.ExeBrd.aEcoObjs[APP_EXE_I2_NO].Value.eV;
            break;
        }
        default:
            pEco->ev = gCcb.ExeBrd.aEcoObjs[APP_EXE_I4_NO].Value.eV;
            break;
        }
        //end

        //pEco->ev           = gCcb.ExeBrd.aEcoObjs[APP_EXE_I4_NO].Value.eV;
        pHoMsg->hdr.ucLen += sizeof(APP_PACKET_RPT_ECO_STRU);
        pEco++;
    }

    if (iMask & (1 << APP_EXE_I5_NO))
    {
        pEco->ucId         = APP_EXE_I5_NO;
        pEco->ev           = gCcb.ExeBrd.aEcoObjs[APP_EXE_I5_NO].Value.eV;
        pHoMsg->hdr.ucLen += sizeof(APP_PACKET_RPT_ECO_STRU);
        pEco++;
    }

    iPayLoad = APP_PROTOL_HEADER_LEN +  pHoMsg->hdr.ucLen ;

    CAN_BUILD_ADDRESS_IDENTIFIER(ulCanId,APP_DEV_TYPE_MAIN_CTRL,CAN_BROAD_CAST_ADDRESS);
    CcbSndAfCanCmd(0,ulCanId,SAPP_CMD_DATA,buf,iPayLoad);

    if (haveZigbee(&gCcb) && zbGetNodes())
    {
        CcbZigbeeModeDedicate(buf,iPayLoad);
    }
#if 0
    if (haveZigbee(&gCcb) && zbGetNodes())
    {
        /* send response */
        unsigned char buf[32];
    
        //unsigned int ulCanId;
        int          iPayLoad;
    
        APP_PACKET_HO_STRU *pHoMsg = (APP_PACKET_HO_STRU *)buf;
    
        APP_PACKET_RPT_ECO_STRU *pEco = (APP_PACKET_RPT_ECO_STRU *)pHoMsg->aucData;
    
        pHoMsg->hdr.ucLen      = 1 ; // 1 for ops type
        pHoMsg->hdr.ucDevType  = APP_DEV_TYPE_MAIN_CTRL;
        pHoMsg->hdr.ucMsgType  = APP_PACKET_HAND_OPERATION;
        pHoMsg->ucOpsType      = APP_PACKET_HO_WQ_NOTIFY;

        if (iMask & (1 << APP_EXE_I4_NO))
        {
            pEco->ucId         = APP_EXE_I4_NO;
            pEco->ev           = gCcb.ExeBrd.aEcoObjs[APP_EXE_I4_NO].Value.eV;
            pHoMsg->hdr.ucLen += sizeof(APP_PACKET_RPT_ECO_STRU);
            pEco++;
        }
        
        if (iMask & (1 << APP_EXE_I5_NO))
        {
            pEco->ucId         = APP_EXE_I5_NO;
            pEco->ev           = gCcb.ExeBrd.aEcoObjs[APP_EXE_I5_NO].Value.eV;
            pHoMsg->hdr.ucLen += sizeof(APP_PACKET_RPT_ECO_STRU);
            pEco++;
        }
    
        iPayLoad = APP_PROTOL_HEADER_LEN +  pHoMsg->hdr.ucLen ;    
        
        CcbZigbeeModeDedicate(buf,iPayLoad);
    }
#endif
}


int CanCcbAfDataHOSpeedReqMsg(MAIN_CANITF_MSG_STRU *pCanItfMsg)
{
    APP_PACKET_HO_STRU *pmg = (APP_PACKET_HO_STRU *)&pCanItfMsg->aucData[1 + RPC_POS_DAT0]; 

    APP_PACKET_HO_SPEED_REQ_STRU *pSpeedReq = (APP_PACKET_HO_SPEED_REQ_STRU *)pmg->aucData;

    uint8_t ucResult = APP_PACKET_HO_ERROR_CODE_UNSUPPORT;

    int     iSrcAdr  = CAN_SRC_ADDRESS(pCanItfMsg->ulCanId);

    uint8_t ucIndex  = (iSrcAdr - APP_HAND_SET_BEGIN_ADDRESS);

    int     iSpeed   = PUMP_SPEED_00;
        //PUMP_SPEED_NUM;

    int     iActive  = 0;

    int     iType = APP_DEV_HS_SUB_NUM;
    

    VOS_LOG(VOS_LOG_WARNING,"Enter %s:%d:%d",__FUNCTION__,pSpeedReq->ucAction,ucIndex);    

    if (ucIndex >=  MAX_HANDLER_NUM)
    {
        ucResult = APP_PACKET_HO_ERROR_CODE_UNKNOW;
        goto end;
    }

    iType = gCcb.aHandler[ucIndex].iDevType;

    DispGetRPumpSwitchState(APP_EXE_C2_NO - APP_EXE_C1_NO,&iActive);

    iSpeed = DispConvertRPumpSpeed2Scale(gCcb.ExeBrd.ausHoldRegister[APP_EXE_HOLD_REG_RPUMP_OFFSET + APP_EXE_C2_NO - APP_EXE_C1_NO]);

    switch(pSpeedReq->ucAction)
    {
    case APP_PACKET_HO_ACTION_START:
        {
            if (iSpeed >= PUMP_SPEED_10)
            {
                ucResult = APP_PACKET_HO_ERROR_CODE_SUCC;
                goto end;
            } 

            iSpeed += 1; 

            CanCcbSaveSpeedMsg(APP_TRX_CAN,pCanItfMsg,iSpeed);

            if (!SearchWork(work_start_speed_regulation))
            {
                CcbInnerWorkStartSpeedRegulation(ucIndex);
            }
            
            return 0;
            
        }
        break;
    default:
        {
            if (iSpeed <= PUMP_SPEED_00)
            {
                ucResult = APP_PACKET_HO_ERROR_CODE_SUCC;
                goto end;
            } 

            iSpeed -= 1; 
            
            CanCcbSaveSpeedMsg(APP_TRX_CAN,pCanItfMsg,iSpeed);
            
            if (!SearchWork(work_start_speed_regulation))
            {
                CcbInnerWorkStartSpeedRegulation(ucIndex);
            }

            return 0;
        }
        break;
    }

end:
    {
        /* send response */
        unsigned char buf[32];

        unsigned int ulCanId;

        int iPayLoad;

        APP_PACKET_HO_STRU           *pHoRsp  = (APP_PACKET_HO_STRU *)buf;
        APP_PACKET_HO_SPEED_RSP_STRU *pSpeedRsp = (APP_PACKET_HO_SPEED_RSP_STRU *)pHoRsp->aucData;

        pHoRsp->hdr = pmg->hdr;

        pHoRsp->hdr.ucLen     = sizeof(APP_PACKET_HO_SPEED_RSP_STRU) + 1; // 1 for ops type
        pHoRsp->hdr.ucDevType = APP_DEV_TYPE_MAIN_CTRL;
        pHoRsp->hdr.ucMsgType |= 0x80;
        pHoRsp->ucOpsType     = pmg->ucOpsType;

        pSpeedRsp->ucAction     = pSpeedReq->ucAction;
        pSpeedRsp->ucResult     = ucResult;
        pSpeedRsp->ucSpeed      = iSpeed;
        pSpeedRsp->ucType       = iType;

        iPayLoad = APP_POROTOL_PACKET_HO_COMMON_LENGTH + sizeof(APP_PACKET_HO_SPEED_RSP_STRU);

        CAN_BUILD_ADDRESS_IDENTIFIER(ulCanId,0x1,CAN_SRC_ADDRESS(pCanItfMsg->ulCanId));

        CcbSndAfCanCmd(pCanItfMsg->iCanChl,ulCanId,SAPP_CMD_DATA,buf,iPayLoad);

    }
    return 0;
}

int CanCcbAfDataHODecPressureReqMsg(MAIN_CANITF_MSG_STRU *pCanItfMsg)
{
    APP_PACKET_HO_STRU *pmg = (APP_PACKET_HO_STRU *)&pCanItfMsg->aucData[1 + RPC_POS_DAT0]; 

    APP_PACKET_HO_SYSTEMTEST_REQ_STRU *pDecPreReq = (APP_PACKET_HO_SYSTEMTEST_REQ_STRU *)pmg->aucData;

    uint8_t ucResult = APP_PACKET_HO_ERROR_CODE_UNSUPPORT;

    int     iSrcAdr  = CAN_SRC_ADDRESS(pCanItfMsg->ulCanId);

    uint8_t ucIndex  = (iSrcAdr - APP_HAND_SET_BEGIN_ADDRESS);

    int     iActive  = 0;

    int     iType = APP_DEV_HS_SUB_NUM;
    

    VOS_LOG(VOS_LOG_WARNING,"Enter %s:%d:%d",__FUNCTION__,pDecPreReq->ucAction,ucIndex);    

    if (ucIndex >=  MAX_HANDLER_NUM)
    {
         return -1;
    }

    iType = gCcb.aHandler[ucIndex].iDevType;

    if (DISP_WORK_STATE_IDLE == gCcb.curWorkState.iMainWorkState4Pw)
    {
        unsigned char buf[32];
    
        unsigned int ulCanId;
        int          iPayLoad;
    
        APP_PACKET_HO_STRU *pHoState             = (APP_PACKET_HO_STRU *)buf;
        APP_PACKET_HO_SYSTEMTEST_RSP_STRU *pLoad = (APP_PACKET_HO_SYSTEMTEST_RSP_STRU *)pHoState->aucData;

        CcbNotDecPressure(iType,pDecPreReq->ucAction);
    
        pHoState->hdr.ucLen      = 1 + sizeof(APP_PACKET_HO_SYSTEMTEST_RSP_STRU) ; // 1 for ops type
        pHoState->hdr.ucDevType  = APP_DEV_TYPE_MAIN_CTRL;
        pHoState->hdr.ucMsgType  = APP_PACKET_HAND_OPERATION;
        pHoState->ucOpsType      = pmg->ucOpsType;
    
        pLoad->ucAction          = pDecPreReq->ucAction;
        pLoad->ucType            = pDecPreReq->ucType;
        pLoad->ucResult          = ucResult;
    
        iPayLoad = APP_POROTOL_PACKET_HO_COMMON_LENGTH +  sizeof(APP_PACKET_HO_SYSTEMTEST_RSP_STRU) ;
    
        CAN_BUILD_ADDRESS_IDENTIFIER(ulCanId,0x1,CAN_SRC_ADDRESS(pCanItfMsg->ulCanId));
        
        CcbSndAfCanCmd(pCanItfMsg->iCanChl,ulCanId,SAPP_CMD_DATA,buf,iPayLoad);
    }
}

int CanCcbAfDataHOSpeedRspMsg(MAIN_CANITF_MSG_STRU *pCanItfMsg)
{
    /* do nothing here */
    pCanItfMsg = pCanItfMsg;
    return 0;
}



void CanCcbSndHOState(int iIndex , int iState)
{

    /* send response */
    unsigned char buf[32];

    unsigned int ulCanId;
    int          iPayLoad;

    APP_PACKET_HO_STRU *pHoState = (APP_PACKET_HO_STRU *)buf;

    APP_PACKET_HO_STATE_STRU *pLoad = (APP_PACKET_HO_STATE_STRU *)pHoState->aucData;

    VOS_LOG(VOS_LOG_WARNING,"Enter %s:%d:%d",__FUNCTION__,iIndex,iState);    


    pHoState->hdr.ucLen      = 1 + sizeof(APP_PACKET_HO_STATE_STRU) ; // 1 for ops type
    pHoState->hdr.ucDevType  = APP_DEV_TYPE_MAIN_CTRL;
    pHoState->hdr.ucMsgType  = APP_PACKET_HAND_OPERATION;
    pHoState->ucOpsType      = APP_PACKET_HO_STA;

    pLoad->ucState           = iState;
    pLoad->ucResult          = APP_PACKET_HO_ERROR_CODE_SUCC;

    pLoad->ucAlarmState      = getAlarmState();  //ex
    
    switch(iState)
    {
    case APP_PACKET_HS_STATE_QTW:
        pLoad->ucAddData    = CcbGetTwFlag();
        break;
    case APP_PACKET_HS_STATE_CIR:
        pLoad->ucAddData    = gCcb.iCirType;
        break;
    default :
        pLoad->ucAddData    = 0;
        break;
    }
    
    if (gCcb.bit1B2Empty)
    {
        pLoad->ucResult      =  APP_PACKET_HO_ERROR_CODE_TANK_EMPTY;
    }

    iPayLoad = APP_POROTOL_PACKET_HO_COMMON_LENGTH +  sizeof(APP_PACKET_HO_STATE_STRU) ;

    if (iIndex == APP_PROTOL_CANID_BROADCAST)
    {
        /* broadcast */
        CAN_BUILD_ADDRESS_IDENTIFIER(ulCanId,APP_DEV_TYPE_MAIN_CTRL,CAN_BROAD_CAST_ADDRESS);
    }
    else
    {
        CAN_BUILD_ADDRESS_IDENTIFIER(ulCanId,APP_DEV_TYPE_MAIN_CTRL,(iIndex + APP_HAND_SET_BEGIN_ADDRESS));
    
    }
    CcbSndAfCanCmd(0,ulCanId,SAPP_CMD_DATA,buf,iPayLoad);

    if (haveZigbee(&gCcb) && zbGetNodes())
    {
        CcbZigbeeModeDedicate(buf,iPayLoad);
    }

}


void CanCcbSaveQtwMsg(int iTrxIndex, void *pMsg)
{
   switch(iTrxIndex)
    {
    case APP_TRX_CAN:
        {
            MAIN_CANITF_MSG_STRU *pCanItfMsg = (MAIN_CANITF_MSG_STRU *)pMsg;
            
            APP_PACKET_HO_STRU *pmg = (APP_PACKET_HO_STRU *)&pCanItfMsg->aucData[1 + RPC_POS_DAT0]; 
            
            APP_PACKET_HO_QTW_REQ_STRU *pQtwReq = (APP_PACKET_HO_QTW_REQ_STRU *)pmg->aucData;
            
            int iSrcAdr  = CAN_SRC_ADDRESS(pCanItfMsg->ulCanId);
            int iIndex   = (iSrcAdr - APP_HAND_SET_BEGIN_ADDRESS);
            
            gCcb.aHandler[iIndex].CommHdr = pmg->hdr;
            gCcb.aHandler[iIndex].ulCanId = pCanItfMsg->ulCanId;
            gCcb.aHandler[iIndex].iCanIdx = pCanItfMsg->iCanChl;
            if(INVALID_FM_VALUE != pQtwReq->ulVolumn)
            {
                gCcb.QtwMeas.ulTotalFm        = pQtwReq->ulVolumn - gCaliParam.pc[DISP_PC_COFF_S1].fc;
            }
            else
            {
                gCcb.QtwMeas.ulTotalFm        = pQtwReq->ulVolumn;
            }

            gCcb.aHandler[iIndex].iCurTrxIndex = iTrxIndex;
        }
        break;
    default:
        {
            ZBITF_MAIN_MSG_STRU *pZigbeeItfMsg = (ZBITF_MAIN_MSG_STRU *)pMsg;
            
            APP_PACKET_HO_STRU *pmg = (APP_PACKET_HO_STRU *)&pZigbeeItfMsg->aucData[APP_ZIGBEE_SUB_PROTOL_LENGTH]; 
            
            APP_PACKET_HO_QTW_REQ_STRU *pQtwReq = (APP_PACKET_HO_QTW_REQ_STRU *)pmg->aucData;

            int iIndex = CanCcbZigbeeSearchHandset (pZigbeeItfMsg->usShortAdr);
            
            if (iIndex >= MAX_HANDLER_NUM)
            {
                return;
            }
            
            gCcb.aHandler[iIndex].CommHdr = pmg->hdr;

            if(INVALID_FM_VALUE != pQtwReq->ulVolumn)
            {
                gCcb.QtwMeas.ulTotalFm        = pQtwReq->ulVolumn - gCaliParam.pc[DISP_PC_COFF_S1].fc;
            }
            else
            {
                gCcb.QtwMeas.ulTotalFm        = pQtwReq->ulVolumn;
            }

            gCcb.aHandler[iIndex].iCurTrxIndex = iTrxIndex;
            
        }        
        break;
    }
    
}

void CanccbFillQtwRsp(APP_PACKET_HO_QTW_RSP_STRU *pQtwRsp,int iAct,int iCode)
{
    pQtwRsp->ucAction     = iAct;
    pQtwRsp->ucResult     = iCode;
    pQtwRsp->ucUnit       = gCcb.MiscParam.iUint4Conductivity;
    pQtwRsp->usPulseNum   = gCcb.FMParam.aulCfg[0];
    pQtwRsp->ulVolumn     = gCcb.QtwMeas.ulTotalFm;
    
    VOS_LOG(VOS_LOG_WARNING,"%s:%d",__FUNCTION__,gCcb.QtwMeas.ulTotalFm);
}

void CanCcbSndQtwRspMsg(int iIndex,int iCode)
{
    /* send response */
    unsigned char buf[32];

    unsigned int ulCanId;
    int iPayLoad;

    APP_PACKET_HO_STRU *pHoRsp          = (APP_PACKET_HO_STRU *)buf;
    APP_PACKET_HO_QTW_RSP_STRU *pQtwRsp = (APP_PACKET_HO_QTW_RSP_STRU *)pHoRsp->aucData;

    pHoRsp->hdr           = gCcb.aHandler[iIndex].CommHdr;
    pHoRsp->hdr.ucLen     = sizeof(APP_PACKET_HO_QTW_RSP_STRU) + 1; // 1 for ops type
    pHoRsp->hdr.ucDevType = APP_DEV_TYPE_MAIN_CTRL;
    pHoRsp->hdr.ucMsgType |= 0x80;
    pHoRsp->ucOpsType     = APP_PACKET_HO_QTW;

    CanccbFillQtwRsp(pQtwRsp,APP_PACKET_HO_ACTION_START,iCode);

    pQtwRsp->ucType = gCcb.aHandler[iIndex].iDevType;

    if (APP_TRX_CAN == gCcb.aHandler[iIndex].iCurTrxIndex)
    {
        iPayLoad = APP_POROTOL_PACKET_HO_COMMON_LENGTH + sizeof(APP_PACKET_HO_QTW_RSP_STRU);
        
        CAN_BUILD_ADDRESS_IDENTIFIER(ulCanId,APP_DEV_TYPE_MAIN_CTRL,CAN_SRC_ADDRESS(gCcb.aHandler[iIndex].ulCanId));
        
        CcbSndAfCanCmd(gCcb.aHandler[iIndex].iCanIdx,ulCanId,SAPP_CMD_DATA,buf,iPayLoad);

        VOS_LOG(VOS_LOG_WARNING,"%s:%x",__FUNCTION__,ulCanId);
    }
    else
    {
        if (haveZigbee(&gCcb))
        {
            sAddr_t addr ;
            addr.addrMode            = afAddr16Bit;
            addr.addr.shortAddr      = gCcb.aHandler[iIndex].usShortAddr;
    
            CcbSndZigbeeCmd(&addr,SAPP_CMD_DATA,(unsigned char *)buf,APP_PROTOL_HEADER_LEN + pHoRsp->hdr.ucLen );
        }
    }    


}

int CanCcbAfDataHOQtwReqMsg(MAIN_CANITF_MSG_STRU *pCanItfMsg)
{

    APP_PACKET_HO_STRU *pmg = (APP_PACKET_HO_STRU *)&pCanItfMsg->aucData[1 + RPC_POS_DAT0]; 

    APP_PACKET_HO_QTW_REQ_STRU *pQtwReq = (APP_PACKET_HO_QTW_REQ_STRU *)pmg->aucData;

    uint8_t ucResult = APP_PACKET_HO_ERROR_CODE_SUCC;

    int     iSrcAdr  = CAN_SRC_ADDRESS(pCanItfMsg->ulCanId);
    uint8_t ucIndex  = (iSrcAdr - APP_HAND_SET_BEGIN_ADDRESS);
    int     iType = APP_DEV_HS_SUB_NUM;

    VOS_LOG(VOS_LOG_WARNING,"Enter %s:%d:%d:%d",__FUNCTION__,pQtwReq->ucAction,iSrcAdr,pQtwReq->ulVolumn);    

    if (ucIndex >=  MAX_HANDLER_NUM)
    {
        ucResult = APP_PACKET_HO_ERROR_CODE_UNKNOW;
        goto end;
    }
    
    iType = gCcb.aHandler[ucIndex].iDevType;

    switch(pQtwReq->ucAction)
    {
    case APP_PACKET_HO_ACTION_START:
        {
            /* late implement */
            if (DISP_WORK_STATE_RUN != gCcb.curWorkState.iMainWorkState4Pw)
            {
                ucResult = APP_PACKET_HO_ERROR_CODE_UNREADY; // failed
            }
            else
            {
               /* setup work */
               if (CcbGetTwFlag())
               {
                    if (!gCcb.aHandler[ucIndex].bit1Qtw)
                    {
                        ucResult = APP_PACKET_HO_ERROR_CODE_UNSUPPORT; // current only support one tw
                    }
               }
               else if(CcbGetTwPendingFlag())
               {
                   if(!(CcbGetTwPendingFlag() & (1 << ucIndex)))
                   {
                       ucResult = APP_PACKET_HO_ERROR_CODE_UNSUPPORT; // current only support one tw
                   }
               }
               else
               {
                   CanCcbSaveQtwMsg(APP_TRX_CAN,pCanItfMsg);

                   /* remove cir work if any */
                   CcbRmvWork(work_start_cir);
                    
                   /* construct work */
                   CcbInnerWorkStartQtw(ucIndex);

                   return 0;
               }
            }
        }
        break;
    case APP_PACKET_HO_ACTION_STOP:
        {
            /* setup work */
            if (!gCcb.aHandler[ucIndex].bit1Qtw)
            {
                ucResult = APP_PACKET_HO_ERROR_CODE_SUCC; //  

                /* 2018/01/24 add for adpater because of its prolonged tw procedure */
                {
                    DISPHANDLE hdl ;
                    
                    hdl = SearchWork(work_start_qtw);
                    
                    if (hdl)
                    {
                        CcbCancelWork(hdl);
                    }
                }
                
            }
            else
            {
                CcbInnerWorkStopQtw(ucIndex);
            }
        }
        break;
    default:
        ucResult = APP_PACKET_HO_ERROR_CODE_UNKNOW; // failed
        break;
    }

end:
    {
        /* send response */
        unsigned char buf[32];

        unsigned int ulCanId;
        int iPayLoad;

        APP_PACKET_HO_STRU *pHoRsp = (APP_PACKET_HO_STRU *)buf;
        APP_PACKET_HO_QTW_RSP_STRU *pQtwRsp = (APP_PACKET_HO_QTW_RSP_STRU *)pHoRsp->aucData;

        pHoRsp->hdr           = pmg->hdr;
        pHoRsp->hdr.ucLen     = sizeof(APP_PACKET_HO_QTW_RSP_STRU) + 1; // 1 for ops type
        pHoRsp->hdr.ucDevType = APP_DEV_TYPE_MAIN_CTRL;
        pHoRsp->hdr.ucMsgType |= 0x80;
        pHoRsp->ucOpsType     = pmg->ucOpsType;

        CanccbFillQtwRsp(pQtwRsp,pQtwReq->ucAction,ucResult);
        pQtwRsp->ucType       = iType;
        
        iPayLoad = APP_POROTOL_PACKET_HO_COMMON_LENGTH + sizeof(APP_PACKET_HO_QTW_RSP_STRU);

        CAN_BUILD_ADDRESS_IDENTIFIER(ulCanId,0x1,CAN_SRC_ADDRESS(pCanItfMsg->ulCanId));

        CcbSndAfCanCmd(pCanItfMsg->iCanChl,ulCanId,SAPP_CMD_DATA,buf,iPayLoad);

    }

    return 0;
}


int CanCcbAfDataHOQtwRspMsg(MAIN_CANITF_MSG_STRU *pCanItfMsg)
{

    pCanItfMsg = pCanItfMsg;

    /* do nothing here */

    return 0;
}

int CanCcbAfDataHODecPressureRspMsg(MAIN_CANITF_MSG_STRU *pCanItfMsg)
{

    pCanItfMsg = pCanItfMsg;

    /* do nothing here */

    return 0;
}


void CanCcbIapProc(int iTrxIndex,void *msg)
{
   /* Notify Ui */
   IAP_NOTIFY_STRU *pNotify = (IAP_NOTIFY_STRU *)gaucIapBuffer;

   switch(iTrxIndex)
   {
   case APP_TRX_CAN:
       {
           MAIN_CANITF_MSG_STRU *pCanItfMsg = (MAIN_CANITF_MSG_STRU *)msg;
           pNotify->ulCanId = pCanItfMsg->ulCanId;
           pNotify->iCanChl = pCanItfMsg->iCanChl;
           pNotify->iMsgLen = pCanItfMsg->iMsgLen;

        
           memcpy(pNotify->data,pCanItfMsg->aucData,pNotify->iMsgLen);    
       }
       break;
   default:
       {
           ZBITF_MAIN_MSG_STRU *pZigbeeItfMsg = (ZBITF_MAIN_MSG_STRU *)msg;

           pNotify->iMsgLen = pZigbeeItfMsg->usMsgLen;
        
           memcpy(pNotify->data,pZigbeeItfMsg->aucData,pNotify->iMsgLen);            
       }
       break;
   }

   pNotify->iTrxIndex = iTrxIndex;

   DispIapIndEntry(pNotify);

}


int CanCcbAfDataHandleOpsMsg(MAIN_CANITF_MSG_STRU *pCanItfMsg)
{
    APP_PACKET_HO_STRU *pmg = (APP_PACKET_HO_STRU *)&pCanItfMsg->aucData[1 + RPC_POS_DAT0]; 

    switch(pmg->ucOpsType)
    {
    case APP_PACKET_HO_CIR:
        if (!(pmg->hdr.ucMsgType & 0x80))
        {
            return CanCcbAfDataHOCirReqMsg(pCanItfMsg);
        }
        return CanCcbAfDataHOCirRspMsg(pCanItfMsg);
    case APP_PACKET_HO_QTW:
        if (!(pmg->hdr.ucMsgType & 0x80))
        {
            return CanCcbAfDataHOQtwReqMsg(pCanItfMsg);
        }
        return CanCcbAfDataHOQtwRspMsg(pCanItfMsg);
    case APP_PACKET_HO_SYSTEM_TEST:
        if (!(pmg->hdr.ucMsgType & 0x80))
        {
           return CanCcbAfDataHODecPressureReqMsg(pCanItfMsg);
        }
        return CanCcbAfDataHODecPressureRspMsg(pCanItfMsg);
        break;
    case APP_PACKET_HO_SPEED:
        if (!(pmg->hdr.ucMsgType & 0x80))
        {
            return CanCcbAfDataHOSpeedReqMsg(pCanItfMsg);
        }
        return CanCcbAfDataHOSpeedRspMsg(pCanItfMsg);
    case APP_PACKET_HO_ADR_RST:
    case APP_PACKET_HO_ADR_SET:
    case APP_PACKET_HO_ADR_QRY:
        if ((pmg->hdr.ucMsgType & 0x80))
        {
            CanCcbIapProc(APP_TRX_CAN,pCanItfMsg);  /* route to UI */   
        }
        break;
    }
    return 0;
}



int CanCcbAfDataRfSearchRsp(MAIN_CANITF_MSG_STRU *pCanItfMsg)
{
    APP_PACKET_RF_STRU *pmg = (APP_PACKET_RF_STRU *)&pCanItfMsg->aucData[1 + RPC_POS_DAT0]; 

    APP_PACKET_RF_SEARCH_RSP_STRU *pRsp = (APP_PACKET_RF_SEARCH_RSP_STRU *)pmg->aucData;

    uint8_t ucResult = APP_PACKET_HO_ERROR_CODE_UNSUPPORT;

    int     iSrcAdr  = CAN_SRC_ADDRESS(pCanItfMsg->ulCanId);

    uint8_t ucIndex  = (iSrcAdr - APP_RF_READER_BEGIN_ADDRESS);

    if (ucIndex >=  MAX_RFREADER_NUM)
    {
        ucResult = APP_PACKET_RF_NO_DEVICE;
        goto end;
    }

    RFREADER_STRU *pRfReader = &gCcb.aRfReader[ucIndex];

    if (pRsp->ucLen)
    {
        APP_PACKET_RPT_RF_STATE_CONT_STRU *pCnt = (APP_PACKET_RPT_RF_STATE_CONT_STRU *)pRsp->aucData;
        pRfReader->ucBlkNum  = pCnt->ucBlkNum;
        pRfReader->ucBlkSize = pCnt->ucBlkSize;
        memcpy(pRfReader->aucUid,pCnt->aucUid,8);

        if (!pRfReader->bit1RfDetected)
        {
            pRfReader->bit1RfDetected  = TRUE;
            pRfReader->bit1RfContValid = FALSE;
        }
    }
    else
    {
        pRfReader->bit1RfDetected  = FALSE;
        pRfReader->bit1RfContValid = FALSE;
    }

    return 0;
end:
    
    return ucResult;
}

int CanCcbAfDataRfReadRsp(MAIN_CANITF_MSG_STRU *pCanItfMsg)
{
    APP_PACKET_RF_STRU *pmg = (APP_PACKET_RF_STRU *)&pCanItfMsg->aucData[1 + RPC_POS_DAT0]; 

    APP_PACKET_RF_READ_RSP_STRU *pRsp = (APP_PACKET_RF_READ_RSP_STRU *)pmg->aucData;

    uint8_t ucResult = APP_PACKET_HO_ERROR_CODE_UNSUPPORT;

    int     iSrcAdr  = CAN_SRC_ADDRESS(pCanItfMsg->ulCanId);

    uint8_t ucIndex  = (iSrcAdr - APP_RF_READER_BEGIN_ADDRESS);

    if (ucIndex >=  MAX_RFREADER_NUM)
    {
        ucResult = APP_PACKET_RF_NO_DEVICE;
        goto end;
    }

    RFREADER_STRU *pRfReader = &gCcb.aRfReader[ucIndex];

    if (APP_PACKET_RF_ERROR_CODE_SUCC == pRsp->ucRslt)
    {
        APP_PACKET_RPT_RF_READ_CONT_STRU *pCnt = (APP_PACKET_RPT_RF_READ_CONT_STRU *)pRsp->aucData;

        if (pCnt->ucOff + pCnt->ucLen <= MAX_RFREADR_CONTENT_SIZE)
        {
            memcpy(&pRfReader->aucRfCont[pCnt->ucOff],pCnt->aucData,pCnt->ucLen);

            pRfReader->bit1RfContValid = TRUE;
        }

    }

    VOS_LOG(VOS_LOG_WARNING,"Enter %s",__FUNCTION__);    

    return 0;
end:
    
    return ucResult;
}



int CanCcbAfDataRfWriteRsp(MAIN_CANITF_MSG_STRU *pCanItfMsg)
{
    (void)pCanItfMsg;
    VOS_LOG(VOS_LOG_WARNING,"Enter %s",__FUNCTION__);    
    return 0;
}


int CanCcbAfDataRfOpsMsg(MAIN_CANITF_MSG_STRU *pCanItfMsg)
{
    APP_PACKET_HO_STRU *pmg = (APP_PACKET_HO_STRU *)&pCanItfMsg->aucData[1 + RPC_POS_DAT0]; 


    switch(pmg->ucOpsType)
    {
    case APP_PACKET_RF_SEARCH:
        if ((pmg->hdr.ucMsgType & 0x80))
        {
            CanCcbAfDataRfSearchRsp(pCanItfMsg); 
            
            CanCcbAfRfIdMsg(0x4);
            /* pump to ui */
            CanCcbIapProc(APP_TRX_CAN,pCanItfMsg);
        }
        break;
    case APP_PACKET_RF_READ:
        if ((pmg->hdr.ucMsgType & 0x80))
        {
            /* pump to ui */
            CanCcbAfDataRfReadRsp(pCanItfMsg);
            
            CanCcbAfRfIdMsg(0x1);
            
            CanCcbIapProc(APP_TRX_CAN,pCanItfMsg);
        }
        break;
    case APP_PACKET_RF_WRITE:
        if ((pmg->hdr.ucMsgType & 0x80))
        {
            CanCcbAfDataRfWriteRsp(pCanItfMsg);
            
            CanCcbAfRfIdMsg(0x2);
            
            /* pump to ui */
            CanCcbIapProc(APP_TRX_CAN,pCanItfMsg);  
        }
        break;
    case APP_PACKET_RF_ADR_RST:
    case APP_PACKET_RF_ADR_SET:
    case APP_PACKET_RF_ADR_QRY:
        if ((pmg->hdr.ucMsgType & 0x80))
        {
            CanCcbIapProc(APP_TRX_CAN,pCanItfMsg);  
        }
        break;
    }
    return 0;
}


int CanCcbAfDataStateIndMsg(MAIN_CANITF_MSG_STRU *pCanItfMsg)
{
    APP_PACKET_STATE_IND_STRU *pmg = (APP_PACKET_STATE_IND_STRU *)&pCanItfMsg->aucData[1 + RPC_POS_DAT0]; 

    {
        int iPackets = (pmg->hdr.ucLen ) / sizeof (APP_PACKET_STATE_STRU);
        APP_PACKET_STATE_STRU *pState = (APP_PACKET_STATE_STRU *)pmg->aucData;
        int iLoop;
        
        for (iLoop = 0; iLoop < iPackets; iLoop++,pState++ )
        {
            switch(pState->ucType)
            {
            case APP_OBJ_N_PUMP:   
                if (pState->ucId < APP_EXE_G_PUMP_NUM)
                {
                    gCcb.ExeBrd.aGPumpObjs[pState->ucId].iState = !!pState->ucState;
                }
                break;
            case APP_OBJ_R_PUMP:   
                if (pState->ucId < APP_EXE_R_PUMP_NUM)
                {
                    gCcb.ExeBrd.aRPumpObjs[pState->ucId].iState = !!pState->ucState;
                }
                break;
            }
        }
    }

    return 0;
}

int CanCcbAfDataExeTocRspMsg(MAIN_CANITF_MSG_STRU *pCanItfMsg)
{
    /* do nothing here */
    pCanItfMsg = pCanItfMsg;

   
   VOS_LOG(VOS_LOG_DEBUG,"Enter %s",__FUNCTION__);    
    
    return 0;
}

int CanCcbAfDataExeOpsMsg(MAIN_CANITF_MSG_STRU *pCanItfMsg)
{
    APP_PACKET_EXE_STRU *pmg = (APP_PACKET_EXE_STRU *)&pCanItfMsg->aucData[1 + RPC_POS_DAT0]; 

    VOS_LOG(VOS_LOG_DEBUG,"Enter %s",__FUNCTION__);    

    switch(pmg->ucOpsType)
    {
    case APP_PACKET_EXE_TOC:
        if ((pmg->hdr.ucMsgType & 0x80))
        {
            return CanCcbAfDataExeTocRspMsg(pCanItfMsg);
        }
        break;
    default:
        break;

    }
    return 0;
}

int CanCcbAfDataMsg(MAIN_CANITF_MSG_STRU *pCanItfMsg)
{
    APP_PACKET_COMM_STRU *pmg = (APP_PACKET_COMM_STRU *)&pCanItfMsg->aucData[1 + RPC_POS_DAT0]; 
    switch((pmg->ucMsgType & 0x7f))
    {
    case APP_PAKCET_COMM_ONLINE_NOTI:
        if (!(pmg->ucMsgType & 0x80))
        {
            return CanCcbAfDataOnLineNotiIndMsg(pCanItfMsg);
        }
        break;
    case APP_PACKET_COMM_HEART_BEAT:
        if ((pmg->ucMsgType & 0x80))
        {
            return CanCcbAfDataHeartBeatRspMsg(pCanItfMsg);
        }
        break;
    case APP_PACKET_CLIENT_REPORT:
        CanCcbAfDataClientRptMsg(pCanItfMsg);
        break;
    case APP_PACKET_HAND_OPERATION:
        CanCcbAfDataHandleOpsMsg(pCanItfMsg);
        break;
    case APP_PACKET_RF_OPERATION:
        CanCcbAfDataRfOpsMsg(pCanItfMsg);
        break;
    case APP_PACKET_STATE_INDICATION:
        CanCcbAfDataStateIndMsg(pCanItfMsg);
        break;
    case APP_PACKET_EXE_OPERATION:
        CanCcbAfDataExeOpsMsg(pCanItfMsg);
        break;
    default: // yet to be implemented
        break;
    }

    return 0;
}


void CanCcbAfProc(MAIN_CANITF_MSG_STRU *pCanItfMsg)
{
    switch((pCanItfMsg->aucData[1 + RPC_POS_CMD1] & 0X7F))
    {
    case SAPP_CMD_DATA:
        CanCcbAfDataMsg(pCanItfMsg);
        break;
    case SAPP_CMD_MODBUS:
        CanCcbAfModbusMsg(pCanItfMsg);
        break;
    }
}

void CanCcbHandlerMgrProc(MAIN_CANITF_MSG_STRU *pCanItfMsg)
{
   /* Notify Ui */
   IAP_NOTIFY_STRU *pNotify = (IAP_NOTIFY_STRU *)gaucIapBuffer;

   pNotify->ulCanId = pCanItfMsg->ulCanId;
   pNotify->iCanChl = pCanItfMsg->iCanChl;
   pNotify->iMsgLen = pCanItfMsg->iMsgLen;
   pNotify->iTrxIndex = APP_TRX_CAN;

   memcpy(pNotify->data,pCanItfMsg->aucData,pNotify->iMsgLen);

   DispIapIndEntry(pNotify);

}

void CanCcbCanItfMsg(SAT_MSG_HEAD *pucMsg)
{
    MAIN_CANITF_MSG_STRU *pCanItfMsg = (MAIN_CANITF_MSG_STRU *)pucMsg;

    switch(pCanItfMsg->aucData[1 + RPC_POS_CMD0])
    {
    case RPC_SYS_AF:
        CanCcbAfProc(pCanItfMsg);
        break;
    case RPC_SYS_BOOT:
        CanCcbIapProc(APP_TRX_CAN,pCanItfMsg);
        break;
    default:
        break;
    }
}


int CanCcbZigbeeHOCirReqMsg(ZBITF_MAIN_MSG_STRU *pZigbeeItfMsg)
{
    APP_PACKET_HO_STRU *pmg = (APP_PACKET_HO_STRU *)&pZigbeeItfMsg->aucData[APP_ZIGBEE_SUB_PROTOL_LENGTH]; 

    APP_PACKET_HO_CIR_REQ_STRU *pCirReq = (APP_PACKET_HO_CIR_REQ_STRU *)pmg->aucData;

    uint8_t ucResult = APP_PACKET_HO_ERROR_CODE_UNSUPPORT;
    //int     iSrcAdr ;
    uint8_t ucIndex ;

    ucIndex = CanCcbZigbeeSearchHandset (pZigbeeItfMsg->usShortAdr);

    if (ucIndex >= MAX_HANDLER_NUM)
    {
        VOS_LOG(VOS_LOG_DEBUG,"unknow hs %x\r\n",pZigbeeItfMsg->usShortAdr);
        CcbZigbeeResetHandset(pZigbeeItfMsg->usShortAdr);
        
        return -1;
    }

    //iSrcAdr = ucIndex + APP_HAND_SET_BEGIN_ADDRESS;

    VOS_LOG(VOS_LOG_WARNING,"Enter %s:%d:%d",__FUNCTION__,pCirReq->ucAction,ucIndex);    


    switch(pCirReq->ucAction)
    {
    case APP_PACKET_HO_ACTION_START:
        {
            if (DISP_WORK_STATE_RUN == gCcb.curWorkState.iMainWorkState4Pw
                && DISP_WORK_SUB_IDLE == gCcb.curWorkState.iSubWorkState4Pw)
            {
               if (!SearchWork(work_start_cir))
               {
                   if (APP_DEV_HS_SUB_HYPER == gCcb.aHandler[ucIndex].iDevType )
                   {
                       CcbInnerWorkStartCir(CIR_TYPE_UP);
                   }
                   else 
                   {
                       if (haveHPCir(&gCcb))
                       {
                           CcbInnerWorkStartCir(CIR_TYPE_HP);
                       }
                   
                   }
               }
               ucResult = APP_PACKET_HO_ERROR_CODE_SUCC;
            }
        }
        break;
    default:
        if (DISP_WORK_STATE_RUN == gCcb.curWorkState.iMainWorkState4Pw
            && DISP_WORK_SUB_RUN_CIR == gCcb.curWorkState.iSubWorkState4Pw)
        {
           if (!SearchWork(work_stop_cir))
           {
               CcbInnerWorkStopCir();
           }   
           ucResult = APP_PACKET_HO_ERROR_CODE_SUCC;
        }
        break;
    }

    {
        /* send response */
        unsigned char buf[32];

        sAddr_t addr ;

        APP_PACKET_HO_STRU *pHoRsp = (APP_PACKET_HO_STRU *)buf;
        APP_PACKET_HO_CIR_RSP_STRU *pCirRsp = (APP_PACKET_HO_CIR_RSP_STRU *)pHoRsp->aucData;

        pHoRsp->hdr = pmg->hdr;

        pHoRsp->hdr.ucLen     = sizeof(APP_PACKET_HO_CIR_RSP_STRU) + 1; // 1 for ops type
        pHoRsp->hdr.ucDevType = APP_DEV_TYPE_MAIN_CTRL;
        pHoRsp->hdr.ucMsgType |= 0x80;
        pHoRsp->ucOpsType     = pmg->ucOpsType;

        pCirRsp->ucAction     = pCirReq->ucAction;
        pCirRsp->ucResult     = ucResult;

        addr.addrMode            = afAddr16Bit;
        addr.addr.shortAddr      = pZigbeeItfMsg->usShortAdr;

        CcbSndZigbeeCmd(&addr,SAPP_CMD_DATA,(unsigned char *)buf,APP_PROTOL_HEADER_LEN + pHoRsp->hdr.ucLen );


    }
    return 0;
}


int CanCcbZigbeeHOQtwReqMsg(ZBITF_MAIN_MSG_STRU *pZigbeeItfMsg)
{

    APP_PACKET_HO_STRU *pmg = (APP_PACKET_HO_STRU *)&pZigbeeItfMsg->aucData[APP_ZIGBEE_SUB_PROTOL_LENGTH]; 

    APP_PACKET_HO_QTW_REQ_STRU *pQtwReq = (APP_PACKET_HO_QTW_REQ_STRU *)pmg->aucData;

    uint8_t ucResult = APP_PACKET_HO_ERROR_CODE_UNSUPPORT;
    int     iSrcAdr ;
    uint8_t ucIndex ;
    int     iType = APP_DEV_HS_SUB_NUM;
    
    ucIndex = CanCcbZigbeeSearchHandset (pZigbeeItfMsg->usShortAdr);

    if (ucIndex >= MAX_HANDLER_NUM)
    {
        VOS_LOG(VOS_LOG_DEBUG,"unknow hs %x\r\n",pZigbeeItfMsg->usShortAdr);
        CcbZigbeeResetHandset(pZigbeeItfMsg->usShortAdr);
        
        return -1;
    }
    iType   = gCcb.aHandler[ucIndex].iDevType;
    iSrcAdr = ucIndex + APP_HAND_SET_BEGIN_ADDRESS;

    VOS_LOG(VOS_LOG_WARNING,"Enter %s:%d:%d:%d",__FUNCTION__,pQtwReq->ucAction,iSrcAdr,pQtwReq->ulVolumn);    

    switch(pQtwReq->ucAction)
    {
    case APP_PACKET_HO_ACTION_START:
        {
            /* late implement */
            if (DISP_WORK_STATE_RUN != gCcb.curWorkState.iMainWorkState4Pw)
            {
                ucResult = APP_PACKET_HO_ERROR_CODE_UNREADY; // failed
            }
            else
            {
               /* setup work */
               if (CcbGetTwFlag())
               {
                    if (!gCcb.aHandler[ucIndex].bit1Qtw)
                    {
                        ucResult = APP_PACKET_HO_ERROR_CODE_UNSUPPORT; // current only support one tw
                    }
               }
               else
               {
                   CanCcbSaveQtwMsg(APP_TRX_ZIGBEE,pZigbeeItfMsg);

                   /* remove cir work if any */
                   CcbRmvWork(work_start_cir);
                    
                   /* construct work */
                   CcbInnerWorkStartQtw(ucIndex);

                   return 0;
               }
            }
        }
        break;
    case APP_PACKET_HO_ACTION_STOP:
        {
            /* setup work */
            if (!gCcb.aHandler[ucIndex].bit1Qtw)
            {
                ucResult = APP_PACKET_HO_ERROR_CODE_SUCC; //  

                /* 2018/01/24 add for adpater because of its prolonged tw procedure */
                {
                    DISPHANDLE hdl ;
                    
                    hdl = SearchWork(work_start_qtw);
                    
                    if (hdl)
                    {
                        CcbCancelWork(hdl);
                    }
                }
                
            }
            else
            {
                CcbInnerWorkStopQtw(ucIndex);
            }
        }
        break;
    default:
        ucResult = APP_PACKET_HO_ERROR_CODE_UNKNOW; // failed
        break;
    }

    {
        /* send response */
        unsigned char buf[32];

        sAddr_t addr ;

        APP_PACKET_HO_STRU *pHoRsp = (APP_PACKET_HO_STRU *)buf;
        APP_PACKET_HO_QTW_RSP_STRU *pQtwRsp = (APP_PACKET_HO_QTW_RSP_STRU *)pHoRsp->aucData;

        pHoRsp->hdr           = pmg->hdr;
        pHoRsp->hdr.ucLen     = sizeof(APP_PACKET_HO_QTW_RSP_STRU) + 1; // 1 for ops type
        pHoRsp->hdr.ucDevType = APP_DEV_TYPE_MAIN_CTRL;
        pHoRsp->hdr.ucMsgType |= 0x80;
        pHoRsp->ucOpsType     = pmg->ucOpsType;

        CanccbFillQtwRsp(pQtwRsp,pQtwReq->ucAction,ucResult);
        pQtwRsp->ucType       = iType;

        addr.addrMode            = afAddr16Bit;
        addr.addr.shortAddr      = pZigbeeItfMsg->usShortAdr;

        CcbSndZigbeeCmd(&addr,SAPP_CMD_DATA,(unsigned char *)buf,APP_PROTOL_HEADER_LEN + pHoRsp->hdr.ucLen );

    }

    return 0;
}

int CanCcbZigbeeHODecPressureReqMsg(ZBITF_MAIN_MSG_STRU *pZigbeeItfMsg)
{

    APP_PACKET_HO_STRU *pmg = (APP_PACKET_HO_STRU *)&pZigbeeItfMsg->aucData[APP_ZIGBEE_SUB_PROTOL_LENGTH]; 

    APP_PACKET_HO_SYSTEMTEST_REQ_STRU *pDecPreReq = (APP_PACKET_HO_SYSTEMTEST_REQ_STRU *)pmg->aucData;

    uint8_t ucResult = APP_PACKET_HO_ERROR_CODE_UNSUPPORT;
    int     iSrcAdr ;
    uint8_t ucIndex ;
    int     iType = APP_DEV_HS_SUB_NUM;
    
    ucIndex = CanCcbZigbeeSearchHandset (pZigbeeItfMsg->usShortAdr);

    if (ucIndex >= MAX_HANDLER_NUM)
    {
        VOS_LOG(VOS_LOG_DEBUG,"unknow hs %x\r\n",pZigbeeItfMsg->usShortAdr);
        CcbZigbeeResetHandset(pZigbeeItfMsg->usShortAdr);
        return -1;
    }
    iType   = gCcb.aHandler[ucIndex].iDevType;
    iSrcAdr = ucIndex + APP_HAND_SET_BEGIN_ADDRESS;

    if (DISP_WORK_STATE_IDLE != gCcb.curWorkState.iMainWorkState4Pw)
    {
        return -1;
    }
    
    ucResult = APP_PACKET_HO_ERROR_CODE_SUCC; //  
    
    CcbNotDecPressure(iType,pDecPreReq->ucAction);

    {
        /* send response */
        unsigned char buf[32];

        sAddr_t addr ;
        
        unsigned int ulCanId;
        int          iPayLoad;
        
        APP_PACKET_HO_STRU *pHoRsp = (APP_PACKET_HO_STRU *)buf;
        APP_PACKET_HO_SYSTEMTEST_RSP_STRU *pLoad = (APP_PACKET_HO_SYSTEMTEST_RSP_STRU *)pHoRsp->aucData;
                
        pLoad->ucAction          = pDecPreReq->ucAction;
        pLoad->ucType            = pDecPreReq->ucType;
        pLoad->ucResult          = ucResult;
        

        pHoRsp->hdr           = pmg->hdr;
        pHoRsp->hdr.ucLen     = sizeof(APP_PACKET_HO_SYSTEMTEST_RSP_STRU) + 1; // 1 for ops type
        pHoRsp->hdr.ucDevType = APP_DEV_TYPE_MAIN_CTRL;
        pHoRsp->hdr.ucMsgType  = APP_PACKET_HAND_OPERATION;
        pHoRsp->ucOpsType     = pmg->ucOpsType;

        pLoad->ucAction          = pDecPreReq->ucAction;
        pLoad->ucType            = pDecPreReq->ucType;
        pLoad->ucResult          = ucResult;

        addr.addrMode            = afAddr16Bit;
        addr.addr.shortAddr      = pZigbeeItfMsg->usShortAdr;

        CcbSndZigbeeCmd(&addr,SAPP_CMD_DATA,(unsigned char *)buf,APP_PROTOL_HEADER_LEN + pHoRsp->hdr.ucLen );

    }
    VOS_LOG(VOS_LOG_WARNING,"Enter %s:%d:%d:%d",__FUNCTION__,pDecPreReq->ucAction,iSrcAdr,pDecPreReq->ucType);    

    return 0;
}

int CanCcbZigbeeHOSpeedReqMsg(ZBITF_MAIN_MSG_STRU *pZigbeeItfMsg)
{
    APP_PACKET_HO_STRU *pmg = (APP_PACKET_HO_STRU *)&pZigbeeItfMsg->aucData[APP_ZIGBEE_SUB_PROTOL_LENGTH]; 

    APP_PACKET_HO_SPEED_REQ_STRU *pSpeedReq = (APP_PACKET_HO_SPEED_REQ_STRU *)pmg->aucData;

    int     iSpeed   = PUMP_SPEED_00;
        //PUMP_SPEED_NUM;
    int     iActive  = 0;

    uint8_t ucResult = APP_PACKET_HO_ERROR_CODE_UNSUPPORT;
    //int     iSrcAdr ;
    uint8_t ucIndex ;

    int      iType = APP_DEV_HS_SUB_NUM;

    ucIndex = CanCcbZigbeeSearchHandset (pZigbeeItfMsg->usShortAdr);

    if (ucIndex >= MAX_HANDLER_NUM)
    {
        VOS_LOG(VOS_LOG_DEBUG,"unknow hs %x\r\n",pZigbeeItfMsg->usShortAdr);
        CcbZigbeeResetHandset(pZigbeeItfMsg->usShortAdr);
        return -1;
    }

    iType = gCcb.aHandler[ucIndex].iDevType;

    //iSrcAdr = ucIndex + APP_HAND_SET_BEGIN_ADDRESS;

    VOS_LOG(VOS_LOG_WARNING,"Enter %s:%d:%d",__FUNCTION__,pSpeedReq->ucAction,ucIndex);    

    DispGetRPumpSwitchState(APP_EXE_C2_NO - APP_EXE_C1_NO,&iActive);

    iSpeed = DispConvertRPumpSpeed2Scale(gCcb.ExeBrd.ausHoldRegister[APP_EXE_HOLD_REG_RPUMP_OFFSET + APP_EXE_C2_NO - APP_EXE_C1_NO]);

    switch(pSpeedReq->ucAction)
    {
    case APP_PACKET_HO_ACTION_START:
        {
            if (iSpeed >= PUMP_SPEED_10)
            {
                ucResult = APP_PACKET_HO_ERROR_CODE_SUCC;
                goto end;
            } 

            iSpeed += 1; 

            CanCcbSaveSpeedMsg(APP_TRX_ZIGBEE,pZigbeeItfMsg,iSpeed);

            if (!SearchWork(work_start_speed_regulation))
            {
                CcbInnerWorkStartSpeedRegulation(ucIndex);
            }
            
            return 0;
            
        }
        break;
    default:
        {
            if (iSpeed <= PUMP_SPEED_00)
            {
                ucResult = APP_PACKET_HO_ERROR_CODE_SUCC;
                goto end;
            } 

            iSpeed -= 1; 
            
            CanCcbSaveSpeedMsg(APP_TRX_ZIGBEE,pZigbeeItfMsg,iSpeed);
            
            if (!SearchWork(work_start_speed_regulation))
            {
                CcbInnerWorkStartSpeedRegulation(ucIndex);
            }

            return 0;
        }
        break;
    }

end:
    {
        /* send response */
        unsigned char buf[32];

        sAddr_t addr ;

        APP_PACKET_HO_STRU           *pHoRsp  = (APP_PACKET_HO_STRU *)buf;
        APP_PACKET_HO_SPEED_RSP_STRU *pSpeedRsp = (APP_PACKET_HO_SPEED_RSP_STRU *)pHoRsp->aucData;

        pHoRsp->hdr = pmg->hdr;

        pHoRsp->hdr.ucLen     = sizeof(APP_PACKET_HO_SPEED_RSP_STRU) + 1; // 1 for ops type
        pHoRsp->hdr.ucDevType = APP_DEV_TYPE_MAIN_CTRL;
        pHoRsp->hdr.ucMsgType |= 0x80;
        pHoRsp->ucOpsType     = pmg->ucOpsType;

        pSpeedRsp->ucAction     = pSpeedReq->ucAction;
        pSpeedRsp->ucResult     = ucResult;
        pSpeedRsp->ucSpeed      = iSpeed;
        pSpeedRsp->ucType       = iType;

        addr.addrMode            = afAddr16Bit;
        addr.addr.shortAddr      = pZigbeeItfMsg->usShortAdr;

        CcbSndZigbeeCmd(&addr,SAPP_CMD_DATA,(unsigned char *)buf,APP_PROTOL_HEADER_LEN + pHoRsp->hdr.ucLen );

    }
    return 0;
}



int CanCcbZigbeeIndMsg(ZBITF_MAIN_MSG_STRU *pZigbeeItfMsg)
{
    APP_PACKET_ZIGBEE_IND_STRU *pZigbeeInd = (APP_PACKET_ZIGBEE_IND_STRU *)&pZigbeeItfMsg->aucData[APP_ZIGBEE_SUB_PROTOL_LENGTH]; 

    switch(pZigbeeInd->hdr.ucDevType)
    {
    case APP_DEV_TYPE_HAND_SET:
        /* fill infomation */
        {
            int iIdx = pZigbeeInd->usAddress - APP_HAND_SET_BEGIN_ADDRESS;
    
            if (pZigbeeInd->usAddress  >= APP_HAND_SET_BEGIN_ADDRESS 
                 && pZigbeeInd->usAddress  < APP_HAND_SET_BEGIN_ADDRESS + APP_HAND_SET_MAX_NUMBER)
            {
                if (!gCcb.aHandler[iIdx].bit1Active)
                {
                    gCcb.aHandler[iIdx].bit1Active = TRUE;
        
                    gCcb.aHandler[iIdx].iTrxMap |= (1 << APP_TRX_ZIGBEE);
        
                    gCcb.aHandler[iIdx].usShortAddr = pZigbeeItfMsg->usShortAdr;
                    CcbHandlerNotify(iIdx);
                }
        
            }

        }
        break;
    }
    return 0;
}


int CanCcbZigbeeHandleOpsMsg(ZBITF_MAIN_MSG_STRU *pZigbeeItfMsg)
{
    APP_PACKET_HO_STRU *pmg = (APP_PACKET_HO_STRU *)&pZigbeeItfMsg->aucData[APP_ZIGBEE_SUB_PROTOL_LENGTH]; 

    switch(pmg->ucOpsType)
    {
    case APP_PACKET_HO_CIR:
        if (!(pmg->hdr.ucMsgType & 0x80))
        {
            return CanCcbZigbeeHOCirReqMsg(pZigbeeItfMsg);
        }
        return 0;
    case APP_PACKET_HO_QTW:
        if (!(pmg->hdr.ucMsgType & 0x80))
        {
            return CanCcbZigbeeHOQtwReqMsg(pZigbeeItfMsg);
        }
        return 0;
    case APP_PACKET_HO_SYSTEM_TEST:
        if (!(pmg->hdr.ucMsgType & 0x80))
        {
           return CanCcbZigbeeHODecPressureReqMsg(pZigbeeItfMsg);
        }
        break;
    case APP_PACKET_HO_SPEED:
        if (!(pmg->hdr.ucMsgType & 0x80))
        {
            return CanCcbZigbeeHOSpeedReqMsg(pZigbeeItfMsg);
        }
        return 0;
    case APP_PACKET_HO_ADR_RST:
    case APP_PACKET_HO_ADR_SET:
    case APP_PACKET_HO_ADR_QRY:
        if ((pmg->hdr.ucMsgType & 0x80))
        {
            CanCcbIapProc(APP_TRX_ZIGBEE,pZigbeeItfMsg);  /* route to UI */   
        }
        break;
    }
    return 0;
}


void CanCcbZigbeeItfMsg(SAT_MSG_HEAD *pucMsg)
{
    ZBITF_MAIN_MSG_STRU *pZigbeeItfMsg = (ZBITF_MAIN_MSG_STRU *)pucMsg;

    uint8_t ucProtol = pZigbeeItfMsg->aucData[0];


    VOS_LOG(VOS_LOG_DEBUG,"zbitf short addr = %d",pZigbeeItfMsg->usShortAdr);
    switch (ucProtol & 0X7F)
    {
    case SAPP_CMD_DATA:
         {
            APP_PACKET_COMM_STRU *pmg = (APP_PACKET_COMM_STRU *)&pZigbeeItfMsg->aucData[APP_ZIGBEE_SUB_PROTOL_LENGTH]; 
            switch((pmg->ucMsgType & 0x7f))
            {
            case APP_PAKCET_COMM_ONLINE_NOTI:
                if (!(pmg->ucMsgType & 0x80))
                {
                    CanCcbZigbeeOnLineNotiIndMsg(pZigbeeItfMsg);
                }
                break;
            case APP_PACKET_COMM_HEART_BEAT:
                if ((pmg->ucMsgType & 0x80))
                {
                    CanCcbZigbeeHeartBeatRspMsg(pZigbeeItfMsg);
                }
                break;
            case APP_PACKET_COMM_ZIGBEE_IND:
                CanCcbZigbeeIndMsg(pZigbeeItfMsg);
                break;
            case APP_PACKET_HAND_OPERATION:
                CanCcbZigbeeHandleOpsMsg(pZigbeeItfMsg);
                break;
            default: // yet to be implemented
                break;
            }  
        }
        break;
    case SAPP_CMD_MODBUS:
        break;
    }
}

void CcbZigbeeResetHandset(uint16 usShortAddr)
{
     APP_PACKET_HOST_RESET_STRU Rst;

     memset(&Rst,0,sizeof(APP_PACKET_HOST_RESET_STRU));

     Rst.hdr.ucDevType = APP_DEV_TYPE_MAIN_CTRL ;
     Rst.hdr.ucMsgType = APP_PACKET_COMM_HOST_RESET;

     if (haveZigbee(&gCcb))
     {
         sAddr_t addr;
         addr.addrMode            = afAddr16Bit;
         addr.addr.shortAddr      = usShortAddr;
         CcbSndZigbeeCmd(&addr,SAPP_CMD_DATA,(unsigned char *)&Rst,sizeof(Rst));
     }
}

void CanCcbZigbeeReportMsg(SAT_MSG_HEAD *pucMsg)
{
    ZBITF_MAIN_MSG_STRU *pZigbeeItfMsg = (ZBITF_MAIN_MSG_STRU *)pucMsg;

    Node_t Node;

    memcpy(&Node,pZigbeeItfMsg->aucData,sizeof(Node_t));

    CcbZigbeeResetHandset(Node.shortAddr);

}

/* 2018/04/12 add */
void CanCcbZbMainMtnMsg(SAT_MSG_HEAD *pucMsg)
{
    ZBITF_MAIN_MTN_MSG_STRU *pZigbeeItfMsg = (ZBITF_MAIN_MTN_MSG_STRU *)pucMsg;


    switch(pZigbeeItfMsg->usSubCode)
    {
    case ZBITF_MAIN_MTN_CODE_RESET:
        {
            ZBITF_MAIN_MTN_RESET_STRU *pReset = (ZBITF_MAIN_MTN_RESET_STRU *)pZigbeeItfMsg->aucData;
            if (!pReset->state)
            {
                if (haveZigbee(&gCcb))
                {
                    APP_PACKET_HOST_RESET_STRU Rst;
               
                    memset(&Rst,0,sizeof(APP_PACKET_HOST_RESET_STRU));
               
                    Rst.hdr.ucDevType = APP_DEV_TYPE_MAIN_CTRL ;
                    Rst.hdr.ucMsgType = APP_PACKET_COMM_HOST_RESET;

                    // issue blind reset whatsever!!
                    CcbZigbeeModeBroadcast((unsigned char *)&Rst,sizeof(Rst));
                }
            }
        }
        break;
    }

}


void CanCcbRestore(void)
{
    if (gCcb.bit1LeakKey4Reset)
    {
        return ;
    }

    switch(gCcb.curWorkState.iMainWorkState)
    {
    case DISP_WORK_STATE_RUN:
        gCcb.curWorkState.iSubWorkState = DISP_WORK_SUB_IDLE;
        CcbCancelAllWork();
        CcbInnerWorkRun();
        break;
    default:
        break;
    }
}

void CcbWorMsgProc(SAT_MSG_HEAD *pucMsg)
{
    MAIN_WORK_MSG_STRU *pWorkMsg = (MAIN_WORK_MSG_STRU *)pucMsg;


    switch(pWorkMsg->iSubMsg)
    {
    case WORK_MSG_TYPE_START_QTW:
        {
            int aResult[2];

            memcpy(aResult,pWorkMsg->aucData,sizeof(aResult));

            gCcb.aHandler[aResult[1]].bit1Qtw = aResult[0] == 0 ? 1 : 0;

            gCcb.aHandler[aResult[1]].bit1PendingQtw = 0;

            CanCcbSndQtwRspMsg(aResult[1],aResult[0]);

            if (!aResult[0])
            {
                VOS_LOG(VOS_LOG_WARNING,"CanCcbTransState4Pw");    
            
                CanCcbTransState4Pw(DISP_WORK_STATE_RUN,DISP_WORK_SUB_RUN_QTW);

                CcbQTwVolumnNotify(gCcb.aHandler[aResult[1]].iDevType,aResult[1],&gCcb.QtwMeas);
                
            }
            CcbNotAscInfo(pWorkMsg->iSubMsg*2 + !!aResult[0]);

        }
        break;
        
    case WORK_MSG_TYPE_STOP_QTW:
        {
            int aResult[2];
    
            memcpy(aResult,pWorkMsg->aucData,sizeof(aResult));
    
            gCcb.aHandler[aResult[1]].bit1Qtw = 0;

            gCcb.aHandler[aResult[1]].bit1PendingQtw = 0;
            
            CcbNotAscInfo(pWorkMsg->iSubMsg*2 + !!aResult[0]);

            VOS_LOG(VOS_LOG_WARNING,"CanCcbTransState4Pw");    

            /* report */
            CanCcbTransState4Pw(DISP_WORK_STATE_RUN,DISP_WORK_SUB_IDLE);

            if (APP_DEV_HS_SUB_HYPER == gCcb.aHandler[aResult[1]].iDevType)
            {
                if (gCcb.QtwMeas.ulCurFm > gCcb.QtwMeas.ulBgnFm)
                {
                   CcbTwStaticsNotify(APP_DEV_HS_SUB_HYPER,aResult[1],&gCcb.QtwMeas);
                }
            
                /* start circulation */
               if (!SearchWork(work_start_cir))
               {
                   CcbInnerWorkStartCir(CIR_TYPE_UP);
               }   
            }
            else
            {
                if (gCcb.QtwMeas.ulCurFm > gCcb.QtwMeas.ulBgnFm)
                {
                   CcbTwStaticsNotify(APP_DEV_HS_SUB_REGULAR,aResult[1],&gCcb.QtwMeas);
                }

                switch(gCcb.ulMachineType)
                {
                case MACHINE_ADAPT:
                case MACHINE_PURIST:
                    break;
                default:
                    /* start circulation if any */
                    if (!gCcb.bit1I44Nw)
                    {
                        if (haveHPCir(&gCcb))
                        {
                             if (!SearchWork(work_start_cir))
                             {
                                 CcbInnerWorkStartCir(CIR_TYPE_HP);
                                 break;
                             }  
                        }
                    }
                    break;
                }
                
            }
            
        }
        break;
    case WORK_MSG_TYPE_SCIR:
        {
            int aResult[1];
    
            memcpy(aResult,pWorkMsg->aucData,sizeof(aResult));
    
            CcbNotAscInfo(pWorkMsg->iSubMsg*2 + !!aResult[0]);

            if (0 == aResult[0])
            {
                gCcb.ulCirTick = gulSecond;

                CanCcbTransState4Pw(DISP_WORK_STATE_RUN,DISP_WORK_SUB_RUN_CIR);
            
            }
            else
            {
                VOS_LOG(VOS_LOG_WARNING,"CanCcbTransState4Pw");   
                
                CanCcbTransState4Pw(DISP_WORK_STATE_RUN,DISP_WORK_SUB_IDLE);
            }
        }        
        break;
    case WORK_MSG_TYPE_ECIR:
        {
            int aResult[1];
    
            memcpy(aResult,pWorkMsg->aucData,sizeof(aResult));
    
            CcbNotAscInfo(pWorkMsg->iSubMsg*2 + !!aResult[0]);

            gCcb.ulLstCirTick = gulSecond;

            switch(gCcb.iCirType)
            {
            case CIR_TYPE_UP:
                {
                    float fValue = gCcb.ExeBrd.aEcoObjs[APP_EXE_I5_NO].Value.eV.fWaterQ;
                    if (fValue < CcbGetSp7())
                    {
                        /* fire alarm */
                        if (!(gCcb.ulFiredAlarmFlags  & ALARM_UP))
                        {
                            if (gulSecond - gCcb.ulAlarmUPTick >= 30)
                            {
                                gCcb.ulFiredAlarmFlags |= ALARM_UP;
                            
                                CcbNotAlarmFire(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_UP_PRODUCT_RESISTENCE,TRUE);
                            }
                        }
                    }                   
                }
                break;
            default:
                break;
            }

            VOS_LOG(VOS_LOG_WARNING,"CanCcbTransState4Pw");    
            
            CanCcbTransState4Pw(DISP_WORK_STATE_RUN,DISP_WORK_SUB_IDLE);

        }        
        break;
    case WORK_MSG_TYPE_ESR:
        {
            int aResult[2];

            memcpy(aResult,pWorkMsg->aucData,sizeof(aResult));

            CanCcbSndHoSpeedRspMsg(aResult[1],aResult[0]);

            if (aResult[0])
            {
                VOS_LOG(VOS_LOG_WARNING,"Ho Speed Regulation Failed");    
            }

            /* Notify UI */
            CcbNotSpeed(gCcb.aHandler[aResult[1]].iDevType,gCcb.aHandler[aResult[1]].iSpeed);
            
        }
        break;        
    case WORK_MSG_TYPE_INIT_RUN:
        {
            int aResult[1];
    
            memcpy(aResult,pWorkMsg->aucData,sizeof(aResult));
           
            CcbNotAscInfo(pWorkMsg->iSubMsg*2 + !!aResult[0]);

            if (0 == aResult[0])
            {
                /* continue to produce water */
                if (!(DISP_WORK_STATE_RUN == gCcb.curWorkState.iMainWorkState
                     && gCcb.curWorkState.iSubWorkState > DISP_WORK_SUB_RUN_INIT))
                {
                    VOS_LOG(VOS_LOG_WARNING,"CanCcbTransState");    
                    CanCcbTransState(DISP_WORK_STATE_RUN,DISP_WORK_SUB_IDLE);
                
                }
                
                gCcb.ulN3PeriodTimer = 0;
                gCcb.ulN3Duration    = 0;
                gCcb.bit1N3Active    = 0;
            }
            else
            {
                gCcb.bit1ProduceWater = FALSE;
                gCcb.bit1B1Check4RuningState = FALSE;
            
                CanCcbTransState(DISP_WORK_STATE_IDLE,DISP_WORK_SUB_IDLE);
            }

            /* check isolated alarms */
            if (  (gCcb.ulFiredAlarmFlags  & ALARM_ROPW)
                && (!gCcb.bit1AlarmRoPw))
            {
                gCcb.ulFiredAlarmFlags &= ~ALARM_ROPW;
                
                CcbNotAlarmFire(DISP_ALARM_PART1,DISP_ALARM_PART1_HIGER_RO_PRODUCT_CONDUCTIVITY,FALSE);
            
            }
            if (  (gCcb.ulFiredAlarmFlags  & ALARM_REJ)
                && (!gCcb.bit1AlarmRej))
            {
                gCcb.ulFiredAlarmFlags &= ~ALARM_REJ;
                
                CcbNotAlarmFire(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_RO_RESIDUE_RATIO,FALSE);
            
            }
            
            if (  (gCcb.ulFiredAlarmFlags  & ALARM_EDI)
                && (!gCcb.bit1AlarmEDI))
            {
                gCcb.ulFiredAlarmFlags &= ~ALARM_EDI;
                
                CcbNotAlarmFire(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_EDI_PRODUCT_RESISTENCE,FALSE);
            
            }
            
            if (  (gCcb.ulFiredAlarmFlags  & ALARM_ROPLV)
                && (!gCcb.bit1AlarmROPLV))
            {
                gCcb.ulFiredAlarmFlags &= ~ALARM_ROPLV;
                
                CcbNotAlarmFire(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_RO_PRODUCT_FLOWING_VELOCITY,FALSE);
            }
            
            if (  (gCcb.ulFiredAlarmFlags  & ALARM_RODV)
                && (!gCcb.bit1AlarmRODV))
            {
                gCcb.ulFiredAlarmFlags &= ~ALARM_RODV;
                
                CcbNotAlarmFire(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_RO_WASTE_FLOWING_VELOCITY,FALSE);
            }      
        }
        break;
    case WORK_MSG_TYPE_RUN:
        {
            int aResult[1];
    
            memcpy(aResult,pWorkMsg->aucData,sizeof(aResult));
            CcbNotAscInfo(pWorkMsg->iSubMsg*2 + !!aResult[0]);

            if (0 == aResult[0])
            {
                if (!(DISP_WORK_STATE_RUN == gCcb.curWorkState.iMainWorkState
                     && gCcb.curWorkState.iSubWorkState > DISP_WORK_SUB_RUN_INIT))
                {
                    VOS_LOG(VOS_LOG_WARNING,"CanCcbTransState");   
                    
                    CanCcbTransState(DISP_WORK_STATE_RUN,DISP_WORK_SUB_IDLE);
                    
                    //CanCcbSndHOState(APP_PROTOL_CANID_BROADCAST,APP_PACKET_HS_STATE_RUN);
                }
            }
            else
            {
                gCcb.bit1ProduceWater        = FALSE;
                gCcb.bit1B1Check4RuningState = FALSE;
            }

            /* check isolated alarms */
            if (  (gCcb.ulFiredAlarmFlags  & ALARM_ROPW)
                && (!gCcb.bit1AlarmRoPw))
            {
                gCcb.ulFiredAlarmFlags &= ~ALARM_ROPW;
                
                CcbNotAlarmFire(DISP_ALARM_PART1,DISP_ALARM_PART1_HIGER_RO_PRODUCT_CONDUCTIVITY,FALSE);
            
            }
            if (  (gCcb.ulFiredAlarmFlags  & ALARM_REJ)
                && (!gCcb.bit1AlarmRej))
            {
                gCcb.ulFiredAlarmFlags &= ~ALARM_REJ;
                
                CcbNotAlarmFire(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_RO_RESIDUE_RATIO,FALSE);
            
            }

            if (  (gCcb.ulFiredAlarmFlags  & ALARM_EDI)
                && (!gCcb.bit1AlarmEDI))
            {
                gCcb.ulFiredAlarmFlags &= ~ALARM_EDI;
                
                CcbNotAlarmFire(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_EDI_PRODUCT_RESISTENCE,FALSE);
            
            }

            if (  (gCcb.ulFiredAlarmFlags  & ALARM_ROPLV)
                && (!gCcb.bit1AlarmROPLV))
            {
                gCcb.ulFiredAlarmFlags &= ~ALARM_ROPLV;
                
                CcbNotAlarmFire(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_RO_PRODUCT_FLOWING_VELOCITY,FALSE);
            }
            
            if (  (gCcb.ulFiredAlarmFlags  & ALARM_RODV)
                && (!gCcb.bit1AlarmRODV))
            {
                gCcb.ulFiredAlarmFlags &= ~ALARM_RODV;
                
                CcbNotAlarmFire(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_RO_WASTE_FLOWING_VELOCITY,FALSE);
            }            
        }
        break;  
    case WORK_MSG_TYPE_WASH:
        {
            int aResult[2];

            CanCcbTransState(DISP_WORK_STATE_IDLE,DISP_WORK_SUB_IDLE);   
            
            memcpy(aResult,pWorkMsg->aucData,sizeof(aResult));
            
            CcbNotAscInfo(pWorkMsg->iSubMsg*2 + !!aResult[0]);

            CcbWashStateNotify(gCcb.iWashType,0);

            if (0 == aResult[0])
            {
                /* move to RUN state*/
                CcbInnerWorkInitRun();
            }
        }
        break;
    case WORK_MSG_TYPE_SFW:
        {
            int aResult[1];
    
            memcpy(aResult,pWorkMsg->aucData,sizeof(aResult));
            CcbNotAscInfo(pWorkMsg->iSubMsg*2 + !!aResult[0]);
        }
        break;
    case WORK_MSG_TYPE_EFW:
        {
            int aResult[1];
    
            memcpy(aResult,pWorkMsg->aucData,sizeof(aResult));
            CcbNotAscInfo(pWorkMsg->iSubMsg*2 + !!aResult[0]);
        }
        break;
    case WORK_MSG_TYPE_SN3:
        {
            int aResult[1];
    
            memcpy(aResult,pWorkMsg->aucData,sizeof(aResult));
            CcbNotAscInfo(pWorkMsg->iSubMsg*2 + !!aResult[0]);

            if (0 == aResult[0])
            {
                gCcb.bit1N3Active = 1;
                gCcb.ulN3Duration = 0;
            }
        }
        break;
    case WORK_MSG_TYPE_EN3:
        {
            int aResult[1];
    
            memcpy(aResult,pWorkMsg->aucData,sizeof(aResult));
            CcbNotAscInfo(pWorkMsg->iSubMsg*2 + !!aResult[0]);
            gCcb.bit1N3Active = 0;
        }
        break;

    case WORK_MSG_TYPE_SKH:
        {
            int aResult[1];
    
            memcpy(aResult,pWorkMsg->aucData,sizeof(aResult));
            CcbNotAscInfo(pWorkMsg->iSubMsg*2 + !!aResult[0]);

            if (aResult[0])
            {
                CanCcbDinProcess();
            }
        }
        break;
    case WORK_MSG_TYPE_EKH:
        {
            if (gCcb.ExeBrd.ucDinState & APP_EXE_DIN_FAIL_MASK)
            {
                CanCcbDinProcess();
            }
            else
            {
                /* restore to previous working state */
                CanCcbRestore();
            }
        }
        break;
    case WORK_MSG_TYPE_EPW:
        {
            CcbProduceWater(gCcb.ulProduceWaterBgnTime);

            CcbPwVolumeStatistics();

        }
        break;
    case WORK_MSG_TYPE_STATE:
        {
           switch(gCcb.curWorkState.iMainWorkState)
           {
           case DISP_WORK_STATE_IDLE:
               switch(gCcb.curWorkState.iSubWorkState)
               {
               case DISP_WORK_SUB_IDLE:
                    CcbNotState(NOT_STATE_INIT);
                    break;
               case DISP_WORK_SUB_WASH:
                    CcbNotState(NOT_STATE_WASH);
                    break;
               }
               break;
               //2018.12.18
           case DISP_WORK_STATE_PREPARE:
               switch(gCcb.curWorkState.iSubWorkState)
               {
               case DISP_WORK_SUB_IDLE:
                   CcbNotState(NOT_STATE_RUN);
                   break;
               }
               break;
               //end
           case DISP_WORK_STATE_RUN:
                switch(gCcb.curWorkState.iSubWorkState)
                {
                case DISP_WORK_SUB_IDLE:
                case DISP_WORK_SUB_RUN_INIT:
                    CcbNotState(NOT_STATE_RUN);
                    break;
               }
                break;
           case DISP_WORK_STATE_LPP:
                CcbNotState(NOT_STATE_LPP);
                CcbCleanup();
                break;
            //ex
           case DISP_WORK_STATE_KP:
               switch(gCcb.curWorkState.iSubWorkState)
               {
               case DISP_WORK_SUB_IDLE:
                    CcbNotState(NOT_STATE_INIT);
                    break;
               }
               break;
            //end
           }
        }
        break;
    case WORK_MSG_TYPE_STATE4PW:
        {
           switch(gCcb.curWorkState.iMainWorkState4Pw)
           {
           case DISP_WORK_STATE_IDLE:
               switch(gCcb.curWorkState.iSubWorkState4Pw)
               {
               default:
                   CanCcbSndHOState(APP_PROTOL_CANID_BROADCAST,APP_PACKET_HS_STATE_IDLE);
                   break;
               case DISP_WORK_SUB_IDLE_DEPRESSURE:
                   CanCcbSndHOState(APP_PROTOL_CANID_BROADCAST,APP_PACKET_HS_STATE_DEC);
                   break;
               }
               break;
           case DISP_WORK_STATE_RUN:
                switch(gCcb.curWorkState.iSubWorkState4Pw)
                {
                case DISP_WORK_SUB_IDLE:
                    CcbNotState(NOT_STATE_OTHER);
                    CanCcbSndHOState(APP_PROTOL_CANID_BROADCAST,APP_PACKET_HS_STATE_RUN);
                    break;
                case DISP_WORK_SUB_RUN_QTW:
                    CcbNotState(NOT_STATE_QTW);
                    CanCcbSndHOState(APP_PROTOL_CANID_BROADCAST,APP_PACKET_HS_STATE_QTW);
                case DISP_WORK_SUB_RUN_QTWING:
                    break;
                case DISP_WORK_SUB_RUN_CIR:
                    CcbNotState(NOT_STATE_CIR);
                    CanCcbSndHOState(APP_PROTOL_CANID_BROADCAST,APP_PACKET_HS_STATE_CIR);
                case DISP_WORK_SUB_RUN_CIRING:
                    //if (CIR_TYPE_UP == gCcb.iCirType) 
                    break;
                case DISP_WORK_SUB_RUN_DEC:
                    CcbNotState(NOT_STATE_DEC);
                    CanCcbSndHOState(APP_PROTOL_CANID_BROADCAST,APP_PACKET_HS_STATE_DEC);
                    break;
                }
                break;
           }
        }
        break;
    case WORK_MSG_TYPE_IDLE:
        {
            int aResult[1];

            memcpy(aResult,pWorkMsg->aucData,sizeof(aResult));
            
            CcbNotAscInfo(pWorkMsg->iSubMsg*2 + !!aResult[0]);

            CanCcbTransState(DISP_WORK_STATE_IDLE,DISP_WORK_SUB_IDLE);

            CanCcbTransState4Pw(DISP_WORK_STATE_IDLE,DISP_WORK_SUB_IDLE);

            CcbCleanup();
        }
        break;
    default :
        break;
    }

}


int CcbModbusWorkEntry(int iChl,unsigned int ulCanId,unsigned char *pucModbus,int iPayLoad,int iTime,modbus_call_back cb)
{
    int iRet;
    
    struct timeval now;
    struct timespec outtime;
    
    sp_thread_mutex_lock( &gCcb.Ipc.mutex );
    
    iRet = CcbSndAfCanCmd(iChl,ulCanId,SAPP_CMD_MODBUS,pucModbus,iPayLoad);
    if (0 != iRet)
    {
        sp_thread_mutex_unlock( &gCcb.Ipc.mutex );
        
        return -1;
    }

    gettimeofday(&now, NULL);
    outtime.tv_sec  = now.tv_sec + iTime/1000;
    outtime.tv_nsec = (now.tv_usec + (iTime  % 1000)*1000)* 1000;

    gCcb.ModbusCb = cb;

    iRet = sp_thread_cond_timedwait( &gCcb.Ipc.cond, &gCcb.Ipc.mutex ,&outtime);//ylf: thread sleep here
    if(ETIMEDOUT == iRet)
    {
        sp_thread_mutex_unlock( &gCcb.Ipc.mutex );
        if (gCcb.ModbusCb )(gCcb.ModbusCb)(iRet,NULL);
        gCcb.ModbusCb = NULL;
        return -1;
    }

    if (gCcb.ulWorkThdAbort & WORK_THREAD_STATE_BLK_MODBUS)
    {
        sp_thread_mutex_unlock( &gCcb.Ipc.mutex );
        if (gCcb.ModbusCb )(gCcb.ModbusCb)(-1,NULL);
        gCcb.ModbusCb = NULL;
        return -2;
    }

    gCcb.ModbusCb = NULL;
    sp_thread_mutex_unlock( &gCcb.Ipc.mutex );  
    
    return 0; // success
}

int  CcbModbusWorkEntryWrapper(int id,int iChl,unsigned int ulCanId,unsigned char *pucModbus,int iPayLoad,int iTime,modbus_call_back cb)
{
    int iRet;

    sp_thread_mutex_lock( &gCcb.WorkMutex );
    gCcb.ulWorkThdState |= MakeThdState(id,WORK_THREAD_STATE_BLK_MODBUS);
    sp_thread_mutex_unlock( &gCcb.WorkMutex );

    sp_thread_mutex_lock( &gCcb.ModbusMutex );
    iRet = CcbModbusWorkEntry(iChl,ulCanId,pucModbus,iPayLoad,iTime,cb);
    sp_thread_mutex_unlock( &gCcb.ModbusMutex );

    sp_thread_mutex_lock( &gCcb.WorkMutex );
    gCcb.ulWorkThdState &= ~ MakeThdState(id,WORK_THREAD_STATE_BLK_MODBUS);
    gCcb.ulWorkThdAbort &= ~ MakeThdState(id,WORK_THREAD_STATE_BLK_MODBUS);
    sp_thread_mutex_unlock( &gCcb.WorkMutex );

    return iRet;
}

void work_init_run_fail(int iWorkId)
{
    int aiCont[1] = {-1};

    /* close all switchs */
    if (CcbGetSwitchObjState(APP_EXE_SWITCHS_MASK))
    {
        CcbSetSwitch(iWorkId,0,0);
    }
    
    /*stop all report */
    if (CcbGetPmObjState(APP_EXE_PM_MASK)
        || CcbGetIObjState(APP_EXE_I_MASK))
    {
        CcbSetIAndBs(iWorkId,0,0);
        
    }

    if (CcbGetFmObjState(APP_FM_FM_MASK))
    {
        CcbSetFms(iWorkId,0,0);
    }
    
    gCcb.bit1AlarmRej  = FALSE;
    gCcb.bit1AlarmRoPw = FALSE;
    gCcb.bit1AlarmEDI  = FALSE;
    gCcb.bit3RuningState = NOT_RUNING_STATE_NONE;

    MainSndWorkMsg(WORK_MSG_TYPE_INIT_RUN,(unsigned char *)aiCont,sizeof(aiCont));
    
}


void work_init_run_succ(int iWorkId)
{
    int aiCont[1] = {0};

    (void )iWorkId;
    gCcb.bit3RuningState = NOT_RUNING_STATE_NONE;
    
    MainSndWorkMsg(WORK_MSG_TYPE_INIT_RUN,(unsigned char *)aiCont,sizeof(aiCont));

}

void work_run_comm_proc(WORK_ITEM_STRU *pWorkItem,CCB *pCcb,RUN_COMM_CALL_BACK cbf,RUN_COMM_CALL_BACK cbs)
{
    int iRet;
    int iTmp;
    int iLoop;

    pCcb->ulProduceWaterBgnTime  = time(NULL); 

    for (iLoop = 0; iLoop < APP_FM_FLOW_METER_NUM; iLoop++)
    {
        pCcb->FlowMeter.aulPwStart[iLoop] = INVALID_FM_VALUE;
        pCcb->FlowMeter.aulPwEnd[iLoop]   = INVALID_FM_VALUE;
    }

    switch(pCcb->ulMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_L_EDI_LOOP:
        {
            /* get B2 reports from exe */
            iRet = CcbGetPmValue(pWorkItem->id,APP_EXE_PM2_NO,1);
            if (iRet )
            {
                VOS_LOG(VOS_LOG_WARNING,"CcbGetPmValue Fail %d",iRet);    
                /* notify ui (late implemnt) */
                cbf(pWorkItem->id);   
                
                return ;
            }    
        
            if (CcbConvert2Pm2SP(pCcb->ExeBrd.aPMObjs[APP_EXE_PM2_NO].Value.ulV) >= B2_FULL)
            {
                CanPrepare4Pm2Full();

                /* close all switchs */    
                iTmp = 0; 
                iRet = CcbUpdateSwitch(pWorkItem->id,0,pCcb->ulRunMask,iTmp);
                if (iRet )
                {
                    VOS_LOG(VOS_LOG_WARNING,"CcbSetSwitch Fail %d",iRet);
                    
                    cbf(pWorkItem->id);        
                    return ;
                }
                
                /* 1. ui promote */
                cbs(pWorkItem->id);  
                return;
            }       

            /* 2018/01/05 add extra 10 seconds for flush according to ZHANG Chunhe */
#ifdef  SYSTEM_TEST
            iTmp = (1 << APP_EXE_E1_NO)|(1<<APP_EXE_E2_NO)|(1 << APP_EXE_E10_NO)|(1<<APP_EXE_C3_NO);
#else
            /* 2018/01/05 add E3 according to ZHANG */
            if(haveB3(&gCcb))
            {
                iTmp = (1 << APP_EXE_E1_NO)|(1<<APP_EXE_E2_NO)|(1<<APP_EXE_E3_NO)|(1<<APP_EXE_C3_NO);
            }
            else
            {
                iTmp = (1 << APP_EXE_E1_NO)|(1<<APP_EXE_E2_NO)|(1<<APP_EXE_E3_NO)
                        |(1 << APP_EXE_E10_NO)|(1<<APP_EXE_C3_NO);
            }

#endif        
            
            iRet = CcbUpdateSwitch(pWorkItem->id,0,pCcb->ulRunMask,iTmp);
            if (iRet )
            {
                VOS_LOG(VOS_LOG_WARNING,"CcbSetSwitch Fail %d",iRet); 
        
                cbf(pWorkItem->id);        
                
                /* notify ui (late implemnt) */
                return ;
            }

            iRet = CcbWorkDelayEntry(pWorkItem->id,10000,CcbDelayCallBack);
            if (iRet )
            {
                VOS_LOG(VOS_LOG_WARNING,"CcbModbusWorkEntry Fail %d",iRet);  
                
                cbf(pWorkItem->id);        
                
                return;
            }
            
            VOS_LOG(VOS_LOG_WARNING,"E1,C3 ON"); 

            pCcb->bit3RuningState = NOT_RUNING_STATE_CLEAN;
            
            CcbNotState(NOT_STATE_OTHER);
            /* E1,C3 ON*/
            /* set  valves */
            if(haveB3(&gCcb))
            {
                iTmp = (1 << APP_EXE_E1_NO)|(1<<APP_EXE_C3_NO);
            }
            else
            {
                iTmp = (1 << APP_EXE_E1_NO)|(1 << APP_EXE_E10_NO)|(1<<APP_EXE_C3_NO);
            }

            iRet = CcbUpdateSwitch(pWorkItem->id,0,pCcb->ulRunMask,iTmp);
            if (iRet )
            {
                VOS_LOG(VOS_LOG_WARNING,"CcbSetSwitch Fail %d",iRet);    
                /* notify ui (late implemnt) */
                cbf(pWorkItem->id);        
                return ;
            }
            
            for (iLoop = 0; iLoop < 10; iLoop++) //延时测进水压?
            {
                /* get B1 reports from exe */
                iRet = CcbGetPmValue(pWorkItem->id,APP_EXE_PM1_NO,1);
                if (iRet )
                {
                    VOS_LOG(VOS_LOG_WARNING,"CcbGetPmValue Fail %d",iRet);    
                    /* notify ui (late implemnt) */
                    cbf(pWorkItem->id);        
                    return ;
                }    
        
                CcbNotSWPressure();
        
                iRet = CcbWorkDelayEntry(pWorkItem->id,1000,CcbDelayCallBack);
                if (iRet )
                {
                    VOS_LOG(VOS_LOG_WARNING,"CcbModbusWorkEntry Fail %d",iRet);  
                    
                    cbf(pWorkItem->id);        
                    
                    return;
                }
        
                
            }
#if 0   
            if (CcbConvert2Pm1Data(pCcb->ExeBrd.aPMObjs[APP_EXE_PM1_NO].Value.ulV) < CcbGetSp1())
#else
            
             iRet =  CcbGetDinState(0);
             if (iRet )
             {
                 VOS_LOG(VOS_LOG_WARNING,"CcbModbusWorkEntry Fail %d",iRet);  
                 
                 cbf(pWorkItem->id);        
                 
                 return;
             }
             
             VOS_LOG(VOS_LOG_DEBUG,"CcbGetDinState ucDinState %d",gCcb.ExeBrd.ucDinState);
             
             if (!(gCcb.ExeBrd.ucDinState & (1 << APP_EXE_DIN_IWP_KEY)))
#endif            
            {
                /* 1. ui promote */
                iTmp = 0; 
                iRet = CcbUpdateSwitch(pWorkItem->id,0,pCcb->ulRunMask,iTmp);
                if (iRet )
                {
                    VOS_LOG(VOS_LOG_WARNING,"CcbSetSwitch Fail %d",iRet);
        
                    cbf(pWorkItem->id);        
                    return;
                }
        
                /* fire alarm */
                if (!gCcb.bit1AlarmTapInPress)
                {
                    gCcb.bit1AlarmTapInPress   = TRUE;
            
                    gCcb.ulFiredAlarmFlags |= ALARM_TLP;
            
                    CcbNotAlarmFire(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_SOURCE_WATER_PRESSURE,TRUE);
                }

                VOS_LOG(VOS_LOG_WARNING," DISP_WORK_STATE_LPP %d",iRet);  
            
                CanCcbTransState(DISP_WORK_STATE_LPP,DISP_WORK_SUB_IDLE);        
                
                return;
            }

            /* 2018/01/05 begin : add for B1 under pressure check according to ZHANG chunhe */
            gCcb.bit1B1Check4RuningState  = TRUE;  
            /* 2018/01/05 end : add for B1 under pressure check */
            
             /* clear alarm */
            if (gCcb.bit1AlarmTapInPress)
            {
                gCcb.bit1AlarmTapInPress   = FALSE;
           
                gCcb.ulFiredAlarmFlags &= ~ALARM_TLP;
           
                CcbNotAlarmFire(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_SOURCE_WATER_PRESSURE,FALSE);
            }
        
            VOS_LOG(VOS_LOG_WARNING,"E1,E3,C1,C3 ON; I1b,I2,B1,S4"); 
            /*E1,E3,C1,C3 ON; I1b,I2,B1,S4*/
            if(haveB3(&gCcb))
            {
                iTmp = (1 << APP_EXE_E1_NO)|(1<<APP_EXE_E3_NO)|(1<<APP_EXE_C1_NO)|(1<<APP_EXE_C3_NO);
            }
            else
            {
                iTmp = (1 << APP_EXE_E1_NO)|(1<<APP_EXE_E3_NO)|(1<<APP_EXE_C1_NO)
                        |(1 << APP_EXE_E10_NO)|(1<<APP_EXE_C3_NO);
            }
            iRet = CcbUpdateSwitch(pWorkItem->id,0,pCcb->ulRunMask,iTmp);
            if (iRet )
            {
                VOS_LOG(VOS_LOG_WARNING,"CcbModbusWorkEntry Fail %d",iRet);    
                /* notify ui (late implemnt) */
                cbf(pWorkItem->id);        
                return ;
            }
            
            iTmp = (1 << APP_EXE_I1_NO)|(1 << APP_EXE_I2_NO)|(GET_B_MASK(APP_EXE_PM1_NO));
            iRet = CcbUpdateIAndBs(pWorkItem->id,0,iTmp,iTmp);
            if (iRet )
            {
                VOS_LOG(VOS_LOG_WARNING,"CcbSetIAndBs Fail %d",iRet);    
                /* notify ui (late implemnt) */
                cbf(pWorkItem->id);        
                return ;
            }

            iTmp = (1<<APP_FM_FM4_NO);
            iRet = CcbUpdateFms(pWorkItem->id,0,iTmp,iTmp);
            if (iRet )
            {
                VOS_LOG(VOS_LOG_WARNING,"CcbUpdateFms Fail %d",iRet);    
                /* notify ui (late implemnt) */
                cbf(pWorkItem->id);        
                return ;
            }  
            
            iRet = CcbWorkDelayEntry(pWorkItem->id,3000,CcbDelayCallBack);
            if (iRet )
            {
                VOS_LOG(VOS_LOG_WARNING,"CcbModbusWorkEntry Fail %d",iRet);    
                /* notify ui (late implemnt) */
                cbf(pWorkItem->id);        
                return ;
            }
        
            /* check data */
            {
                
                {
                    int iValidCount = 0;
                    
                    float fRej ;
                    
                    /* check appromixly 5*60 seconds */
                    for (iLoop = 0; iLoop < DEFAULT_REG_CHECK_DURATION / DEFAULT_REJ_CHECK_PERIOD; iLoop++)                                    {
                        iRet = CcbWorkDelayEntry(pWorkItem->id,DEFAULT_REJ_CHECK_PERIOD*1000,CcbDelayCallBack);
                        if (iRet )
                        {
                            VOS_LOG(VOS_LOG_WARNING,"CcbModbusWorkEntry Fail %d",iRet);    
                            /* notify ui (late implemnt) */
                            cbf(pWorkItem->id);                
                            return ;
                        }  
                
                        fRej = CcbCalcREJ();
                
                        if (fRej >= CcbGetSp2() 
                            && gCcb.ExeBrd.aEcoObjs[APP_EXE_I2_NO].Value.eV.fWaterQ < CcbGetSp3())
                        {
                            iValidCount ++;
                
                            if (iValidCount >= DEFAULT_REJ_CHECK_NUMBER)
                            {
                                break;
                            }
                        }
                        else
                        {
                            iValidCount = 0;
                        }
                    }
                
                    if (fRej < CcbGetSp2())
                    {
                        /*alarem */
                        gCcb.bit1AlarmRej = TRUE;
                        gCcb.ulAlarmRejTick = gulSecond;
                    }
                
                    if (gCcb.ExeBrd.aEcoObjs[APP_EXE_I2_NO].Value.eV.fWaterQ > CcbGetSp3())
                    {
                        gCcb.bit1AlarmRoPw   = TRUE;
                        gCcb.ulAlarmRoPwTick = gulSecond;
                    }
                    
                }

                pCcb->bit3RuningState = NOT_RUNING_STATE_NONE;
                CcbNotState(NOT_STATE_OTHER);
                
                VOS_LOG(VOS_LOG_WARNING,"E1,C1,C3,T,N1 ON I1b,I2,I3,B1,B2,S2,S3,S4"); 
                /* produce water */
                /* E1,C1,C3,T,N1 ON I1b,I2,I3,B1,B2,S2,S3,S4 */
                if(haveB3(&gCcb))
                {
                    iTmp = (1 << APP_EXE_E1_NO)|(1<<APP_EXE_C1_NO)|(1<<APP_EXE_C3_NO);
                    iTmp |=(1 << APP_EXE_T1_NO)|(1 << APP_EXE_N1_NO);
                }
                else
                {
                    iTmp = (1 << APP_EXE_E1_NO)|(1<<APP_EXE_C1_NO)|(1 << APP_EXE_E10_NO)|(1<<APP_EXE_C3_NO);
                    iTmp |=(1 << APP_EXE_T1_NO)|(1 << APP_EXE_N1_NO);
                }

                iRet = CcbUpdateSwitch(pWorkItem->id,0,pCcb->ulRunMask,iTmp);
                if (iRet )
                {
                    VOS_LOG(VOS_LOG_WARNING,"CcbSetSwitch Fail %d",iRet);    
                    /* notify ui (late implemnt) */
                    cbf(pWorkItem->id);        
                    return ;
                }

                iTmp  = (1 << APP_EXE_I1_NO)|(1<<APP_EXE_I2_NO)|(1<<APP_EXE_I3_NO);
                iTmp |= (GET_B_MASK(APP_EXE_PM1_NO))|(GET_B_MASK(APP_EXE_PM2_NO));
                iRet = CcbUpdateIAndBs(pWorkItem->id,0,iTmp,iTmp);
                if (iRet )
                {
                    VOS_LOG(VOS_LOG_WARNING,"CcbSetIAndBs Fail %d",iRet);    
                    /* notify ui (late implemnt) */
                    cbf(pWorkItem->id);        
                    return ;
                }       
         
                iTmp  = (1 << APP_FM_FM2_NO)|(1<<APP_FM_FM3_NO)|(1<<APP_FM_FM4_NO);
                iRet = CcbUpdateFms(pWorkItem->id,0,iTmp,iTmp);
                if (iRet )
                {
                    VOS_LOG(VOS_LOG_WARNING,"CcbUpdateFms Fail %d",iRet);    
                    /* notify ui (late implemnt) */
                    cbf(pWorkItem->id);        
                    return ;
                }  
        
                /* wait a moment for I3 check */
                iRet = CcbWorkDelayEntry(pWorkItem->id,3000,CcbDelayCallBack); 
                if (iRet )
                {
                    VOS_LOG(VOS_LOG_WARNING,"CcbModbusWorkEntry Fail %d",iRet);  
                    
                    /* notify ui (late implemnt) */
                    cbf(pWorkItem->id);   
                    
                    return ;
                }  
        
                if (!(gCcb.ExeBrd.aEcoObjs[APP_EXE_I3_NO].Value.eV.fWaterQ >= CcbGetSp4()))
                {
                    /*alarem */
                    gCcb.bit1AlarmEDI   = TRUE;
                    gCcb.ulAlarmEDITick = gulSecond;
                }
            }    
        }
        break;
    case MACHINE_L_UP:
    case MACHINE_L_RO_LOOP:
         {
             /* get B2 reports from exe */
             iRet = CcbGetPmValue(pWorkItem->id,APP_EXE_PM2_NO,1);
             if (iRet )
             {
                 VOS_LOG(VOS_LOG_WARNING,"CcbGetPmValue Fail %d",iRet);    
                 /* notify ui (late implemnt) */
                 cbf(pWorkItem->id);   
                 
                 return ;
             }    
         
             if (CcbConvert2Pm2SP(pCcb->ExeBrd.aPMObjs[APP_EXE_PM2_NO].Value.ulV) >= B2_FULL)
             {
                 CanPrepare4Pm2Full();
                 
                 /* close all switchs */    
                 iTmp = 0; 
                 iRet = CcbUpdateSwitch(pWorkItem->id,0,pCcb->ulRunMask,iTmp);
                 if (iRet )
                 {
                     VOS_LOG(VOS_LOG_WARNING,"CcbSetSwitch Fail %d",iRet);
                     
                     cbf(pWorkItem->id);        
                     return ;
                 }
                 
                 /* 1. ui promote */
                 cbs(pWorkItem->id);        
                 return;
             }        

            /* 2018/01/05 add extra 10 seconds for flush according to ZHANG Chunhe */
#ifdef  SYSTEM_TEST
            iTmp = (1 << APP_EXE_E1_NO)|(1<<APP_EXE_E2_NO)|(1 << APP_EXE_E10_NO)|(1<<APP_EXE_C3_NO);
#else
            /* 2018/01/05 add E3 according to ZHANG */
            if(haveB3(&gCcb))
            {
                iTmp = (1 << APP_EXE_E1_NO)|(1<<APP_EXE_E2_NO)|(1<<APP_EXE_E3_NO)|(1<<APP_EXE_C3_NO);
            }
            else
            {
                iTmp = (1 << APP_EXE_E1_NO)|(1<<APP_EXE_E2_NO)|(1<<APP_EXE_E3_NO)
                        |(1 << APP_EXE_E10_NO)|(1<<APP_EXE_C3_NO);
            }
#endif        
            
            iRet = CcbUpdateSwitch(pWorkItem->id,0,pCcb->ulRunMask,iTmp);
            if (iRet )
            {
                VOS_LOG(VOS_LOG_WARNING,"CcbSetSwitch Fail %d",iRet); 
        
                cbf(pWorkItem->id);        
                
                /* notify ui (late implemnt) */
                return ;
            }

            iRet = CcbWorkDelayEntry(pWorkItem->id,10000,CcbDelayCallBack);
            if (iRet )
            {
                VOS_LOG(VOS_LOG_WARNING,"CcbModbusWorkEntry Fail %d",iRet);  
                
                cbf(pWorkItem->id);        
                
                return;
            }
            pCcb->bit3RuningState = NOT_RUNING_STATE_CLEAN;

            CcbNotState(NOT_STATE_OTHER);
            
            VOS_LOG(VOS_LOG_WARNING,"E1,C3 ON"); 
        
            /* E1,C3 ON*/
            /* set  valves */
            if(haveB3(&gCcb))
            {
                iTmp = (1 << APP_EXE_E1_NO)|(1<<APP_EXE_C3_NO);
            }
            else
            {
                iTmp = (1 << APP_EXE_E1_NO)|(1 << APP_EXE_E10_NO)|(1<<APP_EXE_C3_NO);
            }

            iRet = CcbUpdateSwitch(pWorkItem->id,0,pCcb->ulRunMask,iTmp);
            if (iRet )
            {
                VOS_LOG(VOS_LOG_WARNING,"CcbSetSwitch Fail %d",iRet);    
                /* notify ui (late implemnt) */
                cbf(pWorkItem->id);        
                return ;
            }
            
            for (iLoop = 0; iLoop < 10; iLoop++)
            {
                /* get B1 reports from exe */
                iRet = CcbGetPmValue(pWorkItem->id,APP_EXE_PM1_NO,1);
                if (iRet )
                {
                    VOS_LOG(VOS_LOG_WARNING,"CcbGetPmValue Fail %d",iRet);    
                    /* notify ui (late implemnt) */
                    cbf(pWorkItem->id);        
                    return ;
                }    
        
                CcbNotSWPressure();
        
        
                iRet = CcbWorkDelayEntry(pWorkItem->id,1000,CcbDelayCallBack);
                if (iRet )
                {
                    VOS_LOG(VOS_LOG_WARNING,"CcbModbusWorkEntry Fail %d",iRet);  
                    
                    cbf(pWorkItem->id);        
                    
                    return;
                }
        
                
            }
        
            if (CcbConvert2Pm1Data(pCcb->ExeBrd.aPMObjs[APP_EXE_PM1_NO].Value.ulV) < CcbGetSp1())
            {
                /* 1. ui promote */
                iTmp = 0; 
                iRet = CcbUpdateSwitch(pWorkItem->id,0,pCcb->ulRunMask,iTmp);
                if (iRet )
                {
                    VOS_LOG(VOS_LOG_WARNING,"CcbSetSwitch Fail %d",iRet);
        
                    cbf(pWorkItem->id);        
                    return;
                }
        
                /* fire alarm */
                if (!gCcb.bit1AlarmTapInPress)
                {
                    gCcb.bit1AlarmTapInPress   = TRUE;
            
                    gCcb.ulFiredAlarmFlags |= ALARM_TLP;
            
                    CcbNotAlarmFire(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_SOURCE_WATER_PRESSURE,TRUE);
                }
            
                CanCcbTransState(DISP_WORK_STATE_LPP,DISP_WORK_SUB_IDLE);        
                
                return;
            }

             /* 2018/01/05 begin : add for B1 under pressure check according to ZHANG chunhe */
             gCcb.bit1B1Check4RuningState  = TRUE;  
             /* 2018/01/05 end : add for B1 under pressure check */
        
             /* clear alarm */
            if (gCcb.bit1AlarmTapInPress)
            {
                gCcb.bit1AlarmTapInPress   = FALSE;
           
                gCcb.ulFiredAlarmFlags &= ~ALARM_TLP;
           
                CcbNotAlarmFire(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_SOURCE_WATER_PRESSURE,FALSE);
            }
        
            VOS_LOG(VOS_LOG_WARNING,"E1,E3,C1,C3 ON; I1b,I2,B1,S4"); 
        
            /*E1,E3,C1,C3 ON; I1b,I2,B1,S4*/
            if(haveB3(&gCcb))
            {
                iTmp = (1 << APP_EXE_E1_NO)|(1<<APP_EXE_E3_NO)|(1<<APP_EXE_C1_NO)|(1<<APP_EXE_C3_NO);
            }
            else
            {
                iTmp = (1 << APP_EXE_E1_NO)|(1<<APP_EXE_E3_NO)|(1<<APP_EXE_C1_NO)
                        |(1 << APP_EXE_E10_NO)|(1<<APP_EXE_C3_NO);
            }
            iRet = CcbUpdateSwitch(pWorkItem->id,0,pCcb->ulRunMask,iTmp);
            if (iRet )
            {
                VOS_LOG(VOS_LOG_WARNING,"CcbModbusWorkEntry Fail %d",iRet);    
                /* notify ui (late implemnt) */
                cbf(pWorkItem->id);        
                return ;
            }
            
            iTmp = (1 << APP_EXE_I1_NO)|(1 << APP_EXE_I2_NO)|(GET_B_MASK(APP_EXE_PM1_NO));
            iRet = CcbUpdateIAndBs(pWorkItem->id,0,iTmp,iTmp);
            if (iRet )
            {
                VOS_LOG(VOS_LOG_WARNING,"CcbModbusWorkEntry Fail %d",iRet);    
                /* notify ui (late implemnt) */
                cbf(pWorkItem->id);        
                return ;
            }

            iTmp = (1<<APP_FM_FM4_NO);
            iRet = CcbUpdateFms(pWorkItem->id,0,iTmp,iTmp);
            if (iRet )
            {
                VOS_LOG(VOS_LOG_WARNING,"CcbUpdateFms Fail %d",iRet);    
                /* notify ui (late implemnt) */
                cbf(pWorkItem->id);        
                return ;
            } 
            
            iRet = CcbWorkDelayEntry(pWorkItem->id,3000,CcbDelayCallBack);
            if (iRet )
            {
                VOS_LOG(VOS_LOG_WARNING,"CcbModbusWorkEntry Fail %d",iRet);    
                /* notify ui (late implemnt) */
                cbf(pWorkItem->id);        
                return ;
            }
        
            /* check data */
            {
                {
                    int iValidCount = 0;
                    
                    float fRej ;
                    
                    /* check appromixly 5*60 seconds */
                    for (iLoop = 0; iLoop < DEFAULT_REG_CHECK_DURATION / DEFAULT_REJ_CHECK_PERIOD; iLoop++)                                    {
                        iRet = CcbWorkDelayEntry(pWorkItem->id,DEFAULT_REJ_CHECK_PERIOD*1000,CcbDelayCallBack);
                        if (iRet )
                        {
                            VOS_LOG(VOS_LOG_WARNING,"CcbModbusWorkEntry Fail %d",iRet);    
                            /* notify ui (late implemnt) */
                            cbf(pWorkItem->id);                
                            return ;
                        }  
                
                        fRej = CcbCalcREJ();
                
                        if (fRej >= CcbGetSp2() 
                            && gCcb.ExeBrd.aEcoObjs[APP_EXE_I2_NO].Value.eV.fWaterQ < CcbGetSp3())
                        {
                            iValidCount ++;
                
                            if (iValidCount >= DEFAULT_REJ_CHECK_NUMBER)
                            {
                                break;
                            }
                        }
                        else
                        {
                            iValidCount = 0;
                        }
                    }
                
                    if (fRej < CcbGetSp2())
                    {
                        /*alarem */
                        gCcb.bit1AlarmRej = TRUE;
                        gCcb.ulAlarmRejTick = gulSecond;
                    }
                
                    if (gCcb.ExeBrd.aEcoObjs[APP_EXE_I2_NO].Value.eV.fWaterQ > CcbGetSp3())
                    {
                        gCcb.bit1AlarmRoPw   = TRUE;
                        gCcb.ulAlarmRoPwTick = gulSecond;
                    }
                    
                }

                pCcb->bit3RuningState = NOT_RUNING_STATE_NONE;
                CcbNotState(NOT_STATE_OTHER);
        
                VOS_LOG(VOS_LOG_WARNING,"E1,C1,C3 ON I1b,I2,B1,B2,S2,S3,S4"); 
                /* produce water */
                /* E1,C1,C3 ON I1b,I2,B1,B2,S2,S3,S4 */
                if(haveB3(&gCcb))
                {
                    iTmp = (1 << APP_EXE_E1_NO)|(1<<APP_EXE_C1_NO)|(1<<APP_EXE_C3_NO);
                }
                else
                {
                    iTmp = (1 << APP_EXE_E1_NO)|(1<<APP_EXE_C1_NO)|(1 << APP_EXE_E10_NO)|(1<<APP_EXE_C3_NO);
                }

                iRet = CcbUpdateSwitch(pWorkItem->id,0,pCcb->ulRunMask,iTmp);
                if (iRet )
                {
                    VOS_LOG(VOS_LOG_WARNING,"CcbSetSwitch Fail %d",iRet);    
                    /* notify ui (late implemnt) */
                    cbf(pWorkItem->id);        
                    return ;
                }
         
                iTmp  = (1 << APP_EXE_I1_NO)|(1<<APP_EXE_I2_NO);
                iTmp |= (GET_B_MASK(APP_EXE_PM1_NO))|(GET_B_MASK(APP_EXE_PM2_NO));
                iRet = CcbUpdateIAndBs(pWorkItem->id,0,iTmp,iTmp);
                if (iRet )
                {
                    VOS_LOG(VOS_LOG_WARNING,"CcbSetIAndBs Fail %d",iRet);    
                    /* notify ui (late implemnt) */
                    cbf(pWorkItem->id);        
                    return ;
                }      
         
                iTmp  = (1 << APP_FM_FM2_NO)|(1<<APP_FM_FM3_NO)|(1<<APP_FM_FM4_NO);
                iRet = CcbUpdateFms(pWorkItem->id,0,iTmp,iTmp);
                if (iRet )
                {
                    VOS_LOG(VOS_LOG_WARNING,"CcbUpdateFms Fail %d",iRet);    
                    /* notify ui (late implemnt) */
                    cbf(pWorkItem->id);        
                    return ;
                }  
            }  
        }
        break;
    case MACHINE_Genie:
    case MACHINE_EDI:
        {
            /* get B2 reports from exe */
            iRet = CcbGetPmValue(pWorkItem->id,APP_EXE_PM2_NO,1);
            if (iRet )
            {
                VOS_LOG(VOS_LOG_WARNING,"CcbGetPmValue Fail %d",iRet);    
                /* notify ui (late implemnt) */
                cbf(pWorkItem->id);   
                
                return ;
            }    
        
            if (CcbConvert2Pm2SP(pCcb->ExeBrd.aPMObjs[APP_EXE_PM2_NO].Value.ulV) >= B2_FULL)
            {
                CanPrepare4Pm2Full();
                
                /* close all switchs */    
                iTmp = 0; 
                iRet = CcbUpdateSwitch(pWorkItem->id,0,pCcb->ulRunMask,iTmp);
                if (iRet )
                {
                    VOS_LOG(VOS_LOG_WARNING,"CcbSetSwitch Fail %d",iRet);
                    
                    cbf(pWorkItem->id);        
                    return ;
                }
                
                /* 1. ui promote */
                cbs(pWorkItem->id);        
                return;
            }     

            /* 2018/01/05 add extra 10 seconds for flush according to ZHANG Chunhe */
#ifdef  SYSTEM_TEST
            iTmp = (1 << APP_EXE_E1_NO)|(1<<APP_EXE_E2_NO)|(1 << APP_EXE_E10_NO)|(1<<APP_EXE_C3_NO);
#else
            /* 2018/01/05 add E3 according to ZHANG */
            if(haveB3(&gCcb))
            {
                iTmp = (1 << APP_EXE_E1_NO)|(1<<APP_EXE_E2_NO)|(1<<APP_EXE_E3_NO)|(1<<APP_EXE_C3_NO);
            }
            else
            {
                iTmp = (1 << APP_EXE_E1_NO)|(1<<APP_EXE_E2_NO)|(1<<APP_EXE_E3_NO)
                        |(1 << APP_EXE_E10_NO)|(1<<APP_EXE_C3_NO);
            }

#endif        
            
            iRet = CcbUpdateSwitch(pWorkItem->id,0,pCcb->ulRunMask,iTmp);
            if (iRet )
            {
                VOS_LOG(VOS_LOG_WARNING,"CcbSetSwitch Fail %d",iRet); 
        
                cbf(pWorkItem->id);        
                
                /* notify ui (late implemnt) */
                return ;
            }

            iRet = CcbWorkDelayEntry(pWorkItem->id,10000,CcbDelayCallBack);
            if (iRet )
            {
                VOS_LOG(VOS_LOG_WARNING,"CcbModbusWorkEntry Fail %d",iRet);  
                
                cbf(pWorkItem->id);        
                
                return;
            }

            pCcb->bit3RuningState = NOT_RUNING_STATE_CLEAN;
            CcbNotState(NOT_STATE_OTHER);
                        
            /* E1,E3 ON*/
            VOS_LOG(VOS_LOG_WARNING,"E1,E3 ON"); 
            
            /* set  valves */
            if(haveB3(&gCcb))
            {
                iTmp = (1 << APP_EXE_E1_NO)|(1<<APP_EXE_E3_NO)|(1<<APP_EXE_C3_NO);
            }
            else
            {
                iTmp = (1 << APP_EXE_E1_NO)|(1<<APP_EXE_E3_NO)|(1 << APP_EXE_E10_NO)|(1 << APP_EXE_C3_NO);
            }

            iRet = CcbUpdateSwitch(pWorkItem->id,0,pCcb->ulRunMask,iTmp);
            if (iRet )
            {
                VOS_LOG(VOS_LOG_WARNING,"CcbSetSwitch Fail %d",iRet);    
                /* notify ui (late implemnt) */
                cbf(pWorkItem->id);        
                return ;
            }
            
            for (iLoop = 0; iLoop < 10; iLoop++)
            {
                /* get B1 reports from exe */
                iRet = CcbGetPmValue(pWorkItem->id,APP_EXE_PM1_NO,1);
                if (iRet )
                {
                    VOS_LOG(VOS_LOG_WARNING,"CcbGetPmValue Fail %d",iRet);    
                    /* notify ui (late implemnt) */
                    cbf(pWorkItem->id);        
                    return ;
                }    
        
                CcbNotSWPressure();
        
        
                iRet = CcbWorkDelayEntry(pWorkItem->id,1000,CcbDelayCallBack);
                if (iRet )
                {
                    VOS_LOG(VOS_LOG_WARNING,"CcbModbusWorkEntry Fail %d",iRet);  
                    
                    cbf(pWorkItem->id);        
                    
                    return;
                }
        
                
            }
        
            if (CcbConvert2Pm1Data(pCcb->ExeBrd.aPMObjs[APP_EXE_PM1_NO].Value.ulV) < CcbGetSp1())
            {
                /* 1. ui promote */
                iTmp = 0; 
                iRet = CcbUpdateSwitch(pWorkItem->id,0,pCcb->ulRunMask,iTmp);
                if (iRet )
                {
                    VOS_LOG(VOS_LOG_WARNING,"CcbSetSwitch Fail %d",iRet);
        
                    cbf(pWorkItem->id);        
                    return;
                }
        
                /* fire alarm */
                if (!gCcb.bit1AlarmTapInPress)
                {
                    gCcb.bit1AlarmTapInPress   = TRUE;
            
                    gCcb.ulFiredAlarmFlags |= ALARM_TLP;
            
                    CcbNotAlarmFire(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_SOURCE_WATER_PRESSURE,TRUE);
                }
            
                CanCcbTransState(DISP_WORK_STATE_LPP,DISP_WORK_SUB_IDLE);        
                
                return;
            }

             /* 2018/01/05 begin : add for B1 under pressure check according to ZHANG chunhe */
             gCcb.bit1B1Check4RuningState  = TRUE;  
             /* 2018/01/05 end : add for B1 under pressure check */
        
             /* clear alarm */
            if (gCcb.bit1AlarmTapInPress)
            {
                gCcb.bit1AlarmTapInPress   = FALSE;
           
                gCcb.ulFiredAlarmFlags &= ~ALARM_TLP;
           
                CcbNotAlarmFire(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_SOURCE_WATER_PRESSURE,FALSE);
            }
        
            VOS_LOG(VOS_LOG_WARNING,"E1,E3,C1 ON; I1b,I2,B1"); 
        
            /*E1,E3,C1 ON; I1b,I2,B1*/
            if(haveB3(&gCcb))
            {
                iTmp = (1 << APP_EXE_E1_NO)|(1<<APP_EXE_E3_NO)|(1<<APP_EXE_C1_NO)|(1<<APP_EXE_C3_NO);
            }
            else
            {
                iTmp = (1 << APP_EXE_E1_NO)|(1<<APP_EXE_E3_NO)|(1<<APP_EXE_C1_NO)|(1 << APP_EXE_E10_NO)|(1 << APP_EXE_C3_NO);
            }
            iRet = CcbUpdateSwitch(pWorkItem->id,0,pCcb->ulRunMask,iTmp);
            if (iRet )
            {
                VOS_LOG(VOS_LOG_WARNING,"CcbModbusWorkEntry Fail %d",iRet);    
                /* notify ui (late implemnt) */
                cbf(pWorkItem->id);        
                return ;
            }
            
            iTmp = (1 << APP_EXE_I1_NO)|(1 << APP_EXE_I2_NO)|(GET_B_MASK(APP_EXE_PM1_NO));
            iRet = CcbUpdateIAndBs(pWorkItem->id,0,iTmp,iTmp);
            if (iRet )
            {
                VOS_LOG(VOS_LOG_WARNING,"CcbModbusWorkEntry Fail %d",iRet);    
                /* notify ui (late implemnt) */
                cbf(pWorkItem->id);        
                return ;
            }
            
            iRet = CcbWorkDelayEntry(pWorkItem->id,3000,CcbDelayCallBack);
            if (iRet )
            {
                VOS_LOG(VOS_LOG_WARNING,"CcbModbusWorkEntry Fail %d",iRet);    
                /* notify ui (late implemnt) */
                cbf(pWorkItem->id);        
                return ;
            }
        
            /* check data */
            {
                {
                    int iValidCount = 0;
                    
                    float fRej ;
                    
                    /* check appromixly 5*60 seconds */
                    for (iLoop = 0; iLoop < DEFAULT_REG_CHECK_DURATION / DEFAULT_REJ_CHECK_PERIOD; iLoop++)                                    {
                        iRet = CcbWorkDelayEntry(pWorkItem->id,DEFAULT_REJ_CHECK_PERIOD*1000,CcbDelayCallBack);
                        if (iRet )
                        {
                            VOS_LOG(VOS_LOG_WARNING,"CcbModbusWorkEntry Fail %d",iRet);    
                            /* notify ui (late implemnt) */
                            cbf(pWorkItem->id);                
                            return ;
                        }  
                
                        fRej = CcbCalcREJ();
                
                        if (fRej >= CcbGetSp2() 
                            && gCcb.ExeBrd.aEcoObjs[APP_EXE_I2_NO].Value.eV.fWaterQ < CcbGetSp3())
                        {
                            iValidCount ++;
                
                            if (iValidCount >= DEFAULT_REJ_CHECK_NUMBER)
                            {
                                break;
                            }
                        }
                        else
                        {
                            iValidCount = 0;
                        }
                    }
                
                    if (fRej < CcbGetSp2())
                    {
                        /*alarem */
                        gCcb.bit1AlarmRej = TRUE;
                        gCcb.ulAlarmRejTick = gulSecond;
                    }
                
                    if (gCcb.ExeBrd.aEcoObjs[APP_EXE_I2_NO].Value.eV.fWaterQ > CcbGetSp3())
                    {
                        gCcb.bit1AlarmRoPw   = TRUE;
                        gCcb.ulAlarmRoPwTick = gulSecond;
                    }
                }

                pCcb->bit3RuningState = NOT_RUNING_STATE_NONE;
                CcbNotState(NOT_STATE_OTHER);
        
                VOS_LOG(VOS_LOG_WARNING,"E1,C1,T,N1 ON I1b,I2,I3,B1,B2,S2"); 
                /* produce water */
                /* E1,C1,T,N1 ON I1b,I2,I3,B1,B2 */
                if(haveB3(&gCcb))
                {
                    iTmp = (1 << APP_EXE_E1_NO)|(1<<APP_EXE_C1_NO)|(1<<APP_EXE_C3_NO);
                }
                else
                {
                    iTmp = (1 << APP_EXE_E1_NO)|(1<<APP_EXE_C1_NO)|(1 << APP_EXE_E10_NO)|(1<<APP_EXE_C3_NO);
                }

                iTmp |=(1 << APP_EXE_T1_NO)|(1 << APP_EXE_N1_NO); 
                iRet = CcbUpdateSwitch(pWorkItem->id,0,pCcb->ulRunMask,iTmp);
                if (iRet )
                {
                    VOS_LOG(VOS_LOG_WARNING,"CcbSetSwitch Fail %d",iRet);    
                    /* notify ui (late implemnt) */
                    cbf(pWorkItem->id);        
                    return ;
                }
         
                iTmp  = (1 << APP_EXE_I1_NO)|(1<<APP_EXE_I2_NO)|(1<<APP_EXE_I3_NO);
                iTmp |= (GET_B_MASK(APP_EXE_PM1_NO))|(GET_B_MASK(APP_EXE_PM2_NO));
                iRet = CcbUpdateIAndBs(pWorkItem->id,0,iTmp,iTmp);
                if (iRet )
                {
                    VOS_LOG(VOS_LOG_WARNING,"CcbSetIAndBs Fail %d",iRet);    
                    /* notify ui (late implemnt) */
                    cbf(pWorkItem->id);        
                    return ;
                }      
         
                /* wait a moment for I3 check */
                iRet = CcbWorkDelayEntry(pWorkItem->id,3000,CcbDelayCallBack); 
                if (iRet )
                {
                    VOS_LOG(VOS_LOG_WARNING,"CcbModbusWorkEntry Fail %d",iRet);  
                    
                    /* notify ui (late implemnt) */
                    cbf(pWorkItem->id);   
                    
                    return ;
                }  
        
                if (!(gCcb.ExeBrd.aEcoObjs[APP_EXE_I3_NO].Value.eV.fWaterQ >= CcbGetSp4()))
                {
                    /*alarem */
                    gCcb.bit1AlarmEDI   = TRUE;
                    gCcb.ulAlarmEDITick = gulSecond;
                }
            }    
        }
        break;
    case MACHINE_UP:
    case MACHINE_RO:    
        /* get B2 reports from exe */
        iRet = CcbGetPmValue(pWorkItem->id,APP_EXE_PM2_NO,1);
        if (iRet )
        {
            VOS_LOG(VOS_LOG_WARNING,"CcbGetPmValue Fail %d",iRet);    
            /* notify ui (late implemnt) */
            cbf(pWorkItem->id);   
            
            return ;
        }    
    
        if (CcbConvert2Pm2SP(pCcb->ExeBrd.aPMObjs[APP_EXE_PM2_NO].Value.ulV) >= B2_FULL)
        {
            CanPrepare4Pm2Full();
            
            /* close all switchs */    
            iTmp = 0; 
            iRet = CcbUpdateSwitch(pWorkItem->id,0,pCcb->ulRunMask,iTmp);
            if (iRet )
            {
                VOS_LOG(VOS_LOG_WARNING,"CcbSetSwitch Fail %d",iRet);
                
                cbf(pWorkItem->id);        
                return ;
            }
            
            /* 1. ui promote */
            cbs(pWorkItem->id);        
            return;
        }    

        /* 2018/01/05 add extra 10 seconds for flush according to ZHANG Chunhe */
#ifdef  SYSTEM_TEST
        iTmp = (1 << APP_EXE_E1_NO)|(1<<APP_EXE_E2_NO)|(1 << APP_EXE_E10_NO)|(1<<APP_EXE_C3_NO);
#else
        /* 2018/01/05 add E3 according to ZHANG */
        if(haveB3(&gCcb))
        {
            iTmp = (1 << APP_EXE_E1_NO)|(1<<APP_EXE_E2_NO)|(1<<APP_EXE_E3_NO)|(1<<APP_EXE_C3_NO);
        }
        else
        {
            iTmp = (1 << APP_EXE_E1_NO)|(1<<APP_EXE_E2_NO)|(1<<APP_EXE_E3_NO)
                    |(1 << APP_EXE_E10_NO)|(1<<APP_EXE_C3_NO);
        }

#endif        
        
        iRet = CcbUpdateSwitch(pWorkItem->id,0,pCcb->ulRunMask,iTmp);
        if (iRet )
        {
            VOS_LOG(VOS_LOG_WARNING,"CcbSetSwitch Fail %d",iRet); 
    
            cbf(pWorkItem->id);        
            
            /* notify ui (late implemnt) */
            return ;
        }

        iRet = CcbWorkDelayEntry(pWorkItem->id,10000,CcbDelayCallBack);
        if (iRet )
        {
            VOS_LOG(VOS_LOG_WARNING,"CcbModbusWorkEntry Fail %d",iRet);  
            
            cbf(pWorkItem->id);        
            
            return;
        }
        pCcb->bit3RuningState = NOT_RUNING_STATE_CLEAN;
        
        CcbNotState(NOT_STATE_OTHER);
                
        /* E1,C3 ON*/
        VOS_LOG(VOS_LOG_WARNING,"E1,E3 ON"); 
        
        /* set  valves */
        if(haveB3(&gCcb))
        {
            iTmp = (1 << APP_EXE_E1_NO)|(1<<APP_EXE_E3_NO)|(1<<APP_EXE_C3_NO);
        }
        else
        {
            iTmp = (1 << APP_EXE_E1_NO)|(1<<APP_EXE_E3_NO)|(1 << APP_EXE_E10_NO)|(1 << APP_EXE_C3_NO);
        }
        iRet = CcbUpdateSwitch(pWorkItem->id,0,pCcb->ulRunMask,iTmp);
        if (iRet )
        {
            VOS_LOG(VOS_LOG_WARNING,"CcbSetSwitch Fail %d",iRet);    
            /* notify ui (late implemnt) */
            cbf(pWorkItem->id);        
            return ;
        }
        
        for (iLoop = 0; iLoop < 10; iLoop++)
        {
            /* get B1 reports from exe */
            iRet = CcbGetPmValue(pWorkItem->id,APP_EXE_PM1_NO,1);
            if (iRet )
            {
                VOS_LOG(VOS_LOG_WARNING,"CcbGetPmValue Fail %d",iRet);    
                /* notify ui (late implemnt) */
                cbf(pWorkItem->id);        
                return ;
            }    
    
            CcbNotSWPressure();
    
    
            iRet = CcbWorkDelayEntry(pWorkItem->id,1000,CcbDelayCallBack);
            if (iRet )
            {
                VOS_LOG(VOS_LOG_WARNING,"CcbModbusWorkEntry Fail %d",iRet);  
                
                cbf(pWorkItem->id);        
                
                return;
            }
    
            
        }
    
        if (CcbConvert2Pm1Data(pCcb->ExeBrd.aPMObjs[APP_EXE_PM1_NO].Value.ulV) < CcbGetSp1())
        {
            /* 1. ui promote */
            iTmp = 0; 
            iRet = CcbUpdateSwitch(pWorkItem->id,0,pCcb->ulRunMask,iTmp);
            if (iRet )
            {
                VOS_LOG(VOS_LOG_WARNING,"CcbSetSwitch Fail %d",iRet);
    
                cbf(pWorkItem->id);        
                return;
            }
    
            /* fire alarm */
            if (!gCcb.bit1AlarmTapInPress)
            {
                gCcb.bit1AlarmTapInPress   = TRUE;
        
                gCcb.ulFiredAlarmFlags |= ALARM_TLP;
        
                CcbNotAlarmFire(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_SOURCE_WATER_PRESSURE,TRUE);
            }
        
            CanCcbTransState(DISP_WORK_STATE_LPP,DISP_WORK_SUB_IDLE);        
            
            return;
        }

         /* 2018/01/05 begin : add for B1 under pressure check according to ZHANG chunhe */
         gCcb.bit1B1Check4RuningState  = TRUE;  
         /* 2018/01/05 end : add for B1 under pressure check */
    
         /* clear alarm */
        if (gCcb.bit1AlarmTapInPress)
        {
            gCcb.bit1AlarmTapInPress   = FALSE;
       
            gCcb.ulFiredAlarmFlags &= ~ALARM_TLP;
       
            CcbNotAlarmFire(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_SOURCE_WATER_PRESSURE,FALSE);
        }
    
        VOS_LOG(VOS_LOG_WARNING,"E1,E3,C1 ON; I1b,I2,B1"); 
    
        /*E1,E3,C1 ON; I1b,I2,B1*/
        if(haveB3(&gCcb))
        {
            iTmp = (1 << APP_EXE_E1_NO)|(1<<APP_EXE_E3_NO)|(1<<APP_EXE_C1_NO)|(1<<APP_EXE_C3_NO);
        }
        else
        {
            iTmp = (1 << APP_EXE_E1_NO)|(1<<APP_EXE_E3_NO)|(1<<APP_EXE_C1_NO)
                    |(1 << APP_EXE_E10_NO)|(1 << APP_EXE_C3_NO);
        }
        iRet = CcbUpdateSwitch(pWorkItem->id,0,pCcb->ulRunMask,iTmp);
        if (iRet )
        {
            VOS_LOG(VOS_LOG_WARNING,"CcbModbusWorkEntry Fail %d",iRet);    
            /* notify ui (late implemnt) */
            cbf(pWorkItem->id);        
            return ;
        }
        
        iTmp = (1 << APP_EXE_I1_NO)|(1 << APP_EXE_I2_NO)|(GET_B_MASK(APP_EXE_PM1_NO));
        iRet = CcbUpdateIAndBs(pWorkItem->id,0,iTmp,iTmp);
        if (iRet )
        {
            VOS_LOG(VOS_LOG_WARNING,"CcbModbusWorkEntry Fail %d",iRet);    
            /* notify ui (late implemnt) */
            cbf(pWorkItem->id);        
            return ;
        }
        
        iRet = CcbWorkDelayEntry(pWorkItem->id,3000,CcbDelayCallBack);
        if (iRet )
        {
            VOS_LOG(VOS_LOG_WARNING,"CcbModbusWorkEntry Fail %d",iRet);    
            /* notify ui (late implemnt) */
            cbf(pWorkItem->id);        
            return ;
        }
    
        /* check data */
        {
            {
                int iValidCount = 0;
                
                float fRej ;
                
                /* check appromixly 5*60 seconds */
                for (iLoop = 0; iLoop < DEFAULT_REG_CHECK_DURATION / DEFAULT_REJ_CHECK_PERIOD; iLoop++)                                {
                    iRet = CcbWorkDelayEntry(pWorkItem->id,DEFAULT_REJ_CHECK_PERIOD*1000,CcbDelayCallBack);
                    if (iRet )
                    {
                        VOS_LOG(VOS_LOG_WARNING,"CcbModbusWorkEntry Fail %d",iRet);    
                        /* notify ui (late implemnt) */
                        cbf(pWorkItem->id);                
                        return ;
                    }  
            
                    fRej = CcbCalcREJ();
            
                    if (fRej >= CcbGetSp2() 
                        && gCcb.ExeBrd.aEcoObjs[APP_EXE_I2_NO].Value.eV.fWaterQ < CcbGetSp3())
                    {
                        iValidCount ++;
            
                        if (iValidCount >= DEFAULT_REJ_CHECK_NUMBER)
                        {
                            break;
                        }
                    }
                    else
                    {
                        iValidCount = 0;
                    }
                }
            
                if (fRej < CcbGetSp2())
                {
                    /*alarem */
                    gCcb.bit1AlarmRej = TRUE;
                    gCcb.ulAlarmRejTick = gulSecond;
                }
            
                if (gCcb.ExeBrd.aEcoObjs[APP_EXE_I2_NO].Value.eV.fWaterQ > CcbGetSp3())
                {
                    gCcb.bit1AlarmRoPw   = TRUE;
                    gCcb.ulAlarmRoPwTick = gulSecond;
                }
                
            }

            pCcb->bit3RuningState = NOT_RUNING_STATE_NONE;
            CcbNotState(NOT_STATE_OTHER);
    
            VOS_LOG(VOS_LOG_WARNING,"E1,C1 ON I1b,I2,B1,B2"); 
            /* produce water */
            /* E1,C1 ON I1b,I2,B1,B2 */
            if(haveB3(&gCcb))
            {
                iTmp = (1 << APP_EXE_E1_NO)|(1<<APP_EXE_C1_NO)|(1<<APP_EXE_C3_NO);
            }
            else
            {
                iTmp = (1 << APP_EXE_E1_NO)|(1<<APP_EXE_C1_NO)|(1 << APP_EXE_E10_NO)|(1<<APP_EXE_C3_NO);
            }
            iRet = CcbUpdateSwitch(pWorkItem->id,0,pCcb->ulRunMask,iTmp);
            if (iRet )
            {
                VOS_LOG(VOS_LOG_WARNING,"CcbSetSwitch Fail %d",iRet);    
                /* notify ui (late implemnt) */
                cbf(pWorkItem->id);        
                return ;
            }
     
            iTmp  = (1 << APP_EXE_I1_NO)|(1<<APP_EXE_I2_NO);
            iTmp |= (GET_B_MASK(APP_EXE_PM1_NO))|(GET_B_MASK(APP_EXE_PM2_NO));
            iRet = CcbUpdateIAndBs(pWorkItem->id,0,iTmp,iTmp);
            if (iRet )
            {
                VOS_LOG(VOS_LOG_WARNING,"CcbSetIAndBs Fail %d",iRet);    
                /* notify ui (late implemnt) */
                cbf(pWorkItem->id);        
                return ;
            }
        }                
        break;
    case MACHINE_PURIST:
        break;
    case MACHINE_ADAPT:
        break;
    }

    gCcb.bit1ProduceWater = TRUE;

    cbs(pWorkItem->id);

}

void work_init_run_wrapper(void *para)
{
    WORK_ITEM_STRU *pWorkItem = (WORK_ITEM_STRU *)para;

    CCB *pCcb = (CCB *)pWorkItem->para;

    int iTmp;

    int iRet;

    /* notify ui (late implemnt) */

    pCcb->bit1AlarmRej     = FALSE;
    pCcb->bit1ProduceWater = FALSE;
    pCcb->bit1LeakKey4Reset= FALSE;
    pCcb->iInitRunTimer    = 0;
    pCcb->bit1B1Check4RuningState = FALSE;
    pCcb->bit1B1UnderPressureDetected = FALSE;

    /* 2018/01/15 add begin*/
    pCcb->bit1AlarmROPHV    = FALSE;
    pCcb->bit1AlarmROPLV    = FALSE;
    pCcb->bit1AlarmRODV     = FALSE;
    pCcb->ulRopVelocity     = 0;
    pCcb->ulLstRopFlow      = INVALID_FM_VALUE;
    pCcb->ulLstRopTick      = 0;
    pCcb->iRopVCheckLowEventCount   = 0;
    pCcb->iRopVCheckLowRestoreCount = 0;
    
    pCcb->ulRodVelocity    = 0;
    pCcb->ulLstRodFlow     = INVALID_FM_VALUE;
    pCcb->ulLstRodTick     = 0;
    pCcb->iRoDVCheckEventCount   = 0;
    pCcb->iRoDVCheckRestoreCount = 0;
    /* 2018/01/15 add end*/

    if (!pCcb->bit1InitRuned)
    {
        pCcb->bit1InitRuned = TRUE;
        
        CanCcbTransState4Pw(DISP_WORK_STATE_RUN,DISP_WORK_SUB_IDLE);
    }

    CanCcbTransState(DISP_WORK_STATE_PREPARE,DISP_WORK_SUB_IDLE);

    switch(pCcb->ulMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_L_EDI_LOOP:
    case MACHINE_L_RO_LOOP:
        /* for simplicity ,start all Bx report */
        iRet = CcbUpdateIAndBs(pWorkItem->id,0,pCcb->ulPMMask,pCcb->ulPMMask);    
        if (iRet )
        {
            VOS_LOG(VOS_LOG_WARNING,"CcbUpdatePmObjState Fail %d",iRet);    
    
            /* notify ui (late implemnt) */
            work_init_run_fail(pWorkItem->id);
            
            return ;
        }
    
        /* get B3 reports from exe */
        iRet = CcbGetPmValue(pWorkItem->id,APP_EXE_PM3_NO,1);
        if (iRet )
        {
            VOS_LOG(VOS_LOG_WARNING,"CcbGetPmValue Fail %d",iRet);  
    
            /* 1. ui promote */
            work_init_run_fail(pWorkItem->id);
            
            /* notify ui (late implemnt) */
            return ;
        }
    
        if (haveB3(pCcb) 
           && CcbConvert2Pm3SP(pCcb->ExeBrd.aPMObjs[APP_EXE_PM3_NO].Value.ulV) < CcbGetSp9())
        {
            /* 1. ui promote */
            work_init_run_fail(pWorkItem->id);
            
            /* 2. goto primary tank filling procedure */
            pCcb->bit1NeedFillTank = 1;
            
            return;
        }   
        

        VOS_LOG(VOS_LOG_WARNING,"Flush for Init Run"); 

        /* wash state for init run */
        /* E1,E2,C3 ON*/
        /* set  valves */
#ifdef  SYSTEM_TEST
        iTmp = (1 << APP_EXE_E1_NO)|(1<<APP_EXE_E2_NO)|(1 << APP_EXE_E10_NO)|(1<<APP_EXE_C3_NO);
#else
        /* 2018/01/05 add E3 according to ZHANG */
        if(haveB3(&gCcb))
        {
            iTmp = (1 << APP_EXE_E1_NO)|(1<<APP_EXE_E2_NO)|(1<<APP_EXE_E3_NO)|(1<<APP_EXE_C3_NO);
        }
        else
        {
            iTmp = (1 << APP_EXE_E1_NO)|(1<<APP_EXE_E2_NO)|(1<<APP_EXE_E3_NO)|(1<<APP_EXE_C3_NO)
                    |(1 << APP_EXE_E10_NO)|(1 << APP_EXE_C3_NO);
        }
#endif        
        
        iRet = CcbSetSwitch(pWorkItem->id,0,iTmp);
        if (iRet )
        {
            VOS_LOG(VOS_LOG_WARNING,"CcbSetSwitch Fail %d",iRet); 
    
            work_init_run_fail(pWorkItem->id);  
            
            /* notify ui (late implemnt) */
            return ;
        }
    
        /* enable S4  flow meter */
        iTmp = 1 << APP_FM_FM4_NO;
        iRet = CcbSetFms(pWorkItem->id,0,iTmp); 
        if (iRet )
        {
            VOS_LOG(VOS_LOG_WARNING,"CcbSetFms Fail %d",iRet); 
    
            work_init_run_fail(pWorkItem->id);  
            
            /* notify ui (late implemnt) */
            return ;
        }
        
        /* enable I1 reports */
        
        /* I ID */
        iTmp = 1 << APP_EXE_I1_NO;
        iRet = CcbUpdateIAndBs(pWorkItem->id,0,iTmp,iTmp);
        if (iRet )
        {
            VOS_LOG(VOS_LOG_WARNING,"CcbSetIAndBs Fail %d",iRet);   
    
            work_init_run_fail(pWorkItem->id);  
            
            /* notify ui (late implemnt) */
            return ;
        }

        pCcb->bit3RuningState = NOT_RUNING_STATE_FLUSH;

        CcbNotState(NOT_STATE_OTHER);

        
        VOS_LOG(VOS_LOG_WARNING,"iPowerOnFlushTime %d",pCcb->MiscParam.iPowerOnFlushTime);    
    
        // pCcb->TMParam.aulTime[TIME_PARAM_InitRunT1] should be replaced by (pCcb->MiscParam.iPowerOnFlushTime*60*1000)
        if(ex_isPackNew)
        {
            ex_isPackNew = 0;
            iRet = CcbWorkDelayEntry(pWorkItem->id,
                                     20*60*1000,
                                     CcbDelayCallBack);
        }
        else
        {
            iRet = CcbWorkDelayEntry(pWorkItem->id,pCcb->MiscParam.iPowerOnFlushTime*60*1000,CcbDelayCallBack);
        }

        if (iRet )
        {
            VOS_LOG(VOS_LOG_WARNING,"CcbWorkDelayEntry Fail %d",iRet);    
    
            work_init_run_fail(pWorkItem->id);  
            
            /* notify ui (late implemnt) */
            return ;
        }
        
        break;
    case MACHINE_Genie:
    case MACHINE_UP:
    case MACHINE_EDI:
    case MACHINE_RO:
        iRet = CcbUpdateIAndBs(pWorkItem->id,0,pCcb->ulPMMask,pCcb->ulPMMask);    
        if (iRet )
        {
            VOS_LOG(VOS_LOG_WARNING,"CcbUpdatePmObjState Fail %d",iRet);    
    
            /* notify ui (late implemnt) */
            work_init_run_fail(pWorkItem->id);
            
            return ;
        }

        VOS_LOG(VOS_LOG_WARNING,"Flush for Init Run"); 


        /* wash state for init run */
        /* E1,E2,E3 ON*/
        /* set  valves */
        if(haveB3(&gCcb))
        {
            iTmp = (1 << APP_EXE_E1_NO)|(1<<APP_EXE_E2_NO)|(1 << APP_EXE_E3_NO)|(1<<APP_EXE_C3_NO);
        }
        else
        {
            iTmp = (1 << APP_EXE_E1_NO)|(1<<APP_EXE_E2_NO)|(1 << APP_EXE_E3_NO)
                    |(1 << APP_EXE_E10_NO)|(1 << APP_EXE_C3_NO);
        }
        
        iRet = CcbSetSwitch(pWorkItem->id,0,iTmp);
        if (iRet )
        {
            VOS_LOG(VOS_LOG_WARNING,"CcbSetSwitch Fail %d",iRet); 
    
            work_init_run_fail(pWorkItem->id);  
            
            /* notify ui (late implemnt) */
            return ;
        }
    
        /* enable I1 reports */
        
        /* I ID */
        iTmp = 1 << APP_EXE_I1_NO;
        iRet = CcbUpdateIAndBs(pWorkItem->id,0,iTmp,iTmp);
        if (iRet )
        {
            VOS_LOG(VOS_LOG_WARNING,"CcbSetIAndBs Fail %d",iRet);   
    
            work_init_run_fail(pWorkItem->id);  
            
            /* notify ui (late implemnt) */
            return ;
        }

        VOS_LOG(VOS_LOG_WARNING,"iPowerOnFlushTime %d",pCcb->MiscParam.iPowerOnFlushTime);    

        pCcb->bit3RuningState = NOT_RUNING_STATE_FLUSH;

        CcbNotState(NOT_STATE_OTHER);

        // pCcb->TMParam.aulTime[TIME_PARAM_InitRunT1] should be replaced by (pCcb->MiscParam.iPowerOnFlushTime*60*1000)
        if(ex_isPackNew)
        {
            ex_isPackNew = 0;
            iRet = CcbWorkDelayEntry(pWorkItem->id,
                                     20*60*1000,
                                     CcbDelayCallBack);
        }
        else
        {
            iRet = CcbWorkDelayEntry(pWorkItem->id,pCcb->MiscParam.iPowerOnFlushTime*60*1000,CcbDelayCallBack);
        }

        if (iRet )
        {
            VOS_LOG(VOS_LOG_WARNING,"CcbWorkDelayEntry Fail %d",iRet);    
    
            work_init_run_fail(pWorkItem->id);  
            
            /* notify ui (late implemnt) */
            //CcbNotState();
            return ;
        }        
        break;
    case MACHINE_PURIST:
        break;
    case MACHINE_ADAPT:
        iRet = CcbUpdateIAndBs(pWorkItem->id,0,pCcb->ulPMMask,pCcb->ulPMMask);    
        if (iRet )
        {
            VOS_LOG(VOS_LOG_WARNING,"CcbUpdatePmObjState Fail %d",iRet);    
    
            /* notify ui (late implemnt) */
            work_init_run_fail(pWorkItem->id);
            
            return ;
        }

        VOS_LOG(VOS_LOG_WARNING,"Flush for Init Run"); 


        /* wash state for init run */
        /* E1,E2,E3 ON*/
        /* set  valves */
        if(haveB3(&gCcb))
        {
            iTmp = (1 << APP_EXE_E1_NO)|(1<<APP_EXE_E2_NO)|(1 << APP_EXE_E3_NO)|(1<<APP_EXE_C3_NO);
        }
        else
        {
            iTmp = (1 << APP_EXE_E1_NO)|(1<<APP_EXE_E2_NO)|(1 << APP_EXE_E3_NO)
                    |(1 << APP_EXE_E10_NO)|(1 << APP_EXE_C3_NO);
        }
        
        iRet = CcbSetSwitch(pWorkItem->id,0,iTmp);
        if (iRet )
        {
            VOS_LOG(VOS_LOG_WARNING,"CcbSetSwitch Fail %d",iRet); 
    
            work_init_run_fail(pWorkItem->id);  
            
            /* notify ui (late implemnt) */
            return ;
        }
    
        /* enable I1 reports */
        
        /* I ID */
        iTmp = 1 << APP_EXE_I1_NO;
        iRet = CcbUpdateIAndBs(pWorkItem->id,0,iTmp,iTmp);
        if (iRet )
        {
            VOS_LOG(VOS_LOG_WARNING,"CcbSetIAndBs Fail %d",iRet);   
    
            work_init_run_fail(pWorkItem->id);  
            
            /* notify ui (late implemnt) */
            return ;
        }

        VOS_LOG(VOS_LOG_WARNING,"iPowerOnFlushTime %d",pCcb->MiscParam.iPowerOnFlushTime);    

        pCcb->bit3RuningState = NOT_RUNING_STATE_FLUSH;

        CcbNotState(NOT_STATE_OTHER);

        // pCcb->TMParam.aulTime[TIME_PARAM_InitRunT1] should be replaced by (pCcb->MiscParam.iPowerOnFlushTime*60*1000)
        if(ex_isPackNew)
        {
            ex_isPackNew = 0;
            iRet = CcbWorkDelayEntry(pWorkItem->id,
                                     20*60*1000,
                                     CcbDelayCallBack);
        }
        else
        {
            iRet = CcbWorkDelayEntry(pWorkItem->id,pCcb->MiscParam.iPowerOnFlushTime*60*1000,CcbDelayCallBack);
        }
        
        if (iRet )
        {
            VOS_LOG(VOS_LOG_WARNING,"CcbWorkDelayEntry Fail %d",iRet);    
    
            work_init_run_fail(pWorkItem->id);  
            
            /* notify ui (late implemnt) */
            return ;
        } 

        //2019.2.14 add
        if(!(DispGetUpCirFlag()
             | DispGetTankCirFlag()
             | DispGetTocCirFlag()
             //| DispGetUpQtwFlag()
             //| DispGetEdiQtwFlag()
             | CcbGetTwFlag()
             | CcbGetTwPendingFlag()))
        {
            iTmp = 0;
            iRet = CcbUpdateSwitch(pWorkItem->id,0,pCcb->ulRunMask,iTmp);
        }
        /*
        iTmp = 0;
        iRet = CcbUpdateSwitch(pWorkItem->id,0,pCcb->ulRunMask,iTmp);
        */
        break;

    }

    CanCcbTransState(DISP_WORK_STATE_RUN,DISP_WORK_SUB_RUN_INIT);

    /* come to normal RUN proc*/
    
    work_run_comm_proc(pWorkItem,pCcb,work_init_run_fail,work_init_run_succ);

}

void work_init_run(void *para)
{
    VOS_LOG(VOS_LOG_WARNING,"Enter %s",__FUNCTION__);    

    work_init_run_wrapper(para);

    VOS_LOG(VOS_LOG_WARNING,"Exit %s",__FUNCTION__);    

}

void work_tank_start_full_loop(void *para)
{
    WORK_ITEM_STRU *pWorkItem = (WORK_ITEM_STRU *)para;

    CCB *pCcb = (CCB *)pWorkItem->para;

    int iTmp;

    int iRet;

    /*E1,E2,C1,C3 ON*/
    if(haveB3(&gCcb))
    {
        iTmp = (1 << APP_EXE_E1_NO)|(1<<APP_EXE_E2_NO)|(1<<APP_EXE_C1_NO)|(1<<APP_EXE_C3_NO);
    }
    else
    {
        iTmp = (1 << APP_EXE_E1_NO)|(1<<APP_EXE_E2_NO)|(1<<APP_EXE_C1_NO)|(1 << APP_EXE_E10_NO)|(1<<APP_EXE_C3_NO);
    }

    iRet = CcbUpdateSwitch(pWorkItem->id,0,iTmp,iTmp);
    if (iRet )
    {
        VOS_LOG(VOS_LOG_WARNING,"CcbSetSwitch Fail %d",iRet);    
        /* notify ui (late implemnt) */
        return ;
    }

    pCcb->bit1PeriodFlushing = TRUE;

    /* update for next round full tank loop */
    pCcb->ulB2FullTick = gulSecond; 

    CcbNotState(NOT_STATE_OTHER);

}


DISPHANDLE CcbInnerWorkStartTankFullLoop(void)
{
    WORK_ITEM_STRU *pWorkItem = CcbAllocWorkItem();

    if (!pWorkItem)
    {
        return DISP_INVALID_HANDLE;
    }

    pWorkItem->proc = work_tank_start_full_loop;
    pWorkItem->para = &gCcb;

    CcbAddWork(WORK_LIST_LP,pWorkItem);

    return (DISPHANDLE)pWorkItem;

}

void work_tank_stop_full_loop(void *para)
{
    WORK_ITEM_STRU *pWorkItem = (WORK_ITEM_STRU *)para;

    CCB *pCcb = (CCB *)pWorkItem->para;

    int iTmp;

    int iRet;

    /*E1,E2,C1,C3 OFF */
    if(haveB3(&gCcb))
    {
        iTmp = (1 << APP_EXE_E1_NO)|(1<<APP_EXE_E2_NO)|(1<<APP_EXE_C1_NO)|(1<<APP_EXE_C3_NO);
    }
    else
    {
        iTmp = (1 << APP_EXE_E1_NO)|(1<<APP_EXE_E2_NO)|(1<<APP_EXE_C1_NO)|(1 << APP_EXE_E10_NO)|(1<<APP_EXE_C3_NO);
    }

    iRet = CcbUpdateSwitch(pWorkItem->id,0,iTmp,0);
    if (iRet )
    {
        VOS_LOG(VOS_LOG_WARNING,"CcbSetSwitch Fail %d",iRet);    
        /* notify ui (late implemnt) */
    }

    pCcb->bit1PeriodFlushing = FALSE;

    CcbNotState(NOT_STATE_OTHER);
}

DISPHANDLE CcbInnerWorkStopTankFullLoop(void)
{
    WORK_ITEM_STRU *pWorkItem = CcbAllocWorkItem();

    if (!pWorkItem)
    {
        return DISP_INVALID_HANDLE;
    }

    pWorkItem->proc = work_tank_stop_full_loop;
    pWorkItem->para = &gCcb;

    CcbAddWork(WORK_LIST_LP,pWorkItem);

    return (DISPHANDLE)pWorkItem;

}



DISPHANDLE CcbInnerWorkInitRun(void)
{
    WORK_ITEM_STRU *pWorkItem = CcbAllocWorkItem();

    if (!pWorkItem)
    {
        return DISP_INVALID_HANDLE;
    }

    pWorkItem->proc = work_init_run;
    pWorkItem->para = &gCcb;

    CcbAddWork(WORK_LIST_LP,pWorkItem);

    return (DISPHANDLE)pWorkItem;
}


void work_rpt_setup_exe(void *para)
{
    WORK_ITEM_STRU *pWorkItem = (WORK_ITEM_STRU *)para;

    CCB *pCcb = (CCB *)pWorkItem->para;

    int iRet;

    VOS_LOG(VOS_LOG_WARNING,"%s : %x & %x",__FUNCTION__,pCcb->WorkRptSetParam4Exe.ulMask,pCcb->WorkRptSetParam4Exe.ulValue);    

    iRet = CcbUpdateIAndBs(pWorkItem->id,0,pCcb->WorkRptSetParam4Exe.ulMask,pCcb->WorkRptSetParam4Exe.ulValue);    

    if (iRet )
    {
        VOS_LOG(VOS_LOG_WARNING,"CcbUpdateIAndBs Fail %d",iRet);    
        /* notify ui (late implemnt) */
        return ;
    }  
}

void work_rpt_setup_fm(void *para)
{
    WORK_ITEM_STRU *pWorkItem = (WORK_ITEM_STRU *)para;

    CCB *pCcb = (CCB *)pWorkItem->para;

    int iRet;
    VOS_LOG(VOS_LOG_WARNING,"%s: %x & %x",__FUNCTION__,pCcb->WorkRptSetParam4Fm.ulMask,pCcb->WorkRptSetParam4Fm.ulValue);    

    iRet = CcbUpdateFms(pWorkItem->id,0,pCcb->WorkRptSetParam4Fm.ulMask,pCcb->WorkRptSetParam4Fm.ulValue);    

    if (iRet )
    {
        VOS_LOG(VOS_LOG_WARNING,"CcbUpdateFms Fail %d",iRet);    
        /* notify ui (late implemnt) */
        return ;
    }  
}



void work_switch_setup_exe(void *para)
{
    WORK_ITEM_STRU *pWorkItem = (WORK_ITEM_STRU *)para;

    CCB *pCcb = (CCB *)pWorkItem->para;

    int iRet;
    
    VOS_LOG(VOS_LOG_WARNING,"%s : %x & %x",__FUNCTION__,pCcb->WorkSwitchSetParam4Exe.ulMask,pCcb->WorkSwitchSetParam4Exe.ulValue);    

    iRet = CcbUpdateSwitch(pWorkItem->id,0,pCcb->WorkSwitchSetParam4Exe.ulMask,pCcb->WorkSwitchSetParam4Exe.ulValue);    

    if (iRet )
    {
        VOS_LOG(VOS_LOG_WARNING,"CcbUpdateSwitch Fail %d",iRet);    
        /* notify ui (late implemnt) */
        return ;
    }  
}


DISPHANDLE CcbInnerWorkSetupExeReport(void)
{
    WORK_ITEM_STRU *pWorkItem = CcbAllocWorkItem();

    if (!pWorkItem)
    {
        return DISP_INVALID_HANDLE;
    }

    pWorkItem->proc = work_rpt_setup_exe;
    pWorkItem->para = &gCcb;

    CcbAddWork(WORK_LIST_LP,pWorkItem);

    return (DISPHANDLE)pWorkItem;
}



DISPHANDLE CcbInnerWorkSetupFmReport(void)
{
    WORK_ITEM_STRU *pWorkItem = CcbAllocWorkItem();

    if (!pWorkItem)
    {
        return DISP_INVALID_HANDLE;
    }

    pWorkItem->proc = work_rpt_setup_fm;
    pWorkItem->para = &gCcb;

    CcbAddWork(WORK_LIST_LP,pWorkItem);

    return (DISPHANDLE)pWorkItem;
}

DISPHANDLE CcbInnerWorkSetupExeSwitch(void)
{
    WORK_ITEM_STRU *pWorkItem = CcbAllocWorkItem();

    if (!pWorkItem)
    {
        return DISP_INVALID_HANDLE;
    }

    pWorkItem->proc = work_switch_setup_exe;
    pWorkItem->para = &gCcb;

    CcbAddWork(WORK_LIST_LP,pWorkItem);

    return (DISPHANDLE)pWorkItem;
}

void work_idle_succ(void)
{
    int aiCont[1] = {0};
    
    MainSndWorkMsg(WORK_MSG_TYPE_IDLE,(unsigned char *)aiCont,sizeof(aiCont));
    
}


void work_idle(void *para)
{
    WORK_ITEM_STRU *pWorkItem = (WORK_ITEM_STRU *)para;

    CCB *pCcb = (CCB *)pWorkItem->para;

    int iTmp;

    VOS_LOG(VOS_LOG_WARNING,"work_idle");    

    pCcb->bit1InitRuned = FALSE;

    /* 1. close all valves */

    iTmp = 0;
    CcbSetSwitch(pWorkItem->id,0,iTmp); // don care modbus exe result
    
    /* 2. disable all reports from exe */
    CcbSetIAndBs(pWorkItem->id,0,iTmp); // don care modbus exe result
    
    /* 3. disable all reports from exe */
    CcbSetFms(pWorkItem->id,0,iTmp); // don care modbus exe result

    /* notify hand set (late implement) */     
    work_idle_succ();
    /* notify ui (late implemnt) */
       
}

void work_wash_fail_comm(int iWorkId)
{
    /* close all switchs */
    if (CcbGetSwitchObjState(APP_EXE_SWITCHS_MASK))
    {
        CcbSetSwitch(iWorkId,0,0);
    }
    
    /*stop all report */
    if (CcbGetPmObjState(APP_EXE_PM_MASK)
        || CcbGetIObjState(APP_EXE_I_MASK))
    {
    
        CcbSetIAndBs(iWorkId,0,0);

        CcbUpdateIObjState(APP_EXE_I_MASK,0);
    }

    if (CcbGetFmObjState(APP_FM_FM_MASK))
    {
        CcbSetFms(iWorkId,0,0);
    }
}

void work_rowash_fail(int iType,int iWorkId)
{
    int aiCont[2] = {-1,iType};
    

    work_wash_fail_comm(iWorkId);


    MainSndWorkMsg(WORK_MSG_TYPE_WASH,(unsigned char *)aiCont,sizeof(aiCont));

}

void work_rowash_succ(int iType)
{
    int aiCont[2] = {0,iType};
    
    MainSndWorkMsg(WORK_MSG_TYPE_WASH,(unsigned char *)aiCont,sizeof(aiCont));

}


void work_idle_rowash(void *para)
{
    WORK_ITEM_STRU *pWorkItem = (WORK_ITEM_STRU *)para;

    CCB *pCcb = (CCB *)pWorkItem->para;

    int iTmp;

    int iRet;

    int iType = (int)pWorkItem->extra;

    pCcb->iWashType = iType;

    /* notify ui (late implemnt) */
    VOS_LOG(VOS_LOG_WARNING,"%s",__FUNCTION__);    

    switch(pCcb->ulMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_L_EDI_LOOP:
    case MACHINE_L_RO_LOOP:
        
        CanCcbTransState(DISP_WORK_STATE_IDLE,DISP_WORK_SUB_WASH);        
        
        /* enable S4 report */
        iTmp = 1 << APP_FM_FM4_NO;
        iRet = CcbSetFms(pWorkItem->id,0,iTmp); // don care modbus exe result
        if (iRet )
        {
            VOS_LOG(VOS_LOG_WARNING,"CcbSetFms Fail %d",iRet);   
            
            /* notify ui (late implemnt) */
            work_rowash_fail(iType,pWorkItem->id);        
            return ;
        }
        
        /* enable E1,E2,E3,C3 ON  */
#ifdef  SYSTEM_TEST
        iTmp = (1 << APP_EXE_E1_NO)|(1<<APP_EXE_E2_NO)|(1<<APP_EXE_E3_NO)|(1<<APP_EXE_E10_NO)|(1<<APP_EXE_C3_NO);
#else
        if(haveB3(&gCcb))
        {
            iTmp = (1 << APP_EXE_E1_NO)|(1<<APP_EXE_E2_NO)|(1<<APP_EXE_E3_NO)|(1<<APP_EXE_C3_NO);
        }
        else
        {
            iTmp = (1 << APP_EXE_E1_NO)|(1<<APP_EXE_E2_NO)|(1<<APP_EXE_E3_NO)
                    |(1 << APP_EXE_E10_NO)|(1<<APP_EXE_C3_NO);
        }
#endif        
        iRet = CcbSetSwitch(pWorkItem->id,0,iTmp); // don care modbus exe result
        if (iRet )
        {
            VOS_LOG(VOS_LOG_WARNING,"CcbSetSwitch Fail %d",iRet);    
        
            /* notify ui (late implemnt) */
            work_rowash_fail(iType,pWorkItem->id);        
            return ;
        }
        
        iRet = CcbWorkDelayEntry(pWorkItem->id,/*pCcb->TMParam.aulTime[TIME_PARAM_RoWashT1]*/13*60*1000,CcbDelayCallBack);
        if (iRet )
        {
            VOS_LOG(VOS_LOG_WARNING,"CcbWorkDelayEntry Fail %d",iRet);    
        
            /* notify ui (late implemnt) */
            work_rowash_fail(iType,pWorkItem->id);        
            return ;
        }
        
        /* enable E1,E2,E3,C1,C3 ON */
        if(haveB3(&gCcb))
        {
            iTmp = (1 << APP_EXE_E1_NO)|(1<<APP_EXE_E2_NO)|(1<<APP_EXE_E3_NO)|(1<<APP_EXE_C3_NO)|(1<<APP_EXE_C1_NO);
        }
        else
        {
            iTmp = (1 << APP_EXE_E1_NO)|(1<<APP_EXE_E2_NO)|(1<<APP_EXE_E3_NO)|(1<<APP_EXE_C1_NO)|
                    (1 << APP_EXE_E10_NO)|(1<<APP_EXE_C3_NO);
        }
        
        iRet = CcbUpdateSwitch(pWorkItem->id,0,APP_EXE_INNER_SWITCHS,iTmp); // don care modbus exe result
        if (iRet )
        {
            VOS_LOG(VOS_LOG_WARNING,"CcbUpdateSwitch Fail %d",iRet);    
        
           /* notify ui (late implemnt) */
           work_rowash_fail(iType,pWorkItem->id);        
           return ;
        }
        
        iRet = CcbWorkDelayEntry(pWorkItem->id,/*pCcb->TMParam.aulTime[TIME_PARAM_RoWashT2]*/5*60*1000,CcbDelayCallBack);
        if (iRet )
        {
            VOS_LOG(VOS_LOG_WARNING,"CcbWorkDelayEntry Fail %d",iRet);    
            /* notify ui (late implemnt) */
            work_rowash_fail(iType,pWorkItem->id);        
            return ;
        }
        
        /* enable E1,E3,C1,C3 ON */
        if(haveB3(&gCcb))
        {
            iTmp = (1 << APP_EXE_E1_NO)|(1<<APP_EXE_E3_NO)|(1<<APP_EXE_C1_NO)|(1<<APP_EXE_C3_NO);
        }
        else
        {
            iTmp = (1 << APP_EXE_E1_NO)|(1<<APP_EXE_E3_NO)|(1<<APP_EXE_C1_NO)|(1 << APP_EXE_E10_NO)|(1<<APP_EXE_C3_NO);
        }
        iRet = CcbUpdateSwitch(pWorkItem->id,0,APP_EXE_INNER_SWITCHS,iTmp); // don care modbus exe result
        if (iRet )
        {
            VOS_LOG(VOS_LOG_WARNING,"CcbUpdateSwitch Fail %d",iRet);    
           /* notify ui (late implemnt) */
           work_rowash_fail(iType,pWorkItem->id);        
           return ;
        }
        
        iRet = CcbWorkDelayEntry(pWorkItem->id,/*pCcb->TMParam.aulTime[TIME_PARAM_RoWashT3]*/2*60*1000,CcbDelayCallBack);
        if (iRet )
        {
            VOS_LOG(VOS_LOG_WARNING,"CcbWorkDelayEntry Fail %d",iRet);    
            /* notify ui (late implemnt) */
            work_rowash_fail(iType,pWorkItem->id);        
            return ;
        }
        

        /* close all valves */
        iTmp = 0;
        iRet = CcbSetSwitch(pWorkItem->id,0,iTmp); // don care modbus exe result
        if (iRet )
        {
            VOS_LOG(VOS_LOG_WARNING,"CcbSetSwitch Fail %d",iRet);    
            /* notify ui (late implemnt) */
            work_rowash_fail(iType,pWorkItem->id);        
            return ;
        }
        
        /* disble  S4 */
        iTmp = 0;
        iRet = CcbSetFms(pWorkItem->id,0,iTmp); // don care modbus exe result
        if (iRet )
        {
            VOS_LOG(VOS_LOG_WARNING,"CcbSetFms Fail %d",iRet);    
            /* notify ui (late implemnt) */
            work_rowash_fail(iType,pWorkItem->id);        
            return ;
        }  
        break;
    case MACHINE_Genie:
    case MACHINE_UP:
    case MACHINE_EDI:
    case MACHINE_RO:
        CanCcbTransState(DISP_WORK_STATE_IDLE,DISP_WORK_SUB_WASH);        
        
        
        /* enable E1,E2,E3 ON  */
        if(haveB3(&gCcb))
        {
            iTmp = (1 << APP_EXE_E1_NO)|(1<<APP_EXE_E2_NO)|(1<<APP_EXE_E3_NO)|(1<<APP_EXE_C3_NO);
        }
        else
        {
            iTmp = (1 << APP_EXE_E1_NO)|(1<<APP_EXE_E2_NO)|(1<<APP_EXE_E3_NO)|(1 << APP_EXE_E10_NO)|(1 << APP_EXE_C3_NO);
        }

        iRet = CcbSetSwitch(pWorkItem->id,0,iTmp); // don care modbus exe result
        if (iRet )
        {
            VOS_LOG(VOS_LOG_WARNING,"CcbSetSwitch Fail %d",iRet);    
        
            /* notify ui (late implemnt) */
            work_rowash_fail(iType,pWorkItem->id);        
            return ;
        }
        
        iRet = CcbWorkDelayEntry(pWorkItem->id,/*pCcb->TMParam.aulTime[TIME_PARAM_RoWashT1]*/13*60*1000,CcbDelayCallBack);
        if (iRet )
        {
            VOS_LOG(VOS_LOG_WARNING,"CcbWorkDelayEntry Fail %d",iRet);    
        
            /* notify ui (late implemnt) */
            work_rowash_fail(iType,pWorkItem->id);        
            return ;
        }
        
        /* enable E1,E3,C1 ON */
        if(haveB3(&gCcb))
        {
            iTmp = (1 << APP_EXE_E1_NO)|(1<<APP_EXE_E3_NO)|(1<<APP_EXE_C1_NO)|(1<<APP_EXE_C3_NO);
        }
        else
        {
            iTmp = (1 << APP_EXE_E1_NO)|(1<<APP_EXE_E3_NO)|(1<<APP_EXE_C1_NO)|(1 << APP_EXE_E10_NO)|(1 << APP_EXE_C3_NO);
        }
        
        iRet = CcbUpdateSwitch(pWorkItem->id,0,APP_EXE_INNER_SWITCHS,iTmp); // don care modbus exe result
        if (iRet )
        {
            VOS_LOG(VOS_LOG_WARNING,"CcbUpdateSwitch Fail %d",iRet);    
        
           /* notify ui (late implemnt) */
           work_rowash_fail(iType,pWorkItem->id);        
           return ;
        }
        
        iRet = CcbWorkDelayEntry(pWorkItem->id,/*pCcb->TMParam.aulTime[TIME_PARAM_RoWashT2]*/5*60*1000,CcbDelayCallBack);
        if (iRet )
        {
            VOS_LOG(VOS_LOG_WARNING,"CcbWorkDelayEntry Fail %d",iRet);    
            /* notify ui (late implemnt) */
            work_rowash_fail(iType,pWorkItem->id);        
            return ;
        }
        

        /* close all valves */
        iTmp = 0;
        iRet = CcbSetSwitch(pWorkItem->id,0,iTmp); // don care modbus exe result
        if (iRet )
        {
            VOS_LOG(VOS_LOG_WARNING,"CcbSetSwitch Fail %d",iRet);    
            /* notify ui (late implemnt) */
            work_rowash_fail(iType,pWorkItem->id);        
            return ;
        }
        break;
    default:
        break; // invlaid value
    }


    /* notify ui (late implement) */
    work_rowash_succ(iType);  
}


void work_idle_syswash(void *para)
{
    WORK_ITEM_STRU *pWorkItem = (WORK_ITEM_STRU *)para;

    CCB *pCcb = (CCB *)pWorkItem->para;

    int iType = (int)pWorkItem->extra;

    pCcb->iWashType = iType;

    /* notify ui (late implemnt) */
    VOS_LOG(VOS_LOG_WARNING,"%s",__FUNCTION__);    

    /* notify ui (late implement) */
    work_rowash_succ(iType);  
}


void work_phwash_fail(int iType,int iWorkId)
{
    int aiCont[2] = {-1,iType};

    work_wash_fail_comm(iWorkId);
    
    MainSndWorkMsg(WORK_MSG_TYPE_WASH,(unsigned char *)aiCont,sizeof(aiCont));
}

void work_phwash_succ(int iType)
{
    int aiCont[2] = {0,iType};

    MainSndWorkMsg(WORK_MSG_TYPE_WASH,(unsigned char *)aiCont,sizeof(aiCont));

}

void work_idle_phwash(void *para)
{
    WORK_ITEM_STRU *pWorkItem = (WORK_ITEM_STRU *)para;

    CCB *pCcb = (CCB *)pWorkItem->para;

    int iTmp;

    int iRet;

    int iType = (int)pWorkItem->extra;

    pCcb->iWashType = iType;

    /* notify ui (late implemnt) */
    VOS_LOG(VOS_LOG_WARNING,"%s",__FUNCTION__);    

    switch(pCcb->ulMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_L_EDI_LOOP:
    case MACHINE_L_RO_LOOP:
        
        CanCcbTransState(DISP_WORK_STATE_IDLE,DISP_WORK_SUB_WASH);        
        
        /* enable S4 report */
        iTmp = 1 << APP_FM_FM4_NO;
        iRet = CcbSetFms(pWorkItem->id,0,iTmp); // don care modbus exe result
        
        if (iRet )
        {
            VOS_LOG(VOS_LOG_WARNING,"CcbSetFms Fail %d",iRet);    
            /* notify ui (late implemnt) */
            work_phwash_fail(iType,pWorkItem->id);        
            return ;
        }
        
        /* enable E1,E2,E3,C3 ON  */
#ifdef  SYSTEM_TEST
        iTmp = (1 << APP_EXE_E1_NO)|(1<<APP_EXE_E2_NO)|(1<<APP_EXE_E3_NO)|(1<<APP_EXE_E10_NO)|(1<<APP_EXE_C3_NO);
#else
        if(haveB3(&gCcb))
        {
            iTmp = (1 << APP_EXE_E1_NO)|(1<<APP_EXE_E2_NO)|(1<<APP_EXE_E3_NO)|(1<<APP_EXE_C3_NO);
        }
        else
        {
            iTmp = (1 << APP_EXE_E1_NO)|(1<<APP_EXE_E2_NO)|(1<<APP_EXE_E3_NO)|(1 << APP_EXE_E10_NO)|(1<<APP_EXE_C3_NO);
        }
#endif        

        iRet = CcbSetSwitch(pWorkItem->id,0,iTmp); // don care modbus exe result
        if (iRet )
        {
            VOS_LOG(VOS_LOG_WARNING,"CcbModbusWorkEntry Fail %d",iRet);    
            /* notify ui (late implemnt) */
            work_phwash_fail(iType,pWorkItem->id);        
            return ;
        }
        
        iRet = CcbWorkDelayEntry(pWorkItem->id,/*pCcb->TMParam.aulTime[TIME_PARAM_PhWashT1]*/10*1000,CcbDelayCallBack);
        if (iRet )
        {
            VOS_LOG(VOS_LOG_WARNING,"CcbWorkDelayEntry Fail %d",iRet);    
            /* notify ui (late implemnt) */
            work_phwash_fail(iType,pWorkItem->id);        
            return ;
        }
        
        /* immense */
        iTmp = (1<<APP_EXE_E2_NO)|(1<<APP_EXE_E3_NO)|(1<<APP_EXE_C1_NO);
        
        iRet = CcbUpdateSwitch(pWorkItem->id,0,APP_EXE_INNER_SWITCHS,iTmp); // don care modbus exe result
        if (iRet )
        {
            VOS_LOG(VOS_LOG_WARNING,"CcbSetSwitch Fail %d",iRet);    
            /* notify ui (late implemnt) */
            work_phwash_fail(iType,pWorkItem->id);        
            return ;
        }
        
        iRet = CcbWorkDelayEntry(pWorkItem->id,/*pCcb->TMParam.aulTime[TIME_PARAM_PhWashT2]*/5*60*1000,CcbDelayCallBack);
        if (iRet )
        {
            VOS_LOG(VOS_LOG_WARNING,"CcbWorkDelayEntry Fail %d",iRet);    
            /* notify ui (late implemnt) */
            work_phwash_fail(iType,pWorkItem->id);        
            return ;
        }
        

        
        /* enable E1,E3,C1,C3 ON */
        if(haveB3(&gCcb))
        {
            iTmp = (1 << APP_EXE_E1_NO)|(1 << APP_EXE_E2_NO)|(1<<APP_EXE_E3_NO)|(1<<APP_EXE_C3_NO);
        }
        else
        {
            iTmp = (1 << APP_EXE_E1_NO)|(1 << APP_EXE_E2_NO)|(1<<APP_EXE_E3_NO)|(1 << APP_EXE_E10_NO)|(1<<APP_EXE_C3_NO);
        }
        iRet = CcbUpdateSwitch(pWorkItem->id,0,APP_EXE_INNER_SWITCHS,iTmp); // don care modbus exe result
        if (iRet )
        {
            VOS_LOG(VOS_LOG_WARNING,"CcbSetSwitch Fail %d",iRet);    
           /* notify ui (late implemnt) */
           work_phwash_fail(iType,pWorkItem->id);        
           return ;
        }
        
        iRet = CcbWorkDelayEntry(pWorkItem->id,/*pCcb->TMParam.aulTime[TIME_PARAM_PhWashT5]*/10*60*1000,CcbDelayCallBack);
        if (iRet )
        {
            VOS_LOG(VOS_LOG_WARNING,"CcbWorkDelayEntry Fail %d",iRet);    
            /* notify ui (late implemnt) */
            work_phwash_fail(iType,pWorkItem->id);        
            return ;
        }
        
        /* E1,E3 */
        if(haveB3(&gCcb))
        {
            iTmp = (1<<APP_EXE_E1_NO)|(1<<APP_EXE_E3_NO)|(1<<APP_EXE_C1_NO)|(1<<APP_EXE_C3_NO);
        }
        else
        {
            iTmp = (1<<APP_EXE_E1_NO)|(1<<APP_EXE_E3_NO)|(1<<APP_EXE_C1_NO)|(1<<APP_EXE_E10_NO)|(1<<APP_EXE_C3_NO);
        }
        iRet = CcbUpdateSwitch(pWorkItem->id,0,APP_EXE_INNER_SWITCHS,iTmp); // don care modbus exe result
        if (iRet )
        {
            VOS_LOG(VOS_LOG_WARNING,"CcbUpdateSwitch Fail %d",iRet);    
           /* notify ui (late implemnt) */
           work_rowash_fail(iType,pWorkItem->id);        
           return ;
        }

        iRet = CcbWorkDelayEntry(pWorkItem->id,10*60*1000,CcbDelayCallBack);
        if (iRet )
        {
            VOS_LOG(VOS_LOG_WARNING,"CcbWorkDelayEntry Fail %d",iRet);    
            /* notify ui (late implemnt) */
            work_phwash_fail(iType,pWorkItem->id);        
            return ;
        }

        /* close all valves */
        iTmp = 0;
        iRet = CcbSetSwitch(pWorkItem->id,0,iTmp); // don care modbus exe result
        if (iRet )
        {
            VOS_LOG(VOS_LOG_WARNING,"CcbSetSwitch Fail %d",iRet);    
            /* notify ui (late implemnt) */
            work_phwash_fail(iType,pWorkItem->id);        
            return ;
        }

        
        /* disable S4 by disable all Sn */
        iTmp = 0;
        iRet = CcbSetFms(pWorkItem->id,0,iTmp); // don care modbus exe result
        if (iRet )
        {
            VOS_LOG(VOS_LOG_WARNING,"CcbSetFms Fail %d",iRet);    
            /* notify ui (late implemnt) */
            work_phwash_fail(iType,pWorkItem->id);        
            return ;
        }  
        break;
    case MACHINE_Genie:
    case MACHINE_UP:
    case MACHINE_EDI:
    case MACHINE_RO:
        CanCcbTransState(DISP_WORK_STATE_IDLE,DISP_WORK_SUB_WASH);        
        
        /* enable E1,E2,E3 ON  */
        if(haveB3(&gCcb))
        {
            iTmp = (1 << APP_EXE_E1_NO)|(1<<APP_EXE_E2_NO)|(1<<APP_EXE_E3_NO)|(1<<APP_EXE_C3_NO);
        }
        else
        {
            iTmp = (1 << APP_EXE_E1_NO)|(1<<APP_EXE_E2_NO)|(1<<APP_EXE_E3_NO)|(1 << APP_EXE_E10_NO)|(1 << APP_EXE_C3_NO);
        }

        iRet = CcbSetSwitch(pWorkItem->id,0,iTmp); // don care modbus exe result
        if (iRet )
        {
            VOS_LOG(VOS_LOG_WARNING,"CcbModbusWorkEntry Fail %d",iRet);    
            /* notify ui (late implemnt) */
            work_phwash_fail(iType,pWorkItem->id);        
            return ;
        }
        
        iRet = CcbWorkDelayEntry(pWorkItem->id,/*pCcb->TMParam.aulTime[TIME_PARAM_PhWashT1]*/10*1000,CcbDelayCallBack);
        if (iRet )
        {
            VOS_LOG(VOS_LOG_WARNING,"CcbWorkDelayEntry Fail %d",iRet);    
            /* notify ui (late implemnt) */
            work_phwash_fail(iType,pWorkItem->id);        
            return ;
        }
        
        /* immense */
        iTmp = (1<<APP_EXE_E3_NO)|(1<<APP_EXE_C1_NO);
        
        iRet = CcbUpdateSwitch(pWorkItem->id,0,APP_EXE_INNER_SWITCHS,iTmp); // don care modbus exe result
        if (iRet )
        {
            VOS_LOG(VOS_LOG_WARNING,"CcbSetSwitch Fail %d",iRet);    
            /* notify ui (late implemnt) */
            work_phwash_fail(iType,pWorkItem->id);        
            return ;
        }
        
        iRet = CcbWorkDelayEntry(pWorkItem->id,/*pCcb->TMParam.aulTime[TIME_PARAM_PhWashT2]*/60*60*1000,CcbDelayCallBack);
        if (iRet )
        {
            VOS_LOG(VOS_LOG_WARNING,"CcbWorkDelayEntry Fail %d",iRet);    
            /* notify ui (late implemnt) */
            work_phwash_fail(iType,pWorkItem->id);        
            return ;
        }
        

        /* enable E1,E2,E3 ON  */
        if(haveB3(&gCcb))
        {
            iTmp = (1 << APP_EXE_E1_NO)|(1<<APP_EXE_E2_NO)|(1<<APP_EXE_E3_NO)|(1<<APP_EXE_C3_NO);
        }
        else
        {
            iTmp = (1 << APP_EXE_E1_NO)|(1<<APP_EXE_E2_NO)|(1<<APP_EXE_E3_NO)|(1 << APP_EXE_E10_NO)|(1 << APP_EXE_C3_NO);
        }
        iRet = CcbUpdateSwitch(pWorkItem->id,0,APP_EXE_INNER_SWITCHS,iTmp); // don care modbus exe result
        if (iRet )
        {
            VOS_LOG(VOS_LOG_WARNING,"CcbSetSwitch Fail %d",iRet);    
           /* notify ui (late implemnt) */
           work_phwash_fail(iType,pWorkItem->id);        
           return ;
        }
        
        iRet = CcbWorkDelayEntry(pWorkItem->id,/*pCcb->TMParam.aulTime[DEFAULT_PhWashT2]*/10*60*1000,CcbDelayCallBack);
        if (iRet )
        {
            VOS_LOG(VOS_LOG_WARNING,"CcbWorkDelayEntry Fail %d",iRet);    
            /* notify ui (late implemnt) */
            work_phwash_fail(iType,pWorkItem->id);        
            return ;
        }
        
        /* E1,E3,C1 */
        if(haveB3(&gCcb))
        {
            iTmp = (1<<APP_EXE_E1_NO)|(1<<APP_EXE_E3_NO)|(1<<APP_EXE_C1_NO)|(1<<APP_EXE_C3_NO);
        }
        else
        {
            iTmp = (1<<APP_EXE_E1_NO)|(1<<APP_EXE_E3_NO)|(1<<APP_EXE_C1_NO)|(1<<APP_EXE_E10_NO)|(1<<APP_EXE_C3_NO);
        }
        iRet = CcbUpdateSwitch(pWorkItem->id,0,APP_EXE_INNER_SWITCHS,iTmp); // don care modbus exe result
        if (iRet )
        {
            VOS_LOG(VOS_LOG_WARNING,"CcbUpdateSwitch Fail %d",iRet);    
           /* notify ui (late implemnt) */
           work_rowash_fail(iType,pWorkItem->id);        
           return ;
        }

        iRet = CcbWorkDelayEntry(pWorkItem->id,/*pCcb->TMParam.aulTime[DEFAULT_PhWashT2]*/10*60*1000,CcbDelayCallBack);
        if (iRet )
        {
            VOS_LOG(VOS_LOG_WARNING,"CcbWorkDelayEntry Fail %d",iRet);    
            /* notify ui (late implemnt) */
            work_phwash_fail(iType,pWorkItem->id);        
            return ;
        }


        /* close all valves */
        iTmp = 0;
        iRet = CcbSetSwitch(pWorkItem->id,0,iTmp); // don care modbus exe result
        if (iRet )
        {
            VOS_LOG(VOS_LOG_WARNING,"CcbSetSwitch Fail %d",iRet);    
            /* notify ui (late implemnt) */
            work_phwash_fail(iType,pWorkItem->id);        
            return ;
        }

        break;
    default:
        break;
    }

   /* notify ui (late implement) */
   work_phwash_succ(iType);
      
}

void work_start_lpp(void *para)
{
    int iTmp;
    int iRet;
    
    WORK_ITEM_STRU *pWorkItem = (WORK_ITEM_STRU *)para;

    CCB *pCcb = (CCB *)pWorkItem->para;

    /* 1. ui promote */
    iTmp = 0; 
    iRet = CcbUpdateSwitch(pWorkItem->id,0,pCcb->ulRunMask,iTmp);
    if (iRet )
    {
        VOS_LOG(VOS_LOG_WARNING,"CcbSetSwitch Fail %d",iRet);
    }
    
    /* fire alarm */
    if (!gCcb.bit1AlarmTapInPress)
    {
        gCcb.bit1AlarmTapInPress   = TRUE;
    
        gCcb.ulFiredAlarmFlags |= ALARM_TLP;
    
        CcbNotAlarmFire(DISP_ALARM_PART1,DISP_ALARM_PART1_LOWER_SOURCE_WATER_PRESSURE,TRUE);
    }
    VOS_LOG(VOS_LOG_WARNING," DISP_WORK_STATE_LPP %d",iRet);  
    
    CanCcbTransState(DISP_WORK_STATE_LPP,DISP_WORK_SUB_IDLE);

    if(MACHINE_ADAPT == gCcb.ulMachineType)
    {
        gCcb.ulAdapterAgingCount = 0XFFFFFF00;
    }

}

DISPHANDLE CcbInnerWorkLpp(void)
{
    WORK_ITEM_STRU *pWorkItem = CcbAllocWorkItem();

    if (!pWorkItem)
    {
        return DISP_INVALID_HANDLE;
    }

    pWorkItem->proc = work_start_lpp;
    pWorkItem->para = &gCcb;

    CcbAddWork(WORK_LIST_LP,pWorkItem);

    return (DISPHANDLE)pWorkItem;
}

DISPHANDLE DispCmdIdleProc(void)
{
    DISPHANDLE handle;

    if (DISP_WORK_STATE_IDLE == gCcb.curWorkState.iMainWorkState
        && DISP_WORK_SUB_IDLE == gCcb.curWorkState.iSubWorkState)
    {
        return DISP_INVALID_HANDLE;
    }

    /* just put into work queue */
    handle = SearchWork(work_idle);

    if (!handle)
    {
        handle = CcbInnerWorkIdle();
    }
    return handle;
}

DISPHANDLE DispCmdWashProc(unsigned char *pucData, int iLength)
{
    DISP_CMD_WASH_STRU *pCmd = (DISP_CMD_WASH_STRU *)pucData;

    (void)iLength;

    /* 2018/01/05 add stop logic to cleaning process */
    if (pCmd->iState)
    {
        if (DISP_WORK_STATE_IDLE != gCcb.curWorkState.iMainWorkState
            || DISP_WORK_SUB_IDLE != gCcb.curWorkState.iSubWorkState)
        {
            return DISP_INVALID_HANDLE;
        }
        
        /* just put into work queue */
        {
            WORK_ITEM_STRU *pWorkItem = CcbAllocWorkItem();
        
            if (!pWorkItem)
            {
                return DISP_INVALID_HANDLE;
            }
        
            switch(pCmd->iType)
            {
            case DISP_CLEAN_RO:
                pWorkItem->proc = work_idle_rowash;
                break;
            case DISP_CLEAN_PH:
                pWorkItem->proc = work_idle_phwash;
                break;
            /*
            case DISP_CLEAN_SYSTEM:
                pWorkItem->proc = work_idle_syswash;
                break;
           */
            }
            pWorkItem->para = &gCcb;
            pWorkItem->extra = (void *)pCmd->iType;
        
            CcbAddWork(WORK_LIST_LP,pWorkItem);
        
            return (DISPHANDLE)pWorkItem;
        }
    }
    else
    {
        if (DISP_WORK_STATE_IDLE == gCcb.curWorkState.iMainWorkState
            && DISP_WORK_SUB_WASH == gCcb.curWorkState.iSubWorkState)
        {
            CcbCancelAllWork();
        }

        return DISP_SPECIAL_HANDLE;
    }
}


DISPHANDLE DispCmdResetProc(void)
{
    DISPHANDLE handle;

    if (DISP_WORK_STATE_LPP != gCcb.curWorkState.iMainWorkState
        || DISP_WORK_SUB_IDLE != gCcb.curWorkState.iSubWorkState)
    {
        return DISP_INVALID_HANDLE;
    }

    handle = SearchWork(work_init_run);

    if (!handle)
    {
        handle = CcbInnerWorkInitRun();
    }

    return handle;

}

//#define LEAK_RESET_RUN
DISPHANDLE DispCmdLeakResetProc(void)
{
    if (DISP_WORK_STATE_KP != gCcb.curWorkState.iMainWorkState
        || DISP_WORK_SUB_IDLE != gCcb.curWorkState.iSubWorkState)
    {
        return DISP_INVALID_HANDLE;
    }

    // CcbPopState();

    gCcb.ulKeyWorkStates   &= ~(1 << APP_EXE_DIN_LEAK_KEY) ;    
    gCcb.ExeBrd.ucDinState &= ~(1 << APP_EXE_DIN_LEAK_KEY) ;   
    gCcb.bit1LeakKey4Reset = FALSE;
    
    if (gCcb.ExeBrd.ucDinState
        & APP_EXE_DIN_FAIL_MASK)
    {
        CanCcbDinProcess();
    }
    else
    {
        //CanCcbRestore();
        gCcb.curWorkState.iMainWorkState = DISP_WORK_STATE_IDLE;
        gCcb.curWorkState.iSubWorkState  = DISP_WORK_SUB_IDLE;

    }

    return DISP_SPECIAL_HANDLE;

}

/*********************************************************************
 * Function:        void work_start_tube_cir(void *para)
 *
 * PreCondition:    None
 *
 * Input:           para  type of WORK_ITEM_STRU 
 *
 * Output:          Start tube Ciculation
 *
 * Side Effects:    None
 *
 * Overview:        Start tube circulation
 *
 * Note:            None.
 ********************************************************************/
void work_start_tube_cir(void *para)
{
    WORK_ITEM_STRU *pWorkItem = (WORK_ITEM_STRU *)para;
    CCB *pCcb = (CCB *)pWorkItem->para;

    int iTmp;
    int iRet;
    

    /* check water level */
    iRet = CcbGetPmValue(pWorkItem->id,APP_EXE_PM2_NO,1);
    if (iRet )
    {
        VOS_LOG(VOS_LOG_WARNING,"CcbGetPmValue Fail %d",iRet);  

        return ;
    }

    if (CcbConvert2Pm2SP(pCcb->ExeBrd.aPMObjs[APP_EXE_PM2_NO].Value.ulV) < CcbGetSp6())
    {
        gCcb.bit1B2Empty = TRUE;
        
        return;
    }

    VOS_LOG(VOS_LOG_DEBUG,"work_start_tube_cir");    
    
    /* set  valves   */
    iTmp = (1 << APP_EXE_C4_NO)|(1 << APP_EXE_N2_NO);
    iRet = CcbUpdateSwitch(pWorkItem->id,0,iTmp,iTmp);
    if (iRet )
    {
        VOS_LOG(VOS_LOG_WARNING,"CcbUpdateSwitch Fail %d",iRet);    
    }

    /* set  valves   */
    iTmp = 1 << APP_EXE_I4_NO;
    iRet = CcbUpdateIAndBs(pWorkItem->id,0,iTmp,iTmp);
    if (iRet )
    {
        VOS_LOG(VOS_LOG_WARNING,"CcbUpdateIAndBs Fail %d",iRet);    
        /* notify ui (late implemnt) */
        return ;
    }

    pCcb->bit1TubeCirOngoing    = TRUE;
    pCcb->ulTubeCirTick         = 0;
    pCcb->ulProduceWaterBgnTime = time(NULL);

}

DISPHANDLE CcbInnerWorkStartTubeCir(void)
{
    WORK_ITEM_STRU *pWorkItem = CcbAllocWorkItem();

    if (!pWorkItem)
    {
        return DISP_INVALID_HANDLE;
    }

    pWorkItem->proc = work_start_tube_cir;
    pWorkItem->para = &gCcb;

    CcbAddWork(WORK_LIST_LP,pWorkItem);

    return (DISPHANDLE)pWorkItem;

}


/*********************************************************************
 * Function:        void work_stop_tube_cir(void *para)
 *
 * PreCondition:    None
 *
 * Input:           para  type of WORK_ITEM_STRU 
 *
 * Output:          Stop producing water
 *
 * Side Effects:    None
 *
 * Overview:        Stop producing water
 *
 * Note:            None.
 ********************************************************************/
void work_stop_tube_cir(void *para)
{
    WORK_ITEM_STRU *pWorkItem = (WORK_ITEM_STRU *)para;
    CCB *pCcb = (CCB *)pWorkItem->para;

    int iTmp;
    int iRet;

    /* set  valves   */
    iTmp = (1 << APP_EXE_C4_NO)|(1 << APP_EXE_N2_NO);
    iRet = CcbUpdateSwitch(pWorkItem->id,0,iTmp,0);
    if (iRet )
    {
        VOS_LOG(VOS_LOG_WARNING,"CcbSetSwitch Fail %d",iRet);    
    }
    
    pCcb->bit1TubeCirOngoing = FALSE;
    pCcb->bit1NeedTubeCir    = FALSE;

}

DISPHANDLE CcbInnerWorkStopTubeCir(void)
{
    WORK_ITEM_STRU *pWorkItem = CcbAllocWorkItem();

    if (!pWorkItem)
    {
        return DISP_INVALID_HANDLE;
    }

    pWorkItem->proc = work_stop_tube_cir;
    pWorkItem->para = &gCcb;

    CcbAddWork(WORK_LIST_LP,pWorkItem);

    return (DISPHANDLE)pWorkItem;

}


DISPHANDLE DispCmdTubeCirProc(unsigned char *pucData, int iLength)
{
    DISP_CMD_TC_STRU *pTc = (DISP_CMD_TC_STRU *)pucData;
    (void)iLength;

    switch(gCcb.ulMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_L_EDI_LOOP:
    case MACHINE_L_RO_LOOP:
        gCcb.bit1NeedTubeCir = pTc->iStart;
        gCcb.iTubeCirTime   = pTc->iDuration;
    
        if ((!gCcb.bit1NeedTubeCir)
            && gCcb.bit1TubeCirOngoing)
        {
            /* stop tub cir */
            if (!SearchWork(work_stop_tube_cir))
            {
                CcbInnerWorkStopTubeCir();
            }        
        }
        return DISP_SPECIAL_HANDLE;
    case MACHINE_Genie:
    case MACHINE_UP:
    case MACHINE_EDI:
    case MACHINE_RO:
        return DISP_INVALID_HANDLE;
    default:
        return DISP_INVALID_HANDLE; 
    }
}

DISPHANDLE DispCmdRunProc(void)
{
    DISPHANDLE handle;


    VOS_LOG(VOS_LOG_WARNING,"DispCmdRunProc Main & Sub %d %d",gCcb.curWorkState.iMainWorkState,gCcb.curWorkState.iSubWorkState);    
    
    if (DISP_WORK_STATE_IDLE != gCcb.curWorkState.iMainWorkState
        || DISP_WORK_SUB_IDLE != gCcb.curWorkState.iSubWorkState)
    {
        return DISP_INVALID_HANDLE;
    }

    handle = SearchWork(work_init_run);

    if (!handle)
    {
        handle = CcbInnerWorkInitRun();
    }

    return handle;

}


void CcbCancelAllWork(void)
{
    int iLoop;

    sp_thread_mutex_lock(&gCcb.WorkMutex);

    for (iLoop = 0; iLoop < WORK_LIST_NUM; iLoop++)
    {
        list_t *pos, *n;
        list_for_each_safe(pos,n,&gCcb.WorkList[iLoop])
        {
            WORK_ITEM_STRU *pWorkItem = list_entry(pos,WORK_ITEM_STRU,list) ;
            {
                pWorkItem->flag |= WORK_FLAG_CANCEL; // append cancel flag

                if (pWorkItem->flag & WORK_FLAG_ACTIVE)
                {
                    pWorkItem->cancel(pWorkItem);
                }
            }
        }
    }
    sp_thread_mutex_unlock(&gCcb.WorkMutex);

}


DISPHANDLE CcbCancelWork(DISPHANDLE handle)
{
    int iLoop;

    sp_thread_mutex_lock(&gCcb.WorkMutex);

    for (iLoop = 0; iLoop < WORK_LIST_NUM; iLoop++)
    {
        list_t *pos, *n;
        list_for_each_safe(pos,n,&gCcb.WorkList[iLoop])
        {
            WORK_ITEM_STRU *pWorkItem = list_entry(pos,WORK_ITEM_STRU,list) ;

            if (((DISPHANDLE)pWorkItem) == handle)
            {
                pWorkItem->flag |= WORK_FLAG_CANCEL; // append cancel flag

                if (pWorkItem->flag & WORK_FLAG_ACTIVE)
                {
                    pWorkItem->cancel(pWorkItem);
                }
                sp_thread_mutex_unlock(&gCcb.WorkMutex);
                return handle;
            }
        }
    }
    sp_thread_mutex_unlock(&gCcb.WorkMutex);

    return DISP_INVALID_HANDLE;

}

void CcbRmvWork(work_proc proc)
{
    int iLoop;

    sp_thread_mutex_lock(&gCcb.WorkMutex);

    for (iLoop = 0; iLoop < WORK_LIST_NUM; iLoop++)
    {
        list_t *pos, *n;
        list_for_each_safe(pos,n,&gCcb.WorkList[iLoop])
        {
            WORK_ITEM_STRU *pWorkItem = list_entry(pos,WORK_ITEM_STRU,list) ;

            if ((pWorkItem->proc) == proc)
            {
                pWorkItem->flag |= WORK_FLAG_CANCEL; // append cancel flag

                if (pWorkItem->flag & WORK_FLAG_ACTIVE)
                {
                    pWorkItem->cancel(pWorkItem);
                }
            }
        }
    }
    sp_thread_mutex_unlock(&gCcb.WorkMutex);

}



DISPHANDLE DispCmdCancelWork(unsigned char *pucData, int iLength)
{
    DISPHANDLE handle = (DISPHANDLE)(*((int *)pucData));

    if (iLength != sizeof(void *))
    {
        return DISP_INVALID_HANDLE;
    }


    return CcbCancelWork(handle); 
}

DISPHANDLE DispCmdHaltProc(void)
{
    DISPHANDLE handle;

    CcbCancelAllWork();

    handle = SearchWork(work_idle);

    if (!handle)
    {
        handle = CcbInnerWorkIdle();
    }  
    else
    {
        VOS_LOG(VOS_LOG_WARNING,"work_idle in queue: %d",gCcb.iBusyWork);    
    }

    return handle;

}

DISPHANDLE DispCmdEngProc(unsigned char *pucData, int iLength)
{
    (void)iLength;

   if (!pucData[0])
   {
       gCcb.bit1EngineerMode = FALSE;
       
       return DISP_SPECIAL_HANDLE;
   }
   else
   {
       gCcb.bit1EngineerMode = TRUE;
    
       /* cancel all work & return to idle */
       return DispCmdHaltProc();
   }

}

DISPHANDLE DispCmdTw(unsigned char *pucData, int iLength)
{
    DISP_CMD_TW_STRU *pCmd = (DISP_CMD_TW_STRU *)pucData;
    (void)iLength;

    int iIdx    = CcbGetHandlerId2Index(pCmd->iDevId);

    int iTwFlag = CcbGetTwFlag();

    VOS_LOG(VOS_LOG_DEBUG,"qtw: %d & %d",gCcb.curWorkState.iMainWorkState4Pw ,gCcb.curWorkState.iSubWorkState4Pw);    
    

    if (DISP_WORK_STATE_RUN != gCcb.curWorkState.iMainWorkState4Pw)
    {
        return DISP_INVALID_HANDLE;
    }

    switch(pCmd->iType)
    {
    case APP_DEV_HS_SUB_HYPER:
    case APP_DEV_HS_SUB_REGULAR:
        if (CcbGetHandlerTwFlag(pCmd->iDevId))
        {
           /* try to stop */
           return CcbInnerWorkStopQtw(iIdx);
           
        }
        else if (iTwFlag) /* at least another one handler is busy */
        {
            return DISP_INVALID_HANDLE;
        }
        else 
        {
            int iPendingTwFlag = CcbGetTwPendingFlag();
            
            if (iPendingTwFlag )
            {
               if (iPendingTwFlag & (1 << iIdx))
               {
                   DISPHANDLE hdl ;
                    
                   hdl = SearchWork(work_start_qtw);
                    
                   if (hdl)
                   {
                       CcbCancelWork(hdl);

                       return hdl;
                   }
               }
               return DISP_INVALID_HANDLE;
            }

        }

        if (!CcbGetHandlerStatus(pCmd->iDevId))
        {
            VOS_LOG(VOS_LOG_DEBUG,"CTW: hs %d : invalid state",pCmd->iDevId);    
            return DISP_INVALID_HANDLE;
        }

        /* try to take water */
        if (CcbGetHandlerType(pCmd->iDevId) == pCmd->iType)
        {
            int iIndex   = (pCmd->iDevId - APP_HAND_SET_BEGIN_ADDRESS);

            if (gCcb.aHandler[iIndex].iTrxMap & (1 << APP_TRX_CAN))
            {
                CanCcbSaveQtw2(APP_TRX_CAN,pCmd->iDevId,pCmd->iVolume);
            }
            else if (gCcb.aHandler[iIndex].iTrxMap & (1 << APP_TRX_ZIGBEE))
            {
                CanCcbSaveQtw2(APP_TRX_ZIGBEE,pCmd->iDevId,pCmd->iVolume);
            }
            else
            {
                CanCcbSaveQtw2(APP_TRX_CAN,pCmd->iDevId,pCmd->iVolume);
            }
            
            /* remove cir work if any */
            CcbRmvWork(work_start_cir);
             
            /* construct work */
            return CcbInnerWorkStartQtw(CcbGetHandlerId2Index(pCmd->iDevId));
            
        }
        break;
    default:
        return DISP_INVALID_HANDLE;
    }

   return DISP_INVALID_HANDLE;
}

DISPHANDLE DispCmdCir(unsigned char *pucData, int iLength)
{
    DISP_CMD_CIR_STRU *pCmd = (DISP_CMD_CIR_STRU *)pucData;

    (void)iLength;

    VOS_LOG(VOS_LOG_DEBUG,"Cir: %d & %d",gCcb.curWorkState.iMainWorkState4Pw ,gCcb.curWorkState.iSubWorkState4Pw);    
    
    if (DISP_WORK_STATE_RUN != gCcb.curWorkState.iMainWorkState4Pw)
    {
        return DISP_INVALID_HANDLE;
    }

   switch(pCmd->iType)
   {
   case CIR_TYPE_UP:
        if ((DISP_WORK_STATE_RUN == gCcb.curWorkState.iMainWorkState4Pw)
            && (DISP_WORK_SUB_IDLE == gCcb.curWorkState.iSubWorkState4Pw))
        {
           if (!SearchWork(work_start_cir))
           {
               return CcbInnerWorkStartCir(CIR_TYPE_UP);
           }
        }
        if ((DISP_WORK_STATE_RUN == gCcb.curWorkState.iMainWorkState4Pw)
            && (DISP_WORK_SUB_RUN_CIR == gCcb.curWorkState.iSubWorkState4Pw))
        {
           if (!SearchWork(work_stop_cir))
           {
               return CcbInnerWorkStopCir();
           }   
        }
        break;
   case CIR_TYPE_HP:
       switch(gCcb.ulMachineType)
       {
       case MACHINE_ADAPT:
       case MACHINE_PURIST:
           break;
       default:
           if ((DISP_WORK_STATE_RUN == gCcb.curWorkState.iMainWorkState4Pw)
               && (DISP_WORK_SUB_IDLE == gCcb.curWorkState.iSubWorkState4Pw))
           {
              if (haveHPCir(&gCcb))
              {
                  if (!SearchWork(work_start_cir))
                  {
                      return CcbInnerWorkStartCir(CIR_TYPE_HP);
                  }
              }
           }
           if ((DISP_WORK_STATE_RUN == gCcb.curWorkState.iMainWorkState4Pw)
               && (DISP_WORK_SUB_RUN_CIR == gCcb.curWorkState.iSubWorkState4Pw)
               && (CIR_TYPE_HP == gCcb.iCirType))
           {
              if (!SearchWork(work_stop_cir))
              {
                  return CcbInnerWorkStopCir();
              }   
           }
           break;
        }
        break;
   }
   return DISP_INVALID_HANDLE;
}



DISPHANDLE DispCmdEngCmdProc(unsigned char *pucData, int iLength)
{
   int iRet = -1;
   (void)iLength;

   if (0 == (gCcb.ulActiveMask & (1 << APP_DEV_TYPE_EXE_BOARD)))
   {
       return DISP_INVALID_HANDLE;
   }
   
   VOS_LOG(VOS_LOG_WARNING,"DispCmdEngCmdProc %d&%d&%d",pucData[0],pucData[1],pucData[2]);    
   
   /* do something here */
   switch(pucData[0])
   {
   case ENG_CMD_TYPE_SWITCH:
       iRet = CcbUpdateSwitch(WORK_LIST_NUM,0,(1 << pucData[1]),pucData[2] ? (1 << pucData[1]) : 0);
       break;
   case ENG_CMD_TYPE_IB:
       iRet = CcbUpdateIAndBs(WORK_LIST_NUM,0,(1 << pucData[1]),pucData[2] ? (1 << pucData[1]) : 0);
       break;
   case ENG_CMD_TYPE_FM:
       iRet = CcbUpdateFms(WORK_LIST_NUM,0,(1 << pucData[1]),pucData[2] ? (1 << pucData[1]) : 0);
       break;
   case ENG_CMD_TYPE_RPUMP:
       iRet = CcbSetExeHoldRegs(WORK_LIST_NUM,((pucData[1]<<8)|pucData[2]),((pucData[3]<<8)|pucData[4]));
       break;
   }
   
   return (-1 == iRet) ?  DISP_INVALID_HANDLE : DISP_SPECIAL_HANDLE;
}


DISPHANDLE DispCmdSwitchReport(unsigned char *pucData, int iLength)
{
    DISP_CMD_SWITCH_STATE_REPORT_STRU *pCmd = (DISP_CMD_SWITCH_STATE_REPORT_STRU *)pucData;
    (void)iLength;
    
   gCcb.bit1SysMonitorStateRpt = !!pCmd->iRptFlag;

   return DISP_SPECIAL_HANDLE;
}


DISPHANDLE DispCmdEntry(int iCmdId,unsigned char *pucData, int iLength)
{
    VOS_LOG(VOS_LOG_WARNING,"DispCmdEntry %d",iCmdId);    
    
    switch(iCmdId)
    {
    case DISP_CMD_IDLE:
        return gCcb.bit1EngineerMode ? NULL : DispCmdIdleProc();
    case DISP_CMD_RUN:
        return  gCcb.bit1EngineerMode ? NULL : DispCmdRunProc();
    case DISP_CMD_WASH:
        return  gCcb.bit1EngineerMode ? NULL : DispCmdWashProc(pucData,iLength);
    case DISP_CMD_CANCEL_WORK:
        return  gCcb.bit1EngineerMode ? NULL : DispCmdCancelWork(pucData,iLength);
    case DISP_CMD_RESET:
        return  gCcb.bit1EngineerMode ? NULL : DispCmdResetProc();
    case DISP_CMD_LEAK_RESET:
        return gCcb.bit1EngineerMode ? NULL : DispCmdLeakResetProc();
    case DISP_CMD_TUBE_CIR:
        return gCcb.bit1EngineerMode ? NULL : DispCmdTubeCirProc(pucData,iLength);
    case DISP_CMD_HALT:
        return  gCcb.bit1EngineerMode ? NULL : DispCmdHaltProc();
    case DISP_CMD_ENG_MODE:
        return DispCmdEngProc(pucData, iLength);
    case DISP_CMD_ENG_CMD:
        return !gCcb.bit1EngineerMode ? NULL : DispCmdEngCmdProc(pucData,iLength);
    case DISP_CMD_TW:
        return gCcb.bit1EngineerMode ? NULL : DispCmdTw(pucData,iLength);
    case DISP_CMD_CIR:
        return gCcb.bit1EngineerMode ? NULL : DispCmdCir(pucData,iLength);
    case DISP_CMD_SWITCH_REPORT:
        return gCcb.bit1EngineerMode ? NULL : DispCmdSwitchReport(pucData,iLength);
    }
    return DISP_INVALID_HANDLE;
}

int DispIapEntry(IAP_CAN_CMD_STRU *pIapCmd)
{
    CAN_BUILD_ADDRESS_IDENTIFIER(pIapCmd->ulCanId,APP_DEV_TYPE_MAIN_CTRL,pIapCmd->ulCanId);

    return CcbSndIapCanCmd(pIapCmd->iCanChl,pIapCmd->ulCanId,pIapCmd->ucCmd,(unsigned char *)pIapCmd->data,pIapCmd->iPayLoadLen);
}

int DispAfEntry(IAP_CAN_CMD_STRU *pIapCmd)
{
    CAN_BUILD_ADDRESS_IDENTIFIER(pIapCmd->ulCanId,APP_DEV_TYPE_MAIN_CTRL,pIapCmd->ulCanId);

    return CcbSndAfCanCmd(pIapCmd->iCanChl,pIapCmd->ulCanId,pIapCmd->ucCmd,(unsigned char *)pIapCmd->data,pIapCmd->iPayLoadLen);
}

int DispAfRfEntry(IAP_CAN_CMD_STRU *pIapCmd)
{
    if (gCcb.aRfReader[pIapCmd->ulCanId].bit1RfDetected)
    {
        CAN_BUILD_ADDRESS_IDENTIFIER(pIapCmd->ulCanId,APP_DEV_TYPE_MAIN_CTRL,(APP_RF_READER_BEGIN_ADDRESS + pIapCmd->ulCanId));
        
        return CcbSndAfCanCmd(pIapCmd->iCanChl,pIapCmd->ulCanId,pIapCmd->ucCmd,(unsigned char *)pIapCmd->data,pIapCmd->iPayLoadLen);
    }

    return -1;
}

int DispZigbeeEntry(IAP_ZB_CMD_STRU *pIapCmd)
{
    return CcbSndZigbeeCmd(&pIapCmd->addr,SAPP_CMD_DATA,(unsigned char *)pIapCmd->data,pIapCmd->iPayLoadLen);
}

int DispGetWorkState(void)
{
    return gCcb.curWorkState.iMainWorkState; 
}

int DispGetWorkState4Pw(void)
{
    return gCcb.curWorkState.iMainWorkState4Pw; 
}

int DispGetSubWorkState4Pw(void)
{
    return gCcb.curWorkState.iSubWorkState4Pw; 
}

void DispSetSubWorkState4Pw(int SubWorkState4Pw)
{
    gCcb.curWorkState.iSubWorkState4Pw = SubWorkState4Pw; 
}


char *DispGetAscInfo(int idx)
{
    if (idx >= 2*WORK_MSG_TYPE_EFW + 1)
    {
        return "none";
    }

    return notify_info[idx];
}

int DispConvertVoltage2RPumpSpeed(float fv)
{
     int speed =(int) (((41.2/(fv/1.23 - 1))/10)*256);  /* 2018/03/07 */

     if (speed >= 255) speed = 255;
     if (speed < 0)    speed = 0;

     return speed;
}

int DispConvertRPumpSpeed2Voltage(int speed)
{
     int iVoltage;

     speed &= 0xff;

     if (0 == speed) return 24000;

     printf("speed = %d\r\n",speed);

     iVoltage =  (int)((1230*(1 + (41.2*256)/(speed*10)))); /* 2018/03/07 */

     if (iVoltage > 24000) return 24000;
     return iVoltage;
}


int DispConvertRPumpSpeed2Scale(int speed)
{
     int iIndex = PUMP_SPEED_NUM - 1;

     speed &= 0XFF;

     for (iIndex = 0; iIndex < PUMP_SPEED_NUM - 1; iIndex++)
     {
        if (speed >= gCcb.aiSpeed2ScaleMap[iIndex])
        {
            return iIndex;
        }
     }

     return iIndex;
}

int DispReadRPumpSpeed(void)
{
    return CcbGetExeHoldRegs(WORK_LIST_NUM,APP_EXE_HOLD_REG_RPUMP_OFFSET,APP_EXE_HOLD_REG_RPUMP_NUM);
}

void DispGetRPumpSpeed(int iIndex ,int *pValue)
{
    ///MODBUS_PACKET_COMM_STRU *pmg = (MODBUS_PACKET_COMM_STRU *)gCcb.aucModbusBuffer;

    *pValue = (gCcb.ExeBrd.ausHoldRegister[APP_EXE_HOLD_REG_RPUMP_OFFSET + iIndex] & 0xff);
}

void DispSetHanlerConfig(DB_HANDLER_STRU *pHdl)
{
   int iIdx = pHdl->address - APP_HAND_SET_BEGIN_ADDRESS;

   if (iIdx >= 0 && iIdx < APP_HAND_SET_MAX_NUMBER)
   {
       gCcb.aHandler[iIdx].iDevType = pHdl->type;
   }
}

void DispGetRPumpSwitchState(int iIndex ,int *pState)
{
   *pState = !!(gCcb.ExeBrd.aRPumpObjs[iIndex].iActive & DISP_ACT_TYPE_SWITCH);
}

void DispStartZigbeeUpd(int iLength,void *Mem)
{
    // send message to main
    int ret;
    
    DATAPROC_ZB_UPD_STRU *pItfMsg = NULL;

    pItfMsg = (DATAPROC_ZB_UPD_STRU *)SatAllocMsg(DATAPROC_ZB_UPD_MSG_SIZE);//malloc(sizeof(MAIN_CANITF_STRU) + CanRcvBuff[ucRcvBufIndex].len);
    if (NULL == pItfMsg)
    {
        return ;
    }

    pItfMsg->msgHead.event = CMD_ZB_DOWNLAOD_MSG;

    pItfMsg->iLength = iLength;
    pItfMsg->mem     = Mem;

    ret = VOS_SndMsg(TASK_HANDLE_ZB,(SAT_MSG_HEAD *)pItfMsg);

    if (0 != ret)
    {
    }
}


void MainResetModulers(void)
{
     APP_PACKET_HOST_RESET_STRU Rst;

     unsigned int ulIdentifier;

     memset(&Rst,0,sizeof(APP_PACKET_HOST_RESET_STRU));

     Rst.hdr.ucDevType = APP_DEV_TYPE_MAIN_CTRL ;
     Rst.hdr.ucMsgType = APP_PACKET_COMM_HOST_RESET;

     CAN_BUILD_ADDRESS_IDENTIFIER(ulIdentifier,APP_DEV_TYPE_MAIN_CTRL,CAN_BROAD_CAST_ADDRESS);

     CcbSndAfCanCmd(0,ulIdentifier,SAPP_CMD_DATA,(unsigned char *)&Rst,sizeof(Rst));

     if (haveZigbee(&gCcb) && zbGetNodes())
     {
         CcbZigbeeModeBroadcast((unsigned char *)&Rst,sizeof(Rst));
     }
}



void MainSndHeartBeat(void)
{
     unsigned char buf[32];
     
     unsigned int ulIdentifier;
     
     APP_PACKET_HEART_BEAT_REQ_STRU *pHbt  = (APP_PACKET_HEART_BEAT_REQ_STRU *)buf;
     APP_PACKET_HO_STATE_STRU       *pLoad = (APP_PACKET_HO_STATE_STRU *)pHbt->aucData;

     VOS_LOG(VOS_LOG_WARNING,"MainSndHeartBeat ");    

     memset(pHbt,0,sizeof(APP_PACKET_HOST_RESET_STRU));

     pHbt->hdr.ucLen     = sizeof(APP_PACKET_HO_STATE_STRU) ;
     pHbt->hdr.ucDevType = APP_DEV_TYPE_MAIN_CTRL ;
     pHbt->hdr.ucMsgType = APP_PACKET_COMM_HEART_BEAT;

     pLoad->ucState      = CanCcbGetHoState();
     pLoad->ucResult     = 0;
     pLoad->ucAlarmState = getAlarmState();
     
     switch(pLoad->ucState)
     {
     case APP_PACKET_HS_STATE_QTW:
         pLoad->ucAddData    = CcbGetTwFlag();
         pLoad->ucResult     = CcbGetTwPendingFlag();
         break;
     case APP_PACKET_HS_STATE_CIR:
         pLoad->ucAddData    = gCcb.iCirType;
         break;
     default :
         pLoad->ucAddData    = 0;
         break;
     }


     /* 2018/04/09 Add begin*/
     if ((CIR_TYPE_HP == gCcb.iCirType)
        && (APP_PACKET_HS_STATE_CIR == CanCcbGetHoState()))
     {
        pLoad->ucState = APP_PACKET_HS_STATE_RUN;
     }
     /* 2018/04/09 Add end */
     

     CAN_BUILD_ADDRESS_IDENTIFIER(ulIdentifier,APP_DEV_TYPE_MAIN_CTRL,CAN_BROAD_CAST_ADDRESS);

     CcbSndAfCanCmd(0,ulIdentifier,SAPP_CMD_DATA,buf,pHbt->hdr.ucLen + APP_PROTOL_HEADER_LEN);

     if (haveZigbee(&gCcb) && zbGetNodes())
     {
         //CcbZigbeeModeDedicate(buf,pHbt->hdr.ucLen + APP_PROTOL_HEADER_LEN);
         CcbZigbeeModeBroadcast(buf,pHbt->hdr.ucLen + APP_PROTOL_HEADER_LEN);
     }
}

void MainInitInnerIpc(void)
{
    int iLoop;
    
    sp_thread_mutex_init(&gCcb.Ipc.mutex,NULL);
    sp_thread_cond_init( &gCcb.Ipc.cond, NULL );
    sp_thread_mutex_init(&gCcb.Ipc4Rfid.mutex,NULL);
    sp_thread_cond_init( &gCcb.Ipc4Rfid.cond, NULL );

    for (iLoop = 0; iLoop < MAX_RFREADER_NUM; iLoop++)
    {
        sp_thread_mutex_init(&gCcb.aRfReader[iLoop].Ipc.mutex,NULL);
        sp_thread_cond_init( &gCcb.aRfReader[iLoop].Ipc.cond, NULL );
        
    }   

}

void MainDeinitInnerIpc(void)
{
    int iLoop;

    sp_thread_mutex_destroy(&gCcb.Ipc.mutex);
    sp_thread_cond_destroy( &gCcb.Ipc.cond );
    sp_thread_mutex_destroy(&gCcb.Ipc4Rfid.mutex);
    sp_thread_cond_destroy( &gCcb.Ipc4Rfid.cond );

    for (iLoop = 0; iLoop < MAX_RFREADER_NUM; iLoop++)
    {
        sp_thread_mutex_destroy(&gCcb.aRfReader[iLoop].Ipc.mutex);
        sp_thread_cond_destroy( &gCcb.aRfReader[iLoop].Ipc.cond );
    }   
    
}

void CcbInitMachineType(void)
{

    switch(gCcb.ulMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:   
        gCcb.ulHyperTwMask  = (1<<APP_EXE_E4_NO)|(1<<APP_EXE_E5_NO)|(1<<APP_EXE_E6_NO)|(1<<APP_EXE_E9_NO)|(1<<APP_EXE_C2_NO)|(1<<APP_EXE_N2_NO);
        gCcb.ulNormalTwMask = (1<<APP_EXE_E4_NO)|(1<<APP_EXE_E5_NO)|(1<<APP_EXE_E6_NO)|(1<<APP_EXE_E9_NO)|(1<<APP_EXE_C2_NO);
        gCcb.ulCirMask      = (1<<APP_EXE_E4_NO)|(1<<APP_EXE_E5_NO)|(1<<APP_EXE_E6_NO)|(1<<APP_EXE_E9_NO)|(1<<APP_EXE_C2_NO)|(1<<APP_EXE_N2_NO);

        gCcb.ulRunMask      = APP_EXE_INNER_SWITCHS & (~((1<<APP_EXE_E4_NO)|(1<<APP_EXE_E5_NO)|(1<<APP_EXE_E6_NO)|(1<<APP_EXE_E9_NO)|(1<<APP_EXE_C2_NO)|(1<<APP_EXE_N2_NO)));

        gCcb.ulPMMask       = APP_EXE_PM_REPORT_MASK;

        break;
    case MACHINE_L_EDI_LOOP:
    case MACHINE_L_RO_LOOP:
        gCcb.ulHyperTwMask  = 0;
        gCcb.ulNormalTwMask = (1<<APP_EXE_E4_NO)|(1<<APP_EXE_E6_NO)|(1<<APP_EXE_C2_NO);
        gCcb.ulCirMask      = (1<<APP_EXE_E4_NO)|(1<<APP_EXE_E6_NO)|(1<<APP_EXE_C2_NO)|(1<<APP_EXE_N2_NO);

        gCcb.ulRunMask      = APP_EXE_INNER_SWITCHS & (~((1<<APP_EXE_E4_NO)|(1<<APP_EXE_E6_NO)|(1<<APP_EXE_C2_NO)|(1<<APP_EXE_N2_NO)));

        gCcb.ulPMMask       = APP_EXE_PM_REPORT_MASK;

        break;
    case MACHINE_Genie:
        gCcb.ulHyperTwMask  = (1<<APP_EXE_E4_NO)|(1<<APP_EXE_E5_NO)|(1<<APP_EXE_E6_NO)|(1<<APP_EXE_E9_NO)|(1<<APP_EXE_C2_NO)|(1<<APP_EXE_N2_NO);
        gCcb.ulNormalTwMask = (1<<APP_EXE_E4_NO)|(1<<APP_EXE_E5_NO)|(1<<APP_EXE_E6_NO)|(1<<APP_EXE_E9_NO)|(1<<APP_EXE_C2_NO);
        gCcb.ulCirMask      = (1<<APP_EXE_E4_NO)|(1<<APP_EXE_E5_NO)|(1<<APP_EXE_E6_NO)|(1<<APP_EXE_E9_NO)|(1<<APP_EXE_C2_NO)|(1<<APP_EXE_N2_NO);

        gCcb.ulRunMask      = APP_EXE_INNER_SWITCHS & (~((1<<APP_EXE_E4_NO)|(1<<APP_EXE_E5_NO)|(1<<APP_EXE_E6_NO)|(1<<APP_EXE_E9_NO)|(1<<APP_EXE_C2_NO)|(1<<APP_EXE_N2_NO)));

        gCcb.ulPMMask       = (((1 << APP_EXE_PM1_NO)|(1 << APP_EXE_PM2_NO)) << APP_EXE_MAX_ECO_NUMBER);
        gCcb.bit1CirSpeedAdjust = TRUE;
        break;
    case MACHINE_UP:
        gCcb.ulHyperTwMask  = (1<<APP_EXE_E4_NO)|(1<<APP_EXE_E5_NO)|(1<<APP_EXE_E6_NO)|(1<<APP_EXE_E9_NO)|(1<<APP_EXE_C2_NO)|(1<<APP_EXE_N2_NO);
        gCcb.ulNormalTwMask = (1<<APP_EXE_E4_NO)|(1<<APP_EXE_E5_NO)|(1<<APP_EXE_E6_NO)|(1<<APP_EXE_E9_NO)|(1<<APP_EXE_C2_NO);
        gCcb.ulCirMask      = (1<<APP_EXE_E4_NO)|(1<<APP_EXE_E5_NO)|(1<<APP_EXE_E6_NO)|(1<<APP_EXE_E9_NO)|(1<<APP_EXE_C2_NO)|(1<<APP_EXE_N2_NO);

//        gCcb.ulRunMask      = APP_EXE_INNER_SWITCHS & (~((1<<APP_EXE_E4_NO)|(1<<APP_EXE_E5_NO)|(1<<APP_EXE_E6_NO)|(1<<APP_EXE_C2_NO)|(1<<APP_EXE_N2_NO)));
        gCcb.ulRunMask      = APP_EXE_INNER_SWITCHS & (~((1<<APP_EXE_E4_NO)|(1<<APP_EXE_E5_NO)|(1<<APP_EXE_E6_NO)|(1<<APP_EXE_E9_NO)|(1<<APP_EXE_C2_NO)|(1<<APP_EXE_N2_NO)));

        gCcb.ulPMMask       = (((1 << APP_EXE_PM1_NO)|(1 << APP_EXE_PM2_NO)) << APP_EXE_MAX_ECO_NUMBER);
        gCcb.bit1CirSpeedAdjust = TRUE;
        
        break;
    case MACHINE_EDI:
    case MACHINE_RO:
        gCcb.ulHyperTwMask  = 0;
        gCcb.ulNormalTwMask = (1<<APP_EXE_E4_NO)|(1<<APP_EXE_E6_NO)|(1<<APP_EXE_C2_NO);
        gCcb.ulCirMask      = (1<<APP_EXE_E4_NO)|(1<<APP_EXE_E6_NO)|(1<<APP_EXE_C2_NO);

        gCcb.ulRunMask      = APP_EXE_INNER_SWITCHS & (~((1<<APP_EXE_E4_NO)|(1<<APP_EXE_E6_NO)|(1<<APP_EXE_C2_NO)));

        gCcb.ulPMMask       = (((1 << APP_EXE_PM1_NO)|(1 << APP_EXE_PM2_NO)) << APP_EXE_MAX_ECO_NUMBER);
        gCcb.bit1CirSpeedAdjust = TRUE;
        
        break;
    case MACHINE_PURIST:
        gCcb.ulHyperTwMask  = (1<<APP_EXE_E4_NO)|(1<<APP_EXE_E5_NO)|(1<<APP_EXE_C2_NO)|(1<<APP_EXE_N2_NO)|(1<<APP_EXE_E9_NO);
        gCcb.ulNormalTwMask = 0;
        gCcb.ulCirMask      = (1<<APP_EXE_E4_NO)|(1<<APP_EXE_E5_NO)|(1<<APP_EXE_C2_NO)|(1<<APP_EXE_N2_NO)|(1<<APP_EXE_E9_NO);

        gCcb.ulRunMask      = 0;

        gCcb.ulPMMask       = ((1 << APP_EXE_PM2_NO) << APP_EXE_MAX_ECO_NUMBER);
        
        gCcb.bit1CirSpeedAdjust = TRUE;
        break;
    case MACHINE_ADAPT:
        gCcb.ulHyperTwMask  = APP_EXE_INNER_SWITCHS;
        gCcb.ulNormalTwMask = APP_EXE_INNER_SWITCHS;
        gCcb.ulCirMask      = APP_EXE_INNER_SWITCHS;
        gCcb.ulRunMask      = APP_EXE_INNER_SWITCHS ;

        gCcb.ulPMMask       = ((1 << APP_EXE_PM1_NO) << APP_EXE_MAX_ECO_NUMBER);
        
        gCcb.bit1CirSpeedAdjust = TRUE;
        break;
    default:  /* late implement */
        gCcb.ulHyperTwMask  = (1<<APP_EXE_E4_NO)|(1<<APP_EXE_E5_NO)|(1<<APP_EXE_E6_NO)|(1<<APP_EXE_E9_NO)|(1<<APP_EXE_C2_NO)|(1<<APP_EXE_N2_NO);
        gCcb.ulNormalTwMask = (1<<APP_EXE_E4_NO)|(1<<APP_EXE_E5_NO)|(1<<APP_EXE_E6_NO)|(1<<APP_EXE_E9_NO)|(1<<APP_EXE_C2_NO);
        gCcb.ulCirMask      = (1<<APP_EXE_E4_NO)|(1<<APP_EXE_E5_NO)|(1<<APP_EXE_E6_NO)|(1<<APP_EXE_E9_NO)|(1<<APP_EXE_C2_NO);

        gCcb.ulRunMask      = APP_EXE_INNER_SWITCHS & (~((1<<APP_EXE_E4_NO)|(1<<APP_EXE_E5_NO)|(1<<APP_EXE_E6_NO)|(1<<APP_EXE_E9_NO)|(1<<APP_EXE_C2_NO)|(1<<APP_EXE_N2_NO)));

        gCcb.ulPMMask       = APP_EXE_PM_REPORT_MASK;
        
        break;
    }

    /* for test purpose  */
}

void MainInitMsg(void)
{
   int iLoop;
   float fv; //2019.1.7
   memset(&gCcb,0,sizeof(gCcb));

   for (iLoop = 0; iLoop < WORK_LIST_NUM; iLoop++)
   {
       INIT_LIST_HEAD(&gCcb.WorkList[iLoop]);
   }
   
   for (iLoop = 0; iLoop < APP_EXE_VALVE_NUM; iLoop++)
   {
       gCcb.ExeBrd.aValveObjs[iLoop].emDispObjType = APP_OBJ_VALVE;
       gCcb.ExeBrd.aValveObjs[iLoop].iDispObjId    = iLoop;
       gCcb.ExeBrd.aValveObjs[iLoop].iVChoice      = APP_OBJ_VALUE_U32;
   }

   for (iLoop = 0; iLoop < APP_EXE_PRESSURE_METER; iLoop++)
   {
       gCcb.ExeBrd.aPMObjs[iLoop].emDispObjType = APP_OBJ_B;
       gCcb.ExeBrd.aPMObjs[iLoop].iDispObjId    = iLoop;
       gCcb.ExeBrd.aPMObjs[iLoop].iVChoice      = APP_OBJ_VALUE_U32;    /* ua */
   }

   for (iLoop = 0; iLoop < APP_EXE_G_PUMP_NUM; iLoop++)
   {
       gCcb.ExeBrd.aGPumpObjs[iLoop].emDispObjType = APP_OBJ_N_PUMP;
       gCcb.ExeBrd.aGPumpObjs[iLoop].iDispObjId    = iLoop;
       gCcb.ExeBrd.aGPumpObjs[iLoop].iVChoice      = APP_OBJ_VALUE_U32; /* ua */
   }

   for (iLoop = 0; iLoop < APP_EXE_R_PUMP_NUM; iLoop++)
   {
       gCcb.ExeBrd.aRPumpObjs[iLoop].emDispObjType = APP_OBJ_R_PUMP;
       gCcb.ExeBrd.aRPumpObjs[iLoop].iDispObjId    = iLoop;
       gCcb.ExeBrd.aRPumpObjs[iLoop].iVChoice      = APP_OBJ_VALUE_U32; 
   }

   for (iLoop = 0; iLoop < APP_EXE_RECT_NUM; iLoop++)
   {
       gCcb.ExeBrd.aRectObjs[iLoop].emDispObjType = APP_OBJ_RECT;
       gCcb.ExeBrd.aRectObjs[iLoop].iDispObjId    = iLoop;
       gCcb.ExeBrd.aRectObjs[iLoop].iVChoice      = APP_OBJ_VALUE_U32;
   }

   for (iLoop = 0; iLoop < APP_EXE_EDI_NUM; iLoop++)
   {
       gCcb.ExeBrd.aEDIObjs[iLoop].emDispObjType = APP_OBJ_EDI;
       gCcb.ExeBrd.aEDIObjs[iLoop].iDispObjId    = iLoop;
       gCcb.ExeBrd.aEDIObjs[iLoop].iVChoice      = APP_OBJ_VALUE_U32;
   }

   for (iLoop = 0; iLoop < APP_EXE_ECO_NUM; iLoop++)
   {
       gCcb.ExeBrd.aEcoObjs[iLoop].emDispObjType = APP_OBJ_I;
       gCcb.ExeBrd.aEcoObjs[iLoop].iDispObjId    = iLoop;
       gCcb.ExeBrd.aEcoObjs[iLoop].iVChoice      = APP_OBJ_VALUE_CUST;
   }

   for (iLoop = 0; iLoop < APP_FM_FLOW_METER_NUM; iLoop++)
   {
       gCcb.FlowMeter.aFmObjs[iLoop].emDispObjType = APP_OBJ_S;
       gCcb.FlowMeter.aFmObjs[iLoop].iDispObjId    = iLoop;
       gCcb.FlowMeter.aFmObjs[iLoop].iVChoice      = APP_OBJ_VALUE_U32;

       gCcb.FlowMeter.aHist[iLoop].ulSec           = gulSecond;
       gCcb.FlowMeter.aHist[iLoop].iVChoice        = APP_OBJ_VALUE_U32;
       gCcb.FlowMeter.aHist[iLoop].curValue.ulV    = INVALID_FM_VALUE;
       gCcb.FlowMeter.aHist[iLoop].lstValue.ulV    = INVALID_FM_VALUE;

       gCcb.FlowMeter.aulHistTotal[iLoop]          = 0;
   }

   for (iLoop = 0; iLoop < PUMP_SPEED_NUM; iLoop++)
   {
       //2019.1.7
       if(10 <= iLoop)
       {
           fv = 24.0;
       }
       else if(iLoop == 9)
       {
           fv = 18.0;
       }
       else if(iLoop == 8)
       {
           fv = 15.0;
       }
       else
       {
           fv = iLoop + 6.0;
       }
       gCcb.aiSpeed2ScaleMap[iLoop] = DispConvertVoltage2RPumpSpeed(fv);
       //end
       //gCcb.aiSpeed2ScaleMap[iLoop] = DispConvertVoltage2RPumpSpeed(((24.0 - 5.0)*iLoop)/PUMP_SPEED_NUM + 5);
   }
   
   // for ccb init
   gCcb.curWorkState.iMainWorkState = DISP_WORK_STATE_IDLE;
   gCcb.curWorkState.iSubWorkState  = DISP_WORK_SUB_IDLE;

   gCcb.curWorkState.iMainWorkState4Pw = DISP_WORK_STATE_IDLE;
   gCcb.curWorkState.iSubWorkState4Pw  = DISP_WORK_SUB_IDLE;

   gCcb.ExeBrd.ucDinState = 0XFF;

   gCcb.ulAdapterAgingCount = 0XFFFFFF00;

   /* for other init */
   MainInitInnerIpc();

   CcbGetParamNotify(NOT_PARAM_ALL_PARAM); /* GET ALL */

   for (iLoop = 0; iLoop < WORK_LIST_NUM + 1; iLoop++)
   {
       sp_thread_sem_init(&gCcb.Sem4Delay[iLoop],0,1); /* binary semphone */
   }

   sp_thread_mutex_init(&gCcb.WorkMutex,NULL);
   
   sp_thread_mutex_init(&gCcb.ModbusMutex,NULL);


   // set reset to all sub modulers
   MainResetModulers();
}

void MainDeInitMsg(void)
{
    int iLoop;
    MainDeinitInnerIpc();

    for (iLoop = 0; iLoop < WORK_LIST_NUM + 1; iLoop++)
    {
        sp_thread_sem_destroy(&gCcb.Sem4Delay[iLoop]);
    }

    sp_thread_mutex_destroy(&gCcb.WorkMutex);

    sp_thread_mutex_destroy(&gCcb.ModbusMutex);

}

void Main_timer_handler(void *arg)
{
    int id = (int )arg;
    int ret;
    TIMER_MSG_STRU *tm = (TIMER_MSG_STRU *)SatAllocMsg(TIMER_MSG_MSG_SIZE);//malloc(sizeof(TIMER_MSG_STRU));
    if (tm)
    {
        tm->id = id;
        tm->msgHead.event = TIMER_MSG;
        ret = VOS_SndMsg(TASK_HANDLE_MAIN,(SAT_MSG_HEAD *)tm);
        if (0 != ret)
        {
        }
    }
}

void MainHeartBeatProc()
{
    int iLoop;
    if (gCcb.ulRegisterMask)
    {
       gCcb.iHbtCnt = (gCcb.iHbtCnt + 1) % (HEART_BEAT_PERIOD - HEART_BEAT_MARGIN);

       gCcb.ulActiveShadowMask = gCcb.ulActiveMask;

       if (0 == gCcb.iHbtCnt)
       {
          for (iLoop = 0; iLoop < 32; iLoop++)
          {
              if ((1 << iLoop) & (gCcb.ulRegisterMask))
              {
                  if ((1 << iLoop) & (gCcb.ulActiveMask))
                  {
                      if (gCcb.aucHbCounts[iLoop] > 0)
                      {
                          gCcb.aucHbCounts[iLoop]-- ;
                      }

                      if (!gCcb.aucHbCounts[iLoop])
                      {
                          gCcb.ulActiveMask &= ~(1 << iLoop);
                          gCcb.aulActMask4Trx[APP_TRX_CAN] &= ~(1 << iLoop);
                          gCcb.aulActMask4Trx[APP_TRX_ZIGBEE] &= ~(1 << iLoop);
                          
                          gCcb.ulActiveMask4HbCheck |= (1 << iLoop);
                      }
                      continue;
                  }
                      /* lost heart beat , raise alarm !*/
                  switch(iLoop)
                  {
                  case APP_PAKCET_ADDRESS_EXE:
                       if (gCcb.ulActiveMask4HbCheck & (1 << iLoop))
                       {
                           gCcb.ulActiveMask4HbCheck &= ~(1 << iLoop);
                           CcbExeBrdNotify();
                       }
                       break;
                   case APP_PAKCET_ADDRESS_FM:
                        if (gCcb.ulActiveMask4HbCheck & (1 << iLoop))
                        {
                            gCcb.ulActiveMask4HbCheck &= ~(1 << iLoop);
                            CcbFmBrdNotify();
                        }
                        break;
                    case APP_HAND_SET_BEGIN_ADDRESS ... APP_HAND_SET_END_ADDRESS:
                        if (gCcb.ulActiveMask4HbCheck & (1 << iLoop))
                        {
                            gCcb.ulActiveMask4HbCheck &= ~(1 << iLoop);
                            gCcb.aHandler[iLoop - APP_HAND_SET_BEGIN_ADDRESS].bit1Active = 0;
                            gCcb.aHandler[iLoop - APP_HAND_SET_BEGIN_ADDRESS].iTrxMap  = 0;                           
                            CcbHandlerNotify(iLoop - APP_HAND_SET_BEGIN_ADDRESS);
                        }
                        break;
                    case APP_RF_READER_BEGIN_ADDRESS ... APP_RF_READER_END_ADDRESS:
                        if (gCcb.ulActiveMask4HbCheck & (1 << iLoop))
                        {
                            gCcb.ulActiveMask4HbCheck &= ~(1 << iLoop);
                            gCcb.aRfReader[iLoop - APP_RF_READER_BEGIN_ADDRESS].bit1Active = 0;
                            CcbRfidNotify(iLoop - APP_RF_READER_BEGIN_ADDRESS);
                        }
                        break;
                        
                        
                  }
              }
          }
          MainSndHeartBeat();
       }
    } 
}

void CcbAddExeSwitchWork(WORK_SETUP_REPORT_STRU *pRpt )
{
    if (!SearchWork(work_switch_setup_exe))
    {
        gCcb.WorkSwitchSetParam4Exe.ulMask = pRpt->ulMask;
        gCcb.WorkSwitchSetParam4Exe.ulValue = pRpt->ulValue;
        CcbInnerWorkSetupExeSwitch();
    }                

}


void CcbAddExeReportWork(WORK_SETUP_REPORT_STRU *pRpt )
{
    if (!SearchWork(work_rpt_setup_exe))
    {
        gCcb.WorkRptSetParam4Exe.ulMask = pRpt->ulMask;
        gCcb.WorkRptSetParam4Exe.ulValue = pRpt->ulValue;
        CcbInnerWorkSetupExeReport();
    }                

}

void CcbAddFmReportWork(WORK_SETUP_REPORT_STRU *pRpt )
{
    if (!SearchWork(work_rpt_setup_fm))
    {
        gCcb.WorkRptSetParam4Fm.ulMask = pRpt->ulMask;
        gCcb.WorkRptSetParam4Fm.ulValue = pRpt->ulValue;
        CcbInnerWorkSetupFmReport();
    }                

}

void MainSecondTask4MainState()
{
    int iLoop;

    printf("check Ro Alarm: 00\r\n");
    switch(gCcb.curWorkState.iMainWorkState)
    {
    case DISP_WORK_STATE_PREPARE:
        gCcb.iInitRunTimer++;
        break;
    case DISP_WORK_STATE_IDLE:
        {
            switch(gCcb.curWorkState.iSubWorkState)
            {
            case DISP_WORK_SUB_IDLE:
                if (gCcb.bit1NeedFillTank
                    && !gCcb.bit1FillingTank
                    && haveB3(&gCcb))
                {
                    if (!SearchWork(work_start_fill_water))
                    {
                        CcbInnerWorkStartFillWater();
                    }                
                }
                break;
            default:
                break;
            }
        }
        break;
    case DISP_WORK_STATE_LPP:
        if (gulSecond - gCcb.ulLPPTick >= (gCcb.TMParam.aulTime[TIME_PARAM_LPP] / 1000))
        {
            /* move to init run */
            if (!SearchWork(work_init_run))
            {
                CcbInnerWorkInitRun();
            }                
            
        }
        break;
    case DISP_WORK_STATE_RUN:
        switch(gCcb.ulMachineType)
        {
        case MACHINE_L_Genie:
        case MACHINE_L_EDI_LOOP:
        case MACHINE_L_RO_LOOP:
        case MACHINE_Genie:
        case MACHINE_UP: //2018.10.24 add
        case MACHINE_RO: //2018.11.12 add , Automatic recovery of water production
        case MACHINE_EDI:
            if (gCcb.bit1B2Full)
            {
                // check report flag
                if (!CcbGetPmObjState(1 << APP_EXE_PM2_NO))
                {
                   /* Active Report Flag */
                   WORK_SETUP_REPORT_STRU Rpt;
                   Rpt.ulMask  = MAKE_B_MASK((1 << APP_EXE_PM2_NO));
                   Rpt.ulValue = MAKE_B_MASK((1 << APP_EXE_PM2_NO));
                   
                   VOS_LOG(VOS_LOG_WARNING,"CcbAddExeReportWork 3");    
                   
                   CcbAddExeReportWork(&Rpt);
                }
                else
                {
                   /* check pressure */
                   if (!(CcbConvert2Pm2SP(gCcb.ExeBrd.aPMObjs[APP_EXE_PM2_NO].Value.ulV) >= B2_FULL))
                   {
                       gCcb.bit1B2Full = FALSE;
                   }
    
                }
    
                if (gCcb.bit1B2Full)
                {
                    if (!gCcb.bit1PeriodFlushing)
                    {
                        if (gulSecond - gCcb.ulB2FullTick >= gCcb.TMParam.aulTime[TIME_PARAM_NormRunT5]/1000)
                        {
                            if (!SearchWork(work_tank_start_full_loop))
                            {
                                CcbInnerWorkStartTankFullLoop();
                            }                
                        }
                    }
                    else
                    {
                        VOS_LOG(VOS_LOG_WARNING,"TF %d:%d",gCcb.TMParam.aulTime[TIME_PARAM_NormRunT4]/1000,gulSecond - gCcb.ulB2FullTick);    
                    }
                }
                
            }
            else
            {
                /* check SP5 */
                {
                   /* check pressure */
                    if (!gCcb.bit1ProduceWater)
                    {
                       //2019.09.29 add
                        if(haveB3(&gCcb))
                        {
                            if ((CcbConvert2Pm2SP(gCcb.ExeBrd.aPMObjs[APP_EXE_PM2_NO].Value.ulV) < CcbGetSp5())
                                 && (CcbConvert2Pm3SP(gCcb.ExeBrd.aPMObjs[APP_EXE_PM3_NO].Value.ulV) > 50.0))
                            {
                                /* start Nomal Run */
                                if (DISP_WORK_SUB_IDLE == gCcb.curWorkState.iSubWorkState)
                                {
                                    if (!SearchWork(work_normal_run))
                                    {
                                        CcbInnerWorkRun();
                                    }
                                }
                            }
                        }//end 2019.9.29
                        else
                        {
                            if (CcbConvert2Pm2SP(gCcb.ExeBrd.aPMObjs[APP_EXE_PM2_NO].Value.ulV) < CcbGetSp5())
                            {
                                /* start Nomal Run */
                                if (DISP_WORK_SUB_IDLE == gCcb.curWorkState.iSubWorkState)
                                {
                                    if (!SearchWork(work_normal_run))
                                    {
                                        CcbInnerWorkRun();
                                    }
                                }
                            }
                        }
                   }
                }
            }   

            if (gCcb.bit1PeriodFlushing)
            {
                if (gulSecond - gCcb.ulB2FullTick >= gCcb.TMParam.aulTime[TIME_PARAM_NormRunT4]/1000)
                {
                    if (!SearchWork(work_tank_stop_full_loop))
                    {
                        CcbInnerWorkStopTankFullLoop();
                    }                
                }
            }
            break;
        default:
            break;
        }

        /*Check B3 */
        if (!CcbGetPmObjState(1 << APP_EXE_PM3_NO)
            && haveB3(&gCcb))
        {
           /* Active Report Flag */
           WORK_SETUP_REPORT_STRU Rpt;
           Rpt.ulMask  = MAKE_B_MASK((1 << APP_EXE_PM3_NO));
           Rpt.ulValue = MAKE_B_MASK((1 << APP_EXE_PM3_NO));
           
           CcbAddExeReportWork(&Rpt);
        }

        if (gCcb.bit1B1Check4RuningState 
            && gCcb.bit1B1UnderPressureDetected)
        {
            //if (gulSecond - gCcb.ulB1UnderPressureTick >= 60)
            if (gulSecond - gCcb.ulB1UnderPressureTick >= DEFAULT_LPP_CHECK_NUMBER)
            {
                //2019.11.08 add for adpet, 
                //TOTEST
                if(MACHINE_ADAPT == gCcb.ulMachineType)
                {
                    for(iLoop = 0; iLoop < MAX_HANDLER_NUM; iLoop++)
                    {
                        if (gCcb.aHandler[iLoop].bit1Qtw)
                        {
                            CcbInnerWorkStopQtw(iLoop);
                        }
                        if (gCcb.aHandler[iLoop].bit1PendingQtw)
                        {
                            DISPHANDLE hdl ;
                            hdl = SearchWork(work_start_qtw);
                            if (hdl)
                            {
                                CcbCancelWork(hdl);
                            }
                            gCcb.aHandler[iLoop].bit1PendingQtw = 0;
                        }
                    }
                } 

                /* move to LPP state */
                if (!SearchWork(work_start_lpp))
                {
                    CcbInnerWorkLpp();
                }   

               /* Cancel Running task */
               {
                    DISPHANDLE hdl ;        
                    hdl = SearchWork(work_init_run);

                    if (hdl)
                    {
                        CcbCancelWork(hdl);
                    }

                    hdl = SearchWork(work_normal_run);

                    if (hdl)
                    {
                        CcbCancelWork(hdl);
                    }                   
               }
            }
        }

        /* 2018/01/15 add begin*/
        switch(gCcb.ulMachineType)
        {
        case MACHINE_L_Genie:
        case MACHINE_L_EDI_LOOP:        
            if (gCcb.bit1AlarmROPLV)
            {
                unsigned int ulMask =  (1 << APP_EXE_T1_NO);
                
                if (CcbGetSwitchObjState(ulMask))
                {
                    WORK_SETUP_SWITCH_STRU Rpt;
                    Rpt.ulMask  = ulMask;
                    Rpt.ulValue = 0;
                    
                    CcbAddExeSwitchWork(&Rpt);
                }
            }
            else
            {
                if (gCcb.bit1ProduceWater)
                {
                    unsigned int ulMask =  (1 << APP_EXE_T1_NO);
                    
                    if (!CcbGetSwitchObjState(ulMask))
                    {
                        WORK_SETUP_SWITCH_STRU Rpt;
                        Rpt.ulMask  = ulMask;
                        Rpt.ulValue = ulMask;
                        
                        CcbAddExeSwitchWork(&Rpt);
                    }
                }
            }
            break;
         }
        /*2018/01/15 add end*/

        break;
    }

    /* 2018/01/11 begin : always enable B1 report according to DU */
    if (gCcb.SubModSetting.ulFlags & (1 << DISP_SM_HaveB2))
    {
        if (gCcb.ulActiveMask   & (1 << APP_PAKCET_ADDRESS_EXE))
        {
            if (!CcbGetPmObjState((1 << APP_EXE_PM2_NO)))
            {
                WORK_SETUP_REPORT_STRU Rpt;
                Rpt.ulMask  = MAKE_B_MASK((1 << APP_EXE_PM2_NO));
                Rpt.ulValue = MAKE_B_MASK((1 << APP_EXE_PM2_NO));
                CcbAddExeReportWork(&Rpt);
            }
        }
    }
    /* 2018/01/11 end */

	//2019.12.2 增加原水箱压力检测检查
    if (!CcbGetPmObjState((1 << APP_EXE_PM3_NO)))
    {
        WORK_SETUP_REPORT_STRU Rpt;
        Rpt.ulMask  = MAKE_B_MASK((1 << APP_EXE_PM3_NO));
        Rpt.ulValue = MAKE_B_MASK((1 << APP_EXE_PM3_NO));
        
        CcbAddExeReportWork(&Rpt);
    }

    /* independent work to fill tank */
    if (gCcb.bit1FillingTank)
    {
        /* check valve state */
        if (!CcbGetSwitchObjState((1 << APP_EXE_E10_NO)))
        {
            WORK_SETUP_SWITCH_STRU Rpt;
            Rpt.ulMask  = (1 << APP_EXE_E10_NO);
            Rpt.ulValue = (1 << APP_EXE_E10_NO);
            
            CcbAddExeSwitchWork(&Rpt);
        }
        
        /* check pm state */
        if (!CcbGetPmObjState((1 << APP_EXE_PM3_NO)))
        {
            WORK_SETUP_REPORT_STRU Rpt;
            Rpt.ulMask  = MAKE_B_MASK((1 << APP_EXE_PM3_NO));
            Rpt.ulValue = MAKE_B_MASK((1 << APP_EXE_PM3_NO));
            
            CcbAddExeReportWork(&Rpt);
        }

    }

}

void MainSecondTask4Pw()
{

    switch(gCcb.curWorkState.iMainWorkState4Pw)
    {
    case DISP_WORK_STATE_RUN:
        
        switch(gCcb.curWorkState.iSubWorkState4Pw)
        {
        case DISP_WORK_SUB_IDLE:
            if (gulSecond - gCcb.ulLstCirTick >= (DEFAULT_IDLECIRTIME / 1000))
            {
                if (haveHPCir(&gCcb))
                {
                    if (!SearchWork(work_start_cir))
                    {
                        CcbInnerWorkStartCir(CIR_TYPE_HP);
                    } 
                }
            }
               
            break;
        case DISP_WORK_SUB_RUN_QTW:
            if (!CcbGetFmObjState((1 << APP_FM_FM3_NO)))
            {
                WORK_SETUP_REPORT_STRU Rpt;
                Rpt.ulMask  = (1 << APP_FM_FM3_NO);
                Rpt.ulValue = (1 << APP_FM_FM3_NO);
                CcbAddFmReportWork(&Rpt);
            }
            if (gCcb.SubModSetting.ulFlags & (1 << DISP_SM_HaveB2))
            {
                if (!CcbGetPmObjState((1 << APP_EXE_PM2_NO)))
                {
                    WORK_SETUP_REPORT_STRU Rpt;
                    Rpt.ulMask  = (1 << APP_EXE_PM2_NO);
                    Rpt.ulValue = (1 << APP_EXE_PM2_NO);
                    CcbAddExeReportWork(&Rpt);
                } 
            }
            break;
        case DISP_WORK_SUB_RUN_CIR:
            {
                int ulDuration = 0;

                switch(gCcb.iCirType)
                {
                case CIR_TYPE_UP:
                    /* toc here if toc is configured */
                    if (haveTOC(&gCcb))
                    {
                        gCcb.iTocStageTimer++;
                        switch(gCcb.iTocStage)
                        {
                        case APP_PACKET_EXE_TOC_STAGE_FLUSH1:
                        {
                            if(gCcb.iTocStageTimer == 150)
                            {
                                Check_TOC_Alarm = 1;
                            }
                            if (gCcb.iTocStageTimer >= 160)
                            {
                                Check_TOC_Alarm = 0;
                                if (!SearchWork(work_start_toc_cir))
                                {
                                    CcbInnerWorkStartCirToc(APP_PACKET_EXE_TOC_STAGE_OXDIZATION);
                                }   
                            }
                            break;
                        }
                        case APP_PACKET_EXE_TOC_STAGE_OXDIZATION:
                            if (gCcb.iTocStageTimer >= 180)
                            {
                                if (!SearchWork(work_start_toc_cir))
                                {
                                    CcbInnerWorkStartCirToc(APP_PACKET_EXE_TOC_STAGE_FLUSH2);
                                }   
                            }
                            break;
                        case APP_PACKET_EXE_TOC_STAGE_FLUSH2:
                            if (gCcb.iTocStageTimer >= 20)
                            {
                                if (!SearchWork(work_stop_cir))
                                {
                                    CcbInnerWorkStopCir();
                                }   
                            }
                            break;
                            
                        }

                        /* check toc alarm */
                    }
                    else
                    {
                        ulDuration = (gCcb.TMParam.aulTime[TIME_PARAM_InterCirDuration]/1000);                    
                        if (gulSecond - gCcb.ulCirTick >= (unsigned int)ulDuration)
                        {
                            if (!SearchWork(work_stop_cir))
                            {
                                CcbInnerWorkStopCir();
                            }   
                        }     
                    }
                    break;
                case CIR_TYPE_HP:
                    ulDuration = 60*60;                    
                    if (gulSecond - gCcb.ulCirTick >= (unsigned int)ulDuration)
                    {
                        if (!SearchWork(work_stop_cir))
                        {
                            CcbInnerWorkStopCir();
                        }   
                    }                    
                    break;

                }

                /* check I5 & SP7*/
                if (!CcbGetIObjState((1 << APP_EXE_I5_NO)))
                {
                    WORK_SETUP_REPORT_STRU Rpt;
                    
                    Rpt.ulMask  = (1 << APP_EXE_I5_NO);
                    Rpt.ulValue = (1 << APP_EXE_I5_NO);
                
                    CcbAddExeReportWork(&Rpt);
                }
                VOS_LOG(VOS_LOG_WARNING,"DISP_WORK_SUB_RUN_CIR %d:%d:%d",gCcb.iCirType,ulDuration,gulSecond - gCcb.ulCirTick);    
                
            }
            break;
        default:
            break;
        } 
        
        gCcb.ulN3PeriodTimer++;

        if (gCcb.ulN3PeriodTimer % (60*60) == 0)
        {
            /* start N3 */
            if (!SearchWork(work_start_N3))
            {
                CcbInnerWorkStartN3();
            }                
        }

        if (gCcb.bit1N3Active)
        {
            gCcb.ulN3Duration++;
            if (gCcb.ulN3Duration >= (gCcb.MiscParam.iTankUvOnTime * 60))
            {
                if (!SearchWork(work_stop_N3))
                {
                    CcbInnerWorkStopN3();
                }              
            }
        }

        if (gCcb.bit1NeedTubeCir)
        {
            if (!gCcb.bit1TubeCirOngoing)
            {
                if (!SearchWork(work_start_tube_cir))
                {
                    CcbInnerWorkStartTubeCir();
                    VOS_LOG(VOS_LOG_DEBUG,"CcbInnerWorkStartTubeCir %d:%d:%d",gCcb.iCirType,gCcb.bit1NeedTubeCir,gCcb.bit1TubeCirOngoing);    
                }  
            }
            else
            {
                gCcb.ulTubeCirTick++;
                if (gCcb.ulTubeCirTick >= (unsigned int)gCcb.MiscParam.iTubeCirDuration * 60) /* Minute to second */
                {
                    if (!SearchWork(work_stop_tube_cir))
                    {
                        CcbInnerWorkStopTubeCir();
                    }                      
                }
            }
            
        }          
        break;
    }

}

//ex
void Ex_DispDecPressure()
{
    if((gCcb.curWorkState.iMainWorkState4Pw == DISP_WORK_STATE_IDLE)
        &&(gCcb.curWorkState.iSubWorkState4Pw ==DISP_WORK_SUB_IDLE_DEPRESSURE)
        &&(ex_gCcb.EX_Check_State.bit1CheckDecPressure == 1))
    {
        if((ex_gulSecond - ex_gCcb.Ex_Delay_Tick.ulDecPressure) > 30)
        {
            int     iType = APP_DEV_HS_SUB_NUM;
            CcbNotDecPressure(iType, 0);
        }
    }
}
//end


void MainSecondTask()
{
    gulSecond++;
    ex_gulSecond++; //ex

    mem_free_auto();
    
    //printf("MainSecondTask \r\n");
    MainHeartBeatProc();

    MainSecondTask4MainState();

    MainSecondTask4Pw();

    Ex_DispDecPressure(); //ex

}


void MainMinuteTask()
{
    CcbNvStaticsNotify();
    CcbPumpStaticsNotify();
    CcbFmStaticsNotify();

}

void MainProcTimerMsg(SAT_MSG_HEAD *pMsg)
{
    TIMER_MSG_STRU *pTimerMsg = (TIMER_MSG_STRU *)(pMsg);

    switch(pTimerMsg->id)
    {
    case TIMER_SECOND:
        MainSecondTask();
        break;
    case TIMER_MINUTE:
        MainMinuteTask();
        break;
    default:
        break;
    }
}


void CcbParamMsgProc(SAT_MSG_HEAD *pucMsg)
{
    MAIN_PARAM_MSG_STRU *pWorkMsg = (MAIN_PARAM_MSG_STRU *)pucMsg;


    switch(pWorkMsg->iSubMsgType)
    {
    case NOT_PARAM_ALL_PARAM:
    case NOT_PARAM_MACHINE_TYPE:
        {
           DISP_GLOBAL_PARAM_STRU *pCnt = (DISP_GLOBAL_PARAM_STRU *)pWorkMsg->aucData;

           CcbSyncSetMachineType(pCnt->iMachineType);
           CcbSyncSetMachineParams(&pCnt->MMParam);
           CcbSyncSetModuleParams(&pCnt->SubModSetting);
           CcbSyncSetAlarmParams(&pCnt->AlarmSettting);
           CcbSyncSetTimeParams(&pCnt->TMParam);
           CcbSyncSetConsumeMaterialParams(&pCnt->CMParam);
           CcbSyncSetFmParams(&pCnt->FmParam);
           CcbSyncSetPmParams(&pCnt->PmParam);
           CcbSyncSetMiscParams(&pCnt->MiscParam);
           
           CcbInitMachineType();
        }
        break;
    case NOT_PARAM_MACHINE_PARAM:
        {
           DISP_MACHINE_PARAM_STRU *pCnt = (DISP_MACHINE_PARAM_STRU *)pWorkMsg->aucData;

           CcbSyncSetMachineParams(pCnt);
           
        }
        break;
    case NOT_PARAM_SUBMODULE_PARAM:
        {
           DISP_SUB_MODULE_SETTING_STRU *pCnt = (DISP_SUB_MODULE_SETTING_STRU *)pWorkMsg->aucData;
    
           CcbSyncSetModuleParams(pCnt);
        }
        break;
    case NOT_PARAM_ALARM_PARAM:
        {
           DISP_ALARM_SETTING_STRU *pCnt = (DISP_ALARM_SETTING_STRU *)pWorkMsg->aucData;
    
           CcbSyncSetAlarmParams(pCnt);
        }
        break;
    case NOT_PARAM_TIME_PARAM:
        {
           DISP_TIME_PARAM_STRU *pCnt = (DISP_TIME_PARAM_STRU *)pWorkMsg->aucData;
    
           CcbSyncSetTimeParams(pCnt);
        }
        break;
    case NOT_PARAM_CM_PARAM:
        {
           DISP_CONSUME_MATERIAL_STRU *pCnt = (DISP_CONSUME_MATERIAL_STRU *)pWorkMsg->aucData;
    
           CcbSyncSetConsumeMaterialParams(pCnt);
        }
        break;
    case NOT_PARAM_FM_PARAM:
        {
            DISP_PM_SETTING_STRU *pCnt = (DISP_PM_SETTING_STRU *)pWorkMsg->aucData;
            
            CcbSyncSetPmParams(pCnt);
        }
        break;
    case NOT_PARAM_PM_PARAM:
        {
            DISP_FM_SETTING_STRU *pCnt = (DISP_FM_SETTING_STRU *)pWorkMsg->aucData;
            
            CcbSyncSetFmParams(pCnt);
        }
        break;
    case NOT_PARAM_MISC_PARAM:
        {
            DISP_MISC_SETTING_STRU *pCnt = (DISP_MISC_SETTING_STRU *)pWorkMsg->aucData;
            
            CcbSyncSetMiscParams(pCnt);
        }
        break;
    default :
        break;
    }

}


void MainMsgProc(void *para, SAT_MSG_HEAD *pMsg)
{
    switch(pMsg->event)
    {
    case INIT_ALL_THREAD_EVENT:
        MainInitMsg();
        break;
    case TIMER_MSG:
        MainProcTimerMsg(pMsg);
        break;
    case CANITF_MAIN_MSG:
        CanCcbCanItfMsg(pMsg);
        break;
    case QUIT_ALL_THREAD_EVENT:
        MainDeInitMsg();
        break;
    case MAIN_WORK_MSG:
        CcbWorMsgProc(pMsg);
        break;
    case MAIN_PARAM_MSG:
        CcbParamMsgProc(pMsg);
        break;
    case ZBITF_MAIN_MSG:
        if (haveZigbee(&gCcb))
        {
            CanCcbZigbeeItfMsg(pMsg);
        }
        break;
    case ZBMAIN_NODE_REPORT_MSG:
        CanCcbZigbeeReportMsg(pMsg);
        break;
    case ZBITF_MAIN_MTN_MSG:
        CanCcbZbMainMtnMsg(pMsg);
        break;
    default:
        break;
   }
}


void CanMsgMsgProc(void *para,SAT_MSG_HEAD *pMsg)
{
    switch(pMsg->event)
    {
    case CANITF_MAIN_MSG:
        CanCcbCanItfMsg(pMsg);
        break;
    default:
        break;
   }
}

void CcbAsyncSetParams(int iType,void *pParam)
{
    /* snd message */
    int iMsgLen  = 0;

    switch(iType)
    {
    case NOT_PARAM_MACHINE_TYPE:
    case NOT_PARAM_ALL_PARAM:
        iMsgLen = sizeof(DISP_GLOBAL_PARAM_STRU);
        break;
    case NOT_PARAM_MACHINE_PARAM:
        iMsgLen = sizeof(DISP_MACHINE_PARAM_STRU);
        break;
    case NOT_PARAM_SUBMODULE_PARAM:
        iMsgLen = sizeof(DISP_SUB_MODULE_SETTING_STRU);
        break;
    case NOT_PARAM_ALARM_PARAM:
        iMsgLen = sizeof(DISP_ALARM_SETTING_STRU);
        break;
    case NOT_PARAM_TIME_PARAM:
        iMsgLen = sizeof(DISP_TIME_PARAM_STRU);
        break;
    case NOT_PARAM_CM_PARAM:
        iMsgLen = sizeof(DISP_CONSUME_MATERIAL_STRU);
        break;
    case NOT_PARAM_FM_PARAM:
        iMsgLen = sizeof(DISP_FM_SETTING_STRU);
        break;
    case NOT_PARAM_PM_PARAM:
        iMsgLen = sizeof(DISP_PM_SETTING_STRU);
        break;
    case NOT_PARAM_MISC_PARAM:
        iMsgLen = sizeof(DISP_MISC_SETTING_STRU);
        break;
    default:
        return ;
    }
    
    {
        int iRet;
        MAIN_PARAM_MSG_STRU *pMsg = (MAIN_PARAM_MSG_STRU *)SatAllocMsg(MAIN_PARAM_MSG_SIZE + iMsgLen);
        if (NULL == pMsg)
        {
            return ;
        }
        pMsg->msgHead.event  = MAIN_PARAM_MSG;
        pMsg->iSubMsgType    = iType;
        pMsg->iMsgLen        = iMsgLen;

        memcpy(pMsg->aucData,pParam,pMsg->iMsgLen);
        
        iRet = VOS_SndMsg(TASK_HANDLE_MAIN,(SAT_MSG_HEAD *)pMsg);
        
        if (0 != iRet)
        {
        }        
    }
}

void CcbSyncSetMachineType(int iMachiType)
{
    gCcb.ulMachineType = iMachiType;
    
}

void CcbSyncSetMachineParams(DISP_MACHINE_PARAM_STRU *pParam)
{
    gCcb.MMParam       = *pParam;
    
}

void CcbSyncSetModuleParams(DISP_SUB_MODULE_SETTING_STRU *pParam)
{
    gCcb.SubModSetting = *pParam;

    if (!haveB3(&gCcb))
    {
        gCcb.bit1NeedFillTank = 0;
        gCcb.bit1FillingTank  = 0;
    }
}

void CcbSyncSetAlarmParams(DISP_ALARM_SETTING_STRU *pParam)
{
    gCcb.AlarmSettting = *pParam;
}

void CcbSyncSetTimeParams(DISP_TIME_PARAM_STRU *pParam)
{
    gCcb.TMParam       = *pParam;
}

void CcbSyncSetConsumeMaterialParams(DISP_CONSUME_MATERIAL_STRU *pParam)
{
    gCcb.CMParam       = *pParam;
}

void CcbSyncSetFmParams(DISP_FM_SETTING_STRU *pParam)
{
    gCcb.FMParam       = *pParam;
}

void CcbSyncSetPmParams(DISP_PM_SETTING_STRU *pParam)
{
    gCcb.PMParam       = *pParam;
}

void CcbSyncSetMiscParams(DISP_MISC_SETTING_STRU *pParam)
{
    gCcb.MiscParam       = *pParam;
}

void CcbInit(void)
{
    //int err = 0;

    VOS_SetLogLevel(VOS_LOG_DEBUG);

    Task_init(15);

    TASK_HANDLE_MAIN      = Task_CreateMessageTask(MainMsgProc,NULL,NULL,&gCcb);
    TASK_HANDLE_CANITF    = Task_CreateMessageTask(CanItfMsgProc,NULL,NULL,&gCanItf[0]);
    TASK_HANDLE_MOCAN     = Task_CreateMessageTask(CanMsgMsgProc,NULL,NULL,&gCcb);
    TASK_HANDLE_ZB        = Task_CreateMessageTask(zbMsgProc,NULL,NULL,NULL);

    TimerInit();

    // start period check timer
    timer_add(1000   ,TIMER_PERIOD ,Main_timer_handler,(void *)TIMER_SECOND);

    timer_add(1000*60,TIMER_PERIOD ,Main_timer_handler,(void *)TIMER_MINUTE);

    Task_DispatchWorkTask(TimerProc,&gTps);

    // send init message to Main Proc
    VOS_SndMsg2(TASK_HANDLE_MAIN    ,INIT_ALL_THREAD_EVENT,0,NULL);
    VOS_SndMsg2(TASK_HANDLE_CANITF  ,INIT_ALL_THREAD_EVENT,0,NULL);
    VOS_SndMsg2(TASK_HANDLE_MOCAN   ,INIT_ALL_THREAD_EVENT,0,NULL);
    VOS_SndMsg2(TASK_HANDLE_ZB      ,INIT_ALL_THREAD_EVENT,0,NULL);
}

//质量检验主板用
int Ex_FactoryTest(int select)
{
    int iTmp, iRet;
    switch(select)
    {
    case 0:
    {
        iTmp  = (1 << APP_FM_FM1_NO)|(1 << APP_FM_FM2_NO)|(1<<APP_FM_FM3_NO)|(1<<APP_FM_FM4_NO);
        iRet = CcbUpdateFms(WORK_LIST_LP, 0, iTmp, iTmp);
        if (iRet )
        {
            return 1;
        }
        break;
    }
    case 1:
    {
        iTmp  = (GET_B_MASK(APP_EXE_PM1_NO))|(GET_B_MASK(APP_EXE_PM2_NO))|(GET_B_MASK(APP_EXE_PM3_NO));
        iRet = CcbUpdateIAndBs(WORK_LIST_LP, 0, iTmp, iTmp);
        if (iRet )
        {
            return 2;
        }
        break;
    }
    default:
        break;
    }

    return 0;
}

//2019.9.16 add
unsigned int  pretreatmentCleaning()
{
    return gCcb.ulKeyWorkStates;
}

#ifdef __cplusplus
}
#endif

