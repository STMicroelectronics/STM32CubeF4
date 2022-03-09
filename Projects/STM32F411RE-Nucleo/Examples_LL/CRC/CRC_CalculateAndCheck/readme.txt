/**
  @page CRC_CalculateAndCheck CRC : CRC calculation and computed CRC value checking
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    Examples_LL/CRC/CRC_CalculateAndCheck/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the CRC_CalculateAndCheck example.
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
   @endverbatim

@par Example Description

This example shows how to configure CRC calculation unit to get a CRC code of a given data buffer,
based on a fixed generator polynomial (default value 0x4C11DB7). Peripheral initialization done 
using LL unitary services functions for optimization purpose (performance and size).

CRC Peripheral is configured to work with default polynomial value (32-bit long).
Normal representation of this polynomial value is :
 X^32 + X^26 + X^23 + X^22 + X^16 + X^12 + X^11 + X^10 +X^8 + X^7 + X^5 + X^4 + X^2 + X + 1.
Generated CRC value is then 32 bits long.

Example execution:
After startup from reset and system configuration, CRC configuration is performed.
CRC code of a given data buffer is computed. 
Data buffer length has been chosen as an exact nb of u32 (32-bit words).
Once calculated, CRC value (uwCRCValue) is compared to the CRC expected value (uwExpectedCRCValue),
and if both are equal, LED2 is turned On. 
In case of errors, LED2 is blinking (1sec period).

@par Keywords

Security, CRC, CRC Polynomial, IEC 60870-5, hardware CRC, user-defined, generating polynomial, CRC Calculate

@par Directory contents 

  - CRC/CRC_CalculateAndCheck/Inc/stm32f4xx_it.h          Interrupt handlers header file
  - CRC/CRC_CalculateAndCheck/Inc/main.h                  Header for main.c module
  - CRC/CRC_CalculateAndCheck/Inc/stm32_assert.h          Template file to include assert_failed function
  - CRC/CRC_CalculateAndCheck/Src/stm32f4xx_it.c          Interrupt handlers
  - CRC/CRC_CalculateAndCheck/Src/main.c                  Main program
  - CRC/CRC_CalculateAndCheck/Src/system_stm32f4xx.c      STM32F4xx system source file


@par Hardware and Software environment

  - This example runs on STM32F411xx devices.
    
  - This example has been tested with NUCLEO-F411RE board and can be
    easily tailored to any other supported device and development board.

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example


  */
