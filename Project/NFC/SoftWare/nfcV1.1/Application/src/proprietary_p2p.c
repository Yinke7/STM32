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


static bool UpdateScreen = false;
static bool UpdateBurstConfig = false;
static bool UpdateCMDID = false;
static int CMD_ID = 0;

static bool COM_ON = false; 		

/***************************/
/* Releated to P2P example */
/*    Needed Ressources    */
/***************************/
extern DeviceMode_t 		devicemode;
extern TagType_t 	nfc_tagtype;


/* Function to call application when target receive data 
 must be mapped to dedicated function to check data and provide answer */
// Callback defined in lib_proprietaryP2P.c

// Client and Server
extern uint8_t  (*P2PReceiveCMDCallBack)(uint8_t*pData, uint16_t nbByte);										// Broadcast to current client new received CMD frame
extern uint8_t  (*P2PReceiveDATCallBack)(uint8_t*pData, uint16_t nbByte);										// Broadcast to current client new received DATA frame

//Server
extern void (*P2PAcknowledgedStatus)(bool);

//Client
extern uint8_t  (*P2PPopTXCallBack)(uint8_t*pData, uint16_t *nbByte,pframeType aFrameType); // Retrieve next TX buffer from current client with associated type
extern bool  		(*P2PTxPendingCallBack) ();																									// callback to check if client app need to send something
extern bool 		(*P2PRxAcknowledged)(bool *);


// Defined CallBack from current P2P Client
//Server
static uint8_t handleCommandFromWOD(uint8_t *pData, 	uint16_t size);
static uint8_t handleDataFromWOD(uint8_t *pData, 		uint16_t size);
static void  LastCmdAcknowledged(bool ack);

//Client
static uint8_t handleCommand(uint8_t *pData, 	uint16_t size);
static uint8_t handleData(uint8_t *pData, 		uint16_t size);
static uint8_t PopDataCommand(uint8_t*pData, uint16_t *nbByte,pframeType aFrameType);
static bool	isCommunicationTokenRequested(void);
static bool	isLastCmdAcknoledge(bool * isacknoledged);


void InitiatorCheckForUserAction(void);

// Application Commands Definitions
// Structure [APP_CMD_SIZE (1 byte counting itself)][APP_CMD_SIZE (1 byte)][APP_CMD_PARAMETERS (n bytes, Optional)]
// These COMMAND can be extented following client application needs

#define MIN_CMD_SIZE 						0x02	

#define APP_CMDCODE_BURSTREQ		0x01  // WOD application request a Burst Data
#define APP_CMDCODE_RESET				0x02 	// WOD request a Reset to default Burst value
#define APP_CMDCODE_BURSTCONFIG 0x06	// WOD upadte the burst data config  - BURST CONFIG COMMAND - Payload Struct [NB Bursts - 1 Byte] [Burst Size (1 Byte])
#define APP_CMDCODE_TOGGLE			0x04	// Current Application request Wod to toggle an object (ie : A Sqarre for Android App)
#define APP_CMDCODE_START_CAM		0x05	// Current Application request Wod to start is camera
#define APP_CMDCODE_GET_SENS    0x03  // Current Application request Wod to send Environment Sensor Values
#define APP_CMDCODE_REQ_WIFIPAIRING		0x07	// Request FAKE WIFI Pairing
#define APP_CMDCODE_REQ_FIRMWARE			0x08	// Request FAKE FW 
#define APP_CMDCODE_UNDEF				0xFF  // Undef command code 


