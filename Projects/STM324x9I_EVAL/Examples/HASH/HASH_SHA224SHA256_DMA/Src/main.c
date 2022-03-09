/**
  ******************************************************************************
  * @file    HASH/HASH_SHA224SHA256_DMA/Src/main.c 
  * @author  MCD Application Team
  * @brief   This example provides a short description of HASH digest calculation
  *          using SHA1 and MD5 example.
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

__ALIGN_BEGIN const uint8_t aInput[] __ALIGN_END = "The STM32 F4 series is the result of a perfect symbiosis of the real-time control capabilities of an MCU and the signal processing performance of a DSP, and thus complements the STM32 portfolio with a new class of devices, digital signal controllers (DSC).";

__ALIGN_BEGIN static uint8_t aSHA224Digest[28] __ALIGN_END;
__ALIGN_BEGIN static uint8_t aExpectSHA224Digest[28] __ALIGN_END = {0x0f, 0x74, 0x43, 0x8f, 0x7d, 0x8f, 0x5c, 0x68,
                                                                    0x54, 0xe5, 0x85, 0xe0, 0xe7, 0x9d, 0x57, 0x4c,
                                                                    0x0d, 0xd7, 0xfc, 0x0b, 0xfc, 0x76, 0xcc, 0x94,
                                                                    0xd9, 0xda, 0xb8, 0xaa};
__ALIGN_BEGIN static uint8_t aSHA256Digest[32] __ALIGN_END;
__ALIGN_BEGIN static uint8_t aExpectSHA256Digest[32] __ALIGN_END = {0xd0, 0x55, 0x16, 0x37, 0x4c, 0x13, 0x11, 0x17,
                                                                    0x44, 0x93, 0x57, 0x3c, 0xa5, 0x9a, 0x54, 0x08,
                                                                    0x6b, 0x2e, 0x45, 0xea, 0x82, 0x02, 0x8e, 0x6a,
                                                                    0x18, 0xe6, 0xc5, 0x40, 0xf7, 0x45, 0xa9, 0xb0};

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
       - Configure the Systick to generate an interrupt each 1 msec
       - Set NVIC Group Priority to 4
       - Global MSP (MCU Support Package) initialization
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
  
  if(HAL_HASH_Init(&HashHandle) != HAL_OK)
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
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  /* Activate the Over-Drive mode */
  HAL_PWREx_EnableOverDrive();
      
  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;  
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);
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
