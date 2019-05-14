#ifndef _INNER_IPC_H_
#define _INNER_IPC_H_

#include "SatIpc.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct
{
    sp_thread_mutex_t mutex;
    sp_thread_cond_t cond;
}INNER_IPC_STRU;

#ifdef __cplusplus
}
#endif
#endif