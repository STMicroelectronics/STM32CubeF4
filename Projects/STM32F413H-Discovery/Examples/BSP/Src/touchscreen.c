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
#define  CIRCLE_RADIUS        28
#define  LINE_LENGHT          30    
/* Private macro -------------------------------------------------------------*/
#define  CIRCLE_XPOS(i)       (((i == 1) | (i == 3)) ?  BSP_LCD_GetXSize()/4 : BSP_LCD_GetXSize() - 60)
#define  CIRCLE_YPOS(i)       (((i == 1) | (i == 2)) ? BSP_LCD_GetYSize()/2 : BSP_LCD_GetYSize() -40)    


/* Private Structures and Enumerations ------------------------------------------------------------*/
/* Possible allowed indexes of touchscreen demo */
typedef enum
{
  TOUCHSCREEN_DEMO_1 = 1,
  TOUCHSCREEN_DEMO_2 = 2,
  TOUCHSCREEN_DEMO_3 = 3,
  TOUCHSCREEN_DEMO_MAX = TOUCHSCREEN_DEMO_3,

} TouchScreenDemoTypeDef;

/* Global variables ---------------------------------------------------------*/
TS_StateTypeDef  TS_State = {0};
__IO uint16_t x_old = 0, y_old = 0;
/* Private variables ---------------------------------------------------------*/
/* Static variable holding the current touch color index : used to change color at each touch */
static uint32_t touchscreen_color_idx = 0;
uint32_t colors[23] = {LCD_COLOR_BLUE, LCD_COLOR_GREEN, LCD_COLOR_RED, LCD_COLOR_CYAN, LCD_COLOR_MAGENTA, LCD_COLOR_YELLOW,
                       LCD_COLOR_LIGHTBLUE, LCD_COLOR_LIGHTGREEN, LCD_COLOR_LIGHTRED, LCD_COLOR_LIGHTMAGENTA, LCD_COLOR_ORANGE,
                       LCD_COLOR_LIGHTYELLOW, LCD_COLOR_DARKBLUE, LCD_COLOR_DARKGREEN, LCD_COLOR_DARKRED, LCD_COLOR_DARKCYAN,
                       LCD_COLOR_DARKMAGENTA, LCD_COLOR_DARKYELLOW, LCD_COLOR_LIGHTGRAY, LCD_COLOR_GRAY, LCD_COLOR_DARKGRAY,
                       LCD_COLOR_BLACK, LCD_COLOR_BROWN}; 
/* Private function prototypes -----------------------------------------------*/
static void     Touchscreen_SetHint_Demo(TouchScreenDemoTypeDef demoIndex);
static uint32_t Touchscreen_Handle_NewTouch(void);
static uint8_t Get_ButtonNum(uint16_t x1, uint16_t y1);
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Touchscreen Demo1 : test touchscreen calibration and single touch in polling mode
  * @param  None
  * @retval None
  */
