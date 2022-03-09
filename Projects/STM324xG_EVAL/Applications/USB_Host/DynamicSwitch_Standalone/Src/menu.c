/**
  ******************************************************************************
  * @file    USB_Host/DynamicSwitch_Standalone/Src/menu.c
  * @author  MCD Application Team
  * @brief   This file implements Menu Functions
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
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
