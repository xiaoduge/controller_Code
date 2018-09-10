#ifndef _SERIAL_H_
#define _SERIAL_H_

typedef struct
{
   int fd; // fd for serial 
   int thd4tx;
   int thd4rx;
}SERIAL_ITF_STRU;

#endif
