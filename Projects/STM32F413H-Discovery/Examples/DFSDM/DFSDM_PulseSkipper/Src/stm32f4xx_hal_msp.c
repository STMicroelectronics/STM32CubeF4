/**
  ******************************************************************************
  * @file    DFSDM/DFSDM_PulseSkipper/Src/stm32f4xx_hal_msp.c
  * @author  MCD Application Team
  * @brief   HAL MSP module.    
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

/** @defgroup HAL_MSP
  * @brief HAL MSP module.
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
DMA_HandleTypeDef            hFilter0Dma;
DMA_HandleTypeDef            hFilter1Dma;
DMA_HandleTypeDef            hFilter2Dma;
DMA_HandleTypeDef            hFilter3Dma;
DMA_HandleTypeDef            hFilter4Dma;
DMA_HandleTypeDef            hdma_i2s_tx;
uint8_t                      channelMspInitCount = 0;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Extern variables ----------------------------------------------------------*/
extern DFSDM_Channel_HandleTypeDef  Dfsdm2Channel1Handle;
extern DFSDM_Channel_HandleTypeDef  DfsdmRightChannelHandle;
extern DFSDM_Filter_HandleTypeDef   Dfsdm2Filter1Handle;
extern DFSDM_Filter_HandleTypeDef   Dfsdm2Filter2Handle;
extern DFSDM_Filter_HandleTypeDef   Dfsdm2Filter3Handle;
extern DFSDM_Filter_HandleTypeDef   Dfsdm2Filter4Handle;
extern DFSDM_Filter_HandleTypeDef   Dfsdm1Filter1Handle;
extern I2S_HandleTypeDef            haudio_i2s;

/** @defgroup HAL_MSP_Private_Functions
  * @{
  */

/**
  * @brief  Initializes the DFSDM channel MSP.
  * @param  hdfsdm_channel : DFSDM channel handle.
  * @retval None
  */
void HAL_DFSDM_ChannelMspInit(DFSDM_Channel_HandleTypeDef *hdfsdm_channel)
{
  GPIO_InitTypeDef  GPIO_InitStruct;
  RCC_PeriphCLKInitTypeDef rcc_ex_clk_init_struct;
    
  if (channelMspInitCount == 0)
  {
    /* Init only once */
    channelMspInitCount++;
  
    /* Enable DFSDM clock */
    __HAL_RCC_DFSDM1_CLK_ENABLE();
    __HAL_RCC_DFSDM2_CLK_ENABLE();

    /* DFSDM pins configuration: DFSDM_CKOUT, DMIC_DATIN pins ------------------*/
    __DFSDM1_CKOUT_ENABLE();
    GPIO_InitStruct.Pin = DFSDM1_CKOUT_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStruct.Alternate = DFSDM1_CKOUT_ALTERNATE;
    HAL_GPIO_Init(DFSDM1_CKOUT_PORT, &GPIO_InitStruct);
    
    __DFSDM2_CKOUT_ENABLE();
    GPIO_InitStruct.Pin = DFSDM2_CKOUT_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStruct.Alternate = DFSDM2_CKOUT_ALTERNATE;
    HAL_GPIO_Init(DFSDM2_CKOUT_PORT, &GPIO_InitStruct);

    /* MP34DT01TR microphones uses DFSDM2_DATIN1 input pin */
    __DFSDM2_DATIN1_ENABLE();
    GPIO_InitStruct.Pin = DFSDM2_DATIN1_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStruct.Alternate = DFSDM2_DATIN1_ALTERNATE;
    HAL_GPIO_Init(DFSDM2_DATIN1_PORT, &GPIO_InitStruct);
    
    /* MP34DT01TR microphones uses DFSDM2_DATIN7 input pin */
    __DFSDM2_DATIN7_ENABLE();
    GPIO_InitStruct.Pin = DFSDM2_DATIN7_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStruct.Alternate = DFSDM2_DATIN7_ALTERNATE;
    HAL_GPIO_Init(DFSDM2_DATIN7_PORT, &GPIO_InitStruct);
    
    /* MP34DT01TR microphones uses DFSDM1_DATIN1 input pin */
    __DFSDM1_DATIN1_ENABLE();
    GPIO_InitStruct.Pin = DFSDM1_DATIN1_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStruct.Alternate = DFSDM1_DATIN1_ALTERNATE;
    HAL_GPIO_Init(DFSDM1_DATIN1_PORT, &GPIO_InitStruct);    

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
    __HAL_RCC_DFSDM2AUDIO_CONFIG(RCC_DFSDM2AUDIOCLKSOURCE_I2SAPB1);
  }
}

/**
  * @brief  Initializes the DFSDM filter MSP.
  * @param  hdfsdm_filter : DFSDM filter handle.
  * @retval None
  */