uint8_t APP_REQ_BURST 								[]	= 	{0x02,APP_CMDCODE_BURSTREQ};
uint8_t APP_RESET_CMD 								[]	= 	{0x02,APP_CMDCODE_RESET};
uint8_t APP_BURSTCONFIG_CMD 					[]  = 	{0x04,APP_CMDCODE_BURSTCONFIG,10,20}; 
uint8_t APP_TOGGLE_OBJECT							[]  =   {0x03,APP_CMDCODE_TOGGLE,0x00};
uint8_t APP_START_CAM_OBJECT					[]  =   {0x02,APP_CMDCODE_START_CAM};
uint8_t APP_GET_SENS_OBJECT						[]  =   {0x02,APP_CMDCODE_GET_SENS};
uint8_t APP_WIFIPAIRING_OBJECT				[]  =   {0x02,APP_CMDCODE_REQ_WIFIPAIRING};
uint8_t APP_GET_FIRMWARE_OBJECT				[]  =   {0x02,APP_CMDCODE_REQ_FIRMWARE};

struct PP2P_Context {
	
	// - Eval ST95HF Context
	uint8_t EvalST95HFContextUpd; // boolean : Is Context has been updated since last command ?
	uint8_t ledController;				// 0 : led managed by Firmware, 1 : led managed by 
	uint8_t ledStatus; 						// b[0] =  led1, b[1] =  led2,b[2] =  led3,b[3] =  led4,b[4] =  led5
	uint8_t TxburstSize;					// Store size of the burst frame
	uint8_t TxBurstNbFrames;   		// Store nb burst to send in loop
	
	bool ledOn ;
	
	uint32_t sendDataBytes;
	uint8_t	 nbBurstSent;
	uint32_t receivedDataBytes;
	uint8_t	 nbBurstReceived;
	
	// Command ID to send
	uint8_t comID;
	// Last received CMD state
	bool cmdAcknowledge;
	bool acknowledgeRdy;
		
	uint8_t		 burstRequested;
	
} pp2pContext;

  bool  pendingACKCmd;

/***************************/
/*       End of P2P        */
/*    Needed Ressources    */
/***************************/

static void refreshScreen(void);


void refreshTxCMDInfo( int cmdID);

void refreshTxDataInfo( int nBBurst);


void refreshRxCMDInfo( int cmdID);

void refreshRxDataInfo( int nBBurst);


/* Initiator = The board that create the NFC field (PCD) */
/* Target = the board that perform retromodulation to answer (PICC in card emulation mode) */

extern bool 		screenRotated;
extern bool			disableRotate;


void initPP2PContext()
{
	// - Eval ST95HF Context

	pp2pContext.TxburstSize = 0xE0;							// Store size of the burst frame
	pp2pContext.TxBurstNbFrames = 0x0A;   		  // Store nb burst to send in loop
	
	pp2pContext.sendDataBytes = 0;
	pp2pContext.nbBurstSent   = 0;
	pp2pContext.receivedDataBytes = 0;
	pp2pContext.nbBurstReceived	 = 0;
	
	
	pp2pContext.ledOn = false;
	LED_Off(LED5);
	// Command ID to send
	pp2pContext.comID = 0xFF;
	// Last received CMD state
	pp2pContext.cmdAcknowledge = false;
	pp2pContext.acknowledgeRdy = false;
	
	pp2pContext.burstRequested = 0;
}

/**
  * @brief  The client (RX) menu
  */
