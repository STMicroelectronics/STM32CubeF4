/**
  ******************************************************************************
  * @file    fatfs_storage.h
  * @author  MCD Application Team
  * @brief   This file contains all the functions prototypes for the storage
  *          firmware driver.
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
#ifndef __FATFS_STORAGE_H
#define __FATFS_STORAGE_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
  
/* Header of a bitmap file */
#pragma pack(1) /* Mandatory to remove any padding */
typedef struct BmpHeader
{
  uint8_t  B;
  uint8_t  M;
  uint32_t fsize;
  uint16_t res1;
  uint16_t res2;
  uint32_t offset;
  uint32_t hsize;
  uint32_t w;
  uint32_t h;
  uint16_t planes;
  uint16_t bpp;
  uint32_t ctype;
  uint32_t dsize;
  uint32_t hppm;
  uint32_t vppm;
  uint32_t colorsused;
  uint32_t colorreq;
}BmpHeader;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
uint32_t Storage_OpenReadFile(uint8_t Xpoz, uint16_t Ypoz, const char *BmpName);
uint32_t Storage_CopyFile(const char *BmpName1, const char *BmpName2);
uint32_t Storage_GetDirectoryBitmapFiles(const char* DirName, char* Files[]);
uint32_t Storage_CheckBitmapFile(const char *BmpName, uint32_t *FileLen);
uint8_t  Buffercmp(uint8_t *pBuffer1, uint8_t *pBuffer2, uint16_t BufferLength);

#ifdef __cplusplus
}
#endif

#endif /* __FATFS_STORAGE_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
