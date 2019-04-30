/**
  ******************************************************************************
  * @file    USB_Host/CDC_Standalone/Src/explorer.c
  * @author  MCD Application Team
  * @brief   This file provides uSD Card drive configuration
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics International N.V. 
  * All rights reserved.</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------ */
#include "main.h"

/* Private define ------------------------------------------------------------ */
/* Private typedef ----------------------------------------------------------- */
/* Private macro ------------------------------------------------------------- */
/* Private variables --------------------------------------------------------- */
FATFS SD_FatFs;
char SD_Path[4];
FILELIST_FileTypeDef FileList;

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
          strncpy((char *)FileList.file[FileList.ptr].name, (char *)fn,
                  FILEMGR_FILE_NAME_SIZE);
          FileList.file[FileList.ptr].type = FILETYPE_FILE;
          FileList.ptr++;
        }
      }
    }
  }
  f_closedir(&dir);
  return res;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
