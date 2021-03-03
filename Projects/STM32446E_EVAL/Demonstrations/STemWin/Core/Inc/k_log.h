/**
  ******************************************************************************
  * @file    k_log.h
  * @author  MCD Application Team
  * @brief   Header for k_log.c file
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
#ifndef __K_LOG_H
#define __K_LOG_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Exported types ------------------------------------------------------------*/
extern uint8_t  *pLOG_CacheBuffer;  
extern uint32_t LOG_IN_ptr;
/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */ 
void k_LogInit(void);
#ifdef __cplusplus
}
#endif

#endif /*__K_LOG_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
