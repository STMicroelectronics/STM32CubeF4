/**
  ******************************************************************************
  * @file    Display/LTDC_AnimatedPictureFromSDCard/Inc/fatfs_storage.h
  * @author  MCD Application Team
  * @brief   This file contains all the functions prototypes for the storage
  *          firmware driver.
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
#ifndef __FAT_FS_STORAGE_H
#define __FAT_FS_STORAGE_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include "ff.h"
#include "stm32f4xx_hal.h"
#include "stm32469i_eval_sd.h"

/** @addtogroup STM32469I_EVAL_FAT_FS
  * @{
  */

/** @addtogroup FAT_FS_STORAGE
  * @{
  */

/** @defgroup FAT_FS_STORAGE_Exported_Types
  * @{
  */
/**
  * @}
  */

/** @defgroup FAT_FS_STORAGE_Exported_Constants
  * @{
  */
/**
  * @}
  */

/** @defgroup FAT_FS_STORAGE_Exported_Macros
  * @{
  */
/**
  * @}
  */

/** @defgroup FAT_FS_STORAGE_Exported_Functions
  * @{
  */
uint32_t Storage_Init (void);
uint32_t Storage_OpenReadFile(uint8_t *Address, const char* BmpName);
uint32_t Storage_GetDirectoryBitmapFiles (const char* DirName, char* Files[]);
uint32_t Storage_CheckBitmapFile(const char* BmpName, uint32_t *FileLen);
uint8_t Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength);

#define MAX_BMP_FILES     25
#define MAX_BMP_FILE_NAME 11

#ifdef __cplusplus
}
#endif

#endif /* __FAT_FS_STORAGE_H */
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
