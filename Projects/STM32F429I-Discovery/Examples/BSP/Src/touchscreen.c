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

/** @addtogroup STM32F4xx_HAL_Examples
  * @{
  */

/** @addtogroup BSP
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define  CIRCLE_RADIUS        30

/* Private macro -------------------------------------------------------------*/
#define  CIRCLE_XPOS(i)       ((i * BSP_LCD_GetXSize()) / 5)
#define  CIRCLE_YPOS(i)       (BSP_LCD_GetYSize() - CIRCLE_RADIUS - 60)

/* Private variables ---------------------------------------------------------*/
static TS_StateTypeDef  TS_State;

/* Private function prototypes -----------------------------------------------*/
static void Touchscreen_SetHint(void);
static void Touchscreen_DrawBackground (uint8_t state);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Touchscreen Demo
  * @param  None
  * @retval None
  */
void Touchscreen_demo(void)
{ 
  uint8_t status = 0;
  uint16_t x,y;
  uint8_t state = 0;
  
  if(IsCalibrationDone() == 0)
  {
    Touchscreen_Calibration();
  }
  
  Touchscreen_SetHint();
  
  status = BSP_TS_Init(BSP_LCD_GetXSize(), BSP_LCD_GetYSize());
  
  if (status != TS_OK)
  {
    BSP_LCD_SetBackColor(LCD_COLOR_WHITE); 
    BSP_LCD_SetTextColor(LCD_COLOR_RED);
    BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()- 95, (uint8_t*)"ERROR", CENTER_MODE);
    BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()- 80, (uint8_t*)"Touchscreen cannot be initialized", CENTER_MODE);
  }
  else
  {
    Touchscreen_DrawBackground(state);
  }
  
  while (1)
  {
    if (status == TS_OK)
    {
      BSP_TS_GetState(&TS_State);
      
      if (TS_State.TouchDetected) {
        x = Calibration_GetX(TS_State.X);
        y = Calibration_GetY(TS_State.Y);
        
        if((TS_State.TouchDetected) && 
           (y > (CIRCLE_YPOS(1) - CIRCLE_RADIUS))&&
             (y < (CIRCLE_YPOS(1) + CIRCLE_RADIUS)))
        {
          
          if((x > (CIRCLE_XPOS(1) - CIRCLE_RADIUS))&&
             (x < (CIRCLE_XPOS(1) + CIRCLE_RADIUS)))
          {
            if((state & 1) == 0)
            {
              Touchscreen_DrawBackground(1);
              BSP_LCD_SetTextColor(LCD_COLOR_BLUE); 
              BSP_LCD_FillCircle(CIRCLE_XPOS(1), CIRCLE_YPOS(1), CIRCLE_RADIUS);
              state |= 1;
            }
          }
          if((x > (CIRCLE_XPOS(2) - CIRCLE_RADIUS))&&
             (x < (CIRCLE_XPOS(2) + CIRCLE_RADIUS)))
          {
            if((state & 2) == 0)
            {          
              Touchscreen_DrawBackground(2);
              BSP_LCD_SetTextColor(LCD_COLOR_RED); 
              BSP_LCD_FillCircle(CIRCLE_XPOS(2), CIRCLE_YPOS(2), CIRCLE_RADIUS);
              state |= 2;
            }          
          }
          
          if((x > (CIRCLE_XPOS(3) - CIRCLE_RADIUS))&&
             (x < (CIRCLE_XPOS(3) + CIRCLE_RADIUS)))
          {
            if((state & 4) == 0)
            {           
              Touchscreen_DrawBackground(4);
              BSP_LCD_SetTextColor(LCD_COLOR_YELLOW); 
              BSP_LCD_FillCircle(CIRCLE_XPOS(3), CIRCLE_YPOS(3), CIRCLE_RADIUS);
              state |= 4;
            }            
          }
          
          if((x > (CIRCLE_XPOS(4) - CIRCLE_RADIUS))&&
             (x < (CIRCLE_XPOS(4) + CIRCLE_RADIUS)))
          {
            if((state & 8) == 0)
            {           
              Touchscreen_DrawBackground(8);
              BSP_LCD_SetTextColor(LCD_COLOR_GREEN); 
              BSP_LCD_FillCircle(CIRCLE_XPOS(4), CIRCLE_YPOS(3), CIRCLE_RADIUS);
              state |= 8;
            }           
          }  
          if (state != 0x0F)
          {
            TS_State.TouchDetected = 0;
          }
          else {
            if(CheckForUserInput() > 0)
            {
              return;
            }
          }
        }   
      }
    }
    
    HAL_Delay(100);
  }
}

/**
  * @brief  Display TS Demo Hint
  * @param  None
  * @retval None
  */
