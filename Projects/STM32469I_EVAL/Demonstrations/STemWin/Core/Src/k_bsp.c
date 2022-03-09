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

/** @addtogroup CORE
  * @{
  */

/** @defgroup KERNEL_BSP
  * @brief Kernel BSP routines
  * @{
  */

/* External variables --------------------------------------------------------*/
extern uint8_t SelLayer;
extern uint8_t  I2C_Address;
TS_StateTypeDef  TS_State = {0};
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
  /* Initialize the NOR */
  BSP_QSPI_Init();
  BSP_QSPI_EnableMemoryMappedMode();  
  
  
  /* Initialize the SDRAM */
  BSP_SDRAM_Init();
  
  BSP_IO_Init();
 
  /* Enable MFX IO Expander interrupt for ETH MII pin */
  BSP_IO_ConfigPin(MII_INT_PIN, IO_MODE_IT_FALLING_EDGE);
  
  HAL_Delay(100);
  
  /* Initialize the Touch screen */
  BSP_TS_Init(800, 480);
        
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
  static GUI_PID_STATE TS_State = {0, 0, 0, 0};
  __IO TS_StateTypeDef  ts;
  uint16_t xDiff, yDiff;
  
  BSP_TS_GetState((TS_StateTypeDef *)&ts);

  if((ts.touchX[0] >= LCD_GetXSize()) ||(ts.touchY[0] >= LCD_GetYSize()) ) 
  {
    ts.touchX[0] = 0;
    ts.touchY[0] = 0;
  }

  xDiff = (TS_State.x > ts.touchX[0]) ? (TS_State.x - ts.touchX[0]) : (ts.touchX[0] - TS_State.x);
  yDiff = (TS_State.y > ts.touchY[0]) ? (TS_State.y - ts.touchY[0]) : (ts.touchY[0] - TS_State.y);
  
  if((TS_State.Pressed != ts.touchDetected ) ||
     (xDiff > 20 )||
       (yDiff > 20))
  {
    TS_State.Pressed = ts.touchDetected;
    TS_State.Layer = SelLayer;
    if(ts.touchDetected) 
    {
      TS_State.x = ts.touchX[0];     
      if(I2C_Address == TS_I2C_ADDRESS)
      {
        if(ts.touchY[0] < 240)
        {
          TS_State.y = ts.touchY[0];
        }
        else
        {
          TS_State.y = (ts.touchY[0] * 480) / 450;
        }
      }
      else
      {
        TS_State.y = ts.touchY[0]; 
      }
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
