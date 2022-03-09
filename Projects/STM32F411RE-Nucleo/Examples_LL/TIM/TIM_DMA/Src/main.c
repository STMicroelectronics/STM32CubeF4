/**
  ******************************************************************************
  * @file    Examples_LL/TIM/TIM_DMA/Src/main.c
  * @author  MCD Application Team
  * @brief   This example describes how to use DMA with TIM3 Update request to
  *          transfer Data from memory to TIM3 Capture Compare Register 3 (CCR3)
  *          using the STM32F4xx TIM LL API.
  *          Peripheral initialization done using LL unitary services functions.
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

/** @addtogroup STM32F4xx_LL_Examples
  * @{
  */

/** @addtogroup TIM_DMA
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define CC_VALUE_NB       3

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Capture Compare buffer */
static uint32_t aCCValue[CC_VALUE_NB] = {0};

/* TIM3 Clock */
static uint32_t TimOutClock = 1;

/* Private function prototypes -----------------------------------------------*/
__STATIC_INLINE void     SystemClock_Config(void);
__STATIC_INLINE void     Configure_DMA(void);
__STATIC_INLINE void     Configure_TIM(void);
__STATIC_INLINE void     LED_Init(void);
__STATIC_INLINE void     LED_Blinking(uint32_t Period);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  /* Configure the system clock to 80 MHz */
  SystemClock_Config();

  /* Initialize LED2 */
  LED_Init();

  /* Configure DMA transfer */  
  Configure_DMA();
  
  /* Configure timer instance */
  Configure_TIM();

  /* Infinite loop */
  while (1)
  {
  }
}

/**
  * @brief  This function enables the peripheral clock for the DMA,
  *         configures the DMA transfer, configures the NVIC for DMA and
  *         enables the DMA.
  * @param  None
  * @retval None
  */
__STATIC_INLINE void  Configure_DMA(void)
{  
  /******************************************************/
  /* Configure NVIC for DMA transfer related interrupts */
  /******************************************************/
  NVIC_SetPriority(DMA1_Stream7_IRQn, 0);
  NVIC_EnableIRQ(DMA1_Stream7_IRQn);

  /*****************************/
  /* Peripheral clock enabling */
  /*****************************/
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1);

  /******************************/
  /* DMA transfer Configuration */
  /******************************/
  LL_DMA_ConfigTransfer(DMA1, LL_DMA_STREAM_7, LL_DMA_DIRECTION_MEMORY_TO_PERIPH |
                                                LL_DMA_PRIORITY_HIGH              |
                                                LL_DMA_MODE_CIRCULAR              |
                                                LL_DMA_PERIPH_NOINCREMENT         |
                                                LL_DMA_MEMORY_INCREMENT           |
                                                LL_DMA_PDATAALIGN_WORD            |
                                                LL_DMA_MDATAALIGN_WORD);
  LL_DMA_SetChannelSelection(DMA1, LL_DMA_STREAM_7, LL_DMA_CHANNEL_5);
  LL_DMA_ConfigAddresses(DMA1, LL_DMA_STREAM_7, (uint32_t)&aCCValue, (uint32_t)&TIM3->CCR3, LL_DMA_GetDataTransferDirection(DMA1, LL_DMA_STREAM_7));
  LL_DMA_SetDataLength(DMA1, LL_DMA_STREAM_7, CC_VALUE_NB);
  LL_DMA_EnableIT_TC(DMA1, LL_DMA_STREAM_7);
  LL_DMA_EnableIT_TE(DMA1, LL_DMA_STREAM_7);
    
  /***************************/
  /* Enable the DMA transfer */
  /***************************/
  LL_DMA_EnableStream(DMA1, LL_DMA_STREAM_7);
}


/**
  * @brief  This function configures TIM3 channel 3 to generate a PWM edge
  *         aligned signal with a frequency equal to 17.57 KHz and a variable
  *         duty cycle that is changed by the DMA after a specific number of 
  *         update DMA requests. The number of this repetitive requests is
  *         defined by the TIM3 repetition counter, each 4 update requests, the
  *         TIM3 Channel 3 Duty Cycle changes to the next new value defined by
  *         the aCCValue.
  * @note   Peripheral configuration is minimal configuration from reset values.
  *         Thus, some useless LL unitary functions calls below are provided as
  *         commented examples - setting is default configuration from reset.
  * @retval None
  */
