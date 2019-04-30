/**
  ******************************************************************************
  * @file    USB_Host/HID_Standalone/Src/keyboard.c 
  * @author  MCD Application Team
  * @brief   This file implements the HID keyboard functions
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright © 2017 STMicroelectronics International N.V. 
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
#define KYBRD_FIRST_COLUMN               7
#define KYBRD_LAST_COLUMN                121
#define KYBRD_FIRST_LINE                 50
#define SMALL_FONT_COLUMN_WIDTH          6
#define SMALL_FONT_LINE_WIDTH            10
#define KYBRD_LAST_LINE                  80

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern HID_DEMO_StateMachine hid_demo;
extern uint8_t *DEMO_KEYBOARD_menu[];
extern uint8_t prev_select;
extern uint32_t hid_demo_ready;
uint8_t KeybrdCharXpos = 0;
uint16_t KeybrdCharYpos = 0;

/* Private function prototypes -----------------------------------------------*/
static void USR_KEYBRD_Init(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Manages Keyboard Menu Process.  
  * @param  None
  * @retval None
  */
void HID_KeyboardMenuProcess(void)
{
  switch(hid_demo.keyboard_state)
  {
  case HID_KEYBOARD_IDLE:
    hid_demo.keyboard_state = HID_KEYBOARD_START;
    HID_SelectItem(DEMO_KEYBOARD_menu, 0);   
    hid_demo.select = 0;
    prev_select = 0;       
    break;
    
  case HID_KEYBOARD_WAIT:
    if(hid_demo.select != prev_select)
    {
      prev_select = hid_demo.select ;
      HID_SelectItem(DEMO_KEYBOARD_menu, hid_demo.select & 0x7F);
      /* Handle select item */
      if(hid_demo.select & 0x80)
      {
        hid_demo.select &= 0x7F;
        switch(hid_demo.select)
        {
        case 0: 
          hid_demo.keyboard_state = HID_KEYBOARD_START;
          break;
          
        case 1: /* Return */
          LCD_LOG_ClearTextZone();
          hid_demo.state = HID_DEMO_REENUMERATE;
          hid_demo.select = 0;
          break;
          
        default:
          break;
        }
      }
    }
    break; 
    
  case HID_KEYBOARD_START:
    USR_KEYBRD_Init();   
    hid_demo.keyboard_state = HID_KEYBOARD_WAIT;
    break;  
    
  default:
    break;
  }
}

/**
  * @brief  Init Keyboard window.     
  * @param  None
  * @retval None
  */
static void USR_KEYBRD_Init(void)
{
  LCD_LOG_ClearTextZone();
  BSP_LCD_SetTextColor(LCD_COLOR_YELLOW);
  
  BSP_LCD_DisplayStringAtLine(5, (uint8_t *)"Use Keyboard to type characters:                     "); 
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  
  KeybrdCharXpos = KYBRD_FIRST_LINE;
  KeybrdCharYpos = KYBRD_FIRST_COLUMN;
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
    KeybrdCharYpos = KYBRD_FIRST_COLUMN;
    
    /* Increment char X position */
    KeybrdCharXpos += SMALL_FONT_LINE_WIDTH;
    
    if(KeybrdCharXpos > KYBRD_LAST_LINE)
    {
      LCD_LOG_ClearTextZone();
      KeybrdCharXpos = KYBRD_FIRST_LINE;
      KeybrdCharYpos = KYBRD_FIRST_COLUMN;
    }
  }
    else if(data == '\r')
  {
    /* Manage deletion of character and update cursor location */
    if( KeybrdCharYpos == KYBRD_FIRST_COLUMN) 
    {
      /* First character of first line to be deleted */
      if(KeybrdCharXpos == KYBRD_FIRST_LINE)
      { 
        KeybrdCharYpos = KYBRD_FIRST_COLUMN; 
      }
      else
      {
        KeybrdCharXpos -= SMALL_FONT_LINE_WIDTH;
        KeybrdCharYpos = (KYBRD_LAST_COLUMN - SMALL_FONT_COLUMN_WIDTH); 
      }
    }
    else
    {
      KeybrdCharYpos -= SMALL_FONT_COLUMN_WIDTH; 
    } 
    BSP_LCD_DisplayChar(KeybrdCharYpos, KeybrdCharXpos, ' '); 
  }
  else
  {
    /* Update the cursor position on LCD */
    BSP_LCD_DisplayChar(KeybrdCharYpos, KeybrdCharXpos, data);  
    
    /* Increment char Y position */
    KeybrdCharYpos += SMALL_FONT_COLUMN_WIDTH;
    
    /* Check if the Y position has reached the last column */
    if(KeybrdCharYpos >= KYBRD_LAST_COLUMN)
    {
      KeybrdCharYpos = KYBRD_FIRST_COLUMN;
      
      /* Increment char X position */
      KeybrdCharXpos += SMALL_FONT_LINE_WIDTH;
    }
    
    if(KeybrdCharXpos > KYBRD_LAST_LINE)
    {
      LCD_LOG_ClearTextZone();
      KeybrdCharXpos = KYBRD_FIRST_LINE;
      /* Start New Display of the cursor position on LCD */
      BSP_LCD_DisplayChar(KeybrdCharYpos,KeybrdCharXpos, data);  
    }
  }
}               

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
