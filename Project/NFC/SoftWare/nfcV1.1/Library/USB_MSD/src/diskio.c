/**
******************* (C) COPYRIGHT 2014 STMicroelectronics ********************
* @file Name          : flash_cmd.c
* @author             : MMY Application Team
* @version            : V1.0.0
* @date               : 12-June-2012
* @description        : Low level disk I/O module
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
******************************************************************************
*/

#if defined (USE_MSD_DRIVE) || defined (USE_RX95HFDEMO_BOARD) || defined (USE_STRFNFCADEMO_BOARD)
#include <string.h>
#include "diskio.h"
#include "mass_mal.h"
#include "Flash_Cmd.h"

volatile FLASH_Status FLASHStatut = FLASH_COMPLETE;
/*-----------------------------------------------------------------------*/
/* Correspondence between physical drive number and physical drive.      */
/* Note that Tiny-FatFs supports only single drive and always            */
/* accesses drive number 0.                                              */
/*-----------------------------------------------------------------------*/


/** @addtogroup FATFS_function
 * 	@{
 */

 /** @addtogroup disk_I/O_function
 * 	@{
 */

/**  
 * @brief  Initialize the disk (used by FATFS)
 * @param  drv : Physical drive nmuber (0..)
 * @retval DSTATUS
 */
DSTATUS disk_initialize (BYTE drv)/* Physical drive nmuber (0..)*/
{
    return 0;  /*Initialize OK*/
}

/**  
 * @brief  Return status of the drive (used by FATFS)
 * @param  drv : Physical drive nmuber (0..)
 * @retval DSTATUS
 */                                                  
DSTATUS disk_status (BYTE drv) /* Physical drive nmuber (0..)*/
{ 
    return 0;  /*Status OK*/
}

/**  
 * @brief  Read data in flash memory (used by FATFS)
 * @param  drv : Physical drive nmuber (0..)
 * @param  *buff : Data buffer to store read data.
 * @param  sector :Sector address (LBA).
 * @param  count : Number of sectors to read (1..255).
 * @retval DRESULT
 */
DRESULT disk_read (
                   BYTE drv,            /* Physical drive nmuber (0..) */
                   BYTE *buff,          /* Data buffer to store read data */
                   DWORD sector,        /* Sector address (LBA) */
                   BYTE count           /* Number of sectors to read (1..255) */
                       )
{

uint32_t Flash_Adress=0;
uint32_t Word_To_Char=0;
uint32_t Flash_Sector=0;
uint32_t Word_Read[FLASH_PAGE_SIZE/4];

//uint32_t *pWord_Read = Word_Read;

	  
	  if(count == 1)
	  {
	  	/*Select the current sector to read*/
		Flash_Sector = sector*FLASH_PAGE_SIZE;

		/*Read data in flash memory*/
	  	Flash_Read_Memory(Flash_Sector, Word_Read, FLASH_PAGE_SIZE);

			/*Convert data for buff word to char*/
			for(Flash_Adress=0 ; Flash_Adress<FLASH_PAGE_SIZE/4 ; Flash_Adress++)
			{
			buff[0+Word_To_Char] = Word_Read[Flash_Adress]  &  0x000000FF;
	  	buff[1+Word_To_Char] = (Word_Read[Flash_Adress] &  0x0000FF00) >> 8 ;
	  	buff[2+Word_To_Char] = (Word_Read[Flash_Adress] &  0x00FF0000) >> 16 ;
	  	buff[3+Word_To_Char] = (Word_Read[Flash_Adress] &  0xFF000000) >> 24 ;
			Word_To_Char+=4;
			}
		return RES_OK;
		}
	 return RES_ERROR;		
        
}

/**  
 * @brief  Write data in flash memory (used by FATFS)
 * @param  drv : Physical drive nmuber (0..)
 * @param  *buff : Data buffer to store read data.
 * @param  sector :Sector address (LBA).
 * @param  count : Number of sectors to read (1..255).
 * @retval DRESULT
 */
#if _READONLY == 0
DRESULT disk_write (
                    BYTE drv,                   /* Physical drive nmuber (0..) */
                    const BYTE *buff,   		/* Data to be written */
                    DWORD sector,               /* Sector address (LBA) */
                    BYTE count                  /* Number of sectors to write (1..255) */
                        )
{
    
uint32_t Flash_Adress=0;
uint32_t Char_To_Word=0;
uint32_t Flash_Sector=0;
uint32_t Word_Write[FLASH_PAGE_SIZE/4];
//uint32_t *pWord_Write = Word_Write;	

		FLASH_Unlock(); /*Enable Erase and Write process*/
 
		  if(count == 1)
	  {
			/*Convert data from buff Char to Word*/ 
			for(Flash_Adress=0 ; Flash_Adress<FLASH_PAGE_SIZE/4 ; Flash_Adress++)
			{
			Word_Write[Flash_Adress] = buff[0+Char_To_Word] & 0x000000FF 
									 |(buff[1+Char_To_Word]  << 8) & 0x0000FFFF 
									 |(buff[2+Char_To_Word]  << 16) & 0x00FFFFFF 
									 |(buff[3+Char_To_Word]  << 24) & 0xFFFFFFFF;
			Char_To_Word+=4;
			}

		/*Select the current sector to write*/
		Flash_Sector = sector*FLASH_PAGE_SIZE;

		/*Write data in flash memory*/
	  	Flash_Write_Memory(Flash_Sector, Word_Write, FLASH_PAGE_SIZE);

		return RES_OK;
	   }

	   return RES_ERROR;	 		
        

}
#endif /* _READONLY */

/**  
 * @brief  Miscellaneous functions (used by FATFS)
 * @param  drv : Physical drive nmuber (0..)
 * @param  ctrl: Control code.
 * @param  *buff : Buffer to send/receive control data.
 * @retval DRESULT.
 */
DRESULT disk_ioctl (
                    BYTE drv,           /* Physical drive nmuber (0..) */
                    BYTE ctrl,          /* Control code */
                    void *buff          /* Buffer to send/receive control data */
                        )
{

    DRESULT res;

    if (drv)
    {
        return RES_PARERR;  
    }

    switch(ctrl)
    {
    case CTRL_SYNC:
			while(FLASHStatut == FLASH_BUSY);

            res = RES_OK;


        break;
    case GET_SECTOR_SIZE:
        *(WORD*)buff = FLASH_PAGE_SIZE;
        res = RES_OK;
        break;


    case GET_SECTOR_COUNT:
		 *(DWORD*)buff = (BANK1_WRITE_END_ADDR-BANK1_WRITE_START_ADDR)/FLASH_PAGE_SIZE;
        res = RES_OK;
        break;


    case GET_BLOCK_SIZE:
		*(WORD*)buff =1;
        res = RES_OK;
        break;


    default:
        res = RES_PARERR;
        break;
    }
    return res;//RES_OK;//
}


/**  
 * @brief  Displays date and time for the files (used by FATFS)
 * @param  drv : Physical drive nmuber (0..)
 * @retval .
 */
DWORD get_fattime(void)
{

	return ((2012UL-1980) << 25)       // Year = 2012
| (1UL << 21)       // Month = Jan
| (1UL << 16)       // Day = 1
| (10U << 11)       // Hour = 10
| (17U << 5)       // Min = 17
| (0U >> 1)       // Sec = 0
;
}

#endif /*USE_MSD_DRIVE*/

/**
 * @}
 */ 

/**
*  @}
*/ 


/******************* (C) COPYRIGHT 2014 STMicroelectronics *****END OF FILE****/
