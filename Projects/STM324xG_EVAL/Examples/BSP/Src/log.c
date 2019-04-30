/**
  ******************************************************************************
  * @file    BSP/Src/log.c 
  * @author  MCD Application Team
  * @brief   This example code shows how to use the STM324xG BSP Drivers
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2017 STMicroelectronics</center></h2>>
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
#include "main.h"
#include "lcd_log.h"

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
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  LCD Log demo 
  * @param  None
  * @retval None
  */
void Log_demo(void)
{ 
  JOYState_TypeDef JoyState = JOY_NONE;
  uint8_t i = 0;
  
  /* Wait For User inputs */
  while(CheckForUserInput() == 0);
  
  BSP_JOY_Init(JOY_MODE_GPIO);
  
  /* Initialize LCD Log module */
  LCD_LOG_Init();
  
  /* Show Header and Footer texts */
  LCD_LOG_SetHeader((uint8_t *)"Log Example");
  LCD_LOG_SetFooter((uint8_t *)"Use Joystick to scroll up/down");
  
  /* Output User logs */
  for (i = 0; i < 10; i++)
  {
    LCD_UsrLog ("This is Line %d \n", i);
  }
  
  HAL_Delay(2000);
  
  /* Clear Old logs */
  LCD_LOG_ClearTextZone();
  
  /* Output new user logs */
  for (i = 0; i < 30; i++)
  {
    LCD_UsrLog ("This is Line %d \n", i);
  }
  
  /* Check for joystick user input for scroll (back and forward) */
  while (1)
  {
    JoyState = BSP_JOY_GetState();
    switch(JoyState)
    {
    case JOY_UP:
      LCD_LOG_ScrollBack();
      break;     
    case JOY_DOWN:
      LCD_LOG_ScrollForward();
      break;          
      
    default:
      break;           
    }
    if(CheckForUserInput() > 0)
    {
      return;
    }    
    HAL_Delay (10);
  }
}

/**
  * @}
  */ 

/**
  * @}
  */
  
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
