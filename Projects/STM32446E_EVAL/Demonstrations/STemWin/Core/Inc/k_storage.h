/**
  ******************************************************************************
  * @file    k_storage.h
  * @author  MCD Application Team
  * @brief   Header for k_storage.c file
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
#ifndef __K_STORAGE_H
#define __K_STORAGE_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "k_module.h"   

   /* Exported constants --------------------------------------------------------*/
#define FILEMGR_LIST_DEPDTH                     24
#define FILEMGR_FILE_NAME_SIZE                  40
#define FILEMGR_FULL_PATH_SIZE                 128
#define FILEMGR_MAX_LEVEL                        3  
#define FILEMGR_MAX_EXT_SIZE                     3
   
#define FILETYPE_DIR                            0
#define FILETYPE_FILE                           1
   
/* Exported types ------------------------------------------------------------*/
typedef enum
{
  USBDISK_DISCONNECTION_EVENT = 1,  
  USBDISK_CONNECTION_EVENT,  
  MSDDISK_DISCONNECTION_EVENT,  
  MSDDISK_CONNECTION_EVENT,     
}   
STORAGE_EventTypeDef;

   typedef struct _FILELIST_LineTypeDef
{
  uint8_t               type;
  uint8_t               name[FILEMGR_FILE_NAME_SIZE];

}
FILELIST_LineTypeDef;

typedef struct _FILELIST_FileTypeDef
{
  FILELIST_LineTypeDef  file[FILEMGR_LIST_DEPDTH] ;
  uint16_t              ptr;
  struct _FILELIST_FileTypeDef *next;
  struct _FILELIST_FileTypeDef *prev;  
}
FILELIST_FileTypeDef;
/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void     k_StorageInit(void);
uint8_t  k_StorageGetStatus (uint8_t unit);
uint32_t k_StorageGetCapacity (uint8_t unit);
uint32_t k_StorageGetFree (uint8_t unit);
int      k_GetData(CHOOSEFILE_INFO * pInfo);
void     k_GetExtOnly(char * pFile, char * pExt);
#ifdef __cplusplus
}
#endif

#endif /*__K_STORAGE_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
