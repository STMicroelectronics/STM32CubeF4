/**
  ******************************************************************************
  * @file    DFSDM/DFSDM_AudioRecord/Src/main.c
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

/** @addtogroup DFSDM_AudioRecord
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#define SaturaLH(N, L, H) (((N)<(L))?(L):(((N)>(H))?(H):(N)))
/* Private variables ---------------------------------------------------------*/
DFSDM_Channel_HandleTypeDef  DfsdmLeftChannelHandle;
DFSDM_Channel_HandleTypeDef  DfsdmRightChannelHandle;
DFSDM_Filter_HandleTypeDef   DfsdmLeftFilterHandle;
DFSDM_Filter_HandleTypeDef   DfsdmRightFilterHandle;
DMA_HandleTypeDef            hLeftDma;
DMA_HandleTypeDef            hRightDma;
I2S_HandleTypeDef            haudio_i2s;
DMA_HandleTypeDef            hdma_i2s_tx;
AUDIO_DrvTypeDef             *audio_drv;
int32_t                      LeftRecBuff[2048];
int32_t                      RightRecBuff[2048];
int16_t                      PlayBuff[4096];
uint32_t                     DmaLeftRecHalfBuffCplt  = 0;
uint32_t                     DmaLeftRecBuffCplt      = 0;
uint32_t                     DmaRightRecHalfBuffCplt = 0;
uint32_t                     DmaRightRecBuffCplt     = 0;
uint32_t                     PlaybackStarted         = 0;
/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void DFSDM_Init(void);
static void Playback_Init(void);
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  uint32_t i;
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

  /* Configure LED3 */
  BSP_LED_Init(LED3);

  /* Initialize DFSDM channels and filter for record */
  DFSDM_Init();

  /* Initialize playback */
  Playback_Init();

  /* Start DFSDM conversions */
  if(HAL_OK != HAL_DFSDM_FilterRegularStart_DMA(&DfsdmRightFilterHandle, RightRecBuff, 2048))
  {
    Error_Handler();
  }
  if(HAL_OK != HAL_DFSDM_FilterRegularStart_DMA(&DfsdmLeftFilterHandle, LeftRecBuff, 2048))
  {
    Error_Handler();
  }

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
}

/**
  * @brief  DFSDM channels and filter initialization
  * @param  None
  * @retval None
  */
