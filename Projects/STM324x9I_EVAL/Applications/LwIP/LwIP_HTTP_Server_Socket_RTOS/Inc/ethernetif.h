/**
  ******************************************************************************
  * @file    ethernetif.h
  * @author  MCD Application Team
  * @brief   Ethernet interface header file.
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

#ifndef __ETHERNETIF_H__
#define __ETHERNETIF_H__


#include "lwip/err.h"
#include "lwip/netif.h"
#include "cmsis_os.h"

/* Exported types ------------------------------------------------------------*/
/* Structure that include link thread parameters */
   struct link_str {
  struct netif *netif;
  osSemaphoreId semaphore;
};
/* Exported functions ------------------------------------------------------- */
err_t ethernetif_init(struct netif *netif);      
void ethernetif_set_link(void const *argument);
void ethernetif_update_config(struct netif *netif);
void ethernetif_notify_conn_changed(struct netif *netif);
#endif