void HAL_DFSDM_FilterMspInit(DFSDM_Filter_HandleTypeDef *hdfsdm_filter)
{
  __HAL_RCC_DMA2_CLK_ENABLE();
  
  if(hdfsdm_filter == &Dfsdm2Filter1Handle)
  {
    /* AUDIO_DFSDM2 FILTER0 */
    /* Configure the hdma_dfsdm_left handle parameters */
    hFilter0Dma.Init.Direction           = DMA_PERIPH_TO_MEMORY;
    hFilter0Dma.Init.PeriphInc           = DMA_PINC_DISABLE;
    hFilter0Dma.Init.MemInc              = DMA_MINC_ENABLE;
    hFilter0Dma.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
    hFilter0Dma.Init.MemDataAlignment    = DMA_MDATAALIGN_WORD;
    hFilter0Dma.Init.Mode                = DMA_CIRCULAR;
    hFilter0Dma.Init.Priority            = DMA_PRIORITY_HIGH;
    hFilter0Dma.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
    hFilter0Dma.Init.MemBurst            = DMA_MBURST_SINGLE;
    hFilter0Dma.Init.PeriphBurst         = DMA_PBURST_SINGLE;
    hFilter0Dma.Init.Channel             = DMA_CHANNEL_8;
    hFilter0Dma.Instance                 = DMA2_Stream0;

    /* Associate the DMA handle */
    __HAL_LINKDMA(&Dfsdm2Filter1Handle, hdmaReg, hFilter0Dma);

    if (HAL_OK != HAL_DMA_Init(&hFilter0Dma))
    {
      Error_Handler();
    }
    HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 0x01, 0);
    HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);
  }
  else if(hdfsdm_filter == &Dfsdm2Filter2Handle)
  {
    /* AUDIO_DFSDM2_FILTER1 */
    /* Configure the hdma_dfsdm_right handle parameters */
    hFilter1Dma.Init.Direction           = DMA_PERIPH_TO_MEMORY;
    hFilter1Dma.Init.PeriphInc           = DMA_PINC_DISABLE;
    hFilter1Dma.Init.MemInc              = DMA_MINC_ENABLE;
    hFilter1Dma.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
    hFilter1Dma.Init.MemDataAlignment    = DMA_MDATAALIGN_WORD;
    hFilter1Dma.Init.Mode                = DMA_CIRCULAR;
    hFilter1Dma.Init.Priority            = DMA_PRIORITY_HIGH;
    hFilter1Dma.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
    hFilter1Dma.Init.MemBurst            = DMA_MBURST_SINGLE;
    hFilter1Dma.Init.PeriphBurst         = DMA_PBURST_SINGLE;
    hFilter1Dma.Init.Channel             = DMA_CHANNEL_8;
    hFilter1Dma.Instance                 = DMA2_Stream1;

    /* Associate the DMA handle */
    __HAL_LINKDMA(&Dfsdm2Filter2Handle, hdmaReg, hFilter1Dma);

    if (HAL_OK != HAL_DMA_Init(&hFilter1Dma))
    {
      Error_Handler();
    }
    HAL_NVIC_SetPriority(DMA2_Stream1_IRQn, 0x01, 0);
    HAL_NVIC_EnableIRQ(DMA2_Stream1_IRQn);
  }
  else if(hdfsdm_filter == &Dfsdm2Filter3Handle)
  {
    /* AUDIO_DFSDM2_FILTER2 */
    /* Configure the hdma_dfsdm_right handle parameters */
    hFilter2Dma.Init.Direction           = DMA_PERIPH_TO_MEMORY;
    hFilter2Dma.Init.PeriphInc           = DMA_PINC_DISABLE;
    hFilter2Dma.Init.MemInc              = DMA_MINC_ENABLE;
    hFilter2Dma.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
    hFilter2Dma.Init.MemDataAlignment    = DMA_MDATAALIGN_WORD;
    hFilter2Dma.Init.Mode                = DMA_CIRCULAR;
    hFilter2Dma.Init.Priority            = DMA_PRIORITY_HIGH;
    hFilter2Dma.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
    hFilter2Dma.Init.MemBurst            = DMA_MBURST_SINGLE;
    hFilter2Dma.Init.PeriphBurst         = DMA_PBURST_SINGLE;
    hFilter2Dma.Init.Channel             = DMA_CHANNEL_8;
    hFilter2Dma.Instance                 = DMA2_Stream2;

    /* Associate the DMA handle */
    __HAL_LINKDMA(&Dfsdm2Filter3Handle, hdmaReg, hFilter2Dma);

    if (HAL_OK != HAL_DMA_Init(&hFilter2Dma))
    {
      Error_Handler();
    }
    HAL_NVIC_SetPriority(DMA2_Stream2_IRQn, 0x01, 0);
    HAL_NVIC_EnableIRQ(DMA2_Stream2_IRQn);
  }
  else if(hdfsdm_filter == &Dfsdm2Filter4Handle)
  {
    /* AUDIO_DFSDM2_FILTER3 */
    /* Configure the hdma_dfsdm_right handle parameters */
    hFilter3Dma.Init.Direction           = DMA_PERIPH_TO_MEMORY;
    hFilter3Dma.Init.PeriphInc           = DMA_PINC_DISABLE;
    hFilter3Dma.Init.MemInc              = DMA_MINC_ENABLE;
    hFilter3Dma.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
    hFilter3Dma.Init.MemDataAlignment    = DMA_MDATAALIGN_WORD;
    hFilter3Dma.Init.Mode                = DMA_CIRCULAR;
    hFilter3Dma.Init.Priority            = DMA_PRIORITY_HIGH;
    hFilter3Dma.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
    hFilter3Dma.Init.MemBurst            = DMA_MBURST_SINGLE;
    hFilter3Dma.Init.PeriphBurst         = DMA_PBURST_SINGLE;
    hFilter3Dma.Init.Channel             = DMA_CHANNEL_8;
    hFilter3Dma.Instance                 = DMA2_Stream3;

    /* Associate the DMA handle */
    __HAL_LINKDMA(&Dfsdm2Filter4Handle, hdmaReg, hFilter3Dma);

    if (HAL_OK != HAL_DMA_Init(&hFilter3Dma))
    {
      Error_Handler();
    }
    HAL_NVIC_SetPriority(DMA2_Stream3_IRQn, 0x01, 0);
    HAL_NVIC_EnableIRQ(DMA2_Stream3_IRQn);
  }
  else if(hdfsdm_filter == &Dfsdm1Filter1Handle)
  {
    /* AUDIO_DFSDM1_FILTER0 */
    /* Configure the hdma_dfsdm_right handle parameters */
    hFilter4Dma.Init.Direction           = DMA_PERIPH_TO_MEMORY;
    hFilter4Dma.Init.PeriphInc           = DMA_PINC_DISABLE;
    hFilter4Dma.Init.MemInc              = DMA_MINC_ENABLE;
    hFilter4Dma.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
    hFilter4Dma.Init.MemDataAlignment    = DMA_MDATAALIGN_WORD;
    hFilter4Dma.Init.Mode                = DMA_CIRCULAR;
    hFilter4Dma.Init.Priority            = DMA_PRIORITY_HIGH;
    hFilter4Dma.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
    hFilter4Dma.Init.MemBurst            = DMA_MBURST_SINGLE;
    hFilter4Dma.Init.PeriphBurst         = DMA_PBURST_SINGLE;
    hFilter4Dma.Init.Channel             = DMA_CHANNEL_3;
    hFilter4Dma.Instance                 = DMA2_Stream6;

    /* Associate the DMA handle */
    __HAL_LINKDMA(&Dfsdm1Filter1Handle, hdmaReg, hFilter4Dma);

    if (HAL_OK != HAL_DMA_Init(&hFilter4Dma))
    {
      Error_Handler();
    }
    HAL_NVIC_SetPriority(DMA2_Stream6_IRQn, 0x01, 0);
    HAL_NVIC_EnableIRQ(DMA2_Stream6_IRQn);
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
  __HAL_RCC_SPI2_CLK_ENABLE();

  /* Enable MCK, SCK, WS, SD and CODEC_INT GPIO clock */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /* CODEC_I2S pins configuration: MCK, SCK, WS and SD pins */
  gpio_init_structure.Pin = GPIO_PIN_3;
  gpio_init_structure.Mode = GPIO_MODE_AF_PP;
  gpio_init_structure.Pull = GPIO_NOPULL;
  gpio_init_structure.Speed = GPIO_SPEED_FAST;
  gpio_init_structure.Alternate = GPIO_AF5_SPI2;
  HAL_GPIO_Init(GPIOA, &gpio_init_structure);

  gpio_init_structure.Pin = GPIO_PIN_3;
  gpio_init_structure.Alternate = GPIO_AF5_SPI2;
  HAL_GPIO_Init(GPIOD, &gpio_init_structure);

  gpio_init_structure.Pin = GPIO_PIN_9;
  gpio_init_structure.Alternate = GPIO_AF5_SPI2;
  HAL_GPIO_Init(GPIOB, &gpio_init_structure);

  gpio_init_structure.Pin = GPIO_PIN_3;
  gpio_init_structure.Alternate = GPIO_AF5_SPI2;
  HAL_GPIO_Init(GPIOC, &gpio_init_structure);

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

  hdma_i2s_tx.Instance = DMA1_Stream4;

  /* Associate the DMA handle */
  __HAL_LINKDMA(&haudio_i2s, hdmatx, hdma_i2s_tx);

  /* Deinitialize the Stream for new transfer */
  HAL_DMA_DeInit(&hdma_i2s_tx);

  if (HAL_OK != HAL_DMA_Init(&hdma_i2s_tx))
  {
    Error_Handler();
  }
  /* Enable and set I2Sx Interrupt to a lower priority */
  HAL_NVIC_SetPriority(SPI2_IRQn, 0x01, 0);
  HAL_NVIC_EnableIRQ(SPI2_IRQn);
  /* I2S DMA IRQ Channel configuration */
  HAL_NVIC_SetPriority(DMA1_Stream4_IRQn, 0x01, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream4_IRQn);
}

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
