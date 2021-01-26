/**
  ******************************************************************************
  * @file    menu_auto.c 
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
	
#include "menu_auto.h"

extern volatile uint8_t KEYPressed;
extern bool 		lockKEYUpDown;
extern volatile bool KEYPress;
extern bool 		StopProcess ;
extern bool 		screenRotated;
extern bool			disableRotate;

/* Variables for the different modes */
extern sURI_Info 		url;
extern DeviceMode_t devicemode;
extern TagType_t nfc_tagtype;

/* Structure to manage the autoscan mode */
MANAGER_CONFIG manager;

/**
 * @brief  This function starts an automatic mode configured to find:
 * @brief  TT1/TT2/TT4A and PCDA
 */
void autoTypeA(void)
{
	manager.SelectedMode = SELECT_PICC | SELECT_PCD | SELECT_P2P;
	manager.PcdMode = TRACK_NFCTYPE1|TRACK_NFCTYPE2|TRACK_NFCTYPE4A;
	manager.PiccMode = TRACK_NFCTYPE2|TRACK_NFCTYPE4A;
	manager.P2pMode = INITIATOR_LLCPA  | TARGET_LLCPA;
	manager.timeoutEmul = DELAY_EMUL; // This is the timeout for tag emulation
	manager.timeoutReadEmul = DELAY_READ_EMUL;
	autoMode();
}

/**
 * @brief  This function starts an automatic mode configured to find:
 * @brief  TT4B 
 */
void autoTypeB(void)
{
	manager.SelectedMode = SELECT_PCD;
	manager.PcdMode = TRACK_NFCTYPE4B;
	manager.PiccMode = TRACK_NOTHING;
	manager.P2pMode = P2P_NOTHING;
	manager.timeoutEmul = DELAY_EMUL; // This is the timeout for tag emulation
	manager.timeoutReadEmul = DELAY_READ_EMUL;
	autoMode();
}

/**
 * @brief  This function starts an automatic mode configured to find:
 * @brief  TT3 and P2P target type F
 */
void autoTypeF(void)
{
	manager.SelectedMode = SELECT_PCD|SELECT_P2P;
	manager.PcdMode = TRACK_NFCTYPE3;
	manager.PiccMode = TRACK_NOTHING;
	manager.P2pMode = INITIATOR_LLCPF;
	manager.timeoutEmul = DELAY_EMUL; // This is the timeout for tag emulation
	manager.timeoutReadEmul = DELAY_READ_EMUL;
	autoMode();
}

/**
 * @brief  This function starts an automatic mode configured to find:
 * @brief  TT5
 */
void autoTypeV(void)
{
	// Type 5 is not supported in tag emulation so we can't find any TT5 reader
	manager.SelectedMode = SELECT_PCD;
	manager.PcdMode = TRACK_NFCTYPE5;
	manager.PiccMode = TRACK_NOTHING;
	manager.P2pMode = P2P_NOTHING;
	manager.timeoutEmul = DELAY_EMUL; // This is the timeout for tag emulation
	manager.timeoutReadEmul = DELAY_READ_EMUL;
	autoMode();
}

/**
 * @brief  This function starts an automatic mode configured to find:
 * @brief  P2P initiator typeA, P2P target type A and F
 */
void autoTypeP2P(void)
{
	manager.SelectedMode = SELECT_P2P;
	manager.PcdMode = TRACK_NOTHING;
	manager.PiccMode = TRACK_NOTHING;
	manager.P2pMode = INITIATOR_LLCPA | INITIATOR_LLCPF | TARGET_LLCPA;
	manager.timeoutEmul = DELAY_EMUL; // This is the timeout for tag emulation
	manager.timeoutReadEmul = DELAY_READ_EMUL;
	autoMode();
}

/**
 * @brief  This function starts an automatic mode configured to find:
 * @brief  TT1/TT2/TT3/TT4A/TT4B/TT5, PCDA and P2P initiator typeA, P2P target type A end F
 */
void autoTypeAll(void)
{
	manager.SelectedMode = SELECT_PICC|SELECT_PCD|SELECT_P2P;
	manager.PcdMode = TRACK_ALL;
	manager.PiccMode = TRACK_NFCTYPE2|TRACK_NFCTYPE4A;
	manager.P2pMode = INITIATOR_LLCPA | INITIATOR_LLCPF | TARGET_LLCPA;
	manager.timeoutEmul = DELAY_EMUL; // This is the timeout for tag emulation
	manager.timeoutReadEmul = DELAY_READ_EMUL;
	autoMode();
}

