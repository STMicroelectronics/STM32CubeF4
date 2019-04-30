/**
  ******************************************************************************
  * @file    Display/LCD_Paint/Src/fatfs_storage.c
  * @author  MCD Application Team
  * @brief   This file includes the Storage (FatFs) driver for the STM324xG-EVAL
  *          application.
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
/* Includes ------------------------------------------------------------------*/
#include "fatfs_storage.h"

/** @addtogroup STM324xG_EVAL_FAT_FS
  * @{
  */

/** @defgroup FATFS_STORAGE
  * @brief This file includes the Storage (FatFs) driver for the STM324xG-EVAL
  *        application.
  * @{
  */

/** @defgroup FATFS_STORAGE_Private_Types
  * @{
  */
/**
  * @}
  */

/** @defgroup FATFS_STORAGE_Private_Defines
  * @{
  */
/**
  * @}
  */

/** @defgroup FATFS_STORAGE_Private_Macros
  * @{
  */
/**
  * @}
  */

/** @defgroup FATFS_STORAGE_Private_Variables
  * @{
  */
uint8_t sector[512];
FATFS fs;
FILINFO fno;
DIR dir;
FIL F;
const uint8_t SlidesCheck[2] =
  {
    0x42, 0x4D
  };
uint32_t BytesRead = 0;

/**
  * @}
  */


/** @defgroup FATFS_STORAGE_Private_FunctionPrototypes
  * @{
  */
/**
  * @}
  */

/** @defgroup FATFS_STORAGE_Private_Functions
  * @{
  */

/**
  * @brief  SDCARD Initialization for FatFs
  * @param  None
  * @retval err : Error status (0=> success, 1=> fail)
  */
uint32_t Storage_Init(void)
{
  BSP_SD_Init();
  
  /****************** FatFs Volume Acess ******************************/
  if (f_mount(0, &fs))
  {
    return 1;
  }
  return 0;
}

/**
  * @brief  Open a file and copy its content to a buffer
  * @param  DirName: the Directory name to open
  * @param  FileName: the file name to open
  * @param  BufferAddress: A pointer to a buffer to copy the file to
  * @param  FileLen: the File length
  * @retval err: Error status (0=> success, 1=> fail)
  */
uint32_t Storage_OpenReadFile(uint32_t Address, const char* BmpName)
{
  uint32_t index = 0, size = 0, i1 = 0;
  uint32_t BmpAddress;
  FIL F1;

  if (f_open(&F1, BmpName, FA_READ) != FR_OK)
  {
    while(1)
    {
    } 
  }
  if (f_read (&F1, sector, 30, &BytesRead) != FR_OK)
  {
    while(1)
    {
    } 
  }
  
  BmpAddress = (uint32_t)sector;

  /* Read bitmap size */
  size = *(uint16_t *) (BmpAddress + 2);
  size |= (*(uint16_t *) (BmpAddress + 4)) << 16;  
 
  /* Get bitmap data address offset */
  index = *(uint16_t *) (BmpAddress + 10);
  index |= (*(uint16_t *) (BmpAddress + 12)) << 16;  
  
  f_close (&F1);
  
  f_open (&F1, BmpName, FA_READ);

  do
  {
    if (size < 256*2)
    {
      i1 = size;
    }
    else
    {
      i1 = 256*2;
    }
    size -= i1;
    f_read (&F1, sector, i1, &BytesRead);

    for (index = 0; index < i1; index++)
    {
      *(__IO uint8_t*) (Address) = *(__IO uint8_t *)BmpAddress;
      
      BmpAddress++;  
      Address++;
    }  
    
    BmpAddress = (uint32_t)sector;
  }
  while (size > 0);

  f_close (&F1);
  
  return 1;
}

/**
  * @brief  Open a file and copy its content to a buffer
  * @param  DirName: the Directory name to open
  * @param  FileName: the file name to open
  * @param  BufferAddress: A pointer to a buffer to copy the file to
  * @param  FileLen: the File length
  * @retval err: Error status (0=> success, 1=> fail)
  */
uint32_t Storage_CheckBitmapFile(const char* BmpName, uint32_t *FileLen)
{

  if (f_mount(0, &fs))
  {
    return 1;
  }
  if (f_open (&F, BmpName, FA_READ))
  {
    return 2;
  }

  f_read (&F, sector, 6, &BytesRead);

  if (Buffercmp((uint8_t *)SlidesCheck, (uint8_t *) sector, 2) != 0)
  {
    return 3;
  }
  return 0;
}

/**
  * @brief  List up to 25 file on the root directory with extension .BMP
  * @param  None
  * @retval The number of the found files
  */
uint32_t Storage_GetDirectoryBitmapFiles (const char* DirName, char* Files[])
{
  
  FATFS fs;
  FILINFO fno;
  DIR dir;
  uint32_t counter = 0, index = 0;
  FRESULT res;

  /* Open filesystem */
  if (f_mount(0, &fs) != FR_OK)
  {
    return 0;
  }

  /* Open directory */
  res = f_opendir(&dir, DirName);
  
  if (res == FR_OK)
  {
    for (;;)
    {
      res = f_readdir(&dir, &fno);
      if (res != FR_OK || fno.fname[0] == 0)
        break;
      if (fno.fname[0] == '.')
        continue;

      if (!(fno.fattrib & AM_DIR))
      {
        do
        {
          counter++;
        }
        while (fno.fname[counter] != 0x2E); /* . */


        if (index < MAX_BMP_FILES)
        {
          if ((fno.fname[counter + 1] == 'B') && (fno.fname[counter + 2] == 'M') && (fno.fname[counter + 3] == 'P'))
          {
            if(sizeof(fno.fname) <= (MAX_BMP_FILE_NAME + 2))
            {
              sprintf (Files[index], "%0s", fno.fname);
              index++;
            }
          }
        }
        counter = 0;
      }
    }
  }
  f_mount (0, NULL);
  return index;
}

/**
  * @brief  Compares two buffers.
  * @param  pBuffer1, pBuffer2: buffers to be compared.
  * @param  BufferLength: buffer's length.
  * @retval  0: pBuffer1 identical to pBuffer2
  *          1: pBuffer1 differs from pBuffer2
  */
uint8_t Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength)
{
  while (BufferLength--)
  {
    if (*pBuffer1 != *pBuffer2)
    {
      return 1;
    }

    pBuffer1++;
    pBuffer2++;
  }

  return 0;
}

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