void ProprietaryP2PConfigClient(void)
{
	u8 NULL_NDEF[2] = {0,0};
	uint8_t Result = 0;
	sAARInfo AAR_struct;
	
	sAARInfo *pAAR;
	
	COM_ON = false;
	UpdateScreen = true;
	
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
	
	/* Prepare to exit loop on Key Press */
	KEYPressed = NOKEY;
	lockKEYUpDown = true;
	
	pendingACKCmd = false;

	LED_Off(LED1);
	
	/*****************************************/
	/* Starting NFC settings for Client part */
	/*****************************************/
	
	pAAR = &AAR_struct;
	
	/* to set only AAR force NDEF size to 0 before */
	NDEF_WriteNDEF(NULL_NDEF);
	
	/* Add the name of the Android application to have an automatic launch of the application */
	//memcpy(pAAR->PakageName, "com.example.pp2papdu", strlen("com.example.pp2papdu")+1);
	memcpy(pAAR->PakageName, "com.nfc.st.demo", strlen("com.nfc.st.demo")+1);
	
	/* Initilialize tag emulation mode */
	devicemode = PICC;
  nfc_tagtype = TT4A;
	
	NDEF_AddAAR(pAAR);
	
	/* Initialize Callback, function called when commands are received by ST95HF */
	P2PTxPendingCallBack = isCommunicationTokenRequested;
	
	
	/* Proprietary P2P Client Callback called on NFC Event*/
	// Callback defined in lib_proprietaryP2P.c
	P2PReceiveCMDCallBack	=	handleCommand;
	P2PReceiveDATCallBack = handleData;
	P2PPopTXCallBack 			= PopDataCommand;
	P2PRxAcknowledged 		= isLastCmdAcknoledge;

	/* Enter a loop that only user can stop with SEL click */
	/* For demonstration purpose allow to perform other actions not rellated to NFC behavior 
		 by recurrent call to the function given in argument */
restart:	 

  /* init and after RF field cut off clear all */
	initPP2PContext();
  
  /* Clear the LCD Screen */
  LCD_Clear(Black);
  
  LCD_DisplayStringLine(Line2, (u8*)"Waiting P2P Server  ");

	if( ConfigManager_P2PP(PP2P_TARGET_NFCA,0) == PP2P_TARGET_NFCA)
	{
		DisableExitButtonAction = true;
		
		/* Connection has been established wait for message to be received */
		Result = ConfigManager_PP2PTarget(CheckForUserAction);
	}
	else	
	{
		/* Initiator not found and task cancelled by user with key press */
		goto end;
	}
	
	/* check exit condition if an error during comm as been detected we can relauch P2P init mode */
	/* But if initiator has cut the field because transmission is done it's normal to exit */
	if( Result != MANAGER_SUCCESSCODE )
	{
		DisableExitButtonAction = false;
		COM_ON = false;		
		LCD_DisplayStringLine(Line2, (u8*)"Communication Error ");
		delay_ms(1000); // field off
		goto restart;
	}
		
	
	/*****************************************/
	/* end of NFC settings for Client part */
	/*****************************************/
	
	/* External Interrupt Disable */
	IntExtOnOffConfig(DISABLE);		
	lockKEYUpDown = false;
	DisableExitButtonAction = false;

end:	
	/* Clear the LCD Screen */
  LCD_Clear(White);
	/* Display Menu */
  DisplayMenu();
}

bool isPendingStreamToSend()
{
	return ((pp2pContext.burstRequested != 0) || (pp2pContext.comID!=APP_CMDCODE_UNDEF));
}

void CheckForUserAction(void)
{
	
	/* Inform user that communication has been established */
	/* This callback is called only when communication is ready */
	if( !COM_ON)
	{
		COM_ON = true;		
    LCD_DisplayStringLine(Line2, (u8*)"Communi. Established");
	}
	
	switch (KEYPressed)
	{
		case DOWN:
			// Send Burst Data
		if (!isPendingStreamToSend())
		   pp2pContext.burstRequested = pp2pContext.TxBurstNbFrames;
			KEYPressed = NOKEY;		
			break;
		case UP: // Visual Demo - demonstrate HW control.. 
			// request to send a command - LED Toggling
			if (!isPendingStreamToSend())
				pp2pContext.comID = APP_CMDCODE_START_CAM;
			KEYPressed = NOKEY;		
			break;
		case RIGHT: // Visual Demo - request WOD to perform action
			// request to send a command - ie . Start Camera
			if (!isPendingStreamToSend())
				pp2pContext.comID = APP_CMDCODE_REQ_FIRMWARE;
			KEYPressed = NOKEY;	
		break;			
		case LEFT: // Use case demonstrating communication in both way
		  // Request Sensor status
			if (!isPendingStreamToSend())
				pp2pContext.comID = APP_CMDCODE_REQ_WIFIPAIRING;
			KEYPressed = NOKEY;		
		break;
		case SEL:
			KEYPressed = NOKEY;		
			break;
    case ROTATE:
      ConfigManager_Stop();
			KEYPressed = NOKEY;		
			break;
	}
		
	if( UpdateScreen == true)
	{
		refreshScreen();
		UpdateScreen = false;
	}
	
	// Check if Command is requested to be acknowledge.
	 if(pendingACKCmd == true)
	 {
		 // simulate last Cmd Acknowledgement
			pp2pContext.cmdAcknowledge = true;
			pp2pContext.acknowledgeRdy = true;
			pendingACKCmd = false;
		 
	 }	
}

