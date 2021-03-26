/**
  ******************************************************************************
  * @file    proprietary_p2p.h
  * @author  MMY Application Team
  * @version V1.0.0
  * @date    01/09/2014
  * @brief   Example of proprietary P2P exchange
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
	
#ifndef __PROPRIETARY_P2P_H
#define __PROPRIETARY_P2P_H

#include "menu.h"
#include "lib_ConfigManager.h"


#define PP2P_KO			0
#define PP2P_OK			1

#define BALL_SIZE 20

void ProprietaryP2PAuto(void);
void ProprietaryP2PConfigServer(void);
void ProprietaryP2PConfigClient(void);
void CheckForUserAction (void);

void PongConfigServer(void);
void PongConfigClient(void);
void PongCheckForUserAction (void);

#endif
