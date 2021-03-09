#include "mifare.h"
#include "stdio.h"
#include "pn532.h"
#include "spi.h"
#include "string.h"

extern uint8_t u95HFBuffer [RFTRANS_95HF_MAX_BUFFER_SIZE+3];


void mifare_get_uid(uint8_t* uid)
{
    ISO14443A_REQA();
    memcpy(uid, u95HFBuffer, 4);
}

//mifare
void mifare_dump(void)
{
    PN532 pn532;
    uint8_t buff[255];
    uint32_t pn532_error = PN532_ERROR_NONE;
    uint8_t key_a[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    uint8_t uid[MIFARE_UID_MAX_LENGTH];
    uint32_t uid_len = 0;
    
    printf("Reading blocks...\r\n");
    for (uint8_t block_number = 0; block_number < 64; block_number++) 
    {
        pn532_error = PN532_MifareClassicAuthenticateBlock(&pn532, uid, uid_len,
        block_number, MIFARE_CMD_AUTH_A, key_a);
        if (pn532_error != PN532_ERROR_NONE) 
        {
            break;
        }
        pn532_error = PN532_MifareClassicReadBlock(&pn532, buff, block_number);
        if (pn532_error != PN532_ERROR_NONE) 
        {
            break;
        }
        printf("%d: ", block_number);
        for (uint8_t i = 0; i < 16; i++) 
        {
            printf("%02x ", buff[i]);
        }
        printf("\r\n");
    }
    if (pn532_error) 
    {
        printf("Error: 0x%02x\r\n", pn532_error);
    }
}