/**
  * @brief  The server (CR) menu
  */
void ProprietaryP2PConfigServer(void)
{
  uint8_t Result = 0;
  
	COM_ON = false;
	
	LED_On(LED1);

	/* print screen TagEmulator */
	/* Set the Title Back Color */
  LCD_SetBackColor(Black);

  /* Set the Title Color */
  LCD_SetTextColor(White);
	
	/* Clear the LCD Screen */
  LCD_Clear(Black);
	
	/* Prepare to exit loop on Key Press */
	KEYPressed = NOKEY;
	lockKEYUpDown = true;
	LED_Off(LED1);
	
	P2PAcknowledgedStatus = LastCmdAcknowledged;
	P2PReceiveCMDCallBack	=	handleCommandFromWOD;
	P2PReceiveDATCallBack = handleDataFromWOD;
	
	/* External Interrupt Enable */
	IntExtOnOffConfig(ENABLE);

restart:

	/* init and after RF field cut off clear all */
	initPP2PContext();
	
  /* Clear the LCD Screen */
  LCD_Clear(Black);
  LCD_DisplayStringLine(Line2, (u8*)"Waiting P2P Client  ");
  
	/* Enable ST95HF P2P capabilities */
	if( ConfigManager_P2PP(PP2P_INITIATOR_NFCA,0) == PP2P_INITIATOR_NFCA)
	{	
		DisableExitButtonAction = true;
		Result = ConfigManager_P2PPInitiator(InitiatorCheckForUserAction);
	}
	else
	{
		/* WOD not found and task cancelled by user with key press */
		goto end;
	}
		
	if( Result != MANAGER_SUCCESSCODE )
	{
		DisableExitButtonAction = false;
		COM_ON = false;		
		LCD_DisplayStringLine(Line2, (u8*)"Communication Error ");
		delay_ms(1000); // field off
		goto restart;
	}
	
	/* External Interrupt Disable */
	IntExtOnOffConfig(DISABLE);		
	lockKEYUpDown = false;
	DisableExitButtonAction = false;
end:	
	/* Clear the LCD Screen */
  LCD_Clear(White);
	/* Display Menu */
  DisplayMenu();
}

