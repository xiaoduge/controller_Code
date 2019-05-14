#ifndef _SAPP_H_
#define _SAPP_H_

// when responding - this is probably not necessary for smooth functionality.
#define SAPP_RSP_MASK                 0x80

// Status codes
#define SAPP_SUCCESS                  0
#define SAPP_FAILURE                  1
#define SAPP_INVALID_FCS              2
#define SAPP_INVALID_FILE             3
#define SAPP_FILESYSTEM_ERROR         4
#define SAPP_ALREADY_STARTED          5
#define SAPP_NO_RESPOSNE              6
#define SAPP_VALIDATE_FAILED          7
#define SAPP_CANCELED                 8
#define SAPP_IGNORED                  9

// Indices into the RPC data (RPC_POS_DAT0):
#define SAPP_REQ_ADDR_LSB             RPC_POS_DAT0
#define SAPP_REQ_ADDR_MSB            (SAPP_REQ_ADDR_LSB+1)
#define SAPP_REQ_DAT0                (SAPP_REQ_ADDR_MSB+1)
#define SAPP_RSP_STATUS               RPC_POS_DAT0
#define SAPP_RSP_ADDR_LSB            (SAPP_RSP_STATUS+1)
#define SAPP_RSP_ADDR_MSB            (SAPP_RSP_ADDR_LSB+1)
#define SAPP_RSP_DAT0                (SAPP_RSP_ADDR_MSB+1)
#define SAPP_READ_HDR_LEN            (SAPP_RSP_DAT0 - SAPP_RSP_STATUS)

typedef enum
{
    SAPP_CMD_RESET = 0,
    SAPP_CMD_SYS_INIT, // restore system setting
    
    SAPP_CMD_AT    = 0x20,
    SAPP_CMD_DATA,
    SAPP_CMD_CFG,
    SAPP_CMD_AIR_AT, // only valid for COO
    SAPP_CMD_AIR_RSP, // only valid for COO
    SAPP_CMD_AIR_DATA, // coo ->other
    
    SAPP_CMD_MODBUS = 0x28,

    SAPP_CMD_DBG_INFO = 0x30,

    //user defined 
    SAPP_CMD_USER  = 0X40,

    // Innder comm
    SAPP_CMD_INNER  = 0X50,
    
    
        
}SAPP_CMD_ENUM;


typedef enum
{
    SAPI_CMD_DEV_STATE = 0x0,
    SAPI_CMD_DATA      = 0x21, /* same as SAPP_CMD_DATA for compatible reason*/
        
}SAPI_CMD_ENUM;



#define SAPP_PAYLOAD_LEN(len) (len-RPC_FRAME_HDR_SZ)

#endif
