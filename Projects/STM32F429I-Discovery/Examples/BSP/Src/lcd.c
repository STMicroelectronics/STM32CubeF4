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
#define LCD_FEATURES_NUM                3

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
void LCD_demo(void)
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
  BSP_LCD_FillRect(0, 0, BSP_LCD_GetXSize(), 80);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_SetBackColor(LCD_COLOR_BLUE); 
  BSP_LCD_SetFont(&Font24);
  BSP_LCD_DisplayStringAt(0, 0, (uint8_t*)"LCD", CENTER_MODE);
  BSP_LCD_SetFont(&Font12);
  BSP_LCD_DisplayStringAt(0, 30, (uint8_t*)"This example shows the different", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 45, (uint8_t*)"LCD Features, use BUTTON", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 60, (uint8_t*)"to display next page", CENTER_MODE);
  
  /* Set the LCD Text Color */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);  
  BSP_LCD_DrawRect(10, 90, BSP_LCD_GetXSize() - 20, BSP_LCD_GetYSize()- 100);
  BSP_LCD_DrawRect(11, 91, BSP_LCD_GetXSize() - 22, BSP_LCD_GetYSize()- 102);
}

/**
  * @brief  Show LCD Features
  * @param  feature: feature index
  * @retval None
  */
static void LCD_Show_Feature(uint8_t feature)
{
  Point Points[]= {{100, 100}, {160, 100}, {160, 140}};
  Point Points2[]= {{100, 150}, {160, 150}, {160, 200}};
  
  BSP_LCD_SetBackColor(LCD_COLOR_WHITE);  
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);    
  BSP_LCD_FillRect(12, 92, BSP_LCD_GetXSize() - 24, BSP_LCD_GetYSize()- 104);
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  
  switch (feature)
  {
  case 0:
    /* Text Feature */
    BSP_LCD_DisplayStringAt(14, 100, (uint8_t*)"Left aligned Text", LEFT_MODE);
    BSP_LCD_DisplayStringAt(0, 115, (uint8_t*)"Center aligned Text", CENTER_MODE);
    BSP_LCD_DisplayStringAt((uint16_t)(-14), 130, (uint8_t*)"Right aligned Text", RIGHT_MODE);
    BSP_LCD_SetFont(&Font24);
    BSP_LCD_DisplayStringAt(14, 180, (uint8_t*)"Font24", LEFT_MODE);
    BSP_LCD_SetFont(&Font20);
    BSP_LCD_DisplayStringAt(BSP_LCD_GetXSize()/2 -20, 180 + Font20.Height, (uint8_t*)"Font20", LEFT_MODE);
    BSP_LCD_SetFont(&Font16);
    BSP_LCD_DisplayStringAt(BSP_LCD_GetXSize() - 80, 180 + Font20.Height + Font16.Height, (uint8_t*)"Font16", LEFT_MODE);
    break;
    
  case 1:
    /* Draw misc. Shapes */
    BSP_LCD_SetTextColor(LCD_COLOR_BLACK); 
    BSP_LCD_DrawRect(20, 100, 60 , 40);
    BSP_LCD_FillRect(20, 150, 60 , 40); 
    
    BSP_LCD_SetTextColor(LCD_COLOR_GRAY); 
    BSP_LCD_DrawCircle(50, 220, 20);
    BSP_LCD_FillCircle(120, 220, 20);
    
    BSP_LCD_SetTextColor(LCD_COLOR_GREEN); 
    BSP_LCD_DrawPolygon(Points, 3);
    BSP_LCD_FillPolygon(Points2, 3);
    
    BSP_LCD_SetTextColor(LCD_COLOR_RED); 
    BSP_LCD_DrawEllipse(BSP_LCD_GetXSize() - 45, 120, 30, 20);
    BSP_LCD_FillEllipse(BSP_LCD_GetXSize() - 45, 170, 30, 20);
    
    BSP_LCD_SetTextColor(LCD_COLOR_BLACK); 
    BSP_LCD_DrawHLine(20, BSP_LCD_GetYSize() - 30, BSP_LCD_GetXSize() / 5);
    BSP_LCD_DrawLine (BSP_LCD_GetXSize() - 150, BSP_LCD_GetYSize()- 20, BSP_LCD_GetXSize()- 20, BSP_LCD_GetYSize()- 50);
    BSP_LCD_DrawLine (BSP_LCD_GetXSize() - 150, BSP_LCD_GetYSize()- 50, BSP_LCD_GetXSize()- 20, BSP_LCD_GetYSize()- 20);
    break;
    
  case 2:
    /* Draw Bitmap */
    BSP_LCD_DrawBitmap(20, 100, (uint8_t *)stlogo);
    HAL_Delay(500);
    
    BSP_LCD_DrawBitmap(BSP_LCD_GetXSize()-100, 100, (uint8_t *)stlogo);
    HAL_Delay(500);
    
    BSP_LCD_DrawBitmap(20, (BSP_LCD_GetYSize()-104-92)/2+100, (uint8_t *)stlogo);
    HAL_Delay(500);    
    
    BSP_LCD_DrawBitmap(BSP_LCD_GetXSize()-100, (BSP_LCD_GetYSize()-104-92)/2+100, (uint8_t *)stlogo);
    HAL_Delay(500);     
    
    BSP_LCD_DrawBitmap(20, BSP_LCD_GetYSize()- 80, (uint8_t *)stlogo);
    HAL_Delay(500);
    
    BSP_LCD_DrawBitmap(BSP_LCD_GetXSize()-100, BSP_LCD_GetYSize()- 80, (uint8_t *)stlogo);
    HAL_Delay(500);    
    break;
  }
}

/**
  * @}
  */ 

/**
  * @}
  */ 
