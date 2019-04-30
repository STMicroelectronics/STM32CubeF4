/**
  ******************************************************************************
  * @file    Examples_LL/DAC/DAC_GenerateWaveform_TriggerHW_Init/Src/main.c
  * @author  MCD Application Team
  * @brief   This example describes how to use the DAC peripheral to generate
  *          a voltage waveform from digital data stream transfered by DMA.
  *          This example is based on the STM32F4xx DAC LL API;
  *          Peripheral initialization done using LL initialization function.
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

/** @addtogroup STM32F4xx_LL_Examples
  * @{
  */

/** @addtogroup DAC_GenerateWaveform_TriggerHW_Init
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* Definitions of environment analog values */
  /* Value of analog reference voltage (Vref+), connected to analog voltage   */
  /* supply Vdda (unit: mV).                                                  */
  #define VDDA_APPLI                       ((uint32_t)3300)
  
/* Definitions of data related to this example */
  /* Full-scale digital value with a resolution of 12 bits (voltage range     */
  /* determined by analog voltage references Vref+ and Vref-,                 */
  /* refer to reference manual).                                              */
  #define DIGITAL_SCALE_12BITS             (__LL_DAC_DIGITAL_SCALE(LL_DAC_RESOLUTION_12B))

/* Definitions of waveform generation values */
  /* Waveform generation: parameters of waveform */
  /* Waveform amplitude (unit: mV) */
  #define WAVEFORM_AMPLITUDE          (VDDA_APPLI)
  /* Waveform amplitude (unit: Hz) */
  #define WAVEFORM_FREQUENCY          ((uint32_t)1000)
  /* Size of array containing DAC waveform samples */
  #define WAVEFORM_SAMPLES_SIZE       (sizeof (WaveformSine_12bits_32samples) / sizeof (uint16_t))
  
  /* Waveform generation: parameters of timer (used as DAC trigger) */
  /* Timer frequency (unit: Hz). With a timer 16 bits and time base           */
  /* freq min 1Hz, range is min=1Hz, max=32kHz.                               */
  #define WAVEFORM_TIMER_FREQUENCY                (WAVEFORM_FREQUENCY * WAVEFORM_SAMPLES_SIZE)
  /* Timer minimum frequency (unit: Hz), used to calculate frequency range.   */
  /* With a timer 16 bits, maximum frequency will be 32000 times this value.  */
  #define WAVEFORM_TIMER_FREQUENCY_RANGE_MIN      ((uint32_t)    1)
  /* Timer prescaler maximum value (0xFFFF for a timer 16 bits) */
  #define WAVEFORM_TIMER_PRESCALER_MAX_VALUE      ((uint32_t)0xFFFF-1)


/* Private macro -------------------------------------------------------------*/
/**
  * @brief  Computation of a data from maximum value on digital scale 12 bits 
  *         (corresponding to voltage Vdda)
  *         to a value on the new scale
  *         (corresponding to voltage defined by WAVEFORM_AMPLITUDE).
  * @param  __DATA_12BITS__: Digital value on scale 12 bits
  * @retval None
  */
#define __WAVEFORM_AMPLITUDE_SCALING(__DATA_12BITS__)                                     \
  (__DATA_12BITS__                                                                        \
   * __LL_DAC_CALC_VOLTAGE_TO_DATA(VDDA_APPLI, WAVEFORM_AMPLITUDE, LL_DAC_RESOLUTION_12B) \
   / __LL_DAC_DIGITAL_SCALE(LL_DAC_RESOLUTION_12B)                                        \
  )

/* Private variables ---------------------------------------------------------*/
__IO uint8_t ubButtonPress = 0;

/* Private variables ---------------------------------------------------------*/