__STATIC_INLINE void  Configure_TIM(void)
{  
  /*************************/
  /* GPIO AF configuration */
  /*************************/
  /* Enable the peripheral clock of GPIOs */
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);

  /* GPIO TIM3_CH3 configuration */
  LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_0, LL_GPIO_MODE_ALTERNATE);
  LL_GPIO_SetPinPull(GPIOB, LL_GPIO_PIN_0, LL_GPIO_PULL_DOWN);
  LL_GPIO_SetPinSpeed(GPIOB, LL_GPIO_PIN_0, LL_GPIO_SPEED_FREQ_HIGH);
  LL_GPIO_SetAFPin_0_7(GPIOB, LL_GPIO_PIN_0, LL_GPIO_AF_2);

  /******************************************************/
  /* Configure the NVIC to handle TIM3 update interrupt */
  /******************************************************/
  NVIC_SetPriority(TIM3_IRQn, 0);
  NVIC_EnableIRQ(TIM3_IRQn);
  
  /******************************/
  /* Peripheral clocks enabling */
  /******************************/
  /* Enable the peripheral clock of TIM3 */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM3);
  
  /***************************/
  /* Time base configuration */
  /***************************/
  /* Set counter mode */
  /* Reset value is LL_TIM_COUNTERMODE_UP */
  //LL_TIM_SetCounterMode(TIM3, LL_TIM_COUNTERMODE_UP);
  
  /* Set the TIM3 auto-reload register to get a PWM frequency at 17.57 KHz */
  /* Note that the timer pre-scaler isn't used, therefore the timer counter   */
  /* clock frequency is equal to the timer frequency.                        */
    /* In this example TIM3 input clock (TIM3CLK) frequency is set to APB1 clock*/
  /*  (PCLK1), since APB1 pre-scaler is equal to 2 and it is twice PCLK2.                        */
  /*    TIM3CLK = PCLK2                                                     */
  /*    PCLK2 = HCLK                                                        */
  /*    => TIM3CLK = HCLK = SystemCoreClock (100 Mhz)                       */
  
  /* TIM3CLK = SystemCoreClock / (APB prescaler & multiplier)              */
  TimOutClock = SystemCoreClock/1;
  LL_TIM_SetAutoReload(TIM3, __LL_TIM_CALC_ARR(TimOutClock, LL_TIM_COUNTERMODE_UP, 17570));
  
  /* Set the repetition counter in order to generate one update event every 4 */
  /* counter cycles.                                                          */
  LL_TIM_SetRepetitionCounter(TIM3, 4-1);
  
  /*********************************/
  /* Output waveform configuration */
  /*********************************/
  /* Set output channel 3 in PWM1 mode */
  LL_TIM_OC_SetMode(TIM3,  LL_TIM_CHANNEL_CH3,  LL_TIM_OCMODE_PWM1);

  /* TIM3 channel 3 configuration:    */
  LL_TIM_OC_ConfigOutput(TIM3, LL_TIM_CHANNEL_CH3, LL_TIM_OCPOLARITY_HIGH | LL_TIM_OCIDLESTATE_HIGH);

  /* Compute compare value to generate a duty cycle at 75% */
  aCCValue[0] = (uint32_t)(((uint32_t) 75 * (LL_TIM_GetAutoReload(TIM3) - 1)) / 100);
  /* Compute compare value to generate a duty cycle at 50% */
  aCCValue[1] = (uint32_t)(((uint32_t) 50 * (LL_TIM_GetAutoReload(TIM3) - 1)) / 100);
  /* Compute compare value to generate a duty cycle at 25% */
  aCCValue[2] = (uint32_t)(((uint32_t) 25 * (LL_TIM_GetAutoReload(TIM3) - 1)) / 100);

  /* Set PWM duty cycle  for TIM3 channel 3*/
  LL_TIM_OC_SetCompareCH3(TIM3, aCCValue[0]);
  
  /* Enable register preload for TIM3 channel 3 */
  LL_TIM_OC_EnablePreload(TIM3, LL_TIM_CHANNEL_CH3);
  
  /****************************/
  /* TIM3 DMA requests set-up */
  /****************************/
  /* Enable DMA request on update event */
  LL_TIM_EnableDMAReq_UPDATE(TIM3);

  /* Enable TIM3 Channel 3 DMA request */
  LL_TIM_EnableDMAReq_CC3(TIM3);
  
  /**********************************/
  /* Start output signal generation */
  /**********************************/
  /* Enable TIM3 channel 3 */
  LL_TIM_CC_EnableChannel(TIM3, LL_TIM_CHANNEL_CH3);
  
  /* Enable TIM3 outputs */
  LL_TIM_EnableAllOutputs(TIM3);
  
  /* Enable counter */
  LL_TIM_EnableCounter(TIM3);
  
  /* Force update generation */
  LL_TIM_GenerateEvent_UPDATE(TIM3);
}

