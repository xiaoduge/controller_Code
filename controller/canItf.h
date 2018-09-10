#ifndef _CAN_ITF_H_
#define _CAN_ITF_H_

#include "cminterface.h"

#ifdef __cplusplus
 extern "C"
 {
#endif

#define CANITF_MAX_CAN_NUM (1)

#define RET_SUCCESS                 0
#define RET_FAILED                  -1
#define BUF_SIZE                    512

typedef struct
{
   int socket; // socket for CAN 
}CAN_ITF_STRU;

extern CAN_ITF_STRU gCanItf[CANITF_MAX_CAN_NUM];

void CanItfMsgProc(void *para,SAT_MSG_HEAD *pMsg);

#ifdef __cplusplus
}
#endif

#endif
