/**
  ******************************************************************************
  * @file    HASH/HASH_SHA224SHA256_DMA/Src/main.c
  * @author  MCD Application Team
  * @brief   This example provides a short description of HASH digest calculation
  *          using SHA224 and SHA256 example.
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

/** @addtogroup HASH_SHA224SHA256_DMA
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* HASH handler declaration */
HASH_HandleTypeDef     HashHandle;

__ALIGN_BEGIN const uint8_t aInput[] __ALIGN_END = "The STM32F4xx series is the result of a perfect symbiosis of the real-time control capabilities of an MCU and the signal processing performance of a DSP, and thus complements the STM32 portfolio with a new class of devices, digital signal controllers (DSC).";

__ALIGN_BEGIN static uint8_t aSHA224Digest[28] __ALIGN_END;
__ALIGN_BEGIN static uint8_t aExpectSHA224Digest[28] __ALIGN_END = {0xd1, 0x7a, 0x28, 0x59, 0xd4, 0x9f, 0x69, 0x9f,
                                                                    0x3c, 0x58, 0xfe, 0x67, 0x16, 0x2f, 0xb9, 0xe0,
                                                                    0xd4, 0x9d, 0xb8, 0x10, 0xbd, 0x75, 0x58, 0x9f,
                                                                    0x7f, 0x32, 0x7e, 0xcc
                                                                    };
__ALIGN_BEGIN static uint8_t aSHA256Digest[32] __ALIGN_END;
__ALIGN_BEGIN static uint8_t aExpectSHA256Digest[32] __ALIGN_END = {0x35, 0x16, 0x2f, 0x40, 0x9c, 0x32, 0xfd, 0x6e,
                                                                    0xe9, 0x6d, 0xb2, 0x98, 0x7e, 0xc2, 0x2f, 0x77,
                                                                    0x43, 0xec, 0x6c, 0xd8, 0xb7, 0x78, 0x44, 0xca,
                                                                    0xeb, 0xed, 0x32, 0xda, 0x49, 0xa1, 0xe3, 0x9c
                                                                    };
/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void Error_Handler(void);

/* Private functions ---------------------------------------------------------*/

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

  /* Configure LED1, LED3 and LED4 */
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED3);
  BSP_LED_Init(LED4);

  /****************************************************************************/
  /****************************** SHA224 **************************************/
  /****************************************************************************/
  HAL_HASH_DeInit(&HashHandle);
  HashHandle.Init.DataType = HASH_DATATYPE_8B;

  if (HAL_HASH_Init(&HashHandle) != HAL_OK)
  {
    Error_Handler();
  }

  /* Start HASH computation using DMA transfer */
  if (HAL_HASHEx_SHA224_Start_DMA(&HashHandle, (uint8_t*)aInput, strlen((char const*)aInput)) != HAL_OK)
  {
    Error_Handler();
  }

  /* Get the computed digest value */
  if(HAL_HASHEx_SHA224_Finish(&HashHandle, aSHA224Digest, 0xFF) != HAL_OK)
  {
    Error_Handler();
  }

  /* Compare computed digest with expected one */
  if(memcmp(aSHA224Digest, aExpectSHA224Digest, sizeof(aExpectSHA224Digest)/sizeof(aExpectSHA224Digest[0])) != 0)
  {
    Error_Handler();
  }
  else
  {
    BSP_LED_On(LED1);
  }

  /****************************************************************************/
  /***************************** SHA256 ***************************************/
  /****************************************************************************/
  HAL_HASH_DeInit(&HashHandle);
  HashHandle.Init.DataType = HASH_DATATYPE_8B;
  
  if(HAL_HASH_Init(&HashHandle) != HAL_OK)
  {
    Error_Handler();
  }
  /* Start HASH computation using DMA transfer */
  if (HAL_HASHEx_SHA256_Start_DMA(&HashHandle, (uint8_t*)aInput, strlen((char const*)aInput)) != HAL_OK)
  {
    Error_Handler();
  }

  /* Get the computed digest value */
  if (HAL_HASHEx_SHA256_Finish(&HashHandle, aSHA256Digest, 0xFF) != HAL_OK)
  {
    Error_Handler();
  }
  
  /* Compare computed digest with expected one */
  if(memcmp(aSHA256Digest, aExpectSHA256Digest, sizeof(aExpectSHA256Digest)/sizeof(aExpectSHA256Digest[0])) != 0)
  {
    Error_Handler();
  }
  else
  {
    BSP_LED_On(LED4);
  }
  while(1)
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
