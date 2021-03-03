/**
  ******************************************************************************
  * @file    rtc.c
  * @author  MCD Application Team
  * @brief   <Add here what does this driver do>
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "rtc.h"

/** @addtogroup MAIN_GROUP< OPTIONAL >
  * @{
  */

/** @defgroup SUB_GROUP< OPTIONAL >
  * @brief 
  * @{
  */

/* External variables --------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
#define RTC_ASYNCH_PREDIV  0x7F   /* LSE as RTC clock */
#define RTC_SYNCH_PREDIV   0x00FF /* LSE as RTC clock */
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
RTC_HandleTypeDef RtcHandle;
/**
  * @brief  Configure the current time and date.
  * @param  None
  * @retval None
  */
void RTC_Init(void)
{
  
  /*##-1- Configure the RTC peripheral #######################################*/
  /* Configure RTC prescaler and RTC data registers */
  /* RTC configured as follow:
  - Hour Format    = Format 24
  - Asynch Prediv  = Value according to source clock
  - Synch Prediv   = Value according to source clock
  - OutPut         = Output Disable
  - OutPutPolarity = High Polarity
  - OutPutType     = Open Drain */ 
  RtcHandle.Instance = RTC;
  RtcHandle.Init.HourFormat = RTC_HOURFORMAT_24;
  RtcHandle.Init.AsynchPrediv = RTC_ASYNCH_PREDIV;
  RtcHandle.Init.SynchPrediv = RTC_SYNCH_PREDIV;
  RtcHandle.Init.OutPut = RTC_OUTPUT_DISABLE;
  RtcHandle.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  RtcHandle.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  
  if(HAL_RTC_Init(&RtcHandle) != HAL_OK)
  {
  }
}

/**
  * @brief RTC MSP Initialization 
  *        This function configures the hardware resources used in This application: 
  *           - Peripheral's clock enable
  * @param hrtc: RTC handle pointer
  *
  * @note  Care must be taken when HAL_RCCEx_PeriphCLKConfig() is used to select 
  *        the RTC clock source; in this case the Backup domain will be reset in  
  *        order to modify the RTC Clock source, as consequence RTC registers (including 
  *        the backup registers) and RCC_BDCR register are set to their reset values.
  *             
  * @retval None
  */
void HAL_RTC_MspInit(RTC_HandleTypeDef *hrtc)
{
  RCC_OscInitTypeDef       Osc;
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct;
  
  /*##-1- Configue LSE as RTC clock soucre ###################################*/ 
  Osc.OscillatorType = RCC_OSCILLATORTYPE_LSE;
  Osc.LSEState = RCC_LSE_ON;
  Osc.HSIState = RCC_HSI_OFF;
  Osc.HSICalibrationValue = 0;
  Osc.LSIState = RCC_LSI_OFF;
  Osc.PLL.PLLState = RCC_PLL_NONE;
  HAL_RCC_OscConfig(&Osc);
  
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
  HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);
  
  /*##-2- Enable RTC peripheral Clocks #######################################*/ 
  /* Enable RTC Clock */ 
  __HAL_RCC_RTC_ENABLE(); 
}

/**
  * @brief RTC MSP De-Initialization 
  *        This function frees the hardware resources used in This application:
  *          - Disable the Peripheral's clock
  * @param hrtc: RTC handle pointer
  * @retval None
  */
void HAL_RTC_MspDeInit(RTC_HandleTypeDef *hrtc)
{
  /*##-1- Reset peripherals ##################################################*/
   __HAL_RCC_RTC_DISABLE();
}


/**
  * @brief RTC MSP De-Initialization 
  *        This function frees the hardware resources used in This application:
  *          - Disable the Peripheral's clock
  * @param hrtc: RTC handle pointer
  * @retval None
  */
void BACKUP_SaveParameter(uint32_t address, uint32_t data)
{
  HAL_RTCEx_BKUPWrite(&RtcHandle,address,data);  
}

/**
  * @brief RTC MSP De-Initialization 
  *        This function frees the hardware resources used in This application:
  *          - Disable the Peripheral's clock
  * @param hrtc: RTC handle pointer
  * @retval None
  */
uint32_t BACKUP_RestoreParameter(uint32_t address)
{
   return HAL_RTCEx_BKUPRead(&RtcHandle,address);  
}
/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