void Touchscreen_demo1(void)
{
  uint16_t x1, y1;
  uint8_t state = 0;
  uint8_t exitTsUseCase = 0;
  uint32_t ts_status = TS_OK;
  
  /* If calibration is not yet done, proceed with calibration */
  if (TouchScreen_IsCalibrationDone() == 0)
  {
    ts_status = Touchscreen_Calibration();
    if(ts_status == TS_OK)
    {
      BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - 65, (uint8_t *)"Touchscreen calibration success.", CENTER_MODE);
    }
  } /* of if (TouchScreen_IsCalibrationDone() == 0) */
  
  if(ts_status == TS_OK)
  {
    /* Display touch screen demo description */
    Touchscreen_SetHint_Demo(TOUCHSCREEN_DEMO_1);
    Touchscreen_DrawBackground_Circles(state);
    
    while (exitTsUseCase == 0)
    {
      if (ts_status == TS_OK)
      {
        /* Check in polling mode in touch screen the touch status and coordinates */
        /* of touches if touch occurred                                           */
        ts_status = BSP_TS_GetState(&TS_State);
        if(TS_State.touchDetected)
        {
          /* One or dual touch have been detected          */
          /* Only take into account the first touch so far */
          
          /* Get X and Y position of the first touch post calibrated */
          x1 = TouchScreen_Get_Calibrated_X(TS_State.touchX[0]);
          y1 = TouchScreen_Get_Calibrated_Y(TS_State.touchY[0]);
          
          if ((y1 > (CIRCLE_YPOS(1) - CIRCLE_RADIUS)) && (y1 < (CIRCLE_YPOS(1) + CIRCLE_RADIUS)))
          {
            if ((x1 > (CIRCLE_XPOS(1) - CIRCLE_RADIUS)) && (x1 < (CIRCLE_XPOS(1) + CIRCLE_RADIUS)))
            {
              if ((state & 1) == 0)
              {
                Touchscreen_DrawBackground_Circles(state);
                BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
                BSP_LCD_FillCircle(CIRCLE_XPOS(1), CIRCLE_YPOS(1), CIRCLE_RADIUS);
                state = 1;
              }
            }            
            if ((x1 > (CIRCLE_XPOS(2) - CIRCLE_RADIUS)) && (x1 < (CIRCLE_XPOS(2) + CIRCLE_RADIUS)))
            {
              if ((state & 2) == 0)
              {
                Touchscreen_DrawBackground_Circles(state);
                BSP_LCD_SetTextColor(LCD_COLOR_RED);
                BSP_LCD_FillCircle(CIRCLE_XPOS(2), CIRCLE_YPOS(2), CIRCLE_RADIUS);
                state = 2;
              }
            }            
          }
          if ((y1 > (CIRCLE_YPOS(3) - CIRCLE_RADIUS)) && (y1 < (CIRCLE_YPOS(3) + CIRCLE_RADIUS)))
          {          
            if ((x1 > (CIRCLE_XPOS(3) - CIRCLE_RADIUS)) && (x1 < (CIRCLE_XPOS(3) + CIRCLE_RADIUS)))
            {
              if ((state & 4) == 0)
              {
                Touchscreen_DrawBackground_Circles(state);
                BSP_LCD_SetTextColor(LCD_COLOR_YELLOW);
                BSP_LCD_FillCircle(CIRCLE_XPOS(3), CIRCLE_YPOS(3), CIRCLE_RADIUS);
                state = 4;
              }
            }
            
            if ((x1 > (CIRCLE_XPOS(4) - CIRCLE_RADIUS)) && (x1 < (CIRCLE_XPOS(4) + CIRCLE_RADIUS)))
            {
              if ((state & 8) == 0)
              {
                Touchscreen_DrawBackground_Circles(state);
                BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
                BSP_LCD_FillCircle(CIRCLE_XPOS(4), CIRCLE_YPOS(4), CIRCLE_RADIUS);
                state = 8;
              }
            }
          }
        } /* of if(TS_State.TouchDetected) */
      } /* of if (ts_status == TS_OK) */ 
      
      /* Wait for a key button press to switch to next test case of BSP validation application */
      /* Otherwise stay in the test */
      exitTsUseCase = CheckForUserInput();
      
      HAL_Delay(20);
    } /* of while (exitTsUseCase == 0) */  
  } /* of if(status == TS_OK) */ 
} 


/**
  * @brief  Touchscreen Demo2 : test touchscreen single and dual touch in polling mode
  * @param  None
  * @retval None
  */
