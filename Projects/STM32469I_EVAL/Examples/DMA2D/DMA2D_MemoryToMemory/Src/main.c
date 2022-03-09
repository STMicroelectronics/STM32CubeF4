/**
  ******************************************************************************
  * @file    DMA2D/DMA2D_MemoryToMemory/Src/main.c
  * @author  MCD Application Team
  * @brief   This example provides a description of how to configure
  *          DMA2D peripheral in Memory to Memory transfer mode
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

/** @addtogroup DMA2D_MemoryToMemory
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
DMA2D_HandleTypeDef Dma2dHandle;

__ALIGN_BEGIN uint32_t aBufferResult[(LAYER_SIZE_X * LAYER_SIZE_Y * LAYER_NB_BYTES_PER_PIXEL) / 4] __ALIGN_END;

/* ARGB4444 input buffer 16bpp */
__ALIGN_BEGIN uint32_t aBufferInput[(LAYER_SIZE_X * LAYER_SIZE_Y * LAYER_NB_BYTES_PER_PIXEL) / 4] __ALIGN_END =
      {0xEFFFFFF2, 0xAB1FFFF0, 0x0504044E, 0x89ADCDEF, 0x15248722, 0x5201245E, 0x01025FDE, 0xFE010203,
       0xEFFFFFF2, 0xAB1FFFF0, 0x0504044E, 0x89ADCDEF, 0x15248722, 0x5201245E, 0x01025FDE, 0xFE010203,
       0xEFFFFFF2, 0xAB1FFFF0, 0x0504044E, 0x89ADCDEF, 0x15248722, 0x5201245E, 0x01025FDE, 0xFE010203,
       0xEFFFFFF2, 0xAB1FFFF0, 0x0504044E, 0x89ADCDEF, 0x15248722, 0x5201245E, 0x01025FDE, 0xFE010203,
       0xEFFFFFF2, 0xAB1FFFF0, 0x0504044E, 0x89ADCDEF, 0x15248722, 0x5201245E, 0x01025FDE, 0xFE010203,
       0xEFFFFFF2, 0xAB1FFFF0, 0x0504044E, 0x89ADCDEF, 0x15248722, 0x5201245E, 0x01025FDE, 0xFE010203,
       0xEFFFFFF2, 0xAB1FFFF0, 0x0504044E, 0x89ADCDEF, 0x15248722, 0x5201245E, 0x01025FDE, 0xFE010203,
       0xEFFFFFF2, 0xAB1FFFF0, 0x0504044E, 0x89ADCDEF, 0x15248722, 0x5201245E, 0x01025FDE, 0xFE010203,
       0xEFFFFFF2, 0xAB1FFFF0, 0x0504044E, 0x89ADCDEF, 0x15248722, 0x5201245E, 0x01025FDE, 0xFE010203,
       0xEFFFFFF2, 0xAB1FFFF0, 0x0504044E, 0x89ADCDEF, 0x15248722, 0x5201245E, 0x01025FDE, 0xFE010203,
       0xEFFFFFF2, 0xAB1FFFF0, 0x0504044E, 0x89ADCDEF, 0x15248722, 0x5201245E, 0x01025FDE, 0xFE010203,
       0xEFFFFFF2, 0xAB1FFFF0, 0x0504044E, 0x89ADCDEF, 0x15248722, 0x5201245E, 0x01025FDE, 0xFE010203,
       0xEFFFFFF2, 0xAB1FFFF0, 0x0504044E, 0x89ADCDEF, 0x15248722, 0x5201245E, 0x01025FDE, 0xFE010203,
       0xEFFFFFF2, 0xAB1FFFF0, 0x0504044E, 0x89ADCDEF, 0x15248722, 0x5201245E, 0x01025FDE, 0xFE010203,
       0xEFFFFFF2, 0xAB1FFFF0, 0x0504044E, 0x89ADCDEF, 0x15248722, 0x5201245E, 0x01025FDE, 0xFE010203,
       0xEFFFFFF2, 0xAB1FFFF0, 0x0504044E, 0x89ADCDEF, 0x15248722, 0x5201245E, 0x01025FDE, 0xFE010203,
       0xEFFFFFF2, 0xAB1FFFF0, 0x0504044E, 0x89ADCDEF, 0x15248722, 0x5201245E, 0x01025FDE, 0xFE010203,
       0xEFFFFFF2, 0xAB1FFFF0, 0x0504044E, 0x89ADCDEF, 0x15248722, 0x5201245E, 0x01025FDE, 0xFE010203,
       0xEFFFFFF2, 0xAB1FFFF0, 0x0504044E, 0x89ADCDEF, 0x15248722, 0x5201245E, 0x01025FDE, 0xFE010203,
       0xEFFFFFF2, 0xAB1FFFF0, 0x0504044E, 0x89ADCDEF, 0x15248722, 0x5201245E, 0x01025FDE, 0xFE010203,
       0xEFFFFFF2, 0xAB1FFFF0, 0x0504044E, 0x89ADCDEF, 0x15248722, 0x5201245E, 0x01025FDE, 0xFE010203,
       0xEFFFFFF2, 0xAB1FFFF0, 0x0504044E, 0x89ADCDEF, 0x15248722, 0x5201245E, 0x01025FDE, 0xFE010203,
       0xEFFFFFF2, 0xAB1FFFF0, 0x0504044E, 0x89ADCDEF, 0x15248722, 0x5201245E, 0x01025FDE, 0xFE010203,
       0xEFFFFFF2, 0xAB1FFFF0, 0x0504044E, 0x89ADCDEF, 0x15248722, 0x5201245E, 0x01025FDE, 0xFE010203,
       0xEFFFFFF2, 0xAB1FFFF0, 0x0504044E, 0x89ADCDEF, 0x15248722, 0x5201245E, 0x01025FDE, 0xFE010203,
       0xEFFFFFF2, 0xAB1FFFF0, 0x0504044E, 0x89ADCDEF, 0x15248722, 0x5201245E, 0x01025FDE, 0xFE010203,
       0xEFFFFFF2, 0xAB1FFFF0, 0x0504044E, 0x89ADCDEF, 0x15248722, 0x5201245E, 0x01025FDE, 0xFE010203,
       0xEFFFFFF2, 0xAB1FFFF0, 0x0504044E, 0x89ADCDEF, 0x15248722, 0x5201245E, 0x01025FDE, 0xFE010203,
       0xEFFFFFF2, 0xAB1FFFF0, 0x0504044E, 0x89ADCDEF, 0x15248722, 0x5201245E, 0x01025FDE, 0xFE010203,
       0xEFFFFFF2, 0xAB1FFFF0, 0x0504044E, 0x89ADCDEF, 0x15248722, 0x5201245E, 0x01025FDE, 0xFE010203,
       0xEFFFFFF2, 0xAB1FFFF0, 0x0504044E, 0x89ADCDEF, 0x15248722, 0x5201245E, 0x01025FDE, 0xFE010203,
       0xEFFFFFF2, 0xAB1FFFF0, 0x0504044E, 0x89ADCDEF, 0x15248722, 0x5201245E, 0x01025FDE, 0xFE010203};

