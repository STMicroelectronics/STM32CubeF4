/**
  ******************************************************************************
  * @file    TIM/TIM_ExtTriggerSynchro/Src/main.c
  * @author  MCD Application Team
  * @brief   This sample code shows how to synchronize TIM peripherals in cascade
  *          mode with an external trigger.
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

/** @addtogroup TIM_ExtTriggerSynchro
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* TIM1 handler declaration: Slave for an external trigger/Master */
TIM_HandleTypeDef    TimMasterHandle;
/* TIM4 handler declaration: Slave/Master */
TIM_HandleTypeDef    TimSlaveMasterHandle;
/* TIM5 handler declaration: Slave */
TIM_HandleTypeDef    TimSlaveHandle;

/* Timer Output Compare Configuration Structure declaration */
TIM_OC_InitTypeDef   sOCConfig;
/* Timer Input Capture Configuration Structure declaration */
TIM_IC_InitTypeDef   sICConfig;

/* Timer Master Configuration Structure declaration */
TIM_MasterConfigTypeDef  sMasterConfig;
/* Timer Slave Configuration Structure declaration */
TIM_SlaveConfigTypeDef   sSlaveConfig;

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void Error_Handler(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
  /* STM32F4xx HAL library initialization:
       - Configure the Flash prefetch, instruction and Data caches
       - Systick timer is configured by default as source of time base, but user 
         can eventually implement his proper time base source (a general purpose 
         timer for example or other time source), keeping in mind that Time base 
         duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and 
         handled in milliseconds basis.
       - Set NVIC Group Priority to 4
       - Low Level Initialization: global MSP (MCU Support Package) initialization
     */
  HAL_Init();

  /* Configure the system clock to 180 MHz */
  SystemClock_Config();

  /* Configure LED3 */
  BSP_LED_Init(LED3);

  /* Set Timers instance */
  TimMasterHandle.Instance      = TIM1;
  TimSlaveMasterHandle.Instance = TIM4;
  TimSlaveHandle.Instance       = TIM5;
 
  /*======= Master1/Slave for an external trigger configuration : TIM1 =======*/
  /* Initialize TIM1 peripheral in Output Compare mode*/
  TimMasterHandle.Init.Period            = 89;
  TimMasterHandle.Init.Prescaler         = (SystemCoreClock / 18000000) - 1;;
  TimMasterHandle.Init.ClockDivision     = 0;
  TimMasterHandle.Init.CounterMode       = TIM_COUNTERMODE_UP;
  TimMasterHandle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if(HAL_TIM_OC_Init(&TimMasterHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }  
  
  /* Configure the output: Channel_1 */
  sOCConfig.OCMode     = TIM_OCMODE_TOGGLE;
  sOCConfig.OCPolarity = TIM_OCPOLARITY_HIGH;    
  if(HAL_TIM_OC_ConfigChannel(&TimMasterHandle, &sOCConfig, TIM_CHANNEL_1) != HAL_OK)
  {
    /* Configuration Error */
    Error_Handler();
  }

  /* Configure the Input: channel_2 */
  sICConfig.ICPolarity  = TIM_ICPOLARITY_RISING;
  sICConfig.ICSelection = TIM_ICSELECTION_DIRECTTI;
  sICConfig.ICPrescaler = TIM_ICPSC_DIV1;
  sICConfig.ICFilter = 0;  
  if(HAL_TIM_IC_ConfigChannel(&TimMasterHandle, &sICConfig, TIM_CHANNEL_2) != HAL_OK)
  {
    /* Configuration Error */
    Error_Handler();
  }
  
  /* Configure TIM1 in Gated Slave mode for the external trigger (Filtered Timer
     Input 2) */
  sSlaveConfig.InputTrigger = TIM_TS_TI2FP2;
  sSlaveConfig.SlaveMode    = TIM_SLAVEMODE_GATED;
  if( HAL_TIM_SlaveConfigSynchronization(&TimMasterHandle, &sSlaveConfig) != HAL_OK)
  {
    /* Configuration Error */
    Error_Handler();
  }
  
  /* Configure TIM1 in Master Enable mode & use the update event as Trigger
     Output (TRGO) */
  sMasterConfig.MasterSlaveMode     = TIM_MASTERSLAVEMODE_ENABLE;
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_ENABLE;
  if( HAL_TIMEx_MasterConfigSynchronization(&TimMasterHandle, &sMasterConfig) != HAL_OK)
  {
    /* Configuration Error */
    Error_Handler();
  }  
  /*=== End of Master1/Slave for an external trigger configuration : TIM1 ====*/

  
  /*=================== Slave/Master configuration : TIM4 ====================*/
  /* Initialize TIM4 peripheral in Output Compare mode*/
  TimSlaveMasterHandle.Init.Period            = 89;
  TimSlaveMasterHandle.Init.Prescaler         = ((SystemCoreClock/2) / 16000000) - 1;;
  TimSlaveMasterHandle.Init.ClockDivision     = 0;
  TimSlaveMasterHandle.Init.CounterMode       = TIM_COUNTERMODE_UP;
  TimSlaveMasterHandle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if(HAL_TIM_OC_Init(&TimSlaveMasterHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
  
  /* Configure the Output Compare channel_3  */
  sOCConfig.OCMode     = TIM_OCMODE_TOGGLE;
  sOCConfig.OCPolarity = TIM_OCPOLARITY_HIGH;
  if(HAL_TIM_OC_ConfigChannel(&TimSlaveMasterHandle, &sOCConfig, TIM_CHANNEL_3) != HAL_OK)
  {
    /* Configuration Error */
    Error_Handler();
  }
  
  /* Configure TIM4 in Gated Slave mode for the internal trigger 0(ITR0) */  
  sSlaveConfig.InputTrigger = TIM_TS_ITR0;
  sSlaveConfig.SlaveMode    = TIM_SLAVEMODE_GATED;
  if( HAL_TIM_SlaveConfigSynchronization(&TimSlaveMasterHandle, &sSlaveConfig) != HAL_OK)
  {
    /* Configuration Error */
    Error_Handler();
  }
  
  /* Configure TIM4 in Master Enable mode & use the update event as Trigger
     Output (TRGO) */
  sMasterConfig.MasterSlaveMode     = TIM_MASTERSLAVEMODE_ENABLE;
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_ENABLE;
  if( HAL_TIMEx_MasterConfigSynchronization(&TimSlaveMasterHandle, &sMasterConfig) != HAL_OK)
  {
    /* Configuration Error */
    Error_Handler();
  }
  /*=============== End of Slave/Master configuration : TIM4 =================*/
  
  
  /*====================== Slave configuration : TIM5 ========================*/
  /* Initialize TIM5 peripheral in Output Compare mode*/
  TimSlaveHandle.Init.Period            = 89;
  TimSlaveHandle.Init.Prescaler         = ((SystemCoreClock/2) / 18000000) - 1;;
  TimSlaveHandle.Init.ClockDivision     = 0;
  TimSlaveHandle.Init.CounterMode       = TIM_COUNTERMODE_UP;
  TimSlaveHandle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if(HAL_TIM_OC_Init(&TimSlaveHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  } 
 
  /* Configure the Output Compare channel_1  */
  sOCConfig.OCMode     = TIM_OCMODE_TOGGLE;
  sOCConfig.OCPolarity = TIM_OCPOLARITY_HIGH;
  if(HAL_TIM_OC_ConfigChannel(&TimSlaveHandle, &sOCConfig, TIM_CHANNEL_1) != HAL_OK)
  {
    /* Configuration Error */
    Error_Handler();
  }  
  
  /* Configure TIM5 in Gated Slave mode for the internal trigger 2(ITR2) */ 
  sSlaveConfig.SlaveMode    = TIM_SLAVEMODE_GATED;
  sSlaveConfig.InputTrigger = TIM_TS_ITR2;
  if(HAL_TIM_SlaveConfigSynchronization(&TimSlaveHandle, &sSlaveConfig) != HAL_OK)
  {
    /* Configuration Error */
    Error_Handler();
  }  
  /*================== End of Slave configuration : TIM5 =====================*/
  
  
  /* 1- Start Timer1 xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx */
  /* Start Channel2 in Input Capture */
  if(HAL_TIM_IC_Start(&TimMasterHandle, TIM_CHANNEL_2) != HAL_OK)
  {
    /* Start Error */
    Error_Handler();
  }  
  /* Start the Output Compare */
  if(HAL_TIM_OC_Start(&TimMasterHandle, TIM_CHANNEL_1) != HAL_OK)
  {
    /* Start Error */
    Error_Handler();
  }

  /* 2- Start Timer3 xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx */
  /* Start the Output Compare */
  if(HAL_TIM_OC_Start(&TimSlaveMasterHandle, TIM_CHANNEL_3) != HAL_OK)
  {
    /* Start Error */
    Error_Handler();
  }
  
  /* 3- Start Timer3 xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx */
  /* Start the Output Compare */
  if(HAL_TIM_OC_Start(&TimSlaveHandle, TIM_CHANNEL_1) != HAL_OK)
  {
    /* Start Error */
    Error_Handler();
  }
  
  /* Infinite loop */
  while (1)
  {
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
  *            HSE Frequency(Hz)              = 25000000
  *            PLL_M                          = 25
  *            PLL_N                          = 360
  *            PLL_P                          = 2
  *            PLL_Q                          = 7
  *            PLL_R                          = 6
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
  HAL_StatusTypeDef ret = HAL_OK;

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
  RCC_OscInitStruct.PLL.PLLN = 360;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  RCC_OscInitStruct.PLL.PLLR = 6;
  
  ret = HAL_RCC_OscConfig(&RCC_OscInitStruct);
  if(ret != HAL_OK)
  {
    while(1) { ; }
  }
  
  /* Activate the OverDrive to reach the 180 MHz Frequency */  
  ret = HAL_PWREx_EnableOverDrive();
  if(ret != HAL_OK)
  {
    while(1) { ; }
  }
  
  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;  
  
  ret = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);
  if(ret != HAL_OK)
  {
    while(1) { ; }
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
