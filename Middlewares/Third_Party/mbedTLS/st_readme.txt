
  @verbatim
  ******************************************************************************
  *
  *         Portions COPYRIGHT 2016-2018 STMicroelectronics, All Rights Reserved
  *         Copyright (C) 2006-2015, ARM Limited, All Rights Reserved
  *
  * @file    st_readme.txt 
  * @author  MCD Application Team
  * @brief   This file lists the main modification done by STMicroelectronics on
  *          mbedTLS for integration with STM32Cube solution.
  ******************************************************************************
  *
  * original licensing conditions
  * as issued by SPDX-License-Identifier: Apache-2.0
  *
  ******************************************************************************
  @endverbatim

### 17-Jan-2020 ###
========================
   + Add in template directory the files :
     - aes_alt_template.[c/h] : mbed TLS AES Hw crypto using HAL/CRYP API

### 10-Dec-2019 ###
========================
   + Upgrade to use mbedTLS V2.16.2


### 21-Dec-2018 ###
========================
   + Upgrade to use mbedTLS V2.14.1

   + Fix Keil compiler warning

   + Update timing_alt_template.c : Protect the mbedtls alternate timing APIs with the flag MBEDTLS_TIMING_ALT

### 06-July-2018 ###
========================
   + Upgrade to use mbedTLS V2.11.0

   + Re-apply the fix for EWARM warnings due to invalid code for 32 bit machines 
     (still not integrated  see https://github.com/ARMmbed/mbedtls/issues/683)

   + add template directory containing the files:
     - net_socket_template.c        : mbedtls networking routines implemented using LwIP API
     - rng_alt_template.c           : mbedtls RNG routing using the HAL_RNG API
     - threading_alt_template.[c/h] : mbedtls mutex management template implementation 
                                      using the CMSIS-RTOS v1/v2 API
     - timing_alt.[c/h]             : mbedTLS alternate timing API based on
                                      CMSIS-RTOS v1/v2 API


### 10-November-2017 ###
========================
   + Upgrade to use mbedTLS V2.6.1
   + fix missing prototype error,in the library/aes.c file, with EWARM toolchain when MBEDTLS_DEPRECATED_REMOVED is defined.


### 09-October-2017 ###
========================
   + Upgrade to use mbedTLS V2.6.0
   + Update the "net_socket_template.c" file to use LwIP API, this make it directly usable in the applications
   + Fix compiler warnings by including newer patches from the mbedTLS git tree


### 23-December-2016 ###
========================
   + Upgrade to use mbedTLS V2.4.0
   + Rename the "net_sockets.c" to "net_sockets_template.c" to provide the net_sockets.c actual implementation in user projects.
   + Fix warnings due to invalid code for 32 bit machines, see https://github.com/ARMmbed/mbedtls/issues/683


### 27-March-2015 ###
=====================
   + Add support of the hardware Cryptographic and Hash processors embedded in STM32F756xx devices.
     This support is activated by defining "USE_STM32F7XX_HW_CRYPTO" macro in PolarSSL config.h file.
   + Fix some compilation warnings 


### 13-March-2015 ###
=====================
   + Align to latest version of STM32Cube CRYP HAL driver: add initialization of Crypto handler's Instance field


### 18-February-2014 ###
========================
   + PolarSSL V1.2.8 customized version for STM32Cube solution.
   + Source files modified to support the hardware Cryptographic and Hash processors embedded in
     STM32F415xx/417xx/437xx/439xx devices. This support is activated by defining
     "USE_STM32F4XX_HW_CRYPTO" macro in PolarSSL config.h file.

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
