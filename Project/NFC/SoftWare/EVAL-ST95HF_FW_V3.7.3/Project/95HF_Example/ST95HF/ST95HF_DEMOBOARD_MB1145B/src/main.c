/**
  ******************************************************************************
  * @file    main.c 
  * @author  MMY Application Team
  * @version V1.0.0
  * @date    03/21/2013
  * @brief   Main program body
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"

#include "VcardCSL1.h"

#include "menu_auto.h"

/** @addtogroup User_Appli
 * 	@{
 *  @brief      <b>This folder contains the application files</b> 
 */
 
/** @addtogroup Main
 * 	@{
 * @brief      This file contains the entry point of this demonstration firmeware
 */


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

static __IO uint32_t TimingDelay = 0;

/* PCD/PICC global memory space */

/* TT1 (PCD only)*/
uint8_t TT1Tag[NFCT1_MAX_TAGMEMORY];

/* TT2 */
uint8_t TT2Tag[NFCT2_MAX_TAGMEMORY];

/* TT3 */
uint8_t TT3Tag[NFCT3_MAX_TAGMEMORY];
uint8_t *TT3AttribInfo = TT3Tag, *TT3NDEFfile = &TT3Tag[NFCT3_ATTRIB_INFO_SIZE];

/* TT4 */
uint8_t CardCCfile [NFCT4_MAX_CCMEMORY];
uint8_t CardNDEFfileT4A [NFCT4_MAX_NDEFMEMORY];
uint8_t CardNDEFfileT4B [NFCT4_MAX_NDEFMEMORY];

/* TT5 (PCD only)*/
uint8_t TT5Tag[NFCT5_MAX_TAGMEMORY];

__IO uint32_t SELStatus = 0;

/* USB */
bool 		USB_Control_Allowed = false;

extern volatile bool refreshMenu;

  
 /* Private function prototypes -----------------------------------------------*/
static bool EVAL_ST95HF_HwUsed(void);
 

/** @addtogroup Main_Private_Functions
 * 	@{
 */

/**
  * @}
  */

/** @addtogroup Main_Public_Functions
 * 	@{
 */

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
	Led_TypeDef i;
	
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x000);
	
	/* -- Configures Main system clocks & power */
	Set_System();
	
	/* Enable CRC periph used by application to compute CRC after download */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC, ENABLE);
	  
/*------------------- Resources Initialization -----------------------------*/
	
	/* configure the interuptions  */
	Interrupts_Config();

	/* configure the timers  */
	Timer_Config( );
	
	/* Configure systick in order to generate one tick every ms */
	/* also used to generate pseudo random numbers (SysTick->VAL) */
	SysTick_Config(SystemCoreClock / 1000);
	
/*------------------- Drivers Initialization -------------------------------*/
	
	//add yinke
		uart_init(115200);
	//end add 
	
	/* Initialize the Joystick */
 //InitJoystick();
	
	/* LCD initialization */
	//LCD_Init();
	//LCD_Clear(White);

	/* configure the LED  */
	for (i=LED1;i<=LED5;i++)
	{
		LED_Config(i);	
		LED_On(i);
	}
	
	/* Display ST Logo and FW Version */
//	STIntro();
//	delay_ms(400);
//	DisplayVersion();
//	delay_ms(2500);
	
	/* activate the USB clock */
	//Set_USBClock();
	delay_ms(10);
	/* initialize the USB  */
	//USB_Init();
	/* but prevent PC to control ST95HF */
	//USB_Cable_Config(DISABLE);
	//USB_Control_Allowed = false;
	
	/* check it's an EVAL-ST95HF board */
	//if( EVAL_ST95HF_HwUsed() != true)
	//	goto End;
	
	/* ST95HF HW Init */
  ConfigManager_HWInit();//st95初始化，获得芯片版本信息，配置spi，通信方式为spi中断方式，而不是轮询方式


	/* Clear the LCD */ 
//  LCD_Clear(White);

//  /* Set the LCD Back Color */
//  LCD_SetBackColor(Blue);

//  /* Set the LCD Text Color */
//  LCD_SetTextColor(White);
//  
  /* Initialize the Menu */
  Menu_Init();

	/* configure the LED  */
	for (i=LED1;i<=LED1;i++)
	{
		LED_Config(i);	
		LED_Off(i);
	}
	
  /* Display the main menu icons */
//  ShowMenuIcons();
	printf("start while loop\r\n");
	while (1)
  {
//		if (refreshMenu)
//		{
//			refreshMenu = false;
//			DisplayMenu();
//		}
//    /* If SEL pushbutton is pressed */
//    if(SELStatus == 1)
//    {
//      /* External Interrupt Disable */
//      IntExtOnOffConfig(DISABLE);

//      /* Execute Sel Function */
//      SelFunc();

//      /* External Interrupt Enable */
//      IntExtOnOffConfig(ENABLE);
//      /* Reset SELStatus value */
//      SELStatus = 0;
//    } 
		//add yinke
			//LED_Troggle(LED1);
			//delayHighPriority_ms(1000);
//			while(USART_RX_BUF[0] == 0x01)
//			{
			
//				printf("read tag...\r\n");
//				TagReading();
				
				printf("write tag...\r\n");
				TagWriteEMAIL();
//				autoTypeAll();
				delay_ms(1000);
//			}
			 
//				printf("writting tag text...\r\n");
//				TagWriteText();
		//end add 
  }
	
//End:
//		return 0;
	
	
}

static bool EVAL_ST95HF_HwUsed(void)
{
	uint16_t GpioState = 0;
	
	GPIO_InitTypeDef  GPIO_InitStructure;
  
  /* Enable the GPIO Clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

  /* Configure the GPIO_LED pin */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GpioState = GPIO_ReadInputData(GPIOA);
	
	if( (GpioState & ((uint16_t)(GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10))) == 0x0000)
		return true;
	else
		return false;
		
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

/**
  * @}
  */

/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
