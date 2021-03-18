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

#include "menu_tagemul.h"

extern const char uVcardCSL1[];

enum{WRITE_GEO,WRITE_URI,WRITE_SMS,WRITE_EMAIL,WRITE_VCARD,WRITE_NOTHING};
extern sURI_Info 		url;
extern sSMSInfo 		sms;
extern sEmailInfo 	email;
extern sGeoInfo 		geo;

extern bool 				KEYPress;
extern bool 				lockKEYUpDown;

extern DeviceMode_t 		devicemode;
extern TagType_t 	nfc_tagtype;

uint8_t contentToWrite = WRITE_NOTHING;


/**
  * @brief  Configures tag emulation for type 4A tag.
  */
void TagEmulT4AGEO(void)
{
	LED_On(LED4);
	/* Initilialize tag emulation mode */
	devicemode = PICC;
	nfc_tagtype = TT4A;
	
	contentToWrite = WRITE_GEO;
	FillTagGEO("GEO TT4A example");
	TagEmul();
	
	LED_Off(LED4);
}
/**
  * @brief  Configures tag emulation for type 4A tag.
  */
void TagEmulT4AURI(void)
{
	LED_On(LED4);
	/* Initilialize tag emulation mode */
	devicemode = PICC;
	nfc_tagtype = TT4A;
	
	contentToWrite = WRITE_URI;
	FillTagURI("URI TT4A example");
	TagEmul();
	
	LED_Off(LED4);
}

/**
  * @brief  Configures tag emulation for type 4A tag.
  */
void TagEmulT4ASMS(void)
{
	LED_On(LED4);
	/* Initilialize tag emulation mode */
	devicemode = PICC;
	nfc_tagtype = TT4A;
	
	contentToWrite = WRITE_SMS;
	FillTagSMS("SMS TT4A example");
	TagEmul();

	LED_Off(LED4);
}

/**
  * @brief  Configures tag emulation for type 4A tag.
  */
void TagEmulT4AEMAIL(void)
{
	LED_On(LED4);
	/* Initilialize tag emulation mode */
	devicemode = PICC;
	nfc_tagtype = TT4A;
	
	contentToWrite = WRITE_EMAIL;
	FillTagEMAIL("EMAIL TT4A example");
	
	TagEmul();

	LED_Off(LED4);
}

/**
  * @brief  Configures tag emulation for type 4A tag.
  */
void TagEmulT4AVCARD(void)
{
	LED_On(LED4);
	/* Initilialize tag emulation mode */
	devicemode = PICC;
	nfc_tagtype = TT4A;
	
	contentToWrite = WRITE_VCARD;
	TagEmul();
	
	LED_Off(LED4);
}

/**
  * @brief  Configures tag emulation for type 4A tag.
  */
void TagEmulT4A(void)
{
	LED_On(LED4);
	/* Initilialize tag emulation mode */
	devicemode = PICC;
	nfc_tagtype = TT4A;
	
	contentToWrite = WRITE_NOTHING;
	TagEmul();

	LED_Off(LED4);
}


/**
  * @brief  Configures tag emulation for type 2 tag.
  */
void TagEmulT2GEO(void)
{
	LED_On(LED2);
	/* Initilialize tag emulation mode */
	devicemode = PICC;
	nfc_tagtype = TT2;
	
	contentToWrite = WRITE_GEO;
	FillTagGEO("GEO TT2 example");
	TagEmul();

	LED_Off(LED2);
}
/**
  * @brief  Configures tag emulation for type 2 tag.
  */
void TagEmulT2URI(void)
{
	LED_On(LED2);
	/* Initilialize tag emulation mode */
	devicemode = PICC;
	nfc_tagtype = TT2;
	
	contentToWrite = WRITE_URI;
	FillTagURI("URI TT2 example");
	TagEmul();

	LED_Off(LED2);
}
/**
  * @brief  Configures tag emulation for type 2 tag.
  */
void TagEmulT2SMS(void)
{
	LED_On(LED2);
	/* Initilialize tag emulation mode */
	devicemode = PICC;
	nfc_tagtype = TT2;
	
	contentToWrite = WRITE_SMS;
	FillTagSMS("SMS TT2 example");
	TagEmul();

	LED_Off(LED2);
}

/**
  * @brief  Configured tag emulation for type 2 tag.
  */
void TagEmulT2(void)
{
//	LED_On(LED2);
	/* Initilialize tag emulation mode */
	devicemode = PICC;
	nfc_tagtype = TT2;
	
	contentToWrite = WRITE_NOTHING;
	TagEmul();

//	LED_Off(LED2);
}

/**
  * @brief  Runs the tag emulation with the selected tag type.
  */
void TagEmul(void)
{
	initFlashNDEF();

	
	if (contentToWrite == WRITE_GEO)
	{
		NDEF_WriteGeo(&geo);
		//LCD_DisplayStringLine(Line3, (u8*)"   GEO configured   ");
	}
	else if (contentToWrite == WRITE_URI)
	{
		NDEF_WriteURI(&url);
		//LCD_DisplayStringLine(Line3, (u8*)"   URL configured   ");
	}
	else if (contentToWrite == WRITE_SMS)
	{
		NDEF_WriteSMS(&sms);
		//LCD_DisplayStringLine(Line3, (u8*)"   SMS configured   ");
	}
	else if (contentToWrite == WRITE_EMAIL)
	{
		NDEF_WriteEmail(&email);
		//LCD_DisplayStringLine(Line3, (u8*)"   EMAIL configured  ");
	}
	else if (contentToWrite == WRITE_VCARD)
	{
		NDEF_WriteNDEF((uint8_t*)uVcardCSL1);
		//LCD_DisplayStringLine(Line3, (u8*)"   VCARD configured  ");
	}
	
	/* Prepare to exit loop on Key Press */
	lockKEYUpDown = true;
	KEYPress = false;
	//LED_Off(LED1);

	/* Start the configuration manager in infinite tag emulation (delay=0)*/
	if( nfc_tagtype == TT2)
	{
		ConfigManager_TagEmulation(PICCEMULATOR_TAG_TYPE_2,0,0);
		printf("nfc_tage TT2\r\n");
	}
	else if( nfc_tagtype == TT4A)
	{
		ConfigManager_TagEmulation(PICCEMULATOR_TAG_TYPE_4A,0,0);
		printf("nfc_tage TT4\r\n");
	}
	manageFlashNDEF();
	
	/* External Interrupt Disable */
	//IntExtOnOffConfig(DISABLE);			
	/* Display Menu */
  //DisplayMenu();
}






