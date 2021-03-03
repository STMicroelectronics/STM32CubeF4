/**
  ******************************************************************************
  * @file    Wifi/WiFi_Client_Server/Inc/es_wifi_conf.h
  * @author  MCD Application Team
  * @brief   es-wifi configuration file.
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

#ifndef __ES_WIFI_CONF_H
#define __ES_WIFI_CONF_H

#ifdef __cplusplus
 extern "C" {
#endif  

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/
#define ES_WIFI_MAX_SSID_NAME_SIZE                  32
#define ES_WIFI_MAX_PSWD_NAME_SIZE                  32
#define ES_WIFI_PRODUCT_ID_SIZE                     32
#define ES_WIFI_PRODUCT_NAME_SIZE                   32
#define ES_WIFI_FW_REV_SIZE                         16
#define ES_WIFI_API_REV_SIZE                        16
#define ES_WIFI_STACK_REV_SIZE                      16
#define ES_WIFI_RTOS_REV_SIZE                       16

#define ES_WIFI_DATA_SIZE                           1600
#define ES_WIFI_MAX_DETECTED_AP                     10
   
#define ES_WIFI_TIMEOUT                             0xFFFF
                                                    
#define ES_WIFI_USE_PING                            1
#define ES_WIFI_USE_AWS                             0
#define ES_WIFI_USE_FIRMWAREUPDATE                  0
#define ES_WIFI_USE_WPS                             0
                                                    
#define ES_WIFI_USE_SPI                             1    
#define ES_WIFI_USE_UART                            (!ES_WIFI_USE_SPI)   
   


#ifdef __cplusplus
}
#endif
#endif /* __ES_WIFI_CONF_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
