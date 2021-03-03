/**
  ******************************************************************************
  * @file    LwIP/LwIP_HTTP_Server_Socket_RTOS/Inc/httpserver-socket.h
  * @author  MCD Application Team
  * @brief   header file for httpserver-socket.c
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
#ifndef __HTTPSERVER_SOCKET_H__
#define __HTTPSERVER_SOCKET_H__

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void http_server_socket_init(void);
void DynWebPage(int conn);

#endif /* __HTTPSERVER_SOCKET_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
