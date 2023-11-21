/**
  ******************************************************************************
  * @file    Examples_LL/ADC/ADC_GroupsRegularInjected/Src/main.c
  * @author  MCD Application Team
  * @brief   This example describes how to use a ADC peripheral with
  *          both ADC groups (ADC group regular and ADC group injected)
  *          in their intended use case:
  *           - group regular for a high number of conversions and continuous
  *             data stream, with DMA transfer capability
  *           - group injected for punctual conversions inserted between
  *             conversions of group regular
  *          This example is based on the STM32F4xx ADC LL API;
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

/** @addtogroup ADC_GroupsRegularInjected
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* Definitions of ADC hardware constraints delays */
/* Note: Only ADC IP HW delays are defined in ADC LL driver driver,           */
/*       not timeout values:                                                  */
/*       Timeout values for ADC operations are dependent to device clock      */
/*       configuration (system clock versus ADC clock),                       */
/*       and therefore must be defined in user application.                   */
/*       Refer to @ref ADC_LL_EC_HW_DELAYS for description of ADC timeout     */
/*       values definition.                                                   */

  /* Timeout values for ADC operations. */
  /* (enable settling time, disable settling time, ...)                       */
  /* Values defined to be higher than worst cases: low clock frequency,       */
  /* maximum prescalers.                                                      */
  /* Example of profile very low frequency : ADC clock frequency 36MHz        */
  /* prescaler 2, sampling time 56 ADC clock cycles, resolution 12 bits.      */
  /*  - ADC enable time: maximum delay is 3 us                                */
  /*    (refer to device datasheet, parameter "tSTAB")                        */
  /*  - ADC disable time: maximum delay should be a few ADC clock cycles      */
  /*  - ADC stop conversion time: maximum delay should be a few ADC clock     */
  /*    cycles                                                                */
  /*  - ADC conversion time: with this hypothesis of clock settings, maximum  */
  /*    delay will be 99us.                                                   */
  /*    (refer to device reference manual, section "Timing")                  */
  /* Unit: ms                                                                 */
  #define ADC_CALIBRATION_TIMEOUT_MS       ((uint32_t)   1)
  #define ADC_ENABLE_TIMEOUT_MS            ((uint32_t)   1)
  #define ADC_DISABLE_TIMEOUT_MS           ((uint32_t)   1)
  #define ADC_STOP_CONVERSION_TIMEOUT_MS   ((uint32_t)   1)
  #define ADC_CONVERSION_TIMEOUT_MS        ((uint32_t)   2)

/* Definitions of environment analog values */
  /* Value of analog reference voltage (Vref+), connected to analog voltage   */
  /* supply Vdda (unit: mV).                                                  */
  #define VDDA_APPLI                       ((uint32_t)3300)

/* Definitions of data related to this example */
  /* Definition of ADCx conversions data table size */
  #define ADC_CONVERTED_DATA_BUFFER_SIZE   ((uint32_t)  64)

  /* Init variable out of expected ADC conversion data range */
  #define VAR_CONVERTED_DATA_INIT_VALUE    (__LL_ADC_DIGITAL_SCALE(LL_ADC_RESOLUTION_12B) + 1)

  /* Parameters of timer (used as ADC conversion trigger) */
  /* Timer frequency (unit: Hz). With a timer 16 bits and time base           */
  /* freq min 1Hz, range is min=1Hz, max=32kHz.                               */
  #define TIMER_FREQUENCY                ((uint32_t) 1000)
  /* Timer minimum frequency (unit: Hz), used to calculate frequency range.   */
  /* With a timer 16 bits, maximum frequency will be 32000 times this value.  */
  #define TIMER_FREQUENCY_RANGE_MIN      ((uint32_t)    1)
  /* Timer prescaler maximum value (0xFFFF for a timer 16 bits) */
  #define TIMER_PRESCALER_MAX_VALUE      ((uint32_t)0xFFFF-1)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Variables for ADC conversion data */
__IO uint16_t aADCxConvertedData[ADC_CONVERTED_DATA_BUFFER_SIZE];             /* ADC group regular conversion data */
__IO uint16_t uhADCxGrpInjectedConvertedData = VAR_CONVERTED_DATA_INIT_VALUE; /* ADC group injected conversion data */

/* Variables for ADC conversion data computation to physical values */
__IO uint16_t uhADCxGrpInjectedConvertedData_Voltage_mVolt = 0;  /* Value of voltage calculated from ADC conversion data (unit: mV) */

/* Variable to report status of DMA transfer of ADC group regular conversions */
/*  0: DMA transfer is not completed                                          */
/*  1: DMA transfer is completed                                              */
/*  2: DMA transfer has not been started yet (initial state)                  */
__IO uint8_t ubDmaTransferStatus = 2; /* Variable set into DMA interruption callback */

