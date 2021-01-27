/**
  ******************************************************************************
  * @file    spi.h
  * @brief   This file contains all the function prototypes for
  *          the spi.c file
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SPI_H__
#define __SPI_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern SPI_HandleTypeDef hspi2;

/* USER CODE BEGIN Private defines */
	
/* RF transceiver Offset of the command and the response -------------------------------- */
#define RFTRANS_95HF_COMMAND_OFFSET														0x00
#define RFTRANS_95HF_LENGTH_OFFSET														0x01
#define RFTRANS_95HF_DATA_OFFSET															0x02

#define MAX(x,y) 				((x > y)? x : y)
#define MIN(x,y) 				((x < y)? x : y)  
#define ABS(x) 					((x)>0 ? (x) : -(x))  

#define MAX_BUFFER_SIZE  																			528 
#define SPI_RESPONSEBUFFER_SIZE																(uint16_t)528
#define RFTRANS_95HF_MAX_BUFFER_SIZE													0x20E


//lib_PCD



#define PCD_SPI 																		SPI2

/* customs command	 ------------------------------------------------------------------------*/
#define PCD_15693ANTICOL 														0xA0
#define PCD_INVENTORY16SLOTS												0xA1
#define PCD_ISWAKEUP																0xA2
#define PCD_GOTOTAGDETECTINGSTATE										0xA3
#define PCD_CALIBRATETHETAGDETECTION								0xA4
#define PCD_CONFIGTYPEBPRIME                        0xA5
#define PCD_CHAINEDCOMMAND                          0xA6
#define PCD_CHAINEDRESPONSE                         0xA7
#define PCD_UPGRADERECEIVEBUFFER                    0xA8

#define PCD_READCUSTOMTAGMEMORY											0xB0
#define PCD_READMCUBUFFER														0xB1
#define PCD_GETHARDWAREVERSION											0xB2
#define PCD_DOWNLOADSTM32MEM												0xB5
#define PCD_READWHOLEMEMORY													0xB6
#define PCD_TRANSPARENT															0xB7
#define PCD_PULSE_POOLINGREADING	   					 	  	0xB8
#define PCD_PULSE_SPINSS			     					  			0xB9
#define PCD_GETINTERFACEPINSTATE   					  		 	0xBA
#define PCD_SETUSBDISCONNECTSTATE   						  	0xBB
#define PCD_GETMCUVERSION														0xBC
#define PCD_RESETSEQUENCE														0xBD
#define PCD_PULSE_IRQIN															0xBE


#ifdef USE_MSD_DRIVE
	
#define PCD_COPYTAGTOFILE														0xC1
#define PCD_COPYFILETOTAG														0xC2

#endif /*USE_MSD_DRIVE*/

#define PCD_POR																			0xC3
#define PCD_TAG_TRAKING															0xC4
#define PCD_CRYPTO_GENERIC													0xC5
#define PCD_NDEF_MESSAGE														0xC6


#define PCD_MULTITAG																0xD0
#define PCD_MULTITAG_EXTENDED												0xD1

#define SUCCESFUL_SENDS															0x80
#define SUCCESFUL_COMMAND														0x00
#define SUCCESFUL_COMMAND_0x80											0x80
//#define COMMAND_FAILED	

/*  PCD status and erroc code------------------------------------------------------ */
#define PCD_SUCCESSCODE															0x00
#define PCD_ERRORCODE_DEFAULT												0x21
#define PCD_ERRORCODE_PARAMETER											0x22
#define PCD_NOREPLY_CODE														0x23
#define PCD_ERRORCODE_UARTDATARATEUNCHANGED					0x24
#define PCD_ERRORCODE_UARTDATARATEPROCESS						0x25
#define PCD_ERRORCODE_PORERROR											0x26
#define PCD_ERRORCODE_PARAMETERLENGTH								0x27
#define PCD_ERRORCODE_TIMEOUT												0x28




/* CR95HF polling status --------------------------------------------------------- */
#define PCD_IDLERES_LENGTH													0x01
#define PCD_WAKEUPSOURCE_TIMEOUT										0x01
#define PCD_WAKEUPSOURCE_TAGDETECTING								0x02
#define PCD_WAKEUPSOURCE_IRRQINN										0x08



/* Nb of bytes of reader commands --------------------------------------------------------- */
#define SELECT_BUFFER_SIZE													10
#define SENDRECV_BUFFER_SIZE												257
#define SEND_BUFFER_SIZE														257
#define IDLE_BUFFER_SIZE														16
#define RDREG_BUFFER_SIZE														5
#define WRREG_BUFFER_SIZE														257
#define BAUDRATE_BUFFER_SIZE												3
#define SUBFREQRES_BUFFER_SIZE											2
#define ACFILTER_BUFFER_SIZE												19
#define TESTMODE_BUFFER_SIZE												4
#define SLEEPMODE_BUFFER_SIZE												4
/* Nb of bytes of reader response --------------------------------------------------------- */
#define PCD_RESPONSEBUFFER_SIZE		 									RFTRANS_95HF_MAX_BUFFER_SIZE

