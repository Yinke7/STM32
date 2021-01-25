/**
  ******************************************************************************
  * @file    drv_LED.h
  * @author  MMY Application Team
  * @version V1.1
  * @date    15/03/2011
  * @brief   This file provides
  *            - set of firmware functions to manage Leds
  *          available on STM3210B-EVAL evaluation board from STMicroelectronics.    
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  */ 


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DRIVER_LED_H
#define __DRIVER_LED_H

#ifdef __cplusplus
 extern "C" {
#endif


#define LEDn                             5

#define LED1_PIN                         GPIO_Pin_10
#define LED1_GPIO_PORT                   GPIOC
#define LED1_GPIO_CLK                    RCC_APB2Periph_GPIOC  
#define LED2_PIN                         GPIO_Pin_0
#define LED2_GPIO_PORT                   GPIOC
#define LED2_GPIO_CLK                    RCC_APB2Periph_GPIOC
#define LED3_PIN                         GPIO_Pin_1
#define LED3_GPIO_PORT                   GPIOC
#define LED3_GPIO_CLK                    RCC_APB2Periph_GPIOC  
#define LED4_PIN                         GPIO_Pin_2
#define LED4_GPIO_PORT                   GPIOC
#define LED4_GPIO_CLK                    RCC_APB2Periph_GPIOC  
#define LED5_PIN                         GPIO_Pin_3
#define LED5_GPIO_PORT                   GPIOC
#define LED5_GPIO_CLK                    RCC_APB2Periph_GPIOC     

typedef enum 
{
  LED1 = 0,
	LED2,
	LED3,
	LED4,
	LED5
} Led_TypeDef;


void LED_Config (Led_TypeDef Led);
void LED_On			(Led_TypeDef Led);
void LED_Off		(Led_TypeDef Led);
void LED_AllOff (void);

#ifdef __cplusplus
}
#endif

#endif /* __LCD_LIB_H */


/**
  * @}
  */ 

/**
  * @}
  */ 

/**
  * @}
  */ 

/**
  * @}
  */   
  
/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
