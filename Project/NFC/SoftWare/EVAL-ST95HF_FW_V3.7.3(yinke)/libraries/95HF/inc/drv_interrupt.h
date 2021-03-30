/**
  ******************************************************************************
  * @file    drv_interrupt.h 
  * @author  MMY Application Team
  * @version V4.0.0
  * @date    02/06/2014
  * @brief   This file configured the interruption raised by 95HF device.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DRV_INTERRUPT_H
#define __DRV_INTERRUPT_H

/* Includes ------------------------------------------------------------------*/
#include "hw_config.h"

/* Exported types ------------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/
/* Exported define -----------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

/* flags -------------------------------------------------------------------- */
//#ifndef CR95HF 
#define SPI_INTERRUPT_MODE_ACTIVATED
//#endif

/** 
 * @brief  SPI Interface pins 
 */ 
#define RFTRANS_95HF_SPI			     						SPI2
#define RFTRANS_95HF_SPI_CLK		    					RCC_APB1Periph_SPI2

#define RFTRANS_95HF_SPI_SCK_PIN        			GPIO_Pin_13                 
#define RFTRANS_95HF_SPI_SCK_GPIO_PORT   			GPIOB                       
#define RFTRANS_95HF_SPI_SCK_GPIO_CLK    			RCC_APB2Periph_GPIOB 
	 
#define RFTRANS_95HF_SPI_MISO_PIN        			GPIO_Pin_14                 
#define RFTRANS_95HF_SPI_MISO_GPIO_PORT  			GPIOB                       
#define RFTRANS_95HF_SPI_MISO_GPIO_CLK   			RCC_APB2Periph_GPIOB 
	 
#define RFTRANS_95HF_SPI_MOSI_PIN        			GPIO_Pin_15                 
#define RFTRANS_95HF_SPI_MOSI_GPIO_PORT  			GPIOB                       
#define RFTRANS_95HF_SPI_MOSI_GPIO_CLK   			RCC_APB2Periph_GPIOB 

#define RFTRANS_95HF_SPI_NSS_PIN             	GPIO_Pin_12                  
#define RFTRANS_95HF_SPI_NSS_GPIO_PORT       	GPIOB                       
#define RFTRANS_95HF_SPI_NSS_GPIO_CLK        	RCC_APB2Periph_GPIOB 

#define RFTRANS_95HF_SPI_DMA                	DMA1
#define RFTRANS_95HF_SPI_DMA_CLK            	RCC_AHBPeriph_DMA1  
	
#define RFTRANS_95HF_SPI_Rx_DMA_Channel     	DMA1_Channel2
#define RFTRANS_95HF_SPI_Rx_DMA_FLAG        	DMA1_FLAG_TC2
	
#define RFTRANS_95HF_SPI_Tx_DMA_Channel    		DMA1_Channel3
#define RFTRANS_95HF_SPI_Tx_DMA_FLAG       		DMA1_FLAG_TC3  
	
#define RFTRANS_95HF_SPI_DR_Base           		0x4001300C

/** 
 * @brief  RFTRANS_95HF UART Interface pins 
 */ 
#define RFTRANS_95HF_UART			        				USART2
#define RFTRANS_95HF_UART_CLK		       				RCC_APB1Periph_USART2

#define RFTRANS_95HF_UART_TX_PIN         			GPIO_Pin_11
#define RFTRANS_95HF_UART_TX_GPIO_PORT   			GPIOB                       
#define RFTRANS_95HF_UART_TX_GPIO_CLK    			RCC_APB2Periph_GPIOB 
	 
#define RFTRANS_95HF_UART_RX_PIN        			GPIO_Pin_10                 
#define RFTRANS_95HF_UART_RX_GPIO_PORT  			GPIOB                       
#define RFTRANS_95HF_UART_RX_GPIO_CLK   			RCC_APB2Periph_GPIOB 


/** 
 * @brief  External Interupt pin PA3
 */ 
#define EXTI_RFTRANS_95HF_PIN        					GPIO_Pin_11
#define EXTI_GPIO_PORT       			      			GPIOB  
#define EXTI_RFTRANS_95HF_PIN_SOURCE        	GPIO_PinSource11
#define EXTI_RFTRANS_95HF_GPIO_CLOCK					RCC_APB2Periph_GPIOB
#define EXTI_RFTRANS_95HF_GPIO_PORT_SOURCE		GPIO_PortSourceGPIOB
#define EXTI_RFTRANS_95HF_LINE								EXTI_Line11


/** 
 * @brief  IRQout Interface pin PA2
 */ 
#define IRQOUT_RFTRANS_95HF_PIN        				GPIO_Pin_10
#define IRQOUT_RFTRANS_95HF_PIN_SOURCE        GPIO_PinSource10
#define IRQOUT_RFTRANS_95HF_GPIO_CLOCK				RCC_APB2Periph_GPIOB
#define IRQOUT_RFTRANS_95HF_GPIO_PORT_SOURCE	GPIO_PortSourceGPIOB



/** 
 * @brief  TIMER definitions 
 */
 