/* Variable to report status of ADC group injected unitary conversion         */
/*  0: ADC group injected unitary conversion is not completed                 */
/*  1: ADC group injected unitary conversion is completed                     */
/*  2: ADC group injected unitary conversion has not been started yet         */
__IO uint8_t ubAdcGrpInjectedUnitaryConvStatus = 2; /* Variable set into ADC interruption callback */


/* Private function prototypes -----------------------------------------------*/
void     SystemClock_Config(void);
void     Configure_DMA(void);
void     Configure_TIM_TimeBase_ADC_trigger(void);
void     Configure_ADC(void);
void     Activate_ADC(void);
void     LED_Init(void);
void     LED_On(void);
void     LED_Off(void);
void     LED_Blinking(uint32_t Period);
void     UserButton_Init(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  /* Configure the system clock to 100 MHz */
  SystemClock_Config();
  
  /* Initialize LED2 */
  LED_Init();
  
  /* Initialize button in EXTI mode */
  UserButton_Init();
  
  /* Configure DMA for data transfer from ADC */
  Configure_DMA();
  
  /* Configure timer as a time base used to trig ADC conversion start */
  Configure_TIM_TimeBase_ADC_trigger();
  
  /* Configure ADC */
  /* Note: This function configures the ADC but does not enable it.           */
  /*       To enable it, use function "Activate_ADC()".                       */
  /*       This is intended to optimize power consumption:                    */
  /*       1. ADC configuration can be done once at the beginning             */
  /*          (ADC disabled, minimal power consumption)                       */
  /*       2. ADC enable (higher power consumption) can be done just before   */
  /*          ADC conversions needed.                                         */
  /*          Then, possible to perform successive "Activate_ADC()",          */
  /*          "Deactivate_ADC()", ..., without having to set again            */
  /*          ADC configuration.                                              */
  Configure_ADC();
  
  /* Activate ADC */
  /* Perform ADC activation procedure to make it ready to convert. */
  Activate_ADC();
  
  
  /* Start ADC group regular conversion */
  /* Note: Hardware constraint (refer to description of the functions         */
  /*       below):                                                            */
  /*       On this STM32 series, setting of these features are not             */
  /*       conditioned to ADC state.                                          */
  /*       However, in order to be compliant with other STM32 series          */
  /*       and to show the best practice usages, ADC state is checked.        */
  /*       Software can be optimized by removing some of these checks, if     */
  /*       they are not relevant considering previous settings and actions    */
  /*       in user application.                                               */
  if (LL_ADC_IsEnabled(ADC1) == 1)
  {
    LL_ADC_REG_StartConversionExtTrig(ADC1, LL_ADC_REG_TRIG_EXT_RISING);
  }
  else
  {
    /* Error: ADC conversion start could not be performed */
    LED_Blinking(LED_BLINK_ERROR);
  }
  
  
  /* Infinite loop */
  while (1)
  {
    /* Note: ADC group injected conversion start is done into push button     */
    /*       IRQ handler, refer to function "UserButton_Callback()".          */
    
    /* Note: LED state depending on ADC conversion status is set into ADC     */
    /*       IRQ handler, refer to function                                   */
    /*       "AdcGrpInjectedUnitaryConvComplete_Callback()".                  */
    
    /* Note: ADC group regular conversions data are stored into array         */
    /*       "aADCxConvertedData".                                            */
    /*       ADC group injected conversions data are stored into variable     */
    /*       "uhADCxGrpInjectedConvertedData".                                */
    /*       (for debug: see variable content into watch window).             */
    
    /* Note: ADC conversion data can be computed to physical values           */
    /*       using ADC LL driver helper macro:                                */
    /*         uhADCxConvertedData_Voltage_mVolt                              */
    /*         = __LL_ADC_CALC_DATA_TO_VOLTAGE(VDDA_APPLI,                    */
    /*                                      uhADCxConvertedData,              */
    /*                                      LL_ADC_RESOLUTION_12B)            */
  }
}

/**
  * @brief  This function configures DMA for transfer of data from ADC
  * @param  None
  * @retval None
  */
