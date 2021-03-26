/**
  ******************************************************************************
  * @file    drv_spi.c 
  * @author  MMY Application Team
  * @version $Revision: 1508 $
  * @date    $Date: 2016-01-08 09:50:04 +0100 (Fri, 08 Jan 2016) $
  * @brief   This file provides a set of firmware functions to manages SPI communications
  ******************************************************************************
  * @copyright
  *
  * <h2><center>&copy; COPYRIGHT 2016 STMicroelectronics</center></h2>
  *
  * Licensed under MMY-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
	******************************************************************************
*/ 

/* Includes ------------------------------------------------------------------*/
#include "drv_spi.h"

#ifdef STM32F401xE
#include "stm32f4xx_nucleo.h" 
#endif

#ifdef STM32F103xB
#include "stm32f1xx_nucleo.h" 
#endif


/** @addtogroup BSP
 * @{
 */

/** @addtogroup X_NUCLEO_NFC03A1_Spi
 * @{
 */
/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
/** @defgroup X_NUCLEO_NFC03A1_Spi_External_Variables
 * @{
 */
 /* SPI Handle declaration */
extern SPI_HandleTypeDef hspi;
/**
 * @}
 */ 

/* Global variables ----------------------------------------------------------*/
 /* Public Functions ----------------------------------------------------------*/
/** @defgroup X_NUCLEO_NFC03A1_Spi_Public_Functions
 * @{
 */
/**  
 *	@brief  Initialise HAL SPI for NFC03A1
 *  @param  None
 *  @retval None
 */
void RFTRANS_SPI_Init(void) 
{	
  
  if(HAL_SPI_GetState(&hspi) == HAL_SPI_STATE_RESET)
  {
    /* SPI Config */
    hspi.Instance	= RFTRANS_95HF_SPI;
    /* SPI baudrate is set to 12,5 MHz maximum (PCLK2/SPI_BaudRatePrescaler = 100/8 = 12,5 MHz) 
    to verify these constraints:
    - ST7735 LCD SPI interface max baudrate is 15MHz for write and 6.66MHz for read
    Since the provided driver doesn't use read capability from LCD, only constraint 
    on write baudrate is considered.
    - SD card SPI interface max baudrate is 25MHz for write/read
    - PCLK2 max frequency is 100 MHz 
    */ 
    hspi.Init.Mode 		        = SPI_MODE_MASTER;
    hspi.Init.BaudRatePrescaler         = SPI_BAUDRATEPRESCALER_64;
    hspi.Init.NSS 			= SPI_NSS_SOFT;
    hspi.Init.CLKPolarity		= SPI_POLARITY_HIGH;
    hspi.Init.CLKPhase			= SPI_PHASE_2EDGE;
    hspi.Init.Direction 		= SPI_DIRECTION_2LINES;
    hspi.Init.DataSize 			= SPI_DATASIZE_8BIT;
    hspi.Init.FirstBit 			= SPI_FIRSTBIT_MSB;
    hspi.Init.TIMode 			= SPI_TIMODE_DISABLE;
    hspi.Init.CRCCalculation 		= SPI_CRCCALCULATION_DISABLE;
    /* Configure SPI Pin */
    RFTRANS_SPI_MSPInit();
    HAL_SPI_Init(&hspi);
  }
}

/**  
 *	@brief  Configure IO for NFC03A1 SPI
 *  @param  None
 *  @retval None
 */
