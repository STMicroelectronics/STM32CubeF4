/**
  ******************************************************************************
  * @file    BSP/Src/sram.c
  * @author  MCD Application Team
  * @brief   This example code shows how to use the SRAM Driver
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
#define SRAM_WRITE_READ_ADDR_OFFSET  ((uint32_t)0x0800)
#define SRAM_BUFFER_SIZE             ((uint32_t)2048)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint16_t sram_aTxBuffer[SRAM_BUFFER_SIZE];
uint16_t sram_aRxBuffer[SRAM_BUFFER_SIZE];
uint8_t ubSramWrite = 0, ubSramRead = 0, ubSramInit = 0, ubSramDmaWrite = 0, ubSramDmaRead = 0;
__IO uint32_t uwSramDMA_Transfer_Complete = 0;

/* Private function prototypes -----------------------------------------------*/
static void SRAM_SetHint(void);
static void Fill_Buffer(uint16_t *pBuffer, uint32_t uwBufferLenght, uint32_t uwOffset);
static uint8_t Buffercmp(uint16_t* pBuffer1, uint16_t* pBuffer2, uint16_t BufferLength);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  SRAM Demo
  * @param  None
  * @retval None
  */
void SRAM_demo (void)
{
  SRAM_SetHint();

  /*##-1- Configure the SRAM device ##########################################*/
  /* SRAM device configuration */
  if (BSP_SRAM_Init() != SRAM_OK)
  {
    ubSramInit++;
  }

  /*##-2- SRAM memory read/write access ######################################*/
  /* Fill the buffer to write */
  Fill_Buffer(sram_aTxBuffer, SRAM_BUFFER_SIZE, 0xC20F);

  /* Write data to the SRAM memory */
  if (BSP_SRAM_WriteData((SRAM_DEVICE_ADDR + SRAM_WRITE_READ_ADDR_OFFSET), sram_aTxBuffer, SRAM_BUFFER_SIZE) != SRAM_OK)
  {
    ubSramWrite++;
  }

  /* Read back data from the SRAM memory */
  if (BSP_SRAM_ReadData((SRAM_DEVICE_ADDR + SRAM_WRITE_READ_ADDR_OFFSET), sram_aRxBuffer, SRAM_BUFFER_SIZE) != SRAM_OK)
  {
    ubSramRead++;
  }

  /*##-3- Checking data integrity ############################################*/
  /* SDRAM initialization */
  BSP_SDRAM_Init();
  if (ubSramInit != 0)
  {
    BSP_LCD_DisplayStringAt(20, 100, (uint8_t *)"SRAM Initialization : FAILED.", LEFT_MODE);
    BSP_LCD_DisplayStringAt(20, 115, (uint8_t *)"SRAM Test Aborted.", LEFT_MODE);
  }
  else
  {
    BSP_LCD_DisplayStringAt(20, 100, (uint8_t *)"SRAM Initialization : OK.", LEFT_MODE);
  }
  if (ubSramWrite != 0)
  {
    BSP_LCD_DisplayStringAt(20, 115, (uint8_t *)"SRAM WRITE : FAILED.", LEFT_MODE);
    BSP_LCD_DisplayStringAt(20, 130, (uint8_t *)"SRAM Test Aborted.", LEFT_MODE);
  }
  else
  {
    BSP_LCD_DisplayStringAt(20, 115, (uint8_t *)"SRAM WRITE : OK.", LEFT_MODE);
  }
  if (ubSramRead != 0)
  {
    BSP_LCD_DisplayStringAt(20, 130, (uint8_t *)"SRAM READ : FAILED.", LEFT_MODE);
    BSP_LCD_DisplayStringAt(20, 145, (uint8_t *)"SRAM Test Aborted.", LEFT_MODE);
  }
  else
  {
    BSP_LCD_DisplayStringAt(20, 130, (uint8_t *)"SRAM READ : OK.", LEFT_MODE);
  }

  if (Buffercmp(sram_aRxBuffer, sram_aTxBuffer, SRAM_BUFFER_SIZE) > 0)
  {
    BSP_LCD_DisplayStringAt(20, 145, (uint8_t *)"SRAM COMPARE : FAILED.", LEFT_MODE);
    BSP_LCD_DisplayStringAt(20, 160, (uint8_t *)"SRAM Test Aborted.", LEFT_MODE);
  }
  else
  {
    BSP_LCD_DisplayStringAt(20, 145, (uint8_t *)"SRAM Test : OK.", LEFT_MODE);
  }

  /* Tempo to allow message display */
  HAL_Delay(300);

  /*##-4- SRAM memory DMAread/DMAwrite access ######################################*/
  /* Disable the LCD to avoid the refresh from the SRAM */
  if (BSP_SRAM_Init() != SRAM_OK)
  {
    ubSramInit++;
  }

  /* Fill the buffer to write with new values */
  Fill_Buffer(sram_aTxBuffer, SRAM_BUFFER_SIZE, 0x1234);

  uwSramDMA_Transfer_Complete = 0;

  /* Write data to the SRAM memory through DMA */
  if (BSP_SRAM_WriteData_DMA((SRAM_DEVICE_ADDR + SRAM_WRITE_READ_ADDR_OFFSET), sram_aTxBuffer, SRAM_BUFFER_SIZE) != SRAM_OK)
  {
    ubSramDmaWrite++;
  }

  /* Wait for the DMA write completion */
  while (uwSramDMA_Transfer_Complete == 0)
  {
    /* infinite loop */
  }

  /* Read back data from the SRAM memory */
  uwSramDMA_Transfer_Complete = 0;
  if (BSP_SRAM_ReadData_DMA((SRAM_DEVICE_ADDR + SRAM_WRITE_READ_ADDR_OFFSET), sram_aRxBuffer, SRAM_BUFFER_SIZE) != SRAM_OK)
  {
    ubSramDmaRead++;
  }

  /* Wait for the DMA write completion */
  while (uwSramDMA_Transfer_Complete == 0)
  {
    /* infinite loop */
  }

  /*##-5- Checking data integrity ############################################*/
  /* SDRAM initialization */
  BSP_SDRAM_Init();
  if (ubSramDmaWrite != 0)
  {
    BSP_LCD_DisplayStringAt(20, 175, (uint8_t *)"SRAM DMA WRITE : FAILED.", LEFT_MODE);
    BSP_LCD_DisplayStringAt(20, 190, (uint8_t *)"SRAM DMA Test Aborted.", LEFT_MODE);
  }
  else
  {
    BSP_LCD_DisplayStringAt(20, 175, (uint8_t *)"SRAM DMA WRITE : OK.", LEFT_MODE);
  }
  if (ubSramRead != 0)
  {
    BSP_LCD_DisplayStringAt(20, 190, (uint8_t *)"SRAM DMA READ : FAILED.", LEFT_MODE);
    BSP_LCD_DisplayStringAt(20, 205, (uint8_t *)"SRAM DMA Test Aborted.", LEFT_MODE);
  }
  else
  {
    BSP_LCD_DisplayStringAt(20, 190, (uint8_t *)"SRAM DMA READ : OK.", LEFT_MODE);
  }

  if (Buffercmp(sram_aRxBuffer, sram_aTxBuffer, SRAM_BUFFER_SIZE) > 0)
  {
    BSP_LCD_DisplayStringAt(20, 205, (uint8_t *)"SRAM DMA COMPARE : FAILED.", LEFT_MODE);
    BSP_LCD_DisplayStringAt(20, 220, (uint8_t *)"SRAM DMA Test Aborted.", LEFT_MODE);
  }
  else
  {
    BSP_LCD_DisplayStringAt(20, 205, (uint8_t *)"SRAM DMA Test : OK.", LEFT_MODE);
  }

  while (1)
  {
    if (CheckForUserInput() > 0)
    {
      return;
    }
  }
}

