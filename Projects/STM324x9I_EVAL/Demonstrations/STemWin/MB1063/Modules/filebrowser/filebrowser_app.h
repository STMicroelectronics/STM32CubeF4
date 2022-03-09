/**
  ******************************************************************************
  * @file    filebrowser_app.h
  * @author  MCD Application Team
  * @brief   Header for filebrowser_app.c file
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
