#ifndef _MSG_H_
#define _MSG_H_

#include "SatIpc.h"
#include "list.h"
#include "SatError.h"
#ifdef __cplusplus
extern "C"
{
#endif

typedef struct msgbuf
{
    long int   mtype;
    long int   msgLen;
    char*      mtext;
}Msgbuf;

typedef struct msgItem {
    list_t list;
    Msgbuf msgInfo;
}MsgItem;

typedef struct MsgQueue
{
    list_t            list;
    int               used;
    int               msgId;
    sp_sem_t          items;
    sp_sem_t           slots;
    
    sp_thread_mutex_t queue_mutex;
    
    list_t             Msghead;

    int                MsgNum;
    
}MsgQueue;

typedef struct 
{
    list_t            busy;
    list_t            free;
    sp_thread_mutex_t Mutex;
}MsgMan;



typedef struct
{
    unsigned  int   sender;
    unsigned  int   receiver;
    unsigned  int   event;
    unsigned  int   len;
} SAT_MSG_HEAD;

typedef  struct
{
    SAT_MSG_HEAD msgHead;
    char         data[1]; 
}SAT_MSG_STRU;

#define MAX_OS_MSG_SIZE   (8300)
#define MAX_MSG_QUEUE_SIZE 5  
#define MAX_QUEUE_LENGTH 8192

void init_msgQueue();

int MsgQueueAlloc(void);
int msg_send(int msg_id, char* pMsg, int msgtype,int msglen);
int msg_recv(int msg_id, char** ppMsg,int *msgtype);
int msg_destroy(int msg_id);
int SndMsg(int msg_id, SAT_MSG_HEAD *pMsg);
int SndMsg2(int msg_id, int event, int nMsgLen, char* pMsg);
SAT_MSG_HEAD * SatAllocMsg(int MsgLen);
void SatFreeMsg(void *pMsg);

#ifdef __cplusplus
}
#endif

#endif
