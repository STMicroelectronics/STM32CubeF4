/**
  ******************************************************************************
  * @file    usbd_app.c
  * @author  MCD Application Team
  * @brief   This file provides the USBD application method.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
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