const uint16_t WaveformSine_12bits_32samples[] =
{
__WAVEFORM_AMPLITUDE_SCALING(2048),
__WAVEFORM_AMPLITUDE_SCALING(2447),
__WAVEFORM_AMPLITUDE_SCALING(2831),
__WAVEFORM_AMPLITUDE_SCALING(3185),
__WAVEFORM_AMPLITUDE_SCALING(3495),
__WAVEFORM_AMPLITUDE_SCALING(3750),
__WAVEFORM_AMPLITUDE_SCALING(3939),
__WAVEFORM_AMPLITUDE_SCALING(4056),
__WAVEFORM_AMPLITUDE_SCALING(4095),
__WAVEFORM_AMPLITUDE_SCALING(4056),
__WAVEFORM_AMPLITUDE_SCALING(3939),
__WAVEFORM_AMPLITUDE_SCALING(3750),
__WAVEFORM_AMPLITUDE_SCALING(3495),
__WAVEFORM_AMPLITUDE_SCALING(3185),
__WAVEFORM_AMPLITUDE_SCALING(2831),
__WAVEFORM_AMPLITUDE_SCALING(2447),
__WAVEFORM_AMPLITUDE_SCALING(2048),
__WAVEFORM_AMPLITUDE_SCALING(1649),
__WAVEFORM_AMPLITUDE_SCALING(1265),
__WAVEFORM_AMPLITUDE_SCALING(911),
__WAVEFORM_AMPLITUDE_SCALING(601),
__WAVEFORM_AMPLITUDE_SCALING(346),
__WAVEFORM_AMPLITUDE_SCALING(157),
__WAVEFORM_AMPLITUDE_SCALING(40),
__WAVEFORM_AMPLITUDE_SCALING(0),
__WAVEFORM_AMPLITUDE_SCALING(40),
__WAVEFORM_AMPLITUDE_SCALING(157),
__WAVEFORM_AMPLITUDE_SCALING(346),
__WAVEFORM_AMPLITUDE_SCALING(601),
__WAVEFORM_AMPLITUDE_SCALING(911),
__WAVEFORM_AMPLITUDE_SCALING(1265),
__WAVEFORM_AMPLITUDE_SCALING(1649)
};

/* Private function prototypes -----------------------------------------------*/
void     SystemClock_Config(void);
void     Configure_DMA(void);
void     Configure_TIM_TimeBase_DAC_trigger(void);
void     Configure_DAC(void);
void     Activate_DAC(void);
void     LED_Init(void);
void     LED_On(void);
void     LED_Off(void);
void     LED_Blinking(uint32_t Period);
void     UserButton_Init(void);
void     WaitForUserButtonPress(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  /* Configure the system clock to 180 MHz */
  SystemClock_Config();
  
  /* Initialize LED1 */
  LED_Init();
  
  /* Initialize button in EXTI mode */
  UserButton_Init();
  
  /* Wait for User push-button press */
  WaitForUserButtonPress();
  
  /* Turn-off LED1 */
  LED_Off();
  
  /* Configure DMA for data transfer from DAC */
  Configure_DMA();
  
  /* Configure timer as a time base used to trig DAC conversion start */
  Configure_TIM_TimeBase_DAC_trigger();
  
  /* Configure DAC channel */
  Configure_DAC();
  
  /* Activate DAC channel */
  Activate_DAC();
  
  /* Turn-on LED1 */
  LED_On();
  
  /* Infinite loop */
  while (1)
  {
  }
}

/**
  * @brief  This function configures DMA for transfer of data from DAC
  * @param  None
  * @retval None
  */
