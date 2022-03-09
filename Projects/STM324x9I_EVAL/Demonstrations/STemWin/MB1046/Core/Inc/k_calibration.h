/**
  ******************************************************************************
  * @file    k_calibration.h
  * @author  MCD Application Team
  * @brief   Header for k_calibration.c file
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
