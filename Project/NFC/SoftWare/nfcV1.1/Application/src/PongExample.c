/**
  ******************************************************************************
  * @file    proprietary_p2p.c
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
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
  */ 

#include <stdio.h>
#include "proprietary_p2p.h"


extern volatile uint8_t KEYPressed;
extern bool lockKEYUpDown;

extern bool DisableExitButtonAction;

/***************************/
/* Releated to P2P example */
/*    Needed Ressources    */
/***************************/
extern DeviceMode_t 		devicemode;
extern TagType_t 	nfc_tagtype;

extern bool RequestOwnerchip;

static uint8_t PongInitiator_SendRequestReceiveResponse(void);
static uint8_t PongTarget_ReceiveRequestSendResponse(uint8_t *pData, uint16_t *size);

/* Function to call application when target receive data 
 must be mapped to dedicated function to check data and provide answer */
extern uint8_t (*P2PReceiveBUFCallBack)(uint8_t*pData, uint16_t *nbByte);




/***************************/
/*       End of P2P        */
/*    Needed Ressources    */
/***************************/

static void PongRefreshScreen(bool client);

/* Initiator = The board that create the NFC field (PCD) */
/* Target = the board that perform retromodulation to answer (PICC in card emulation mode) */
/* Pong is not really a P2P exemple as Server (Initiator) always send request and Client (Target) always answer */

extern bool 		screenRotated;
extern bool			disableRotate;

uint8_t pos1=90, pos2=90, posBallY = 120-BALL_SIZE/2, score1=0, score2=0;
static uint8_t old_pos1=0, old_pos2=0;
uint16_t posBallX = 40;
uint8_t reversed = 0x00, old_reversed = 0x00, reversed2 = 0x00, old_reversed2 = 0x00;

/**
  * @brief  The client (RX) menu
  */
void PongConfigClient(void)
{
	/* Client set-up */
	
	if (!screenRotated)
	{
		screenRotated= !screenRotated;
		LCD_Rotate();
	}
	
	LED_On(LED1);
	/* print screen TagEmulator */
	/* Set the Title Back Color */
  LCD_SetBackColor(Black);

  /* Set the Title Color */
  LCD_SetTextColor(White);
	
	/* Clear the LCD Screen */
  LCD_Clear(Black);
		
	/* External Interrupt Enable */
	IntExtOnOffConfig(ENABLE);
	disableRotate = true; 
	
	/* Prepare to exit loop on Key Press */
	KEYPressed = NOKEY;
	lockKEYUpDown = true;
	DisableExitButtonAction = true;
	
	LED_Off(LED1);
	
	/****************************/
	/* Starting NFC Client part */
	/****************************/
	
	/* Initilialize tag emulation mode */
	devicemode = PICC;
	nfc_tagtype = TT4A;
	
	/* Initialize Callback */
	P2PReceiveBUFCallBack = PongTarget_ReceiveRequestSendResponse;
	
	/* Enter a loop that only user can stop with SEL click */
	ConfigManager_CardEmulation(PICCEMULATOR_TAG_TYPE_4A, PongCheckForUserAction );
	
	/****************************/
	/* End NFC Client part      */
	/****************************/
		
	/* External Interrupt Disable */
	IntExtOnOffConfig(DISABLE);		
	lockKEYUpDown = false;
	DisableExitButtonAction = false;
  old_pos1 = 0x00; /* to force display at next start */
	old_pos2 = 0x00; /* to force display at next start */
  score1=0, score2=0; /* to restart a game at 0, if change to Server */
	
	/* Clear the LCD Screen */
  LCD_Clear(White);
	/* Display Menu */
  DisplayMenu();
}


void PongCheckForUserAction (void)
{
	PongRefreshScreen(true);
	
	switch (KEYPressed)
	{
		case DOWN:
			KEYPressed = NOKEY;
			if (pos2 <= 170)
				pos2+=10;
			break;
		case UP:
			KEYPressed = NOKEY;
			if (pos2 >= 10)
				pos2-=10;
			break;
				
		case SEL:
			ConfigManager_Stop();
		break;
	}
}

