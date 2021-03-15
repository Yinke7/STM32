/*
 *****************************************************************************
 * Copyright by ams AG                                                       *
 * All rights are reserved.                                                  *
 *                                                                           *
 * IMPORTANT - PLEASE READ CAREFULLY BEFORE COPYING, INSTALLING OR USING     *
 * THE SOFTWARE.                                                             *
 *                                                                           *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS       *
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT         *
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS         *
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT  *
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,     *
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT          *
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,     *
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY     *
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT       *
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE     *
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.      *
 *****************************************************************************
 */
/*
 *      PROJECT:   AS3911 firmware
 *      $Revision: $
 *      LANGUAGE:  ANSI C
 */

/*!
 *
 *  \author G. Wagner
 *
 *  \brief MCC layer implementation
 *
 */

/*
 ******************************************************************************
 * INCLUDES
 ******************************************************************************
 */

#include "rfal_crc.h"
#include "lib_iso14443a.h"
#include "lib_iso14443Apcd.h"
#include "mcc_macros.h"
#include "mcc_crypto.h"
#include "mcc_crypto_reader.h"
#include "mcc_raw_request.h"
#include "mcc.h"
#include "utils.h"
#include "st_errno.h"
//#include "lib_95HFConfigManager.h"
#include "lib_ConfigManager.h"

/*
 ******************************************************************************
 * MACROS
 ******************************************************************************
 */

#if 0
#define MCC_DBG dbgLog
#define MCC_DUMP dbgHexDump
#else
#define MCC_DBG(...)
#define MCC_DUMP(...)
#endif
/*
 ******************************************************************************
 * DEFINES
 ******************************************************************************
 */
#define MCC_AUTHENTICATION_STEP1_TIMEOUT    (10)
#define MCC_AUTHENTICATION_STEP2_TIMEOUT    (20)

#define CRC_PRELOAD_VALUE                   (0x6363)
#define CRC_LEN                             (2)

#define AUTH_CMD_LEN    (4)
#define AUTH2_CMD_LEN   (8)
#define AUTH_RSP_LEN    (5)

#define UID_LEN         (4)

#define HIGH_BYTE(x) (((x) >> 8) & 0xFF)
#define LOW_BYTE(x)  ((x)        & 0xFF)


/*! Timeout of mifare read command in milliseconds. */
#define MCC_READ_TIMEOUT             5

/*! Timeout of mifare write command request transmission part in milliseconds. */
#define MCC_WRITE_COMMAND_TIMEOUT    10

/*! Timeout of mifare write command data transmission part in milliseconds. */
#define MCC_WRITE_DATA_TIMEOUT       10

#define mifareResponseLen            21


#define ISO14443A_MAX_UID_LENGTH 10
#define ISO14443A_MAX_CASCADE_LEVELS 3
/*
 ******************************************************************************
 * LOCAL VARIABLES
 ******************************************************************************
 */

uint16_t buffer[MCC_BUFFER_SIZE];
static CryptoHandle_t mccHandle;
CryptoHandle_t *handle = &mccHandle;



/*definition for mifare trial*/
ReturnCode err = ERR_NONE;
uint16_t numBytesReceived;
//uint8_t mifare_request[2];
ISO14443A_CARD card;

uint8_t mifare_reply;

extern ISO14443A_CARD 	ISO14443A_Card;
/*
 ******************************************************************************
 * LOCAL FUNCTION PROTOTYPES
 ******************************************************************************
 */
static int8_t buildCommand ( uint8_t *cmd, uint8_t keySelect, uint8_t block );
static int8_t attachParityInformation( uint16_t *cmd, size_t length);
static ReturnCode attachParityInformationOld ( uint16_t *cmd, size_t length );

/*
 ******************************************************************************
 * GLOBAL FUNCTIONS
 ******************************************************************************
 */
int8_t mccInitialize ( void )
{
    int8_t rv = ERR_NONE;

    sprintf( dataOut, "mcc init\n");
    //HAL_UART_Transmit( &UartHandle, ( uint8_t * )dataOut, strlen( dataOut ), 500 );
	
    //rv = ISO14443A_Init();

    mccResetCipher();

    return rv;
}

