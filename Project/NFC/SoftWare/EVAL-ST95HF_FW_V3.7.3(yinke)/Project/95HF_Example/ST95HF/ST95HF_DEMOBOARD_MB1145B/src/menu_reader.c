/**
  ******************************************************************************
  * @file    menu_reader.c
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
#include "menu_reader.h"

extern uint8_t 					TagUID[10];
extern ISO14443A_CARD 	ISO14443A_Card;
extern ISO14443B_CARD 	ISO14443B_Card;
extern FELICA_CARD 			FELICA_Card;
extern uint8_t 					NDEF_Buffer [];
extern volatile bool 		KEYPress;
extern bool							lockKEYUpDown;

extern sURI_Info 				url;
extern sSMSInfo 				sms;
extern sEmailInfo 			email;
extern sGeoInfo 				geo;

/* Variables for the different modes */
extern DeviceMode_t 				devicemode;
extern TagType_t 			nfc_tagtype;

/**
  * @brief  This function looks for a tag and write the UID on the screen
  */
void TagHunting(void)
{
	uint8_t currentLine = Line0;
	uint8_t TagType = TRACK_NOTHING, tagfounds=TRACK_ALL;
	bool FirstTagFounded = true;
	
	u8 i;
	char UID[20] = {' '};
	char InvertedUID[20] = {' '};
  char LastUIDFound[20] = {' '};
	char EmptyString[20] = "                    ";
	
//	LCD_Clear(Black);

	IntRotateOnOffConfig(DISABLE);

//  /* Set the Back Color */
//  LCD_SetBackColor(Blue);
//  /* Set the Text Color */
//  LCD_SetTextColor(White);
//  
//  LCD_DisplayStringLine(Line0, (u8*)"  Tag Hunting mode  ");
//	LCD_DisplayStringLine(Line9, (u8*)"   Select to exit   ");

	/* Set the Back Color */
//  LCD_SetBackColor(Black);
//	LCD_DisplayStringLine(Line3, (u8*)" Bring tag close to ");
//	LCD_DisplayStringLine(Line4, (u8*)"the antenna to start");
//	LCD_DisplayStringLine(Line5, (u8*)"   UID detection    ");
	
//	 /* Set the Back Color */
//  LCD_SetBackColor(Black);
//  /* Set the Text Color */
//  LCD_SetTextColor(White);
	
	/* External Interrupt Enable */
	IntExtOnOffConfig(ENABLE);
	/* Prepare to exit loop on Key Press */
	KEYPress = false;
	lockKEYUpDown = true;
	
	/* initialize array */
	memcpy(LastUIDFound, EmptyString,20);
	
	/* wait user action to go back to previous screen */
	//while(!KEYPress)
	{		
		/* Scan to find if there is a tag */
		memcpy(UID, EmptyString,20);
		memcpy(InvertedUID, EmptyString,20);
		TagType = ConfigManager_TagHunting(tagfounds);
		
		/* Tag has been discovered, clear screen */
		if( TagType != TRACK_NOTHING && FirstTagFounded == true)
		{
//			LCD_DisplayStringLine(Line3, (u8*)"                    ");
//			LCD_DisplayStringLine(Line4, (u8*)"                    ");
//			LCD_DisplayStringLine(Line5, (u8*)"                    ");
			FirstTagFounded = false;
		}			
		
		/* Check the tag type found */
 		if(TagType&TRACK_NFCTYPE1)
		{
			LED_On(LED1);
			Hex2Char( TagUID, 6, UID);
	
			for(i=0; i<12; i=i+2)
			{
				InvertedUID[11-i] = UID[i+1];
				InvertedUID[11-(i+1)] = UID[i];
			}
			memcpy(&InvertedUID[17],"TT1",3);
			/* Set the Text Color */
//			LCD_SetTextColor(Green);
			if(memcmp (LastUIDFound, TagUID, 6))
			{	
				memcpy(LastUIDFound,TagUID,6);
				if (currentLine++ > Line7) currentLine = Line1;
//				LCD_DisplayStringLine(currentLine, (u8*)InvertedUID);				
			}
			else
			{
//				LCD_ClearLine(currentLine);
//				LCD_DisplayStringLine(currentLine, (u8*)InvertedUID);	
			}
		}
		else if (TagType&TRACK_NFCTYPE2)
		{	
			LED_On(LED2);
			Hex2Char( ISO14443A_Card.UID, ISO14443A_Card.UIDsize, UID);
			
			memcpy(&UID[17],"TT2",3);
			/* Set the Text Color */
//			LCD_SetTextColor(Yellow);		
			if(memcmp (LastUIDFound, ISO14443A_Card.UID, ISO14443A_Card.UIDsize))
			{	
				memcpy(LastUIDFound,ISO14443A_Card.UID,ISO14443A_Card.UIDsize);	
				if (currentLine++ > Line7) currentLine = Line1;
//				LCD_DisplayStringLine(currentLine, (u8*)UID);	
			}
			else
			{
//				LCD_ClearLine(currentLine);
//				LCD_DisplayStringLine(currentLine, (u8*)UID);	
			}
		}
		else if (TagType&TRACK_NFCTYPE3)
		{
			LED_On(LED3);
			Hex2Char( FELICA_Card.UID, 8 , UID);
			
			memcpy(&UID[17],"TT3",3);
			/* Set the Text Color */
//			LCD_SetTextColor(Orange);		
			if(memcmp (LastUIDFound, FELICA_Card.UID, 8))
			{	
				memcpy(LastUIDFound,FELICA_Card.UID,8);
				if (currentLine++ > Line7) currentLine = Line1;
//				LCD_DisplayStringLine(currentLine, (u8*)UID);				
			}
			else
			{
//				LCD_ClearLine(currentLine);
//				LCD_DisplayStringLine(currentLine, (u8*)UID);	
			}
		}
		else if (TagType&TRACK_NFCTYPE4A)
		{	
			LED_On(LED4);
			Hex2Char( ISO14443A_Card.UID, ISO14443A_Card.UIDsize, UID);
			
			memcpy(&UID[16],"TT4A",4);
			/* Set the Text Color */
//			LCD_SetTextColor(Red);			
			if(memcmp (LastUIDFound, ISO14443A_Card.UID, ISO14443A_Card.UIDsize))
			{	
				memcpy(LastUIDFound,ISO14443A_Card.UID,ISO14443A_Card.UIDsize);	
				if (currentLine++ > Line7) currentLine = Line1;
//				LCD_DisplayStringLine(currentLine, (u8*)UID);
			}
			else
			{
//				LCD_ClearLine(currentLine);
//				LCD_DisplayStringLine(currentLine, (u8*)UID);	
			}
		}
		else if (TagType&TRACK_NFCTYPE4B)
		{	
			LED_On(LED5);
			Hex2Char( ISO14443B_Card.PUPI, 4 , UID);
		
			memcpy(&UID[16],"TT4B",4);
			/* Set the Text Color */
//			LCD_SetTextColor(Blue);			
			if(memcmp (LastUIDFound, ISO14443B_Card.PUPI, 4))
			{	
				memcpy(LastUIDFound,ISO14443B_Card.PUPI,4);		
				if (currentLine++ > Line7) currentLine = Line1;
//				LCD_DisplayStringLine(currentLine, (u8*)UID);
			}
			else
			{
//				LCD_ClearLine(currentLine);
//				LCD_DisplayStringLine(currentLine, (u8*)UID);	
			}
		}
		else if (TagType&TRACK_NFCTYPE5)
		{
			Hex2Char( TagUID, 8, UID);
			
			for(i=0; i<16; i=i+2)
			{
				InvertedUID[15-i] = UID[i+1];
				InvertedUID[15-(i+1)] = UID[i];
			}
			memcpy(&InvertedUID[17],"TT5",3);
			/* Set the Text Color */
//			LCD_SetTextColor(White);
			if(memcmp (LastUIDFound, TagUID, 8))
			{	
				memcpy(LastUIDFound,TagUID,8);		
				if (currentLine++ > Line7) currentLine = Line1;
//				LCD_DisplayStringLine(currentLine, (u8*)InvertedUID);
			}
			else
			{
//				LCD_ClearLine(currentLine);
//				LCD_DisplayStringLine(currentLine, (u8*)InvertedUID);	
			}
		}
		else /* No supported tags found */
			LED_AllOff();
		
		PCD_FieldOff();
	}
	
//	LCD_Clear(White);
//  DisplayMenu();
}