void Configure_DMA(void)
{
  /*## Configuration of NVIC #################################################*/
  /* Configure NVIC to enable DMA interruptions */
  NVIC_SetPriority(DMA2_Stream0_IRQn, 0);
  NVIC_EnableIRQ(DMA2_Stream0_IRQn);
  
  /*## Configuration of DMA ##################################################*/
  /* Enable the peripheral clock of DMA */
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA2);
  
  /* Configure the DMA transfer */
  /*  - DMA transfer in circular mode to match with ADC configuration:        */
  /*    DMA unlimited requests.                                               */
  /*  - DMA transfer from ADC without address increment.                      */
  /*  - DMA transfer to memory with address increment.                        */
  /*  - DMA transfer from ADC by half-word to match with ADC configuration:   */
  /*    ADC resolution 12 bits.                                               */
  /*  - DMA transfer to memory by half-word to match with ADC conversion data */
  /*    buffer variable type: half-word.                                      */
    LL_DMA_SetChannelSelection(DMA2, LL_DMA_STREAM_0, LL_DMA_CHANNEL_0);
  LL_DMA_ConfigTransfer(DMA2,
                        LL_DMA_STREAM_0,
                        LL_DMA_DIRECTION_PERIPH_TO_MEMORY |
                        LL_DMA_MODE_CIRCULAR              |
                        LL_DMA_PERIPH_NOINCREMENT         |
                        LL_DMA_MEMORY_INCREMENT           |
                        LL_DMA_PDATAALIGN_HALFWORD        |
                        LL_DMA_MDATAALIGN_HALFWORD        |
                        LL_DMA_PRIORITY_HIGH               );
  
  /* Set DMA transfer addresses of source and destination */
  LL_DMA_ConfigAddresses(DMA2,
                        LL_DMA_STREAM_0,
                         LL_ADC_DMA_GetRegAddr(ADC1, LL_ADC_DMA_REG_REGULAR_DATA),
                         (uint32_t)&aADCxConvertedData,
                         LL_DMA_DIRECTION_PERIPH_TO_MEMORY);
  
  /* Set DMA transfer size */
  LL_DMA_SetDataLength(DMA2,
                        LL_DMA_STREAM_0,
                       ADC_CONVERTED_DATA_BUFFER_SIZE);
  
  /* Enable DMA transfer interruption: transfer complete */
  LL_DMA_EnableIT_TC(DMA2,
                        LL_DMA_STREAM_0);

  
  /* Enable DMA transfer interruption: half transfer */
  LL_DMA_EnableIT_HT(DMA2,
                        LL_DMA_STREAM_0);
  
  /* Enable DMA transfer interruption: transfer error */
  LL_DMA_EnableIT_TE(DMA2,
                        LL_DMA_STREAM_0);
  
  /*## Activation of DMA #####################################################*/
  /* Enable the DMA transfer */
    LL_DMA_EnableStream(DMA2,LL_DMA_STREAM_0);
}

/**
  * @brief  Configure timer as a time base (timer instance: TIM2) 
  *         used to trig ADC conversion start.
  * @note   In this ADC example, timer instance must be on APB1 (clocked by PCLK1)
  *         to be compliant with frequency calculation used in this function.
  * @param  None
  * @retval None
  */
void Configure_TIM_TimeBase_ADC_trigger(void)
{
  uint32_t timer_clock_frequency = 0;             /* Timer clock frequency */
  uint32_t timer_prescaler = 0;                   /* Time base prescaler to have timebase aligned on minimum frequency possible */
  uint32_t timer_reload = 0;                      /* Timer reload value in function of timer prescaler to achieve time base period */
  
  /*## Configuration of NVIC #################################################*/
  /* Note: In this example, timer interrupts are not activated.               */
  /*       If needed, timer interruption at each time base period is          */
  /*       possible.                                                          */
  /*       Refer to timer examples.                                           */
  
  /* Note: In this example, timer interrupts are not activated */
  /*       If needed, timer interruption at each time base     */
  /*       period is possible.                                 */
  /*       Refer to timer examples.                            */
  
  /* Configuration of timer as time base:                                     */ 
  /* Caution: Computation of frequency is done for a timer instance on APB1   */
  /*          (clocked by PCLK1)                                              */
  /* Timer frequency is configured from the following constants:              */
  /* - TIMER_FREQUENCY: timer frequency (unit: Hz).                           */
  /* - TIMER_FREQUENCY_RANGE_MIN: timer minimum frequency possible            */
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
  timer_prescaler = ((timer_clock_frequency / (TIMER_PRESCALER_MAX_VALUE * TIMER_FREQUENCY_RANGE_MIN)) +1);
  /* Timer reload calculation */
  timer_reload = (timer_clock_frequency / (timer_prescaler * TIMER_FREQUENCY));
  
  /* Enable the timer peripheral clock */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM2);
  
  /* Set timer pre-scaler value */
  LL_TIM_SetPrescaler(TIM2, (timer_prescaler - 1));
  
  /* Set timer auto-reload value */
  LL_TIM_SetAutoReload(TIM2, (timer_reload - 1));
  
  /* Counter mode: select up-counting mode */
  LL_TIM_SetCounterMode(TIM2, LL_TIM_COUNTERMODE_UP); 
  
  /* Set the repetition counter */
  LL_TIM_SetRepetitionCounter(TIM2, 0);
  
  /* Note: In this example, timer interrupts are not activated.               */
  /*       If needed, timer interruption at each time base period is          */
  /*       possible.                                                          */
  /*       Refer to timer examples.                                           */
  
  /* Set timer the trigger output (TRGO) */
  LL_TIM_SetTriggerOutput(TIM2, LL_TIM_TRGO_UPDATE);
  
  /* Enable counter */
  LL_TIM_EnableCounter(TIM2);
}

