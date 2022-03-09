/**
  ******************************************************************************
  * @file    BSP/Src/eeprom.c 
  * @author  MCD Application Team
  * @brief   This example code shows how to manage an I2C M24LR64 
  *          EEPROM memory
  *          =================================================================== 
  *          Notes:
  *           - This driver is intended for STM32F4xx families devices only. 
  *           - The I2C EEPROM memory (M24LR64) is available on separate daughter 
  *             board ANT7-M24LR-A, which is not provided with the STM32F429I 
  *             DISCOVERY board.
  *             To use this driver you have to connect the ANT7-M24LR-A to CN3 
  *             connector of STM32F429I DISCOVERY board.
  *             Switch EE_M24LR64 needs to be enabled in your project.
  *          ===================================================================
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
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;

/* Private define ------------------------------------------------------------*/
#define EEPROM_FEATURES_NUM     2
#define BUFFER_SIZE1            (countof(Tx1Buffer)-1 + 9)
#define EEPROM_WRITE_ADDRESS1   0x50
#define EEPROM_READ_ADDRESS1    0x50

/* Private macro -------------------------------------------------------------*/
#define countof(a) (sizeof(a) / sizeof(*(a)))

/* Private variables ---------------------------------------------------------*/
extern uint8_t NbLoop;
static uint8_t EEPROMFeature = 0;
uint8_t EEPROMConnected =1;

/* Private function prototypes -----------------------------------------------*/
static void EEPROM_SetHint(void);
static void EEPROM_Show_Feature(uint8_t feature);
static TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  EEPROM Demo
  * @param  None
  * @retval None
  */
void EEPROM_demo(void)
{ 
  EEPROM_SetHint();
  EEPROMFeature = 0;
  
  EEPROM_Show_Feature(EEPROMFeature); 
  
  while (1)
  {
    if(CheckForUserInput() > 0)
    {
      if(++EEPROMFeature < EEPROM_FEATURES_NUM)
      {
        EEPROM_Show_Feature(EEPROMFeature); 
      }
      else
      {
        return;
      }
    }
    HAL_Delay(100);
  }
}

/**
  * @brief  Display EEPROM Demo Hint
  * @param  None
  * @retval None
  */
static void EEPROM_SetHint(void)
{
  /* Clear the LCD */ 
  BSP_LCD_Clear(LCD_COLOR_WHITE);
  
  /* Set LCD Demo description */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_FillRect(0, 0, BSP_LCD_GetXSize(), 80);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_SetBackColor(LCD_COLOR_BLUE); 
  BSP_LCD_SetFont(&Font24);
  BSP_LCD_DisplayStringAt(0, 0, (uint8_t*)"EEPROM", CENTER_MODE);
  BSP_LCD_SetFont(&Font12);
  BSP_LCD_DisplayStringAt(0, 30, (uint8_t*)"This example shows the different", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 45, (uint8_t*)"EEPROM Features, use BUTTON", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 60, (uint8_t*)"to start EEPROM data transfer", CENTER_MODE);
  
  /* Set the LCD Text Color */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);  
  BSP_LCD_DrawRect(10, 90, BSP_LCD_GetXSize() - 20, BSP_LCD_GetYSize()- 100);
  BSP_LCD_DrawRect(11, 91, BSP_LCD_GetXSize() - 22, BSP_LCD_GetYSize()- 102);
}

/**
  * @brief  Show EEPROM Features
  * @param  feature: feature index
  * @retval None
  */
