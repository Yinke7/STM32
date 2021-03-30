/**
  ******************************************************************************
  * @file    menu_cardemul.c
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
#include "menu_cardemul.h"


extern volatile bool 							KEYPress;
extern bool 											lockKEYUpDown;
extern PICCEMULATOR								CardEmulator;
extern PICCNFCT4_SELECTCMD_APPL		ApplicationSelected;

extern uint32_t 									FileSize;
extern uint32_t			 							FileSizeReceive;
extern uint32_t 									CRCFile;
extern uint32_t 									CRCCalc;
extern uint32_t 									ElapsedTimeMs;
extern bool 											AppliFeedBackReady;
extern uint8_t										FILE_TRANSFER_END;

static void DisplayFWUScreen(void);

extern uint32_t										NbByteReceived ;
extern uint8_t 										FileStatus;

extern uint32_t 									nb_ms_elapsed;

extern DeviceMode_t 		devicemode;
extern TagType_t 	nfc_tagtype;

/* space allocated to store firmeware inside internal RAM */
uint8_t			Firmware_Buffer [FIRMEWARE_SIZE];	

static uint32_t i,NbPixelPrevious=0,NbPixelCurrent=0;
static bool DisplayFileSize = true;
/* Avoid allocation in function to speed up application */
uint8_t CRCString[20] = "                    ";
uint8_t FileSizeString[20] = "                    ";
uint8_t ElapsedTimeString[20] = "                    ";

static void ApplicationInit(void);
static void ApplicationClose(void);

/**
  * @brief  Configures card emulation for type 4A tag.
  */
void CardEmul14443A(void)
{
	u8 NULL_NDEF[2] = {0,0};
	sAARInfo AAR_struct;
	
	sAARInfo *pAAR;
	
	pAAR = &AAR_struct;
	
	/* to set only AAR force NDEF size to 0 before */
	NDEF_WriteNDEF(NULL_NDEF);
	
	memcpy(pAAR->PakageName, "com.nfc.st95hfdemo", strlen("com.nfc.st95hfdemo")+1);
	
	/* Initilialize tag emulation mode */
	devicemode = PICC;
	nfc_tagtype = TT4A;
	
	NDEF_AddAAR(pAAR);

	LED_On(LED4);
	
	ApplicationInit();
	
	/* Start card emulation mode */
	ConfigManager_CardEmulation(PICCEMULATOR_TAG_TYPE_4A, CardEmul);
	
	LED_Off(LED4);
	
	ApplicationClose();
}


/**
  * @brief  Runs the card emulation with the selected tag type.
  */
