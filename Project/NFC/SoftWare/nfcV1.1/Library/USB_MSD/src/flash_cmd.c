/**
******************* (C) COPYRIGHT 2014 STMicroelectronics ********************
* @file          : flash_cmd.c
* @author        : MMY Application Team
* @version       : V1.0.0
* @date          : 12-June-2012
* @brief         : Middle layer flash command
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/

#include <string.h>
#include "Flash_Cmd.h"



uint32_t WRITE[FLASH_PAGE_SIZE/4];		

volatile FLASH_Status FLASHStatus = FLASH_COMPLETE;
volatile TestStatus MemoryProgramStatus = PASSED;


/** @addtogroup flash
 * 	@{
 */

 /** @addtogroup flash_function
 * 	@{
 */


/**  
 * @brief  Initialize the flash memory
 * @retval None
 */
void Flash_Init(void)
{ 
  /* Unlock the Flash Bank1 Program Erase controller */
  FLASH_Unlock();

}


/**  
 * @brief  Read data in flash memory
 * @param  Offset : offset memory.
 * @param  *Read : point to the data buffer.
 * @param  Transfer_Length : number of byte to read.
 * @retval None
 */
void Flash_Read_Memory(uint32_t Offset, uint32_t *Read, uint32_t Transfer_Length)
{
  uint32_t Physical_Address = BANK1_WRITE_START_ADDR + Offset;
  uint32_t *pRead = Read;
  uint32_t NthDataToRead=0;

  /*Read word in flash memory*/
  while(NthDataToRead<(Transfer_Length/4))
  {

    *pRead= *((uint32_t *)Physical_Address);

   *pRead++;
   Physical_Address += 4;
   NthDataToRead++;

  }

}

/**  
 * @brief  Write data in flash memory
 * @param  Offset : offset memory.
 * @param  *Data: point to the data buffer.
 * @param  Transfer_Length : number of byte to write.
 * @retval None
 */
void Flash_Write_Memory(uint32_t Offset, uint32_t *Data, uint32_t Transfer_Length)
{   
  uint32_t Physical_Address = BANK1_WRITE_START_ADDR;	
  uint32_t NthDataToWrite=0;
  uint32_t Page_To_Write=0;

  //uint32_t *pWRITE = WRITE;

  /*Find the page to write*/
  Page_To_Write = Offset/FLASH_PAGE_SIZE;

  Physical_Address += Page_To_Write*FLASH_PAGE_SIZE;

   /*Pre-copy Data to write*/
   memcpy(WRITE, Data, FLASH_PAGE_SIZE);

   
  	NthDataToWrite=0;

  /*Erase all page before write new data*/
  FLASHStatus = FLASH_ErasePage(BANK1_WRITE_START_ADDR + (FLASH_PAGE_SIZE * Page_To_Write));
  /*wait erase process complet*/
  while(FLASHStatus == FLASH_BUSY);
  

   /*Write word in flash memory*/
	while(NthDataToWrite < Transfer_Length/4)
  	{
    	FLASHStatus = FLASH_ProgramWord(Physical_Address , WRITE[NthDataToWrite]);
    	Physical_Address +=4;
		NthDataToWrite++;
  	}
	
}

/**
 * @}
 */ 

/**
*  @}
*/ 

/******************* (C) COPYRIGHT 2014 STMicroelectronics *****END OF FILE****/

