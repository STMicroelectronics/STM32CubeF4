/**
  ******************************************************************************
  * @file    USB_Host/DynamicSwitch_Standalone/Src/menu.c
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
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Demo state machine.
  * @param  None
  * @retval None
  */
void Menu_Init(void)
{
  msc_demo.state = MSC_DEMO_IDLE;
  hid_demo.state = HID_DEMO_IDLE;
  audio_demo.state = AUDIO_DEMO_IDLE;
}

/**
  * @brief  Manages DS Menu Process.
  * @param  None
  * @retval None
  */
void DS_MenuProcess(void)
{
  switch(Appli_state)
  {
  case APPLICATION_IDLE:
    break;

  case APPLICATION_MSC:
    MSC_MenuProcess();
    break;

  case APPLICATION_AUDIO:
    AUDIO_MenuProcess();
    break;

  case APPLICATION_HID:
    HID_MenuProcess();
    break;

  default:
	break;
  }
  if(Appli_state == APPLICATION_DISCONNECT)
  {
    Appli_state = APPLICATION_IDLE;
    LCD_ErrLog("USB device disconnected!\n");
    Menu_Init();

    LCD_UsrLog("Plug your device To Continue...\n");
  }
}

/**
  * @brief  EXTI line detection callbacks.
  * @param  GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  static JOYState_TypeDef JoyState = JOY_NONE;
  static uint32_t debounce_time = 0;

  if(GPIO_Pin == GPIO_PIN_2)
  {
    /* Get the Joystick State */
    JoyState = BSP_JOY_GetState();

    /* Clear joystick interrupt pending bits */
    BSP_IO_ITClear(JOY_ALL_PINS);

    if(Appli_state == APPLICATION_MSC)
    {
      MSC_DEMO_ProbeKey(JoyState);
    }
    else if(Appli_state == APPLICATION_HID)
    {
      HID_DEMO_ProbeKey(JoyState);
    }
    else if(Appli_state == APPLICATION_AUDIO)
    {
      if(audio_select_mode == AUDIO_SELECT_MENU)
      {
        AUDIO_MenuProbeKey(JoyState);
      }
      else if(audio_select_mode == AUDIO_PLAYBACK_CONTROL)
      {
        AUDIO_PlaybackProbeKey(JoyState);
      }
    }
    switch(JoyState)
    {
    case JOY_LEFT:
      LCD_LOG_ScrollBack();
      break;

    case JOY_RIGHT:
      LCD_LOG_ScrollForward();
      break;

    default:
      break;
    }
  }

  if(audio_demo.state == AUDIO_DEMO_PLAYBACK)
  {
    if(GPIO_Pin == KEY_BUTTON_PIN)
    {
      /* Prevent debounce effect for user key */
      if((HAL_GetTick() - debounce_time) > 50)
      {
        debounce_time = HAL_GetTick();
      }
      else
      {
        return;
      }

      /* Change the selection type */
      if(audio_select_mode == AUDIO_SELECT_MENU)
      {
        Audio_ChangeSelectMode(AUDIO_PLAYBACK_CONTROL);
      }
      else if(audio_select_mode == AUDIO_PLAYBACK_CONTROL)
      {
        Audio_ChangeSelectMode(AUDIO_SELECT_MENU);
      }
    }
  }
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
