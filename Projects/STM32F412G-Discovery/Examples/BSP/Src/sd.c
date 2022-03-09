/**
  ******************************************************************************
  * @file    BSP/Src/sd.c 
  * @author  MCD Application Team
  * @brief   This example code shows how to use the SD Driver
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

/** @addtogroup STM32F4xx_HAL_Examples
  * @{
  */

/** @addtogroup BSP
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define BLOCK_START_ADDR         0     /* Block start address      */
#define NUM_OF_BLOCKS            5     /* Total number of blocks   */
#define BUFFER_WORDS_SIZE        ((BLOCKSIZE * NUM_OF_BLOCKS) >> 2) /* Total data size in bytes */
#define HEADBAND_HEIGHT          64
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint32_t aTxBuffer[BUFFER_WORDS_SIZE];
uint32_t aRxBuffer[BUFFER_WORDS_SIZE];
/* Private function prototypes -----------------------------------------------*/
void SD_main_test(void);
void SD_DMA_test(void);
void SD_Detection(void);
static void SD_SetHint(void);
static void SD_DMA_SetHint(void);
static void Fill_Buffer(uint32_t *pBuffer, uint32_t uwBufferLenght, uint32_t uwOffset);
static uint8_t Buffercmp(uint32_t* pBuffer1, uint32_t* pBuffer2, uint16_t BufferLength);
/* Global extern variables ---------------------------------------------------*/
extern uint8_t SDDetectIT;

__IO uint32_t SDWriteStatus = 0, SDReadStatus = 0;

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  SD Demo polling detection
  * @param  None
  * @retval None
  */
void SD_demo (void)
{ 
  SD_main_test();
  
  while (1)
  {
    SD_Detection();

    if(CheckForUserInput() > 0)
    {
      BSP_SD_DeInit();
      return;
    }
  }
}

/**
  * @brief  SD Demo polling detection
  * @param  None
  * @retval None
  */
void SD_DMA_demo (void)
{ 
  SD_DMA_test();
  
  while (1)
  {
    SD_Detection();

    if(CheckForUserInput() > 0)
    {
      BSP_SD_DeInit();
      SdmmcTest = 0;
      return;
    }
  }
}

/**
  * @brief  SD Demo exti detection
  * @param  None
  * @retval None
  */
void SD_exti_demo (void)
{ 
  SD_main_test();

  if(BSP_SD_IsDetected() != SD_PRESENT)
  {
    BSP_SD_Init();
    BSP_LCD_SetTextColor(LCD_COLOR_RED);
    BSP_LCD_DisplayStringAt(5, BSP_LCD_GetYSize()-15, (uint8_t *)"SD Not Connected", LEFT_MODE);
  }
  else 
  {
    BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
    BSP_LCD_DisplayStringAt(5, BSP_LCD_GetYSize()-15, (uint8_t *)"SD Connected    ", LEFT_MODE);
  }

  BSP_SD_ITConfig();

  while (1)
  {
    /* Check SDDetectIT global variable state set in HAL_GPIO_EXTI_Callback in main.c */
    if (SDDetectIT != 0)
    {
      SD_Detection();
      SDDetectIT = 0;
    }
    
    if(CheckForUserInput() > 0)
    {
      BSP_SD_DeInit();
      return;
    }
  }
}

/**
  * @brief  SD_Detection checks detection and writes msg on display
  * @param  None
  * @retval None
  */
void SD_Detection(void)
{
  static uint8_t prev_status = 2;
 
   /* Check if the SD card is plugged in the slot */
  if(BSP_SD_IsDetected() != SD_PRESENT)
  {
    if(prev_status != SD_NOT_PRESENT)
    {
      prev_status = SD_NOT_PRESENT;
      BSP_LCD_SetTextColor(LCD_COLOR_RED);
      BSP_LCD_DisplayStringAt(5, BSP_LCD_GetYSize()-15, (uint8_t *)"SD Not Connected", LEFT_MODE);
    }
  }
  else if (prev_status != SD_PRESENT)
  {
    prev_status = SD_PRESENT;
    BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
    BSP_LCD_DisplayStringAt(5, BSP_LCD_GetYSize()-15, (uint8_t *)"SD Connected    ", LEFT_MODE);
  }
}

/**
  * @brief  Main part of SD Demo Test
  * @param  None
  * @retval None
  */
