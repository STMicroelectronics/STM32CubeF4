/**
  ******************************************************************************
  * @file    Display/LTDC_PicturesFromSDCard/Inc/main.h
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
#include <stdlib.h>
#include "fatfs_storage.h"

/* FatFs includes component */
#include "ff_gen_drv.h"
#include "sd_diskio.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define LCD_SCREEN_WIDTH              800
#define LCD_SCREEN_HEIGHT             480
#define ARGB8888_BYTE_PER_PIXEL       4

/* LTDC foreground layer address 800x480 in ARGB8888 */
#define LCD_FG_LAYER_ADDRESS          LCD_FB_START_ADDRESS

/* LTDC background layer address 800x480 in ARGB8888 following the foreground layer */
#define LCD_BG_LAYER_ADDRESS          LCD_FG_LAYER_ADDRESS + (LCD_SCREEN_WIDTH * LCD_SCREEN_HEIGHT * ARGB8888_BYTE_PER_PIXEL)

#define INTERNAL_BUFFER_START_ADDRESS LCD_BG_LAYER_ADDRESS + (LCD_SCREEN_WIDTH * LCD_SCREEN_HEIGHT * ARGB8888_BYTE_PER_PIXEL)

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