void Touchscreen_demo2(void)
{
  uint8_t exitTsUseCase = 0;
  uint32_t ts_status = TS_OK;

  /* If calibration is not yet done, proceed with calibration */
  if (TouchScreen_IsCalibrationDone() == 0)
  {
    ts_status = Touchscreen_Calibration();
    if(ts_status == TS_OK)
    {
      BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - 65, (uint8_t *)"Touchscreen calibration success.", CENTER_MODE);
    }
  } /* of if (TouchScreen_IsCalibrationDone() == 0) */
  
  if(ts_status == TS_OK)
  {
    /* Display touch screen demo2 description */
    Touchscreen_SetHint_Demo(TOUCHSCREEN_DEMO_2);

    while (exitTsUseCase == 0)
    {
      Touchscreen_Handle_NewTouch();

      /* Wait for a key button press to switch to next test case of BSP validation application */
      /* Otherwise stay in the test */
      exitTsUseCase = CheckForUserInput();

      HAL_Delay(100);

    } /* of while (exitTsUseCase == 0) */

  } /* of if(status == TS_OK) */
}

/**
  * @brief  Touchscreen Demo3 : test touchscreen single touch in interrupt mode
  * @param  None
  * @retval None
  */
void Touchscreen_demo3(void)
{
  uint16_t x1, y1, i, j, k, l;
  uint8_t exitTsUseCase = 0;
  uint32_t ts_status = TS_OK; 
  
  /* If calibration is not yet done, proceed with calibration */
  if (TouchScreen_IsCalibrationDone() == 0)
  {
    ts_status = Touchscreen_Calibration();
    if(ts_status == TS_OK)
    {
      BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - 65, (uint8_t *)"Touchscreen calibration success.", CENTER_MODE);
    }
  } /* of if (TouchScreen_IsCalibrationDone() == 0) */
  
  BSP_TS_ITConfig();
  
  /* Display touch screen demo2 description */
  Touchscreen_SetHint_Demo(TOUCHSCREEN_DEMO_3);
  while (CheckForUserInput() == 0);
  
  BSP_LCD_Clear(LCD_COLOR_WHITE);
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  for(k = 0; k < 6; k++)
  {
    for(l = 0; l < 6; l++)
    {
      BSP_LCD_DrawRect(40*k, 40*l,40,40);
    }
  }
  
  while (exitTsUseCase == 0)
  {
    ts_status = BSP_TS_GetState(&TS_State);
    if(TS_State.touchDetected)
    {
      /* One or dual touch have been detected          */
      /* Only take into account the first touch so far */
      
      /* Get X and Y position of the first touch post calibrated */
      x1 = TouchScreen_Get_Calibrated_X(TS_State.touchX[0]);
      y1 = TouchScreen_Get_Calibrated_Y(TS_State.touchY[0]);
      
      for(i = 0; i < 6; i++)
      {
        for(j = 0; j < 6; j++)
        {
          if(((x1 > 40*i) && (x1 < 40*(i+1))) && ((y1 > 40*j) && (y1 < 40*(j+1))))
          {
            BSP_LCD_Clear(LCD_COLOR_WHITE);
            for(k = 0; k < 6; k++)
            {
              for(l = 0; l < 6; l++)
              {
                BSP_LCD_SetTextColor(colors[(touchscreen_color_idx++ % 23)]);
                
                BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
                BSP_LCD_DrawRect(40*k, 40*l,40,40);
              }
            }
            BSP_LCD_SetTextColor(colors[(touchscreen_color_idx++ % 23)]);
            BSP_LCD_FillRect(40*i, 40*j,40,40);
          }
        }
      }
    }
    /* Wait for a key button press to switch to next test case of BSP validation application */
    /* Otherwise stay in the test */
    exitTsUseCase = CheckForUserInput();    
  }  
}

/**
  * @brief  Touchscreen_Handle_NewTouch: factorization of touch management
  * @param  None
  * @retval TS_OK or TS_ERROR
  */
