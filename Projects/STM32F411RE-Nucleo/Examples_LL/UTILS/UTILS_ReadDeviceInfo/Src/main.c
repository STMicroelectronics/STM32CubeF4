/**
  ******************************************************************************
  * @file    Examples_LL/UTILS/UTILS_ReadDeviceInfo/Src/main.c
  * @author  MCD Application Team
  * @brief   This example describes how to read UID, Device ID and Revision ID 
  *          through the STM32F4xx UTILS LL API.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/** @addtogroup STM32F4xx_LL_Examples
  * @{
  */

/** @addtogroup UTILS_ReadDeviceInfo
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Buffer used for displaying different UTILS info */
uint8_t aShowDeviceID[30]    = {0};
uint8_t aShowRevisionID[30]  = {0};
uint8_t aShowCoordinate[40]  = {0};
uint8_t aShowWaferNumber[30] = {0};
uint8_t aShowLotNumber[30]   = {0};

/* Private function prototypes -----------------------------------------------*/
void     GetMCUInfo(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  /* Get different information available in the MCU */
  GetMCUInfo();
  
  /* Infinite loop */
  while (1)
  {
  }
}

/**
  * @brief  Get different information available in the MCU (Device ID, Revision ID & UID)
  * @param  None
  * @retval None
  */
void GetMCUInfo(void)
{
  register uint32_t size_string = 0, read_info = 0, read_info2 = 0;

  /* Display Device ID in string format */
  sprintf((char*)aShowDeviceID,"Device ID = 0x%lX", LL_DBGMCU_GetDeviceID());
  
  /* Display Revision ID in string format */
  sprintf((char*)aShowRevisionID,"Revision ID = 0x%lX", LL_DBGMCU_GetRevisionID());

  /* Display  X and Y coordinates on the wafer expressed in BCD format */
  sprintf((char*)aShowCoordinate,"X and Y coordinates = 0x%lX", LL_GetUID_Word0());
  
  /* Display Waver number and lot number in string format */
  read_info = LL_GetUID_Word1();
  read_info2 = LL_GetUID_Word2();
  sprintf((char*)aShowWaferNumber,"Wafer NB = 0x%X", (uint8_t)read_info);
  size_string = sprintf((char*)aShowLotNumber,"Lot NB = 0x%lX", read_info2);
  sprintf((char*)aShowLotNumber+size_string,"%lX", (read_info >> 8));
}

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
