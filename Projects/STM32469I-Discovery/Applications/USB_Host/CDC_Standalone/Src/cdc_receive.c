/**
  ******************************************************************************
  * @file    USB_Host/CDC_Standalone/Src/cdc_receive.c 
  * @author  MCD Application Team
  * @brief   CDC Receive state machine
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
#include "main.h"

/* Private define ------------------------------------------------------------*/
#define RX_BUFF_SIZE   0x400  /* Max Received data 1KB */

/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint16_t xPos, yLinePos;
uint8_t CDC_RX_Buffer[RX_BUFF_SIZE];

/* Private function prototypes -----------------------------------------------*/
static void DumpReceivedData(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Handles CDC Receive Menu. 
  * @param  None
  * @retval None
  */
void CDC_Handle_Receive_Menu(void)
{
  switch(CdcDemo.Receive_state)
  {
  case CDC_RECEIVE_IDLE:
    /* Start Reception */
    LCD_LOG_ClearTextZone();
    CdcDemo.Receive_state = CDC_RECEIVE_WAIT;
    USBH_CDC_Stop(&hUSBHost);
    LCD_DbgLog("Receiving data ...\n");
    
    xPos = 0;
    yLinePos = 5;
    
    memset(CDC_RX_Buffer, 0, RX_BUFF_SIZE); 
    USBH_CDC_Receive(&hUSBHost, CDC_RX_Buffer, RX_BUFF_SIZE);
    
    BSP_LCD_ClearStringLine(19);
    BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
    BSP_LCD_DisplayStringAtLine(19, (uint8_t *)"Press User button to start sending data ");
    break;
    
  case CDC_RECEIVE_WAIT:
    if((BSP_PB_GetState(BUTTON_WAKEUP) == SET) && (Appli_state != APPLICATION_DISCONNECT))
    {
      CdcDemo.state =  CDC_DEMO_WAIT;
      CdcDemo.Receive_state = CDC_RECEIVE_IDLE;
    }
    break;
    
  default:
    break;
  }
}

/**
  * @brief  CDC data receive callback.
  * @param  phost: Host handle
  * @retval None
  */
void USBH_CDC_ReceiveCallback(USBH_HandleTypeDef *phost)
{
  DumpReceivedData();
  USBH_CDC_Receive(&hUSBHost, CDC_RX_Buffer, RX_BUFF_SIZE);
}

/**
  * @brief  Displays received data
  * @param  data: Keyboard data to be displayed
  * @retval None
  */
static void DumpReceivedData(void)
{
  uint16_t size;
  uint8_t *ptr = CDC_RX_Buffer;
  
  size = USBH_CDC_GetLastReceivedDataSize(&hUSBHost);
  BSP_LCD_SetTextColor(LCD_COLOR_YELLOW);
  
  while(size--)
  {
    if((*ptr != '\n') && (*ptr != '\r'))
    { 
      if(*ptr == '\t')
      {
         BSP_LCD_DisplayChar(xPos, LINE(yLinePos), ' ');
      }
      else
      {
        BSP_LCD_DisplayChar(xPos, LINE(yLinePos), *ptr);
      }
      xPos += 10;
    }
    else if(*ptr == '\n')
    {
      yLinePos++;
      xPos = 0;
    }
    
    ptr++;
    
    if(xPos > (BSP_LCD_GetXSize() - 7))
    {
      xPos = 0;
      yLinePos++;
    }
    
    if(yLinePos > 13)
    {
      xPos = 0;
      yLinePos = 5;
    }
  }
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