static uint32_t Touchscreen_Handle_NewTouch(void)
{
#define TS_MULTITOUCH_FOOTPRINT_CIRCLE_RADIUS 15
#define TOUCH_INFO_STRING_SIZE                70
  uint16_t x1 = 0;
  uint16_t y1 = 0;
  uint16_t x2 = 0;
  uint16_t y2 = 0; 
  uint32_t drawTouch1 = 0; /* activate/deactivate draw of footprint of touch 1 */
  uint32_t drawTouch2 = 0; /* activate/deactivate draw of footprint of touch 2 */

  uint32_t ts_status = TS_OK;
  uint8_t lcd_string[TOUCH_INFO_STRING_SIZE] = "";

  /* Check in polling mode in touch screen the touch status and coordinates */
  /* of touches if touch occurred                                           */
  ts_status = BSP_TS_GetState(&TS_State);
  if(TS_State.touchDetected)
  {
    /* One or dual touch have been detected  */

    /* Erase previous information on touchscreen play pad area */
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    BSP_LCD_FillRect(12, 92, BSP_LCD_GetXSize() - 24, BSP_LCD_GetYSize() - 104);
    
    /* Re-Draw touch screen play area on LCD */
    BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
    BSP_LCD_DrawRect(10, 90, BSP_LCD_GetXSize() - 20, BSP_LCD_GetYSize() - 100);
    BSP_LCD_DrawRect(11, 91, BSP_LCD_GetXSize() - 22, BSP_LCD_GetYSize() - 102);

    /* Erase previous information on bottom text bar */
    BSP_LCD_FillRect(0, 0, BSP_LCD_GetXSize(), 85);

    /* Deactivate drawing footprint of touch 1 and touch 2 until validated against boundaries of touch pad values */
    drawTouch1 = drawTouch2 = 0;

    /* Get X and Y position of the first touch post calibrated */
    x1 = TouchScreen_Get_Calibrated_X(TS_State.touchX[0]);
    y1 = TouchScreen_Get_Calibrated_Y(TS_State.touchY[0]);
    
    if((y1 > (90 + TS_MULTITOUCH_FOOTPRINT_CIRCLE_RADIUS)) &&
       (y1 < (BSP_LCD_GetYSize() - 10 - TS_MULTITOUCH_FOOTPRINT_CIRCLE_RADIUS)) &&
       (x1 > (10 + TS_MULTITOUCH_FOOTPRINT_CIRCLE_RADIUS)) &&
       (x1 < (BSP_LCD_GetXSize() - 20 - TS_MULTITOUCH_FOOTPRINT_CIRCLE_RADIUS)))
    {
      drawTouch1 = 1;
    }

    /* If valid touch 1 position : inside the reserved area for the use case : draw the touch */
    if(drawTouch1 == 1)
    {
      /* Draw circle of first touch : turn on colors[] table */
      BSP_LCD_SetTextColor(colors[(touchscreen_color_idx++ % 23)]);
      BSP_LCD_FillCircle(x1, y1, TS_MULTITOUCH_FOOTPRINT_CIRCLE_RADIUS);

      BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
      BSP_LCD_SetFont(&Font16);
      BSP_LCD_DisplayStringAt(0, 0, (uint8_t *)"TOUCH INFO : ", CENTER_MODE);

      BSP_LCD_SetFont(&Font12);
      sprintf((char*)lcd_string, "x1 = %d, y1 = %d, Event = %s, Weight = %d",
              x1,
              y1,
              ts_event_string_tab[TS_State.touchEventId[0]],
              TS_State.touchWeight[0]);
      BSP_LCD_DisplayStringAt(0, 15, lcd_string, CENTER_MODE);
    } /* of if(drawTouch1 == 1) */

    if(TS_State.touchDetected > 1)
    {
      /* Get X and Y position of the second touch post calibrated */
      x2 = TouchScreen_Get_Calibrated_X(TS_State.touchX[1]);
      y2 = TouchScreen_Get_Calibrated_Y(TS_State.touchY[1]);
      
      if((y2 > (90 + TS_MULTITOUCH_FOOTPRINT_CIRCLE_RADIUS)) &&
         (y2 < (BSP_LCD_GetYSize() - 10 - TS_MULTITOUCH_FOOTPRINT_CIRCLE_RADIUS)) &&
         (x2 > (10 + TS_MULTITOUCH_FOOTPRINT_CIRCLE_RADIUS)) &&
         (x2 < (BSP_LCD_GetXSize() - 20 - TS_MULTITOUCH_FOOTPRINT_CIRCLE_RADIUS)))
      {
        drawTouch2 = 1;
      }

      /* If valid touch 2 position : inside the reserved area for the use case : draw the touch */
      if(drawTouch2 == 1)
      {
        sprintf((char*)lcd_string, "x2 = %d, y2 = %d, Event = %s, Weight = %d",
                x2,
                y2,
                ts_event_string_tab[TS_State.touchEventId[1]],
                TS_State.touchWeight[1]);
        BSP_LCD_DisplayStringAt(0, 35, lcd_string, CENTER_MODE);

        /* Draw circle of second touch : turn on color[] table */
        BSP_LCD_SetTextColor(colors[(touchscreen_color_idx++ % 24)]);
        BSP_LCD_FillCircle(x2, y2, TS_MULTITOUCH_FOOTPRINT_CIRCLE_RADIUS);
      } /* of if(drawTouch2 == 1) */

    } /* of if(TS_State.touchDetected > 1) */

    if((drawTouch1 == 1) || (drawTouch2 == 1))
    {
      /* Get updated gesture Id in global variable 'TS_State' */
      ts_status = BSP_TS_Get_GestureId(&TS_State);

      sprintf((char*)lcd_string, "Gesture Id = %s", ts_gesture_id_string_tab[TS_State.gestureId]);
      BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
      BSP_LCD_DisplayStringAt(0, 70, lcd_string, CENTER_MODE);
    }
    else
    {
      /* Invalid touch position */
      BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
      BSP_LCD_DisplayStringAt(0, 30, (uint8_t *)"!! Invalid touch position !!", CENTER_MODE);
      BSP_LCD_DisplayStringAt(0, 45, (uint8_t *)"Use drawn touch area", CENTER_MODE);
    }
  } /* of if(TS_State.TouchDetected) */

  return(ts_status);
}


