/**
  ******************************************************************************
  * @file    USB_Host/CDC_Standalone/Src/cdc_send.c 
  * @author  MCD Application Team
  * @brief   CDC Send state machine
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
/* The size of the buffer depends on the user's needs */
#define TX_BUFF_SIZE  (64 * 1024)

/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t CDC_TX_Buffer[TX_BUFF_SIZE];
__IO uint8_t CDC_SendState = 0;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Handles CDC Send Menu. 
  * @param  None
  * @retval None
*/
void CDC_Handle_Send_Menu(void)
{
  switch(CdcDemo.Send_state)
  {
  case CDC_SEND_IDLE:
    CdcDemo.Send_state = CDC_SEND_WAIT;
    CDC_SendState = 0;
    USBH_CDC_Stop(&hUSBHost);
    memset(CDC_TX_Buffer, 0x5A, TX_BUFF_SIZE);
    LCD_LOG_ClearTextZone();
    LCD_DbgLog("Sending data ...\n"); 
    USBH_CDC_Transmit(&hUSBHost, CDC_TX_Buffer, TX_BUFF_SIZE);
    break;
  
  case CDC_SEND_WAIT:
    if(CDC_SendState == 1) /* Data sent */
    {
      BSP_LCD_ClearStringLine(19);
      BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
      BSP_LCD_DisplayStringAtLine(19, (uint8_t *)"Press User button to start receiving data ");
      
      /* Wait for User Input */
      while((BSP_PB_GetState(BUTTON_WAKEUP) != SET) && (Appli_state != APPLICATION_DISCONNECT))
      {
      }
      HAL_Delay(400);
      
      CdcDemo.state = CDC_DEMO_RECEIVE;
      CdcDemo.Send_state = CDC_SEND_IDLE;
    }
    break;
    
  default:
    break;
  }
}

/**
  * @brief  CDC data transmit callback.
  * @param  phost: Host handle
  * @retval None
  */
void USBH_CDC_TransmitCallback(USBH_HandleTypeDef *phost)
{
  LCD_DbgLog (">> Data sent\n" );
  CDC_SendState = 1;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