/**
  * @brief  The server (CR) menu
  */
void PongConfigServer(void)
{
	int8_t moveX = 1, moveY = 1;
	uint8_t speed = 0x05;
	uint8_t TagType = 0;
  uint8_t String[20] = "                    ";
	
	if (screenRotated)
	{
		screenRotated= !screenRotated;
		LCD_Rotate();
	}
	
	LED_On(LED1);
	/* print screen TagEmulator */
	/* Set the Title Back Color */
  LCD_SetBackColor(Black);

  /* Set the Title Color */
  LCD_SetTextColor(White);
	
	/* Clear the LCD Screen */
  LCD_Clear(Black);
  
  memcpy(String,"Waiting for opponent",20);
  LCD_DisplayStringLine(Line3,String);
	
	/* External Interrupt Enable */
	IntExtOnOffConfig(ENABLE);
	disableRotate = true; 
	
	/* Prepare to exit loop on Key Press */
	KEYPressed = NOKEY;
	lockKEYUpDown = true;
	LED_Off(LED1);
	
	while( (TagType != TRACK_NFCTYPE4A) && (KEYPressed != SEL) )
	{
		TagType = ConfigManager_TagHunting(TRACK_NFCTYPE4A);	
	}
	
  /* Clear the LCD Screen */
  LCD_Clear(Black);
  
	while(KEYPressed != SEL)
  {
		PongRefreshScreen(false);
		switch (KEYPressed)
		{
			case DOWN:
				KEYPressed = NOKEY;
				if (pos1 <= 170)
					pos1+=10;
				break;
			case UP:
				KEYPressed = NOKEY;
				if (pos1 >= 10)
					pos1-=10;
				break;
			case RIGHT:
				KEYPressed = NOKEY;
        if(speed < 0x05)
        {
				  speed = 0x00;
        }
        else
        {
          speed -= 5;
        }
				break;
			case LEFT:
				KEYPressed = NOKEY;
			  if(speed > 0xFA)
        {
				  speed = 0xFF;
        }
        else
        {
          speed += 5;
        }
				break;
		}
		if(PongInitiator_SendRequestReceiveResponse()==PCD_SUCCESSCODE)
		{
			posBallX+=moveX;
			/* Goal */
			if (posBallX >= 640-BALL_SIZE)
			{
				moveX=-1;
				moveY=1;
				posBallX=600-BALL_SIZE;
				posBallY=120-BALL_SIZE/2;
				if (score1 < 9)
				{
					score1++;
				}
				else
				{
					score1 = 0;
					score2 = 0;
				}
			}
			else if (posBallX <= 0)
			{
				moveX=1;
				moveY=1;
				posBallX=40;
				posBallY=120-BALL_SIZE/2;
				if (score2 < 9)
				{
					score2++;
				}
				else
				{
					score1 = 0;
					score2 = 0;
				}
			}
			
			/* UP and DOWN walls */
			posBallY+=moveY;
			if (posBallY >= 240-BALL_SIZE)
				moveY=-1;
			else if (posBallY <= 0)
				moveY=1;
			
			/* Player 1 */
			if (posBallX <= 20 && ((posBallY > pos1 && posBallY < pos1+60) || (posBallY+BALL_SIZE > pos1 && posBallY+BALL_SIZE < pos1+60)))
				moveX=1;
			/* Player 2 */
			else if (posBallX+BALL_SIZE >= 620 && ((posBallY > pos2 && posBallY < pos2+60) || (posBallY+BALL_SIZE > pos2 && posBallY+BALL_SIZE < pos2+60)))
				moveX=-1;
		}
		if( posBallX >= 0x140 )
    {
      /* Reduce speed Client side (visible when speed=0) */
      delay_ms(speed+1);
    }
    else
    {
      delay_ms(speed);
    }
  }
	PCD_FieldOff( );
	
	/* External Interrupt Disable */
	IntExtOnOffConfig(DISABLE);		
	lockKEYUpDown = false;
	old_pos1 = 0x00; /* to force display at next start */
  old_pos2 = 0x00; /* to force display at next start */
	score1=0, score2=0; /* to restart a game at 0 */
	
	/* Clear the LCD Screen */
  LCD_Clear(White);
	/* Display Menu */
  DisplayMenu();
}