void Configure_DMA(void)
{
  /*## Configuration of NVIC #################################################*/ 
  /* Configure NVIC to enable DMA interruptions */
  NVIC_SetPriority(DMA1_Stream5_IRQn, 1); /* DMA IRQ lower priority than DAC IRQ */
  NVIC_EnableIRQ(DMA1_Stream5_IRQn);
  
  /*## Configuration of DMA ##################################################*/
  
  /* Enable the peripheral clock of DMA */
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1);
  /* Configure the DMA transfer */
  /*  - DMA transfer in circular mode to have an unlimited DAC signal         */
  /*    generation.                                                           */
  /*  - DMA transfer to DAC without address increment.                        */
  /*  - DMA transfer from memory with address increment.                      */
  /*  - DMA transfer to DAC by half-word to match with DAC resolution 12 bits */
  /*  - DMA transfer from memory by half-word to match with DAC data          */
  /*    buffer variable type: half-word.
                                      */
    
  LL_DMA_SetChannelSelection(DMA1, LL_DMA_STREAM_5, LL_DMA_CHANNEL_7);
  LL_DMA_ConfigTransfer(DMA1,
                        LL_DMA_STREAM_5,
                        LL_DMA_DIRECTION_MEMORY_TO_PERIPH |
                        LL_DMA_MODE_CIRCULAR              |
                        LL_DMA_PERIPH_NOINCREMENT         |
                        LL_DMA_MEMORY_INCREMENT           |
                        LL_DMA_PDATAALIGN_HALFWORD        |
                        LL_DMA_MDATAALIGN_HALFWORD        |
                        LL_DMA_PRIORITY_HIGH               );
  

  /* Set DMA transfer addresses of source and destination */
  LL_DMA_ConfigAddresses(DMA1,
                         LL_DMA_STREAM_5,
                         (uint32_t)&WaveformSine_12bits_32samples,
                         LL_DAC_DMA_GetRegAddr(DAC1, LL_DAC_CHANNEL_1, LL_DAC_DMA_REG_DATA_12BITS_RIGHT_ALIGNED),
                         LL_DMA_DIRECTION_MEMORY_TO_PERIPH);
  
  /* Set DMA transfer size */
  LL_DMA_SetDataLength(DMA1,
                       LL_DMA_STREAM_5,
                       WAVEFORM_SAMPLES_SIZE);
  
  /* Enable DMA transfer interruption: transfer error */
  LL_DMA_EnableIT_TE(DMA1,
                     LL_DMA_STREAM_5);
  
  /* Note: In this example, the only DMA interruption activated is            */
  /*       tranfer error.                                                     */
  /*       If needed, DMA interruptions of half of transfer                   */
  /*       and transfer complete can be activated.                            */
  /*       Refer to DMA examples.                                             */
  
  /*## Activation of DMA #####################################################*/
  /* Enable the DMA transfer */
  LL_DMA_EnableStream(DMA1,
                       LL_DMA_STREAM_5); 
}


/**
  * @brief  Configure timer as a time base (timer instance: TIM6) 
  *         used to trig DAC conversion.
  * @note   In this DC example, timer instance must be on APB1 (clocked by PCLK1)
  *         to be compliant with frequency calculation used in this function.
  * @param  None
  * @retval None
  */