void SD_main_test (void)
{ 
  uint8_t SD_state = MSD_OK;

  SD_SetHint();
  SD_state = BSP_SD_Init();
  
  if(SD_state != MSD_OK)
  {
    if(SD_state == MSD_ERROR_SD_NOT_PRESENT)
    {
      BSP_LCD_DisplayStringAt(5, 70, (uint8_t *)"SD shall be inserted", LEFT_MODE);
    }
    else
    {
      BSP_LCD_DisplayStringAt(5, 70, (uint8_t *)"SD Initialization : FAIL.", LEFT_MODE);
    }
    BSP_LCD_DisplayStringAt(5, 85, (uint8_t *)"SD Test Aborted.", LEFT_MODE);
  }
  else
  {
    BSP_LCD_DisplayStringAt(5, 70, (uint8_t *)"SD Initialization : OK.", LEFT_MODE);
    
    SD_state = BSP_SD_Erase(BLOCK_START_ADDR, NUM_OF_BLOCKS);
    
    /* Wait until SD cards are ready to use for new operation */
    while((BSP_SD_GetCardState() != SD_TRANSFER_OK))
    {
    }
    
    if(SD_state != MSD_OK)
    {
      BSP_LCD_DisplayStringAt(5, 85, (uint8_t *)"SD ERASE : FAILED.", LEFT_MODE);
      BSP_LCD_DisplayStringAt(5, 100, (uint8_t *)"SD Test Aborted.", LEFT_MODE);
    }
    else
    {
      BSP_LCD_DisplayStringAt(5, 85, (uint8_t *)"SD ERASE : OK.", LEFT_MODE);
      
      /* Fill the buffer to write */
      Fill_Buffer(aTxBuffer, BUFFER_WORDS_SIZE, 0x22FF);
      SD_state = BSP_SD_WriteBlocks(aTxBuffer, BLOCK_START_ADDR, NUM_OF_BLOCKS, 1000);
      
    /* Wait until SD cards are ready to use for new operation */
      while((BSP_SD_GetCardState() != SD_TRANSFER_OK))
      {
      }
      
      if(SD_state != MSD_OK)
      {
        BSP_LCD_DisplayStringAt(5, 100, (uint8_t *)"SD WRITE : FAILED.", LEFT_MODE);
        BSP_LCD_DisplayStringAt(5, 115, (uint8_t *)"SD Test Aborted.", LEFT_MODE);
      }
      else
      {
        BSP_LCD_DisplayStringAt(5, 100, (uint8_t *)"SD WRITE : OK.", LEFT_MODE);
        SD_state = BSP_SD_ReadBlocks(aRxBuffer, BLOCK_START_ADDR, NUM_OF_BLOCKS, 1000);
        
        /* Wait until SD cards are ready to use for new operation */
        while((BSP_SD_GetCardState() != SD_TRANSFER_OK))
        {
        }
        
        if(SD_state != MSD_OK)
        {
          BSP_LCD_DisplayStringAt(5, 115, (uint8_t *)"SD READ : FAILED.", LEFT_MODE);
          BSP_LCD_DisplayStringAt(5, 130, (uint8_t *)"SD Test Aborted.", LEFT_MODE);
        }
        else
        {
          BSP_LCD_DisplayStringAt(5, 115, (uint8_t *)"SD READ : OK.", LEFT_MODE);
          if(Buffercmp(aTxBuffer, aRxBuffer, BUFFER_WORDS_SIZE) > 0)
          {
            BSP_LCD_DisplayStringAt(5, 130, (uint8_t *)"SD COMPARE : FAILED.", LEFT_MODE);
            BSP_LCD_DisplayStringAt(5, 145, (uint8_t *)"SD Test Aborted.", LEFT_MODE);
          }
          else
          {
            BSP_LCD_DisplayStringAt(5, 130, (uint8_t *)"SD Test : OK.", LEFT_MODE);
            BSP_LCD_DisplayStringAt(5, 145, (uint8_t *)"SD can be removed.", LEFT_MODE);
          }
        }
      }
    }
  }
}

/**
  * @brief  Main part of SD Demo Test
  * @param  None
  * @retval None
  */
