/**
  ******************************************************************************
  * @file    BSP/Src/mems.c  
  * @author  MCD Application Team
  * @brief   This example code shows how to use MEMS features.
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
#define CIRCLE_UP_X_POS (BSP_LCD_GetXSize()/2)
#define CIRCLE_UP_Y_POS (90+CIRCLE_RADIUS+10)

#define CIRCLE_DOWN_X_POS CIRCLE_UP_X_POS
#define CIRCLE_DOWN_Y_POS (310-CIRCLE_RADIUS-10)

#define CIRCLE_LEFT_X_POS (BSP_LCD_GetXSize()/5)
#define CIRCLE_LEFT_Y_POS (310-110)

#define CIRCLE_RIGHT_X_POS (4*(BSP_LCD_GetXSize()/5))
#define CIRCLE_RIGHT_Y_POS CIRCLE_LEFT_Y_POS

#define CIRCLE_CENTRAL_X_POS CIRCLE_UP_X_POS
#define CIRCLE_CENTRAL_Y_POS CIRCLE_LEFT_Y_POS

/* Private macro -------------------------------------------------------------*/
#define ABS(x)         (x < 0) ? (-x) : x

/* Private variables ---------------------------------------------------------*/
float Buffer[3];
float Xval, Yval, Zval = 0x00;
uint8_t state = 0;

/* Private function prototypes -----------------------------------------------*/
static void MEMS_SetHint(void);
static void Gyroscope_DrawBackground(uint8_t state);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  MEMS demo
  * @param  None
  * @retval None
  */
void MEMS_demo(void)
{
  MEMS_SetHint();
  
  if (BSP_GYRO_Init() != GYRO_OK)
  {
    BSP_LCD_SetBackColor(LCD_COLOR_WHITE); 
    BSP_LCD_SetTextColor(LCD_COLOR_RED);
    BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()- 95, (uint8_t*)"ERROR", CENTER_MODE);
    BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()- 80, (uint8_t*)"MEMS cannot be initialized", CENTER_MODE);
  }
  else
  {
    Gyroscope_DrawBackground(state);
  }
  
  BSP_GYRO_Reset();
  
  while (1)
  {	
    /* Read Gyro Angular data */
    BSP_GYRO_GetXYZ(Buffer);
    
    /* Update autoreload and capture compare registers value */
    Xval = ABS((Buffer[0]));
    Yval = ABS((Buffer[1])); 
    Zval = ABS((Buffer[2])); 
    
    if((Xval>Yval) && (Xval>Zval))
    {
      if(Buffer[0] > 10000.0f)
      { 
        if((state & 1) == 0)
        {
          Gyroscope_DrawBackground(1);			
          state |= 1;
          HAL_Delay(1000);
        }        
      }
      else if(Buffer[0] < -10000.0f)
      { 
        if((state & 2) == 0)
        {          
          Gyroscope_DrawBackground(2);            
          state |= 2;
          HAL_Delay(1000);
        }           
      }      
    }
    else if ((Yval>Xval) && (Yval>Zval))
    {
      if(Buffer[1] < -10000.0f)
      {
        if((state & 4) == 0)
        {                     
          Gyroscope_DrawBackground(4);            
          state |= 4;
          HAL_Delay(1000);
        }   
      }
      else if(Buffer[1] > 10000.0f)
      {
        if((state & 8) == 0)
        {          
          Gyroscope_DrawBackground(8);            
          state |= 8;
          HAL_Delay(1000);
        }
      }     
    } 
    else if ((Zval>Xval) && (Zval>Yval))
    {
      if(Buffer[2] < -10000.0f)
      {
        if((state & 16) == 0)
        {                     
          Gyroscope_DrawBackground(16);            
          state |= 16;
          HAL_Delay(1000);
        }   
      }		 
      else if(Buffer[2] > 10000.0f)
      {
        if((state & 32) == 0)
        {          
          Gyroscope_DrawBackground(32);            
          state |= 32;
          HAL_Delay(1000);			
        }
      }  	
    } 
    if (state != 0x3F)	
    {
    }
    else if (CheckForUserInput() > 0)
    { 
      state = 0;    
      return;
    }
  }
}