static void Touchscreen_SetHint(void)
{
  /* Clear the LCD */ 
  BSP_LCD_Clear(LCD_COLOR_WHITE);
  
  /* Set Touchscreen Demo description */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_FillRect(0, 0, BSP_LCD_GetXSize(), 80);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_SetBackColor(LCD_COLOR_BLUE); 
  BSP_LCD_SetFont(&Font24);
  BSP_LCD_DisplayStringAt(0, 0, (uint8_t*)"Touchscreen", CENTER_MODE);
  BSP_LCD_SetFont(&Font12);
  BSP_LCD_DisplayStringAt(0, 30, (uint8_t*)"Please use the Touchscreen to", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 45, (uint8_t*)"activate the colored circle", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 60, (uint8_t*)"inside the rectangle", CENTER_MODE);
  
  /* Set the LCD Text Color */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);  
  BSP_LCD_DrawRect(10, 90, BSP_LCD_GetXSize() - 20, BSP_LCD_GetYSize()- 100);
  BSP_LCD_DrawRect(11, 91, BSP_LCD_GetXSize() - 22, BSP_LCD_GetYSize()- 102); 
}

/**
  * @brief  Draw Touchscreen Background
  * @param  state: touch zone state
  * @retval None
  */
static void Touchscreen_DrawBackground (uint8_t state)
{
  switch(state)
  {
  case 0:
    BSP_LCD_SetTextColor(LCD_COLOR_BLUE); 
    BSP_LCD_FillCircle(CIRCLE_XPOS(1), CIRCLE_YPOS(1), CIRCLE_RADIUS);
    
    BSP_LCD_SetTextColor(LCD_COLOR_RED); 
    BSP_LCD_FillCircle(CIRCLE_XPOS(2), CIRCLE_YPOS(2), CIRCLE_RADIUS);

    BSP_LCD_SetTextColor(LCD_COLOR_YELLOW); 
    BSP_LCD_FillCircle(CIRCLE_XPOS(3), CIRCLE_YPOS(3), CIRCLE_RADIUS);

    BSP_LCD_SetTextColor(LCD_COLOR_GREEN); 
    BSP_LCD_FillCircle(CIRCLE_XPOS(4), CIRCLE_YPOS(3), CIRCLE_RADIUS);
    
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE); 
    BSP_LCD_FillCircle(CIRCLE_XPOS(1), CIRCLE_YPOS(1), CIRCLE_RADIUS - 2);        
    BSP_LCD_FillCircle(CIRCLE_XPOS(2), CIRCLE_YPOS(2), CIRCLE_RADIUS - 2);
    BSP_LCD_FillCircle(CIRCLE_XPOS(3), CIRCLE_YPOS(3), CIRCLE_RADIUS - 2);
    BSP_LCD_FillCircle(CIRCLE_XPOS(4), CIRCLE_YPOS(3), CIRCLE_RADIUS - 2); 
    break;
    
  case 1:
    BSP_LCD_SetTextColor(LCD_COLOR_BLUE); 
    BSP_LCD_FillCircle(CIRCLE_XPOS(1), CIRCLE_YPOS(1), CIRCLE_RADIUS);
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE); 
    BSP_LCD_FillCircle(CIRCLE_XPOS(1), CIRCLE_YPOS(1), CIRCLE_RADIUS - 2);        
    break;
    
  case 2:
    BSP_LCD_SetTextColor(LCD_COLOR_RED); 
    BSP_LCD_FillCircle(CIRCLE_XPOS(2), CIRCLE_YPOS(2), CIRCLE_RADIUS);
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE); 
    BSP_LCD_FillCircle(CIRCLE_XPOS(2), CIRCLE_YPOS(2), CIRCLE_RADIUS - 2); 
    break;
    
  case 4:
    BSP_LCD_SetTextColor(LCD_COLOR_YELLOW); 
    BSP_LCD_FillCircle(CIRCLE_XPOS(3), CIRCLE_YPOS(3), CIRCLE_RADIUS);
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE); 
    BSP_LCD_FillCircle(CIRCLE_XPOS(3), CIRCLE_YPOS(3), CIRCLE_RADIUS - 2); 
    break;
    
  case 8:
    BSP_LCD_SetTextColor(LCD_COLOR_GREEN); 
    BSP_LCD_FillCircle(CIRCLE_XPOS(4), CIRCLE_YPOS(4), CIRCLE_RADIUS);
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE); 
    BSP_LCD_FillCircle(CIRCLE_XPOS(4), CIRCLE_YPOS(4), CIRCLE_RADIUS - 2); 
    break; 
  }
}

/**
  * @}
  */ 

/**
  * @}
  */ 
