/******************** (C) COPYRIGHT 2014 STMicroelectronics ********************
* File Name          : flash_cmd.h
* Author             : MMY Application Team
* Version            : V1.0.0
* Date               : 12-June-2012
* Description        : Middle layer flash command
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/
#include "stm32f10x.h"
#include "stm32f10x_flash.h"

typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;


#if defined (STM32F10X_HD) || defined (STM32F10X_HD_VL) || defined (STM32F10X_CL) || defined (STM32F10X_XL)
  #define FLASH_PAGE_SIZE    ((uint16_t)0x800)
#else
  #define FLASH_PAGE_SIZE    ((uint16_t)0x400)
#endif

#ifdef STM32F10X_HD
#define BANK1_WRITE_START_ADDR  ((uint32_t)0x08060000)
#define BANK1_WRITE_END_ADDR    ((uint32_t)0x08070000)

#else
#define BANK1_WRITE_START_ADDR  ((uint32_t)0x08010000)
#define BANK1_WRITE_END_ADDR    ((uint32_t)0x08020000)
#endif /*STM32F10X_HD*/



#ifdef STM32F10X_XL
 #define BANK2_WRITE_START_ADDR   ((uint32_t)0x08088000)
 #define BANK2_WRITE_END_ADDR     ((uint32_t)0x0808C000)
#endif /* STM32F10X_XL */


/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/  

void Flash_Init(void);
void Flash_Write_Memory(uint32_t Offset, uint32_t *Data, uint32_t Transfer_Length);
void Flash_Read_Memory(uint32_t Offset, uint32_t *Read, uint32_t Transfer_Length);

/******************* (C) COPYRIGHT 2014 STMicroelectronics *****END OF FILE****/