static void DFSDM_Init(void)
{
  /* Initialize channel 1 (left channel)*/
  __HAL_DFSDM_CHANNEL_RESET_HANDLE_STATE(&DfsdmLeftChannelHandle);
  DfsdmLeftChannelHandle.Instance                      = DFSDM1_Channel0;
  DfsdmLeftChannelHandle.Init.OutputClock.Activation   = ENABLE;
  DfsdmLeftChannelHandle.Init.OutputClock.Selection    = DFSDM_CHANNEL_OUTPUT_CLOCK_AUDIO;
  DfsdmLeftChannelHandle.Init.OutputClock.Divider      = 24;
  DfsdmLeftChannelHandle.Init.Input.Multiplexer        = DFSDM_CHANNEL_EXTERNAL_INPUTS;
  DfsdmLeftChannelHandle.Init.Input.DataPacking        = DFSDM_CHANNEL_STANDARD_MODE;
  DfsdmLeftChannelHandle.Init.Input.Pins               = DFSDM_CHANNEL_SAME_CHANNEL_PINS;
  DfsdmLeftChannelHandle.Init.SerialInterface.Type     = DFSDM_CHANNEL_SPI_RISING;
  DfsdmLeftChannelHandle.Init.SerialInterface.SpiClock = DFSDM_CHANNEL_SPI_CLOCK_INTERNAL;
  DfsdmLeftChannelHandle.Init.Awd.FilterOrder          = DFSDM_CHANNEL_SINC1_ORDER;
  DfsdmLeftChannelHandle.Init.Awd.Oversampling         = 10;
  DfsdmLeftChannelHandle.Init.Offset                   = 0;
  DfsdmLeftChannelHandle.Init.RightBitShift            = 2;
  if(HAL_OK != HAL_DFSDM_ChannelInit(&DfsdmLeftChannelHandle))
  {
    Error_Handler();
  }

  /* Initialize channel 0 (right channel)*/
  __HAL_DFSDM_CHANNEL_RESET_HANDLE_STATE(&DfsdmRightChannelHandle);
  DfsdmRightChannelHandle.Instance                      = DFSDM1_Channel3;
  DfsdmRightChannelHandle.Init.OutputClock.Activation   = ENABLE;
  DfsdmRightChannelHandle.Init.OutputClock.Selection    = DFSDM_CHANNEL_OUTPUT_CLOCK_AUDIO;
  DfsdmRightChannelHandle.Init.OutputClock.Divider      = 24;
  DfsdmRightChannelHandle.Init.Input.Multiplexer        = DFSDM_CHANNEL_EXTERNAL_INPUTS;
  DfsdmRightChannelHandle.Init.Input.DataPacking        = DFSDM_CHANNEL_STANDARD_MODE;
  DfsdmRightChannelHandle.Init.Input.Pins               = DFSDM_CHANNEL_FOLLOWING_CHANNEL_PINS;
  DfsdmRightChannelHandle.Init.SerialInterface.Type     = DFSDM_CHANNEL_SPI_FALLING;
  DfsdmRightChannelHandle.Init.SerialInterface.SpiClock = DFSDM_CHANNEL_SPI_CLOCK_INTERNAL;
  DfsdmRightChannelHandle.Init.Awd.FilterOrder          = DFSDM_CHANNEL_SINC1_ORDER;
  DfsdmRightChannelHandle.Init.Awd.Oversampling         = 10;
  DfsdmRightChannelHandle.Init.Offset                   = 0;
  DfsdmRightChannelHandle.Init.RightBitShift            = 2;
  if(HAL_OK != HAL_DFSDM_ChannelInit(&DfsdmRightChannelHandle))
  {
    Error_Handler();
  }

  /* Initialize filter 0 (left channel) */
  __HAL_DFSDM_FILTER_RESET_HANDLE_STATE(&DfsdmLeftFilterHandle);
  DfsdmLeftFilterHandle.Instance                          = DFSDM1_Filter0;
  DfsdmLeftFilterHandle.Init.RegularParam.Trigger         = DFSDM_FILTER_SW_TRIGGER;
  DfsdmLeftFilterHandle.Init.RegularParam.FastMode        = ENABLE;
  DfsdmLeftFilterHandle.Init.RegularParam.DmaMode         = ENABLE;
  DfsdmLeftFilterHandle.Init.InjectedParam.Trigger        = DFSDM_FILTER_SW_TRIGGER;
  DfsdmLeftFilterHandle.Init.InjectedParam.ScanMode       = DISABLE;
  DfsdmLeftFilterHandle.Init.InjectedParam.DmaMode        = DISABLE;
  DfsdmLeftFilterHandle.Init.InjectedParam.ExtTrigger     = DFSDM_FILTER_EXT_TRIG_TIM8_TRGO;
  DfsdmLeftFilterHandle.Init.InjectedParam.ExtTriggerEdge = DFSDM_FILTER_EXT_TRIG_BOTH_EDGES;
  DfsdmLeftFilterHandle.Init.FilterParam.SincOrder        = DFSDM_FILTER_SINC3_ORDER;
  DfsdmLeftFilterHandle.Init.FilterParam.Oversampling     = 128;
  DfsdmLeftFilterHandle.Init.FilterParam.IntOversampling  = 1;
  if(HAL_OK != HAL_DFSDM_FilterInit(&DfsdmLeftFilterHandle))
  {
    Error_Handler();
  }
  /* Initialize filter 1 (right channel) */
  __HAL_DFSDM_FILTER_RESET_HANDLE_STATE(&DfsdmRightFilterHandle);
  DfsdmRightFilterHandle.Instance                          = DFSDM1_Filter1;
  DfsdmRightFilterHandle.Init.RegularParam.Trigger         = DFSDM_FILTER_SYNC_TRIGGER;
  DfsdmRightFilterHandle.Init.RegularParam.FastMode        = ENABLE;
  DfsdmRightFilterHandle.Init.RegularParam.DmaMode         = ENABLE;
  DfsdmRightFilterHandle.Init.InjectedParam.Trigger        = DFSDM_FILTER_SW_TRIGGER;
  DfsdmRightFilterHandle.Init.InjectedParam.ScanMode       = DISABLE;
  DfsdmRightFilterHandle.Init.InjectedParam.DmaMode        = DISABLE;
  DfsdmRightFilterHandle.Init.InjectedParam.ExtTrigger     = DFSDM_FILTER_EXT_TRIG_TIM8_TRGO;
  DfsdmRightFilterHandle.Init.InjectedParam.ExtTriggerEdge = DFSDM_FILTER_EXT_TRIG_BOTH_EDGES;
  DfsdmRightFilterHandle.Init.FilterParam.SincOrder        = DFSDM_FILTER_SINC3_ORDER;
  DfsdmRightFilterHandle.Init.FilterParam.Oversampling     = 128;
  DfsdmRightFilterHandle.Init.FilterParam.IntOversampling  = 1;
  if(HAL_OK != HAL_DFSDM_FilterInit(&DfsdmRightFilterHandle))
  {
    Error_Handler();
  }

  /* Configure Regular channel */
  if(HAL_OK != HAL_DFSDM_FilterConfigRegChannel(&DfsdmLeftFilterHandle, DFSDM_CHANNEL_0, DFSDM_CONTINUOUS_CONV_ON))
  {
    Error_Handler();
  }
  /* Configure Regular channel */
  if(HAL_OK != HAL_DFSDM_FilterConfigRegChannel(&DfsdmRightFilterHandle, DFSDM_CHANNEL_3, DFSDM_CONTINUOUS_CONV_ON))
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
  haudio_i2s.Instance = SPI3;
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

  if(0 != audio_drv->Init(AUDIO_I2C_ADDRESS, OUTPUT_DEVICE_HEADPHONE, 90, AUDIO_FREQUENCY_16K))
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
    /* Toggle LED3 with a period of one second */
    BSP_LED_Toggle(LED3);
    HAL_Delay(1000);
  }
}

