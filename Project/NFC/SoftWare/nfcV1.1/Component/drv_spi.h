/**
  ******************************************************************************
  * @file    drv_spi.h
  * @author  MMY Application Team
  * @version $Revision: 1508 $
  * @date    $Date: 2016-01-08 09:50:04 +0100 (Fri, 08 Jan 2016) $
  * @brief   This file provides a set of firmware functions to manages SPI communications
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
#ifndef __DRV_SPI_H
#define __DRV_SPI_H

/* Includes ----------------------------------------------------------------- */
#include "main.h"

/** @addtogroup BSP
  * @{
  */

/** @defgroup X_NUCLEO_NFC03A1_Spi
  * @{
  */

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/** @defgroup X_NUCLEO_NFC03A1_Spi_Exported_Constants
  * @{
  */
#define SPI_RESPONSEBUFFER_SIZE		        255
/** 
 * @brief  SPI Interface pins 
 */ 
#define RFTRANS_95HF_SPI		        SPI1

#define RFTRANS_95HF_SPI_SCK_PIN                GPIO_PIN_5                
#define RFTRANS_95HF_SPI_SCK_GPIO_PORT          GPIOA                  
	 
#define RFTRANS_95HF_SPI_MISO_PIN               GPIO_PIN_6
#define RFTRANS_95HF_SPI_MISO_GPIO_PORT         GPIOA

#define RFTRANS_95HF_SPI_MOSI_PIN               GPIO_PIN_7
#define RFTRANS_95HF_SPI_MOSI_GPIO_PORT         GPIOA                

#define RFTRANS_95HF_SPI_NSS_PIN             	GPIO_PIN_6                  
#define RFTRANS_95HF_SPI_NSS_GPIO_PORT       	GPIOB                       

/** 
 * @brief  SPI DMA definitions 
 */

#define RFTRANS_95HF_SPI_DMA                	((uint32_t)DMA1)
	
#define RFTRANS_95HF_SPI_RX_DMA_CHANNEL     	DMA1_Stream2
#define RFTRANS_95HF_SPI_RX_DMA_FLAG        	DMA1_FLAG_TC2
	
#define RFTRANS_95HF_SPI_TX_DMA_CHANNEL    	DMA1_Stream3
#define RFTRANS_95HF_SPI_TX_DMA_FLAG       	DMA1_FLAG_TC3  
	
#define RFTRANS_95HF_SPI_DR_Base           	0x4001300C

/* Maximum Timeout values for flags waiting loops. These timeouts are not based
   on accurate values, they just guarantee that the application will not remain
   stuck if the SPI communication is corrupted.
   You may modify these timeout values depending on CPU frequency and application
   conditions (interrupts routines ...). */   
#define SPIx_TIMEOUT_MAX                   	1000
/** 
 * @brief  External Interupt pin PA9
 */ 
 /*
  IRQ_IN (CR95HF Pin 12)
 */
#define IRQIN_RFTRANS_95HF_PIN                  GPIO_PIN_9
#define IRQIN_RFTRANS_GPIO_PORT                 GPIOA  
	
/** 
 * @brief  IRQout Interface pin PA10
 */ 
 	/* 
	IRQ_OUT (CR95HF Pin 14)
	*/
#define IRQOUT_RFTRANS_95HF_PIN        		GPIO_PIN_10
#define IRQOUT_RFTRANS_95HF_PORT       		GPIOA

/** 
 * @brief  Communication Interface PA10
 */
#define INTERFACE_PIN             		GPIO_PIN_7
#define INTERFACE_GPIO_PORT       		GPIOC
/**
  * @}
  */

/* External variables --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/** @defgroup X_NUCLEO_NFC03A1_Spi_Exported_Macro
  * @{
  */
/** 
 * @brief  ST95HF Macro definition 
 */
/* set state on SPI_NSS pin */
#define RFTRANS_95HF_NSS_LOW() 			HAL_GPIO_WritePin(RFTRANS_95HF_SPI_NSS_GPIO_PORT, RFTRANS_95HF_SPI_NSS_PIN, GPIO_PIN_RESET)
#define RFTRANS_95HF_NSS_HIGH()  		HAL_GPIO_WritePin(RFTRANS_95HF_SPI_NSS_GPIO_PORT, RFTRANS_95HF_SPI_NSS_PIN, GPIO_PIN_SET)
/* set state on IRQ_In pin */
#define RFTRANS_95HF_IRQIN_LOW() 		HAL_GPIO_WritePin(IRQIN_RFTRANS_GPIO_PORT, IRQIN_RFTRANS_95HF_PIN, GPIO_PIN_RESET)	
#define RFTRANS_95HF_IRQIN_HIGH()		HAL_GPIO_WritePin(IRQIN_RFTRANS_GPIO_PORT, IRQIN_RFTRANS_95HF_PIN, GPIO_PIN_SET)
/* set state on SPI_CLK pin */
#define RFTRANS_95HF_SCK_LOW() 			HAL_GPIO_WritePin(RFTRANS_95HF_SPI_SCK_GPIO_PORT, RFTRANS_95HF_SPI_SCK_PIN, GPIO_PIN_RESET)
#define RFTRANS_95HF_SCK_HIGH()  		HAL_GPIO_WritePin(RFTRANS_95HF_SPI_SCK_GPIO_PORT, RFTRANS_95HF_SPI_SCK_PIN, GPIO_PIN_SET)
/**
  * @}
  */
  
/* Exported functions ------------------------------------------------------- */
/** @defgroup X_NUCLEO_NFC03A1_Spi_Exported_Functions
  * @{
  */
void RFTRANS_SPI_Init(void);
void RFTRANS_SPI_MSPInit(void);
void RFTRANS_ConfigureInterfacePin(void);
void SPI_SendReceiveBuffer(const uint8_t *pCommand, uint8_t length, uint8_t *pResponse);
uint8_t SPI_SendReceiveByte(uint8_t data); 

/**
  * @}
  */

/**
  * @}
  */
  
/**
  * @}
  */
#endif /* __DRV_SPI_H */

/******************* (C) COPYRIGHT 2016 STMicroelectronics *****END OF FILE****/
