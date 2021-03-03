/**
  ******************************************************************************
  * @file    k_bsp.c
  * @author  MCD Application Team
  * @brief   This file provides the kernel bsp functions
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "k_bsp.h"
#include "k_calibration.h"
#include "GUI.h"
#include "DIALOG.h"

/** @addtogroup CORE
  * @{
  */

/** @defgroup KERNEL_BSP
  * @brief Kernel BSP routines
  * @{
  */


/* External variables --------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Initializes LEDs, SDRAM, touch screen, CRC and SRAM.
  * @param  None 
  * @retval None
  */
void k_BspInit(void)
{
  
  GPIO_InitTypeDef GPIO_InitStructure;
  
  /* Enable CS GPIO clock and  Configure GPIO PIN for Gyroscope Chip select */
  __HAL_RCC_GPIOC_CLK_ENABLE();  
  GPIO_InitStructure.Pin = GPIO_PIN_1;
  GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStructure.Pull  = GPIO_NOPULL;
  GPIO_InitStructure.Speed = GPIO_SPEED_MEDIUM;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);
  
  /* Deselect : Chip Select high */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_SET);
    
  /* Configure LED3 and LED4 */
  BSP_LED_Init(LED3);
  BSP_LED_Init(LED4);  
  
  /* Initialize the SDRAM */
  BSP_SDRAM_Init();
  
  /* Initialize the Touch screen */
  BSP_TS_Init(240, 320);
  
  /* Enable CRC to Unlock GUI */
  __HAL_RCC_CRC_CLK_ENABLE();
  
  /* Enable Back up SRAM */
  __HAL_RCC_BKPSRAM_CLK_ENABLE();
  
}


/**
  * @brief  Read the coordinate of the point touched and assign their
  *         value to the variables u32_TSXCoordinate and u32_TSYCoordinate
  * @param  None
  * @retval None
  */
void k_TouchUpdate(void)
{
  GUI_PID_STATE TS_State;
  static TS_StateTypeDef prev_state;
  __IO TS_StateTypeDef  ts;
  uint16_t xDiff, yDiff;  
  
  BSP_TS_GetState((TS_StateTypeDef *)&ts);
  
  TS_State.Pressed = ts.TouchDetected;

  xDiff = (prev_state.X > ts.X) ? (prev_state.X - ts.X) : (ts.X - prev_state.X);
  yDiff = (prev_state.Y > ts.Y) ? (prev_state.Y - ts.Y) : (ts.Y - prev_state.Y);
  
  if((prev_state.TouchDetected != ts.TouchDetected )||
     (xDiff > 3 )||
       (yDiff > 3))
  {
    prev_state.TouchDetected = ts.TouchDetected;
    
    if((ts.X != 0) &&  (ts.Y != 0)) 
    {
      prev_state.X = ts.X;
      prev_state.Y = ts.Y;
    }
      
    if(k_CalibrationIsDone())
    {
      TS_State.Layer = 0;
      TS_State.x = k_CalibrationGetX (prev_state.X);
      TS_State.y = k_CalibrationGetY (prev_state.Y);
    }
    else
    {
      TS_State.Layer = 0;
      TS_State.x = prev_state.X;
      TS_State.y = prev_state.Y;
    }
    
    GUI_TOUCH_StoreStateEx(&TS_State);
  }
}

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