/**
  * @brief  Initializes the DFSDM channel MSP.
  * @param  hdfsdm_channel : DFSDM channel handle.
  * @retval None
  */
void HAL_DFSDM_ChannelMspInit(DFSDM_Channel_HandleTypeDef *hdfsdm_channel)
{
  if (hdfsdm_channel == &DfsdmLeftChannelHandle)
  {
    GPIO_InitTypeDef  GPIO_InitStruct;
    RCC_PeriphCLKInitTypeDef rcc_ex_clk_init_struct;

    /* Enable DFSDM clock */
    __HAL_RCC_DFSDM_CLK_ENABLE();

    /* DFSDM pins configuration: DFSDM_CKOUT, DMIC_DATIN pins ------------------*/
    __HAL_RCC_GPIOC_CLK_ENABLE();
    GPIO_InitStruct.Pin = GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF8_DFSDM1;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /* MP34DT01TR microphones uses DFSDM_DATIN0 input pin */
    __HAL_RCC_GPIOB_CLK_ENABLE();
    GPIO_InitStruct.Pin = GPIO_PIN_1;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF8_DFSDM1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* Configure and enable PLLI2S clock to generate audio clock */
    rcc_ex_clk_init_struct.PeriphClockSelection = (RCC_PERIPHCLK_I2S_APB1 | RCC_PERIPHCLK_DFSDM);
    rcc_ex_clk_init_struct.I2sApb1ClockSelection = RCC_I2SAPB1CLKSOURCE_PLLI2S;
    rcc_ex_clk_init_struct.DfsdmClockSelection = RCC_DFSDM1CLKSOURCE_APB2;
    rcc_ex_clk_init_struct.PLLI2S.PLLI2SM = 8;
    rcc_ex_clk_init_struct.PLLI2S.PLLI2SN = 344;
    rcc_ex_clk_init_struct.PLLI2S.PLLI2SR = 7;

    HAL_RCCEx_PeriphCLKConfig(&rcc_ex_clk_init_struct);

    /* I2S_APB1 selected as DFSDM audio clock source */
    __HAL_RCC_DFSDM1AUDIO_CONFIG(RCC_DFSDM1AUDIOCLKSOURCE_I2SAPB1);
  }
}

