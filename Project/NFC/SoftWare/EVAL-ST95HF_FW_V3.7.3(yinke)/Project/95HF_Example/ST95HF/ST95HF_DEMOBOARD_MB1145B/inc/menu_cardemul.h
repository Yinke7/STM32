/**
  ******************************************************************************
  * @file    menu_cardemul.h
  * @author  MMY Application Team
  * @version V1.0.0
  * @date    A FAIRE
  * @brief   A FAIRE
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

#ifndef _MENU_CARDEMUL_H
#define _MENU_CARDEMUL_H

#include "menu.h"

#include "lib_NDEF_AAR.h"
#include "lib_flash_ndef.h"
#include "lib_ConfigManager.h"

void CardEmul14443A(void);
void CardEmul(void);

#endif
