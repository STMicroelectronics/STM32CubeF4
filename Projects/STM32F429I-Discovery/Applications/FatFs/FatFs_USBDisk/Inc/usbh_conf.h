/**
  ******************************************************************************
  * @file    FatFs/FatFs_USBDisk/Inc/usbh_conf.h
  * @author  MCD Application Team
  * @brief   General low level driver configuration
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USBH_CONF_H
#define __USBH_CONF_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Exported types ------------------------------------------------------------*/
#define USBH_MAX_NUM_ENDPOINTS                2
#define USBH_MAX_NUM_INTERFACES               2
#define USBH_MAX_NUM_CONFIGURATION            1
#define USBH_MAX_NUM_SUPPORTED_CLASS          1
#define USBH_KEEP_CFG_DESCRIPTOR              0
#define USBH_MAX_SIZE_CONFIGURATION           0x200
#define USBH_MAX_DATA_BUFFER                  0x200
#define USBH_DEBUG_LEVEL                      0
#define USBH_USE_OS                           0
    
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* CMSIS OS macros */   
#if (USBH_USE_OS == 1)
  #include "cmsis_os.h"
  #define   USBH_PROCESS_PRIO    osPriorityNormal
#endif

 /* Memory management macros */   
#define USBH_malloc               malloc
#define USBH_free                 free
#define USBH_memset               memset
#define USBH_memcpy               memcpy
    
 /* DEBUG macros */  
#if (USBH_DEBUG_LEVEL > 0)
#define  USBH_UsrLog(...)   printf(__VA_ARGS__);\
                            printf("\n");
#else
#define USBH_UsrLog(...)   
#endif 
                            
                            
#if (USBH_DEBUG_LEVEL > 1)

#define  USBH_ErrLog(...)   printf("ERROR: ") ;\
                            printf(__VA_ARGS__);\
                            printf("\n");
#else
#define USBH_ErrLog(...)   
#endif

#if (USBH_DEBUG_LEVEL > 2)                         
#define  USBH_DbgLog(...)   printf("DEBUG : ") ;\
                            printf(__VA_ARGS__);\
                            printf("\n");
#else
#define USBH_DbgLog(...)                         
#endif
                            
/* Exported functions ------------------------------------------------------- */

#endif /* __USB_CONF_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
