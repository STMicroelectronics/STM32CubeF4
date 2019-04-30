/**
  ******************************************************************************
  * @file    BSP/Src/sd.c 
  * @author  MCD Application Team
  * @brief   This example code shows how to use the SD Driver
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2017 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/** @addtogroup STM32F4xx_HAL_Examples
  * @{
  */

/** @addtogroup BSP
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define BLOCK_START_ADDR         0     /* Block start address      */
#define BLOCKSIZE                512   /* Block Size in Bytes      */
#define NUM_OF_BLOCKS            2     /* Total number of blocks   */
#define BUFFER_WORDS_SIZE        ((BLOCKSIZE * NUM_OF_BLOCKS) >> 2) /* Total data size in bytes */
/*tmp */
#define SD_OK 0
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint32_t aTxBuffer[BUFFER_WORDS_SIZE];
uint32_t aRxBuffer[BUFFER_WORDS_SIZE];

/* Private function prototypes -----------------------------------------------*/
static void SD_SetHint(void);
static void Fill_Buffer(uint32_t *pBuffer, uint32_t uwBufferLenght, uint32_t uwOffset);
static uint8_t Buffercmp(uint32_t* pBuffer1, uint32_t* pBuffer2, uint16_t BufferLength);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  SD Demo
  * @param  None
  * @retval None
  */
void SD_demo (void)
{ 
  uint8_t SD_state = SD_OK;

  SD_SetHint();
  SD_state = BSP_SD_Init();
  
  if(SD_state != SD_OK)
  {
    BSP_LCD_DisplayStringAt(5, 90, (uint8_t *)"SD Initialization", LEFT_MODE);
    BSP_LCD_DisplayStringAt(5, 105, (uint8_t *)"     FAILED", LEFT_MODE);
    BSP_LCD_DisplayStringAt(5, 120, (uint8_t *)"SD Test Aborted.", LEFT_MODE);
  }
  else
  {
    BSP_LCD_DisplayStringAt(5, 75, (uint8_t *)"SD Init : OK.", LEFT_MODE);
    
    SD_state = BSP_SD_Erase(BLOCK_START_ADDR, NUM_OF_BLOCKS);
    
    if(SD_state != SD_OK)
    {
      BSP_LCD_DisplayStringAt(5, 90, (uint8_t *)"SD ERASE : FAILED.", LEFT_MODE);
      BSP_LCD_DisplayStringAt(5, 105, (uint8_t *)"SD Test Aborted.", LEFT_MODE);
    }
    else
    {
      BSP_LCD_DisplayStringAt(5, 90, (uint8_t *)"SD ERASE : OK.", LEFT_MODE);
      
      /* Fill the buffer to write */
      Fill_Buffer(aTxBuffer, BUFFER_WORDS_SIZE, 0x22FF);
      SD_state = BSP_SD_WriteBlocks(aTxBuffer, BLOCK_START_ADDR, NUM_OF_BLOCKS, 1);
      
      if(SD_state != SD_OK)
      {
        BSP_LCD_DisplayStringAt(5, 105, (uint8_t *)"SD WRITE : FAILED.", LEFT_MODE);
        BSP_LCD_DisplayStringAt(5, 120, (uint8_t *)"SD Test Aborted.", LEFT_MODE);
      }
      else
      {
        BSP_LCD_DisplayStringAt(5, 105, (uint8_t *)"SD WRITE : OK.", LEFT_MODE);
        SD_state = BSP_SD_ReadBlocks(aRxBuffer, BLOCK_START_ADDR, NUM_OF_BLOCKS, 1);
        if(SD_state != SD_OK)
        {
          BSP_LCD_DisplayStringAt(5, 120, (uint8_t *)"SD READ : FAILED.", LEFT_MODE);
          BSP_LCD_DisplayStringAt(5, 135, (uint8_t *)"SD Test Aborted.", LEFT_MODE);
        }
        else
        {
          BSP_LCD_DisplayStringAt(5, 120, (uint8_t *)"SD READ : OK.", LEFT_MODE);
          if(Buffercmp(aTxBuffer, aRxBuffer, BUFFER_WORDS_SIZE) > 0)
          {
            BSP_LCD_DisplayStringAt(5, 135, (uint8_t *)"SD COMPARE : FAILED.", LEFT_MODE);
            BSP_LCD_DisplayStringAt(5, 150, (uint8_t *)"SD Test Aborted.", LEFT_MODE);
          }
          else
          {
            BSP_LCD_DisplayStringAt(5, 135, (uint8_t *)"SD Test : OK.", LEFT_MODE);
            BSP_LCD_DisplayStringAt(5, 150, (uint8_t *)"SD can be removed.", LEFT_MODE);
          }
        }
      }
    }
  }
  
  while (1)
  {
    if(CheckForUserInput() > 0)
    {
      /* BSP_SD_DeInit(); */
      return;
    }
  }
}

/**
  * @brief  Display SD Demo Hint
  * @param  None
  * @retval None
  */
static void SD_SetHint(void)
{
  /* Clear the LCD */ 
  BSP_LCD_Clear(LCD_COLOR_WHITE);
  
  /* Set SD Demo description */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_FillRect(0, 0, BSP_LCD_GetXSize(), 55);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_SetBackColor(LCD_COLOR_BLUE); 
  BSP_LCD_SetFont(&Font24);

  BSP_LCD_SetFont(&Font12);
  BSP_LCD_DisplayStringAt(0, 10, (uint8_t *)"SD Demo", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 25, (uint8_t *)"Shows how to write", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 40, (uint8_t *)"& read data on SD", CENTER_MODE);

  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_SetBackColor(LCD_COLOR_WHITE); 
}

/**
  * @brief  Fills buffer with user predefined data.
  * @param  pBuffer: pointer on the buffer to fill
  * @param  uwBufferLenght: size of the buffer to fill
  * @param  uwOffset: first value to fill on the buffer
  * @retval None
  */
static void Fill_Buffer(uint32_t *pBuffer, uint32_t uwBufferLenght, uint32_t uwOffset)
{
  uint32_t tmpIndex = 0;

  /* Put in global buffer different values */
  for (tmpIndex = 0; tmpIndex < uwBufferLenght; tmpIndex++ )
  {
    pBuffer[tmpIndex] = tmpIndex + uwOffset;
  }
}

/**
  * @brief  Compares two buffers.
  * @param  pBuffer1, pBuffer2: buffers to be compared.
  * @param  BufferLength: buffer's length
  * @retval 1: pBuffer identical to pBuffer1
  *         0: pBuffer differs from pBuffer1
  */
static uint8_t Buffercmp(uint32_t* pBuffer1, uint32_t* pBuffer2, uint16_t BufferLength)
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
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
