/**
  ******************************************************************************
  * @file    k_calibration.h
  * @author  MCD Application Team
  * @brief   Header for k_calibration.c file
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
#ifndef __K_CALIBRATION_H
#define __K_CALIBRATION_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */ 
void     k_CalibrationInit(void);
uint8_t  k_CalibrationIsDone(void);
uint16_t k_CalibrationGetX(uint16_t x);
uint16_t k_CalibrationGetY(uint16_t y);

#ifdef __cplusplus
}
#endif

#endif /*__K_CALIBRATION_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
