/**
  ******************************************************************************
  * @file    BSP/Src/sram.c
  * @author  MCD Application Team
  * @brief   This example code shows how to use the PSRAM Driver
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

/** @addtogroup STM32F7xx_HAL_Examples
  * @{
  */

/** @addtogroup BSP
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define BUFFER_SIZE         ((uint32_t)0x1000)
#define PSRAM_WRITE_READ_ADDR     ((uint32_t)0x0800)
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint16_t sram_aTxBuffer[BUFFER_SIZE];
uint16_t sram_aRxBuffer[BUFFER_SIZE];
uint8_t ubSramWrite = 0, ubSramRead = 0, ubSramInit = 0, ubCompare = 0;
__IO uint8_t  write_complete = 0;
/* Private function prototypes -----------------------------------------------*/
static void PSRAM_SetHint(void);
static void Fill_Buffer(uint16_t *pBuffer, uint32_t uwBufferLength, uint32_t uwOffset);
static uint8_t Buffercmp(uint16_t* pBuffer1, uint16_t* pBuffer2, uint16_t BufferLength);
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  PSRAM Demo
  * @param  None
  * @retval None
  */
void PSRAM_demo (void)
{
  PSRAM_SetHint();

  /* Disable the LCD to avoid the refrech from the SDRAM */
  BSP_LCD_DisplayOff();

  /*##-1- Configure the PSRAM device ##########################################*/
  /* PSRAM device configuration */
  if(BSP_PSRAM_Init() != PSRAM_OK)
  {
    ubSramInit++;
  }

  /*##-2- PSRAM memory read/write access ######################################*/
  /* Fill the buffer to write */
  Fill_Buffer(sram_aTxBuffer, BUFFER_SIZE, 0xC20F);

  /* Write data to the PSRAM memory */
  if(BSP_PSRAM_WriteData(PSRAM_DEVICE_ADDR + PSRAM_WRITE_READ_ADDR, sram_aTxBuffer, BUFFER_SIZE) != PSRAM_OK)
  {
    ubSramWrite++;
  }

  /* Read back data from the PSRAM memory */
  if(BSP_PSRAM_ReadData(PSRAM_DEVICE_ADDR + PSRAM_WRITE_READ_ADDR, sram_aRxBuffer, BUFFER_SIZE) != PSRAM_OK)
  {
    ubSramRead++;
  }

  /*##-3- Checking data integrity ############################################*/
  /* Enable the LCD */
  BSP_LCD_DisplayOn();

  if(ubSramInit != 0)
  {
    BSP_LCD_DisplayStringAt(20, 100, (uint8_t *)"PSRAM Initialization : FAILED.", LEFT_MODE);
    BSP_LCD_DisplayStringAt(20, 115, (uint8_t *)"PSRAM Test Aborted.", LEFT_MODE);
    ubSramInit = 0xFF;
  }
  else
  {
    BSP_LCD_DisplayStringAt(20, 100, (uint8_t *)"PSRAM Initialization : OK.", LEFT_MODE);
    ubSramInit = 0;
  }
  if(ubSramWrite != 0)
  {
    BSP_LCD_DisplayStringAt(20, 115, (uint8_t *)"PSRAM WRITE : FAILED.", LEFT_MODE);
    BSP_LCD_DisplayStringAt(20, 130, (uint8_t *)"PSRAM Test Aborted.", LEFT_MODE);
    ubSramWrite = 0xFF;
  }
  else
  {
    BSP_LCD_DisplayStringAt(20, 115, (uint8_t *)"PSRAM WRITE : OK.", LEFT_MODE);
    ubSramWrite = 0;
  }
  if(ubSramRead != 0)
  {
    BSP_LCD_DisplayStringAt(20, 130, (uint8_t *)"PSRAM READ : FAILED.", LEFT_MODE);
    BSP_LCD_DisplayStringAt(20, 145, (uint8_t *)"PSRAM Test Aborted.", LEFT_MODE);
    ubSramRead = 0xFF;
  }
  else
  {
    BSP_LCD_DisplayStringAt(20, 130, (uint8_t *)"PSRAM READ : OK.", LEFT_MODE);
    ubSramRead = 0;
  }

  if(Buffercmp(sram_aRxBuffer, sram_aTxBuffer, BUFFER_SIZE) > 0)
  {
    BSP_LCD_DisplayStringAt(20, 145, (uint8_t *)"PSRAM COMPARE : FAILED.", LEFT_MODE);
    BSP_LCD_DisplayStringAt(20, 160, (uint8_t *)"PSRAM Test Aborted.", LEFT_MODE);
    ubCompare ++;
  }
  else
  {
    BSP_LCD_DisplayStringAt(20, 145, (uint8_t *)"PSRAM Test : OK.", LEFT_MODE);
    ubCompare =0;
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
  * @brief  PSRAM Demo
  * @param  None
  * @retval None
  */
void PSRAM_DMA_demo (void)
{
  PSRAM_SetHint();

  /* Disable the LCD to avoid the refrech from the PSRAM */
  BSP_LCD_DisplayOff();

  /*##-1- Configure the PSRAM device ##########################################*/
  /* PSRAM device configuration */
  if(BSP_PSRAM_Init() != PSRAM_OK)
  {
    ubSramInit++;
  }

  /*##-2- PSRAM memory read/write access ######################################*/
  /* Fill the buffer to write */
  Fill_Buffer(sram_aTxBuffer, BUFFER_SIZE, 0xC20F);

  /* Write data to the PSRAM memory */
  if(BSP_PSRAM_WriteData_DMA(PSRAM_DEVICE_ADDR + PSRAM_WRITE_READ_ADDR, sram_aTxBuffer, BUFFER_SIZE) != PSRAM_OK)
  {
    ubSramWrite++;
  }


  /* Wait for Transfer complete */
  while(write_complete == 0);

  /* Read back data from the PSRAM memory */
  if(BSP_PSRAM_ReadData_DMA(PSRAM_DEVICE_ADDR + PSRAM_WRITE_READ_ADDR, sram_aRxBuffer, BUFFER_SIZE) != PSRAM_OK)
  {
    ubSramRead++;
  }

  /*##-3- Checking data integrity ############################################*/
  /* Enable the LCD */
  BSP_LCD_DisplayOn();

  if(ubSramInit != 0)
  {
    BSP_LCD_DisplayStringAt(20, 100, (uint8_t *)"PSRAM Initialization : FAILED.", LEFT_MODE);
    BSP_LCD_DisplayStringAt(20, 115, (uint8_t *)"PSRAM Test Aborted.", LEFT_MODE);
  }
  else
  {
    BSP_LCD_DisplayStringAt(20, 100, (uint8_t *)"PSRAM Initialization : OK.", LEFT_MODE);
  }
  if(ubSramWrite != 0)
  {
    BSP_LCD_DisplayStringAt(20, 115, (uint8_t *)"PSRAM WRITE : FAILED.", LEFT_MODE);
    BSP_LCD_DisplayStringAt(20, 130, (uint8_t *)"PSRAM Test Aborted.", LEFT_MODE);
  }
  else
  {
    BSP_LCD_DisplayStringAt(20, 115, (uint8_t *)"PSRAM WRITE : OK.", LEFT_MODE);
  }
  if(ubSramRead != 0)
  {
    BSP_LCD_DisplayStringAt(20, 130, (uint8_t *)"PSRAM READ : FAILED.", LEFT_MODE);
    BSP_LCD_DisplayStringAt(20, 145, (uint8_t *)"PSRAM Test Aborted.", LEFT_MODE);
  }
  else
  {
    BSP_LCD_DisplayStringAt(20, 130, (uint8_t *)"PSRAM READ : OK.", LEFT_MODE);
  }

  if(Buffercmp(sram_aRxBuffer, sram_aTxBuffer, BUFFER_SIZE) > 0)
  {
    BSP_LCD_DisplayStringAt(20, 145, (uint8_t *)"PSRAM COMPARE : FAILED.", LEFT_MODE);
    BSP_LCD_DisplayStringAt(20, 160, (uint8_t *)"PSRAM Test Aborted.", LEFT_MODE);
  }
  else
  {
    BSP_LCD_DisplayStringAt(20, 145, (uint8_t *)"PSRAM Test : OK.", LEFT_MODE);
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
  * @brief  Display PSRAM Demo Hint
  * @param  None
  * @retval None
  */
static void PSRAM_SetHint(void)
{
  /* Clear the LCD */
  BSP_LCD_Clear(LCD_COLOR_WHITE);

  /* Set LCD Demo description */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_FillRect(0, 0, BSP_LCD_GetXSize(), 80);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_SetBackColor(LCD_COLOR_BLUE);
  BSP_LCD_SetFont(&Font24);
  BSP_LCD_DisplayStringAt(0, 0, (uint8_t *)"PSRAM", CENTER_MODE);
  BSP_LCD_SetFont(&Font12);
  BSP_LCD_DisplayStringAt(0, 30, (uint8_t *)"This example shows how to write", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 45, (uint8_t *)"and read data on the PSRAM", CENTER_MODE);

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
static void Fill_Buffer(uint16_t *pBuffer, uint32_t uwBufferLength, uint32_t uwOffset)
{
  uint32_t tmpindex = 0;

  /* Put in global buffer different values */
  for (tmpindex = 0; tmpindex < uwBufferLength; tmpindex++ )
  {
    pBuffer[tmpindex] = tmpindex + uwOffset;
  }
}

/**
  * @brief  Compares two buffers.
  * @param  pBuffer1, pBuffer2: buffers to be compared.
  * @param  BufferLength: buffer's length
  * @retval 1: pBuffer identical to pBuffer1
  *         0: pBuffer differs from pBuffer1
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
  write_complete = 1;
}
/**
  * @}
  */

/**
  * @}
  */