int8_t mccDeinitialise ( uint8_t keep )
{
    sprintf( dataOut, "mcc deinit\n");
    //HAL_UART_Transmit( &UartHandle, ( uint8_t * )dataOut, strlen( dataOut ), 500 );
	
    if (!keep)
    {
			PCD_FieldOff();
    }

    return ERR_NONE;

}

void mccSetKey ( const uint8_t *key )
{
    uint64_t bigKey = 0;
    unsigned int i;

    sprintf( dataOut, "mcc set key\n");
    //HAL_UART_Transmit( &UartHandle, ( uint8_t * )dataOut, strlen( dataOut ), 500 );

    if ( key == 0 )
    {
				sprintf( dataOut, "no key passed!\n");
				//HAL_UART_Transmit( &UartHandle, ( uint8_t * )dataOut, strlen( dataOut ), 500 );
        return;
    }

    // Copy key into an variable that is big enouth to hold the 
    // 48bits of the key at once
    for ( i = 0; i < 6; i++ )
    {
        bigKey |= ((uint64_t)key[i] << ((5-i)*8));
    }

		sprintf( dataOut, "  orig key:");
		//HAL_UART_Transmit( &UartHandle, ( uint8_t * )dataOut, strlen( dataOut ), 500 );
    MCC_DUMP( key, 6 );
		sprintf( dataOut, "  trans key: ");
		//HAL_UART_Transmit( &UartHandle, ( uint8_t * )dataOut, strlen( dataOut ), 500 );
		
		sprintf( dataOut, " %x %x %x %x\n", (uint16_t)(bigKey >> 48),
                              (uint16_t)(bigKey >> 32),
                              (uint16_t)(bigKey >> 16),
                              (uint16_t)(bigKey >> 0)
                            );
		//HAL_UART_Transmit( &UartHandle, ( uint8_t * )dataOut, strlen( dataOut ), 500 );

    mccCryptoInit( handle, bigKey );

}

void mccResetCipher ( void )
{
		sprintf( dataOut, "mcc reset\n");
		//HAL_UART_Transmit( &UartHandle, ( uint8_t * )dataOut, strlen( dataOut ), 500 );
    mccCryptoReset( handle, 0 );
}

int8_t mccAuthenticate ( uint8_t keySelect,
                     uint8_t block,
                     uint8_t *uid,
                     uint8_t uidLength,
                     uint8_t *key,
                     uint32_t nonce )
{
    int8_t rv = 0;

		sprintf( dataOut, "mcc auth\n");
		//HAL_UART_Transmit( &UartHandle, ( uint8_t * )dataOut, strlen( dataOut ), 500 );
	
    if ( uid == 0  || uidLength != UID_LEN )
    {
				sprintf( dataOut, "E: uid incorrect! Expected %d bytes, got %d -> abort!\n", UID_LEN, uidLength);
				//HAL_UART_Transmit( &UartHandle, ( uint8_t * )dataOut, strlen( dataOut ), 500 );
				sprintf( dataOut, "I; Failed authentication step 1\n");
				//HAL_UART_Transmit( &UartHandle, ( uint8_t * )dataOut, strlen( dataOut ), 500 );
			
        rv = ERR_PARAM;
        goto out;
    }

    if ( key == 0 )
    {
				sprintf( dataOut, "E: no key given!\n");
				//HAL_UART_Transmit( &UartHandle, ( uint8_t * )dataOut, strlen( dataOut ), 500 );
				sprintf( dataOut, "I; Failed authentication step 1\n");
				//HAL_UART_Transmit( &UartHandle, ( uint8_t * )dataOut, strlen( dataOut ), 500 );	
        rv = ERR_PARAM;
        goto out;
    }

		sprintf( dataOut, "  uid  : %x\n", *uid);
		//HAL_UART_Transmit( &UartHandle, ( uint8_t * )dataOut, strlen( dataOut ), 500 );
		sprintf( dataOut, "  block: %x\n", block);
		//HAL_UART_Transmit( &UartHandle, ( uint8_t * )dataOut, strlen( dataOut ), 500 );
		
    rv = mccAuthenticateStep1( keySelect, block, uid, uidLength, key );
    EVAL_ERR_NE_GOTO( ERR_NONE, rv, out );

    rv = mccAuthenticateStep2( nonce );

out:
    return rv;
}

