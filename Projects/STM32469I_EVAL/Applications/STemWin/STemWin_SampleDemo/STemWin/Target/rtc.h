/**
  ******************************************************************************
  * @file    RTC.h
  * @author  MCD Application Team
  * @brief   Header for RTC.c module
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
#ifndef __RTC_H
#define __RTC_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "DIALOG.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
   
extern  RTC_HandleTypeDef RtcHandle;
/* Exported functions ------------------------------------------------------- */ 
void     RTC_Init(void);
void     BACKUP_SaveParameter(uint32_t address, uint32_t data);
uint32_t BACKUP_RestoreParameter(uint32_t address);
#ifdef __cplusplus
}
#endif

#endif /*__RTC_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
