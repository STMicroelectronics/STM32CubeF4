/**
  ******************************************************************************
  * @file    vncserver_app.h
  * @author  MCD Application Team
  * @brief   VNC application header file
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
#ifndef __VNCSERVER_APP_H
#define __VNCSERVER_APP_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Network address type*/
#define IP_ADDRESS                 (uint8_t) 1
#define SUBNET_MASK                (uint8_t) 2
#define GW_ADDRESS                 (uint8_t) 3
   
#define IP_ADDR3           192        
#define IP_ADDR2           168
#define IP_ADDR1           0
#define IP_ADDR0           10

#define NETMASK_ADDR3      255
#define NETMASK_ADDR2      255
#define NETMASK_ADDR1      255
#define NETMASK_ADDR0      0

#define GW_ADDR3           192
#define GW_ADDR2           168
#define GW_ADDR1           0
#define GW_ADDR0           1
   
/* DHCP process states */
#define DHCP_START                 (uint8_t) 1
#define DHCP_WAIT_ADDRESS          (uint8_t) 2
#define DHCP_ADDRESS_ASSIGNED      (uint8_t) 3
#define DHCP_TIMEOUT               (uint8_t) 4
#define DHCP_LINK_DOWN             (uint8_t) 5
#define DHCP_STOP                  (uint8_t) 6

/* notifications from App to Server */
#define NOTIFY_SERVER_NETIF_UP                (uint8_t) 1
#define NOTIFY_SERVER_NETIF_DOWN              (uint8_t) 2
#define NOTIFY_SERVER_DHCP_START              (uint8_t) 3
#define NOTIFY_SERVER_DHCP_WAIT_ADDRESS       (uint8_t) 4
#define NOTIFY_SERVER_DHCP_ADDRESS_ASSIGNED   (uint8_t) 5
#define NOTIFY_SERVER_DHCP_TIMEOUT            (uint8_t) 6
   
/* notifications from Server to App */
#define NOTIFY_APP_DHCP_ENABLED                (uint8_t) 10
#define NOTIFY_APP_DHCP_DISABLED               (uint8_t) 11
#define NOTIFY_APP_IPADDRESS_UPDATE            (uint8_t) 12

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void _VNCApp_GetIPAddress(uint8_t type, uint8_t addr3, uint8_t addr2, uint8_t addr1, uint8_t addr0);
void _VNCServer_GetAssignedAddress(uint8_t type, uint8_t addr3, uint8_t addr2, uint8_t addr1, uint8_t addr0);
void NetworkInit(uint8_t use_dhcp);
void DHCP_thread(void const * argument);
void _VNCServer_Notify(uint8_t ID);
void _VNCApp_Notify(uint8_t ID);


#ifdef __cplusplus
}
#endif

#endif /* __VNCSERVER_APP_H */
