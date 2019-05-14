#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
 
#include <net/if.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
 
#include <linux/can.h>
#include <linux/can/raw.h>

#include "ccb.h"
#include "canItf.h"
#include "msg.h"
#include "messageitf.h"
#include "Interface.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define msleep(n)   usleep(n*1000)

#define DEV_CAN0      "can0"
#define DEV_CAN1      "can1"


//#define SET_RCVTIMEO(tv,s)	struct timeval tv = {s,0}
#define CAN_RCV_BUFFER_NUM (6)


typedef struct  {

	unsigned short		usLen;     // total CAN network layer message usLen
	unsigned short		usDataLen; // how many data left to  be received

	unsigned char		*pucHead;   // memory buffer pucHead pointer
	unsigned char		*pucData;   // current data pos

    unsigned char       aucbuf[MAX_CAN_MESSAGE_LEN];

	unsigned short      usSubCanAdr; // 

    unsigned int        ulCanid;

    unsigned int        ulChlNo;

    unsigned int        ulTicks;     // for Dead check
    
}CAN_Rcv_buff;

typedef struct  {

	unsigned short		usLen;     // total CAN network layer message usLen
	unsigned short		usDataLen; // how many data left to have been sended 

	unsigned char		*pucHead;   // memory buffer pucHead pointer
	unsigned char		*pucData;   // current data pos

    unsigned int        ulCanid;
    
}CAN_Snd_buff;


CAN_Rcv_buff CanRcvBuff[CAN_RCV_BUFFER_NUM];

CAN_ITF_STRU gCanItf[CANITF_MAX_CAN_NUM];

char *gaCANDev[CANITF_MAX_CAN_NUM] = {
	DEV_CAN0,
};

static unsigned int gCanItfSecond = 0;

#define CANITF_TIME_4_RCV_BUFFER_CHECK 3

/* forward declaration */
static void CanItfRcvProc(void *lparam);

static void CanInitBufs(void)
{
    int iLoop = 0;
    for (iLoop = 0; iLoop < CAN_RCV_BUFFER_NUM; iLoop++)
    {
        memset(&CanRcvBuff[iLoop],0,sizeof(CAN_Rcv_buff));

        CanRcvBuff[iLoop].ulCanid = 0XFFFFFFFF;
    }
}

static void CanInitBuf(int iIdx)
{
    memset(&CanRcvBuff[iIdx],0,sizeof(CAN_Rcv_buff));

    CanRcvBuff[iIdx].ulCanid = 0XFFFFFFFF;
}


static int CanGetBuf(unsigned int ulChlNo,unsigned int ulCanId)
{
    int iLoop = 0;
    for (iLoop = 0; iLoop < CAN_RCV_BUFFER_NUM; iLoop++)
    {
        if (ulCanId == CanRcvBuff[iLoop].ulCanid
            && ulChlNo == CanRcvBuff[iLoop].ulChlNo)
        {
            return iLoop ;
        }

    }

    return CAN_RCV_BUFFER_NUM;
}

static void CanRelBuf(int Idx)
{
    if (Idx < CAN_RCV_BUFFER_NUM)
    {
        CanRcvBuff[Idx].ulCanid = 0XFFFFFFFF;
    }
}

static int CanAllocBuf(void)
{
    int iLoop = 0;
    for (iLoop = 0; iLoop < CAN_RCV_BUFFER_NUM; iLoop++)
    {
        if (0XFFFFFFFF == CanRcvBuff[iLoop].ulCanid)
        {
            return iLoop ;
        }
    }
    return CAN_RCV_BUFFER_NUM;
}

static void CanBufCheck(void)
{
    int iLoop = 0;
    for (iLoop = 0; iLoop < CAN_RCV_BUFFER_NUM; iLoop++)
    {
        if (0XFFFFFFFF == CanRcvBuff[iLoop].ulCanid)
        {
            continue;
        }

        if (gCanItfSecond - CanRcvBuff[iLoop].ulTicks >= CANITF_TIME_4_RCV_BUFFER_CHECK)
        {
            CanRelBuf(iLoop);
        }
    }

}


