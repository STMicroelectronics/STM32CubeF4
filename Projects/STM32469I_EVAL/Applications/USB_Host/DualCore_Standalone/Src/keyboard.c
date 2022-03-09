/**
  ******************************************************************************
  * @file    USB_Host/DualCore_Standalone/Src/keyboard.c
  * @author  MCD Application Team
  * @brief   This file implements the HID keyboard functions
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
#define KYBRD_FIRST_COLUMN               (uint16_t)10
#define KYBRD_LAST_COLUMN                (uint16_t)794
#define KYBRD_FIRST_LINE                 (uint16_t) 80
#define SMALL_FONT_COLUMN_WIDTH                    8
#define SMALL_FONT_LINE_WIDTH                      20
#define KYBRD_LAST_LINE                  (uint16_t)320
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern uint8_t *DEMO_KEYBOARD_menu[];
extern uint8_t prev_select;
extern uint32_t hid_demo_ready;
extern DEMO_StateMachine demo;
uint16_t KeybrdCharYpos = 0;
uint16_t KeybrdCharXpos = 0;
uint16_t CurrentLastXpos[KYBRD_LAST_LINE] = {0};
/* Private function prototypes -----------------------------------------------*/
static void USR_KEYBRD_Init(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Handle Keyboard
  * @param  None
  * @retval None
  */
void HID_KeyboardMenuProcess(void)
{
  switch(demo.keyboard_state)
  {
  case HID_KEYBOARD_IDLE:
    demo.keyboard_state = HID_KEYBOARD_START;
    Demo_SelectItem(DEMO_KEYBOARD_menu, 0);
    demo.select = 0;
    break;

  case HID_KEYBOARD_WAIT:
    if(demo.select != prev_select)
    {
      prev_select = demo.select;
      Demo_SelectItem(DEMO_KEYBOARD_menu, demo.select & 0x7F);
      /* Handle select item */
      if(demo.select & 0x80)
      {
        demo.select &= 0x7F;
        switch(demo.select)
        {
        case 0:
          demo.keyboard_state = HID_KEYBOARD_START;
          break;

        case 1: /* return */
          LCD_LOG_ClearTextZone();
          demo.state = DEMO_IDLE;
          LCD_UsrLogY("> HID application closed.\n");
          demo.select = 0;
          break;

        default:
          break;
        }
      }
    }
    break;

  case HID_KEYBOARD_START:
    USR_KEYBRD_Init();
    demo.keyboard_state = HID_KEYBOARD_WAIT;
    break;

  default:
    break;
  }
}

/**
  * @brief  USR_KEYBRD_Init.
  * @param  None
  * @retval None
  */
static void USR_KEYBRD_Init(void)
{
  LCD_LOG_ClearTextZone();
  BSP_LCD_SetTextColor(LCD_COLOR_YELLOW);

  BSP_LCD_DisplayStringAtLine(4, (uint8_t *)"Use Keyboard to type characters:");
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);

  KeybrdCharYpos = KYBRD_FIRST_LINE;
  KeybrdCharXpos = KYBRD_FIRST_COLUMN;
}

/**
  * @brief  Processes Keyboard data.
  * @param  data: Keyboard data to be displayed
  * @retval None
  */
void USR_KEYBRD_ProcessData(uint8_t data)
{
  if(data == '\n')
  {
    KeybrdCharXpos = KYBRD_FIRST_COLUMN;

    /* Increment char Y position */
    KeybrdCharYpos += SMALL_FONT_LINE_WIDTH;

    if(KeybrdCharYpos > KYBRD_LAST_LINE)
    {
      LCD_LOG_ClearTextZone();
      KeybrdCharYpos = KYBRD_FIRST_LINE;
      KeybrdCharXpos = KYBRD_FIRST_COLUMN;
    }
  }
  else if(data == '\r')
  {
    /* Manage deletion of character and update cursor location */
    if(KeybrdCharXpos == KYBRD_FIRST_COLUMN)
    {
      /* First character of first line to be deleted */
      if(KeybrdCharYpos == KYBRD_FIRST_LINE)
      {
        KeybrdCharXpos = KYBRD_FIRST_COLUMN;
      }
      else
      {
        KeybrdCharYpos -= SMALL_FONT_LINE_WIDTH;
        KeybrdCharXpos = (KYBRD_LAST_COLUMN - SMALL_FONT_COLUMN_WIDTH);
      }
    }
    else
    {
      if(CurrentLastXpos[KeybrdCharYpos] > KYBRD_FIRST_COLUMN)
      {
        CurrentLastXpos[KeybrdCharYpos] -= SMALL_FONT_COLUMN_WIDTH;
        KeybrdCharXpos = CurrentLastXpos[KeybrdCharYpos];
      }
      else if(KeybrdCharYpos > KYBRD_FIRST_LINE)
      {
        KeybrdCharYpos -= SMALL_FONT_LINE_WIDTH;
        CurrentLastXpos[KeybrdCharYpos] -= SMALL_FONT_COLUMN_WIDTH;
        KeybrdCharXpos = CurrentLastXpos[KeybrdCharYpos];
      }
      else
      {
      }
    }
    BSP_LCD_DisplayChar(CurrentLastXpos[KeybrdCharYpos], KeybrdCharYpos, ' ');
  }
  else
  {
    /* Update the cursor position on LCD */
    BSP_LCD_DisplayChar(KeybrdCharXpos, KeybrdCharYpos, data);

    /* Increment char X position */
    KeybrdCharXpos += SMALL_FONT_COLUMN_WIDTH;

    CurrentLastXpos[KeybrdCharYpos] = KeybrdCharXpos;
    /* Check if the X position has reached the last column */
    if(KeybrdCharXpos == KYBRD_LAST_COLUMN)
    {
      KeybrdCharXpos = KYBRD_FIRST_COLUMN;

      /* Increment char Y position */
      KeybrdCharYpos += SMALL_FONT_LINE_WIDTH;
    }

    if(KeybrdCharYpos > KYBRD_LAST_LINE)
    {
      LCD_LOG_ClearTextZone();
      KeybrdCharYpos = KYBRD_FIRST_LINE;
      /* Start New Display of the cursor position on LCD */
      BSP_LCD_DisplayChar(KeybrdCharXpos,KeybrdCharYpos, data);
    }
  }
}
