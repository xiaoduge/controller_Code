#ifndef _SAT_ERROR_H_
#define _SAT_ERROR_H_

typedef enum
{
    // FOR SYSTEM USE
	SAT_ERROR_SYSTEM_BASE  = 0x0,
	SAT_ERROR_MSG_BASE     = 0x1000,

    // USER begin here
    SAT_ERROR_USER_BASE    = 0x8000,
		
}SAT_ERROR_ENUM;

#endif