int8_t mccAuthenticateStep1 ( const uint8_t keySelect,
                          uint8_t block,
                          const uint8_t *uid,
                          uint8_t uidLength,
                          const uint8_t *key )
{
    uint8_t rv = ERR_NONE;
    uint8_t cmd[AUTH_CMD_LEN];
    uint16_t bytesReceived;
    uint8_t rsp[AUTH_RSP_LEN];
    uint32_t uid32;
    int i;
    
		sprintf( dataOut, "A: mcc auth step 1\n");
		//HAL_UART_Transmit( &UartHandle, ( uint8_t * )dataOut, strlen( dataOut ), 500 );
    if ( uidLength != UID_LEN || uid == 0 )
    {
				sprintf( dataOut, "E: uid incorrect! Expected %d bytes, got %d -> abort!\n", UID_LEN, uidLength);
				//HAL_UART_Transmit( &UartHandle, ( uint8_t * )dataOut, strlen( dataOut ), 500 );
				sprintf( dataOut, "I; Failed authentication step 1\n");
				//HAL_UART_Transmit( &UartHandle, ( uint8_t * )dataOut, strlen( dataOut ), 500 );
        rv = ERR_PARAM;
        goto out;
    }

    if ( key == 0 )
    {
				sprintf( dataOut, "E: no key given!\n");
				//HAL_UART_Transmit( &UartHandle, ( uint8_t * )dataOut, strlen( dataOut ), 500 );
				sprintf( dataOut, "I; Failed authentication step 1\n");
				//HAL_UART_Transmit( &UartHandle, ( uint8_t * )dataOut, strlen( dataOut ), 500 );			
        rv = ERR_PARAM;
        goto out;
    }

		sprintf( dataOut, "A: mcc auth step 1\n");
		//HAL_UART_Transmit( &UartHandle, ( uint8_t * )dataOut, strlen( dataOut ), 500 );		
    rv = buildCommand( cmd, keySelect, block );
    EVAL_ERR_NE_GOTO( ERR_NONE, rv, out );

    for ( i = 0; i < AUTH_CMD_LEN; i++ )
    {
        buffer[i] = cmd[i];
    }

    rv = attachParityInformation( buffer, AUTH_CMD_LEN );
    EVAL_ERR_NE_GOTO( ERR_NONE, rv, out );

    if( mccCryptoIsActive( handle ) == 1 ) 
    {
        mccCryptoTranscode( handle, buffer, AUTH_CMD_LEN, 0 );
    }

    rv = mccSendRawRequest( buffer, AUTH_CMD_LEN,
                    rsp, AUTH_RSP_LEN,
                    &bytesReceived, MCC_AUTHENTICATION_STEP1_TIMEOUT, 0 );
    EVAL_ERR_NE_GOTO( ERR_NONE, rv, out );

    if ( bytesReceived != AUTH_RSP_LEN - 1)
    {
				sprintf( dataOut, "E: received 0x%x bytes, expected %d bytes -> abort!\n", bytesReceived, AUTH_RSP_LEN - 1);
				//HAL_UART_Transmit( &UartHandle, ( uint8_t * )dataOut, strlen( dataOut ), 500 );				
				sprintf( dataOut, "I: Failed: Auth Step 1\n");
				//HAL_UART_Transmit( &UartHandle, ( uint8_t * )dataOut, strlen( dataOut ), 500 );					
        MCC_DUMP( rsp, bytesReceived );
        mccResetCipher( );
        return ERR_NOTFOUND;
    }

    mccCryptoReset( handle, 1 );
    mccSetKey( key );

    mccCryptoAuthReaderStep1( handle, TO_UINT32_T( uid ), TO_UINT32_T( rsp ) );

    uid32 = TO_UINT32_T( uid );

		sprintf( dataOut, "I: uid: %X%X\n", (uint16_t)( uid32 >> 16 ), (uint16_t)uid32);
		//HAL_UART_Transmit( &UartHandle, ( uint8_t * )dataOut, strlen( dataOut ), 500 );				
		sprintf( dataOut, "I: N_c: 0x%X%X\n", (uint16_t)( TO_UINT32_T(rsp) >> 16 ), (uint16_t) TO_UINT32_T(rsp));
		//HAL_UART_Transmit( &UartHandle, ( uint8_t * )dataOut, strlen( dataOut ), 500 );	
		sprintf( dataOut, "D: odd = 0x%x%x\n", (uint16_t)(handle->lfsr_odd >> 16), (uint16_t)handle->lfsr_odd);
		//HAL_UART_Transmit( &UartHandle, ( uint8_t * )dataOut, strlen( dataOut ), 500 );			
		sprintf( dataOut, "D: evn = 0x%x%x\n", (uint16_t)(handle->lfsr_even >> 16), (uint16_t)handle->lfsr_even);
		//HAL_UART_Transmit( &UartHandle, ( uint8_t * )dataOut, strlen( dataOut ), 500 );	
#ifdef DEBUG_LFSR
		sprintf( dataOut,  "I: LFSR: 0x%X%X%X%X\n", (uint16_t)( handle->lfsr_lfsr >> 48 )
                                    , (uint16_t)( handle->lfsr_lfsr >> 32 )
                                    , (uint16_t)( handle->lfsr_lfsr >> 16 )
                                    , (uint16_t)( handle->lfsr_lfsr ) );
		HAL_UART_Transmit( &UartHandle, ( uint8_t * )dataOut, strlen( dataOut ), 500 );

#endif // DEBUG_LFSR
		sprintf( dataOut, "I: Success: Auth Step 1\n");
		//HAL_UART_Transmit( &UartHandle, ( uint8_t * )dataOut, strlen( dataOut ), 500 );	

out:
    return rv;
}

