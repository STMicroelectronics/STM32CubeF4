/**
  ******************************************************************************
  * @file    Display/LCD_Paint/Inc/main.h
  * @author  MCD Application Team
  * @brief   Header for main.c module
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2017 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software
  * distributed under the License is distributed on an "AS IS" BASIS,
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32469i_eval.h"
#include "stm32469i_eval_lcd.h"
#include "stm32469i_eval_ts.h"
#include "stm32469i_eval_sdram.h"
#include "stm32469i_eval_camera.h"
#include <stdlib.h>
#include <stdio.h>

/* FatFs includes component */
#include "ff_gen_drv.h"
#include "sd_diskio.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

/* Alias for LCD_FB_START_ADDRESS : frame buffer displayed on LCD */
#define LCD_FRAME_BUFFER              LCD_FB_START_ADDRESS

#define WVGA_RES_X                    800
#define WVGA_RES_Y                    480

#define LCD_SCREEN_WIDTH              WVGA_RES_X
#define LCD_SCREEN_HEIGHT             WVGA_RES_Y

#define ARGB8888_BYTE_PER_PIXEL       4
#define RGB888_BYTE_PER_PIXEL         3

/* Buffer LCD Converted to RGB888 in SDRAM in order to be copied to SD Card   */
/* Starts directly at end of LCD frame buffer                                 */
#define CONVERTED_FRAME_BUFFER        LCD_FRAME_BUFFER + (LCD_SCREEN_WIDTH * LCD_SCREEN_HEIGHT * ARGB8888_BYTE_PER_PIXEL)

/* Imported globals ----------------------------------------------------------*/
extern TS_StateTypeDef  TS_State;

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
uint8_t  Touchscreen_Calibration(void);
uint16_t TouchScreen_Get_Calibrated_X(uint16_t x);
uint16_t TouchScreen_Get_Calibrated_Y(uint16_t y);
uint8_t  TouchScreen_IsCalibrationDone(void);

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