/**
  * @brief  Initialize LED2.
  * @param  None
  * @retval None
  */
__STATIC_INLINE void LED_Init(void)
{
  /* Enable the LED2 Clock */
  LED2_GPIO_CLK_ENABLE();

  /* Configure IO in output push-pull mode to drive external LED2 */
  LL_GPIO_SetPinMode(LED2_GPIO_PORT, LED2_PIN, LL_GPIO_MODE_OUTPUT);
  LL_GPIO_SetPinOutputType(LED2_GPIO_PORT, LED2_PIN, LL_GPIO_OUTPUT_PUSHPULL);
  LL_GPIO_SetPinSpeed(LED2_GPIO_PORT, LED2_PIN, LL_GPIO_SPEED_FREQ_LOW);
  LL_GPIO_SetPinPull(LED2_GPIO_PORT, LED2_PIN, LL_GPIO_PULL_NO);
}

/**
  * @brief  Set LED2 to Blinking mode for an infinite loop (toggle period based on value provided as input parameter).
  * @param  Period : Period of time (in ms) between each toggling of LED
  *   This parameter can be user defined values. Pre-defined values used in that example are :
  *     @arg LED_BLINK_FAST : Fast Blinking
  *     @arg LED_BLINK_SLOW : Slow Blinking
  *     @arg LED_BLINK_ERROR : Error specific Blinking
  * @retval None
  */
__STATIC_INLINE void LED_Blinking(uint32_t Period)
{
  /* Toggle IO in an infinite loop */
  while (1)
  {
    LL_GPIO_TogglePin(LED2_GPIO_PORT, LED2_PIN);  
    LL_mDelay(Period);
  }
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow :
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 100000000
  *            HCLK(Hz)                       = 100000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 2
  *            APB2 Prescaler                 = 1
  *            HSE Frequency(Hz)              = 8000000
  *            PLL_M                          = 8
  *            PLL_N                          = 400
  *            PLL_P                          = 4
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale1 mode
  *            Flash Latency(WS)              = 3
  * @param  None
  * @retval None
  */
void SystemClock_Config(void)
{
  /* Enable HSE oscillator */
  LL_RCC_HSE_EnableBypass();
  LL_RCC_HSE_Enable();
  while(LL_RCC_HSE_IsReady() != 1)
  {
  };

  /* Set FLASH latency */
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_3);

  /* Main PLL configuration and activation */
  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_8, 400, LL_RCC_PLLP_DIV_4);
  LL_RCC_PLL_Enable();
  while(LL_RCC_PLL_IsReady() != 1)
  {
  };

  /* Sysclk activation on the main PLL */
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
  {
  };

  /* Set APB1 & APB2 prescaler */
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_2);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);

  /* Set systick to 1ms */
  SysTick_Config(100000000 / 1000);

  /* Update CMSIS variable (which can be updated also through SystemCoreClockUpdate function) */
  SystemCoreClock = 100000000;
}

/******************************************************************************/
/*   USER IRQ HANDLER TREATMENT                                               */
/******************************************************************************/
/**
  * @brief  Timer update interrupt processing
  * @param  None
  * @retval None
  */
void TimerUpdate_Callback(void)
{
  static uint32_t UpdateEventCnt = 0;

  /* At every update event the CCR3 register is updated with a new value */
  /* which is DMA transferred from aCCValue[].                           */
  /* Note that the update event (UEV) is generated after upcounting is   */
  /* repeated for the number of times programmed in the repetition       */
  /* counter register (TIM1_RCR) + 1                                     */
  if (LL_TIM_OC_GetCompareCH3(TIM3) != aCCValue[UpdateEventCnt])
  {
    LED_Blinking(LED_BLINK_ERROR);
  }
  else
  {
    UpdateEventCnt = (UpdateEventCnt+1) % CC_VALUE_NB;
  }
}

/**
  * @brief  DMA transfer complete callback
  * @note   This function is executed when the transfer complete interrupt
  *         is generated after DMA transfer
  * @retval None
  */
void TransferComplete_Callback()
{
  /* Once the DMA transfer is completed the CCR3 value must match */
  /* the value of the last element of aCCValue[].                 */
  if (LL_TIM_OC_GetCompareCH3(TIM3) != aCCValue[CC_VALUE_NB-1])
  {
    LED_Blinking(LED_BLINK_ERROR);
  }
}

/**
  * @brief  DMA transfer error callback
  * @note   This function is executed when the transfer error interrupt
  *         is generated during DMA transfer
  * @retval None
  */
void TransferError_Callback()
{
  LED_Blinking(LED_BLINK_ERROR);
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
     ex: printf("Wrong parameters value: file %s on line %d", file, line) */

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