/**
  * @brief  This function allow to sleep until a tag is detected or Timeout occurs
  */
void TagDetectWakeUp(void)
{
	uint8_t DacDataH,WuSource;
	
	 /* Set the Back Color */
  LCD_SetBackColor(Blue);
  /* Set the Text Color */
  LCD_SetTextColor(White);
  
  LCD_DisplayStringLine(Line0, (u8*)" Tag Detect WU mode ");

	/* Set the Back Color */
  LCD_SetBackColor(White);
	/* Set the Text Color */
  LCD_SetTextColor(Blue);
	
	LCD_ClearLine(1);
	LCD_DisplayStringLine(Line2, (u8*)"Calibration Process ");
	LCD_ClearLine(3);
	LCD_DisplayStringLine(Line4, (u8*)"Remove any tag close");
	LCD_DisplayStringLine(Line5, (u8*)"to antenna and press");
	LCD_DisplayStringLine(Line6, (u8*)"any joystick button ");
	LCD_ClearLine(7);
	LCD_ClearLine(8);
	LCD_ClearLine(9);
	
	/* External Interrupt Enable */
	IntExtOnOffConfig(ENABLE);
	/* Prepare to exit loop on Key Press */
	KEYPress = false;
	
	while(!KEYPress)
	{		
	}
	
	/* Launch calibration */
	if( PCD_TagDetectCalibration( 0x00, &DacDataH) != PCD_SUCCESSCODE)
	{	
		LCD_DisplayStringLine(Line2, (u8*)" Calibration Failed ");
		LCD_ClearLine(3);
		LCD_ClearLine(4);
		LCD_ClearLine(5);
		LCD_ClearLine(6);
		LCD_ClearLine(7);
		LCD_ClearLine(8);
		
		KEYPress = false;
	
		while(!KEYPress)
		{		
		}
		LCD_Clear(White);
		DisplayMenu();
		
		return;
	}
	else
	{
		LCD_DisplayStringLine(Line2, (u8*)"  Calibration Done  ");
		LCD_ClearLine(3);
		LCD_DisplayStringLine(Line4, (u8*)"Enter Low Power mode");
		LCD_DisplayStringLine(Line5, (u8*)"    by pressing     ");
		LCD_DisplayStringLine(Line6, (u8*)"any joystick button ");
		LCD_DisplayStringLine(Line7, (u8*)"Then bring a tag to ");
		LCD_DisplayStringLine(Line8, (u8*)"   wake up ST95HF   ");
	}
	
	KEYPress = false;
	
	while(!KEYPress)
	{		
	}
	LCD_ClearLine(1);
	LCD_ClearLine(2);
	LCD_DisplayStringLine(Line3, (u8*)" ST95HF is in idle  ");
	LCD_DisplayStringLine(Line4, (u8*)"mode waiting for tag");
	LCD_DisplayStringLine(Line5, (u8*)"     to wake up     ");
	LCD_ClearLine(6);
	LCD_ClearLine(7);
	LCD_ClearLine(8);
	
	/* Set the Back Color */
  LCD_SetBackColor(Blue);
  /* Set the Text Color */
  LCD_SetTextColor(White);
	LCD_DisplayStringLine(Line9, (u8*)"   Select to exit   ");
	/* Set the Back Color */
  LCD_SetBackColor(White);
  /* Set the Text Color */
  LCD_SetTextColor(Blue);
	
	WuSource = 0x0B;
	KEYPress = false;
	while( (WuSource != 0x02) && (!KEYPress) )
	{
		WuSource = 0x0B;
		PCD_WaitforTagDetection (&WuSource, 0x20, DacDataH, 0x1E);
	}	
	
	if( WuSource == 0x01)
	{	
		LCD_DisplayStringLine(Line2, (u8*)"   Wake-up due to   ");
		LCD_DisplayStringLine(Line3, (u8*)"    user action     ");
	}
	else
	{	
		LCD_DisplayStringLine(Line2, (u8*)"   Wake-up due to   ");
		LCD_DisplayStringLine(Line3, (u8*)"   tag detection    ");
	}	
	
	LCD_ClearLine(4);
	LCD_ClearLine(5);
	LCD_ClearLine(6);
	LCD_ClearLine(7);
	LCD_ClearLine(8);
	
	
	KEYPress = false;	
	while(!KEYPress)
	{		
	}
	LCD_Clear(White);
	DisplayMenu();
		
	
}
/**
  * @brief  This function looks for a tag and write the UID on the screen
  */
