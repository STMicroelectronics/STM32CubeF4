/**
  ******************************************************************************
  * @file    LibJPEG/LibJPEG_Encoding/Inc/encode.h 
  * @author  MCD Application Team
  * @brief   header of encode.c file
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
#ifndef __ENCODE_H
#define __ENCODE_H

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

void jpeg_encode(JFILE *file, JFILE *file1, uint32_t width, uint32_t height, uint32_t image_quality, uint8_t * buff);

#endif /* __ENCODE_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
