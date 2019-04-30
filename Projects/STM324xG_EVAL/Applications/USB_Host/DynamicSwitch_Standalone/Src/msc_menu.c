/**
  ******************************************************************************
  * @file    USB_Host/DynamicSwitch_Standalone/Src/menu.c
  * @author  MCD Application Team
  * @brief   This file implements MSC Menu Functions
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
MSC_DEMO_StateMachine msc_demo;
uint8_t prev_select = 0;

uint8_t *MSC_main_menu[] =
{
  (uint8_t *)"      1 - File Operations                                                   ",
  (uint8_t *)"      2 - Explorer Disk                                                     ",
  (uint8_t *)"      3 - Re-Enumerate                                                      ",
};

/* Private function prototypes -----------------------------------------------*/
static void MSC_SelectItem(uint8_t **menu, uint8_t item);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Manages MSC Menu Process.
  * @param  None
  * @retval None
  */
void MSC_MenuProcess(void)
{
  switch(msc_demo.state)
  {
  case MSC_DEMO_IDLE:
    BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
    BSP_LCD_DisplayStringAtLine(14, (uint8_t *)"                                                 ");
    BSP_LCD_DisplayStringAtLine(15, (uint8_t *)"Use [Buttons Left/Right] to scroll up/down       ");
    BSP_LCD_DisplayStringAtLine(16, (uint8_t *)"Use [Joystick Up/Down] to scroll MSC menu        ");
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    MSC_SelectItem(MSC_main_menu, 0);
    msc_demo.state = MSC_DEMO_WAIT;
    msc_demo.select = 0;
    break;

  case MSC_DEMO_WAIT:
    if(msc_demo.select != prev_select)
    {
      prev_select = msc_demo.select;
      MSC_SelectItem(MSC_main_menu, msc_demo.select & 0x7F);

      /* Handle select item */
      if(msc_demo.select & 0x80)
      {
        switch(msc_demo.select & 0x7F)
        {
        case 0:
          msc_demo.state = MSC_DEMO_FILE_OPERATIONS;
          break;

        case 1:
          msc_demo.state = MSC_DEMO_EXPLORER;
          break;

        case 2:
          msc_demo.state = MSC_REENUMERATE;
          break;

        default:
          break;
        }
      }
    }
    break;

  case MSC_DEMO_FILE_OPERATIONS:
    /* Read and Write File Here */
    if(Appli_state == APPLICATION_MSC)
    {
      MSC_File_Operations();
    }
    msc_demo.state = MSC_DEMO_WAIT;
    break;

  case MSC_DEMO_EXPLORER:
    /* Display disk content */
    if(Appli_state == APPLICATION_MSC)
    {
      /* Register the file system object to the FatFs module */
      if(f_mount(&USBH_fatfs, "", 0 ) != FR_OK)
      {
        LCD_ErrLog("Cannot Initialize FatFs! \n");
      }
      else
      {
        Explore_Disk("0:/", 1);
      }
    }
    msc_demo.state = MSC_DEMO_WAIT;
    break;

  case MSC_REENUMERATE:
    /* Force MSC Device to re-enumerate */
    USBH_ReEnumerate(&hUSBHost);
    msc_demo.state = MSC_DEMO_WAIT;

    break;

  default:
    break;
  }
  msc_demo.select &= 0x7F;
}

/**
  * @brief  Manages the menu on the screen.
  * @param  menu: Menu table
  * @param  item: Selected item to be highlighted
  * @retval None
  */
static void MSC_SelectItem(uint8_t **menu, uint8_t item)
{
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);

  switch(item)
  {
  case 0:
    BSP_LCD_SetBackColor(LCD_COLOR_MAGENTA);
    BSP_LCD_DisplayStringAtLine(17, menu[0]);
    BSP_LCD_SetBackColor(LCD_COLOR_BLUE);
    BSP_LCD_DisplayStringAtLine(18, menu[1]);
    BSP_LCD_DisplayStringAtLine(19, menu[2]);
    break;

  case 1:
    BSP_LCD_SetBackColor(LCD_COLOR_BLUE);
    BSP_LCD_DisplayStringAtLine(17, menu[0]);
    BSP_LCD_SetBackColor(LCD_COLOR_MAGENTA);
    BSP_LCD_DisplayStringAtLine(18, menu[1]);
    BSP_LCD_SetBackColor(LCD_COLOR_BLUE);
    BSP_LCD_DisplayStringAtLine(19, menu[2]);
    break;

  case 2:
    BSP_LCD_SetBackColor(LCD_COLOR_BLUE);
    BSP_LCD_DisplayStringAtLine(17, menu[0]);
    BSP_LCD_SetBackColor(LCD_COLOR_BLUE);
    BSP_LCD_DisplayStringAtLine(18, menu[1]);
    BSP_LCD_SetBackColor(LCD_COLOR_MAGENTA);
    BSP_LCD_DisplayStringAtLine(19, menu[2]);
    break;
  }
  BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
}

/**
  * @brief  Probes the MSC joystick state.
  * @param  state: Joystick state
  * @retval None
  */
void MSC_DEMO_ProbeKey(JOYState_TypeDef state)
{
  /* Handle Menu inputs */
  if((state == JOY_UP) && (msc_demo.select > 0))
  {
    msc_demo.select--;
  }
  else if((state == JOY_DOWN) && (msc_demo.select < 2))
  {
    msc_demo.select++;
  }
  else if(state == JOY_SEL)
  {
    msc_demo.select |= 0x80;
  }
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
