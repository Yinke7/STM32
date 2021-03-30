/**
  ******************************************************************************
  * @file    menu_writer.c
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

#include "menu_writer.h"

extern const char uVcardCSL1[];

/* Variables for tag writing */
extern uint8_t 					TagUID[10];
extern ISO14443A_CARD 	ISO14443A_Card;
extern ISO14443B_CARD 	ISO14443B_Card;
extern FELICA_CARD 			FELICA_Card;
enum {TEXT,URI,SMS,VCARD,EMAIL,GEO};
uint8_t writeChoice = URI;
extern sURI_Info 		url;
extern sSMSInfo 		sms;
extern sEmailInfo 	email;
extern sGeoInfo 		geo;
static char message[] = "ST95HF Text NDEF Record";

extern volatile bool		KEYPress;
extern bool							lockKEYUpDown;

/* Variables for the different modes */
extern DeviceMode_t devicemode;
extern TagType_t nfc_tagtype;

/**
  * @brief  This function fills the demo URI structure and call the TagWriting function
  */
void TagWriteText(void)
{
	writeChoice = TEXT;
	TagWriting();
}

/**
  * @brief  This function fills the demo URI structure and call the TagWriting function
  */
void TagWriteURI(void)
{
	writeChoice = URI;
	/* Fill the structure of the NDEF URI */
	FillTagURI("ST website for near field communication");
	TagWriting();
}
/**
  * @brief  This function fills the demo SMS structure and call the TagWriting function
  */
void TagWriteSMS(void)
{
	writeChoice = SMS;
	/* Fill the structure of the NDEF SMS */
	FillTagSMS("SMS test with ST95HF library");
	TagWriting();
}
/**
  * @brief  This function fills the demo SMS structure and call the TagWriting function
  */
void TagWriteEMAIL(void)
{
	writeChoice = EMAIL;
	/* Fill the structure of the NDEF EMAIL */
	FillTagEMAIL("EMAIL test with ST95HF library");
	TagWriting();
}
/**
  * @brief  This function fills the demo GEO structure and call the TagWriting function
  */
void TagWriteGEO(void)
{
	writeChoice = GEO;
	/* Fill the structure of the NDEF GEO */
	FillTagGEO("ST95HF firmware birth place");
	TagWriting();
}
/**
  * @brief  This function fills the demo VCARD structure and call the TagWriting function
  */
void TagWriteVCARD(void)
{
	writeChoice = VCARD;
	TagWriting();
}
/**
  * @brief  This function scans for a tag and writes the content of the RAM buffer inside
  */
