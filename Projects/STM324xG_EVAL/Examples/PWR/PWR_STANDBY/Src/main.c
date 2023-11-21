/**
  ******************************************************************************
  * @file    PWR/PWR_STANDBY/Src/main.c 
  * @author  MCD Application Team
  * @brief   This sample code shows how to use STM32F4xx PWR HAL API to enter
  *          and exit the Standby mode.
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

/** @addtogroup PWR_STANDBY
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define LED_TOGGLE_DELAY         100

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* RTC handler declaration */
RTC_HandleTypeDef RTCHandle;
RTC_TimeTypeDef RTC_TimeStructure;
RTC_DateTypeDef RTC_DateStructure;
RTC_AlarmTypeDef RTC_AlarmStructure;
static __IO uint32_t TimingDelay;

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void RTC_Config(void);

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

  /* Configure LED1, LED2 */
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED2);

  /* Configure the system clock to 168 MHz */
  SystemClock_Config();

  /* Enable Power Clock */
  __HAL_RCC_PWR_CLK_ENABLE();

  /* Check and Clear the Wakeup flag */
  if (__HAL_PWR_GET_FLAG(PWR_FLAG_WU) != RESET)
  {
    __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);
  }

  /* Initialize the Key push-button to generate external interrupts */
  BSP_PB_Init(BUTTON_KEY, BUTTON_MODE_EXTI);

  /* RTC configuration */
  RTC_Config();

  /* Turn on LED1 */
  BSP_LED_On(LED1);

  /* Enable WakeUp Pin PWR_WAKEUP_PIN1 connected to PA.00 */
  HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN1);

  while (1)
  {
  }
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 168000000
  *            HCLK(Hz)                       = 168000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 4
  *            APB2 Prescaler                 = 2
  *            HSE Frequency(Hz)              = 25000000
  *            PLL_M                          = 25
  *            PLL_N                          = 336
  *            PLL_P                          = 2
  *            PLL_Q                          = 7
  *            VDD(V)                         = 3.3
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
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;  
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }

  /* STM32F405x/407x/415x/417x Revision Z and upper devices: prefetch is supported  */
  if (HAL_GetREVID() >= 0x1001)
  {
    /* Enable the Flash prefetch */
    __HAL_FLASH_PREFETCH_BUFFER_ENABLE();
  }
}

/**
  * @brief  Configures the RTC.
  * @param  None
  * @retval None
  */
static void RTC_Config(void)
{ 
  /* Enable Power Clock*/
  __HAL_RCC_PWR_CLK_ENABLE();
  
  /* Allow Access to RTC Backup domaine */
  HAL_PWR_EnableBkUpAccess();
  
  RTCHandle.Instance = RTC;

  /* Check if the system was resumed from StandBy mode */
  if(__HAL_PWR_GET_FLAG(PWR_FLAG_SB) != RESET)
  {
    /* Clear StandBy flag */
    __HAL_PWR_CLEAR_FLAG(PWR_FLAG_SB);

    /* Disable the write protection for RTC registers */
    __HAL_RTC_WRITEPROTECTION_DISABLE(&RTCHandle);

    /* Wait for RTC APB registers synchronisation (needed after start-up from Reset)*/
    if (HAL_RTC_WaitForSynchro(&RTCHandle) != HAL_OK)
    {
      /* Initialization Error */
      Error_Handler();
    }

    /* Enable the write protection for RTC registers */
    __HAL_RTC_WRITEPROTECTION_ENABLE(&RTCHandle);
    /* No need to configure the RTC as the RTC config(clock source, enable,
    prescaler,...) are kept after wake-up from STANDBY */
  }
  else
  {
    /* Reset Backup Domaine */
    __HAL_RCC_BACKUPRESET_FORCE();
    __HAL_RCC_BACKUPRESET_RELEASE();

    /* Set the RTC time base to 1s */
    /* Configure RTC prescaler and RTC data registers as follows:
    - Hour Format = Format 24
    - Asynch Prediv = Value according to source clock
    - Synch Prediv = Value according to source clock
    - OutPut = Output Disable
    - OutPutPolarity = High Polarity
    - OutPutType = Open Drain */
    RTCHandle.Init.HourFormat = RTC_HOURFORMAT_24;
    RTCHandle.Init.AsynchPrediv = RTC_ASYNCH_PREDIV;
    RTCHandle.Init.SynchPrediv = RTC_SYNCH_PREDIV;
    RTCHandle.Init.OutPut = RTC_OUTPUT_DISABLE;
    RTCHandle.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
    RTCHandle.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
    if (HAL_RTC_Init(&RTCHandle) != HAL_OK)
    {
      /* Initialization Error */
      Error_Handler();
    }

    /* Set the time to 01h 00mn 00s AM */
    RTC_TimeStructure.TimeFormat = RTC_HOURFORMAT12_AM;
    RTC_TimeStructure.Hours = 0x01;
    RTC_TimeStructure.Minutes = 0x00;
    RTC_TimeStructure.Seconds = 0x00;
    if (HAL_RTC_SetTime(&RTCHandle, &RTC_TimeStructure, RTC_FORMAT_BCD) == HAL_ERROR)
    {
      /* Initialization Error */
      Error_Handler();
    }
  }
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
  /* Turn LED2 on */
  BSP_LED_On(LED2);
  while(1)
  {
  }
}

