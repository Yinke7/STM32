#include "lib_nfcm1picc.h"
#include "lib_ConfigManager.h"
// ATQA
static uint8_t rATQA_1k[2]    = {0x04, 0x00};               // indicate Mifare classic 1k 4Byte UID

// SAK
static uint8_t rSAK_1k   = 0x08;    // mifare 1k

//UID
static uint8_t uid[4] = {0x11, 0x22, 0x33, 0x44};

//dumpbyte
static uint8_t dumpbyte[12] = {'M', '1', ' ', 'e', 'm', 'u', 'l', 'a', 't', 'i', 'o', 'n'};

//default trialer block
static uint8_t defualttrialerblock[16] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
                                          0xFF,0x07,0x80,0x69,
                                          0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};


//Manufacturer block 
static uint8_t manufacturerblock[16];

     

//writing flag
static bool IsWritting = false;
    
//send buffer 
static uint8_t M1_SendBuffer[255] ;
																					
																					
																					
//extern varies
extern uint8_t          u95HFBuffer [RFTRANS_95HF_MAX_BUFFER_SIZE+3];  
extern DeviceMode_t devicemode;
extern TagType_t nfc_tagtype;               
extern uint8_t contentToWrite;
																					
typedef struct MifareBlock {
    uint8_t data[16];
}Block_t;


typedef struct MifareSector {
    Block_t datablock[3];               //data block
    Block_t trailerblock;               //trailer block per sector
}Sector_t;


typedef struct MifareCard1K {
    Sector_t Sector[64];
}Card1K_t;


//定义全局变量M1 卡 1K容量
static Card1K_t M1_Card1K;

//需要写入的页
static uint8_t pagetowrite;


int8_t PICCNFCM1_ReplyCommand( uc8 *pData )
{
	uint8_t	InsCode = pData[PICC_DATA_OFFSET];
	
    
    if(IsWritting == true)
    {
        //write data step2
        PICCNFCM1_Write_Step2(pData);
        
        IsWritting = false;
        return PICCNFCM1_SUCCESSCODE;
        
    }
    //M1 reader cmd
	switch (InsCode)
	{
        
//		case PICCNFCT2_SECTOR_SELECT :
//			PICCNFCT2_SelectSector ( pData );
//		break;
        
        //reqa
		case M1_REQA:
            /*Code REQA*/
            PICCNFCM1_ATQA(pData);
        break;

        //keyA authentication
        case M1_AUTHENTICATION_A:
            /*Code A*/
            break;
        
        //keyB authentication
        case M1_AUTHENTICATION_B:
            /*Code B*/
            break;
        
        //read data
		case M1_READ:
			PICCNFCM1_Read(pData);
		break;
		
        //write data
		case M1_WRITE:
            //write data step1
			PICCNFCM1_Write_Step1(pData, &pagetowrite);
            IsWritting = true;
        
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

//处理应答
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
	
    memcpy(pDataToEmit, (uint8_t *)&M1_Card1K.Sector[BlockNumber], M1_BYTE_PER_BLOCK_SIZE);
    
    pDataToEmit[M1_BYTE_PER_BLOCK_SIZE] = SEND_MASK_APPENDCRC | SEND_MASK_8BITSINFIRSTBYTE;
    
	PICC_Send ((M1_BYTE_PER_BLOCK_SIZE + 1), pDataToEmit);

	return PICCNFCM1_SUCCESSCODE ;
}


//Respone Write
int8_t PICCNFCM1_Write_Step1(uc8 *pData, uint8_t *pagenumber)
{
    uint8_t BlockNumber;

    if (pData[PICC_DATA_OFFSET] != M1_WRITE)
        return PICCNFCM1_ERRORCODE_COMMANDUNKNOWN;
    
    BlockNumber = pData[PICC_DATA_OFFSET + 1];
    
    
//	addr = pData[PICC_DATA_OFFSET+1]*4 + PICCNFCT2_sector*PICCNFCT2_SECTOR_SIZE;
    
    /* Check if addr is correct */
    if (BlockNumber >= M1_BLOCK_MAXNUM_SIZE)
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
	*pagenumber = BlockNumber;
	return PICCNFCM1_SUCCESSCODE ;      
    
}


//recieve data to write in block
int8_t PICCNFCM1_Write_Step2(uc8 * pData)
{
    if(pagetowrite >= M1_BLOCK_MAXNUM_SIZE)
        return PICCNFCM1_ERRORCODE_COMMANDUNKNOWN;
    
    //copy data block
    if((pagetowrite % 4) == 3)
    {
        /*
        block 3 is access block, can not be written  
        */
        
        printf("write block is forbaded\r\n");
        return PICCNFCM1_ERRORCODE_COMMANDUNKNOWN;
    }
    
    //copy trailer block
    else 
    {
        memcpy(&(M1_Card1K.Sector[pagetowrite/4].datablock[pagetowrite%4].data), pData, M1_BYTE_PER_BLOCK_SIZE) ;
    }
    
    //seng ACK
    M1_ACK();
    
    return PICCNFCM1_SUCCESSCODE;
}


//Init Card Data
int8_t M1_Card_init(void)
{
    uint8_t i;
    //uid
    memcpy(manufacturerblock, uid, M1_UID_SIZE);
    
    //other bytes 
    memcpy(manufacturerblock + M1_UID_SIZE, dumpbyte, M1_BYTE_PER_BLOCK_SIZE - M1_UID_SIZE);
    
    //copy to card
    memcpy( &(M1_Card1K.Sector[0].datablock[0]), manufacturerblock, M1_BYTE_PER_BLOCK_SIZE);
    
    for(i = 0;i < M1_BLOCK_MAXNUM_SIZE; i++)
    {
        memcpy( &(M1_Card1K.Sector[i].trailerblock), defualttrialerblock, M1_BYTE_PER_BLOCK_SIZE);
    }
    
    return PICCNFCM1_SUCCESSCODE;
}
//auth 1
int8_t PICCNFCM1_AUTHENTICATION_Step1(uint8_t * pData)
{
    
    return PICCNFCM1_SUCCESSCODE;
}

//auth 2
int8_t PICCNFCM1_AUTHENTICATION_Step2(uint8_t * pData)
{
    
    return PICCNFCM1_SUCCESSCODE;
}

//auth 3 
int8_t PICCNFCM1_AUTHENTICATION_Step3(uint8_t * pData)
{
    
    return PICCNFCM1_SUCCESSCODE;
}


//nack
void M1_NACK (void)
{
	uc8 pDataToEmit[2] = {0x00,0x14};
	PICC_Send(2,pDataToEmit);
}



//ack
void M1_ACK (void)
{
	uc8 pDataToEmit[2] = {0xAA,0x14};
	PICC_Send(2,pDataToEmit);
}



void TagEmulM1(void)
{
    M1_Card_init();
	ConfigManager_TagEmulation(PICCEMULATOR_TAG_TYPE_2,0,0);
}


/*  import from proxmark3 @https://github.com/Yinke7/proxmark3-1    */














