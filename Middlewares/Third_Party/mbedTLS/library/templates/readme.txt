
  @verbatim
  ******************************************************************************
  *
  *   COPYRIGHT (C) 2018 STMicroelectronics
  *
  * @file    readme.txt
  * @author  MCD Application Team
  * @brief   This file describes the content of the "templates" directory
  ******************************************************************************
  *
  * original licensing conditions
  * as issued by SPDX-License-Identifier: Apache-2.0
  *
  ******************************************************************************
  @endverbatim

This file contains template files that provide some alternate implementation for
mbedTLS algorithms.

aes_alt_template.[c/h]
-------------------------
Implements the mbedTLS AES crypto symmetric algorithm using the HAL/CRYP API.
As it is generic on ST families, you may have to :
- fill the appropriate hal header file.
for instance, stm32<xxxxx>_hal.h becomes stm32f4xx_hal.h for F4

- Make sure your mbed TLS config file enables the flag MBEDTLS_AES_ALT.

- Files need to be copied at user level and renamed to "aes_alt.[c/h]"

Note this implementation is thread-safe ready and can be run from different threads.

net_sockets_template.c
-------------------------
implements of the mbedTLS networking API using the LwIP TCP/IP Stack.
This file implements the strict minimum required to ensure TCP/IP connection.
This file need to be copied at user level and renamed to "net_sockets.c"

rng_alt_tempate.c
---------------------
Implements the function mbedtls_hardware_poll(), required by mbedTLS to generate
random numbers. The function is using the HAL/RNG API to generate random number
using the rng hw IP.

threading_alt_template.[c/h]
-----------------------------
Implements the mutex management API required by mbedTLS, using the CMSIS-RTOS
V1 & V2 API

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */