/**
  ******************************************************************************
  * @file    LwIP/LwIP_UDP_Echo_Client/Inc/udp_echoclient.h
  * @author  MCD Application Team
  * @brief   Header file for udp_echoclient.c
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
#ifndef __UDP_ECHOCLIENT_H__
#define __UDP_ECHOCLIENT_H__

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void udp_echoclient_connect(void);
void udp_echoclient_send(void);

#endif /* __UDP_ECHOCLIENT_H__ */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