/**
  * @brief  Initializes the DFSDM filter MSP.
  * @param  hdfsdm_filter : DFSDM filter handle.
  * @retval None
  */
void HAL_DFSDM_FilterMspInit(DFSDM_Filter_HandleTypeDef *hdfsdm_filter)
{
  if(hdfsdm_filter == &DfsdmLeftFilterHandle)
  {
    /* Configure DMA1_Channel4 */
    __HAL_RCC_DMA2_CLK_ENABLE();

    /* AUDIO_DFSDMx_LEFT FILTER */
    /* Configure the hdma_dfsdm_left handle parameters */
    hLeftDma.Init.Direction           = DMA_PERIPH_TO_MEMORY;
    hLeftDma.Init.PeriphInc           = DMA_PINC_DISABLE;
    hLeftDma.Init.MemInc              = DMA_MINC_ENABLE;
    hLeftDma.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
    hLeftDma.Init.MemDataAlignment    = DMA_MDATAALIGN_WORD;
    hLeftDma.Init.Mode                = DMA_CIRCULAR;
    hLeftDma.Init.Priority            = DMA_PRIORITY_HIGH;
    hLeftDma.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
    hLeftDma.Init.Channel             = DMA_CHANNEL_3;
    hLeftDma.Instance                 = DMA2_Stream6;

    /* Associate the DMA handle */
    __HAL_LINKDMA(&DfsdmLeftFilterHandle, hdmaReg, hLeftDma);

    if (HAL_OK != HAL_DMA_Init(&hLeftDma))
    {
      Error_Handler();
    }
    HAL_NVIC_SetPriority(DMA2_Stream6_IRQn, 0x01, 0);
    HAL_NVIC_EnableIRQ(DMA2_Stream6_IRQn);
  }
  else if(hdfsdm_filter == &DfsdmRightFilterHandle)
  {
    /* AUDIO_DFSDMx_RIGHT_FILTER */
    /* Configure the hdma_dfsdm_right handle parameters */
    hRightDma.Init.Direction           = DMA_PERIPH_TO_MEMORY;
    hRightDma.Init.PeriphInc           = DMA_PINC_DISABLE;
    hRightDma.Init.MemInc              = DMA_MINC_ENABLE;
    hRightDma.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
    hRightDma.Init.MemDataAlignment    = DMA_MDATAALIGN_WORD;
    hRightDma.Init.Mode                = DMA_CIRCULAR;
    hRightDma.Init.Priority            = DMA_PRIORITY_HIGH;
    hRightDma.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
    hRightDma.Init.MemBurst            = DMA_MBURST_SINGLE;
    hRightDma.Init.PeriphBurst         = DMA_PBURST_SINGLE;
    hRightDma.Init.Channel             = DMA_CHANNEL_3;
    hRightDma.Instance                 = DMA2_Stream4;

    /* Associate the DMA handle */
    __HAL_LINKDMA(&DfsdmRightFilterHandle, hdmaReg, hRightDma);

    if (HAL_OK != HAL_DMA_Init(&hRightDma))
    {
      Error_Handler();
    }
    HAL_NVIC_SetPriority(DMA2_Stream4_IRQn, 0x01, 0);
    HAL_NVIC_EnableIRQ(DMA2_Stream4_IRQn);
  }
}

/**
  * @brief  I2S MSP Init.
  * @param  hsai : pointer to a SAI_HandleTypeDef structure that contains
  *                the configuration information for SAI module.
  * @retval None
  */
