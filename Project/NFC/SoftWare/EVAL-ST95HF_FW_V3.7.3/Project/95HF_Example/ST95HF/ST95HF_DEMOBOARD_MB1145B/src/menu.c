/**
  ******************************************************************************
  * @file    menu.c 
  * @author  MMY Application Team
  * @version V1.0.0
  * @date    08/21/2013
  * @brief   Menu navigation driver for EVAL-ST95HF demonstration.
 ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MMY-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "menu.h"
#include "usb_hid.h"

/** @addtogroup User_Appli
 * 	@{
 *  @brief      <b>This folder contains the application files</b> 
 */
 
/** @addtogroup Menu
 * 	@{
 *  @brief      This file contains the menu navigation driver for EVAL-ST95HF. 
 */


/* As there is lot's of picture the size of the code increase a lot */
/* and the time to load the application is longer... */
/* to speed up things you can define the DEBUG flag to embed less picture */
//#define DEBUG

/* bmp to be used by this application */
#ifndef DEBUG

/* logo for introduction */
#include "stlogo.h"

/* icon file for menu */
#include "tagreader_icon.h"
#include "tagwritter_icon.h"
#include "tagemul_icon.h"
#include "p2p_icon.h"
#include "cardemul_icon.h"
#include "automode_icon.h"
#include "background.h"

#else

/* icons */
#include "tagreader_icon.h"

#endif

volatile bool 				KEYPress 						= false;
volatile uint8_t 			KEYPressed					= NOKEY;
bool 									lockKEYUpDown 			= false;
bool									rotate							= false;
volatile bool					refreshMenu					= false;

extern bool 					USB_Control_Allowed;
extern int8_t					HIDTransaction;
extern bool						screenRotated;
extern bool           disableRotate;

// Structures used for writter/reader/emulation
sURI_Info 	url;
sSMSInfo 		sms;
sEmailInfo 	email;
sGeoInfo 		geo;

/* Private typedef -----------------------------------------------------------*/
typedef void (* tMenuFunc)(void);
typedef struct sMenuItem * tMenuItem;
typedef struct sMenu * tMenu;

/* Private function prototypes -----------------------------------------------*/

/* Private define ------------------------------------------------------------*/

#define NumberOfIcons 5
#define NbIconeByLine 2
#define NbIconeByColumn 3

/* icon screen are displayed like this */
/* icon are 60x60 bmp */

/********************************/
/*															*/
/*															*/
/*															*/
/*  =====  =====  =====  =====  */
/*  = 1 =  = 2 =  = 3 =  = 4 =  */
/*  =====  =====  =====  =====  */
/*															*/
/*  =====  =====  =====  =====  */
/*  = 5 =  = 6 =  = 7 =  = 8 =  */
/*  =====  =====  =====  =====  */
/*															*/
/*  =====  =====  =====  =====  */
/*  = 9 =  = 10=  = 11=  = 12=  */
/*  =====  =====  =====  =====  */
/*															*/
/********************************/

/* put icon address in this array, null if nothing to display */
/* put menu position that match with the icon */
#ifndef DEBUG
const unsigned char * IconsAddr[NumberOfIcons] = {tagreader_icon, tagwritter_icon, tagemul_icon, cardemul_icon, p2p_icon}; 
#else
const unsigned char * IconsAddr[NumberOfIcons] = {tagreader_icon, tagreader_icon, tagreader_icon, tagreader_icon,tagreader_icon, tagreader_icon}; 
#endif
															
uint16_t IconRect[NumberOfIcons][2] ={{149, 29}, {239, 29}, {149, 101}, {239, 101},{149, 172}};
													 
//uint16_t IconRect[12][2] ={{31, 33}, {96, 33}, {161, 33}, {226, 33},
// 													 {31, 98}, {96, 98}, {161, 98}, {226, 98},
// 													 {31, 163}, {96, 163}, {161, 163}, {226, 163}};

const char ASCII [16][2] = { {0x30,0x00}, {0x31,0x01}, {0x32,0x02}, {0x33,0x03}, {0x34,0x04},  
														 {0x35,0x05}, {0x36,0x06}, {0x37,0x07}, {0x38,0x08}, {0x39,0x09}, 
														 {0x61,0x0A}, {0x62,0x0B}, {0x63,0x0C}, {0x64,0x0D}, {0x65,0x0E},
														 {0x66,0x0F}, 
													 };

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

bool MenuActivated = true;
 
uint8_t MenuItemIndex = 0, nMenuLevel = 0;
uint8_t ItemNumb[MAX_MENU_LEVELS];

tMenuItem psMenuItem, psCurrentMenuItem;
tMenu psPrevMenu[MAX_MENU_LEVELS];
tMenu psCurrentMenu;

struct sMenuItem
{
  uint8_t* pszTitle;
  tMenuFunc pfMenuFunc;
  tMenuFunc pfUpDownMenuFunc;
  tMenu psSubMenu;
};

