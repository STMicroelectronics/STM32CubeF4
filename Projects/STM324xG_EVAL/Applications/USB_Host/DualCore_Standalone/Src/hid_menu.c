/**
  ******************************************************************************
  * @file    USB_Host/DualCore_Standalone/Src/hid_menu.c
  * @author  MCD Application Team
  * @brief   This file implements HID Menu Functions
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
extern DEMO_StateMachine demo;
extern HID_MOUSE_Info_TypeDef mouse_info;
extern uint8_t prev_select;

uint8_t *DEMO_HID_menu[] =
{
  (uint8_t *)"      1 - Start HID                                                         ",
  (uint8_t *)"      2 - Return                                                            ",
  (uint8_t *)"                                                                            ",
};

uint8_t *DEMO_KEYBOARD_menu[] =
{
  (uint8_t *)"      1 - Start Keyboard / Clear                                            ",
  (uint8_t *)"      2 - Return                                                            ",
  (uint8_t *)"                                                                            ",
};

uint8_t *DEMO_MOUSE_menu[] =
{
  (uint8_t *)"      1 - Start Mouse / Re-Initialize                                       ",
  (uint8_t *)"      2 - Return                                                            ",
  (uint8_t *)"                                                                            ",
};

/* Private function prototypes -----------------------------------------------*/
static void USBH_MouseDemo(USBH_HandleTypeDef *phost);
static void USBH_KeybdDemo(USBH_HandleTypeDef *phost);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Manages HID Menu Process.
  * @param  None
  * @retval None
  */
void HID_MenuProcess(void)
{
  switch(demo.hid_state)
  {
  case APPLI_HID_IDLE:
    Demo_SelectItem(DEMO_HID_menu, 0);
    demo.hid_state = APPLI_HID_WAIT;
    demo.select = 0;
    break;

  case APPLI_HID_WAIT:
    if(demo.select != prev_select)
    {
      prev_select = demo.select;

      Demo_SelectItem(DEMO_HID_menu, demo.select & 0x7F);
      /* Handle select item */
      if(demo.select & 0x80)
      {
        demo.select &= 0x7F;
        switch(demo.select)
        {
        case 0:
          demo.hid_state = APPLI_HID_START;
          break;

        case 1:
          demo.state = DEMO_IDLE;
          demo.select = 0;
          LCD_UsrLogY("> HID application closed.\n");
          break;

        default:
          break;
        }
      }
    }
    break;

  case APPLI_HID_START:
    if(Appli_FS_state == APPLICATION_FS_READY)
    {
      if(USBH_HID_GetDeviceType(&hUSBHost_FS) == HID_KEYBOARD)
      {
        demo.keyboard_state = HID_KEYBOARD_IDLE;
        demo.hid_state = APPLI_HID_KEYBOARD;
      }
      else if(USBH_HID_GetDeviceType(&hUSBHost_FS) == HID_MOUSE)
      {
        demo.mouse_state = HID_MOUSE_IDLE;
        demo.hid_state = APPLI_HID_MOUSE;
      }
    }
    else
    {
      LCD_ErrLog("No supported HID device!\n");
      demo.hid_state = APPLI_HID_WAIT;
    }
    break;

  case APPLI_HID_MOUSE:
    if(Appli_FS_state == APPLICATION_FS_READY)
    {
      HID_MouseMenuProcess();
      USBH_MouseDemo(&hUSBHost_FS);
    }
    break;

  case APPLI_HID_KEYBOARD:
    if(Appli_FS_state == APPLICATION_FS_READY)
    {
      HID_KeyboardMenuProcess();
      USBH_KeybdDemo(&hUSBHost_FS);
    }
    break;

  default:
    break;
  }
}

/**
  * @brief  Main routine for Mouse application
  * @param  phost: Host handle
  * @retval None
  */
static void USBH_MouseDemo(USBH_HandleTypeDef *phost)
{
  HID_MOUSE_Info_TypeDef *m_pinfo;

  m_pinfo = USBH_HID_GetMouseInfo(phost);

  if(m_pinfo != NULL)
  {
    /* Handle Mouse data position */
    USR_MOUSE_ProcessData(&mouse_info);

    if(m_pinfo->buttons[0])
    {
      HID_MOUSE_ButtonPressed(0);
    }
    else
    {
      HID_MOUSE_ButtonReleased(0);
    }

    if( m_pinfo->buttons[1])
    {
      HID_MOUSE_ButtonPressed(2);
    }
    else
    {
      HID_MOUSE_ButtonReleased(2);
    }

    if( m_pinfo->buttons[2])
    {
      HID_MOUSE_ButtonPressed(1);
    }
    else
    {
      HID_MOUSE_ButtonReleased(1);
    }
  }
}

/**
  * @brief  Main routine for Keyboard application
  * @param  phost: Host handle
  * @retval None
  */
static void USBH_KeybdDemo(USBH_HandleTypeDef *phost)
{
  HID_KEYBD_Info_TypeDef *k_pinfo;
  char c;

  k_pinfo = USBH_HID_GetKeybdInfo(phost);

  if(k_pinfo != NULL)
  {
    c = USBH_HID_GetASCIICode(k_pinfo);
    if(c != 0)
    {
      USR_KEYBRD_ProcessData(c);
    }
  }
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
