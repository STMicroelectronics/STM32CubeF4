/**
  ******************************************************************************
  * @file    k_bsp.c
  * @author  MCD Application Team
  * @brief   This file provides the kernel bsp functions
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
  * @brief  Initializes LEDs, touch screen, CRC and SRAM.
  * @param  None 
  * @retval None
  */
void k_BspInit(void)
{
  /* Configure LED1 and LED3 */
   BSP_LED_Init(LED1);
   BSP_LED_Init(LED3);
  
  BSP_SDRAM_Init();
  
  /* Initialize the Touch screen */
  BSP_TS_Init(320, 240);
  
  /* Enable CRC to Unlock GUI */
  __HAL_RCC_CRC_CLK_ENABLE();
}

/**
  * @brief  Initializes BSP Audio
  * @param  None 
  * @retval None
  */
void k_BspAudioInit(void)
{
  BSP_AUDIO_OUT_Init(OUTPUT_DEVICE_AUTO, AUDIO_DEFAULT_VOLUME, I2S_AUDIOFREQ_44K);
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

  xDiff = (prev_state.x > ts.x) ? (prev_state.x - ts.x) : (ts.x - prev_state.x);
  yDiff = (prev_state.y > ts.y) ? (prev_state.y - ts.y) : (ts.y - prev_state.y);
  
  if((prev_state.TouchDetected != ts.TouchDetected )&&
     ((xDiff > 3 )||
       (yDiff > 3)))
  {
    prev_state.TouchDetected = ts.TouchDetected;
    
    if((ts.x != 0) &&  (ts.y != 0)) 
    {
      prev_state.x = ts.x;
      prev_state.y = ts.y;
    }
      
    if(k_CalibrationIsDone())
    {
      TS_State.Layer = 0;
      TS_State.x = k_CalibrationGetX (prev_state.x);
      TS_State.y = k_CalibrationGetY (prev_state.y);
    }
    else
    {
      TS_State.Layer = 0;
      TS_State.x = prev_state.x;
      TS_State.y = prev_state.y;
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
