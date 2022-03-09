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

typedef enum
{
  LCD_FEAT_TEXT                = 0x00,  /* Text display in default LCD mode */
  LCD_FEAT_SIMPLE_SHAPES       = 0x01,  /* Simple shapes displayed in default LCD mode */
  LCD_FEAT_BASIC_BITMAP        = 0x02,  /* Basic bitmaps 'stlogo' repeated display in default LCD mode */
  LCD_FEAT_ULPM_DATA_ONLY      = 0x03,  /* ULPM mode In and Out with Data only */
  LCD_FEAT_ULPM_DATA_AND_CLOCK = 0x04,  /* ULPM mode In and Out with Data and Clock */
  LCD_FEAT_NB_MAX              = 0x05,  /* Max number of LCD features to show */

} LcdDemoFeatures_TypeDef;

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static uint8_t lcd_feature_idx = 0; /* LCD feature index */

/* Private function prototypes -----------------------------------------------*/
static void LCD_SetInitialHint(void);
static void LCD_SetHint(uint8_t feature_index);
static void LCD_Show_Feature(uint8_t feature_index);
static void LCD_Show_FeatureText(void);
static void LCD_Show_FeatureSimpleShapes(void);
static void LCD_Show_FeatureBasicBitmap(void);


/* Private functions ---------------------------------------------------------*/


/**
  * @brief  LCD demo
  * @param  None
  * @retval None
  */
void LCD_demo(void)
{
  lcd_feature_idx = 0;

  LCD_SetInitialHint();

  LCD_Show_Feature(lcd_feature_idx);
  lcd_feature_idx++;

  while (1)
  {
    if (CheckForUserInput() > 0)
    {
      /* Careful : The last step (LCD_FEAT_NB_MAX) is used to restore LCD DSI normal mode */
      if (lcd_feature_idx <= LCD_FEAT_BASIC_BITMAP)
      //if (lcd_feature_idx <= LCD_FEAT_NB_MAX)
      {
        LCD_Show_Feature(lcd_feature_idx);
        lcd_feature_idx++;
      }
      else
      {
        /* Exit LCD demo once passed the last feature shown */
        return;
      }
    } /* of if (CheckForUserInput() > 0) */

    HAL_Delay(100);
  }
}

/**
  * @brief  Display LCD demo initial hint
  * @param  None
  * @retval None
  */
static void LCD_SetInitialHint(void)
{
  /* Clear the LCD */
  BSP_LCD_Clear(LCD_COLOR_WHITE);

  /* Set LCD Demo description */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_FillRect(0, 0, BSP_LCD_GetXSize(), 80);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_SetBackColor(LCD_COLOR_BLUE);
  BSP_LCD_SetFont(&Font24);
  BSP_LCD_DisplayStringAt(0, 0, (uint8_t *)"LCD Demo", CENTER_MODE);
  BSP_LCD_SetFont(&Font12);
  BSP_LCD_DisplayStringAt(0, 30, (uint8_t *)"This example shows the different", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 45, (uint8_t *)"LCD Features, press User button", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 60, (uint8_t *)"to switch to next test", CENTER_MODE);

  /* Set the LCD Text Color */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_DrawRect(10, 90, BSP_LCD_GetXSize() - 20, BSP_LCD_GetYSize() - 100);
  BSP_LCD_DrawRect(11, 91, BSP_LCD_GetXSize() - 22, BSP_LCD_GetYSize() - 102);
}

/**
  * @brief  Display LCD demo contextual hint
  * @param  feature_index : index of the feature to show
  * @retval None
  */