void InitiatorCheckForUserAction(void)
{
	uint8_t DummyBuffer[256];
	
	if( !COM_ON)
	{
		COM_ON = true;		
		LCD_DisplayStringLine(Line2, (u8*)"Communi. Established");
	}
	
	switch (KEYPressed)
	{
		case DOWN: // Toggle the led
			if( pp2pContext.ledOn == false)
			{
				APP_TOGGLE_OBJECT[2]=0x01;		
				if(PP2P_SendCMD(APP_TOGGLE_OBJECT,3) == PP2P_SUCCESS)
				{	
					KEYPressed = NOKEY;
					pp2pContext.ledOn = true; //FGR must move when ACK received
					pp2pContext.cmdAcknowledge = false; //By default CMD not acknowledged by WOD
					pp2pContext.acknowledgeRdy = false; //Status not received by WOD so far
				}
				//else //CMD not accepted by PP2P interface
					// Nothing to do, KEYPressed keep value down until command is accepted 
			}
			else
			{
				APP_TOGGLE_OBJECT[2]=0x00;		
				if(PP2P_SendCMD(APP_TOGGLE_OBJECT,3) == PP2P_SUCCESS)
				{
					KEYPressed = NOKEY;
					pp2pContext.ledOn = false; //FGR must move when ACK received
					pp2pContext.cmdAcknowledge = false; //By default CMD not acknowledged by WOD
					pp2pContext.acknowledgeRdy = false; //Status not received by WOD so far
				}
				//else //CMD not accepted by PP2P interface
					// Nothing to do, KEYPressed keep value down until command is accepted 
			}
			break;
		case UP: // Send CONFIGURATION
			if(PP2P_SendCMD(APP_BURSTCONFIG_CMD,4) == PP2P_SUCCESS)
			{
				KEYPressed = NOKEY;
				pp2pContext.TxBurstNbFrames = APP_BURSTCONFIG_CMD[2]; //FGR should be done when ack received
				pp2pContext.TxburstSize 		= APP_BURSTCONFIG_CMD[3]; //FGR should be done when ack received
				pp2pContext.cmdAcknowledge = false; //By default CMD not acknowledged by WOD
				pp2pContext.acknowledgeRdy = false; //Status not received by WOD so far
			}
			break;
			
		case RIGHT: // Send Data Buffer
			if( pp2pContext.nbBurstSent < pp2pContext.TxBurstNbFrames)
			{
				DummyBuffer[0] = pp2pContext.TxburstSize;
				if(PP2P_SendDATA(DummyBuffer,(pp2pContext.TxburstSize+1)) == PP2P_SUCCESS)
				{
					pp2pContext.nbBurstSent ++;
					pp2pContext.cmdAcknowledge = false; //By default CMD not acknowledged by WOD
					pp2pContext.acknowledgeRdy = false; //Status not received by WOD so far
				}
				//else /* SERVER is busy or error */
			}
			else
			{
				KEYPressed = NOKEY; // We have sent all the data
				pp2pContext.nbBurstSent = 0;
			}
			break;

    case LEFT:
      if( APP_BURSTCONFIG_CMD[2] < 100 )
      {
        APP_BURSTCONFIG_CMD[2] += 10;
      }
      KEYPressed = NOKEY;
			break;
      
		case SEL:
      if( APP_BURSTCONFIG_CMD[3] < 200 )
      {
        APP_BURSTCONFIG_CMD[3] += 20;
      }
      KEYPressed = NOKEY;
			break;

    case ROTATE:
      ConfigManager_Stop();
			KEYPressed = NOKEY;		
			break;
		
		default:
			break;
	}
}

static uint8_t handleCommandFromWOD(uint8_t *pData, 	uint16_t size)
{
	uint8_t String[20] = "                    ";
	
	if( pData[3] == APP_CMDCODE_START_CAM)
	{
		P2P_SendACK(true);
		memcpy(String,"    CAM STARTED     ",20);
	}		
	else if( pData[3] == APP_CMDCODE_REQ_WIFIPAIRING)
	{
		P2P_SendACK(true);
		memcpy(String," WIFI PAIRING DONE  ",20);
	}		
	else if( pData[3] == APP_CMDCODE_REQ_FIRMWARE)
	{
		P2P_SendACK(true);
		memcpy(String,"  FIRMWARE UPGRADE  ",20);
	}		
	else
	{
		P2P_SendACK(false);
		memcpy(String,"ERR CMD NOT SUPPORT ",20);
	}
	
	LCD_DisplayStringLine(Line1,String);
	
	return PP2P_SUCCESS;
}

static uint8_t handleDataFromWOD(uint8_t *pData, 		uint16_t size)
{
	
		return PP2P_SUCCESS;
}

#if 0
static bool handleAckFromClient(void)
{

			return PP2P_SUCCESS;
}	
#endif

static void LastCmdAcknowledged(bool ack)
{
	if( pp2pContext.acknowledgeRdy == false)
	{
		if( ack == true)
			pp2pContext.cmdAcknowledge = true;
		else
			pp2pContext.cmdAcknowledge = false;
		
		pp2pContext.acknowledgeRdy = true;
	}
	//else
		//should not happened!!!
}

/****************************************** Initiator part *****************************************/


