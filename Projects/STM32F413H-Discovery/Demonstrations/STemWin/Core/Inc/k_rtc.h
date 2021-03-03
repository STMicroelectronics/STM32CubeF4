/**
  ******************************************************************************
  * @file    k_rtc.h
  * @author  MCD Application Team
  * @brief   Header for k_rtc.c file
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __K_RTC_H
#define __K_RTC_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */ 
void     k_CalendarBkupInit(void);
void     k_BkupSaveParameter(uint32_t address, uint32_t data);
uint32_t k_BkupRestoreParameter(uint32_t address);

void k_SetTime  (RTC_TimeTypeDef *Time);
void k_GetTime  (RTC_TimeTypeDef *Time);
void k_SetDate  (RTC_DateTypeDef *Date);
void k_GetDate  (RTC_DateTypeDef *Date);

#ifdef __cplusplus
}
#endif

#endif /*__K_RTC_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