struct sMenu
{
  uint8_t* pszTitle;
  tMenuItem psItems;
  uint8_t nItems;
};

/*------------------------------ Menu level 4 -------------------------------*/

/*------------------------------ Menu level 3 -------------------------------*/
struct sMenuItem TagEmulT4AMenuItems[] = {{(u8*)"    Emulate GEO     ", TagEmulT4AGEO, IdleFunc},
																					{(u8*)"    Emulate URL     ", TagEmulT4AURI, IdleFunc},
																				  {(u8*)"    Emulate SMS     ", TagEmulT4ASMS, IdleFunc},
																				  {(u8*)"   Emulate EMAIL    ", TagEmulT4AEMAIL, IdleFunc},
																					{(u8*)"   Emulate VCARD    ", TagEmulT4AVCARD, IdleFunc},
																				  {(u8*)"  Previous content  ", TagEmulT4A, IdleFunc},
																				  {(u8*)"<-     Return       ", ReturnFunc, IdleFunc}};
struct sMenu TagEmulT4AMenu = {(u8*)"   TT4A Emulator    ", TagEmulT4AMenuItems, countof(TagEmulT4AMenuItems)};


struct sMenuItem TagEmulT2MenuItems[] = {{(u8*)"    Emulate GEO     ", TagEmulT2GEO, IdleFunc},
																				 {(u8*)"    Emulate URL     ", TagEmulT2URI, IdleFunc},
																				 {(u8*)"    Emulate SMS     ", TagEmulT2SMS, IdleFunc},
																				 {(u8*)"  Previous content  ", TagEmulT2, IdleFunc},
																				 {(u8*)"<-     Return       ", ReturnFunc, IdleFunc}};
struct sMenu TagEmulT2Menu = {(u8*)"   TT2 Emulator     ", TagEmulT2MenuItems, countof(TagEmulT2MenuItems)};

/*------------------------------ Menu level 2 -------------------------------*/
struct sMenuItem TagReaderMenuItems[] = {{(u8*)"    TAG Hunting     ", TagHunting, IdleFunc},
																				 {(u8*)" TAG Detect Wake-UP ", TagDetectWakeUp, IdleFunc},
																				 {(u8*)"    TAG Reading     ", TagReading, IdleFunc},
																				 {(u8*)"    PC link mode    ", USBRemoteControl, IdleFunc},
																				 {(u8*)"<-     Return       ", ReturnFunc, IdleFunc}};															 
struct sMenu TagReaderMenu = {(u8*)"   Tag Reader Mode  ", TagReaderMenuItems, countof(TagReaderMenuItems)};

struct sMenuItem TagWriterMenuItems[] = {{(u8*)"Write TEXT        30", TagWriteText, IdleFunc},
																				 {(u8*)"Write GEO         67", TagWriteGEO, IdleFunc},
																				 {(u8*)"Write URI         71", TagWriteURI, IdleFunc},
																				 {(u8*)"Write SMS        156", TagWriteSMS, IdleFunc},
																				 {(u8*)"Write EMAIL     1050",TagWriteEMAIL, IdleFunc},
																				 {(u8*)"Write VCARD     7711", TagWriteVCARD, IdleFunc},
																				 {(u8*)"<-     Return       ", ReturnFunc, IdleFunc}};																 
struct sMenu TagWriterMenu = {(u8*)"   Tag Writer Mode  ", TagWriterMenuItems, countof(TagWriterMenuItems)};

struct sMenuItem TagEmulMenuItems[] = {{(u8*)"  TT2  (ISO14443A)  ", IdleFunc, IdleFunc,&TagEmulT2Menu},
																			 {(u8*)"  TT4A (ISO14443A)  ", IdleFunc, IdleFunc,&TagEmulT4AMenu},
                                       {(u8*)"<-     Return       ", ReturnFunc, IdleFunc}};
struct sMenu TagEmulMenu = {(u8*)"  Tag Emulator Mode ", TagEmulMenuItems, countof(TagEmulMenuItems)};


struct sMenuItem P2PMenuItems[] = { /*{(u8*)"   Prop P2P Auto    ", ProprietaryP2PAuto, IdleFunc},  */
																	 {(u8*)" Pong Config Client ", PongConfigClient, IdleFunc}, 
                                   {(u8*)" Pong Config Server ", PongConfigServer, IdleFunc},
																	 {(u8*)"  Prop P2P Client   ", ProprietaryP2PConfigClient, IdleFunc},
                                   {(u8*)"  Prop P2P Server   ", ProprietaryP2PConfigServer, IdleFunc},
																	 {(u8*)"<-     Return       ", ReturnFunc, IdleFunc}};
struct sMenu P2PMenu = {(u8*)"    Peer to Peer    ", P2PMenuItems, countof(P2PMenuItems)};

