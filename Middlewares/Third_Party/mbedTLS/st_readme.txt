
  @verbatim
  ******************************************************************************
  *
  *           Portions COPYRIGHT 2016 STMicroelectronics
  *           Copyright (C) 2006-2015, ARM Limited, All Rights Reserved
  *
  * @file    st_readme.txt 
  * @author  MCD Application Team
  * @brief   This file lists the main modification done by STMicroelectronics on
  *          mbedTLS for integration with STM32Cube solution. 
  ******************************************************************************
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  @endverbatim

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
