/**
  ******************************************************************************
  * @file    DFSDM/DFSDM_PulseSkipper/Src/main.c
  * @author  MCD Application Team
  * @brief   This example describes how to use DFSDM HAL API to realize
  *          audio recording.
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

/** @addtogroup DFSDM_PulseSkipper
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#define SaturaLH(N, L, H) (((N)<(L))?(L):(((N)>(H))?(H):(N)))
/* Private variables ---------------------------------------------------------*/
DFSDM_Channel_HandleTypeDef  Dfsdm2Channel1Handle;
DFSDM_Channel_HandleTypeDef  Dfsdm2Channel0Handle;
DFSDM_Channel_HandleTypeDef  Dfsdm2Channel7Handle;
DFSDM_Channel_HandleTypeDef  Dfsdm2Channel6Handle;
DFSDM_Channel_HandleTypeDef  Dfsdm1Channel1Handle;
DFSDM_Filter_HandleTypeDef   Dfsdm2Filter1Handle;
DFSDM_Filter_HandleTypeDef   Dfsdm2Filter2Handle;
DFSDM_Filter_HandleTypeDef   Dfsdm2Filter3Handle;
DFSDM_Filter_HandleTypeDef   Dfsdm2Filter4Handle;
DFSDM_Filter_HandleTypeDef   Dfsdm1Filter1Handle;
I2S_HandleTypeDef            haudio_i2s;
AUDIO_DrvTypeDef             *audio_drv;
DFSDM_MultiChannelConfigTypeDef           MchdlyPulseSkipper;    
int32_t                      *LeftRecBuff;
int32_t                      *RightRecBuff;
int32_t                      Dfsdm2Filter1RecBuff[2048];
int32_t                      Dfsdm2Filter2RecBuff[2048];
int32_t                      Dfsdm2Filter3RecBuff[2048];
int32_t                      Dfsdm2Filter4RecBuff[2048];
int32_t                      Dfsdm1Filter1RecBuff[2048];
int16_t                      PlayBuff[4096];
uint32_t                     DmaLeftRecHalfBuffCplt  = 0;
uint32_t                     DmaLeftRecBuffCplt      = 0;
uint32_t                     DmaRightRecHalfBuffCplt = 0;
uint32_t                     DmaRightRecBuffCplt     = 0;
uint32_t                     PlaybackStarted         = 0;

TIM_OC_InitTypeDef           sOCConfig;
/* Master configuration structure */
TIM_MasterConfigTypeDef      sMasterConfig;
/* Slave configuration structure */
TIM_SlaveConfigTypeDef       sSlaveConfig;
#if defined(USE_CHANNEL_DELAY)  
__IO uint8_t ubGeneratingPulse = 0;
#endif
/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void DFSDM_Init(void);
static void Playback_Init(void);
#if defined(USE_CHANNEL_DELAY)   
static void PDM_Delay(void);
#endif
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  uint32_t i = 0;
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

  /* Configure the system clock to have a frequency of 100 MHz */
  SystemClock_Config();
      
  /* Initialize DFSDM channels and filter for record */
  DFSDM_Init();
#if defined(USE_CHANNEL_DELAY)
      /* Enable SYSCFG Clock */
  __HAL_RCC_SYSCFG_CLK_ENABLE();  
  /* Make sure DFSDM2 CKOUT, elected as clock source for the bitstream clock is blocked */
  Pulse_Skipper_Bitstream_Stop();
  
  /* MCHDLY initialization */
  MchdlyPulseSkipper.DFSDM1ClockOut = DFSDM1_CKOUT_DFSDM2_CKOUT;
  MchdlyPulseSkipper.DFSDM2ClockOut = DFSDM2_CKOUT_DFSDM2_CKOUT;
  MchdlyPulseSkipper.DFSDM1ClockIn = DFSDM1_CKIN_DFSDM2_CKOUT;
  MchdlyPulseSkipper.DFSDM2ClockIn = DFSDM2_CKIN_DFSDM2_CKOUT;
  MchdlyPulseSkipper.DFSDM1BitClkDistribution = DFSDM1_T4_OC1_BITSTREAM_CKIN1;
  MchdlyPulseSkipper.DFSDM2BitClkDistribution = DFSDM2_T3_OC4_BITSTREAM_CKIN0 | DFSDM2_T3_OC3_BITSTREAM_CKIN1 | DFSDM2_T3_OC2_BITSTREAM_CKIN6 | DFSDM2_T3_OC1_BITSTREAM_CKIN7; 
  MchdlyPulseSkipper.DFSDM2DataDistribution = DFSDM2_DATIN0_TO_DATIN1_PAD | DFSDM2_DATIN6_TO_DATIN7_PAD;
  MchdlyPulseSkipper.DFSDM1DataDistribution = DFSDM1_DATIN0_TO_DATIN0_PAD;
  HAL_DFSDM_ConfigMultiChannelDelay(&MchdlyPulseSkipper);
  
  Pulse_Skipper_Init();
