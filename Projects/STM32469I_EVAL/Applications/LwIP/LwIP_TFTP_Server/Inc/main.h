/**
  ******************************************************************************
  * @file    LwIP/LwIP_TFTP_Server/Inc/main.h
  * @author  MCD Application Team
  * @brief   This file contains all the functions prototypes for the main.c
  *          file.
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
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "stm32469i_eval.h"
#include "stm32469i_eval_lcd.h"
#include "stm32469i_eval_sdram.h"
#include "stm32469i_eval_io.h"
#include "stm32469i_eval_sd.h"

/* FatFs includes component */
#include "ff_gen_drv.h"
#include "sd_diskio.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

#define USE_LCD        /* enable LCD  */
#define USE_DHCP       /* enable DHCP, if disabled static address is used */

/*Static IP ADDRESS: IP_ADDR0.IP_ADDR1.IP_ADDR2.IP_ADDR3 */
#define IP_ADDR0   (uint8_t) 192
#define IP_ADDR1   (uint8_t) 168
#define IP_ADDR2   (uint8_t) 0
#define IP_ADDR3   (uint8_t) 10

/*NETMASK*/
#define NETMASK_ADDR0   (uint8_t) 255
#define NETMASK_ADDR1   (uint8_t) 255
#define NETMASK_ADDR2   (uint8_t) 255
#define NETMASK_ADDR3   (uint8_t) 0

/*Gateway Address*/
#define GW_ADDR0   (uint8_t) 192
#define GW_ADDR1   (uint8_t) 168
#define GW_ADDR2   (uint8_t) 0
#define GW_ADDR3   (uint8_t) 1

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