struct sMenuItem CardEmulMenuItems[] = {{(u8*)"  DWL (ISO14443A)   ", CardEmul14443A, IdleFunc},
                                        {(u8*)"<-     Return       ", ReturnFunc, IdleFunc}};
struct sMenu CardEmulMenu = {(u8*)" Card Emulator Mode ", CardEmulMenuItems, countof(CardEmulMenuItems)};



/*------------------------------ Menu level 1 -------------------------------*/
struct sMenuItem MainMenuItems[] = {
  {(u8*)"     Tag Reader     ", IdleFunc, IdleFunc, &TagReaderMenu},
  {(u8*)"     Tag Writer     ", IdleFunc, IdleFunc, &TagWriterMenu},
  {(u8*)"    Tag Emulator    ", IdleFunc, IdleFunc, &TagEmulMenu},
  {(u8*)"   Card Emulator    ", IdleFunc, IdleFunc, &CardEmulMenu },
  {(u8*)"    Peer to Peer    ", IdleFunc, IdleFunc, &P2PMenu}
};
struct sMenu MainMenu = {(u8*)"     Main menu      ", MainMenuItems, countof(MainMenuItems)};

/** @addtogroup Menu_Private_Functions
 * 	@{
 */


/**
  * @brief  Reads key from demoboard.
  * @param  None
  * @retval Return RIGHT, LEFT, SEL, UP, DOWN or NOKEY
  */
 uint8_t ReadKey(void)
{
  /* "right" key is pressed */
  if(!GPIO_ReadInputDataBit( J_RIGHT_PORT, J_RIGHT_PIN ))
  {
    while(GPIO_ReadInputDataBit( J_RIGHT_PORT, J_RIGHT_PIN ) == Bit_RESET)
    {
    } 
		if (screenRotated)
			return LEFT; 
		else
			return RIGHT; 
  }	
  /* "left" key is pressed */
  if(!GPIO_ReadInputDataBit( J_LEFT_PORT, J_LEFT_PIN ))
  {
    while(GPIO_ReadInputDataBit( J_LEFT_PORT, J_LEFT_PIN ) == Bit_RESET)
    {
    }
    if (screenRotated)
			return RIGHT; 
		else
			return LEFT; 
  }
  /* "up" key is pressed */
  if(!GPIO_ReadInputDataBit( J_UP_PORT, J_UP_PIN ))
  {
    while(GPIO_ReadInputDataBit( J_UP_PORT, J_UP_PIN ) == Bit_RESET)
    {
    }
		if (screenRotated)
			return DOWN; 
		else
			return UP; 
  }
  /* "down" key is pressed */
  if(!GPIO_ReadInputDataBit( J_DOWN_PORT, J_DOWN_PIN ))
  {
    while(GPIO_ReadInputDataBit( J_DOWN_PORT, J_DOWN_PIN ) == Bit_RESET)
    {
    } 
		if (screenRotated)
			return UP; 
		else
			return DOWN; 
  }
  /* "sel" key is pressed */
  if(!GPIO_ReadInputDataBit( J_SEL_PORT, J_SEL_PIN ))
  {
    while(GPIO_ReadInputDataBit( J_SEL_PORT, J_SEL_PIN ) == Bit_RESET)
    {
    } 
    return SEL; 
  }
  /* No key is pressed */
  else 
  {
    return NOKEY;
  }
}

/**
  * @brief  Wait user presses joystick.
  * @param  None
  * @retval None
  */
void WaitUserAction(void)
{	
	MenuActivated = false;
	while(ReadKey() == NOKEY)
  {	
  }
	MenuActivated = true;
}

/**
  * @brief  Wait debounce key.
  * @param  None
  * @retval None
  */
void WaitDebounce(void)
{
	while(ReadKey() != NOKEY)
  {
  } 
}

/**
  * @brief  convert a char to it's ASCII coding byte.
  * @param  MacAddr : pointer on the string to convert
	* @param  size : the size of the string
  * @retval None
  */
void Char2Hex( char* MacAddr ,uint32_t size)
{
	char result [12];
	uint8_t i=0,k=0,l=0;
	
	for(k=0; k<size; k++)
	{
		for(i=0; i<16; i++)
		{
			if (MacAddr[k] == ASCII [i][0])
			{	
				result[l] = ASCII [i][1];
				l++;
				break;
			}
		}
	}
	
	/* convert char to hex and switch byte order to ease copy in NDEF file */
	for(i=0; i<6; i++)
	{
		MacAddr[5-i] = ((result[2*i]&0x0F)<<4)| (result[2*i+1]&0x0F);
	}
}

/**
  * @brief  convert a ASCII coding byte to it's representing char.
  * @param  pDataIn : pointer on the byte array to translate
	* @param  NumberOfByte : the size of the array
	* @param  pString : pointer on the string created
  * @retval None
  */
