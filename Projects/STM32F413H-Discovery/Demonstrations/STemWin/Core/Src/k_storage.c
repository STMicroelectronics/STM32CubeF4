/**
******************************************************************************
* @file    k_storage.c
* @author  MCD Application Team
* @brief   This file provides the kernel storage functions
******************************************************************************
* @attention
*
* <h2><center>&copy; Copyright © 2017 STMicroelectronics International N.V. 
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
#include "k_storage.h"

/** @addtogroup CORE
* @{
*/

/** @defgroup KERNEL_STORAGE
* @brief Kernel storage routines
* @{
*/


/* External variables --------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
FATFS mSDDISK_FatFs;         /* File system object for MSD disk logical drive */
char mSDDISK_Drive[4];       /* MSD Host logical drive number */
osMessageQId StorageEvent;
DIR dir;

extern uint32_t UserButton_press;
extern uint32_t calibration_data[3*2];

static uint32_t StorageStatus[NUM_DISK_UNITS];
/* Private function prototypes -----------------------------------------------*/
static void StorageThread(void const * argument);

/* Private functions ---------------------------------------------------------*/


/**
* @brief  Storage drives initialization
* @param  None 
* @retval None
*/
void k_StorageInit(void)
{  
  /* Link the micro SD disk I/O driver */
  FATFS_LinkDriver(&SD_Driver, mSDDISK_Drive);
  
  /* Create USB background task */
  osThreadDef(STORAGE_Thread, StorageThread, osPriorityLow, 0, 512);
  osThreadCreate (osThread(STORAGE_Thread), NULL);
  
  /* Create Storage Message Queue */
  osMessageQDef(osqueue, 10, uint16_t);
  StorageEvent = osMessageCreate (osMessageQ(osqueue), NULL);
  
  /* Enable SD Interrupt mode */
  BSP_SD_Init();
  BSP_SD_ITConfig();
  
  if(BSP_SD_IsDetected())
  {
    osMessagePut ( StorageEvent, MSDDISK_CONNECTION_EVENT, 0);
  }  
}

/**
* @brief  Storage Thread
* @param  argument: pointer that is passed to the thread function as start argument.
* @retval None
*/
static void StorageThread(void const * argument)
{
  osEvent event;
  
  for( ;; )
  {
    event = osMessageGet( StorageEvent, osWaitForever );
    
    if( event.status == osEventMessage )
    {
      switch(event.value.v)
      {        
      case MSDDISK_CONNECTION_EVENT:
        f_mount(&mSDDISK_FatFs, mSDDISK_Drive, 0);
        StorageStatus[MSD_DISK_UNIT] = 1;
        break;
        
      case MSDDISK_DISCONNECTION_EVENT:
        f_mount(0, mSDDISK_Drive, 0);
        StorageStatus[MSD_DISK_UNIT] = 0;        
        break;         
      }
    }
  }
}

/**
* @brief  SD detect callback
* @param  None
* @retval None
*/ 
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if(GPIO_Pin == TS_INT_PIN)
  {
      k_TouchUpdate();
  }
  else if(GPIO_Pin == SD_DETECT_PIN)
  {
    if((BSP_SD_IsDetected()))
    {  
      /* After sd disconnection, a SD Init is required */
      BSP_SD_Init();
      
      osMessagePut ( StorageEvent, MSDDISK_CONNECTION_EVENT, 0);
    }
    else
    {
      osMessagePut ( StorageEvent, MSDDISK_DISCONNECTION_EVENT, 0);
    }
  }
}

/**
* @brief  Storage get status
* @param  unit: logical storage unit index.
* @retval int
*/
uint8_t k_StorageGetStatus (uint8_t unit)
{  
  return StorageStatus[unit];
}

/**
* @}
*/

/**
* @}
*/

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