static void LCD_SetHint(uint8_t feature_index)
{
  if(feature_index < LCD_FEAT_NB_MAX)
  {
    /* Clear the LCD */
    BSP_LCD_Clear(LCD_COLOR_WHITE);

    /* Set LCD Demo description */
    BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
    BSP_LCD_FillRect(0, 0, BSP_LCD_GetXSize(), 80);
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    BSP_LCD_SetBackColor(LCD_COLOR_BLUE);
    BSP_LCD_SetFont(&Font24);
    BSP_LCD_DisplayStringAt(0, 0, (uint8_t *)"LCD Demo", CENTER_MODE);
    BSP_LCD_SetFont(&Font12);

    switch(feature_index)
    {
      case LCD_FEAT_TEXT :
        BSP_LCD_DisplayStringAt(0, 30, (uint8_t *)"Basic Text", CENTER_MODE);
        BSP_LCD_DisplayStringAt(0, 45, (uint8_t *)"With different Fonts sizes", CENTER_MODE);
        break;
      case LCD_FEAT_SIMPLE_SHAPES :
        BSP_LCD_DisplayStringAt(0, 30, (uint8_t *)"Simple Shapes", CENTER_MODE);
        BSP_LCD_DisplayStringAt(0, 45, (uint8_t *)"Draws circles, ellipses, polygons...", CENTER_MODE);
        break;
      case LCD_FEAT_BASIC_BITMAP :
        BSP_LCD_DisplayStringAt(0, 30, (uint8_t *)"Basic Bitmap", CENTER_MODE);
        BSP_LCD_DisplayStringAt(0, 45, (uint8_t *)"Display logo several times...", CENTER_MODE);
        break;
      case LCD_FEAT_ULPM_DATA_ONLY :
        BSP_LCD_DisplayStringAt(0, 30, (uint8_t *)"ULPM mode in/out", CENTER_MODE);
        BSP_LCD_DisplayStringAt(0, 45, (uint8_t *)"data only...", CENTER_MODE);
        break;
      case LCD_FEAT_ULPM_DATA_AND_CLOCK :
        BSP_LCD_DisplayStringAt(0, 30, (uint8_t *)"ULPM mode in/out", CENTER_MODE);
        BSP_LCD_DisplayStringAt(0, 45, (uint8_t *)"data and clock ...", CENTER_MODE);
        break;

      default :
        break;
    }

    BSP_LCD_DisplayStringAt(0, 60, (uint8_t *)"press User button for next test", CENTER_MODE);

    /* Set the LCD Text Color */
    BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
    BSP_LCD_DrawRect(10, 90, BSP_LCD_GetXSize() - 20, BSP_LCD_GetYSize() - 100);
    BSP_LCD_DrawRect(11, 91, BSP_LCD_GetXSize() - 22, BSP_LCD_GetYSize() - 102);

  } /* of if(feature_index < LCD_FEAT_NB_MAX) */

}

/**
  * @brief  LCD demo Text
  * @param  None
  * @retval None
  */
static void LCD_Show_FeatureText(void)
{
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  BSP_LCD_DisplayStringAt(14, 100, (uint8_t *)"Left aligned Text", LEFT_MODE);
  BSP_LCD_DisplayStringAt(0, 115, (uint8_t *)"Center aligned Text", CENTER_MODE);
  BSP_LCD_DisplayStringAt(14, 130, (uint8_t *)"Right aligned Text", RIGHT_MODE);
  BSP_LCD_SetFont(&Font24);
  BSP_LCD_DisplayStringAt(14, 180, (uint8_t *)"Font24", LEFT_MODE);
  BSP_LCD_SetFont(&Font20);
  BSP_LCD_DisplayStringAt(BSP_LCD_GetXSize() / 2 - 20, 180, (uint8_t *)"Font20", LEFT_MODE);
  BSP_LCD_SetFont(&Font16);
  BSP_LCD_DisplayStringAt(BSP_LCD_GetXSize() - 80, 184, (uint8_t *)"Font16", LEFT_MODE);
}

/**
  * @brief  LCD demo Simple Shapes
  * @param  None
  * @retval None
  */
static void LCD_Show_FeatureSimpleShapes(void)
{
  Point Points[]  = {{20, 150}, {80, 150}, {80, 200}};
  Point Points2[] = {{100, 150}, {160, 150}, {160, 200}};

  /* Draw misc Shapes inside rectangle area below */
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  BSP_LCD_DrawRect(20, 100, 60 , 40);

  /* Clear internal of rectangle area before display */
  BSP_LCD_FillRect(100, 100, 60 , 40);

  BSP_LCD_SetTextColor(LCD_COLOR_GRAY);
  BSP_LCD_DrawCircle(BSP_LCD_GetXSize() - 120, 120, 20);
  BSP_LCD_FillCircle(BSP_LCD_GetXSize() - 40, 120, 20);

  BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
  BSP_LCD_DrawPolygon(Points, 3);
  BSP_LCD_FillPolygon(Points2, 3);

  BSP_LCD_SetTextColor(LCD_COLOR_RED);
  BSP_LCD_DrawEllipse(BSP_LCD_GetXSize() - 120, 170, 30, 20);
  BSP_LCD_FillEllipse(BSP_LCD_GetXSize() - 50, 170, 30, 20);

  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  BSP_LCD_DrawHLine(20, BSP_LCD_GetYSize() - 30, BSP_LCD_GetXSize() / 5);
  BSP_LCD_DrawLine (BSP_LCD_GetXSize() - 150, BSP_LCD_GetYSize() - 20, BSP_LCD_GetXSize() - 20, BSP_LCD_GetYSize() - 50);
  BSP_LCD_DrawLine (BSP_LCD_GetXSize() - 150, BSP_LCD_GetYSize() - 50, BSP_LCD_GetXSize() - 20, BSP_LCD_GetYSize() - 20);
}

