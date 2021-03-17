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
******************************************************************************
* INCLUDES
******************************************************************************
*/

//#include "delay.h"
//#include "logger.h"
#include "mcc.h"
#include <string.h>
#include <stdbool.h>
#include "utils.h"
#include "drv_95HF.h"
#include "lib_PCD.h"

#ifdef __cplusplus
extern "C" {
#endif
/*
******************************************************************************
* DEFINES
******************************************************************************
*/

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
* LOCAL VARIABLES
******************************************************************************
*/
static uint8_t mccRawBuffer[0xFF];
static uint8_t mccRawBuffer95HF[0xFF];
uint8_t mccRvBuffer95HF[0xFF];

/*
******************************************************************************
* LOCAL FUNCTION PROTOTYPES
******************************************************************************
*/
static uint16_t mccCopyToRawBuffer ( uint16_t *message, uint16_t length );
static uint16_t mccExtractMessage ( uint8_t* response, uint16_t responseLength );

/*
******************************************************************************
* GLOBAL FUNCTIONS
******************************************************************************
*/
ReturnCode mccSendRawRequest ( uint16_t *request,
                       uint16_t requestLength,
                       uint8_t *response,
                       uint16_t maxResponseLength,
                       uint16_t *responseLength,
                       uint16_t timeout,
                       uint8_t fourBitResponse )
{
    ReturnCode err = ERR_NONE;
 //   uint8_t lsbs_in_last_byte;
    //uint16_t len_bits;
		//uint16_t fdt;
		//uint16_t DummyL = 0;
		uint8_t i = 0;
    /* Setup receive operation. */
    //err = st25r3911PrepareReceive(TRUE);
		//st25r3911PrepareReceive(true);
    //EVAL_ERR_NE_GOTO(ERR_NONE, err, out);

    /* Mask data reception. */
    //err = st25r3911ExecuteCommand(ST25R3911_CMD_MASK_RECEIVE_DATA);
		//st25r3911ExecuteCommand(ST25R3911_CMD_MASK_RECEIVE_DATA);
    //EVAL_ERR_NE_GOTO(ERR_NONE, err, out);

    mccCopyToRawBuffer(request, requestLength);
		
		for (i = 0; i < requestLength*2; i++)
		{
			mccRawBuffer95HF[i+2] = mccRawBuffer[i];
		}
		mccRawBuffer95HF[PCD_COMMAND_OFFSET ] = SEND_RECEIVE;
		mccRawBuffer95HF[PCD_LENGTH_OFFSET  ] = requestLength *2 + 1;
		mccRawBuffer95HF[requestLength*2 + 2] = 0x18;
		//fdt=rfalGetFDTListen( );

		/* DataToSend CodeCmd Length Data*/
		/* Parameters[0] first byte to emmit	*/
		memcpy(&(mccRawBuffer95HF[PCD_DATA_OFFSET]),mccRawBuffer,requestLength);
		
		memset(mccRvBuffer95HF, 0, 0xFF);
		/* Send the command the Rf transceiver	*/
		drv95HF_SendReceive(mccRawBuffer95HF, mccRvBuffer95HF);
		if(mccRvBuffer95HF[1]== 0x01)
		{
			err = PCD_SUCCESSCODE;
			*responseLength = 0x01;
			return err;
		}
		else if (PCD_IsReaderResultCodeOk (SEND_RECEIVE,mccRvBuffer95HF) != PCD_SUCCESSCODE)
			{
				if(*mccRvBuffer95HF == PCD_ERRORCODE_NOTAGFOUND)
					err = PCD_ERRORCODE_TIMEOUT;
				else
					err = PCD_ERRORCODE_DEFAULT;
			}
			err = PCD_SUCCESSCODE;
		
  	//err = drv95HF_SendReceive(mccRawBuffer95HF, response);//ST25R3911_FDT_NONE	);
    //EVAL_ERR_NE_GOTO(ERR_NONE, err, out);

    /* Receive response. */
//		timeout = st25r3911SetNoResponseTime_64fcs(timeout*1000/4.72);
//    err = st25r3911RxNBytes(mccRawBuffer, sizeof(mccRawBuffer), responseLength, timeout);
//    EVAL_ERR_NE_GOTO(ERR_NONE, err, out);

//    if (((*responseLength * 8) / 9) > maxResponseLength)
//    {
#ifdef mcc_DBG
//			sprintf( dataOut, "limiting l=%hx ,ml=%hx\n",*responseLength,maxResponseLength );
//			HAL_UART_Transmit( &UartHandle, ( uint8_t * )dataOut, strlen( dataOut ), 500 );	
#endif
//        *responseLength = maxResponseLength / 8 * 9;
//        err = ERR_NONE; /* This will but an existing CRC */
//    }
			*responseLength = mccExtractMessage(response, *responseLength);

//out:
    return err;
}

/*
******************************************************************************
* LOCAL FUNCTIONS
******************************************************************************
*/
static uint16_t mccCopyToRawBuffer ( uint16_t *message, uint16_t length )
{
    int i; //bytepos = 0;
    //int bitpos = 0;
    memset(mccRawBuffer,0,sizeof(mccRawBuffer));
#ifdef mcc_DBG
		sprintf( dataOut, "\r\ntransmitting: \r\n");
		HAL_UART_Transmit( &UartHandle, ( uint8_t * )dataOut, strlen( dataOut ), 500);

	
    for(i = 0; i<length; i++)
    {			
				sprintf( dataOut, "%hx,",message[i]);
				HAL_UART_Transmit( &UartHandle, ( uint8_t * )dataOut, strlen( dataOut ), 500);
    }
		sprintf( dataOut, "\n");
		HAL_UART_Transmit( &UartHandle, ( uint8_t * )dataOut, strlen( dataOut ), 500);
#endif

//    for (i = 0; i < length; i++)
//    {
//        uint16_t m = message[i];
//        mccRawBuffer[bytepos] |= (m & ((1<<(8 - bitpos))-1)) << bitpos;
//        bytepos++;
//        mccRawBuffer[bytepos] |= (m >> (8-bitpos));

//        bitpos += 1;
//        if (bitpos >=8 )
//        {
//            bitpos -= 8;
//            bytepos++;
//        }
//    }
		
    for (i = 0; i < length; i++)
    {
        uint16_t m = message[i];
				mccRawBuffer[i*2+1] = m;
        mccRawBuffer[i*2] = m >> 8;
		}
#ifdef mcc_DBG
		sprintf( dataOut, "  raw: ");
		HAL_UART_Transmit( &UartHandle, ( uint8_t * )dataOut, strlen( dataOut ), 500);
    for ( i= 0; i< (length*2);i++)
    {
			sprintf( dataOut, "%hhx,",mccRawBuffer[i]);
			HAL_UART_Transmit( &UartHandle, ( uint8_t * )dataOut, strlen( dataOut ), 500);
    }
		sprintf( dataOut, "\n");
		HAL_UART_Transmit( &UartHandle, ( uint8_t * )dataOut, strlen( dataOut ), 500);
#endif
    return length;
}

static uint16_t mccExtractMessage ( uint8_t* response, uint16_t responseLength )
{
    int bytes;
    int i =0;
	
		responseLength = mccRvBuffer95HF[1];
		
		if (responseLength==0)
    {
        response[0] = mccRawBuffer[0];
        return 1;
    }
		
#ifdef mcc_DBG
		sprintf( dataOut, "extracting (%d bytes)", responseLength);
		HAL_UART_Transmit( &UartHandle, ( uint8_t * )dataOut, strlen( dataOut ), 500);
    MCC_DUMP(mccRvBuffer95HF,responseLength);
#endif
		
		bytes = (responseLength - 3) / 2;

		
	
    for (i = 0; i < bytes; i++)
    {
      response[i] = mccRvBuffer95HF[i*2 +2];
    }
		
#ifdef mcc_DBG
		sprintf( dataOut, " extracted: (%d bytes)", bytes);
		HAL_UART_Transmit( &UartHandle, ( uint8_t * )dataOut, strlen( dataOut ), 500);
    MCC_DUMP(mccRvBuffer95HF,bytes);
#endif
		
    return bytes;
}

#ifdef __cplusplus
}
#endif
