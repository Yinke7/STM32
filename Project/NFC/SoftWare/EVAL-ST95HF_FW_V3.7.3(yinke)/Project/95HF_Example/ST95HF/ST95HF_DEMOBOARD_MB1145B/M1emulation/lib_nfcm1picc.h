#ifndef _LIB_NFCM1PICC_H_
#define _LIB_NFCM1PICC_H_

#include "lib_PICC.h"


/*M1 Commands*/
#define M1_Request                  0x26
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





/*M1 Functions*/
int8_t PICCNFCM1_ReplyCommand( uc8 *pData );
int8_t PICCNFCM1_Read(uc8 *pData);
int8_t PICCNFCM1_Write(uc8 *pData);


#endif

