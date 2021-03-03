/**
  ******************************************************************************
  * @file    es_wifi_io.h
  * @author  MCD Application Team
  * @brief   This file contains the functions prototypes for es_wifi IO operations.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
#ifndef __WIFI_IO__
#define __WIFI_IO__

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/
#define WIFI_RESET_MODULE()                do{\
                                            HAL_GPIO_WritePin(GPIOH, GPIO_PIN_1, GPIO_PIN_RESET );\
                                            HAL_Delay(10);\
                                            HAL_GPIO_WritePin( GPIOH, GPIO_PIN_1, GPIO_PIN_SET );\
                                            HAL_Delay(500);\
                                             }while(0);


#define WIFI_ENABLE_NSS()                  do{ \
                                             HAL_GPIO_WritePin( GPIOG, GPIO_PIN_11, GPIO_PIN_RESET );\
                                             HAL_Delay(10);\
                                             }while(0);

#define WIFI_DISABLE_NSS()                 do{ \
                                             HAL_GPIO_WritePin( GPIOG, GPIO_PIN_11, GPIO_PIN_SET );\
                                             HAL_Delay(10);\
                                             }while(0);

#define WIFI_IS_CMDDATA_READY()            (HAL_GPIO_ReadPin(GPIOG, GPIO_PIN_12) == GPIO_PIN_SET)

    

/* Exported functions ------------------------------------------------------- */ 
void    SPI_WIFI_MspInit(SPI_HandleTypeDef* hspi);
int8_t  SPI_WIFI_DeInit(void);
int8_t  SPI_WIFI_Init(void);
int16_t SPI_WIFI_ReceiveData(uint8_t *pData, uint16_t len, uint32_t timeout);
int16_t SPI_WIFI_SendData( uint8_t *pData, uint16_t len, uint32_t timeout);
void    SPI_WIFI_Delay(uint32_t Delay);
    
#ifdef __cplusplus
}
#endif

#endif /* __WIFI_IO__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/    
