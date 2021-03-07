/**
 ******************************************************************************
 * @file    nfc06a1_conf.h
 * @author  MMY Application Team
 * @brief   This file contains definitions for the NFC6 components bus interfaces
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under Ultimate Liberty license
 * SLA0094, the "License"; You may not use this file except in compliance with
 * the License. You may obtain a copy of the License at:
 *                             www.st.com/SLA0094
 *
 ******************************************************************************
 */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef NFC06A1_CONF_H_
#define NFC06A1_CONF_H_

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"
#include "gpio.h"


#if 0 /* Code not compiled, reserved for Doxygen */
/** @addtogroup BSP
  * @{
  */

/** @addtogroup NFC06A1
  * @{
  */
#endif /* MISRAC2012 4.4 : Avoid a section of code seems to be commented out */

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#if 0 /* Code not compiled, reserved for Doxygen */
/** @defgroup NFC06A1_Exported_Constants
  * @{
  */
#endif /* MISRAC2012 4.4 : Avoid a section of code seems to be commented out */

#define NFC06A1_LED1_PIN_CLK_ENABLE()     __HAL_RCC_GPIOA_CLK_ENABLE();
#define NFC06A1_LED2_PIN_CLK_ENABLE()     __HAL_RCC_GPIOA_CLK_ENABLE();
#define NFC06A1_LED3_PIN_CLK_ENABLE()     __HAL_RCC_GPIOB_CLK_ENABLE();
#define NFC06A1_LED4_PIN_CLK_ENABLE()     __HAL_RCC_GPIOC_CLK_ENABLE();
#define NFC06A1_LED5_PIN_CLK_ENABLE()     __HAL_RCC_GPIOC_CLK_ENABLE();
#define NFC06A1_LED6_PIN_CLK_ENABLE()     __HAL_RCC_GPIOA_CLK_ENABLE();
#define NFC06A1_LED1_PIN_CLK_DISABLE()    __HAL_RCC_GPIOA_CLK_DISABLE();
#define NFC06A1_LED2_PIN_CLK_DISABLE()    __HAL_RCC_GPIOA_CLK_DISABLE();
#define NFC06A1_LED3_PIN_CLK_DISABLE()    __HAL_RCC_GPIOB_CLK_DISABLE();
#define NFC06A1_LED4_PIN_CLK_DISABLE()    __HAL_RCC_GPIOC_CLK_DISABLE();
#define NFC06A1_LED5_PIN_CLK_DISABLE()    __HAL_RCC_GPIOC_CLK_DISABLE();
#define NFC06A1_LED6_PIN_CLK_DISABLE()    __HAL_RCC_GPIOA_CLK_DISABLE();
#define NFC06A1_LED1_PIN				  LED1_Pin
#define NFC06A1_LED1_PIN_PORT             LED1_GPIO_Port
#define NFC06A1_LED2_PIN                  LED2_Pin
#define NFC06A1_LED2_PIN_PORT             LED2_GPIO_Port
#define NFC06A1_LED3_PIN                  LED3_Pin
#define NFC06A1_LED3_PIN_PORT             LED3_GPIO_Port
#define NFC06A1_LED4_PIN                  LED4_Pin
#define NFC06A1_LED4_PIN_PORT             LED4_GPIO_Port
#define NFC06A1_LED5_PIN                  LED5_Pin
#define NFC06A1_LED5_PIN_PORT             LED5_GPIO_Port
#define NFC06A1_LED6_PIN                  LED6_Pin
#define NFC06A1_LED6_PIN_PORT             LED6_GPIO_Port
#if 0 /* Code not compiled, reserved for Doxygen */
/**
  * @}
  */
#endif /* MISRAC2012 4.4 : Avoid a section of code seems to be commented out */

/* Exported macro ------------------------------------------------------------*/
#if 0 /* Code not compiled, reserved for Doxygen */
/** @defgroup NFC06A1_Exported_Macro
  * @{
  */
#endif /* MISRAC2012 4.4 : Avoid a section of code seems to be commented out */

#define NFC06A1_ALLLED_GPIO_CLK_ENABLE()  do{__HAL_RCC_GPIOA_CLK_ENABLE(); \
                                             __HAL_RCC_GPIOB_CLK_ENABLE(); \
                                             __HAL_RCC_GPIOC_CLK_ENABLE(); \
                                            }while(0)

#define NFC06A1_ALLLED_GPIO_CLK_DISABLE() do{__HAL_RCC_GPIOA_CLK_DISABLE(); \
                                             __HAL_RCC_GPIOB_CLK_DISABLE(); \
                                             __HAL_RCC_GPIOC_CLK_DISABLE(); \
                                            }while(0)
#if 0 /* Code not compiled, reserved for Doxygen */
/**
  * @}
  */
#endif /* MISRAC2012 4.4 : Avoid a section of code seems to be commented out */
/* Exported variables --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
#if 0 /* Code not compiled, reserved for Doxygen */
/**
  * @}
  */

/**
  * @}
  */
#endif /* MISRAC2012 4.4 : Avoid a section of code seems to be commented out */

#ifdef __cplusplus
}
#endif
#endif /* NFC06A1_CONF_H_ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