void HAL_I2S_MspInit(I2S_HandleTypeDef *hsai)
{
  GPIO_InitTypeDef  gpio_init_structure;

  /* Enable I2S Audio clock */
  __HAL_RCC_SPI3_CLK_ENABLE();

  /* Enable MCK, SCK, WS, SD and CODEC_INT GPIO clock */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /* CODEC_I2S pins configuration: MCK, SCK, WS and SD pins */
  gpio_init_structure.Pin = GPIO_PIN_7;
  gpio_init_structure.Mode = GPIO_MODE_AF_PP;
  gpio_init_structure.Pull = GPIO_NOPULL;
  gpio_init_structure.Speed = GPIO_SPEED_FAST;
  gpio_init_structure.Alternate = GPIO_AF6_SPI3;
  HAL_GPIO_Init(GPIOC, &gpio_init_structure);

  gpio_init_structure.Pin = GPIO_PIN_12;
  gpio_init_structure.Alternate = GPIO_AF7_SPI3;
  HAL_GPIO_Init(GPIOB, &gpio_init_structure);

  gpio_init_structure.Pin = GPIO_PIN_4;
  gpio_init_structure.Alternate = GPIO_AF6_SPI3;
  HAL_GPIO_Init(GPIOA, &gpio_init_structure);

  gpio_init_structure.Pin = GPIO_PIN_5;
  gpio_init_structure.Alternate = GPIO_AF6_SPI3;
  HAL_GPIO_Init(GPIOB, &gpio_init_structure);

  /* Configure DMA used for I2S */
  __HAL_RCC_DMA1_CLK_ENABLE();
  hdma_i2s_tx.Init.Channel             = DMA_CHANNEL_0;
  hdma_i2s_tx.Init.Direction           = DMA_MEMORY_TO_PERIPH;
  hdma_i2s_tx.Init.PeriphInc           = DMA_PINC_DISABLE;
  hdma_i2s_tx.Init.MemInc              = DMA_MINC_ENABLE;
  hdma_i2s_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
  hdma_i2s_tx.Init.MemDataAlignment    = DMA_MDATAALIGN_HALFWORD;
  hdma_i2s_tx.Init.Mode                = DMA_CIRCULAR;
  hdma_i2s_tx.Init.Priority            = DMA_PRIORITY_HIGH;
  hdma_i2s_tx.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
  hdma_i2s_tx.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
  hdma_i2s_tx.Init.MemBurst            = DMA_MBURST_SINGLE;
  hdma_i2s_tx.Init.PeriphBurst         = DMA_MBURST_SINGLE;

  hdma_i2s_tx.Instance = DMA1_Stream5;

  /* Associate the DMA handle */
  __HAL_LINKDMA(&haudio_i2s, hdmatx, hdma_i2s_tx);

  /* Deinitialize the Stream for new transfer */
  HAL_DMA_DeInit(&hdma_i2s_tx);

  if (HAL_OK != HAL_DMA_Init(&hdma_i2s_tx))
  {
    Error_Handler();
  }
  /* Enable and set I2Sx Interrupt to a lower priority */
  HAL_NVIC_SetPriority(SPI3_IRQn, 0x01, 0);
  HAL_NVIC_EnableIRQ(SPI3_IRQn);
  /* I2S DMA IRQ Channel configuration */
  HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, 0x01, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);
}

/**
  * @brief  Half regular conversion complete callback.
  * @param  hdfsdm_filter : DFSDM filter handle.
  * @retval None
  */
void HAL_DFSDM_FilterRegConvHalfCpltCallback(DFSDM_Filter_HandleTypeDef *hdfsdm_filter)
{
  if(hdfsdm_filter == &DfsdmLeftFilterHandle)
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
  if(hdfsdm_filter == &DfsdmLeftFilterHandle)
  {
    DmaLeftRecBuffCplt = 1;
  }
  else
  {
    DmaRightRecBuffCplt = 1;
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
