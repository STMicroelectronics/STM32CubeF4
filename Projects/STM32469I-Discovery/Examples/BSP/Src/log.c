/**
  ******************************************************************************
  * @file    BSP/Src/log.c
  * @author  MCD Application Team
  * @brief   This example code shows how to use the LCD Log firmware functions
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
#include "main.h"
#include "lcd_log.h"

/** @addtogroup STM32F4xx_HAL_Examples
  * @{
  */

/** @addtogroup BSP
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
typedef enum {
  TS_ACT_NONE = 0,
  TS_ACT_SCROLL_UP,
  TS_ACT_SCROLL_DOWN
}TS_ActionTypeDef;

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
  TS_ActionTypeDef ts_action = TS_ACT_NONE;
  uint8_t ts_status = TS_OK;
  uint8_t i = 0;

  /* Wait For User inputs */
  while (CheckForUserInput() == 0);

  if (TouchScreen_IsCalibrationDone() == 0)
  {
    ts_status = Touchscreen_Calibration();
    if(ts_status == TS_OK)
    {
      BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - 65, (uint8_t *)"Touchscreen calibration success.", CENTER_MODE);
    }
    else
    {
      BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - 65, (uint8_t *)"ERROR : touchscreen not yet calibrated.", CENTER_MODE);
      ts_status = TS_ERROR;
    }
  } /* of if (TouchScreen_IsCalibrationDone() == 0) */

  /* Initialize LCD Log module */
  LCD_LOG_Init();

  /* Show Header and Footer texts */
  LCD_LOG_SetHeader((uint8_t *)"Log Example");
  LCD_LOG_SetFooter((uint8_t *)"Use touch up/down to scroll");


  /* Output User logs */
  for (i = 0; i < 10; i++)
  {
    LCD_UsrLog ("This is Line %d \n", i);
  }

  HAL_Delay(1000);

  /* Clear Old logs */
  LCD_LOG_ClearTextZone();

  /* Output new user logs */
  for (i = 0; i < 30; i++)
  {
    LCD_UsrLog ("This is Line %d \n", i);
  }

  if(ts_status == TS_OK)
  {
    /* Set touchscreen in Interrupt mode and program EXTI accordingly on falling edge of TS_INT pin */
    ts_status = BSP_TS_ITConfig();
    BSP_TEST_APPLI_ASSERT(ts_status != TS_OK);
    Touchscreen_DrawBackground_Circle_Buttons(32);
    BSP_LCD_SetFont(&Font12);
  }

  /* Check for joystick user input for scroll (back and forward) */
  while (1)
  {
    ts_action = (TS_ActionTypeDef) TouchScreen_GetTouchPosition();

    switch (ts_action)
    {
      case TS_ACT_SCROLL_UP:
        LCD_LOG_ScrollBack();
        break;
      case TS_ACT_SCROLL_DOWN:
        LCD_LOG_ScrollForward();
        break;

      default:
        break;
    }

    if (CheckForUserInput() > 0)
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
