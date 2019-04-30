/**
  ******************************************************************************
  * @file    USB_Host/CDC_Standalone/Src/menu.c 
  * @author  MCD Application Team
  * @brief   This file implements Menu Functions
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

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
CDC_DEMO_StateMachine CdcDemo;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Demo state machine.
  * @param  None
  * @retval None
  */
void Menu_Init(void)
{
  USBH_UsrLog("Starting CDC Demo");
  CdcDemo.state = CDC_DEMO_WAIT;
}

/**
  * @brief  Manages CDC Menu Process.
  * @param  None
  * @retval None
  */
void CDC_MenuProcess(void)
{
  switch(CdcDemo.state)
  {
  case CDC_DEMO_WAIT:
    if(Appli_state == APPLICATION_READY)
    {
      BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
      BSP_LCD_DisplayStringAtLine(19, (uint8_t *)"Press User button to start sending data ");
      
      /* Wait for User Input */
      while((BSP_PB_GetState(BUTTON_WAKEUP) != SET) && (Appli_state != APPLICATION_DISCONNECT))
      {
      }
      CdcDemo.state = CDC_DEMO_SEND;
     
      /* Prevent debounce effect for user key */
      HAL_Delay(400);
      
      BSP_LCD_ClearStringLine(19);
    }
    break;
    
  case CDC_DEMO_SEND:
    /* Read and Write File Here */
    if(Appli_state == APPLICATION_READY)
    {
      CDC_Handle_Send_Menu();
    }
    break; 
    
    case CDC_DEMO_RECEIVE:
    if(Appli_state == APPLICATION_READY)
    {
      CDC_Handle_Receive_Menu();
    }
    break;
    
  default:
    break;
  }
  
  if(Appli_state == APPLICATION_DISCONNECT)
  {
    Appli_state = APPLICATION_IDLE;
    LCD_LOG_ClearTextZone();
    LCD_ErrLog("CDC device disconnected!\n");
    CdcDemo.state = CDC_DEMO_WAIT;
    CdcDemo.Receive_state = CDC_RECEIVE_IDLE;
    CdcDemo.Send_state = CDC_SEND_IDLE;
    CdcDemo.select = 0;
  }
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
