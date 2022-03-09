/**
  ******************************************************************************
  * @file    BSP/Src/lcd.c 
  * @author  MCD Application Team
  * @brief   This example code shows how to use LCD drawing features.
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
#define LCD_FEATURES_NUM                4
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static uint8_t LCD_Feature = 0;
/* Private function prototypes -----------------------------------------------*/
static void LCD_SetHint(void);
static void LCD_Show_Feature(uint8_t feature);
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  LCD demo
  * @param  None
  * @retval None
  */
void LCD_demo (void)
{ 
  LCD_SetHint();
  LCD_Feature = 0;
  LCD_Show_Feature (LCD_Feature); 
  
  while (1)
  {
    
    if(CheckForUserInput() > 0)
    {
      if(++LCD_Feature < LCD_FEATURES_NUM)
      {
       LCD_Show_Feature (LCD_Feature); 
      }
      else
      {
        return;
      }
    }
    HAL_Delay(100);
  }
}

/**
  * @brief  Display LCD demo hint
  * @param  None
  * @retval None
  */
static void LCD_SetHint(void)
{
  /* Clear the LCD */ 
  BSP_LCD_Clear(LCD_COLOR_WHITE);

  /* Clear the LCD */ 
  BSP_LCD_Clear(LCD_COLOR_WHITE);
  
  /* Set Joystick Demo description */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_FillRect(0, 0, BSP_LCD_GetXSize(), 55);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_SetBackColor(LCD_COLOR_BLUE); 
  BSP_LCD_SetFont(&Font24);

  BSP_LCD_SetFont(&Font12);
  BSP_LCD_DisplayStringAt(0, 10, (uint8_t *)"LCD Demo", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 25, (uint8_t *)"Use User BUTTON to", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 40, (uint8_t *)"display next page", CENTER_MODE);

  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_SetBackColor(LCD_COLOR_WHITE); 
}

/**
  * @brief  Show LCD Features
  * @param  feature : feature index
  * @retval None
  */
static void LCD_Show_Feature(uint8_t feature)
{
  Point Points[]= {{20, 70}, {60, 70}, {60, 100}};
  Point Points2[]= {{80, 70}, {120, 70}, {120, 100}};
  
  BSP_LCD_SetBackColor(LCD_COLOR_WHITE);  
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);    
  BSP_LCD_FillRect(0, 60, BSP_LCD_GetXSize(), BSP_LCD_GetYSize()- 40);
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);

  switch (feature)
  {
  case 0:
    /* Text Feature */
    BSP_LCD_SetFont(&Font24);
    BSP_LCD_DisplayStringAt(14, 80, (uint8_t *)"Font24", LEFT_MODE); 
    BSP_LCD_SetFont(&Font20);
    BSP_LCD_DisplayStringAt(0, 105, (uint8_t *)"Font20", CENTER_MODE); 
    BSP_LCD_SetFont(&Font16);
    BSP_LCD_DisplayStringAt(14, 130, (uint8_t *)"Font16", RIGHT_MODE); 
    break;
    
  case 1:
    /* Draw misc. Shapes part 1*/
    BSP_LCD_SetTextColor(LCD_COLOR_BLACK); 
    BSP_LCD_DrawRect(20, 70, 40 , 20);
    BSP_LCD_FillRect(70, 70, 40 , 20); 
    
    BSP_LCD_SetTextColor(LCD_COLOR_BLUE); 
    BSP_LCD_DrawCircle(40, 120, 20);
    BSP_LCD_FillCircle(90, 120, 20);
     
    BSP_LCD_SetTextColor(LCD_COLOR_RED); 
    BSP_LCD_DrawLine (20, 70, 20+40 , 70+20);
    BSP_LCD_DrawLine (20, 70+20, 20+40 , 70);
    BSP_LCD_DrawHLine(20, 120, 40);
    BSP_LCD_DrawVLine(40, 100, 40);
    break;
 
  case 2:
    /* Draw misc. Shapes part 2*/
    BSP_LCD_SetTextColor(LCD_COLOR_GREEN); 
    BSP_LCD_DrawPolygon(Points, 3);
    BSP_LCD_FillPolygon(Points2, 3);
    
    BSP_LCD_SetTextColor(LCD_COLOR_RED); 
    BSP_LCD_DrawEllipse(BSP_LCD_GetXSize() - 100, 110, 20, 30);
    BSP_LCD_FillEllipse(BSP_LCD_GetXSize() - 40, 110, 20, 30);
    break;
  
  case 3:
    /* Draw Bitmap */
    BSP_LCD_DrawBitmap(20, 70, (uint8_t *)stlogo);
    HAL_Delay(200);
    break;
  }
}

/**
  * @}
  */ 

/**
  * @}
  */ 