/****************************************** Initiator part *****************************************/

static uint8_t PongInitiator_SendRequestReceiveResponse(void)
{
	uint8_t SendReceiveBuffer[5] = {0,0,0,0,0};
	uint16_t size = 0;
	uint8_t status = PP2P_ERRORCODE_DEFAULT;
	
	/* Add the variables to update */
	SendReceiveBuffer[size++] = score2;
	SendReceiveBuffer[size++] = (uint8_t)(posBallX>>8);
	SendReceiveBuffer[size++] = (uint8_t)(posBallX&0x00FF);
	SendReceiveBuffer[size++] = posBallY;
	SendReceiveBuffer[size++] = reversed;
	
	status = PP2P_SendBUF(SendReceiveBuffer, &size);
	
	if( status == PP2P_SUCCESS )
	{
		LED_On(LED1);
		pos2 = SendReceiveBuffer[0];
		reversed2 = SendReceiveBuffer[1]&0x10;
		// If the other rotate button is pressed
		if (reversed2 != old_reversed2) 
			reversed = reversed2;
		old_reversed2 = reversed2;
	}
	else
	{
		/* communication lost restart */
		LED_Off(LED1);
	}		
	
	return status;
}

/**************************************** End of Initiator part **************************************/

/****************************************** Target part *********************************************/

static uint8_t PongTarget_ReceiveRequestSendResponse(uint8_t *pData, uint16_t *size)
{
	
	/* Receive data from the server */
	score2 = pData[0];
	posBallX = (pData[1]<<8)|pData[2];
	posBallY = pData[3];
	reversed = pData[4]&0x10;
	// If the other rotate button is pressed
	if (reversed != old_reversed) 
		reversed2 = reversed;
	old_reversed = reversed;
	
	/* Prepare response */
	pData[0] =  pos2;
	pData[1] = reversed2;

	*size = 2;
	
	return PP2P_SUCCESS;
}

/**************************************** End of Target part **************************************/

