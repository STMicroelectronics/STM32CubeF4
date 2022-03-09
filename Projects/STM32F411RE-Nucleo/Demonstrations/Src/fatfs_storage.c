/**
  ******************************************************************************
  * @file    Demonstrations/Src/fatfs_storage.c
  * @author  MCD Application Team

  * @brief   This file includes the Storage (FatFs) driver 
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
#include "main.h"
#include <string.h>

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t aBuffer[512];
FATFS fs;
FILINFO MyFileInfo;
DIR MyDirectory;
FIL MyFile;
UINT BytesWritten, BytesRead;

const uint8_t SlidesCheck[2] =
  {
    0x42, 0x4D
  };


/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Open a file and copy its content to a buffer
  * @param  Xpoz: X position
  * @param  Ypoz: Y position
  * @param  BmpName: A pointer to the Bmp file name
  * @retval err: Error status (0=> success, 1=> fail)
  */
uint32_t Storage_OpenReadFile(uint8_t Xpoz, uint16_t Ypoz, const char *BmpName)
{
  uint32_t index = 0, size = 0, width = 0, height = 0, i1 = 0, x = 0, y = 159;
  uint32_t bmpaddress, bit_pixel = 0;
  FIL file1;
  uint16_t color = 0;
  
  f_open(&file1, BmpName, FA_READ);
  f_read(&file1, aBuffer, 30, &BytesRead);
  
  bmpaddress = (uint32_t)aBuffer;
  
  /* Read bitmap size */
  size = *(uint16_t *) (bmpaddress + 2);
  size |= (*(uint16_t *) (bmpaddress + 4)) << 16;
  
  /* Get bitmap data address offset */
  index = *(uint16_t *) (bmpaddress + 10);
  index |= (*(uint16_t *) (bmpaddress + 12)) << 16;
  
  /* Read bitmap width */
  width = *(uint16_t *) (bmpaddress + 18);
  width |= (*(uint16_t *) (bmpaddress + 20)) << 16;
  
  /* Read bitmap height */
  height = *(uint16_t *) (bmpaddress + 22);
  height |= (*(uint16_t *) (bmpaddress + 24)) << 16;
  
  /* Read bit/pixel */
  bit_pixel = *(uint16_t *) (bmpaddress + 28);  
  bit_pixel = bit_pixel/8;
  
  size = (size - index);
  
  /* Synchronize f_read right in front of the image data */
  f_close (&file1);  
 
  f_open (&file1, (TCHAR const*)BmpName, FA_READ);
  
  f_read(&file1, aBuffer, index, &BytesRead);
  
  do
  {
    if(size < 512)
    {
      i1 = size;
    }
    else
    {
      i1 = 512;
    }
    size -= i1; 
   
    f_read(&file1, aBuffer, i1, (UINT *)&BytesRead);
    
    for (index = 0; index < (i1/bit_pixel); index++)
    {
      
      if( index%bit_pixel == 0)
      {
        color = (aBuffer[ index + 1] << 8) | aBuffer[ index];
      }
      x = index/bit_pixel;
      BSP_LCD_DrawPixel(x, y, color); 
    }
    y--;
    for (index = (i1/bit_pixel); index < i1; index++)
    {
      if( index%bit_pixel == 0)
      {
        color = (aBuffer[ index + 1] << 8) | aBuffer[ index];
      } 
      x = index/bit_pixel - width;
      BSP_LCD_DrawPixel(x, y, color); 
    }
    y--;
  }
  while (size > 0);
  f_close(&file1);
  
  return 1;
}      

/**
  * @brief  Opens a file and copies its content to a buffer.
  * @param  BmpName: A pointer to the Bmp file name
  * @param  FileLen: A pointer to the Bmp file length
  * @retval err: Error status (0=> success, 1=> fail)
  */
uint32_t Storage_CheckBitmapFile(const char* BmpName, uint32_t *FileLen)
{
//  FRESULT res;
//  uint32_t err = 0;
//  
//  res = f_open(&MyFile, BmpName, FA_READ);
//  if(res != FR_OK)
//  {
//    err = 1;
//  }
//  
//  return err;
  
  if(f_mount(&fs, (TCHAR const*)"",0))
  {
    return 1;
  }
  if(f_open (&MyFile, (TCHAR const*)BmpName, FA_READ))
  {
    return 2;
  }
  
  f_read (&MyFile, aBuffer, 6, (UINT *)&BytesRead);
  
  if (Buffercmp((uint8_t *)SlidesCheck, (uint8_t *) aBuffer, 2) != 0)
  {
    return 3;
  }
  return 0;
}

/**
  * @brief  List up to 25 file on the root directory with extension .BMP
  * @param  DirName: A pointer to the Directory name
  * @param  Files: Buffer to contain read files
  * @retval The number of the found files
  */
uint32_t Storage_GetDirectoryBitmapFiles(const char* DirName, char* Files[])
{
  FRESULT res;
  uint32_t index = 0;
  
  /* Open filesystem */
  if(f_mount(&fs, (TCHAR const*)"",0) != FR_OK)
  {
    return 0;
  }
  
  /* Start to search for wave files */
  res = f_findfirst(&MyDirectory, &MyFileInfo, DirName, "*.bmp");

  /* Repeat while an item is found */
  while (MyFileInfo.fname[0])
  {
    if(res == FR_OK)
    {
      if(index < MAX_BMP_FILES)
      {
        sprintf (Files[index++], "%s", MyFileInfo.fname);
      }
      /* Search for next item */
      res = f_findnext(&MyDirectory, &MyFileInfo);
    }
    else
    {
      index = 0;
      break;
    }
  }

  f_closedir(&MyDirectory);

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
