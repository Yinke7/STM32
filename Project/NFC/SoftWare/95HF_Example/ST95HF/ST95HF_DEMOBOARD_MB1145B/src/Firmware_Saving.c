/**
  ******************************************************************************
  * @file    Firmware_Saving.c 
  * @author  MMY Application Team
  * @version V1.0
  * @date    08/13/2013
  * @brief   This file contain the fonction that manage data transfer from RAM 
	* @brief   to ROM. The data saved, were received in a firmeware upgrade context.
  ******************************************************************************
  * @copyright
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  */

  
/* Includes ------------------------------------------------------------------*/
#include "common.h"
#include "stm32f10x_crc.h"
#include "stm32f10x_flash.h"
#include "lib_cardemul.h"

/** @addtogroup User_Appli
 * 	@{
 *  @brief      <b>This folder contains the application files</b> 
 */
 
/** @addtogroup Firmware_Saving
 * 	@{
 * @brief      This file contain the function used to save data in internal flash of MCU 
 */
 
extern uint16_t 				FileSize;
extern uint32_t					NbByteReceived ;
extern uint8_t 					Firmware_Buffer[FIRMEWARE_SIZE];

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

uint32_t FlashDestination = ApplicationAddress; /* Flash user program offset */
uint16_t PageSize = PAGE_SIZE;
uint32_t EraseCounter = 0x0;
uint32_t NbrOfPage = 0;
FLASH_Status FLASHstatus = FLASH_COMPLETE;
uint32_t RamSource;

/* Private function prototypes -----------------------------------------------*/

/** @addtogroup Firmware_Saving_Private_Functions
 * 	@{
 *  @brief	No Private Function
 */

/**
  * @}
  */

/** @addtogroup Firmware_Saving_Public_Functions
 * 	@{
 */

/**
  * @brief  Save data in internal flash of MCU
  * @param  None
  * @retval The size of the file
  */
int32_t Transfert_FW (void)
{	
  int32_t  j, size = 0;
	uint32_t 	data=0;
	uint32_t *pdata;

	/* Initialize FlashDestination variable */
  FlashDestination = ApplicationAddress;
		
	size = FileSize;
	/* Test the size of the image to be sent */
	/* Image size is greater than Flash size */
	if (size > (FLASH_SIZE - 1))
	{
		return -1;
	}

	/* Erase the needed pages where the user application will be loaded */
	/* Define the number of page to be erased */
	NbrOfPage = FLASH_PagesMask(size);

	/* Erase the FLASH pages */
	for (EraseCounter = 0; (EraseCounter < NbrOfPage) && (FLASHstatus == FLASH_COMPLETE); EraseCounter++)
	{
		FLASHstatus = FLASH_ErasePage(FlashDestination + (PageSize * EraseCounter));
	}
	
	pdata = (uint32_t*)(&Firmware_Buffer[0]);
	for(j=0;j<size;j+=4)
	{
		data = *pdata;
		FLASH_ProgramWord(FlashDestination,data);
		pdata++;
		FlashDestination += 4;
	}              
     
  return (int32_t)size;
}

/**
  * @brief  Calculate the CRC of the file in RAM
  * @param  fileName : Address of the first byte
  * @retval The file's CRC
  */
uint32_t file_CRC(const char * fileName)
{
	uint32_t 	crc=0,
						data=0,
						jj=0;
	uint32_t *pdata;

	CRC_ResetDR();
	pdata = (uint32_t*)(&Firmware_Buffer[0]);
	for(jj=0;jj<NbByteReceived;jj+=4)
	{
		data = *pdata;
		crc=CRC_CalcCRC(data);
		pdata++;
	}
	
	return crc;
	
}

/**
  * @brief  Calculate the CRC of the file in Flash
  * @param  FwSize : Size of the firmware
  * @retval The file's CRC
  */
uint32_t Flash_CRC(uint32_t FwSize)
{

	uint32_t 	crc=0;

	
	CRC_ResetDR();					
	crc=CRC_CalcBlockCRC((uint32_t *)ApplicationAddress, FwSize);

	
	return crc;
	
}

/**
  * @}
  */


/**
  * @}
  */

/**
  * @}
  */

/*******************(C)COPYRIGHT 2014 STMicroelectronics *****END OF FILE****/
