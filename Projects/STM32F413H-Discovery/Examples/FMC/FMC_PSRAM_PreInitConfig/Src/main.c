/**
  ******************************************************************************
  * @file    FMC/FMC_PSRAM_PreInitConfig/Src/main.c
  * @author  MCD Application Team
  * @brief   This example describes how to configure and use QuadSPI through
  *          the STM32F4xx HAL API.
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

/** @addtogroup FMC_PSRAM_PreInitConfig
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#if defined(__ICCARM__)
#pragma section =".psram"
#endif

/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void GpioToggle(void);

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
  
  /* Configure LEDs */
  BSP_LED_Init(LED4);
  BSP_LED_Init(LED3);
  
  while(1)
  {
    /* Execute the code from PSRAM memory ------------------------------------ */
    GpioToggle();
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
  * @brief  Toggle the GPIOs
  * @param  None
  * @retval None
  */
#if defined(__CC_ARM)
 #pragma arm section code = ".psram"
 #pragma no_inline
 static void GpioToggle(void)
#elif defined(__ICCARM__)
 static void GpioToggle(void) @ ".psram"
#elif defined(__GNUC__)
 static void __attribute__((section(".psram"), noinline)) GpioToggle(void)
#endif
{
  BSP_LED_Toggle(LED4);
  /* Insert delay 200 ms */
  HAL_Delay(200);
  BSP_LED_Toggle(LED3);
  /* Insert delay 200 ms */
  HAL_Delay(200);
}
#if defined(__CC_ARM)
#pragma arm section code
#endif

/**
  * @}
  */

/**
  * @}
  */
