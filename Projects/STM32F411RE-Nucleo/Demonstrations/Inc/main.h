/**
  ******************************************************************************
  * @file    Demonstrations/Inc/main.h 
  * @author  MCD Application Team
  * @brief   Header for main.c module
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
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_nucleo.h"
#include "stm32_adafruit_sd.h"
#include "stm32_adafruit_lcd.h"
#include <stdio.h>
#include <stdlib.h>

/* FatFs includes component */
#include "ff_gen_drv.h"
#include "sd_diskio.h"
#include "fatfs_storage.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define  MAX_BMP_FILES  25
#define  MAX_BMP_FILE_NAME 11

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
