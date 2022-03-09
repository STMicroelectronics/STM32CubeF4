/**
  ******************************************************************************
  * @file    USB_Host/FWupgrade_Standalone/Inc/command.h
  * @author  MCD Application Team
  * @brief   Header file for command.c
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
#ifndef __COMMAND_H
#define __COMMAND_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "flash_if.h"
#include "main.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void COMMAND_Upload(void);
void COMMAND_Download(void);
void COMMAND_Jump(void);

#ifdef __cplusplus
}
#endif

#endif  /* __COMMAND_H */
