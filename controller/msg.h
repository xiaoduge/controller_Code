#ifndef _MSG_H_
#define _MSG_H_

#include "SatIpc.h"
#include "list.h"
#include "SatError.h"
#ifdef __cplusplus
extern "C"
{
#endif

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
