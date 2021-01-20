/**
  ******************************************************************************
  * @file    spi.c
  * @brief   This file provides code for the configuration
  *          of the SPI instances.
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

/* Includes ------------------------------------------------------------------*/
#include "spi.h"

/* USER CODE BEGIN 0 */

#include "string.h" 
#include "stdbool.h"


#define RFTRANS_95HF_MAX_BUFFER_SIZE													0x20E
uint8_t				u95HFBuffer [RFTRANS_95HF_MAX_BUFFER_SIZE+3];

bool flagspi2 = false;



/* USER CODE END 0 */

SPI_HandleTypeDef hspi2;

/* SPI2 init function */
void MX_SPI2_Init(void)
{

  hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
  hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_HIGH;
  hspi2.Init.CLKPhase = SPI_PHASE_2EDGE;
  hspi2.Init.NSS = SPI_NSS_SOFT;
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_128;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi2) != HAL_OK)
  {
    Error_Handler();
  }

}

void HAL_SPI_MspInit(SPI_HandleTypeDef* spiHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(spiHandle->Instance==SPI2)
  {
  /* USER CODE BEGIN SPI2_MspInit 0 */

  /* USER CODE END SPI2_MspInit 0 */
    /* SPI2 clock enable */
    __HAL_RCC_SPI2_CLK_ENABLE();

    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**SPI2 GPIO Configuration
    PB13     ------> SPI2_SCK
    PB14     ------> SPI2_MISO
    PB15     ------> SPI2_MOSI
    */
    GPIO_InitStruct.Pin = GPIO_PIN_13|GPIO_PIN_15;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_14;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* USER CODE BEGIN SPI2_MspInit 1 */

  /* USER CODE END SPI2_MspInit 1 */
  }
}

