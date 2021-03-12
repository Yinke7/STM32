/**
  ******************************************************************************
  * @file    drv_uart.h
  * @author  MMY Application Team
  * @version $Revision: 1508 $
  * @date    $Date: 2016-01-08 09:50:04 +0100 (Fri, 08 Jan 2016) $
  * @brief   This file provides 
  				- set of firmware functions to manages UART communications
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

/* Define to prevent recursive inclusion ------------------------------------ */
#ifndef __DRV_UART_H
#define __DRV_UART_H

/* Includes ----------------------------------------------------------------- */
#include "main.h"

/** @addtogroup BSP
  * @{
  */

/** @defgroup X_NUCLEO_NFC03A1_Uart
  * @{
  */

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/** @defgroup X_NUCLEO_NFC03A1_Uart_Exported_Constants
  * @{
  */
#define UART_RESPONSEBUFFER_SIZE       255
#define UARTx_TIMEOUT_MAX              1000

/** 
 * @brief  RFTRANS_95HF UART Interface pins 
 */ 
#define RFTRANS_95HF_UART		USART1

#define RFTRANS_95HF_UART_RX_PIN       	GPIO_PIN_10
#define RFTRANS_95HF_UART_RX_GPIO_PORT  GPIOA
	 
#define RFTRANS_95HF_UART_TX_PIN        GPIO_PIN_9                 
#define RFTRANS_95HF_UART_TX_GPIO_PORT  GPIOA

#ifdef USE_CR95HF_UART
#define INTERFACE_PIN             	GPIO_PIN_7
#define INTERFACE_GPIO_PORT       	GPIOC
#endif


/**
  * @}
  */

/* External variables --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
/** @defgroup X_NUCLEO_NFC03A1_uart_Exported_Functions
  * @{
  */
/* UART functions */
void RFTRANS_UART_Init(const uint32_t BaudRate);
void RFTRANS_UART_MSPInit(void);
void UART_SendByte(uint8_t data);
uint8_t UART_ReceiveByte(void);
void UART_SendBuffer(const uint8_t *pCommand, uint8_t length); 
void UART_ReceiveBuffer	(uint8_t *pResponse, uint8_t length);

/**
  * @}
  */
  
/**
  * @}
  */
  
/**
  * @}
  */

#endif /* __DRV_UART_H */

/******************* (C) COPYRIGHT 2016 STMicroelectronics *****END OF FILE****/