void TagReading(void)
{
	uint8_t status;
	int8_t TagType = TRACK_NOTHING;
	bool FirstTagFounded = true;
  char LastUIDFound[20] = {' '};
	bool NewTagDetected = false;

//	LCD_Clear(Black);

//  /* Set the Back Color */
//  LCD_SetBackColor(Blue);
//  /* Set the Text Color */
//  LCD_SetTextColor(White);
//  
//  LCD_DisplayStringLine(Line0, (u8*)"  Tag reader mode   ");
//  LCD_DisplayStringLine(Line9, (u8*)"   Select to exit   ");
//	
//	 /* Set the Back Color */
//  LCD_SetBackColor(Black);
//  /* Set the Text Color */
//  LCD_SetTextColor(White);
//	
//	LCD_DisplayStringLine(Line2, (u8*)" CR95HF is waiting  ");
//  LCD_DisplayStringLine(Line3, (u8*)"for a tag with NDEF ");
//	LCD_DisplayStringLine(Line4, (u8*)"   content inside   ");
	
//	/* External Interrupt Enable */
//	IntExtOnOffConfig(ENABLE);
//	IntRotateOnOffConfig(DISABLE);
//	/* Prepare to exit loop on Key Press */
//	KEYPress = false;
//	lockKEYUpDown = true;
	/* wait user action to go back to previous screen */
//	while(!KEYPress)
	//while(1)
	{
		/* Scan to find if there is a tag */
		TagType = ConfigManager_TagHunting(TRACK_ALL);	
		
		/* Tag has been discovered, clear screen */
//		if( TagType != TRACK_NOTHING && FirstTagFounded == true)
//		{
////			LCD_DisplayStringLine(Line2, (u8*)"                    ");
////			LCD_DisplayStringLine(Line3, (u8*)"                    ");
////			LCD_DisplayStringLine(Line4, (u8*)"                    ");
//			FirstTagFounded = false;
//		}		
		switch(TagType)
		{
			case TRACK_NFCTYPE1:
				if(memcmp (LastUIDFound, TagUID, 6))
				{	
					memcpy(LastUIDFound,TagUID,6);	
//					LED_On(LED1);
//					LCD_DisplayStringLine(Line2, (u8*)"Reading TT1...      ");
					NewTagDetected = true;
				}
				else
					goto End;
				break;
				
			case TRACK_NFCTYPE2:
				if(memcmp (LastUIDFound, ISO14443A_Card.UID, ISO14443A_Card.UIDsize))
				{	
					memcpy(LastUIDFound,ISO14443A_Card.UID,ISO14443A_Card.UIDsize);	
//					LED_On(LED2);
//					LCD_DisplayStringLine(Line2, (u8*)"Reading TT2...      ");
					printf("³É¹¦£¡\r\n");
					NewTagDetected = true;
				}
				else
					goto End;
				break;	
				
			case TRACK_NFCTYPE3:
				if(memcmp (LastUIDFound, FELICA_Card.UID, 8))
				{	
					memcpy(LastUIDFound,FELICA_Card.UID,8);
//					LED_On(LED3);
//					LCD_DisplayStringLine(Line2, (u8*)"Reading TT3...      ");
					NewTagDetected = true;
				}
				else
					goto End;
				break;	
				
			case TRACK_NFCTYPE4A:
				if(memcmp (LastUIDFound, ISO14443A_Card.UID, ISO14443A_Card.UIDsize))
				{	
					memcpy(LastUIDFound,ISO14443A_Card.UID,ISO14443A_Card.UIDsize);	
//					LED_On(LED4);
//					LCD_DisplayStringLine(Line2, (u8*)"Reading TT4A...     ");
					NewTagDetected = true;
				}
				else
					goto End;
				break;	
				
			case TRACK_NFCTYPE4B:
				if(memcmp (LastUIDFound, ISO14443B_Card.PUPI, 4))
				{	
					memcpy(LastUIDFound,ISO14443B_Card.PUPI, 4);
//					LED_On(LED5);
//					LCD_DisplayStringLine(Line2, (u8*)"Reading TT4B...     ");		
					NewTagDetected = true;					
				}
				else
					goto End;
				break;	
				
			case TRACK_NFCTYPE5:
				if(memcmp (LastUIDFound, TagUID, 8))
				{	
					memcpy(LastUIDFound,TagUID, 8);	
//					LED_On(LED2);
//					LED_On(LED5);
//					LCD_DisplayStringLine(Line2, (u8*)"Reading TT5...      ");
					NewTagDetected = true;
				}
				else
					goto End;
				break;	
				
				default:
					LED_AllOff();
					break;
		}
		
		/* Check the tag type found */
		if (NewTagDetected == true)
		{			
			uint8_t i;
			LED_Troggle(LED1);
			//Hex2Char((u8 *)LastUIDFound,20,uidstring);
//			printf("Tag detected: TagType [%02X], UID [",TagType);
//			for(i = 0;i < 20; i ++)
//			{
//				printf("%02X ",LastUIDFound[i]);
//			}
//			printf("]\r\n");
			
			NewTagDetected = false;
			/* Try to read the NDEF message inside the tag */
			//TagType = ConfigManager_TagHunting(TRACK_NFCTYPE2);
			//ISO14443A_Init( );
			status = readNDEFfromTAG();
			//status = PCDNFCT2_ReadNDEF();
			//status = PCDNFCT2_Read();
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
				printf("Empty Tag\r\n");
			}
			else if (status != PCDNFC_OK)
			{
//				LCD_ClearLine(Line3);LCD_ClearLine(Line4);LCD_ClearLine(Line5);LCD_ClearLine(Line6);LCD_ClearLine(Line7);
//				LCD_SetTextColor(Red);
//				LCD_DisplayStringLine(Line3, (u8*)"Transmission Error  ");
//				LCD_SetTextColor(White);
				// In case of transmission error clear the LastUID
				printf("Transmission Error\r\n");
				
				memset(LastUIDFound,'\0',20);
			}
		}

End:		
	NewTagDetected = false;	
	}
	
	PCD_FieldOff();