/**
  * @brief  Configure ADC (ADC instance: ADC1) and GPIO used by ADC channels.
  * @note   In case re-use of this function outside of this example:
  *         This function includes checks of ADC hardware constraints before
  *         executing some configuration functions.
  *         - In this example, all these checks are not necessary but are
  *           implemented anyway to show the best practice usages
  *           corresponding to reference manual procedure.
  *           (On some STM32 series, setting of ADC features are not
  *           conditioned to ADC state. However, in order to be compliant with
  *           other STM32 series and to show the best practice usages,
  *           ADC state is checked anyway with same constraints).
  *           Software can be optimized by removing some of these checks,
  *           if they are not relevant considering previous settings and actions
  *           in user application.
  *         - If ADC is not in the appropriate state to modify some parameters,
  *           the setting of these parameters is bypassed without error
  *           reporting:
  *           it can be the expected behavior in case of recall of this 
  *           function to update only a few parameters (which update fulfills
  *           the ADC state).
  *           Otherwise, it is up to the user to set the appropriate error 
  *           reporting in user application.
  * @note   Peripheral configuration is minimal configuration from reset values.
  *         Thus, some useless LL unitary functions calls below are provided as
  *         commented examples - setting is default configuration from reset.
  * @param  None
  * @retval None
  */