int8_t mccAuthenticateStep2 ( uint32_t nonce )
{
    // Current nonce + generated nonce are in the cmd
    // for step two. Therefore the size is twice as big
    // as before.
    uint16_t cmd[AUTH2_CMD_LEN];
    uint8_t rsp[AUTH_RSP_LEN];
    uint16_t bytesReceived;
    int8_t rv = 0;
	uint8_t ii = 0;
	uint8_t TempByte;
    unsigned int i;

		sprintf( dataOut, "A: mcc auth step 2\n");
		//HAL_UART_Transmit( &UartHandle, ( uint8_t * )dataOut, strlen( dataOut ), 500 );	
		sprintf( dataOut,  "I: N_r: 0x%x%x\n", (uint16_t)(nonce >> 16), (uint16_t)(nonce));
		//HAL_UART_Transmit( &UartHandle, ( uint8_t * )dataOut, strlen( dataOut ), 500 );		

    // 1. Set nonce for reader
    UINT32_T_TO_ARRAY_PARITY( nonce, cmd );

    // 2. Step 2 of authentication in the crypto algorithm
    mccCryptoAuthReaderStep2( handle, cmd );

		sprintf( dataOut,  "D: odd = 0x%x%x\n", (uint16_t)(handle->lfsr_odd >> 16), (uint16_t)handle->lfsr_odd);
		//HAL_UART_Transmit( &UartHandle, ( uint8_t * )dataOut, strlen( dataOut ), 500 );		
		
		sprintf( dataOut,  "D: evn = 0x%x%x\n", (uint16_t)(handle->lfsr_even >> 16), (uint16_t)handle->lfsr_even );
		//HAL_UART_Transmit( &UartHandle, ( uint8_t * )dataOut, strlen( dataOut ), 500 );
		
#ifdef DEBUG_LFSR
		sprintf( dataOut,  "I: LFSR: 0x%X%X%X%X\n", (uint16_t)( handle->lfsr_lfsr >> 48 )
                                    , (uint16_t)( handle->lfsr_lfsr >> 32 )
                                    , (uint16_t)( handle->lfsr_lfsr >> 16 )
                                    , (uint16_t)( handle->lfsr_lfsr ) );
		HAL_UART_Transmit( &UartHandle, ( uint8_t * )dataOut, strlen( dataOut ), 500 );
#endif // DEBUG_LFSR


		for( ii = 0; ii < AUTH2_CMD_LEN; ii++)
		{
			 TempByte = cmd[ii] >> 8;
			cmd[ii] = (cmd[ii] <<8) | (TempByte << 7);
		}
		
    // 2. send it back to the card
    rv = mccSendRawRequest( cmd, AUTH2_CMD_LEN,
                     rsp, AUTH_RSP_LEN,
                     &bytesReceived, MCC_AUTHENTICATION_STEP2_TIMEOUT, 0 );
    EVAL_ERR_NE_GOTO( ERR_NONE, rv, out );

    // 2.1 check if response contains 4 bytes only
    if ( bytesReceived != 4 )
    {
				sprintf( dataOut,   "D: odd = 0x%x%x\n", (uint16_t)(handle->lfsr_odd >> 16), (uint16_t)handle->lfsr_odd);
				//HAL_UART_Transmit( &UartHandle, ( uint8_t * )dataOut, strlen( dataOut ), 500 );		
				sprintf( dataOut,   "I: Failed: Auth Step 2\n" );
				//HAL_UART_Transmit( &UartHandle, ( uint8_t * )dataOut, strlen( dataOut ), 500 );	
        MCC_DUMP( rsp, bytesReceived );
        mccResetCipher( );
        return ERR_NOTFOUND;
    }

    // 3. convert back
    for ( i = 0; i < AUTH_RSP_LEN; i ++ )
    {
        buffer[i] = rsp[i];
    }

    // 4. feed it to the crypto algorithm
    rv = mccCryptoAuthReaderStep3( handle, buffer );
    if ( rv != 1 )
    {
				sprintf( dataOut,   "E: failed authentication step 3!\n" );
				//HAL_UART_Transmit( &UartHandle, ( uint8_t * )dataOut, strlen( dataOut ), 500 );	
        mccResetCipher();
        return ERR_NOTFOUND;
    }

    rv = ERR_NONE;

		sprintf( dataOut,  "I: prng = 0x%x\n", (uint16_t)(handle->prng) );
		//HAL_UART_Transmit( &UartHandle, ( uint8_t * )dataOut, strlen( dataOut ), 500 );	

    mccCryptoSetActive( handle, 1 );

		sprintf( dataOut,  "I: state: %d\n", mccCryptoIsActive( handle ) );
		//HAL_UART_Transmit( &UartHandle, ( uint8_t * )dataOut, strlen( dataOut ), 500 );	

		sprintf( dataOut, "I: Success: Auth Step 2\n" );
		//HAL_UART_Transmit( &UartHandle, ( uint8_t * )dataOut, strlen( dataOut ), 500 );	

out:
    return rv;
}

