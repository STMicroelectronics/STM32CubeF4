/**
  ******************************************************************************
  * @file    RTC.h
  * @author  MCD Application Team
  * @brief   Header for RTC.c module
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
