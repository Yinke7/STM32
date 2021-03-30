/******************** (C) COPYRIGHT 2014 STMicroelectronics ********************
* File Name          : main.h
* Author             : MMY/SMD Application Team
* Version            : V3.0.0
* Date               : december-2012
* Description        : Hardware Configuration & Setup
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MSD_RW_H
#define __MSD_RW_H

#ifdef USE_MSD_DRIVE

#include "stm32f10x.h"
#include "string.h"
#include "stdio.h"
#include "usb_type.h"
#include "ff.h"
#include "miscellaneous.h"
#include "hw_config.h"
#include "lib_pcd.h"

/* success and error code -------------------------------------------------------------------- */
#define MSD_SUCCESS_CODE											RESULTOK
#define MSD_ERROR_DEFAULT											0x71



FATFS ff;         /* Work area (file system object) for logical drives */
FIL fdft;      /* file objects */
FATFS fs;         /* Work area (file system object) for logical drives */
FIL fdst;      /* file objects */
FRESULT res;
UINT bw, br;
extern FRESULT res;
extern UINT bw, br;



/* external function -------------------------------------------------------------------- */
int8_t MSD_ReadFile		( uint8_t *File_Data, uint16_t *Length, const char *File_Name );
int8_t MSD_WriteFile	( uint8_t *File_Data, uint16_t Length, const char *File_Name );
int8_t MSD_RebootUSB	( void );


int8_t MSD_CopyFileToTag 			( uc8 *pCommand, uint8_t *pResponse );
int8_t MSD_CopyTagToFile			( uc8 *pCommand, uint8_t *pResponse );

#endif /*USE_MSD_DRIVE*/

#endif