/* protocol allowed ----------------------------------------------------------------------- */
#define PCD_PROTOCOL_FIELDOFF												0x00
#define PCD_PROTOCOL_ISO15693												0x01
#define PCD_PROTOCOL_ISO14443A											0x02
#define PCD_PROTOCOL_ISO14443B											0x03
#define PCD_PROTOCOL_FELICA													0x04
							

#define PCD_PROTOCOL_ISO14443_SR										0x05
#define PCD_PROTOCOL_TOPAZ													0x06
		
	
/* RF transceiver Offset of the command and the response ---------------------------------- */
#define PCD_COMMAND_OFFSET													RFTRANS_95HF_COMMAND_OFFSET
#define PCD_STATUS_OFFSET														RFTRANS_95HF_COMMAND_OFFSET
#define PCD_LENGTH_OFFSET														RFTRANS_95HF_LENGTH_OFFSET
#define PCD_DATA_OFFSET															RFTRANS_95HF_DATA_OFFSET

/* Command parameter  -------------------------------------------------------------------*/
#define SEND_MASK_APPENDCRC														0x20
#define SEND_MASK_DONTAPPENDCRC												0x00
#define SEND_MASK_8BITSINFIRSTBYTE										0x08



#define PCD_TRANSPARENTCOMMAND_OFFSET								0x01

#define ECHOREPLY_OFFSET														0x00
	
#define PSEUDOREPLY_OFFSET													0x00
#define PSEUDOREPLY_LENGTH													0x01

#define READERREPLY_STATUSOFFSET										0x00

#define IDN_RESULTSCODE_OK													0x00
/* protocol select status  --------------------------------------------------------------------- */
#define PROTOCOLSELECT_LENGTH												0x02
#define PROTOCOLSELECT_RESULTSCODE_OK								0x00
#define PROTOCOLSELECT_ERRORCODE_CMDLENGTH					0x82
#define PROTOCOLSELECT_ERRORCODE_INVALID						0x83
/* baud rate command  -------------------------------------------------------------------------- */
#define	BAUDRATE_LENGTH						  								0x01
#define	BAUDRATE_DATARATE_DEFAULT			  						57600
/* send receive field status  ----------------------------------------------------------------- */
#define SENDRECV_RESULTSCODE_OK											0x80
#define SENDRECV_ERRORCODE_COMERROR									0x86
#define SENDRECV_ERRORCODE_FRAMEWAIT								0x87
#define SENDRECV_ERRORCODE_SOF											0x88
#define SENDRECV_ERRORCODE_OVERFLOW									0x89
#define SENDRECV_ERRORCODE_FRAMING									0x8A
#define SENDRECV_ERRORCODE_EGT											0x8B
#define SENDRECV_ERRORCODE_LENGTH										0x8C
#define SENDRECV_ERRORCODE_CRC											0x8D
#define SENDRECV_ERRORCODE_RECEPTIONLOST						0x8E
#define SENDRECV_RESULTSRESIDUAL										0x90

#define PCD_ERRORCODE_NOTAGFOUND										0x87


#define SENDRECV_ERRORCODE_SOFT											0xFF



/* control byte according to protocol  -------------------------------------------------------- */
#define CONTROL_MAX_NBBYTE													0x03
#define CONTROL_15693_NBBYTE												0x01
#define CONTROL_15693_CRCMASK												0x02
#define CONTROL_15693_COLISIONMASK									0x01

#define ISO14443A_NBBYTE														0x03
#define ISO14443A_COLISIONMASK											0x80
#define ISO14443A_CRCMASK														0x20
#define ISO14443A_CRC_ERRORCODE_TYPEA								0x20
#define ISO14443A_PARITYMASK												0x10
#define ISO14443A_NBSIGNIFICANTBITMASK							0x0F
#define ISO14443A_FIRSTCOLISIONBITMASK							0x0F

#define CONTROL_14443B_NBBYTE												0x01
#define CONTROL_14443B_CRCMASK											0x02
#define CONTROL_14443B_ERRORCODE										0x02
#define CONTROL_14443B_COLISIONMASK									0x01

#define CONTROL_FELICA_NBBYTE												0x01
#define CONTROL_FELICA_CRCMASK											0x02
#define CONTROL_FELICA_COLISIONMASK									0x01

/* Command parameter  -------------------------------------------------------------------*/
#define PCD_ISO14443A_APPENDCRC												0x20
#define PCD_ISO14443A_DONTAPPENDCRC										0x00
#define PCD_ISO14443A_A8BITSINFIRSTBYTE								0x08

/* Speed parameters commom to  ISO14443B protocols --------------------------------------*/
#define PCD_ISO14443B_TRANSMISSION_SPEED_106K  				0x00
#define PCD_ISO14443B_TRANSMISSION_SPEED_212K	 				0x40
#define PCD_ISO14443B_TRANSMISSION_SPEED_424K	 				0x80
#define PCD_ISO14443B_TRANSMISSION_SPEED_848K	 				0xC0

