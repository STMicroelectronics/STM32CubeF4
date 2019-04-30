/**
  ******************************************************************************
  * @file    usbd_app.c
  * @author  MCD Application Team
  * @brief   This file provides the USBD application method.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics International N.V. 
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
#include "usbd_app.h"

/** @addtogroup USB_DEVICE_MODULE
  * @{
  */

/** @defgroup USB_APPLICATION
  * @brief usb application routines 
  * @{
  */

/* External variables --------------------------------------------------------*/
USBD_HandleTypeDef USBD_Device;

/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Start USBD
  * @param  None.
  * @retval USBD status.
  */
USBD_ErrorTypdef  USBDSTOR_Start(void)
{ 
  /* Init MSC Application */
  USBD_Init(&USBD_Device, &MSC_Desc, 0);
  
  /* Add Supported Class */
  USBD_RegisterClass(&USBD_Device, &USBD_MSC);
  
  /* Add Storage callbacks for MSC Class */
  USBD_MSC_RegisterStorage(&USBD_Device, &USBD_DISK_fops);
    
  return USBD_ERROR_NONE;
}
/**
  * @brief  Connect USBD
  * @param  None.
  * @retval USBD status.
  */
USBD_ErrorTypdef  USBDSTOR_Connect(void)
{
  /* Start Device Process */
  USBD_Start(&USBD_Device);
  
  return USBD_ERROR_NONE;
}

/**
  * @brief  Refresh USBD
  * @param  None.
  * @retval USBD status.
  */
USBD_ErrorTypdef  USBDSTOR_Refrech(void)
{
  return USBD_ERROR_NONE;
}

/**
  * @brief  Stop USBD
  * @param  None.
  * @retval USBD status.
  */
USBD_ErrorTypdef  USBDSTOR_Disconnect(void)
{
  USBD_Stop(&USBD_Device);  
  return USBD_ERROR_NONE;
}


/**
  * @brief  DeInit USBD
  * @param  None.
  * @retval USBD status.
  */
USBD_ErrorTypdef  USBDSTOR_Stop(void)
{
  USBD_DeInit(&USBD_Device);  
  return USBD_ERROR_NONE;
}
/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
