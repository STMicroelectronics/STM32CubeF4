/**
  ******************************************************************************
  * @file    BSP/Src/joystick.c 
  * @author  MCD Application Team
  * @brief   This example code shows how to use the joystick feature in the 
  *          stm32412g_discovery driver
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

/** @addtogroup STM32F4xx_HAL_Examples
  * @{
  */

/** @addtogroup BSP
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define HEADBAND_HEIGHT                 64
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static JOYState_TypeDef JoyState = JOY_NONE;
/* Private function prototypes -----------------------------------------------*/
static void Joystick_SetHint(void);
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Joystick Gpio demo
  * @param  None
  * @retval None
  */
void Joystick_demo (void)
{ 

  uint8_t status = 0;

  Joystick_SetHint();
  
  status = BSP_JOY_Init(JOY_MODE_GPIO);
  
  if (status != HAL_OK)
  {
    BSP_LCD_SetBackColor(LCD_COLOR_WHITE); 
    BSP_LCD_SetTextColor(LCD_COLOR_RED);
    BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()- 95, (uint8_t *)"ERROR", CENTER_MODE);
    BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()- 80, (uint8_t *)"Joystick cannot be initialized", CENTER_MODE);
  }
  
  while (1)
  {
    if (status == HAL_OK)
    {
      /* Get the Joystick State */
      JoyState = BSP_JOY_GetState();
      Joystick_SetCursorPosition();
    }
    if(CheckForUserInput() > 0)
    {
      return;
    }
    HAL_Delay(6);
  }
}

/**
  * @brief  Joystick cursor position
  * @param  None
  * @retval None
  */
void Joystick_SetCursorPosition(void)
{
  static uint16_t xPtr = 6;
  static uint16_t yPtr = HEADBAND_HEIGHT + 6;
  static uint16_t old_xPtr = 6;
  static uint16_t old_yPtr = HEADBAND_HEIGHT + 6;

      
  switch(JoyState)
  {
  case JOY_UP:
    if(yPtr > HEADBAND_HEIGHT + 6)
    {
      yPtr--;
    }
    break;
  case JOY_DOWN:
    if(yPtr < (BSP_LCD_GetYSize() - 6 - 11))
    {
      yPtr++;
    }
    break;
  case JOY_LEFT:
    if(xPtr > 6)
    {
      xPtr--;
    }
    break;
  case JOY_RIGHT:
    if(xPtr < (BSP_LCD_GetXSize() - 2 - 11))
    {
      xPtr++;
    }
    break;
  default:
    break;
  }

  BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);

  if(JoyState == JOY_SEL)
  {
    BSP_LCD_SetTextColor(LCD_COLOR_RED);
    BSP_LCD_DisplayChar(xPtr, yPtr, 'X');

  }
  else if(JoyState == JOY_NONE)
  {
    BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
    BSP_LCD_DisplayChar(xPtr, yPtr, 'X');
  }
  else
  {
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    BSP_LCD_DisplayChar(old_xPtr, old_yPtr, 'X');
    BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
    BSP_LCD_DisplayChar(xPtr, yPtr, 'X');

    old_xPtr = xPtr;
    old_yPtr = yPtr;
  }
}



/**
  * @brief  Display joystick demo hint
  * @param  None
  * @retval None
  */
static void Joystick_SetHint(void)
{
  /* Clear the LCD */ 
  BSP_LCD_Clear(LCD_COLOR_WHITE);
  
  /* Set Joystick Demo description */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_FillRect(0, 0, BSP_LCD_GetXSize(), HEADBAND_HEIGHT);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_SetBackColor(LCD_COLOR_BLUE); 
  BSP_LCD_SetFont(&Font16);
  BSP_LCD_DisplayStringAt(0, 1, (uint8_t *)"Joystick", CENTER_MODE);
  BSP_LCD_SetFont(&Font12);
  BSP_LCD_DisplayStringAt(0, 20, (uint8_t *)"Use the joystick", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 35, (uint8_t *)"to move the pointer", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 50, (uint8_t *)"inside the rectangle", CENTER_MODE);
  
  /* Set the LCD Text Color */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);  
  BSP_LCD_DrawRect(5, HEADBAND_HEIGHT + 5, BSP_LCD_GetXSize() - 10, BSP_LCD_GetYSize() - HEADBAND_HEIGHT - 10);
}
/**
  * @}
  */ 

/**
  * @}
  */ 