void RFTRANS_SPI_MSPInit(void) 
{	
  GPIO_InitTypeDef GPIO_InitStruct;
  
  if(hspi.Instance == RFTRANS_95HF_SPI)
  {
    /* Enable GPIOC clock */
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    
    
    
    /**SPI1 GPIO Configuration    
    PA5     ------> SPI1_SCK
    PA6     ------> SPI1_MISO
    PA7     ------> SPI1_MOSI 
    PB6			------> SPI1_CS 
    */
    /* Configure SPI SCK */
    GPIO_InitStruct.Pin 		= RFTRANS_95HF_SPI_SCK_PIN;
    GPIO_InitStruct.Mode 		= GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull 		= GPIO_PULLUP;
    GPIO_InitStruct.Speed 		= GPIO_SPEED_HIGH;
#ifndef USE_STM32F1xx_NUCLEO
    GPIO_InitStruct.Alternate           = NUCLEO_SPIx_SCK_AF;
#endif
    HAL_GPIO_Init(RFTRANS_95HF_SPI_SCK_GPIO_PORT, &GPIO_InitStruct);
    
    /* Configure SPI MISO and MOSI */ 
    GPIO_InitStruct.Pin 		= RFTRANS_95HF_SPI_MOSI_PIN;
    GPIO_InitStruct.Mode 		= GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull 		= GPIO_PULLDOWN;
#ifndef USE_STM32F1xx_NUCLEO
    GPIO_InitStruct.Alternate           = NUCLEO_SPIx_SCK_AF;
#endif
    HAL_GPIO_Init(RFTRANS_95HF_SPI_MOSI_GPIO_PORT, &GPIO_InitStruct);
    
    GPIO_InitStruct.Pin 		= RFTRANS_95HF_SPI_MISO_PIN;
    GPIO_InitStruct.Pull 		= GPIO_PULLUP;
    HAL_GPIO_Init(RFTRANS_95HF_SPI_MISO_GPIO_PORT, &GPIO_InitStruct);
    
    /* Peripheral clock enable */
    __HAL_RCC_SPI1_CLK_ENABLE();
    
    __HAL_RCC_SPI1_FORCE_RESET();
    __HAL_RCC_SPI1_RELEASE_RESET();
    
    /* SPINSS as output push pull */
    GPIO_InitStruct.Pin 		= RFTRANS_95HF_SPI_NSS_PIN;
    GPIO_InitStruct.Mode 		= GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull 		= GPIO_NOPULL;
    GPIO_InitStruct.Speed 		= GPIO_SPEED_HIGH;
    HAL_GPIO_Init(RFTRANS_95HF_SPI_NSS_GPIO_PORT, &GPIO_InitStruct);
    
    /* SPI_NSS  = High Level  */
    RFTRANS_95HF_NSS_HIGH();
    
    /* Configure Mcu IRQ_IN GPIO pin */
    GPIO_InitStruct.Pin 	        = IRQOUT_RFTRANS_95HF_PIN;
    GPIO_InitStruct.Mode 	        = GPIO_MODE_IT_FALLING;
    GPIO_InitStruct.Pull 	        = GPIO_PULLUP;
    GPIO_InitStruct.Speed               = GPIO_SPEED_LOW;
    HAL_GPIO_Init(IRQOUT_RFTRANS_95HF_PORT, &GPIO_InitStruct);
    
    /* Configure Mcu IRQ_OUT pin as open drain output */
    GPIO_InitStruct.Pin 	        = IRQIN_RFTRANS_95HF_PIN;
    GPIO_InitStruct.Mode 	        = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed               = GPIO_SPEED_HIGH;
    HAL_GPIO_Init(IRQIN_RFTRANS_GPIO_PORT, &GPIO_InitStruct);
    
    /* Set signal to high */
    RFTRANS_95HF_IRQIN_HIGH();
  }
}

/**  
 *	@brief  Configure interface pin to select serial interface
 *  @param  None
 *  @retval None
 */
void RFTRANS_ConfigureInterfacePin(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  
  __HAL_RCC_GPIOC_CLK_ENABLE();
  
  /* SPINSS as output push pull */
  GPIO_InitStruct.Pin 			= INTERFACE_PIN;
  GPIO_InitStruct.Mode 			= GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull 			= GPIO_NOPULL;
  GPIO_InitStruct.Speed 		= GPIO_SPEED_LOW;
  HAL_GPIO_Init(INTERFACE_GPIO_PORT, &GPIO_InitStruct);
}

/**  
 *	@brief  Sends one byte over SPI and recovers a response
 *  @param  data : data to send
 *  @retval data response from SPIx 
 */
uint8_t SPI_SendReceiveByte(uint8_t data) 
{	
  uint8_t ret_value;	
  
  HAL_SPI_TransmitReceive(&hspi, (uint8_t*) &data, &ret_value, 1, SPIx_TIMEOUT_MAX);
  
  return ret_value;
}

/**
 *	@brief  reveive a byte array over SPI
 *  @param  pCommand  : pointer on the buffer to send
 *  @param  length	 	: length of the buffer to send
 *  @param  pResponse : pointer on the buffer response
 *  @retval None 
 */
void SPI_SendReceiveBuffer(const uint8_t *pCommand, uint8_t length, uint8_t *pResponse) 
{
  uint8_t i;
  
  for(i=0; i<length; i++)
    pResponse[i] = SPI_SendReceiveByte(pCommand[i]);

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

/******************* (C) COPYRIGHT 2016 STMicroelectronics *****END OF FILE****/
