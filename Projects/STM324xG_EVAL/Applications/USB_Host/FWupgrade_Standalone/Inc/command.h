/**
  ******************************************************************************
  * @file    USB_Host/FWupgrade_Standalone/Inc/command.h
  * @author  MCD Application Team
  * @brief   Header file for command.c
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