static void PongRefreshScreen(bool client)
{
	static uint8_t old_posBallY, old_score1=9, old_score2=9;
	static uint16_t old_posBallX;

	if (client)
	{
		if (KEYPressed == ROTATE)
		{
			KEYPressed = NOKEY;
			reversed2^=0x10;
		}
		/* Clear previous position */
		LCD_SetTextColor(Black);
		/* Clear only if the paddle moves */
		if (old_pos2 != pos2) LCD_DrawRect(old_pos2, 300, 60, 10);

		/* Clear the ball only if it moved */
		if( (old_posBallY != posBallY) || (old_posBallX != posBallX))
		{
			/* manage the split on both screens */
			if( (old_posBallX >= 0x12C) && (old_posBallX < 0x140) )
			{
					/* draw horizontal lines */
					LCD_DrawLine( 0x00, old_posBallY, (BALL_SIZE - (0x13F - old_posBallX)), LCD_DIR_HORIZONTAL);
					LCD_DrawLine( 0x00, (old_posBallY + BALL_SIZE), (BALL_SIZE - (0x13F - old_posBallX)), LCD_DIR_HORIZONTAL);
					/* draw vertical lines */
					LCD_DrawLine( ((old_posBallX + BALL_SIZE) - 0x140), old_posBallY , BALL_SIZE, LCD_DIR_VERTICAL);
			}
			else if( old_posBallX >= 0x140 )
			{
				LCD_DrawRect( old_posBallY, (old_posBallX - 0x140) , BALL_SIZE, BALL_SIZE);
			}
		}
		/* Draw the new one */
		LCD_SetTextColor(Red);
		/* Redrawing the red Paddle if it has been moved or if the ball bounced off on it */
		if( (old_pos2 != pos2) || ((old_posBallX == 0x258) && (posBallX == 0x257)) || ((old_posBallX == 0x26B) && (posBallX == (0x258 - BALL_SIZE))) )
		{
			LCD_DrawRect(pos2, 300, 60, 10);
		}
		
		LCD_SetTextColor(White);
		/* refreshes only when the ball moves */
		if( (old_posBallY != posBallY) || (old_posBallX != posBallX))
		{
			/* When the ball reaches the limit of the screen, splits the ball on both screen */
			if( (posBallX >= 0x12C) && (posBallX < 0x140) )
			{
				/* draw horizontal lines */
				if( posBallX != 0x12C)
				{
					LCD_DrawLine( 0x00, posBallY, (BALL_SIZE - (0x13F - posBallX)), LCD_DIR_HORIZONTAL);
					LCD_DrawLine( 0x00, (posBallY + BALL_SIZE), (BALL_SIZE - (0x13F - posBallX)), LCD_DIR_HORIZONTAL);
				}
				/* draw vertical lines */
				LCD_DrawLine( ((posBallX + BALL_SIZE) - 0x140), posBallY , BALL_SIZE, LCD_DIR_VERTICAL);
			}
			else if( posBallX >= 0x140 )
			{
				LCD_DrawRect( posBallY, (posBallX - 0x140) , BALL_SIZE, BALL_SIZE);
			}
		}
		/* Screen flip */
		if (reversed2 != old_reversed2)
		{
			if (!reversed2)
			{ 
				/* Clear the other side */
				LCD_Rotate();
				LCD_DisplayChar(Line0, 19, ' ');
				LCD_Rotate();
				/* Write the new score */
				LCD_DisplayChar(Line0, 0, score2+'0');
			}
			else
			{
				if (!old_reversed2) /* Clear the other side */
					LCD_DisplayChar(Line0, 0, ' ');
				/* Write the new score */
				LCD_Rotate();
				LCD_DisplayChar(Line0, 19, score2+'0');
				LCD_Rotate();
			}
		}
		/* Score update */
		else if (old_score2 != score2)
		{ 
			if (!reversed2)
				LCD_DisplayChar(Line0, 0, score2+'0');
			else
			{
				LCD_Rotate();
				LCD_DisplayChar(Line0, 19, score2+'0');
				LCD_Rotate();
			}
		}
		/* Ball passes through the score */
    else if( (old_posBallY == 0x18) && (posBallY == 0x19) && (!reversed2) && (posBallX > 0x100) && (posBallX < 0x180) )
		{
			LCD_DisplayChar(Line0, 0, score2+'0');
    }
    /* Ball passes through the score */
    else if( (old_posBallY == 0xC4) && (posBallY == 0xC3) && (reversed2) && (posBallX > 0x100) && (posBallX < 0x180) )
		{
			LCD_Rotate();
			LCD_DisplayChar(Line0, 19, score2+'0');
			LCD_Rotate();
		}
		old_reversed2 = reversed2;
	}
	else
	{
		if (KEYPressed == ROTATE)
		{
			KEYPressed = NOKEY;
			reversed^=0x10;
		}
		/* Clear previous position */
		LCD_SetTextColor(Black);
		/* Clear only if the paddle moves */
		if (old_pos1 != pos1) LCD_DrawRect(old_pos1, 10, 60, 10);

		if( (old_posBallY != posBallY) || (old_posBallX != posBallX))
		{
			if( (old_posBallX >= 0x12C) && (old_posBallX < 0x140) )
			{
				if(old_posBallX != 0x13F)
				{
					/* draw horizontal lines */
					LCD_DrawLine(old_posBallX, old_posBallY, (0x13F - old_posBallX), LCD_DIR_HORIZONTAL);
					LCD_DrawLine(old_posBallX, (old_posBallY + BALL_SIZE), (0x13F - old_posBallX), LCD_DIR_HORIZONTAL);
				}
				/* draw vertical lines */
				LCD_DrawLine(old_posBallX, old_posBallY, BALL_SIZE, LCD_DIR_VERTICAL);
			}
			else if( old_posBallX < 0x12C )
			{
				LCD_DrawRect(old_posBallY, old_posBallX, BALL_SIZE, BALL_SIZE);
			}
		}
		
		/* Draw the new one */
		/* Redrawing the green Paddle if it has been moved or if the ball bounced off on it */
		LCD_SetTextColor(Green);
		if( (old_pos1 != pos1) || ((old_posBallX == 0x14) && (posBallX == 0x15)) || ((old_posBallX == 0x01) && (posBallX == 0x28)) )
		{
			LCD_DrawRect(pos1, 10, 60, 10);
		}
		/* This case redraws the paddle when the ball hits the top or bottom */
		if((old_posBallX == 0x14) && (posBallX == 0x15) && ((old_pos1 == (old_posBallY - BALL_SIZE)) || (old_pos1 == (old_posBallY))) )
		{
			LCD_DrawRect(pos1, 10, 60, 10);
		}
		
		/* Redrawing the ball after each move and if on the server screen */
		LCD_SetTextColor(White);
		/* refreshes only when the ball moves */
		if( (old_posBallY != posBallY) || (old_posBallX != posBallX))
		{
			/* When the ball reaches the limit of the screen, splits the ball on both screen */
			if( (posBallX >= 0x12C) && (posBallX < 0x140) )
			{
				if(posBallX != 0x13F)
				{
					/* draw horizontal lines */
					LCD_DrawLine(posBallX, posBallY, (0x13F - posBallX), LCD_DIR_HORIZONTAL);
					LCD_DrawLine(posBallX, (posBallY + BALL_SIZE), (0x13F - posBallX), LCD_DIR_HORIZONTAL);
				}
				/* draw vertical lines */
				LCD_DrawLine(posBallX, posBallY, BALL_SIZE, LCD_DIR_VERTICAL);
			}
			else if( posBallX < 0x12C )
			{
				LCD_DrawRect(posBallY, posBallX, BALL_SIZE, BALL_SIZE);
			}
		}
		/* Screen flip */
		if (reversed != old_reversed)
		{
			if (!reversed)
			{ 
				/* Clear the other side */
				LCD_Rotate();
				LCD_DisplayChar(Line0, 0, ' ');
				LCD_Rotate();
				/* Write the new score */
				LCD_DisplayChar(Line0, 19, score1+'0');
			}
			else
			{
				if (!old_reversed) /* Clear the other side */
					LCD_DisplayChar(Line0, 19, ' ');
				/* Write the new score */
				LCD_Rotate();
				LCD_DisplayChar(Line0, 0, score1+'0');
				LCD_Rotate();
			}
		}
		/* Score update */
		else if (old_score1 != score1)
		{ 
			if (!reversed)
				LCD_DisplayChar(Line0, 19, score1+'0');
			else
			{
				LCD_Rotate();
				LCD_DisplayChar(Line0, 0, score1+'0');
				LCD_Rotate();
			}
		}
		/* Ball passes through the score */
		else if( (old_posBallY == 0x18) && (posBallY == 0x19) && (!reversed) && (posBallX > 0x100) && (posBallX < 0x180) )
		{
			LCD_DisplayChar(Line0, 19, score1+'0');
		}
    /* Ball passes through the score */
		else if( (old_posBallY == 0xC4) && (posBallY == 0xC3) && (reversed) && (posBallX > 0x100) && (posBallX < 0x180) )
		{
			LCD_Rotate();
			LCD_DisplayChar(Line0, 0, score1+'0');
			LCD_Rotate();
		}
		old_reversed = reversed;
	}	
	old_pos1 = pos1;
	old_pos2 = pos2;
	old_posBallX = posBallX;
	old_posBallY = posBallY;
	old_score1 = score1;
	old_score2 = score2;
}
