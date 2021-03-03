/**
  ******************************************************************************
  * @file    BSP/Src/joystick.c 
  * @author  MCD Application Team
  * @brief   This example code shows how to use the joystick feature in the 
  *          stm324xx_nucleo_144 driver
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static JOYState_TypeDef JoyState = JOY_NONE;
/* Private function prototypes -----------------------------------------------*/

static void Joystick_SetHint(void);
void Joystick_SetCursorPosition(void);

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
  
  status = BSP_JOY_Init();
  
  if (status != HAL_OK)
  {
    BSP_LCD_SetBackColor(LCD_COLOR_WHITE); 
    BSP_LCD_SetTextColor(LCD_COLOR_RED);
    BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()- 70, (uint8_t *)"ERROR", CENTER_MODE);
    BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()- 55, (uint8_t *)"Joystick cannot", CENTER_MODE);
    BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()- 40, (uint8_t *)"be initialized", CENTER_MODE);
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
  BSP_LCD_FillRect(0, 0, BSP_LCD_GetXSize(), 40);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_SetBackColor(LCD_COLOR_BLUE); 
  BSP_LCD_SetFont(&Font24);

  BSP_LCD_SetFont(&Font12);
  BSP_LCD_DisplayStringAt(0, 10, (uint8_t *)"Joystick Demo", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 25, (uint8_t *)"Move the pointer", CENTER_MODE);

  
  /* Set the LCD Text Color */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);  
  BSP_LCD_DrawRect(10, 50, BSP_LCD_GetXSize() - 20, BSP_LCD_GetYSize()- 60);
  BSP_LCD_DrawRect(11, 51, BSP_LCD_GetXSize() - 22, BSP_LCD_GetYSize()- 62);
}

/**
  * @brief  Joystick cursor position
  * @param  None
  * @retval None
  */
void Joystick_SetCursorPosition(void)
{
  static uint16_t xPtr = 12;
  static uint16_t yPtr = 52;
  static uint16_t old_xPtr = 12;
  static uint16_t old_yPtr = 52;

      
      switch(JoyState)
      {
      case JOY_UP:
        if(yPtr > 52)
        {
          yPtr--;
        }
        break;     
      case JOY_DOWN:
        if(yPtr < (BSP_LCD_GetYSize() - 12 - 11))
        {
          yPtr++;
        }
        break;          
      case JOY_LEFT:
        if(xPtr > 12)
        {
          xPtr--;
        }
        break;         
      case JOY_RIGHT:
        if(xPtr < (BSP_LCD_GetXSize() - 8 - 11))
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
  * @}
  */ 

/**
  * @}
  */ 
  
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