void Hex2Char( u8* pDataIn, u16 NumberOfByte, char* pString )
{
	u8 data;
	uint8_t i=0;
	
	for(i=0; i<NumberOfByte; i++)
	{
		/* First char */
		data = (*pDataIn & 0xF0)>>4;
		if( data < 0x0A)
			*pString = data + 0x30;  /* ASCII offset for number */
		else
			*pString = data + 0x37; 	/* ASCII offset for letter */
	
		pString++;
	
		/* Second char */
		data = (*pDataIn & 0x0F);
		if( data < 0x0A)
			*pString = data + 0x30;  /* ASCII offset for number */
		else
			*pString = data + 0x37; 	/* ASCII offset for letter */
	
		pString++;
		pDataIn++;
	}
	
}

/**
  * @brief  Displays the current menu.
  * @param  None
  * @retval None
  */
void DisplayMenu(void)
{
  uint32_t Line = 0, index = 0;
  tMenuItem psMenuItem2;
	
	/* All the led must be off when displaying the menu */
	LED_AllOff();

  /* Set the Back Color */
  LCD_SetBackColor(Blue);
  /* Set the Text Color */
  LCD_SetTextColor(White);
	
  /* Clear the LCD Screen */
  LCD_Clear(White);

  LCD_DisplayStringLine(Line, psCurrentMenu->pszTitle);
  Line ++;

  /* Set the Back Color */
  LCD_SetBackColor(White);

  /* Set the Text Color */
  LCD_SetTextColor(Blue);

  while(index < (psCurrentMenu->nItems)-1)
  {
    psMenuItem2 = &(psCurrentMenu->psItems[index]);
    LCD_DisplayStringLine(Line, psMenuItem2->pszTitle);
    index++;
    Line ++;
  }
	/* Last line for the last choice (Return) */
	psMenuItem2 = &(psCurrentMenu->psItems[index]);
  LCD_DisplayStringLine(Line9, psMenuItem2->pszTitle);
  /* Set the Back Color */
  LCD_SetBackColor(Green);

  /* Set the Text Color */
  LCD_SetTextColor(White);

  /* Get the current menu */
  psMenuItem = &(psCurrentMenu->psItems[MenuItemIndex]);

	if (MenuItemIndex != psCurrentMenu->nItems-1)
		LCD_DisplayStringLine(MenuItemIndex+1 , psMenuItem->pszTitle);
	else
		LCD_DisplayStringLine(Line9 , psMenuItem->pszTitle);
	
	IntRotateOnOffConfig(ENABLE);
	lockKEYUpDown = false;
	
}

/**
  * @brief  Idle function.
  * @param  None
  * @retval None
  */
void IdleFunc(void)
{
  /* Nothing to execute: return */
  return;
}

/**
  * @brief  Initializes the navigation menu.
  * @param  None
  * @retval None
  */
void Menu_Init(void)
{
  psCurrentMenu = &MainMenu;
  psPrevMenu[nMenuLevel] = psCurrentMenu;
  psMenuItem = MainMenuItems;	
}

/**
  * @brief  Display menu icons.
  * @param  None
  * @retval None
  */
void DisplayIcons(void)
{
  uint32_t i = 0;
	
	for(i=0; i<NumberOfIcons; i++)
	{
			LCD_WriteBMP_FAT(IconRect[i][0]+2, IconRect[i][1]+3, (const char*)(IconsAddr[i]));
	}	
}

/**
  * @brief  This function is executed when the "RETURN" menu is selected.
  * @param  None
  * @retval None
  */
void ReturnFunc(void)
{
  psMenuItem->pfUpDownMenuFunc();

  if(nMenuLevel == 0)
  {
    nMenuLevel++;
  }

  psCurrentMenu = psPrevMenu[nMenuLevel-1];
  psMenuItem = &psCurrentMenu->psItems[0];
  ItemNumb[nMenuLevel] = 0;
  MenuItemIndex = 0;
  nMenuLevel--;

  if(nMenuLevel != 0)
  {
    DisplayMenu();
  }
  else
  {
    ShowMenuIcons();
  }
}

/**
  * @}
  */


/** @addtogroup Menu_Public_Functions
 * 	@{
 */

/**
  * @brief  Restart current SW.
  * @param  None
  * @retval None
  */