/**
  * @brief  Display SRAM Demo Hint
  * @param  None
  * @retval None
  */
static void SRAM_SetHint(void)
{
  /* Clear the LCD */
  BSP_LCD_Clear(LCD_COLOR_WHITE);

  /* Set LCD Demo description */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_FillRect(0, 0, BSP_LCD_GetXSize(), 80);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_SetBackColor(LCD_COLOR_BLUE);
  BSP_LCD_SetFont(&Font24);
  BSP_LCD_DisplayStringAt(0, 0, (uint8_t *)"SRAM", CENTER_MODE);
  BSP_LCD_SetFont(&Font12);
  BSP_LCD_DisplayStringAt(0, 30, (uint8_t *)"This example shows how to write", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 45, (uint8_t *)"and read data on the SRAM", CENTER_MODE);

  /* Set the LCD Text Color */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_DrawRect(10, 90, BSP_LCD_GetXSize() - 20, BSP_LCD_GetYSize() - 100);
  BSP_LCD_DrawRect(11, 91, BSP_LCD_GetXSize() - 22, BSP_LCD_GetYSize() - 102);

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
static void Fill_Buffer(uint16_t *pBuffer, uint32_t uwBufferLenght, uint32_t uwOffset)
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
  * @retval 0: pBuffer identical to pBuffer1
  *         1: pBuffer differs from pBuffer1
  */
static uint8_t Buffercmp(uint16_t* pBuffer1, uint16_t* pBuffer2, uint16_t BufferLength)
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
  * @brief  DMA transfer complete callback.
  * @param  hdma: pointer to a SRAM_HandleTypeDef structure that contains
  *                the configuration information for SRAM module.
  * @retval None
  */
void HAL_SRAM_DMA_XferCpltCallback(DMA_HandleTypeDef *hdma)
{
  uwSramDMA_Transfer_Complete++;
}

/**
  * @brief  DMA transfer complete error callback.
  * @param  hdma: pointer to a SRAM_HandleTypeDef structure that contains
  *                the configuration information for SRAM module.
  * @retval None
  */
void HAL_SRAM_DMA_XferErrorCallback(DMA_HandleTypeDef *hdma)
{
  BSP_LCD_SetTextColor(LCD_COLOR_RED);
  BSP_LCD_DisplayStringAt(20, 215, (uint8_t *)"DMA ERROR", LEFT_MODE);
}
/**
  * @}
  */

/**
  * @}
  */