void CardEmul(void)
{	
	switch(ApplicationSelected)
	{
		case PICCNFCT4_APPLI_FWU :	
			if (FileStatus == RFT_FILEWRITE)
			{	
				if(DisplayFileSize == true)
				{	
					/* Prepare screen before download */
					LCD_ClearLine(2);
					sprintf((char*)FileSizeString,"File size: %d B",FileSize);
					LCD_DisplayStringLine(Line2,FileSizeString);
					LCD_DisplayStringLine(Line3, "Download on going...");		
						
					/* reset systick variable to know number of ms elapsed when download complete */
					nb_ms_elapsed = 0;
						
					/* This is to avoid updating screen */
					DisplayFileSize	= false;
				}
									
				NbPixelCurrent=(NbByteReceived*256/FileSize);
					
				/* Complete Bar from NbPixelPrevious to NbPixelCurrent with step of 1px */
				for(i=NbPixelPrevious;i<NbPixelCurrent;i++)
				{
					LCD_DrawRect(125,i+30,20,1);
				}
				NbPixelPrevious=NbPixelCurrent;				
			}
			if (FILE_TRANSFER_END)
			{
				ElapsedTimeMs = nb_ms_elapsed;
				CRCCalc=file_CRC("fw.bin");
				AppliFeedBackReady = true;
					
				sprintf((char*)CRCString,"   CRC : %X     \n",CRCCalc); 
				/* Clear the LCD Screen */
				LCD_Clear(Green);
				LCD_Clear_ActiveWindow();
					
				LCD_SetBackColor(Blue);
				/* Set the Title Color */
				LCD_SetTextColor(White);
	
				LCD_DisplayStringLine(Line0, " Card Emulator Mode ");
				LCD_DisplayStringLine(Line9, (u8*)"Exit:  Push JoyStick");

				LCD_SetBackColor(Green);
				LCD_SetTextColor(Black);
					
				if(CRCFile==CRCCalc)
				{						
					LCD_DisplayStringLine(Line1," File transfer done ");
					LCD_DisplayStringLine(Line3,CRCString);
					
					LCD_DisplayStringLine(Line5," Elasped Time (ms): ");
					sprintf((char*)ElapsedTimeString,"       %d ",ElapsedTimeMs);
					LCD_DisplayStringLine(Line6,ElapsedTimeString);
					sprintf((char*)FileSizeString,"File size: %d B",FileSizeReceive);
					LCD_DisplayStringLine(Line8,FileSizeString);
						
					NbByteReceived=0;
					FILE_TRANSFER_END=0;
					while(!KEYPress)
					{
						/* Here we let some time to uploader to retrieve appli feedback */ 
						//PICCEmul_ManagePICCEmulation ();
					}
				}
				else
				{
					LCD_DisplayStringLine(Line1,"File transfer failed");
					LCD_DisplayStringLine(Line3,"   CRC computed:    ");
					sprintf((char*)CRCString,"   CRC : %X     \n",CRCCalc);
					LCD_DisplayStringLine(Line4,CRCString);
					LCD_DisplayStringLine(Line6,"   CRC expected:    ");
					sprintf((char*)CRCString,"   CRC : %X     \n",CRCFile);					
					LCD_DisplayStringLine(Line7,CRCString);
					sprintf((char*)FileSizeString,"File size: %d B",FileSizeReceive);
					LCD_DisplayStringLine(Line8,FileSizeString);
					NbByteReceived=0;
					FILE_TRANSFER_END=0;
					while(!KEYPress)
					{
						/* Here we let some time to uploader to retrieve appli feedback */ 
						//PICCEmul_ManagePICCEmulation ();
					}
				}
			}
			break;

		default:
			/* not handle in this demo firmware */
			break;
	}
	
	if( KEYPress == true)
	{	
		ConfigManager_Stop();
		return;
	}
	
}

/**
  * @brief  Runs the card emulation with the selected tag type.
  */
static void ApplicationInit(void)
{
	DisplayFWUScreen();
	
	/* Init all variable */
	CRCCalc = 0; 
	FileSize = 0;
	FileSizeReceive = 0;
	CRCFile = 0;
	CRCCalc = 0;
	FILE_TRANSFER_END=0;
	AppliFeedBackReady = false;
	ApplicationSelected =	PICCNFCT4_APPLI_UNKNOWN;
	NbPixelPrevious=0;
	NbPixelCurrent=0;
	NbByteReceived=0;
	DisplayFileSize = true;
	
	/* External Interrupt Enable */
	IntExtOnOffConfig(ENABLE);
	IntRotateOnOffConfig(DISABLE);
	
	/* Prepare to exit loop on Key Press */
	KEYPress = false;
	lockKEYUpDown = true;
}

/**
  * @brief  Runs the card emulation with the selected tag type.
  */
static void ApplicationClose(void)
{
	FileStatus = RFT_FILEUNKNOWN;
	
	/* External Interrupt Disable */
	IntExtOnOffConfig(DISABLE);		
	
	/* Clear the LCD Screen */
	LCD_Clear(White);
	
	/* Display Menu */
  DisplayMenu();
}

/**
  * @brief  Display a wait for reader screen.
  * @param  None
  * @retval None
  */
static void DisplayFWUScreen(void)
{		
	/* print screen TagEmulator */
	/* Set the Title Back Color */
  LCD_SetBackColor(Blue);

  /* Set the Title Color */
  LCD_SetTextColor(White);
	
	/* Clear the LCD Screen */
  LCD_Clear(Green);

  LCD_DisplayStringLine(Line0, " Card Emulator Mode ");
	LCD_DisplayStringLine(Line9, (u8*)"Exit:  Push JoyStick");
	
	/* Set the Text Back Color */
  LCD_SetBackColor(Green);
	/* Set the Text Color */
  LCD_SetTextColor(Black);

	LCD_DisplayStringLine(Line2, "  Ready to receive  ");
	LCD_DisplayStringLine(Line3, "        Data        ");
	
	LCD_DrawRect(125,30,20,256); 
	
}
