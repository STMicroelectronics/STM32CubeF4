/**
  ******************************************************************************
  * @file    hardware_rng.c
  * @author  MCD Application Team
  * @brief   mbedtls entropy data generator using the HAL_RNG API.
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

#include "mbedtls_config.h"

#ifdef MBEDTLS_ENTROPY_HARDWARE_ALT

#include "main.h"
#include <string.h>

#include "mbedtls/entropy_poll.h"

extern RNG_HandleTypeDef RngHandle;

int mbedtls_hardware_poll( void *Data, unsigned char *Output, size_t Len, size_t *oLen )
{
  uint32_t index;
  uint32_t randomValue;
		
  for (index = 0; index < Len/4; index++)
  {
    if (HAL_RNG_GenerateRandomNumber(&RngHandle, &randomValue) == HAL_OK)
    {
      *oLen += 4;
      memset(&(Output[index * 4]), (int)randomValue, 4);
    }
    else
    {
      Error_Handler();
    }
  }
  
  return 0;
}


#endif /*MBEDTLS_ENTROPY_HARDWARE_ALT*/
