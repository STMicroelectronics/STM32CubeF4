/**
  ******************************************************************************
  * @file    USB_Host/DynamicSwitch_Standalone/Src/audio_explorer.c 
  * @author  MCD Application Team
  * @brief   This file provides uSD Card drive configuration
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
/* Includes ------------------------------------------------------------------ */
#include "main.h"

/* Private typedef ----------------------------------------------------------- */
FATFS SD_FatFs;
char SD_Path[4];
FILELIST_FileTypeDef FileList;

/* Private define ------------------------------------------------------------ */
/* Private macro ------------------------------------------------------------- */
/* Private variables --------------------------------------------------------- */
/* Private function prototypes ----------------------------------------------- */
/* Private functions --------------------------------------------------------- */

/**
  * @brief  Initializes the SD Storage.
  * @param  None
  * @retval Status
  */
uint8_t SD_StorageInit(void)
{
  /* Initializes the SD card device */
  BSP_SD_Init();

  /* Check if the SD card is plugged in the slot */
  if (BSP_SD_IsDetected() == SD_PRESENT)
  {
    /* Link the SD Card disk I/O driver */
    if (FATFS_LinkDriver(&SD_Driver, SD_Path) == 0)
    {
      if ((f_mount(&SD_FatFs, (TCHAR const *)SD_Path, 0) != FR_OK))
      {
        /* FatFs Initialization Error */
        LCD_ErrLog("Cannot Initialize FatFs! \n");
        return 1;
      }
      else
      {
        LCD_DbgLog("INFO : FatFs Initialized! \n");
      }
    }
  }
  else
  {
    LCD_ErrLog("SD card NOT plugged \n");
    return 1;
  }
  return 0;
}

/**
  * @brief  Copies disk content in the explorer list.
  * @param  None
  * @retval Operation result
  */
FRESULT SD_StorageParse(void)
{
  FRESULT res;
  FILINFO fno;
  DIR dir;
  char *fn;


  res = f_opendir(&dir, SD_Path);
  FileList.ptr = 0;

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

      if (FileList.ptr < FILEMGR_LIST_DEPDTH)
      {
        if ((fno.fattrib & AM_DIR) == 0)
        {
          if ((strstr(fn, "wav")) || (strstr(fn, "WAV")))
          {
            strncpy((char *)FileList.file[FileList.ptr].name, (char *)fn,
                    FILEMGR_FILE_NAME_SIZE);
            FileList.file[FileList.ptr].type = FILETYPE_FILE;
            FileList.ptr++;
          }
        }
      }
    }
  }
  f_closedir(&dir);
  return res;
}
