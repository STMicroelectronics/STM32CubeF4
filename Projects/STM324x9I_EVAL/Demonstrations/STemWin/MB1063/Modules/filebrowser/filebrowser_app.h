/**
  ******************************************************************************
  * @file    filebrowser_app.h
  * @author  MCD Application Team
  * @brief   Header for filebrowser_app.c file
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
#ifndef __FILEBROWSER_APP_H
#define __FILEBROWSER_APP_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "k_storage.h"
   
/* Exported constants --------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void     FILEMGR_GetParentDir (char *dir);
void     FILEMGR_GetFileOnly (char *file, char *path);
uint8_t  FILEMGR_ParseDisks (char *path, FILELIST_FileTypeDef *list);
#ifdef __cplusplus
}
#endif

#endif /* __FILEBROWSER_APP_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