void USBRemoteControl(void)
{

	LCD_Clear(Black);

  IntExtOnOffConfig(ENABLE);
	IntRotateOnOffConfig(DISABLE);

  /* Set the Back Color */
  LCD_SetBackColor(Blue);
  /* Set the Text Color */
  LCD_SetTextColor(White);
  
  LCD_DisplayStringLine(Line0, (u8*)"    PC link mode    ");
  LCD_DisplayStringLine(Line9, (u8*)"   Select to exit   ");
	
	 /* Set the Back Color */
  LCD_SetBackColor(Black);
  /* Set the Text Color */
  LCD_SetTextColor(White);
	
	LCD_DisplayStringLine(Line3, (u8*)"  use PC software:  ");
	LCD_DisplayStringLine(Line4, (u8*)"   ST95HF dev SW    ");
	LCD_DisplayStringLine(Line5, (u8*)"         to         ");
	LCD_DisplayStringLine(Line6, (u8*)"drive ST95HF by USB ");
	
	
	/* Allow USB to send command to ST95HF */
	HIDTransaction = false;
	USB_Control_Allowed = true;
	USB_Cable_Config(ENABLE);
	
	KEYPress = false;
	lockKEYUpDown = true;
	/* wait user action to go back to previous screen */
	while(!KEYPress)
	{
	}
	
	/* disable ST95HF driving through USB*/
	USB_Control_Allowed = false;
	
	LCD_Clear(White);
  DisplayMenu();
}

/**
  * @brief  Display the ST introduction.
  * @param  None
  * @retval None
  */
void STIntro(void)
{	
#ifndef DEBUG
		LCD_WriteBMP_FAT(40, 24, (const char*)(uSTlogo));
		delay_ms(1000);
#endif
}

/**
  * @brief  Display the About menu.
  * @param  None
  * @retval None
  */
void DisplayVersion(void)
{
  LCD_Clear(White);

  /* Set the Back Color */
  LCD_SetBackColor(Blue);
  /* Set the Text Color */
  LCD_SetTextColor(White);

  LCD_ClearLine(Line0);
  LCD_DisplayStringLine(Line1, "       ST95HF       ");
  LCD_DisplayStringLine(Line2, "   Demonstration    ");
	LCD_DisplayStringLine(Line3, MCUVERSION_STRING);
  LCD_ClearLine(Line4);
  LCD_DisplayStringLine(Line5, "      MMY Team      ");
  LCD_DisplayStringLine(Line6, "   COPYRIGHT 2020   ");
  LCD_DisplayStringLine(Line7, " STMicroelectronics ");
  LCD_ClearLine(Line8);
  LCD_DisplayStringLine(Line9, " www.st.com/nfc-rfid");

}

/**
  * @brief  Show the main menu icon.
  * @param  None
  * @retval None
  */
