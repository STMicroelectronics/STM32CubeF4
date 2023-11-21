/**
  ******************************************************************************
  * @file    IWDG/IWDG_Example/Src/main.c  
  * @author  MCD Application Team
  * @brief   This sample code shows how to use the STM32F4xx IWDG HAL API 
  *          to update at regular period the IWDG counter and how to simulate a
  *          software fault generating an MCU IWDG reset on expiry of a 
  *          programmed time period.
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

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void Error_Handler(void);
static uint32_t GetLSIFrequency(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  /* STM32F4xx HAL library initialization:
       - Configure the Flash prefetch, instruction and Data caches
       - Configure the Systick to generate an interrupt each 1 msec
       - Set NVIC Group Priority to 4
       - Global MSP (MCU Support Package) initialization
     */
  HAL_Init();
  
  /* Configure the system clock to 168 MHz */
  SystemClock_Config();     
  
  /* Configure LED1, LED2 and LED3 */     
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED2);
  BSP_LED_Init(LED3);
  
  /* Configure the User Button in EXTI Mode */
  BSP_PB_Init(BUTTON_TAMPER, BUTTON_MODE_EXTI);
  
  /*##-1- Check if the system has resumed from IWDG reset ####################*/
  if(__HAL_RCC_GET_FLAG(RCC_FLAG_IWDGRST) != RESET)
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
  /* Set counter reload value to obtain 250ms IWDG Timeout.
     IWDG counter clock Frequency = LsiFreq / 32
     Counter Reload Value = 250ms / IWDG counter clock period
                          = 0.25s / (32/LsiFreq)
                          = LsiFreq / (32 * 4)
                          = LsiFreq / 128 */
  IwdgHandle.Instance = IWDG;

  IwdgHandle.Init.Prescaler = IWDG_PRESCALER_32;
  IwdgHandle.Init.Reload    = uwLsiFreq/128;
  
  if(HAL_IWDG_Init(&IwdgHandle) != HAL_OK)
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
    if(HAL_IWDG_Refresh(&IwdgHandle) != HAL_OK)
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
  uint32_t pclk1 = 0;
  TIM_IC_InitTypeDef timinputconfig;  
  
  /* Enable the LSI oscillator */
  __HAL_RCC_LSI_ENABLE();
  
  /* Wait till LSI is ready */
  while (__HAL_RCC_GET_FLAG(RCC_FLAG_LSIRDY) == RESET)
  {
  }
  
  /* Configure the TIM peripheral */ 
  /* Set TIMx instance */  
  TimInputCaptureHandle.Instance = TIM5;
  
  /* TIM5 configuration: Input Capture mode ---------------------
  The LSI oscillator is connected to TIM5 CH4.
  The Rising edge is used as active edge.
  The TIM5 CCR4 is used to compute the frequency value. 
  ------------------------------------------------------------ */
  TimInputCaptureHandle.Init.Prescaler         = 0; 
  TimInputCaptureHandle.Init.CounterMode       = TIM_COUNTERMODE_UP;  
  TimInputCaptureHandle.Init.Period            = 0xFFFF; 
  TimInputCaptureHandle.Init.ClockDivision     = 0;     
  TimInputCaptureHandle.Init.RepetitionCounter = 0; 
  if(HAL_TIM_IC_Init(&TimInputCaptureHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
  /* Connect internally the TIM5_CH4 Input Capture to the LSI clock output */
  HAL_TIMEx_RemapConfig(&TimInputCaptureHandle, TIM_TIM5_LSI);
  
  /* Configure the Input Capture of channel 4 */
  timinputconfig.ICPolarity  = TIM_ICPOLARITY_RISING;
  timinputconfig.ICSelection = TIM_ICSELECTION_DIRECTTI;
  timinputconfig.ICPrescaler = TIM_ICPSC_DIV8;
  timinputconfig.ICFilter    = 0;
  
  if(HAL_TIM_IC_ConfigChannel(&TimInputCaptureHandle, &timinputconfig, TIM_CHANNEL_4) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
  
  /* Reset the flags */
  TimInputCaptureHandle.Instance->SR = 0;
  
  /* Start the TIM Input Capture measurement in interrupt mode */
  if(HAL_TIM_IC_Start_IT(&TimInputCaptureHandle, TIM_CHANNEL_4) != HAL_OK)
  {
    Error_Handler();
  }
  
  /* Wait until the TIM5 get 2 LSI edges (refer to TIM5_IRQHandler() in 
  stm32f4xx_it.c file) */
  while(uwMeasurementDone == 0)
  {
  }
  uwCaptureNumber = 0;
  
  /* Deinitialize the TIM5 peripheral registers to their default reset values */
  HAL_TIM_IC_DeInit(&TimInputCaptureHandle);
  
  /* Compute the LSI frequency, depending on TIM5 input clock frequency (PCLK1)*/
  /* Get PCLK1 frequency */
  pclk1 = HAL_RCC_GetPCLK1Freq();
  
  /* Get PCLK1 prescaler */
  if((RCC->CFGR & RCC_CFGR_PPRE1) == 0)
  { 
    /* PCLK1 prescaler equal to 1 => TIMCLK = PCLK1 */
    return ((pclk1 * 8) / uwPeriodValue);
  }
  else
  { /* PCLK1 prescaler different from 1 => TIMCLK = 2 * PCLK1 */
    return (((2 * pclk1) * 8) / uwPeriodValue);
  }
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 168000000
  *            HCLK(Hz)                       = 168000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 4
  *            APB2 Prescaler                 = 2
  *            HSE Frequency(Hz)              = 25000000
  *            PLL_M                          = 25
  *            PLL_N                          = 336
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
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);
  
  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;  
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);

  /* STM32F405x/407x/415x/417x Revision Z and upper devices: prefetch is supported  */
  if (HAL_GetREVID() >= 0x1001)
  {
    /* Enable the Flash prefetch */
    __HAL_FLASH_PREFETCH_BUFFER_ENABLE();
  }
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  /* Turn LED3 on */
  BSP_LED_On(LED3);
  while(1)
  {
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
void assert_failed(uint8_t* file, uint32_t line)
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
