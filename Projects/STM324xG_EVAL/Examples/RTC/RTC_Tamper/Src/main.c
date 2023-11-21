/**
  ******************************************************************************
  * @file    RTC/RTC_Tamper/Src/main.c 
  * @author  MCD Application Team
  * @brief   This sample code shows how to use STM32F4xx RTC HAL API to write/read 
  *          data to/from RTC Backup data registers and demonstrates the Tamper  
  *          detection feature.
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

/** @addtogroup RTC_Tamper
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* RTC handler declaration */
RTC_HandleTypeDef RtcHandle;

__IO FlagStatus TamperStatus;

/* Backup registers table */
uint32_t aBKPDataReg[BACKUP_COUNT] =
{
  RTC_BKP_DR0,  RTC_BKP_DR1,  RTC_BKP_DR2, 
  RTC_BKP_DR3,  RTC_BKP_DR4,  RTC_BKP_DR5,
  RTC_BKP_DR6,  RTC_BKP_DR7,  RTC_BKP_DR8, 
  RTC_BKP_DR9,  RTC_BKP_DR10, RTC_BKP_DR11, 
  RTC_BKP_DR12, RTC_BKP_DR13, RTC_BKP_DR14, 
  RTC_BKP_DR15, RTC_BKP_DR16, RTC_BKP_DR17, 
  RTC_BKP_DR18, RTC_BKP_DR19
};

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void RTC_TamperConfig(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{ 
  uint32_t index = 0;
  
  /* STM32F4xx HAL library initialization:
       - Configure the Flash prefetch, instruction and Data caches
       - Configure the Systick to generate an interrupt each 1 msec
       - Set NVIC Group Priority to 4
       - Global MSP (MCU Support Package) initialization
     */
  HAL_Init();
  
  /* Configure LED1, LED2, LED3 and LED4 */
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED2);
  BSP_LED_Init(LED3);
  BSP_LED_Init(LED4);

  /* Configure the system clock to 168 MHz */
  SystemClock_Config();

  RtcHandle.Instance = RTC;
  
  /*##-1- Configure the RTC peripheral #######################################*/
  /* Configure RTC prescaler and RTC data registers */
  /* RTC configured as follow:
  - Hour Format    = Format 24
  - Asynch Prediv  = Value according to source clock
  - Synch Prediv   = Value according to source clock
  - OutPut         = Output Disable
  - OutPutPolarity = High Polarity
  - OutPutType     = Open Drain */ 
  RtcHandle.Init.HourFormat = RTC_HOURFORMAT_24;
  RtcHandle.Init.AsynchPrediv = RTC_ASYNCH_PREDIV;
  RtcHandle.Init.SynchPrediv = RTC_SYNCH_PREDIV;
  RtcHandle.Init.OutPut = RTC_OUTPUT_DISABLE;
  RtcHandle.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  RtcHandle.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  __HAL_RTC_RESET_HANDLE_STATE(&RtcHandle);
  if(HAL_RTC_Init(&RtcHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler(); 
  }
        
  /* Clear the Tamper Flag */
  __HAL_RTC_TAMPER_CLEAR_FLAG(&RtcHandle,RTC_FLAG_TAMP1F);
  
  /*##-2- Configure RTC Tamper ###############################################*/
  /* Configure RTC Tamper */
  RTC_TamperConfig();
  
  /*##-3- Write Data on the Back Up registers ################################*/
  for(index = 0; index < BACKUP_COUNT; index++)
  {
    HAL_RTCEx_BKUPWrite(&RtcHandle,aBKPDataReg[index], 0xDF59 + (index * 0x5A));
  }
  
  /*##-4- Check Data is stored on the Back Up registers ######################*/
  for(index = 0; index < BACKUP_COUNT; index++)
  {
    if(HAL_RTCEx_BKUPRead(&RtcHandle, aBKPDataReg[index]) != (0xDF59 + (index * 0x5A)))
    {
      Error_Handler();
    }
  }
  
  /* Turn LED2 on */
  BSP_LED_On(LED2);

  /* Reset flag after writing of backup register in order to wait for new button press */
  TamperStatus = RESET;
  
  /*##-5- Wait for the tamper button is pressed ##############################*/
  while(TamperStatus != SET);
  
  /*##-6- Deactivate the tamper ##############################################*/
  HAL_RTCEx_DeactivateTamper(&RtcHandle, RTC_TAMPER_1);
 
  /*##-7- Check Data is cleared on the Back Up registers #####################*/
  for(index = 0; index < BACKUP_COUNT; index++)
  {
    if(HAL_RTCEx_BKUPRead(&RtcHandle, aBKPDataReg[index]) != 0x00)
    {
      Error_Handler();
    }
  }
  
  /* Infinite loop */  
  while (1)
  {
    /* Toggle LED4 */
    BSP_LED_Toggle(LED4);
    
    /* Delay */
    HAL_Delay(100);
  }
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
  /* Turn LED3 on */
  BSP_LED_On(LED3);
  while(1)
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
  * @brief  Configure the Tamper.
  * @param  None
  * @retval None
  */
static void RTC_TamperConfig(void)
{
  RTC_TamperTypeDef  stamperstructure;

  /* Use PC13 as Tamper 1 with interrupt mode */
  stamperstructure.Filter = RTC_TAMPERFILTER_DISABLE;
  stamperstructure.PinSelection = RTC_TAMPERPIN_PC13;
  stamperstructure.Tamper = RTC_TAMPER_1;
  stamperstructure.Trigger = RTC_TAMPERTRIGGER_FALLINGEDGE;
  stamperstructure.SamplingFrequency = RTC_TAMPERSAMPLINGFREQ_RTCCLK_DIV256;
  stamperstructure.PrechargeDuration = RTC_TAMPERPRECHARGEDURATION_1RTCCLK;
  stamperstructure.TamperPullUp = RTC_TAMPER_PULLUP_DISABLE;
  stamperstructure.TimeStampOnTamperDetection = RTC_TIMESTAMPONTAMPERDETECTION_DISABLE;
      
  if(HAL_RTCEx_SetTamper_IT(&RtcHandle, &stamperstructure) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler(); 
  }   
}

/**
  * @brief  Tamper 1 callback. 
  * @param  hrtc: pointer to a RTC_HandleTypeDef structure that contains
  *               the configuration information for RTC.
  * @retval None
  */
void HAL_RTCEx_Tamper1EventCallback(RTC_HandleTypeDef *hrtc)
{
  /* LED1 On: Tamper button pressed */
  BSP_LED_On(LED1);
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
