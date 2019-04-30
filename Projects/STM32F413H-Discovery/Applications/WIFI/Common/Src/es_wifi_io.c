/**
  ******************************************************************************
  * @file    es_wifi_io.c
  * @author  MCD Application Team
  * @brief   This file implments the IO operations to deal with the es-wifi
  *          module. It mainly Inits and Deinits the SPI interface. Send and
  *          receive data over it.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics International N.V. 
  * All rights reserved.</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "es_wifi_io.h"
#include <string.h>

/* Private define ------------------------------------------------------------*/
#define MIN(a, b)  ((a) < (b) ? (a) : (b))
/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi;

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
                       COM Driver Interface (SPI)
*******************************************************************************/
/**
  * @brief  Initialize SPI MSP
  * @param  hspi: SPI handle
  * @retval None
  */
void SPI_WIFI_MspInit(SPI_HandleTypeDef* hspi)
{
  
  GPIO_InitTypeDef GPIO_Init;
  
  __HAL_RCC_SPI3_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  
  /* configure Wake up pin */
  HAL_GPIO_WritePin( GPIOB, GPIO_PIN_15, GPIO_PIN_RESET ); 
  GPIO_Init.Pin       = GPIO_PIN_15;
  GPIO_Init.Mode      = GPIO_MODE_OUTPUT_PP;
  GPIO_Init.Pull      = GPIO_NOPULL;
  GPIO_Init.Speed     = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init( GPIOB, &GPIO_Init );

  /* configure Data ready pin */
  GPIO_Init.Pin       = GPIO_PIN_12;
  GPIO_Init.Mode      = GPIO_MODE_IT_RISING;
  GPIO_Init.Pull      = GPIO_NOPULL;
  GPIO_Init.Speed     = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init( GPIOG, &GPIO_Init );

  /* configure Reset pin */
  GPIO_Init.Pin       = GPIO_PIN_1;
  GPIO_Init.Mode      = GPIO_MODE_OUTPUT_PP;
  GPIO_Init.Pull      = GPIO_NOPULL;
  GPIO_Init.Speed     = GPIO_SPEED_FREQ_LOW;
  GPIO_Init.Alternate = 0;
  HAL_GPIO_Init( GPIOH, &GPIO_Init );
  
  /* configure SPI NSS pin pin */
  HAL_GPIO_WritePin( GPIOG, GPIO_PIN_11, GPIO_PIN_SET ); 
  GPIO_Init.Pin       = GPIO_PIN_11;
  GPIO_Init.Mode      = GPIO_MODE_OUTPUT_PP;
  GPIO_Init.Pull      = GPIO_NOPULL;
  GPIO_Init.Speed     = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init( GPIOG, &GPIO_Init );
  
  /* configure SPI CLK pin */
  GPIO_Init.Pin       = GPIO_PIN_12;
  GPIO_Init.Mode      = GPIO_MODE_AF_PP;
  GPIO_Init.Pull      = GPIO_NOPULL;
  GPIO_Init.Speed     = GPIO_SPEED_FREQ_MEDIUM;
  GPIO_Init.Alternate = GPIO_AF7_SPI3;
  HAL_GPIO_Init( GPIOB, &GPIO_Init );
  
  /* configure SPI MOSI pin */
  GPIO_Init.Pin       = GPIO_PIN_5;
  GPIO_Init.Mode      = GPIO_MODE_AF_PP;
  GPIO_Init.Pull      = GPIO_NOPULL;
  GPIO_Init.Speed     = GPIO_SPEED_FREQ_MEDIUM;
  GPIO_Init.Alternate = GPIO_AF6_SPI3;
  HAL_GPIO_Init( GPIOB, &GPIO_Init );
  
  /* configure SPI MISO pin */
  GPIO_Init.Pin       = GPIO_PIN_4;
  GPIO_Init.Mode      = GPIO_MODE_AF_PP;
  GPIO_Init.Pull      = GPIO_PULLUP;
  GPIO_Init.Speed     = GPIO_SPEED_FREQ_MEDIUM;
  GPIO_Init.Alternate = GPIO_AF6_SPI3;
  HAL_GPIO_Init( GPIOB, &GPIO_Init );
}

/**
  * @brief  Initialize the SPI3
  * @param  None
  * @retval None
  */