/* -------------------------------------------------------------------------- 
* timeout configuration (us)
* --------------------------------------------------------------------------
* 72 MHz / 72 = 1MHz (1us )
* 1�s * 1000 + 1�s ~= 1ms	
* -------------------------------------------------------------------------- */
#define TIMER_TIMEOUT													TIM3
#define TIMER_TIMEOUT_PERIOD									1000
#define TIMER_TIMEOUT_PRESCALER								72
#define TIMER_TIMEOUT_CLOCK										RCC_APB1Periph_TIM3

/* -------------------------------------------------------------------------- 
* timeout configuration (ms)
* --------------------------------------------------------------------------
* 72 MHz / 72 = 1MHz (1us )
* 1�s * 1000 + 1�s ~= 1ms	
* -------------------------------------------------------------------------- */
#define APPLI_TIMER_TIMEOUT										TIM4
#define APPLI_TIMER_TIMEOUT_PERIOD						1000
#define APPLI_TIMER_TIMEOUT_PRESCALER					72
#define APPLI_TIMER_TIMEOUT_CLOCK							RCC_APB1Periph_TIM4
/** 
 * @brief  NVIC definitions 
 */

/**
 *	@brief  Interrupts configuration
|	 name					|	preemption priority		|	sub proiority			|	channel						|
|---------------|-----------------------|-------------------|-------------------|
| 95HF					|						0						|		0								|	EXTI3_IRQn				|
|---------------|-----------------------|-------------------|-------------------|
| timeout				|						0 					|		0								|	TIM3_IRQn					|
|---------------|-----------------------|-------------------|-------------------|
| appli timeout	|						0 					|		0								|	TIM4_IRQn					|
|---------------|-----------------------|-------------------|-------------------|
 */
#define EXTI_RFTRANS_95HF_PREEMPTION_PRIORITY	1
#define EXTI_RFTRANS_95HF_SUB_PRIORITY				1
//#define EXTI_RFTRANS_95HF_IRQ_CHANNEL					EXTI3_IRQn
//add yinke 
#define EXTI_RFTRANS_95HF_IRQ_CHANNEL					EXTI15_10_IRQn
//end add 

#define TIMER_TIMEOUT_PREEMPTION_PRIORITY			0
#define TIMER_TIMEOUT_SUB_PRIORITY						0
#define TIMER_TIMEOUT_IRQ_CHANNEL							TIM3_IRQn

#define APPLI_TIMER_TIMEOUT_PREEMPTION_PRIORITY			0
#define APPLI_TIMER_TIMEOUT_SUB_PRIORITY						1
#define APPLI_TIMER_TIMEOUT_IRQ_CHANNEL							TIM4_IRQn

/** 
 * @brief  IRQ names 
 */
#define TIMER_TIMEOUT_IRQ_HANDLER			 				TIM3_IRQHandler
#define APPLI_TIMER_TIMEOUT_IRQ_HANDLER				TIM4_IRQHandler
#define RFTRANS_95HF_IRQ_HANDLER							EXTI15_10_IRQHandler	//EXTI3_IRQHandler EXTI_Line11

/* Exported functions ------------------------------------------------------- */

// set state on SPI_NSS pin
#define RFTRANS_95HF_NSS_LOW() 					GPIO_ResetBits(RFTRANS_95HF_SPI_NSS_GPIO_PORT, RFTRANS_95HF_SPI_NSS_PIN)
#define RFTRANS_95HF_NSS_HIGH()  				GPIO_SetBits  (RFTRANS_95HF_SPI_NSS_GPIO_PORT, RFTRANS_95HF_SPI_NSS_PIN)
// set state on IRQ_In pin
#define RFTRANS_95HF_IRQIN_LOW() 				GPIO_WriteBit(RFTRANS_95HF_UART_RX_GPIO_PORT, RFTRANS_95HF_UART_RX_PIN,Bit_RESET)	
#define RFTRANS_95HF_IRQIN_HIGH()  			GPIO_WriteBit(RFTRANS_95HF_UART_RX_GPIO_PORT, RFTRANS_95HF_UART_RX_PIN,Bit_SET)
// set state on SPI_CLK pin
#define RFTRANS_95HF_SCK_LOW() 					GPIO_ResetBits(RFTRANS_95HF_SPI_SCK_GPIO_PORT, RFTRANS_95HF_SPI_SCK_PIN)
#define RFTRANS_95HF_SCK_HIGH()  				GPIO_SetBits  (RFTRANS_95HF_SPI_SCK_GPIO_PORT, RFTRANS_95HF_SPI_SCK_PIN)



/* Timeout use during the communication with the 95HF device with SPI or UART path */
void drvInt_TimeoutTimerConfig				 ( void );
void drvInt_AppliTimeoutTimerConfig		 ( void );
void StartTimeOut											 ( uint16_t delay );
void StopTimeOut											 ( void );
void StartAppliTimeOut								 ( uint16_t delay );
void StopAppliTimeOut									 ( void );

void IRQOut_Config										 ( void );

void SendSPINSSPulse									 ( void );

void drvInt_Enable_Reply_IRQ					 ( void );
void drvInt_Enable_RFEvent_IRQ				 ( void );
void drvInt_Disable_95HF_IRQ					 ( void );

/* SPI or UART configuration */
#ifdef CR95HF
void SPINSS_Config										 ( void );
#endif /* CR95HF */


#endif /* __DRV_INTERRUPT_H */