#define PCD_ISO14443B_RECEPTION_SPEED_106K						0x00
#define PCD_ISO14443B_RECEPTION_SPEED_212K		 				0x10
#define PCD_ISO14443B_RECEPTION_SPEED_424K		 				0x20
#define PCD_ISO14443B_RECEPTION_SPEED_848K		 				0x30

#define PCD_ISO14443B_APPEND_CRC							 				0x01

/* Error codes for Higher level */
#define PCDNFC_OK 										0
#define PCDNFC_ERROR 									1
#define PCDNFC_ERROR_MEMORY_TAG 			2
#define PCDNFC_ERROR_MEMORY_INTERNAL 	3
#define PCDNFC_ERROR_LOCKED 					4
#define PCDNFC_ERROR_NOT_FORMATED			5


/* protocol selected for the reader  -----------------------------------------------*/
typedef enum {
 	PCDPROTOCOL_UNKNOWN 	 = 0,
	PCDPROTOCOL_14443A,
	PCDPROTOCOL_14443B,
	PCDPROTOCOL_18092,
	PCDPROTOCOL_15693,
	PCDPROTOCOL_NFCDEPA,
	PCDPROTOCOL_NFCDEPF
}PCD_PROTOCOL;

					 
/* Functions ---------------------------------------------------------------- */
int8_t PCD_IsReaderResultCodeOk 		( uint8_t CmdCode,uint8_t *ReaderReply);
int8_t PCD_IsCRCOk 									( uint8_t Protocol , uint8_t *pReaderReply );

int8_t 	PCD_CheckSendReceive				(uint8_t *pCommand, uint8_t *pResponse);

void PCD_FieldOff								( void );
void PCD_FieldOn 								( void );


int8_t PCD_IDN						( uint8_t *pResponse);
int8_t PCD_Echo						( uint8_t *pResponse);
int8_t PCD_SendEOF				( uint8_t *pResponse);
int8_t PCD_ProtocolSelect	( uint8_t Length,uint8_t Protocol, uint8_t *Parameters, uint8_t *pResponse);
int8_t PCD_SendRecv				( uint8_t Length,uint8_t *Parameters,uint8_t *pResponse);
int8_t PCD_Idle						( uint8_t Length,uint8_t *Data );
int8_t PCD_ReadRegister		( uint8_t Length,uint8_t Address,uint8_t RegCount,uint8_t Flags,uint8_t *pResponse);
int8_t PCD_WriteRegister	( uint8_t Length,uint8_t Address,uint8_t Flags,uint8_t *pData,uint8_t *pResponse);


int8_t PCD_PORsequence				( void );
void PCD_Receive_SPI_Response( uint8_t *pData );

int8_t PCD_TagDetectCalibration (uint8_t Wuperiod,uint8_t *pDacDataH);
int8_t PCD_WaitforTagDetection (uint8_t *WuSource, uint8_t WuPeriod, uint8_t DacDataH, uint8_t NbTrials);

#ifdef CR95HF
int8_t PCD_BaudRate 			( uint8_t BaudRate,uint8_t *pResponse);
int8_t PCD_ChangeUARTBaudRate 		( uint8_t *pCommand, uint8_t *pResponse);
#endif /* CR95HF */




//  Send command field status
#define SEND_RESULTSCODE_OK													0x00
#define SEND_ERRORCODE_LENGTH												0x82
#define SEND_ERRORCODE_PROTOCOL											0x83
/*  Idle command field status ----------------------------------------------------------------- */
#define IDLE_RESULTSCODE_OK													0x00
#define IDLE_ERRORCODE_LENGTH												0x82
/*  read register command field status -------------------------------------------------------- */
#define READREG_RESULTSCODE_OK											0x00
#define READREG_ERRORCODE_LENGTH										0x82
/*  write register command field status ------------------------------------------------------- */
#define WRITEREG_RESULTSCODE_OK											0x00
/*  Baud rate command field status ------------------------------------------------------------ */
#define BAUDRATE_RESULTSCODE_OK											0x55
/*  AC filter command field status ------------------------------------------------------------ */
#define ACFILTER_RESULTSCODE_OK											0x00
#define ACFILTER_ERRORCODE_LENGTH										0x82
/*  sub freq command field status ------------------------------------------------------------- */
#define SUBFREQ_RESULTSCODE_OK											0x00
/*  Test mode command field status ------------------------------------------------------------ */
#define TESTMODE_RESULTSCODE_OK											0x00





/* USER CODE END Private defines */

void MX_SPI2_Init(void);

/* USER CODE BEGIN Prototypes */

int8_t ConfigManager_IDN(void);
int8_t ConfigManager_PORsequence( void );
int8_t SelectProtocol(void);
int8_t AdjustTimerW(void);
int8_t ModulationGain(void);
int8_t ISO14443A_REQA(void);
int8_t ISO14443A_ANTICOL1(uint8_t *puid);
int8_t ISO14443A_SELECT1(uint8_t *puid);
int8_t ISO14443A_Anticollison_Algorithm(void);
int8_t Readtag(void);
int8_t Writetag(void);



int8_t PCD_IsReaderResultCodeOk (uint8_t CmdCode,uint8_t *ReaderReply);



	
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __SPI_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