/**
  * @brief  Display MEMS demo hint
  * @param  None
  * @retval None
  */
static void MEMS_SetHint(void)
{
  /* Clear the LCD */
  BSP_LCD_Clear(LCD_COLOR_WHITE);
  
  /* Set LCD Demo description */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_FillRect(0, 0, BSP_LCD_GetXSize(), 80);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_SetBackColor(LCD_COLOR_BLUE); 
  BSP_LCD_SetFont(&Font24);
  BSP_LCD_DisplayStringAt(0, 0, (uint8_t*)"MEMS", CENTER_MODE);
  BSP_LCD_SetFont(&Font12);
  BSP_LCD_DisplayStringAt(0, 30, (uint8_t*)"This example shows MEMS Features", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 45, (uint8_t*)"move board around axis", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 60, (uint8_t*)"to start test", CENTER_MODE);
  
  /* Set the LCD Text Color */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);  
  BSP_LCD_DrawRect(10, 90, BSP_LCD_GetXSize() - 20, BSP_LCD_GetYSize()- 100);
  BSP_LCD_DrawRect(11, 91, BSP_LCD_GetXSize() - 22, BSP_LCD_GetYSize()- 102);
}

/**
  * @brief  Draw Touchscreen Background
  * @param  Localstate: Touch zone state
  * @retval None
  */
