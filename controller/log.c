#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "log.h"

#ifdef __cplusplus
extern "C"
{
#endif

static int VosLogLevel = VOS_LOG_ERROR;

int VOS_logger(int level,char *filename,int lineno,const char* fmt,...)
{
    int  ret = 0;
    va_list   args;
    va_start(args, fmt);
  

    if (level> VosLogLevel)

    {
        return 0;
    }
  
    printf("[VOS_log FileName = %s LineNo = %d] : ",filename,lineno);
    vprintf(fmt,args);
    printf("\r\n");
  
  
    va_end(args);
  
    return  ret;
}

void VOS_SetLogLevel(int iLevel)
{
    if (iLevel >=VOS_LOG_BUTT ||  iLevel < 0)
    {
        return ;
    }
    VosLogLevel = iLevel;
}

#ifdef __cplusplus
}
#endif

