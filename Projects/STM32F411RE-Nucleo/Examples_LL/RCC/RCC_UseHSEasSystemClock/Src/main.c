/**
  ******************************************************************************
  * @file    Examples_LL/RCC/RCC_UseHSEasSystemClock/Src/main.c
  * @author  MCD Application Team
  * @brief   This example describes how to use the RCC LL API how to start the HSE 
  *          and use it as system clock.
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

/** @addtogroup RCC_UseHSEasSystemClock
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#if (USE_TIMEOUT == 1)
#define TIMEOUT_VALUE    1000 /* Time-out set to 1 sec */
#endif /* USE_TIMEOUT */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void     StartHSE(void);
uint32_t RCC_WaitForHSEReady(void);
void     LED_Init(void);
void     LED_On(void);
void     LED_Blinking(uint32_t Period);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  register uint32_t frequency = 0;
  
  /* Configure Systick to 1 ms with the current frequency which should be HSI */
  frequency = HSI_VALUE;
									   
  LL_Init1msTick(frequency);

  /* Initialize LED2 */
  LED_Init();

  /* Start HSE */
  StartHSE();

  if (RCC_WaitForHSEReady() == RCC_ERROR_NONE)
  {
    /* Turn-on LED2 to indicate that HSE is ready */
    LED_On();
  }
  else
  {
    /* Problem to switch to HSE, blink LED2 */
     LED_Blinking(LED_BLINK_ERROR);
  }
  
  /* Infinite loop */
  while (1)
  {
  }
}

/**
  * Brief   This function enables the interruption HSE ready,
  *         and start the HSE as external clock.
  * @param  None
  * Retval  None
  */
void StartHSE(void)
{
  /* Configure NVIC for RCC */
  NVIC_EnableIRQ(RCC_IRQn); 
  NVIC_SetPriority(RCC_IRQn,0); 
  
  /* Enable interrupt on HSE ready */
  /* Enable the CSS 
     Enable the HSE and set HSEBYP to use the external clock 
     instead of an oscillator 
     Enable HSE */
  /* Note : the clock is switched to HSE in the RCC_IRQHandler ISR */
  LL_RCC_EnableIT_HSERDY(); 
  LL_RCC_HSE_EnableCSS(); 
  LL_RCC_HSE_EnableBypass(); 
  LL_RCC_HSE_Enable(); 
}

/**
  * @brief  Wait for HSE ready
  * @param  None
  * @retval RCC_ERROR_NONE if no error
  */
uint32_t RCC_WaitForHSEReady()
{
#if (USE_TIMEOUT == 1)
  /* Set timeout to 1 sec */
  uint32_t timeout = TIMEOUT_VALUE;
#endif /* USE_TIMEOUT */
  
  /* Check that the condition is met */
  while (LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_HSE)
  {
#if (USE_TIMEOUT == 1)
    /* Check Systick counter flag to decrement the time-out value */
    if (LL_SYSTICK_IsActiveCounterFlag()) 
    { 
      if(--timeout == 0)
      {
        /* Time-out occurred. Return an error */
        return RCC_ERROR_TIMEOUT;
      }
    } 
#endif /* USE_TIMEOUT */
  }
  return RCC_ERROR_NONE;
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
    /* Error if LED2 is slowly blinking (1 sec. period) */
    LL_GPIO_TogglePin(LED2_GPIO_PORT, LED2_PIN);  
    LL_mDelay(Period);
  }
}

/******************************************************************************/
/*   USER IRQ HANDLER TREATMENT                                               */
/******************************************************************************/
/**
  * @brief  This function handles the HSE ready detection (called in RCC_IRQHandler)
  * @param  None
  * @retval None
  */
void HSEReady_Callback(void)
{
  /* Switch the system clock to HSE */
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSE); 
  
  /* 1ms config with HSE 8MHz*/
  LL_Init1msTick(HSE_VALUE);
}

/**
  * @brief  This function handles failure detected on the HSE clock (called in NMI_Handler)
  * @param  None
  * @retval None
  */
void HSEFailureDetection_Callback(void)
{
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