void Configure_TIM_TimeBase_DAC_trigger(void)
{
  uint32_t timer_clock_frequency = 0;             /* Timer clock frequency */
  uint32_t timer_prescaler = 0;                   /* Time base prescaler to have timebase aligned on minimum frequency possible */
  uint32_t timer_reload = 0;                      /* Timer reload value in function of timer prescaler to achieve time base period */
  
  /*## Configuration of NVIC #################################################*/ 
  /* Note: In this example, timer interruptions are not activated.            */
  /*       If needed, timer interruption at each time base period is          */
  /*       possible.                                                          */
  /*       Refer to timer examples.                                           */
  
  /*## Configuration of timer ################################################*/
  
  /* Configuration of timer as time base:                                     */ 
  /* Caution: Computation of frequency is done for a timer instance on APB1   */
  /*          (clocked by PCLK1)                                              */
  /* Timer frequency is configured from the following constants:              */
  /* - WAVEFORM_TIMER_FREQUENCY: timer frequency (unit: Hz).                  */
  /* - WAVEFORM_TIMER_FREQUENCY_RANGE_MIN: timer minimum frequency possible   */
  /*   (unit: Hz).                                                            */
  /* Note: Refer to comments at these literals definition for more details.   */
  
  /* Retrieve timer clock source frequency */
  /* If APB1 prescaler is different of 1, timers have a factor x2 on their    */
  /* clock source.                                                            */
  if (LL_RCC_GetAPB1Prescaler() == LL_RCC_APB1_DIV_1)
  {
    timer_clock_frequency = __LL_RCC_CALC_PCLK1_FREQ(SystemCoreClock, LL_RCC_GetAPB1Prescaler());
  }
  else
  {
    timer_clock_frequency = (__LL_RCC_CALC_PCLK1_FREQ(SystemCoreClock, LL_RCC_GetAPB1Prescaler()) * 2);
  }
  
  /* Timer prescaler calculation */
  /* (computation for timer 16 bits, additional + 1 to round the prescaler up) */
  timer_prescaler = ((timer_clock_frequency / (WAVEFORM_TIMER_PRESCALER_MAX_VALUE * WAVEFORM_TIMER_FREQUENCY_RANGE_MIN)) +1);
  /* Timer reload calculation */
  timer_reload = (timer_clock_frequency / (timer_prescaler * WAVEFORM_TIMER_FREQUENCY));
  
  /* Enable the timer peripheral clock */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM6);
  
  /* Set timer pre-scaler value */
  LL_TIM_SetPrescaler(TIM6, (timer_prescaler - 1));
  
  /* Set timer auto-reload value */
  LL_TIM_SetAutoReload(TIM6, (timer_reload - 1));
  
  /* Set counter mode */
  LL_TIM_SetCounterMode(TIM6, LL_TIM_COUNTERMODE_UP); 
  
  /* Note: In this example, timer interruptions are not activated.            */
  /*       If needed, timer interruption at each time base period is          */
  /*       possible.                                                          */
  /*       Refer to timer examples.                                           */
  
  /* Set timer the trigger output (TRGO) */
  LL_TIM_SetTriggerOutput(TIM6, LL_TIM_TRGO_UPDATE);
  
  /*## Activation of timer ###################################################*/
  /* Enable counter */
  LL_TIM_EnableCounter(TIM6);
}

/**
  * @brief  Configure DAC (DAC instance: DAC1, DAC instance channel: channel1 )
  *         and GPIO used by DAC channel.
  * @note   This function configures the DAC channel but does not enable it,
  *         in order to optimize power consumption (DAC channel enabled only
  *         when needed).
  * @param  None
  * @retval None
  */
void Configure_DAC(void)
{
  LL_DAC_InitTypeDef DAC_InitStruct;
  
  /*## Configuration of GPIO used by DAC channels ############################*/
  
  /* Enable GPIO Clock */
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA); 
  
  /* Configure GPIO in analog mode to be used as DAC output */
  LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_4, LL_GPIO_MODE_ANALOG);
  
  /*## Configuration of NVIC #################################################*/
  /* Configure NVIC to enable DAC1 interruptions */
  NVIC_SetPriority(TIM6_DAC_IRQn, 0);
  NVIC_EnableIRQ(TIM6_DAC_IRQn);
  
  /*## Configuration of DAC ##################################################*/
  
  /* Enable DAC clock */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_DAC1);
  
  /* Update fields of initialization structure */
  /* Note: Parameter "WaveAutoGenerationConfig" is not set because discarded, */
  /*       since wave auto generation is disabled.                            */
  DAC_InitStruct.TriggerSource            = LL_DAC_TRIG_EXT_TIM6_TRGO;
  DAC_InitStruct.WaveAutoGeneration       = LL_DAC_WAVE_AUTO_GENERATION_NONE;
  DAC_InitStruct.OutputBuffer             = LL_DAC_OUTPUT_BUFFER_ENABLE;
  
  /* Initialize DAC instance according to parameters defined in               */
  /* initialization structure.                                                */
  LL_DAC_Init(DAC1, LL_DAC_CHANNEL_1, &DAC_InitStruct);
  
  /* Set the mode for the selected DAC channel */
  // LL_DAC_SetMode(DAC1, LL_DAC_CHANNEL_1, LL_DAC_MODE_NORMAL_OPERATION);
  
  /* Enable DAC channel DMA request */
  LL_DAC_EnableDMAReq(DAC1, LL_DAC_CHANNEL_1);
  
  /* Enable interruption DAC channel1 underrun */
  LL_DAC_EnableIT_DMAUDR1(DAC1);
}

