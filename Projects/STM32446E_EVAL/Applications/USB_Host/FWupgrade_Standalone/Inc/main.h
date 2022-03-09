/**
  ******************************************************************************
  * @file    USB_Host/FWupgrade_Standalone/Inc/main.h
  * @author  MCD Application Team
  * @brief   Header for main.c module
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
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stdio.h"
#include "usbh_core.h"
#include "stm32446e_eval.h"
#include "stm32446e_eval_io.h"
#include "stm32446e_eval_lcd.h"
#include "usbh_msc.h"
#include "ff.h"
#include "ff_gen_drv.h"
#include "usbh_diskio_dma.h"
#include "flash_if.h"
#include "command.h"

/* Exported types ------------------------------------------------------------*/
typedef enum {
  APPLICATION_IDLE = 0,
  APPLICATION_READY,
  APPLICATION_CONNECT,
  APPLICATION_DISCONNECT,
}FW_ApplicationTypeDef;

extern USBH_HandleTypeDef hUSBHost;
extern FATFS USBH_fatfs;
extern FW_ApplicationTypeDef Appli_state;
extern __IO uint32_t UploadCondition;

/* Exported constants --------------------------------------------------------*/
/* This value can be equal to (512 * x) according to RAM size availability with x=[1, 128]
   In this project x is fixed to 64 => 512 * 64 = 32768bytes = 32 Kbytes */
#define BUFFER_SIZE        ((uint16_t)512*64)

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void FW_UPGRADE_Process(void);
void Fail_Handler(void);
void FatFs_Fail_Handler(void);
void Erase_Fail_Handler(void);

#endif /* __MAIN_H */
