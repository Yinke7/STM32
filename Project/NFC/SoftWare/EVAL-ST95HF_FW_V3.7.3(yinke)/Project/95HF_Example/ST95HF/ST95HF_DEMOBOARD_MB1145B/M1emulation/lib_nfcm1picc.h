#ifndef _LIB_NFCM1PICC_H_
#define _LIB_NFCM1PICC_H_

#include "lib_PICC.h"


/*M1 Commands*/
#define M1_REQA                     0x26
#define M1_Wakeup                   0x52
#define M1_Cascadelevel1            0x93
#define M1_Cascadelevel2            0x97
#define M1_Anticollision            0x20
#define M1_Select                   0x70
#define M1_Halt                     0x5000
#define M1_AUTHENTICATION_A         0x60
#define M1_AUTHENTICATION_B         0x61
#define M1_READ                     0x30
#define M1_WRITE                    0xA2
#define M1_PersonalizeUIDUsage      0x40
#define M1_SET_MOD_TYPE             0x43
#define M1_Decrement                0xC0
#define M1_Increment                0xC1
#define M1_Restore                  0xC2
#define M1_Transfer                 0xB0


/*Macros*/
#define PICCNFCM1_SUCCESSCODE                       RESULTOK    
#define PICCNFCM1_ERRORCODE_COMMANDUNKNOWN          0x76           



/*M1 Functions*/
int8_t PICCNFCM1_ReplyCommand( uc8 *pData );
int8_t PICCNFCM1_Read(uc8 *pData);
int8_t PICCNFCM1_Write(uc8 *pData);
int8_t PICCNFCM1_ATQA(uc8 *pData);

#endif

