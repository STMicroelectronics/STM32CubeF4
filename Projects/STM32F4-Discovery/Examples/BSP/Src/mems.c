/**
  ******************************************************************************
  * @file    BSP/Src/mems.c 
  * @author  MCD Application Team
  * @brief   This example code shows how to use MEMS features.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2017 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "mems.h"

/** @addtogroup STM32F4xx_HAL_Examples
  * @{
  */

/** @addtogroup BSP
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern __IO uint8_t UserPressButton;

/* Init af threshold to detect acceleration on MEMS */
/* Typical value: 
      - No  acceleration: X, Y inferior to 100 (positive or negative)
      - Max acceleration: X, Y around 2000 (positive or negative) */
int16_t ThresholdHigh = 200;
int16_t ThresholdLow = -200;

/* Private function prototypes -----------------------------------------------*/
static void ACCELERO_ReadAcc(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Test ACCELERATOR MEMS Hardware.
  *         The main objective of this test is to check acceleration on 2 axes X and Y
  * @param  None
  * @retval None
  */
void ACCELERO_MEMS_Test(void)
{
  /* Init Accelerometer MEMS */
  if(BSP_ACCELERO_Init() != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler(); 
  }
  
  UserPressButton = 0;
  while(!UserPressButton)
  {
    ACCELERO_ReadAcc();
  }
}

/**
  * @brief  Read Acceleration data.
  * @param  None
  * @retval None
  */
static void ACCELERO_ReadAcc(void)
{
  /* Accelerometer variables */
  int16_t buffer[3] = {0};
  int16_t xval, yval = 0x00;
  
  /* Read Acceleration */
  BSP_ACCELERO_GetXYZ(buffer);
  
  xval = buffer[0];
  yval = buffer[1];
  
  if((ABS(xval))>(ABS(yval)))
  {
    if(xval > ThresholdHigh)
    { 
      /* LED5 On */
      BSP_LED_On(LED5);
      HAL_Delay(10);
    }
    else if(xval < ThresholdLow)
    { 
      /* LED4 On */
      BSP_LED_On(LED4);      
      HAL_Delay(10);
    }
    else
    { 
      HAL_Delay(10);
    }
  }
  else
  {
    if(yval < ThresholdLow)
    {
      /* LED6 On */
      BSP_LED_On(LED6);
      HAL_Delay(10);
    }
    else if(yval > ThresholdHigh)
    {
      /* LED3 On */
      BSP_LED_On(LED3);
      HAL_Delay(10);
    } 
    else
    { 
      HAL_Delay(10);
    }
  } 
  
  BSP_LED_Off(LED3);
  BSP_LED_Off(LED4);
  BSP_LED_Off(LED5);
  BSP_LED_Off(LED6);
}

/**
  * @}
  */ 

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