void Configure_ADC(void)
{
  /*## Configuration of GPIO used by ADC channels ############################*/
  
  /* Note: On this STM32 device, ADC1 channel 4 is mapped on GPIO pin PA.04 */ 
  
  /* Enable GPIO Clock */
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
  
  /* Configure GPIO in analog mode to be used as ADC input */
  LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_4, LL_GPIO_MODE_ANALOG);
  
  /*## Configuration of NVIC #################################################*/
  /* Configure NVIC to enable ADC1 interruptions */
  NVIC_SetPriority(ADC_IRQn, 0);
  NVIC_EnableIRQ(ADC_IRQn);
  
  /*## Configuration of ADC ##################################################*/
  
  /*## Configuration of ADC hierarchical scope: common to several ADC ########*/
  
  /* Enable ADC clock (core clock) */
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_ADC1);
  
  /* Note: Hardware constraint (refer to description of the functions         */
  /*       below):                                                            */
  /*       On this STM32 series, setting of these features are not             */
  /*       conditioned to ADC state.                                          */
  /*       However, in order to be compliant with other STM32 series          */
  /*       and to show the best practice usages, ADC state is checked.        */
  /*       Software can be optimized by removing some of these checks, if     */
  /*       they are not relevant considering previous settings and actions    */
  /*       in user application.                                               */
  if(__LL_ADC_IS_ENABLED_ALL_COMMON_INSTANCE() == 0)
  {
    /* Note: Call of the functions below are commented because they are       */
    /*       useless in this example:                                         */
    /*       setting corresponding to default configuration from reset state. */
    
    /* Set ADC clock (conversion clock) common to several ADC instances */
    LL_ADC_SetCommonClock(__LL_ADC_COMMON_INSTANCE(ADC1), LL_ADC_CLOCK_SYNC_PCLK_DIV2);
    
    /* Set ADC measurement path to internal channels */
    LL_ADC_SetCommonPathInternalCh(__LL_ADC_COMMON_INSTANCE(ADC1), LL_ADC_PATH_INTERNAL_VREFINT);
    
    
  /*## Configuration of ADC hierarchical scope: multimode ####################*/
  
    /* Note: ADC multimode is not available on this device:                   */
    /*       only 1 ADC instance is present.                                  */
    /* Set ADC multimode configuration */
    // LL_ADC_SetMultimode(__LL_ADC_COMMON_INSTANCE(ADC1), LL_ADC_MULTI_INDEPENDENT);
    
    /* Set ADC multimode DMA transfer */
    // LL_ADC_SetMultiDMATransfer(__LL_ADC_COMMON_INSTANCE(ADC1), LL_ADC_MULTI_REG_DMA_EACH_ADC);
    
    /* Set ADC multimode: delay between 2 sampling phases */
    // LL_ADC_SetMultiTwoSamplingDelay(__LL_ADC_COMMON_INSTANCE(ADC1), LL_ADC_MULTI_TWOSMP_DELAY_1CYCLE);
    
  }
  
  
  /*## Configuration of ADC hierarchical scope: ADC instance #################*/
  
  /* Note: Hardware constraint (refer to description of the functions         */
  /*       below):                                                            */
  /*       On this STM32 series, setting of these features are not             */
  /*       conditioned to ADC state.                                          */
  /*       However, ADC state is checked anyway with standard requirements    */
  /*       (refer to description of this function).                           */
  if (LL_ADC_IsEnabled(ADC1) == 0)
  {
    /* Note: Call of the functions below are commented because they are       */
    /*       useless in this example:                                         */
    /*       setting corresponding to default configuration from reset state. */
    
    /* Set ADC data resolution */
    // LL_ADC_SetResolution(ADC1, LL_ADC_RESOLUTION_12B);
    
    /* Set ADC conversion data alignment */
    // LL_ADC_SetResolution(ADC1, LL_ADC_DATA_ALIGN_RIGHT);
    
    /* Set Set ADC sequencers scan mode, for all ADC groups                   */
    /* (group regular, group injected).                                       */
    // LL_ADC_SetSequencersScanMode(ADC1, LL_ADC_SEQ_SCAN_DISABLE);
    
  }
  
  
  /*## Configuration of ADC hierarchical scope: ADC group regular ############*/
  
  /* Note: Hardware constraint (refer to description of the functions         */
  /*       below):                                                            */
  /*       On this STM32 series, setting of these features are not             */
  /*       conditioned to ADC state.                                          */
  /*       However, ADC state is checked anyway with standard requirements    */
  /*       (refer to description of this function).                           */
  if (LL_ADC_IsEnabled(ADC1) == 0)
  {
    /* Set ADC group regular trigger source */
    LL_ADC_REG_SetTriggerSource(ADC1, LL_ADC_REG_TRIG_EXT_TIM2_TRGO);
    
    /* Set ADC group regular trigger polarity */
    // LL_ADC_REG_SetTriggerEdge(ADC1, LL_ADC_REG_TRIG_EXT_RISING);
    
    /* Set ADC group regular continuous mode */
    LL_ADC_REG_SetContinuousMode(ADC1, LL_ADC_REG_CONV_SINGLE);
    
    /* Set ADC group regular conversion data transfer */
    LL_ADC_REG_SetDMATransfer(ADC1, LL_ADC_REG_DMA_TRANSFER_UNLIMITED);
    
    /* Specify which ADC flag between EOC (end of unitary conversion)         */
    /* or EOS (end of sequence conversions) is used to indicate               */
    /* the end of conversion.                                                 */
    // LL_ADC_REG_SetFlagEndOfConversion(ADC1, LL_ADC_REG_FLAG_EOC_SEQUENCE_CONV);
    
    /* Set ADC group regular sequencer */
    /* Note: On this STM32 series, ADC group regular sequencer is              */
    /*       fully configurable: sequencer length and each rank               */
    /*       affectation to a channel are configurable.                       */
    /*       Refer to description of function                                 */
    /*       "LL_ADC_REG_SetSequencerLength()".                               */
    
    /* Set ADC group regular sequencer length and scan direction */
    LL_ADC_REG_SetSequencerLength(ADC1, LL_ADC_REG_SEQ_SCAN_DISABLE);
    
    /* Set ADC group regular sequencer discontinuous mode */
    // LL_ADC_REG_SetSequencerDiscont(ADC1, LL_ADC_REG_SEQ_DISCONT_DISABLE);
    
    /* Set ADC group regular sequence: channel on the selected sequence rank. */
    LL_ADC_REG_SetSequencerRanks(ADC1, LL_ADC_REG_RANK_1, LL_ADC_CHANNEL_4);
  }
  
  
  /*## Configuration of ADC hierarchical scope: ADC group injected ###########*/
  
  /* Note: Hardware constraint (refer to description of the functions         */
  /*       below):                                                            */
  /*       On this STM32 series, setting of these features are not             */
  /*       conditioned to ADC state.                                          */
  /*       However, ADC state is checked anyway with standard requirements    */
  /*       (refer to description of this function).                           */
  if (LL_ADC_IsEnabled(ADC1) == 0)
  {
    /* Note: Call of the functions below are commented because they are       */
    /*       useless in this example:                                         */
    /*       setting corresponding to default configuration from reset state. */
    
    /* Set ADC group injected trigger source */
    LL_ADC_INJ_SetTriggerSource(ADC1, LL_ADC_INJ_TRIG_SOFTWARE);
    
    /* Set ADC group injected trigger polarity */
    // LL_ADC_INJ_SetTriggerEdge(ADC1, LL_ADC_INJ_TRIG_EXT_RISING);
    
    /* Set ADC group injected conversion trigger  */
    // LL_ADC_INJ_SetTrigAuto(ADC1, LL_ADC_INJ_TRIG_INDEPENDENT);
    
    /* Set ADC group injected sequencer */
    /* Note: On this STM32 series, ADC group injected sequencer is             */
    /*       fully configurable: sequencer length and each rank               */
    /*       affectation to a channel are configurable.                       */
    /*       Refer to description of function                                 */
    /*       "LL_ADC_INJ_SetSequencerLength()".                               */
    
    /* Set ADC group injected sequencer length and scan direction */
    LL_ADC_INJ_SetSequencerLength(ADC1, LL_ADC_INJ_SEQ_SCAN_DISABLE);
    
    /* Set ADC group injected sequencer discontinuous mode */
    // LL_ADC_INJ_SetSequencerDiscont(ADC1, LL_ADC_INJ_SEQ_DISCONT_DISABLE);
    
    /* Set ADC group injected sequence: channel on the selected sequence rank. */
    LL_ADC_INJ_SetSequencerRanks(ADC1, LL_ADC_INJ_RANK_1, LL_ADC_CHANNEL_VREFINT);
  }
  
  
  /*## Configuration of ADC hierarchical scope: channels #####################*/
  
  /* Note: Hardware constraint (refer to description of the functions         */
  /*       below):                                                            */
  /*       On this STM32 series, setting of these features are not             */
  /*       conditioned to ADC state.                                          */
  /*       However, in order to be compliant with other STM32 series          */
  /*       and to show the best practice usages, ADC state is checked.        */
  /*       Software can be optimized by removing some of these checks, if     */
  /*       they are not relevant considering previous settings and actions    */
  /*       in user application.                                               */
  if (LL_ADC_IsEnabled(ADC1) == 0)
  {
    /* Set ADC channels sampling time */
    /* Note: Considering interruption occurring after each number of          */
    /*       "ADC_CONVERTED_DATA_BUFFER_SIZE" ADC conversions                 */
    /*       (IT from DMA transfer complete),                                 */
    /*       select sampling time and ADC clock with sufficient               */
    /*       duration to not create an overhead situation in IRQHandler.      */
    /* Note: Set long sampling time due to internal channels (VrefInt,        */
    /*       temperature sensor) constraints.                                 */
    /*       Refer to description of function                                 */
    /*       "LL_ADC_SetChannelSamplingTime()".                               */
    LL_ADC_SetChannelSamplingTime(ADC1, LL_ADC_CHANNEL_4, LL_ADC_SAMPLINGTIME_56CYCLES);
    LL_ADC_SetChannelSamplingTime(ADC1, LL_ADC_CHANNEL_VREFINT, LL_ADC_SAMPLINGTIME_480CYCLES);
    
  }
  
  
  /*## Configuration of ADC transversal scope: analog watchdog ###############*/
  
  /* Note: On this STM32 series, there is only 1 analog watchdog available.    */
  
  /* Set ADC analog watchdog: channels to be monitored */
  // LL_ADC_SetAnalogWDMonitChannels(ADC1, LL_ADC_AWD_DISABLE);
  
  /* Set ADC analog watchdog: thresholds */
  // LL_ADC_SetAnalogWDThresholds(ADC1, LL_ADC_AWD_THRESHOLD_HIGH, __LL_ADC_DIGITAL_SCALE(LL_ADC_RESOLUTION_12B));
  // LL_ADC_SetAnalogWDThresholds(ADC1, LL_ADC_AWD_THRESHOLD_LOW, 0x000);
  
  
  /*## Configuration of ADC transversal scope: oversampling ##################*/
  
  /* Note: Feature not available on this STM32 series */
  
  
  /*## Configuration of ADC interruptions ####################################*/
  /* Enable interruption ADC group regular overrun */
  LL_ADC_EnableIT_OVR(ADC1);

  /* Enable interruption ADC group injected end of sequence conversions       */
  /* Note: On this STM32 series, there is no flag of group injected            */
  /*       end of unitary conversion. Therefore, flag of group injected       */
  /*       end of sequence conversions is used (equivalent when there is      */
  /*       only 1 rank in group injected sequencer).                          */
  LL_ADC_EnableIT_JEOS(ADC1);
  
  /* Note: in this example, ADC group regular end of conversions              */
  /*       (number of ADC conversions defined by DMA buffer size)             */
  /*       are notified by DMA transfer interruptions).                       */
  
}