int8_t mccSendRequest ( uint8_t *request,
                    uint16_t requestLength,
                    uint8_t *response,
                    uint16_t maxResponseLength,
                    uint16_t *responseLength,
                    uint16_t timeout,
                    uint8_t fourBitResponse )
{
    uint16_t crc;
    uint16_t bits = 0;
    uint16_t bytes = 0;
    int8_t rv = 0;
		uint8_t ii = 0;
		uint8_t TempByte;
    unsigned int i;
		volatile uint16_t tpLength = requestLength;
	
		sprintf( dataOut, "mcc send request\n" );
		//HAL_UART_Transmit( &UartHandle, ( uint8_t * )dataOut, strlen( dataOut ), 500 );	
	
    // 1. write to buffer
		sprintf( dataOut, "  request (%d): ", requestLength );
		//HAL_UART_Transmit( &UartHandle, ( uint8_t * )dataOut, strlen( dataOut ), 500 );	
    for ( i = 0; i < requestLength; i++ )
    {
				sprintf( dataOut, "%x, ", request[i] );		//tbd
				//HAL_UART_Transmit( &UartHandle, ( uint8_t * )dataOut, strlen( dataOut ), 500 );	
        buffer[i] = request[i];
    }
		sprintf( dataOut,  "\n" );
		//HAL_UART_Transmit( &UartHandle, ( uint8_t * )dataOut, strlen( dataOut ), 500 );	
		
    // 2. calculate CRC
    crc = rfalCrcCalculateCcitt( CRC_PRELOAD_VALUE, request, requestLength );
    buffer[requestLength] = LOW_BYTE( crc );
    buffer[requestLength+1] = HIGH_BYTE( crc );

    // 3. calculate parity
    rv = attachParityInformationOld( buffer, requestLength + CRC_LEN );
    EVAL_ERR_NE_GOTO( ERR_NONE, rv, out );

//		mccCryptoSetActive( handle , 0 );
		
    // 4. cypher is used -> encrypt
    if ( mccCryptoIsActive( handle ) != 0 )
    {
        mccCryptoTranscode( handle, buffer, requestLength + CRC_LEN, 0 );
    }

		for( ii = 0; ii < requestLength + CRC_LEN; ii++)
		{
			TempByte = buffer[ii] >> 8;
			buffer[ii] = (buffer[ii] <<8) | (TempByte << 7);
		}
		
		
		
    // 5. transceive data
    rv = mccSendRawRequest( buffer, requestLength + CRC_LEN,
                     response, maxResponseLength, responseLength,
                     timeout, fourBitResponse );
    EVAL_ERR_NE_GOTO( ERR_NONE, rv, out );
		
		delay_ms(300);
		
    // 6. Copy response back
    for ( i = 0; i < *responseLength; i++ )
    {
        buffer[i] = response[i];
    }

    // 7. Encryption enabled ?
    if ( mccCryptoIsActive( handle ) != 0 )
    {
        bytes = *responseLength;
        bits = 0;
        // 7.1 response received? (0 or 1 byte)
        if ( *responseLength == 0 || *responseLength == 1 )
        {
            bytes = 0;
            bits = 4;
						sprintf( dataOut, "D: %d bytes %d bits\n", bytes, bits );
						//HAL_UART_Transmit( &UartHandle, ( uint8_t * )dataOut, strlen( dataOut ), 500 );	
        }

        // 7.2 transcode received data
				sprintf( dataOut, "D: %d bytes %d bits\n", bytes, bits );
				//HAL_UART_Transmit( &UartHandle, ( uint8_t * )dataOut, strlen( dataOut ), 500 );	
        mccCryptoTranscode( handle, buffer, bytes, bits );

    }

    // 7. write output
    for ( i = 0; i < *responseLength; i++ )
    {
        response[i] = buffer[i];
    }

out:
    return rv;
}

