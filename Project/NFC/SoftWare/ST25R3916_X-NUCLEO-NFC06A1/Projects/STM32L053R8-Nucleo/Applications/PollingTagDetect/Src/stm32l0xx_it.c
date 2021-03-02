/**
  ******************************************************************************
  * @file    stm32l0xx_it.c 
  * @author  MMY Application Team
  * @brief   Main Interrupt Service Routines.
  *          This file provides function for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  *  This software component is licensed by ST under Ultimate Liberty license
  * SLA0094, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0094
  *
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32l0xx_it.h"
#include "nucleo_l053r8_bus.h"
#include "st25r3916_irq.h"

/** @addtogroup X-CUBE-NFC6_Applications
 *  @{
 */

/** @addtogroup PollingTagDetect
 *  @{
 */
   
/** @defgroup Interrupt_Handlers
 *  @brief Exceptions handlers and peripherals interrupt service routine.
 *  @{
 */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
#ifdef RFAL_USE_I2C
extern I2C_HandleTypeDef Handle_I2c1;
#endif /* RFAL_USE_I2C */
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M0 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
//void NMI_Handler(void)
//{
//}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}


/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
//void SVC_Handler(void)
//{
//}
//
///**
//  * @brief  This function handles PendSVC exception.
//  * @param  None
//  * @retval None
//  */
//void PendSV_Handler(void)
//{
//}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
  HAL_IncTick();
}

/******************************************************************************/
/*                 STM32L0xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32l0xxxx.s).                                             */
/******************************************************************************/
#ifdef RFAL_USE_I2C
/**
  * @brief This function handles EXTI line0 interrupt.
  * @param  None
  * @return None
*/
void EXTI0_1_IRQHandler(void)
{
  BSP_I2C1_IRQHandler();
}

void I2C1_IRQHandler(void)
{
  BSP_I2C1_EV_IRQHandler();
  BSP_I2C1_ER_IRQHandler();
}
#else
/**
  * @brief This function handles EXTI line0 interrupt.
  * @param  None
  * @return None
*/
void EXTI0_1_IRQHandler(void)
{
  BSP_SPI1_IRQHandler();
}
#endif /* RFAL_USE_I2C */


/**
  * @}
  */ 

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
