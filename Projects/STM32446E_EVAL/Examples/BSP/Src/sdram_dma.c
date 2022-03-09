/**
  ******************************************************************************
  * @file    BSP/Src/sdram_dma.c 
  * @author  MCD Application Team
  * @brief   This example code shows how to use the SDRAM Driver
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
#define BUFFER_SIZE            ((uint32_t)0x0100)
#define WRITE_READ_ADDR_OFFSET        ((uint32_t)0x0800)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static uint32_t sdram_aTxBuffer[BUFFER_SIZE];
static uint32_t sdram_aRxBuffer[BUFFER_SIZE];
extern SDRAM_HandleTypeDef sdramHandle;
//DMA_HandleTypeDef   hdma;
/* DMA transfer complete flag */
__IO uint32_t uwDMA_Transfer_Complete = 0;

/* Private function prototypes -----------------------------------------------*/
static void SDRAM_DMA_SetHint(void);
static void Fill_Buffer(uint32_t *pBuffer, uint32_t uwBufferLength, uint32_t uwOffset);
static uint8_t Buffercmp(uint32_t* pBuffer1, uint32_t* pBuffer2, uint16_t BufferLength);


/* Private functions ---------------------------------------------------------*/

/**
  * @brief  SDRAM Demo
  * @param  None
  * @retval None
  */
void SDRAM_DMA_demo (void)
{ 
  
  SDRAM_DMA_SetHint();

  /* -1- SDRAM device configuration */ 
  if(BSP_SDRAM_Init() != SDRAM_OK)
  {
    BSP_LCD_DisplayStringAt(20, 115, (uint8_t *)"SDRAM Initialization : FAILED.", LEFT_MODE);
    BSP_LCD_DisplayStringAt(20, 130, (uint8_t *)"SDRAM Test Aborted.", LEFT_MODE);
  }
  else
  {
    BSP_LCD_DisplayStringAt(20, 100, (uint8_t *)"SDRAM Initialization : OK.", LEFT_MODE);
  }

  /* -2- Fill the buffer to write */
  Fill_Buffer(sdram_aTxBuffer, BUFFER_SIZE, 0xA244250F);   
  
  /* -3- Write data to the SDRAM memory using DMA */
  uwDMA_Transfer_Complete = 0;
  if(BSP_SDRAM_WriteData_DMA((SDRAM_WRITE_READ_ADDR + WRITE_READ_ADDR_OFFSET), sdram_aTxBuffer, BUFFER_SIZE) != SDRAM_OK)
  {
    BSP_LCD_DisplayStringAt(20, 115, (uint8_t *)"SDRAM WRITE dma : FAILED.", LEFT_MODE);
    BSP_LCD_DisplayStringAt(20, 130, (uint8_t *)"SDRAM Test Aborted.", LEFT_MODE);
  }
  else
  { 
    BSP_LCD_DisplayStringAt(20, 115, (uint8_t *)"SDRAM WRITE DMA : REQUESTED", LEFT_MODE);
  }

  /* Wait until DMA transfer is complete */
  while (!uwDMA_Transfer_Complete)
  {
  }
  BSP_LCD_DisplayStringAt(20, 130, (uint8_t *)"SDRAM WRITE DMA : COMPLETED", LEFT_MODE);
 
  /* -4- Read back data from the SDRAM memory using DMA */
  uwDMA_Transfer_Complete = 0;

  if(BSP_SDRAM_ReadData_DMA(SDRAM_WRITE_READ_ADDR + WRITE_READ_ADDR_OFFSET, sdram_aRxBuffer, BUFFER_SIZE) != SDRAM_OK)
  {
    BSP_LCD_DisplayStringAt(20, 145, (uint8_t *)"SDRAM  READ DMA : FAILED", LEFT_MODE);
    BSP_LCD_DisplayStringAt(20, 160, (uint8_t *)"SDRAM  Test Aborted", LEFT_MODE);
  }
  else
  {
    BSP_LCD_DisplayStringAt(20, 145, (uint8_t *)"SDRAM READ  DMA : REQUESTED", LEFT_MODE);
  }

  /* Wait until DMA transfer is complete */
  while (!uwDMA_Transfer_Complete)
  {
  }

  BSP_LCD_DisplayStringAt(20, 160, (uint8_t *)"SDRAM READ  DMA : COMPLETED", LEFT_MODE);
  
  /* -5- Compare both buffers */
  if(Buffercmp(sdram_aTxBuffer, sdram_aRxBuffer, BUFFER_SIZE) > 0)
  {
    BSP_LCD_DisplayStringAt(20, 175, (uint8_t *)"SDRAM COMPARE : FAILED.", LEFT_MODE);
    BSP_LCD_DisplayStringAt(20, 190, (uint8_t *)"SDRAM Test Aborted.", LEFT_MODE);
  }
  else
  {
    BSP_LCD_DisplayStringAt(20, 175, (uint8_t *)"SDRAM TEST      : SUCCESSFUL", LEFT_MODE);
  }
 
  while (1)
  {    
    if(CheckForUserInput() > 0)
    {
      return;
    }
  }
}

