/**
  ******************************************************************************
  * @file    BSP/Src/joystick.c
  * @author  MCD Application Team
  * @brief   This example code shows how to use the joystick feature in the
  *          stm32469i_eval driver
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
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static JOYState_TypeDef JoyState = JOY_NONE;
/* Private function prototypes -----------------------------------------------*/
static void Joystick_SetHint(uint8_t exti_vs_gpio);
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Joystick Gpio demo
  * @param  None
  * @retval None
  */
void Joystick_gpio_demo (void)
{

  uint8_t status = 0;

  Joystick_SetHint(0);

  status = BSP_JOY_Init(JOY_MODE_GPIO);

  if (status != IO_OK)
  {
    BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
    BSP_LCD_SetTextColor(LCD_COLOR_RED);
    BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - 95, (uint8_t *)"ERROR", CENTER_MODE);
    BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - 80, (uint8_t *)"Joystick cannot be initialized", CENTER_MODE);
  }

  while (1)
  {
    if (status == IO_OK)
    {
      Joystick_SetCursorPosition();
    }
    if (CheckForUserInput() > 0)
    {
      return;
    }
    HAL_Delay(2);
  }
}

/**
  * @brief  Joystick Exti demo
  * @param  None
  * @retval None
  */
void Joystick_exti_demo (void)
{
  uint8_t status = 0;
  uint32_t ITstatus = 0;

  Joystick_SetHint(1);

  status = BSP_JOY_Init(JOY_MODE_EXTI);

  if (status != IO_OK)
  {
    BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
    BSP_LCD_SetTextColor(LCD_COLOR_RED);
    BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - 95, (uint8_t *)"ERROR", CENTER_MODE);
    BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - 80, (uint8_t *)"Joystick cannot be initialized", CENTER_MODE);
  }

  if (status == IO_OK)
  {
    Joystick_SetCursorPosition();
  }

  while (1)  /* pull for USER button in GPIO mode */
  {
    if (mfx_exti_received == 1)
    {
      mfx_exti_received = 0;
      ITstatus = BSP_IO_ITGetStatus(JOY_ALL_PINS);
      if (ITstatus)
      {
        Joystick_SetCursorPosition();
      }
      BSP_IO_ITClearPin(JOY_ALL_PINS);
    }
    if (CheckForUserInput() > 0)
    {
      return;
    }
    HAL_Delay(2);
  }
}

/**
  * @brief  Joystick cursor position
  * @param  None
  * @retval None
  */
void Joystick_SetCursorPosition(void)
{
  static uint16_t xPtr = 12;
  static uint16_t yPtr = 92;
  static uint16_t old_xPtr = 12;
  static uint16_t old_yPtr = 92;

  /* Get the Joystick State */
  JoyState = BSP_JOY_GetState();

  switch (JoyState)
  {
    case JOY_UP:
      if (yPtr > 92)
      {
        yPtr--;
      }
      break;
    case JOY_DOWN:
      if (yPtr < (BSP_LCD_GetYSize() - 24 - 11))
      {
        yPtr++;
      }
      break;
    case JOY_LEFT:
      if (xPtr > 12)
      {
        xPtr--;
      }
      break;
    case JOY_RIGHT:
      if (xPtr < (BSP_LCD_GetXSize() - 18 - 11))
      {
        xPtr++;
      }
      break;
    default:
      break;
  }

  BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_SetFont(&Font24);

  if (JoyState == JOY_SEL)
  {
    BSP_LCD_SetTextColor(LCD_COLOR_RED);
    BSP_LCD_DisplayChar(xPtr, yPtr, 'X');

  }
  else if (JoyState == JOY_NONE)
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
static void Joystick_SetHint(uint8_t exti_vs_gpio)
{
  /* Clear the LCD */
  BSP_LCD_Clear(LCD_COLOR_WHITE);

  /* Set Joystick Demo description */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_FillRect(0, 0, BSP_LCD_GetXSize(), 80);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_SetBackColor(LCD_COLOR_BLUE);
  BSP_LCD_SetFont(&Font24);
  if (exti_vs_gpio == 0)
  {
    BSP_LCD_DisplayStringAt(0, 0, (uint8_t *)"MFX Joystick Gpio", CENTER_MODE);
  }
  else
  {
    BSP_LCD_DisplayStringAt(0, 0, (uint8_t *)"MFX Joystick Exti", CENTER_MODE);
  }
  BSP_LCD_SetFont(&Font12);
  BSP_LCD_DisplayStringAt(0, 30, (uint8_t *)"Please use the joystick to", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 45, (uint8_t *)"move the pointer inside the", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 60, (uint8_t *)"rectangle", CENTER_MODE);

  /* Set the LCD Text Color */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_DrawRect(10, 90, BSP_LCD_GetXSize() - 20, BSP_LCD_GetYSize() - 100);
  BSP_LCD_DrawRect(11, 91, BSP_LCD_GetXSize() - 22, BSP_LCD_GetYSize() - 102);
}
/**
  * @}
  */

/**
  * @}
  */