/**
  * @brief  SYSTICK callback
  * @param  None
  * @retval None
  */
void HAL_SYSTICK_Callback(void)
{
  HAL_IncTick();
  if (TimingDelay != 0)
  { 
    TimingDelay--;
  }
  else
  {
    /* Toggle LED1 */
    BSP_LED_Toggle(LED1);
    TimingDelay = LED_TOGGLE_DELAY;
  }
}

/**
  * @brief  EXTI line detection callbacks
  * @param  GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if(GPIO_Pin == KEY_BUTTON_PIN)
  {  
    HAL_RTC_GetTime(&RTCHandle, &RTC_TimeStructure, RTC_FORMAT_BIN);
    HAL_RTC_GetDate(&RTCHandle, &RTC_DateStructure, RTC_FORMAT_BIN);
    
    /* Set the alarm to current time + 5s */
    RTC_AlarmStructure.Alarm  = RTC_ALARM_A;
    RTC_AlarmStructure.AlarmTime.TimeFormat = RTC_TimeStructure.TimeFormat;
    RTC_AlarmStructure.AlarmTime.Hours = RTC_TimeStructure.Hours;
    RTC_AlarmStructure.AlarmTime.Minutes = RTC_TimeStructure.Minutes;
    RTC_AlarmStructure.AlarmTime.Seconds = (RTC_TimeStructure.Seconds + 0x05) % 60;
    RTC_AlarmStructure.AlarmDateWeekDay = 0x31;
    RTC_AlarmStructure.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_DATE;
    RTC_AlarmStructure.AlarmMask = RTC_ALARMMASK_DATEWEEKDAY | RTC_ALARMMASK_HOURS | RTC_ALARMMASK_MINUTES;
    RTC_AlarmStructure.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_NONE;
    
    /* The Following Wakeup sequence is highly recommended prior to each Standby
       mode entry mainly  when using more than one wakeup source this is to not 
       miss any wakeup event:
       - Disable all used wakeup sources,
       - Clear all related wakeup flags,
       - Re-enable all used wakeup sources,
       - Enter the Standby mode.
    */

    /*## Disable all used wakeup sources #####################################*/
    /* Disable Wake-up timer */
    HAL_PWR_DisableWakeUpPin(PWR_WAKEUP_PIN1);
    
    /* Disable RTC Alarm */
    HAL_RTC_DeactivateAlarm(&RTCHandle, RTC_ALARM_A);

    /*## Clear all related wakeup flags ######################################*/
    /* Clear PWR wake up Flag */
    __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);
    
    /* Clear the Alarm Flag */
    __HAL_RTC_ALARM_CLEAR_FLAG(&RTCHandle, RTC_FLAG_ALRAF);

    /*## Re-enable all used wakeup sources ###################################*/
    /* Set RTC alarm */
    if(HAL_RTC_SetAlarm_IT(&RTCHandle, &RTC_AlarmStructure, RTC_FORMAT_BIN) != HAL_OK) 
    {
      /* Initialization Error */
      Error_Handler();
    }

    /* Enable WKUP pin */
    HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN1);

    /* Turn LED1 off */
    BSP_LED_Off(LED1);

    /*## Enter Standby Mode ##################################################*/
    HAL_PWR_EnterSTANDBYMode();
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