/**************************************** End Initiator part ***************************************/

/******************************************* Target part *******************************************/



/***************************************** End Target part *****************************************/

static void refreshScreen(void)
{
	uint8_t BurstNbFramesString[20] = "                    ";
	uint8_t burstSizeString[20] = "                    ";
	uint8_t String[20] = "                    ";
	
	if( UpdateBurstConfig)
	{
		sprintf((char*)BurstNbFramesString,"Set Burst frame: %2d",pp2pContext.TxBurstNbFrames);
		LCD_DisplayStringLine(Line4,BurstNbFramesString);
		sprintf((char*)burstSizeString,"Set Burst size: %3d",pp2pContext.TxburstSize);
		LCD_DisplayStringLine(Line5,burstSizeString);
		UpdateBurstConfig = false;
	}
	
	if( UpdateCMDID)
	{	
    LCD_DisplayStringLine(Line5,String);
		sprintf((char*)String,"Cmd ID %02d            ",CMD_ID);
		LCD_DisplayStringLine(Line4,String);
    LCD_DisplayStringLine(Line5,(uint8_t *)"                   ");
		UpdateCMDID = false;
	}
}

// Defined CallBack from current P2P Client

/**
 *	@brief  CallBack Method handling command received from Wod.
 *  @param	Data	: pointer on Received payload. Selected protocol dependant
 *  @return PCD_SUCCESSCODE : the command was succedfully send
 *  @return PCD_ERRORCODE_DEFAULT : the PCD device returned an error code
 *  @return PCD_ERRORCODE_PARAMETERLENGTH : Length parameter is erroneous
 */

uint8_t handleCommand(uint8_t *pData, uint16_t size)
{
	if (size<MIN_CMD_SIZE)
	{
		return PP2P_ERRORCODE_DEFAULT;
	}
	else
		switch( pData[2])
		{
			case APP_CMDCODE_BURSTREQ: // WOD APP request a burst
				pp2pContext.burstRequested = pp2pContext.TxBurstNbFrames;
				pendingACKCmd = true;
				break;
			case APP_CMDCODE_RESET: // WOD APP request a reset of burst process
				pp2pContext.sendDataBytes = 0;
				pp2pContext.nbBurstSent 	= 0;
			  pp2pContext.cmdAcknowledge = true;
				pp2pContext.acknowledgeRdy = true;
				break;
			case APP_CMDCODE_BURSTCONFIG:
					pp2pContext.TxBurstNbFrames = pData[3];
					pp2pContext.TxburstSize 		= pData[4];
					pp2pContext.cmdAcknowledge = true;
					pp2pContext.acknowledgeRdy = true;
					UpdateScreen = true;
			    UpdateBurstConfig = true;
				break;
			case APP_CMDCODE_TOGGLE:
				if (pData[3]==0x01)
				{
					pp2pContext.ledOn = true;
					LED_On(LED5);
				}
				else 
				{
					pp2pContext.ledOn = false;
					LED_Off(LED5);
				}
				pp2pContext.cmdAcknowledge = true;
				pp2pContext.acknowledgeRdy = true;
				break;
				
			default:
				// CMD Not Yet Supported
					pp2pContext.cmdAcknowledge = false;
					pp2pContext.acknowledgeRdy = true;
		}
		return PP2P_SUCCESS;
};

uint8_t handleData(uint8_t *pData, uint16_t size)
{
	if (size==0)
	{
		return PP2P_ERRORCODE_DEFAULT;
	}
	else
	{
		// To Be Done : Data could be store here in dedicated buffer for futher treatment
		
		// We keep trace of received data
		pp2pContext.receivedDataBytes += size;
		pp2pContext.nbBurstReceived++;
		refreshRxDataInfo(pp2pContext.nbBurstReceived);
	  return PP2P_SUCCESS;
	}
};

