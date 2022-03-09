/**
  ******************************************************************************
  * @file    LwIP/LwIP_HTTP_Server_Netconn_RTOS/Inc/app_ethernet.h 
  * @author  MCD Application Team
  * @brief   Header for app_ethernet.c module
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
#ifndef __APP_ETHERNET_H
#define __APP_ETHERNET_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "lwip/netif.h"
   
#define USE_DHCP

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* DHCP process states */
   
   /*Static IP ADDRESS*/
#define IP_ADDR0   192
#define IP_ADDR1   168
#define IP_ADDR2   0
#define IP_ADDR3   10
   
/*NETMASK*/
#define NETMASK_ADDR0   255
#define NETMASK_ADDR1   255
#define NETMASK_ADDR2   255
#define NETMASK_ADDR3   0

/*Gateway Address*/
#define GW_ADDR0   192
#define GW_ADDR1   168
#define GW_ADDR2   0
#define GW_ADDR3   1 
   
#define VNC_IDLE                   (uint8_t) 0   
#define VNC_INIT                   (uint8_t) 9     
#define VNC_LINK_UP                (uint8_t) 1
#define VNC_WAIT_FOR_ADDRESS       (uint8_t) 2
#define VNC_ADDRESS_ASSIGNED       (uint8_t) 3
#define VNC_ERROR                  (uint8_t) 4
#define VNC_LINK_DOWN              (uint8_t) 5
#define VNC_PROCESS                (uint8_t) 6   
#define VNC_START                  (uint8_t) 7  
#define VNC_CONN_ESTABLISHED       (uint8_t) 8     
   
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void     VNC_SERVER_Start(void);
void     VNC_SERVER_Stop (void);
uint8_t  VNC_GetState(void);
void     VNC_SetState(uint8_t state);
void     VNC_SetLockState(uint8_t LockState);
uint8_t  VNC_GetLockState(void);
#ifdef __cplusplus
}
#endif

#endif /* __APP_ETHERNET_H */