/**
  * @brief  Perform ADC activation procedure to make it ready to convert
  *         (ADC instance: ADC1).
  * @note   Operations:
  *         - ADC instance
  *           - Enable ADC
  *         - ADC group regular
  *           none: ADC conversion start-stop to be performed
  *                 after this function
  *         - ADC group injected
  *           none: ADC conversion start-stop to be performed
  *                 after this function
  * @param  None
  * @retval None
  */
void Activate_ADC(void)
{
  #if (USE_TIMEOUT == 1)
  uint32_t Timeout = 0; /* Variable used for timeout management */
  #endif /* USE_TIMEOUT */
  
  /*## Operation on ADC hierarchical scope: ADC instance #####################*/
  
  /* Note: Hardware constraint (refer to description of the functions         */
  /*       below):                                                            */
  /*       On this STM32 series, setting of these features are not             */
  /*       conditioned to ADC state.                                          */
  /*       However, in order to be compliant with other STM32 series          */
  /*       and to show the best practice usages, ADC state is checked.        */
  /*       Software can be optimized by removing some of these checks, if     */
  /*       they are not relevant considering previous settings and actions    */
  /*       in user application.                                               */
  if (LL_ADC_IsEnabled(ADC1) == 0)
  {
    /* Enable ADC */
    LL_ADC_Enable(ADC1);
    
  }
  
  /*## Operation on ADC hierarchical scope: ADC group regular ################*/
  /* Note: No operation on ADC group regular performed here.                  */
  /*       ADC group regular conversions to be performed after this function  */
  /*       using function:                                                    */
  /*       "LL_ADC_REG_StartConversion();"                                    */
  
  /*## Operation on ADC hierarchical scope: ADC group injected ###############*/
  /* Note: No operation on ADC group injected performed here.                 */
  /*       ADC group injected conversions to be performed after this function */
  /*       using function:                                                    */
  /*       "LL_ADC_INJ_StartConversion();"                                    */
  
}