/**
  * @brief  DMA conversion complete callback
  * @note   This function is executed when the transfer complete interrupt 
  *         is generated
  * @retval None
  */
void HAL_SDRAM_DMA_XferCpltCallback(DMA_HandleTypeDef *hdma)
{
  /* Set transfer complete flag */
	uwDMA_Transfer_Complete = 1;
}

/**
  * @brief  DMA transfer complete error callback.
  * @param  hdma: DMA handle
  * @retval None
  */

void HAL_SDRAM_DMA_XferErrorCallback(DMA_HandleTypeDef *hdma)
{
  BSP_LCD_SetTextColor(LCD_COLOR_RED);
  BSP_LCD_DisplayStringAt(20, 215, (uint8_t *)"DMA ERROR", LEFT_MODE);
}
/**
  * @brief  Display SDRAM Demo Hint
  * @param  None
  * @retval None
  */
static void SDRAM_DMA_SetHint(void)
{
  /* Clear the LCD */ 
  BSP_LCD_Clear(LCD_COLOR_WHITE);
  
  /* Set LCD Demo description */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_FillRect(0, 0, BSP_LCD_GetXSize(), 80);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_SetBackColor(LCD_COLOR_BLUE); 
  BSP_LCD_SetFont(&Font24);
  BSP_LCD_DisplayStringAt(0, 0, (uint8_t *)"SDRAM DMA", CENTER_MODE);
  BSP_LCD_SetFont(&Font12);
  BSP_LCD_DisplayStringAt(0, 30, (uint8_t *)"This example shows how to write", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 45, (uint8_t *)"and read data on SDRAM", CENTER_MODE);

   /* Set the LCD Text Color */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);  
  BSP_LCD_DrawRect(10, 90, BSP_LCD_GetXSize() - 20, BSP_LCD_GetYSize()- 100);
  BSP_LCD_DrawRect(11, 91, BSP_LCD_GetXSize() - 22, BSP_LCD_GetYSize()- 102);
  
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  BSP_LCD_SetBackColor(LCD_COLOR_WHITE); 
 }

/**
  * @brief  Fills buffer with user predefined data.
  * @param  pBuffer: pointer on the buffer to fill
  * @param  uwBufferLength: size of the buffer to fill
  * @param  uwOffset: first value to fill on the buffer
  * @retval None
  */
static void Fill_Buffer(uint32_t *pBuffer, uint32_t uwBufferLength, uint32_t uwOffset)
{
  uint32_t tmpIndex = 0;
  
  /* Put in global buffer different values */
  for (tmpIndex = 0; tmpIndex < uwBufferLength; tmpIndex++ )
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