void TagWriting(void)
{
	uint8_t status, TagType;
	char LastUIDFound[20] = {' '};
	char EmptyString[20] = "                    ";
	bool NewTagDetected = false;
	
	LED_On(LED1);
//	LCD_Clear(Black);

//  /* Set the Back Color */
//  LCD_SetBackColor(Blue);
//  /* Set the Text Color */
//  LCD_SetTextColor(White);
//  
//  LCD_DisplayStringLine(Line0, (u8*)"  Tag Writer Mode   ");
//  LCD_DisplayStringLine(Line9, (u8*)"   Select to exit   ");
//	
//	 /* Set the Back Color */
//  LCD_SetBackColor(Black);
//  /* Set the Text Color */
//  LCD_SetTextColor(White);
//	
//	LCD_DisplayStringLine(Line2, (u8*)"ST95HF is searching ");
//  LCD_DisplayStringLine(Line3, (u8*)"for a tag to write..");
	
	LED_Off(LED1);
	nfc_tagtype = UNDEFINED_TAG_TYPE;
	devicemode = PCD;
	
	/* External Interrupt Enable */
//	IntExtOnOffConfig(ENABLE);
//	IntRotateOnOffConfig(DISABLE);
	/* Prepare to exit loop on SEL Key Press */
	KEYPress = false; 
	lockKEYUpDown = true;
	/* wait user action to go back to previous screen */
//	while(!KEYPress)
	{		
//		LCD_SetTextColor(White);
		
		/* Scan to find if there is a supported tag */
		TagType = ConfigManager_TagHunting(TRACK_ALL);	
		/* Check the tag type found */
		switch(TagType)
		{
			/* Write a message and power on one led */
			
			case TRACK_NFCTYPE1:
				if(memcmp (LastUIDFound, TagUID, 6))
				{	
					memcpy(LastUIDFound,TagUID,6);	
//					LCD_DisplayStringLine(Line2, (u8*)"Writing TT1...      ");
					printf("Writing TT1...\r\n");
					LED_On(LED1);
					NewTagDetected = true;
				}
				else
					goto End;
				break;
				
			case TRACK_NFCTYPE2:
				if(memcmp (LastUIDFound, ISO14443A_Card.UID, ISO14443A_Card.UIDsize))
				{	
					memcpy(LastUIDFound,ISO14443A_Card.UID,ISO14443A_Card.UIDsize);	
//					LCD_DisplayStringLine(Line2, (u8*)"Writing TT2...      ");
					printf("Writing TT2...\r\n");
					LED_On(LED2);
					NewTagDetected = true;
					}
				else
					goto End;
				break;	
			
			case TRACK_NFCTYPE3:
				if(memcmp (LastUIDFound, FELICA_Card.UID, 8))
				{	
					memcpy(LastUIDFound,FELICA_Card.UID,8);
//					LCD_DisplayStringLine(Line2, (u8*)"Writing TT3...      ");
					printf("Writing TT3...\r\n");
					LED_On(LED3);
					NewTagDetected = true;
			  }
				else
					goto End;
				break;	
				
			case TRACK_NFCTYPE4A:
				if(memcmp (LastUIDFound, ISO14443A_Card.UID, ISO14443A_Card.UIDsize))
				{	
					memcpy(LastUIDFound,ISO14443A_Card.UID,ISO14443A_Card.UIDsize);	
//					LCD_DisplayStringLine(Line2, (u8*)"Writing TT4A...      ");
					printf("Writing TT4A...\r\n");
					LED_On(LED4);
					NewTagDetected = true;
				}
				else
					goto End;
				break;	
					
			case TRACK_NFCTYPE4B:
				if(memcmp (LastUIDFound, ISO14443B_Card.PUPI, 4))
				{	
					memcpy(LastUIDFound,ISO14443B_Card.PUPI, 4);
//					LCD_DisplayStringLine(Line2, (u8*)"Writing TT4B...      ");
					printf("Writing TT4B...\r\n");
					LED_On(LED5);
					NewTagDetected = true;
		    }
				else
					goto End;
				break;	
				
			case TRACK_NFCTYPE5:
				if(memcmp (LastUIDFound, TagUID, 8))
				{	
					memcpy(LastUIDFound,TagUID,8);	
//					LCD_DisplayStringLine(Line2, (u8*)"Writing TT5...       ");
					printf("Writing TT5...\r\n");
					LED_On(LED2);
					LED_On(LED5);
					NewTagDetected = true;
				}
				else
					goto End;
				break;
				
				default:
					LED_AllOff();
					break;
		}
				
		/* We write the chosen message to the tag */
		if( NewTagDetected == true)
		{
//			LCD_ClearLine(Line3);
//			LCD_ClearLine(Line4);
//			LCD_ClearLine(Line5);	
//			LCD_ClearLine(Line6);
//			LCD_ClearLine(Line7);
			//printf("tag detcted\r\n");
			NewTagDetected = false;
			strcpy(url.protocol,"http://");
			strcpy(url.URI_Message,"st.com");
			strcpy(url.Information,"654");
			//if (writeChoice == URI)
			status = NDEF_WriteURI(&url);
//			else if (writeChoice == SMS)
//				status = NDEF_WriteSMS(&sms);
//			else if (writeChoice == EMAIL)
//				status = NDEF_WriteEmail(&email);
//			else if (writeChoice == VCARD)
//				status = NDEF_WriteNDEF((uint8_t*)uVcardCSL1);
//			else if (writeChoice == GEO)
//				status = NDEF_WriteGeo(&geo);
//			else if (writeChoice == TEXT)
//				status = NDEF_WriteText(message);
			
			/* Check if an error occured */
			switch (status)
			{
				case PCDNFC_OK:
//					LCD_SetTextColor(Green);
//					LCD_DisplayChar(Line2, 16, 'D');
//					LCD_DisplayChar(Line2, 17, 'o');
//					LCD_DisplayChar(Line2, 18, 'n');
//					LCD_DisplayChar(Line2, 19, 'e');
//				
//					LCD_ClearLine(Line3);
//					LCD_ClearLine(Line4);
//					LCD_ClearLine(Line5);	
//					LCD_ClearLine(Line6);
//					LCD_ClearLine(Line7);
				printf("Done\r\n");
					
//					if (writeChoice == URI)
//						LCD_DisplayStringLine(Line6, (u8*)" URL message stored ");
//					else if (writeChoice == SMS)
//						LCD_DisplayStringLine(Line6, (u8*)" SMS message stored ");
//					else if (writeChoice == EMAIL)
//						LCD_DisplayStringLine(Line6, (u8*)"EMAIL message stored");
//					else if (writeChoice == VCARD)
//						LCD_DisplayStringLine(Line6, (u8*)"VCARD message stored");
//					else if (writeChoice == GEO)
//						LCD_DisplayStringLine(Line6, (u8*)" GEO message stored ");
//					else if (writeChoice == TEXT)
//						LCD_DisplayStringLine(Line6, (u8*)"TEXT message stored ");

				printf("message stored\r\n");
					break;
				case PCDNFC_ERROR_MEMORY_INTERNAL:
					LCD_SetTextColor(Red);
//					LCD_DisplayStringLine(Line2, (u8*)"Memory space in this");
//					LCD_DisplayStringLine(Line3, (u8*)" demonstration has  ");
//					LCD_DisplayStringLine(Line4, (u8*)"  been limited for  ");
//					LCD_DisplayStringLine(Line5, (u8*)"  this type of tag  ");
//					LCD_DisplayStringLine(Line6, (u8*)"  Please select a   ");
//					LCD_DisplayStringLine(Line7, (u8*)"  smaller content   ");
				printf("Please select a smaller content\r\n");
					break;
				case PCDNFC_ERROR_MEMORY_TAG:
//					LCD_SetTextColor(Red);
//					LCD_ClearLine(Line2);
//					LCD_DisplayStringLine(Line3, (u8*)" Not enough memory  ");
//					LCD_DisplayStringLine(Line4, (u8*)"   inside the tag   ");
//					LCD_ClearLine(Line5);	
//					LCD_ClearLine(Line6);
//					LCD_ClearLine(Line7);
				printf("Not enough memory inside the tag\r\n");
				
					break;
				case PCDNFC_ERROR_LOCKED:
//					LCD_SetTextColor(Red);
//				  LCD_ClearLine(Line2);
//					LCD_DisplayStringLine(Line3, (u8*)" The TAG is locked  ");
//					LCD_ClearLine(Line4);
//					LCD_ClearLine(Line5);
//					LCD_ClearLine(Line6);
//					LCD_ClearLine(Line7);
				printf("The tag is locked\r\n");
				
					break;
				default:
//					LCD_SetTextColor(Red);
//					LCD_ClearLine(Line2);
//					LCD_DisplayStringLine(Line3, (u8*)" Transmission Error ");
//					LCD_ClearLine(Line4);
//					LCD_ClearLine(Line5);
//					LCD_ClearLine(Line6);
//					LCD_ClearLine(Line7);
				
				printf("Transmission Error\r\n");
					memcpy(LastUIDFound,EmptyString,20); /* if Transmission error allow retry */
					break;
			}
		}
End:
	NewTagDetected = false;					
	}
		
	PCD_FieldOff();				

//	LCD_Clear(White);
//  DisplayMenu();
//  IntExtOnOffConfig(ENABLE);
}
