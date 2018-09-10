#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>
#include <errno.h>
#include <sys/time.h>
#include <signal.h>
#include <termios.h>
#include <assert.h>


#include "msg.h"
#include "msgdef.h"
#include "Interface.h"
#include "memory.h"
#include "SerialItf4zb.h"
#include "messageitf.h"

#include "cminterface.h"

#include "sapp.h"
#include "rpc.h"
#include "vtask.h"


#ifdef __cplusplus
extern "C"
{
#endif

#undef SERIAL_DEBUG

#ifdef SERIAL_DEBUG
#define SERIAL_DP(fmt, args...) do {printf(fmt,##args);printf("\r\n");}while(0);
#define SERIAL_DP1(fmt, args...) do {printf(fmt,##args);}while(0);
#else
#define SERIAL_DP(fmt, args...) 
#define SERIAL_DP1(fmt, args...) do {printf(fmt,##args);}while(0);
#endif



#define MAX_SERAIL_MESSAGE_LEN 256

typedef struct
{
    unsigned char ucStage;
    unsigned char ucIndex;
    unsigned char ucMsgLen;
    unsigned char ucChecksum;
    unsigned char Data[MAX_SERAIL_MESSAGE_LEN];
}SERIAL_BUS_PARSER;

#define MAX_RCV_BUFF_LEN 1024
char UART_Rcvbuff[MAX_RCV_BUFF_LEN];
int  rcvfront;
int  rcvrear;

extern TASK_HANDLE TASK_HANDLE_ZB;
extern TASK_HANDLE TASK_HANDLE_SERIALITF4ZB;

SERIAL_BUS_PARSER  SerBusParser;
SERIAL_ITF_STRU gzbSerialItfStru;

void Serial_InitParser(void)
{
    SerBusParser.ucStage = rpcSteSOF;
    SerBusParser.ucIndex = 0;
}


int UART_ParseBusMessage(void)
{
    unsigned char ucData;
    
    while(rcvrear != rcvfront)
    {
        ucData = UART_Rcvbuff[rcvrear];
        rcvrear = (rcvrear+1)%MAX_RCV_BUFF_LEN;
        switch(SerBusParser.ucStage)
        {
        case rpcSteSOF:
            if (RPC_UART_SOF == ucData)
            {
                SerBusParser.ucStage = rpcSteLen;
            }
            break;
        case rpcSteLen:
           if (ucData > SBL_MAX_SIZE)
           {
             SerBusParser.ucStage = rpcSteSOF;
             break;
           }
           else
           {
             SerBusParser.ucStage = rpcSteData;
             
             SerBusParser.ucChecksum = 0;
             SerBusParser.ucIndex = 0;
             SerBusParser.ucMsgLen = ucData + (RPC_FRAME_HDR_SZ );  // Combine the parsing of Len, Cmd0 & Cmd1 with the data for BUS TYPE NONE ,OTHER busitf.
             // no break;
           }            

        case rpcSteData:
            SerBusParser.ucChecksum ^= ucData;
            SerBusParser.Data[SerBusParser.ucIndex] = ucData;
            
            if (++SerBusParser.ucIndex == SerBusParser.ucMsgLen)
            {
              SerBusParser.ucStage = rpcSteFcs;
            }
            break;
        case rpcSteFcs:
          SerBusParser.ucStage = rpcSteSOF;
        
          if ((SerBusParser.ucChecksum == ucData)) //  && ((SerKeyParser.Data[RPC_POS_CMD0] & RPC_SUBSYSTEM_MASK) == RPC_SYS_BOOT
          {
            return 1; 
          }
          break;
        default:
          SerBusParser.ucStage = rpcSteSOF;
          SerBusParser.ucIndex = 0;
          break;
        }
    }
    return 0;
}

//#define SET_RCVTIMEO(tv,s)    struct timeval tv = {s,0}

int zbSerialInit(int *SerialFd,char *SERIAL_DEV_NAME)
{
    int fd;
    int i;
    struct termios opt; 
    
    fd = open(SERIAL_DEV_NAME, O_RDWR);     
    if (fd < 0) 
    {       
        printf("error to open %s\n",SERIAL_DEV_NAME);       
        return -1;  
    }       
    tcgetattr(fd, &opt);      
    
    opt.c_iflag = IGNBRK | IGNPAR ;
    opt.c_oflag = 0 ;
    opt.c_cflag = CS8 | CREAD | CLOCAL ;
    opt.c_lflag = 0 ;
    
    for ( i=0 ; i<NCCS ; i++ ) opt.c_cc[i] = 0 ;

    opt.c_cc[VTIME] = 1;    
    opt.c_cc[VMIN] = 0;       

    cfsetispeed(&opt, B115200);    
    cfsetospeed(&opt, B115200);        
    
    if(tcsetattr(fd, TCSANOW, &opt) != 0)    
    {        
        perror("serial error");        
        return -1;    
    }   

    tcflush(fd, TCIOFLUSH);   

    *SerialFd = fd;

    return 0;

}

static int serial_send(int fd, char data[],int len)
{
    int ret = write(fd, data, len);
    
    return ret;
}

static void zbSndMsg(SERIAL_ITF_STRU *pItf,SAT_MSG_HEAD *pMsg)
{
    ZB_SERIALITF_STRU *pCanCmd = (ZB_SERIALITF_STRU *)pMsg;

    SERIAL_DP("SO:%d",pCanCmd->msgHead.len);

    serial_send(pItf->fd,pCanCmd->data,pCanCmd->msgHead.len);
    
}

void zbSerialItfMsgProc(void *para,SAT_MSG_HEAD *pMsg)
{
    SERIAL_ITF_STRU *pItf = (SERIAL_ITF_STRU * )para;
    switch(pMsg->event)
    {
    case INIT_ALL_THREAD_EVENT:
        zbSerialItfInit();
        break;
    case ZB_SERIALITF_MSG:
        zbSndMsg(pItf,pMsg);
        break;
    default:
        break;
   }
}

static void zbSerialRcvFrame(void)
{
    // send message to main
    int ret;
    ZB_SERIALITF_STRU *pItfMsg = NULL;

    SERIAL_DP("SI:%d",SerBusParser.ucMsgLen);
    
    pItfMsg = (ZB_SERIALITF_STRU *)SatAllocMsg(ZB_SERIALITF_MSG_SIZE + SerBusParser.ucMsgLen);//malloc(sizeof(MAIN_CANITF_STRU) + CanRcvBuff[ucRcvBufIndex].len);
    if (NULL == pItfMsg)
    {
        return ;
    }

    pItfMsg->msgHead.event = SERIALITF_ZB_MSG;

    memcpy(pItfMsg->data,SerBusParser.Data,SerBusParser.ucMsgLen);

    ret = VOS_SndMsg(TASK_HANDLE_ZB,(SAT_MSG_HEAD *)pItfMsg);

    if (0 != ret)
    {
    }
}
#if 0
static void zbSerialItfRfFrame(char *data, int len)
{
    // send message to main
    int ret;
    ZB_SERIALITF_STRU *pItfMsg = NULL;

    WL_ITF_MSG_DATA_COMMON_STRU *pWlData;

    //RFLOADER_DP("SI:%d",SerBusParser.ucMsgLen);
    
    pItfMsg = (ZB_SERIALITF_STRU *)SatAllocMsg(ZB_SERIALITF_MSG_SIZE + WIFI_FRAME_LENGTH(WL_ITF_MSG_DATA_HEADER_SIZE + len)) ;//malloc(sizeof(MAIN_CANITF_STRU) + CanRcvBuff[ucRcvBufIndex].len);
    if (NULL == pItfMsg)
    {
        return ;
    }

    pItfMsg->msgHead.event = SERIALITF_ZB_RF_MSG;

    pWlData = (WL_ITF_MSG_DATA_COMMON_STRU *)&pItfMsg->data[WIFI_FRAME_HEADER_LENGTH];

    pWlData->itfhdr.ucItfMsgType = WL_ITF_MSG_DATA;
    pWlData->itfhdr.ucObjType    = WL_ITF_OBJ_ZB;
    pWlData->itfhdr.usObjId      = (data[0] << 8) | data[1];
    pWlData->subhdr.usSubMsgType = WL_ITF_MSG_DATA_ZB_SERIAL;
    pWlData->subhdr.ulSubMsgLen  = len;

    memcpy(pWlData->data,data,len);

    wlitf_frame_construct(pItfMsg->data,(WL_ITF_MSG_DATA_HEADER_SIZE + len));

    ret = VOS_SndMsg(TASK_HANDLE_ZB,(SAT_MSG_HEAD *)pItfMsg);

    if (0 != ret)
    {
    }
}
#endif

static void zbSerialItfRcvData(char *data,int len)
{
    int loop;
    int ret;

    SERIAL_DP("SRaw:%d",len);
    
    for (loop = 0; loop < len; loop++)
    {
        UART_Rcvbuff[rcvfront] = data[loop];
        rcvfront = (rcvfront + 1)% MAX_RCV_BUFF_LEN;

        ret = UART_ParseBusMessage();

        if (ret)
        {
            zbSerialRcvFrame();
        }        
    
    }
}

static void zbSerialItfReset(void)
{
    Serial_InitParser();
    rcvfront = rcvrear = 0;
}

void zbSerialItfRcvProc(void *lparam)
{
    SERIAL_ITF_STRU  *pCanItf = (SERIAL_ITF_STRU *)lparam;
    int           ret = 0;

    fd_set    rdset;

    struct timeval      timeout;

    char buf[256];

    for(;pCanItf->fd != -1;)
    {
    
       timeout.tv_sec  = 0;  
       timeout.tv_usec = 200*1000;
       
       FD_ZERO(&rdset);
   
       FD_SET(pCanItf->fd, &rdset); // read sockets
       
       ret = select(pCanItf->fd + 1, &rdset, NULL, NULL, &timeout);     
        if (ret > 0)
        {
           if (FD_ISSET(pCanItf->fd, &rdset))   
           {
               ret = read(pCanItf->fd, buf, 255);
               if (ret > 0 )
               {
                   //RFLOADER_DP("Rcv Data : %d",ret)
                   zbSerialItfRcvData(buf,ret);
               }
            }
        }
#if 0  /* 20170812 */    
        else
        {
            zbSerialItfReset();
        }
#endif        
    }
    gzbSerialItfStru.thd4rx = 0;

}
#ifndef WIN32

static int api_get_thread_policy (pthread_attr_t *attr)
{
    int policy;
    int rs = pthread_attr_getschedpolicy (attr, &policy);
    assert (rs == 0);

    switch (policy)
    {
        case SCHED_FIFO:
            printf ("policy = SCHED_FIFO\n");
            break;
        case SCHED_RR:
            printf ("policy = SCHED_RR");
            break;
        case SCHED_OTHER:
            printf ("policy = SCHED_OTHER\n");
            break;
        default:
            printf ("policy = UNKNOWN\n");
            break; 
    }
    return policy;
}

static void api_show_thread_priority (pthread_attr_t *attr,int policy)
{
    int priority = sched_get_priority_max (policy);
    assert (priority != -1);
    printf ("max_priority = %d\n", priority);
    priority = sched_get_priority_min (policy);
    assert (priority != -1);
    printf ("min_priority = %d\n", priority);
}

static int api_get_thread_priority (pthread_attr_t *attr)
{
    struct sched_param param;
    int rs = pthread_attr_getschedparam (attr, &param);
    assert (rs == 0);
    printf ("priority = %d\n", param.__sched_priority);
    return param.__sched_priority;
}

static void api_set_thread_policy (pthread_attr_t *attr,int policy)
{
    int rs = pthread_attr_setschedpolicy (attr, policy);
    assert (rs == 0);
    api_get_thread_policy (attr);
}
#endif


int zbSerialItfInit(void)
{

    rcvfront = 0;
    rcvrear = 0;
    Serial_InitParser();

    /* set thread opportunity */
#ifndef WIN32
    if (0)
    {
        pthread_attr_t attr;      
        struct sched_param sched; 

        int rs;
        int policy;
        rs = pthread_attr_init (&attr);
        policy = api_get_thread_policy (&attr);
        api_set_thread_policy(&attr,SCHED_RR);
        rs = pthread_attr_destroy (&attr);
    }
#endif
    return 0;
}
#ifdef __cplusplus
}
#endif