/*
 ******************************************************************************
 * LOCAL FUNCTIONS
 ******************************************************************************
 */

static int8_t buildCommand ( uint8_t *cmd, uint8_t keySelect, uint8_t block )
{
    uint16_t crc;

    cmd[0] = keySelect;
    cmd[1] = block;

    crc = rfalCrcCalculateCcitt( CRC_PRELOAD_VALUE, cmd, CRC_LEN );
    cmd[2] = LOW_BYTE( crc );
    cmd[3] = HIGH_BYTE( crc );

    return ERR_NONE;
}

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


static ReturnCode attachParityInformationOld ( uint16_t *cmd, size_t length )
{
    unsigned int i = 0;
    for ( i = 0; i < length; i++ )
    {
        if ( mccParityOdd( cmd[i] ) == 1 )
        {
            cmd[i] |= 0x0100;
        }
        else
        {
            cmd[i] &= ~0x0100;
        }
    }

    return ERR_NONE;
}




ReturnCode MifareAuthentication(uint8_t authentication_key,uint8_t *key,uint8_t block)	
{
		uint8_t uidLength;
		if (err==0)
		{
			
			uidLength = ISO14443A_Card.UIDsize;	
			
			if  (authentication_key == MCC_AUTH_KEY_A)
				{
					err = mccAuthenticateStep1(authentication_key,
							block,
							ISO14443A_Card.UID, //&buf[8],
							uidLength, //buf[7],
							key);
					if (ERR_NONE != err)
					{
							return err;
					}
				}
					err = mccAuthenticateStep2(0x11223344);
				}
		return err;
}

