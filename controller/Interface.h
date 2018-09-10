#ifndef _INTERFACE_H_
#define _INTERFACE_H_

#if (!defined(UINT8)) 
   typedef unsigned char            UINT8;
#endif

#if (!defined(UINT16))
   typedef unsigned short             UINT16;
#endif

#if (!defined(UINT32))
   typedef unsigned int            UINT32;
#endif

#if (!defined(uint8)) 
   typedef unsigned char            uint8;
#endif

#if (!defined(uint16))
   typedef unsigned short             uint16;
#endif

#if (!defined(uint32))
   typedef unsigned int            uint32;
#endif

#if (!defined(TRUE))
   #define TRUE                     1
#endif

#if (!defined(FALSE))
   #define FALSE                     0
#endif

// for SBL

#define SBL_FORCE_BOOT               0xF8
#define SBL_FORCE_RUN               (SBL_FORCE_BOOT ^ 0xFF)


typedef enum
{
  rpcSteSOF,
  rpcSteLen,
  rpcSteData,
  rpcSteFcs
} rpcSte_t;

#define SBL_RW_BUF_LEN               64

// Commands to Bootloader
typedef enum
{
    // Commands to Bootloader
    SBL_WRITE_CMD                = 0x01,
    SBL_READ_CMD                 = 0x02,
    SBL_ENABLE_CMD               = 0x03,
    SBL_HANDSHAKE_CMD            = 0x04,
    SBL_QUERY_ID_CMD             = 0x05,
    SBL_QUERY_VERSION_CMD        = 0x06,
    SBL_SET_ADDR_CMD             = 0x07,
    SBL_SET_BOOTP_CMD            = 0x08,
    SBL_FILE_SIZE_CMD            = 0x09,
    
    // Commands to Target Application
    SBL_TGT_BOOTLOAD             = 0x10,  // Erase the image valid signature & jump to bootloader.
}SAPP_CMD_BOOT_ENUM;

// Responses from Bootloader - for historical consistency, SBL has OR'ed the MSBit of all commands
// when responding - this is probably not necessary for smooth functionality.
#define SBL_RSP_MASK                 0x80

// Status codes
#define SBL_SUCCESS                  0
#define SBL_FAILURE                  1
#define SBL_INVALID_FCS              2
#define SBL_INVALID_FILE             3
#define SBL_FILESYSTEM_ERROR         4
#define SBL_ALREADY_STARTED          5
#define SBL_NO_RESPOSNE              6
#define SBL_VALIDATE_FAILED          7
#define SBL_CANCELED                 8
#define SBL_IGNORED                  9

// Indices into the RPC data (RPC_POS_DAT0):
#define SBL_REQ_ADDR_LSB             RPC_POS_DAT0
#define SBL_REQ_ADDR_MSB            (SBL_REQ_ADDR_LSB+1)
#define SBL_REQ_DAT0                (SBL_REQ_ADDR_MSB+1)
#define SBL_RSP_STATUS               RPC_POS_DAT0
#define SBL_RSP_ADDR_LSB            (SBL_RSP_STATUS+1)
#define SBL_RSP_ADDR_MSB            (SBL_RSP_ADDR_LSB+1)
#define SBL_RSP_DAT0                (SBL_RSP_ADDR_MSB+1)
#define SBL_READ_HDR_LEN            (SBL_RSP_DAT0 - SBL_RSP_STATUS)

// Buffer size - it has to be big enough for the largest RPC packet and overhead.
#define SBL_BUF_SIZE                 196
#define SBL_MAX_SIZE                (SBL_BUF_SIZE - RPC_FRAME_HDR_SZ - RPC_UART_FRAME_OVHD)


#define BUILD_UINT32(Byte0, Byte1, Byte2, Byte3) \
          ((uint32)((uint32)((Byte0) & 0x00FF) \
          + ((uint32)((Byte1) & 0x00FF) << 8) \
          + ((uint32)((Byte2) & 0x00FF) << 16) \
          + ((uint32)((Byte3) & 0x00FF) << 24)))

#define BUILD_UINT16(loByte, hiByte) \
          ((uint16)(((loByte) & 0x00FF) + (((hiByte) & 0x00FF) << 8)))

#define HI_UINT16(a) (((a) >> 8) & 0xFF)
#define LO_UINT16(a) ((a) & 0xFF)

#define BUILD_UINT8(hiByte, loByte) \
          ((uint8)(((loByte) & 0x0F) + (((hiByte) & 0x0F) << 4)))

#define HI_UINT8(a) (((a) >> 4) & 0x0F)
#define LO_UINT8(a) ((a) & 0x0F)

#endif

