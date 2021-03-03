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
#include "stm32469i_eval.h"
#include "stm32469i_eval_lcd.h"
#include "stm32469i_eval_sdram.h"
#include "stm32469i_eval_camera.h"
#include "stm32469i_eval_ts.h"

/* FatFs includes component */
#include "ff_gen_drv.h"
#include "usbh_diskio_dma.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

#define WVGA_RES_X                    800
#define WVGA_RES_Y                    480

#define LCD_SCREEN_WIDTH              WVGA_RES_X
#define LCD_SCREEN_HEIGHT             WVGA_RES_Y

#define ARGB8888_BYTE_PER_PIXEL       4
#define RGB888_BYTE_PER_PIXEL         3
#define RGB565_BYTE_PER_PIXEL         2

/* LTDC foreground layer address 800x480 in ARGB8888 */
#define CAMERA_FRAME_BUFFER           (LCD_FB_START_ADDRESS + (LCD_SCREEN_WIDTH * LCD_SCREEN_HEIGHT * ARGB8888_BYTE_PER_PIXEL))

#define CONVERTED_FRAME_BUFFER        (CAMERA_FRAME_BUFFER + (CAMERA_480x272_RES_X * CAMERA_480x272_RES_Y * RGB565_BYTE_PER_PIXEL))

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