static void Gyroscope_DrawBackground (uint8_t Localstate)
{
  switch(Localstate)
  {
  case 0:
    /* Circle for Y Positive Log */
    BSP_LCD_SetTextColor(LCD_COLOR_BLUE); 
    BSP_LCD_FillCircle(CIRCLE_LEFT_X_POS, CIRCLE_LEFT_Y_POS, CIRCLE_RADIUS);
    
    /* Circle for Y Positive Log */
    BSP_LCD_SetTextColor(LCD_COLOR_GREEN); 
    BSP_LCD_FillCircle(CIRCLE_RIGHT_X_POS, CIRCLE_RIGHT_Y_POS, CIRCLE_RADIUS);
    
    /* Circle for X Negative Log */
    BSP_LCD_SetTextColor(LCD_COLOR_YELLOW); 
    BSP_LCD_FillCircle(CIRCLE_UP_X_POS, CIRCLE_UP_Y_POS, CIRCLE_RADIUS);
    
    /* Circle for X Negative Log */
    BSP_LCD_SetTextColor(LCD_COLOR_RED); 
    BSP_LCD_FillCircle(CIRCLE_DOWN_X_POS, CIRCLE_DOWN_Y_POS, CIRCLE_RADIUS);
    
    /* Circle for Z Negative Log */
    BSP_LCD_SetTextColor(LCD_COLOR_ORANGE); 
    BSP_LCD_FillCircle(CIRCLE_CENTRAL_X_POS, CIRCLE_CENTRAL_Y_POS, CIRCLE_RADIUS);
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE); 
    BSP_LCD_FillCircle(CIRCLE_LEFT_X_POS, CIRCLE_LEFT_Y_POS, CIRCLE_RADIUS - 2);        
    BSP_LCD_FillCircle(CIRCLE_RIGHT_X_POS, CIRCLE_RIGHT_Y_POS,  CIRCLE_RADIUS - 2); 
    BSP_LCD_FillCircle(CIRCLE_UP_X_POS, CIRCLE_UP_Y_POS, CIRCLE_RADIUS - 2);
    BSP_LCD_FillCircle(CIRCLE_DOWN_X_POS, CIRCLE_DOWN_Y_POS, CIRCLE_RADIUS - 2);
    BSP_LCD_FillCircle(CIRCLE_CENTRAL_X_POS, CIRCLE_CENTRAL_Y_POS, CIRCLE_RADIUS - 2);
    break;
    
    /* X positive Log */
  case 1:
    BSP_LCD_SetTextColor(LCD_COLOR_RED); 
    BSP_LCD_FillCircle(CIRCLE_DOWN_X_POS, CIRCLE_DOWN_Y_POS, CIRCLE_RADIUS);
    BSP_LCD_SetFont(&Font24);
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    BSP_LCD_SetBackColor(LCD_COLOR_RED);
    BSP_LCD_DisplayStringAt(CIRCLE_DOWN_X_POS-(CIRCLE_RADIUS/2), CIRCLE_DOWN_Y_POS-(CIRCLE_RADIUS/2), (uint8_t*)"X+",LEFT_MODE);
    break;
    
    /* X Negative Log */
  case 2:
    BSP_LCD_SetTextColor(LCD_COLOR_YELLOW); 
    BSP_LCD_FillCircle(CIRCLE_UP_X_POS, CIRCLE_UP_Y_POS, CIRCLE_RADIUS);
    BSP_LCD_SetFont(&Font24);
    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
    BSP_LCD_SetBackColor(LCD_COLOR_YELLOW);
    BSP_LCD_DisplayStringAt(CIRCLE_UP_X_POS-(CIRCLE_RADIUS/2), CIRCLE_UP_Y_POS-(CIRCLE_RADIUS/2), (uint8_t*)"X-", LEFT_MODE);
    break;
    
    /* Y Negative Log */
  case 4:
    BSP_LCD_SetTextColor(LCD_COLOR_BLUE); 
    BSP_LCD_FillCircle(CIRCLE_LEFT_X_POS, CIRCLE_LEFT_Y_POS, CIRCLE_RADIUS);
    BSP_LCD_SetFont(&Font24);
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    BSP_LCD_SetBackColor(LCD_COLOR_BLUE);
    BSP_LCD_DisplayStringAt(CIRCLE_LEFT_X_POS-(CIRCLE_RADIUS/2), CIRCLE_LEFT_Y_POS-(CIRCLE_RADIUS/2), (uint8_t*)"Y-", LEFT_MODE);
    break;
    
    /* Y Positive Log */
  case 8:
    BSP_LCD_SetTextColor(LCD_COLOR_GREEN); 
    BSP_LCD_FillCircle(CIRCLE_RIGHT_X_POS, CIRCLE_RIGHT_Y_POS, CIRCLE_RADIUS);
    BSP_LCD_SetFont(&Font24);
    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
    BSP_LCD_SetBackColor(LCD_COLOR_GREEN);
    BSP_LCD_DisplayStringAt(CIRCLE_RIGHT_X_POS-(CIRCLE_RADIUS/2), CIRCLE_RIGHT_Y_POS-(CIRCLE_RADIUS/2), (uint8_t*)"Y+", LEFT_MODE);
    break;
    
    /* Z Negative Log */
  case 16:	 
    if((state & 32) == 0)
    {    
      BSP_LCD_SetTextColor(LCD_COLOR_ORANGE);
      BSP_LCD_FillCircle(CIRCLE_CENTRAL_X_POS, CIRCLE_CENTRAL_Y_POS, CIRCLE_RADIUS);
    }
    BSP_LCD_SetFont(&Font16);
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    BSP_LCD_SetBackColor(LCD_COLOR_ORANGE);
    BSP_LCD_DisplayStringAt(CIRCLE_CENTRAL_X_POS-(CIRCLE_RADIUS/2), CIRCLE_CENTRAL_Y_POS-(CIRCLE_RADIUS/2), (uint8_t*)"Z-", LEFT_MODE);
    break;
    
    /* Z Positive Log */
  case 32:
    if((state & 16) == 0)
    {	 
      BSP_LCD_SetTextColor(LCD_COLOR_ORANGE); 
      BSP_LCD_FillCircle(CIRCLE_CENTRAL_X_POS, CIRCLE_CENTRAL_Y_POS, CIRCLE_RADIUS);
    }		
    BSP_LCD_SetFont(&Font16);
    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
    BSP_LCD_SetBackColor(LCD_COLOR_ORANGE);
    BSP_LCD_DisplayStringAt(CIRCLE_CENTRAL_X_POS, CIRCLE_CENTRAL_Y_POS, (uint8_t*)"Z+", LEFT_MODE);
    break;
  }
}

/**
  * @}
  */ 

/**
  * @}
  */