#endif

  /* Initialize playback */
  Playback_Init();
  
  /* Start DFSDM conversions */
  /* Make sure to start DFSDM2 Filter 0 conversion the last one after the others Filters 
  ** Then all DFSDM2 filters will be synchronyzed (Filter 1, 2 and 3 are programmed to be
  ** synchronized with Filter 0)
  ** As DFSDM1 and DFSDM2 filters cannot be synchronyzed, we need to start audio DFSDM clocks
  ** once DFSDM conversions started
  */
  if(HAL_OK != HAL_DFSDM_FilterRegularStart_DMA(&Dfsdm1Filter1Handle, Dfsdm1Filter1RecBuff, 2048))
  {
    Error_Handler();
  }
  if(HAL_OK != HAL_DFSDM_FilterRegularStart_DMA(&Dfsdm2Filter2Handle, Dfsdm2Filter2RecBuff, 2048))
  {
    Error_Handler();
  }
  if(HAL_OK != HAL_DFSDM_FilterRegularStart_DMA(&Dfsdm2Filter3Handle, Dfsdm2Filter3RecBuff, 2048))
  {
    Error_Handler();
  }
  if(HAL_OK != HAL_DFSDM_FilterRegularStart_DMA(&Dfsdm2Filter4Handle, Dfsdm2Filter4RecBuff, 2048))
  {
    Error_Handler();
  }
  if(HAL_OK != HAL_DFSDM_FilterRegularStart_DMA(&Dfsdm2Filter1Handle, Dfsdm2Filter1RecBuff, 2048))
  {
    Error_Handler();
  }
#if defined(USE_CHANNEL_DELAY)
  /* DFSDM2 CKOUT is elected as clock source for the bitstream clock and start clocking */
  Pulse_Skipper_Bitstream_Start();
#endif
  
#if defined(PLAY_DFSDM2_DATIN01)
  LeftRecBuff  = Dfsdm2Filter1RecBuff;
  RightRecBuff = Dfsdm2Filter2RecBuff;
#endif  
#if defined(PLAY_DFSDM2_DATIN06)
  LeftRecBuff  = Dfsdm2Filter2RecBuff;
  RightRecBuff = Dfsdm2Filter4RecBuff;
#endif  
#if defined(PLAY_DFSDM2_DATIN07)
  LeftRecBuff  = Dfsdm2Filter2RecBuff;
  RightRecBuff = Dfsdm2Filter3RecBuff;
#endif  
#if defined(PLAY_DFSDM2_DATIN16)
  LeftRecBuff  = Dfsdm2Filter1RecBuff;
  RightRecBuff = Dfsdm2Filter4RecBuff;
#endif  
#if defined(PLAY_DFSDM2_DATIN17)
  LeftRecBuff  = Dfsdm2Filter1RecBuff;
  RightRecBuff = Dfsdm2Filter3RecBuff;
#endif  
#if defined(PLAY_DFSDM2_DATIN67)
  LeftRecBuff  = Dfsdm2Filter3RecBuff;
  RightRecBuff = Dfsdm2Filter4RecBuff;
#endif  
#if defined(PLAY_DFSDM12_DATIN10)
  LeftRecBuff  = Dfsdm1Filter1RecBuff;
  RightRecBuff = Dfsdm2Filter2RecBuff;
#endif  
#if defined(PLAY_DFSDM12_DATIN11)
  LeftRecBuff  = Dfsdm1Filter1RecBuff;
  RightRecBuff = Dfsdm2Filter1RecBuff;
#endif  
#if defined(PLAY_DFSDM12_DATIN16)
  LeftRecBuff  = Dfsdm1Filter1RecBuff;
  RightRecBuff = Dfsdm2Filter4RecBuff;
#endif  
#if defined(PLAY_DFSDM12_DATIN17)
  LeftRecBuff  = Dfsdm1Filter1RecBuff;
  RightRecBuff = Dfsdm2Filter3RecBuff;
