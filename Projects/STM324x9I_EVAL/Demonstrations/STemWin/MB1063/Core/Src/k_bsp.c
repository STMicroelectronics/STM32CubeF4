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
  /* Configure LED1, LED2, LED3 and LED4 */
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED2);
  BSP_LED_Init(LED3);
  BSP_LED_Init(LED4);  
  
  /* Initialize the SDRAM */
  BSP_SDRAM_Init();
  
  /* Initialize the Touch screen */
  BSP_TS_Init(640, 480);
  
  /* Enable CRC to Unlock GUI */
  __HAL_RCC_CRC_CLK_ENABLE();
  
  /* Enable Back up SRAM */
  __HAL_RCC_BKPSRAM_CLK_ENABLE();
  
}

/**
  * @brief  Initializes BSP Audio
  * @param  None 
  * @retval None
  */
void k_BspAudioInit(void)
{
  BSP_AUDIO_OUT_Init(OUTPUT_DEVICE_AUTO, AUDIO_DEFAULT_VOLUME, I2S_AUDIOFREQ_44K);
  BSP_AUDIO_OUT_SetAudioFrameSlot(CODEC_AUDIOFRAME_SLOT_02);
}



/**
  * @brief  Read the coordinate of the point touched and assign their
  *         value to the variables u32_TSXCoordinate and u32_TSYCoordinate
  * @param  None
  * @retval None
  */
void k_TouchUpdate(void)
{
  static GUI_PID_STATE TS_State = {{0}, {0}, {0}, {0}};
  __IO TS_StateTypeDef  ts;
  uint16_t xDiff, yDiff;  

  BSP_TS_GetState((TS_StateTypeDef *)&ts);

  if((ts.x >= LCD_GetXSize()) ||(ts.y >= LCD_GetYSize()) ) 
  {
    ts.x = 0;
    ts.y = 0;
    ts.TouchDetected =0;
  }

  xDiff = (TS_State.x > ts.x) ? (TS_State.x - ts.x) : (ts.x - TS_State.x);
  yDiff = (TS_State.y > ts.y) ? (TS_State.y - ts.y) : (ts.y - TS_State.y);
  
  
  if((TS_State.Pressed != ts.TouchDetected ) ||
     (xDiff > 30 )||
      (yDiff > 30))
  {
    TS_State.Pressed = ts.TouchDetected;
    TS_State.Layer = 0;
    if(ts.TouchDetected) 
    {
      TS_State.x = ts.x;
      TS_State.y = ts.y;
      GUI_TOUCH_StoreStateEx(&TS_State);
    }
    else
    {
      GUI_TOUCH_StoreStateEx(&TS_State);
      TS_State.x = 0;
      TS_State.y = 0;
    }
  }
}

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