/**
  * @brief  Perform DAC activation procedure to make it ready to generate
  *         a voltage (DAC instance: DAC1).
  * @note   Operations:
  *         - Enable DAC instance channel
  *         - Wait for DAC instance channel startup time
  * @param  None
  * @retval None
  */
void Activate_DAC(void)
{
  __IO uint32_t wait_loop_index = 0;
  
  /* Enable DAC channel */
  LL_DAC_Enable(DAC1, LL_DAC_CHANNEL_1);
  
  /* Delay for DAC channel voltage settling time from DAC channel startup.    */
  /* Compute number of CPU cycles to wait for, from delay in us.              */
  /* Note: Variable divided by 2 to compensate partially                      */
  /*       CPU processing cycles (depends on compilation optimization).       */
  /* Note: If system core clock frequency is below 200kHz, wait time          */
  /*       is only a few CPU processing cycles.                               */
  wait_loop_index = ((LL_DAC_DELAY_STARTUP_VOLTAGE_SETTLING_US * (SystemCoreClock / (100000 * 2))) / 10);
  while(wait_loop_index != 0)
  {
    wait_loop_index--;
  }
  
  /* Enable DAC channel trigger */
  /* Note: DAC channel conversion can start from trigger enable:              */
  /*       - if DAC channel trigger source is set to SW:                      */
  /*         DAC channel conversion will start after trig order               */
  /*         using function "LL_DAC_TrigSWConversion()".                      */
  /*       - if DAC channel trigger source is set to external trigger         */
  /*         (timer, ...):                                                    */
  /*         DAC channel conversion can start immediately                     */
  /*         (after next trig order from external trigger)                    */
  LL_DAC_EnableTrigger(DAC1, LL_DAC_CHANNEL_1);
}

/**
  * @brief  Initialize LED1.
  * @param  None
  * @retval None
  */
void LED_Init(void)
{
  /* Enable the LED1 Clock */
  LED1_GPIO_CLK_ENABLE();

  /* Configure IO in output push-pull mode to drive external LED1 */
  LL_GPIO_SetPinMode(LED1_GPIO_PORT, LED1_PIN, LL_GPIO_MODE_OUTPUT);
  /* Reset value is LL_GPIO_OUTPUT_PUSHPULL */
  //LL_GPIO_SetPinOutputType(LED1_GPIO_PORT, LED1_PIN, LL_GPIO_OUTPUT_PUSHPULL);
  /* Reset value is LL_GPIO_SPEED_FREQ_LOW */
  //LL_GPIO_SetPinSpeed(LED1_GPIO_PORT, LED1_PIN, LL_GPIO_SPEED_FREQ_LOW);
  /* Reset value is LL_GPIO_PULL_NO */
  //LL_GPIO_SetPinPull(LED1_GPIO_PORT, LED1_PIN, LL_GPIO_PULL_NO);
}

/**
  * @brief  Turn-on LED1.
  * @param  None
  * @retval None
  */
void LED_On(void)
{
  /* Turn LED1 on */
  LL_GPIO_SetOutputPin(LED1_GPIO_PORT, LED1_PIN);
}

/**
  * @brief  Turn-off LED1.
  * @param  None
  * @retval None
  */
void LED_Off(void)
{
  /* Turn LED1 off */
  LL_GPIO_ResetOutputPin(LED1_GPIO_PORT, LED1_PIN);
}

/**
  * @brief  Set LED1 to Blinking mode for an infinite loop (toggle period based on value provided as input parameter).
  * @param  Period : Period of time (in ms) between each toggling of LED
  *   This parameter can be user defined values. Pre-defined values used in that example are :
  *     @arg LED_BLINK_FAST : Fast Blinking
  *     @arg LED_BLINK_SLOW : Slow Blinking
  *     @arg LED_BLINK_ERROR : Error specific Blinking
  * @retval None
  */
void LED_Blinking(uint32_t Period)
{
  /* Turn LED1 on */
  LL_GPIO_SetOutputPin(LED1_GPIO_PORT, LED1_PIN);
  
  /* Toggle IO in an infinite loop */
  while (1)
  {
    LL_GPIO_TogglePin(LED1_GPIO_PORT, LED1_PIN);  
    LL_mDelay(Period);
  }
}