/**
  * @brief  LCD demo Basic Bitmap
  * @param  None
  * @retval None
  */
static void LCD_Show_FeatureBasicBitmap(void)
{
  /* Draw several Bitmaps 'stlogo' inside rectangle area */
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  BSP_LCD_DrawBitmap(20, 100, (uint8_t *)stlogo);
  HAL_Delay(50);

  BSP_LCD_DrawBitmap(BSP_LCD_GetXSize() / 2 - 40, 100, (uint8_t *)stlogo);
  HAL_Delay(50);

  BSP_LCD_DrawBitmap(BSP_LCD_GetXSize() - 100, 100, (uint8_t *)stlogo);
  HAL_Delay(50);

  BSP_LCD_DrawBitmap(20, BSP_LCD_GetYSize() - 80, (uint8_t *)stlogo);
  HAL_Delay(50);

  BSP_LCD_DrawBitmap(BSP_LCD_GetXSize() / 2 - 40, BSP_LCD_GetYSize() - 80, (uint8_t *)stlogo);
  HAL_Delay(50);

  BSP_LCD_DrawBitmap(BSP_LCD_GetXSize() - 100, BSP_LCD_GetYSize() - 80, (uint8_t *)stlogo);
  HAL_Delay(50);
}



/**
  * @brief  Show LCD Features
  * @param  feature_index : feature index
  * @retval None
  */
static void LCD_Show_Feature(uint8_t feature_index)
{
  uint8_t lcd_status = LCD_OK;

  if(feature_index < LCD_FEAT_NB_MAX)
  {
    LCD_SetHint(feature_index);

    BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);

    /* Clear in white the internal part of rectangle */
    BSP_LCD_FillRect(12, 92, BSP_LCD_GetXSize() - 24, BSP_LCD_GetYSize() - 104);
    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);

    switch(feature_index)
    {
      case LCD_FEAT_TEXT :
        /* Keep default LCD/DSI and LTDC Mode for this demo item */
        LCD_Show_FeatureText();
        break;
      case LCD_FEAT_SIMPLE_SHAPES :
        /* Keep default LCD/DSI and LTDC Mode for this demo item */
        LCD_Show_FeatureSimpleShapes();
        break;
      case LCD_FEAT_BASIC_BITMAP :
        /* Keep default LCD/DSI and LTDC Mode for this demo item */
        LCD_Show_FeatureBasicBitmap();
        break;
      case LCD_FEAT_ULPM_DATA_ONLY :
        BSP_LCD_SetFont(&Font16);
        BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() / 2, (uint8_t *)"Only testable in mode Video Burst", CENTER_MODE);
        break;
      case LCD_FEAT_ULPM_DATA_AND_CLOCK :
        BSP_LCD_SetFont(&Font16);
        BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() / 2, (uint8_t *)"Only testable in mode Video Burst", CENTER_MODE);
        break;
      default :
        break;
    } /* of switch(feature_index) */

  } /* of if(feature_index < LCD_FEAT_NB_MAX) */
  else if(feature_index == LCD_FEAT_NB_MAX)
  {
    /* Just after last index, restore the normal BSP LCD mode for following tests of the BSP display results */

    /* Proceed to LTDC and DSI initialization with the configuration filled in above */
    lcd_status = BSP_LCD_Init();
    BSP_TEST_APPLI_ASSERT(lcd_status != LCD_OK);

    /* Clear the LCD */
    BSP_LCD_Clear(LCD_COLOR_WHITE);

    /* Print text to Describe the current LCD DSI configuration */
    /* Print text to Describe the current LCD DSI configuration */
    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
    BSP_LCD_SetBackColor(LCD_COLOR_WHITE);

    BSP_LCD_SetFont(&Font16);
    BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() / 2 - 20, (uint8_t *)"Successfully restored", CENTER_MODE);
    BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() / 2 - 0, (uint8_t *)"Default LCD Mode.", CENTER_MODE);
    BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() / 2 + 20, (uint8_t *)"Press User button to switch to next demo", CENTER_MODE);

  } /* of else if(feature_index == LCD_FEAT_NB_MAX) */
}

/**
  * @}
  */

/**
  * @}
  */