/**
  * @brief  Display TS Demo Hint for all touchscreen demos depending on passed
  *         demoIndex in parameter.
  * @param  demoIndex : parameter of type @ref TouchScreenDemoTypeDef
  * @retval None
  */
static void Touchscreen_SetHint_Demo(TouchScreenDemoTypeDef demoIndex)
{
  if(demoIndex <= TOUCHSCREEN_DEMO_MAX)
  {
    /* Clear the LCD */
    BSP_LCD_Clear(LCD_COLOR_WHITE);

    /* Set Touchscreen Demo1 description */
    BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
    BSP_LCD_FillRect(0, 0, BSP_LCD_GetXSize(), 80);
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    BSP_LCD_SetBackColor(LCD_COLOR_BLUE);
    BSP_LCD_SetFont(&Font16);

    if(demoIndex == TOUCHSCREEN_DEMO_1)
    {
      BSP_LCD_DisplayStringAt(0, 0, (uint8_t *)"TS basic polling", CENTER_MODE);
      BSP_LCD_SetFont(&Font12);
      BSP_LCD_DisplayStringAt(0, 30, (uint8_t *)"Use the TS to activate the", CENTER_MODE);
      BSP_LCD_DisplayStringAt(0, 45, (uint8_t *)"colored circle inside the rectangle", CENTER_MODE);
      BSP_LCD_DisplayStringAt(0, 60, (uint8_t *)"Press User button to quit.", CENTER_MODE);
    }
    else if (demoIndex == TOUCHSCREEN_DEMO_2)
    {
      BSP_LCD_DisplayStringAt(0, 0, (uint8_t *)"TS dual touch polling", CENTER_MODE);
      BSP_LCD_SetFont(&Font12);
      BSP_LCD_DisplayStringAt(0, 30, (uint8_t *)"Please press the Touchscreen to", CENTER_MODE);
      BSP_LCD_DisplayStringAt(0, 45, (uint8_t *)"activate single and", CENTER_MODE);
      BSP_LCD_DisplayStringAt(0, 60, (uint8_t *)"dual touch", CENTER_MODE);
    }
    else
    {
      BSP_LCD_DisplayStringAt(0, 0, (uint8_t *)"TS IT mode", CENTER_MODE);
      
      BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
      BSP_LCD_SetBackColor(LCD_COLOR_BLUE);
    
      BSP_LCD_SetFont(&Font12);
      BSP_LCD_DisplayStringAt(0, 35, (uint8_t *)"Use the TS to select ", CENTER_MODE);
      BSP_LCD_DisplayStringAt(0, 50, (uint8_t *)"the required rectangle", CENTER_MODE);
      BSP_LCD_DisplayStringAt(0, 65, (uint8_t *)"Press button to start/quit.", CENTER_MODE);      
    }

    if(demoIndex != TOUCHSCREEN_DEMO_3)
    {
    /* Set the LCD Text Color */
    BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
    BSP_LCD_DrawRect(10, 90, BSP_LCD_GetXSize() - 20, BSP_LCD_GetYSize() - 100);
    BSP_LCD_DrawRect(11, 91, BSP_LCD_GetXSize() - 22, BSP_LCD_GetYSize() - 102);
    }
  } /* of if(demoIndex <= TOUCHSCREEN_DEMO_MAX) */
}