uint8_t PopDataCommand(uint8_t*pData, uint16_t *nbByte,pframeType aFrameType)
{

	// if data need to send : Pop data from array or list and request to PP2P agent to send this data
	if (pp2pContext.burstRequested!=0)
		{
			*nbByte = pp2pContext.TxburstSize;
			 memset(pData,0xAD, pp2pContext.TxburstSize);
			*aFrameType = DATA_TYPE;
			pp2pContext.sendDataBytes += pp2pContext.TxburstSize;
			pp2pContext.burstRequested--;
			refreshTxDataInfo(pp2pContext.TxBurstNbFrames - pp2pContext.burstRequested);
			return *nbByte;
		}
		else if (pp2pContext.comID != 0xFF)
		{
		// Pop the next command to send
		
			switch (pp2pContext.comID)
			{
				case APP_CMDCODE_REQ_WIFIPAIRING: // A command use to toggle the Smartphone Led
								*nbByte = APP_WIFIPAIRING_OBJECT[0];
								memcpy(pData,APP_WIFIPAIRING_OBJECT, APP_WIFIPAIRING_OBJECT[0]);
								*aFrameType = STD_CMD_TYPE;
								refreshTxCMDInfo(APP_CMDCODE_REQ_WIFIPAIRING);
								pp2pContext.comID = APP_CMDCODE_UNDEF;
								return *nbByte;
//					break;
			  case APP_CMDCODE_START_CAM: // A command use to Start CAMERA
								*nbByte = APP_START_CAM_OBJECT[0];
								memcpy(pData,APP_START_CAM_OBJECT, APP_START_CAM_OBJECT[0]);
								*aFrameType = STD_CMD_TYPE;
								refreshTxCMDInfo(APP_CMDCODE_START_CAM);
								pp2pContext.comID = APP_CMDCODE_UNDEF;
								return *nbByte;
//					break;
				case APP_CMDCODE_REQ_FIRMWARE: // A command use to Start CAMERA
								*nbByte = APP_GET_FIRMWARE_OBJECT[0];
								memcpy(pData,APP_GET_FIRMWARE_OBJECT, APP_GET_FIRMWARE_OBJECT[0]);
								*aFrameType = STD_CMD_TYPE;
								refreshTxCMDInfo(APP_CMDCODE_REQ_FIRMWARE);
								pp2pContext.comID = APP_CMDCODE_UNDEF;
								return *nbByte;
//		     break;
				default:
					// unsupported command.
				{
					
					*aFrameType = UNDEF_TYPE;
					*nbByte = 0;
					pp2pContext.comID = APP_CMDCODE_UNDEF;
					return 0;
				}
			}
		}
	else
	{
		*aFrameType = UNDEF_TYPE;
		*nbByte = 0;
		return 0;
	}

}
	

bool isCommunicationTokenRequested(void)
{
	return !((pp2pContext.burstRequested==0) && (pp2pContext.comID==0xFF) );
}

bool	isLastCmdAcknoledge(bool * isacknoledged)
{
	if (pp2pContext.acknowledgeRdy)
	{
		*isacknoledged = pp2pContext.cmdAcknowledge;
		pp2pContext.acknowledgeRdy = false; // clear acknowledge
		return true;
	}
	return false;
}


void refreshTxCMDInfo( int cmdID)
{
	CMD_ID = cmdID;
	UpdateScreen = true;
	UpdateCMDID = true;
		
}

void refreshTxDataInfo( int nBBurst)
{
	uint8_t String[20] = "                    ";
	sprintf((char*)String,"Tx Burst :%02d",nBBurst);
	LCD_DisplayStringLine(Line7,String);
} 


void refreshRxCMDInfo( int cmdID)
{
	uint8_t String[20] = "Cmd ID              ";
	sprintf((char*)String,"Rx Cmd Id : %02d",cmdID);
	LCD_DisplayStringLine(Line7,String);
}

void refreshRxDataInfo( int nBBurst)
{
	uint8_t String[20] = "                    ";
	sprintf((char*)String,"Rx Burst :%02d",nBBurst);
	LCD_DisplayStringLine(Line8,String);
} 
