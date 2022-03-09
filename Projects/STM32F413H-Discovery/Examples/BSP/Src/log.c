/**
  ******************************************************************************
  * @file    BSP/Src/log.c 
  * @author  MCD Application Team
  * @brief   This example code shows how to use the LCD Log firmware functions
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
#include "lcd_log.h"

/** @addtogroup STM32F4xx_HAL_Examples
  * @{
  */

/** @addtogroup BSP
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
typedef enum {
  TS_ACT_NONE = 0,
  TS_ACT_SCROLL_UP,
  TS_ACT_SCROLL_DOWN
}TS_ActionTypeDef;
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
uint32_t tick_1, tick_2;
/**
  * @brief  LCD Log demo 
  * @param  None
  * @retval None
  */
void Log_demo(void)
{ 
   TS_ActionTypeDef ts_action = TS_ACT_NONE;
  uint8_t ts_status = TS_OK;
  uint8_t i = 0;

  /* If calibration is not yet done, proceed with calibration */
  if (TouchScreen_IsCalibrationDone() == 0)
  {
    ts_status = Touchscreen_Calibration();
    if(ts_status == TS_OK)
    {
      BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - 65, (uint8_t *)"Touchscreen calibration success.", CENTER_MODE);
    }
  }
  
  /* Initialize LCD Log module */
  LCD_LOG_Init();

                
  /* Show Header and Footer texts */
  LCD_LOG_SetHeader((uint8_t *)"Log Example");
  LCD_LOG_SetFooter((uint8_t *)"Use touch up/down to scroll");

  /* Output new user logs */
  for (i = 0; i < 16; i++)
  {
    LCD_UsrLog ("This is Line %d \n", i);
  }

  if(ts_status == TS_OK)
  {
    /* Set touchscreen in Interrupt mode and program EXTI accordingly on falling edge of TS_INT pin */
    ts_status = BSP_TS_ITConfig();
    BSP_TEST_APPLI_ASSERT(ts_status != TS_OK);
    Touchscreen_DrawBackground_Circles(32);
    BSP_LCD_SetFont(&Font12);
  }

  /* Check for joystick user input for scroll (back and forward) */
  while (1)
  {
    ts_action = (TS_ActionTypeDef) TouchScreen_GetTouchPosition();

    switch (ts_action)
    {
      case TS_ACT_SCROLL_UP:
        tick_1 = HAL_GetTick();
        LCD_LOG_ScrollBack();
        tick_2 = HAL_GetTick();
        break;
      case TS_ACT_SCROLL_DOWN:
        LCD_LOG_ScrollForward();
        break;

      default:
        break;
    }

    if (CheckForUserInput() > 0)
    {
      return;
    }
  }
}

/**
  * @}
  */ 

/**
  * @}
  */ 