void ShowMenuIcons(void)
{
  uint32_t MyKey = 0; 
	uint8_t CurrentIcone = 0;	

  /* Disable the JoyStick interrupts */
  IntExtOnOffConfig(DISABLE);

  WaitDebounce();

  /* Initializes the Menu state machine */
  Menu_Init();

  MenuItemIndex = 0;
  
	#ifndef DEBUG
	LCD_WriteBMP_FAT_size(0,25,img_background,320,215);
	#else
	LCD_Clear(White);
	#endif

  /* Set the Back Color */
  LCD_SetBackColor(Blue);

  /* Set the Text Color */
  LCD_SetTextColor(White);

	/* Display title */
  LCD_DisplayStringLine(Line0, psMenuItem->pszTitle);
  
  /* Set the Back Color */
  LCD_SetBackColor(White);

  /* Set the Text Color */
  LCD_SetTextColor(Black);

  /* Displays Icons */    
  DisplayIcons();

  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);
  /* Disable LCD Window mode */
  LCD_WindowModeDisable(); 


	LCD_DrawRect(IconRect[CurrentIcone][1], IconRect[CurrentIcone][0], 63, 63);
	LCD_DrawRect(IconRect[CurrentIcone][1]+1, IconRect[CurrentIcone][0]+1, 61, 61);
	

  while(GPIO_ReadInputDataBit(J_SEL_PORT, J_SEL_PIN) == RESET)
  {
  }
  /* Endless loop */
  while(1)
  {	
		if (refreshMenu)
		{
			#ifndef DEBUG
			LCD_WriteBMP_FAT_size(0,25,img_background,320,215);
			#else
			LCD_Clear(White);
			#endif
			/* Set the Back Color */
			LCD_SetBackColor(Blue);
			/* Set the Text Color */
			LCD_SetTextColor(White);
			/* Display title */
			LCD_DisplayStringLine(Line0, psMenuItem->pszTitle);
			/* Set the Back Color */
			LCD_SetBackColor(White);
			/* Set the Text Color */
			LCD_SetTextColor(Black);
			/* Displays Icons */    
			DisplayIcons();
			LCD_WindowModeDisable(); 
			LCD_DrawRect(IconRect[CurrentIcone][1], IconRect[CurrentIcone][0], 63, 63);
			LCD_DrawRect(IconRect[CurrentIcone][1]+1, IconRect[CurrentIcone][0]+1, 61, 61);
			refreshMenu = false;
		}
    /* Check which key is pressed */
    MyKey = ReadKey();

    /* If "UP" pushbutton is pressed */
    if(MyKey == UP)
    {
      /* Set the Text Color */
      LCD_SetTextColor(White);
      LCD_DrawRect(IconRect[CurrentIcone][1], IconRect[CurrentIcone][0], 63, 63);
			LCD_DrawRect(IconRect[CurrentIcone][1]+1, IconRect[CurrentIcone][0]+1, 61, 61);
					
			if ( CurrentIcone-NbIconeByLine >= 0)
				CurrentIcone -= NbIconeByLine;
			else
			{
				while( CurrentIcone+NbIconeByLine < NumberOfIcons)
				{
					CurrentIcone += NbIconeByLine;
				}
			}
			
    }
    /* If "DOWN" pushbutton is pressed */
    if(MyKey == DOWN)
    {
      /* Set the Text Color */
      LCD_SetTextColor(White);
      LCD_DrawRect(IconRect[CurrentIcone][1], IconRect[CurrentIcone][0], 63, 63);
			LCD_DrawRect(IconRect[CurrentIcone][1]+1, IconRect[CurrentIcone][0]+1, 61, 61);
			
			if ( CurrentIcone+NbIconeByLine < NumberOfIcons)
				CurrentIcone += NbIconeByLine;
			else
			{
				while( CurrentIcone-NbIconeByLine >= 0)
				{
					CurrentIcone -= NbIconeByLine;
				}
			}

    }
    /* If "LEFT" pushbutton is pressed */
    if(MyKey == LEFT)
    {
      /* Set the Text Color */
      LCD_SetTextColor(White);
      LCD_DrawRect(IconRect[CurrentIcone][1], IconRect[CurrentIcone][0], 63, 63);
			LCD_DrawRect(IconRect[CurrentIcone][1]+1, IconRect[CurrentIcone][0]+1, 61, 61);
			
			if ( CurrentIcone != 0)
				CurrentIcone = CurrentIcone-1;
			else
				CurrentIcone = NumberOfIcons-1;
    }
    /* If "RIGHT" pushbutton is pressed */
    if(MyKey == RIGHT)
    {
      /* Set the Text Color */
      LCD_SetTextColor(White);
      LCD_DrawRect(IconRect[CurrentIcone][1], IconRect[CurrentIcone][0], 63, 63);
			LCD_DrawRect(IconRect[CurrentIcone][1]+1, IconRect[CurrentIcone][0]+1, 61, 61);
     
			if ( CurrentIcone != NumberOfIcons-1)
				CurrentIcone = CurrentIcone+1;
			else
				CurrentIcone = 0;
		
    }
		
		if( MyKey == RIGHT || MyKey == LEFT || MyKey == DOWN || MyKey == UP)
		{
			/* Set the Text Color */
      LCD_SetTextColor(Black);
      LCD_DrawRect(IconRect[CurrentIcone][1], IconRect[CurrentIcone][0], 63, 63);
			LCD_DrawRect(IconRect[CurrentIcone][1]+1, IconRect[CurrentIcone][0]+1, 61, 61);

      /* Set the Back Color */
      LCD_SetBackColor(Blue);
      /* Set the Text Color */
      LCD_SetTextColor(White);
      /* Test on the MenuItemIndex value before incrementing it */
			MenuItemIndex = CurrentIcone;
			
      /* Get the current menu */
      psMenuItem = &(psCurrentMenu->psItems[MenuItemIndex]);
      LCD_DisplayStringLine(Line0, psMenuItem->pszTitle);
      ItemNumb[nMenuLevel] = MenuItemIndex;
		}
		
    /* If "SEL" pushbutton is pressed */
    if(MyKey == SEL)
    {
      SelFunc();			
      IntExtOnOffConfig(ENABLE);
      return;
    }
  }    
}

/**
  * @brief  This function is executed when any of "UP" push-butttons is pressed.
  * @param  None
  * @retval None
  */
void UpFunc(void)
{
	if(MenuActivated == false)
		return;
	
  /* Set the Back Color */
  LCD_SetBackColor(White);
  /* Set the Text Color */
  LCD_SetTextColor(Blue);
	psMenuItem = &psCurrentMenu->psItems[MenuItemIndex];
	if (MenuItemIndex != psCurrentMenu->nItems-1)
		LCD_DisplayStringLine((MenuItemIndex + 1), psMenuItem->pszTitle);
	else
		LCD_DisplayStringLine(Line9, psMenuItem->pszTitle);

  if(MenuItemIndex > 0)
  {
    MenuItemIndex--;
  }
  else
  {
    MenuItemIndex = psCurrentMenu->nItems - 1;
  }
  /* Set the Back Color */
  LCD_SetBackColor(Green);
  /* Set the Text Color */
  LCD_SetTextColor(White);
  psMenuItem = &psCurrentMenu->psItems[MenuItemIndex];
	if (MenuItemIndex != psCurrentMenu->nItems-1)
		LCD_DisplayStringLine((MenuItemIndex + 1), psMenuItem->pszTitle);
	else
		LCD_DisplayStringLine(Line9, psMenuItem->pszTitle);
  ItemNumb[nMenuLevel] = MenuItemIndex;
}

