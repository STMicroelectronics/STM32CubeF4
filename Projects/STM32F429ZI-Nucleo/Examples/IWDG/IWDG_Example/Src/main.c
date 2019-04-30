/**
  ******************************************************************************
  * @file    IWDG/IWDG_Example/Src/main.c
  * @author  MCD Application Team
  * @brief   This sample code shows how to use the STM32F429xx IWDG HAL API
  *          to update at regular period the IWDG counter and how to simulate a
  *          software fault generating an MCU IWDG reset on expiry of a
  *          programmed time period.
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

/** @addtogroup STM32F4xx_HAL_Examples
  * @{
  */

/** @addtogroup IWDG_Example
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* IWDG and TIM handlers declaration */
IWDG_HandleTypeDef IwdgHandle;
TIM_HandleTypeDef  TimInputCaptureHandle;
RCC_ClkInitTypeDef RCC_ClockFreq;

static __IO uint32_t uwLsiFreq = 0;
__IO uint32_t uwCaptureNumber = 0;
__IO uint32_t uwPeriodValue = 0;
__IO uint32_t uwMeasurementDone = 0;

uint16_t tmpCC4[2] = {0, 0};

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void Error_Handler(void);
static uint32_t GetLSIFrequency(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
  /* STM32F4xx HAL library initialization:
       - Configure the Flash prefetch
       - Systick timer is configured by default as source of time base, but user 
         can eventually implement his proper time base source (a general purpose 
         timer for example or other time source), keeping in mind that Time base 
         duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and 
         handled in milliseconds basis.
       - Set NVIC Group Priority to 4
       - Low Level Initialization
     */
  HAL_Init();

  /* Configure the system clock to 180 MHz */
  SystemClock_Config();

  /* Configure LED1, LED2, LED3 and User push-button */
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED2);
  BSP_LED_Init(LED3);

  /* Configure User push-button */
  BSP_PB_Init(BUTTON_USER, BUTTON_MODE_EXTI);

  /*##-1- Check if the system has resumed from IWDG reset ####################*/
  if (__HAL_RCC_GET_FLAG(RCC_FLAG_IWDGRST) != RESET)
  {
    /* IWDGRST flag set: Turn LED1 on */
    BSP_LED_On(LED1);

    /* Clear reset flags */
    __HAL_RCC_CLEAR_RESET_FLAGS();
  }
  else
  {
    /* IWDGRST flag is not set: Turn LED1 off */
    BSP_LED_Off(LED1);
  }

  /*##-2- Get the LSI frequency: TIM5 is used to measure the LSI frequency ###*/
  uwLsiFreq = GetLSIFrequency();

  /*##-3- Configure the IWDG peripheral ######################################*/
  /* Set counter reload value to obtain 250ms IWDG TimeOut.
     IWDG counter clock Frequency = LsiFreq / 32
     Counter Reload Value = 250ms / IWDG counter clock period
                          = 0.25s / (32/LsiFreq)
                          = LsiFreq / (32 * 4)
                          = LsiFreq / 128 */
  IwdgHandle.Instance = IWDG;

  IwdgHandle.Init.Prescaler = IWDG_PRESCALER_32;
  IwdgHandle.Init.Reload    = uwLsiFreq / 128;

  if (HAL_IWDG_Init(&IwdgHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /* Infinite loop */
  while (1)
  {
    /* Toggle LED2 */
    BSP_LED_Toggle(LED2);

    /* Insert 240 ms delay */
    HAL_Delay(240);

    /* Refresh IWDG: reload counter */
    if (HAL_IWDG_Refresh(&IwdgHandle) != HAL_OK)
    {
      /* Refresh Error */
      Error_Handler();
    }
  }
}

/**
  * @brief  Configures TIM5 to measure the LSI oscillator frequency.
  * @param  None
  * @retval LSI Frequency
  */
static uint32_t GetLSIFrequency(void)
{
  uint32_t pclk1 = 0, latency = 0;
  TIM_IC_InitTypeDef timinputconfig = {0};
  RCC_OscInitTypeDef oscinit = {0};
  RCC_ClkInitTypeDef  clkinit =  {0};
  
  /* Enable LSI Oscillator */
  oscinit.OscillatorType = RCC_OSCILLATORTYPE_LSI;
  oscinit.LSIState = RCC_LSI_ON;
  oscinit.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&oscinit)!= HAL_OK)
  {
    Error_Handler(); 
  }

  /* Configure the TIM peripheral */
  /* Set TIMx instance */
  TimInputCaptureHandle.Instance = TIMx;

  /* TIMx configuration: Input Capture mode ---------------------
  The LSI clock is connected to TIM5 CH4.
  The Rising edge is used as active edge.
  The TIM5 CCR4 is used to compute the frequency value.
  ------------------------------------------------------------ */
  TimInputCaptureHandle.Init.Prescaler         = 0;
  TimInputCaptureHandle.Init.CounterMode       = TIM_COUNTERMODE_UP;
  TimInputCaptureHandle.Init.Period            = 0xFFFF;
  TimInputCaptureHandle.Init.ClockDivision     = 0;
  TimInputCaptureHandle.Init.RepetitionCounter = 0;

  if (HAL_TIM_IC_Init(&TimInputCaptureHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
  /* Connect internally the  TIM5 CH4 Input Capture to the LSI clock output */
  HAL_TIMEx_RemapConfig(&TimInputCaptureHandle, TIMx_REMAP);

  /* Configure the Input Capture of channel 4 */
  timinputconfig.ICPolarity  = TIM_ICPOLARITY_RISING;
  timinputconfig.ICSelection = TIM_ICSELECTION_DIRECTTI;
  timinputconfig.ICPrescaler = TIM_ICPSC_DIV8;
  timinputconfig.ICFilter    = 0;

  if (HAL_TIM_IC_ConfigChannel(&TimInputCaptureHandle, &timinputconfig, TIM_CHANNEL_4) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /* Reset the flags */
  TimInputCaptureHandle.Instance->SR = 0;

  /* Start the TIM Input Capture measurement in interrupt mode */
  if (HAL_TIM_IC_Start_IT(&TimInputCaptureHandle, TIM_CHANNEL_4) != HAL_OK)
  {
    /* Starting Error */
    Error_Handler();
  }

  /* Wait until the TIM5 get 2 LSI edges (refer to TIM5_IRQHandler() in
  stm32f4xx_it.c file) */
  while (uwMeasurementDone == 0)
  {
  }
  uwCaptureNumber = 0;

  /* Deinitialize the TIM5 peripheral registers to their default reset values */
  HAL_TIM_IC_DeInit(&TimInputCaptureHandle);

  /* Compute the LSI frequency, depending on TIM5 input clock frequency (PCLK1)*/
  /* Get PCLK1 frequency */
  pclk1 = HAL_RCC_GetPCLK1Freq();
  HAL_RCC_GetClockConfig(&clkinit, &latency);

  /* Get PCLK1 prescaler */
  if ((clkinit.APB1CLKDivider) == RCC_HCLK_DIV1)
  {
    /* PCLK1 prescaler equal to 1 => TIMCLK = PCLK1 */
    return ((pclk1 / uwPeriodValue) * 8);
  }
  else
  {
    /* PCLK1 prescaler different from 1 => TIMCLK = 2 * PCLK1 */
    return (((2 * pclk1) / uwPeriodValue) * 8) ;
  }
}

/**
  * @brief  Input Capture callback in non blocking mode 
  * @param  htim : TIM IC handle
  * @retval None
  */
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
  /* Get the Input Capture value */
  tmpCC4[uwCaptureNumber++] = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_4);

  if (uwCaptureNumber >= 2)
  {
    /* Compute the period length */
    uwPeriodValue = (uint16_t)(0xFFFF - tmpCC4[0] + tmpCC4[1] + 1);
    uwMeasurementDone = 1;
    uwCaptureNumber = 0;
  }
}


/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  while (1)
  {
  }
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 180000000
  *            HCLK(Hz)                       = 180000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 4
  *            APB2 Prescaler                 = 2
  *            HSE Frequency(Hz)              = 8000000
  *            PLL_M                          = 8
  *            PLL_N                          = 360
  *            PLL_P                          = 2
  *            PLL_Q                          = 7
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale1 mode
  *            Flash Latency(WS)              = 5
  * @param  None
  * @retval None
  */
static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;

  /* Enable Power Control clock */
  __HAL_RCC_PWR_CLK_ENABLE();
  
  /* The voltage scaling allows optimizing the power consumption when the device is 
     clocked below the maximum system frequency, to update the voltage scaling value 
     regarding system frequency refer to product datasheet.  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  
  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 360;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
  
  if(HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
  
  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;  
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
}
#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