static void EEPROM_Show_Feature(uint8_t feature)
{
  uint8_t Tx1Buffer[] = "STM32F429I EEPROM Ex.";
  uint8_t Rx1Buffer[BUFFER_SIZE1] = {0}; 
  uint8_t Tx2Buffer[BUFFER_SIZE1] = {0}; 
  __IO TestStatus TransferStatus1 = FAILED;
  __IO uint16_t NumDataRead = 0; 
  
  BSP_LCD_SetBackColor(LCD_COLOR_WHITE);  
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);    
  BSP_LCD_FillRect(12, 92, BSP_LCD_GetXSize() - 24, BSP_LCD_GetYSize()- 104);
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  
  /* Initialize the I2C EEPROM driver ----------------------------------------*/
  if (BSP_EEPROM_Init() != EEPROM_OK)
  {
    BSP_LCD_SetTextColor(LCD_COLOR_RED);    
    BSP_LCD_DisplayStringAt(0, 115, (uint8_t*)"Initialization problem", CENTER_MODE);
    BSP_LCD_DisplayStringAt(0, 130, (uint8_t*)"Check if HW connected or", CENTER_MODE);
    BSP_LCD_DisplayStringAt(0, 145, (uint8_t*)"HW version not supported", CENTER_MODE);
    return;
  }
  EEPROMConnected = 1;
  
  /* Wait for EEPROM standby state */
  BSP_EEPROM_WaitEepromStandbyState();  
  
  switch (feature)
  {
  case 0:
    /* Read old parameter in EEPROM */
    if (EEPROMConnected == 1)
    {
      /* Set the Number of data to be read */
      NumDataRead = BUFFER_SIZE1;
      
      /* Read from I2C EEPROM from EEPROM_READ_ADDRESS1 */
      if (BSP_EEPROM_ReadBuffer(Rx1Buffer, EEPROM_READ_ADDRESS1, (uint16_t *)(&NumDataRead)) != EEPROM_OK)
      {
        BSP_LCD_SetTextColor(LCD_COLOR_RED);    
        BSP_LCD_DisplayStringAt(0, 115, (uint8_t*)"Init issue at read old data", CENTER_MODE);
        BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
        BSP_LCD_DisplayStringAt(0, 145, (uint8_t*)"Press again USER key", CENTER_MODE);
        BSP_LCD_DisplayStringAt(0, 160, (uint8_t*)"To write new data", CENTER_MODE);
        return;
      }
      BSP_LCD_DisplayStringAt(0, 115, (uint8_t*)"String read", CENTER_MODE);
      BSP_LCD_DisplayStringAt(0, 130, (uint8_t*)"in EEPROM:", CENTER_MODE);
      BSP_LCD_SetTextColor(LCD_COLOR_BLUE);    
      BSP_LCD_DisplayStringAt(0, 160, Rx1Buffer, CENTER_MODE); 
      BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
      BSP_LCD_DisplayStringAt(0, 190, (uint8_t*)"Press USER key", CENTER_MODE);
      BSP_LCD_DisplayStringAt(0, 205, (uint8_t*)"To write new data", CENTER_MODE);
    }
    else
    {
      BSP_LCD_SetTextColor(LCD_COLOR_RED);    
      BSP_LCD_DisplayStringAt(0, 115, (uint8_t*)"Problem to communicate", CENTER_MODE);
      BSP_LCD_DisplayStringAt(0, 130, (uint8_t*)"with EEPROM", CENTER_MODE);
      BSP_LCD_DisplayStringAt(0, 145, (uint8_t*)"Press again USER key", CENTER_MODE);
    }
    break;
    
  case 1:
    /* Write new parameter in EEPROM */
    if (EEPROMConnected == 1)
    {
      snprintf((char*)Tx2Buffer, BUFFER_SIZE1, "%s test %d", Tx1Buffer, NbLoop);
      /* First write in the memory followed by a read of the written data --------*/
      /* Write on I2C EEPROM to EEPROM_WRITE_ADDRESS1 */
      if (BSP_EEPROM_WriteBuffer(Tx2Buffer, EEPROM_WRITE_ADDRESS1, BUFFER_SIZE1) != EEPROM_OK)
      {
        BSP_LCD_SetTextColor(LCD_COLOR_RED);    
        BSP_LCD_DisplayStringAt(0, 115, (uint8_t*)"Init issue at write", CENTER_MODE);
        return;
      }
      
      /* Wait for EEPROM standby state */
      BSP_EEPROM_WaitEepromStandbyState();  
      
      /* Set the Number of data to be read */
      NumDataRead = BUFFER_SIZE1;
      
      /* Read from I2C EEPROM from EEPROM_READ_ADDRESS1 */
      if (BSP_EEPROM_ReadBuffer(Rx1Buffer, EEPROM_READ_ADDRESS1, (uint16_t *)(&NumDataRead)) != EEPROM_OK)
      {
        BSP_LCD_SetTextColor(LCD_COLOR_RED);    
        BSP_LCD_DisplayStringAt(0, 115, (uint8_t*)"Init issue at read", CENTER_MODE);
        return;
      }
      
      /* Check if the data written to the memory is read correctly */
      TransferStatus1 = Buffercmp(Tx2Buffer, Rx1Buffer, BUFFER_SIZE1);
      if(TransferStatus1 != FAILED)
      {
        BSP_LCD_DisplayStringAt(0, 115, (uint8_t*)"String writes", CENTER_MODE);
        BSP_LCD_DisplayStringAt(0, 130, (uint8_t*)"in EEPROM:", CENTER_MODE);
        BSP_LCD_SetTextColor(LCD_COLOR_BLUE);    
        BSP_LCD_DisplayStringAt(0, 160, Tx2Buffer, CENTER_MODE); 
      }
      else 
      {
        BSP_LCD_SetTextColor(LCD_COLOR_RED);    
        BSP_LCD_DisplayStringAt(0, 115, (uint8_t*)"FAILED to write!", CENTER_MODE); 
        BSP_LCD_DisplayStringAt(0, 130, (uint8_t*)"Press USER key to end test", CENTER_MODE); 
      }
    }
    else
    {
      BSP_LCD_SetTextColor(LCD_COLOR_RED);    
      BSP_LCD_DisplayStringAt(0, 115, (uint8_t*)"Problem to communicate", CENTER_MODE);
      BSP_LCD_DisplayStringAt(0, 130, (uint8_t*)"again with EEPROM", CENTER_MODE);
      BSP_LCD_DisplayStringAt(0, 130, (uint8_t*)"Press USER key to end test", CENTER_MODE);
    }
    break;
  }
}

/**
  * @brief  Basic management of the timeout situation.
  * @param  None
  * @retval None
  */
void BSP_EEPROM_TIMEOUT_UserCallback(void)
{
  EEPROMConnected = 0;
}

/**
  * @brief  Compares two buffers.
  * @param  pBuffer1, pBuffer2: buffers to be compared.
  * @param  BufferLength: buffer's length
  * @retval PASSED: pBuffer1 identical to pBuffer2
  *         FAILED: pBuffer1 differs from pBuffer2
  */
static TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength)
{
  while(BufferLength--)
  {
    if(*pBuffer1 != *pBuffer2)
    {
      return FAILED;
    }
    
    pBuffer1++;
    pBuffer2++;
  }
  
  return PASSED;  
}

/**
  * @}
  */ 

/**
  * @}
  */ 