#endif  
    
  /* Start loopback */
  while(1)
  {    
    if((DmaLeftRecHalfBuffCplt == 1) && (DmaRightRecHalfBuffCplt == 1))
    {
      /* Store values on Play buff */
      for(i = 0; i < 1024; i++)
      {
        PlayBuff[2*i]     = SaturaLH((LeftRecBuff[i] >> 8), -32768, 32767);
        PlayBuff[(2*i)+1] = SaturaLH((RightRecBuff[i] >> 8), -32768, 32767);
      }
      if(PlaybackStarted == 0)
      {
        if(0 != audio_drv->Play(AUDIO_I2C_ADDRESS, (uint16_t *) &PlayBuff[0], 4096))
        {
          Error_Handler();
        }
        if(HAL_OK != HAL_I2S_Transmit_DMA(&haudio_i2s, (uint16_t *) &PlayBuff[0], 4096))
        {
          Error_Handler();
        }
        PlaybackStarted = 1;
      }
      DmaLeftRecHalfBuffCplt  = 0;
      DmaRightRecHalfBuffCplt = 0;
    }
    if((DmaLeftRecBuffCplt == 1) && (DmaRightRecBuffCplt == 1))
    {
      /* Store values on Play buff */
      for(i = 1024; i < 2048; i++)
      {
        PlayBuff[2*i]     = SaturaLH((LeftRecBuff[i] >> 8), -32768, 32767);
        PlayBuff[(2*i)+1] = SaturaLH((RightRecBuff[i] >> 8), -32768, 32767);
      }
      DmaLeftRecBuffCplt  = 0;
      DmaRightRecBuffCplt = 0;

#if defined(USE_CHANNEL_DELAY) 
      
      if (ubGeneratingPulse == 0)
      {
        ubGeneratingPulse = 1;
        PDM_Delay();
      }
      
#endif      
  
    }
  }
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follows :
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 100000000
  *            HCLK(Hz)                       = 100000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 2
  *            APB2 Prescaler                 = 1
  *            MSI Frequency(Hz)              = 8000000
  *            PLL_M                          = 8
  *            PLL_N                          = 200
  *            PLL_R                          = 2
  *            PLL_P                          = 2
  *            PLL_Q                          = 7
  *            Flash Latency(WS)              = 3
  * @param  None
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_PeriphCLKInitTypeDef RCC_PeriphCLKInitStruct;

  HAL_StatusTypeDef ret = HAL_OK;

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
  RCC_OscInitStruct.PLL.PLLN = 200;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  RCC_OscInitStruct.PLL.PLLR = 2;
  ret = HAL_RCC_OscConfig(&RCC_OscInitStruct);

  if(ret != HAL_OK)
  {
    Error_Handler();
  }

  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  ret = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3);
  if(ret != HAL_OK)
  {
    Error_Handler();
  }

  /* Configure the FMPI2C clock source */
  HAL_RCCEx_GetPeriphCLKConfig(&RCC_PeriphCLKInitStruct); 
  RCC_PeriphCLKInitStruct.PeriphClockSelection = RCC_PERIPHCLK_FMPI2C1;
  RCC_PeriphCLKInitStruct.Fmpi2c1ClockSelection = RCC_FMPI2C1CLKSOURCE_SYSCLK;
  HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphCLKInitStruct);
  
}

/**
  * @brief  DFSDM channels and filter initialization
  * @param  None
  * @retval None
  */
