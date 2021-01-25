/**
  ******************************************************************************
  * @file    menu.h
  * @author  MMY Application Team
  * @version V1.0.0
  * @date    08/21/2013
  * @brief   Menu navigation driver for EVAL-ST95HF demonstration.
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
#ifndef __MENU_H
#define __MENU_H

/* Includes ------------------------------------------------------------------*/
#include "menu_writer.h"
#include "menu_reader.h"
#include "menu_tagemul.h"
#include "menu_cardemul.h"
#include "proprietary_p2p.h"


#include "drv_LED.h"
#include "drv_lcdspi_ili9341.h"

#include "stdio.h" /* for sprintf */

/* Private types ------------------------------------------------------------*/
/* Private constants --------------------------------------------------------*/
#define J_SEL_PIN				GPIO_Pin_6
#define J_SEL_PORT			GPIOB
#define J_SEL_CLOCK			RCC_APB2Periph_GPIOB

#define J_DOWN_PIN			GPIO_Pin_7
#define J_DOWN_PORT			GPIOB
#define J_DOWN_CLOCK		RCC_APB2Periph_GPIOB

#define J_LEFT_PIN			GPIO_Pin_5
#define J_LEFT_PORT			GPIOB
#define J_LEFT_CLOCK		RCC_APB2Periph_GPIOB

#define J_RIGHT_PIN			GPIO_Pin_8
#define J_RIGHT_PORT		GPIOB
#define J_RIGHT_CLOCK		RCC_APB2Periph_GPIOB

#define J_UP_PIN				GPIO_Pin_9
#define J_UP_PORT				GPIOB
#define J_UP_CLOCK			RCC_APB2Periph_GPIOB

#define ROTATE_PIN			GPIO_Pin_4
#define ROTATE_PORT			GPIOC
#define ROTATE_CLOCK		RCC_APB2Periph_GPIOC

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define  MAX_MENU_LEVELS 4
#define  NOKEY  0
#define  SEL    1
#define  RIGHT  2
#define  LEFT   3
#define  UP     4
#define  DOWN   5
#define  KEY    6
#define  ROTATE 7


#define ALARM_ICON 0x64FE0F54
#define USB_ICON   0x64FE8F96
#define WATCH_ICON 0x64FF0FD8
#define ST_LOGO    0x64F7FBFA
#define HELP       0x64FA543C
#define TSENSOR	   0x64FD8E4C

/* Module private variables --------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
#define countof(a) (sizeof(a) / sizeof(*(a)))

/* Private functions ---------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void Menu_Init(void);
void DisplayMenu(void);
void SelFunc(void);
void UpFunc(void);
void DownFunc(void);
void ReturnFunc(void);
void InitJoystick(void);
uint8_t ReadKey(void);
void Dummy_Func(void);
void IdleFunc(void);
void DisplayIcons(void);
void ShowMenuIcons(void);
void STIntro(void);
void DisplayVersion(void);
void ProductPres(void);
void PassiveP2P_RunAsTarget(void);
void PassiveP2P_RunAsInitiator(void);
void Hex2Char( u8* pDataIn, u16 NumberOfByte, char* pString );

void USBRemoteControl(void);

void IntRotateOnOffConfig(FunctionalState NewState);
void IntExtOnOffConfig(FunctionalState NewState);

void FillTagGEO(char* pTagType);
void FillTagURI(char* pTagType);
void FillTagSMS(char* pTagType);
void FillTagEMAIL(char* pTagType);

#endif /* __MENU_H */

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
