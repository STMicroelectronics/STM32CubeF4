/**
  ******************************************************************************
  * @file    LibJPEG/LibJPEG_Encoding/Inc/decode.h 
  * @author  MCD Application Team
  * @brief   header of decode.c file
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
#ifndef __DECODE_H
#define __DECODE_H

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

void jpeg_decode(JFILE *file, uint32_t width, uint8_t * buff, uint8_t (*callback)(uint8_t*, uint32_t));

#endif /* __DECODE_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