static void DFSDM_Init(void)
{  
  /* Initialize DFSDM1 channel 1 */
  __HAL_DFSDM_CHANNEL_RESET_HANDLE_STATE(&Dfsdm1Channel1Handle);
  Dfsdm1Channel1Handle.Instance                      = DFSDM1_DATIN1_INSTANCE;
  Dfsdm1Channel1Handle.Init.OutputClock.Activation   = ENABLE;
  Dfsdm1Channel1Handle.Init.OutputClock.Selection    = DFSDM_CHANNEL_OUTPUT_CLOCK_AUDIO;
  Dfsdm1Channel1Handle.Init.OutputClock.Divider      = 24;
  Dfsdm1Channel1Handle.Init.Input.Multiplexer        = DFSDM_CHANNEL_EXTERNAL_INPUTS;
  Dfsdm1Channel1Handle.Init.Input.DataPacking        = DFSDM_CHANNEL_STANDARD_MODE;
  Dfsdm1Channel1Handle.Init.Input.Pins               = DFSDM_CHANNEL_SAME_CHANNEL_PINS;
  Dfsdm1Channel1Handle.Init.SerialInterface.Type     = DFSDM_CHANNEL_SPI_RISING;
#if defined(USE_CHANNEL_DELAY)
  Dfsdm1Channel1Handle.Init.SerialInterface.SpiClock = DFSDM_CHANNEL_SPI_CLOCK_EXTERNAL;
#else  
  Dfsdm1Channel1Handle.Init.SerialInterface.SpiClock = DFSDM_CHANNEL_SPI_CLOCK_INTERNAL;
#endif  
  Dfsdm1Channel1Handle.Init.Awd.FilterOrder          = DFSDM_CHANNEL_SINC1_ORDER;
  Dfsdm1Channel1Handle.Init.Awd.Oversampling         = 10;
  Dfsdm1Channel1Handle.Init.Offset                   = 0;
  Dfsdm1Channel1Handle.Init.RightBitShift            = 2;
  if(HAL_OK != HAL_DFSDM_ChannelInit(&Dfsdm1Channel1Handle))
  {
    Error_Handler();
  }
  
  /* Initialize DFSDM2 channel 1 */  
  __HAL_DFSDM_CHANNEL_RESET_HANDLE_STATE(&Dfsdm2Channel1Handle);
  Dfsdm2Channel1Handle.Instance                      = DFSDM2_DATIN1_INSTANCE;
  Dfsdm2Channel1Handle.Init.OutputClock.Activation   = ENABLE;
  Dfsdm2Channel1Handle.Init.OutputClock.Selection    = DFSDM_CHANNEL_OUTPUT_CLOCK_AUDIO;
  Dfsdm2Channel1Handle.Init.OutputClock.Divider      = 24;
  Dfsdm2Channel1Handle.Init.Input.Multiplexer        = DFSDM_CHANNEL_EXTERNAL_INPUTS;
  Dfsdm2Channel1Handle.Init.Input.DataPacking        = DFSDM_CHANNEL_STANDARD_MODE;
  Dfsdm2Channel1Handle.Init.Input.Pins               = DFSDM_CHANNEL_SAME_CHANNEL_PINS;
  Dfsdm2Channel1Handle.Init.SerialInterface.Type     = DFSDM_CHANNEL_SPI_RISING;
#if defined(USE_CHANNEL_DELAY)
  Dfsdm2Channel1Handle.Init.SerialInterface.SpiClock = DFSDM_CHANNEL_SPI_CLOCK_EXTERNAL;
#else  
  Dfsdm2Channel1Handle.Init.SerialInterface.SpiClock = DFSDM_CHANNEL_SPI_CLOCK_INTERNAL;
#endif  
  Dfsdm2Channel1Handle.Init.Awd.FilterOrder          = DFSDM_CHANNEL_SINC1_ORDER;
  Dfsdm2Channel1Handle.Init.Awd.Oversampling         = 10;
  Dfsdm2Channel1Handle.Init.Offset                   = 0;
  Dfsdm2Channel1Handle.Init.RightBitShift            = 2;
  if(HAL_OK != HAL_DFSDM_ChannelInit(&Dfsdm2Channel1Handle))
  {
    Error_Handler();
  }

  /* Initialize DFSDM2 channel 0 */  
  __HAL_DFSDM_CHANNEL_RESET_HANDLE_STATE(&Dfsdm2Channel0Handle);
  Dfsdm2Channel0Handle.Instance                      = DFSDM2_DATIN0_INSTANCE;
  Dfsdm2Channel0Handle.Init.OutputClock.Activation   = ENABLE;
  Dfsdm2Channel0Handle.Init.OutputClock.Selection    = DFSDM_CHANNEL_OUTPUT_CLOCK_AUDIO;
  Dfsdm2Channel0Handle.Init.OutputClock.Divider      = 24;
  Dfsdm2Channel0Handle.Init.Input.Multiplexer        = DFSDM_CHANNEL_EXTERNAL_INPUTS;
  Dfsdm2Channel0Handle.Init.Input.DataPacking        = DFSDM_CHANNEL_STANDARD_MODE;
#if defined(USE_CHANNEL_DELAY)  
  Dfsdm2Channel0Handle.Init.Input.Pins               = DFSDM_CHANNEL_SAME_CHANNEL_PINS;
#else  
  Dfsdm2Channel0Handle.Init.Input.Pins               = DFSDM_CHANNEL_FOLLOWING_CHANNEL_PINS;
#endif  

  Dfsdm2Channel0Handle.Init.SerialInterface.Type     = DFSDM_CHANNEL_SPI_FALLING;

 
#if defined(USE_CHANNEL_DELAY)
  Dfsdm2Channel0Handle.Init.SerialInterface.SpiClock = DFSDM_CHANNEL_SPI_CLOCK_EXTERNAL;
#else  
  Dfsdm2Channel0Handle.Init.SerialInterface.SpiClock = DFSDM_CHANNEL_SPI_CLOCK_INTERNAL;
#endif  
  Dfsdm2Channel0Handle.Init.Awd.FilterOrder          = DFSDM_CHANNEL_SINC1_ORDER;
  Dfsdm2Channel0Handle.Init.Awd.Oversampling         = 10;
  Dfsdm2Channel0Handle.Init.Offset                   = 0;
  Dfsdm2Channel0Handle.Init.RightBitShift            = 2;
  if(HAL_OK != HAL_DFSDM_ChannelInit(&Dfsdm2Channel0Handle))
  {
    Error_Handler();
  }
  
  /* Initialize DFSDM2 channel 7 */
  __HAL_DFSDM_CHANNEL_RESET_HANDLE_STATE(&Dfsdm2Channel7Handle);
  Dfsdm2Channel7Handle.Instance                      = DFSDM2_DATIN7_INSTANCE;
  Dfsdm2Channel7Handle.Init.OutputClock.Activation   = ENABLE;
  Dfsdm2Channel7Handle.Init.OutputClock.Selection    = DFSDM_CHANNEL_OUTPUT_CLOCK_AUDIO;
  Dfsdm2Channel7Handle.Init.OutputClock.Divider      = 24;
  Dfsdm2Channel7Handle.Init.Input.Multiplexer        = DFSDM_CHANNEL_EXTERNAL_INPUTS;
  Dfsdm2Channel7Handle.Init.Input.DataPacking        = DFSDM_CHANNEL_STANDARD_MODE;
  Dfsdm2Channel7Handle.Init.Input.Pins               = DFSDM_CHANNEL_SAME_CHANNEL_PINS;

  Dfsdm2Channel7Handle.Init.SerialInterface.Type     = DFSDM_CHANNEL_SPI_FALLING;
#if defined(USE_CHANNEL_DELAY)
  Dfsdm2Channel7Handle.Init.SerialInterface.SpiClock = DFSDM_CHANNEL_SPI_CLOCK_EXTERNAL;
#else  
  Dfsdm2Channel7Handle.Init.SerialInterface.SpiClock = DFSDM_CHANNEL_SPI_CLOCK_INTERNAL;
#endif  
  Dfsdm2Channel7Handle.Init.Awd.FilterOrder          = DFSDM_CHANNEL_SINC1_ORDER;
  Dfsdm2Channel7Handle.Init.Awd.Oversampling         = 10;
  Dfsdm2Channel7Handle.Init.Offset                   = 0;
  Dfsdm2Channel7Handle.Init.RightBitShift            = 2;
  if(HAL_OK != HAL_DFSDM_ChannelInit(&Dfsdm2Channel7Handle))
  {
    Error_Handler();
  }
  
  /* Initialize DFSDM2 channel 6 */
  __HAL_DFSDM_CHANNEL_RESET_HANDLE_STATE(&Dfsdm2Channel6Handle);
  Dfsdm2Channel6Handle.Instance                      = DFSDM2_DATIN6_INSTANCE;
  Dfsdm2Channel6Handle.Init.OutputClock.Activation   = ENABLE;
  Dfsdm2Channel6Handle.Init.OutputClock.Selection    = DFSDM_CHANNEL_OUTPUT_CLOCK_AUDIO;
  Dfsdm2Channel6Handle.Init.OutputClock.Divider      = 24;
  Dfsdm2Channel6Handle.Init.Input.Multiplexer        = DFSDM_CHANNEL_EXTERNAL_INPUTS;
  Dfsdm2Channel6Handle.Init.Input.DataPacking        = DFSDM_CHANNEL_STANDARD_MODE;
#if defined(USE_CHANNEL_DELAY)  
  Dfsdm2Channel6Handle.Init.Input.Pins               = DFSDM_CHANNEL_SAME_CHANNEL_PINS;
#else  
  Dfsdm2Channel6Handle.Init.Input.Pins               = DFSDM_CHANNEL_FOLLOWING_CHANNEL_PINS;
#endif  

  Dfsdm2Channel0Handle.Init.SerialInterface.Type     = DFSDM_CHANNEL_SPI_RISING;
#if defined(USE_CHANNEL_DELAY)
  Dfsdm2Channel6Handle.Init.SerialInterface.SpiClock = DFSDM_CHANNEL_SPI_CLOCK_EXTERNAL;
#else  
  Dfsdm2Channel6Handle.Init.SerialInterface.SpiClock = DFSDM_CHANNEL_SPI_CLOCK_INTERNAL;
#endif  
  Dfsdm2Channel6Handle.Init.Awd.FilterOrder          = DFSDM_CHANNEL_SINC1_ORDER;
  Dfsdm2Channel6Handle.Init.Awd.Oversampling         = 10;
  Dfsdm2Channel6Handle.Init.Offset                   = 0;
  Dfsdm2Channel6Handle.Init.RightBitShift            = 2;
  if(HAL_OK != HAL_DFSDM_ChannelInit(&Dfsdm2Channel6Handle))
  {
    Error_Handler();
  }
  
  /* Initialize DFSDM1 filter 0 */
  __HAL_DFSDM_FILTER_RESET_HANDLE_STATE(&Dfsdm1Filter1Handle);
  Dfsdm1Filter1Handle.Instance                          = DFSDM1_FILTER0;
  Dfsdm1Filter1Handle.Init.RegularParam.Trigger         = DFSDM_FILTER_SW_TRIGGER;
  Dfsdm1Filter1Handle.Init.RegularParam.FastMode        = ENABLE;
  Dfsdm1Filter1Handle.Init.RegularParam.DmaMode         = ENABLE;
  Dfsdm1Filter1Handle.Init.InjectedParam.Trigger        = DFSDM_FILTER_SW_TRIGGER;
  Dfsdm1Filter1Handle.Init.InjectedParam.ScanMode       = DISABLE;
  Dfsdm1Filter1Handle.Init.InjectedParam.DmaMode        = DISABLE;
  Dfsdm1Filter1Handle.Init.InjectedParam.ExtTrigger     = DFSDM_FILTER_EXT_TRIG_TIM8_TRGO;
  Dfsdm1Filter1Handle.Init.InjectedParam.ExtTriggerEdge = DFSDM_FILTER_EXT_TRIG_BOTH_EDGES;
  Dfsdm1Filter1Handle.Init.FilterParam.SincOrder        = DFSDM_FILTER_SINC3_ORDER;
  Dfsdm1Filter1Handle.Init.FilterParam.Oversampling     = 128;
  Dfsdm1Filter1Handle.Init.FilterParam.IntOversampling  = 1;
  if(HAL_OK != HAL_DFSDM_FilterInit(&Dfsdm1Filter1Handle))
  {
    Error_Handler();
  }
  
  /* Initialize DFSDM2 filter 1 */
  __HAL_DFSDM_FILTER_RESET_HANDLE_STATE(&Dfsdm2Filter1Handle);
  Dfsdm2Filter1Handle.Instance                          = DFSDM2_FILTER0;
  Dfsdm2Filter1Handle.Init.RegularParam.Trigger         = DFSDM_FILTER_SW_TRIGGER;
  Dfsdm2Filter1Handle.Init.RegularParam.FastMode        = ENABLE;
  Dfsdm2Filter1Handle.Init.RegularParam.DmaMode         = ENABLE;
  Dfsdm2Filter1Handle.Init.InjectedParam.Trigger        = DFSDM_FILTER_SW_TRIGGER;
  Dfsdm2Filter1Handle.Init.InjectedParam.ScanMode       = DISABLE;
  Dfsdm2Filter1Handle.Init.InjectedParam.DmaMode        = DISABLE;
  Dfsdm2Filter1Handle.Init.InjectedParam.ExtTrigger     = DFSDM_FILTER_EXT_TRIG_TIM8_TRGO;
  Dfsdm2Filter1Handle.Init.InjectedParam.ExtTriggerEdge = DFSDM_FILTER_EXT_TRIG_BOTH_EDGES;
  Dfsdm2Filter1Handle.Init.FilterParam.SincOrder        = DFSDM_FILTER_SINC3_ORDER;
  Dfsdm2Filter1Handle.Init.FilterParam.Oversampling     = 128;
  Dfsdm2Filter1Handle.Init.FilterParam.IntOversampling  = 1;
  if(HAL_OK != HAL_DFSDM_FilterInit(&Dfsdm2Filter1Handle))
  {
    Error_Handler();
  }
  
  /* Initialize DFSDM2 filter 2 */
  __HAL_DFSDM_FILTER_RESET_HANDLE_STATE(&Dfsdm2Filter2Handle);
  Dfsdm2Filter2Handle.Instance                          = DFSDM2_FILTER1;
  Dfsdm2Filter2Handle.Init.RegularParam.Trigger         = DFSDM_FILTER_SYNC_TRIGGER;
  Dfsdm2Filter2Handle.Init.RegularParam.FastMode        = ENABLE;
  Dfsdm2Filter2Handle.Init.RegularParam.DmaMode         = ENABLE;
  Dfsdm2Filter2Handle.Init.InjectedParam.Trigger        = DFSDM_FILTER_SW_TRIGGER;
  Dfsdm2Filter2Handle.Init.InjectedParam.ScanMode       = DISABLE;
  Dfsdm2Filter2Handle.Init.InjectedParam.DmaMode        = DISABLE;
  Dfsdm2Filter2Handle.Init.InjectedParam.ExtTrigger     = DFSDM_FILTER_EXT_TRIG_TIM8_TRGO;
  Dfsdm2Filter2Handle.Init.InjectedParam.ExtTriggerEdge = DFSDM_FILTER_EXT_TRIG_BOTH_EDGES;
  Dfsdm2Filter2Handle.Init.FilterParam.SincOrder        = DFSDM_FILTER_SINC3_ORDER;
  Dfsdm2Filter2Handle.Init.FilterParam.Oversampling     = 128;
  Dfsdm2Filter2Handle.Init.FilterParam.IntOversampling  = 1;
  if(HAL_OK != HAL_DFSDM_FilterInit(&Dfsdm2Filter2Handle))
  {
    Error_Handler();
  }
  
  /* Initialize DFSDM2 filter 3 */
  __HAL_DFSDM_FILTER_RESET_HANDLE_STATE(&Dfsdm2Filter3Handle);
  Dfsdm2Filter3Handle.Instance                          = DFSDM2_FILTER2;
  Dfsdm2Filter3Handle.Init.RegularParam.Trigger         = DFSDM_FILTER_SYNC_TRIGGER;
  Dfsdm2Filter3Handle.Init.RegularParam.FastMode        = ENABLE;
  Dfsdm2Filter3Handle.Init.RegularParam.DmaMode         = ENABLE;
  Dfsdm2Filter3Handle.Init.InjectedParam.Trigger        = DFSDM_FILTER_SW_TRIGGER;
  Dfsdm2Filter3Handle.Init.InjectedParam.ScanMode       = DISABLE;
  Dfsdm2Filter3Handle.Init.InjectedParam.DmaMode        = DISABLE;
  Dfsdm2Filter3Handle.Init.InjectedParam.ExtTrigger     = DFSDM_FILTER_EXT_TRIG_TIM8_TRGO;
  Dfsdm2Filter3Handle.Init.InjectedParam.ExtTriggerEdge = DFSDM_FILTER_EXT_TRIG_BOTH_EDGES;
  Dfsdm2Filter3Handle.Init.FilterParam.SincOrder        = DFSDM_FILTER_SINC3_ORDER;
  Dfsdm2Filter3Handle.Init.FilterParam.Oversampling     = 128;
  Dfsdm2Filter3Handle.Init.FilterParam.IntOversampling  = 1;
  if(HAL_OK != HAL_DFSDM_FilterInit(&Dfsdm2Filter3Handle))
  {
    Error_Handler();
  }
  
  /* Initialize DFSDM2 filter 4 */
  __HAL_DFSDM_FILTER_RESET_HANDLE_STATE(&Dfsdm2Filter4Handle);
  Dfsdm2Filter4Handle.Instance                          = DFSDM2_FILTER3;
  Dfsdm2Filter4Handle.Init.RegularParam.Trigger         = DFSDM_FILTER_SYNC_TRIGGER;
  Dfsdm2Filter4Handle.Init.RegularParam.FastMode        = ENABLE;
  Dfsdm2Filter4Handle.Init.RegularParam.DmaMode         = ENABLE;
  Dfsdm2Filter4Handle.Init.InjectedParam.Trigger        = DFSDM_FILTER_SW_TRIGGER;
  Dfsdm2Filter4Handle.Init.InjectedParam.ScanMode       = DISABLE;
  Dfsdm2Filter4Handle.Init.InjectedParam.DmaMode        = DISABLE;
  Dfsdm2Filter4Handle.Init.InjectedParam.ExtTrigger     = DFSDM_FILTER_EXT_TRIG_TIM8_TRGO;
  Dfsdm2Filter4Handle.Init.InjectedParam.ExtTriggerEdge = DFSDM_FILTER_EXT_TRIG_BOTH_EDGES;
  Dfsdm2Filter4Handle.Init.FilterParam.SincOrder        = DFSDM_FILTER_SINC3_ORDER;
  Dfsdm2Filter4Handle.Init.FilterParam.Oversampling     = 128;
  Dfsdm2Filter4Handle.Init.FilterParam.IntOversampling  = 1;
  if(HAL_OK != HAL_DFSDM_FilterInit(&Dfsdm2Filter4Handle))
  {
    Error_Handler();
  }

  /* Configure Regular channel */
  if(HAL_OK != HAL_DFSDM_FilterConfigRegChannel(&Dfsdm1Filter1Handle, DFSDM_DATIN1_CHANNEL, DFSDM_CONTINUOUS_CONV_ON))
  {
    Error_Handler();
  } 
  /* Configure Regular channel */
  if(HAL_OK != HAL_DFSDM_FilterConfigRegChannel(&Dfsdm2Filter1Handle, DFSDM_DATIN1_CHANNEL, DFSDM_CONTINUOUS_CONV_ON))
  {
    Error_Handler();
  }
  /* Configure Regular channel */
  if(HAL_OK != HAL_DFSDM_FilterConfigRegChannel(&Dfsdm2Filter2Handle, DFSDM_DATIN0_CHANNEL, DFSDM_CONTINUOUS_CONV_ON))
  {
    Error_Handler();
  } 
  /* Configure Regular channel */
  if(HAL_OK != HAL_DFSDM_FilterConfigRegChannel(&Dfsdm2Filter3Handle, DFSDM_DATIN7_CHANNEL, DFSDM_CONTINUOUS_CONV_ON))
  {
    Error_Handler();
  } 
  /* Configure Regular channel */
  if(HAL_OK != HAL_DFSDM_FilterConfigRegChannel(&Dfsdm2Filter4Handle, DFSDM_DATIN6_CHANNEL, DFSDM_CONTINUOUS_CONV_ON))
  {
    Error_Handler();
  } 
}

