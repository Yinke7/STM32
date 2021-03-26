/**
  ******************************************************************************
  * @file    drv_uart.c
  * @author  MMY Application Team
  * @version $Revision: 1508 $
  * @date    $Date: 2016-01-08 09:50:04 +0100 (Fri, 08 Jan 2016) $
  * @brief   This file provides  a set of firmware functions to manages UART communications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2016 STMicroelectronics</center></h2>
  *
  * Licensed under ST MYLIBERTY SOFTWARE LICENSE AGREEMENT (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/myliberty  
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied,
  * AND SPECIFICALLY DISCLAIMING THE IMPLIED WARRANTIES OF MERCHANTABILITY,
  * FITNESS FOR A PARTICULAR PURPOSE, AND NON-INFRINGEMENT.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "drv_uart.h"


/** @addtogroup BSP
 * @{
 */

/** @addtogroup X_NUCLEO_NFC03A1_Uart
 * @{
 */
/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
/** @defgroup X_NUCLEO_NFC03A1_Uart_External_Variables
 * @{
 */
/* UART Handle declaration */
extern UART_HandleTypeDef       huart;
extern __IO uint8_t		uTimeOut;
/**
 * @}
 */ 

/* Global variables ----------------------------------------------------------*/
/* Public Functions ----------------------------------------------------------*/
/** @defgroup X_NUCLEO_NFC03A1_uart_Public_Functions
 * @{
 */
/**  
 *	@brief  Configure UART on NFC03A1 board
 *  @param  BaudRate : value of the Baudrate to use during UART communication
 *  @retval None 
 */
void RFTRANS_UART_Init(const uint32_t BaudRate) 
{	
  
  if(HAL_UART_GetState(&huart) == HAL_UART_STATE_RESET)
  {
    huart.Instance 			= USART1;
    huart.Init.BaudRate 		= BaudRate; 
    huart.Init.WordLength 	        = UART_WORDLENGTH_8B; 
    huart.Init.StopBits 		= UART_STOPBITS_1;
    huart.Init.Parity 		        = UART_PARITY_NONE;
    huart.Init.Mode 		        = UART_MODE_TX_RX;
    huart.Init.HwFlowCtl 		= UART_HWCONTROL_NONE;
    huart.Init.OverSampling             = UART_OVERSAMPLING_16;

    RFTRANS_UART_MSPInit(); 
    HAL_UART_Init(&huart);
  }
}

/**  
 *	@brief  Configure UART IO for NFC03A1 board
 *  @param  None
 *  @retval None
 */
void RFTRANS_UART_MSPInit(void) 
{	
  GPIO_InitTypeDef GPIO_InitStruct;
  /* Enable GPIOC clock */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  
   /* Peripheral clock enable */
  __HAL_RCC_USART1_CLK_ENABLE();
  
  /* USART1 GPIO Configuration */
  
  GPIO_InitStruct.Pin 			= RFTRANS_95HF_UART_TX_PIN ;
  GPIO_InitStruct.Mode 			= GPIO_MODE_AF_PP;
  GPIO_InitStruct.Speed 		= GPIO_SPEED_LOW;
  
#ifdef STM32F401xE      
  GPIO_InitStruct.Alternate             = GPIO_AF7_USART1;
  GPIO_InitStruct.Pull 			= GPIO_NOPULL;
#endif

#ifdef STM32F103xB        
  GPIO_InitStruct.Pull      = GPIO_PULLUP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
#endif 
  
  HAL_GPIO_Init(RFTRANS_95HF_UART_TX_GPIO_PORT, &GPIO_InitStruct);
  
  /* UART RX GPIO pin configuration  */ 
  GPIO_InitStruct.Pin = RFTRANS_95HF_UART_RX_PIN;
  
#ifdef STM32F401xE      
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
#endif                  
  
#ifdef STM32F103xB      
  GPIO_InitStruct.Mode      = GPIO_MODE_INPUT;
#endif 
  
  HAL_GPIO_Init(RFTRANS_95HF_UART_RX_GPIO_PORT, &GPIO_InitStruct); 

  
  __HAL_RCC_USART1_FORCE_RESET();  
  __HAL_RCC_USART1_RELEASE_RESET();
  
  /* NVIC for USART1 */
  HAL_NVIC_SetPriority(USART1_IRQn, 0, 3);
  HAL_NVIC_EnableIRQ(USART1_IRQn);
  
  
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /* Configure unused pin to not interfer with uart com */
  /* Configure SPI SCK */
  GPIO_InitStruct.Pin 			= RFTRANS_95HF_SPI_SCK_PIN;
  GPIO_InitStruct.Mode 			= GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull 			= GPIO_PULLDOWN;
  GPIO_InitStruct.Speed 		= GPIO_SPEED_LOW;
  HAL_GPIO_Init(RFTRANS_95HF_SPI_SCK_GPIO_PORT, &GPIO_InitStruct);

   /* Configure SPI MISO and MOSI */ 
  GPIO_InitStruct.Pin 			= RFTRANS_95HF_SPI_MOSI_PIN;
  HAL_GPIO_Init(RFTRANS_95HF_SPI_MOSI_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Pin 			= RFTRANS_95HF_SPI_MISO_PIN;
  GPIO_InitStruct.Mode 			= GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull 			= GPIO_PULLDOWN;
  HAL_GPIO_Init(RFTRANS_95HF_SPI_MISO_GPIO_PORT, &GPIO_InitStruct);
  
  /* SPINSS as output push pull */
  GPIO_InitStruct.Pin 			= RFTRANS_95HF_SPI_NSS_PIN;
  GPIO_InitStruct.Mode 			= GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull 			= GPIO_PULLUP;
  HAL_GPIO_Init(RFTRANS_95HF_SPI_NSS_GPIO_PORT, &GPIO_InitStruct);

  /* SPI_NSS  = High Level  */
  RFTRANS_95HF_NSS_HIGH();
}

/**
  * @brief  Send one byte over UART
  * @param  data 	: data to send
  * @retval None
  */
void UART_SendByte(uint8_t data) 
{	
	/* Send byte through the UART peripheral */
  HAL_UART_Transmit(&huart, &data, 1, UARTx_TIMEOUT_MAX);
}

/**
  * @brief  Receive one byte over UART
  * @param  None
  * @retval Return received data
  */
uint8_t UART_ReceiveByte( void ) 
{	
  uint8_t data = 0;
  
  HAL_UART_Receive(&huart, &data, 1, UARTx_TIMEOUT_MAX);
  
  return data;
}

/**
  * @brief  Send a byte array over UART
  * @param  pData  : buffer to send
  * @param  length : number of bytes to send
  * @retval None
  */
void UART_SendBuffer(const uint8_t *pCommand, uint8_t length) 
{
  uint8_t i;
  
  for(i=0; i<length; i++)
  {
    UART_SendByte(pCommand[i]);
  }
}

/**
  * @brief  Send a byte array over UART
  * @param  pResponse : pointer on the buffer response
  * @param  length 	 : number of bytes to read
  * @retval None
  */
void UART_ReceiveBuffer(uint8_t *pResponse, uint8_t length) 
{
  uint8_t i;
  
  for(i=0; i<length; i++)
    pResponse[i] = UART_ReceiveByte();
	
}

/**
 * @}
 */ 

/**
 * @}
 */ 

/**
*  @}
*/ 

/******************* (C) COPYRIGHT 2016 STMicroelectronics *****END OF FILE****/
