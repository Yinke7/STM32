#include "lib_nfcm1picc.h"
#include "lib_ConfigManager.h"
#include "mcc_crypto.h"
#include "mcc_crypto_card.h"
#include "mcc_macros.h"
#include "mcc_raw_request.h"
#include "mcc.h"
#include "mcc_macros.h"
// ATQA
static uint8_t rATQA_1k[2]    = {0x04, 0x00};               // indicate Mifare classic 1k 4Byte UID

// SAK
static uint8_t rSAK_1k   = 0x08;    // mifare 1k

//UID
static uint8_t carduid[4] = {0x11, 0x22, 0x33, 0x44};

//dumpbyte
static uint8_t dumpbyte[12] = {'M', '1', ' ', 'e', 'm', 'u', 'l', 'a', 't', 'i', 'o', 'n'};

//default trialer block
static uint8_t defualttrialerblock[16] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
                                          0xFF,0x07,0x80,0x69,
                                          0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};

//KeyA / B
uint8_t KeyA[6] = {0xff,0xff,0xff,0xff,0xff,0xff};
uint8_t KeyB[6] = {0xff,0xff,0xff,0xff,0xff,0xff};
                               


//Manufacturer block 
static uint8_t manufacturerblock[16];

     

//writing flag
static bool IsWritting = false;
    
//send buffer 
static uint8_t M1_SendBuffer[255] ;
static uint8_t M1_RecvBuffer[255] ;

//crypot 1
CryptoHandle_t handle_M1;    
uint16_t u16buffer[MCC_BUFFER_SIZE];

//AUTH ARCOS
                                          
#define MCC_AUTHENTICATION_STEP1_TIMEOUT    (10)
#define MCC_AUTHENTICATION_STEP2_TIMEOUT    (20)

#define CRC_PRELOAD_VALUE                   (0x6363)
#define CRC_LEN                             (2)

#define AUTH1_CMD_LEN           (4)
#define AUTH1_RSP_LEN           (8)

#define CARD_AUTH_CMD_LEN       (AUTH1_CMD_LEN)
#define CARD_AUTH_RSP_LEN       (AUTH1_RSP_LEN * 2)
#define CARD_AUTH2_CMD_LEN      (4)

#define UID_LEN     (4)

#define HIGH_BYTE(x)    (((x) >> 8) & 0xFF)
#define LOW_BYTE(x)     ((x)        & 0xFF)



//extern uint8_t        u95HFBuffer [RFTRANS_95HF_MAX_BUFFER_SIZE+3];  
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
	uint8_t i ;
    uint8_t rtvale = PICCNFCM1_SUCCESSCODE;
    uint8_t blocknum;
    //uint8_t noncecard[4] = {0x01,0x02,0x03,0x04};
	uint8_t noncecard[4] = {0x31,0x32,0x33,0x35};
    
    
    
    if(IsWritting == true)
    {
        //write data step2
        PICCNFCM1_Write_Step2(pData);
        
        IsWritting = false;
        return PICCNFCM1_SUCCESSCODE;
        
    }
    
    //M1 reader cmd 60是啥？读uid命令？验证。那tm的要先验证通过才能读到uid？
    //可以先读uid,流程没对啊，他这没先读uid的嘛，以来就验证
    printf("<<< ");
    for (i = 0 ;i <pData[PICC_LENGTH_OFFSET] + 2; i++)
    {
        printf("[%02X] ", pData[i]);
    }
		printf("\r\n");
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
            
//            PICCNFCM1_AUTHENTICATION_Step1(pData);
            blocknum = pData[PICC_DATA_OFFSET + 1];
            
            AuthenticateCardStep1(MCC_AUTH_KEY_A, blocknum, carduid, sizeof(carduid), KeyA, noncecard);
            /*Code A*/
            break;
        
        //keyB authentication
        case M1_AUTHENTICATION_B:
            /*Code B*/
            break;
        
        //read data读uid也会进入这里？读是 
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
            rtvale =  PICCNFCM1_ERRORCODE_COMMANDUNKNOWN;
        
        break;
	}
    
	return rtvale; 
}

