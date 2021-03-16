#include "lib_nfcm1picc.h"

// ATQA
static uint8_t rATQA_1k[2]    = {0x04, 0x00};                               // indicate Mifare classic 1k 4Byte UID

// SAK
static uint8_t rSAK_1k   = 0x08;    // mifare 1k

//UID
static uint8_t uid[4] = {0x11, 0x22, 0x33, 0x44};

//dumpbyte
static uint8_t dumpbyte[12] = {'M','1','e', 'm', 'u', 'l', 'a', 't', 'i', 'o', 'n'};

//Manufacturer block 
static uint8_t manufacturerblock[16];

extern uint8_t          u95HFBuffer [RFTRANS_95HF_MAX_BUFFER_SIZE+3];       

//writing flag
static bool IsWritting = false;
    
//send buffer 
static uint8_t M1_SendBuffer[255] ;

typedef struct SectorTrailer{
    uint8_t key_a[6];           //keya
    uint8_t access[4];          //¿ØÖÆ×Ö½Ú
    uint8_t key_b[6];           //keyb
}Trailer_t;


typedef struct MifareSector{
    uint8_t datablock0[16];
    uint8_t datablock1[16];
    uint8_t datablock2[16];         //data block
    Trailer_t trailerblock;         //trailer block per sector
}Sector_t;

typedef struct MifareCard1K{
    Sector_t Sector[64];  
}Card1K_t;

static Card1K_t M1_Card1K;



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
//Respone Read
int8_t PICCNFCM1_Read(uc8 *pData)
{
    uint16_t addr;
	uint8_t BlockNumber;
	uint8_t	*pDataToEmit = M1_SendBuffer;
	
	if (pData[PICC_DATA_OFFSET] != M1_READ)
		return PICCNFCM1_ERRORCODE_COMMANDUNKNOWN;	
	
	BlockNumber = pData[PICC_DATA_OFFSET+1];
	
    memcpy(pDataToEmit, (uint8_t *)&M1_Card1K.Sector[BlockNumber], M1_READ_BUFFER_SIZE);
    
    pDataToEmit[M1_READ_BUFFER_SIZE] = SEND_MASK_APPENDCRC | SEND_MASK_8BITSINFIRSTBYTE;
    
	PICC_Send ((M1_READ_BUFFER_SIZE+1), pDataToEmit);
    
//	/* creates the card response  */
//	if( (addr+PICCNFCT2_READ_BUFFER_SIZE) > (NFCT2_MAX_TAGMEMORY - PICCNFCT2_READ_BUFFER_SIZE) )
//	{
//		if( addr < NFCT2_MAX_TAGMEMORY)
//		{
//			/* or SAK case? */
//			memcpy( pDataToEmit,&TT2Tag[addr],(NFCT2_MAX_TAGMEMORY-addr));
//			/* remaining byte set to 0 */
//			for (i=0; i< PICCNFCT2_READ_BUFFER_SIZE - (NFCT2_MAX_TAGMEMORY-addr) ; i++)
//			{
//				pDataToEmit[NFCT2_MAX_TAGMEMORY-addr+i] = 0;
//			}
//		}
//		else
//		{
//			/* or SAK case? */
//			for (i=0; i< PICCNFCT2_READ_BUFFER_SIZE; i++)
//			{
//				pDataToEmit[i] = 0;
//			}
//		}
//	}
//	else
//	{
//		memcpy( pDataToEmit,&TT2Tag[addr],PICCNFCT2_READ_BUFFER_SIZE);
////	}
//	/* control byte append CRC + 8 bits	*/
//  pDataToEmit [PICCNFCT2_READ_BUFFER_SIZE] = SEND_MASK_APPENDCRC | SEND_MASK_8BITSINFIRSTBYTE;

//	/* emits the card response to the reader*/
//	PICC_Send ((PICCNFCT2_READ_BUFFER_SIZE+1),pDataToEmit);
	
	return PICCNFCM1_SUCCESSCODE ;
}

//Respone Write
int8_t PICCNFCM1_Write(uc8 *pData)
{
    uint8_t BlockNumber;

    if (pData[PICC_DATA_OFFSET] != M1_WRITE)
        return PICCNFCM1_ERRORCODE_COMMANDUNKNOWN;
    
    IsWritting = true;
    
    BlockNumber = pData[PICC_DATA_OFFSET + 1];
    
//	addr = pData[PICC_DATA_OFFSET+1]*4 + PICCNFCT2_sector*PICCNFCT2_SECTOR_SIZE;
    
    /* Check if addr is correct */
    if (BlockNumber >= M1_BLOCK_MAXNUM)
    {
        M1_NACK();
        return PICCNFCM1_ERRORCODE_GENERIC;
    }

//	memcpy(&TT2Tag[addr],&pData[PICC_DATA_OFFSET+2],PICCNFCT2_WRITE_BUFFER_SIZE);
	/* Send an ACK */
	M1_ACK();
	
	/* This part allows to write the NDEF to the flash only when it is finished */
//	if (addr == 16 && writing == 0)
//		writing = 1;
//	else if (addr == 16)
//	{
//		writing = 0;
//		updateFlash = true;
//	}
	
	return PICCNFCM1_SUCCESSCODE ;	
}

//Init Card Data
int8_t M1_Card_init(void)
{
    memcpy(manufacturerblock, uid, M1_UID_SIZE);
    memcpy(manufacturerblock + M1_UID_SIZE, dumpbyte, 16 - M1_UID_SIZE);
    
    memcpy(M1_Card1K.Sector[0].datablock0, manufacturerblock, 16);
    
    return PICCNFCM1_SUCCESSCODE;
}


void M1_NACK (void)
{
	uc8 pDataToEmit[2] = {0x00,0x14};
	PICC_Send(2,pDataToEmit);
}


void M1_ACK (void)
{
	uc8 pDataToEmit[2] = {0xAA,0x14};
	PICC_Send(2,pDataToEmit);
}




/*  import from proxmark3 @https://github.com/Yinke7/proxmark3-1  **/