/**
  * @brief  Draw Touchscreen Background
  * @param  state : touch zone state
  * @retval None
  */
void Touchscreen_DrawBackground_Circles(uint8_t state)
{
  uint16_t x, y;  
  switch (state)
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

    case 32:
      BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
      BSP_LCD_FillCircle(CIRCLE_XPOS(3), CIRCLE_YPOS(3)-5, CIRCLE_RADIUS);
      BSP_LCD_FillCircle(CIRCLE_XPOS(4), CIRCLE_YPOS(4)-5, CIRCLE_RADIUS);

      BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
      BSP_LCD_FillCircle(CIRCLE_XPOS(3), CIRCLE_YPOS(3)-5, CIRCLE_RADIUS - 2);
      BSP_LCD_FillCircle(CIRCLE_XPOS(4), CIRCLE_YPOS(4)-5, CIRCLE_RADIUS - 2);


      BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
      BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
      BSP_LCD_SetFont(&Font16);
      x = CIRCLE_XPOS(3) - 10;
      y = CIRCLE_YPOS(3) - (BSP_LCD_GetFont()->Height)/2;
      BSP_LCD_DisplayStringAt(x, y, (uint8_t *)"Up", LEFT_MODE);
      x = CIRCLE_XPOS(4) - 10;
      y = CIRCLE_YPOS(4)  - (BSP_LCD_GetFont()->Height)/2;
      BSP_LCD_DisplayStringAt(x, y, (uint8_t *)"Dw", LEFT_MODE);
      BSP_LCD_SetFont(&Font12);

      break;      
  }
}


/**
  * @brief  Draw Touchscreen Background
  * @param  state : touch zone state
  * @retval None
  */
void Touchscreen_DrawControl_Buttons(void)
{
  /* Volume up */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_FillRect(170, 80, 60, 30);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_FillRect(172, 82, 56, 26);
  
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_DisplayStringAt(185, 93, (uint8_t *)"Vol+", LEFT_MODE);
  
  /* Volume down button*/
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_FillRect(10, 80, 60, 30);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_FillRect(12, 82, 56, 26);
  
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_DisplayStringAt(25, 93, (uint8_t *)"Vol-", LEFT_MODE);
  
  /* freq up button*/
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_FillRect(170, 200, 60, 30);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_FillRect(172, 202, 56, 26);
  
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_DisplayStringAt(185, 213, (uint8_t *)"Freq+", LEFT_MODE);
  
  /* freq down button */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_FillRect(10, 200, 60, 30);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_FillRect(12, 202, 56, 26);
  
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_DisplayStringAt(25, 213, (uint8_t *)"Freq-", LEFT_MODE);
  
  /* pause/play button*/
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_FillRect(90, 155, 60, 30);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_FillRect(92, 157, 56, 26);
  
  
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_DisplayStringAt(105, 165, (uint8_t *)"Pause", LEFT_MODE);
}