//处理应答
int8_t PICCNFCM1_ATQA(uc8 *pData)
{
    uint8_t datalen = 2;
    uint8_t pDataToEmit[] = {0xAA, 0x14};
    if (pData[PICC_DATA_OFFSET] != M1_REQA)
		return PICCNFCM1_ERRORCODE_COMMANDUNKNOWN;	
    printf("REQA\r\n");//你这样，你把收到rc522的命令都打印出来
		
    pDataToEmit [datalen] = SEND_MASK_APPENDCRC | SEND_MASK_8BITSINFIRSTBYTE;
    PICC_Send(datalen + 1,pDataToEmit);
    
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
		//你这构造的不对吧
		//怎么不对
    uint8_t i;
    //uid
    memcpy(manufacturerblock, carduid, M1_UID_SIZE);
    
    //other bytes 
    memcpy(manufacturerblock + M1_UID_SIZE, dumpbyte, M1_BYTE_PER_BLOCK_SIZE - M1_UID_SIZE);
    
    //copy to card 这是啥，数据 block 0 第一个block
    memcpy( &(M1_Card1K.Sector[0].datablock[0]), manufacturerblock, M1_BYTE_PER_BLOCK_SIZE);
    
    for(i = 0;i < M1_BLOCK_MAXNUM_SIZE; i++)
    {
        memcpy( &(M1_Card1K.Sector[i].trailerblock), defualttrialerblock, M1_BYTE_PER_BLOCK_SIZE);
    }
    
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

//添加校验位
static int8_t attachParityInformation ( uint16_t *cmd, size_t length )
{
    unsigned int i = 0;
    for ( i = 0; i < length; i++ )
    {
        if ( mccParityOdd( cmd[i] ) == 1 )
        {
            cmd[i] = (cmd[i] << 8) ^ 0x80;
        }
        else
        {
            cmd[i] = (cmd[i] << 8) ^ 0x00;
        }
    }

    return ERR_NONE;
}

//

//发送四字节随机数给reader
int8_t AuthenticateCardStep1(const uint8_t keySelect,
                                uint8_t block,
                                const uint8_t *uid,
                                uint8_t uidLength,
                                const uint8_t *key,
                                uint8_t *nonce)
{
    uint8_t rv = ERR_NONE;
    uint8_t cmd[CARD_AUTH_CMD_LEN];
    uint16_t bytesReceived;
    uint8_t rspone[CARD_AUTH_RSP_LEN];
    uint16_t rsp2crypto[AUTH1_RSP_LEN];
    uint32_t uid32;
		uint8_t pDataToEmit[255];
    
		int i;
    int ii;
		u8 TempByte;
    if( uidLength != UID_LEN || uid == 0 )
    {
        rv = ERR_PARAM;
        goto out;
    }
    if(key == 0)
    {
        rv = ERR_PARAM;
        goto out;
    }
		
    UINT32_T_TO_ARRAY_PARITY( TO_UINT32_T(nonce), u16buffer );
    //将单字节的cmd放到双字节的buffer中，便于下一步添加校验字节
//    for (i = 0; i < CARD_AUTH_CMD_LEN; i++)
//    {
//        u16buffer[i] = nonce[i];
//    }
    
    
    //添加奇偶校验
//    rv = attachParityInformation( u16buffer, CARD_AUTH_CMD_LEN );
    for( ii = 0; ii < CARD_AUTH_CMD_LEN; ii++)
		{
			 TempByte = u16buffer[ii] >> 8;
			u16buffer[ii] = (u16buffer[ii] <<8) | (TempByte << 7);
		}
		
		printf("u16buffer ");
		for ( i = 0;i < CARD_AUTH_CMD_LEN; i++)
		{
			printf("[%04X] ",u16buffer[i]);
		}
		printf("\r\n");
		
		//reset the respone array 
			memset(rspone, 0, CARD_AUTH_RSP_LEN);
    //发送命令并接收回应数据
		//这个函数中的ST95命令(0x04 SENDRECIEVE)是ST95作为reader,向tag发送数据是用的；
		//而当st95作为tag,向reader发送数据时应该用(0x06 SEND)
//    rv = mccSendRawRequest( u16buffer, CARD_AUTH_CMD_LEN,
//                    rspone, CARD_AUTH_RSP_LEN,
//                    &bytesReceived, 10, 0 );
		
		
		for (i = 0; i < CARD_AUTH_CMD_LEN; i++)
		{
//			pDataToEmit[i * 2] = u16buffer[i]>>8;
//			pDataToEmit[i * 2 + 1] = u16buffer[i]&0xff;
			pDataToEmit[i] = u16buffer[i]>>8 & 0xff;
		}
		
//		pDataToEmit [CARD_AUTH_CMD_LEN * 2] = SEND_MASK_APPENDCRC | SEND_MASK_8BITSINFIRSTBYTE;
		pDataToEmit [CARD_AUTH_CMD_LEN] = SEND_MASK_APPENDCRC | SEND_MASK_8BITSINFIRSTBYTE;
		
		rv = M1_PICC_Send(CARD_AUTH_CMD_LEN + 1,pDataToEmit);
		
		
    if(rv != 0)
		{
			printf("err \r\n");
		}
//    printf("<<< ");
//    for (i = 0; i < CARD_AUTH_RSP_LEN; i++)
//    {
//        printf("[%02X]",rspone[i]);
//    }
//		printf("\r\n");
    
    //接收的rsp字节数不等于预期值  
    if ( bytesReceived != 8)
    {
        printf( "E: received %d bytes, expected %d bytes -> abort!\r\n", bytesReceived, 8);
        printf( "I: Failed: Auth Step 1\r\n");
    }
    
    
    //先重置寄存器
    mccCryptoReset( &handle_M1, 1 );
    //导入密钥key
//    mccSetKey( key );
    mccSetKeyCard(key);
    
    //加密第一步，使用uid和随机数
    mccCryptoAuthCardStep1 (&handle_M1, TO_UINT32_T(uid), TO_UINT32_T(nonce));
    uid32 = TO_UINT32_T( uid );
    printf( "I: uid: 0x%X%X\r\n", (uint16_t)( uid32 >> 16 ), (uint16_t)uid32);		
    printf( "I: N_c: 0x%X%X\r\n", (uint16_t)( TO_UINT32_T(rspone) >> 16 ), (uint16_t) TO_UINT32_T(rspone));
    printf( "D: odd = 0x%x%x\r\n", (uint16_t)(handle_M1.lfsr_odd >> 16), (uint16_t)&handle_M1.lfsr_odd);
    printf( "D: evn = 0x%x%x\r\n", (uint16_t)(handle_M1.lfsr_even >> 16), (uint16_t)&handle_M1.lfsr_even);
    
    for (i = 0; i< AUTH1_RSP_LEN; i++)
    {
        
    }
    
    //加密第二步,解析rsp
    mccCryptoAuthCardStep2(&handle_M1, (uint16_t *)rspone);
    
    
    
out:
    return rv;
}

void mccSetKeyCard( const uint8_t *key )
{
    uint64_t bigKey = 0;
    unsigned int i;

    //sprintf( dataOut, "mcc set key\n");
    //HAL_UART_Transmit( &UartHandle, ( uint8_t * )dataOut, strlen( dataOut ), 500 );

    if ( key == 0 )
    {
				//sprintf( dataOut, "no key passed!\n");
				//HAL_UART_Transmit( &UartHandle, ( uint8_t * )dataOut, strlen( dataOut ), 500 );
        return;
    }

    // Copy key into an variable that is big enouth to hold the 
    // 48bits of the key at once
    for ( i = 0; i < 6; i++ )
    {
        bigKey |= ((uint64_t)key[i] << ((5-i)*8));
    }

		//sprintf( dataOut, "  orig key:");
		//HAL_UART_Transmit( &UartHandle, ( uint8_t * )dataOut, strlen( dataOut ), 500 );
//    MCC_DUMP( key, 6 );
		//sprintf( dataOut, "  trans key: ");
		//HAL_UART_Transmit( &UartHandle, ( uint8_t * )dataOut, strlen( dataOut ), 500 );
		
//		sprintf( dataOut, " %x %x %x %x\n", (uint16_t)(bigKey >> 48),
//                              (uint16_t)(bigKey >> 32),
//                              (uint16_t)(bigKey >> 16),
//                              (uint16_t)(bigKey >> 0)
//                            );
		//HAL_UART_Transmit( &UartHandle, ( uint8_t * )dataOut, strlen( dataOut ), 500 );

    mccCryptoInit( &handle_M1, bigKey );
}

int8_t mccAuthenticateCardStep2(uint8_t *nonce)
{
    uint16_t cmd[CARD_AUTH2_CMD_LEN];
    uint8_t rsp[CARD_AUTH_RSP_LEN];
    uint16_t bytesReceived;
    int8_t rv = 0;
	uint8_t ii = 0;
	uint8_t TempByte;
    uint32_t i;
    
    
    
//    mccCryptoAuthCardStep2(&handle_M1,rsp)
    return rv;

}




ReturnCode MifareAuthenticationCard(uint8_t authentication_key,uint8_t *key,uint8_t block)
{
    
    uint16_t rtvalue ;
    
    
    return rtvalue;
    
}
int8_t M1_PICC_Send(uc8 Length,uc8 *Parameters)	
{
	uint8_t i;
	uint8_t *DataToSend = M1_SendBuffer,
					*pResponse = M1_RecvBuffer;

	/* check the function parameters	*/
	if (CHECKVAL (Length,1,255)==false)
		return PICC_ERRORCODE_PARAMETERLENGTH; 
	/* build the command */
	DataToSend[PICC_COMMAND_OFFSET ] = SEND;
	DataToSend[PICC_LENGTH_OFFSET  ] = Length;	

	/* DataToSend CodeCmd Length Data	*/
	/* Parameters[0] first byte to emmit	*/
	memcpy(&(DataToSend[PICC_DATA_OFFSET]),Parameters,Length);
	
	printf(">>> ");
		for ( i = 0;i < Length + 2; i++)
		{
			printf("[%02X] ",DataToSend[i]);
		}
		printf("\r\n");
	
	
	/* send the command to the PICC and retrieve its response */
	drv95HF_SendReceive(DataToSend, pResponse);
	
	/* check the PICC response */
	if ( PICC_IsResultCodeOk ( SEND , pResponse ) != PICC_SUCCESSCODE)
	{
		return PICC_ERRORCODE_DEFAULT;
	}

	return PICC_SUCCESSCODE;
}