/**
  * @brief  Playback initialization
  * @param  None
  * @retval None
  */
static void Playback_Init(void)
{
  uint16_t buffer_fake[16] = {0x00};

  /* Initialize I2S */
  __HAL_I2S_RESET_HANDLE_STATE(&haudio_i2s);

  /* Configure the I2S peripheral */
  haudio_i2s.Instance = SPI2;
  /* Disable I2S block */
  __HAL_I2S_DISABLE(&haudio_i2s);

  /* I2S peripheral configuration */
  haudio_i2s.Init.AudioFreq = I2S_AUDIOFREQ_16K;
  haudio_i2s.Init.ClockSource = I2S_CLOCK_PLL;
  haudio_i2s.Init.CPOL = I2S_CPOL_LOW;
  haudio_i2s.Init.DataFormat = I2S_DATAFORMAT_16B;
  haudio_i2s.Init.MCLKOutput = I2S_MCLKOUTPUT_ENABLE;
  haudio_i2s.Init.Mode = I2S_MODE_MASTER_TX;
  haudio_i2s.Init.Standard =  I2S_STANDARD_PHILIPS;
  haudio_i2s.Init.FullDuplexMode = I2S_FULLDUPLEXMODE_DISABLE;

  /* Init the I2S */
  HAL_I2S_Init(&haudio_i2s);

 /* Enable I2S block */
  __HAL_I2S_ENABLE(&haudio_i2s);

  /* I2C Init */
   AUDIO_IO_Init();

  /* Initialize audio driver */
  if(WM8994_ID != wm8994_drv.ReadID(AUDIO_I2C_ADDRESS))
  {
    Error_Handler();
  }

  audio_drv = &wm8994_drv;
  audio_drv->Reset(AUDIO_I2C_ADDRESS);
  /* Send fake I2S data in order to generate MCLK needed by WM8994 to set its registers
  * MCLK is generated only when a data stream is sent on I2S */
  HAL_I2S_Transmit_DMA(&haudio_i2s, buffer_fake, 16);

  if(0 != audio_drv->Init(AUDIO_I2C_ADDRESS, OUTPUT_DEVICE_HEADPHONE, 60, AUDIO_FREQUENCY_16K))
  {
    Error_Handler();
  }
  /* Stop sending fake I2S data */
  HAL_I2S_DMAStop(&haudio_i2s);
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
  while (1)
  {
  }
}

