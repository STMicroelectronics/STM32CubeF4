/**
  ******************************************************************************
  * @file    BSP/Inc/mems.h 
  * @author  MCD Application Team
  * @brief   Header for mems.c module
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
#ifndef __MEMS_H
#define __MEMS_H

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
#define ABS(x)         (x < 0) ? (-x) : x

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void ACCELERO_MEMS_Test(void);
void Audio_Test(void);
#endif /* __MEMS_H */
