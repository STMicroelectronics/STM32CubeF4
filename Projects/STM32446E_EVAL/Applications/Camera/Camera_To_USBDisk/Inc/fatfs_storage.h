/**
  ******************************************************************************
  * @file    Camera/Camera_To_USBDisk/Inc/fatfs_storage.h
  * @author  MCD Application Team
  * @brief   This file contains all the functions prototypes for the storage
  *          firmware driver.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2017 STMicroelectronics </center></h2>
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
#ifndef __FATFS_STORAGE_H
#define __FATFS_STORAGE_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include "ff.h"


/** @addtogroup STM324xG_EVAL_FAT_FS
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
uint32_t Storage_OpenReadFile(uint32_t Address, const char* BmpName);
uint32_t Storage_GetDirectoryBitmapFiles (const char* DirName, char* Files[]);
uint32_t Storage_CheckBitmapFile(const char* BmpName, uint32_t *FileLen);
uint8_t Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength);

#define MAX_BMP_FILES 10
#define MAX_BMP_FILE_NAME 11

#ifdef __cplusplus
}
#endif

#endif /* __FATFS_STORAGE_H */
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