/**
  * @brief  Half regular conversion complete callback.
  * @param  hdfsdm_filter : DFSDM filter handle.
  * @retval None
  */
void HAL_DFSDM_FilterRegConvHalfCpltCallback(DFSDM_Filter_HandleTypeDef *hdfsdm_filter)
{
  if(hdfsdm_filter == &Dfsdm2Filter1Handle)
  {
    DmaLeftRecHalfBuffCplt = 1;
  }
  else
  {
    DmaRightRecHalfBuffCplt = 1;
  }
}

/**
  * @brief  Regular conversion complete callback.
  * @note   In interrupt mode, user has to read conversion value in this function
            using HAL_DFSDM_FilterGetRegularValue.
  * @param  hdfsdm_filter : DFSDM filter handle.
  * @retval None
  */
void HAL_DFSDM_FilterRegConvCpltCallback(DFSDM_Filter_HandleTypeDef *hdfsdm_filter)
{
  if(hdfsdm_filter == &Dfsdm2Filter1Handle)
  {
    DmaLeftRecBuffCplt = 1;
  }
  else
  {
    DmaRightRecBuffCplt = 1;
  }
}

#if defined(USE_CHANNEL_DELAY)   
/**
  * @brief  PDM Delay initialization
  * @param  None
  * @retval None
  */
