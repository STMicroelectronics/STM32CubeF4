/**
  ******************************************************************************
  * @file    Camera/Camera_To_USBDisk/Inc/main.h 
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
#include "stm32446e_eval.h"
#include "stm32446e_eval_io.h"
#include "stm32446e_eval_lcd.h"
#include "stm32446e_eval_sdram.h"
#include "stm32446e_eval_camera.h"
#include "stm32446e_eval_ts.h"
#include "fatfs_storage.h"

/* FatFs includes component */
#include "ff_gen_drv.h"
#include "usbh_diskio_dma.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* LCD Frame Buffer address */
#define CAMERA_FRAME_BUFFER               ((uint32_t)0xC0130000)

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void Touchscreen_Calibration (void);
uint16_t Calibration_GetX(uint16_t x);
uint16_t Calibration_GetY(uint16_t y);
uint8_t IsCalibrationDone(void);

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
