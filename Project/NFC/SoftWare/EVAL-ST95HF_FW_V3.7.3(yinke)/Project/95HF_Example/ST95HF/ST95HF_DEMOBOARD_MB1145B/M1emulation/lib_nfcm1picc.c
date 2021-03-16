#include "lib_nfcm1picc.h"

// ATQA
static uint8_t rATQA_1k[2]    = {0x04, 0x00};                               // indicate Mifare classic 1k 4Byte UID

// SAK
static uint8_t rSAK_1k   = 0x08;    // mifare 1k

//buffer
extern uint8_t          u95HFBuffer [RFTRANS_95HF_MAX_BUFFER_SIZE+3];       



int8_t PICCNFCM1_ReplyCommand( uc8 *pData )
{
	uint8_t	InsCode = pData[PICC_DATA_OFFSET];
	
	switch (InsCode)
	{
//		case PICCNFCT2_SECTOR_SELECT :
//			PICCNFCT2_SelectSector ( pData );
//		break;
		case M1_REQA:
            /*Code R*/
            PICCNFCM1_ATQA(pData);
        break;
        

        case M1_AUTHENTICATION_A:
            /*Code A*/
            break;
        case M1_AUTHENTICATION_B:
            /*Code B*/
            break;
        
		case M1_READ:
			PICCNFCM1_Read(pData);
		break;
		
		case M1_WRITE:
			PICCNFCM1_Write(pData);
		break;
        
		default:
//			if (flag_sector_select == 1) /* Catches the SELECT_SECTOR packet 2 */
//				PICCNFCT2_SelectSector ( pData );
//			else
				return PICCNFCM1_ERRORCODE_COMMANDUNKNOWN;
        break;
	}
	
	return PICCNFCM1_SUCCESSCODE; 
}


int8_t PICCNFCM1_ATQA(uc8 *pData)
{
    uc8 pDataToEmit[2] = {0xAA, 0x14};
    if (pData[PICC_DATA_OFFSET] != M1_REQA)
		return PICCNFCM1_ERRORCODE_COMMANDUNKNOWN;	
    
    PICC_Send(2,pDataToEmit);
    
}
int8_t PICCNFCM1_Read(uc8 *pData)
{
    
}
int8_t PICCNFCM1_Write(uc8 *pData)
{
    
}



/*  import from proxmark3 @https://github.com/Yinke7/proxmark3-1  **/