/**
 * @brief  This function launch an automatic mode where NFC chip change is state and result is printed on the screen
 */
void autoMode(void)
{
  uint8_t status;
	char message[21];
	
	/* Clear the LCD Screen */
//  LCD_Clear(Black);
//	/* Set the Back Color */
//  LCD_SetBackColor(Blue);
//  /* Set the Text Color */
//  LCD_SetTextColor(White);
//  LCD_DisplayStringLine(Line0, (u8*)"   AutoScan Mode    ");
//  LCD_DisplayStringLine(Line9, (u8*)"   Select to exit   ");
//	 /* Set the Back Color */
//  LCD_SetBackColor(Black);
//  /* Set the Text Color */
//  LCD_SetTextColor(White);
//	
//	// Print all the parameters selected
//  LCD_DisplayStringLine(Line2, (u8*)"Configured to detect");
	// Try to find TT1/2/3/4A/4B/5/
	if (manager.SelectedMode&SELECT_PCD)
	{
		memset(message,'\0',20);
		strcat(message,"TT");
		if (manager.PcdMode&TRACK_NFCTYPE1)strcat(message,"1/");
		if (manager.PcdMode&TRACK_NFCTYPE2)strcat(message,"2/");
		if (manager.PcdMode&TRACK_NFCTYPE3)strcat(message,"3/");
		if (manager.PcdMode&TRACK_NFCTYPE4A)strcat(message,"4A/");
		if (manager.PcdMode&TRACK_NFCTYPE4B)strcat(message,"4B/");
		if (manager.PcdMode&TRACK_NFCTYPE5)strcat(message,"5/");
		LCD_DisplayStringLine(Line3, (u8*)message);
	}
	// Try to find PCD
	if (manager.SelectedMode&SELECT_PICC)
	{
		if( (manager.PiccMode&TRACK_NFCTYPE4A) || (manager.PiccMode&TRACK_NFCTYPE2) )
//			LCD_DisplayStringLine(Line4, (u8*)" NFCA reader        ");
		printf("NFCA reader\r\n");
	}
	// Try to find P2P
	if (manager.SelectedMode&SELECT_P2P)
	{
		if( manager.P2pMode & TARGET_LLCPA)
//			LCD_DisplayStringLine(Line5, (u8*)" P2P Initiator typeA");
		printf("P2P Initiator typeA\r\n");
		if( (manager.P2pMode & INITIATOR_LLCPA) && (manager.P2pMode & INITIATOR_LLCPF) )
//			LCD_DisplayStringLine(Line6, (u8*)" P2P Target typeA/F ");
		printf("P2P Target typeA/F\r\n");
		else if (manager.P2pMode & INITIATOR_LLCPA)
//			LCD_DisplayStringLine(Line6, (u8*)" P2P Target typeA   ");
		printf("P2P Target typeA\r\n");
		else if (manager.P2pMode & INITIATOR_LLCPF)
//			LCD_DisplayStringLine(Line6, (u8*)" P2P Target typeF   ");	
			printf("P2P Target typeF\r\n");
	}
//	LCD_DisplayStringLine(Line8, (u8*)"  Scan started...   ");
	printf("Scan started...\r\n");
	
	initFlashNDEF();
  
  /* if we want to find PCD we have to emulate tag, put default content */
  devicemode = PICC;
  nfc_tagtype = TT4A;
  strcpy(url.protocol,"http://");
  strcpy(url.URI_Message,"st.com");
  NDEF_WriteURI(&url);
  
  delay_ms(1000);
  
//  LCD_ClearLine(Line1);LCD_ClearLine(Line2);LCD_ClearLine(Line3);LCD_ClearLine(Line4);
//  LCD_ClearLine(Line5);LCD_ClearLine(Line6);LCD_ClearLine(Line7);LCD_ClearLine(Line8);
	
	/* External Interrupt Enable */
	IntExtOnOffConfig(ENABLE);
	IntRotateOnOffConfig(DISABLE);
	/* Prepare to exit loop on SEL Key Press */
	KEYPress = false; 
	lockKEYUpDown = true;
	/* wait user action to go back to previous screen */
	while(!KEYPress)
	{
		devicemode = UNDEFINED_MODE;		
		ConfigManager_AutoMode(&manager);
		
			// If the st95hf is configured as PCD it means that a PICC has been found
			if (devicemode == PCD)
		{	
      // Determine what kind of tag was found
			if (nfc_tagtype == TT1)
			{
				LED_AllOff();LED_On(LED1);
			}
			else if (nfc_tagtype == TT2)
			{
				LED_AllOff();LED_On(LED2);
			}
			else if (nfc_tagtype == TT3)
			{
				LED_AllOff();LED_On(LED3);
			}
			else if (nfc_tagtype == TT4A)
			{
				LED_AllOff();LED_On(LED4);
			}
			else if (nfc_tagtype == TT4B)
			{
				LED_AllOff();LED_On(LED5);
			}
			else if (nfc_tagtype == TT5)
			{
				LED_AllOff();LED_On(LED2);LED_On(LED5);
			}
			
			/* Try to read the NDEF message inside the tag */
			status = readNDEFfromTAG();
			if (status == PCDNFC_ERROR_MEMORY_INTERNAL)
			{
//				LCD_ClearLine(Line3);LCD_ClearLine(Line4);LCD_ClearLine(Line5);LCD_ClearLine(Line6);LCD_ClearLine(Line7);
//				LCD_SetTextColor(Red);
//				LCD_DisplayStringLine(Line3, (u8*)"Cannot fill internal");
//				LCD_DisplayStringLine(Line4, (u8*)"memory with NDEF    ");
//				LCD_DisplayStringLine(Line5, (u8*)"message             ");
//				LCD_SetTextColor(White);
				printf("Cannot fill internal\r\n");
			}
			else if (status == PCDNFC_ERROR_NOT_FORMATED)
			{
//				LCD_ClearLine(Line3);LCD_ClearLine(Line4);LCD_ClearLine(Line5);LCD_ClearLine(Line6);LCD_ClearLine(Line7);
//				LCD_SetTextColor(White);
//				LCD_DisplayStringLine(Line3, (u8*)"     Empty tag      ");
				printf("Empty tag\r\n");
			}
      
      delay_ms(100);
		}
    else if (devicemode == PICC)
    {    
//      LCD_ClearLine(Line1);LCD_ClearLine(Line2);LCD_ClearLine(Line3);LCD_ClearLine(Line4);
//      LCD_ClearLine(Line5);LCD_ClearLine(Line6);LCD_ClearLine(Line7);LCD_ClearLine(Line8);
//      LED_AllOff();
//      memcpy(message,"NFCA reader detected",20);
//      LCD_DisplayStringLine(Line5, (u8*)message);
		printf("NFCA reader detected\r\n");
		}
		else if(manager.Result == TARGET_LLCPA)
		{
//			LCD_ClearLine(Line1);LCD_ClearLine(Line2);LCD_ClearLine(Line3);LCD_ClearLine(Line4);
//      LCD_ClearLine(Line5);LCD_ClearLine(Line6);LCD_ClearLine(Line7);LCD_ClearLine(Line8);
//      LED_AllOff();
//			memcpy(message,"P2P Initiator TypeA ",20);
//      LCD_DisplayStringLine(Line5, (u8*)message);
			printf("P2P Initiator TypeA\r\n");
		}
		else if(manager.Result == INITIATOR_LLCPA)
		{
//			LCD_ClearLine(Line1);LCD_ClearLine(Line2);LCD_ClearLine(Line3);LCD_ClearLine(Line4);
//      LCD_ClearLine(Line5);LCD_ClearLine(Line6);LCD_ClearLine(Line7);LCD_ClearLine(Line8);
//      LED_AllOff();
//			memcpy(message," P2P Target TypeA   ",20);
//      LCD_DisplayStringLine(Line5, (u8*)message);
			printf("P2P Target TypeA\r\n");
		}
		else if(manager.Result == INITIATOR_LLCPF)
		{
//			LCD_ClearLine(Line1);LCD_ClearLine(Line2);LCD_ClearLine(Line3);LCD_ClearLine(Line4);
//      LCD_ClearLine(Line5);LCD_ClearLine(Line6);LCD_ClearLine(Line7);LCD_ClearLine(Line8);
//      LED_AllOff();
//			memcpy(message," P2P Target TypeF   ",20);
//      LCD_DisplayStringLine(Line5, (u8*)message);
			printf("P2P Target TypeF\r\n");
		}
	}// Exit the loop only when the hunt has been stoped by the user
	
//  DisplayMenu();
}