static void PDM_Delay(void)
{
  PulseSkipper_InitTypeDef pulseSkipper;    
  
#if defined(GENERATE_DELAY_DFSDM1_CH1)  
  pulseSkipper.DFSDM1PulseSkipperCh = DFSDM1_PULSE_SKIPPER_CH13;
#elif defined(GENERATE_DELAY_DFSDM1_CH0)  
  pulseSkipper.DFSDM1PulseSkipperCh = DFSDM1_PULSE_SKIPPER_CH02;
#elif defined(GENERATE_DELAY_DFSDM2_CH0)
  pulseSkipper.DFSDM2PulseSkipperCh = DFSDM2_PULSE_SKIPPER_CH04;
#elif defined(GENERATE_DELAY_DFSDM2_CH1)
  pulseSkipper.DFSDM2PulseSkipperCh = DFSDM2_PULSE_SKIPPER_CH15;
#elif defined(GENERATE_DELAY_DFSDM2_CH6)
  pulseSkipper.DFSDM2PulseSkipperCh = DFSDM2_PULSE_SKIPPER_CH26;
#elif defined(GENERATE_DELAY_DFSDM2_CH7)
  pulseSkipper.DFSDM2PulseSkipperCh = DFSDM2_PULSE_SKIPPER_CH37;
#endif  
  
  Pulse_Skipper_Generate_Pulse(&pulseSkipper);
}
#endif

/**
  * @}
  */

/**
  * @}
  */
