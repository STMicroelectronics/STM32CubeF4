/**
******************************************************************************
* @file    BSP/Src/qspi.c 
* @author  MCD Application Team
* @brief   This example code shows how to use the QSPI Driver
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
#define BUFFER_SIZE         ((uint32_t)0x0200)
#define WRITE_READ_ADDR     ((uint32_t)0x0050)
#define HEADBAND_HEIGHT     64
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t qspi_aTxBuffer[BUFFER_SIZE];
uint8_t qspi_aRxBuffer[BUFFER_SIZE];

/* Private function prototypes -----------------------------------------------*/
static void     QSPI_SetHint(void);
static void     Fill_Buffer (uint8_t *pBuffer, uint32_t uwBufferLength, uint32_t uwOffset);
static uint8_t  Buffercmp   (uint8_t* pBuffer1, uint8_t* pBuffer2, uint32_t BufferLength);

/* Private functions ---------------------------------------------------------*/

/**
* @brief  QSPI Demo
* @param  None
* @retval None
*/
void QSPI_demo (void)
{ 
  /* QSPI info structure */
  static QSPI_Info pQSPI_Info;
  uint8_t status;
  
  QSPI_SetHint();
  
  /*##-1- Configure the QSPI device ##########################################*/
  /* QSPI device configuration */ 
  status = BSP_QSPI_Init();
  
  if ((status == QSPI_NOT_SUPPORTED) || (status == QSPI_ERROR))
  {
    BSP_LCD_DisplayStringAt(5, 70, (uint8_t*)"QSPI Init : FAILED", LEFT_MODE);
    BSP_LCD_DisplayStringAt(5, 85, (uint8_t*)"QSPI Test Aborted", LEFT_MODE);
  }
  else
  {
    BSP_LCD_DisplayStringAt(5, 70, (uint8_t*)"QSPI Init : OK", LEFT_MODE);
    
    /*##-2- Read & check the QSPI info #######################################*/
    /* Initialize the structure */
    pQSPI_Info.FlashSize        = (uint32_t)0x00;
    pQSPI_Info.EraseSectorSize    = (uint32_t)0x00;
    pQSPI_Info.EraseSectorsNumber = (uint32_t)0x00;
    pQSPI_Info.ProgPageSize       = (uint32_t)0x00;
    pQSPI_Info.ProgPagesNumber    = (uint32_t)0x00;
    
    /* Read the QSPI memory info */
    BSP_QSPI_GetInfo(&pQSPI_Info);
    
    /* Test the correctness */
    if((pQSPI_Info.FlashSize != 0x1000000) || (pQSPI_Info.EraseSectorSize != 0x1000)  ||
       (pQSPI_Info.ProgPageSize != 0x100)  || (pQSPI_Info.EraseSectorsNumber != 4096) ||
       (pQSPI_Info.ProgPagesNumber != 65536))
    {
      BSP_LCD_DisplayStringAt(5, 85, (uint8_t*)"QSPI GET INFO : FAILED", LEFT_MODE);
      BSP_LCD_DisplayStringAt(5, 100, (uint8_t*)"QSPI Test Aborted", LEFT_MODE);
    }
    else
    {
      BSP_LCD_DisplayStringAt(5, 85, (uint8_t*)"QSPI GET INFO : OK", LEFT_MODE);
      
      /*##-3- Erase QSPI memory ################################################*/ 
      if(BSP_QSPI_Erase_Block(WRITE_READ_ADDR) != QSPI_OK)
      {
        BSP_LCD_DisplayStringAt(5, 100, (uint8_t*)"QSPI ERASE : FAILED", LEFT_MODE);
        BSP_LCD_DisplayStringAt(5, 115, (uint8_t*)"QSPI Test Aborted", LEFT_MODE);
      }
      else
      {
        BSP_LCD_DisplayStringAt(5, 100, (uint8_t*)"QSPI ERASE : OK", LEFT_MODE);
        
        /*##-4- QSPI memory read/write access  #################################*/   
        /* Fill the buffer to write */
        Fill_Buffer(qspi_aTxBuffer, BUFFER_SIZE, 0xD20F);   
        
        /* Write data to the QSPI memory */
        if(BSP_QSPI_Write(qspi_aTxBuffer, WRITE_READ_ADDR, BUFFER_SIZE) != QSPI_OK)
        {
          BSP_LCD_DisplayStringAt(5, 115, (uint8_t*)"QSPI WRITE : FAILED", LEFT_MODE);
          BSP_LCD_DisplayStringAt(5, 130, (uint8_t*)"QSPI Test Aborted", LEFT_MODE);
        }
        else
        {
          BSP_LCD_DisplayStringAt(5, 115, (uint8_t*)"QSPI WRITE : OK", LEFT_MODE);
          
          /* Read back data from the QSPI memory */
          if(BSP_QSPI_Read(qspi_aRxBuffer, WRITE_READ_ADDR, BUFFER_SIZE) != QSPI_OK)
          {
            BSP_LCD_DisplayStringAt(5, 130, (uint8_t*)"QSPI READ : FAILED", LEFT_MODE);
            BSP_LCD_DisplayStringAt(5, 145, (uint8_t*)"QSPI Test Aborted", LEFT_MODE);
          }
          else
          {
            BSP_LCD_DisplayStringAt(5, 130, (uint8_t*)"QSPI READ : OK", LEFT_MODE);
            
            /*##-5- Checking data integrity ############################################*/  
            if(Buffercmp(qspi_aRxBuffer, qspi_aTxBuffer, BUFFER_SIZE) > 0)
            {
              BSP_LCD_DisplayStringAt(5, 145, (uint8_t*)"QSPI COMPARE : FAILED", LEFT_MODE);
              BSP_LCD_DisplayStringAt(5, 160, (uint8_t*)"QSPI Test Aborted", LEFT_MODE);
            }
            else
            {    
              BSP_LCD_DisplayStringAt(5, 145, (uint8_t*)"QSPI Test : OK", LEFT_MODE);
            }  
          }
        }
      }
    }
  }
  
  while (1)
  {    
    if(CheckForUserInput() > 0)
    {
      BSP_QSPI_DeInit();
      return;
    }
  }
}

/**
* @brief  Display QSPI Demo Hint
* @param  None
* @retval None
*/
static void QSPI_SetHint(void)
{
  /* Clear the LCD */ 
  BSP_LCD_Clear(LCD_COLOR_WHITE);
  
  /* Set LCD Demo description */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_FillRect(0, 0, BSP_LCD_GetXSize(), HEADBAND_HEIGHT);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_SetBackColor(LCD_COLOR_BLUE); 
  BSP_LCD_SetFont(&Font16);
  BSP_LCD_DisplayStringAt(0, 1, (uint8_t*)"QSPI", CENTER_MODE);
  BSP_LCD_SetFont(&Font12);
  BSP_LCD_DisplayStringAt(0, 20, (uint8_t*)"This example shows how to", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 35, (uint8_t*)"write and read data on", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 50, (uint8_t*)"QSPI flash memory", CENTER_MODE);
  
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
static void Fill_Buffer(uint8_t *pBuffer, uint32_t uwBufferLenght, uint32_t uwOffset)
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
static uint8_t Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint32_t BufferLength)
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
