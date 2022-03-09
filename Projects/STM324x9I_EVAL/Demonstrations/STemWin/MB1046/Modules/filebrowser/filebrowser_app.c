/**
  ******************************************************************************
  * @file    filebrowser_app.c
  * @author  MCD Application Team
  * @brief   Utilities for file handling
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
/* Includes ------------------------------------------------------------------*/
#include "filebrowser_app.h"
#include <string.h>

/** @addtogroup FILE_BROWSER_MODULE
  * @{
  */

/** @defgroup FILE_BROWSER
 * @brief file browser routines
 * @{
 */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Copy disk content in the explorer list
  * @param  path: pointer to root path
  * @param  list: pointer to file list
  * @retval Status
  */
uint8_t  FILEMGR_ParseDisks (char *path, FILELIST_FileTypeDef *list)
{
  FRESULT res;
  FILINFO fno;
  DIR dir;
  char *fn;
  
  res = f_opendir(&dir, path);
  list->ptr = 0;
  
  if (res == FR_OK)
  {
    
    while (1)
    {
      res = f_readdir(&dir, &fno);
      
      if (res != FR_OK || fno.fname[0] == 0)
      {
        break;
      }
      if (fno.fname[0] == '.')
      {
        continue;
      }

      fn = fno.fname;

      if (list->ptr < FILEMGR_LIST_DEPDTH)
      {
        if ((fno.fattrib & AM_DIR) == AM_DIR)
        {
          strncpy((char *)list->file[list->ptr].name, (char *)fn, FILEMGR_FILE_NAME_SIZE);
          list->file[list->ptr].type = FILETYPE_DIR;
          list->ptr++;
        }
        else
        {
          strncpy((char *)list->file[list->ptr].name, (char *)fn, FILEMGR_FILE_NAME_SIZE);
          list->file[list->ptr].type = FILETYPE_FILE;
          list->ptr++;
        }
      }   
    }
  }
  f_closedir(&dir);
  return res;
}

/**
  * @brief  Retrieve the parent directory from full file path
  * @param  dir: pointer to parent directory
  * @retval None
*/
void FILEMGR_GetParentDir (char *dir)
{
  uint16_t idx = FILEMGR_FULL_PATH_SIZE;
  
  for ( ; idx > 0; idx --)
  {
    
    if (dir [idx] == '/')
    {
      dir [idx + 1] = 0;
      break;
    }
    dir [idx + 1] = 0;
  }
}


/**
  * @brief  Retrieve the file name from a full file path
  * @param  file: pointer to base path
  * @param  path: pointer to full path
  * @retval None
*/
void FILEMGR_GetFileOnly (char *file, char *path)
{
  char *baseName1, *baseName2;
  baseName1 = strrchr(path,'/');
  baseName2 = strrchr(path,':');
  
  if(baseName1++) 
  { 
    strcpy(file, baseName1);
  }
  else 
  {
    if (baseName2++) 
    {
      
      strcpy(file, baseName2);
    }
    else
    {
      strcpy(file, path);
    }
  }
}

/**
* @}
*/ 

/**
* @}
*/ 