/**
  * @brief  Configures User push-button in EXTI Line Mode.
  * @param  None
  * @retval None
  */
void UserButton_Init(void)
{
  /* Enable the BUTTON Clock */
  USER_BUTTON_GPIO_CLK_ENABLE();
  
  /* Configure GPIO for BUTTON */
  LL_GPIO_SetPinMode(USER_BUTTON_GPIO_PORT, USER_BUTTON_PIN, LL_GPIO_MODE_INPUT);
  LL_GPIO_SetPinPull(USER_BUTTON_GPIO_PORT, USER_BUTTON_PIN, LL_GPIO_PULL_NO);
  
  /* Connect External Line to the GPIO */
  USER_BUTTON_SYSCFG_SET_EXTI();
  
  /* Enable a rising trigger EXTI line 13 Interrupt */
  USER_BUTTON_EXTI_LINE_ENABLE();
  USER_BUTTON_EXTI_FALLING_TRIG_ENABLE();
  
  /* Configure NVIC for USER_BUTTON_EXTI_IRQn */
  NVIC_EnableIRQ(USER_BUTTON_EXTI_IRQn); 
  NVIC_SetPriority(USER_BUTTON_EXTI_IRQn,0x03);  
  
}

/**
  * @brief  Wait for User push-button press to start transfer.
  * @param  None 
  * @retval None
  */
void WaitForUserButtonPress(void)
{
  while (ubButtonPress == 0)
  {
    LL_GPIO_TogglePin(LED1_GPIO_PORT, LED1_PIN);
    LL_mDelay(LED_BLINK_FAST);
  }
  ubButtonPress = 0;
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow :
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 180000000
  *            HCLK(Hz)                       = 180000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 2
  *            APB2 Prescaler                 = 1
  *            HSE Frequency(Hz)              = 8000000
  *            PLL_M                          = 8
  *            PLL_N                          = 360
  *            PLL_P                          = 2
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale1 mode
  *            Flash Latency(WS)              = 5
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
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_5);

  /* Enable PWR clock */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

  /* Activation OverDrive Mode */
  LL_PWR_EnableOverDriveMode();
  while(LL_PWR_IsActiveFlag_OD() != 1)
  {
  };

  /* Activation OverDrive Switching */
  LL_PWR_EnableOverDriveSwitching();
  while(LL_PWR_IsActiveFlag_ODSW() != 1)
  {
  };

  /* Main PLL configuration and activation */
  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_8, 360, LL_RCC_PLLP_DIV_2);
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
  SysTick_Config(180000000 / 1000);

  /* Update CMSIS variable (which can be updated also through SystemCoreClockUpdate function) */
  SystemCoreClock = 180000000;
}

/******************************************************************************/
/*   USER IRQ HANDLER TREATMENT                                               */
/******************************************************************************/

/**
  * @brief  Function to manage IRQ Handler
  * @param  None
  * @retval None
  */
void UserButton_Callback(void)
{
  /* On the first press on user button, update only user button variable      */
  /* to manage waiting function.                                              */
  if(ubButtonPress == 0)
  {
    /* Update User push-button variable : to be checked in waiting loop in main program */
    ubButtonPress = 1;
  }
}

/**
  * @brief  DMA transfer error callback
  * @note   This function is executed when the transfer error interrupt
  *         is generated during DMA transfer
  * @retval None
  */
void DacDmaTransferError_Callback()
{
  /* Error detected during DMA transfer */
  LED_Blinking(LED_BLINK_ERROR);
}

/**
  * @brief  DAC underrun interruption callback
  * @note   This function is executed when DAC channel underrun error occurs.
  * @retval None
  */
void DacUnderrunError_Callback(void)
{
  /* Note: Disable DAC interruption that caused this error before entering in */
  /*       infinite loop below.                                               */
  
  /* Disable interruption DAC channel1 underrun */
  LL_DAC_DisableIT_DMAUDR1(DAC1);
  
  /* Error from ADC */
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