void HAL_SPI_MspDeInit(SPI_HandleTypeDef* spiHandle)
{

  if(spiHandle->Instance==SPI2)
  {
  /* USER CODE BEGIN SPI2_MspDeInit 0 */

  /* USER CODE END SPI2_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_SPI2_CLK_DISABLE();

    /**SPI2 GPIO Configuration
    PB13     ------> SPI2_SCK
    PB14     ------> SPI2_MISO
    PB15     ------> SPI2_MOSI
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15);

  /* USER CODE BEGIN SPI2_MspDeInit 1 */

  /* USER CODE END SPI2_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
void drv95HF_SPIPollingCommand(void);
void drv95HF_ResetSPI(void);
void drv95HF_SendIRQINPulse(void);
int8_t ConfigManager_PORsequence( void );
int8_t  drv95HF_SendReceive(uint8_t *pCommand, uint8_t *pResponse);
void drv95HF_SendSPICommand(uint8_t * pcmd);
void drv95HF_ReceiveSPIResponse(uint8_t * txbuff);

void SPI_SendReceiveBuffer(SPI_HandleTypeDef *hspi, uint8_t * pTxdata, uint8_t *pRxdata ,uint16_t length);


void drv95HF_ResetSPI(void)
{
		uint8_t cmdresetspi = 0x01;
		HAL_GPIO_WritePin(SPI2_SWNSS_GPIO_Port,SPI2_SWNSS_Pin,GPIO_PIN_SET);
		HAL_Delay(1);
		HAL_GPIO_WritePin(SPI2_SWNSS_GPIO_Port,SPI2_SWNSS_Pin,GPIO_PIN_RESET);
		//HAL_Delay(1);
		
		HAL_SPI_Transmit(&hspi2, &cmdresetspi, 1, 0xff);	
		//HAL_SPI_Transmit_IT(&hspi2, &restspicmd, 1);
	
		HAL_GPIO_WritePin(SPI2_SWNSS_GPIO_Port,SPI2_SWNSS_Pin,GPIO_PIN_SET);
		HAL_Delay(3);
	
		drv95HF_SendIRQINPulse();
	
		HAL_Delay(10);
	
	
}

void drv95HF_SendIRQINPulse(void)
{
		HAL_GPIO_WritePin(ST95_IRQ_IN_GPIO_Port,ST95_IRQ_IN_Pin,GPIO_PIN_SET) ;
		HAL_Delay(1);
		HAL_GPIO_WritePin(ST95_IRQ_IN_GPIO_Port,ST95_IRQ_IN_Pin,GPIO_PIN_RESET) ;
		HAL_Delay(1);
		HAL_GPIO_WritePin(ST95_IRQ_IN_GPIO_Port,ST95_IRQ_IN_Pin,GPIO_PIN_SET) ;
		HAL_Delay(10);
}


int8_t ConfigManager_PORsequence( void )
{
	uint8_t cmdecho = 0x55; //ECHO
	//uint8_t reply = 0xff;
	uint16_t timeout = 10; 
	
	
		drv95HF_SendIRQINPulse();	//select SPI or Uart
			
		drv95HF_ResetSPI();
	
	
		HAL_GPIO_WritePin(SPI2_SWNSS_GPIO_Port,SPI2_SWNSS_Pin,GPIO_PIN_SET);
		HAL_Delay(1);
		HAL_GPIO_WritePin(SPI2_SWNSS_GPIO_Port,SPI2_SWNSS_Pin,GPIO_PIN_RESET);
		HAL_Delay(1);
	
	do {
		
		drv95HF_SendReceive(&cmdecho, u95HFBuffer);
		//HAL_SPI_TransmitReceive_IT(&hspi2,&command, &repay, 1);
		
		HAL_Delay(10);
		timeout --;
	}while( ( timeout ) && (u95HFBuffer[0] != 0x55));
	
	
		HAL_GPIO_WritePin(SPI2_SWNSS_GPIO_Port,SPI2_SWNSS_Pin,GPIO_PIN_SET);
		HAL_Delay(1);
		
		if (timeout == 0)
		{
			printf("Echo respone err 0x%02X\r\n",u95HFBuffer[0]);
			return 1;
			
		}
		else
		{
			printf("Echo respone 0x%02x follow timeout=%d\r\n",u95HFBuffer[0] ,timeout);	
			return 0;
		}	
}


//read ID
int8_t ConfigManager_IDN(void)
{
		//uint8_t txcmdtype = 0x00;
		uint8_t rxdata;
		uint8_t cmdID[2] = {0x01,0x00};
		uint8_t datatx[17] = {0xff};
		uint8_t datarx[30] = {0xff};
		uint8_t cnt = 0;
		
		drv95HF_SendReceive(cmdID, datarx);

		
		//memcpy(buff,rxbuff,sizeof(rxbuff));
		
		printf("CMD NID Respone: ");
		for(cnt = 0; cnt < sizeof(datarx);cnt++)
		{
			if(cnt == sizeof(datarx) - 1)
			{
				printf("%02x\r\n",datarx[cnt]);
			}
			else
			{
				printf("%02x ",datarx[cnt]);
			}
			
		}
			
	/* send the command to the PICC and retrieve its response */
	//drv95HF_SendReceive(DataToSend, pResponse);
		return 0;
}

//send cmd 
void drv95HF_SendSPICommand(uint8_t * pcmd)
{
		uint8_t controlbyte = 0x00;
		uint8_t cmdecho = 0x55;
		uint8_t reply = 0xff;
		uint16_t bufferlength = 0;
		uint8_t dummybuff[MAX_BUFFER_SIZE];
		
//		HAL_GPIO_WritePin(SPI2_SWNSS_GPIO_Port,SPI2_SWNSS_Pin,GPIO_PIN_SET);
//		HAL_Delay(1);
		HAL_GPIO_WritePin(SPI2_SWNSS_GPIO_Port,SPI2_SWNSS_Pin,GPIO_PIN_RESET);
		HAL_Delay(1);
		
		//type
		//HAL_SPI_TransmitReceive(&hspi2,&controlbyte,&rxdata,1,0xff);
		HAL_SPI_Transmit(&hspi2,&controlbyte,1,0xff);
		//cmd ID
	if(*pcmd == 0x55)
	{
		/* Send a sending request to xx95HF */ 
		HAL_SPI_TransmitReceive(&hspi2, &cmdecho, &reply, 1,0xff);
		//printf("ECHO reply=%02X\r\n",reply);
	}
	else
	{
			if( pcmd[RFTRANS_95HF_COMMAND_OFFSET] == 0x24 )
			{
				bufferlength = pcmd[RFTRANS_95HF_LENGTH_OFFSET] + RFTRANS_95HF_DATA_OFFSET + 256;
			}
			else if (pcmd[RFTRANS_95HF_COMMAND_OFFSET] == 0x44)
			{
				bufferlength = pcmd[RFTRANS_95HF_LENGTH_OFFSET] + RFTRANS_95HF_DATA_OFFSET + 512;
			}
			else
			{
				bufferlength = pcmd[RFTRANS_95HF_LENGTH_OFFSET] + RFTRANS_95HF_DATA_OFFSET;
			}
	
		SPI_SendReceiveBuffer(&hspi2, pcmd, dummybuff, bufferlength);
		//HAL_SPI_Transmit(&hspi2,pcmd + 1, 1, 0xff);
	
	}
		HAL_GPIO_WritePin(SPI2_SWNSS_GPIO_Port,SPI2_SWNSS_Pin,GPIO_PIN_SET);
		HAL_Delay(1);
}

//wait polling
void drv95HF_SPIPollingCommand(void)
{
	uint8_t txpollcmd = 0x03;
	uint8_t rxdata = 0;
	uint16_t timeout = 500;
	
	if(false)	//polling
	{
		printf("use spi poll\r\n");
		timeout = 500;
		HAL_GPIO_WritePin(SPI2_SWNSS_GPIO_Port,SPI2_SWNSS_Pin,GPIO_PIN_RESET);
		HAL_Delay(1);
		do{
				HAL_SPI_TransmitReceive(&hspi2,&txpollcmd,&rxdata,1,0xff);

				HAL_Delay(2);
				timeout --;
		}while(((rxdata &0x08) != 0x08) && (timeout != 0));
		HAL_GPIO_WritePin(SPI2_SWNSS_GPIO_Port,SPI2_SWNSS_Pin,GPIO_PIN_SET);
		HAL_Delay(1);
		
	}
	else	//interrupt
	{
		printf("use interrupt\r\n");
			timeout = 500;
		 while(flagspi2 == false && timeout != 0)
		 {
				HAL_Delay(2);
				timeout --;
		 }
			  flagspi2 = false;
	}
	
	if(timeout == 0)
	{
		printf("ST95 polling timeout err\r\n");
	}
	else 
	{
		printf("ST95 ready to read data\r\n");
	}
}

//read respone
void drv95HF_ReceiveSPIResponse(uint8_t * pData)
{
		uint8_t DummyBuffer[MAX_BUFFER_SIZE];
		uint8_t cmdrxdata = 0x02;
		uint8_t DUMMY_BYTE = 0xff;
		uint16_t lengthToRead = 0;
	
	
		HAL_GPIO_WritePin(SPI2_SWNSS_GPIO_Port,SPI2_SWNSS_Pin,GPIO_PIN_SET);
		HAL_Delay(1);
		HAL_GPIO_WritePin(SPI2_SWNSS_GPIO_Port,SPI2_SWNSS_Pin,GPIO_PIN_RESET);
		HAL_Delay(1);
	
		//cmd data
		HAL_SPI_Transmit(&hspi2, &cmdrxdata, 1, 0xff);
	
//		pData[RFTRANS_95HF_COMMAND_OFFSET] = SPI_SendReceiveByte(RFTRANS_95HF_SPI, DUMMY_BYTE);
		HAL_SPI_TransmitReceive(&hspi2, &DUMMY_BYTE, pData + RFTRANS_95HF_COMMAND_OFFSET, 1, 0xff);

	if(pData[RFTRANS_95HF_COMMAND_OFFSET] == 0x55)
	{
		pData[RFTRANS_95HF_LENGTH_OFFSET]  = 0x00;
		/* In case we were in listen mode error code cancelled by user (0x85 0x00) must be retrieved */
//		pData[RFTRANS_95HF_LENGTH_OFFSET+1] = SPI_SendReceiveByte(RFTRANS_95HF_SPI, DUMMY_BYTE);
//		pData[RFTRANS_95HF_LENGTH_OFFSET+2] = SPI_SendReceiveByte(RFTRANS_95HF_SPI, DUMMY_BYTE);
		HAL_SPI_TransmitReceive(&hspi2, &DUMMY_BYTE, pData + RFTRANS_95HF_LENGTH_OFFSET + 1, 1, 0xff);
		HAL_SPI_TransmitReceive(&hspi2, &DUMMY_BYTE, pData + RFTRANS_95HF_LENGTH_OFFSET + 2, 1, 0xff);
		
	}
	else if(pData[RFTRANS_95HF_COMMAND_OFFSET] == 0xFF)
	{
		pData[RFTRANS_95HF_LENGTH_OFFSET]  = 0x00;
//		pData[RFTRANS_95HF_LENGTH_OFFSET+1] = SPI_SendReceiveByte(RFTRANS_95HF_SPI, DUMMY_BYTE);
//		pData[RFTRANS_95HF_LENGTH_OFFSET+2] = SPI_SendReceiveByte(RFTRANS_95HF_SPI, DUMMY_BYTE);
		HAL_SPI_TransmitReceive(&hspi2, &DUMMY_BYTE, pData + RFTRANS_95HF_LENGTH_OFFSET + 1, 1, 0xff);
		HAL_SPI_TransmitReceive(&hspi2, &DUMMY_BYTE, pData + RFTRANS_95HF_LENGTH_OFFSET + 2, 1, 0xff);
	}
	else
	{
		/* Recover the "Length" byte */
//		pData[RFTRANS_95HF_LENGTH_OFFSET]  = SPI_SendReceiveByte(RFTRANS_95HF_SPI, DUMMY_BYTE);
		HAL_SPI_TransmitReceive(&hspi2, &DUMMY_BYTE, pData + RFTRANS_95HF_LENGTH_OFFSET, 1, 0xff);
		/* Checks the data length */
		if( !( ( ( pData[RFTRANS_95HF_COMMAND_OFFSET] & 0xE0) == 0x80 ) && (pData[RFTRANS_95HF_LENGTH_OFFSET] == 0x00) ) )
    {
      lengthToRead = (uint16_t)(pData[RFTRANS_95HF_COMMAND_OFFSET] & 0x60);
      lengthToRead = (lengthToRead << 3) + pData[RFTRANS_95HF_LENGTH_OFFSET];
			/* Recover data 	*/
#ifdef USE_DMA
      SPI_SendReceiveBufferDMA(RFTRANS_95HF_SPI, DummyBuffer, lengthToRead, &pData[RFTRANS_95HF_DATA_OFFSET]);    
#else
//      SPI_SendReceiveBuffer(RFTRANS_95HF_SPI, DummyBuffer, lengthToRead, &pData[RFTRANS_95HF_DATA_OFFSET]);
			SPI_SendReceiveBuffer(&hspi2, DummyBuffer, pData + RFTRANS_95HF_DATA_OFFSET, lengthToRead);
#endif	
		}
	}
	
	
		
	
		HAL_GPIO_WritePin(SPI2_SWNSS_GPIO_Port,SPI2_SWNSS_Pin,GPIO_PIN_SET);
		HAL_Delay(1);
}

//send recieved 
int8_t drv95HF_SendReceive(uint8_t *pCommand, uint8_t *pResponse)
{
		
		printf("CMD[%02X]\r\n",pCommand[0]);
		drv95HF_SendSPICommand(pCommand);
		
		//printf("POLL\r\n");
		drv95HF_SPIPollingCommand();
	
		//printf("RESPONE\r\n");
		drv95HF_ReceiveSPIResponse(pResponse);
	
		
}


//spi buff
void SPI_SendReceiveBuffer(SPI_HandleTypeDef *hspi, uint8_t * txbuff, uint8_t* Rxbuff, uint16_t length)
{
		uint16_t i = 0;
		length = MIN(SPI_RESPONSEBUFFER_SIZE,length);
		for(i = 0; i < length; i++)
		{
				HAL_SPI_TransmitReceive(hspi, txbuff + i, Rxbuff + i, 1, 0xff);
		}
}



/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