/**
  * @brief  TouchScreen get touch position
  * @param  None
  * @retval None
  */
uint8_t TouchScreen_GetTouchPosition(void)
{
  uint16_t x1, y1;
  uint8_t circleNr = 0;
  
  /* Check in polling mode in touch screen the touch status and coordinates */
  /* of touches if touch occurred                                           */
  BSP_TS_GetState(&TS_State);
  if(TS_State.touchDetected)
  {
    /* One or dual touch have been detected          */
    /* Only take into account the first touch so far */
    
    /* Get X and Y position of the first */          
    x1 = TouchScreen_Get_Calibrated_X(TS_State.touchX[0]);
    y1 = TouchScreen_Get_Calibrated_Y(TS_State.touchY[0]);
    
    if ((y1 > (CIRCLE_YPOS(3) - CIRCLE_RADIUS)) && (y1 < (CIRCLE_YPOS(3) + CIRCLE_RADIUS)))
    {          
      if ((x1 > (CIRCLE_XPOS(3) - CIRCLE_RADIUS)) && (x1 < (CIRCLE_XPOS(3) + CIRCLE_RADIUS)))
      {
        
        circleNr = 1;
      }
      
      if ((x1 > (CIRCLE_XPOS(4) - CIRCLE_RADIUS)) && (x1 < (CIRCLE_XPOS(4) + CIRCLE_RADIUS)))
      {
        
        circleNr = 2;
      }
    }
    else
    {
      if (((y1 < 220) && (y1 > 140)) &&
          ((x1 < 160) && (x1 > 100)))
      {
        circleNr = 0xFE;   /* top part of the screen */
      }
      else
      {
        circleNr = 0xFF;
      }
    }
  } /* of if(TS_State.TouchDetected) */
  return circleNr;
}

/**
  * @brief  TouchScreen get touch position
  * @param  None
  * @retval None
  */
uint8_t TouchScreen_GetTouchButtonPosition(void)
{
  uint16_t x1, y1;
  uint8_t buttonnum = 0;

  
  /* Check in polling mode in touch screen the touch status and coordinates */
  /* of touches if touch occurred                                           */
  BSP_TS_GetState(&TS_State);
  
    /* Get X and Y position of the first */          
    x1 = TouchScreen_Get_Calibrated_X(TS_State.touchX[0]);
    y1 = TouchScreen_Get_Calibrated_Y(TS_State.touchY[0]);
    
  if(TS_State.touchDetected)
  { 
    if((x_old != x1) || (y_old != y1))
    {
      x_old = x1; 
      y_old = y1; 
      buttonnum = Get_ButtonNum(x1, y1);     
    }
  }
  return buttonnum;
}

static uint8_t Get_ButtonNum(uint16_t x1, uint16_t y1)
{
  uint8_t buttonnum = 0; 
  if ((y1 < 110))
  {          
    if ((x1 > 10) && (x1 < 85))
    {
      
      buttonnum = 3; /* Vol - */
    }
    
    else if ((x1 > 170) && (x1 < 240))
    {
      
      buttonnum = 4; /* Vol + */
    }
  }
  else if ((y1 > 115) && (y1 < 190))
  {          
    if ((x1 > 90) && (x1 < 190))
    {
      
      buttonnum = 0xFE; /* Pause/Play */
    } 
  }
  else  if (y1 > 200)
  {          
    if ((x1 > 10) && (x1 < 80))
    {
      
      buttonnum = 1; /* freq - */
    }
    
    else if ((x1 > 170) && (x1 < 240))
    {
      
      buttonnum = 2; /* freq+ */
    }
  }
  else
  {
    buttonnum = 0xFF;
  }  
  return buttonnum;
}
/**
  * @}
  */

/**
  * @}
  */