/* Private function prototypes -----------------------------------------------*/
static void TransferError(DMA2D_HandleTypeDef* dma2dHandle);
static void TransferComplete(DMA2D_HandleTypeDef* dma2dHandle);
static void SystemClock_Config(void);
static void Error_Handler(void);
/* Private functions ---------------------------------------------------------*/
ErrorStatus Buffercmp(uint32_t* pBuffer1, uint32_t* pBuffer2, uint32_t BufferLength);

/**
  * @brief  Main program
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

  /* Configure LED1, LED2, LED3 and LED4 */
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED2);
  BSP_LED_Init(LED3);
  BSP_LED_Init(LED4);

  /*##-1- Configure the DMA2D Mode, Output Color Mode and output offset #############*/
  Dma2dHandle.Init.Mode         = DMA2D_M2M; /* Mode Memory To Memory */
  Dma2dHandle.Init.ColorMode    = DMA2D_ARGB4444; /* Output color mode is ARGB4444 : 16 bpp */
  Dma2dHandle.Init.OutputOffset = 0x0; /* No offset on output */

  /*##-2- DMA2D Callbacks Configuration ######################################*/
  Dma2dHandle.XferCpltCallback  = TransferComplete;
  Dma2dHandle.XferErrorCallback = TransferError;
  
  /*##-3- Foreground Layer Configuration ###########################################*/
  Dma2dHandle.LayerCfg[1].AlphaMode = DMA2D_NO_MODIF_ALPHA; /* Keep original Alpha from ARGB4444 input */
  Dma2dHandle.LayerCfg[1].InputAlpha = 0xFF; /* Fully opaque */
  Dma2dHandle.LayerCfg[1].InputColorMode = DMA2D_INPUT_ARGB4444; /* Input color is ARGB4444 : 16 bpp */
  Dma2dHandle.LayerCfg[1].InputOffset = 0x0; /* No offset in input */

  Dma2dHandle.Instance = DMA2D;

  /*##-4- DMA2D Initialisation ###############################################*/
  if(HAL_DMA2D_Init(&Dma2dHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /* Configure Foreground layer (ie layer 1) */
  if(HAL_DMA2D_ConfigLayer(&Dma2dHandle, 1) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /*##-5- Start DMA2D transfer ###############################################*/
  if(HAL_DMA2D_Start_IT(&Dma2dHandle, 
                        (uint32_t)&aBufferInput,  /* Source memory buffer      */ 
                        (uint32_t)&aBufferResult, /* Destination memory buffer */
                        LAYER_SIZE_X,             /* Width of the 2D memory transfer in pixels */
                        LAYER_SIZE_Y)             /* Height of the 2D memory transfer in lines */
      != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /*##-6- Wait for the end of the transfer ###################################*/
  /*  Before starting a new transfer, you need to check the current state of
      the peripheral; if it’s busy you need to wait for the end of current
      transfer before starting a new one.
      For simplicity reasons, this example is just waiting till the end of the
      transfer, but application may perform other tasks while transfer operation
      is ongoing. */
  while (HAL_DMA2D_GetState(&Dma2dHandle) != HAL_DMA2D_STATE_READY)
  {
  }

  /*##-7- Check transfer correctness #########################################*/
  if(Buffercmp(aBufferInput, 
               aBufferResult, 
               ((LAYER_SIZE_X * LAYER_SIZE_Y * LAYER_NB_BYTES_PER_PIXEL) / 4)) == ERROR)
  {
    /* KO */
    /* Turn on LED2 */
    BSP_LED_On(LED2);
  }
  else
  {
    /* OK */
    /* Turn on LED4 */
    BSP_LED_On(LED4);
  }

  while (1) { ; } 
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

/**
  * @brief  DMA2D Transfer completed callback
  * @param  hdma2d: DMA2D handle.
  * @note   This example shows a simple way to report end of DMA2D transfer, and
  *         you can add your own implementation.
  * @retval None
  */
static void TransferComplete(DMA2D_HandleTypeDef *hdma2d)
{
  /* Turn LED1 on */
  BSP_LED_On(LED1);
}

/**
  * @brief  DMA2D error callbacks
  * @param  hdma2d: DMA2D handle
  * @note   This example shows a simple way to report DMA2D transfer error, and you can
  *         add your own implementation.
  * @retval None
  */
static void TransferError(DMA2D_HandleTypeDef *hdma2d)
{
  /* Turn LED2 on */
  BSP_LED_On(LED2);
}

/**
  * @brief  Compares two buffers.
  * @param  pBuffer1, pBuffer2: buffers to be compared.
  * @param  BufferLength: buffer's length.
  * @retval  0: pBuffer1 identical to pBuffer2
  *          1: pBuffer1 differs from pBuffer2
  */
ErrorStatus Buffercmp(uint32_t* pBuffer1, uint32_t* pBuffer2, uint32_t BufferLength)
{
  while (BufferLength--)
  {
    if (*pBuffer1 != *pBuffer2)
    {
      return ERROR;
    }
    pBuffer1++;
    pBuffer2++;
  }
  return SUCCESS;
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
