/**
  ******************************************************************************
  * @file    menu_auto.h 
  * @author  MMY Application Team
  * @version V1.0.0
  * @date    10/07/2014
  * @brief   This file provides functions to scan for a PCD/PICC and display the result. 
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

/* Define to prevent recursive inclusion --------------------------------------------*/
#ifndef _MENU_AUTO_H
#define _MENU_AUTO_H

#include "menu.h"

#define DELAY_EMUL 50
#define DELAY_READ_EMUL 500

void autoTypeA(void);
void autoTypeB(void);
void autoTypeF(void);
void autoTypeV(void);
void autoTypeP2P(void);
void autoTypeAll(void);
void autoMode(void);

#endif