ReturnCode MifareRead(uint8_t block,uint8_t *response)
{
	uint16_t *responseLength;
	uint8_t mifare_request[2];
//	HAL_Delay(100);
	
	/* MiFare read block command. */
//	if (err ==0)
//		{
	mifare_request[0] = MCC_READ_BLOCK;
	mifare_request[1] = block; //BLOCK number
	*responseLength = mifareResponseLen;
			
	err = mccSendRequest(mifare_request,
				sizeof(mifare_request),
				response,
				*responseLength,
				&numBytesReceived,
				MCC_READ_TIMEOUT,
				false);
					
	*responseLength = numBytesReceived;
	return err;
}

		
ReturnCode MifareWrite(uint8_t block, uint8_t *txData)
{
	uint16_t *txSize;
//	uint16_t numBytesReceived;
  uint8_t mifare_request[2];
//  uint8_t mifare_reply;
				
  // MiFare write block command. 
  //Generate MiFare write block request. 
//	mifare_request[0] = MCC_WRITE_BLOCK;
	
	mifare_request[0] = 0xA0;
	mifare_request[1] = block; //BLOCK number;
//	memcpy(&mifare_request[2],txData,16);	

		//Send write block request, enabling mifare 4 bit response. /
	err = mccSendRequest(mifare_request,
						sizeof(mifare_request),
							&mifare_reply,
							sizeof(mifare_reply),
							&numBytesReceived,
              MCC_WRITE_COMMAND_TIMEOUT,
              true);

		//Stop processing write request if an error occured./
	if (ERR_NONE == err)
				{
					// No error occured. Send the data. /
					err = mccSendRequest(txData,//&buf[1],
									16,
									&mifare_reply,
									*txSize,
									&numBytesReceived,
									MCC_WRITE_DATA_TIMEOUT,
									false);

					*txSize = numBytesReceived;
				}			
				
		return err;
/////////////////////////////////////////////////////////////////////////
}		

ReturnCode MifareHalt(uint8_t *response)
{
	uint16_t *responseLength;
	uint8_t mifare_request[2];
	
//	HAL_Delay(100);
//	if (err ==0)
	mifare_request[0] = MCC_HALT;
	mifare_request[1] = 0x00; 
	*responseLength = mifareResponseLen;
			
	err = mccSendRequest(mifare_request,
				sizeof(mifare_request),
				response,
				*responseLength,
				&numBytesReceived,
				MCC_READ_TIMEOUT,
				false);
	
	if (ERR_NONE == err)	
	{	
		mccCryptoReset( handle, 0 );
	}	
	*responseLength = numBytesReceived;
	return err;		
}

ReturnCode MifareTest(void)
{
	uint16_t *responseLength;
	uint8_t response[0xFF];
	ReturnCode err=0;
	//uint8_t	txData[16] = {0x00,0x11,0x22,0x33,0x66,0x66,0x66,0x66,0x66,0x99,0xaa,0xbb,0xcc,0xdd,0xee,0xff};
	uint8_t	txData[16] = {0x00,0x22,0x22,0x22,0x44,0x55,0x66,0x77,0x88,0x99,0xaa,0xbb,0xcc,0xdd,0xee,0xff};
	
	uint8_t key_A[6] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
	uint8_t block=0x01;
	/* Mifaire read block trial.*/
	
  /* Configure demoboard for MiFare. */
  err = mccInitialize();
  //mccResetCipher();
  *responseLength = 0;

	err=MifareAuthentication(MCC_AUTH_KEY_A,key_A,block);	
	
	/* MiFare read block command. */
	if (ERR_NONE == err)	
	{	
		err = MifareRead(block,response);
	}
	memcpy(txData, response, 16);
	txData[1]++;
	if (ERR_NONE == err)	
	{
		err = MifareWrite(block, txData);
	}
	if (ERR_NONE == err)	
	{	
		err = MifareHalt(response);
	}
	return err;
	delay_ms(100);
}