static int CanSocketInit(int iChl)
{
    int socked;
	struct sockaddr_can can_addr;
	struct ifreq ifr;
	
	if((socked = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0) 
	{
		printf("open can socket error.\r\n");
		return RET_FAILED;
	}

    memset(&ifr, 0, sizeof(ifr));
	strcpy(ifr.ifr_name, gaCANDev[iChl]);
	
	//strcpy(ifr.ifr_name, "can1");
	
	if (RET_SUCCESS != ioctl(socked, SIOCGIFINDEX, &ifr))
	{
		printf("ioctl SIOCGIFINDEX failed.");
		close(socked);
		return RET_FAILED;
	}

    memset(&can_addr, 0, sizeof(can_addr));
	can_addr.can_family  = AF_CAN;
	can_addr.can_ifindex = ifr.ifr_ifindex; 
  
	if(bind(socked, (struct sockaddr *)&can_addr, sizeof(can_addr)) < 0) 
	{
		printf("bind can socket failed.");
		close(socked);
		return RET_FAILED;
	}

	// set filter for your application
	if (0)
	{
          struct can_filter rfilter[2];

	      // for standard
          rfilter[0].can_id   = 0x123;
          rfilter[0].can_mask = CAN_SFF_MASK;

		  // for extend
          rfilter[1].can_id   = 0x10000000;
          rfilter[1].can_mask = CAN_EFF_MASK ;
      
          setsockopt(socked, SOL_CAN_RAW, CAN_RAW_FILTER, &rfilter, sizeof(rfilter));		
	}
#if 0
    {
        //SET_RCVTIMEO(tv, 2);
        struct timeval tv = {2,0};
        if (setsockopt(socked, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv, sizeof(tv)))
        {
          //UserMan_Log("Setting socket timeout to %ds failed!\n", 2);
        }
    }
#endif
    gCanItf[iChl].socket = socked;

    return 0;

}

static int can_send(int socket, struct can_frame *pframe)
{
    int ret = write(socket, pframe, sizeof(struct can_frame));
    if (sizeof(struct can_frame) != ret)
    {
        return ret;
    }
    else
    {
        return RET_SUCCESS;
    }
}

static void CanSndMsg(CAN_ITF_STRU *pItf,SAT_MSG_HEAD *pMsg)
{
    MAIN_CANITF_MSG_STRU *pCanCmd = (MAIN_CANITF_MSG_STRU *)pMsg;

    CAN_Snd_buff SndBuf;

    int ret ;

    int MsgLen;

    struct can_frame frame;

    SndBuf.pucHead = (unsigned char *)pCanCmd->aucData;

    SndBuf.pucData = SndBuf.pucHead;

    SndBuf.usLen = SndBuf.usDataLen = pCanCmd->iMsgLen;;

    frame.can_id = pCanCmd->ulCanId;

    // printf("CAN Send : %d \r\n",SndBuf.usDataLen);

    do {
        if (SndBuf.usDataLen >= 8)
        {
            MsgLen = 8;
        }
        else
        {
            MsgLen = SndBuf.usDataLen;
        }    

        frame.can_dlc = MsgLen;

        memcpy(frame.data,SndBuf.pucData,MsgLen);

        
        ret = can_send(pItf[pCanCmd->iCanChl].socket, &frame);
        
        if (RET_SUCCESS == ret)
        {
            SndBuf.usDataLen -= MsgLen;

            if (0 == SndBuf.usDataLen)
            {
            }
            else
            {
                SndBuf.pucData += MsgLen;
            }
        }
        else
        {
            // CAN  failer
            printf("CAN Send failure: Left bytes : %d ,error code = %d\r\n",SndBuf.usDataLen,ret);
            break;
        }

    }
    while(SndBuf.usDataLen > 0);
	msleep(25);
	
}


static void CanItfSecondTask()
{
	gCanItfSecond++;

    CanBufCheck();

    //printf("CanItfSecondTask \r\n");
    
}

static void CanItfTimerMsg(SAT_MSG_HEAD *pMsg)
{
    TIMER_MSG_STRU *pTimerMsg = (TIMER_MSG_STRU *)(pMsg);

    switch(pTimerMsg->id)
    {
    case TIMER_SECOND:
        CanItfSecondTask();
        break;
    default:
        break;
    }
}

static void CanItfSecondhandler(void *arg)
{
    int id = (int )arg;
    int ret;
    TIMER_MSG_STRU *tm = (TIMER_MSG_STRU *)SatAllocMsg(TIMER_MSG_MSG_SIZE);//malloc(sizeof(TIMER_MSG_STRU));
    if (tm)
    {
        tm->id = id;
        tm->msgHead.event = TIMER_MSG;
        ret = VOS_SndMsg(TASK_HANDLE_CANITF,(SAT_MSG_HEAD *)tm);
        if (0 != ret)
        {
        }
    }
   
}

static int CanItfInit(void)
{
    int iLoop = 0; 
    for (iLoop = 0; iLoop < CANITF_MAX_CAN_NUM; iLoop++)
	{
		int ret = CanSocketInit(iLoop);
		
		if (RET_SUCCESS == ret)
		{
            printf("Can init succ\r\n");
		}
		else
		{
		    printf("can init fail\r\n");
		}
	
	}

    CanInitBufs();
    
    Task_DispatchWorkTask(CanItfRcvProc,&gCanItf[0]);

    // start period check timer
    timer_add(1000,TIMER_PERIOD,CanItfSecondhandler,TIMER_SECOND);
	
    return 0;
}


void CanItfMsgProc(void *para,SAT_MSG_HEAD *pMsg)
{
    CAN_ITF_STRU *pItf = (CAN_ITF_STRU *)para;

	switch(pMsg->event)
	{
	case INIT_ALL_THREAD_EVENT:
        CanItfInit();
        break;
	case MAIN_CANITF_MSG:
        CanSndMsg(pItf,pMsg);
        break;
    case TIMER_MSG:
        CanItfTimerMsg(pMsg);
        break;
    default:
        break;
   }
}


/**************************************************************************************************
 * @fn          CanItfFrameCheck
 *
 * @brief       parse according to the RPC protocol.
 *
 * input parameters
 *
 * None.
 *
 * output parameters
 *
 * None.
 *
 * @return      TRUE if the downloaded code has been enabled; FALSE otherwise.
 */
 // ylf: h->c |SOF|LEN|CMD0|CMD1|DATAS|FCS|
 // FCS = FUN(|SB_LEN|SB_CMD1|SB_CMD2|DATAS(=SB_LEN)|)
 //      c->h 
static int CanItfFrameCheck(int iIdx)
{
  unsigned char ch;
  unsigned char rpcSte = rpcSteSOF;
  unsigned char sbFcs=0, sbIdx=0,sbLen=0;

  int len = CanRcvBuff[iIdx].usDataLen;
  unsigned char *data = CanRcvBuff[iIdx].aucbuf;

  while (len--)
  {
    ch = *data++;
    switch (rpcSte)
    {
    case rpcSteSOF:
      if (RPC_UART_SOF == ch)
      {
        rpcSte = rpcSteLen;
      }
      break;

    case rpcSteLen:
      {
        rpcSte = rpcSteData;
        sbFcs = sbIdx = 0;
        sbLen = ch + 3;  // Combine the parsing of Len, Cmd0 & Cmd1 with the data.
        // no break;
      }

    case rpcSteData:
      sbFcs ^= ch;

      if (++sbIdx == sbLen)
      {
        rpcSte = rpcSteFcs;
      }
      break;

    case rpcSteFcs:
      rpcSte = rpcSteSOF;

      if (sbFcs == ch) 
      {
         // process
         return TRUE;
      }
      break;

    default:
     break;
    }
  }

  return FALSE;
}

static void CanRcvFrame(int iChl,unsigned char ucRcvBufIndex)
{
    // send message to main
    int ret;

    TASK_HANDLE task = TASK_HANDLE_MAIN;

    MAIN_CANITF_MSG_STRU *pCanItfMsg = NULL;

    if (!CanItfFrameCheck(ucRcvBufIndex)) // message invalid
    {
        return;
    }
    
	pCanItfMsg = (MAIN_CANITF_MSG_STRU *)SatAllocMsg(MAIN_CANITF_MSG_SIZE + CanRcvBuff[ucRcvBufIndex].usLen);//malloc(sizeof(MAIN_CANITF_MSG_STRU) + CanRcvBuff[ucRcvBufIndex].len);
	if (NULL == pCanItfMsg)
	{
		return ;
	}
    pCanItfMsg->msgHead.event  = CANITF_MAIN_MSG;
    pCanItfMsg->iMsgLen        = CanRcvBuff[ucRcvBufIndex].usLen;
    pCanItfMsg->ulCanId        = CanRcvBuff[ucRcvBufIndex].ulCanid;
	pCanItfMsg->iCanChl        = iChl;

    memcpy(pCanItfMsg->aucData,CanRcvBuff[ucRcvBufIndex].aucbuf,CanRcvBuff[ucRcvBufIndex].usLen);

    if (pCanItfMsg->aucData[1 + RPC_POS_CMD0] == RPC_SYS_AF
        && (pCanItfMsg->aucData[1 + RPC_POS_CMD1] & 0X7F) == SAPP_CMD_MODBUS)
    {
        task = TASK_HANDLE_MOCAN;
    }
    ret = VOS_SndMsg(task,(SAT_MSG_HEAD *)pCanItfMsg);

    if (0 != ret)
    {
    }
}




static void CanItfRcvData(int iChl,struct can_frame *frame)
{
    unsigned short usCanLen;

    int  iIndex = CanGetBuf(iChl,frame->can_id);

    //printf("CanItfRcvData 0x%x\r\n",frame->can_id);

    if (iIndex >= CAN_RCV_BUFFER_NUM)
    {
        iIndex = CanAllocBuf();
    }

    if (iIndex >= CAN_RCV_BUFFER_NUM)
    {
        return ; // no buffer availabe
    }
    
    if (0 == CanRcvBuff[iIndex].usLen)
    {

        // judge message tag
        if (frame->data[0] != RPC_UART_SOF)
        {
           // invalid tag
           return ;
        }
        
        // the head of networklayer have been received, this must be the second segment of networklayer
        usCanLen = frame->data[1 + RPC_POS_LEN] + RPC_UART_FRAME_OVHD + RPC_FRAME_HDR_SZ;
        
        CanRcvBuff[iIndex].usLen = usCanLen;
        CanRcvBuff[iIndex].usDataLen = frame->can_dlc;
    
        CanRcvBuff[iIndex].pucHead = CanRcvBuff[iIndex].aucbuf;
        CanRcvBuff[iIndex].pucData = CanRcvBuff[iIndex].pucHead;
        
        memcpy(CanRcvBuff[iIndex].pucData,frame->data,frame->can_dlc);
        CanRcvBuff[iIndex].pucData += frame->can_dlc;
        
        CanRcvBuff[iIndex].ulCanid = frame->can_id;
        CanRcvBuff[iIndex].ulChlNo = iChl;
        CanRcvBuff[iIndex].ulTicks = gCanItfSecond; // late implment
    
    }
    else
    {
        // other segment
        usCanLen = CanRcvBuff[iIndex].usDataLen + frame->can_dlc;
    
        if (usCanLen > CanRcvBuff[iIndex].usLen
           || ((usCanLen < CanRcvBuff[iIndex].usLen)
               && (frame->can_dlc < 8)))
        {
            // invalid message
            CanInitBuf(iIndex);
            return;
        }
        
        memcpy(CanRcvBuff[iIndex].pucData,frame->data,frame->can_dlc);
        CanRcvBuff[iIndex].pucData += frame->can_dlc;
        CanRcvBuff[iIndex].usDataLen += frame->can_dlc;
        
    }
    
    
    if (CanRcvBuff[iIndex].usDataLen == CanRcvBuff[iIndex].usLen
     && 0 != CanRcvBuff[iIndex].usLen)
    {
       // a frame is received
    
       CanRcvFrame(iChl,iIndex);
    
       // clear rcv buffer
       CanInitBuf(iIndex);
    }

}

static void CanItfRcvProc(void *lparam)
{
	CAN_ITF_STRU  *pCanItf = (CAN_ITF_STRU *)lparam;
    int           ret = 0;

	struct can_frame frame;

    fd_set    rdset;

	int       socketMax = 0;

	struct timeval		timeout;

    //memset(&frame, 0, sizeof(frame));  
    
    // msg("I Start Can Receiver");


     for(;;)
     {
        int iLoop;
        socketMax = 0;
        timeout.tv_sec = 1;  
        timeout.tv_usec = 0;
        
        FD_ZERO(&rdset);

        for (iLoop = 0; iLoop < CANITF_MAX_CAN_NUM; iLoop++)
        {
            FD_SET(pCanItf[iLoop].socket, &rdset); // read sockets

            if (pCanItf[iLoop].socket > socketMax)
            {
                socketMax = pCanItf[iLoop].socket;
            }
        }
        
        ret = select(socketMax + 1, &rdset, NULL, NULL, &timeout);     
        if (ret > 0)
        {
        
           for (iLoop = 0; iLoop < CANITF_MAX_CAN_NUM; iLoop++)
           {
               if (FD_ISSET(pCanItf[iLoop].socket, &rdset))   
               {
                   ret = read(pCanItf[iLoop].socket, &frame, sizeof(struct can_frame));
                   if (ret > 0 )CanItfRcvData(iLoop,&frame);
               }
           }
        }
     }

}

#ifdef __cplusplus
}
#endif

