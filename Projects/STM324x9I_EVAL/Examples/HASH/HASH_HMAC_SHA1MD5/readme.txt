/**
  @page HASH_HMAC_SHA1MD5  HMAC digest calculation using HMAC SHA1 and HMAC MD5 example
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    HASH/HASH_HMAC_SHA1MD5/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the digest calculation using SHA1 and MD5 Example.
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

How to use the HASH peripheral to hash data with HMAC SHA-1 
and HMAC MD5 algorithms.

For this example, DMA is not used for data transfer, the CPU is using HASH peripheral in
polling mode.

The key used is long type (2088 bit = 261 bytes).
The HMAC SHA-1 message digest result is a 160 bit (20 bytes) data and the HMAC MD5 message 
digest result is a 128 bit (16 bytes) data.

The expected HASH digests (for SHA1 and MD5) are already computed using an online
HASH tool. Those values are compared to those computed by the HASH peripheral.
In case there is a mismatch the red LED is turned ON.
In case the SHA1 digest is computed correctly the green LED (LED1) is turned ON.
In case the MD5 digest is computed correctly the blue LED (LED4) is turned ON.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

System, Security, HASH, HMAC, SHA1, MD5, DMA, digest

@par Directory contents 

  - HASH/HASH_HMAC_SHA1MD5/Inc/stm32f4xx_hal_conf.h    HAL configuration file
  - HASH/HASH_HMAC_SHA1MD5/Inc/stm32f4xx_it.h          Interrupt handlers header file
  - HASH/HASH_HMAC_SHA1MD5/Inc/main.h                  Main program header file  
  - HASH/HASH_HMAC_SHA1MD5/Src/stm32f4xx_it.c          Interrupt handlers
  - HASH/HASH_HMAC_SHA1MD5/Src/main.c                  Main program
  - HASH/HASH_HMAC_SHA1MD5/Src/stm32f4xx_hal_msp.c     HAL MSP module
  - HASH/HASH_HMAC_SHA1MD5/Src/system_stm32f4xx.c      STM32F4xx system clock configuration file


@par Hardware and Software environment

  - This example runs on STM32F439xx devices.
  
  - This example has been tested with STMicroelectronics STM32439I-EVAL RevB 
    evaluation boards and can be easily tailored to any other supported device 
    and development board.     


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example   
   

 */
