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
#include "image_f4.h"

/** @addtogroup STM32F4xx_HAL_Examples
  * @{
  */

/** @addtogroup BSP
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define LCD_FEATURES_NUM                4
#define HEADBAND_HEIGHT                 64
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
  
  /* Set LCD Demo description */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_FillRect(0, 0, BSP_LCD_GetXSize(), HEADBAND_HEIGHT);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_SetBackColor(LCD_COLOR_BLUE); 
  BSP_LCD_SetFont(&Font16);
  BSP_LCD_DisplayStringAt(0, 1, (uint8_t *)"LCD", CENTER_MODE);
  BSP_LCD_SetFont(&Font12);
  BSP_LCD_DisplayStringAt(0, 20, (uint8_t *)"This example shows LCD", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 35, (uint8_t *)"features, press button", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 50, (uint8_t *)"to display next page", CENTER_MODE);
 }

/**
  * @brief  Show LCD Features
  * @param  feature : feature index
  * @retval None
  */
static void LCD_Show_Feature(uint8_t feature)
{
  Point Points[]= {{20, 120}, {80, 120}, {80, 170}};
  Point Points2[]= {{100, 120}, {160, 120}, {160, 170}};
  
  BSP_LCD_SetBackColor(LCD_COLOR_WHITE);  
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_FillRect(0, HEADBAND_HEIGHT, BSP_LCD_GetXSize(), BSP_LCD_GetYSize() - HEADBAND_HEIGHT);
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);

  switch (feature)
  {
  case 0:
    /* Text Feature */
    
    BSP_LCD_DisplayStringAt(0, 70, (uint8_t *)"Left aligned Text", LEFT_MODE);
    BSP_LCD_DisplayStringAt(0, 85, (uint8_t *)"Center aligned Text", CENTER_MODE);
    BSP_LCD_DisplayStringAt(0, 100, (uint8_t *)"Right aligned Text", RIGHT_MODE);
    BSP_LCD_SetFont(&Font24);
    BSP_LCD_DisplayStringAt(0, 118, (uint8_t *)"Font24", CENTER_MODE);
    BSP_LCD_SetFont(&Font20);
    BSP_LCD_DisplayStringAt(0, 140, (uint8_t *)"Font20", CENTER_MODE);
    BSP_LCD_SetFont(&Font16);
    BSP_LCD_DisplayStringAt(0, 160, (uint8_t *)"Font16", CENTER_MODE);
    break;
    
  case 1:
    /* Draw misc. Shapes */
    BSP_LCD_SetTextColor(LCD_COLOR_GRAY); 
    BSP_LCD_DrawCircle(BSP_LCD_GetXSize() - 120, 90, 20);
    BSP_LCD_FillCircle(BSP_LCD_GetXSize() - 40, 90, 20);
    
    BSP_LCD_SetTextColor(LCD_COLOR_GREEN); 
    BSP_LCD_DrawPolygon(Points, 3);
    BSP_LCD_FillPolygon(Points2, 3);
    break;

  case 2:
    /* Draw other misc. Shapes */
    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
    BSP_LCD_DrawRect(20, 70, 60 , 40);
    BSP_LCD_FillRect(100, 70, 60 , 40);

    BSP_LCD_SetTextColor(LCD_COLOR_RED); 
    BSP_LCD_DrawEllipse(50, 140, 30, 20);
    BSP_LCD_FillEllipse(BSP_LCD_GetXSize() - 50, 140, 30, 20);
    
    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
    BSP_LCD_DrawHLine(20, BSP_LCD_GetYSize() - 5, BSP_LCD_GetXSize() - 40);
    BSP_LCD_DrawVLine(BSP_LCD_GetXSize()/2, HEADBAND_HEIGHT + 5, BSP_LCD_GetYSize() - HEADBAND_HEIGHT - 10);
    BSP_LCD_DrawLine (20, BSP_LCD_GetYSize()- 20, BSP_LCD_GetXSize()- 20, BSP_LCD_GetYSize()- 60);
    BSP_LCD_DrawLine (20, BSP_LCD_GetYSize()- 60, BSP_LCD_GetXSize()- 20, BSP_LCD_GetYSize()- 20);
    break;
    
  case 3:
    /* Draw Bitmap */
    BSP_LCD_DrawBitmap(0, 0, (uint8_t *)ImageF4);
    break;
  }
}

/**
  * @}
  */ 

/**
  * @}
  */ 
