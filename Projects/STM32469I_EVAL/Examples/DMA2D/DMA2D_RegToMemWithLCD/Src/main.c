/**
 ******************************************************************************
 * @file    DMA2D/DMA2D_RegToMemWithLCD/Src/main.c
 * @author  MCD Application Team
 * @brief   This example provides a description of how to configure DMA2D peripheral in
 *          Register to Memory transfer mode and display the result on LCD.
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

/** @addtogroup DMA2D_RegToMemWithLCD
 * @{
 */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

DMA2D_HandleTypeDef Dma2dHandle;
extern LTDC_HandleTypeDef  hltdc_eval;
/* DMA2D output buffer to be displayed on LCD */
uint32_t aBufferResult[(LAYER_SIZE_X * LAYER_SIZE_Y * ARGB4444_BYTE_PER_PIXEL) / 4];

/* Private function prototypes -----------------------------------------------*/
static void DMA2D_Config(void);
static void TransferError(DMA2D_HandleTypeDef* Dma2dHandle);
static void TransferComplete(DMA2D_HandleTypeDef* Dma2dHandle);
static void SystemClock_Config(void);
static void OnError_Handler(uint32_t condition);
static void LCD_LayerDefaultInit(uint16_t LayerIndex, uint32_t FB_Address);

/* Private functions ---------------------------------------------------------*/

/**
 * @brief   Main program
 * @param  None
 * @retval None
 */
int main(void)
{
  HAL_StatusTypeDef hal_status = HAL_OK;
  uint8_t  lcd_status = LCD_OK;

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

  /* Configure the system clock */
  SystemClock_Config();

  /* Wait until MFX is ready after reset */
  HAL_Delay(100);

  /* Configure LED1, LED2 and LED3 */
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED2);
  BSP_LED_Init(LED3);

  /*##-1- Initialize the SDRAM  ##############################################*/
  BSP_SDRAM_Init();

  /*##-2- Initialize the LCD #################################################*/

  /* Proceed to LTDC, DSI initialization and LCD screen initialization
   * with the configuration filled in above */
  lcd_status = BSP_LCD_Init();
  LCD_LayerDefaultInit(1, (uint32_t)&aBufferResult);
  BSP_LCD_SelectLayer(1);
  
  OnError_Handler(lcd_status != LCD_OK);

  HAL_Delay(100);

  /*##-3- DMA2D configuration ################################################*/
  DMA2D_Config();

  /*##-4- Start DMA2D transfer ###############################################*/  
  hal_status = HAL_DMA2D_Start_IT(&Dma2dHandle,
      (uint32_t)LCD_COLOR_GREEN, /* Fill the DMA2D output register with this color */
      (uint32_t)&aBufferResult,    /* DMA2D output register */
      LAYER_SIZE_X,
      LAYER_SIZE_Y);
  OnError_Handler(hal_status != HAL_OK);

  while (1)
  {
    ;
  }
}

static void LCD_LayerDefaultInit(uint16_t LayerIndex, uint32_t FB_Address)
{
  LCD_LayerCfgTypeDef  Layercfg;
  
  Layercfg.FBStartAdress = (uint32_t)&aBufferResult;
  Layercfg.Alpha = 255;
  Layercfg.Alpha0 = 0;
  Layercfg.Backcolor.Blue = 0;
  Layercfg.Backcolor.Green = 0;
  Layercfg.Backcolor.Red = 0;
  Layercfg.BlendingFactor1 = LTDC_BLENDING_FACTOR1_PAxCA;
  Layercfg.BlendingFactor2 = LTDC_BLENDING_FACTOR2_PAxCA;
  Layercfg.ImageWidth = 200;
  Layercfg.ImageHeight = 200;
  
    /* Layer Init */
  Layercfg.WindowX0 = (BSP_LCD_GetXSize()/2) - (Layercfg.ImageWidth/2) ;
  Layercfg.WindowX1 = Layercfg.WindowX0 + Layercfg.ImageWidth;
  Layercfg.WindowY0 = (BSP_LCD_GetYSize()/2) - (Layercfg.ImageHeight/2) ;
  Layercfg.WindowY1 = Layercfg.WindowY0 + Layercfg.ImageHeight;
  Layercfg.PixelFormat = LTDC_PIXEL_FORMAT_ARGB8888;
  
  HAL_LTDC_ConfigLayer(&hltdc_eval, &Layercfg, LayerIndex);
}

/**
 * @brief DMA2D configuration.
 * @note  This function Configure the DMA2D peripheral :
 *        1) Configure the transfer mode : register to memory
 *        2) Configure the color mode to ARGB8888
 * @retval
 *  None
 */

static void DMA2D_Config(void)
{
  HAL_StatusTypeDef hal_status = HAL_OK;

  /* Register to memory mode with ARGB8888 as colorMode */  
  Dma2dHandle.Init.Mode         = DMA2D_R2M;
  Dma2dHandle.Init.ColorMode    = DMA2D_ARGB8888;
  Dma2dHandle.Init.OutputOffset = 0x0;     

  Dma2dHandle.XferCpltCallback  = TransferComplete;
  Dma2dHandle.XferErrorCallback = TransferError;
  Dma2dHandle.Instance          = DMA2D;

  /* DMA2D Initialization */
  hal_status = HAL_DMA2D_Init(&Dma2dHandle);
  OnError_Handler(hal_status != HAL_OK);

  hal_status = HAL_DMA2D_ConfigLayer(&Dma2dHandle, 1);
  OnError_Handler(hal_status != HAL_OK);
}

/**
 * @brief  On Error Handler on condition TRUE.
 * @param  condition : Can be TRUE or FALSE
 * @retval None
 */
static void OnError_Handler(uint32_t condition)
{
  if(condition)
  {
    BSP_LED_On(LED3);
    while(1) { ; } /* Blocking on error */
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
  /* Turn LED1 On */
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
  /* Turn LED2 On */
  BSP_LED_On(LED2);
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

#ifdef USE_FULL_ASSERT

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