/**
  * @brief  This function is executed when any of "DOWN" push-butttons is pressed.
  * @param  None
  * @retval None
  */
void DownFunc(void)
{
	if(MenuActivated == false)
		return;
		
  /* Set the Back Color */
  LCD_SetBackColor(White);
  /* Set the Text Color */
  LCD_SetTextColor(Blue);
  psMenuItem = &psCurrentMenu->psItems[MenuItemIndex];
	if (MenuItemIndex != psCurrentMenu->nItems-1)
		LCD_DisplayStringLine((MenuItemIndex + 1), psMenuItem->pszTitle);
	else
		LCD_DisplayStringLine(Line9, psMenuItem->pszTitle);
      
  /* Test on the MenuItemIndex value before incrementing it */
  if(MenuItemIndex >= ((psCurrentMenu->nItems)-1))
  {
    MenuItemIndex = 0;
  }
  else
  {
    MenuItemIndex++;
  }
  /* Set the Back Color */
  LCD_SetBackColor(Green);
  /* Set the Text Color */
  LCD_SetTextColor(White);
  /* Get the current menu */
  psMenuItem = &(psCurrentMenu->psItems[MenuItemIndex]);
	if (MenuItemIndex != psCurrentMenu->nItems-1)
		LCD_DisplayStringLine((MenuItemIndex + 1), psMenuItem->pszTitle);
	else
		LCD_DisplayStringLine(Line9, psMenuItem->pszTitle);
  ItemNumb[nMenuLevel] = MenuItemIndex;
}

/**
  * @brief  This function is executed when "SEL" push-button is pressed.
  * @param  None
  * @retval None
  */
void SelFunc(void)
{
  psCurrentMenuItem = psMenuItem;

  if(psMenuItem->psSubMenu != '\0')
  {
    /* Update the current Item by the submenu */
    MenuItemIndex = 0;
    psCurrentMenu = psMenuItem->psSubMenu;
    psMenuItem = &(psCurrentMenu->psItems)[MenuItemIndex];
    DisplayMenu();
    nMenuLevel++;
    psPrevMenu[nMenuLevel] = psCurrentMenu;
  } 
  psCurrentMenuItem->pfMenuFunc();
}

/**
  * @brief  Initialize GPIO mapping
  * @param  None
  * @retval None
  */
void InitJoystick(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	
	/* Enable AFIO clocks */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
       
  /* Enable BUTTON GPIO clock */
  RCC_APB2PeriphClockCmd( J_SEL_CLOCK 	|
                          J_DOWN_CLOCK	|
													J_LEFT_CLOCK	|
													J_RIGHT_CLOCK |
													J_UP_CLOCK		|
													ROTATE_CLOCK  , 
													ENABLE );

	/* Configure JOYSTICK BUTTON pins as input */
 
  /* JOYSTICK SEL Button */
  GPIO_InitStructure.GPIO_Pin   = J_SEL_PIN;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
  GPIO_Init( J_SEL_PORT, &GPIO_InitStructure );

  /* JOYSTICK DOWN Button */
  GPIO_InitStructure.GPIO_Pin   = J_DOWN_PIN ;
  GPIO_Init( J_DOWN_PORT, &GPIO_InitStructure );

  /* JOYSTICK LEFT Button */
  GPIO_InitStructure.GPIO_Pin   = J_LEFT_PIN;
  GPIO_Init( J_LEFT_PORT, &GPIO_InitStructure );  

  /* JOYSTICK RIGHT Button */
  GPIO_InitStructure.GPIO_Pin   = J_RIGHT_PIN;
  GPIO_Init( J_RIGHT_PORT, &GPIO_InitStructure );
    
  /* JOYSTICK UP Button */
  GPIO_InitStructure.GPIO_Pin   = J_UP_PIN;
  GPIO_Init( J_UP_PORT, &GPIO_InitStructure );  
	
	/* Rotate Button */
  GPIO_InitStructure.GPIO_Pin   = ROTATE_PIN;
  GPIO_Init( ROTATE_PORT, &GPIO_InitStructure ); 
	 
	/* for EXTI config */
	 
	/* RIGHT Button */
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource8);

  /* LEFT Button */
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource5);

  /* DOWN Button */
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource7);

  /* UP Button */
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource9);

  /* SEL Button */
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource6);
	
	/* ROTATE Button */
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource4);
	
  /* Initializes the EXTI_InitStructure for the rotate button*/
  EXTI_StructInit(&EXTI_InitStructure);
	EXTI_ClearITPendingBit(EXTI_Line4);
	EXTI_InitStructure.EXTI_Mode 			= EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Line 			= EXTI_Line4;
	EXTI_InitStructure.EXTI_Trigger 	= EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd 	= ENABLE;
	EXTI_Init(&EXTI_InitStructure);

}