int8_t SPI_WIFI_Init(void)
{
  uint32_t tickstart = HAL_GetTick();
  uint8_t Prompt[6];
  uint8_t count = 0;
  HAL_StatusTypeDef  Status;
  
  hspi.Instance               = SPI3;
  SPI_WIFI_MspInit(&hspi);
  
  hspi.Init.Mode              = SPI_MODE_MASTER;
  hspi.Init.Direction         = SPI_DIRECTION_2LINES;
  hspi.Init.DataSize          = SPI_DATASIZE_16BIT;
  hspi.Init.CLKPolarity       = SPI_POLARITY_LOW;
  hspi.Init.CLKPhase          = SPI_PHASE_1EDGE;
  hspi.Init.NSS               = SPI_NSS_SOFT;
  hspi.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;  /* 50/4= 12.5MHz (Inventek WIFI module supportes up to 20MHz)*/
  hspi.Init.FirstBit          = SPI_FIRSTBIT_MSB;
  hspi.Init.TIMode            = SPI_TIMODE_DISABLE;
  hspi.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE;
  hspi.Init.CRCPolynomial     = 0;
  
  HAL_SPI_Init( &hspi );
  
  WIFI_RESET_MODULE();
  
  WIFI_ENABLE_NSS(); 
  
  while (WIFI_IS_CMDDATA_READY())
  {
    Status = HAL_SPI_Receive(&hspi , &Prompt[count], 1, 0xFFFF);  
    count += 2;
    if(((HAL_GetTick() - tickstart ) > 0xFFFF) || (Status != HAL_OK))
    {
      WIFI_DISABLE_NSS(); 
      return -1;
    }    
  }
  
  if((Prompt[0] != 0x15) ||(Prompt[1] != 0x15) ||(Prompt[2] != '\r')||
       (Prompt[3] != '\n') ||(Prompt[4] != '>') ||(Prompt[5] != ' '))
  {
    WIFI_DISABLE_NSS(); 
    return -1;
  }    
   
  WIFI_DISABLE_NSS(); 
  return 0;
}

/**
  * @brief  Initialize the UART1
  * @param  None
  * @retval None
  */
int8_t SPI_WIFI_DeInit(void)
{
  HAL_SPI_DeInit( &hspi );
  return 0;
}

/**
  * @brief  Receive wifi Data from SPI
  * @param  Pdata : pointer to data
  * @retval Length of received data (payload)
  */
int16_t SPI_WIFI_ReceiveData(uint8_t *pData, uint16_t len, uint32_t timeout)
{
  uint32_t tickstart = HAL_GetTick();
  int16_t length = 0;
   
  WIFI_DISABLE_NSS(); 
  
  while (!WIFI_IS_CMDDATA_READY())
  {
    if((HAL_GetTick() - tickstart ) > timeout)
    {
      return -1;
    }
  }
  
  WIFI_ENABLE_NSS(); 
  
  while (WIFI_IS_CMDDATA_READY())
  {
    if((length < len) || (!len))
    {
      HAL_SPI_Receive(&hspi, pData, 1, timeout) ;
      length += 2;
      pData  += 2;
      
      if((HAL_GetTick() - tickstart ) > timeout)
      {
        WIFI_DISABLE_NSS(); 
        return -1;
      }
    }
    else
    {
      break;
    }
  }
  
  if(*(--pData) == 0x15)
  {
    length--;
  }
  
  WIFI_DISABLE_NSS(); 
  return length;
}

/**
  * @brief  Send wifi Data thru SPI
  * @param  Pdata : pointer to data
  * @retval Length of sent data
  */
int16_t SPI_WIFI_SendData( uint8_t *pdata,  uint16_t len, uint32_t timeout)
{
  uint32_t tickstart = HAL_GetTick();
   
  while (!WIFI_IS_CMDDATA_READY())
  {
    if((HAL_GetTick() - tickstart ) > timeout)
    {
      return -1;
    }
  }
  
  WIFI_ENABLE_NSS(); 
  
  if(len & 0x1) 
  {
    pdata[len] = '\n';
  }
  if( HAL_SPI_Transmit(&hspi, (uint8_t *)pdata , (len+1)/2, timeout) != HAL_OK)
  {
    return -1;
  }
  pdata[len] = 0;  
  return len;
}

/**
  * @brief  Delay
  * @param  Delay in ms
  * @retval None
  */
void SPI_WIFI_Delay(uint32_t Delay)
{
  HAL_Delay(Delay);
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
