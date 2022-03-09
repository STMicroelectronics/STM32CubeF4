/**
  ******************************************************************************
  * @file    TIM/TIM_CascadeSynchro/Src/main.c
  * @author  MCD Application Team
  * @brief   This example shows how to command 2 Timers as slaves (TIM3 & TIM4)
  *          using a Timer as master (TIM2)
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

/** @addtogroup TIM_CascadeSynchro
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Timer1 handler declaration: Master */
TIM_HandleTypeDef         TimMasterHandle;

/* Timer3 handler declaration: Slave1 */
TIM_HandleTypeDef         TimSlave1Handle;

/* Timer4 handler declaration: Slave2 */
TIM_HandleTypeDef         TimSlave2Handle;

/* Output compare structure */
TIM_OC_InitTypeDef        sOCConfig;

/* Master configuration structure */
TIM_MasterConfigTypeDef   sMasterConfig;

/* Slave configuration structure */
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

  /* Configure LED3 */
  BSP_LED_Init(LED3);
  
  /* Timers configuration ------------------------------------------------------
     1/TIM2 is configured as Master Timer:
         - PWM Mode is used
         - The TIM2 Update event is used as Trigger Output  

     2/TIM3 is slave for TIM2 and Master for TIM4,
         - PWM Mode is used
         - The ITR1(TIM2) is used as input trigger 
         - Gated mode is used, so start and stop of slave counter
           are controlled by the Master trigger output signal(TIM2 update event).
         - The TIM3 Update event is used as Trigger Output. 

     3/TIM4 is slave for TIM3,
         - PWM Mode is used
         - The ITR2(TIM3) is used as input trigger
         - Gated mode is used, so start and stop of slave counter
           are controlled by the Master trigger output signal(TIM3 update event).

     In this example TIM2 input clock (TIM2CLK) is set to 2 * APB1 clock (PCLK1), 
     since APB1 prescaler is different from 1.   
     TIM2CLK = 2 * PCLK1  
     PCLK1 = HCLK / 4 
     => TIM2CLK = HCLK / 2 = SystemCoreClock /2

    The TIM2 counter clock is equal to SystemCoreClock/2 = 180 MHz/2.

    The Master Timer TIM2 is running at:
    TIM2 frequency = TIM2 counter clock / (TIM2_Period + 1) = 351.562 KHz
    TIM2_Period = (TIM2 counter clock / TIM2 frequency) - 1 = 182
    and the duty cycle is equal to: TIM2_CCR1/(TIM2_ARR + 1) = 25%

     The TIM3 is running:
     - At (TIM2 frequency)/ (TIM3 period + 1) = 87.891 KHz and a duty cycle
       equal to TIM3_CCR1/(TIM3_ARR + 1) = 25%

     The TIM4 is running:
     - At (TIM3 frequency)/ (TIM4 period + 1) = 21.973 KHz and a duty cycle
       equal to TIM4_CCR1/(TIM4_ARR + 1) = 25%

    Note:
     SystemCoreClock variable holds HCLK frequency and is defined in SystemClock_Config().
     Each time the core clock (HCLK) changes, user had to update SystemCoreClock
     variable value. Otherwise, any configuration based on this variable will be incorrect.
     This variable is updated in three ways:
      1) by calling CMSIS function SystemCoreClockUpdate()
      2) by calling HAL API function HAL_RCC_GetSysClockFreq()
      3) each time HAL_RCC_ClockConfig() is called to configure the system clock frequency
  --------------------------------------------------------------------------- */

  /* Set Timers instance */
  TimMasterHandle.Instance = TIM2;
  TimSlave1Handle.Instance = TIM3;
  TimSlave2Handle.Instance = TIM4;
 
  /*====================== Master configuration : TIM2 =======================*/
  /* Initialize TIM2 peripheral in PWM mode*/
  TimMasterHandle.Init.Period            = 255;
  TimMasterHandle.Init.Prescaler         = 0;
  TimMasterHandle.Init.ClockDivision     = 0;
  TimMasterHandle.Init.CounterMode       = TIM_COUNTERMODE_UP;
  TimMasterHandle.Init.RepetitionCounter = 0;
  TimMasterHandle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if(HAL_TIM_PWM_Init(&TimMasterHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /* Configure the PWM_channel_1  */
  sOCConfig.OCMode       = TIM_OCMODE_PWM1;
  sOCConfig.OCPolarity   = TIM_OCPOLARITY_HIGH;
  sOCConfig.Pulse        = 64;  
  sOCConfig.OCNPolarity  = TIM_OCNPOLARITY_HIGH;
  sOCConfig.OCFastMode   = TIM_OCFAST_DISABLE;
  sOCConfig.OCIdleState  = TIM_OCIDLESTATE_RESET;
  sOCConfig.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  if (HAL_TIM_PWM_ConfigChannel(&TimMasterHandle, &sOCConfig, TIM_CHANNEL_1) != HAL_OK)
  {
    /* Configuration Error */
    Error_Handler();
  }

  /* Configure TIM2 as master & use the update event as Trigger Output (TRGO) */
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
  sMasterConfig.MasterSlaveMode     = TIM_MASTERSLAVEMODE_ENABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&TimMasterHandle, &sMasterConfig) != HAL_OK)
  {
    /* Configuration Error */
    Error_Handler();
  }     
  
  /*================== End of Master configuration : TIM2 ====================*/


  /*====================== Slave1 configuration : TIM3 =======================*/
  /* Initialize TIM3 peripheral in PWM mode*/
  TimSlave1Handle.Init.Period            = 3;
  TimSlave1Handle.Init.Prescaler         = 0;
  TimSlave1Handle.Init.ClockDivision     = 0;
  TimSlave1Handle.Init.CounterMode       = TIM_COUNTERMODE_UP;
  TimSlave1Handle.Init.RepetitionCounter = 0;
  TimSlave1Handle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_PWM_Init(&TimSlave1Handle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /* Configure the PWM_channel_1  */
  sOCConfig.OCMode     = TIM_OCMODE_PWM1;
  sOCConfig.OCPolarity = TIM_OCPOLARITY_HIGH;
  sOCConfig.Pulse = 1;
  if (HAL_TIM_PWM_ConfigChannel(&TimSlave1Handle, &sOCConfig, TIM_CHANNEL_1) != HAL_OK)
  {
    /* Configuration Error */
    Error_Handler();
  }

  /* Configure TIM3 in Gated slave mode &
  use the Internal Trigger 1 (ITR1) as trigger source */
  sSlaveConfig.SlaveMode        = TIM_SLAVEMODE_GATED;
  sSlaveConfig.InputTrigger     = TIM_TS_ITR1;
  sSlaveConfig.TriggerPolarity  = TIM_TRIGGERPOLARITY_NONINVERTED;
  sSlaveConfig.TriggerPrescaler = TIM_TRIGGERPRESCALER_DIV1;
  sSlaveConfig.TriggerFilter    = 0;
  if (HAL_TIM_SlaveConfigSynchronization(&TimSlave1Handle, &sSlaveConfig) != HAL_OK)
  {
    /* Configuration Error */
    Error_Handler();
  }
  
  /* Configure TIM3 as master & use the update event as Trigger Output (TRGO) */
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
  sMasterConfig.MasterSlaveMode     = TIM_MASTERSLAVEMODE_ENABLE;
  if( HAL_TIMEx_MasterConfigSynchronization(&TimSlave1Handle,&sMasterConfig) != HAL_OK)
  {
    /* Configuration Error */
    Error_Handler();
  } 

  /*================== End of Slave1 configuration : TIM3 ====================*/


  /*====================== Slave2 configuration : TIM4 =======================*/
  /* Initialize TIM4 peripheral in PWM mode*/
  TimSlave2Handle.Init.Period            = 3;
  TimSlave2Handle.Init.Prescaler         = 0;
  TimSlave2Handle.Init.ClockDivision     = 0;
  TimSlave2Handle.Init.CounterMode       = TIM_COUNTERMODE_UP;
  TimSlave2Handle.Init.RepetitionCounter = 0;
  TimSlave2Handle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_PWM_Init(&TimSlave2Handle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /* Configure the PWM_channel_3  */
  sOCConfig.OCMode     = TIM_OCMODE_PWM1;
  sOCConfig.OCPolarity = TIM_OCPOLARITY_HIGH;
  sOCConfig.Pulse = 1;
  if (HAL_TIM_PWM_ConfigChannel(&TimSlave2Handle, &sOCConfig, TIM_CHANNEL_3) != HAL_OK)
  {
    /* Configuration Error */
    Error_Handler();
  }

  /* Configure TIM4 in Gated slave mode &
  use the Internal Trigger 2 (ITR2) as trigger source */
  sSlaveConfig.SlaveMode     = TIM_SLAVEMODE_GATED;
  sSlaveConfig.InputTrigger  = TIM_TS_ITR2;
  if (HAL_TIM_SlaveConfigSynchronization(&TimSlave2Handle, &sSlaveConfig) != HAL_OK)
  {
    /* Configuration Error */
    Error_Handler();
  }

  /*================== End of Slave2 configuration : TIM4 ====================*/


  /* Start Master PWM generation */
  if (HAL_TIM_PWM_Start(&TimMasterHandle, TIM_CHANNEL_1) != HAL_OK)
  {
    /* PWM generation Error */
    Error_Handler();
  }

  /* Start Slave1 PWM generation */
  if (HAL_TIM_PWM_Start(&TimSlave1Handle, TIM_CHANNEL_1) != HAL_OK)
  {
    /* PWM generation Error */
    Error_Handler();
  }
  /* Start Slave2 PWM generation */
  if (HAL_TIM_PWM_Start(&TimSlave2Handle, TIM_CHANNEL_3) != HAL_OK)
  {
    /* PWM generation Error */
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
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 360;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  RCC_OscInitStruct.PLL.PLLR = 2;
  
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