/**
  * @brief  Initialize LED2.
  * @param  None
  * @retval None
  */
void LED_Init(void)
{
  /* Enable the LED2 Clock */
  LED2_GPIO_CLK_ENABLE();

  /* Configure IO in output push-pull mode to drive external LED2 */
  LL_GPIO_SetPinMode(LED2_GPIO_PORT, LED2_PIN, LL_GPIO_MODE_OUTPUT);
  /* Reset value is LL_GPIO_OUTPUT_PUSHPULL */
  //LL_GPIO_SetPinOutputType(LED2_GPIO_PORT, LED2_PIN, LL_GPIO_OUTPUT_PUSHPULL);
  /* Reset value is LL_GPIO_SPEED_FREQ_LOW */
  //LL_GPIO_SetPinSpeed(LED2_GPIO_PORT, LED2_PIN, LL_GPIO_SPEED_FREQ_LOW);
  /* Reset value is LL_GPIO_PULL_NO */
  //LL_GPIO_SetPinPull(LED2_GPIO_PORT, LED2_PIN, LL_GPIO_PULL_NO);
}

/**
  * @brief  Turn-on LED2.
  * @param  None
  * @retval None
  */
void LED_On(void)
{
  /* Turn LED2 on */
  LL_GPIO_SetOutputPin(LED2_GPIO_PORT, LED2_PIN);
}

/**
  * @brief  Turn-off LED2.
  * @param  None
  * @retval None
  */
