#ifndef _VTASK_H_
#define _VTASK_H_

#include "msg.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef void *TASK_HANDLE;

typedef void (*task_msg_proc)(void *para,SAT_MSG_HEAD *msg);
typedef void (*task_work_proc)(void *para);

typedef void (*task_msg_bc)(void *para);
typedef void (*task_msg_ec)(void *para);


TASK_HANDLE Task_CreateMessageTask(task_msg_proc proc,task_msg_bc bc,task_msg_ec ec,void *para);
int Task_DispatchWorkTask(task_work_proc proc,void *para);

int Task_init(int iMaxTasks);

void Task_RelaseTask(TASK_HANDLE task);

int VOS_SndMsg(TASK_HANDLE task, SAT_MSG_HEAD *pMsg);
int VOS_SndMsg2(TASK_HANDLE task, int event, int nMsgLen, char* pMsg);

#ifdef __cplusplus
}
#endif

#endif