void SD_DMA_test (void)
{ 
  uint8_t SD_state = MSD_OK;
  SdmmcTest = 1;
  SD_DMA_SetHint();
  SD_state = BSP_SD_Init();
  
  if(SD_state != MSD_OK)
  {
    if(SD_state == MSD_ERROR_SD_NOT_PRESENT)
    {
      BSP_LCD_DisplayStringAt(5, 70, (uint8_t *)"SD shall be inserted", LEFT_MODE);
    }
    else
    {
      BSP_LCD_DisplayStringAt(5, 70, (uint8_t *)"SD Initialization : FAIL.", LEFT_MODE);
    }
    BSP_LCD_DisplayStringAt(5, 85, (uint8_t *)"SD Test Aborted.", LEFT_MODE);
  }
  else
  {
    BSP_LCD_DisplayStringAt(5, 70, (uint8_t *)"SD Initialization : OK.", LEFT_MODE);
    
    SD_state = BSP_SD_Erase(BLOCK_START_ADDR, NUM_OF_BLOCKS);
    
    /* Wait until SD cards are ready to use for new operation */
    while((BSP_SD_GetCardState() != SD_TRANSFER_OK))
    {
    }
    
    if(SD_state != MSD_OK)
    {
      BSP_LCD_DisplayStringAt(5, 85, (uint8_t *)"SD ERASE : FAILED.", LEFT_MODE);
      BSP_LCD_DisplayStringAt(5, 100, (uint8_t *)"SD Test Aborted.", LEFT_MODE);
    }
    else
    {
      BSP_LCD_DisplayStringAt(5, 85, (uint8_t *)"SD ERASE : OK.", LEFT_MODE);
      
      /* Fill the buffer to write */
      Fill_Buffer(aTxBuffer, BUFFER_WORDS_SIZE, 0x22FF);
      SD_state = BSP_SD_WriteBlocks_DMA(aTxBuffer, BLOCK_START_ADDR, NUM_OF_BLOCKS);
      
      /* Wait for the write process is completed */
      while((SDWriteStatus == 0))
      {
      }
      SDWriteStatus = 0;
      
      /* Wait until SD cards are ready to use for new operation */
      while((BSP_SD_GetCardState() != SD_TRANSFER_OK))
      {
      }
      
      if(SD_state != MSD_OK)
      {
        BSP_LCD_DisplayStringAt(5, 100, (uint8_t *)"SD WRITE : FAILED.", LEFT_MODE);
        BSP_LCD_DisplayStringAt(5, 115, (uint8_t *)"SD Test Aborted.", LEFT_MODE);
      }
      else
      {
        BSP_LCD_DisplayStringAt(5, 100, (uint8_t *)"SD WRITE : OK.", LEFT_MODE);
        SD_state = BSP_SD_ReadBlocks_DMA(aRxBuffer, BLOCK_START_ADDR, NUM_OF_BLOCKS);
        
        /* Wait for the read process is completed */
        while(SDReadStatus == 0)
        {
        }
        SDReadStatus = 0;
        
        /* Wait until SD cards are ready to use for new operation */
        while(BSP_SD_GetCardState() != SD_TRANSFER_OK)
        {          
        }
      
        if(SD_state != MSD_OK)
        {
          BSP_LCD_DisplayStringAt(5, 115, (uint8_t *)"SD READ : FAILED.", LEFT_MODE);
          BSP_LCD_DisplayStringAt(5, 130, (uint8_t *)"SD Test Aborted.", LEFT_MODE);
        }
        else
        {
          BSP_LCD_DisplayStringAt(5, 115, (uint8_t *)"SD READ : OK.", LEFT_MODE);
          if(Buffercmp(aTxBuffer, aRxBuffer, BUFFER_WORDS_SIZE) > 0)
          {
            BSP_LCD_DisplayStringAt(5, 130, (uint8_t *)"SD COMPARE : FAILED.", LEFT_MODE);
            BSP_LCD_DisplayStringAt(5, 145, (uint8_t *)"SD Test Aborted.", LEFT_MODE);
          }
          else
          {
            BSP_LCD_DisplayStringAt(5, 130, (uint8_t *)"SD Test : OK.", LEFT_MODE);
            BSP_LCD_DisplayStringAt(5, 145, (uint8_t *)"SD can be removed.", LEFT_MODE);
          }
        }
      }
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
  
  /* Set LCD Demo description */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_FillRect(0, 0, BSP_LCD_GetXSize(), HEADBAND_HEIGHT);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_SetBackColor(LCD_COLOR_BLUE); 
  BSP_LCD_SetFont(&Font16);
  BSP_LCD_DisplayStringAt(0, 1, (uint8_t *)"SD", CENTER_MODE);
  BSP_LCD_SetFont(&Font12);
  BSP_LCD_DisplayStringAt(0, 20, (uint8_t *)"This example shows how to", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 35, (uint8_t *)"write and read on microSD", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 50, (uint8_t *)"Detect the card presence", CENTER_MODE);

   /* Set the LCD Text Color */
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  BSP_LCD_SetBackColor(LCD_COLOR_WHITE); 
 }

/**
  * @brief  Display SD Demo Hint
  * @param  None
  * @retval None
  */
static void SD_DMA_SetHint(void)
{
  /* Clear the LCD */ 
  BSP_LCD_Clear(LCD_COLOR_WHITE);
  
  /* Set LCD Demo description */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_FillRect(0, 0, BSP_LCD_GetXSize(), HEADBAND_HEIGHT);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_SetBackColor(LCD_COLOR_BLUE); 
  BSP_LCD_SetFont(&Font16);
  BSP_LCD_DisplayStringAt(0, 1, (uint8_t *)"SD DMA Mode", CENTER_MODE);
  BSP_LCD_SetFont(&Font12);
  BSP_LCD_DisplayStringAt(0, 20, (uint8_t *)"This example shows how to", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 35, (uint8_t *)"write and read on microSD", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 50, (uint8_t *)"Detect the card presence", CENTER_MODE);

   /* Set the LCD Text Color */
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
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
  * @brief Tx Transfer completed callbacks
  * @param none
  * @retval None
  */
void BSP_SD_WriteCpltCallback()
{
  SDWriteStatus = 1;
}

/**
  * @brief Rx Transfer completed callbacks
  * @param none
  * @retval None
  */
void BSP_SD_ReadCpltCallback()
{
  SDReadStatus = 1;
} 

/**
  * @}
  */ 

/**
  * @}
  */ 
