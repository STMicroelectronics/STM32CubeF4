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
  
#if defined(USE_STM32469I_DISCO_REVC)
  /* Reset the LCD */
  k_Bsp_LCD_Reset();
#endif

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

#if defined(USE_STM32469I_DISCO_REVC)
/* PCB REV A : nothing to be done, XRES of LCD is not cabled */

/**
  * @brief  Reset the LCD
  * @param  None
  * @retval None
  */
void k_Bsp_LCD_Reset(void)
{
  /* PCB REV B : XRES of LCD connected to PH7 (active low) */
  GPIO_InitTypeDef  gpio_init_structure;

  __HAL_RCC_GPIOH_CLK_ENABLE();

  /* Configure the GPIO on PH7 */
  gpio_init_structure.Pin   = GPIO_PIN_7;
  gpio_init_structure.Mode  = GPIO_MODE_OUTPUT_PP;
  gpio_init_structure.Pull  = GPIO_NOPULL;
  gpio_init_structure.Speed = GPIO_SPEED_MEDIUM;
  HAL_GPIO_Init( GPIOH, &gpio_init_structure );

  /* Activate XRES active low */
  HAL_GPIO_WritePin( GPIOH, GPIO_PIN_7, GPIO_PIN_RESET );
  /* wait 20 ms */
  HAL_Delay(20);

  /* Deactivate XRES */
  HAL_GPIO_WritePin( GPIOH, GPIO_PIN_7, GPIO_PIN_SET );
}
#endif /* USE_STM32469I_DISCO_REVC */

/**
  * @}
  */

/**
  * @}
  */
