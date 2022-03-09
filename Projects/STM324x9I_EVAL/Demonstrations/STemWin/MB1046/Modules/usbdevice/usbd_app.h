/**
  ******************************************************************************
  * @file    usbd_app.h
  * @author  MCD Application Team
  * @brief   Header for usbd_app.c file
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USBD_APP_H
#define __USBD_APP_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usbd_core.h"
#include "usbd_desc.h"
#include "usbd_msc.h"
#include "usbd_storage.h"
#include "stm324x9i_eval.h" 
#include "stm324x9i_eval_sd.h"
   
/* Exported constants --------------------------------------------------------*/   
#define CONNECTED                  0x00
#define DISCONNECTED               0x01
   
/* Exported types ------------------------------------------------------------*/

typedef enum
{
  USBD_ERROR_NONE = 0, 
  USBD_ERROR_IO,
  USBD_ERROR_HW, 
  USBD_ERROR_UNKNOWN, 
  
}USBD_ErrorTypdef;


/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
USBD_ErrorTypdef  USBDSTOR_Start(void);
USBD_ErrorTypdef  USBDSTOR_Connect(void);
USBD_ErrorTypdef  USBDSTOR_Refrech(void);
USBD_ErrorTypdef  USBDSTOR_Stop(void);
USBD_ErrorTypdef  USBDSTOR_Disconnect(void);

#ifdef __cplusplus
}
#endif

#endif /*__USBD_APP_H */
