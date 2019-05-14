#ifndef _SERIAL_ITF4ZB_
#define _SERIAL_ITF4ZB_

#include "serial.h"
#include "datatype.h"

extern SERIAL_ITF_STRU gzbSerialItfStru;
void zbSerialItfMsgProc(void *para,SAT_MSG_HEAD *pMsg);
int zbSerialItfInit(void);
void zbSerialItfRcvProc(void *lparam);
int zbSerialInit(int *SerialFd,char *SERIAL_DEV_NAME);

#endif
