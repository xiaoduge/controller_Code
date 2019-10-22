#ifndef _DASERVICE_EVENT_H
#define _DASERVICE_EVENT_H

#include "satmsg.h"

#ifdef __cplusplus
 extern "C"
 {
#endif



#define	USER_MESSAGE_EVENT				    (unsigned short int)2001

#define MAIN_CANITF_MSG                      (USER_MESSAGE_EVENT+1)

#define CANITF_MAIN_MSG                      (USER_MESSAGE_EVENT+2)

#define TIMER_MSG                            (USER_MESSAGE_EVENT+3)

#define MAIN_WORK_MSG                        (USER_MESSAGE_EVENT+4)

#define MAIN_PARAM_MSG                       (USER_MESSAGE_EVENT+5)

#define ZB_SERIALITF_MSG                     (USER_MESSAGE_EVENT+8)

#define SERIALITF_ZB_MSG                     (USER_MESSAGE_EVENT+9)

#define CMD_ZB_DOWNLAOD_MSG                  (USER_MESSAGE_EVENT+12)

#define CMD_ZB_HANDSHAKE_MSG                 (USER_MESSAGE_EVENT+13)

#define CMD_ZB_FILESIZE_MSG                  (USER_MESSAGE_EVENT+14)

#define DATAPRCO_ZB_MSG                      (USER_MESSAGE_EVENT+19)

#define DATAPRCO_CFG_NETWORK_ZB             (USER_MESSAGE_EVENT+28)

#define CMD_NETWORK_CFG_MSG                 (USER_MESSAGE_EVENT+30)

#define ZBITF_MAIN_MSG                      (USER_MESSAGE_EVENT+31)
#define MAIN_ZBITF_MSG                      (USER_MESSAGE_EVENT+32)

#define ZBMAIN_NODE_REPORT_MSG              (USER_MESSAGE_EVENT+33)

#define ZBITF_MAIN_MTN_MSG                  (USER_MESSAGE_EVENT+34)

#define MAIN_PRINTER_MSG                    (USER_MESSAGE_EVENT+35)

#define TCP_CLIENT_SEND_DATA				(USER_MESSAGE_EVENT + 0x100)

#define ASTCP_CLIENT_RECEIVED				(TCP_CLIENT_SEND_DATA + 1)


#ifdef __cplusplus
}
#endif

#endif