//  DisplayMenu();
}

/**
  * @brief  This function looks for a tag and write the UID on the screen
  */

uint8_t readNDEFfromTAG(void)
{
	uint8_t status;
	sRecordInfo RecordStruct;
	
	/* Init information fields */
	memset(url.Information,'\0',400);
	memset(sms.Information,'\0',400);
	memset(email.Information,'\0',400);
	memset(geo.Information,'\0',100);
	

	if (nfc_tagtype == TT1)
	{
		errchk(PCDNFCT1_ReadNDEF());
	}
	else if (nfc_tagtype == TT2)
	{
		status = PCDNFCT2_ReadNDEF();
		printf("PCDNFCT2_ReadNDEF status [%02X]\r\n",status);
		errchk(status);
		
//		errchk(PCDNFCT2_ReadNDEF());
	}
	else if (nfc_tagtype == TT3)
	{
		errchk(PCDNFCT3_ReadNDEF());
	}
	else if (nfc_tagtype == TT4A || nfc_tagtype == TT4B)
	{
		errchk(PCDNFCT4_ReadNDEF());
	}
	else if (nfc_tagtype == TT5)
	{
		errchk(PCDNFCT5_ReadNDEF());
	}
	else
		return ERRORCODE_GENERIC;
	
//	LCD_SetTextColor(Green);
//	LCD_DisplayChar(Line2, 16, 'D');
//	LCD_DisplayChar(Line2, 17, 'o');
//	LCD_DisplayChar(Line2, 18, 'n');
//	LCD_DisplayChar(Line2, 19, 'e');
	
//	LCD_SetTextColor(White);
	// Avoid printing useless characters
	memset(NDEF_Buffer,'\0',20);
	status = NDEF_IdentifyNDEF( &RecordStruct, NDEF_Buffer);
	if(status == RESULTOK && RecordStruct.TypeLength != 0)
	{
//		LCD_ClearLine(Line3);LCD_ClearLine(Line4);LCD_ClearLine(Line5);LCD_ClearLine(Line6);LCD_ClearLine(Line7);
		if (NDEF_ReadURI(&RecordStruct, &url)==RESULTOK)
		{
//			LCD_SetBackColor(White);LCD_SetTextColor(Black);
//			LCD_DisplayStringLine(Line3, (u8*)"    URI content:    ");
//			LCD_SetBackColor(Black);LCD_SetTextColor(White);
//			LCD_DisplayStringLine(Line4, (u8*)url.Information);
//			LCD_DisplayStringLine(Line5, (u8*)url.protocol);	
//			LCD_DisplayStringLine(Line6, (u8*)url.URI_Message);
			printf("URI \r\n");
		}
		else if (NDEF_ReadSMS(&RecordStruct, &sms)==RESULTOK)
		{
//			LCD_SetBackColor(White);LCD_SetTextColor(Black);
//			LCD_DisplayStringLine(Line3, (u8*)"    SMS content:    ");
//			LCD_SetBackColor(Black);LCD_SetTextColor(White);
//			LCD_DisplayStringLine(Line4, (u8*)sms.Information);
//			LCD_DisplayStringLine(Line5, (u8*)sms.PhoneNumber);	
//			LCD_DisplayStringLine(Line6, (u8*)sms.Message);
			printf("SMS \r\n");
		}
		else if(NDEF_ReadEmail(&RecordStruct, &email)==RESULTOK)
		{
//			LCD_SetBackColor(White);LCD_SetTextColor(Black);
//			LCD_DisplayStringLine(Line3, (u8*)"   EMAIL content:   ");
//			LCD_SetBackColor(Black);LCD_SetTextColor(White);
//			LCD_DisplayStringLine(Line4, (u8*)email.Information);
//			LCD_DisplayStringLine(Line5, (u8*)email.EmailAdd);
//			LCD_DisplayStringLine(Line6, (u8*)email.Subject);	
//			LCD_DisplayStringLine(Line7, (u8*)email.Message);
			printf("EMAIL \r\n");
		}
		else if(NDEF_ReadGeo(&RecordStruct, &geo)==RESULTOK)
		{
//			LCD_SetBackColor(White);LCD_SetTextColor(Black);
//			LCD_DisplayStringLine(Line3, (u8*)"    GEO content:    ");
//			LCD_SetBackColor(Black);LCD_SetTextColor(White);
//			LCD_DisplayStringLine(Line4, (u8*)geo.Information);
//			LCD_DisplayStringLine(Line5, (u8*)geo.Latitude);	
//			LCD_DisplayStringLine(Line6, (u8*)geo.Longitude);
			printf("GEO \r\n");
		}
		// This part has to be improved, it is just in order to write a simple text NDEF for the M24LR discovery
		else if(RecordStruct.NDEF_Type == TEXT_TYPE)
		{
//			LCD_SetBackColor(White);LCD_SetTextColor(Black);
//			LCD_DisplayStringLine(Line3, (u8*)"   TEXT content:    ");
//			LCD_SetBackColor(Black);LCD_SetTextColor(White);
//			LCD_DisplayStringLine(Line4, &NDEF_Buffer[3]);
			printf("TEXT \r\n");
		}
		else if(RecordStruct.NDEF_Type == VCARD_TYPE)
		{
//			LCD_SetBackColor(White);LCD_SetTextColor(Black);
//			LCD_DisplayStringLine(Line3, (u8*)"   VCARD detected   ");
//			LCD_SetBackColor(Black);LCD_SetTextColor(White);
			printf("VCARD \r\n");
		}
		else
		{
//			LCD_DisplayStringLine(Line3, (u8*)" Unknown NDEF type  ");
			printf("UNKOWN\r\n");
		}		
//		LCD_SetBackColor(Black);
	}
	else if (RecordStruct.TypeLength == 0)
	{
//		LCD_ClearLine(Line3);LCD_ClearLine(Line4);LCD_ClearLine(Line5);LCD_ClearLine(Line6);LCD_ClearLine(Line7);
//		LCD_DisplayStringLine(Line3, (u8*)"  No NDEF content   ");
		printf("No DNEF content\r\n");
	}
	else
	{
//		LCD_ClearLine(Line3);LCD_ClearLine(Line4);LCD_ClearLine(Line5);LCD_ClearLine(Line6);LCD_ClearLine(Line7);
//		LCD_DisplayStringLine(Line3, (u8*)"Error parsing NDEF  ");
		printf("Error parsing NDEF \r\n");
	}

	return RESULTOK;
Error:
	
	printf("readNDEFfromTAG Error\r\n");
	return status;
}
