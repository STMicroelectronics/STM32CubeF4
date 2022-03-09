/**
  ******************************************************************************
  * @file    USB_Host/HID_RTOS/Src/keyboard.c 
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
#define KYBRD_FIRST_COLUMN               7
#define KYBRD_LAST_COLUMN                121
#define KYBRD_FIRST_LINE                 50
#define SMALL_FONT_COLUMN_WIDTH          6
#define SMALL_FONT_LINE_WIDTH            10
#define KYBRD_LAST_LINE                  120

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern HID_DEMO_StateMachine hid_demo;
extern uint32_t hid_demo_ready;
uint8_t KeybrdCharXpos = 0;
uint16_t KeybrdCharYpos = 0;

/* Private function prototypes -----------------------------------------------*/
static void USR_KEYBRD_Init(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Handles Keyboard Menu.
  * @param  None
  * @retval None
  */
void HID_KeyboardMenuProcess(void)
{
  if(hid_demo.keyboard_state == HID_KEYBOARD_START)
  {
    LCD_LOG_ClearTextZone();
    BSP_LCD_ClearStringLine(18);
    BSP_LCD_ClearStringLine(19);
    USBH_HID_KeybdInit(&hUSBHost);
    USR_KEYBRD_Init();
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
  
  BSP_LCD_DisplayStringAtLine(5, (uint8_t *)"Use Keyboard to type chars:            "); 
  BSP_LCD_DisplayStringAtLine(18, (uint8_t *)"Reset the board to restart            ");  
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
    if(KeybrdCharYpos == KYBRD_FIRST_COLUMN) 
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
