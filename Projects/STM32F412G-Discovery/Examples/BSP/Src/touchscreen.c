/**
  ******************************************************************************
  * @file    BSP/Src/touchscreen.c
  * @author  MCD Application Team
  * @brief   This example code shows how to use the touchscreen driver.
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

/** @addtogroup STM32F7xx_HAL_Examples
  * @{
  */

/** @addtogroup BSP
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define  CIRCLE_RADIUS        37
#define  LINE_LENGHT          30    
/* Private macro -------------------------------------------------------------*/
#define  CIRCLE_XPOS(i)       ((i * BSP_LCD_GetXSize()) / 5)
#define  CIRCLE_YPOS(i)       (BSP_LCD_GetYSize() - CIRCLE_RADIUS - 60)

/* Private Structures and Enumerations ------------------------------------------------------------*/

/* Global variables ---------------------------------------------------------*/
TS_StateTypeDef  TS_State = {0};

/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void     Touchscreen_SetHint_Demo(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Touchscreen Demo : test touchscreen calibration and single touch in polling mode
  * @param  None
  * @retval None
  */
void TS_demo(void)
{
  uint16_t x1, y1;
  uint32_t ts_status = TS_OK;

  /* Touchscreen initialization */
  ts_status = BSP_TS_Init(BSP_LCD_GetXSize(), BSP_LCD_GetYSize());

  if(ts_status == TS_OK)
  {
    /* Display touch screen demo description */
    Touchscreen_SetHint_Demo();

    while (CheckForUserInput() == 0)
    {
        /* Check in polling mode in touch screen the touch status and coordinates */
        /* of touches if touch occurred                                           */
        ts_status = BSP_TS_GetState(&TS_State);
        if(TS_State.touchDetected)
        {
          /* One or dual touch have been detected          */
          /* Only take into account the first touch so far */
          
          /* Get X and Y position of the first touch post calibrated */
          x1 = TS_State.touchX[0];
          y1 = TS_State.touchY[0];
          
          if((y1>83) && (y1<157))
          {
            if ((x1 > 23) && (x1 < 97)) 
            {
              BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
              BSP_LCD_FillCircle(60, 120, CIRCLE_RADIUS);
              BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
              BSP_LCD_FillCircle(180, 120, CIRCLE_RADIUS-1);
              BSP_LCD_FillCircle(60, 200, CIRCLE_RADIUS-1);
              BSP_LCD_FillCircle(180, 200, CIRCLE_RADIUS-1);
            }
            if ((x1 > 143) && (x1 < 217)) 
            {
              BSP_LCD_SetTextColor(LCD_COLOR_RED);
              BSP_LCD_FillCircle(180, 120, CIRCLE_RADIUS);
              BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
              BSP_LCD_FillCircle(60, 120, CIRCLE_RADIUS-1);
              BSP_LCD_FillCircle(60, 200, CIRCLE_RADIUS-1);
              BSP_LCD_FillCircle(180, 200, CIRCLE_RADIUS-1);             
            }           
          }
          if((y1>163) && (y1<237))
          {
            if ((x1 > 23) && (x1 < 97)) 
            {
              BSP_LCD_SetTextColor(LCD_COLOR_YELLOW);
              BSP_LCD_FillCircle(60, 200, CIRCLE_RADIUS);
              BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
              BSP_LCD_FillCircle(180, 120, CIRCLE_RADIUS-1);
              BSP_LCD_FillCircle(60, 120, CIRCLE_RADIUS-1);
              BSP_LCD_FillCircle(180, 200, CIRCLE_RADIUS-1);             
            }
            if ((x1 > 143) && (x1 < 217)) 
            {
              BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
              BSP_LCD_FillCircle(180, 200, CIRCLE_RADIUS);
              BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
              BSP_LCD_FillCircle(180, 120, CIRCLE_RADIUS-1);
              BSP_LCD_FillCircle(60, 120, CIRCLE_RADIUS-1);
              BSP_LCD_FillCircle(60, 200, CIRCLE_RADIUS-1);              
            }           
          }
        }   
    }
    return;  
  }
}

/**
  * @brief  Display TS Demo Hint for all touchscreen demos depending on passed
  *         demoIndex in parameter.
  * @param  demoIndex : parameter of type @ref TouchScreenDemoTypeDef
  * @retval None
  */
static void Touchscreen_SetHint_Demo(void)
{
  /* Clear the LCD */
  BSP_LCD_Clear(LCD_COLOR_WHITE);
  
  /* Set Touchscreen Demo1 description */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_FillRect(0, 0, BSP_LCD_GetXSize(), 80);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_SetBackColor(LCD_COLOR_BLUE);
  BSP_LCD_SetFont(&Font16);
  
  BSP_LCD_DisplayStringAt(0, 0, (uint8_t *)"TS basic polling", CENTER_MODE);
  BSP_LCD_SetFont(&Font12);
  BSP_LCD_DisplayStringAt(0, 30, (uint8_t *)"Touch the screen to activate", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 45, (uint8_t *)"the colored circle inside the", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 60, (uint8_t *)"rectangle. Then press User button", CENTER_MODE);
  
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_DrawCircle(60, 120, CIRCLE_RADIUS); 
  BSP_LCD_SetTextColor(LCD_COLOR_RED);
  BSP_LCD_DrawCircle(180, 120, CIRCLE_RADIUS);
  BSP_LCD_SetTextColor(LCD_COLOR_YELLOW);
  BSP_LCD_DrawCircle(60, 200, CIRCLE_RADIUS); 
  BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
  BSP_LCD_DrawCircle(180, 200, CIRCLE_RADIUS);                
}


/**
  * @}
  */

/**
  * @}
  */