void LED_Off(void)
{
  /* Turn LED2 off */
  LL_GPIO_ResetOutputPin(LED2_GPIO_PORT, LED2_PIN);
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
void LED_Blinking(uint32_t Period)
{
  /* Turn LED2 on */
  LL_GPIO_SetOutputPin(LED2_GPIO_PORT, LED2_PIN);
  
  /* Toggle IO in an infinite loop */
  while (1)
  {
    LL_GPIO_TogglePin(LED2_GPIO_PORT, LED2_PIN);  
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
  
  /* if(Button_Mode == BUTTON_MODE_EXTI) */
  {
    /* Connect External Line to the GPIO */
    USER_BUTTON_SYSCFG_SET_EXTI();
    
    /* Enable a rising trigger EXTI line 13 Interrupt */
    USER_BUTTON_EXTI_LINE_ENABLE();
    USER_BUTTON_EXTI_FALLING_TRIG_ENABLE();
    
    /* Configure NVIC for USER_BUTTON_EXTI_IRQn */
    NVIC_EnableIRQ(USER_BUTTON_EXTI_IRQn); 
    NVIC_SetPriority(USER_BUTTON_EXTI_IRQn,0x03);  
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
  * @brief  Function to manage IRQ Handler
  * @param  None
  * @retval None
  */
void UserButton_Callback(void)
{
  /* Turn LED off before performing a new ADC conversion start */
  LED_Off();
  
  /* Reset status variable of ADC group injected unitary conversion before    */
  /* performing a new ADC group injected conversion start.                     */
  /* Note: Optionally, for this example purpose, check ADC unitary            */
  /*       conversion status before starting another ADC conversion.          */

  if (ubAdcGrpInjectedUnitaryConvStatus != 0)
  {
    ubAdcGrpInjectedUnitaryConvStatus = 0;
  }
  else
  {
    /* Error: Previous action (ADC conversion or DMA transfer) not yet        */
    /* completed.                                                             */
    LED_Blinking(LED_BLINK_ERROR);
  }
  
  /* Init variable containing ADC conversion data */
  uhADCxGrpInjectedConvertedData = VAR_CONVERTED_DATA_INIT_VALUE;
  
  /* Start ADC group injected conversion */
  /* Note: Hardware constraint (refer to description of the functions         */
  /*       below):                                                            */
  /*       On this STM32 series, setting of these features are not             */
  /*       conditioned to ADC state.                                          */
  /*       However, in order to be compliant with other STM32 series          */
  /*       and to show the best practice usages, ADC state is checked.        */
  /*       Software can be optimized by removing some of these checks, if     */
  /*       they are not relevant considering previous settings and actions    */
  /*       in user application.                                               */
  if (LL_ADC_IsEnabled(ADC1) == 1)
  {
    LL_ADC_INJ_StartConversionSWStart(ADC1);
  }
  else
  {
    /* Error: ADC conversion start could not be performed */
    LED_Blinking(LED_BLINK_ERROR);
  }
}

/**
  * @brief  DMA transfer complete callback
  * @note   This function is executed when the transfer complete interrupt
  *         is generated
  * @retval None
  */
void AdcDmaTransferComplete_Callback()
{
  /* Update status variable of DMA transfer */
  ubDmaTransferStatus = 1;
}

/**
  * @brief  DMA half transfer callback
  * @note   This function is executed when the half transfer interrupt
  *         is generated
  * @retval None
  */
void AdcDmaTransferHalf_Callback()
{
  /* Update status variable of DMA transfer */
  ubDmaTransferStatus = 0;
}

/**
  * @brief  DMA transfer error callback
  * @note   This function is executed when the transfer error interrupt
  *         is generated during DMA transfer
  * @retval None
  */
void AdcDmaTransferError_Callback()
{
  if(ubDmaTransferStatus == 1)
  {
    /* Update status variable of DMA transfer */
    ubDmaTransferStatus = 0;
  }
  
  /* Error detected during DMA transfer */
  LED_Blinking(LED_BLINK_ERROR);
}

/**
  * @brief  ADC group regular overrun interruption callback
  * @note   This function is executed when ADC group regular
  *         overrun error occurs.
  * @retval None
  */
void AdcGrpRegularOverrunError_Callback(void)
{
  /* Note: Disable ADC interruption that caused this error before entering in */
  /*       infinite loop below.                                               */
  
  /* Disable ADC group regular overrun interruption */
  LL_ADC_DisableIT_OVR(ADC1);
  
  /* Error from ADC */
  LED_Blinking(LED_BLINK_ERROR);
}

/**
  * @brief  ADC group injected end of unitary conversion interruption callback
  * @note   This function is executed when the ADC group injected 
  *         sequencer has converted one ranks of the sequence.
  * @retval None
  */
void AdcGrpInjectedUnitaryConvComplete_Callback()
{
  /* Retrieve ADC conversion data */
  /* (data maximum amplitude corresponds to ADC resolution: 12 bits) */
  uhADCxGrpInjectedConvertedData = LL_ADC_INJ_ReadConversionData12(ADC1, LL_ADC_INJ_RANK_1);
  
  /* Computation of ADC conversions raw data to physical values               */
  /* using LL ADC driver helper macro.                                        */
  uhADCxGrpInjectedConvertedData_Voltage_mVolt = __LL_ADC_CALC_DATA_TO_VOLTAGE(VDDA_APPLI, uhADCxGrpInjectedConvertedData, LL_ADC_RESOLUTION_12B);
  
  /* Update status variable of ADC unitary conversion */
  ubAdcGrpInjectedUnitaryConvStatus = 1;
  
  /* Set LED depending on ADC unitary conversion status */
  /* - Turn-on if ADC group injected unitary conversion is completed */
  /* - Turn-off if ADC group injected unitary conversion is not completed */
  LED_On();

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