/**
  * @brief  Enables or disables Rotate interrupt
  * @param  New State
  * @retval None
	*/
void IntRotateOnOffConfig(FunctionalState NewState)
{
  EXTI_InitTypeDef EXTI_InitStructure;

  /* Initializes the EXTI_InitStructure */
  EXTI_StructInit(&EXTI_InitStructure);

  /* Disable the EXTI line 5, 6, 7, 8 and 9 on falling edge */
  if(NewState == DISABLE)
  {
    EXTI_InitStructure.EXTI_Line = EXTI_Line4;
    EXTI_InitStructure.EXTI_LineCmd = DISABLE;
    EXTI_Init(&EXTI_InitStructure);
  }
  /* Enable the EXTI line 5, 6, 7, 8 and 9 on falling edge */
  else
  {
		disableRotate = false;
    /* Clear the the EXTI line 5, 6, 7, 8 and 9 interrupt pending bit */
    EXTI_ClearITPendingBit(EXTI_Line4);

    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Line = EXTI_Line4;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
  }
}

/**
  * @brief  Enables or disables EXTI for the menu navigation keys :
  * @brief  EXTI lines 3, 7 and 15 which correpond respectively
  * @brief  to "DOWN", "SEL" and "UP".
  * @param  New State
  * @retval None
	*/
void IntExtOnOffConfig(FunctionalState NewState)
{
  EXTI_InitTypeDef EXTI_InitStructure;

  /* Initializes the EXTI_InitStructure */
  EXTI_StructInit(&EXTI_InitStructure);

  /* Disable the EXTI line 5, 6, 7, 8 and 9 on falling edge */
  if(NewState == DISABLE)
  {
    EXTI_InitStructure.EXTI_Line = EXTI_Line5 | EXTI_Line6 | EXTI_Line7 | EXTI_Line8 | EXTI_Line9;
    EXTI_InitStructure.EXTI_LineCmd = DISABLE;
    EXTI_Init(&EXTI_InitStructure);
  }
  /* Enable the EXTI line 5, 6, 7, 8 and 9 on falling edge */
  else
  {
    /* Clear the the EXTI line 5, 6, 7, 8 and 9 interrupt pending bit */
    EXTI_ClearITPendingBit(EXTI_Line5 | EXTI_Line6 | EXTI_Line7 | EXTI_Line8 | EXTI_Line9);

    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Line = EXTI_Line5 | EXTI_Line6 | EXTI_Line7 | EXTI_Line8 | EXTI_Line9;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
  }
}

/**
  * @brief  This function fills the demo GEO structure
  */
void FillTagGEO(char* pTagType)
{
	/* Fill the structure of the NDEF GEO */
	strcpy(geo.Information,pTagType);
	strcpy(geo.Latitude, "48.132974");
	strcpy(geo.Longitude,"-1.635912");
}
/**
  * @brief  This function fills the demo URI structure
  */
void FillTagURI(char* pTagType)
{
	/* Fill the structure of the NDEF URI */
	strcpy(url.Information,pTagType);
	strcpy(url.protocol,"http://");
	strcpy(url.URI_Message,"st.com/nfc-rfid");
}
/**
  * @brief  This function fills the demo SMS structure
  */
void FillTagSMS(char* pTagType)
{
	/* Fill the structure of the NDEF SMS */
	strcpy(sms.Information,pTagType);
	strcpy(sms.PhoneNumber,"0123456789");
	strcpy(sms.Message,"This is a SMS message in order to demonstrate the capability of the ST95HF to emulate a tag");
}
/**
  * @brief  This function fills the demo SMS structure
  */
void FillTagEMAIL(char* pTagType)
{
	/* Fill the structure of the NDEF Email */
	strcpy(email.Information,pTagType);
	strcpy(email.EmailAdd,"test@st.com");
	strcpy(email.Subject,"STMicroelectronics wikipedia");
	strcpy(email.Message,"STMicroelectronics is a French-Italian multinational electronics and semiconductor manufacturer headquartered in Geneva, Switzerland. It is Europe s largest semiconductor chip maker based on revenue. ST is among the world leaders in a broad range of segments, including semiconductors for industrial applications, inkjet printheads, MEMS (Micro-Electro-Mechanical Systems), MPEG decoders and smartcard chips, automotive integrated circuits, computer peripherals, and chips for wireless and mobile applications. While STMicroelectronics corporate headquarters and the headquarters for EMEA region are based in Geneva, the holding company, STMicroelectronics N.V. is registered in Amsterdam, Netherlands. The company s US headquarters is in Coppell, Texas. Headquarters for the Asia-Pacific region is in Singapore whilst Japan and Korea operations are headquartered in Tokyo. The company headquarters for the Greater China region is in Shanghai.");
}


/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
