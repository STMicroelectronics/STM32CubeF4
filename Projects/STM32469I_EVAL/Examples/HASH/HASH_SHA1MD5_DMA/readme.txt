/**
  @page HASH_SHA1MD5_DMA  HASH digest calculation using SHA1 and MD5 example
  with DMA transfer.
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    HASH/HASH_SHA1MD5_DMA/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the HASH digest calculation using SHA1 and MD5 example.
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

How to use the HASH peripheral to hash data using SHA-1 and MD5 algorithms 
when data are fed to the HASH unit with DMA.

For this example, DMA is used to transfer data from memory to the HASH processor.
The message to hash is a 2048 bit data.
The SHA-1 message digest result is a 160 bit data, and the MD5 message digest
result is a 128 bit data.
The expected HASH digests (for SHA1 and MD5) are already computed using an online
HASH tool. Those values are compared to those computed by the HASH peripheral.
In case there is a mismatch the red LED is turned ON.
In case the SHA1 digest is computed correctly the green LED (LED1) is turned ON.
In case the MD5 digest is computed correctly the blue LED (LED4) is turned ON.

@note The connection of the LCD reset pin to a dedicated GPIO PK7 instead of the STM32F469 NRST pin may cause residual display on LCD with applications/examples that do not require display.
	  The LCD clear can be ensured by hardware through the boards power off/power on or by software calling the BSP_LCD_Reset() function.

@par Keywords

System, Security, HASH, SHA1, MD5, digest, DMA

@par Directory contents 

  - HASH/HASH_SHA1MD5_DMA/Inc/stm32f4xx_hal_conf.h    HAL configuration file
  - HASH/HASH_SHA1MD5_DMA/Inc/stm32f4xx_it.h          Interrupt handlers header file
  - HASH/HASH_SHA1MD5_DMA/Inc/main.h                  Header for main.c module  
  - HASH/HASH_SHA1MD5_DMA/Src/stm32f4xx_it.c          Interrupt handlers
  - HASH/HASH_SHA1MD5_DMA/Src/main.c                  Main program
  - HASH/HASH_SHA1MD5_DMA/Src/stm32f4xx_hal_msp.c     HAL MSP module
  - HASH/HASH_SHA1MD5_DMA/Src/system_stm32f4xx.c      STM32F4xx system source file


@par Hardware and Software environment

  - This example runs on STM32F479xx devices.
  
  - This example has been tested and validated with STM32479I-EVAL RevC board and can be
    easily tailored to any other supported device and development board.

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example 



  */
