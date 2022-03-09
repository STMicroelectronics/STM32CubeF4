/**
  ******************************************************************************
  * @file    k_log.h
  * @author  MCD Application Team
  * @brief   Header for k_log.c file
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
